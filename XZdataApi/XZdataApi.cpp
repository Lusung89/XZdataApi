// XZdataApi.cpp : Defines the exported functions for the DLL application.
//


#include "stdafx.h"
#include "XZdataApi.h"
#include <iostream>
#include "Lcommon.h"


// This is an example of an exported variable
XZDATAAPI_API int nXZdataApi=0;

// This is an example of an exported function.
XZDATAAPI_API int __stdcall XZdataApi_INIT(void)
{
	
	return 42;
}


XZDATAAPI_API int __stdcall INITXZDATA(byte *OrgStoreNO, byte *StoreNO, byte *EcrNO, byte *Version)
{
	int int_result, i, j, z_cnt, zini_nzno_seq;
	string  stmp, dd, ddzz;  
	//TIniFile *MyIni;
	CIniReader MyIni;

	int_result = 0;

	_Strsprintf(stmp,"%s", XZDATADIR);

	if (!DirExists(stmp))  //if (!DirectoryExists(str_x_path))
	{
		//writelog("建立歷史目錄 :" + str_x_path);
		SHCreateDirectory(NULL, StringToWString(stmp).c_str());   //ForceDirectories(str_x_path);
	}

	if (!!DirExists("C:\\FTLPOS\\XZDATA\\WX\\"))
		SHCreateDirectory(NULL, L"C:\\FTLPOS\\XZDATA\\WX\\");   //ForceDirectories("C:\\FTLPOS\\XZDATA\\WX\\");

	//MyIni = new TIniFile(XZDATA_INI);
	MyIni.setINIFileName(XZDATA_INI);

	//[ECR}
	MyIni.WriteString("ECR", "OrgStoreNO", string((char*)OrgStoreNO));  //Jack Add _ByteArrayToAnsiString
	MyIni.WriteString("ECR", "StoreNO", string((char*)StoreNO));
	MyIni.WriteString("ECR", "EcrNO", string((char*)EcrNO));
	MyIni.WriteString("ECR", "Version", string((char*)Version));

	//[X] 區段
	stmp = MyIni.ReadString("X", "ptran_cnt", "00000000");
	MyIni.WriteString("X", "ptran_cnt", stmp);

	stmp = MyIni.ReadString("X", "sx_date", "");
	MyIni.WriteString("X", "sx_date", stmp);

	stmp = MyIni.ReadString("X", "ex_date", "");
	MyIni.WriteString("X", "ex_date", stmp);

	i = stoi(MyIni.ReadString("X", "pxno_seq", "0"));
	MyIni.WriteString("X", "pxno_seq", int2str(i));

	j = stoi(MyIni.ReadString("X", "Autoxno_seq", int2str(i)) );    //自動日結用
	MyIni.WriteString("X", "Autoxno_seq", int2str(j));

	stmp = MyIni.ReadString("X", "xfilename", "");
	MyIni.WriteString("X", "xfilename", stmp);

	stmp = MyIni.ReadString("X", "xrptname", "");
	MyIni.WriteString("X", "xrptname", stmp);

	stmp = MyIni.ReadString("X", "CHRRPTNAME", "");
	MyIni.WriteString("X", "CHRRPTNAME", stmp);

	//[Z] 區段
	stmp = MyIni.ReadString("Z", "pz_date", currentDateTime(1) ); //  FormatDateTime("yyyymmddhhnnss", Now()));
	MyIni.WriteString("Z", "pz_date", stmp);
	dd = stmp;

	i = stoi(MyIni.ReadString("Z", "pzno_seq", "0"));   // MyIni.ReadInteger("Z", "pzno_seq", 0);
	MyIni.WriteString("Z", "pzno_seq", int2str(i));

	i = stoi(MyIni.ReadString("Z", "nzno_seq", "1"));      //MyIni.ReadInteger("Z", "nzno_seq", 1);
	MyIni.WriteString("Z", "nzno_seq", int2str(i));
	zini_nzno_seq = i;

	stmp = MyIni.ReadString("Z", "pz_cnt", "00000");
	MyIni.WriteString("Z", "pz_cnt", stmp);

	stmp = MyIni.ReadString("Z", "nz_cnt", "00001");
	MyIni.WriteString("Z", "nz_cnt", stmp);
	z_cnt = stoi(stmp);

	//z_cnt = zini_nz_cnt;
	//String dd=zini_pz_date;
	stmp = MyIni.ReadString("Z", "datez", dd.substr(6, 2) + _IntToStr(zini_nzno_seq));
	MyIni.WriteString("Z", "datez", stmp);

	stmp = MyIni.ReadString("Z", "zfilename", "");
	MyIni.WriteString("Z", "zfilename", stmp);

	stmp = MyIni.ReadString("Z", "zrptname", "");
	MyIni.WriteString("Z", "zrptname", stmp);

	stmp = MyIni.ReadString("Z", "deptname", "");
	MyIni.WriteString("Z", "deptname", stmp);

	//strcpy(zini_inqtname,MyIni->ReadString("Z","inqtname","").c_str());
	i = stoi(MyIni.ReadString("Z", "am_tclsacc", "0"));   //MyIni->ReadInteger("Z", "am_tclsacc", 0);
	MyIni.WriteString("Z", "am_tclsacc", int2str(i));

	_Strsprintf(ddzz,"%2s%02d", dd.substr(6, 2), _IntToStr(zini_nzno_seq));
	stmp = MyIni.ReadString("Z", "yyyymmddz", dd.substr(0, 6) + ddzz);
	MyIni.WriteString("Z", "yyyymmddz", stmp);

	////////////////////////////////
	stmp = MyIni.ReadString("VXZ", "nzno_seq", "1");
	MyIni.WriteString("VXZ", "nzno_seq", stmp);

	stmp = MyIni.ReadString("VXZ", "PZ_DATE", "");
	MyIni.WriteString("VXZ", "PZ_DATE", stmp);

	stmp = MyIni.ReadString("VXZ", "CD_SATNCODE", "");
	MyIni.WriteString("VXZ", "CD_SATNCODE", stmp);

	stmp = MyIni.ReadString("VXZ", "CS_ENTRY", "");
	MyIni.WriteString("VXZ", "CS_ENTRY", stmp);

	stmp = MyIni.ReadString("AUTOZ", "LST_ZCNT_DTTM", currentDateTime(2)); // FormatDateTime("yyyymmdd", Now()));   //換日參考日
	MyIni.WriteString("AUTOZ", "LST_ZCNT_DTTM", stmp);     //換日參考日

	stmp = MyIni.ReadString("AUTOZ", "LST_AUTOZ_DTTM", currentDateTime(2)); // FormatDateTime("yyyymmdd", Now()));  //前次自動日結時間
	MyIni.WriteString("AUTOZ", "LST_AUTOZ_DTTM", stmp);     //前次自動日結時間

	stmp = MyIni.ReadString("AUTOZ", "NST_AUTOZ_DTTM", "");
	if (Trim(stmp) == "")
		MyIni.WriteString("AUTOZ", "NST_AUTOZ_DTTM", "20131015");  //自動日結時間啟用日期
	//if( Trim(stmp)="")
	//   tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM",dd.SubString(1,8));  //現再自動日結時間

	stmp = MyIni.ReadString("XREPORT", "XREPORTDATE", currentDateTime(2));   // FormatDateTime("yyyymmdd", Now()));
	MyIni.WriteString("XREPORT", "XREPORTDATE", stmp);
	stmp = MyIni.ReadString("XREPORT", "ZDTCnts", "0");
	MyIni.WriteString("XREPORT", "ZDTCnts", stmp);
	stmp = MyIni.ReadString("XREPORT", "SALCnts", "0");
	MyIni.WriteString("XREPORT", "SALCnts", stmp);

	//delete MyIni;
	return int_result;
}

//收銀員帳表
XZDATAAPI_API int __stdcall WX(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
	int int_result = 0;
	XDATA *xdata = new XDATA();
	int_result = xdata->WriteData(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName), string((char*)Version));
	delete xdata;

	return int_result;

}

//日結
XZDATAAPI_API int __stdcall WZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int AutoZ)
{
	int int_result = 0;
	string stmp;

	if (AutoZ == 1)
	{
		AUTOZDATA *autozdata = new AUTOZDATA();
		int_result = autozdata->WriteData(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName),string((char*)Version),	"");
		delete autozdata;

		// 2013/10/21 Update
		if (int_result == 0 || int_result == 2)
		{
			stmp = ZDATATMP_INI;
			if (!PathFileExists(StringToWString(stmp).c_str()))    //if (FileExists(ZDATATMP_INI))
			{
				DeleteFile(StringToWString(stmp).c_str());  //DeleteFile(ZDATATMP_INI);
				logsprintf("TfrmAction::_WZ Return=%d, 刪除檔案=%s ", int_result, ZDATATMP_INI);
				//writelog(stmp);

			}
		}

		if (int_result != 0)
			int_result = 1; // 日結 OK
	}
	else
	{
		ZDATA *zdata = new ZDATA();
		int_result = zdata->WriteData(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName), string((char*)Version),	"");
		delete zdata;
	}
	return int_result;

}

//讀帳
XZDATAAPI_API int __stdcall WXReport(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
	XREPORT *xreport = new XREPORT();
	xreport->WriteData(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName), string((char*)Version));
	delete xreport; 

	return 0;
}


//擔當同交班
XZDATAAPI_API int __stdcall Checkin(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
	CHECKIN *checkin = new CHECKIN();
	checkin->WriteData(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName), string((char*)Version));
	delete checkin;

	return 0;

}


XZDATAAPI_API int __stdcall WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{

	return(wx_ok(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo),
		string((char*)SalFileName), string((char*)Version) ));

	
}


//---------------------------------------------------------------------------
XZDATAAPI_API int __stdcall WZ_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, bool delFolder)
{

	return(wz_ok(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo),
		         string((char*)SalFileName), string((char*)Version)  ));

	
}



//---------------------------重點商品列管帳表---------------------------------------
XZDATAAPI_API int __stdcall WSPC(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int iQryType)
{
	int iRtn;

	SPCDATA *AllSpcData = new SPCDATA();
	iRtn = AllSpcData->WriteSpcInqData(string((char*)SalFileName), "V01.9999", iQryType);
	delete AllSpcData; 

	// iRtn > 0 : SUCCEED
	return iRtn;

}


XZDATAAPI_API int __stdcall WVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
	int int_result = 0;
	VXZDATA *vxzdata = new VXZDATA();
	int_result = vxzdata->WriteData(string((char*)StoreNO),	string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName), string((char*)Version));
	delete vxzdata; 

	return int_result;  // int_result > 0 : SUCCEED
}


// This is the constructor of a class that has been exported.
// see XZdataApi.h for the class definition
CXZdataApi::CXZdataApi()
{
	return;
}
