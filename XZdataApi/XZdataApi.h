// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the XZDATAAPI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// XZDATAAPI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#include <stdio.h>
#include <tchar.h>
#include "ULOG.h"
#include "UXZDataClass.h"


#ifdef XZDATAAPI_EXPORTS
#define XZDATAAPI_API __declspec(dllexport)
#else
#define XZDATAAPI_API __declspec(dllimport)
#endif

using namespace std;

// This class is exported from the XZdataApi.dll
class XZDATAAPI_API CXZdataApi {
public:
	CXZdataApi(void);
	~CXZdataApi(void);
	// TODO: add your methods here.


private:

};



//How to Build XZDataApi.dll for windows XP
//[Project] => [Properities] => [Configuration Properities] => [General] => [Platform ToolSet] ==>  "Visual Studio 2013 - Windows XP (v120_xp)"
//[Platform ToolSet] ==> (Default:)"Visual Studio 2013 (V120)
extern "C" XZDATAAPI_API int nXZdataApi;

extern "C" XZDATAAPI_API int __stdcall XZdataApi_INIT(void);
extern "C" XZDATAAPI_API int __stdcall INITXZDATA(byte *OrgStoreNO, byte *StoreNO, byte *EcrNO, byte *Version);
extern "C" XZDATAAPI_API int __stdcall WX(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
extern "C" XZDATAAPI_API int __stdcall WZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int AutoZ);
extern "C" XZDATAAPI_API int __stdcall WXReport(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
extern "C" XZDATAAPI_API int __stdcall Checkin(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
extern "C" XZDATAAPI_API int __stdcall WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);
extern "C" XZDATAAPI_API int __stdcall WZ_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, bool delFolder);
extern "C" XZDATAAPI_API int __stdcall WSPC(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int iQryType);
extern "C" XZDATAAPI_API int __stdcall WVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);

