//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainUnit1.cpp", Form1);
USEFORM("CommandsUnit1.cpp", FormCommands);
USEFORM("ErrorLogUnit.cpp", FormErrorLog);
USEFORM("TempPressLogUnit.cpp", FormTempPressLog);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
    Application->Initialize();
    Application->Title = "BaroCal8";
    Application->CreateForm(__classid(TForm1), &Form1);
    Application->CreateForm(__classid(TFormCommands), &FormCommands);
    Application->CreateForm(__classid(TFormErrorLog), &FormErrorLog);
    Application->CreateForm(__classid(TFormTempPressLog), &FormTempPressLog);
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
