//---------------------------------------------------------------------------

#ifndef IPAddrUnitH
#define IPAddrUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormIPaddr : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label2;
  TEdit *EditAddr1;
  TEdit *EditAddr2;
  TBevel *Bevel2;
  TEdit *EditAddr3;
  TLabel *Label11;
  TLabel *Label7;
  TButton *Button1;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall EditAddr1KeyPress(TObject *Sender, char &Key);
  void __fastcall EditAddr2KeyPress(TObject *Sender, char &Key);
  void __fastcall EditAddr3KeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
  __fastcall TFormIPaddr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormIPaddr *FormIPaddr;
//---------------------------------------------------------------------------
#endif
