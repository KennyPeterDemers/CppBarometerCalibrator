//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainUnit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------

void __fastcall TForm1::SocketRead(TObject *Sender, TCustomWinSocket *Socket) {
  char ch, *ptr, str[100];
  float NewPressure;
  AnsiString TimeStr;

	// for logging
	AnsiString addr;
	int port;
	char endPoint[128];

  int i = Socket->RemotePort - 2101;
  if(Socket->RemoteAddress == EditAddr2->Text) i += 16;
  else if(Socket->RemoteAddress == EditAddr3->Text) i += 32;
  else if(Socket->RemoteAddress == EditAddr4->Text) i += 48;

  NoActivityTime[i] = 0;

  // !ATTN! COMMENT THESE TWO LINES OUT
  //sprintf(str, "%s %i", Socket->RemoteAddress.c_str(), i);
  //MemoMonitor->Lines->Add(str);

  int PortMon = CSpinEditPortMon->Value - 1;
  int PortT1Chk = CSpinEditPortT1Chk->Value - 1;

	//sprintf(str,"%d", i);
  //StatusBar1->Panels->Items[1]->Text = str;

  while(Socket->ReceiveBuf(&ch, 1) != -1) {
		switch(ch) {
			case '>':
					break;
			case 13:
					RcvStr[i][RcvIndex[i]] = 0;
					LogRxTextLogged(Socket, &RcvStr[i][0]);

								// TEMPERATURE CONTROLLER
								/*if(i==14) {                     KPD NEED TO ADD BACK IN FOR NEW IP ADDRESS
									ptr = &RcvStr[i][0];
                  CurrentTemp = atof(ptr);
                  sprintf(str,"%0.1f °C", CurrentTemp);
                  LabelTemperature->Caption = str;
                }

                // PRESSURE CONTROLLER

								else if(i==15) {
									ptr = &RcvStr[i][0];
                  while(*ptr != ' ') ++ptr;
                  CurrentPressure = atof(ptr);
                  PressureReadingReady = true;
                  sprintf(str,"%.2f hPa", CurrentPressure);
                  LabelPressure->Caption = str;
								}*/

                // BAROMETERS
                //else {
                  if(i == PortT1Chk && TableChkFlag) {
										//sprintf(str,"%s", &RcvStr[i][0]);
                    //MemoMain->Lines->Add(str);
                    MemoMain->Lines->Add(&RcvStr[i][0]);
                    //if(strncmp(&RcvStr[i][0], "00000", 5) == 0) {
                    //  TableChkFlag = false;
                      //sprintf(str, "Line Count: %i", MemoT1Chk->Lines->Count);
                      //MemoT1Chk->Lines->Add(str);
                    //}
                  }

                  if((i == PortMon) && CheckBoxPortMonEnable->Checked) {
                    sprintf(str,"%02i: %s", i+1, &RcvStr[i][0]);
                    MemoMonitor->Lines->Add(str);
                  }

if (BaroReadFlag[i])
{
  // *** PATCH 1: guard BEFORE any ReadBaroArray[ReadBaroIndex] access ***
	if (ReadBaroIndex >= ReadBaroArrayMax)
  {
    StatusBar1->Panels->Items[2]->Text = "ReadBaroIndex at limit.";
		sprintf(str, "ReadBaroIndex is %d. Limit is %d.", ReadBaroIndex, ReadBaroArrayMax);
    LogError(str);

    // Keep behavior sane: we consumed the flag but can't store
    BaroReadFlag[i] = false;
    return;   // If you're inside a loop and 'return' is wrong, use: continue; or break;
  }

  // SET PORTNUM TO NONZERO TO VALIDATE PORT
  ReadBaroArray[ReadBaroIndex].PortNum = i + 1;
  ReadBaroArray[ReadBaroIndex].RefTemp = CurrentTemp;
  ReadBaroArray[ReadBaroIndex].RefPressure = CurrentPressure;

  if (ModeVal == RUN_MODE)
  {
    try
    {
      ReadBaroArray[ReadBaroIndex].Pressure = atof(&RcvStr[i][0]);

			if (ReadBaroIndex < ReadBaroArrayMax) ++ReadBaroIndex;
      else {
        StatusBar1->Panels->Items[2]->Text = "ReadBaroIndex at limit.";
        sprintf(str,"ReadBaroIndex is %d. Limit is %d.", ReadBaroIndex, ReadBaroArrayMax);
        LogError(str);
      }
    }
    catch (...)
    {
      ReadBaroArray[ReadBaroIndex].Pressure = 0.0;
      sprintf(str,"RUN MODE. Floating point conversion err on port %d (%0.1f %.2f", i+1, CurrentTemp, CurrentPressure);
			LogError(str);
		}
  }
  else
	{
    try
    {
			// Patch 2: prevent overflow (CaptureStr is char[20])
			strncpy(ReadBaroArray[ReadBaroIndex].CaptureStr, &RcvStr[i][0], sizeof(ReadBaroArray[ReadBaroIndex].CaptureStr) - 1);
			ReadBaroArray[ReadBaroIndex].CaptureStr[sizeof(ReadBaroArray[ReadBaroIndex].CaptureStr) - 1] = '\0';

			if (ReadBaroIndex < ReadBaroArrayMax) ++ReadBaroIndex;
			else {
        StatusBar1->Panels->Items[2]->Text = "ReadBaroIndex at limit.";
				sprintf(str,"ReadBaroIndex is %d. Limit is %d.", ReadBaroIndex, ReadBaroArrayMax); // (I removed the stray ] )
        LogError(str);
      }
		}
		catch (...)
    {
      sprintf(str,"CAPTURE MODE. Error reading string.(%0.1f %.2f", CurrentTemp, CurrentPressure);
      LogError(str);
    }
  }

  // --- everything below stays exactly like you had it ---
  AnsiString serial = EditSerialNum[i]->Text;   // converts UnicodeString -> AnsiString

  sprintf(str, "%02d %s %06.2f %03.1f %s",
    i+1,
    serial.c_str(),
    (float)CurrentPressure,
    (float)CurrentTemp,
    &RcvStr[i][0]
  );

  MemoBaroReadMon->Lines->Add(str);
  BaroReadFlag[i] = false;
}
							//}

                // Reset the RcvIndex
                RcvIndex[i] = 0;

      case 10:  break;

      default:  try {
                  RcvStr[i][RcvIndex[i]] = ch;
                  if(RcvIndex[i] < 998) ++RcvIndex[i];
                }
                catch (...) {
                  LogError("SOCKET READ overflow");
                }
    }
  }

  //StatusBar1->Panels->Items[1]->Text = "    ";

}

//---------------------------------------------------------------------------
