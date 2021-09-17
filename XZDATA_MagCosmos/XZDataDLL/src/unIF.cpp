//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------   
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#include "unAction.h"

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}


//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall INITXZDATA(byte *OrgStoreNO, byte *StoreNO, byte *EcrNO, byte *Version)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_INITXZDATA(OrgStoreNO,StoreNO,EcrNO,Version);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall WX(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_WX(StoreNO,EcrNO,SalesNo,SalFileName,Version);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall WZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int AutoZ)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_WZ(StoreNO,EcrNO,SalesNo,SalFileName,Version,AutoZ);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall WXReport(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_WXReport(StoreNO,EcrNO,SalesNo,SalFileName,Version);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall Checkin(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_Checkin(StoreNO,EcrNO,SalesNo,SalFileName,Version);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_WX_OK(StoreNO,EcrNO,SalesNo,SalFileName,Version);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall WZ_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, bool delFolder)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_WZ_OK(StoreNO,EcrNO,SalesNo,SalFileName,Version,delFolder);
        } __finally {
                delete frmAction;
        }

        return iResult;
}

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall WSPC(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int iQryType)
{
        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;
        try {
                iResult = frmAction->_WSPC(StoreNO,EcrNO,SalesNo,SalFileName,Version,iQryType);
        } __finally {
                delete frmAction;
        }

        return iResult;
}


extern "C" __declspec(dllexport) int __stdcall WVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{

        TfrmAction *frmAction = new TfrmAction(NULL);
        int iResult = 0;

        try {
                iResult = frmAction->_WVXZ(StoreNO,EcrNO,SalesNo,SalFileName,Version);
        } __finally {
                delete frmAction;
        }
        
        return iResult;
}



