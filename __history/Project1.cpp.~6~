//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("TempPressLogUnit.cpp", FormTempPressLog);
USEFORM("CommandsUnit1.cpp", FormCommands);
USEFORM("ErrorLogUnit.cpp", FormErrorLog);
USEFORM("MainUnit1.cpp", Form1);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TFormErrorLog), &FormErrorLog);
		Application->CreateForm(__classid(TFormTempPressLog), &FormTempPressLog);
		Application->CreateForm(__classid(TFormCommands), &FormCommands);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
