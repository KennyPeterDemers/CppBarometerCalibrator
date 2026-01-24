//---------------------------------------------------------------------------

#ifndef ErrorLogUnitH
#define ErrorLogUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormErrorLog : public TForm
{
__published:	// IDE-managed Components
  TMemo *MemoErrorLog;
  TMainMenu *MainMenu1;
  TMenuItem *File1;
  TMenuItem *SaveAs1;
  TMenuItem *N1;
  TMenuItem *Exit1;
  TMenuItem *View1;
  TMenuItem *ClearErrorFlag1;
  TSaveDialog *SaveDialog1;
  void __fastcall SaveAs1Click(TObject *Sender);
  void __fastcall ClearErrorFlag1Click(TObject *Sender);
  void __fastcall Exit1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TFormErrorLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormErrorLog *FormErrorLog;
//---------------------------------------------------------------------------
#endif
