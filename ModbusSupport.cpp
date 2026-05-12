#include <IdTCPClient.hpp>
#include <IdGlobal.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include "ModbusSupport.h"
#include "Logger.h"

// Forward declaration

static void WriteU16BE(TBytes &b, int ofs, unsigned short v)
{
		b[ofs+0] = (Byte)((v >> 8) & 0xFF);
    b[ofs+1] = (Byte)(v & 0xFF);
}

static unsigned short ReadU16BE(const TBytes &b, int ofs)
{
    return (unsigned short)((b[ofs] << 8) | b[ofs+1]);
}

static void ReadExact(TIdTCPClient* c, TBytes &outBuf, int len)
{
    outBuf.Length = len;
    int got = 0;

    while (got < len)
    {
        TBytes chunk;
        // Read exactly the remaining number of bytes into 'chunk'
        c->IOHandler->ReadBytes(chunk, len - got, false);

        if (chunk.Length <= 0)
            throw Exception(L"Socket closed while reading Modbus response.");

        // Copy chunk into outBuf at offset 'got'
        for (int i = 0; i < chunk.Length; ++i)
            outBuf[got + i] = chunk[i];

        got += chunk.Length;
    }
}

static bool ReadModbusFrame(TIdTCPClient* c, TBytes &frame)
{
	char str[100];

	// 1) Read MBAP header (7 bytes) INCLUDING UnitId
	TBytes mbap;
	ReadExact(c, mbap, 7);

	// MBAP[4..5] = Length (big-endian), counts UnitId + PDU
	unsigned short len = (unsigned short)((mbap[4] << 8) | mbap[5]);

	if (len > 260) {
		sprintf(str, "Invalid Modbus Header read - too many %d bytes", len);
		Logger::Log(LogType::Note, str);
		return false;
	}
	if (len < 2) {
		sprintf(str, "Invalid Modbus Header read - Only %d bytes", len);
		Logger::Log(LogType::Note, str);
		return false;
	}
	// 2) Read ONLY the PDU bytes (len - 1)
	int pduLen = (int)len - 1;

	TBytes pdu;
	ReadExact(c, pdu, pduLen);

	// 3) Combine: MBAP(7) + PDU
	frame.Length = 7 + pduLen;

	for (int i = 0; i < 7; ++i)
		frame[i] = mbap[i];

	for (int i = 0; i < pduLen; ++i)
		frame[7 + i] = pdu[i];

	// ---- HEX LOGGING ----
	AnsiString hexLine;

	for (int i = 0; i < frame.Length; ++i)
	{
			hexLine += IntToHex((int)frame[i], 2);
			if (i < frame.Length - 1)
					hexLine += " ";
	}

	AnsiString msg = "RX Modbus (" + IntToStr(frame.Length) + " bytes): " + hexLine;

	Logger::Log(LogType::Note, msg.c_str());
  return true;
}

static bool Modbus_ValidateMbapBasic(const TBytes& resp, unsigned short expectedTx, Byte expectedUnit)
{
    if (resp.Length < 8) return false; // MBAP(7) + FC(1)
    if (ReadU16BE(resp, 0) != expectedTx) return false;
    if (ReadU16BE(resp, 2) != 0) return false;          // ProtocolId must be 0
    if (resp[6] != expectedUnit) return false;
    return true;
}

static bool Modbus_CheckException(const TBytes& resp, const char* context)
{
    if (resp.Length < 8) {
				Logger::Log(LogType::Note, (AnsiString(context) + ": response too short").c_str());
        return false;
    }
    Byte fc = resp[7];
    if ((fc & 0x80) == 0) return true; // not an exception

    if (resp.Length < 9) {
				Logger::Log(LogType::Note, (AnsiString(context) + ": exception response too short").c_str());
        return false;
    }

    char str[120];
    sprintf(str, "%s: Modbus exception FC=0x%X ExCode=0x%X", context, fc, resp[8]);
    Logger::Log(LogType::Note, str);
    return false;
}

bool Modbus_Connect(TIdTCPClient* client, const AnsiString& host, int port, int ct_ms, int rt_ms)
{
    if (!client) return false;
    try {
        client->Host = host;
        client->Port = port;
        client->ConnectTimeout = ct_ms;
				client->ReadTimeout = rt_ms;
        client->Connect();
        return client->Connected();
    }
		catch (const Exception& e) {
				AnsiString msg = AnsiString("Modbus_Connect: ") + AnsiString(e.Message);
				Logger::Log(LogType::Note, msg.c_str());
				return false;
		}
		catch (...) {
        Logger::Log(LogType::Note, "Modbus_Connect: exception");
        return false;
    }
}

bool Modbus_WriteSingleRegister(
    TIdTCPClient* client,
    unsigned short& txId,
    Byte unitId,
    unsigned short regAddr,
    unsigned short value)
{
    if (!client || !client->Connected()) return false;

    const unsigned short thisTx = txId++;
    TBytes req;
    req.Length = 12;

    WriteU16BE(req, 0, thisTx);
    WriteU16BE(req, 2, 0);
    WriteU16BE(req, 4, 6);
    req[6] = unitId;
    req[7] = 0x06;
    WriteU16BE(req, 8, regAddr);
    WriteU16BE(req,10, value);

    try {
        client->IOHandler->Write(req);

        TBytes resp;
        if (!ReadModbusFrame(client, resp)) {
            Logger::Log(LogType::Note, "WriteSingleRegister: no response");
            return false;
        }
        if (!Modbus_ValidateMbapBasic(resp, thisTx, unitId)) {
            Logger::Log(LogType::Note, "WriteSingleRegister: MBAP mismatch");
            return false;
        }
        if (!Modbus_CheckException(resp, "WriteSingleRegister")) return false;

        if (resp[7] != 0x06) {
            char s[100];
            sprintf(s, "WriteSingleRegister: unexpected FC=0x%X", resp[7]);
            Logger::Log(LogType::Note, s);
            return false;
        }
        return true;
    }
    catch (...) {
        Logger::Log(LogType::Note, "WriteSingleRegister: exception");
        return false;
    }
}

bool Modbus_ReadHoldingRegisters(
    TIdTCPClient* client,
    unsigned short& txId,
    Byte unitId,
    unsigned short startAddr,
    unsigned short qty,
    TBytes& outResp)
{
		outResp.Length = 0;

    if (!client || !client->Connected()) return false;
		if (qty == 0) return false;
		if (qty > 125) return false;   // FC03 limit

		const unsigned short thisTx = txId++;
    TBytes req;
    req.Length = 12;

    WriteU16BE(req, 0, thisTx);
    WriteU16BE(req, 2, 0);
    WriteU16BE(req, 4, 6);
    req[6] = unitId;
    req[7] = 0x03;
    WriteU16BE(req, 8, startAddr);
    WriteU16BE(req,10, qty);

    try {
        client->IOHandler->Write(req);

        TBytes resp;
        if (!ReadModbusFrame(client, resp)) {
            Logger::Log(LogType::Note, "ReadHoldingRegisters: no response");
            return false;
        }
        if (!Modbus_ValidateMbapBasic(resp, thisTx, unitId)) {
            Logger::Log(LogType::Note, "ReadHoldingRegisters: MBAP mismatch");
            return false;
        }
        if (!Modbus_CheckException(resp, "ReadHoldingRegisters")) return false;

        // Normal: MBAP(7) + FC(1) + BC(1) + data(2*qty)
        const int need = 9 + (2 * qty);
        if (resp.Length < need) {
            char s[120];
            sprintf(s, "ReadHoldingRegisters: response too short need=%d got=%d", need, (int)resp.Length);
            Logger::Log(LogType::Note, s);
            return false;
        }
        if (resp[7] != 0x03) {
            char s[80];
            sprintf(s, "ReadHoldingRegisters: unexpected FC=0x%X", resp[7]);
            Logger::Log(LogType::Note, s);
            return false;
        }
        if (resp[8] != (Byte)(2 * qty)) {
            char s[120];
            sprintf(s, "ReadHoldingRegisters: bad bytecount bc=%u expected=%u", resp[8], (unsigned)(2 * qty));
            Logger::Log(LogType::Note, s);
            return false;
        }

        outResp = resp;
        return true;
    }
    catch (...) {
        Logger::Log(LogType::Note, "ReadHoldingRegisters: exception");
        return false;
    }
}

// ---- intent helpers ----

bool F4T_TerminateProfile(TIdTCPClient* client, unsigned short& txId, Byte unitId)
{
    const unsigned short REG_ProfileActionRequest_16566 = 16566;
    const unsigned short VAL_Terminate_148 = 148;

    Logger::Log(LogType::Note, "F4T: Terminate profile (16566=148)");
    return Modbus_WriteSingleRegister(client, txId, unitId, REG_ProfileActionRequest_16566, VAL_Terminate_148);
}

bool F4T_ReadLoop1Regs272to274(
    TIdTCPClient* client,
    unsigned short& txId,
    Byte unitId,
    unsigned short& out272,
    unsigned short& out273,
    unsigned short& out274)
{
    Logger::Log(LogType::Note, "F4T: Read regs 272-274");
    TBytes resp;
    if (!Modbus_ReadHoldingRegisters(client, txId, unitId, 272, 3, resp)) return false;

    out272 = ReadU16BE(resp, 9);
    out273 = ReadU16BE(resp, 11);
    out274 = ReadU16BE(resp, 13);

    AnsiString msg = "Read regs 272-274: " + IntToStr((int)out272) + ", " + IntToStr((int)out273) + ", " + IntToStr((int)out274);
		Logger::Log(LogType::Note, msg.c_str());
    return true;
}

bool F4T_SetLoop1ControlModeOff(TIdTCPClient* client, unsigned short& txId, Byte unitId, unsigned short loop1ControlModePduAddr) {

		const unsigned short VAL_ModeOff_62 = 62; // Off

		AnsiString msg = "F4T: Set Loop1 Control Mode Off addr=" + IntToStr((int)loop1ControlModePduAddr) + " val=62";
		Logger::Log(LogType::Note, msg.c_str());
		// Write Single Register (FC=06)
		return Modbus_WriteSingleRegister(client, txId, unitId, loop1ControlModePduAddr, VAL_ModeOff_62);
}

bool F4T_SetLoop1ControlModeAuto(TIdTCPClient* client, unsigned short& txId, Byte unitId, unsigned short loop1ControlModePduAddr) {

		const unsigned short VAL_ModeAuto_10 = 10;  // Auto

		AnsiString msg = "F4T: Set Loop1 Control Mode Auto addr=" + IntToStr((int)loop1ControlModePduAddr) + " val=10";
		Logger::Log(LogType::Note, msg.c_str());
		// Write Single Register (FC=06)
		return Modbus_WriteSingleRegister(client, txId, unitId, loop1ControlModePduAddr, VAL_ModeAuto_10);
}

bool F4T_ReadProfileState(TIdTCPClient* client, unsigned short& txId, Byte unitId, unsigned short& outState) {

		const unsigned short REG_ProfileState_16568 = 16568;

    Logger::Log(LogType::Note, "F4T: Read Profile State (16568)");
    TBytes resp;
    if (!Modbus_ReadHoldingRegisters(client, txId, unitId, REG_ProfileState_16568, 1, resp)) return false;

    outState = ReadU16BE(resp, 9);
    AnsiString msg = "ProfileState(16568)=" + IntToStr((int)outState);
	Logger::Log(LogType::Note, msg.c_str());
    return true;
}
