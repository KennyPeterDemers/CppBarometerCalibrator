//---------------------------------------------------------------------------

#ifndef TempPressLogUnitH
#define TempPressLogUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TFormTempPressLog : public TForm
{
__published:	// IDE-managed Components
  TMemo *MemoTempPressLog;
  TMainMenu *MainMenu1;
  TMenuItem *File1;
  TMenuItem *View1;
  TMenuItem *Save1;
  TMenuItem *Clear1;
  TSaveDialog *SaveDialog1;
  TMenuItem *N1;
  TMenuItem *Exit1;
  TCSpinEdit *CSpinEditTempPressLogDelay;
  TLabel *Label1;
  TMenuItem *Enabled1;
  void __fastcall Clear1Click(TObject *Sender);
  void __fastcall Save1Click(TObject *Sender);
  void __fastcall Exit1Click(TObject *Sender);
  void __fastcall CSpinEditTempPressLogDelayChange(TObject *Sender);
  void __fastcall Enabled1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TFormTempPressLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTempPressLog *FormTempPressLog;
//---------------------------------------------------------------------------
#endif
