//---------------------------------------------------------------------------

#ifndef unActionH
#define unActionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmAction : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
        __fastcall TfrmAction(TComponent* Owner);
        int __fastcall _INITXZDATA(byte *OrgStoreNO, byte *StoreNO, byte *EcrNO, byte *Version);
        int __fastcall _WX(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
        int __fastcall _WZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int AutoZ);
        int __fastcall _WXReport(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
        int __fastcall _Checkin(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
        int __fastcall _WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
        int __fastcall _WZ_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, bool delFolder);
        int __fastcall _WSPC(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int iQryType);
        int __fastcall _WVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);


        int __fastcall _SaveZCntToFile( String InPutSalFile ,int Z_Cnt , String OutPutSalFile );


};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAction *frmAction;
//---------------------------------------------------------------------------
#endif
