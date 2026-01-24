//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IPAddrUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormIPaddr *FormIPaddr;
//---------------------------------------------------------------------------
__fastcall TFormIPaddr::TFormIPaddr(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormIPaddr::Button1Click(TObject *Sender)
{
  ModalResult = mrOk;  
}
//---------------------------------------------------------------------------
void __fastcall TFormIPaddr::EditAddr1KeyPress(TObject *Sender, char &Key)
{
  if(EditAddr1->ReadOnly) MessageBeep(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormIPaddr::EditAddr2KeyPress(TObject *Sender, char &Key)
{
  if(EditAddr2->ReadOnly) MessageBeep(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormIPaddr::EditAddr3KeyPress(TObject *Sender, char &Key)
{
  if(EditAddr3->ReadOnly) MessageBeep(0);
}
//---------------------------------------------------------------------------
