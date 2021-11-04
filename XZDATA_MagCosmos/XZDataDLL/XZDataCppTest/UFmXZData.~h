//---------------------------------------------------------------------------

#ifndef UFmXZDataH
#define UFmXZDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "..\SRC\UXZDataClass.h"
#include <utilcls.h>
#include <io.h>
#include <fcntl.h>


//---------------------------------------------------------------------------
class TFmMain : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TButton *Button1;
        TEdit *EdiSalFile;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TButton *Button7;
        TLabel *Label1;
        TEdit *Edit1;
        TLabel *Label2;
        TButton *Button8;
        TButton *Button9;
        TButton *Button10;
        TLabel *Label3;
        TEdit *EdAutoZDttm;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *Edi_ST;
        TEdit *Edi_ED;
        TEdit *Edit2;
        TLabel *Label6;
        TRadioButton *RadioButton1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
         __fastcall TFmMain(TComponent* Owner);
         void __fastcall TFmMain::mLog(AnsiString sLog);
         int __fastcall TFmMain::loadsalfile(AnsiString FileName, bool for_x, AnsiString &First1099, AnsiString &Last1099);
         int __fastcall TFmMain::Clear_Tran_cnt_INI(int TranCnt);
         AnsiString __fastcall TFmMain::File2String(AnsiString fn, AnsiString path);
         int __fastcall TFmMain::_WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
         AnsiString __fastcall TFmMain::GetUniqueSal(AnsiString sSalFileName);

         XDATA *xdata;
         ZDATA *zdata;
         XREPORT *xreport;
         CHECKIN *checkin;
         SPCDATA *AllSpcData;
         VXZDATA *vxzdata;
         AUTOZDATA *autozdata;

};
//---------------------------------------------------------------------------
extern PACKAGE TFmMain *FmMain;
//---------------------------------------------------------------------------
#endif
