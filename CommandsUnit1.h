//---------------------------------------------------------------------------

#ifndef CommandsUnit1H
#define CommandsUnit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormCommands : public TForm
{
__published:	// IDE-managed Components
  TMemo *Memo1;
private:	// User declarations
public:		// User declarations
  __fastcall TFormCommands(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCommands *FormCommands;
//---------------------------------------------------------------------------
#endif
