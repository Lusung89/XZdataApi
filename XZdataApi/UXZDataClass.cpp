//---------------------------------------------------------------------------

//#include <vcl.h>
#include "stdafx.h"
#include "UXZDataClass.h"
#include "Lcommon.h"

//---------------------------------------------------------------------------
//#pragma hdrstop
//#pragma package(smart_init)

/// <summary> _FileLayout Version 
/// Open Unicode (UTF8) file  
/// Ver : NextPOS_LAYOUT_20210311.xls 
/// </summary>
/// <param name="StoreNO"></param>
/// <param name = "EcrNO">< / param>
/// <param name="SalesNo"></param>
/// <param name="SalFileName"></param>  
/// <param name="Version"></param> 
/// <returns> int</returns>

int __stdcall wx_ok(string StoreNO, string EcrNO, string SalesNo, string SalFileName, string Version)
{
	string str_ecr_ini, str_tmp_ini, stmp;

	str_ecr_ini = XZDATA_INI;
	str_tmp_ini = XDATATMP_INI;

	logsprintf("wx_ok : (%s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str());

	//TIniFile *tif_ecr;
	//TIniFile *tif_tmp;
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 

	string str_xdt, str_xrp, str_97, str_crp,
		str_tran_cnt,
		str_ptran_cnt,
		str_pxno_seq,
		str_sx_date,
		str_ex_date,
		str_tot_opne_x,
		str_x_pay_cnt,
		str_sys_date;

	// 2013/10/21 Update

	if (!PathFileExists(StringToWString(str_tmp_ini).c_str()))    //if (!FileExists(str_tmp_ini))
	{
		logsprintf("wx_ok :(%s) 不存在...Return 1.", str_tmp_ini.c_str());
		return 1;
	}

	try
	{
		tif_ecr.setINIFileName(XZDATA_INI);
		tif_tmp.setINIFileName(XDATATMP_INI);

		//READ X INI
		str_97 = tif_tmp.ReadString("X", "x0097filename", " ");
		str_xdt = tif_tmp.ReadString("X", "XFILENAME", " ");
		str_xrp = tif_tmp.ReadString("X", "XRPTNAME", " ");
		str_crp = tif_tmp.ReadString("X", "CHRRPTNAME", " ");
		str_sys_date = tif_tmp.ReadString("X", "XFILEDTTM", "");

		str_tran_cnt = tif_tmp.ReadString("ECR", "TRAN_CNT", " ");
		str_ptran_cnt = tif_tmp.ReadString("X", "PTRAN_CNT", " ");
		str_pxno_seq = tif_tmp.ReadString("X", "PXNO_SEQ", " ");
		str_sx_date = tif_tmp.ReadString("X", "SX_DATE", " ");
		str_ex_date = tif_tmp.ReadString("X", "EX_DATE", " ");
		//str_tot_opne_x= tif_tmp->ReadString("SUM","tot_open_x", " ");
		//str_x_pay_cnt = tif_tmp->ReadString("PAY","x_pay_cnt" , "0");

		//READ END
		//WRITE X INI
		tif_ecr.WriteString("X", "x0097filename", str_97);
		tif_ecr.WriteString("X", "XFILENAME", str_xdt);
		tif_ecr.WriteString("X", "XRPTNAME", str_xrp);
		tif_ecr.WriteString("X", "CHRRPTNAME", str_crp);
		tif_ecr.WriteString("X", "XFILEDTTM", str_sys_date);
		tif_ecr.WriteString("X", "SALESLOGINDTTM", "");


		tif_ecr.WriteString("ECR", "TRAN_CNT", str_tran_cnt);
		tif_ecr.WriteString("X", "PTRAN_CNT", str_ptran_cnt);
		tif_ecr.WriteString("X", "PXNO_SEQ", str_pxno_seq);
		tif_ecr.WriteString("X", "SX_DATE", str_sx_date);
		tif_ecr.WriteString("X", "EX_DATE", str_ex_date);
		tif_ecr.WriteString("ECR", "tot_open_x", "0");
		tif_ecr.WriteString("PAY", "x_pay_cnt", "0");

		//WRITE END
	}
	catch (...)
	{
		;;//delete tif_ecr, tif_tmp;
	}

	/*DeleteFile(StringToWString(str_tmp_ini).c_str() );
	writelog("刪除檔案 " + str_tmp_ini);*/

	if (!DirExists("C:\\FTLPOS\\XZDATA\\WX\\"))  //if (!DirectoryExists(str_x_path))
		SHCreateDirectory(NULL, L"C:\\FTLPOS\\XZDATA\\WX\\");   //ForceDirectories(str_x_path);

	DeleteFile(StringToWString(str_tmp_ini).c_str());
	writelog("wx_ok 完成: 刪除檔案 " + str_tmp_ini);

	return 1;


}


int __stdcall wz_ok(string StoreNO, string EcrNO, string SalesNo, string SalFileName, string Version)
{
	
	string str_ecr_ini, str_tmp_ini, str_zini_datez, sInvoFile, sInvoFilePath, stmp, sTmpInvoFile, sInvoTransFilePath, str_folder_path;
	string str_sal_file, str_log_file, str_ecr_file, str_mwlog_file, str_zsl_file, str_dbtxt_file, str_dbtxt2_file, str_802log_file;
	int OutNZCnt = 0;

	str_ecr_ini = XZDATA_INI;
	str_tmp_ini = ZDATATMP_INI;  //".\\ZDT_TMP.INI";


	logsprintf("wz_ok : (%s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str());
	//writelog(stmp);

	// 2013/10/21 Update

	if (!PathFileExists(StringToWString(str_tmp_ini).c_str()))    //if (!FileExists(str_tmp_ini))
	{
		logsprintf("wz_ok :(%s) 不存在...Return 1.", str_tmp_ini.c_str());
		return 1;
	}

	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 

	string str_zdt, str_zrp, str_dpt, str_98, str_pz_date, str_am_tclsacc, str_ipt, str_zsl, str_tran_cnt, ZfileTranCnt;
	try
	{
		tif_ecr.setINIFileName(str_ecr_ini); // = new TIniFile(XZDATA_INI);
		tif_tmp.setINIFileName(str_tmp_ini); // = new TIniFile(ZDATATMP_INI);

		//READ Z INI
		//str_zini_datez=tif_tmp->ReadString("Z", "DATEZ", "");
		str_tran_cnt = tif_tmp.ReadString("ECR", "TRAN_CNT", " ");
		str_zdt = tif_tmp.ReadString("Z", "ZFILENAME", " ");
		str_zrp = tif_tmp.ReadString("Z", "ZRPTNAME", " ");
		str_dpt = tif_tmp.ReadString("Z", "DEPTNAME", " ");
		str_ipt = tif_tmp.ReadString("Z", "INQTNAME", " ");
		str_zsl = tif_tmp.ReadString("Z", "ZSLNAME", " ");
		str_98 = tif_tmp.ReadString("Z", "x0098filename", " ");

		ZfileTranCnt = tif_tmp.ReadString("Z", "ZfileTranCnt", "");  // ZDT 交易序號
		str_pz_date = tif_tmp.ReadString("Z", "PZ_DATE", " ");
		str_am_tclsacc = tif_tmp.ReadString("Z", "AM_TCLSACC", " ");
		//READ END

		//WRITE Z INI
		tif_ecr.WriteString("Z", "ZFILENAME", str_zdt);
		tif_ecr.WriteString("Z", "ZRPTNAME", str_zrp);
		tif_ecr.WriteString("Z", "DEPTNAME", str_dpt);
		tif_ecr.WriteString("Z", "INQTNAME", str_ipt);
		tif_ecr.WriteString("Z", "ZSLNAME", str_zsl);
		tif_ecr.WriteString("Z", "x0098filename", str_98);

		stmp = tif_tmp.ReadString("AUTOZ", "LST_AUTOZ_DTTM", "");   //前次自動日結時間
		tif_ecr.WriteString("AUTOZ", "LST_AUTOZ_DTTM", stmp);   //前次自動日結時間

		stmp = tif_tmp.ReadString("AUTOZ", "LST_ZCNT_DTTM", "");   //換日參考日
		tif_ecr.WriteString("AUTOZ", "LST_ZCNT_DTTM", stmp);     //換日參考日

		tif_ecr.WriteString("Z", "PZ_DATE", str_pz_date);
		tif_ecr.WriteString("Z", "AM_TCLSACC", str_am_tclsacc);

		tif_ecr.WriteString("ECR", "TRAN_CNT", str_tran_cnt);
		tif_ecr.WriteString("ECR", "tot_open_x", "0");
		tif_ecr.WriteString("ECR", "tot_open_z", "0");
		tif_ecr.WriteString("PAY", "x_pay_cnt", "0");
		tif_ecr.WriteString("PAY", "z_pay_cnt", "0");
		tif_ecr.WriteString("Z", "ZfileTranCnt", ZfileTranCnt);    // ZDT 交易序號

		writelog("■ WZ_OK:Clear tot_open_x, tot_open_z  ■");

		//巡迴驗收
		tif_ecr.WriteString("SPC", "pSPCno_seq", "0");
		tif_ecr.WriteString("SPC", "SPC0080filename", "");
		tif_ecr.WriteString("VXZ", "PZNO_SEQ", "0");
		tif_ecr.WriteString("VXZ", "NZNO_SEQ", "1");
		tif_ecr.WriteString("VXZ", "PZ_DATE", "");
		tif_ecr.WriteString("VXZ", "CD_SATNCODE", "");
		tif_ecr.WriteString("VXZ", "CS_ENTRY", "");

		OutNZCnt = stoi(tif_ecr.ReadString("Z", "nz_cnt", "0"));

		string report_year_month_dd = tif_ecr.ReadString("Backup", "PZ_DATE", ""); // 前次結帳系統年月日  前次日結
		string report_year_month = report_year_month_dd.substr(0, 6);              // 前次結帳系統年月  前次日結
		string today_year_month = currentDateTime(2).substr(0, 6);                //	Date().FormatString("yyyymm"); // 系統年月

		int iMainTainHistory = 0;
		iMainTainHistory = stoi(tif_ecr.ReadString("Backup", "MAINTAINHISTORY", "1"));
		tif_ecr.WriteString("Backup", "MAINTAINHISTORY", int2str(iMainTainHistory));
		str_zini_datez = tif_ecr.ReadString("Backup", "DATEZ", "");    //備份 sal*.TXT條件  日結 ddz
		int report_day = _StrToInt(tif_ecr.ReadString("Backup", "PZ_DATE", "").substr(6, 2)); //前次結帳系統日  前次日結

		string syyyymmdd = currentDateTime(2, 1);			   //系統日+1 FormatDateTime("yyyymmdd", Now() + 1);
		int today_day = _StrToInt(syyyymmdd.substr(6, 2));     //系統日+1


		sInvoTransFilePath = "C:\\FTLPOS\\RCP\\TRANS\\";
		sInvoFilePath = "C:\\FTLPOS\\RCP\\";
		sTmpInvoFile = _ExtractFile(str_zdt);                   //ExtractFileName(str_zdt);
		sInvoFile = sInvoFilePath + _ChangeFileExt(sTmpInvoFile, string(".VOL"));

		if (iMainTainHistory && Trim(str_zini_datez) != "") //delFolder)
		{
			// 刪除本次日結與系統日間的目錄
			MtnDirHistory(report_year_month_dd, syyyymmdd, str_zini_datez);
		
		}    // if (iMainTainHistory) //delFolder)


		//備份相關檔案
		str_folder_path = _ExtractFilePath(str_zrp);

		if (!DirExists(str_folder_path) && Trim(str_folder_path) != "")
		{
			writelog("wz_ok:建立目錄 " + str_folder_path);
			SHCreateDirectory(NULL, StringToWString(str_folder_path).c_str());  // ForceDirectories(str_folder_path);
		}

		//所有備份檔新路徑
		str_zsl_file = _ChangeFileExt(str_zdt, string(".ZSL"));

		str_sal_file = str_folder_path + _ExtractFile(SalFileName);
		_Strsprintf(str_log_file, "%s%s", XZDATADIR, "XZDATA.LOG");   //SalFileName.SubString(1, SalFileName.Pos(".")) + "log";
		str_ecr_file = str_folder_path + _ExtractFile(XZDATA_INI);    // "XZDATA.INI";

		str_mwlog_file = SalFileName.substr(0, SalFileName.find(".")) + "mw";

		writelog("wz_ok: 備份LOG檔 ------- " + str_log_file + " TO " + str_folder_path + _ExtractFile(str_log_file));
		stmp = str_folder_path + _ExtractFile(str_log_file);
		DeleteFile(StringToWString(stmp).c_str());

		stmp = str_folder_path + _ExtractFile(str_log_file);
		//if (RenameFile(str_log_file, str_folder_path + ExtractFileName(str_log_file)))
		if (MoveFile(StringToWString(str_log_file).c_str(), StringToWString(stmp).c_str()))
		{
			writelog("wz_ok: 備份LOG檔 完成" + str_log_file + " TO " + str_folder_path + _ExtractFile(str_log_file));
		}
		else
		{
			writelog("wz_ok: 備份LOG檔 失敗" + str_log_file + " TO " + str_folder_path + _ExtractFile(str_log_file));
		}


		string InputFile, OutputFile;
		//_Strsprintf(str_log_file, "C:\\FTLPOS\\XZDATA\\SAL\\%s.sal", EcrNO);
		_Strsprintf(str_log_file, "C:\\FTLPOS\\XZDATA\\SAL\\%s.sal", EcrNO.c_str());
		OutputFile = str_log_file;

		writelog("wz_ok: 備份SAL檔 ------- " + str_log_file + " TO " + str_folder_path + _ExtractFile(str_log_file));
		stmp = str_folder_path + _ExtractFile(str_log_file);
		DeleteFile(StringToWString(stmp).c_str());

		stmp = str_folder_path + _ExtractFile(str_log_file);
		if (MoveFile(StringToWString(str_log_file).c_str(), StringToWString(stmp).c_str()))
		{
			writelog("wz_ok: 備份SAL檔 完成" + str_log_file + " TO " + str_folder_path + _ExtractFile(str_log_file));
			InputFile == str_folder_path + _ExtractFile(str_log_file);
		}
		else
		{
			writelog("wz_ok: 備份SAL檔 失敗" + str_log_file + " TO " + str_folder_path + _ExtractFile(str_log_file));
		}

		if (DeleteFile(StringToWString(str_tmp_ini).c_str()))
		{
			writelog("刪除檔案 : " + str_tmp_ini);
		}
	}
	catch (...)
	{
		//delete tif_ecr, tif_tmp;
		stmp = XZDATA_INI;
		if (CopyFile(StringToWString(stmp).c_str(), StringToWString(str_ecr_file).c_str(), false))
			writelog("備份XZDATA_INI檔 " + str_ecr_file);
		else
			writelog("備份XZDATA_INI檔失敗 " + str_ecr_file);

		str_ecr_file = str_folder_path + "Application.INI";
		if (CopyFile(L"C:\\FTLPOS\\INI\\Application.INI", StringToWString(str_ecr_file).c_str(), false))
			writelog("備份Application.INI檔 " + str_ecr_file);
		else
			writelog("備份Application.INI檔失敗 " + str_ecr_file);
	}

	string sRtnSaleDataPath;
	sRtnSaleDataPath = "C:\\FTLPOS\\MMK\\Telegram\\";
	_MtnDirFile_Ex(sRtnSaleDataPath, 30, 0);

	sRtnSaleDataPath = "C:\\FTLPOS\\MMK\\RCV\\";
	_MtnDirFile_Ex(sRtnSaleDataPath, 30, 250);

	sRtnSaleDataPath = "C:\\FTLPOS\\MMK\\SEND\\";
	_MtnDirFile_Ex(sRtnSaleDataPath, 30, 250);

	_MtnDirFile_Ex(sInvoFilePath, 90, 0);
	sRtnSaleDataPath = "C:\\FTLPOS\\RCP\\RcpUsed\\";
	_MtnDirFile_Ex(sRtnSaleDataPath, 90, 100);

	_MtnDirFile_Ex(sInvoTransFilePath, 7, 0);
	_MtnDirFile_Ex(XZDATASAL, 60, 0);

	DeleteFile(StringToWString(str_tmp_ini).c_str());
	writelog("wz_ok 完成 : 刪除檔案 " + str_tmp_ini);

	return 1;

}



int __stdcall MtnDirHistory(string RptYYYYMMDD, string SysYYYYMMDD, string RptDatez)
{
	int report_day, today_day;
	string stmp, del_dir;

	// 刪除本次日結與系統日間的目錄
	writelog("■ MtnDirHistory: 歷史檔案維護功能開始 ■"); //2003/02/07
	
	report_day = _StrToInt(RptYYYYMMDD.substr(6, 2));
	today_day = _StrToInt(SysYYYYMMDD.substr(6, 2));

	writelog("前次日結YYYYMM：" + RptYYYYMMDD);
	writelog("系統年月：" + SysYYYYMMDD);
	writelog("前次日結 Backup->PZ_DATE..DD：" + _IntToStr(report_day));
	writelog("系統日+1：" + _IntToStr(today_day));

	try
	{
		if (Trim(RptYYYYMMDD) != "" && RptYYYYMMDD.substr(0, 6) <= SysYYYYMMDD.substr(0, 6) )
		{
			if (RptYYYYMMDD.substr(0, 6) == SysYYYYMMDD.substr(0, 6) ) //帳表年月及系統年月相同
			{
				if (report_day >= today_day)          // 帳表DD>=系統DD
				{
					//;; //do nothing ...
					//string del_dir; // = "C:\\FTLPOS\\History\\" + _Strsprintf(stmp, "%02d", today_day);
					_Strsprintf(del_dir, "C:\\FTLPOS\\History\\%02d", today_day);
					_Strsprintf(stmp, "%02d", today_day);
					if (stmp == RptDatez.substr(0, 2))
					{
						logsprintf("MtnDirHistory 歷史檔案維護 001 : history(%s) == RptDatez(%s) => 不執行刪除 %s", stmp.c_str(), RptDatez.substr(0, 2).c_str(), del_dir.c_str());
					}
					else
					{
						_DelTree(del_dir);
					}

				}
				else
				{
					for (int i = report_day + 1; i <= today_day; i++)
					{
						string del_dir; // = "C:\\FTLPOS\\History\\" + FormatFloat("00", i);
						_Strsprintf(del_dir, "C:\\FTLPOS\\History\\%02d", i);
						_Strsprintf(stmp, "%02d", i);
						if (stmp == RptDatez.substr(0, 2))
						{
							logsprintf("MtnDirHistory 歷史檔案維護 002 : history(%s) == RptDatez(%s) => 不執行刪除 %s", stmp.c_str(), RptDatez.substr(0, 2).c_str(), del_dir.c_str());
						}
						else
						{
							_DelTree(del_dir);
						}  //2003/03/27

					}

				}
			}
			else  //帳表年月及系統年月不同
			{
				for (int i = report_day + 1; i <= 31; i++)  //1. 刪 (帳表日+1 -> 31)
				{
					//string del_dir; // = "C:\\FTLPOS\\History\\" + FormatFloat("00", i);
					_Strsprintf(del_dir, "C:\\FTLPOS\\History\\%02d", i);
					_Strsprintf(stmp, "%02d", i);
					if (stmp == RptDatez.substr(0, 2))
					{
						logsprintf("MtnDirHistory 歷史檔案維護 003 : history(%s) == RptDatez(%s) => 不執行刪除 %s", stmp.c_str(), RptDatez.substr(0, 2).c_str(), del_dir.c_str());
					}
					else
					{
						_DelTree(del_dir);
					}

				}

				for (int i = 1; i <= today_day; i++)      //2. 刪 (1 -> 系統日)
				{
					//string del_dir; // = "C:\\FTLPOS\\History\\" + FormatFloat("00", i);
					_Strsprintf(del_dir, "C:\\FTLPOS\\History\\%02d", i);
					_Strsprintf(stmp, "%02d", i);
					if (stmp == RptDatez.substr(0, 2))
					{
						logsprintf("MtnDirHistory 歷史檔案維護 004 : history(%s) == RptDatez(%s) => 不執行刪除 %s", stmp.c_str(), RptDatez.substr(0, 2).c_str(), del_dir.c_str());
					}
					else
					{
						_DelTree(del_dir);
					}

				}
			}

			writelog("MtnDirHistory:歷史檔案維護功能結束"); //2003/02/07
		}   // end of if (Trim(report_year_month)!="" && report_year_month<=today_year_month)
		else
		{
			writelog("MtnDirHistory:帳表年月資料空白或大於系統日,放棄執行歷史檔案維護功能");
		}

	} // end of    try
	catch (...)
	{
		writelog("MtnDirHistory: 歷史檔案維護功能 Catch error");

	}

	return 1;
}




// REPORT *report;     報表Class  手動交班
/// <summary>  XDATA::GetXDTData
/// REPORT *report;     報表Class
/// </summary>
/// <param name=""></param>
/// <returns>int</returns>
int __fastcall XDATA::GetXDTData(string StrZCnt, string StoreNO, string EcrNO, string SalesNo, string XDTsysDttm, string StrNxnoSeq,
						  string Version, string Z_date, string DDZ_1099, string ZNT_1099, string sIdx,
						  string &RtnXDTData, string &RtnXRPFile, string &RtnCRPFile)
{

    string sTmp, sSalesLoginDttm, str_x_first, ed_date_time, str_nz_cnt, str_YYYYMMDDZ, str_ver_num, SaleDataVer,
               str_date_z,str_sal_last_1099, str_tran_cnt, str_sum_tran, AutoXFg ;

    x->no_tm = g_ecr_no;                      //收銀機機號
    x->no_casher = g_cashier;                 //收銀員號碼
    str_nz_cnt=StrZCnt;
    str_ver_num=Version;
    str_date_z=Z_date;                            //DDZ

    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本
    //Charles update 20160225 for 2016030100 - 1028 追加銀聯卡&Smart Pay欄位

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

    logsprintf("★ XDATA::GetXDTData 計算班表 GetXDTData:StrZCnt=%s, StoreNO=%s, EcrNO=%s, SalesNo=%s, StrNxnoSeq=%s, Version=%s, Z_date=%s, DDZ_1099=%s, ZNT_1099=%s, RtnXRPFile=%s, RtnXRPFile=%s ",
                StrZCnt.c_str(), StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), StrNxnoSeq.c_str(),
                Version.c_str(), Z_date.c_str(), DDZ_1099.c_str(), ZNT_1099.c_str(), RtnXRPFile.c_str(), RtnCRPFile );

    if (x->no_casher.length() < 5)
        x->no_casher = "00001";

	sSalesLoginDttm = tif_ecr.ReadString("X", "SALESLOGINDTTM", "");     //tif_ecr->ReadString("X", "SALESLOGINDTTM", "");

	sTmp = tif_ecr.ReadString("ECR", "TRAN_CNT", "00001");  //tif_ecr->ReadString("ECR", "TRAN_CNT", "00001");      //目前交易序號,但SAL最後一筆須減1
	_Strsprintf(str_tran_cnt,"%05d", _StrToInt(sTmp) - 1>0 ? _StrToInt(sTmp) - 1 : 1);

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入交易序號");
	_Strsprintf(str_tran_cnt,"%05d", int_sum_transcnt);

    if ((int_sum_transcnt + 1) > 99999)    //此次收銀員帳交易序號
    {
        int_sum_transcnt = 0;
    }

	_Strsprintf(str_sum_tran,"%05d", (int_sum_transcnt + 1));


    //X帳序號  01~99
    x->no_seq = StrNxnoSeq;
    str_x_first = gsalX_first; // gpchar_x_first;    //X帳第一筆Sal(不代表為SAL第一筆)

    //x->dt_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(1,8); //前次結帳日期
    //x->tm_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(9,6); //前次結帳時間

    //sIdx::  01:第一筆, 10:最後一筆,  11:只有一筆

	ed_date_time = currentDateTime(1);  //  FormatDateTime("yyyymmddhhnnss", Now());
    x->dt_begin = _StringSegment_EX(ZNT_1099, SGM, 5).substr(0,8);
    x->tm_begin = _StringSegment_EX(ZNT_1099, SGM, 5).substr(8,6);

    AutoXFg="1";   //0:當作是自動日結之交班

    if( Trim(sIdx)=="01" )         //01:第一筆
       {
         if( Trim(sSalesLoginDttm)=="")
           sSalesLoginDttm=_StringSegment_EX(ZNT_1099, SGM, 5);
         x->dt_begin=sSalesLoginDttm.substr(0,8);       //前次結帳日期
         x->tm_begin=sSalesLoginDttm.substr(8,6);       //前次結帳時間
         x->dt_end = x->dt_begin;
         x->tm_end = "235959";
         AutoXFg="0";  //0:當作是自動日結之交班

       }
    else if( Trim(sIdx)=="10" )  //10:最後一筆
       {
		   // 2018/08/20 make By Lu
		   // x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(0,8);
		   // x->tm_begin = "000001";
		    
		       x->dt_begin = sSalesLoginDttm.substr(0, 8);        
		       x->tm_begin = sSalesLoginDttm.substr(8, 6);        
                
			   x->dt_end = _StringSegment_EX(ZNT_1099, SGM, 5).substr(0, 8);   // x->dt_begin;  2018/08/20 make By Lu
               x->tm_end = ed_date_time.substr(8,6);
               AutoXFg="1";  //0:當作是自動日結之交班

       }
    else if( Trim(sIdx)=="11" )  //11:只有一筆
       {
		   if (Trim(sSalesLoginDttm) == "")
              sSalesLoginDttm=_StringSegment_EX(ZNT_1099, SGM, 5);

         // 預防換日後,馬上日結  
         if( ed_date_time.substr(0,8) >  _StringSegment_EX(ZNT_1099, SGM, 5).substr(0,8) )
           {
               logsprintf("★ XDATA::計算班表 只有一筆, (ed_date_time:%s) > (1099_Dttm:%s) 取 1099_Dttm 為交班之日期.",
				   ed_date_time.c_str(), _StringSegment_EX(ZNT_1099, SGM, 5).substr(0, 8).c_str());
               
               x->dt_begin=sSalesLoginDttm.substr(0,8);       //前次結帳日期
               x->tm_begin=sSalesLoginDttm.substr(8,6);       //前次結帳時間
               x->dt_end = _StringSegment_EX(ZNT_1099, SGM, 5).substr(0,8);    // x->dt_begin;
               x->tm_end = "235959";
               AutoXFg="1";  //0:當作是自動日結之交班
           }
         else
           {
             x->dt_begin=sSalesLoginDttm.substr(0,8);
             x->tm_begin=sSalesLoginDttm.substr(8,6);
             x->dt_end = ed_date_time.substr(0,8);
             x->tm_end = ed_date_time.substr(8,6);
             AutoXFg="1"; //0:當作是自動日結之交班
           }
       }
    else
       {

		   // 2018/08/20 make By Lu
		   //x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).substr(0, 8);     //前次結帳日期
		   //x->tm_begin = "000001";
		   x->dt_begin = sSalesLoginDttm.substr(0, 8);
		   x->tm_begin = sSalesLoginDttm.substr(8, 6);
		   x->dt_end = _StringSegment_EX(ZNT_1099, SGM, 5).substr(0, 8);   // x->dt_begin;  2018/08/20 make By Lu  
           x->tm_end = "235959";
           AutoXFg="0";  //0:當作是自動日結之交班
       }


    string str_invo_char;                    //發票英文字母
	str_invo_char = tif_ecr.ReadString("ECR", "INVO_CHAR", "XX");  //tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.length() < 2)
        str_invo_char = "XX";

    if (gbl_tot_haveline)
    {
        x->no_rcstart = invoice_start_end(gi_tot_line, true);    //發票開始號碼
        x->no_rcend = invoice_start_end(gi_tot_line, false);     //發票結束號碼
    }
    else    //無交易發票
    {
        x->no_rcend   = "0000000000";              //發票開始號碼
        x->no_rcstart = "0000000000";              //發票結束號碼
    }

    //本次結帳日期
    //ed_date_time = FormatDateTime("yyyymmddhhnnss",Now());      //?????????
    //x->dt_end = ed_date_time.SubString(1,8);
    //x->tm_end = ed_date_time.SubString(9,6);

    //無開始時間、與結束時間相同
    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000") || Trim(x->dt_begin) == "")
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }

    str_YYYYMMDDZ=ed_date_time.substr(0,8)+Trim(StrNxnoSeq);

    string x_a;
   	char buffer[10240];

	sprintf_s(buffer, sizeof(buffer),"3095||%04d||%-6s||%-2s||%14s||%05d||%-10s||%2s||%-20s||%8s||%6s||%8s||%6s||", 102,
                                        g_tencode.c_str(),
                                        x->no_tm.c_str(),         //收銀機機號
										(x->dt_end + x->tm_end).c_str(),  //ed_date_time.c_str(),   XDTsysDttm.c_str(),
                                        stoi(str_nz_cnt),
										str_YYYYMMDDZ.substr(2, 8).c_str(),
                                        x->no_seq.c_str(),        //交班序號
                                        x->no_casher.c_str(),     //收銀員號碼
                                        x->dt_begin.c_str(),      //前次結帳日期,
                                        x->tm_begin.c_str(),      //前次結帳時間
                                        x->dt_end.c_str(),        //本次結帳日期
                                        x->tm_end.c_str()         //本次結帳日期
                                     );
	x_a = string(buffer);
    int iC0TotCount = 0;    //點卡手續費來客數
    int iSumQt_trans = 0;  //本日交易品項

    //本日來客數 20140101
	_Strsprintf(x->tb_trans.qt_tcust, "%08d", SumQt_tcust(0, gi_tot_line)); // x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //本日來客數
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項


    //本日交易品項 20140101
    //Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);    //NextPOS
    _Strsprintf(x->tb_trans.qt_trans, "%08d", iSumQt_trans - giI);  // SumQt_trans(0, gi_tot_line) - giI);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt=0;
    
     // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt, R4_C01TaxAmt,R4_C01FTaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01TaxAmt=R4_C01FTaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  2014/0315
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  NextPOS
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;


    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line);
    TotMiss(0, gi_tot_line);    //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
    _Strsprintf(x->tb_trans.am_trcpntx,"%010d", giI-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );        //發票日結帳額(免)
	_Strsprintf(x->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));           //發票日結帳額(稅)
	_Strsprintf(x->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        //發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    //TotMiss(0,gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(x->tb_trans.qt_miss,"%08d", giA);        //誤打發票次數
	_Strsprintf(x->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //誤打發票金額(免)
	_Strsprintf(x->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //誤打發票金額(稅)
	_Strsprintf(x->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        //誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //不開發票金額, 不開發票金額誤退重
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtNTX + PreSal10_AmtTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

	_Strsprintf(x->tb_trans.am_totsale,"%010d", int_totsale);  

    string x_b;

    x_b  = (x->tb_trans.qt_tcust+ SGM);                      //本日來客數  14
    x_b += (x->tb_trans.qt_trans+ SGM);                      //本日交易品項 15
    x_b += (x->no_rcstart+ SGM);                              //發票開始號碼16
    x_b += (x->no_rcend+ SGM);                                //發票結束號碼17
    x_b += (x->tb_trans.am_trcpntx+ SGM);                    //發票日結帳額(免)18
    x_b += (x->tb_trans.am_trcptax+ SGM);                    //發票日結帳額(稅)19
    x_b += (x->tb_trans.am_trcptot+ SGM);                    //發票日結帳金額20
    //x_b += (x->tb_trans.qt_miss+ SGM);                       //誤打發票次數21   2021/03/18 取消 
    x_b += (x->tb_trans.am_missntx+ SGM);                    //誤打發票金額(免)21
    x_b += (x->tb_trans.am_misstax+ SGM);                    //誤打發票金額(稅)22
    x_b += (x->tb_trans.am_miss+ SGM);                       //誤打發票金額23
    x_b += (x->tb_trans.am_totsale+ SGM);                    //本日實際銷貨收入24
    
    //代售付TABLE===============================================================

    string x_c;
    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                          //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;         //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;         //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE;    //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);   //giH giI 食安退貨金額、次數       //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(x->tb_newadd.qt_cetel,"%08d", giD);          //代收次數
	_Strsprintf(x->tb_newadd.am_cetel,"%010d", giC);          //代收金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA, iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA = giH;  iSub_C0InvoAmtC = giJ;  iSub_C0InvoQtyC = giK;


    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //x_c += (x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty)+"|");
            //x_c += (x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt)+"|");

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //x_c += (x->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty)+"|");
        //x_c += (x->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt)+"|");
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;
    // NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    //Sub_C0InvoAmt4VDC(0, gi_rec_line); 
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //代售合計次數                    代售商品合計
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;   //2014/03/15
    //代售合計金額   無須加入預售商品 代售商品合計
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);   //2014/03/15

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;
    // 代售合計次數 25 ~ 代付合計金額 28
	_Strsprintf(x_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //代現金TABLE===============================================================

    int iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    SumTbCash(0,gi_tot_line);   //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣  負項表示
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//消費券
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

	_Strsprintf(x->tb_cash.qt_bonus,"%08d", giA); //-giT);       //禮券回數
	_Strsprintf(x->tb_cash.am_bonus,"%010d", giB); //-iCSMAmt);            //禮券金額
    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

	_Strsprintf(x->tb_cash.qt_vndcpn,"%08d", 0);             //廠商折價券回數 目前不使用
	_Strsprintf(x->tb_cash.am_vndcpn,"%010d", 0);             //廠商折價券金額

    _Strsprintf(x->tb_cash.qt_stocpn,"%08d", giC);           //折價券回數  門市代現金回數
	_Strsprintf(x->tb_cash.am_stocpn,"%010d", giD); //+iPreSalCPNDisAmt);           //折價券金額  門市代現金金額
    SumCpn41 +=  giD;                                    //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

	_Strsprintf(x->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //代現金合計回數
	_Strsprintf(x->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

	_Strsprintf(x->tb_cash.qt_bonout,"%08d", giG);           //溢收合計回數
	_Strsprintf(x->tb_cash.am_bonout,"%010d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //giE:餘額券金額 / giF:餘額券次數
    iqt_ffu1=giI;                                        //現金卡回數  電子錢次數  3275
    iam_ffu1=giJ;                                        //現金卡金額  電子錢金額  刷卡合計 3261

    string x_d;

    x_d += (x->tb_cash.qt_bonus+SGM);                 //禮券回數 29
    x_d += (x->tb_cash.am_bonus+ SGM);                //禮券金額 30
    x_d += (x->tb_cash.qt_vndcpn+ SGM);               //廠商折價券回數 31
    x_d += (x->tb_cash.am_vndcpn+ SGM);               //廠商折價券金額 32
    x_d += (x->tb_cash.qt_stocpn+ SGM);               //折價券回數 33
    x_d += (x->tb_cash.am_stocpn+ SGM);               //折價券金額 34
    x_d += (x->tb_cash.qt_totcash+ SGM);              //代現金合計回數 35
    x_d += (x->tb_cash.am_totcash+ SGM);              //代現金合計金額 36

	_Strsprintf(sTmp,"%08d", giF);
    x_d += (sTmp+ SGM);                         //餘額合計回數 37
	_Strsprintf(sTmp,"%010d", giE);
    x_d += (sTmp+ SGM);                         //餘額合計金額 38

    x_d += (x->tb_cash.qt_bonout+ SGM);               //溢收合計回數 39
    x_d += (x->tb_cash.am_bonout+ SGM);               //溢收合計金額 40

    //==========================================================================

    //現金合計 含 消費券-餘額券
    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(x->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);

    string x_e;

    x_e += (x->am_cash+ SGM);               //現金合計  41
    
    //收銀員明細FIELD===========================================================

    string x_f;

    //借零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tb_vendfld[i].am_vend,"%08d", 0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

		_Strsprintf(x->tb_casher.tb_vendfld[i].tm_vend,"%06d", 0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

	_Strsprintf(x->tb_casher.qt_vendtot,"%08d", 0);
	_Strsprintf(x->tb_casher.am_vendtot,"%010d", 0);


    //x_f += (x->tb_casher.qt_vendtot+"|");       
	_Strsprintf(sTmp,"%010d", VisuilCpnAmt);
    x_f += (sTmp+ SGM);      //VisuilCpnAmt :  無紙本折價金額   42

    //x_f += (x->tb_casher.am_vendtot+"|");      //借零合計金額
	_Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    x_f += (sTmp+ SGM);                           // 代收商品折讓單折扣金額 43

    string x_g;

    //投零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tg_feedfld[i].am_feed,"%08d", 0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

		_Strsprintf(x->tb_casher.tg_feedfld[i].tm_feed,"%06d", 0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

	_Strsprintf(x->tb_casher.qt_feedtot,"%08d", 0);
	_Strsprintf(x->tb_casher.am_feedtot,"%010d", 0);

    //x_g += (x->tb_casher.qt_feedtot+ SGM);      //投零次數         2021/03/18 取消 
    //x_g += (x->tb_casher.am_feedtot+ SGM);      //投零合計金額     2021/03/18 取消 

    string x_h;

    //投庫
    int int_drop_line = (gtsl_drop->size() - 1);   //投庫次數

    logsprintf("產生收銀員交班明細表:投庫筆數(%d)",gtsl_drop->size());
   
    //初始化全部填零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tg_dropfld[i].am_drop,"%010d", 0);    //投庫金額
		_Strsprintf(x->tb_casher.tg_dropfld[i].tm_drop,"%-6s", " ");    //執行時間
    }

    int int_drops = 0;                            //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                         //投庫總額
    int x_rtn_cnt=0;

    string str_drop_time, s;

    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
		s = *ls; // gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
             logsprintf("產生收銀員交班明細表:非投庫(%s)資料",s.c_str());
              continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, SGM, 11) );     //現金
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );     //提貨單 FU_SPACE10
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );     //提貨單 FU_SPACE10
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 12) );    //禮卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 13) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );    //提貨單 FU_SPACE10
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );    //提貨單 FU_SPACE10

		//中獎發票金額  500
		int_drop_money += _StrToInt(_StringSegment_EX(s, SGM, 16));
        //中獎發票金額  1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 27) );
        //中獎發票金額  200
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 15) );

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 18) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 20) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 22) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 24) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 26) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 28) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 30) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 32) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 34) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 36) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 38) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 40) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 42) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 44) );

        //每次投庫金額
		_Strsprintf(x->tb_casher.tg_dropfld[int_drops].am_drop,"%010d", int_drop_money);

        int_drop_sum += int_drop_money;                         //投庫總額

        str_drop_time = _StringSegment_EX(s, SGM, 5).substr(8,6);  //投庫時間

        //每次投庫時間
		_Strsprintf(x->tb_casher.tg_dropfld[int_drops].tm_drop,"%-6s", str_drop_time.c_str());

        int_drop_money = 0;
        ++int_drops;                             //投庫次數
        //sTmp.printf("產生收銀員交班明細表:投庫:int_drop_money=%d,int_drops=%d",
        //           int_drop_money,int_drops);
        //writelog(sTmp );

    }

    
    string x_i;
	_Strsprintf(x->tb_casher.qt_droptot, "%08d", int_drops);
	x_i += (x->tb_casher.qt_droptot + SGM);        //投庫次數  44
	
	_Strsprintf(x->tb_casher.am_droptot, "%010d", int_drop_sum);
	x_i +=(x->tb_casher.am_droptot + SGM);        //投庫合計金額  45

    SumUpdat(0,gi_rec_line);
	_Strsprintf(x->tb_casher.qt_update, "%08d", giA);
    x_i += (x->tb_casher.qt_update + SGM);               //更正次數 46
	_Strsprintf(x->tb_casher.am_update, "%010d", giB);
    x_i += (x->tb_casher.am_update + SGM);               //更正合計金額 47

    SumVoid(0,gi_tot_line);
	_Strsprintf(x->tb_casher.qt_void, "%08d", giA);
    x_i += (x->tb_casher.qt_void+ SGM); 	            //交易取消次數 48
	_Strsprintf(x->tb_casher.am_void, "%010d", giB);
    x_i += (x->tb_casher.am_void+ SGM);                 //交易取消合計金額 49

    SumInpdp(0,gi_rec_line);                                       //部門登錄
	x_i += (_Strsprintf(x->tb_newadd.qt_inpdp,"%08d", giA) + SGM);       //手動輸入部門次數、部門13、14  50
	x_i += (_Strsprintf(x->tb_newadd.am_inpdp,"%010d", giB) + SGM);       //手動輸入部門金額            51 

	x_i += (_Strsprintf(x->tb_casher.qt_openbox,"%08d", gi_open_x) + SGM);            //開錢櫃次數  52

    //小計折扣  1011
    // SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	x_i += (_Strsprintf(x->tb_casher.qt_dise,"%08d", giC) + SGM);                 //折扣次數  53
	x_i += (_Strsprintf(x->tb_casher.am_dise,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + SGM);    //折扣金額 不含折讓單 : iPreSalINVODisAmt 54

    //pay_sub_amt(0,gi_pay_line);
	x_i += (_Strsprintf(x->tb_newadd.qt_ffu6,"%08d", giA) + SGM);         //促銷折扣/讓次數 55
	x_i += (_Strsprintf(x->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + SGM);         //促銷折扣/讓金額  56


    easy_card(0,gi_easy_line);
	x_i += (_Strsprintf(x->tb_casher.qt_disret1,"%08d", giA) + SGM);                   //加值機開售卡次數 57
	x_i += (_Strsprintf(x->tb_casher.am_disret1,"%010d", giB) + SGM);                  //加值機開售卡金額 58
	x_i += (_Strsprintf(x->tb_casher.qt_disret2,"%08d", giC) + SGM);                   //加值機加值次數   59
	x_i += (_Strsprintf(x->tb_casher.am_disret2,"%010d", giD) + SGM);                  //加值機加值金額   60
    
    //追加TABLE=================================================================

    string x_j;

    //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    x_j += (_Strsprintf(x->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + SGM);     // 代售,手續費銷售金額(免)  61 
	x_j += (_Strsprintf(x->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + SGM);  // 代售,手續費作廢金額(免)  62

    //食安次數、金額  int iqt_food,iamt_food;
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu9,"%08d", iqt_food) + SGM);    //食安次數 63
	x_j += (_Strsprintf(x->tb_newadd.am_ffu9,"%010d", iamt_food) + SGM);  //食安金額 64
        
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu1,"%08d", iqt_ffu1) + SGM);         //刷卡次數 65
	x_j += (_Strsprintf(x->tb_newadd.am_ffu1,"%010d", iam_ffu1) + SGM);        //刷卡金額 66
       
    //發票金額0次數
	x_j += (_Strsprintf(x->tb_newadd.qt_parking,"%08d", 0) + SGM);          //代收停車費次數 67
	x_j += (_Strsprintf(x->tb_newadd.am_parking,"%010d", 0) + SGM);         //代收停車費金額 68
    x_j += (x->tb_newadd.qt_cetel+ SGM);                              //代收次數 69
    x_j += (x->tb_newadd.am_cetel+ SGM);                              //代收金額 70

    x_j += (_Strsprintf(x->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + SGM);    //版本前置符號  71
	x_j += (_Strsprintf(x->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + SGM);     //版本號碼     72
	x_j += (_Strsprintf(x->tb_newadd.qt_rcp0,"%08d", TotZero(0, gi_tot_line)) + SGM);             //發票金額0次數 73

    //不開發票金額
	x_j += (_Strsprintf(x->tb_newadd.am_ffu4,"%08d", int_novoice_amt) + SGM);         //不開發票金額 74
	x_j += (_Strsprintf(x->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + SGM);   //不開發票誤退重合計 75

    SumLOSS(0,gi_rec_line);
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu5,"%08d", giA) + SGM);          //TM廢棄Loss次數 76
	x_j += (_Strsprintf(x->tb_newadd.am_ffu5,"%010d", giB) + SGM);         //TM廢棄Loss金額 77

    x_j += (_Strsprintf(x->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + SGM);   //手續費合計 78


    SumRelDimAgn(0, gi_rec_line); //即時購代售 79
    int iSumRelDimAgn=giB;

    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數  即時購代售點卡
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu7,"%08d", iSumRelDimAgn + iC0InvoAmt4VDCN) + SGM);   //原加值機加值取消次數  即時購代售 79

	x_j += (_Strsprintf(x->tb_newadd.am_ffu2,"%010d", iCSMAmt) + SGM);      //消費券金額 80


    //pay_sub_amt(0,gi_pay_line);
    //x_j += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //促銷折扣/讓次數
    //x_j += (x->tb_newadd.am_ffu6.sprintf("%08d", giB)+"|");         //促銷折扣/讓金額


    //fetc_card(0,gi_fetc_line);
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",giFetcCashAddCnt)+"|");  //備用八次數  現金 加值次數
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",giFetcCashAddamt)+"|");  //備用八金額  現金 加值金額
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額
    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //備用十次數  FETC 結帳次數  保留
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"J"); //giFetcSumAmt);     //備用十金額  FETC 結帳金額  保留

    // 2021/3/17  Lu Update 取消投庫金額,時間  NextPOS_LAYOUT_20210311.xls  
    //for (int i=0; i<60; i++)
    //        {
    //          x_h += x->tb_casher.tg_dropfld[i].am_drop+ SGM;   //("00000000|");  //投庫金額
    //          x_h += x->tb_casher.tg_dropfld[i].tm_drop+ SGM;   //("      |");    //投庫時間
    //        }

    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);  即時購(應稅) 即時購與銷轉進值應稅 81
    x_h +=sTmp;   //"00000000|";
	_Strsprintf(sTmp,"%010d||", giD);     // 即時購(免稅) 即時購與銷轉進值免稅 82
    x_h +=sTmp;


    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

	_Strsprintf(sTmp,"%08d||", giA / 200); //發票兌獎張數 83
    x_h +=sTmp;
	_Strsprintf(sTmp,"%010d||", giA);      //發票兌獎金額 84
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for 收銀員交班明細表
    //SumCash41+=giA;                // 2012/10/08 for 收銀員交班明細表

     //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);  //電子禮券回數  85
    x_h +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //電子禮券金額 86
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD, 作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
	_Strsprintf(sTmp,"%010d||%010d||%08d", 
        iC0TotAmt, iC0GivUpAmt, iC0TotCount);   // 代售手續費應稅金額 87,代售手續費應稅作廢金額 88,代售手續費來客數 89
    x_h +=sTmp;

    string str_xdt_path, str_xrp_path;   //X資料、帳表存放路徑

    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);


    //string str_x_path;
    //DDZ_1099=str_date_z;

    if( stoi(DDZ_1099.substr(0,2)) < 1 || stoi(DDZ_1099.substr(0,2)) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, SGM, 7).substr(2,3) ;


    //AutoXFg: 0:當作是自動日結之交班
    //X報表
    string xdt_1095_data;
        
    sprintf_s(buffer, sizeof(buffer),"%s%s%s%s%s%s%s%s%s||%s||%10s||%13s||1||2||3||4||5||6||7||8||9||0||\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),      //x_g.c_str(),      
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          " ",              //CD_DAYWEA
                          "Tran_Cnt",
                           AutoXFg.c_str() //0:當作是自動日結之交班
                          );                      
                          
	xdt_1095_data = string(buffer);

	logsprintf("XDATA::GetXDTData 產生收銀員交班表 xdt_1095_data:(%s) Len=%d", xdt_1095_data.c_str(), xdt_1095_data.length());

    RtnXDTData=xdt_1095_data;

    //收銀員帳表  *.XRP    String &RtnXRPData, String &RtnCRPData
	strcpy_s(report->char_filename, sizeof(report->char_filename), RtnXRPFile.c_str()); //  ChangeFileExt(str_x_filename,".xrp").c_str());
    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  產生收銀員交班表  *.XRP                ***************/
	logsprintf("產生收銀員交班表:(%s)", report->char_filename);
 
	DeleteFile(StringToWString(RtnXRPFile).c_str());
    report->CreateReport("x", xdt_1095_data, StrBillPay, g_tencode, g_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0, "" );

    //str_new_name = str_x_path;
    //str_new_name +=   ExtractFileName(report->char_filename) ;

    //DeleteFile(str_new_name);

    //if (RenameFile(report->char_filename,str_new_name))       //搬檔
    //    writelog("產生收銀員帳表(XRP)檔案 " + str_new_name + " ->gchar_date_time="+gchar_date_time);
    //else
    //    writelog("產生收銀員帳表(XRP)檔案失敗 " + str_new_name);


    //str_xrp_path = str_new_name;        //寫入INI變數

    //寫入SAL檔1097Type
    string str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver,
               str_nxno_seq,
               str_ptran_cnt;

    str_rec_type = "1097";
    str_rec_len  = "0106";
    str_tencode = g_tencode;   //店號
	_Strsprintf(str_ecr_no,"%02d", _StrToInt(x->no_tm, "機號"));              //機號
    str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

    //int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入1097交易序號"); //目前寫入97交易序號
    //str_trans_cnt.sprintf("%05d", int_sum_transcnt);


    //20031215

    /**
    if ((int_sum_transcnt + 1) > 99999)
           int_sum_transcnt = 0;

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );
    str_ptran_cnt = str_sum_tran;
    **/

    //str_pxno_seq  //上次x帳累積次數

    //str_nxno_seq  //目前x次到第幾次

    // 2007/07/17
	_Strsprintf(str_z_cnt,"%02d", _StrToInt(str_nxno_seq, "交班序號"));

    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

	_Strsprintf(str_cashier_no,"%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    str_start_trans = _StringSegment_EX(str_x_first, SGM, 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

	_Strsprintf(str_totsale_sum,"%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
	_Strsprintf(str_xdt_no, "%010ld", _StrToInt(x->no_seq));

    str_ver = "FFFFFFFF";


    string str_trans1_5, CasherRptFile, Head1041;    //交易序號前5碼
    //str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    CasherRptFile=RtnCRPFile;  //ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
	_Strsprintf(Head1041,"3041||0063||%-6s||%2s||%14s||%05d||%-10s||%-20s||",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        XDTsysDttm.c_str(),                   //x->dt_begin+x->tm_begin,
                        _StrToInt(str_nz_cnt),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        
                        );


     
   string str_01,str_99;
   _Strsprintf(str_01, "%4s||%4s||%-6s||%-2s||%14s||%5d||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||\n",
       "3001",
       "0033",
       str_tencode.c_str(),
       str_ecr_no.c_str(),
       XDTsysDttm.c_str(),   //str_sys_date.c_str(),
       _StrToInt(str_nz_cnt),
       (str_trans1_5 + str_sum_tran).c_str(),
       "Z0",
       SaleDataVer.c_str(),                   //SALVER,  "2012022100",
       "00",     //退貨區分
       str_ecr_no.c_str(),                    //原始交易機號
       (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
       "0000000000",                          //退貨交易序號
       str_cashier_no.c_str(),                //收銀員代號
       "00",                                  //客層代碼
       " ",                                   //會員編號
       " ",                                   //卡號
       " ",                                   //部門次數  
       " ",                                   //部門金額
       " ",                                   //交易混登區分
       g_orgtencode.c_str(),                 //原始店號 gchar_orgtencode,   
       " ",                                   //SCO設備控制碼
       " "                                   //前次交易機號
       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

   _Strsprintf(str_99, "%4s||%4s||%-6s||%-2s||%14s||%5d||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||\n",
                   "3099",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   XDTsysDttm.c_str(),   //str_sys_date.c_str(),
                   _StrToInt(str_nz_cnt),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   str_cashier_no.c_str(),                //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   0,                                     //部門次數 
                   0,                                     //部門金額
                   " ",                                   //交易混登區分
                   g_orgtencode.c_str(),                  //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                    //前次交易機號
                   );


    string _str_tran_cnt;    //交易序號
    _str_tran_cnt = (str_trans1_5 + str_sum_tran);


    //寫入1097交易序號
    string str_97="";

	_Strsprintf(str_97,"%4s||%4s||%-6s||%2s||%14s||%05d||%-10s||%8s||%10s||000%8s||%10s||%8s||\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        XDTsysDttm.c_str(),      //str_sys_date.c_str(),
                        stoi(str_nz_cnt),      //str_z_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );


    /***************  產生收銀員交班明細表  收銀員交接班明細表 ***************/

    string SumCasherRptData;
	DeleteFile(StringToWString(RtnCRPFile).c_str());
    SumCasherRptXDT(NAUTOZ, 0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt,Head1041, StrNxnoSeq, RtnCRPFile,
                    str_01, str_97, xdt_1095_data, SumCasherRptData, iamt_food, iqt_food, 1);
	logsprintf("產生收銀員交班明細表:%s", CasherRptFile.c_str());
   
    RtnXDTData+=SumCasherRptData;

    logsprintf("XDATA::GetXDTData End:StrZCnt=%s, StrNxnoSeq=%s, Z_date=%s, DDZ_1099=%s, ZNT_1099=%s, RtnXRPFile=%s, RtnXRPFile=%s  ",
		StrZCnt.c_str(), StrNxnoSeq.c_str(), Z_date.c_str(), DDZ_1099.c_str(), ZNT_1099.c_str(), RtnXRPFile.c_str(), RtnCRPFile.c_str());

     return 1;

}



/// <summary>  XDATA::WriteData
/// 收銀員/交班 手動交  報表Class
/// </summary>
/// <param name=""></param>
/// <returns>int</returns>
int __fastcall XDATA::WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version)
{
    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName,true, "", "");               //載入SAL檔，將檔案依型態分類


    string sLog;
	logsprintf("XDATA::WriteData(%s, %s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str());

    if (!int_reslut)
        return 0;

    string str_ver_num,str_nz_cnt, sTmp, sSalesLoginDttm;

	str_ver_num = get_version(Version);     //版本編號


	//sprintf_s(gchar_tencode, sizeof(gchar_tencode),"%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no),"%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier),"%5.5s", SalesNo.c_str());
    g_cashier = SalesNo;

    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號

    //SPOS_XRPDT *x = new SPOS_XRPDT;         //X帳檔案結構

    //備份載入之原始之料
    Auto_Sal->clear();
	_wListLoadFromFile(Auto_Sal,SalFileName.c_str());      //for Unicode
	//_ListToFile(Auto_Sal,"c:\\ftlpos\\xzdata\\Bak.sal");
    _ListToWFile(Auto_Sal,"c:\\ftlpos\\xzdata\\Bak.sal");  //for Unicode
    //執行 loadsalfile(SalFileName,true, "", ""); ,會建立 Auto_Sal->Assign(tsl_x_data);

	Autotsl_rec->assign(gtsl_rec->begin(), gtsl_rec->end());
	Autotsl_tot->assign(gtsl_tot->begin(), gtsl_tot->end());
	Autotsl_drop->assign(gtsl_drop->begin(), gtsl_drop->end());
	Autotsl_easy_card->assign(gtsl_easy_card->begin(), gtsl_easy_card->end());
	Autotsl_pay_sub->assign(gtsl_pay_sub->begin(), gtsl_pay_sub->end());
	Autotsl_bil->assign(gtsl_bil->begin(), gtsl_bil->end());
	Autotsl_fetc_card->assign(gtsl_fetc_card->begin(), gtsl_fetc_card->end());
	Autotsl_subsale->assign(tsl_subsale->begin(), tsl_subsale->end());
	Auto_1051_sal->assign(gtsl_1051_sal->begin(), gtsl_1051_sal->end());


    TStringList *Z_CntLst = new TStringList;
    TStringList *Z_CntLsttmp = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->clear();
    ZDT_Lst->clear();
    Z_CntLsttmp->clear();

    if( !_GetZCntLst(Z_CntLsttmp, Auto_Sal  ) )
      {

        // 2013/10/21 Update
		tif_ecr.WriteString("X", "XOPERATION", "0");   //tif_ecr->WriteString("X", "XOPERATION", "0");
        logsprintf("XDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");

        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";

	   if (PathFileExists(StringToWString(sTmp).c_str()))  //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());  //DeleteFile(sTmp);
			   logsprintf("XDATA::DeleteFile=%s.", sTmp.c_str());
           }
        delete Z_CntLst;
        delete ZDT_Lst;
        delete Z_CntLsttmp;
        return 0;    //千萬不可回 return OK;
      }
    //Z_CntLst->Assign(Z_CntLsttmp);


    string str_x_filename,              //XDATA檔名
               str_ptran_cnt,               //前次結帳序號
               str_date_z,                  //DataZ(日期加X帳次數)
               str_tran_cnt,                //開店以來結帳金額
               str_pxno_seq,                //X帳結帳次數
               str_nxno_seq,                //下次結帳數
               str_YYYYMMDDZ,
               SaleDataVer,
               str_xrp_filename,
               str_crp_filename,
               XDTYYYYMMDD;                 //XDTYYYYMMDD:交班時間


	str_date_z = tif_ecr.ReadString("Z", "DATEZ", ""); //->ReadString("Z", "DATEZ", "");             //ddn
	sTmp = tif_ecr.ReadString("ECR", "TRAN_CNT", "00001");      //tif_ecr->ReadString("ECR", "TRAN_CNT", "00001");      //目前交易序號,但SAL最後一筆須檢1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    _Strsprintf(str_tran_cnt,"%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );


	str_YYYYMMDDZ = tif_ecr.ReadString("Z", "YYYYMMDDZ", "201001010000");   //tif_ecr->ReadString("Z", "YYYYMMDDZ", "201001010000");

    string str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;


	str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "00001");  //tif_ecr->ReadString("Z", "NZ_CNT", "00001");          //999999目前日結序號  換日不歸零
	str_date_time = tif_ecr.ReadString("Z", "pz_date", g_date_time);  //tif_ecr->ReadString("Z", "pz_date", gchar_date_time);  //2003/03/08 2004/03/22 liu
    if (str_date_time.length() < 8)
		str_date_time = currentDateTime(1);  //FormatDateTime("yyyymmddhhnnss", Now());        //2003/03/08 liu

	XDTYYYYMMDD = currentDateTime(1);  //FormatDateTime("yyyymmddhhnnss", Now());
    if(str_date_z.length() < 3)    //DATAZ=日期+Z帳次數  ddn
       str_date_z = (str_date_time.substr(6,2) + "1");

	logsprintf("XDATA::ECR->WriteData 取的目前 XZDATA.INI 之日結序號:%s, 交易序號:%s", str_nz_cnt.c_str(), str_tran_cnt.c_str());

    string str_sal_last, str_sal_last_1099;        //*.sal 最後一筆
    str_sal_last = gsal_last;                      // gpchar_sal_last;
    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本

     //2016/11/03     
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

    //if (str_tran_cnt.Length() < 5)  //上次交易序號
    //2012/08/06 Mark 已INI 為依據
    //if( Trim(str_tran_cnt)=="00000" )
    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, SGM, 7).substr(TRANCNT_NO_POS-1,5);  //交易序號

	logsprintf("XDATA::Last1099(%s), ECR->TRAN_CNT=%s", str_sal_last_1099.c_str(), str_tran_cnt.c_str());

	str_pxno_seq = tif_ecr.ReadString("X", "pxno_seq", "01");  //tif_ecr->ReadString("X", "pxno_seq", "01");                  //上次x帳累積次數

    _Strsprintf(str_nxno_seq,"%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //目前x次到第幾次
    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";


    string str_pzno_seq;
	str_pzno_seq = tif_ecr.ReadString("Z", "NZNO_SEQ", "");  //tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數1~9   換日歸1


	_Strsprintf(str_x_filename,"%s%02d%s%02d%02d.xdt",
                                         XZDATADIR,
                                         _StrToInt(g_ecr_no),
                                        (XDTYYYYMMDD.substr(2,6)).c_str(),
                                         _StrToInt(str_pzno_seq),_StrToInt(str_nxno_seq) );

    str_xrp_filename=_ChangeFileExt(str_x_filename,string(".xrp")).c_str();
	str_crp_filename = _ChangeFileExt(str_x_filename, string(".crp")).c_str();


    string DDZ_1099, str_x_path;
    DDZ_1099=str_date_z;
    if(  _StrToInt(DDZ_1099.substr(0,2),"DDZ_1099") < 1 || _StrToInt(DDZ_1099.substr(0,2),"DDZ_1099") > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, SGM, 7).substr(2,3) ;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //收銀員帳表備份路徑
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.substr(0,2) + "\\" + "0" + DDZ_1099.substr(2,1) + "\\";


    //帳表檔名
    string sTmpPzData;

    if ( Trim(str_x_path)=="" )
    {
        //無指定路徑由系繰日期決定

        str_x_path = "C:\\FTLPOS\\HISTORY\\";
		sTmpPzData = tif_ecr.ReadString("Z", "PZ_DATE", currentDateTime(1));    //tif_ecr->ReadString("Z", "PZ_DATE", FormatDateTime("yyyymmddhhnnss", Now())).Trim();

        if (sTmpPzData.empty())
        {            str_x_path += (str_date_time.substr(6,2) + "\\01\\");
            writelog("確認歷史目錄 01:" + str_x_path +" ->str_date_time="+str_date_time);

        }
        else
        {
            str_x_path += ( sTmpPzData.substr(6,2) + "\\0" );

			if (tif_ecr.ReadString("Z", "NZNO_SEQ", "").length() < 1) //               tif_ecr->ReadString("Z", "NZNO_SEQ", "").Length() < 1)
               { str_x_path += "1\\";
                writelog("確認歷史目錄 02:" + str_x_path  );
               }
            else
               {
				str_x_path += (tif_ecr.ReadString("Z", "NZNO_SEQ", "") + "\\");   // (tif_ecr->ReadString("Z", "NZNO_SEQ", "") + "\\");
                writelog("確認歷史目錄 03:" + str_x_path);
               }
        }

		tif_ecr.WriteString("Z", "XPATH", str_x_path); //   WriteString("Z", "XPATH", str_x_path);
    }

	if (!DirExists(str_x_path))  //if (!DirectoryExists(str_x_path))
       {
        writelog("建立歷史目錄 :" + str_x_path);
		SHCreateDirectory(NULL, StringToWString(str_x_path).c_str());   //ForceDirectories(str_x_path);
       }

   string StrXDTFileName, StrXRPFileName, StrCRPFileName;
   StrXDTFileName = (str_x_path + _ExtractFile(str_x_filename));

   StrXRPFileName = (str_x_path + _ExtractFile(str_xrp_filename));
   
   if (!PathFileExists(StringToWString(StrXRPFileName).c_str()))   //if (FileExists(StrXRPFileName))
	   DeleteFile(StringToWString(StrXRPFileName).c_str());

   StrCRPFileName = (str_x_path + _ExtractFile(str_crp_filename));
   if (!PathFileExists(StringToWString(StrCRPFileName).c_str()))   //if (FileExists(StrCRPFileName))
	   DeleteFile(StringToWString(StrCRPFileName).c_str());

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //str_orgam_tclsacc=str_am_tclsacc;

  string SaleXDT, sRec, TmpZcnt, str_xrp, XRPFile, CRPFile, sIdx;
  SaleXDT="";
  //XRPFile=ChangeFileExt( str_x_filename, ".Xrp");     //ExtractFileName(str_z_filename)
  //CRPFile=ChangeFileExt( str_x_filename, ".Crp");


  //for( int i=0; i< Z_CntLsttmp->Count; i++)
  for (list<string>::iterator ls = Z_CntLsttmp->begin(); ls != Z_CntLsttmp->end(); ls++)
    {
		sRec = *ls; // Z_CntLsttmp->Strings[i];
       TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
       GetZCNT_SalData(TmpZcnt) ;
       if( gtsl_tot->size() > 0 || gtsl_drop->size() > 0)
         {
           Z_CntLst->push_back(sRec);
		   logsprintf("XDATA::加入Z_CntLst:日結序號(%s)(%s)", TmpZcnt.c_str(), sRec.c_str());
         }
    }

    if(  Z_CntLst->size() <= 0 )
    {
		tif_ecr.WriteString("X", "XOPERATION", "0"); //->WriteString("X", "XOPERATION", "0");
        logsprintf("交班失敗 Z_CntLst->Count <=%d, XDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原", Z_CntLst->size());
 
        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
		if (!PathFileExists(StringToWString(sTmp).c_str()))   //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("XDATA::DeleteFile=%s.", sTmp.c_str());
           }
           
        delete Z_CntLst;
        delete ZDT_Lst;
        delete Z_CntLsttmp;
        return 0;    //千萬不可回 return OK;
    }


  str_nxno_seq=str_pxno_seq;
  Z_CntLst->sort();
  int iIdx=0;
  //for( int i=0; i< Z_CntLst->Count; i++)
  for (list<string>::iterator ls = Z_CntLst->begin(); ls != Z_CntLst->end(); ls++)
       {

         //sIdx::  01:第一筆, 10:最後一筆,  11:只有一筆
         if( Z_CntLst->size() == 1 )
            sIdx="11";
		 else if (ls == Z_CntLst->begin()) //i == 0)
            sIdx="01";
		 else if (ls == Z_CntLst->end() ) //Z_CntLst->Count - 1)
            sIdx="10";
         else
            sIdx="09";

		 sRec = *ls; // Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
        
         //XRPFile.sprintf("c:\\FtlPos\\XZDATA\\%s%s.Xrp",TmpZcnt.c_str(), str_nxno_seq.c_str() );
         //CRPFile.sprintf("c:\\FtlPos\\XZDATA\\%s%s.Crp",TmpZcnt.c_str(), str_nxno_seq.c_str() );
		 _Strsprintf(XRPFile,"c:\\FtlPos\\XZDATA\\Tmp.Xrp");
		 _Strsprintf(CRPFile, "c:\\FtlPos\\XZDATA\\Tmp.Crp");
         //CRPFile=ChangeFileExt( str_x_filename, ".Crp");
         GetZCNT_SalData(TmpZcnt) ;

         if( gtsl_tot->size() > 0 || gtsl_drop->size() > 0)
           {
             iIdx++;

			 _Strsprintf(str_nxno_seq,"%02d", (_StrToInt(str_nxno_seq, "pxno_seq") + 1)); //目前x次到第幾次
             if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
                  str_nxno_seq = "01";

             for( int i=0 ; i< 20; i++)     //INIT 代售付ｔａｂｌｅ 15-17 代售, 18-19 代付
                {
                   BaseTbSubrev[i].iqt5_totrev=0;
                   BaseTbSubrev[i].iam5_totrev=0;
                   for( int j=0 ; j<10; j++)
                      {
                        BaseTbSubrev[i].it8_subrev[j].iqt_subrev=0;
                        BaseTbSubrev[i].it8_subrev[j].iam_subrev=0;
                      }
                 }

            GetXDTData(TmpZcnt, g_tencode,  g_ecr_no, g_cashier, XDTYYYYMMDD, str_nxno_seq,
                    string(str_ver_num), str_date_z, DDZ_1099, sRec, sIdx, SaleXDT, XRPFile, CRPFile  );

          // int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo , String StrNxnoSeq,
          //                String Version, String Z_date, String DDZ_1099, String ZNT_1099,
          //                String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile );

            logsprintf("★ XDATA::產生交班表(%s, XRPFile=%s, CRPFile=%s)",SaleXDT.c_str(), XRPFile.c_str(), CRPFile.c_str() );

            ZDT_Lst->push_back(SaleXDT);
            str_xrp+=SaleXDT;

            SaleXDT="";

            file_cat( XRPFile.c_str(), StrXRPFileName.c_str() );
            logsprintf("XDATA::file_cat(%s, %s)",XRPFile.c_str(), StrXRPFileName.c_str() );

            file_cat( CRPFile.c_str(), StrCRPFileName.c_str() );
            logsprintf("XDATA::file_cat(%s, %s)",CRPFile.c_str(), StrCRPFileName.c_str() );
          }
          else
          {
            logsprintf("XDATA::(%s) 不輸出交班表",sRec.c_str()  );
          }

       } // end of for( int i=0; i< Z_CntLst->Count; i++)

       /**
     sTmp=ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+ExtractFileName(ZrpFile);
     CopyFile(ZrpFile.c_str(), str_zrp_path.c_str(), false);
     sLog.sprintf("XDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
     writelog( sLog);
     DeleteFile( ZrpFile );
     sLog.sprintf("XDATA::DeleteFile(%s)",ZrpFile.c_str() );
     writelog( sLog);

     str_dpt_path=sTmp+ExtractFileName(DptFile);
     CopyFile(DptFile.c_str(), str_dpt_path.c_str(), false);
     sLog.sprintf("XDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
     writelog( sLog);
     DeleteFile( DptFile );
     sLog.sprintf("XDATA::DeleteFile(%s)",DptFile.c_str() );
     writelog( sLog);
      **/
     delete Z_CntLst;
     delete ZDT_Lst;
     delete Z_CntLsttmp;
     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    string str_xdt_path, str_xrp_path;   //X資料、帳表存放路徑
 
    //str_xrp_path = str_new_name;        //寫入INI變數

    //寫入SAL檔1097Type
   string str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver;

    str_rec_type = "3097";
    str_rec_len  = "0021";

    str_tencode = g_tencode;   //店號

	_Strsprintf(str_ecr_no,"%02d", _StrToInt(x->no_tm, "機號"));              //機號
    str_sys_date = XDTYYYYMMDD; //(x->dt_end + x->tm_end);               //日期時間

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入1097交易序號"); //目前寫入97交易序號
	_Strsprintf(str_trans_cnt,"%05d", int_sum_transcnt);

	string str_sum_tran;       //此次收銀員帳交易序號


    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

	_Strsprintf(str_sum_tran,"%05d", (int_sum_transcnt + 1));

    //tif_tmp->WriteString("ECR","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_sum_tran;

    //str_pxno_seq  //上次x帳累積次數
    //str_nxno_seq  //目前x次到第幾次

    // 2007/07/17
	_Strsprintf(str_z_cnt,"%02d", _StrToInt(str_nxno_seq, "交班序號"));
    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

	_Strsprintf(str_cashier_no,"%-8s", Trim(x->no_casher).c_str() );   //收銀員代號

    //str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

	_Strsprintf(str_totsale_sum,"%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
	_Strsprintf(str_xdt_no,"%010ld", _StrToInt(x->no_seq));

    str_ver = "FFFFFFFF";


    string str_trans1_5, CasherRptFile;;    //交易序號前5碼
    //str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;
 
    string str_01,str_99;
    _Strsprintf(str_01, "%4s||%4s||%-6s||%-2s||%14s||%05d||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||\n",
	               "3001",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
				   _StrToInt(str_nz_cnt),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   str_cashier_no.c_str(),                //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   " ",                                   //部門次數
                   " ",                                   //部門金額
                   " ",                                   //交易混登區分
                   g_orgtencode.c_str(),                  //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                   //前次交易機號
                                       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);
    	
    _Strsprintf(str_99,"%4s||%4s||%-6s||%-2s||%14s||%05d||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%05d||%010d||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||\n",
                   "3099",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
				   _StrToInt(str_nz_cnt),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   str_cashier_no.c_str(),                //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   0,                                     //部門次數
                   0,                                     //部門金額 
                   " ",                                   //交易混登區分
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                   //前次交易機號
                                       );


    string _str_tran_cnt;    //交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);

    //寫入1097交易序號
    string str_97;
    /**
    str_97.sprintf("%4s|%4s|%-6s|%2s|%14s|%05d|%-10s|%8s|%10s|000%8s|%10s|%8s|",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.ToInt(),      //str_z_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );
    **/


    string str_sal;
    str_sal = str_01 +  str_xrp + str_99;  //str_97 + str_99;

    str_xdt_path=StrXDTFileName;

    if (w_WriteSAL(str_sal, str_xdt_path, 3) == OK)
    {
        writelog("產生收銀員 UTF8 XDT =>" + str_xdt_path);
    }
    else
    {
        writelog("無法產生收銀員 UTF8 XDT =>" + str_xdt_path);
        return 0;
    }

/******
FILE *fxdata;
#ifdef WIN32
	errno_t err;
    if ((err = fopen_s(&fxdata, str_xdt_path.c_str(), "w")) == 0)
#else
	if ((fxdata = fopen(str_xdt_path.c_str(), "w")) != NULL)
#endif

    {

        fprintf(fxdata,"%s",str_sal.c_str());
        fclose(fxdata);
        writelog("產生收銀員 XDT " + str_xdt_path);
    }
    else
    {
        writelog("無法產生收銀員 XDT " + str_xdt_path);
        return 0;
    }
******/

    /****   已交由 FTLPOS.exe 寫入 交易nn.SAL檔
     //寫入交易nn.SAL檔
    if( file_cat( str_xdt_path.c_str(), SalFileName.c_str() ) < 0 )
       {
         sTmp.printf("file_cat %s to %s Error. ",str_xdt_path.c_str(), SalFileName.c_str() );
         writelog(sTmp );
         // return 0;
       }
    else
       {
         sTmp.printf("file_cat %s to %s OK. ",str_xdt_path.c_str(), SalFileName.c_str() );
         writelog(sTmp );

       }
    *****/

    ///////////////////////////////////////////////////////////////////////////////////

	tif_ecr.WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran); //  WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印

    if ((int_sum_transcnt + 2) > 99999)
            int_sum_transcnt = 1;
    else
         int_sum_transcnt +=2;

	_Strsprintf(str_sum_tran,"%05d", int_sum_transcnt);

	tif_tmp.WriteString("ECR", "TRAN_CNT", str_sum_tran);   //tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    string sTtmpPTRAN_CNT;  //2012/06/04

    sTtmpPTRAN_CNT=str_trans1_5.substr(2,3) + str_ptran_cnt;
	tif_tmp.WriteString("X", "PTRAN_CNT", sTtmpPTRAN_CNT);   //tif_tmp->WriteString("X", "PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));

	tif_tmp.WriteString("X", "PXNO_SEQ", str_nxno_seq);   //tif_tmp->WriteString("X", "PXNO_SEQ", str_nxno_seq);
	tif_tmp.WriteString("X", "SX_DATE", (x->dt_begin + x->tm_begin));   //tif_tmp->WriteString("X", "SX_DATE", (x->dt_begin + x->tm_begin));
	tif_tmp.WriteString("X", "EX_DATE", (x->dt_end + x->tm_end));  //tif_tmp->WriteString("X", "EX_DATE", (x->dt_end + x->tm_end));
	tif_tmp.WriteString("ECR", "tot_open_x", "0");  //tif_tmp->WriteString("ECR", "tot_open_x", "0");
	tif_tmp.WriteString("ECR", "x_pay_cnt", "0");  //tif_tmp->WriteString("ECR", "x_pay_cnt", "0");

	tif_tmp.WriteString("X", "XFILEDTTM", str_sys_date);  //tif_tmp->WriteString("X", "XFILEDTTM", str_sys_date);    // XDT 之DTTM
	tif_tmp.WriteString("X", "XFILENAME", StrXDTFileName);  //tif_tmp->WriteString("X", "XFILENAME", StrXDTFileName);
	tif_tmp.WriteString("X", "XRPTNAME", StrXRPFileName);  //tif_tmp->WriteString("X", "XRPTNAME", StrXRPFileName);
	tif_tmp.WriteString("X", "CHRRPTNAME", StrCRPFileName);  //tif_tmp->WriteString("X", "CHRRPTNAME", StrCRPFileName);  
    
    //tif_tmp->WriteString("X","SALESLOGINDTTM" , sSalesLoginDttm );

    //delete x,tif_ecr;

    logsprintf("交班完成:寫入本次交班交易序號 tif_tmp->X->PTRAN_CNT=%s",sTtmpPTRAN_CNT.c_str() );

    return OK;

}



int __fastcall ZDATA::GetZDTData(string StrZCnt, string StoreNO,  string EcrNO, string SalesNo ,
                          string Version, string PZ_date, string Z_date, string DDZ_1099, string ZDT_1050,
                          string &RtnZDTData, string &Tclsacc,
                          string &str_zrp_path, string &str_dpt_path)
{
   int int_reslut = 0;
   string sLog, sTmp,str_ver_num, str_tran_cnt,ZDT_1050_DDZ;

   string str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               str_date_time, str_datez,    //目前帳表系統時間
               z_a;

    sTmp=_StringSegment_EX(ZDT_1050, SGM, 7);  //交易序號
    ZDT_1050_DDZ=sTmp.substr(2,3);
    str_am_tclsacc=Tclsacc;
    str_z_date=Z_date;
    str_pz_date=PZ_date;

    str_ver_num=Version ;
	   
	_Strsprintf(sYYMMDDZZ, "%s%s", str_z_date.substr(4, 4).c_str(), StrZCnt.c_str());


    logsprintf("** ZDATA::GetZDTData 產生日結帳表 Version=%s, ZDT:StrZCnt=%s, PZ_date=%s, Z_date=%s, DDZ_1099=%s, Tclsacc=%s **",
		Version.c_str(), StrZCnt.c_str(), PZ_date.c_str(), Z_date.c_str(), DDZ_1099.c_str(), Tclsacc.c_str());

    int i,j;
    for( i=0 ; i< 20; i++)     //INIT 代售付ｔａｂｌｅ 15-17 代售, 18-19 代付
           {
             BaseTbSubrev[i].iqt5_totrev=0;
             BaseTbSubrev[i].iam5_totrev=0;
             for( j=0 ; j<10; j++)
                {
                  BaseTbSubrev[i].it8_subrev[j].iqt_subrev=0;
                  BaseTbSubrev[i].it8_subrev[j].iam_subrev=0;
                }
           }

    pay_bill_cnt = 0;     //門市提貨券次數 20031223
    pay_bill_amt = 0;     //門市提貨券金額
    mpay_bill_cnt = 0;     //門市提貨券次數 20031223
    mpay_bill_amt = 0;     //門市提貨券金額

    Cpn71_cnt=0;                  //折價券71次數
    Cpn71_amt=0;                  //折價券71金額
    Cpn72_cnt=0;                  //折價券72次數
    Cpn72_amt=0;                  //折價券72金額
    Cpn73_cnt=0;                  //折價券73次數
    Cpn73_amt=0;                  //折價券73金額
    Cpn74_cnt=0;                  //折價券74次數
    Cpn74_amt=0;                  //折價券74金額
    Cpn75_cnt=0;                  //折價券75次數
    Cpn75_amt=0;                  //折價券75金額
    Cpn76_cnt=0;                  //折價券76次數
    Cpn76_amt=0;                  //折價券76金額

    Cpn77_cnt=0;                  //門市提貨券77次數
    Cpn77_amt=0;                  //門市提貨券77金額
    Cpn78_cnt=0;                  //門市提貨券78次數
    Cpn78_amt=0;                  //門市提貨券78金額


    z->no_tm = g_ecr_no;             //收銀機機號

    //日結序號
	_Strsprintf(z->bg_noclose,"%05d", _StrToInt(StrZCnt, "日結序號"));


    string ssysdttm, s1050sysdttm;
    ssysdttm=g_date_time;
    s1050sysdttm=_StringSegment_EX(ZDT_1050, SGM, 5);

    if( s1050sysdttm.substr(0,8) < ssysdttm.substr(0,8) )
      {
        z->bg_dtclose = s1050sysdttm.substr(0,8);      //結帳日期
        z->bg_tmclose = "235959";                         //結帳時間

        logsprintf("ZDATA::GetZDTData: s1050sysdttm=%s 小於 ssysdttm=%s, Set 1096 日結序號=%s, 結帳日期 z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
      }
    else
      {
        z->bg_dtclose = str_z_date.substr(0,8);      //結帳日期
        z->bg_tmclose = str_z_date.substr(8,6);      //結帳時間

        logsprintf("ZDATA::GetZDTData: s1050sysdttm=%s 大等於 ssysdttm=%s, Set 1096 日結序號=%s, 結帳日期 z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
      }



    //z->bg_dtclose = str_z_date.SubString(1,8);      //結帳日期
    //z->bg_tmclose = str_z_date.SubString(9,6);      //結帳時間
    z->ed_dtclose = str_pz_date.substr(0,8);     //前次結帳日期
    z->ed_tmclose = str_pz_date.substr(8,6);     //前次結帳時間

    
	_Strsprintf(z_a,"3096||%04d||%-6s||%-2s||%14s||%5s||%-10s||%8s||%6s||%8s||%6s||",
                                      189,
                                      g_tencode.c_str(),              //店號 3 
                                      z->no_tm.c_str(),           //收銀機機號 4
                                      Z_date.c_str(),             //現在時間 5
                                      z->bg_noclose.c_str(),      //日結序號 6
                                      sYYMMDDZZ.c_str(),          //Z帳表序號 7
                                      z->ed_dtclose.c_str(),      //前次結帳日期 8
                                      z->ed_tmclose.c_str(),      //前次結帳時間 9
                                      z->bg_dtclose.c_str(),      //結帳日期 10
                                      z->bg_tmclose.c_str()       //結帳時間 11
                                    );

    // ZDATA::GetZDTData 03 交易明細ｆｉｅｌｄ  =============================================

    //本日來客數/本日交易品項  SumQt_trans() ==> 併入 SumQt_tcust  NextPOS
    int iC0TotCount=0;     //點卡手續費來客數
    int iSumQt_trans = 0;  //本日交易品項
	_Strsprintf(z->tb_trans.qt_tcust,"%08d", SumQt_tcust(0, gi_tot_line)); //本日來客數
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項

    //本日交易品項
    //Sub_C0C1TaxInvoQty(0, gi_rec_line);  ==> 併入 Sub_C0C1TaxInvoAmt;   點卡手續費數量 NextPOS  
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);  //NextPOS
	_Strsprintf(z->tb_trans.qt_trans,"%08d", iSumQt_trans - giI); // NextPOS 本日交易品項 SumQt_trans(0, gi_tot_line)

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;


    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt, R4_C01TaxAmt,R4_C01FTaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01TaxAmt=R4_C01FTaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  NextPOS
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line); //1050==> R4+Z0    NextPOS
    TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(z->tb_trans.am_trcpntx,"%010d", giI - C01FTaxAmt - R4_C01FTaxAmt - (PreSal09_AmtNTX + R4PreSal09_AmtNTX));       // 發票日結帳額(免)
	_Strsprintf(z->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));       // 發票日結帳額(稅)
	_Strsprintf(z->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));       // 發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    logsprintf("ZDATA::GetZDTData: 發票結帳累計金額=%s, 發票日結帳金額=%d ", str_am_tclsacc.c_str(), int_tot_amt  );

      //本日結帳累計金額   //20150318 Mantis:0019359(開封店)維修通知函---150315-0078本日結帳累計金額已超過-1000萬
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000 )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));

	Tclsacc = int2str(int_acc_tot_amt);

	_Strsprintf(z->tb_trans.am_tclsacc,"%010d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    //TotMiss(0,gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
	_Strsprintf(z->tb_trans.qt_miss,"%08d", giA);       //  誤打發票次數
	_Strsprintf(z->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  誤打發票金額(免)
	_Strsprintf(z->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  誤打發票金額(稅)
	_Strsprintf(z->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));       //  誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //不開發票金額, 不開發票金額誤退重
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;
    //ZDATA::GetZDTData 本日實際銷貨收入
	_Strsprintf(z->tb_trans.am_totsale,"%010d", int_totsale);


    if (gbl_tot_haveline)
    {
        z->no_rcstart = invoice_start_end(gi_tot_line, true);    //發票開始號碼
        z->no_rcend = invoice_start_end(gi_tot_line, false);     //發票結束號碼
    }
    else    //無發票資料
    {
        z->no_rcstart = "0000000000";                            //發票開始號碼
        z->no_rcend   = "0000000000";                            //發票結束號碼
    }


    string z_b;

    z_b += (z->tb_trans.qt_tcust+ SGM);             //本日來客數 12
    z_b += (z->tb_trans.qt_trans+ SGM);             //本日交易品項 13
    z_b += (z->tb_trans.am_tclsacc+ SGM);           //本日結帳累計金額 14
    z_b += (z->no_rcstart+ SGM);                     //發票開始號碼 15
    z_b += (z->no_rcend+ SGM);                       //發票結束號碼 16
    z_b += (z->tb_trans.am_trcpntx+ SGM);           //發票日結帳額(免) 17
    z_b += (z->tb_trans.am_trcptax+ SGM);           //發票日結帳額(稅) 18
    z_b += (z->tb_trans.am_trcptot+ SGM);           //發票日結帳金額 19
    z_b += (z->tb_trans.qt_miss+ SGM);               //誤打發票次數 20
    z_b += (z->tb_trans.am_missntx+ SGM);           //誤打發票金額(免) 21
    z_b += (z->tb_trans.am_misstax+ SGM);           //誤打發票金額(稅) 22
    z_b += (z->tb_trans.am_miss+ SGM);              //誤打發票金額 23
    z_b += (z->tb_trans.am_totsale+ SGM);           //本日實際銷貨收入 24

 
    /* ZDATA::GetZDTData 代售付ｔａｂｌｅ       */

    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);   //giH giI 食安退貨金額、次數     //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(z->tb_newadd.qt_agnrcv,"%08d", giD);             //代收費用次數
	_Strsprintf(z->tb_newadd.am_agnrcv,"%010d", giC);             //代收費用金額


    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA, iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA = giH;  iSub_C0InvoAmtC = giJ;  iSub_C0InvoQtyC = giK;

    string z_c;
    int iAmt, iQty,k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    //NextPOS
    //Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    //Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;    

    //代售合計次數 25
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //代售合計金額   無須加入預售商品 26  
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //代付合計次數 27
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;
    //代付合計金額 28
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

	_Strsprintf(z_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );  //代售手續費商品合計回數 25 ~ 代付商品合計金額 28

    //代現金ｔａｂｌｅ=============================================
    int  iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    SumTbCash(0,gi_tot_line);  //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額


    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//消費券
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

    writelog("giS:消費券金額 , giT:消費券回數");

	_Strsprintf(z->tb_cash.qt_bonus,"%08d", giA); //-giT);            //禮券回數 29
	_Strsprintf(z->tb_cash.am_bonus,"%010d", giB); //-iCSMAmt);       //禮券金額 30


    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

	_Strsprintf(z->tb_cash.qt_vndcpn,"%08d", 0);              //廠商代現金回數 31
	_Strsprintf(z->tb_cash.am_vndcpn,"%010d", 0);             //廠商代現金金額 32

	_Strsprintf(z->tb_cash.qt_stocpn,"%08d", giC);           //折價券回數 33
	_Strsprintf(z->tb_cash.am_stocpn,"%010d", giD);          //折價券金額 34
    SumCpn41 +=  giD;                                        //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

	_Strsprintf(z->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt));  // - giT));  //代現金合計回數 35
	_Strsprintf(z->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額 36

    _Strsprintf(z->tb_cash.qt_bonout,"%08d", giG);            //溢收合計回數 找餘額券回數 37
	_Strsprintf(z->tb_cash.am_bonout,"%010d", giH);           //溢收合計金額 找餘額券金額 38

    iTotOt2Rtn=giE;                                      //餘額券
    iqt_ffu1=giI;                                        //現金卡回數  電子錢 1847
    iam_ffu1=giJ;                                        //現金卡金額  電子錢 1855    刷卡合計

    string z_d;

    z_d += (z->tb_cash.qt_bonus+ SGM);                //禮券回數 29
    z_d += (z->tb_cash.am_bonus+ SGM);                //禮券金額 30
    z_d += (z->tb_cash.qt_vndcpn+ SGM);               //廠商折價券回數 31
    z_d += (z->tb_cash.am_vndcpn+ SGM);               //廠商折價券金額 32
    z_d += (z->tb_cash.qt_stocpn+ SGM);               //折價券回數 33
    z_d += (z->tb_cash.am_stocpn+ SGM);               //折價券金額 34
    z_d += (z->tb_cash.qt_totcash+ SGM);              //代現金合計回數 35
    z_d += (z->tb_cash.am_totcash+ SGM);              //代現金合計金額 36

	_Strsprintf(sTmp,"%08d", giF);
    z_d += (sTmp+ SGM);                               //餘額合計回數 37
	_Strsprintf(sTmp,"%010d", giE);
    z_d += (sTmp+ SGM);                               //餘額合計金額 38

    z_d += (z->tb_cash.qt_bonout+ SGM);               //溢收合計回數 39
    z_d += (z->tb_cash.am_bonout+ SGM);               //溢收合計金額 40

    //ZDATA::GetZDTData======================================================
    //現金合計 含 消費券

    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(z->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);

    //ZDATA::GetZDTData Z帳部門別集計TABLE      =============================================

    string z_f;

    //s1部門合計交易項數
    //s2部門合計銷售金額
    //s3部門合計誤退項數
    //s4部門合計誤退金額
    int s1, s2, s3, s4;

    s1 = s2 = s3 = s4 = 0;
    DEPT *dept;
	dept = new DEPT[20];
    try
    {
        //先將值填零
        for (int i=0; i<20; i++)
        {
			_Strsprintf(z->tb_depcal[i].am_dpsitm,"%08d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpsale,"%010d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpmitm,"%08d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpmiss,"%010d", 0);

            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //writelog("INIT 部門交易資料, 部門帳");
        //計算所有部門交易資料, 部門帳


          if (gbl_rec_haveline)
            SumAllDept(dept);

        // writelog("計算所有部門交易資料, 部門帳 OK");
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //部門01交易項數 89
        // ~
        //部門20誤退金額 168
        ////////////////////////////////////////////////////////////////////////////////////////////////
        for (int i=0; i<20; i++)
        {
			_Strsprintf(z->tb_depcal[i].am_dpsitm,"%08d", dept[i].am_dpsitm); 
            z_f += (z->tb_depcal[i].am_dpsitm+ SGM);
            s1 += dept[i].am_dpsitm;

			_Strsprintf(z->tb_depcal[i].am_dpsale,"%010d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+ SGM);
            s2 += dept[i].am_dpsale;

			_Strsprintf(z->tb_depcal[i].am_dpmitm,"%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+ SGM);
            s3 += dept[i].am_dpmitm;

			_Strsprintf(z->tb_depcal[i].am_dpmiss,"%010d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+ SGM);
            s4 += dept[i].am_dpmiss;
        }


    }
	catch (...)
    {
		;;
    }
	delete[] dept;
   //ZDATA::GetZDTData writelog("Z帳部門別集計TABLE OK");

    string z_g;

	z_g += (_Strsprintf(z->am_tdpsitm,"%08d", s1) + SGM);             // 部門合計交易項數 41
	z_g += (_Strsprintf(z->am_dpstot,"%010d", s2) + SGM);             // 部門合計銷售金額 42
	z_g += (_Strsprintf(z->am_tdpmitm,"%08d", s3) + SGM);             // 部門合計誤退項數 43
	z_g += (_Strsprintf(z->am_dpmtot,"%010d", s4) + SGM);             // 部門合計誤退金額 44
    z_g += (z->am_cash+"||");                                          // 現金合計含消費券 45


    //ZDATA::GetZDTData 收銀員明細ｆｉｅｌｄ=============================================

    string z_h;

   	_Strsprintf(sTmp,"%010d", VisuilCpnAmt);                       //無紙本折價金額  46
    z_h += (sTmp+ SGM);                                           //借零次數  : VisuilCpnAmt :      
    _Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    z_h += (sTmp+ SGM);                                           //代售商品折讓金額  47

	z_h += (_Strsprintf(z->tb_casher.qt_feedtot,"%08d", 0) + SGM);     // 投零次數 48
	z_h += (_Strsprintf(z->tb_casher.am_feedtot,"%010d", 0) + SGM);    // 投零合計金額 49

    //投庫
	int int_drop_line = gtsl_drop->size() - 1; //  (gtsl_drop->Count - 1);
    int int_drops = 0;           //投庫次數
    int int_drop_money = 0;       //投庫金額
    int int_drop_sum = 0;    //投庫總額
    int x_rtn_cnt=0;

    string   s1041;
    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
        //StringSegment_EX(s, "|", 15)
		s1041 = *ls; // gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
			   logsprintf("產生日結帳表:非投庫(%s)資料", s1041.c_str());
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, SGM, 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //IC付款 FU_SPACE10
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //信用卡 FU_SPACE10 
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 12));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 13));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //提貨單 FU_SPACE10

		 //ZDATA::GetZDTData中獎發票金額 500
		int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 16));
        //ZDATA::GetZDTData中獎發票金額 1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 27));

        //ZDATA::GetZDTData中獎發票金額 200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 15));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 18)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 20)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 22)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 24)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 26)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 28)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 30)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 32)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 34)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 36)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 38)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 40)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 42)); //費用 13金額 食安退款金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 44)); //費用 14金額 提貨券金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數
    }

	z_h += (_Strsprintf(z->tb_casher.qt_droptot,"%08d", int_drops) + SGM);         // 投庫次數 50
	z_h += (_Strsprintf(z->tb_casher.am_droptot,"%010d", int_drop_sum) + SGM);     // 投庫合計金額 51

    //ZDATA::GetZDTData 更正次數、金額
    SumUpdat(0,gi_rec_line);
	z_h += (_Strsprintf(z->tb_casher.qt_update,"%08d", giA) + SGM);           // 更正次數 52
	z_h += (_Strsprintf(z->tb_casher.am_update,"%010d", giB) + SGM);          // 更正合計金額 53


    //交易取消
    SumVoid(0,gi_tot_line);
	z_h += (_Strsprintf(z->tb_casher.qt_void,"%08d", giA) + SGM);            // 交易取消次數 54
	z_h += (_Strsprintf(z->tb_casher.am_void,"%010d", giB) + SGM);           // 交易取消合計金額 55

     //TIniFile *ini_ecr;
    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
		
        int_ZINI_TRAN_MODE_CNT = stoi(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_CNT", "0"));
        int_ZINI_TRAN_MODE_AMT = stoi(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_AMT", "0"));

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr.WriteString("ECR", "ZINI_TRAN_MODE_CNT", "0");
        tif_ecr.WriteString("ECR", "ZINI_TRAN_MODE_AMT", "0");
    }
	catch (...)
    {
        ;;
    }

	z_h += (_Strsprintf(z->tb_casher.qt_train,"%08d", int_ZINI_TRAN_MODE_CNT) + SGM);  // 訓練合計次數 56
	z_h += (_Strsprintf(z->tb_casher.am_train,"%010d", int_ZINI_TRAN_MODE_AMT) + SGM); // 訓練合計金額 57

    //手動輸入部門13、14
    SumInpdp(0,gi_rec_line);
	z_h += (_Strsprintf(z->tb_newadd.qt_inpdp,"%08d", giA) + SGM);             //手動輸入部門次數 58
	z_h += (_Strsprintf(z->tb_newadd.am_inpdp,"%010d", giB) + SGM);            //手動輸入部門金額 59

	z_h += (_Strsprintf(z->tb_casher.qt_openbox,"%08d", gi_open_z) + SGM);     // 開錢櫃次數 60

    //折扣讓次數   //小計折扣
    //SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	z_h += (_Strsprintf(z->tb_casher.qt_disc,"%08d", giC) + SGM);             // 折扣次數(應稅) 61
	z_h += (_Strsprintf(z->tb_casher.am_disc,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + SGM);  // 折扣金額(應稅) 62

    //pay_sub_amt(0,gi_pay_line);
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu6,"%08d", giA) + SGM);            //促銷折扣/讓次數 63
	z_h += (_Strsprintf(z->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + SGM);             //促銷折扣/讓金額 64

    //加值機
    easy_card(0,gi_easy_line);
	z_h += (_Strsprintf(z->tb_casher.qt_disret1,"%08d", giA) + SGM);          // 加值機開/售卡次數 65
	z_h += (_Strsprintf(z->tb_casher.am_disret1,"%010d", giB) + SGM);         // 加值機開/售卡金額 66
	z_h += (_Strsprintf(z->tb_casher.qt_disret2,"%08d", giC) + SGM);          // 加值機加值次數 67
	z_h += (_Strsprintf(z->tb_casher.am_disret2,"%010d", giD) + SGM);         // 加值機加值金額 68

    //fetc_card(0,gi_fetc_line);   //e通卡   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    z_h += (_Strsprintf(z->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + SGM);    // 代售,手續費銷售金額(免) 69
	z_h += (_Strsprintf(z->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + SGM); // 代售,手續費作廢金額(免) 70

    //食安次數、金額  int iqt_food,iamt_food;
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu9,"%08d", iqt_food) + SGM);    //食安退款筆數  71
	z_h += (_Strsprintf(z->tb_newadd.am_ffu9,"%010d", iamt_food) + SGM);  //食安退款金額  72

    //   05 追加 table        =============================================

    string z_i;

	z_i += (_Strsprintf(z->tb_newadd.qt_cashCard,"%08d", iqt_ffu1) + SGM);     //刷卡/現金卡次數 73
	z_i += (_Strsprintf(z->tb_newadd.am_cashCard,"%010d", iam_ffu1) + SGM);    //刷卡/現金卡金額 74
	z_i += (_Strsprintf(z->tb_newadd.qt_parking,"%08d", 0) + SGM);             //代收停車費次數  75
	z_i += (_Strsprintf(z->tb_newadd.am_parking,"%010d", 0) + SGM);            //代收停車費金額  76
	z_i += (z->tb_newadd.qt_agnrcv + SGM);                                     //代收費用次數  77
	z_i += (z->tb_newadd.am_agnrcv + SGM);                                     //代收費用金額  78
	z_i += (_Strsprintf(z->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + SGM);     //版本前置符號 79
	z_i += (_Strsprintf(z->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + SGM);      //版本號碼     80
	z_i += (_Strsprintf(z->tb_newadd.qt_rcp0,"%010d", TotZero(0, gi_tot_line)) + SGM);             //發票金額0次數 81

    //ZDATA::GetZDTData 不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);

	z_i += (_Strsprintf(z->tb_newadd.am_ffu4,"%010d", int_novoice_amt) + SGM);        //不開發票金額 82
	z_i += (_Strsprintf(z->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + SGM);   //  不開發票金額誤退重 83

    SumLOSS(0,gi_rec_line);
	z_i += (_Strsprintf(z->tb_newadd.qt_ffu5,"%08d", giA) + SGM);             // TM廢棄Loss值次數 84
	z_i += (_Strsprintf(z->tb_newadd.am_ffu5,"%010d", giB) + SGM);            // TM廢棄Loss值金額 85

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);     // 健康捐合計
	z_i += (_Strsprintf(z->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + SGM);   //手續費合計 86

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	z_i += (_Strsprintf(z->tb_newadd.qt_ffu7,"%010d", iSumRelDimAgn + iC0InvoAmt4VDCN) + SGM);   // 即時購代售金額 87

    //消費券
	z_i += (_Strsprintf(z->tb_newadd.am_posZRpt,"%010d", iCSMAmt) + SGM);      //消費券 88
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //部門01交易項數 89
    // ~
    //部門20誤退金額 168
    ////////////////////////////////////////////////////////////////////////////////////////////////
    string z_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值  
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    即時購(應稅) 169
    z_k +=sTmp;   //"00000000|";
	_Strsprintf(sTmp,"%010d||", giD);     // 即時購與銷轉進值免稅 即時購(免稅) 170
    z_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

	_Strsprintf(sTmp,"%08d||", giA / 200); //發票兌獎張數 171
    z_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", giA);     //發票兌獎金額 172
    z_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);  //電子禮券回數 173
    z_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //電子禮券金額 174
    z_k +=sTmp;

    string StrTran_cnt;
    int iTran_cnt;
    sTmp = _StringSegment_EX(gsLast1099, SGM, 7).substr(TRANCNT_NO_POS - 1, 5);  //交易序號
    iTran_cnt= _StrToInt(sTmp);

    if ((iTran_cnt + 1) > 99999)
        iTran_cnt = 0; //00001
    else
        _Strsprintf(StrTran_cnt, "00%3s%05d", DDZ_1099.c_str(),  (iTran_cnt + 1) );

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    // 代售手續費應稅金額 175 ~ 代售手續費來客數 177
	_Strsprintf(sTmp,"%010d||%010d||%08d||%s||%10s||1||2||3||4||5||6||7||8||9||0||",
        iC0TotAmt, iC0GivUpAmt, iC0TotCount, " ", StrTran_cnt.c_str() );

    z_k +=sTmp;

    //string DDZ_1099;
    //DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
    //   DDZ_1099=str_datez;

    //ZDATA::GetZDTData Z報表
    string str_zrp;

   	_Strsprintf(str_zrp,"%s%s%s%s%s%s%s%s%s\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str()
                            );

	logsprintf("ZDATA::GetZDTData::(%s) Len=%d", str_zrp.c_str(), str_zrp.length());

    string  Zrpfilename;
    //sTmp = ExtractFileName(SalFileName);
	_Strsprintf(Zrpfilename,"%s%s_%s.zrp",
                                        XZDATADIR,
                                        sYYMMDDZZ.c_str(),
                                        ZDT_1050_DDZ.c_str()  );


    //ZDATA::GetZDTData Z報表
	strcpy_s(report->char_filename, sizeof(report->char_filename), Zrpfilename.c_str()); //ChangeFileExt(str_z_filename,".zrp").c_str());
    report->CreateReport("Z", str_zrp, StrBillPay, g_tencode, g_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev , NULL, 0, "");
    //備份Z報表
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //部門帳表
	strcpy_s(report->char_filename, sizeof(report->char_filename), _ChangeFileExt(Zrpfilename, string(".dpt") ).c_str());
    report->CreateReport("D", str_zrp, StrBillPay, g_tencode, g_ecr_no ,0,0,0,0, NULL,0,"");
    //備份部門帳表
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    RtnZDTData=str_zrp;

  return(0);
}



//ZDATA::WriteData  日結
int __fastcall ZDATA::WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version, const string AutoZDttm)
{

    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    string sLog, sTmp, TmpZcnt,sRec;
    string str_ver_num, str_tran_cnt, SaleZDT;


	logsprintf("ZDATA::WriteData(%s, %s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str());

    if (!int_reslut)
        return 0;

	//sprintf_s(gchar_tencode, sizeof(gchar_tencode), "%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", SalesNo.c_str());
    g_cashier = SalesNo;
    str_ver_num = Version;    //get_version(Version);   日結

    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號

    //備份載入之原始之料
    Auto_Sal->clear();
	_wListLoadFromFile(Auto_Sal, SalFileName); 
	Autotsl_rec->assign(gtsl_rec->begin(), gtsl_rec->end() );
	Autotsl_tot->assign(gtsl_tot->begin(), gtsl_tot->end());
	Autotsl_drop->assign(gtsl_drop->begin(), gtsl_drop->end());
	Autotsl_easy_card->assign(gtsl_easy_card->begin(), gtsl_easy_card->end());
	Autotsl_pay_sub->assign(gtsl_pay_sub->begin(), gtsl_pay_sub->end());
	Autotsl_bil->assign(gtsl_bil->begin(), gtsl_bil->end());
	Autotsl_fetc_card->assign(gtsl_fetc_card->begin(), gtsl_fetc_card->end());
	Autotsl_subsale->assign(tsl_subsale->begin(), tsl_subsale->end());
    //Auto_1051_sal->Assign(gtsl_1051_sal);       自動日結無須使用

    TStringList *Z_CntLst = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->clear();
    ZDT_Lst->clear();

    if( !_GetZCntLst(Z_CntLst, Auto_Sal ) )
        return 0;

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z帳結構

    //目前Z帳時間
    string str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               DDZ_1099,
               str_orgam_tclsacc,
               str_zrp,
               str_z_filename,               //Z帳輸出檔名
               Lst_Zcnt_Dttm,
               str_sal_first;

    string str_date_time, str_datez;               //目前帳表系統時間
    string  Zrpfilename, str_zrp_path, str_dpt_path;


    str_date_time = g_date_time;
	sTmp = tif_ecr.ReadString("ECR", "TRAN_CNT", "00001"); //    ReadString("ECR", "TRAN_CNT", "00001");      //目前交易序號,但SAL最後一筆須檢1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    _Strsprintf(str_tran_cnt,"%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    logsprintf("ZDATA::WriteData 取的目前之交易序號:%s", str_tran_cnt.c_str());

    str_sal_first = gsal_first;                     // gpchar_sal_first;
	str_pz_date = tif_ecr.ReadString("Z", "PZ_DATE", ""); //tif_ecr->ReadString("Z", "PZ_DATE", "");       //前次時間

    sTmp = _StringSegment_EX(str_sal_first, SGM, 5);
    if (sTmp < str_pz_date)
        str_pz_date = sTmp;

    str_z_date  = str_date_time;                               //現在時間
	str_datez = tif_ecr.ReadString("Z", "DATEZ", "");   // tif_ecr->ReadString("Z", "DATEZ", "");           //DDZ
	Lst_Zcnt_Dttm = tif_ecr.ReadString("AUTOZ", "LST_ZCNT_DTTM", str_z_date.substr(0, 8));     //tif_ecr->ReadString("AUTOZ", "LST_ZCNT_DTTM", str_z_date.SubString(1, 8));   //換日參考日

    //將帳表日資料備份已被將來判斷殺目錄條件 james - 2003/02/07

	sTmp = tif_ecr.ReadString("Z", "YYYYMMDDZ", (str_date_time.substr(0, 8) + "0100")); // tif_ecr->ReadString("Z", "YYYYMMDDZ", (str_date_time.SubString(1, 8) + "0100"));  //自手動日結要如此

    //將帳表日資料備份已被將來判斷殺目錄條件 james - 2003/02/07
	tif_ecr.WriteString("Backup", "pz_date", sTmp); // WriteString("Backup", "pz_date", sTmp);

    //tif_ecr->WriteString("Backup", "pz_date", str_pz_date);
    //備份 sal8.TXT條件
    //tif_ecr->WriteString("Backup", "datez", str_datez);

    if ((str_pz_date == "") || (str_pz_date.length() < 8))  //無相關資料，交易記錄歸零
    {
		tif_ecr.WriteString("Z", "pz_date", str_z_date);  // tif_ecr->WriteString("Z", "pz_date", str_z_date);
		tif_ecr.WriteString("Z", "pzno_seq", "0");        //tif_ecr->WriteString("Z", "pzno_seq", "0");
		tif_ecr.WriteString("Z", "nzno_seq", "1");  //tif_ecr->WriteString("Z", "nzno_seq", "1");
		tif_ecr.WriteString("Z", "pz_cnt", "00000");  //tif_ecr->WriteString("Z", "pz_cnt", "00000");
		tif_ecr.WriteString("Z", "nz_cnt", "00001");  //tif_ecr->WriteString("Z", "nz_cnt", "00001");
		tif_ecr.WriteString("Z", "datez", (str_date_time.substr(6, 2) + "1"));  //tif_ecr->WriteString("Z", "datez", (str_date_time.SubString(7,2) + "1"));
        //20031225
		tif_ecr.WriteString("Z", "YYYYMMDDZ", (str_date_time.substr(0, 8) + "0100")); // tif_ecr->WriteString("Z", "YYYYMMDDZ", (str_date_time.SubString(1, 8) + "0100"));
		tif_ecr.WriteString("Z", "monthz", currentDateTime(1).substr(4, 2)); // tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm", Now()));

		tif_ecr.WriteString("Z", "zfilename", "");   // tif_ecr->WriteString("Z", "zfilename", "");
		tif_ecr.WriteString("Z", "zrptname", ""); // tif_ecr->WriteString("Z", "zrptname", "");
		tif_ecr.WriteString("Z", "deptname", ""); // tif_ecr->WriteString("Z", "deptname", "");
		tif_ecr.WriteString("Z", "inqtname", ""); // tif_ecr->WriteString("Z", "inqtname", "");

		tif_ecr.WriteString("Z", "PZNO_SEQ", "0"); // tif_ecr->WriteString("VXZ", "PZNO_SEQ", "0");
		tif_ecr.WriteString("Z", "NZNO_SEQ", "1"); // tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");
		tif_ecr.WriteString("Z", "PZ_DATE", ""); // tif_ecr->WriteString("VXZ", "PZ_DATE", "");

        //tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        //tif_ecr->WriteString("VXZ","CS_ENTRY","");

        string str_xpath;               //指定收銀員帳表目錄
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.substr(6,2) + "\\01\\");
		tif_ecr.WriteString("Z", "XPATH", str_xpath); // tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date;
        Lst_Zcnt_Dttm=str_z_date.substr(0,8);
    }

	str_pzno_seq = tif_ecr.ReadString("Z", "NZNO_SEQ", "");         //目前Z帳次數 01~09  換日歸1
	str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "");            //目前日結序號  換日不歸零 00001~99999

	tif_ecr.WriteString("Z_OFF", "nz_cnt", str_nz_cnt); // tif_ecr->WriteString("Z_OFF", "nz_cnt", str_nz_cnt);            // 離線資料使用
	tif_ecr.WriteString("Z_OFF", "pz_cnt", tif_ecr.ReadString("Z", "pz_cnt", "00000")); // tif_ecr->WriteString("Z_OFF", "pz_cnt", tif_ecr->ReadString("Z", "pz_cnt", "00000"));  // 離線資料使用

	str_am_tclsacc = tif_ecr.ReadString("Z", "AM_TCLSACC", ""); // tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零

    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.length() < 5)
        str_nz_cnt = "00001";

	_Strsprintf(sYYMMDDZZ,"%s%02d", str_pz_date.substr(2, 6).c_str(), _StrToInt(str_pzno_seq));
    


	_Strsprintf(str_z_filename,"%s%02d%s%02d%s.zdt",
                                        XZDATADIR,
                                        _StrToInt(g_ecr_no),
										str_pz_date.substr(2,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        str_pz_date.substr(8,4).c_str()    );

    //換日Flag
    bool bl_change_day;

    //判斷換日
    //if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
    if ( str_z_date.substr(0,8) > str_pz_date.substr(0,8) &&
         str_z_date.substr(0,8) > Lst_Zcnt_Dttm  )
        bl_change_day = true;
    else
        bl_change_day = false;


    DDZ_1099=_StringSegment_EX(gsLast1099, SGM, 7).substr(2,3) ;

    if(stoi(DDZ_1099.substr(0,2)) < 1 || stoi(DDZ_1099.substr(0,2)) > 31 )
       DDZ_1099=str_datez;

	tif_ecr.WriteString("Backup", "datez", DDZ_1099); // tif_ecr->WriteString("Backup", "datez", DDZ_1099);  //歷史目錄維護用


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

     str_orgam_tclsacc=str_am_tclsacc;

     string ZrpFile, DptFile;

     ZrpFile=_ChangeFileExt( str_z_filename, string(".zrp"));     //ExtractFileName(str_z_filename)
	 DptFile = _ChangeFileExt(str_z_filename, string(".dpt"));
     // sBackupPath=ExtractFilePath(BackupFileName);
     //   sFileNameNonPath = ExtractFileName(BackupFileName);    //檔名不包含路徑

     //for( int i=0; i< Z_CntLst->Count; i++)
	 for (list<string>::iterator ls = Z_CntLst->begin(); ls != Z_CntLst->end(); ls++)
       {
		   sRec = *ls; // Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);
         GetZCNT_SalData(TmpZcnt) ;
         GetZDTData(TmpZcnt, g_tencode, g_ecr_no, g_cashier,
                    string(str_ver_num), str_pz_date, str_z_date, DDZ_1099, sRec,
                    SaleZDT, str_am_tclsacc, str_zrp_path, str_dpt_path  );

         ZDT_Lst->push_back(SaleZDT);
         str_zrp+=SaleZDT;

         SaleZDT="";

         file_cat( str_zrp_path.c_str(), ZrpFile.c_str() );
         logsprintf("ZDATA::file_cat(%s, %s)",str_zrp_path.c_str(), ZrpFile.c_str() );

         file_cat( str_dpt_path.c_str(), DptFile.c_str() );
         logsprintf("ZDATA::file_cat(%s, %s)",str_dpt_path.c_str(), DptFile.c_str() );

       } // end of for( int i=0; i< Z_CntLst->Count; i++)

     sTmp=_ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+_ExtractFile(ZrpFile);
	 CopyFile(StringToWString(ZrpFile).c_str(), StringToWString(str_zrp_path).c_str(), false);
     logsprintf("ZDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
	 DeleteFile(StringToWString(ZrpFile).c_str());
     logsprintf("ZDATA::DeleteFile(%s)",ZrpFile.c_str() );

     str_dpt_path=sTmp+_ExtractFile(DptFile);
	 CopyFile(StringToWString(DptFile).c_str(), StringToWString(str_dpt_path).c_str(), false);
     logsprintf("ZDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
	 DeleteFile(StringToWString(DptFile).c_str());
     logsprintf("ZDATA::DeleteFile(%s)",DptFile.c_str() );

     delete Z_CntLst;
     delete ZDT_Lst;

     //寫入SAL檔98Type
     string str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_p_z_cnt,
               str_n_z_cnt,
               str_start_trans,            //開始交易序號
               str_am_tclascc,
               str_am_totsale,
               str_zdt_no,
               str_ver,
               str_sal_last_1099,
               SaleDataVer,
               str_zdt_path;

    str_rec_type = "3098";
    str_rec_len = "23";
    str_tencode = g_tencode;
	//_Strsprintf(str_ecr_no,"%s", g_ecr_no.c_str());
    str_ecr_no = g_ecr_no;
    str_sys_date = str_z_date;      //日期時間

    //98交易序號 //20031215
    int int_sum_tran_cnt ;

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, SGM, 7).substr(TRANCNT_NO_POS-1,5);  //交易序號

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

	logsprintf("ZDATA::Last1099(%s), ECR->TRAN_CNT=%s", str_sal_last_1099.c_str(), str_tran_cnt.c_str());

    string str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
		_Strsprintf(str_sum_tran,"%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //前次日結序號，目前日結序號
	_Strsprintf(str_p_z_cnt,"%05d", _StrToInt(tif_ecr.ReadString("Z", "PZNO_SEQ", ""), "前次日結序號"));
	_Strsprintf(str_n_z_cnt,"%05d", _StrToInt(tif_ecr.ReadString("Z", "NZNO_SEQ", ""), "目前日結序號"));


    //if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
	if (tif_ecr.ReadString("Z", "PZNO_SEQ", "").length() < 5)
        str_p_z_cnt = "00000";

    //if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
	if (tif_ecr.ReadString("Z", "NZNO_SEQ", "").length() < 5)
        str_n_z_cnt = "00001";

    int i_tran_cnt, int_am_tclsacc, int_acc_tot_amt;

    int_acc_tot_amt=_StrToInt(str_am_tclsacc);
    //string str_sal_first;
    //str_sal_first = gsal_first;   //gpchar_sal_first;

     //開始交易序號
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, SGM, 7).substr(TRANCNT_NO_POS-1,5));
	_Strsprintf(str_start_trans,"%05d", i_tran_cnt);

    //int_am_tclsacc=int_acc_tot_amt;
    //str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //本日結帳累計金額  
    str_am_totsale = "0";   //z->tb_trans.am_totsale;         //本日實際銷貨收入  
	_Strsprintf(str_zdt_no,"%010ld", _StrToInt(str_nz_cnt, "日結序號"));

    str_ver = "1||2||3||4||5||6||7||8||9||0";

    string str_trans1_5;    //交易序號前五碼

    string str_sal_last;
    str_sal_last = gsal_last;           // gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, SGM, 7).SubString(1,5);
    //if( Trim(str_trans1_5)=="")
    str_trans1_5="00"+DDZ_1099;

	tif_ecr.WriteString("Backup", "datez", DDZ_1099); // WriteString("Backup", "datez", DDZ_1099);  //歷史目錄維護用  不可刪

    string str_01,str_99;

	_Strsprintf(str_01,"%4s||%4s||%-6s||%-2s||%14s||%5s||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                   "3001",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   " ",                                   //部門次數
                   " ",                                   //部門金額
                   " ",                                   //交易混登區分 
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                  //前次交易機號
                                       );

    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

	_Strsprintf(str_99,"%4s||%4s||%-6s||%-2s||%14s||%5s||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                   "3099",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   "0",                                     //部門次數
                   "0",                                     //部門金額
                   " ",                                   //交易混登區分 
                   g_orgtencode.c_str(),                      //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                    //前次交易機號
                                       );

    string _str_tran_cnt;        //交易序號
    string str_st_tran_cnt;     //開始交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    string str_98;  //寫入3098交易序號
	_Strsprintf(str_98,"%4s||%4s||%6s||%2s||%14s||%-5s||%-10s||%5s||%5s||%10s||0000%8s||0000%8s||%10s||%s||\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_p_z_cnt.c_str(),
                        str_n_z_cnt.c_str(),
                        str_st_tran_cnt.c_str(),
                        str_am_tclascc.c_str(),
                        str_am_totsale.c_str(),
                        str_zdt_no.c_str(),
                        str_ver.c_str()
                        );



    string str_sal;
    //str_sal = str_01 + "\n"+ str_zrp + str_98 + str_99;
    str_sal = str_01 + "\n"+ str_zrp + str_99;

    // nextPOS
    if (w_WriteSAL(str_sal, str_z_filename,0) == OK) 
       {
        //備份ZDT帳
        str_zdt_path = BackupFile(str_z_filename, "", DDZ_1099);
		tif_tmp.WriteString("Z", "x1098filename", str_zdt_path); // tif_tmp->WriteString("Z", "x1098filename", str_zdt_path);
       }
    else
      {
		tif_tmp.WriteString("Z", "x1098filename", "");   //tif_tmp->WriteString("Z", "x1098filename", "");
      }


    /////////////////////////////////////////////////////////////////

    string StrZslFile;
    if (w_WriteSAL(str_sal, SalFileName,0) == OK)
       {
         //備份ZSL帳
         sTmp = _ExtractFile(SalFileName);
		 _Strsprintf(StrZslFile, "%s%s_%s%s", XZDATASAL, str_sys_date.substr(1, 8).c_str(), DDZ_1099.c_str(), sTmp.c_str());
		 if (CopyFile(StringToWString(SalFileName).c_str(), StringToWString(StrZslFile).c_str(), false))
           {
			   logsprintf("ZDATA:: 備份SAL %s TO %s", SalFileName.c_str(), StrZslFile.c_str());
           }
         else
           {
			   logsprintf("ZDATA:: 備份SAL %s TO %s 失敗!!", SalFileName.c_str(), StrZslFile.c_str());
           }

		 StrZslFile = BackupFile(SalFileName, _ChangeFileExt(str_zdt_path, string(".zsl")), DDZ_1099);
		 tif_tmp.WriteString("Z", "ZSLNAME", StrZslFile);   // tif_tmp->WriteString("Z", "ZSLNAME", StrZslFile);
		 sTmp = _ChangeFileExt(StrZslFile, string(".asl"));
		 //_ListToFile(Auto_Sal, sTmp.c_str() ); // ->SaveToFile(sTmp);
         _ListToWFile(Auto_Sal, sTmp);

         writelog("ZDATA::備份載入之SAL Auto_Sal->SaveToFile=>" + sTmp);

       }
    else
       {
		tif_tmp.WriteString("Z", "ZSLNAME", "");  //tif_tmp->WriteString("Z", "ZSLNAME", "");
       }

    string str_ipt_path;

    //更改XZDATA.INI
    int int_nzno_seq   = _StrToInt(str_pzno_seq, "pzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

	tif_tmp.WriteString("Z", "ZFILENAME", str_zdt_path);   //tif_tmp->WriteString("Z", "ZFILENAME", str_zdt_path);
	tif_tmp.WriteString("Z", "ZRPTNAME", str_zrp_path);   //tif_tmp->WriteString("Z","ZRPTNAME"  , str_zrp_path);
	tif_tmp.WriteString("Z", "DEPTNAME", str_dpt_path);   //tif_tmp->WriteString("Z","DEPTNAME"  , str_dpt_path);
	tif_tmp.WriteString("Z", "INQTNAME", str_ipt_path);   //tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

	tif_tmp.WriteString("Z", "PZ_DATE", str_z_date);   //tif_tmp->WriteString("Z","PZ_DATE"     , str_z_date);
	tif_tmp.WriteString("Z", "AM_TCLSACC", int2str(int_am_tclsacc));   //tif_tmp->WriteString("Z","AM_TCLSACC" , IntToStr(int_am_tclsacc)) ;
	tif_tmp.WriteString("Z", "ZfileTranCnt", _str_tran_cnt);   //tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT 交易序號

	tif_tmp.WriteString("Z", "tot_open_z", "0");   //tif_tmp->WriteString("ECR","tot_open_z", "0");
	tif_ecr.WriteString("Z", "tot_open_z", "0");   //tif_ecr->WriteString("ECR","tot_open_z", "0");

	tif_tmp.WriteString("VXZ", "NZNO_SEQ", "1");   //tif_tmp->WriteString("VXZ", "NZNO_SEQ", "1");
	tif_ecr.WriteString("VXZ", "NZNO_SEQ", "1");   //tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");

	tif_tmp.WriteString("VXZ", "PZ_DATE", "");   //tif_tmp->WriteString("VXZ", "PZ_DATE", "");
	tif_ecr.WriteString("VXZ", "PZ_DATE", "");   //tif_ecr->WriteString("VXZ", "PZ_DATE", "");


	tif_tmp.WriteString("AUTOZ", "LST_AUTOZ_DTTM", str_z_date.substr(0, 8));   //tif_tmp->WriteString("AUTOZ","LST_AUTOZ_DTTM",str_z_date.SubString(1,8)); //AutoZ_Dt);   //前次自動日結時間
    //tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM","");  //現再自動日結時間無作用
	tif_tmp.WriteString("AUTOZ", "LST_ZCNT_DTTM", str_z_date.substr(0, 8));   //tif_tmp->WriteString("AUTOZ","LST_ZCNT_DTTM",str_z_date.SubString(1,8));   //換日參考日


    if (bl_change_day) //判斷是否換日
    {
        //sLog.sprintf("ZDATA::日結換日  pz_date=%s, z_date=%s",str_pz_date, str_z_date);
        //writelog( sLog);

		tif_ecr.WriteString("Z", "PZNO_SEQ", "0"); // tif_ecr->WriteString("Z", "PZNO_SEQ", "0");
		tif_ecr.WriteString("Z", "NZNO_SEQ", "1"); //tif_ecr->WriteString("Z","NZNO_SEQ", "1");
		tif_ecr.WriteString("Z", "PZ_CNT", str_nz_cnt); // tif_ecr->WriteString("Z", "PZ_CNT", str_nz_cnt);
        if (++int_nz_cnt > 99999)
            {
			tif_ecr.WriteString("Z", "PZ_CNT", "00000"); // tif_ecr->WriteString("Z","PZ_CNT", "00000");
             int_nz_cnt = 1;
            }

        _Strsprintf(str_nz_cnt,"%05d",int_nz_cnt);
		tif_ecr.WriteString("Z", "NZ_CNT", str_nz_cnt); // tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
		tif_ecr.WriteString("Z", "DATEZ", (str_z_date.substr(6, 2) + "1")); // tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
		tif_ecr.WriteString("Z", "YYYYMMDDZ", (str_z_date.substr(0, 8) + "0100")); // tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));
		tif_ecr.WriteString("Z", "monthz", currentDateTime(1).substr(4, 2)); // tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        sLog="ZDATA::換日處理 DATEZ=" + str_z_date.substr(6,2)+ "1" +  ", YYYYMMDDZ=" + str_z_date.substr(6,2) + "0100";
        writelog( sLog);

    }
    else
    {

		tif_ecr.WriteString("Z", "PZNO_SEQ", str_pzno_seq); // tif_ecr->WriteString("Z", "PZNO_SEQ", str_pzno_seq);

        if (++int_nzno_seq > 9)
           {
			tif_ecr.WriteString("Z", "PZ_CNT", "00000"); //tif_ecr->WriteString("Z","PZ_CNT", "00000");
            int_nzno_seq = 1;
           }

        _Strsprintf(str_pzno_seq,"%d", int_nzno_seq);

		tif_ecr.WriteString("Z", "NZNO_SEQ", str_pzno_seq); //tif_ecr->WriteString("Z","NZNO_SEQ", str_pzno_seq);
		tif_ecr.WriteString("Z", "PZ_CNT", str_nz_cnt); //tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);

        if (++int_nz_cnt > 99999)
            int_nz_cnt = 1;

		_Strsprintf(str_nz_cnt,"%05d", int_nz_cnt);

		tif_ecr.WriteString("Z", "NZ_CNT", str_nz_cnt);  //tif_ecr->WriteString("Z", "NZ_CNT", str_nz_cnt);
		tif_ecr.WriteString("Z", "DATEZ", str_z_date.substr(6, 2) + int2str(int_nzno_seq));  //tif_ecr->WriteString("Z", "DATEZ", (str_z_date.SubString(7, 2) + IntToStr(int_nzno_seq)));
		
		_Strsprintf(sTmp, "%s%02d00", str_z_date.substr(0, 8).c_str(), int_nzno_seq);
		tif_ecr.WriteString("Z", "YYYYMMDDZ", sTmp);  //tif_ecr->WriteString("Z", "YYYYMMDDZ", (str_z_date.SubString(1, 8) + FormatFloat("00",int_nzno_seq) + "00"));
		
		tif_ecr.WriteString("Z", "monthz", currentDateTime(1).substr(4, 2)); // FormatDateTime("mm", Now()));

        //sLog.sprintf("ZDATA::日結不換日  pz_date=%s, z_date=%s, SET NZNO_SEQ=%s",str_pz_date, str_z_date, str_pzno_seq);
        //writelog( sLog);

		sLog = "ZDATA::未換日處理 DATEZ=" + str_z_date.substr(6, 2) + _IntToStr(int_nzno_seq) + ", YYYYMMDDZ=" + sTmp;
        writelog( sLog);


    }

    logsprintf("ZDATA::寫入 XZDATAINI [AUTOZ]->LST_AUTOZ_DTTM=%s, [AUTOZ]->LST_AUTOZ_DTTM=%s, [AUTOZ]->LST_ZCNT_DTTM=%s, [Z]->NZ_CNT=%s",
		str_z_date.substr(0, 8).c_str(), str_z_date.substr(0, 8).c_str(), str_z_date.substr(0, 8).c_str(), str_nz_cnt.c_str());

    str_pz_date    = str_z_date;
	str_pzno_seq = tif_ecr.ReadString("Z", "NZNO_SEQ", "");   //tif_ecr->ReadString("Z", "NZNO_SEQ", "");      //目前Z帳次數   換日歸1

    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";


    //指定X 帳PATH
    string str_x_path = write_xpath((str_pz_date.substr(0,8) + "0" + str_pzno_seq + ".zdt" ));
	tif_ecr.WriteString("Z", "XPATH", str_x_path);    //tif_ecr->WriteString("Z", "XPATH", str_x_path);

	tif_ecr.WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);  //tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印
    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

	_Strsprintf(str_sum_tran,"%05d", int_sum_tran_cnt);

	tif_ecr.WriteString("ECR", "TRAN_CNT", str_sum_tran);  //tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    // 自動日結 不將X帳歸零    _WZ_OK 會規0
	tif_ecr.WriteString("X", "PTRAN_CNT", "00000000");  //tif_ecr->WriteString("X", "PTRAN_CNT", "00000000");
	tif_ecr.WriteString("X", "SX_DATE", "");  //tif_ecr->WriteString("X", "SX_DATE"  , "");
	tif_ecr.WriteString("X", "EX_DATE", "");  //tif_ecr->WriteString("X", "EX_DATE"  , "");
    //自動日結不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理
	tif_ecr.WriteString("X", "PXNO_SEQ", "");  //tif_ecr->WriteString("X", "PXNO_SEQ" , "0");


    logsprintf("ZDATA::寫入 XZDATAINI Z->NZ_CNT=%s, AM_TCLSACC=%d, ECR->TRAN_CNT=%s, XPATH=%s, 日結 -> 將X帳歸零",
		str_nz_cnt.c_str(), int_am_tclsacc, str_sum_tran.c_str(), str_x_path.c_str());

    return OK;


}


//XREPORT::WriteData() X帳表, 讀帳  NextPOS
int __fastcall XREPORT::WriteData( const string StoreNO, const string EcrNO, const string SalesNo ,
                                   const string SalFileName, const string Version)
{
    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    string sLog;
	logsprintf("X帳表 XREPORT::WriteData(%s, %s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str());
    //if (!int_reslut)
    //    return 0;

    string str_ver_num, sTmp;

	//sprintf_s(gchar_tencode, sizeof(gchar_tencode),"%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", SalesNo.c_str());
    g_cashier = SalesNo;

    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號

    str_ver_num = get_version(Version);

    SPOS_ZRPDT *xr = new SPOS_ZRPDT;       //Z帳結構

	//目前Z帳時間
    string str_z_date;         //現在時間
    string str_pz_date;        //前次時間

    string str_date_time;
    str_date_time = g_date_time;
    str_z_date  = str_date_time;
	str_pz_date = tif_ecr.ReadString("Z", "PZ_DATE", "");  

    if (str_pz_date.length() < 6)
        str_pz_date = str_z_date;

    xr->no_tm = g_ecr_no;        //收銀機機號

    //日結序號
    string str_nz_cnt, sYYMMDDZZ, str_tran_cnt;
	str_nz_cnt = tif_ecr.ReadString("Z", "nz_cnt", "00001");  //tif_ecr->ReadString("Z", "nz_cnt", "00001");

    _Strsprintf(xr->bg_noclose,"%05d", _StrToInt(str_nz_cnt,"nz_cnt"));

	string  str_pzno_seq = tif_ecr.ReadString("Z", "NZNO_SEQ", "1");     //目前Z帳次數 01~09  換日歸1
    //str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999
    //str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零

	sTmp = tif_ecr.ReadString("ECR", "TRAN_CNT", "00001");              //目前交易序號,但SAL最後一筆須檢1
 	_Strsprintf(str_tran_cnt,"%05d", _StrToInt(sTmp) - 1>0 ? _StrToInt(sTmp) - 1 : 1);

	logsprintf("XREPORT::ECR->TRAN_CNT 取的目前之交易序號:%s", str_tran_cnt.c_str());

    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";

	_Strsprintf(sYYMMDDZZ,"%s%02d", str_pz_date.substr(2, 6).c_str(), _StrToInt(str_pzno_seq));

    xr->bg_dtclose = str_z_date.substr(0,8);      //結帳日期 = 現在日期 
	xr->bg_tmclose = str_z_date.substr(8, 6);     //結帳時間 = 現在時間
	xr->ed_dtclose = str_pz_date.substr(0, 8);    //前次結帳日期
	xr->ed_tmclose = str_pz_date.substr(8, 6);    //前次結帳時間

    string xr_a;
    string z_a;
    int iC0TotCount=0;
    int iSumQt_trans = 0;
	_Strsprintf(xr_a,"3096||%04d||%6s||%2s||%14s||%5s||%-10s||%8s||%6s||%8s||%6s||", 189,
                                      g_tencode.c_str(),
                                      xr->no_tm.c_str(),           //收銀機機號
                                      str_date_time.c_str(),      //現在時間
                                      xr->bg_noclose.c_str(),      //日結序號
									  sYYMMDDZZ.c_str(),                   //Z帳表序號
                                      xr->ed_dtclose.c_str(),      //前次結帳日期
                                      xr->ed_tmclose.c_str(),      //前次結帳時間
                                      xr->bg_dtclose.c_str(),      //結帳日期
                                      xr->bg_tmclose.c_str()      //結帳時間
                                    );


    //交易明細ｆｉｅｌｄ=============================================

    //本日來客數/本日交易品項  SumQt_trans() ==> 併入 SumQt_tcust  NextPOS
	_Strsprintf(xr->tb_trans.qt_tcust,"%08d", SumQt_tcust(0, gi_tot_line));
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項
   
    //Sub_C0C1TaxInvoQty(0, gi_rec_line) ==> 併入 Sub_C0C1TaxInvoAmt;   點卡手續費數量 NextPOS 
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);  
	//_Strsprintf(xr->tb_trans.qt_trans,"%08d", SumQt_trans(0, gi_tot_line) - giC);
    _Strsprintf(xr->tb_trans.qt_trans, "%08d", iSumQt_trans - giI);   // NextPOS 本日交易品項

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;

    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    // Sub_C0C1TaxInvoAmt(0, gi_rec_line);             // NextPOS 
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);             // NextPOS    
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

     //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);      //  NextPOS  
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    //int iPreSalINVODisAmt ;
    //SumPreSal_INVO_DisAmt(0, gi_rec_line);
    //iPreSalINVODisAmt = giU;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line);    //1050==> R4+Z0   //  NextPOS 
    TotMiss(0, gi_tot_line);    //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
    _Strsprintf(xr->tb_trans.am_trcpntx,"%010d", giI - C01FTaxAmt - R4_C01FTaxAmt - (PreSal09_AmtNTX + R4PreSal09_AmtNTX));      //發票日結帳額(免)
	_Strsprintf(xr->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));      //發票日結帳額(稅)
	_Strsprintf(xr->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));      //發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ); // + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    logsprintf("XREPORT::WriteData(%s, %s, %s)",
		xr->tb_trans.am_trcpntx.c_str(), xr->tb_trans.am_trcptax.c_str(), xr->tb_trans.am_trcptot.c_str());

    //本日結帳累計金額
    string str_am_tclsacc;     //累積Z帳金額
    int int_acc_tot_amt;
    str_am_tclsacc = tif_ecr.ReadString("Z","AM_TCLSACC","");      //換日不歸零

    if (str_am_tclsacc.length() < 1)
        str_am_tclsacc = "0";

    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));

	_Strsprintf(xr->tb_trans.am_tclsacc,"%010d", int_acc_tot_amt);
    // xr->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
    // TotMiss(0,gi_tot_line);       //1050 ==> R2R3R4  NextPOS  
    // RC01FTaxAmt=giC; RC01TaxAmt=giD;
	_Strsprintf(xr->tb_trans.qt_miss,"%08d", giA);        // 誤打發票次數   1050 ==> R2R3R4
	_Strsprintf(xr->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        // 誤打發票金額(免)
	_Strsprintf(xr->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        // 誤打發票金額(稅)
	_Strsprintf(xr->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        // 誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX);


    //不開發票金額 NextPOS  
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);  //  NextPOS  
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
	_Strsprintf(xr->tb_trans.am_totsale,"%010d", (int_tot_amt - int_miss_amt + int_novoice_amt - int_novoice_miss_amt));

    //發票開始號碼
    //發票結束號碼 NextPOS  
    if (gbl_tot_haveline)
    {
        xr->no_rcstart = invoice_start_end(gi_tot_line, true);
        xr->no_rcend = invoice_start_end(gi_tot_line, false);
    }
    else
    {
        xr->no_rcstart = "0000000000";
        xr->no_rcend   = "0000000000";
    }

    string xr_b;

    xr_b += (xr->tb_trans.qt_tcust+"||");             //本日來客數
    xr_b += (xr->tb_trans.qt_trans+"||");             //本日交易品項
    xr_b += (xr->tb_trans.am_tclsacc+"||");          //本日結帳累計金額

    xr_b += (xr->no_rcstart+"||");                   //發票開始號碼
    xr_b += (xr->no_rcend+"||");                      //發票結束號碼
    xr_b += (xr->tb_trans.am_trcpntx+"||");           //發票日結帳額(免)
    xr_b += (xr->tb_trans.am_trcptax+"||");          //發票日結帳額(稅)
    xr_b += (xr->tb_trans.am_trcptot+"||");           //發票日結帳金額

    xr_b += (xr->tb_trans.qt_miss+"||");             //誤打發票次數
    xr_b += (xr->tb_trans.am_missntx+"||");           //誤打發票金額(免)
    xr_b += (xr->tb_trans.am_misstax+"||");           //誤打發票金額(稅)
    xr_b += (xr->tb_trans.am_miss+"||");              //誤打發票金額
    xr_b += (xr->tb_trans.am_totsale+"||");           //本日實際銷貨收入   24


    //代售付ｔａｂｌｅ===========================================

    int iRentAmt, iRemtCnt;
    //代收  NextPOS  
    SumKamt(0,gi_tot_line);                 //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;                 //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    //NextPOS 
    sub_times(0,gi_rec_line);   //NextPOS giH giI 食安退貨金額、次數   //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(xr->tb_newadd.qt_agnrcv,"%08d", giD);               //代收費用次數
	_Strsprintf(xr->tb_newadd.am_agnrcv,"%010d", giC);               //代收費用金額
	
	if (!DirExists("C:\\FTLPOS\\HISTORY\\"))
		SHCreateDirectory(NULL, L"C:\\FTLPOS\\HISTORY\\");  //ForceDirectories("C:\\FTLPOS\\HISTORY\\");

    //產生代售報表 2011/05/30  
	strcpy_s(report->char_filename, sizeof(report->char_filename), "C:\\FTLPOS\\HISTORY\\subsale.rpt");
   
    report->CreateReport("s", " ", StrBillPay, g_tencode , g_ecr_no, 0,0,0,0,tsl_subsale, 0, "");

   // int CreateReport(string rpt_type, string sSal, string sPayStr,
   //      char* ptr_tencode, char* ptr_ecr_no,
   //      int iqt5_15217totrev = 0, int iam5_15217totrev = 0,
   //      int iqt5_18219totrev = 0, int iam5_18219totrev = 0,
   //      TStringList * tsl = NULL, int AutoZflg = 0, string s1093 = NULL);

    //NextPOS 
    // Sub_C0InvoAmt(0,gi_rec_line);        //NextPOS  外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giH;  iSub_C0InvoAmtC=giJ;  iSub_C0InvoQtyC=giK;

    string xr_c;
    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)           //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


   

    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    //NextPOS
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    // Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //代售合計次數  代售手續費商品合計回數
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //代售合計金額  代售手續費商品合計金額 無須加入預售商品
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

    //25~28  代售手續費商品合計回數, 代售手續費商品合計金額, 代付商品合計回數, 代付商品合計金額
	_Strsprintf(xr_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

   //////////////////////////////////////////////////////////////////////////////


    // 03 代現金ｔａｂｌｅ=============================================
    int iqt_ffu1,iam_ffu1,iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;

    //NextPOS
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    //NextPOS
    SumTbCash(0,gi_tot_line);  //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額


    //預售類(折價券S0 10/MMS0 11 )折扣 NextPOS
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣 NextPOS
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;


    int iCSMAmt=0;//消費券 NextPOS
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

	_Strsprintf(xr->tb_cash.qt_bonus,"%08d", giA); //-giT);               //禮券回數
	_Strsprintf(xr->tb_cash.am_bonus,"%010d", giB); //-iCSMAmt);               //禮券金額

	_Strsprintf(xr->tb_cash.qt_vndcpn,"%08d", 0);                //廠商代現金回數
	_Strsprintf(xr->tb_cash.am_vndcpn,"%010d", 0);                //廠商代現金金額

	_Strsprintf(xr->tb_cash.qt_stocpn,"%08d", giC);              //門市代現金回數
	_Strsprintf(xr->tb_cash.am_stocpn,"%010d", giD); //+iPreSalCPNDisAmt );              //門市代現金金額  折價券

	_Strsprintf(xr->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));     //代現金合計回數
	_Strsprintf(xr->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));     //代現金合計金額

    //xr->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));     //代現金合計回數
    //xr->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));     //代現金合計金額

	_Strsprintf(xr->tb_cash.qt_bonout,"%08d", giG);              //溢收合計回數
	_Strsprintf(xr->tb_cash.am_bonout,"%010d", giH);              //溢收合計金額

    iTotOt2Rtn=giE;                                       //餘額券
    iqt_ffu1=giI;                                         //現金卡回數   電子錢回數
    iam_ffu1=giJ;                                         //現金卡金額   電子錢金額   刷卡合計

    string xr_d;

    xr_d += (xr->tb_cash.qt_bonus+"||");               //禮券回數      29
    xr_d += (xr->tb_cash.am_bonus+"||");                //禮券金額     30
    xr_d += (xr->tb_cash.qt_vndcpn+"||");               //廠商折價券回數   31
    xr_d += (xr->tb_cash.am_vndcpn+"||");               //廠商折價券金額   32
    xr_d += (xr->tb_cash.qt_stocpn+"||");               //折價券回數       33
    xr_d += (xr->tb_cash.am_stocpn+"||");               //折價券金額       34
    xr_d += (xr->tb_cash.qt_totcash+"||");              //代現金合計回數   35
    xr_d += (xr->tb_cash.am_totcash+"||");              //代現金合計金額   36

	_Strsprintf(sTmp,"%08d", giF);
    xr_d += (sTmp+"||");                               //餘額合計回數      37
	_Strsprintf(sTmp,"%08d", giE);
    xr_d += (sTmp+"||");                               //餘額合計金額      38

    xr_d += (xr->tb_cash.qt_bonout+"||");                 //溢收合計回數       39
    xr_d += (xr->tb_cash.am_bonout+"||");;                //溢收合計金額       40

    //==========================================================================

    //現金合計 含 消費券 NextPOS
    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(xr->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);

    //sLog.sprintf("XREPORT::現金合計(%s)",xr->am_cash);
    //writelog( sLog);


    //char xre[29];
    //xre[0] = 0;

   //strcpy(xre, xr->am_cash.c_str());         //現金合計
    //strcat(xre, xr->no_rcstart.c_str());      //發票開始號碼
    //strcat(xre, xr->no_rcend.c_str());        //發票結束號碼
    //xre[28] = 0;


    // Z帳部門別集計TABLE      =============================================
    string xr_f;

    int s1, s2, s3, s4;
    s1 = s2 = s3 = s4 = 0;

    DEPT *dept;

    try
    {

        dept = new DEPT[20];

        //先將值填零
        for (int i=0; i<20; i++)
        {
			_Strsprintf(xr->tb_depcal[i].am_dpsitm,"%08d", 0);
			_Strsprintf(xr->tb_depcal[i].am_dpsale,"%010d", 0);
			_Strsprintf(xr->tb_depcal[i].am_dpmitm,"%08d", 0);
			_Strsprintf(xr->tb_depcal[i].am_dpmiss,"%010d", 0);
            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //計算所有部門交易資料, 部門帳 NextPOS
        if (gbl_rec_haveline)
            SumAllDept(dept);

        for (int i=0; i<20; i++)
        {
			_Strsprintf(xr->tb_depcal[i].am_dpsitm,"%08d", dept[i].am_dpsitm);
            xr_f += (xr->tb_depcal[i].am_dpsitm+"||");
            s1 += dept[i].am_dpsitm;

			_Strsprintf(xr->tb_depcal[i].am_dpsale,"%010d", dept[i].am_dpsale);
            xr_f += (xr->tb_depcal[i].am_dpsale+"||");
            s2 += dept[i].am_dpsale;

			_Strsprintf(xr->tb_depcal[i].am_dpmitm,"%08d", dept[i].am_dpmitm);
            xr_f += (xr->tb_depcal[i].am_dpmitm+"||");
            s3 += dept[i].am_dpmitm;

			_Strsprintf(xr->tb_depcal[i].am_dpmiss,"%010d", dept[i].am_dpmiss);
            xr_f += (xr->tb_depcal[i].am_dpmiss+"||");
            s4 += dept[i].am_dpmiss;
        }
    }
	catch (...)
    {
		;;
    }
	delete[] dept;


    string xr_g;

	xr_g += (_Strsprintf(xr->am_tdpsitm,"%08d", s1) + "||");     //部門合計交易項數 41
	xr_g += (_Strsprintf(xr->am_dpstot,"%010d", s2) + "||");     //部門合計銷售金額 42
	xr_g += (_Strsprintf(xr->am_tdpmitm,"%08d", s3) + "||");     //部門合計誤退項數 43
	xr_g += (_Strsprintf(xr->am_dpmtot,"%010d", s4) + "||");     //部門合計誤退金額 44
    xr_g +=  (xr->am_cash+"||");                           //現金合計含消費券 45


    //收銀員明細ｆｉｅｌｄ =============================================

    string xr_h;

	xr_h += (_Strsprintf(xr->tb_casher.qt_vendtot,"%08d", 0) + "||");         // 借零次數      46
    //sTmp.sprintf("%08d",VisuilCpnAmt);                                   //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    //xr_h += (sTmp+"|");

    //xr_h += (xr->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");         // 借零合計金額  47
	_Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    xr_h += (sTmp+"||");                           // 代收商品折讓單折扣  借零合計金額

	xr_h += (_Strsprintf(xr->tb_casher.qt_feedtot,"%08d", 0) + "||");         // 投零次數      48
	xr_h += (_Strsprintf(xr->tb_casher.am_feedtot,"%010d", 0) + "||");         // 投零合計金額  49

    //投庫  NextPOS
    string x_h;

    int int_drop_line = (gtsl_drop->size() - 1);
    int int_drops = 0;                           //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                    //投庫總額
    int x_rtn_cnt=0;

    string   s1041;
    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
		s1041 = *ls; // gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
             logsprintf("產生X帳表:非投庫(%s)資料",s1041.c_str());
             continue;
           }


        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, SGM, 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //IC付款  FU_SPACE10
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //信用卡  FU_SPACE10 
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 12));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 13));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //提貨單  FU_SPACE10 
        
        //中獎發票金額 500
		int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 16));

        //中獎發票金額 1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 17));

        //中獎發票金額 200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 15));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 18)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 20)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 22)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 24)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 26)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 28)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 30)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 32)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 34)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 36)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 38)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 40)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 42)); //費用 13金額   食安退款金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 44)); //費用 14金額   提貨券金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數

    }

	xr_h += (_Strsprintf(xr->tb_casher.qt_droptot,"%08d", int_drops) + "||");          //投庫次數    50
	xr_h += (_Strsprintf(xr->tb_casher.am_droptot,"%010d", int_drop_sum) + "||");   //投庫合計金額    51

    //更正次數、金額  NextPOS
    SumUpdat(0,gi_rec_line);
	xr_h += (_Strsprintf(xr->tb_casher.qt_update,"%08d", giA) + "||");            //更正次數         52
	xr_h += (_Strsprintf(xr->tb_casher.am_update,"%010d", giB) + "||");            //更正合計金額     53

    //交易取消  NextPOS
    SumVoid(0,gi_tot_line);  
	xr_h += (_Strsprintf(xr->tb_casher.qt_void,"%08d", giA) + "||");               // 交易取消次數     54
	xr_h += (_Strsprintf(xr->tb_casher.am_void,"%010d", giB) + "||");               // 交易取消合計金額 55

    //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {

        int_ZINI_TRAN_MODE_CNT = stoi(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_CNT", "0"));
        int_ZINI_TRAN_MODE_AMT = stoi(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_AMT", "0"));
        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

         //   tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
         //   tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

    }
    catch(...) 
    {
          ;;
    }

    
	xr_h += (_Strsprintf(xr->tb_casher.qt_train,"%08d", int_ZINI_TRAN_MODE_CNT) + "||"); // 訓練合計次數  56
	xr_h += (_Strsprintf(xr->tb_casher.am_train,"%010d", int_ZINI_TRAN_MODE_AMT) + "||"); // 訓練合計金額  57


    //手輸部門加總13、14  NextPOS
    SumInpdp(0,gi_rec_line);
	xr_h += (_Strsprintf(xr->tb_newadd.qt_inpdp,"%08d", giA) + "||");             //手動輸入部門次數  58
	xr_h += (_Strsprintf(xr->tb_newadd.am_inpdp,"%010d", giB) + "||");            //手動輸入部門金額   59

	xr_h += (_Strsprintf(xr->tb_casher.qt_openbox,"%08d", gi_open_z) + "||");      // 開錢櫃次數      60

    //折扣次數   //小計折扣 NextPOS  
    //SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	xr_h += (_Strsprintf(xr->tb_casher.qt_disc,"%08d", giC) + "||");             // 折扣次數(應稅)   61
	xr_h += (_Strsprintf(xr->tb_casher.am_disc,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + "||");   // 折扣金額(應稅)   62

    //NextPOS
    //pay_sub_amt(0,gi_pay_line); 
	xr_h += (_Strsprintf(xr->tb_newadd.qt_ffu6,"%08d", giA) + "||");             //促銷折扣/讓次數   63
	xr_h += (_Strsprintf(xr->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + "||");             //促銷折扣/讓金額   64


    //加值機  NextPOS  
    //TM一卡通加值(支付)明細電文檔(3432)
    //TM遠鑫加值(支付)明細電文檔 (3234)
    //TM二代悠遊卡加值 二代悠遊加值明細 3238
    easy_card(0,gi_easy_line);
	xr_h += (_Strsprintf(xr->tb_casher.qt_disret1,"%08d", giA) + "||");            // 加值機開/售卡次數  65
	xr_h += (_Strsprintf(xr->tb_casher.am_disret1,"%010d", giB) + "||");            // 加值機開/售卡金額  66
	xr_h += (_Strsprintf(xr->tb_casher.qt_disret2,"%08d", giC) + "||");            // 加值機加值次數     67
	xr_h += (_Strsprintf(xr->tb_casher.am_disret2,"%010d", giD) + "||");            // 加值機加值金額     68

    //fetc_card(0,gi_fetc_line);   //e通卡  // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;

   
	xr_h += (_Strsprintf(xr->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + "||");    //備用八次數  現金 加值次數   69   2015/08/19 代售,手續費銷售金額(免)
	xr_h += (_Strsprintf(xr->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + "||");  //備用八金額  現金 加值金額   70   2015/08/19 代售,手續費作廢金額(免)

    //食安次數、金額  int iqt_food,iamt_food;
	xr_h += (_Strsprintf(xr->tb_newadd.qt_ffu9,"%08d", iqt_food) + "||");  //備用九次數  信用卡 加值次數 71   2015/08/19 0
	xr_h += (_Strsprintf(xr->tb_newadd.am_ffu9,"%010d", iamt_food) + "||");  //備用九金額  信用卡 加值金額 72   2015/08/19 0

    
    //追加 table  =============================================

    string xr_i;
	xr_i += (_Strsprintf(xr->tb_newadd.qt_cashCard,"%08d", iqt_ffu1) + "||");         //刷卡/現金卡次數  73
	xr_i += (_Strsprintf(xr->tb_newadd.am_cashCard,"%010d", iam_ffu1) + "||");         //刷卡/現金卡金額  74
	xr_i += (_Strsprintf(xr->tb_newadd.qt_parking,"%08d", 0) + "||");           //代收停車費次數         75
	xr_i += (_Strsprintf(xr->tb_newadd.am_parking,"%010d", 0) + "||");           //代收停車費金額 ,不開發票金額誤退重  76
	xr_i += (xr->tb_newadd.qt_agnrcv + "||");                              //代收費用次數     77
	xr_i += (xr->tb_newadd.am_agnrcv + "||");                              //代收費用金額     78
	xr_i += (_Strsprintf(xr->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + "||");    //版本前置符號      79
	xr_i += (_Strsprintf(xr->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + "||");    //版本號碼          80
	xr_i += (_Strsprintf(xr->tb_newadd.qt_rcp0,"%010d", TotZero(0, gi_tot_line)) + "||");  // 發票金額0次數

    //不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);
	xr_i += (_Strsprintf(xr->tb_newadd.am_ffu4,"%010d", int_novoice_amt) + "||");        //82 備用四金額 , 不開發票金額
	xr_i += (_Strsprintf(xr->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + "||");   //83 加值機結帳金額,不開發票金額誤退重

    // 廢棄 3110 NextPOS
    SumLOSS(0,gi_rec_line);
	xr_i += (_Strsprintf(xr->tb_newadd.qt_ffu5,"%08d", giA) + "||");             //84 TM廢棄Loss值次數
	xr_i += (_Strsprintf(xr->tb_newadd.am_ffu5,"%010d", giB) + "||");             //85 TM廢棄Loss值金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計
	xr_i += (_Strsprintf(xr->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + "||");     //86 原加值機加值取消金額

    //即時購代售 NextPOS
    SumRelDimAgn(0, gi_rec_line); 
    int iSumRelDimAgn=giB;

    //xr_i += (xr->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	xr_i += (_Strsprintf(xr->tb_newadd.qt_ffu7,"%010d", iSumRelDimAgn + iC0InvoAmt4VDCN) + "||");   //87 原加值機加值取消次數

	xr_i += (_Strsprintf(xr->tb_newadd.am_posZRpt,"%010d", iCSMAmt) + "||");           //88 扣卡機扣卡金額  消費券


    string xr_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值 NextPOS
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    169 即時購與銷轉進值應稅
    xr_k +=sTmp;   //"00000000|";
	_Strsprintf(sTmp,"%010d||", giD);     // 170 即時購與銷轉進值免稅
    xr_k +=sTmp;

    //發票兌獎金額 NextPOS
    VoicePayAmt(0,gi_tot_line);
	_Strsprintf(sTmp,"%08d||", giA / 200); //171發票兌獎張數
    xr_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", giA);     //172發票兌獎金額
    xr_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);  //173 電子禮券回數
    xr_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //174 電子禮券金額
    xr_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //代售手續費應稅金額 giD.  代售手續費應稅作廢金額 giE , 代售手續費來客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
	_Strsprintf(sTmp,"%010d||%010d||%08d||%s||%10s||1||2||3||4||5||6||7||8||9||0||", iC0TotAmt, iC0GivUpAmt, iC0TotCount,
       "W",
       "Tran_cnt" );   //175,176,177, 178~189   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    xr_k +=sTmp;

    //xr_i += xr->tb_newadd.qt_ffu4.sprintf("%08d",gi_mix);         //代收來客數

    //xr_i += xr->tb_newadd.qt_posZRpt.sprintf("%08d",0);           //扣卡機扣卡次數

    // xr_h += xr->tb_casher.qt_pack.sprintf("%08d",iTotOt2Rtn); //giE);     // 加值機結帳次數  2007/12/01 找餘額券

    //xr_i += xr->tb_newadd.qt_ffu10.sprintf("%08d",0); //giFetcSumCnt);     //備用十次數  FETC 結帳次數   保留
    //xr_i += xr->tb_newadd.am_ffu10.sprintf("%08d",0); //giFetcSumAmt);     //備用十金額  FETC 結帳金額   保留


    //計算門市提貨券  3240 NextPOS
    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);
     

    //XReport讀帳報表
    string str_xr;
       
	_Strsprintf(str_xr,"%s%s%s%s%s%s%s%s%s\n",
                   xr_a.c_str(),
                   xr_b.c_str(),
                   xr_c.c_str(),
                   xr_d.c_str(),
                   xr_g.c_str(),
                   xr_h.c_str(),
                   xr_i.c_str(),
                   xr_f.c_str(),
                   xr_k.c_str()
                   );

    //report = new REPORT();      //讀帳帳表

    logsprintf("XREPORT::(%s)Len=%d",str_xr.c_str(), str_xr.length());

	if (!DirExists("C:\\FTLPOS\\HISTORY\\"))
		SHCreateDirectory(NULL, L"C:\\FTLPOS\\HISTORY\\");  //ForceDirectories("C:\\FTLPOS\\HISTORY\\");

	strcpy_s(report->char_filename, sizeof(report->char_filename), "C:\\FTLPOS\\HISTORY\\XReport.xpp");
   // report->CreateReport('X', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no);

   // 2006/08/03 for 代收票券  NextPOS
   report->CreateReport("X", str_xr, StrBillPay, g_tencode, g_ecr_no,
                        iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev , NULL, 0, "");

    logsprintf("CreateReport: 產生產生讀帳帳表 C:\\FTLPOS\\HISTORY\\XReport.xpp ,代售合計=(%d),(%d)",iqt5_15217totrev,iam5_15217totrev);
        
	strcpy_s(report->char_filename, sizeof(report->char_filename), "C:\\FTLPOS\\HISTORY\\XReport.xdp");
    report->CreateReport("d", str_xr, StrBillPay, g_tencode, g_ecr_no, 0,0,0,0,NULL,0,"" );  //NextPOS
    writelog("產生讀帳帳表(部門) C:\\FTLPOS\\HISTORY\\XReport.xdp");

    // 檢查 3050 及 SAL 3050 筆數是否一致
    int RtnZDTCnts, RtnSALCnts;
    string  strchkfile;
	_Strsprintf(strchkfile,"C:\\FTLPOS\\XZDATA\\SAL\\%s.sal", g_ecr_no.c_str());
    //_GetZDTZSALCntCheck(gtsl_tot, strchkfile, str_nz_cnt, RtnZDTCnts, RtnSALCnts ); NestPOS Mark 

	tif_ecr.WriteString("XREPORT", "XREPORTDATE", str_date_time);    //tif_ecr->WriteString("XREPORT", "XREPORTDATE", str_date_time);
	//tif_ecr.WriteString("XREPORT", "ZDTCnts", int2str(RtnZDTCnts)); // tif_ecr->WriteString("XREPORT", "ZDTCnts", IntToStr(RtnZDTCnts));
	//tif_ecr.WriteString("XREPORT", "SALCnts", int2str(RtnSALCnts)); // tif_ecr->WriteString("XREPORT", "SALCnts", IntToStr(RtnSALCnts));

	delete xr;  // , tif_ecr;
    return 1;
}



string __fastcall ZDATA::write_xpath(string FileName)
{
    string str_file_dd,                //日期
               str_file_num,               //次數
               str_bak_path;                //備份路徑


    string str_his_folder = "C:\\FTLPOS\\HISTORY\\";
	string str_abs_file = _ExtractFile(FileName);   //ExtractFileName(FileName);

    str_file_dd  = str_abs_file.substr(6,2);      //DD
    str_file_num = str_abs_file.substr(8,2);      //01~09

    //備份路徑
    if ( (!str_file_dd.empty()) && (!str_file_num.empty()) )
        str_bak_path = ( str_his_folder + str_file_dd + "\\" + str_file_num + "\\" );
    else
        str_bak_path = str_his_folder;

    //建立目錄
	
	if (!DirExists(str_bak_path))
		SHCreateDirectory(NULL, StringToWString(str_bak_path).c_str()); // ForceDirectories(str_bak_path);

    return str_bak_path;

}


//擔當讀帳同交班
/// <summary> CHECKIN::WriteData
/// 擔當讀帳同交班  報表Class
/// </summary>
/// <param name=""></param>
/// <returns>int</returns>
int __fastcall CHECKIN::WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version)
{
    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName,true, "", "");               //載入SAL檔，將檔案依型態分類

    //if (!int_reslut)
    //    return 0;


    string sLog,sTmp;
	logsprintf("擔當讀帳 CHECKIN::WriteData(%s, %s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str());

    string str_ver_num;
    str_ver_num = get_version(Version);
	//sprintf_s(gchar_tencode, sizeof(gchar_tencode), "%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", SalesNo.c_str());
    g_cashier = SalesNo;
    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號
   
    string str_x_filename,              //XDATA檔名
               str_ptran_cnt,               //前次結帳序號
               str_date_z,                  //DataZ(日期加X帳次數)
               str_tran_cnt,                //開店以來結帳金額
               str_pxno_seq,                //X帳結帳次數
               str_nxno_seq,                //下次結帳數
               str_YYYYMMDDZ,
               str_date_time,
               ed_date_time ,
               str_nz_cnt,
               SaleDataVer;


	str_date_z = tif_ecr.ReadString("Z", "DATEZ", ""); //tif_ecr->ReadString("Z", "DATEZ", "");             //ddn
    //str_tran_cnt = tif_ecr->ReadString("ECR","TRAN_CNT","");      //目前交易序號
	sTmp = tif_ecr.ReadString("ECR", "TRAN_CNT", "00001"); // tif_ecr->ReadString("ECR", "TRAN_CNT", "00001");      //目前交易序號,但SAL最後一筆須檢1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    _Strsprintf(str_tran_cnt,"%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    logsprintf("CHECKIN::ECR->WriteData 取的目前之交易序號:%s", str_tran_cnt.c_str());


	str_YYYYMMDDZ = tif_ecr.ReadString("Z", "YYYYMMDDZ", "201001010000");   //tif_ecr->ReadString("Z", "YYYYMMDDZ", "201001010000");
	str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "00001"); // tif_ecr->ReadString("Z", "NZ_CNT", "00001");          //999999目前日結序號  換日不歸零

    str_date_time = g_date_time;

	str_pxno_seq = tif_ecr.ReadString("X", "pxno_seq", ""); // tif_ecr->ReadString("X", "pxno_seq", "");        //上次x帳累積次數
                                                                                                              
    if ( str_pxno_seq.length() < 2)                                                                                    
        str_pxno_seq = "00";                                                                                     
                                                                                                              
	_Strsprintf(str_nxno_seq,"%02d", (_StrToInt(str_pxno_seq) + 1));   //目前x次到第幾次


    //最多99次X帳
    if ( (_StrToInt(str_nxno_seq) > 99) )                                                                        
        str_nxno_seq = "01";                                                                                     
                                                                                                              
    x->no_tm = g_ecr_no;                           //收銀機機號
    x->no_casher = g_cashier;                      //收銀員號碼

    if (!Trim(x->no_casher).length())
        x->no_casher = "00001";
                                                                                                              
    //日結序號                                                                                                
    x->no_seq = str_nxno_seq;

    string str_x_first, str_sal_last, str_sal_last_1099;        //*.sal 最後一筆
    str_x_first = gsalX_first;                                  //gpchar_x_first;
    str_sal_last = gsal_last;                                   //gpchar_sal_last;
    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

    if (str_x_first != "")
    {
        sTmp = _StringSegment_EX(str_x_first, SGM, 5);
        x->dt_begin = sTmp.substr(0, 8);    //資料版本 str_x_first.substr(20,8);   //開始日期
        x->tm_begin = sTmp.substr(8, 6);   //開始時間
    }
    else
    {
        x->dt_begin = "00000000";
        x->tm_begin = "000000";
    }


    string str_invo_char;                    //發票字軌
	str_invo_char = tif_ecr.ReadString("ECR", "INVO_CHAR", "XX");   //tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.length() < 2)
        str_invo_char = "XX";
                                                                                                              
    if (gbl_tot_haveline)
    {                                                                                                         
        //發票開始號碼、發票結束號碼                                                                          
        x->no_rcstart = invoice_start_end(gi_tot_line, true);
        x->no_rcend   = invoice_start_end(gi_tot_line, false);
    }                                                                                                         
    else                                                                                                      
    {                                                                                                         
        x->no_rcend   = "0000000000";
        x->no_rcstart = "0000000000";                                                                         
    }

    //X報表輸出檔名                                                                                           
    str_x_filename = "C:\\FTLPOS\\HISTORY\\checkin.xdt";
                                                                                                              
    //結束時間目前時間為目前結帳時間                                                                          
    x->dt_end = str_date_time.substr(0,8);
    x->tm_end = str_date_time.substr(8,6);

    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000"))
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }
                                                                                                              
    string x_a;
    
    _Strsprintf(x_a,"3095||%04d||%-6s||%-2s||%14s||%5s||%-10s||%2s||%5s||%8s||%6s||%8s||%6s||",102,
                                        g_tencode.c_str(),
                                        x->no_tm.c_str(),         //收銀機機號 4
                                        ed_date_time.c_str(),
                                        str_nz_cnt.c_str(),
										str_YYYYMMDDZ.substr(2, 8).c_str(),
                                        x->no_seq.c_str(),        //交班序號
                                        x->no_casher.c_str(),     //收銀員號碼
                                        x->dt_begin.c_str(),      //開始日期 10
                                        x->tm_begin.c_str(),      //開始時間 11
                                        x->dt_end.c_str(),        //結束日期
                                        x->tm_end.c_str()         //結束時間
                                     );
                                                                                                              
    //交易明細ｆｉｅｌｄ=====================================================

    //本日來客數/本日交易品項  SumQt_trans() ==> 併入 SumQt_tcust  NextPOS
    int iC0TotCount = 0;
    int iSumQt_trans = 0;  //本日交易品項
	_Strsprintf(x->tb_trans.qt_tcust,"%08d", SumQt_tcust(0, gi_tot_line));   //本日來客數
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項


    //本日交易品項
    //Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);    //NextPOS
	_Strsprintf(x->tb_trans.qt_trans,"%08d", iSumQt_trans  - giI);  // SumQt_trans(0, gi_tot_line)

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;

    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line); NextPOS
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line);
    TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(x->tb_trans.am_trcpntx,"%010d", giI - C01FTaxAmt - R4_C01FTaxAmt - (PreSal09_AmtNTX + R4PreSal09_AmtNTX));        //發票日結帳額(免)
	_Strsprintf(x->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));        //發票日結帳額(稅)
	_Strsprintf(x->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        //發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    //TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(x->tb_trans.qt_miss,"%08d", giA);        //誤打發票次數
	_Strsprintf(x->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //誤打發票金額(免)
	_Strsprintf(x->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //誤打發票金額(稅)
	_Strsprintf(x->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        //誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt- ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //不開發票金額, 不開發票金額誤退重
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX +PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

	_Strsprintf(x->tb_trans.am_totsale,"%010d", int_totsale);

    string x_b;

    x_b  = (x->tb_trans.qt_tcust+SGM);                      //本日來客數  14
    x_b += (x->tb_trans.qt_trans+ SGM);                      //本日交易品項 15
    x_b += (x->no_rcstart+ SGM);                              //發票開始號碼 16
    x_b += (x->no_rcend+ SGM);                                //發票結束號碼 17
    x_b += (x->tb_trans.am_trcpntx+ SGM);                    //發票日結帳額(免) 18
    x_b += (x->tb_trans.am_trcptax+ SGM);                    //發票日結帳額(稅) 19
    x_b += (x->tb_trans.am_trcptot+ SGM);                    //發票日結帳金額 21
    //x_b += (x->tb_trans.qt_miss+ SGM);                       //誤打發票次數 2021/03/18 取消 
    x_b += (x->tb_trans.am_missntx+ SGM);                    //誤打發票金額(免) 21
    x_b += (x->tb_trans.am_misstax+ SGM);                    //誤打發票金額(稅) 22
    x_b += (x->tb_trans.am_miss+ SGM);                       //誤打發票金額 23
    x_b += (x->tb_trans.am_totsale+ SGM);                    //本日實際銷貨收入 24

    //代售付TABLE===============================================================

    string x_c;
    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                     //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);    //giH giI 食安退貨金額、次數    //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(x->tb_newadd.qt_cetel,"%08d", giD);          //代收次數
	_Strsprintf(x->tb_newadd.am_cetel,"%010d", giC);          //代收金額

    // Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giH;  iSub_C0InvoAmtC=giJ;  iSub_C0InvoQtyC=giK;

    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //x_c += (x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty)+"|");
            //x_c += (x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt)+"|");

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //x_c += (x->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty)+"|");
        //x_c += (x->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt)+"|");
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    //Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //代售合計次數
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //代售合計金額   無須加入預售商品
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN ; //+ ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;
    //25 ~ 28
	_Strsprintf(x_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //代現金TABLE===============================================================
    int iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    SumTbCash(0,gi_tot_line);   //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//消費券
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

	_Strsprintf(x->tb_cash.qt_bonus,"%08d", giA); //-giT);                //禮券回數
	_Strsprintf(x->tb_cash.am_bonus,"%010d", giB); //-iCSMAmt);            //禮券金額
    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

	_Strsprintf(x->tb_cash.qt_vndcpn,"%08d", 0);             //廠商折價券回數 目前不使用
	_Strsprintf(x->tb_cash.am_vndcpn,"%010d", 0);             //廠商折價券金額

	_Strsprintf(x->tb_cash.qt_stocpn, "%08d", giC);           //折價券回數
	_Strsprintf(x->tb_cash.am_stocpn, "%010d", giD); //+iPreSalCPNDisAmt);           //折價券金額
    SumCpn41 +=  giD;      //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;
    _Strsprintf(x->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //代現金合計回數
	_Strsprintf(x->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

	_Strsprintf(x->tb_cash.qt_bonout,"%08d", giG);           //溢收合計回數
	_Strsprintf(x->tb_cash.am_bonout,"%010d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //giE:餘額券金額 / giF:餘額券次數
    iqt_ffu1=giI;                                        //現金卡回數  電子錢次數  3275
    iam_ffu1=giJ;                                        //現金卡金額  電子錢金額  刷卡合計 3261

    string x_d;

    x_d += (x->tb_cash.qt_bonus+ SGM);                //禮券回數 29
    x_d += (x->tb_cash.am_bonus+ SGM);                //禮券金額 30
    x_d += (x->tb_cash.qt_vndcpn+ SGM);               //廠商折價券回數 31
    x_d += (x->tb_cash.am_vndcpn+ SGM);               //廠商折價券金額 32
    x_d += (x->tb_cash.qt_stocpn+ SGM);               //折價券回數 33
    x_d += (x->tb_cash.am_stocpn+ SGM);               //折價券金額 34
    x_d += (x->tb_cash.qt_totcash+ SGM);              //代現金合計回數 35
    x_d += (x->tb_cash.am_totcash+ SGM);              //代現金合計金額 36

	_Strsprintf(sTmp,"%08d", giF);
    x_d += (sTmp+ SGM);                         //餘額合計回數 37
	_Strsprintf(sTmp,"%010d", giE);   
    x_d += (sTmp+ SGM);                         //餘額合計金額 38

    x_d += (x->tb_cash.qt_bonout+ SGM);               //溢收合計回數 39
    x_d += (x->tb_cash.am_bonout+ SGM);               //溢收合計金額 40

    //==========================================================================

    //現金合計 含 消費券
    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(x->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);

    string x_e;

    x_e += (x->am_cash+ SGM);               //現金合計 40
   
    //收銀員明細FIELD===========================================================
                                                                                                              
    string x_f;

    //借零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tb_vendfld[i].am_vend,"%010d", 0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

		_Strsprintf(x->tb_casher.tb_vendfld[i].tm_vend,"%06d", 0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

	_Strsprintf(x->tb_casher.qt_vendtot,"%08d", 0);
	_Strsprintf(x->tb_casher.am_vendtot,"%010d", 0);

    //x_f += (x->tb_casher.qt_vendtot+"|");       
	_Strsprintf(sTmp,"%010d", VisuilCpnAmt);
    x_f += (sTmp+ SGM);                          //VisuilCpnAmt :  無紙本折價金額 42  

    //x_f += (x->tb_casher.am_vendtot+"|");      //借零合計金額
	_Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    x_f += (sTmp+ SGM);                           // 代收商品折讓單折扣  43

    string x_g;

    //投零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tg_feedfld[i].am_feed,"%010d", 0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

		_Strsprintf(x->tb_casher.tg_feedfld[i].tm_feed,"%06d", 0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

	_Strsprintf(x->tb_casher.qt_feedtot,"%08d", 0);
	_Strsprintf(x->tb_casher.am_feedtot,"%010d", 0);

   // x_g += (x->tb_casher.qt_feedtot+ SGM);      //投零次數 2021/03/18 取消
   // x_g += (x->tb_casher.am_feedtot+ SGM);      //投零合計金額 2021/03/18 取消

    string x_h;
                                                                                                              
    //投庫                                                                                                    
    int int_drop_line = (gtsl_drop->size() - 1);   //投庫次數

    //初始化全部填零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tg_dropfld[i].am_drop,"%08d", 0);    //投庫金額
		_Strsprintf(x->tb_casher.tg_dropfld[i].tm_drop,"%-6s", " ");    //執行時間
    }

    int int_drops = 0;                            //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                         //投庫總額
    int x_rtn_cnt=0;                              //本收銀員投庫累計次數
    string str_drop_time,s;

    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
		s = *ls; // gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
			 logsprintf("產生 擔當X 帳表:非投庫(%s)資料", s.c_str() );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, SGM, 11) );     //現金
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );     //IC付款
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );     //信用卡
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 12) );    //禮卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 13) );    //折價卷
        //int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 16) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );    //提貨單

		//中獎發票金額  500
		int_drop_money += _StrToInt(_StringSegment_EX(s, SGM, 16));

        //中獎發票金額  1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 27) );

        //中獎發票金額  200
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 15) );

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 18) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 20) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 22) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 24) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 26) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 28) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 30) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 32) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 34) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 36) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 38) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 40) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 42) ); //食安退款金額
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 44) ); //提貨券金額

        //每次投庫金額
		_Strsprintf(x->tb_casher.tg_dropfld[int_drops].am_drop,"%010d", int_drop_money);

        int_drop_sum += int_drop_money;                         //投庫總額

        str_drop_time = _StringSegment_EX(s, SGM, 5).substr(8,6);  //投庫時間

        //每次投庫時間
		_Strsprintf(x->tb_casher.tg_dropfld[int_drops].tm_drop,"%-6s", str_drop_time.c_str());

        int_drop_money = 0;
        ++int_drops;                             //投庫次數
     }
    
    string x_i;

	x_i += (_Strsprintf(x->tb_casher.qt_droptot,"%08d", int_drops) + SGM);        //投庫次數 44
	x_i += (_Strsprintf(x->tb_casher.am_droptot,"%010d", int_drop_sum) + SGM);     //投庫合計金額 45

    SumUpdat(0,gi_rec_line);
	x_i += (_Strsprintf(x->tb_casher.qt_update,"%08d", giA) + SGM);               //更正次數 46
	x_i += (_Strsprintf(x->tb_casher.am_update,"%010d", giB) + SGM);               //更正合計金額 47

    SumVoid(0,gi_tot_line);
	x_i += (_Strsprintf(x->tb_casher.qt_void,"%08d", giA) + SGM);                 //交易取消次數 48
	x_i += (_Strsprintf(x->tb_casher.am_void,"%010d", giB) + SGM);                 //交易取消合計金額 49

     SumInpdp(0,gi_rec_line);
	 x_i += (_Strsprintf(x->tb_newadd.qt_inpdp,"%08d", giA) + SGM);       //手動輸入部門次數、部門13、14 50
	 x_i += (_Strsprintf(x->tb_newadd.am_inpdp,"%010d", giB) + SGM);       //手動輸入部門金額 51

	 x_i += (_Strsprintf(x->tb_casher.qt_openbox, "%08d", gi_open_x) + SGM);            //開錢櫃次數 52

    //小計折扣  1011
    //SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	x_i += (_Strsprintf(x->tb_casher.qt_dise,"%08d", giC) + SGM);                 //折扣次數 53
	x_i += (_Strsprintf(x->tb_casher.am_dise,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + SGM);    //折扣金額 54

    //pay_sub_amt(0,gi_pay_line);
	x_i += (_Strsprintf(x->tb_newadd.qt_ffu6,"%08d", giA) + SGM);         //促銷折扣/讓次數 55
	x_i += (_Strsprintf(x->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + SGM);         //促銷折扣/讓金額 56


    easy_card(0,gi_easy_line);
	x_i += (_Strsprintf(x->tb_casher.qt_disret1,"%08d", giA) + SGM);                  //加值機開售卡次數 57
	x_i += (_Strsprintf(x->tb_casher.am_disret1,"%08d", giB) + SGM);                  //加值機開售卡金額 58
	x_i += (_Strsprintf(x->tb_casher.qt_disret2,"%08d", giC) + SGM);                  //加值機加值次數 59
	x_i += (_Strsprintf(x->tb_casher.am_disret2,"%08d", giD) + SGM);                  //加值機加值金額 60

   
    //追加TABLE=================================================================
                                                                                                              
    string x_j;                                                                                           
                                                                                                              
    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //備用八金額  現金 加值金額 2015/08/19 代售,手續費作廢金額(免)
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + SGM);  //代售,手續費銷售金額(免) 61
	x_j += (_Strsprintf(x->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + SGM);  //代售,手續費作廢金額(免) 62

    //食安次數、金額  int iqt_food,iamt_food;
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu9,"%08d", iqt_food) + SGM);    //63
	x_j += (_Strsprintf(x->tb_newadd.am_ffu9,"%010d", iamt_food) + SGM);  //64

    // 20151123修正成食安次數、金額  Charles.Wang
 	x_j += (_Strsprintf(x->tb_newadd.qt_ffu1,"%08d", iqt_ffu1) + SGM);         // 刷卡金額 65
	x_j += (_Strsprintf(x->tb_newadd.am_ffu1,"%010d", iam_ffu1) + SGM);        // 刷卡合計 66

    //發票金額0次數

	x_j += (_Strsprintf(x->tb_newadd.qt_parking,"%08d", 0) + SGM);         //代收停車費次數 67
	x_j += (_Strsprintf(x->tb_newadd.am_parking,"%010d", 0) + SGM);        //代收停車費金額 68
	x_j += (x->tb_newadd.qt_cetel + SGM);                              //代收次數69
	x_j += (x->tb_newadd.am_cetel + SGM);                              //代收金額70
    
    x_j += (_Strsprintf(x->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + SGM);    //版本前置符號 71
	x_j += (_Strsprintf(x->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + SGM);     //版本號碼 72
	x_j += (_Strsprintf(x->tb_newadd.qt_rcp0,"%08d", TotZero(0, gi_tot_line)) + SGM);    //發票金額0次數 73

    //不開發票金額
	x_j += (_Strsprintf(x->tb_newadd.am_ffu4,"%010d", int_novoice_amt) + SGM);        //不開發票金額 74
	x_j += (_Strsprintf(x->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + SGM);   //不開發票誤退重合計 75

    SumLOSS(0,gi_rec_line);
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu5,"%08d", giA) + SGM);         //TM廢棄Loss次數 76
	x_j += (_Strsprintf(x->tb_newadd.am_ffu5,"%010d", giB) + SGM);         //TM廢棄Loss金額 77

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);        // 健康捐合計
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //原加值機加值取消次數
	x_j += (_Strsprintf(x->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + SGM);   //手續費合計 78

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu7,"%08d", iSumRelDimAgn + iC0InvoAmt4VDCN) + SGM);   //即時購代售79 

	x_j += (_Strsprintf(x->tb_newadd.am_ffu2,"%010d", iCSMAmt) + SGM);      //消費券80
   
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    即時購與銷轉進值應稅 81
    x_h +=sTmp;   //"00000000|";
	_Strsprintf(sTmp,"%010d||", giD);     //即時購與銷轉進值免稅 82
    x_h +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);
	_Strsprintf(sTmp,"%08d||", giA / 200); //發票兌獎張數 83
    x_h +=sTmp; 
	_Strsprintf(sTmp,"%010d||", giA);     //發票兌獎金額 84
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for 收銀員交班明細表
    //SumCash41+=giA;                // 2012/10/08 for 收銀員交班明細表

    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);  //電子禮券回數 85
    x_h +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //電子禮券金額 86
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
	_Strsprintf(sTmp,"%010d||%010d||%08d||%s||%10s||0||1||2||3||4||5||6||7||8||9||", 
        iC0TotAmt, iC0GivUpAmt, iC0TotCount, "W", "Tran_cnt"); //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    x_h +=sTmp;

    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);

    //X報表
    string xdt_1095_data;
    
	_Strsprintf(xdt_1095_data,"%s%s%s%s%s%s%s%s%s\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),    //x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str()
                          
                          );


  FILE *fxdata;   //"C:\\FTLPOS\\HISTORY\\checkin.xdt"

  // nextPOS
  if (w_WriteSAL(xdt_1095_data, str_x_filename, 3) == OK)  //3 OverWrite
  {
      logsprintf("File OutPut OK:%s", str_x_filename.c_str());
  }
  else
  {
      logsprintf("File Open Error:%s", str_x_filename.c_str());
  }

/********
#ifdef WIN32
	errno_t err;
	if ((err = fopen_s(&fxdata, str_x_filename.c_str(), "w")) == 0)
#else
	if ((fxdata = fopen(str_x_filename.c_str(),"w")) != NULL)
#endif
    {
        fprintf(fxdata,"%s",xdt_1095_data.c_str() );
        fclose(fxdata);
    }
    else
    {
         logsprintf("File Open Error:%s",str_x_filename.c_str());
    }
**********/

    //X報表                                                                                                   
    string str_xrp_checkin,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_cashier_no,
               str_start_trans;
                                                                                                              
    str_xrp_checkin=xdt_1095_data;

    //if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
	if (!DirExists("C:\\FTLPOS\\HISTORY\\"))
		SHCreateDirectory(NULL, L"C:\\FTLPOS\\HISTORY\\" );   //ForceDirectories("C:\\FTLPOS\\HISTORY\\");
        
    //report = new REPORT();      //報到帳表
	strcpy_s(report->char_filename, sizeof(report->char_filename),"C:\\FTLPOS\\HISTORY\\checkin.xrp");
    report->CreateReport("c", str_xrp_checkin, StrBillPay, g_tencode, g_ecr_no,
                              iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 ,"");
    writelog("產生報到帳表 C:\\FTLPOS\\HISTORY\\checkin.xrp");

    /**************************************************************************/

    str_tencode = g_tencode;   //店號
    _Strsprintf(str_ecr_no,"%02d", _StrToInt(x->no_tm,"機號"));              //機號
    str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

	str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "");    //tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

	_Strsprintf(str_cashier_no,"%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    str_start_trans = _StringSegment_EX(str_x_first, SGM, 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

    string str_trans1_5, CasherRptFile;    //交易序號前5碼
    //str_trans1_5 = str_sal_last.SubString(36,5);

    CasherRptFile=_ChangeFileExt(string(report->char_filename),string(".Crp"));
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
	_Strsprintf(sTmp,"3041||0063||%6s||%2s||%14s||%5s||%-10s||%-20s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        (str_trans1_5 + str_tran_cnt).c_str(),
                        str_cashier_no.c_str()
                        );


    //產生收銀員交班明細表  收銀員交接班明細表
    string SumCasherRptData;
    SumCasherRpt(NAUTOZ, 0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt, sTmp, "00",
                 CasherRptFile, "", "", "", "", SumCasherRptData, iamt_food, iqt_food);
    logsprintf("產生收銀員交班明細表:%s",CasherRptFile.c_str());

   // delete x, tif_ecr, report;
    return 1;
}

CHECKIN::CHECKIN()
{
    x = new SPOS_XRPDT;   //X帳檔案結構
    //tif_ecr   = new TIniFile(XZDATA_INI);
	tif_ecr.setINIFileName(XZDATA_INI);
    report = new REPORT();      //報到帳表
    writelog("報到 開始");

}

CHECKIN::~CHECKIN()
{
    // delete report;      //報到帳表
    delete x, tif_ecr, report;
    writelog("報到 結束");
}

XDATA::XDATA()
{
    //
    writelog("★★ 收銀員帳表 開始");
    x = new SPOS_XRPDT;         //X帳檔案結構
    //tif_ecr = new TIniFile(XZDATA_INI);
	//tif_tmp = new TIniFile(XDATATMP_INI);
	tif_ecr.setINIFileName(XZDATA_INI);
	tif_tmp.setINIFileName(XDATATMP_INI);
    report = new REPORT();      //收銀員帳表
}

XDATA::~XDATA()
{
    //
    delete report;      //收銀員帳表
    delete x,tif_ecr;
    //delete tif_tmp;
    writelog("★★ 收銀員帳表 結束");
}

XREPORT::XREPORT()
{
     
    writelog("讀帳 開始");
    report = new REPORT();      //讀帳帳表
    tif_ecr.setINIFileName(XZDATA_INI);
    tif_tmp.setINIFileName(ZDATATMP_INI);
}

XREPORT::~XREPORT()
{
    //
    delete report;      //讀帳帳表
   
    writelog("讀帳 結束");
}

ZDATA::ZDATA()
{
    //
    writelog("Z帳 開始");
    z = new SPOS_ZRPDT;         //Z帳結構
   // tif_tmp = new TIniFile(ZDATATMP_INI);
   // tif_ecr = new TIniFile(XZDATA_INI);
	tif_ecr.setINIFileName(XZDATA_INI);
	tif_tmp.setINIFileName(ZDATATMP_INI);
    report = new REPORT();      //日結帳表

    

}

ZDATA::~ZDATA()
{
    delete z, tif_ecr;

    delete report;      //日結、部門帳表
   // delete tif_tmp;

    writelog("Z帳 結束");

}

void __fastcall ZDATA::mtn_dat_folder() //維護DAT下傳檔30日內
{
 //   string Dir;
 //   string FileName;
 //   TSearchRec SearchRec;
 //   int iAttributes = 0;
 //   iAttributes |= faAnyFile * true;

 //   string str_dat_path = "C:\\Abacus\\download\\dat\\";
 //   Dir = str_dat_path;

 //   TStringList *TempList = new TStringList;
 //   try
 //   {

 //       TempList->Clear();

 //       if (FindFirst(Dir+"*.tm", iAttributes, SearchRec) == 0)
 //       {
 //           do
 //           {
 //               if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
 //               {
 //                   TempList->Add(SearchRec.Name);
 //               }
 //           } while (FindNext(SearchRec) == 0);

 //           FindClose(SearchRec);
 //       }

 //       string str_dd_flag = FormatDateTime("yymmdd",Now()-30);
 //       string str_dd_file;

 //       for ( int i = 0; i <= (TempList->Count-1); i++)
 //       {
 //           FileName = TempList->Strings[i];

 //           if (FileName.Length() < 13)
 //           {
 //               TempList->Delete(i);
 //               i = 0;
 //               continue;
 //           }

 //           str_dd_file = FileName.SubString(3,6);

 //           if (str_dd_file.AnsiCompare(str_dd_flag) < 0)
 //           {
 //               FileName = (Dir + TempList->Strings[i]);
 //               DeleteFile(FileName);
 //               writelog("維護DAT刪除檔案 " + FileName);
 //           }
 //       }
 //   }
	//catch (...)
 //   {
	//	;;
 //   }
	//delete TempList; // destroy the list object
}

void __fastcall ZDATA::del_list_file(TStringList *slist, string path)
{
    string str_sal_00;

    //for (int i=0; i<list->Count; i++) //for (list<string>::iterator ls = gtsl_1051_sal->begin(); ls != gtsl_1051_sal->end(); ls++)
	for (list<string>::iterator ls = slist->begin(); ls != slist->end(); ls++)
    {
		str_sal_00 = path + *ls; // list->Strings[i]);
		DeleteFile(StringToWString(str_sal_00).c_str());
        writelog("三十天目錄維護刪除檔案 " + str_sal_00);
    }

    slist->clear();
}

/////////////////////////////////////////////////////////////////////////////////////






SPCDATA::SPCDATA()
{
    //
    int i;
    writelog("重點商品列管帳表 開始");
    //tif_ecr = new TIniFile(XZDATA_INI);
	tif_ecr.setINIFileName(XZDATA_INI);
	//tif_tmp.setINIFileName(XDATATMP_INI);
   
    tb_setgrpcount=0;
    for(i=0; i<109; i++)
       {
        tb_setgrp[i].no_setgrp="";
        tb_setgrp[i].nm_setgrp="";
        tb_setgrp[i].no_setsubgrp="";
        tb_setgrp[i].qt_setgrp=0;
        tb_setgrp[i].am_setgrp=0;
        tb_setgrp[i].MustPrinted=0;
       }


    for(i=0; i<109; i++)
       {
               tb_30TMgrp[i].no_setgrp="";
               tb_30TMgrp[i].nm_setgrp="";
               tb_30TMgrp[i].amt_minpric=0;
               tb_30TMgrp[i].amt_maxpric=0;
               tb_30TMgrp[i].no_setsubgrp="";
       }

    Get30TMData();   

}



SPCDATA::~SPCDATA()
{

    //delete report;      //收銀員帳表
    //delete tif_ecr;
    //delete tif_tmp;
    writelog("重點商品列管帳表 結束");
}



//重點商品列管帳表
//iQrySw=1 ,查詢
int __fastcall SPCDATA::WriteSpcInqData(const string SalFileName, const string Version,
                                        const int iQrySw )
{


    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    if (!int_reslut)
        return 0;

    //TStringList  *Tm30Data = new TStringList;
    //TStringList  *tsl_Spcdata = new TStringList;

    //SPOS_XRPDT *x = new SPOS_XRPDT;         //X帳檔案結構

    string str_x_filename,              //XDATA檔名
               str_ptran_cnt,               //前次結帳序號
               str_date_z,                  //DataZ(日期加X帳次數)
               str_tran_cnt,
               str_pxno_seq,                //X帳結帳次數
               str_nxno_seq,                //下次結帳數
               str_pSPCDttm,
               str_pSPC0010DataKey,
               str_pSpcno_seq,
               str_npSpcno_seq,
               str_nz_cnt,
               str_ecr_no,
               str_trans1_5,
               pzDate_date_time,
               str_tencode,
               sTmp,
               sLog;


	pzDate_date_time = tif_ecr.ReadString("Z", "PZ_DATE", currentDateTime(1)); // FormatDateTime("yyyymmddhhnnss", Now()));
    str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零
    str_date_z = tif_ecr.ReadString("Z","DATEZ","");           //ddn
    //str_tran_cnt = tif_ecr->ReadString("ECR","TRAN_CNT","");    //目前交易序號
    sTmp = tif_ecr.ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1
	_Strsprintf(str_tran_cnt,"%05d", _StrToInt(sTmp) - 1>0 ? _StrToInt(sTmp) - 1 : 1);
    logsprintf("SPCDATA::ECR->ReadString 取的目前之交易序號:%s", str_tran_cnt.c_str());

    string str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;
	str_date_time = currentDateTime(1); //  FormatDateTime("yyyymmddhhnnss", Now());

    if (str_date_z.length() < 3)    //DATAZ=日期+Z帳次數  ddn
        str_date_z = (str_date_time.substr(6,2) + "1");

    string str_sal_last;        //LOG最後一筆
    str_sal_last = gsal_last;   //gpchar_sal_last;

    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last, SGM, 7).substr(5,5);
        //str_sal_last.SubString(41,5);

    str_trans1_5 = _StringSegment_EX(str_sal_last, SGM, 7).substr(0,5);
        //str_sal_last.SubString(36,5);


	str_pxno_seq = tif_ecr.ReadString("X", "pxno_seq", "");   //tif_ecr->ReadString("X", "pxno_seq", "");                  //上次重點商品列管帳表累積次數
    _Strsprintf(str_nxno_seq,"%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //目前重點商品列管帳表到第幾次
    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";


    str_pSpcno_seq = tif_ecr.ReadString("SPC","pSPCno_seq","");                //上次x帳累積次數
	_Strsprintf(str_npSpcno_seq,"%02d", (_StrToInt(str_pSpcno_seq, "pSPCno_seq") + 1)); //目前x次到第幾次

    str_pSPCDttm = tif_ecr.ReadString("SPC","pSPCDttm",pzDate_date_time);
    str_pSPC0010DataKey=tif_ecr.ReadString("SPC","pSPCData","");                //上次重點商品列管帳表最後一筆0010
    //str_pSPC0010DataKey:yyyymmddhhnnss|00ZZn99999

    if (_StrToInt(str_nxno_seq,"pSPCno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";
   
    string str_invo_char;                    //發票英文字母
    str_invo_char = tif_ecr.ReadString("ECR", "INVO_CHAR", "");

    if (str_invo_char.length() < 2)
        str_invo_char = "XX";
    
    string str_pzno_seq;
    str_pzno_seq = tif_ecr.ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數   換日歸1

    //20031230
    //結束時間目前時間為目前結帳時間

	_Strsprintf(str_ecr_no,"%02d", _StrToInt(g_ecr_no) );              //機號

    str_tencode = g_tencode;   //店號
	ed_date_time = currentDateTime(1);  //FormatDateTime("yyyymmddhhnnss", Now());

    string spc_a;
    
	_Strsprintf(spc_a,"3180||0025||%-6s||%2s||%14s||%5s||%10s||%2s||%10s||%14s||",
                                        g_tencode.c_str(),
                                        str_ecr_no.c_str(),
                                        str_date_time.c_str(),
                                        str_nz_cnt.c_str(),       //日結序號
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_nxno_seq.c_str(),
                                        " ",                      //TM統計表號
                                        str_pSPCDttm.c_str()
                                     );

    //重點商品列管帳表
    tb_setgrpcount=0;

    tb_setgrpcount=SumSPCData(0, gi_rec_line, str_pSPC0010DataKey);

    //寫入SAL檔97Type
    string str_rec_type,
               str_rec_len,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver,
               str_write_PrvRpt;

    str_rec_type = "3097";
    str_rec_len  = "0021";

    //str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入97交易序號"); //目前寫入97交易序號
	_Strsprintf(str_trans_cnt,"%05d", int_sum_transcnt);

    string str_sum_tran;       //此次收銀員帳交易序號
    //20031215
    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

	_Strsprintf(str_sum_tran,"%05d", (int_sum_transcnt + 1));

    str_ptran_cnt = str_trans_cnt;
	    
	_Strsprintf(str_z_cnt,"%05d", _StrToInt(str_nxno_seq, "交班序號"));
    str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零
    

    string str_write_sal, str_write_rpt, stmp;

    if( tb_setgrpcount > 0)
      {
       if( iQrySw == 1)   // 查詢
         {
          str_write_sal = "c:\\abacus\\TmpSpc.sal";
          str_write_rpt = "c:\\abacus\\posdata\\TmpSpc.rpt";
          str_write_PrvRpt = "c:\\abacus\\posdata\\TmpPrvSpc.rpt";

		  if (!PathFileExists(StringToWString(str_write_sal).c_str()))  //if (FileExists(str_write_sal))
			  DeleteFile(StringToWString(str_write_sal).c_str());

          //if (FileExists( str_write_rpt))
          //      DeleteFile( str_write_rpt);

          if( WriteSPCSAL(spc_a,str_write_sal))
            {
             tif_ecr.WriteString("SPC","TMPSPfilename", str_write_sal);

            }
          else
            {
              tif_ecr.WriteString("SPC","TMPSPfilename", "");
              //tif_ecr->WriteString("SPC","TMPSPRptname", "");
            }

         }
       else
         {
           //寫入交易nn.SAL檔

          if (WriteSPCSAL(spc_a,SalFileName))
             {
                 tif_ecr.WriteString("ECR","TRAN_CNT", str_sum_tran);
				 str_write_sal = "C:\\FTLPOS\\XZDATA\\00000" + str_trans_cnt + ".sal";
				 str_write_rpt = "C:\\FTLPOS\\XZDATA\\TmpSpc.rpt";
				 str_write_PrvRpt = "C:\\FTLPOS\\XZDATA\\TmpPrvSpc.rpt";

				 if (!PathFileExists(StringToWString(str_write_sal).c_str()))   //if (FileExists(str_write_sal))
					 DeleteFile(StringToWString(str_write_sal).c_str());

				 if (!PathFileExists(StringToWString(str_write_rpt).c_str()))   //if (FileExists(str_write_rpt))
					 DeleteFile(StringToWString(str_write_rpt).c_str());

                if( WriteSPCSAL(spc_a,str_write_sal))
                    {
					tif_ecr.WriteString("SPC", "SPC0080filename", str_write_sal); // WriteString("SPC", "SPC0080filename", str_write_sal);
                     if( WriteSPCReport(spc_a, str_write_rpt) )
                        {
                          tif_ecr.WriteString("SPC","TMPSPRptname", str_write_rpt);
						  if (CopyFile(StringToWString(str_write_rpt).c_str(), StringToWString(str_write_PrvRpt).c_str(), false))
                              tif_ecr.WriteString("SPC","TMPPRVSPRptname", str_write_PrvRpt);
                        }
                     else
                        {
                          tif_ecr.WriteString("SPC","TMPSPRptname", "");
                        }
                    }
                else
                   {
                    tif_ecr.WriteString("SPC","SPC0080filename", "");
                    tif_ecr.WriteString("SPC","TMPSPRptname", "");
                   }

               tif_ecr.WriteString("SPC","pSPCDttm",str_date_time);
               tif_ecr.WriteString("SPC","pSPCTran_cnt",str_date_z + str_ptran_cnt );
               tif_ecr.WriteString("SPC","pSPCno_seq",Trim(str_npSpcno_seq) );
               tif_ecr.WriteString("SPC","pSPCData",Trim(str_pSPC0010DataKey) );
               logsprintf("WriteSpcInqData: 重點商品列管資料處裡完成, 設定 Ecr_no.ini->SPC->pSPCData=%s",
                             str_pSPC0010DataKey.c_str() );
            }
          else
            {
              tif_ecr.WriteString("SPC","SPC0080filename", "");
              tif_ecr.WriteString("SPC","TMPSPRptname", "");
              tb_setgrpcount=0;
            }

        } // end of else
    } // end of  if( tb_setgrpcount > 0)
   else
    {
       tif_ecr.WriteString("SPC","SPC0080filename", "");
       tif_ecr.WriteString("SPC","TMPSPfilename", "");
       tif_ecr.WriteString("SPC","TMPSPRptname", "");
       tb_setgrpcount=0;
       logsprintf("WriteSpcInqData: 無重點商品列管資料 " );
    }

   //delete x,tif_ecr;

    return(tb_setgrpcount);
}




// 重點商品列管帳表
int __fastcall SPCDATA::SumSPCData(int StartLine, int TotalLine, string &PrvSPCDataKey )
{
    string sRType, sType, stmp, sSpcType,s;
    string str_dept, tmpDttm_Tran, tmpStDttm_Tran, tmpEdDttm_Tran;

    int iDept, iDataCnt, i_type, itmpQt, itmpAm;
    int seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam, segDEP, segUNIT;

    iDept = iDataCnt = 0;      //部門別
    //數量、金額
    giA = giB = 0;

    tmpStDttm_Tran=PrvSPCDataKey;
    tmpDttm_Tran="";

    if (gbl_rec_haveline)
    {
       // for (int i=StartLine; i<=TotalLine; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;

            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
                // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
                // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19, segDEP.CD_DEP=20, segUNIT:29;
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19, segDEP = 20, segUNIT = 29;
                break;
            case 113:
                //seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                //seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                segDONGLE = 11, segNam = 18, segDEP = 41, segUNIT = 30;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }
 
            //str_dept = gtsl_rec->Strings[i].SubString(119,2).Trim();  //部門
            //str_dept = gtsl_rec->Strings[i].SubString(124,2).Trim();    //判別列管群號
            str_dept = _StringSegment_EX(s, SGM, segDEP);

            //tmpDttm_Tran=gtsl_rec->Strings[i].SubString(21,14).Trim()+"|";
            tmpDttm_Tran=_StringSegment_EX(s, SGM, 5)+ SGM;

            //tmpDttm_Tran+=gtsl_rec->Strings[i].SubString(36,10).Trim()+"|";
            tmpDttm_Tran+=_StringSegment_EX(s, SGM, 7)+ SGM;

            giA = giB = 0;


            if( tmpDttm_Tran <= PrvSPCDataKey )
              continue;

            if (str_dept.length() > 0)
                iDept = _StrToInt(str_dept);
            else
                continue;

            if( iDept < 1 && iDept > 99 )
              {
                 logsprintf("SumSPCData:1010->列管群號 Error (%s-%s-%d)",
                               tmpDttm_Tran, str_dept, iDept );
                 continue;
              }

            sRType = Trim(_StringSegment_EX(s, SGM, seg11)); // .substr(0, 2);
            //gtsl_rec->Strings[i].SubString(370,2);

            sType = _StringSegment_EX(s, SGM, seg1); // 8);
            //gtsl_rec->Strings[i].SubString(68,2);


			sSpcType = Trim(_StringSegment_EX(s, SGM, seg2)); // .Trim();
            //gtsl_rec->Strings[i].SubString(224,1);
			if (_StrFind(sType, "A0") || _StrFind(sType,"A1"))
               { ;; }
			else if (_StrFind(sType,"S0") || _StrFind(sType,"S1"))
                    {
                        if( sSpcType == "2" || sSpcType == "02" )  //代售
                           { ;; }
                        else
                           { continue; }
                    }
            else
               { continue; }



            itmpQt=_StrToInt(_StringSegment_EX(s, SGM, seg4));   // QT_ITEM
            itmpAm= _StrToInt(_StringSegment_EX(s, SGM, seg5));   //AM_ITEM
            iDataCnt++;
            //正常銷售
			if (_StrFind(sRType,"FF") || _StrFind(sRType,"Z0") || _StrFind(sRType,"Z1"))
            {
                    //sType = gtsl_rec->Strings[i].SubString(68,2);

				if (_StrFind(sType,"A0") || _StrFind(sType,"S0"))        //部門銷售
                    {
                        giA += itmpQt;
                        giB += itmpAm;
                        //_StrToInt(gtsl_rec->Strings[i].SubString(159,8));
                    }
				else if (_StrFind(sType,"A1") || _StrFind(sType,"S1"))   //銷售取消
                    {
                        giA -= itmpQt;
                        giB -= itmpAm;
                    }

                    //tb_setgrp[iDept].no_setgrp="";
                    //tb_setgrp[iDept].nm_setgrp="";
                    //tb_setgrp[iDept].no_setsubgrp="";
                    tb_setgrp[iDept].MustPrinted=1;
                    tb_setgrp[iDept].qt_setgrp+=giA;
                    tb_setgrp[iDept].am_setgrp+=giB;
                    tb_setgrp[iDept].no_setgrp=str_dept;
                    tb_setgrp[iDept].nm_setgrp=tb_30TMgrp[iDept].nm_setgrp;
                    if( Trim(tb_setgrp[iDept].nm_setgrp) == "")
                        tb_setgrp[iDept].nm_setgrp="列管群 "+str_dept;

                    tb_setgrp[iDept].no_setsubgrp=tb_30TMgrp[iDept].no_setsubgrp;


            }
			else if (_StrFind(sRType,"R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {

                    //sType = gtsl_rec->Strings[i].SubString(68,2);

				if (_StrFind(sType,"A0") || _StrFind(sType,"S0"))        //部門銷售
                    {
                        giA -= itmpQt;
                        giB -= itmpAm;
                    }
				else if (_StrFind(sType,"A1") || _StrFind(sType,"S1"))   //銷售取消
                    {
                        giA += itmpQt;
                        giB += itmpAm;
                    }

                    tb_setgrp[iDept].MustPrinted=1;
                    tb_setgrp[iDept].qt_setgrp+=giA;
                    tb_setgrp[iDept].am_setgrp+=giB;
                    tb_setgrp[iDept].no_setgrp=str_dept;
                    tb_setgrp[iDept].nm_setgrp=tb_30TMgrp[iDept].nm_setgrp;
                    if( Trim(tb_setgrp[iDept].nm_setgrp) == "")
                        tb_setgrp[iDept].nm_setgrp="列管群 "+str_dept;
                    tb_setgrp[iDept].no_setsubgrp=tb_30TMgrp[iDept].no_setsubgrp;
         
             }

        }
    }

    PrvSPCDataKey=tmpDttm_Tran;
    logsprintf("SumSPCData:1010->重點商品列管帳表資料筆數(%d) ST_Dttm_Tran(%s)~~ED_Dttm_Tran(%s)",
                iDataCnt, tmpStDttm_Tran.c_str(), PrvSPCDataKey.c_str() );

    //PrvSPCDataKey=tmpDttm_Tran;
    return iDataCnt;
}



int __fastcall SPCDATA::WriteSPCSAL(string strHdata,  string SALFileName)
{
    FILE *f_write_sal;
    string tmpdata, str_01, str_99, tmp_str;
    int iCnt,i, cnt80, cnt81;

    iCnt=cnt80=cnt81=0;

    /******************************************/
    TStringList *tmpsl = new TStringList();
    tmpsl->clear();
    tmp_str="c:\\FTLPOS\\XZDATA\\1081SpcTmp.txt";  // FTLPOS.exe 產生
	if (!PathFileExists(StringToWString(tmp_str).c_str()))    //if (FileExists(tmp_str))
       {
		_wListLoadFromFile(tmpsl, tmp_str.c_str());
        logsprintf("WriteSPCSAL: Load 列管商品銷售明細資料(%s) (1081筆數=%d) OK.",
                        tmp_str.c_str(),tmpsl->size()  );
        //DeleteFile(tmp_str);    doont delete this file
       }
     else
       {
        logsprintf("WriteSPCSAL: 列管商品銷售明細資料(%s) not find.",
                        tmp_str.c_str(),tmpsl->size()  );
       }
    /****************************************/


    for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp == 0 && tb_setgrp[i].MustPrinted==0)
         if(tb_setgrp[i].qt_setgrp == 0 )   // 2009/10/02
           continue;

         iCnt++;
       }

    if( (tb_setgrpcount <= 0 || iCnt <=0) && tmpsl->size()==0)
       {
        writelog(" 無資料寫入SAL檔(1080-1081) " + SALFileName);
        return 0;
      }


#ifdef WIN32
	errno_t err;
	if ((err = fopen_s(&f_write_sal, SALFileName.c_str(), "a")) != 0)
#else
	if ((f_write_sal = fopen(SALFileName.c_str(), "a")) == NULL)
#endif
    {
        writelog("寫入SAL檔(3080-3181)發生錯誤 " + SALFileName);
        delete tmpsl;
        return 0;
    }

    _Strsprintf(str_01,"%4s||%4s||%s|%2s||%10s||%2s||%2s||%10s||%10s||%-20s||%4s||%-20s||%-20s||%05d||%010ld||%s||%6s||%-20s||%2s||%s||",
                                        "3001",
                                        "0033",
                                        strHdata.substr(12,46).c_str(),
                                        "Z0",
                                        " " ,      //SALE_DATAVER 
                                        " ",       //RTN_TYPE  
                                        " ",       //ORGECR_NO
                                        " ",       //ORGTRAN_CNT 
                                        " ",           //RTNTRAN_CNT
                                        " ",       //NO_SALES
                                        " ",       // NO_KIND
                                        " ",       //NO_VIP
                                        " ",       //NO_CARD
                                        0,         //QT_DEPT
                                        0,         //AM_DEPT
                                        " ",       //TRAN_TYPE
                                        " ",       //NO_ORGSTORE
                                        " ",       //SCO_TYPE
                                        " ",       //NO_TMPREV
                                        "0||1||2||3||4||5||6||7||8||9"
                                         );

   fprintf(f_write_sal,"%s\n",str_01.c_str());

   /*********************3180 OutPut*****************************/
   for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp==0 && tb_setgrp[i].MustPrinted==0 )
         if(tb_setgrp[i].qt_setgrp==0 )  // 2009/10/02
           continue;

         cnt80++;
		 _Strsprintf(tmpdata,"%s%-2s||%-30s||%-30s||%08ld||%010.0f||%s||",
                           strHdata.c_str(),
                           tb_setgrp[i].no_setgrp.c_str(),
                           tb_setgrp[i].nm_setgrp.c_str(),
                           tb_setgrp[i].no_setsubgrp.c_str(),
                           tb_setgrp[i].qt_setgrp,
                           tb_setgrp[i].am_setgrp,
                           "0||1||2||3||4||5||6||7||8||9"
                            );
         fprintf(f_write_sal,"%s\n",tmpdata.c_str());
       }

   /*********************0081 OutPut*****************************/
   if( tmpsl->size() > 0)
     {
       //for (int i=0; i<tmpsl->Count; i++)
		 for (list<string>::iterator ls = tmpsl->begin(); ls != tmpsl->end(); ls++)
           {
             cnt81++;
			 tmpdata = *ls; // tmpsl->Strings[i];
             fprintf(f_write_sal,"%s\n",tmpdata.c_str());
            }
     }
   delete tmpsl;
   /***********************************************************/

   str_99 = str_01;
   str_99 = "3099" + str_99.erase(0,4);
   fprintf(f_write_sal,"%s\n",str_99.c_str());
   //tif_tmp->WriteString("X","x0097filename", str_write_sal);


   fclose(f_write_sal);

   logsprintf("WriteSPCSAL: 列管商品銷售統計(%s) 3080筆數=%d, 3081筆數=%d OK. ",  SALFileName.c_str(), cnt80, cnt81 );

   return iCnt;
}




int __fastcall SPCDATA::Get30TMData()
{

   FILE *plu_file;
   string sQryFileName,stmp,sData;
   int LineLength, igQryStatus, tot_cnt, itmpPrice;
   char tmp_buf[256],data_buffer[513],c;
   int handle, tmp_no_setgrp;

   LineLength=0;
   igQryStatus=0;
   tot_cnt=0;

  _Strsprintf(sQryFileName,"c:\\abacus\\posdata\\30.tm");
    
#pragma region Check  sQryFileName is ok (>80) Byte

#ifdef WIN32
  errno_t err;
  if( (err = _sopen_s(&handle, sQryFileName.c_str(), O_RDONLY, _SH_DENYNO, _S_IWRITE)) != 0)
#else
  if ((handle = _sopen(sQryFileName.c_str(), O_RDONLY, _SH_DENYNO, _S_IWRITE)) == -1)
#endif
     {
      logsprintf( tmp_buf,"Get30TMData: %s 資料有誤. (file not find)",sQryFileName);
      return(0);
     }
  else
     {
      LineLength = _filelength(handle);
      _close(handle);
     }

  if( LineLength < 80)
     {
      logsprintf( tmp_buf,"Get30TMData: CheckLength %s 資料有誤. DATA Length < %d",sQryFileName.c_str(), 80);
      return(0);
     }

#pragma endregion



#pragma region Read  sQryFileName 
#ifdef WIN32
  //errno_t err;
  if ((err = fopen_s(&plu_file, sQryFileName.c_str(), "r")) != 0)
#else
  if ((plu_file=fopen(sQryFileName.c_str(), "r")) == NULL)
#endif
       {
        logsprintf( tmp_buf,"Get30TMData: %s 資料有誤. (fopen error)",sQryFileName.c_str());
        return(0);
       }


  while( !feof(plu_file) )
      {
        memset( data_buffer,'\0',sizeof(data_buffer));
        c=0;

        scan_fixstr(plu_file, data_buffer, 78);
        data_buffer[78] = 0;
        stmp=string(data_buffer);

        fscanf_s(plu_file,"%c",&c );

        tot_cnt++;
        if( c != 0x0d && c != 0x0a)
          {
            logsprintf( tmp_buf,"Get30TMData: %s 資料0x0d0x0a有誤. (行數=%ld)",sQryFileName.c_str(),tot_cnt);
            //Post=false;
            igQryStatus=0;
            break;
            }

        //sData="|";
          tmp_no_setgrp=_StrToInt(stmp.substr(0,2));
          if( tmp_no_setgrp > 0 &&  tmp_no_setgrp < 100)
            {
				tb_30TMgrp[tmp_no_setgrp].no_setgrp = Trim(stmp.substr(0, 2));   
				tb_30TMgrp[tmp_no_setgrp].nm_setgrp = Trim(stmp.substr(2, 30)); 

               itmpPrice=_StrToInt(stmp.substr(32,8));
               tb_30TMgrp[tmp_no_setgrp].amt_minpric=itmpPrice;

               itmpPrice=_StrToInt(stmp.substr(40,8));
               tb_30TMgrp[tmp_no_setgrp].amt_maxpric=itmpPrice;

               tb_30TMgrp[tmp_no_setgrp].no_setsubgrp=stmp.substr(48,30);

              //sData+=(stmp.SubString(1,2)+"|"+stmp.SubString(3,30)+"|"+stmp.SubString(33,8)+"|"+stmp.SubString(41,8)+"|");
             //sData+=(stmp.SubString(49,3)+" "+stmp.SubString(52,3)+" "+stmp.SubString(55,3)+" "+stmp.SubString(58,3)+" "+stmp.SubString(61,3)+" ");
             //sData+=(stmp.SubString(64,3)+" "+stmp.SubString(67,3)+" "+stmp.SubString(70,3)+" "+stmp.SubString(73,3)+" "+stmp.SubString(76,3)+" |FFFFFFFF|");
             //ls->Add(sData);
            }
          else
            {
              logsprintf( tmp_buf,"Get30TMData: NO_SETGRP 資料有誤. (%s)",stmp.c_str() );
            }

        fscanf_s(plu_file,"\n");
   }  // end of while

  fclose(plu_file);
#pragma endregion

  logsprintf( tmp_buf,"Get30TMData: %s (總筆數=%ld)", sQryFileName.c_str(), tot_cnt );

  return(tot_cnt);
}



int __fastcall SPCDATA::WriteSPCReport(string strHdata, string RptSALFileName)
{

   TStringList *tslCrp, *tsltmp;
   tslCrp = new TStringList;
   tsltmp = new TStringList;

   string nt,NowTime,PrvTime,  sStoreNo, sStoreName, stmp, strtmp, strQt;
   int i;

   tslCrp->clear();
   tsltmp->clear();
   for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp==0 && tb_setgrp[i].MustPrinted==0)
         if(tb_setgrp[i].qt_setgrp==0 )     // 2009/10/02
           continue;

         if( tb_setgrp[i].qt_setgrp > 99999 )
            strQt="?????";
         else
			 _Strsprintf(strQt,"%5ld", tb_setgrp[i].qt_setgrp);

		 _Strsprintf(stmp," %-2s  %-20s %5s %8.0f",
                           tb_setgrp[i].no_setgrp.c_str(),
                           Trim(tb_setgrp[i].nm_setgrp.substr(0,20)).c_str(),
                           strQt.c_str(),
                           tb_setgrp[i].am_setgrp
                            );

         tsltmp->push_back(stmp);
       }

   if( tsltmp->size() <= 0 )
     {
      delete tslCrp;
      delete tsltmp;
      return(0);
     }


   nt=strHdata.substr(20,16);
   //NowTime=nt.SubString(3,2)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   NowTime=nt.substr(0,4)+"-"+nt.substr(4,2)+"-"+nt.substr(6,2)+" "+nt.substr(8,2)+":"+nt.substr(10,2);
   nt=strHdata.substr(67,16);
   //PrvTime=nt.SubString(3,2)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   PrvTime=nt.substr(0,4)+"-"+nt.substr(4,2)+"-"+nt.substr(6,2)+" "+nt.substr(8,2)+":"+nt.substr(10,2);
   read_ten_name(sStoreNo, sStoreName);

   _Strsprintf(strtmp,"           列管商品銷售統計表"); tslCrp->push_back(strtmp);
   //strtmp.sprintf("%s  前次統計:%s",NowTime,PrvTime ); tslCrp->Add(strtmp);
   _Strsprintf(strtmp, "列管商品統計時間:%s", NowTime.c_str()); tslCrp->push_back(strtmp);
   _Strsprintf(strtmp, "前次列管統計時間:%s", PrvTime.c_str()); tslCrp->push_back(strtmp);
   _Strsprintf(strtmp, "店號:%s    店名:%s  機號:%s", strHdata.substr(10, 6).c_str(), sStoreName.c_str(), strHdata.substr(17, 2).c_str());
   tslCrp->push_back(strtmp);

   //strtmp.sprintf("%s     機號:%s",NowTime.c_str(),sTranCnt.SubString(18,2) ); tslCrp->Add(strtmp);

   _Strsprintf(strtmp, "收銀員:%s\n", strHdata.substr(55, 8).c_str() );     tslCrp->push_back(strtmp);
   _Strsprintf(strtmp,"群號 列管群名稱            數量     金額");   tslCrp->push_back(strtmp);

   //tsltmp->Sort();
   //for (int i=0; i<tsltmp->Count; i++)
   for (list<string>::iterator ls = tsltmp->begin(); ls != tsltmp->end(); ls++)
    {
		stmp = *ls; // tsltmp->Strings[i];
		tslCrp->push_back(stmp);
    }

   _Strsprintf(strtmp, "----------------------------------------\n\n\n\n");  tslCrp->push_back(strtmp);
   _Strsprintf(strtmp,"\x1b\x69");  tslCrp->push_back(strtmp); //切紙

    //輸出檔案
   if (!PathFileExists(StringToWString(RptSALFileName).c_str()))   //if (FileExists(RptSALFileName.c_str()))
	   DeleteFile(StringToWString(RptSALFileName).c_str());

   // _ListToFile(tslCrp, RptSALFileName.c_str());  //tslCrp->SaveToFile(RptSALFileName);
   _ListToWFile(tslCrp, RptSALFileName);
  
   //tslCrp->Clear();
   i=tsltmp->size();
   delete tslCrp;
   delete tsltmp;

   logsprintf("WriteSPCReport: 列管商品銷售統計表 OK. (%s)",RptSALFileName.c_str() );

   return(i);

}




//盤點人員讀帳   call by _WVXZ
/// <summary> VXZDATA::WriteData
/// 盤點人員讀帳   call by _WVXZ  報表Class
/// </summary>
/// <param name=""></param>
/// <returns>int</returns>
int __fastcall VXZDATA::WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version)
{

    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    string sLog, sTmp;
	logsprintf("盤點人員讀帳 VXZDATA::WriteData(%s, %s, %s, %s, %s)", StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str());

    if (!int_reslut)
        return 0;

    string str_ver_num, str_tran_cnt;

	//sprintf_s(gchar_tencode, sizeof(gchar_tencode), "%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", SalesNo.c_str());
    g_cashier = SalesNo;
    str_ver_num = get_version(Version);

    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z帳結構
    string str_z_filename;              //Z帳輸出檔名

    //目前Z帳時間
    string str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               str_vxpzno_seq;              //盤點人員讀帳次數


    string str_date_time, str_datez,str_cd_satncode,str_cs_entry;               //目前帳表系統時間
    str_date_time = g_date_time;


    sTmp = tif_ecr.ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須減1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
	_Strsprintf(str_tran_cnt,"%05d", _StrToInt(sTmp) - 1>0 ? _StrToInt(sTmp) - 1 : 1);


    str_z_date  = str_date_time;                                 //現在時間

    //str_pz_date = tif_ecr->ReadString("VXZ","PZ_DATE","");       //前次時間
    str_pz_date = tif_ecr.ReadString("Z","PZ_DATE",str_z_date);       //前次時間  2013/08/26 Update
	if (str_pz_date.empty())
		str_pz_date = str_z_date;

    str_datez = tif_ecr.ReadString("Z","DATEZ","");           //DDZ
    str_cd_satncode=tif_ecr.ReadString("VXZ","CD_SATNCODE","");
    str_cs_entry=tif_ecr.ReadString("VXZ","CS_ENTRY","");


    logsprintf("VXZDATA:ECR->WriteData 取的目前之交易序號:%s, CD_SATNCODE=%s, CS_ENTRY=%s",
                  str_tran_cnt.c_str(), str_cd_satncode.c_str(), str_cs_entry.c_str() );

    //將帳表日資料備份已被將來判斷殺目錄條件 james - 2003/02/07
    //tif_ecr->WriteString("Backup", "pz_date", str_pz_date);
    //備份 sal8.TXT條件
    //tif_ecr->WriteString("Backup", "datez", str_datez);

    /**********************

    019915 TM1 5/27 測TM130524R1版本--人員簽到造成日結序號錯誤。

    日結後收銀員登入日結序號00006。
    盤點人員登入後日結序號變成00001，
    另外還影響後續一般交易的日結序號也是00001？


    if ((str_pz_date == "") || (str_pz_date.Length() < 8))  //無相關資料，交易記錄歸零
    {
        tif_ecr->WriteString("Z", "pz_date", str_z_date);
        tif_ecr->WriteString("Z", "C", "0");
        tif_ecr->WriteString("Z", "nzno_seq", "1");
        tif_ecr->WriteString("Z", "pz_cnt", "00000");
        tif_ecr->WriteString("Z", "nz_cnt", "00001");
        tif_ecr->WriteString("Z", "datez", (str_date_time.SubString(7,2) + "1"));
        //20031225
        tif_ecr->WriteString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        tif_ecr->WriteString("Z", "zfilename", "");
        tif_ecr->WriteString("Z", "zrptname", "");
        tif_ecr->WriteString("Z", "deptname", "");
        tif_ecr->WriteString("Z", "inqtname", "");

        tif_ecr->WriteString("VXZ", "PZNO_SEQ", "0");
        tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");
        tif_ecr->WriteString("VXZ", "PZ_DATE", "");

        //tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        //tif_ecr->WriteString("VXZ","CS_ENTRY","");


        string str_xpath;               //指定收銀員帳表目錄
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date;
    }
    ****************************/

    str_pzno_seq   = tif_ecr.ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數 01~09  換日歸1
    str_nz_cnt     = tif_ecr.ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999
    str_am_tclsacc = tif_ecr.ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零
    str_vxpzno_seq = tif_ecr.ReadString("VXZ", "NZNO_SEQ", "1");


    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.length() < 5)
        str_nz_cnt = "00001";

    //sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_vxpzno_seq));
	_Strsprintf(sYYMMDDZZ,"%02d", _StrToInt(str_vxpzno_seq));
    
//#ifdef TM_20

	_Strsprintf(str_z_filename,"%s%02d%s%02d%02d.vdt",
		XZDATADIR,
        _StrToInt(g_ecr_no),
		str_pz_date.substr(2,6).c_str(),
		_StrToInt(str_pzno_seq),
		_StrToInt(str_vxpzno_seq)              );

//#else
    //輸出檔案 yyyymmddnn.zdt ex:2002010101.vdt
   
	
//#endif

    //換日Flag
    bool bl_change_day;

    //判斷換日
    if (str_pz_date.substr(0,8).compare(str_z_date.substr(1,8)))
        bl_change_day = true;
    else
        bl_change_day = false;

    z->no_tm = g_ecr_no;             //收銀機機號

    //日結序號
	_Strsprintf(z->bg_noclose,"%05d", _StrToInt(str_nz_cnt, "日結序號"));


    z->bg_dtclose = str_z_date.substr(0,8);      //結帳日期
    z->bg_tmclose = str_z_date.substr(8,6);      //結帳時間
    z->ed_dtclose = str_pz_date.substr(0,8);     //前次結帳日期
    z->ed_tmclose = str_pz_date.substr(8,6);     //前次結帳時間


    string z_a;
   
	_Strsprintf(z_a,"3092||%04d||%-6s||%-2s||%14s||%5s||%-10s||%8s||%6s||%8s||%6s||", 189,
                                      g_tencode.c_str(),
                                      z->no_tm.c_str(),           //收銀機機號
                                      str_date_time.c_str(),      //現在時間
                                      z->bg_noclose.c_str(),      //日結序號
                                      sYYMMDDZZ.c_str(),          //Z帳表序號
                                      z->ed_dtclose.c_str(),      //前次結帳日期
                                      z->ed_tmclose.c_str(),      //前次結帳時間
                                      z->bg_dtclose.c_str(),      //結帳日期
                                      z->bg_tmclose.c_str()      //結帳時間
                                    );


    // 03 交易明細ｆｉｅｌｄ  =============================================

    //本日來客數/本日交易品項  SumQt_trans() ==> 併入 SumQt_tcust  NextPOS
    int iC0TotCount = 0;
    int iSumQt_trans = 0;  //本日交易品項
	_Strsprintf(z->tb_trans.qt_tcust,"%08d", SumQt_tcust(0, gi_tot_line));
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項


    //本日交易品項
    //Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);  //NextPOS
    _Strsprintf(z->tb_trans.qt_trans, "%08d", iSumQt_trans - giI); // SumQt_trans(0, gi_tot_line) - giI);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;

    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  NextPOS
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line);
    TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(z->tb_trans.am_trcpntx,"%010d", giI - C01FTaxAmt - R4_C01FTaxAmt - (PreSal09_AmtNTX + R4PreSal09_AmtNTX));       // 發票日結帳額(免)
	_Strsprintf(z->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));       // 發票日結帳額(稅)
	_Strsprintf(z->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));       // 發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    //本日結帳累計金額
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));
          //int_acc_tot_amt=_StrToInt(str_am_tclsacc,"本日結帳金額累計");


	_Strsprintf(z->tb_trans.am_tclsacc,"%010d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    //TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
	_Strsprintf(z->tb_trans.qt_miss,"%08d", giA);       //  誤打發票次數
	_Strsprintf(z->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  誤打發票金額(免)
	_Strsprintf(z->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  誤打發票金額(稅)
	_Strsprintf(z->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));       //  誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //不開發票金額, 不開發票金額誤退重
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    //本日實際銷貨收入
	_Strsprintf(z->tb_trans.am_totsale,"%010d", int_totsale);


    if (gbl_tot_haveline)
    {
        z->no_rcstart = invoice_start_end(gi_tot_line, true);    //發票開始號碼
        z->no_rcend = invoice_start_end(gi_tot_line, false);     //發票結束號碼
    }
    else    //無發票資料
    {
        z->no_rcstart = "0000000000";                            //發票開始號碼
        z->no_rcend   = "0000000000";                            //發票結束號碼
    }


    string z_b;

    z_b += (z->tb_trans.qt_tcust+SGM);             //本日來客數
    z_b += (z->tb_trans.qt_trans+ SGM);             //本日交易品項
    z_b += (z->tb_trans.am_tclsacc+ SGM);           //本日結帳累計金額
    z_b += (z->no_rcstart+ SGM);                   //發票開始號碼
    z_b += (z->no_rcend+ SGM);                      //發票結束號碼
    z_b += (z->tb_trans.am_trcpntx+ SGM);          //發票日結帳額(免)
    z_b += (z->tb_trans.am_trcptax+ SGM);           //發票日結帳額(稅)
    z_b += (z->tb_trans.am_trcptot+ SGM);          //發票日結帳金額
    z_b += (z->tb_trans.qt_miss+ SGM);              //誤打發票次數
    z_b += (z->tb_trans.am_missntx+ SGM);           //誤打發票金額(免)
    z_b += (z->tb_trans.am_misstax+ SGM);           //誤打發票金額(稅)
    z_b += (z->tb_trans.am_miss+ SGM);              //誤打發票金額
    z_b += (z->tb_trans.am_totsale+ SGM);           //本日實際銷貨收入


    /* 03 代售付ｔａｂｌｅ       *///===========================================

    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                 //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);     //giH giI 食安退貨金額、次數    //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(z->tb_newadd.qt_agnrcv,"%08d", giD);             //代收費用次數
	_Strsprintf(z->tb_newadd.am_agnrcv,"%010d", giC);             //代收費用金額


    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giH;  iSub_C0InvoAmtC=giJ;  iSub_C0InvoQtyC=giK;

    string z_c;
    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    //Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //代售合計次數
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //代售合計金額   無須加入預售商品
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;
    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

	_Strsprintf(z_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //代現金ｔａｂｌｅ=============================================
    int  iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    SumTbCash(0,gi_tot_line);   //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//消費券
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

    writelog("giS:消費券金額 , giT:消費券回數");

	_Strsprintf(z->tb_cash.qt_bonus,"%08d", giA); //-giT);            //禮券回數
	_Strsprintf(z->tb_cash.am_bonus,"%08d", giB); //-iCSMAmt);            //禮券金額


    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

	_Strsprintf(z->tb_cash.qt_vndcpn,"%08d", 0);             //廠商代現金回數 31
	_Strsprintf(z->tb_cash.am_vndcpn,"%010d", 0);             //廠商代現金金額 32

	_Strsprintf(z->tb_cash.qt_stocpn,"%08d", giC);           //門市代現金回數
	_Strsprintf(z->tb_cash.am_stocpn,"%010d", giD); //+iPreSalCPNDisAmt);           //門市代現金金額
    SumCpn41 +=  giD;         //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

	_Strsprintf(z->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //代現金合計回數
	_Strsprintf(z->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //代現金合計回數
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //代現金合計金額

	_Strsprintf(z->tb_cash.qt_bonout,"%08d", giG);           //溢收合計回數   37
	_Strsprintf(z->tb_cash.am_bonout,"%010d", giH);           //溢收合計金額  38

    iTotOt2Rtn=giE;                                      //餘額券
    iqt_ffu1=giI;                                        //現金卡回數  電子錢 1847
    iam_ffu1=giJ;                                        //現金卡金額  電子錢 1855    刷卡合計

    string z_d;

    z_d += (z->tb_cash.qt_bonus+SGM);                //禮券回數
    z_d += (z->tb_cash.am_bonus+ SGM);                //禮券金額
    z_d += (z->tb_cash.qt_vndcpn+ SGM);               //廠商折價券回數
    z_d += (z->tb_cash.am_vndcpn+ SGM);               //廠商折價券金額
    z_d += (z->tb_cash.qt_stocpn+ SGM);               //折價券回數
    z_d += (z->tb_cash.am_stocpn+ SGM);               //折價券金額
    z_d += (z->tb_cash.qt_totcash+ SGM);              //代現金合計回數
    z_d += (z->tb_cash.am_totcash+ SGM);              //代現金合計金額

	_Strsprintf(sTmp,"%08d", giF);
    z_d += (sTmp+ SGM);                               //餘額合計回數
	_Strsprintf(sTmp,"%010d", giE);
    z_d += (sTmp+ SGM);                               //餘額合計金額

    z_d += (z->tb_cash.qt_bonout+ SGM);               //溢收合計回數
    z_d += (z->tb_cash.am_bonout+ SGM);               //溢收合計金額

    //==========================================================================


    //現金合計 含 消費券

    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(z->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);

    // writelog("現金合計 含 消費券");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //現金合計
    //strcat(ze,z->no_rcstart.c_str());      //發票開始號碼
    //strcat(ze,z->no_rcend.c_str());        //發票結束號碼
    //ze[28] = 0;


    // Z帳部門別集計TABLE      =============================================

	string z_f, z_a1093, z_ammdis;
	
	// 部門促銷折扣金額        Lu 2018/06/13
	_Strsprintf(z_a1093, "3093||%04d||%-6s||%-2s||%14s||%5s||%-10s||", 37,
		g_tencode.c_str(),
		z->no_tm.c_str(),           //收銀機機號
		str_date_time.c_str(),      //現在時間
		z->bg_noclose.c_str(),      //日結序號
		sYYMMDDZZ.c_str()          //Z帳表序號
		);
	z_ammdis = "";


    //s1部門合計交易項數
    //s2部門合計銷售金額
    //s3部門合計誤退項數
    //s4部門合計誤退金額
    int s1, s2, s3, s4;

    s1 = s2 = s3 = s4 = 0;
    DEPT *dept;
    try
    {
        dept = new DEPT[20];
        //先將值填零
        for (int i=0; i<20; i++)
        {
			_Strsprintf(z->tb_depcal[i].am_dpsitm,"%08d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpsale,"%010d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpmitm,"%08d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpmiss,"%010d", 0);

            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;

			dept[i].am_mmDis = 0;         // 部門促銷折扣金額        Lu 2018/06/13
			dept[i].am_mmDismiss = 0;     // 部門促銷折扣誤退金額    Lu 2018/06/13
        }

        //writelog("INIT 部門交易資料, 部門帳");
        //計算所有部門交易資料, 部門帳


          if (gbl_rec_haveline)
            SumAllDept(dept);


        // writelog("計算所有部門交易資料, 部門帳 OK");
         ///////////////////////////////////////////////////////////////////////////////////////////////
        //部門01交易項數 89
        // ~
        //部門20誤退金額 168
        ////////////////////////////////////////////////////////////////////////////////////////////////

        for (int i=0; i<20; i++)
        {
			_Strsprintf(z->tb_depcal[i].am_dpsitm,"%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+SGM);
            s1 += dept[i].am_dpsitm;

			_Strsprintf(z->tb_depcal[i].am_dpsale,"%010d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+ SGM);
            s2 += dept[i].am_dpsale;

			_Strsprintf(z->tb_depcal[i].am_dpmitm,"%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+ SGM);
            s3 += dept[i].am_dpmitm;

			_Strsprintf(z->tb_depcal[i].am_dpmiss,"%010d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+ SGM);
            s4 += dept[i].am_dpmiss;

			// 部門促銷折扣金額        Lu 2018/06/13
			_Strsprintf(sTmp,"%010d", dept[i].am_mmDis - dept[i].am_mmDismiss);
			z_a1093 += (sTmp + SGM);

			_Strsprintf(sTmp,"%010d||%010d", dept[i].am_mmDis, dept[i].am_mmDismiss);
			z_ammdis += (sTmp + SGM);

        }
		// 部門促銷折扣金額        Lu 2018/06/13
		_Strsprintf(sTmp, "%s\n", "0||1||2||3||4||5||6||7||8||9||");
		z_a1093 += (sTmp);
		z_ammdis += (sTmp);

    }
	catch (...)
    {
		;;
    }
	delete[] dept;
    //writelog("Z帳部門別集計TABLE OK");

    string z_g;

	z_g += (_Strsprintf(z->am_tdpsitm,"%08d", s1) + SGM);             // 部門合計交易項數
	z_g += (_Strsprintf(z->am_dpstot,"%010d", s2) + SGM);             // 部門合計銷售金額
	z_g += (_Strsprintf(z->am_tdpmitm,"%08d", s3) + SGM);             // 部門合計誤退項數
	z_g += (_Strsprintf(z->am_dpmtot,"%010d", s4) + SGM);             // 部門合計誤退金額
    z_g += (z->am_cash+ SGM);                                         // 現金合計含消費券 45


    //收銀員明細ｆｉｅｌｄ=============================================

    string z_h;

    _Strsprintf(sTmp,"%010d", VisuilCpnAmt);
    z_h += (sTmp+ SGM);                                            //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // 借零合計金額
	_Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    z_h += (sTmp+ SGM);                                            // 代收商品折讓單折扣  借零合計金額

	z_h += (_Strsprintf(z->tb_casher.qt_feedtot, "%08d", 0) + SGM);    // 投零次數
	z_h += (_Strsprintf(z->tb_casher.am_feedtot, "%010d", 0) + SGM);    // 投零合計金額

    //投庫
    int int_drop_line = (gtsl_drop->size() - 1);
    int int_drops = 0;           //投庫次數
    int int_drop_money = 0;       //投庫金額
    int int_drop_sum = 0;    //投庫總額
    int x_rtn_cnt=0;

    string   s1041;
    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
        //StringSegment_EX(s, "|", 15)
		s1041 = *ls; // gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
			   logsprintf("產生日結帳表:非投庫(%s)資料", s1041.c_str());
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, SGM, 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //IC付款
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //信用卡
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 12));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 13));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //提貨單

		//中獎發票金額  500
		int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 16));

        //中獎發票金額  1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 27));

        //中獎發票金額  200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 15));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 18)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 20)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 22)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 24)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 26)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 28)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 30)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 32)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 34)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 36)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 38)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 40)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 42)); //費用 13金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 44)); //費用 14金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數
    }

	z_h += (_Strsprintf(z->tb_casher.qt_droptot,"%08d", int_drops) + SGM);        // 投庫次數
	z_h += (_Strsprintf(z->tb_casher.am_droptot,"%010d", int_drop_sum) + SGM);     // 投庫合計金額

    //writelog("投庫合計金額 OK");

    //更正次數、金額
    SumUpdat(0,gi_rec_line);
	z_h += (_Strsprintf(z->tb_casher.qt_update,"%08d", giA) + SGM);          // 更正次數
	z_h += (_Strsprintf(z->tb_casher.am_update,"%010d", giB) + SGM);          // 更正合計金額


    //交易取消
    SumVoid(0,gi_tot_line);
	z_h += (_Strsprintf(z->tb_casher.qt_void,"%08d", giA) + SGM);            // 交易取消次數
	z_h += (_Strsprintf(z->tb_casher.am_void,"%010d", giB) + SGM);            // 交易取消合計金額


    //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = stoi(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_CNT", "0"));
		int_ZINI_TRAN_MODE_AMT = stoi(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_AMT", "0"));

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr.WriteString("ECR", "ZINI_TRAN_MODE_CNT", "0");
        tif_ecr.WriteString("ECR", "ZINI_TRAN_MODE_AMT", "0");
    }
	catch(...)
    {
        ;;
    }


	z_h += (_Strsprintf(z->tb_casher.qt_train,"%08d", int_ZINI_TRAN_MODE_CNT) + SGM); // 訓練合計次數
	z_h += (_Strsprintf(z->tb_casher.am_train,"%010d", int_ZINI_TRAN_MODE_AMT) + SGM); // 訓練合計金額

    //手動輸入部門13、14
    SumInpdp(0,gi_rec_line);
	z_h += (_Strsprintf(z->tb_newadd.qt_inpdp,"%08d", giA) + SGM);            //手動輸入部門次數
	z_h += (_Strsprintf(z->tb_newadd.am_inpdp,"%010d", giB) + SGM);            //手動輸入部門金額

	z_h += (_Strsprintf(z->tb_casher.qt_openbox,"%08d", gi_open_z) + SGM);     // 開錢櫃次數

    //折扣讓次數   //小計折扣
    //SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	z_h += (_Strsprintf(z->tb_casher.qt_disc,"%08d", giC) + SGM);             // 折扣次數(應稅)
	z_h += (_Strsprintf(z->tb_casher.am_disc,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + SGM);            // 折扣金額(應稅)

    //pay_sub_amt(0,gi_pay_line);
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu6,"%08d", giA) + SGM);            //促銷折扣/讓次數
	z_h += (_Strsprintf(z->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + SGM);             //促銷折扣/讓金額

    //加值機
    easy_card(0,gi_easy_line);
	z_h += (_Strsprintf(z->tb_casher.qt_disret1,"%08d", giA) + SGM);          // 加值機開/售卡次數
	z_h += (_Strsprintf(z->tb_casher.am_disret1,"%010d", giB) + SGM);         // 加值機開/售卡金額
	z_h += (_Strsprintf(z->tb_casher.qt_disret2,"%08d", giC) + SGM);          // 加值機加值次數
	z_h += (_Strsprintf(z->tb_casher.am_disret2,"%010d", giD) + SGM);          // 加值機加值金額

    //fetc_card(0,gi_fetc_line);   //e通卡    2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    z_h += (_Strsprintf(z->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + SGM);    //代售,手續費銷售金額(免)
	z_h += (_Strsprintf(z->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + SGM); //代售,手續費作廢金額(免)


    //食安次數、金額  int iqt_food,iamt_food;
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu9,"%08d", iqt_food) + SGM);    //食安退款筆數  71
	z_h += (_Strsprintf(z->tb_newadd.am_ffu9,"%010d", iamt_food) + SGM);  //食安退款金額  72

    //   05 追加 table        =============================================

    string z_i;

	z_i += (_Strsprintf(z->tb_newadd.qt_cashCard,"%08d", iqt_ffu1) + SGM);    //刷卡/現金卡次數    電子錢 1847
	z_i += (_Strsprintf(z->tb_newadd.am_cashCard,"%010d", iam_ffu1) + SGM);    //刷卡/現金卡金額    電子錢  刷卡合計 1855
	z_i += (_Strsprintf(z->tb_newadd.qt_parking,"%08d", 0) + SGM);            //代收停車費次數
	z_i += (_Strsprintf(z->tb_newadd.am_parking,"%010d", 0) + SGM);            //代收停車費金額
    z_i += (z->tb_newadd.qt_agnrcv+ SGM);                                //代收費用次數  1831
    z_i += (z->tb_newadd.am_agnrcv+ SGM);                                //代收費用金額  1839
	z_i += (_Strsprintf(z->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + SGM);      //版本前置符號
	z_i += (_Strsprintf(z->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + SGM);      //版本號碼
	z_i += (_Strsprintf(z->tb_newadd.qt_rcp0,"%010d", TotZero(0, gi_tot_line)) + SGM);             //發票金額0次數

    //不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);

	z_i += (_Strsprintf(z->tb_newadd.am_ffu4,"%08d", int_novoice_amt) + SGM);        //不開發票金額 82
	z_i += (_Strsprintf(z->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + SGM);    //  不開發票金額誤退重 83

    SumLOSS(0,gi_rec_line);
	z_i += (_Strsprintf(z->tb_newadd.qt_ffu5,"%08d", giA) + SGM);             // TM廢棄Loss值次數
	z_i += (_Strsprintf(z->tb_newadd.am_ffu5,"%010d", giB) + SGM);             // TM廢棄Loss值金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計
	z_i += (_Strsprintf(z->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + SGM);   //手續費合計 86

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    // z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", giB+iSub_C0InvoAmtC)+"|");             //原加值機加值取消次數 86
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	z_i += (_Strsprintf(z->tb_newadd.qt_ffu7,"%010d", iSumRelDimAgn + iC0InvoAmt4VDCN) + SGM);   //原加值機加值取消次數 87

    //消費券
	z_i += (_Strsprintf(z->tb_newadd.am_posZRpt,"%010d", iCSMAmt) + SGM);      //消費券 88

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //部門01交易項數 89
    // ~
    //部門20誤退金額 168
    ////////////////////////////////////////////////////////////////////////////////////////////////

    string z_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);      即時購與銷轉進值應稅 169
    z_k +=sTmp;   //"00000000|";
	_Strsprintf(sTmp,"%010d||", giD);     //即時購與銷轉進值免稅 170
    z_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

	_Strsprintf(sTmp,"%08d||", giA / 200); //發票兌獎張數
    z_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", giA);     //發票兌獎金額
    z_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);  //電子禮券回數
    z_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //電子禮券金額
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
	_Strsprintf(sTmp,"%010d||%010d||%08d||%s||%-10s||0||1||2||3||4||5||6||7||8||9||",
     iC0TotAmt, iC0GivUpAmt, iC0TotCount, "W", "Tran_cnt");  // 代售手續費應稅金額 175 ~ 代售手續費來客數 177
   
    z_k +=sTmp;

    //z_h += z->tb_casher.qt_pack.sprintf("%08d", iTotOt2Rtn); //giE);     // 加值機結帳次數  2007/12/01 找餘額券

    //z_i += z->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix);          //代收來客數

    //z_i += z->tb_newadd.qt_posZRpt.sprintf("%08d", 0);            //扣卡機扣卡次數

    //z_i += z->tb_newadd.qt_ffu10.sprintf("%08d",0); //giFetcSumCnt);     //備用十次數  FETC 結帳次數 保留
    //z_i += z->tb_newadd.am_ffu10.sprintf("%08d",0); //giFetcSumAmt);     //備用十金額  FETC 結帳金額 保留




    //if (z_a.Length()!=34)
    //{
    //    writelog( ("z_a 長度應為 34 目前長度 " + IntToStr(z_a.Length())) );
    //    z_a = z_a.SubString(1,34);
    //}

    //if (z_b.Length()!=88)
    //{
    //    writelog( ("z_b 長度應為 88 目前長度 " + IntToStr(z_b.Length())) );
    //    z_b = z_b.SubString(1,88);
    //}

    //if (z_c.Length()!=720)
    //{
    //    writelog( ("z_c 長度應為 720 目前長度 " + IntToStr(z_c.Length())) );
    //    z_c = z_c.SubString(1,720);
    //}

    //if (z_d.Length()!=80)
    //{
    //    writelog( ("z_d 長度應為 80 目前長度 " + IntToStr(z_d.Length())) );
    //    z_d = z_d.SubString(1,80);
   // }

   // if (strlen(ze)!=28)
   // {
   //     writelog( ("z_e 長度應為 29 目前長度 " + IntToStr(strlen(ze))) );
   //     ze[28] = 0;
   // }

   // if (z_f.Length()!=640)
   // {
   //     writelog( ("z_f 長度應為 640 目前長度 " + IntToStr(z_f.Length())) );
   //     z_f = z_f.SubString(1,640);
   // }

   // if (z_g.Length()!=32)
   // {
  //      writelog( ("z_g 長度應為 32 目前長度 " + IntToStr(z_g.Length())) );
  //      z_g = z_g.SubString(1,32);
  //  }

  //  if (z_h.Length()!=168)
  //  {
  //      writelog( ("z_h 長度應為 168 目前長度 " + IntToStr(z_h.Length())) );
  //      z_h = z_h.SubString(1,168);
  //  }

  //  if (z_i.Length()!=208)
  //  {
  //      writelog( ("z_i 長度應為 208 目前長度 " + IntToStr(z_i.Length())) );
  //      z_i = z_i.SubString(1,208);
  //  }


    string DDZ_1099;
    DDZ_1099=str_datez;
    // 2013/08/26  Update
	if (_StrToInt(DDZ_1099.substr(0, 2)) < 1 || _StrToInt(DDZ_1099.substr(0, 2)) > 31)
       DDZ_1099=_StringSegment_EX(gsLast1099, SGM, 7).substr(2,3) ; //DDZ_1099=str_datez;

    //記錄Z PATH於INI
    string str_zdt_path, str_zrp_path;


    //Z報表
    string str_zrp;
        
	_Strsprintf(str_zrp,"%s%s%s%s%s%s%s%s%s\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str()
                            );

	logsprintf("VXZDATA::(%s) Len=%d", str_zrp.c_str(), str_zrp.length());
    //report = new REPORT();      //日結帳表

    //Z報表
	strcpy_s(report->char_filename, sizeof(report->char_filename), _ChangeFileExt(str_z_filename, string(".vrp")).c_str());

    if( _StrToInt(str_cs_entry)==1)
        report->CreateReport("V", str_zrp, StrBillPay, g_tencode, g_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL,0,"" );
   else
        report->CreateReport("v", str_zrp, StrBillPay, g_tencode, g_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0, "");
    //備份Z報表
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //部門帳表
    string str_dpt_path;
	strcpy_s(report->char_filename, sizeof(report->char_filename), _ChangeFileExt(str_z_filename, string(".vpt")).c_str());

    if (_StrToInt(str_cs_entry) == 1)
    {
        report->CreateReport("E", str_zrp, StrBillPay, g_tencode, g_ecr_no,
            0, 0, 0, 0, tsl_subsale, 0, z_ammdis);
    }
    else
    {
        report->CreateReport("e", str_zrp, StrBillPay, g_tencode, g_ecr_no,
            0, 0, 0, 0, tsl_subsale, 0, z_ammdis);
    }
    //備份部門帳表
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    //寫入SAL檔98Type
    string str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_p_z_cnt,
               str_n_z_cnt,
               str_start_trans,            //開始交易序號
               str_am_tclascc,
               str_am_totsale,
               str_zdt_no,
               str_ver,
               str_sal_last_1099,
               SaleDataVer;

    str_rec_type = "3015";
    str_rec_len = "0023";
    str_tencode = g_tencode;
	_Strsprintf(str_ecr_no,"%02d", _StrToInt(z->no_tm, "z->no_tm"));
    str_sys_date = (z->bg_dtclose + z->bg_tmclose);      //日期時間

    //98交易序號 //20031215
    int int_sum_tran_cnt ;

    //= tif_ecr->ReadInteger("ECR","TRAN_CNT",1);
    //str_tran_cnt.sprintf("%05d",int_sum_tran_cnt);

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

    //if (str_tran_cnt.Length() < 5)
    //if( Trim(str_tran_cnt)=="00000")
    //if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
    //    str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    //sLog.sprintf("VXZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    //writelog( sLog);


    string str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
		_Strsprintf(str_sum_tran,"%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //前次日結序號，目前日結序號
    //str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "前次日結序號"));
    //str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "目前日結序號"));


    if (tif_ecr.ReadString("Z","PZNO_SEQ","").length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr.ReadString("Z","NZNO_SEQ","").length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc;

    string str_sal_first;
    str_sal_first = gsal_first; // gpchar_sal_first;

     //開始交易序號
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, SGM, 7).substr(TRANCNT_NO_POS-1,5));
	_Strsprintf(str_start_trans,"%05d", i_tran_cnt);

    int_am_tclsacc=int_acc_tot_amt;
	_Strsprintf(str_am_tclascc,"%08d", int_am_tclsacc);          //本日結帳累計金額
    str_am_totsale = z->tb_trans.am_totsale;                //本日實際銷貨收入
	_Strsprintf(str_zdt_no, "%010ld", _StrToInt(z->bg_noclose));     //ZDT序號代入Z檔名
    str_ver = "FFFFFFFF|";


    string str_trans1_5;    //交易序號前五碼

    string str_sal_last;
    str_sal_last = gsal_last;                      // gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    string str_01,str_99;
    _Strsprintf(str_01,"%4s||%4s||%-6s||%-2s||%14s||%5s||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                   "3001",
                   "0033",    
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   " ",                                   //部門次數
                   " ",                                   //部門金額
                   " ",                                   //交易混登區分 
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                   //前次交易機號    
                   );



    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);
    _Strsprintf(str_99, "%4s||%4s||%-6s||%-2s||%14s||%5s||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%05d||%10d||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
	               "3099",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   0,                                     //部門次數
                   0,                                     //部門金額
                   " ",                                   //交易混登區分 
                   g_orgtencode.c_str(),                      //原始店號
                   " ",                                   //保留
                   " "                                    //前次交易機號
                                       );



    string _str_tran_cnt;        //交易序號
    string str_st_tran_cnt;     //開始交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    string str_98;  //寫入3015交易序號
	_Strsprintf(str_98,"%4s||%4s||%6s||%2s||%14s||%-5s||%-10s||%4s||%2s||%-20s||%1s||%-30s||%10s||%s||\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        "0000",
                        "01",
                        SalesNo.c_str(),      //收銀員代碼
                        str_cs_entry.c_str(),            //1,2
                        str_cd_satncode.c_str(),
                        "        ",
                        "0||1||2||3||4||5||6||7||8||9||0"
                        );



    string str_sal;
    //str_sal = str_01 + "\n"+ str_zrp + str_98 + str_99;
	str_sal = str_01 + "\n" + str_98 + str_zrp + z_a1093 + str_99;

    if (w_WriteSAL(str_sal, str_z_filename, 0) == OK)
       {
        //備份ZDT帳
        str_zdt_path = BackupFile(str_z_filename,"", DDZ_1099);
        tif_tmp.WriteString("VXZ","x1092filename", str_zdt_path);
       }
    else
      {
       tif_tmp.WriteString("VXZ","x1092filename", "");
      }

    /////////////////////////////////////////////////////////////////

    string StrZslFile;  //WriteSAL_A
    if (w_WriteSAL(str_sal, SalFileName, 1) == OK)  //1 a+
       {
         //備份ZSL帳
		   string sas = SalFileName;
		   sTmp = _ChangeFileExt(sas, string(".zzz"));

		   StrZslFile = BackupFile(SalFileName, sTmp, DDZ_1099); // for test
         //tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);
         ;;
       }
    else
       {
         //tif_tmp->WriteString("Z","ZSLNAME","");
         ;;
       }


    string str_ipt_path;

    //更改XZDATA.INI
    int int_vxnzno_seq   = _StrToInt(str_vxpzno_seq, "vxpzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp.WriteString("VXZ","ZFILENAME" , str_zdt_path);
    tif_ecr.WriteString("VXZ","ZFILENAME" , str_zdt_path);

    tif_tmp.WriteString("VXZ","ZRPTNAME"  , str_zrp_path);
    tif_ecr.WriteString("VXZ","ZRPTNAME"  , str_zrp_path);

    tif_tmp.WriteString("VXZ","DEPTNAME"  , str_dpt_path);
    tif_ecr.WriteString("VXZ","DEPTNAME"  , str_dpt_path);

    tif_tmp.WriteString("VXZ","PZ_DATE"     , str_z_date);
    tif_ecr.WriteString("VXZ","PZ_DATE"     , str_z_date);

    //tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

    //tif_tmp->WriteString("VXZ","PZ_DATE"     , str_z_date);
    //tif_tmp->WriteInteger("Z","AM_TCLSACC" , int_am_tclsacc);
    //tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT 交易序號

    //tif_tmp->WriteString("ECR","tot_open_z", "0");
    //tif_ecr->WriteString("ECR","tot_open_z", "0");
    /*
    if (bl_change_day) //判斷是否換日
    {
        tif_ecr->WriteString("Z","PZNO_SEQ", "0");
        tif_ecr->WriteString("Z","NZNO_SEQ", "1");
        tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);
        if (++int_nz_cnt > 99999)
            int_nz_cnt = 1;

        str_nz_cnt.sprintf("%05d",int_nz_cnt);
        tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));
    }
    else
    {
    */
        tif_ecr.WriteString("VXZ","PZNO_SEQ", str_vxpzno_seq);

        if (++int_vxnzno_seq > 99)
            int_vxnzno_seq = 1;

		_Strsprintf(str_vxpzno_seq,"%d", int_vxnzno_seq);

        tif_ecr.WriteString("VXZ","NZNO_SEQ", str_vxpzno_seq);
        //tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);

        //if (++int_nz_cnt > 99999)
        //    int_nz_cnt = 1;

        //str_nz_cnt.sprintf("%05d", int_nz_cnt);

        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        //tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
        //                      + FormatFloat("00",int_nzno_seq) + "00"));
        //tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));
    //}



    str_pz_date    = str_z_date;
    //str_pzno_seq   = tif_ecr->ReadString("Z","NZNO_SEQ","");      //目前Z帳次數   換日歸1

    // tif_ecr->WriteString("VXZ", "nzno_seq", "1");


    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";

    //指定X 帳PATH
    //string str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    //tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr.WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印

    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

	_Strsprintf(str_sum_tran,"%05d", int_sum_tran_cnt);

    tif_tmp.WriteString("ECR", "TRAN_CNT", str_sum_tran);
    tif_ecr.WriteString("ECR", "TRAN_CNT", str_sum_tran);

	logsprintf("VXZDATA::寫入交易序號 ECR->TRAN_CNT=%s", str_sum_tran.c_str());

    return OK;
}



VXZDATA::VXZDATA()
{
    //
    writelog("盤點人員讀帳 開始");
    z = new SPOS_ZRPDT;         //Z帳結構
    //tif_tmp = new TIniFile(ZDATATMP_INI);
    //tif_ecr = new TIniFile(XZDATA_INI);
	tif_ecr.setINIFileName(XZDATA_INI);
	tif_tmp.setINIFileName(ZDATATMP_INI);
    report = new REPORT();      //日結帳表

}

VXZDATA::~VXZDATA()
{
    delete z, tif_ecr;

    delete report;      //日結、部門帳表
    //delete tif_tmp;
    //mtn_dat_folder();   //維護DAT目錄，保存三十日以內資料
 
}


///////////////////////////////////////////////////////////////////////////////
//自動日結Section

string  __fastcall AUTOZDATA::GetSysDttmMaxTransNo(string sDttm)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    string sTmp, sRec, sTmpDttm, RecNo, tmpTrans, rtns;

 try
   {
      rtns="";
      if( Auto_Sal->size() > 0)
       {
        //for (int i=0;  i<Auto_Sal->Count; i++)  
		   for (list<string>::iterator ls = Auto_Sal->begin(); ls != Auto_Sal->end(); ls++)
            {
				sRec = *ls; // Auto_Sal->Strings[i];
             RecNo=_StringSegment_EX(sRec, "|", 1);
             if( RecNo!="1001")
                 continue;
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             tmpTrans=_StringSegment_EX(sRec, "|", 7);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( sDttm != "" && sTmpDttm.substr(0,8) == sDttm )
                  {
                    rtns=tmpTrans;
                    break;
                  }
            }
        }

   }
 catch(...)
   {
     writelog("AUTOZDATA:: GetSysDttmMaxTransNo Auto_Sal Catch error...");
   }
return(rtns);

}




int __fastcall AUTOZDATA::GetYYYYMDD_SalData(string &AutoZCnt, string AutoZdt , int AutoZ)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    string sTmp, sRec, sTmpDttm,TmpZcnt, SalZcnt, RecNo;
    int irtn=0;
    TStringList *sTmlLst = new TStringList();
    SalZcnt=AutoZCnt;

 try
   {
    if( AutoZ ==1)
      {
	   sTmlLst->assign(Auto_Sal->begin(), Auto_Sal->end() );
       if( sTmlLst->size() > 0)
       {
        Auto_Sal->clear();
        //for (int i=0;  i<sTmlLst->Count; i++)  //for (list<string>::iterator ls = gtsl_1051_sal->begin(); ls != gtsl_1051_sal->end(); ls++)
		for (list<string>::iterator ls = sTmlLst->begin(); ls != sTmlLst->end(); ls++)
            {
				sRec = *ls; // sTmlLst->Strings[i];
             RecNo=_StringSegment_EX(sRec, "|", 1);
             if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
               TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //日結序號
             else
               TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號

             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt )
                  {
                    if( RecNo=="1001" )   //取得 1001 中最大日結序號
                      {
                        SalZcnt=TmpZcnt;
                        AutoZCnt=SalZcnt;
                      }
                  }
             // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt )
               {
                 Auto_Sal->push_back(sRec);
               }
             else
               {
                   ;;
               }
            }
        }

      } // end of  if( AutoZ ==1)
   }
 catch(...)
   {
     writelog("AUTOZDATA::GetYYYYMDD_SalData Auto_Sal Catch error...");
   }

  delete  sTmlLst;

  //////////////////////////////

 try
   {
    gtsl_rec->clear();
    //gtsl_rec->Assign(Autotsl_rec);
    if( Autotsl_rec->size() > 0)
       {
        //for (int i=0;  i<Autotsl_rec->Count; i++)   
		   for (list<string>::iterator ls = Autotsl_rec->begin(); ls != Autotsl_rec->end(); ls++)
            {
			 sRec = *ls; // Autotsl_rec->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt)
               {
                 gtsl_rec->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_rec->Delete(i);
               }


            }
       }
    irtn+=1;

    gtsl_tot->clear();
    //gtsl_tot->Assign(Autotsl_tot);
    if( Autotsl_tot->size() > 0)
       {
         //for (int i=0;  i<Autotsl_tot->Count; i++) 
		   for (list<string>::iterator ls = Autotsl_tot->begin(); ls != Autotsl_tot->end(); ls++)
            {
			 sRec = *ls; // Autotsl_tot->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                    //AutoZCnt=SalZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt)
               {
                 gtsl_tot->push_back(sRec);
                 //sTmp.sprintf("AUTOZDATA::GetYYYYMDD_SalData:(%s)", sRec.c_str() );
                 //writelog(sTmp);
               }
             else
               {
                 ;;//gtsl_tot->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_drop->clear();
    //gtsl_drop->Assign(Autotsl_drop);
    if( Autotsl_drop->size() > 0)    
       {
		  // for (int i = 0; i<Autotsl_drop->Count; i++) 
		   for (list<string>::iterator ls = Autotsl_drop->begin(); ls != Autotsl_drop->end(); ls++)
            {
		     sRec = *ls; // Autotsl_drop->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt)
               {
                 gtsl_drop->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_drop->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_easy_card->clear();
    //gtsl_easy_card->Assign(Autotsl_easy_card);
    if( Autotsl_easy_card->size() > 0)
       {
        //for (int i=0;  i<Autotsl_easy_card->Count; i++)
		   for (list<string>::iterator ls = Autotsl_easy_card->begin(); ls != Autotsl_easy_card->end(); ls++)
            {
			 sRec = *ls; // Autotsl_easy_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt )
               {
                 gtsl_easy_card->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_easy_card->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_pay_sub->clear();
    //gtsl_pay_sub->Assign(Autotsl_pay_sub);
    if( Autotsl_pay_sub->size() > 0)
       {
           //for (int i=0;  i<Autotsl_pay_sub->Count; i++) 
		   for (list<string>::iterator ls = Autotsl_pay_sub->begin(); ls != Autotsl_pay_sub->end(); ls++)
            {
		   	 sRec = *ls; // Autotsl_pay_sub->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(1,8) <= AutoZdt)
               {
                 gtsl_pay_sub->push_back(sRec);
               }
             else
               {
                 ;; //gtsl_pay_sub->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_bil->clear();
    //gtsl_bil->Assign(Autotsl_bil);
    if( Autotsl_bil->size() > 0)
       {
        //for (int i=0;  i<Autotsl_bil->Count; i++) 
		   for (list<string>::iterator ls = Autotsl_bil->begin(); ls != Autotsl_bil->end(); ls++)
            {
			 sRec = *ls; // Autotsl_bil->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt)
               {
                gtsl_bil->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_bil->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_fetc_card->clear();
    //gtsl_fetc_card->Assign(Autotsl_fetc_card);
    if( Autotsl_fetc_card->size() > 0)
       {
        //for (int i=0;  i<Autotsl_fetc_card->Count; i++)
		   for (list<string>::iterator ls = Autotsl_fetc_card->begin(); ls != Autotsl_fetc_card->end(); ls++)
            {
			 sRec = *ls; // Autotsl_fetc_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt)
               {
                 gtsl_fetc_card->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_fetc_card->Delete(i);
               }
            }
       }
    irtn+=1;

    tsl_subsale->clear();
    //tsl_subsale->Assign(Autotsl_subsale);
    if( Autotsl_subsale->size() > 0)
       {
        //for (int i=0;  i<Autotsl_subsale->Count; i++)
		   for (list<string>::iterator ls = Autotsl_subsale->begin(); ls != Autotsl_subsale->end(); ls++)
            {
			 sRec = *ls; // Autotsl_subsale->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.substr(0,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.substr(0,8) <= AutoZdt)
               {
                 tsl_subsale->push_back(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;
    } // end of try
  catch(...)
    {
      writelog("AUTOZDATA::GetYYYYMDD_SalData Catch error...");
      irtn=0;
    }

    return(irtn);

}




int __fastcall AUTOZDATA::GetZDTData(string StrZCnt, string StoreNO,  string EcrNO, string SalesNo ,
                          string Version, string PZ_date, string Z_date, string DDZ_1099, string ZDT_1050,
                          string &RtnZDTData, string &Tclsacc,
                          string &str_zrp_path, string &str_dpt_path)
{
    int int_reslut = 0;
    string sLog, sTmp,str_ver_num, str_tran_cnt, ZDT_1050_DDZ;

    string str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               str_date_time, str_datez,    //目前帳表系統時間
               z_a;

    sTmp=_StringSegment_EX(ZDT_1050, SGM, 7);  //交易序號
    ZDT_1050_DDZ=sTmp.substr(2,3);
    str_am_tclsacc=Tclsacc;
    str_z_date=Z_date;
    str_pz_date=PZ_date;
    str_ver_num=Version ;
    //sYYMMDDZZ.sprintf("%s%s", str_pz_date.SubString(3,6).c_str(),StrZCnt );
	_Strsprintf(sYYMMDDZZ, "%s%s", str_z_date.substr(4, 4).c_str(), StrZCnt.c_str());


    logsprintf("** AUTOZDATA::GetZDTData 產生日結帳表 Version=%s, ZDT:StrZCnt=%s, PZ_date=%s, Z_date=%s, DDZ_1099=%s, Tclsacc=%s **",
		Version.c_str(), StrZCnt.c_str(), PZ_date.c_str(), Z_date.c_str(), DDZ_1099.c_str(), Tclsacc.c_str());

    int i,j;
    for( i=0 ; i< 20; i++)     //INIT 代售付ｔａｂｌｅ 15-17 代售, 18-19 代付
           {
             BaseTbSubrev[i].iqt5_totrev=0;
             BaseTbSubrev[i].iam5_totrev=0;
             for( j=0 ; j<10; j++)
                {
                  BaseTbSubrev[i].it8_subrev[j].iqt_subrev=0;
                  BaseTbSubrev[i].it8_subrev[j].iam_subrev=0;
                }
           }

    pay_bill_cnt = 0;     //門市提貨券次數 20031223
    pay_bill_amt = 0;     //門市提貨券金額
    mpay_bill_cnt = 0;     //門市提貨券次數 20031223
    mpay_bill_amt = 0;     //門市提貨券金額

    Cpn71_cnt=0;                  //折價券71次數
    Cpn71_amt=0;                  //折價券71金額
    Cpn72_cnt=0;                  //折價券72次數
    Cpn72_amt=0;                  //折價券72金額
    Cpn73_cnt=0;                  //折價券73次數
    Cpn73_amt=0;                  //折價券73金額
    Cpn74_cnt=0;                  //折價券74次數
    Cpn74_amt=0;                  //折價券74金額
    Cpn75_cnt=0;                  //折價券75次數
    Cpn75_amt=0;                  //折價券75金額
    Cpn76_cnt=0;                  //折價券76次數
    Cpn76_amt=0;                  //折價券76金額

    Cpn77_cnt=0;                  //門市提貨券77次數
    Cpn77_amt=0;                  //門市提貨券77金額
    Cpn78_cnt=0;                  //門市提貨券78次數
    Cpn78_amt=0;                  //門市提貨券78金額

	z->no_tm = g_ecr_no;             //收銀機機號

    //日結序號
	_Strsprintf(z->bg_noclose,"%05d", _StrToInt(StrZCnt, "日結序號"));
    string ssysdttm, s1050sysdttm;
    
    ssysdttm=g_date_time;
    s1050sysdttm=_StringSegment_EX(ZDT_1050, SGM, 5);

    if( s1050sysdttm.substr(0,8) < ssysdttm.substr(0,8) )
      {
        z->bg_dtclose = s1050sysdttm.substr(0,8);      //結帳日期
        z->bg_tmclose = "235959";                         //結帳時間
      }
    else
      {
        //z->bg_dtclose = str_z_date.SubString(1,8);      //結帳日期   取的目前最大1050之交易
        //z->bg_tmclose = str_z_date.SubString(9,6);      //結帳時間

        z->bg_dtclose = s1050sysdttm.substr(0,8);      //結帳日期  1090 日期
        z->bg_tmclose = "235959";                         //結帳時間

      }

    logsprintf("AUTOZDATA::GetZDTData: s1050sysdttm=%s, ssysdttm=%s, Set 1096 日結序號=%s, 結帳日期 z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );

    z->ed_dtclose = str_pz_date.substr(0,8);     //前次結帳日期
    z->ed_tmclose = str_pz_date.substr(8,6);     //前次結帳時間
	
    _Strsprintf(z_a, "3096||%04d||%-6s||%-2s||%14s||%5s||%-10s||%8s||%6s||%8s||%6s||",
                                      189,
                                      g_tencode.c_str(),
                                      z->no_tm.c_str(),           //收銀機機號
                                      Z_date.c_str(),             //現在時間
                                      z->bg_noclose.c_str(),      //日結序號
                                      sYYMMDDZZ.c_str(),          //Z帳表序號
                                      z->ed_dtclose.c_str(),      //前次結帳日期
                                      z->ed_tmclose.c_str(),      //前次結帳時間
                                      z->bg_dtclose.c_str(),      //結帳日期
                                      z->bg_tmclose.c_str()      //結帳時間
                                    );

    // ZDATA::GetZDTData 03 交易明細ｆｉｅｌｄ  =============================================

    //本日來客數/本日交易品項  SumQt_trans()==>併入 SumQt_tcust  NextPOS
    int iC0TotCount = 0;
    int iSumQt_trans = 0;  //本日交易品項
	_Strsprintf(z->tb_trans.qt_tcust,"%08d", SumQt_tcust(0, gi_tot_line)); //本日來客數
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項

    //本日交易品項/本日交易品項  SumQt_trans() ==> 併入 SumQt_tcust  NextPOS
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);  //NextPOS
	_Strsprintf(z->tb_trans.qt_trans,"%08d", iSumQt_trans - giI); // NextPOS 本日交易品項 SumQt_trans(0, gi_tot_line)

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt, C01TaxAmt, C01FTaxAmt, RC01TaxAmt, RC01FTaxAmt, R4_C01FTaxAmt, R4_C01TaxAmt;
    
    // 點卡手續費應免稅
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  NextPOS
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line);  //1050==> R4+Z0    NextPOS
    TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(z->tb_trans.am_trcpntx,"%010d", giI - C01FTaxAmt - R4_C01FTaxAmt - (PreSal09_AmtNTX + R4PreSal09_AmtNTX));       // 發票日結帳額(免)
	_Strsprintf(z->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));       // 發票日結帳額(稅)
	_Strsprintf(z->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));       // 發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    logsprintf("ZDATA::GetZDTData: 發票結帳累計金額=%s, 發票日結帳金額=%d ", str_am_tclsacc.c_str(), int_tot_amt  );

      //本日結帳累計金額
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));

    Tclsacc=_IntToStr(int_acc_tot_amt);

	_Strsprintf(z->tb_trans.am_tclsacc,"%010d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    //TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
	_Strsprintf(z->tb_trans.qt_miss,"%08d", giA);       //  誤打發票次數
	_Strsprintf(z->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  誤打發票金額(免)
	_Strsprintf(z->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  誤打發票金額(稅)
	_Strsprintf(z->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));       //  誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //不開發票金額, 不開發票金額誤退重
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+PreSal10_AmtTX+PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;
    //ZDATA::GetZDTData 本日實際銷貨收入
	_Strsprintf(z->tb_trans.am_totsale,"%010d", int_totsale);


    if (gbl_tot_haveline)
    {
        z->no_rcstart = invoice_start_end(gi_tot_line, true);    //發票開始號碼
        z->no_rcend = invoice_start_end(gi_tot_line, false);     //發票結束號碼
    }
    else    //無發票資料
    {
        z->no_rcstart = "0000000000";                            //發票開始號碼
        z->no_rcend   = "0000000000";                            //發票結束號碼
    }


    string z_b;

    z_b += (z->tb_trans.qt_tcust+SGM);             //本日來客數
    z_b += (z->tb_trans.qt_trans+SGM);             //本日交易品項
    z_b += (z->tb_trans.am_tclsacc+ SGM);           //本日結帳累計金額
    z_b += (z->no_rcstart+SGM);                   //發票開始號碼
    z_b += (z->no_rcend+SGM);                      //發票結束號碼
    z_b += (z->tb_trans.am_trcpntx+SGM);          //發票日結帳額(免)
    z_b += (z->tb_trans.am_trcptax+SGM);           //發票日結帳額(稅)
    z_b += (z->tb_trans.am_trcptot+ SGM);          //發票日結帳金額
    z_b += (z->tb_trans.qt_miss+ SGM);              //誤打發票次數
    z_b += (z->tb_trans.am_missntx+ SGM);           //誤打發票金額(免)
    z_b += (z->tb_trans.am_misstax+ SGM);           //誤打發票金額(稅)
    z_b += (z->tb_trans.am_miss+ SGM);              //誤打發票金額
    z_b += (z->tb_trans.am_totsale+ SGM);           //本日實際銷貨收入

    /* ZDATA::GetZDTData 代售付ｔａｂｌｅ       */

    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                 //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);     //giH giI 食安退貨金額、次數     //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(z->tb_newadd.qt_agnrcv,"%08d", giD);             //代收費用次數
	_Strsprintf(z->tb_newadd.am_agnrcv,"%010d", giC);             //代收費用金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giH;  iSub_C0InvoAmtC=giJ;  iSub_C0InvoQtyC=giK;

    string z_c;
    int iAmt, iQty, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    //Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //代售合計次數
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //代售合計金額   無須加入預售商品
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;
    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

	_Strsprintf(z_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev ); //代售手續費商品合計回數 25 ~ 代付商品合計金額 28

    //代現金ｔａｂｌｅ=============================================
    int  iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    SumTbCash(0,gi_tot_line);   //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//消費券
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

    writelog("giS:消費券金額 , giT:消費券回數");

	_Strsprintf(z->tb_cash.qt_bonus,"%08d", giA); //-giT);            //禮券回數 29
	_Strsprintf(z->tb_cash.am_bonus,"%010d", giB); //-iCSMAmt);       //禮券金額 30 

    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

	_Strsprintf(z->tb_cash.qt_vndcpn,"%08d", 0);             //廠商代現金回數 31
	_Strsprintf(z->tb_cash.am_vndcpn,"%010d", 0);             //廠商代現金金額 32

	_Strsprintf(z->tb_cash.qt_stocpn,"%08d", giC);            //折價券回數 33
	_Strsprintf(z->tb_cash.am_stocpn,"%010d", giD);            //折價券金額 34
    SumCpn41+=giD;                                            //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

	_Strsprintf(z->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //代現金合計回數 35
	_Strsprintf(z->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額 36

   	_Strsprintf(z->tb_cash.qt_bonout,"%08d", giG);           //溢收合計回數 找餘額券回數 37
	_Strsprintf(z->tb_cash.am_bonout,"%010d", giH);           //溢收合計金額 找餘額券金額 38

    iTotOt2Rtn=giE;                                      //餘額券
    iqt_ffu1=giI;                                        //現金卡回數  電子錢 1847
    iam_ffu1=giJ;                                        //現金卡金額  電子錢 1855    刷卡合計

    string z_d;

    z_d += (z->tb_cash.qt_bonus+ SGM);                //禮券回數
    z_d += (z->tb_cash.am_bonus+ SGM);                //禮券金額
    z_d += (z->tb_cash.qt_vndcpn+ SGM);               //廠商折價券回數
    z_d += (z->tb_cash.am_vndcpn+ SGM);               //廠商折價券金額
    z_d += (z->tb_cash.qt_stocpn+ SGM);               //折價券回數
    z_d += (z->tb_cash.am_stocpn+ SGM);               //折價券金額
    z_d += (z->tb_cash.qt_totcash+ SGM);              //代現金合計回數
    z_d += (z->tb_cash.am_totcash+ SGM);              //代現金合計金額

	_Strsprintf(sTmp,"%08d", giF);
    z_d += (sTmp+ SGM);                               //餘額合計回數 37
	_Strsprintf(sTmp,"%010d", giE);
    z_d += (sTmp+ SGM);                               //餘額合計金額 38

    z_d += (z->tb_cash.qt_bonout+ SGM);               //溢收合計回數 39
    z_d += (z->tb_cash.am_bonout+ SGM);               //溢收合計金額 40

    //ZDATA::GetZDTData======================================================
    //現金合計 含 消費券

    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(z->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);
    // writelog("現金合計 含 消費券");

	//char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //現金合計
    //strcat(ze,z->no_rcstart.c_str());      //發票開始號碼
    //strcat(ze,z->no_rcend.c_str());        //發票結束號碼
    //ze[28] = 0;

    //ZDATA::GetZDTData Z帳部門別集計TABLE      =============================================

    string z_f;

    //s1部門合計交易項數
    //s2部門合計銷售金額
    //s3部門合計誤退項數
    //s4部門合計誤退金額
    int s1, s2, s3, s4;

    s1 = s2 = s3 = s4 = 0;
    DEPT *dept;
	dept = new DEPT[20];
    try
    {
        //先將值填零
        for (int i=0; i<20; i++)
        {
			_Strsprintf(z->tb_depcal[i].am_dpsitm,"%08d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpsale,"%010d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpmitm,"%08d", 0);
			_Strsprintf(z->tb_depcal[i].am_dpmiss,"%010d", 0);

            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //writelog("INIT 部門交易資料, 部門帳");
        //計算所有部門交易資料, 部門帳

          if (gbl_rec_haveline)
            SumAllDept(dept);

        // writelog("計算所有部門交易資料, 部門帳 OK");
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //部門01交易項數 89
        // ~
        //部門20誤退金額 168
        ////////////////////////////////////////////////////////////////////////////////////////////////

        for (int i=0; i<20; i++)
        {
			_Strsprintf(z->tb_depcal[i].am_dpsitm,"%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+ SGM);
            s1 += dept[i].am_dpsitm;

			_Strsprintf(z->tb_depcal[i].am_dpsale,"%010d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+ SGM);
            s2 += dept[i].am_dpsale;

			_Strsprintf(z->tb_depcal[i].am_dpmitm,"%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+ SGM);
            s3 += dept[i].am_dpmitm;

			_Strsprintf(z->tb_depcal[i].am_dpmiss,"%010d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+ SGM);
            s4 += dept[i].am_dpmiss;
        }

    }
	catch(...)
    {
      
    }
	delete[] dept;
   //ZDATA::GetZDTData writelog("Z帳部門別集計TABLE OK");

    string z_g;

	z_g += (_Strsprintf(z->am_tdpsitm,"%08d", s1) + SGM);             // 部門合計交易項數 41
	z_g += (_Strsprintf(z->am_dpstot,"%010d", s2) + SGM);             // 部門合計銷售金額 42
	z_g += (_Strsprintf(z->am_tdpmitm,"%08d", s3) + SGM);             // 部門合計誤退項數 43
	z_g += (_Strsprintf(z->am_dpmtot,"%010d", s4) + SGM);             // 部門合計誤退金額 44
	z_g += (z->am_cash + SGM);                                   // 現金合計含消費券 45


    //ZDATA::GetZDTData 收銀員明細ｆｉｅｌｄ=============================================

    string z_h;
       
	_Strsprintf(sTmp,"%010d", VisuilCpnAmt);                       //無紙本折價金額  46
    z_h += (sTmp+ SGM);                                            //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    
	_Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    z_h += (sTmp+ SGM);                                            //代售商品折讓金額  47

	z_h += (_Strsprintf(z->tb_casher.qt_feedtot,"%08d", 0) + SGM);    // 投零次數  48
	z_h += (_Strsprintf(z->tb_casher.am_feedtot,"%010d", 0) + SGM);    // 投零合計金額 49

    //投庫
    int int_drop_line = (gtsl_drop->size() - 1);
    int int_drops = 0;           //投庫次數
    int int_drop_money = 0;       //投庫金額
    int int_drop_sum = 0;    //投庫總額
    int x_rtn_cnt=0;

    string   s1041;
    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
  		s1041 = *ls; 
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
			   logsprintf("產生日結帳表:非投庫(%s)資料", s1041.c_str());
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, SGM, 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //IC付款 FU_SPACE10
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //信用卡 FU_SPACE10
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 12));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 13));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 63));     //提貨單 FU_SPACE10


		 //ZDATA::GetZDTData中獎發票金額  500
		int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 16));
        //ZDATA::GetZDTData中獎發票金額  1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 27));

        //ZDATA::GetZDTData中獎發票金額  200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, SGM, 15));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 18)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 20)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 22)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 24)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 26)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 28)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 30)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 32)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 34)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 36)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 38)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 40)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 42)); //費用 13金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, SGM, 44)); //費用 14金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數
    }

	z_h += (_Strsprintf(z->tb_casher.qt_droptot,"%08d", int_drops) + SGM);        // 投庫次數 50
	z_h += (_Strsprintf(z->tb_casher.am_droptot,"%010d", int_drop_sum) + SGM);     // 投庫合計金額 51

    //writelog("投庫合計金額 OK");

    //ZDATA::GetZDTData 更正次數、金額
    SumUpdat(0,gi_rec_line);
	z_h += (_Strsprintf(z->tb_casher.qt_update,"%08d", giA) + SGM);          // 更正次數 52
	z_h += (_Strsprintf(z->tb_casher.am_update,"%010d", giB) + SGM);          // 更正合計金額 53


    //交易取消
    SumVoid(0,gi_tot_line);
	z_h += (_Strsprintf(z->tb_casher.qt_void,"%08d", giA) + SGM);            // 交易取消次數 54
	z_h += (_Strsprintf(z->tb_casher.am_void,"%010d", giB) + SGM);            // 交易取消合計金額 55


     //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = _StrToInt(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_CNT", "0"));
		int_ZINI_TRAN_MODE_AMT = _StrToInt(tif_ecr.ReadString("ECR", "ZINI_TRAN_MODE_AMT", "0"));

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr.WriteString("ECR", "ZINI_TRAN_MODE_CNT", "0");
        tif_ecr.WriteString("ECR", "ZINI_TRAN_MODE_AMT", "0");
    }
	catch (...)
    {
        //delete ini_ecr;
        ;;
    }


	z_h += (_Strsprintf(z->tb_casher.qt_train,"%08d", int_ZINI_TRAN_MODE_CNT) + SGM); // 訓練合計次數 56
	z_h += (_Strsprintf(z->tb_casher.am_train,"%010d", int_ZINI_TRAN_MODE_AMT) + SGM); // 訓練合計金額 57

    //手動輸入部門13、14
    SumInpdp(0,gi_rec_line);
	z_h += (_Strsprintf(z->tb_newadd.qt_inpdp,"%08d", giA) + SGM);            //手動輸入部門次數 58
	z_h += (_Strsprintf(z->tb_newadd.am_inpdp,"%010d", giB) + SGM);            //手動輸入部門金額 59

	z_h += (_Strsprintf(z->tb_casher.qt_openbox,"%08d", gi_open_z) + SGM);     // 開錢櫃次數 60

    //折扣讓次數   //小計折扣
    //SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	z_h += (_Strsprintf(z->tb_casher.qt_disc,"%08d", giC) + SGM);             // 折扣次數(應稅) 61
	z_h += (_Strsprintf(z->tb_casher.am_disc,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + SGM);      // 折扣金額(應稅) 62

    //pay_sub_amt(0,gi_pay_line);
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu6,"%08d", giA) + SGM);            //促銷折扣/讓次數 63
	z_h += (_Strsprintf(z->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + SGM);             //促銷折扣/讓金額 64

    //加值機
    easy_card(0,gi_easy_line);
	z_h += (_Strsprintf(z->tb_casher.qt_disret1,"%08d", giA) + SGM);          // 加值機開/售卡次數 65
	z_h += (_Strsprintf(z->tb_casher.am_disret1,"%010d", giB) + SGM);         // 加值機開/售卡金額 66
	z_h += (_Strsprintf(z->tb_casher.qt_disret2,"%08d", giC) + SGM);          // 加值機加值次數 67
	z_h += (_Strsprintf(z->tb_casher.am_disret2,"%010d", giD) + SGM);          // 加值機加值金額 68

    //fetc_card(0,gi_fetc_line);   //e通卡  2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數  2015/08/19 代售,手續費銷售金額(免)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //備用八金額  現金 加值金額   2015/08/19 代售,手續費作廢金額(免)
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + SGM);  // 代售,手續費銷售金額(免) 69
	z_h += (_Strsprintf(z->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + SGM); // 代售,手續費作廢金額(免) 70

    //食安次數、金額  int iqt_food,iamt_food;
	z_h += (_Strsprintf(z->tb_newadd.qt_ffu9,"%08d", iqt_food) + SGM);  //食安退款筆數  71
	z_h += (_Strsprintf(z->tb_newadd.am_ffu9,"%010d", iamt_food) + SGM); //食安退款金額  72

    //   05 追加 table        =============================================

    string z_i;

	z_i += (_Strsprintf(z->tb_newadd.qt_cashCard,"%08d", iqt_ffu1) + SGM);    //刷卡/現金卡次數  73
	z_i += (_Strsprintf(z->tb_newadd.am_cashCard,"%010d", iam_ffu1) + SGM);    //刷卡/現金卡金額  74
	z_i += (_Strsprintf(z->tb_newadd.qt_parking,"%08d", 0) + SGM);            //代收停車費次數   75
	z_i += (_Strsprintf(z->tb_newadd.am_parking,"%010d", 0) + SGM);            //代收停車費金額   76
	z_i += (z->tb_newadd.qt_agnrcv + SGM);                                    //代收費用次數  77
	z_i += (z->tb_newadd.am_agnrcv + SGM);                                    //代收費用金額  78
	z_i += (_Strsprintf(z->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + SGM);     //版本前置符號 79
	z_i += (_Strsprintf(z->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + SGM);      //版本號碼 80
	z_i += (_Strsprintf(z->tb_newadd.qt_rcp0,"%010d", TotZero(0, gi_tot_line)) + SGM);              //發票金額0次數 81

    //ZDATA::GetZDTData 不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);

	z_i += (_Strsprintf(z->tb_newadd.am_ffu4,"%010d", int_novoice_amt) + SGM);         //不開發票金額 82
	z_i += (_Strsprintf(z->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + SGM);    //  不開發票金額誤退重 83

    SumLOSS(0,gi_rec_line);
	z_i += (_Strsprintf(z->tb_newadd.qt_ffu5,"%08d", giA) + SGM);             // TM廢棄Loss值次數 84
	z_i += (_Strsprintf(z->tb_newadd.am_ffu5,"%010d", giB) + SGM);             // TM廢棄Loss值金額 85

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    SumHealthAmt(0,gi_tot_line);         // 健康捐合計
	z_i += (_Strsprintf(z->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + SGM);     //原加值機加值取消金額 86

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", giB+iSub_C0InvoAmtC)+"|");             //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	z_i += (_Strsprintf(z->tb_newadd.qt_ffu7,"%010d", iSumRelDimAgn + iC0InvoAmt4VDCN) + SGM);        // 即時購代售金額 87


    //消費券
	z_i += (_Strsprintf(z->tb_newadd.am_posZRpt,"%010d", iCSMAmt) + SGM);      //消費券 88

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //部門01交易項數 89
    // ~
    //部門20誤退金額 168
    ////////////////////////////////////////////////////////////////////////////////////////////////
    string z_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);     即時購與銷轉進值應稅 169
    z_k +=sTmp;   //"00000000|";
	_Strsprintf(sTmp,"%010d||", giD);     //即時購與銷轉進值免稅     170
    z_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

	_Strsprintf(sTmp,"%08d||", giA / 200); //發票兌獎張數 171
    z_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", giA);      //發票兌獎金額  172
    z_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);  //電子禮券回數 173
    z_k +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //電子禮券金額 174
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //代售,手續費giD. 代售,手續費作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
	

    string StrTran_cnt;
    int iTran_cnt;
    sTmp = _StringSegment_EX(gsLast1099, SGM, 7).substr(TRANCNT_NO_POS - 1, 5);  //交易序號
    iTran_cnt = _StrToInt(sTmp);

    if ((iTran_cnt + 1) > 99999)
        iTran_cnt = 0; //00001
    else
        _Strsprintf(StrTran_cnt, "00%3s%05d", DDZ_1099.c_str(), (iTran_cnt + 1));

    // 代售手續費應稅金額 175 ~ 代售手續費來客數 177
    _Strsprintf(sTmp, "%010d||%010d||%08d||%s||%10s||1||2||3||4||5||6||7||8||9||0||",
        iC0TotAmt, iC0GivUpAmt, iC0TotCount, " ", StrTran_cnt.c_str());
    
    z_k +=sTmp;
  
    string str_zrp;
    _Strsprintf(str_zrp,"%s%s%s%s%s%s%s%s%s\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str()
                            );

	logsprintf("ZDATA::GetZDTData::(%s) Len=%d", str_zrp.c_str(), str_zrp.length());

    string  Zrpfilename;
    //sTmp = ExtractFileName(SalFileName);

	_Strsprintf(Zrpfilename,"%s%s_%s.zrp",
                                        XZDATADIR,
                                        sYYMMDDZZ.c_str(),
                                        ZDT_1050_DDZ.c_str()  );


    //ZDATA::GetZDTData Z報表
	strcpy_s(report->char_filename, sizeof(report->char_filename), Zrpfilename.c_str()); //ChangeFileExt(str_z_filename,".zrp").c_str());
    report->CreateReport("Z", str_zrp, StrBillPay, g_tencode, g_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0, "");
    //備份Z報表
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //部門帳表
	strcpy_s(report->char_filename, sizeof(report->char_filename), _ChangeFileExt(Zrpfilename, string(".dpt")).c_str());
    report->CreateReport("D", str_zrp, StrBillPay, g_tencode, g_ecr_no, 0,0,0,0, NULL, 0, "");
    //備份部門帳表
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    RtnZDTData=str_zrp;
	logsprintf("AUTOZDATA::GetZDTData: END ");

  return(0);
}


//自動日結
/// <summary> AUTOZDATA::WriteData 自動日結
/// 自動日結  報表Class
/// </summary>
/// <param name=""></param>
/// <returns>int</returns>
int __fastcall AUTOZDATA::WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                    const string SalFileName, const string Version, const string AutoZDttm)
{

    string RtnAutoXData, sLog, sTmp,str_ver_num, str_tran_cnt;
    string str_z_filename;              //Z帳輸出檔名

    logsprintf("AUTOZDATA::自動日結 WriteData(StoreNO=%s, EcrNO=%s, SalesNo=%s, SalFileName=%s, Version=%s, AutoZDttm=%s)",
		StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str(), AutoZDttm.c_str());

    //int int_reslut = 0;
    //int_reslut = loadsalfile(SalFileName);               //載入SAL檔，將檔案依型態分類
    //if (!int_reslut)
    //    return 0;

    //備份載入之原始之料
    //Auto_Sal->LoadFromFile(SalFileName);
    //Autotsl_rec->Assign(gtsl_rec);
    //Autotsl_tot->Assign(gtsl_tot);
    //Autotsl_drop->Assign(gtsl_drop);
    //Autotsl_easy_card->Assign(gtsl_easy_card);
    //Autotsl_pay_sub->Assign(gtsl_pay_sub);
    //Autotsl_bil->Assign(gtsl_bil);
    //Autotsl_fetc_card->Assign(gtsl_fetc_card);
    //Autotsl_subsale->Assign(tsl_subsale);
 
    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z帳結構
    //目前Z帳時間

    string str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               Lst_AutoZ_Dttm,              //前次自動日結時間
               Nst_AutoZ_Dttm,              //現再自動日結時間
               AutoZ_Dt,                    //日結日歷日
               str_date_time, str_datez,    //目前帳表系統時間
               AutoZ_Cnt,
               Lst_ZCNT_Dttm,
               str_orgam_tclsacc,
               DDZ_1099, sRec, TmpZcnt, SaleZDT,str_zrp_path, str_dpt_path,
               str_zrp,
               str_sal_first,
               AutoZstartTrans ;


    str_nz_cnt     = tif_ecr.ReadString("Z", "NZ_CNT", "00001");
    int iTmpCnt =  _StrToInt( str_nz_cnt )-1;
    if( iTmpCnt <= 0)
        iTmpCnt=99999;
    _Strsprintf(AutoZ_Cnt,"%05d",iTmpCnt);

    Lst_AutoZ_Dttm=tif_ecr.ReadString("AUTOZ","LST_AUTOZ_DTTM","");              //前次自動日結時間
    Nst_AutoZ_Dttm=tif_ecr.ReadString("AUTOZ","NST_AUTOZ_DTTM","");              //現再自動日結時間
	Lst_ZCNT_Dttm = tif_ecr.ReadString("AUTOZ", "LST_ZCNT_DTTM", currentDateTime(2)); //  FormatDateTime("yyyymmdd", Now()));

	if (Trim(AutoZDttm) == "" || AutoZDttm.length() != 8)
		AutoZ_Dt = currentDateTime(2, -1); //   FormatDateTime("yyyymmdd", Now() - 1);  //日結日歷日
    else
        AutoZ_Dt = AutoZDttm;

    //AutoZ_Dt="20130522" ;

    ////////////////////////////   自動交班  NPOS02/23 尚未修改 //////////////////////////////

     RtnAutoXData=AutoXData(StoreNO, EcrNO, SalesNo , SalFileName, Version, AutoZ_Cnt, AutoZ_Dt ) ;

     gtsl_rec->clear();
     gtsl_tot->clear();
     gtsl_drop->clear();
     gtsl_easy_card->clear();
     gtsl_pay_sub->clear();
     gtsl_bil->clear();
     gtsl_fetc_card->clear();
     tsl_subsale->clear();
     gtsl_1051_sal->clear();
     str_date_time = g_date_time;  // 交易時間取自動交班


     //////////////////////////////////////////////////////////////////////

    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName, false , AutoZ_Cnt, AutoZ_Dt);               //載入SAL檔，將檔案依型態分類
    
    if (!int_reslut)
       {
         // 2013/10/21 Update
        tif_ecr.WriteString("X","XOPERATION", "0");
        logsprintf("AUTOZDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");

        tif_ecr.WriteString("Z","ZOPERATION","0");
        logsprintf("AUTOZDATA::設定 [Z]->ZOPERATION=0..預防 XZDATA.INI 被還原");

        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
		if (!PathFileExists(StringToWString(sTmp).c_str()))  //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("AUTOZDATA::DeleteFile=%s.", sTmp.c_str());
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
		if (!PathFileExists(StringToWString(sTmp).c_str()))   //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("AUTOZDATA::DeleteFile=%s.", sTmp.c_str());
           }


		 sTmp = currentDateTime(2); // FormatDateTime("yyyymmdd", Now());
         tif_ecr.WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
         logsprintf("AUTOZDATA::loadsalfile:%s Error...無須進行日結動作.", SalFileName.c_str() );
         logsprintf("AUTOZDATA::設定自動日結時間 AUTOZ->LST_AUTOZ_DTTM=%s..自動日結結束.", sTmp.c_str() );
         //return 0;      //千萬不可回 return OK;
         return 2;      //2013/10/28
       }

	//sprintf_s(gchar_tencode, sizeof(gchar_tencode), "%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", SalesNo.c_str());
    g_cashier = SalesNo;
    str_ver_num = Version;   //get_version(Version);       //日結

    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號


    //備份載入之原始之料
    Auto_Sal->clear();
	_wListLoadFromFile(Auto_Sal, SalFileName.c_str()); 
	Autotsl_rec->assign(gtsl_rec->begin(), gtsl_rec->end());
	Autotsl_tot->assign(gtsl_tot->begin(), gtsl_tot->end() );
	Autotsl_drop->assign(gtsl_drop->begin(), gtsl_drop->end());
	Autotsl_easy_card->assign(gtsl_easy_card->begin(), gtsl_easy_card->end());
	Autotsl_pay_sub->assign(gtsl_pay_sub->begin(), gtsl_pay_sub->end());
	Autotsl_bil->assign(gtsl_bil->begin(), gtsl_bil->end());
	Autotsl_fetc_card->assign(gtsl_fetc_card->begin(), gtsl_fetc_card->end() );
	Autotsl_subsale->assign(tsl_subsale->begin(), tsl_subsale->end());
    //Auto_1051_sal->Assign(gtsl_1051_sal);           自動日結無須使用

    AutoZstartTrans=GetSysDttmMaxTransNo(str_date_time.substr(0,8) );

    if( !GetYYYYMDD_SalData(AutoZ_Cnt, AutoZ_Dt, 1) )    // 取得 日歷日之 sal 資料
      {
        // 2013/10/21 Update
        tif_ecr.WriteString("X","XOPERATION", "0");
        logsprintf("AUTOZDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");

        tif_ecr.WriteString("Z","ZOPERATION","0");
        logsprintf("AUTOZDATA::設定 [Z]->ZOPERATION=0..預防 XZDATA.INI 被還原");

        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
		if (!PathFileExists(StringToWString(sTmp).c_str()))   // if (FileExists(sTmp))
          {
			  DeleteFile(StringToWString(sTmp).c_str());
			  logsprintf("AUTOZDATA::DeleteFile=%s.", sTmp.c_str());
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
		if (!PathFileExists(StringToWString(sTmp).c_str())) //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("AUTOZDATA::DeleteFile=%s.", sTmp.c_str());
           }

		sTmp = currentDateTime(2); // FormatDateTime("yyyymmdd", Now());
        tif_ecr.WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
        logsprintf("AUTOZDATA::GetYYYYMDD_SalData:Auto_Sal 中查無相關日結資料(%s)製作帳表...無須進行日結動作.", AutoZ_Dt.c_str() );
        logsprintf("AUTOZDATA::設定自動日結時間 AUTOZ->LST_AUTOZ_DTTM=%s..自動日結結束.", sTmp.c_str() );
        //return 0;    //千萬不可回 return OK;
        return 2;      //2013/10/28
      }

    sTmp = tif_ecr.ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    _Strsprintf(str_tran_cnt,"%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    logsprintf("AUTOZDATA::前次自動日結時間=%s, 現在自動日結時間=%s, 日歷日=%s, 最大日結序號=%s, ECR->WriteData 取的目前之交易序號:%s, 換日參日Lst_ZCNT_Dttm=%s, AutoZstartTrans=%s",
		Lst_AutoZ_Dttm.c_str(), currentDateTime(2).c_str(), AutoZ_Dt.c_str(), AutoZ_Cnt.c_str(), str_tran_cnt.c_str(), Lst_ZCNT_Dttm.c_str(), AutoZstartTrans.c_str());

    TStringList *Z_CntLst = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->clear();
    ZDT_Lst->clear();

    if( !_GetZCntLst(Z_CntLst, Auto_Sal  ) )
      {
        // 2013/10/21 Update
        tif_ecr.WriteString("X","XOPERATION", "0");
        logsprintf("AUTOZDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");

        tif_ecr.WriteString("Z","ZOPERATION","0");
        logsprintf("AUTOZDATA::設定 [Z]->ZOPERATION=0..預防 XZDATA.INI 被還原");

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
		 if (!PathFileExists(StringToWString(sTmp).c_str())) //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("AUTOZDATA::DeleteFile=%s.", sTmp.c_str());
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
		if (!PathFileExists(StringToWString(sTmp).c_str())) //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("AUTOZDATA::DeleteFile=%s.", sTmp.c_str());
           }

		sTmp = currentDateTime(2); // FormatDateTime("yyyymmdd", Now());
        tif_ecr.WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
        logsprintf("AUTOZDATA::_GetZCntLst:Auto_Sal 中查無相關日結序號製作帳表...無須進行日結動作.");
        logsprintf("AUTOZDATA::設定自動日結時間 AUTOZ->LST_AUTOZ_DTTM=%s..自動日結結束.", sTmp.c_str() );
        //return 0;    //千萬不可回 return OK;
        return 2;      
      }


    str_sal_first = gsal_first; // gpchar_sal_first;
    str_pz_date = tif_ecr.ReadString("Z","PZ_DATE","");       //前次時間

    string sTmpFirstDttm;
    sTmpFirstDttm=_StringSegment_EX(str_sal_first, SGM, 5);

    //維修通知函---131209-0153...FM-010171-羅東北城店 SC 營收日12/7缺TM2 Z帳，查看.ZRP檔 檔案名稱異常
    //if( _StringSegment_EX(str_sal_first, "|", 5) < str_pz_date )
    if( sTmpFirstDttm < str_pz_date  && Trim(sTmpFirstDttm).length()==14 )
      {
        //sLog.sprintf("AUTOZDATA:: 取的目前開始之交易:%s",str_sal_first.c_str() );
        //writelog( sLog);
        str_pz_date=sTmpFirstDttm; //_StringSegment_EX(str_sal_first, "|", 5);
        logsprintf("AUTOZDATA:: 取的目前開始之交易:%s Set str_pz_date=%s",str_sal_first.c_str(), str_pz_date.c_str() );
      }


     if( Autotsl_tot->size() > 0)
       {
		   list<string>::iterator ls = Autotsl_tot->end(); ls--;
		    sTmp = *ls;       //Autotsl_tot->Strings[Autotsl_tot->size()-1];
            logsprintf("AUTOZDATA:: 取的目前最大1050之交易:%s",sTmp.c_str() );
            str_z_date=_StringSegment_EX(sTmp, SGM, 5);
       }
     else
       {
        str_z_date = _StringSegment_EX(gsLast1099, SGM, 5);  //str_date_time;   //現在時間
       }

    if( Trim(str_z_date) == "")
        str_z_date=str_date_time;   //現在時間

	str_datez = tif_ecr.ReadString("Z", "DATEZ", (str_date_time.substr(6, 2) + "1") );           //DDZ
    sTmp=tif_ecr.ReadString("Z", "YYYYMMDDZ", (str_date_time.substr(0,8) + "0100" ));  //自動日結要如此

    //將帳表日資料備份已被將來判斷殺目錄條件, 刪除目錄開始日期 james - 2003/02/07
    tif_ecr.WriteString("Backup", "pz_date", sTmp);


    //備份 sal8.TXT條件
    tif_ecr.WriteString("Backup", "datez", str_datez);

    if ((str_pz_date == "") || (str_pz_date.length() < 8))  //無相關資料，交易記錄歸零
    {
        tif_ecr.WriteString("Z", "pz_date", str_z_date);
        tif_ecr.WriteString("Z", "pzno_seq", "0");
        tif_ecr.WriteString("Z", "nzno_seq", "1");
        tif_ecr.WriteString("Z", "pz_cnt", "00000");
        tif_ecr.WriteString("Z", "nz_cnt", "00001");
        tif_ecr.WriteString("Z", "datez", (str_date_time.substr(6,2) + "1"));
        //20031225
        tif_ecr.WriteString("Z", "YYYYMMDDZ", (str_date_time.substr(0,8) + "0100"));
		tif_ecr.WriteString("Z", "monthz", currentDateTime(2).substr(4, 2)); // FormatDateTime("mm", Now()));

        tif_ecr.WriteString("Z", "zfilename", "");
        tif_ecr.WriteString("Z", "zrptname", "");
        tif_ecr.WriteString("Z", "deptname", "");
        tif_ecr.WriteString("Z", "inqtname", "");

        tif_ecr.WriteString("VXZ", "PZNO_SEQ", "0");
        tif_ecr.WriteString("VXZ", "NZNO_SEQ", "1");
        tif_ecr.WriteString("VXZ", "PZ_DATE", "");

        //tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        //tif_ecr->WriteString("VXZ","CS_ENTRY","");

        string str_xpath;               //指定收銀員帳表目錄
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.substr(6,2) + "\\01\\");
        tif_ecr.WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date.substr(0,8)+"000000";
    }

    str_pzno_seq   = tif_ecr.ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數 01~09  換日歸1
    str_nz_cnt     = tif_ecr.ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999
    tif_ecr.WriteString("Z_OFF", "nz_cnt", str_nz_cnt  );            // 離線資料使用
    tif_ecr.WriteString("Z_OFF", "pz_cnt", tif_ecr.ReadString("Z","pz_cnt","00000")  );  // 離線資料使用

    str_am_tclsacc = tif_ecr.ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零


    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.length() < 5)
        str_nz_cnt = "00001";

    _Strsprintf(sYYMMDDZZ,"%s%02d", str_pz_date.substr(2,6).c_str(),_StrToInt(str_pzno_seq));
    


	_Strsprintf(str_z_filename,"%s%02d%s%02d%05s.zdt",
		XZDATADIR,
        _StrToInt(g_ecr_no),
		str_pz_date.substr(2,6).c_str(),
		_StrToInt(str_pzno_seq),
		AutoZ_Cnt.c_str()              );


    //換日Flag
    bool bl_change_day;

    //判斷換日
    //if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
    if (str_pz_date.substr(0,8).compare(str_date_time.substr(0,8)))
        {
          logsprintf("AUTOZDATA::換日判斷 pz_date=%s, date_time=%s.. 系統已換日 Set bl_change_day = true",str_pz_date.c_str(), str_date_time.c_str() );
          bl_change_day = true;
        }
    else
        {
          logsprintf("AUTOZDATA::換日判斷 pz_date=%s, date_time=%s.. 系統未換日 Set bl_change_day = true",str_pz_date.c_str(), str_date_time.c_str() );
          bl_change_day = false;
        }

    logsprintf("AUTOZDATA::Last1099=(%s)",gsLast1099.c_str());

    DDZ_1099=_StringSegment_EX(gsLast1099, SGM, 7).substr(2,3) ;

    if(  _StrToInt( DDZ_1099.substr(0,2) ) < 1 || _StrToInt( DDZ_1099.substr(0,2) ) > 31 )
       DDZ_1099=str_datez;
   
    tif_ecr.WriteString("Backup", "datez",DDZ_1099);  //歷史目錄維護用 ,絕對不可刪除目錄


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  str_orgam_tclsacc=str_am_tclsacc;

   string ZrpFile, DptFile;

   ZrpFile = _ChangeFileExt(str_z_filename, string(".zrp"));     //ExtractFileName(str_z_filename)
   DptFile = _ChangeFileExt(str_z_filename, string(".dpt"));
     // sBackupPath=ExtractFilePath(BackupFileName);
     //   sFileNameNonPath = ExtractFileName(BackupFileName);    //檔名不包含路徑
  //for( int i=0; i< Z_CntLst->Count; i++)
   for (list<string>::iterator ls = Z_CntLst->begin(); ls != Z_CntLst->end(); ls++)
       {
		 sRec = *ls; // Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
         GetZCNT_SalData(TmpZcnt) ;
         GetZDTData(TmpZcnt, g_tencode,  g_ecr_no, g_cashier,
                    string(str_ver_num), str_pz_date, str_z_date, DDZ_1099, sRec,
                    SaleZDT, str_am_tclsacc, str_zrp_path, str_dpt_path  );

         ZDT_Lst->push_back(SaleZDT);
         str_zrp+=SaleZDT;

         SaleZDT="";

         file_cat( str_zrp_path.c_str(), ZrpFile.c_str() );
         logsprintf("AUTOZDATA::file_cat(%s, %s)",str_zrp_path.c_str(), ZrpFile.c_str() );

         file_cat( str_dpt_path.c_str(), DptFile.c_str() );
         logsprintf("AUTOZDATA::file_cat(%s, %s)",str_dpt_path.c_str(), DptFile.c_str() );

       } // end of for( int i=0; i< Z_CntLst->Count; i++)

     sTmp = _ExtractFilePath(str_zrp_path); // ExtractFilePath(str_zrp_path);
	 str_zrp_path = sTmp + _ExtractFile(ZrpFile); // ExtractFileName(ZrpFile);
	 CopyFile(StringToWString(ZrpFile).c_str(), StringToWString(str_zrp_path).c_str(), false);
     logsprintf("AUTOZDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
	 DeleteFile(StringToWString(ZrpFile).c_str());
     logsprintf("AUTOZDATA::DeleteFile(%s)",ZrpFile.c_str() );

	 str_dpt_path = sTmp + _ExtractFile(DptFile);
	 CopyFile(StringToWString(DptFile).c_str(), StringToWString(str_dpt_path).c_str(), false);
     logsprintf("AUTOZDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
	 DeleteFile(StringToWString(DptFile).c_str());
     logsprintf("AUTOZDATA::DeleteFile(%s)",DptFile.c_str() );

     delete Z_CntLst;
     delete ZDT_Lst;


     //寫入SAL檔98Type
    string str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_p_z_cnt,
               str_n_z_cnt,
               str_start_trans,            //開始交易序號
               str_am_tclascc,
               str_am_totsale,
               str_zdt_no,
               str_ver,
               str_sal_last_1099,
               SaleDataVer,
               str_zdt_path;



    str_rec_type = "3098";
    str_rec_len = "23";
    str_tencode = g_tencode;
    _Strsprintf(str_ecr_no,"%02d",_StrToInt(z->no_tm,"z->no_tm"));
    str_sys_date = (z->bg_dtclose + z->bg_tmclose);      //日期時間

    //98交易序號 //20031215
    int int_sum_tran_cnt ;

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

    //if (str_tran_cnt.Length() < 5)
    //if( Trim(str_tran_cnt)=="00000")
    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, SGM, 7).substr(TRANCNT_NO_POS-1,5);  //交易序號

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

	logsprintf("AUTOZDATA::Last1099(%s), ECR->TRAN_CNT=%s", str_sal_last_1099.c_str(), str_tran_cnt.c_str());

    string str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
		_Strsprintf(str_sum_tran,"%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //前次日結序號，目前日結序號
	_Strsprintf(str_p_z_cnt,"%05d", _StrToInt(tif_ecr.ReadString("Z", "PZNO_SEQ", ""), "前次日結序號"));
	_Strsprintf(str_n_z_cnt,"%05d", _StrToInt(tif_ecr.ReadString("Z", "NZNO_SEQ", ""), "目前日結序號"));


    if (tif_ecr.ReadString("Z","PZNO_SEQ","").length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr.ReadString("Z","NZNO_SEQ","").length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc;

    //str_sal_first = gsal_first;   //gpchar_sal_first;

     //開始交易序號
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, SGM, 7).substr(TRANCNT_NO_POS-1,5));
	_Strsprintf(str_start_trans,"%05d", i_tran_cnt);

    //int_am_tclsacc=int_acc_tot_amt;
    //str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //本日結帳累計金額
    str_am_totsale = "0";                                      //z->tb_trans.am_totsale;                //本日實際銷貨收入
	_Strsprintf(str_zdt_no,"%010ld", _StrToInt(z->bg_noclose));     //ZDT序號代入Z檔名
    //str_ver = "FFFFFFFF";
    str_ver = "1||2||3||4||5||6||7||8||9||0";

    string str_trans1_5;    //交易序號前五碼

    //str_sal_last = gsal_last;                        //gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    tif_ecr.WriteString("Backup", "datez",DDZ_1099);  //歷史目錄維護用    不可刪

    string str_01,str_99;

    _Strsprintf(str_01, "%4s||%4s||%-6s||%-2s||%14s||%5s||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                   "3001",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   AutoZ_Cnt.c_str(),                     // str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   " ",                                   //部門次數
                   " ",                                   //部門金額
                   " ",                                   //交易混登區分
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                  // SCO設備控制碼 20Byte
                   " "                                  //前次交易機號
                                       );

    

    _Strsprintf(str_99, "%4s||%4s||%-6s||%-2s||%14s||%5s||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                   "3001",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   AutoZ_Cnt.c_str(),                     //str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   "0",                                     //部門次數
                   "0",                                     //部門金額
                   " ",                                   //交易混登區分
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                  // SCO設備控制碼
                   " "                                  //前次交易機號
                                       );

    string _str_tran_cnt;        //交易序號
    string str_st_tran_cnt;     //開始交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    string str_98;  //寫入3098交易序號
    _Strsprintf(str_98, "%4s||%4s||%6s||%2s||%14s||%-5s||%-10s||%5s||%5s||%10s||0000%8s||0000%8s||%10s||%s||\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        AutoZ_Cnt.c_str(),          //str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_p_z_cnt.c_str(),
                        str_n_z_cnt.c_str(),
                        str_st_tran_cnt.c_str(),
                        str_am_tclascc.c_str(),
                        str_am_totsale.c_str(),
                        str_zdt_no.c_str(),
                        str_ver.c_str()
                        );

    string str_sal;
    //str_sal = str_01 + "\n"+ RtnAutoXData + "\n" +str_zrp + str_98 + str_99;

    if( Trim(RtnAutoXData)=="")
       str_sal = str_01 + "\n"+ str_zrp + str_99;
    else
       str_sal = str_01 + "\n"+ RtnAutoXData + "\n" +str_zrp + str_99;

    ///////////////////////

    if (w_WriteSAL(str_sal, str_z_filename, 0) == OK)
       {
        //備份ZDT帳
        str_zdt_path = BackupFile(str_z_filename,"", DDZ_1099);
        tif_tmp.WriteString("Z","x1098filename", str_zdt_path);
       }
    else
      {
       tif_tmp.WriteString("Z","x1098filename", "");
      }

    /////////////////////////////////////////////////////////////////

    string StrZslFile;
    if (w_WriteSAL(str_sal, SalFileName,0) == OK)
       {
         //備份ZSL帳

         sTmp = _ExtractFile(SalFileName);
		 _Strsprintf(StrZslFile, "%s%s_%s%s", XZDATASAL, str_sys_date.substr(0, 8).c_str(), DDZ_1099.c_str(), sTmp.c_str());
		 if (CopyFile(StringToWString(SalFileName).c_str(), StringToWString(StrZslFile).c_str(), false))
           {
             logsprintf("AUTOZDATA:: InPutFile SalFileName 備份SAL %s TO %s",
				 SalFileName.c_str(), StrZslFile.c_str());
           }
         else
           {
			   logsprintf("AUTOZDATA:: InPutFile SalFileName 備份SAL %s TO %s 失敗!!", SalFileName.c_str(), StrZslFile.c_str());
           }


		 StrZslFile = BackupFile(SalFileName, _ChangeFileExt(str_zdt_path, string(".zsl")), DDZ_1099);
         tif_tmp.WriteString("Z","ZSLNAME", StrZslFile);

		 sTmp = _ChangeFileExt(StrZslFile, string(".asl"));
		 //_ListToFile(Auto_Sal, sTmp.c_str()); // Auto_Sal->SaveToFile(sTmp);
         _ListToWFile(Auto_Sal, sTmp);
         writelog("AUTOZDATA::備份載入之SAL Auto_Sal->SaveToFile=>" + sTmp);

       }
    else
       {
         tif_tmp.WriteString("Z","ZSLNAME","");
       }


    string str_ipt_path;

    //更改XZDATA.INI
    int int_nzno_seq   = _StrToInt(str_pzno_seq, "pzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp.WriteString("Z","ZFILENAME" , str_zdt_path);
    tif_tmp.WriteString("Z","ZRPTNAME"  , str_zrp_path);
    tif_tmp.WriteString("Z","DEPTNAME"  , str_dpt_path);
    tif_tmp.WriteString("Z","INQTNAME"  , str_ipt_path);

    tif_tmp.WriteString("Z","PZ_DATE"     ,AutoZ_Dt+"235959" ); //str_z_date);         2013/09/12
    tif_tmp.WriteString("Z","AM_TCLSACC" , _IntToStr(int_am_tclsacc) );
    tif_tmp.WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT 交易序號

    tif_tmp.WriteString("AUTOZ","LST_AUTOZ_DTTM",str_date_time.substr(0,8)); //AutoZ_Dt);   //前次自動日結時間
    //tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM","");  //現再自動日結時間無作用
    tif_tmp.WriteString("AUTOZ","LST_ZCNT_DTTM",Lst_ZCNT_Dttm);   //換日參考日

    tif_tmp.WriteString("ECR","tot_open_z", "0");
    tif_ecr.WriteString("ECR","tot_open_z", "0");

    tif_tmp.WriteString("VXZ", "NZNO_SEQ", "1");
    tif_ecr.WriteString("VXZ", "NZNO_SEQ", "1");

    tif_tmp.WriteString("VXZ", "PZ_DATE", "");
    tif_ecr.WriteString("VXZ", "PZ_DATE", "");


    if (bl_change_day) //判斷是否換日
    {
        tif_ecr.WriteString("Z","PZNO_SEQ", "0");
        tif_ecr.WriteString("Z","NZNO_SEQ", "1");
        tif_ecr.WriteString("Z","PZ_CNT", AutoZ_Cnt); // str_nz_cnt);

        //if (++int_nz_cnt > 99999) 自動日結不異動
        //    int_nz_cnt = 1;
        //str_nz_cnt.sprintf("%05d",int_nz_cnt);
        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);

        //tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));

        tif_ecr.WriteString("Z","DATEZ", ( str_date_time.substr(6,2) + "1" ));
        tif_ecr.WriteString("Z","YYYYMMDDZ", ( str_date_time.substr(0,8) + "0100" ));
		tif_ecr.WriteString("Z", "monthz", currentDateTime(2).substr(4, 2)); // FormatDateTime("mm", Now()));

        //tif_ecr->WriteString("Backup", "datez",( str_date_time.SubString(7,2) + "1" ) );  //歷史目錄維護用

        sLog="AUTOZDATA::換日處理 DATEZ=" + str_date_time.substr(6,2)+ "1" +  ", YYYYMMDDZ=" + str_date_time.substr(0,8)+"0100";
        writelog( sLog);



    }
    else
    {
        tif_ecr.WriteString("Z","PZNO_SEQ", str_pzno_seq);

        if (++int_nzno_seq > 9)
            int_nzno_seq = 1;

		_Strsprintf(str_pzno_seq,"%d", int_nzno_seq);

        tif_ecr.WriteString("Z","NZNO_SEQ", str_pzno_seq);
        tif_ecr.WriteString("Z","PZ_CNT", AutoZ_Cnt);  //str_nz_cnt);

        //if (++int_nz_cnt > 99999)       自動日結不異動
        //    int_nz_cnt = 1;
        //str_nz_cnt.sprintf("%05d", int_nz_cnt);
        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);

        //tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
        //                      + FormatFloat("00",int_nzno_seq) + "00"));

        tif_ecr.WriteString("Z","DATEZ", ( str_date_time.substr(6,2) + _IntToStr(int_nzno_seq)));
		_Strsprintf(sTmp, "%s%02d00", str_date_time.substr(0, 8).c_str(), int_nzno_seq);
		tif_ecr.WriteString("Z", "YYYYMMDDZ", sTmp);  //(str_date_time.substr(0,8) + FormatFloat("00",int_nzno_seq) + "00"));
		tif_ecr.WriteString("Z", "monthz", currentDateTime(2).substr(4, 2)); // FormatDateTime("mm", Now()));

        //tif_ecr->WriteString("Backup", "datez",( str_date_time.SubString(7,2) + "1" ) );  //歷史目錄維護用

		sLog = "AUTOZDATA::未換日處理 DATEZ=" + str_date_time.substr(6, 2) + _IntToStr(int_nzno_seq) + ", YYYYMMDDZ=" + sTmp; // str_date_time.substr(0, 8) + FormatFloat("00", int_nzno_seq) + "00";
        writelog( sLog);

    }

    str_pz_date    = str_z_date;
    str_pzno_seq   = tif_ecr.ReadString("Z","NZNO_SEQ","");      //目前Z帳次數   換日歸1

    if (str_pzno_seq.length() < 1)
        str_pzno_seq = "1";

    //指定X 帳PATH
    string  str_x_path;
    //str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    str_x_path = write_xpath((str_date_time.substr(0,8) + "0" + str_pzno_seq + ".zdt" ));
    tif_ecr.WriteString("Z","XPATH",str_x_path);

    tif_ecr.WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印
    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    _Strsprintf(str_sum_tran,"%05d", int_sum_tran_cnt );

    tif_tmp.WriteString("ECR", "TRAN_CNT", str_sum_tran);


    string LstXdtDttm;

    LstXdtDttm=tif_ecr.ReadString("X","EX_DATE","");    // 上次日結時間
    if( LstXdtDttm.substr(0,8) >= str_date_time.substr(0,8) ) // 00:00~04:00 有交班
      {
        logsprintf("AUTOZDATA::自動日結完成  LstXdtDttm=%s  大於等於 %s  不將X帳歸零.", LstXdtDttm.c_str(), str_date_time.c_str() );
      }
    else
      {
        //tif_ecr->WriteString("X","PTRAN_CNT", AutoZstartTrans.SubString(3,8) );
        logsprintf("AUTOZDATA::自動日結完成  LstXdtDttm=%s  小於 %s  將X帳歸零.", LstXdtDttm.c_str(), str_date_time.c_str() );
        // 自動日結 不將X帳歸零    _WZ_OK 會規0
        tif_ecr.WriteString("X", "PTRAN_CNT", "00000000");
        tif_ecr.WriteString("X", "SX_DATE"  , "");
        tif_ecr.WriteString("X", "EX_DATE"  , "");
      }

     //2015/03/11 Mantis:019302_交班失敗，防止自動日結XZDATA.INI被回寫
     sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
	 if (!PathFileExists(StringToWString(sTmp).c_str()))   //if (FileExists(sTmp))
           {
			   DeleteFile(StringToWString(sTmp).c_str());
			   logsprintf("XDATA::DeleteFile=%s.", sTmp.c_str());
           }

   logsprintf("AUTOZDATA::自動日結完成 DDZ_1099=%s, XZDATAINI Z->PZ_CNT=%s, ECR->TRAN_CNT=%s, AM_TCLSACC=%d, [AUTOZ]->Lst_ZCNT_Dttm=%s, XPATH=%s,  AutoZstartTrans=%s ",
	   DDZ_1099.c_str(), AutoZ_Cnt.c_str(), str_sum_tran.c_str(), int_am_tclsacc, Lst_ZCNT_Dttm.c_str(), str_x_path.c_str(), AutoZstartTrans.c_str());

    return OK;

}


AUTOZDATA::AUTOZDATA()
{
    //
    writelog("Z帳 開始");
    z = new SPOS_ZRPDT;         //Z帳結構
    x = new SPOS_XRPDT;
    //tif_tmp = new TIniFile(ZDATATMP_INI);
    //tif_ecr = new TIniFile(XZDATA_INI);
	tif_ecr.setINIFileName(XZDATA_INI);
	tif_tmp.setINIFileName(ZDATATMP_INI);
    report = new REPORT();      //日結帳表


}

AUTOZDATA::~AUTOZDATA()
{
    delete z, tif_ecr;

    delete report;      //日結、部門帳表
   // delete tif_tmp;

    writelog("AUTOZ帳 結束");

   
}

////////////////////////////////////////////////////////////////////////////////
//自動收銀員交班 ,自動交班
///////////////////////////////////////////////////////////////////////////////
string __fastcall AUTOZDATA::AutoXData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version, const string AutoZ_cnt,
                                const string AutoZ_Dt )
{
    int int_reslut = 0;
    int_reslut = wloadsalfile(SalFileName, true, AutoZ_cnt, AutoZ_Dt);               //載入SAL檔，將檔案依型態分類

    string sLog, SalAutoZcnt;

    logsprintf("AUTOZDATA::自動收銀員交班 AutoXData(%s, %s, %s, %s, %s, %s, %s)",
		StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), SalFileName.c_str(), Version.c_str(), AutoZ_cnt.c_str(), AutoZ_Dt.c_str());
    SalAutoZcnt=AutoZ_cnt;

    if (!int_reslut)
        return "";

    if( gtsl_tot->size() > 0 || gtsl_drop->size() > 0 )
      {
       ;;
      }
    else
      {
        logsprintf("AUTOZDATA::自動收銀員交班 =>無交易及投庫資料, 不產生收銀員交班表, gtsl_tot->Count=%d, gtsl_drop->Count=%d ",
                      gtsl_tot->size(), gtsl_drop->size());
        return "";
      }

    string str_ver_num,str_nz_cnt, sTmp, sSalesLoginDttm;

    str_ver_num = get_version(Version);     //版本編號

	//sprintf_s(gchar_tencode, sizeof(gchar_tencode), "%6.6s", StoreNO.c_str());
    g_tencode = StoreNO;
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%2s", EcrNO.c_str());
    g_ecr_no = EcrNO;

    if (Trim(g_orgtencode) == "")
        g_orgtencode = StoreNO;
		//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", gchar_tencode);      //原始店號

    string str_x_filename,              //XDATA檔名
               str_ptran_cnt,               //前次結帳序號
               str_date_z,                  //DataZ(日期加X帳次數)
               str_tran_cnt,                //開店以來結帳金額
               str_pxno_seq,                //X帳結帳次數
               str_nxno_seq,                //下次結帳數
               str_YYYYMMDDZ,
               SaleDataVer;

    str_date_z = tif_ecr.ReadString("Z","DATEZ","");             //ddn
    sTmp = tif_ecr.ReadString("ECR","TRAN_CNT","00001");         //目前交易序號,但SAL最後一筆須檢1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    _Strsprintf(str_tran_cnt,"%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

	logsprintf("AUTOZDATA::AutoXData ECR->WriteData 取的目前之交易序號:%s", str_tran_cnt.c_str());

    str_YYYYMMDDZ=tif_ecr.ReadString("Z","YYYYMMDDZ","201001010000");

    string str_date_time, ed_date_time ;
    	
    str_nz_cnt = tif_ecr.ReadString("Z", "NZ_CNT", "00001");             //999999目前日結序號  換日不歸零
    str_date_time = tif_ecr.ReadString("Z","pz_date", g_date_time);  //2003/03/08 2004/03/22 liu
	if (str_date_time.length() < 8)
		str_date_time = currentDateTime(1); //  FormatDateTime("yyyymmddhhnnss", Now());        //2003/03/08 liu

    if(str_date_z.length() < 3)    //DATAZ=日期+Z帳次數  ddn
       str_date_z = (str_date_time.substr(6,2) + "1");

    string str_sal_last, str_sal_last_1099, AutoZstartTrans ;        //*.sal 最後一筆
    str_sal_last = gsal_last;                                      // gpchar_sal_last;
    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, SGM, 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
		SaleDataVer = "2021030100";  //Default:Ver     last:"2017120100"

     if( Trim(SalesNo)=="")
       {
         sTmp= Trim(_StringSegment_EX(str_sal_last_1099, SGM, 14));
		 //sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", sTmp.c_str());
         g_cashier = Trim(sTmp);
       }
     else
	 {
		 //sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%5.5s", SalesNo.c_str());
         g_cashier = Trim(SalesNo);
	 }
   
    //2012/08/06 Mark 已INI 為依據
    //if( Trim(str_tran_cnt)=="00000" )
    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, SGM, 7).substr(TRANCNT_NO_POS-1,5);  //交易序號

   	logsprintf("AUTOZDATA::AutoXData Last1099(%s), ECR->TRAN_CNT=%s, SalAutoZcnt=%s", str_sal_last_1099.c_str(), str_tran_cnt.c_str(), SalAutoZcnt.c_str());
    
    str_pxno_seq = tif_ecr.ReadString("X","Autoxno_seq","01");                  //自動日結規不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理

    _Strsprintf(str_nxno_seq,"%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //目前x次到第幾次

    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";

    x->no_tm = g_ecr_no;                      //收銀機機號
    x->no_casher = g_cashier;                 //收銀員號碼

    if (x->no_casher.length() < 5)
        x->no_casher = "00001";

    //X帳序號  01~99
    x->no_seq = str_nxno_seq;

    string str_x_first;                      //X帳第一筆Sal(不代表為SAL第一筆)
    str_x_first = gsalX_first;    //string(gpchar_x_first);

    //x->dt_begin = tif_ecr->ReadString("X","login_dttm","").SubString(1,8); //開始日期
    //x->tm_begin = tif_ecr->ReadString("X","login_dttm","").SubString(9,6); //開始時間

	sTmp = tif_ecr.ReadString("X", "EX_DATE", currentDateTime(1));
	if (sTmp.length() < 8)
		sTmp = currentDateTime(1);
	x->dt_begin = sTmp.substr(0, 8); //前次結帳日期
	x->tm_begin = sTmp.substr(8, 6); //前次結帳時間

    sSalesLoginDttm=tif_ecr.ReadString("X","SALESLOGINDTTM","");

    string str_invo_char;                    //發票英文字母
    str_invo_char = tif_ecr.ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.length() < 2)
        str_invo_char = "XX";

	_wListLoadFromFile(Auto_Sal,SalFileName.c_str());    // for Unicode
	Autotsl_rec->assign(gtsl_rec->begin(), gtsl_rec->end() );
	Autotsl_tot->assign(gtsl_tot->begin(), gtsl_tot->end());
	Autotsl_drop->assign(gtsl_drop->begin(), gtsl_drop->end());
	Autotsl_easy_card->assign(gtsl_easy_card->begin(), gtsl_easy_card->end());
	Autotsl_pay_sub->assign(gtsl_pay_sub->begin(), gtsl_pay_sub->end());
	Autotsl_bil->assign(gtsl_bil->begin(), gtsl_bil->end());
	Autotsl_fetc_card->assign(gtsl_fetc_card->begin(), gtsl_fetc_card->end());
	Autotsl_subsale->assign(tsl_subsale->begin(), tsl_subsale->end());
	Auto_1051_sal->assign(gtsl_1051_sal->begin(), gtsl_1051_sal->end());

    if( !GetYYYYMDD_SalData(SalAutoZcnt,AutoZ_Dt,1) )
        return ""; // 取得 日歷日之 sal 資料

    logsprintf("AUTOZDATA::自動收銀員交班 .. 現在自動日結時間=%s, 日歷日=%s, 最大日結序號=%s, ",
		currentDateTime(2).c_str(), AutoZ_Dt.c_str(), SalAutoZcnt.c_str());


    if (gbl_tot_haveline)
    {
        x->no_rcstart = invoice_start_end(gi_tot_line, true);    //發票開始號碼
        x->no_rcend = invoice_start_end(gi_tot_line, false);     //發票結束號碼
    }
    else    //無交易發票
    {
        x->no_rcend   = "0000000000";              //發票開始號碼
        x->no_rcstart = "0000000000";              //發票結束號碼
    }

    string str_pzno_seq;
    str_pzno_seq = tif_ecr.ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數1~9   換日歸1


    _Strsprintf(str_x_filename,"%s%02d%s%02d%02d.xdt",
                                         XZDATADIR,
                                         _StrToInt(g_ecr_no),
                                        (str_date_time.substr(2,6)).c_str(),
                                         _StrToInt(str_pzno_seq),_StrToInt(str_nxno_seq) );
    //本次結帳日期
    if( Autotsl_tot->size() > 0)
       {
		   list<string>::iterator ls = Autotsl_tot->end();  ls--;
		   //list <string>::const_reverse_iterator c1_crIter = Autotsl_tot->crend();
		   //c1_crIter--;  sTmp = *c1_crIter
		   sTmp = *ls; // Autotsl_tot->Strings[Autotsl_tot->size() - 1];
            logsprintf("AutoXData:: 取的目前最大3050之交易:%s",sTmp.c_str() );
            ed_date_time=_StringSegment_EX(sTmp, SGM, 5);
       }
    else
       {
        ed_date_time = _StringSegment_EX(str_sal_last_1099, SGM, 5);   
       }

    x->dt_end = AutoZ_Dt.substr(0,8); // ed_date_time.SubString(1,8);
    x->tm_end = "235959";                //ed_date_time.SubString(9,6);

    if( Trim(ed_date_time)=="")
        ed_date_time = x->dt_end+x->tm_end;   //FormatDateTime("yyyymmddhhnnss",Now());


    //無開始時間、與結束時間相同
    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000") || Trim(x->dt_begin) == "")
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }

    // 2012/03/30 Update
    if( Trim(sSalesLoginDttm)=="" )
        sSalesLoginDttm=ed_date_time;

    x->dt_begin=sSalesLoginDttm.substr(0,8);
    x->tm_begin=sSalesLoginDttm.substr(8,6);

    string x_a;
    
    // 2013/08/28 Update
	_Strsprintf(x_a,"3095||%04d||%-6s||%-2s||%8s%6s||%05d||%-10s||%2s||%-20s||%8s||%6s||%8s||%6s||", 102,
                                        g_tencode.c_str(),
                                        x->no_tm.c_str(),         //收銀機機號
                                        x->dt_end.c_str(),        //本次結帳日期  ed_date_time.c_str(),
                                        x->tm_end.c_str(),         //本次結帳日期
                                        _StrToInt(SalAutoZcnt),          //str_nz_cnt.ToInt(),
										str_YYYYMMDDZ.substr(2, 8).c_str(),
                                        x->no_seq.c_str(),        //交班序號
                                        x->no_casher.c_str(),     //收銀員號碼
                                        x->dt_begin.c_str(),      //前次結帳日期,
                                        x->tm_begin.c_str(),      //前次結帳時間
                                        x->dt_end.c_str(),        //本次結帳日期
                                        x->tm_end.c_str()         //本次結帳日期
                                     );
    


    //本日來客數
    int iC0TotCount = 0;    //點卡手續費來客數
    int iSumQt_trans = 0;  //本日交易品項

	_Strsprintf(x->tb_trans.qt_tcust,"%08d", SumQt_tcust(0, gi_tot_line));   //本日來客數
    iC0TotCount=giA; //點卡手續費來客數
    iSumQt_trans = giB; //本日交易品項

    //本日交易品項
    //Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);  //NextPOS
	_Strsprintf(x->tb_trans.qt_trans,"%08d", iSumQt_trans - giI); // NextPOS 本日交易品項 SumQt_trans(0, gi_tot_line)

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    // Sub_C0C1TaxInvoAmt(0, gi_rec_line);  NextPOS
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE; R4_C01TaxAmt=giF;

    /*  ST 預售及兌換商品統計 : 預售開發票不入營收 ,  預售兌換入營收(不開發票金額)不開發票  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //預售應稅
    PreSal09_AmtNTX = giL;       //預售免稅
    R2R3PreSal09_AmtTX = giI;    //R2R3預售應稅
    R2R3PreSal09_AmtNTX = giJ;  //R2R3預售免稅
    R4PreSal09_AmtTX = giX;     //R4預售應稅
    R4PreSal09_AmtNTX= giN;     //R4預售免稅

    PreSal10_AmtTX = giU;        //兌換應稅
    PreSal10_AmtNTX = giK;       //兌換免稅
    R2R3PreSal10_AmtTX = giG;    //R2R兌換應稅
    R2R3PreSal10_AmtNTX= giH;    //R2R兌換免稅
    R4PreSal10_AmtTX = giW;     //R4兌換應稅
    R4PreSal10_AmtNTX= giM;     //R4兌換免稅

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    //TotCount(0,gi_tot_line);
    TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(x->tb_trans.am_trcpntx,"%010d", giI - C01FTaxAmt - R4_C01FTaxAmt - (PreSal09_AmtNTX + R4PreSal09_AmtNTX));        //發票日結帳額(免)
	_Strsprintf(x->tb_trans.am_trcptax,"%010d", giF - C01TaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + R4PreSal09_AmtTX));        //發票日結帳額(稅)
	_Strsprintf(x->tb_trans.am_trcptot,"%010d", giG - C01FTaxAmt - C01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        //發票日結帳金額
    int_tot_amt = giG-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    //TotMiss(0, gi_tot_line);  //NextPOS  giA = giB = giC = giD, 合併 (  giI = A,  giF = B, giG = C : TotCount)
	_Strsprintf(x->tb_trans.qt_miss,"%08d", giA);        //誤打發票次數
	_Strsprintf(x->tb_trans.am_missntx,"%010d", giB - RC01FTaxAmt - R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //誤打發票金額(免)
	_Strsprintf(x->tb_trans.am_misstax,"%010d", giC - RC01TaxAmt - R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //誤打發票金額(稅)
	_Strsprintf(x->tb_trans.am_miss,"%010d", giD - RC01FTaxAmt - RC01TaxAmt - R4_C01FTaxAmt - R4_C01TaxAmt - (R2R3PreSal09_AmtTX + R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX));        //誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //不開發票金額, 不開發票金額誤退重
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+PreSal10_AmtTX+PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

	_Strsprintf(x->tb_trans.am_totsale,"%010d", int_totsale);

    string x_b;

    x_b  = (x->tb_trans.qt_tcust+ SGM);                      //本日來客數  14
    x_b += (x->tb_trans.qt_trans+ SGM);                      //本日交易品項 15
    x_b += (x->no_rcstart+ SGM);                              //發票開始號碼 16
    x_b += (x->no_rcend+ SGM);                                //發票結束號碼 17
    x_b += (x->tb_trans.am_trcpntx+ SGM);                    //發票日結帳額(免) 18
    x_b += (x->tb_trans.am_trcptax+ SGM);                    //發票日結帳額(稅) 19
    x_b += (x->tb_trans.am_trcptot+ SGM);                    //發票日結帳金額 20
    //x_b += (x->tb_trans.qt_miss+ SGM);                       //誤打發票次數  2021/03/18 取消
    x_b += (x->tb_trans.am_missntx+ SGM);                    //誤打發票金額(免) 21
    x_b += (x->tb_trans.am_misstax+ SGM);                    //誤打發票金額(稅) 22
    x_b += (x->tb_trans.am_miss+ SGM);                       //誤打發票金額 23
    x_b += (x->tb_trans.am_totsale+ SGM);                    //本日實際銷貨收入 24


    //代售付TABLE===============================================================

    string x_c;
    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                     //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);     //giH giI 食安退貨金額、次數    //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

	_Strsprintf(x->tb_newadd.qt_cetel,"%08d", giD);          //代收次數
	_Strsprintf(x->tb_newadd.am_cetel,"%010d", giC);          //代收金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giH;  iSub_C0InvoAmtC=giJ;  iSub_C0InvoQtyC=giK;


    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] 代售
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   代付
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //x_c += (x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty)+"|");
            //x_c += (x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt)+"|");

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //x_c += (x->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty)+"|");
        //x_c += (x->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt)+"|");
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    //Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;


    //代售合計次數
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //代售合計金額   無須加入預售商品
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;
    // 代售合計次數 25~ 代付合計金額 28
	_Strsprintf(x_c,"%08ld||%010ld||%08ld||%010ld||",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //代現金TABLE===============================================================

    int iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //電子禮券回數
    EcPrecentAmt=giA;  //電子禮券金額

    SumTbCash(0,gi_tot_line);    //代現金  刷卡合計
    EcPrecentCnt=giL;  //電子禮券回數
    EcPrecentAmt+=giM;  //電子禮券金額

    //預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//消費券
    iCSMAmt=SumBillCSM(0, gi_bil_line);//計算 2008/12/04 消費券
    //giS:消費券金額 , giT:消費券回數;

	_Strsprintf(x->tb_cash.qt_bonus,"%08d", giA); //-giT);       //禮券回數
	_Strsprintf(x->tb_cash.am_bonus,"%010d", giB); //-iCSMAmt);            //禮券金額
    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

	_Strsprintf(x->tb_cash.qt_vndcpn,"%08d", 0);             //廠商折價券回數 目前不使用
	_Strsprintf(x->tb_cash.am_vndcpn,"%010d", 0);             //廠商折價券金額

	_Strsprintf(x->tb_cash.qt_stocpn, "%08d", giC);           //折價券回數
	_Strsprintf(x->tb_cash.am_stocpn, "%010d", giD); //+iPreSalCPNDisAmt);           //折價券金額
    SumCpn41+=giD;    //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;
 
	_Strsprintf(x->tb_cash.qt_totcash,"%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //代現金合計回數
	_Strsprintf(x->tb_cash.am_totcash,"%010d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

	_Strsprintf(x->tb_cash.qt_bonout,"%08d", giG);           //溢收合計回數
	_Strsprintf(x->tb_cash.am_bonout,"%010d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //giE:餘額券金額 / giF:餘額券次數
    iqt_ffu1=giI;                                        //現金卡回數  電子錢次數  3275
    iam_ffu1=giJ;                                        //現金卡金額  電子錢金額  刷卡合計 3261

    string x_d;

    x_d += (x->tb_cash.qt_bonus+ SGM);                //禮券回數  29
    x_d += (x->tb_cash.am_bonus+ SGM);                //禮券金額  30
    x_d += (x->tb_cash.qt_vndcpn+ SGM);               //廠商折價券回數 31
    x_d += (x->tb_cash.am_vndcpn+ SGM);               //廠商折價券金額 32
    x_d += (x->tb_cash.qt_stocpn+ SGM);               //折價券回數 33
    x_d += (x->tb_cash.am_stocpn+ SGM);               //折價券金額 34
    x_d += (x->tb_cash.qt_totcash+ SGM);              //代現金合計回數 35
    x_d += (x->tb_cash.am_totcash+ SGM);              //代現金合計金額 36

	_Strsprintf(sTmp,"%08d", giF);
    x_d += (sTmp+ SGM);                         //餘額合計回數  37
	_Strsprintf(sTmp,"%010d", giE);
    x_d += (sTmp+ SGM);                         //餘額合計金額  38

    x_d += (x->tb_cash.qt_bonout+ SGM);               //溢收合計回數 39
    x_d += (x->tb_cash.am_bonout+ SGM);               //溢收合計金額 40

    //==========================================================================

    //現金合計 含 消費券-餘額券
    SumCash41=SumCash(0,gi_tot_line);
	_Strsprintf(x->am_cash,"%010d", SumCash41 - iTotOt2Rtn + iCSMAmt);

    string x_e;

    x_e += (x->am_cash+ SGM);               //現金合計  41
  
    //收銀員明細FIELD===========================================================

    string x_f;

    //借零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tb_vendfld[i].am_vend,"%08d", 0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

		_Strsprintf(x->tb_casher.tb_vendfld[i].tm_vend,"%06d", 0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

	_Strsprintf(x->tb_casher.qt_vendtot,"%08d", 0);
	_Strsprintf(x->tb_casher.am_vendtot,"%010d", 0);

    //x_f += (x->tb_casher.qt_vendtot+"|");      
	_Strsprintf(sTmp,"%010d", VisuilCpnAmt);
    x_f += (sTmp+ SGM);                          //VisuilCpnAmt :  無紙本折價金額   42  
    //x_f += (x->tb_casher.am_vendtot+"|");      
	_Strsprintf(sTmp,"%010d", iPreSalINVODisAmt);
    x_f += (sTmp+ SGM);                           // 代收商品折讓單折扣  43


    string x_g;

    //投零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tg_feedfld[i].am_feed,"%08d", 0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

		_Strsprintf(x->tb_casher.tg_feedfld[i].tm_feed,"%06d", 0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

	_Strsprintf(x->tb_casher.qt_feedtot,"%08d", 0);
	_Strsprintf(x->tb_casher.am_feedtot,"%010d", 0);

   // x_g += (x->tb_casher.qt_feedtot+ SGM);      //投零次數  2021/03/18 取消 
   // x_g += (x->tb_casher.am_feedtot+ SGM);      //投零合計金額 2021/03/18 取消 


    string x_h;

    //投庫
    int int_drop_line = (gtsl_drop->size() - 1);   //投庫次數

	logsprintf("AUTOZDATA::AutoXData 產生收銀員交班明細表:投庫筆數(%d)", gtsl_drop->size());

    //初始化全部填零
    for (int i=0; i<MaxDropRec; i++)
    {
		_Strsprintf(x->tb_casher.tg_dropfld[i].am_drop,"%010d", 0);    //投庫金額
		_Strsprintf(x->tb_casher.tg_dropfld[i].tm_drop,"%-6s", " ");    //執行時間
    }

    int int_drops = 0;                            //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                         //投庫總額
    int x_rtn_cnt=0;

    string str_drop_time, s;

    //for (int i=0; i<=int_drop_line; i++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
    {
		s = *ls; // gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, SGM, 10) );
        if( x_rtn_cnt <= 0)
           {
			   logsprintf("產生收銀員交班明細表:非投庫(%s)資料", s.c_str());
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, SGM, 11) );     //現金
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );     //IC付款 FU_SPACE10
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );     //信用卡 FU_SPACE10
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 12) );    //禮卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 13) );    //折價卷
        //int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 63) );    //提貨單 FU_SPACE10

		 //中獎發票金額 500
		int_drop_money += _StrToInt(_StringSegment_EX(s, SGM, 16));

        //中獎發票金額 1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 27) );

        //中獎發票金額 200
        int_drop_money += _StrToInt( _StringSegment_EX(s, SGM, 15) );

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 18) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 20) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 22) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 24) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 26) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 28) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 30) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 32) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 34) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 36) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 38) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 40) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 42) ); //食安退款金額
        int_drop_money +=_StrToInt( _StringSegment_EX(s, SGM, 44) ); //提貨券金額

        //每次投庫金額
		_Strsprintf(x->tb_casher.tg_dropfld[int_drops].am_drop,"%010d", int_drop_money);

        int_drop_sum += int_drop_money;                         //投庫總額

        str_drop_time = _StringSegment_EX(s, SGM, 5).substr(8,6);  //投庫時間

        //每次投庫時間
		_Strsprintf(x->tb_casher.tg_dropfld[int_drops].tm_drop,"%-6s", str_drop_time.c_str());

        int_drop_money = 0;
        ++int_drops;                             //投庫次數
        //sTmp.printf("AUTOZDATA::AutoXData 產生收銀員交班明細表:投庫:int_drop_money=%d,int_drops=%d",
        //           int_drop_money,int_drops);
        //writelog(sTmp );

    }

    
    string x_i;

	x_i += (_Strsprintf(x->tb_casher.qt_droptot,"%08d", int_drops) + SGM);        //投庫次數 44
	x_i += (_Strsprintf(x->tb_casher.am_droptot,"%010d", int_drop_sum) + SGM);     //投庫合計金額 45

    SumUpdat(0,gi_rec_line);
	x_i += (_Strsprintf(x->tb_casher.qt_update,"%08d", giA) + SGM);               //更正次數  46
	x_i += (_Strsprintf(x->tb_casher.am_update,"%010d", giB) + SGM);               //更正合計金額 47

    SumVoid(0,gi_tot_line);
	x_i += (_Strsprintf(x->tb_casher.qt_void,"%08d", giA) + SGM);                 //交易取消次數 48
	x_i += (_Strsprintf(x->tb_casher.am_void,"%010d", giB) + SGM);                 //交易取消合計金額 49

    SumInpdp(0,gi_rec_line);                                       //部門登錄
	x_i += (_Strsprintf(x->tb_newadd.qt_inpdp,"%08d", giA) + SGM);       //手動輸入部門次數、部門13、14 50
	x_i += (_Strsprintf(x->tb_newadd.am_inpdp,"%010d", giB) + SGM);       //手動輸入部門金額 51

	x_i += (_Strsprintf(x->tb_casher.qt_openbox, "%08d", gi_open_x) + SGM);            //開錢櫃次數 52

    //小計折扣  1011
    //SumDisc(0,gi_pay_line);
    pay_sub_amt(0, gi_pay_line); // NextPOS 合併 SumDisc  C=A, D=B
	x_i += (_Strsprintf(x->tb_casher.qt_dise,"%08d", giC) + SGM);                 //折扣次數 53
	x_i += (_Strsprintf(x->tb_casher.am_dise,"%010d", giD + iPreSalMMDisAmt_MFP - iPreSalINVODisAmt) + SGM);         //折扣金額 54

    //pay_sub_amt(0,gi_pay_line);
	x_i += (_Strsprintf(x->tb_newadd.qt_ffu6,"%08d", giA) + SGM);         //促銷折扣/讓次數 55
	x_i += (_Strsprintf(x->tb_newadd.am_ffu6,"%010d", giB + iPreSalMMDisAmt_TM) + SGM);         //促銷折扣/讓金額 56


    easy_card(0,gi_easy_line);
	x_i += (_Strsprintf(x->tb_casher.qt_disret1,"%08d", giA) + SGM);                  //加值機開售卡次數  57
	x_i += (_Strsprintf(x->tb_casher.am_disret1,"%010d", giB) + SGM);                  //加值機開售卡金額 58
	x_i += (_Strsprintf(x->tb_casher.qt_disret2,"%08d", giC) + SGM);                  //加值機加值次數  59
	x_i += (_Strsprintf(x->tb_casher.am_disret2,"%010d", giD) + SGM);                  //加值機加值金額 60
       
    //追加TABLE=================================================================
    string x_j;

    //fetc_card(0,gi_fetc_line);  // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu8,"%08d", iC0TotAmtNTx) + SGM);     // 代售,手續費銷售金額(免) 61
	x_j += (_Strsprintf(x->tb_newadd.am_ffu8,"%010d", iC0GivUpAmtNTx) + SGM);  // 代售,手續費作廢金額(免) 62

    //食安次數、金額  int iqt_food,iamt_food;
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu9,"%08d", iqt_food) + SGM);    //食安退款筆數 63
	x_j += (_Strsprintf(x->tb_newadd.am_ffu9,"%010d", iamt_food) + SGM);  //食安退款金額 64

    // 20151123修正成食安次數、金額  Charles.Wang
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu1,"%08d", iqt_ffu1) + SGM);         //刷卡/現金卡次數  65
	x_j += (_Strsprintf(x->tb_newadd.am_ffu1,"%010d", iam_ffu1) + SGM);        //刷卡/現金卡金額  66

    //發票金額0次數
	x_j += (_Strsprintf(x->tb_newadd.qt_parking,"%08d", 0) + SGM);          //代收停車費次數 67
	x_j += (_Strsprintf(x->tb_newadd.am_parking,"%010d", 0) + SGM);         //代收停車費金額 68
    x_j += (x->tb_newadd.qt_cetel+ SGM);                              //代收次數69
    x_j += (x->tb_newadd.am_cetel+ SGM);                              //代收金額70

    x_j += (_Strsprintf(x->tb_newadd.tx_mark, "%2s", str_ver_num.substr(0, 2).c_str()) + SGM);    //版本前置符號  71
	x_j += (_Strsprintf(x->tb_newadd.tx_ver, "%6s", str_ver_num.substr(2, 6).c_str()) + SGM);     //版本號碼 72
	x_j += (_Strsprintf(x->tb_newadd.qt_rcp0,"%08d", TotZero(0, gi_tot_line)) + SGM);    //發票金額0次數 73

    //不開發票金額

	x_j += (_Strsprintf(x->tb_newadd.am_ffu4,"%08d", int_novoice_amt) + SGM);        // 不開發票金額 74
	x_j += (_Strsprintf(x->tb_casher.am_pack,"%010d", int_novoice_miss_amt) + SGM);   //不開發票誤退重合計 75

    SumLOSS(0,gi_rec_line);
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu5,"%08d", giA) + SGM);         //TM廢棄Loss次數 76
	x_j += (_Strsprintf(x->tb_newadd.am_ffu5,"%010d", giB) + SGM);         //TM廢棄Loss金額 77

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //原加值機加值取消次數
	x_j += (_Strsprintf(x->tb_newadd.am_ffu7,"%010d", iSub_C0InvoAmtA) + SGM);   //手續費合計 78


    SumRelDimAgn(0, gi_rec_line); //即時購代售   S0/S1   VDC > 0
    int iSumRelDimAgn=giB;


    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
	x_j += (_Strsprintf(x->tb_newadd.qt_ffu7,"%010d", iSumRelDimAgn + iC0InvoAmt4VDCN) + SGM);   //即時購代售金額 79

	x_j += (_Strsprintf(x->tb_newadd.am_ffu2,"%010d", iCSMAmt) + SGM);      //消費券 80
       

    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
	_Strsprintf(sTmp,"%010d||", giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);     即時購與銷轉進值應稅 81
    x_h +=sTmp;                          
	_Strsprintf(sTmp,"%010d||", giD);     //即時購與銷轉進值免稅 82
    x_h +=sTmp;


    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

	_Strsprintf(sTmp,"%08d||", giA / 200); //發票兌獎張數 83
    x_h +=sTmp;
	_Strsprintf(sTmp,"%010d||", giA);       //發票兌獎金額 84
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for 收銀員交班明細表
    //SumCash41+=giA;                // 2012/10/08 for 收銀員交班明細表

     //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
	_Strsprintf(sTmp,"%08d||", EcPrecentCnt);   //電子禮券回數 85
    x_h +=sTmp;
	_Strsprintf(sTmp,"%010d||", EcPrecentAmt);  //電子禮券金額 86
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
	_Strsprintf(sTmp, "%010d||%010d||%08d||%s||%10s|%13s||1||2||3||4||5||6||7||8||9||0||",
        iC0TotAmt, iC0GivUpAmt, iC0TotCount, "X", "Tran_cnt","1");

    
    x_h +=sTmp;
    string str_xdt_path, str_xrp_path;   //X資料、帳表存放路徑

    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);

    string DDZ_1099, str_x_path;
    DDZ_1099=_StringSegment_EX(gsLast1099, SGM, 7).substr(2,3) ;

    if( stoi(DDZ_1099.substr(0,2)) < 1 || stoi(DDZ_1099.substr(0,2)) > 31 )
       DDZ_1099=str_date_z;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //收銀員帳表備份路徑
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.substr(0,2) + "\\" + "0" + DDZ_1099.substr(2,1) + "\\";



    //帳表檔名
    string str_new_name, sTmpPzData;

    /******/
    if ( Trim(str_x_path)=="" )
    {
        //無指定路徑由系繰日期決定

        str_x_path = "C:\\FTLPOS\\HISTORY\\";
		sTmpPzData = tif_ecr.ReadString("Z", "PZ_DATE", currentDateTime(1)); //  FormatDateTime("yyyymmddhhnnss", Now())).Trim();

        if (sTmpPzData.empty() ) // == NULL)
        {
            str_x_path += (str_date_time.substr(6,2) + "\\01\\");
            writelog("確認歷史目錄 01:" + str_x_path +" ->str_date_time="+str_date_time);

        }
        else
        {
            str_x_path += ( sTmpPzData.substr(6,2) + "\\0" );

            if (tif_ecr.ReadString("Z","NZNO_SEQ","").length() < 1)
               { str_x_path += "1\\";
                writelog("確認歷史目錄 02:" + str_x_path  );
               }
            else
               {
                str_x_path += (tif_ecr.ReadString("Z","NZNO_SEQ","") + "\\");
                writelog("確認歷史目錄 03:" + str_x_path);
               }
        }

        tif_ecr.WriteString("Z","XPATH",str_x_path);
    }

	
	if (!DirExists(str_x_path))
       {
        writelog("建立歷史目錄 :" + str_x_path);
		SHCreateDirectory(NULL, StringToWString(str_x_path).c_str() );   //ForceDirectories(str_x_path);
       }
    str_new_name = (str_x_path + _ExtractFile(str_x_filename) );
    
    /*******/

    str_xdt_path = str_new_name;                     //寫入INI變數

    //X報表
    string xdt_1095_data;
   
	_Strsprintf(xdt_1095_data,"%s%s%s%s%s%s%s%s%s\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),   // x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str()
                          );

	logsprintf("AUTOZDATA::AutoXData CreateReport:(%s) Len=%d", xdt_1095_data.c_str(), xdt_1095_data.length());

    //report = new REPORT();      //收銀員帳表

	strcpy_s(report->char_filename, sizeof(report->char_filename),  _ChangeFileExt(str_x_filename, string(".xrp")).c_str());


    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  產生收銀員交班表  *.XRP                ***************/
    report->CreateReport("x", xdt_1095_data, StrBillPay, g_tencode, g_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 1 ,"");


     // 收銀員交班明細表
    //str_date_z          //ddn
    //str_tran_cnt    //目前交易序號

   // string sTmp ;
   // sTmp.printf("CreateReport: 產生收銀員帳表檔案 代售合計=%d",iam5_15217totrev);
   // writelog(sTmp);

    str_new_name = str_x_path;
    str_new_name +=   _ExtractFile(report->char_filename) ;

    DeleteFile(StringToWString(str_new_name).c_str());

    if (MoveFileA(report->char_filename,str_new_name.c_str()))       //搬檔
        writelog("AUTOZDATA::AutoXData 產生收銀員帳表(XRP)檔案 " + str_new_name + " ->gchar_date_time="+g_date_time);
    else
        writelog("AUTOZDATA::AutoXData 產生收銀員帳表(XRP)檔案失敗 " + str_new_name);


    str_xrp_path = str_new_name;        //寫入INI變數

    //寫入SAL檔1097Type
    string str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver;

    str_rec_type = "3097";
    str_rec_len  = "0021";

    str_tencode = g_tencode;   //店號

	_Strsprintf(str_ecr_no,"%02d", _StrToInt(x->no_tm, "機號"));              //機號
    str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入3097交易序號"); //目前寫入97交易序號
	_Strsprintf(str_trans_cnt,"%05d", int_sum_transcnt);

    string str_sum_tran;       //此次收銀員帳交易序號
    //20031215
    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

	_Strsprintf(str_sum_tran,"%05d", (int_sum_transcnt + 1));

    //xxx
    //tif_tmp->WriteString("ECR","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_sum_tran;


    //str_pxno_seq  //上次x帳累積次數

    //str_nxno_seq  //目前x次到第幾次

    // 2007/07/17
	_Strsprintf(str_z_cnt,"%02d", _StrToInt(str_nxno_seq, "交班序號"));
    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

	_Strsprintf(str_cashier_no,"%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    str_start_trans = _StringSegment_EX(str_x_first, SGM, 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

	_Strsprintf(str_totsale_sum,"%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
	_Strsprintf(str_xdt_no,"%010ld", _StrToInt(x->no_seq));

    str_ver = "FFFFFFFF";


    string str_trans1_5, CasherRptFile;;    //交易序號前5碼
    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    CasherRptFile=_ChangeFileExt(str_xdt_path,string(".Crp"));
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
	_Strsprintf(sTmp,"3041||0063||%-6s||%2s||%8s%6s||%05d||%-10s||%-20s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        x->dt_end.c_str(),        //本次結帳日期  ed_date_time.c_str(),
                        x->tm_end.c_str(),         //本次結帳日期
                        _StrToInt(SalAutoZcnt),                    // str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );



    string str_01,str_99;
    _Strsprintf(str_01, "%4s||%4s||%-6s||%-2s||%14s||%05d||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                   "3001",
                   "0033",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   _StrToInt(SalAutoZcnt),                      
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   str_cashier_no.c_str(),                //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   " ",                                   //部門次數
                   " ",                                   //部門金額 
                   " ",                                   //交易混登區分  
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                  //前次交易機號
                                       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

    _Strsprintf(str_99, "%4s||%4s||%-6s||%-2s||%14s||%05d||%-10s||%2s||%-10s||%2s||%-2s||%-10s||%-10s||%-20s||%-4s||%-20s||%-20s||%-5s||%-10s||%s||%-6s||%-20.20s||%2s||1||2||3||4||5||6||7||8||9||0||",
                  "3001",
                  "0033",
                  str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   _StrToInt(SalAutoZcnt),                     //str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   str_cashier_no.c_str(),                //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   "0",                                     //部門次數
                   "0",                                     //部門金額
                   " ",                                   //交易混登區分 
                   g_orgtencode.c_str(),                     //原始店號
                   " ",                                   //SCO設備控制碼
                   " "                                  //前次交易機號
                                       );


    string _str_tran_cnt;    //交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);

    //寫入1097交易序號
    string str_97;
	_Strsprintf(str_97,"%4s||%4s||%-6s||%2s||%14s||%05d||%-10s||%8s||%10s||000%8s||%10s||%8s||",
                        str_rec_type.c_str(),     
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        ed_date_time.c_str(),                 //str_sys_date.c_str(),
                        _StrToInt(SalAutoZcnt),                 //str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );

    string str_sal;
    //str_sal = str_01 +  str_97 + str_99;
    str_sal = str_97;

    FILE *fxdata;
#ifdef WIN32
	errno_t err;
	if ((err = fopen_s(&fxdata, str_xdt_path.c_str(), "w")) == 0)
#else
	if ((fxdata = fopen(str_xdt_path.c_str(), "w")) != NULL)
#endif
    {

        fprintf(fxdata,"%s\n",str_01.c_str());
        fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s\n",
                            x_a.c_str(),
                            x_b.c_str(),
                            x_c.c_str(),
                            x_d.c_str(),
                            x_e.c_str(),
                            x_f.c_str(),
                            x_g.c_str(),
                            x_i.c_str(),
                            x_j.c_str(),
                            x_h.c_str()
                             );

        fclose(fxdata);
    }
    else
    {
        writelog("AUTOZDATA::AutoXData 無法產生收銀員DATA " + str_xdt_path);
        return "";
    }


    /***************  產生收銀員交班明細表  收銀員交接班明細表 ***************/
    string SumCasherRptData;
    SumCasherRpt(AUTOZ,0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt, sTmp, str_nxno_seq,
                 CasherRptFile, str_xdt_path, str_01, str_97, str_99, SumCasherRptData, iamt_food, iqt_food, 1);

    xdt_1095_data+=SumCasherRptData;
	logsprintf("AUTOZDATA::AutoXData 產生收銀員交班明細表:%s", CasherRptFile.c_str());

    /*************************************************************************/
    // 自動交班不異動無須累進交易序號
    //if ((int_sum_transcnt + 2) > 99999)
    //        int_sum_transcnt = 1;
    //else
    //     int_sum_transcnt +=2;
    //str_sum_tran.sprintf("%05d", int_sum_transcnt );
   // tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    string sTtmpPTRAN_CNT;  //2012/06/04
    sTtmpPTRAN_CNT=str_trans1_5.substr(2,3) + str_ptran_cnt;
    //tif_ecr->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));

    //自動日結規不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理
    //tif_ecr->WriteString("X","PXNO_SEQ",   str_nxno_seq);

    tif_ecr.WriteString("X","SX_DATE",   (x->dt_begin + x->tm_begin));
    //tif_tmp->WriteString("X","EX_DATE",   (x->dt_end + x->tm_end));     //此為下次交班之(前次結帳日期時間) 自動交班不異動

    tif_ecr.WriteString("ECR","tot_open_x", "0");
    tif_ecr.WriteString("ECR","x_pay_cnt" ,  "0");

    tif_ecr.WriteString("X","XFILEDTTM", str_sys_date);    // XDT 之DTTM
    tif_ecr.WriteString("X","XFILENAME", str_xdt_path);
    tif_ecr.WriteString("X","XRPTNAME" , str_xrp_path);
    tif_ecr.WriteString("X","CHRRPTNAME" , CasherRptFile);
    
    //tif_tmp->WriteString("X","SALESLOGINDTTM" , sSalesLoginDttm );

    //delete x,tif_ecr;

    logsprintf("AUTOZDATA::AutoXData 交班完成:寫入本次交班交易序號 tif_tmp->X->PTRAN_CNT=%s, AutoZ_cnt=%s",sTtmpPTRAN_CNT.c_str(), AutoZ_cnt.c_str() );

    return( xdt_1095_data);
}




////////////////////////////////////////////////////////////////////////////////

void __fastcall AUTOZDATA::mtn_dat_folder() //維護DAT下傳檔30日內
{
    //String Dir;
    //String FileName;
    //TSearchRec SearchRec;
    //int iAttributes = 0;
    //iAttributes |= faAnyFile * true;

    //string str_dat_path = "C:\\Abacus\\download\\dat\\";
    //Dir = str_dat_path;

    //TStringList *TempList = new TStringList;
    //try
    //{

    //    TempList->Clear();

    //    if (FindFirst(Dir+"*.tm", iAttributes, SearchRec) == 0)
    //    {
    //        do
    //        {
    //            if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
    //            {
    //                TempList->Add(SearchRec.Name);
    //            }
    //        } while (FindNext(SearchRec) == 0);

    //        FindClose(SearchRec);
    //    }

    //    string str_dd_flag = FormatDateTime("yymmdd",Now()-30);
    //    string str_dd_file;

    //    for ( int i = 0; i <= (TempList->Count-1); i++)
    //    {
    //        FileName = TempList->Strings[i];

    //        if (FileName.Length() < 13)
    //        {
    //            TempList->Delete(i);
    //            i = 0;
    //            continue;
    //        }

    //        str_dd_file = FileName.SubString(3,6);

    //        if (str_dd_file.AnsiCompare(str_dd_flag) < 0)
    //        {
    //            FileName = (Dir + TempList->Strings[i]);
    //            DeleteFile(FileName);
    //            writelog("AUTOZDATA:維護DAT刪除檔案 " + FileName);
    //        }
    //    }
    //}
    //__finally
    //{
    //    delete TempList; // destroy the list object
    //}

}

void __fastcall AUTOZDATA::del_list_file(TStringList *slist, string path)
{
    string str_sal_00;

    //for (int i=0; i<list->Count; i++)
	for (list<string>::iterator ls = slist->begin(); ls != slist->end(); ls++)
    {
		str_sal_00 = path + *ls; // list->Strings[i];
		DeleteFile(StringToWString(str_sal_00).c_str());
        writelog("AUTOZDATA:三十天目錄維護刪除檔案 " + str_sal_00);
    }

    slist->clear();
}

string __fastcall AUTOZDATA::write_xpath(string FileName)
{
    string str_file_dd,                //日期
               str_file_num,               //次數
               str_bak_path;                //備份路徑


    string str_his_folder = "C:\\FTLPOS\\HISTORY\\";
    string str_abs_file = _ExtractFile(FileName);

    str_file_dd  = str_abs_file.substr(6,2);
    str_file_num = str_abs_file.substr(8,2);

    //備份路徑
    if ( (!str_file_dd.empty()) && (!str_file_num.empty()) )
        str_bak_path = ( str_his_folder + str_file_dd + "\\" + str_file_num + "\\" );
    else
        str_bak_path = str_his_folder;

    //建立目錄
	if (!DirExists(str_bak_path))
		SHCreateDirectory(NULL, StringToWString(str_bak_path).c_str()); //ForceDirectories(str_bak_path);

    return str_bak_path;

}
