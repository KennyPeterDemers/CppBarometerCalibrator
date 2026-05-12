// Logger.h
#pragma once
#include <vector>
#include <string>
#include <stdint.h>

enum class LogType : uint8_t
{
    Tx,
    Rx,
    Note
};

struct LogEntry
{
    uint64_t seq;
    std::string timeOfDay;   // HH:MM:SS.mmm, matches Wireshark Time of Day
	std::string endpoint;
	LogType logType;
	int bytes;
    std::string hex;
	std::string ascii;
    std::string asciiNote;   // e.g. "0:ESC 5:CR 6:LF"

};


class Logger
{
public:
    static void Init(size_t maxEntries = 5000, size_t maxBytesToDump = 256);

    static void Log(bool isTx, const char* endpoint, const void* data, int bytes);
	static void Log(LogType kind, const char* text);

	static const std::vector<LogEntry>& Entries();
	static std::vector<LogEntry> Snapshot();
	static void GetTimeOfDay9(char* buf, size_t bufSize);

    // NEW: realtime file logging
    static bool StartRealtimeFile(const wchar_t* path, bool durableFlush = true);
    static void StopRealtimeFile();

private:
	static void EnsureInit();
};

