//---------------------------------------------------------------------------

#include "stdafx.h"
#include "UReport.h"

//---------------------------------------------------------------------------
//#pragma hdrstop
//#pragma package(smart_init)

//部門帳處理程序
int __fastcall REPORT::Dept(string &ZData, const char *ptr_tencode, const char *ptr_ecr_no, int XR, TStringList *tsl, string s1093)
{

    string str_nz_cnt;             //交易序號
    string str_tencode;           //店號
    string str_ecr_no;             //機號
    string str_bg_noclose;
    int int_dept = 88;                //前一個部門在ZDATA中開始位子

    //if( XR==21 || XR==22)
	_Strsprintf(str_bg_noclose, "%08d", _StrToInt(_StringSegment_EX(ZData, SGM, 7), "ZData") );

    //將部門儲存至TStringList方便處理  for (list<string>::iterator s = S1050Lst->begin(); s != S1050Lst->end(); s++)
    TStringList *tsl_zdata = new TStringList;
	tsl_zdata->clear();
	list<string>::iterator ls;
	
    string sDpt,stmp;
    for (int i=0; i<20; i++)
    {
        int_dept++;    //89~168 開始
        sDpt=_StringSegment_EX(ZData, SGM, int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, SGM, int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, SGM, int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, SGM, int_dept);

        tsl_zdata->push_back(sDpt);        //部門每一段資料為32

    }

    if (XR==1)
    {
        str_nz_cnt = "00000";
    }
    else
    {
        str_nz_cnt=_StringSegment_EX(ZData, SGM, 6); // 2013/07/22
        
    }



    str_tencode = ptr_tencode;                  //店號
    str_ecr_no  = ptr_ecr_no;                    //機號

    //部門帳結構
    typedef struct
    {
        int qty;
        int amt;
        int rqty;
        int ramt;

    }sut_dept;


    sut_dept *dept;
	ls = tsl_zdata->begin();;
    try
    {
        dept = new sut_dept[20];

        //儲存於TStringList中部門帳寫入結構中
        for (int i=0; i<20; i++, ls++)
        {
			if (ls != tsl_zdata->end())
			{
				stmp = *ls;
				//dept[i].qty = _StrToInt(stmp.substr(0, 8),"");
				//dept[i].amt = _StrToInt(stmp.substr(8, 8), "");
				//dept[i].rqty = _StrToInt(stmp.substr(16, 8), "");
				//dept[i].ramt = _StrToInt(stmp.substr(24, 8),"");

                dept[i].qty = _StrToInt(stmp.substr(0, 8), "");
                dept[i].amt = _StrToInt(stmp.substr(8, 10), "");
                dept[i].rqty = _StrToInt(stmp.substr(18, 8), "");
                dept[i].ramt = _StrToInt(stmp.substr(26, 10), "");

			}
			else
			{
				break;
			}
        }
    }
	catch(...)
    {
        
    }
	delete tsl_zdata;


    string qtyamt,  //數量加總及銷售金額加總
		   ramt;    //誤打、退貨/重印金額加總
    string   StrLine;

    StrLine="";

    if( XR==21 || XR==22)
		_Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_bg_noclose.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    else
		_Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_nz_cnt.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("序號：" + str_nz_cnt));
    //to_ptr(("店號：" + str_tencode + "     " + "機號：" + str_ecr_no));
    to_ptr("");

    if (XR==21) //21:簽到 22:簽退
        to_ptr("       盤點人員部門帳表(簽到)   ");
    else if (XR==22) //21:簽到 22:簽退
        to_ptr("       盤點人員部門帳表(簽退)   ");
    else
        to_ptr("               部門帳表        ");

    to_ptr("");


    //輸出至檔案
    string sCount;
    int int_a = 0, int_b = 0;

	_Strsprintf(StrLine, "部門   銷數 銷售金額    退數 退貨金額");
    to_ptr(StrLine);
    for (int i=0; i<20; i++)
    {
        int_a -= dept[i].rqty;
        int_b -= dept[i].ramt;

		_Strsprintf(qtyamt,"%5d %8d", dept[i].qty, dept[i].amt);            //"%6d %8d"
		_Strsprintf(ramt,"%5d %8d", int_a, int_b);                         //"%6d %8d"
		_Strsprintf(sCount,"%02d", (i + 1));

      //StrLine.sprintf("%3s   %s     %s",sCount, qtyamt, ramt);
		_Strsprintf(StrLine, "%3s   %s  %s", sCount.c_str(), qtyamt.c_str(), ramt.c_str());
        to_ptr(StrLine);
       

        //to_ptr(("部門" + sCount));
        //to_ptr(qtyamt);
        //to_ptr(ramt);
        //to_ptr("");

        int_a = 0;
        int_b = 0;
    }

    //to_ptr("部門合計");

    int_a -= _StrToInt(_StringSegment_EX(ZData, SGM, 43));
    int_b -= _StrToInt(_StringSegment_EX(ZData, SGM, 44));

	_Strsprintf(qtyamt,"%5d %8d", _StrToInt(_StringSegment_EX(ZData, SGM, 41)),       //"%6d %8d"
                              _StrToInt(_StringSegment_EX(ZData, SGM, 42))   );   //"%6d %8d"
	_Strsprintf(ramt,"%5d %8d", int_a, int_b);

	_Strsprintf(StrLine,"--------------------------------------");
    to_ptr(StrLine);

	_Strsprintf(StrLine, "合計 %s   %s", qtyamt.c_str(), ramt.c_str());
    to_ptr(StrLine);

    //to_ptr("\n\n\n\n");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //部門合計加總
    //to_ptr("部門合計");
    //to_ptr(qtyamt);
    //to_ptr(ramt);

	/////////////////////////////////  部門促銷報表  ////////////////////////////////////
	int int_a_Tot, int_b_Tot;
	if ( Trim(s1093) != "")
	{
		int_a = 0; int_b = 0; int_a_Tot = 0;  int_b_Tot = 0;

		to_ptr("");
		if (XR == 21 || XR == 22)
			_Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_bg_noclose.c_str(),
			str_tencode.c_str(), str_ecr_no.c_str());
		else
			_Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_nz_cnt.c_str(),
			str_tencode.c_str(), str_ecr_no.c_str());
		to_ptr(StrLine);

		if (XR == 21) //21:簽到 22:簽退
			to_ptr("        部門促銷報表(簽到)     ");
		else if (XR == 22) //21:簽到 22:簽退
			to_ptr("        部門促銷報表(簽退)     ");
		else
			to_ptr("           部門促銷報表     ");

		to_ptr("");

		_Strsprintf(StrLine, "部門   銷售促銷金額   退貨促銷金額");
		to_ptr(StrLine);
		for (int i = 1; i<21; i++)
		{
			int_a = _StrToInt(_StringSegment_EX(s1093, SGM, (2 * i) - 1));
			int_a_Tot += int_a;  // total
			int_b = _StrToInt(_StringSegment_EX(s1093, SGM, (2 * i)));
			int_b_Tot += int_b;  // total

			_Strsprintf(qtyamt,"%8d", int_a);
			_Strsprintf(ramt,"%8d", int_b);
			_Strsprintf(sCount,"%02d", (i));

			_Strsprintf(StrLine, "%3s   %s      %s", sCount.c_str(), qtyamt.c_str() , ramt.c_str());
			to_ptr(StrLine);
			int_a = 0;
			int_b = 0;
		}

		_Strsprintf(ramt,"%8d      %8d", int_a_Tot, int_b_Tot);

		_Strsprintf(StrLine, "--------------------------------------");
		to_ptr(StrLine);

		_Strsprintf(StrLine, "合計  %s   ", ramt.c_str() );
		to_ptr(StrLine);

		//to_ptr("\n\n\n\n");
		to_ptr("");
		to_ptr("");
		to_ptr("");
		to_ptr("");
		// 2019/11/06 For SCO
		//_Strsprintf(StrLine, "\x1b\x69");
		//to_ptr(StrLine);
	}


    ///////////////////////代售商品金額報表//////////////////////////////////

   if( ( XR==21 || XR==22) && tsl )
     {
       int int_amt = 0;                        //代售金額加總

       //單筆記錄、商品名稱、銷售金額
       string str_record, str_bar_name, str_item_amt;
	   // 2019/11/06 For SCO
	   to_ptr("---------------------------------------");
       StrLine="";
	   _Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_bg_noclose.c_str(),
		   str_tencode.c_str(), str_ecr_no.c_str());
       to_ptr(StrLine);

       to_ptr("");
       to_ptr("           代售商品金額報表     ");
       to_ptr("");

	   _Strsprintf(StrLine, "序%s商品名稱%17s銷售金額", " ", " ");
       to_ptr(StrLine);
       int int_space = 1;
	   int i = 0;
       //for (int i=0; i<tsl->Count; i++)
	   for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++, i++)
       {
		str_record = *ls; // tsl->Strings[i];
        str_bar_name = _StringSegment_EX(str_record, SGM, 3);
        str_item_amt = _StringSegment_EX(str_record, SGM, 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //刪除全形空白
        {
            int_space = str_bar_name.find("　");
            if (int_space)
            {
                str_bar_name.erase(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //刪除半形空白
        {
            int_space = str_bar_name.find(" ");
            if (int_space)
            {
                str_bar_name.erase(int_space,2);
            }
        }

		_Strsprintf(str_record,"%02d.%-26s%7s", i + 1, str_bar_name.substr(0, 26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);
 
        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
       }


    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    合計%16d"," ",int_amt);
	_Strsprintf(str_record,"合計%15s %16d", " ", int_amt);
    to_ptr(str_record);

    to_ptr("");

    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine,"\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    //to_ptr("Save");
    //return 0;

    }

    ///////////////////////////////////////////////////////////////////////////


    //寫入檔案
    to_ptr("Save");

    delete [] dept;
    return 0;
}




//即時連線商品集計帳
int __fastcall REPORT::INQReport(const char *ptr_tencode, const char *ptr_ecr_no,
                                 INQTBL *InqTbl)
{

    string str_nz_cnt;             //交易序號
    string str_tencode;           //店號
    string str_ecr_no;             //機號
 
    str_tencode = ptr_tencode;                  //店號
    str_ecr_no  = ptr_ecr_no;                    //機號

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(XZDATA_INI);
	str_nz_cnt = m_IniReader.ReadString("Z", "NZ_CNT", "00001");   //tif_ecr->ReadString("Z", "NZ_CNT", "");     //累積結帳次數序號
	if (str_nz_cnt.length() < 5)
		str_nz_cnt = "00001";

 //   TIniFile *tif_ecr;
 //   try
 //       {
 //           tif_ecr = new TIniFile(XZDATA_INI);
 //           str_nz_cnt = tif_ecr->ReadString("Z","NZ_CNT","");     //累積結帳次數序號

 //           if (str_nz_cnt.Length() < 5)
 //               str_nz_cnt = "00001";
 //       }
	//catch (...)
 //       {
 //           delete tif_ecr;
 //       }
   
    string qtyamt,  //數量加總
               ramt;    //金額加總
   
    to_ptr(gchar_date_format);
    to_ptr(("序號：" + str_nz_cnt));
    to_ptr(("店號：" + str_tencode + "    " + "機號：" + str_ecr_no));
    to_ptr("");
 // to_ptr("       部門帳表        ");
    to_ptr(" 即時連線商品交易統計表 ");
    to_ptr("");

    //輸出至檔案
    string sCount;
    int int_a = 0, int_b = 0;

    for (int i=1; i<20; i++)
    {
        int_a = InqTbl[i].qt_InqSale;
        int_b = InqTbl[i].am_InqSale;
        if( int_a > 0 || int_b > 0)
          {
			_Strsprintf(qtyamt,"%6d%18d", int_a, int_b);
           //ramt.sprintf("%6d%18d", int_a, int_b);
			_Strsprintf(sCount,"%02d", i);

           to_ptr(("連線商品  " + sCount));
           to_ptr(qtyamt);
           // to_ptr(ramt);
           to_ptr("");
          }
        int_a = 0;
        int_b = 0;
    }

    //部門合計加總
    //to_ptr(qtyamt);
    //to_ptr(ramt);

    //寫入檔案
    to_ptr("Save");
    return 0;
}


// int __fastcall REPORT::X(string &XData, string sPayStr, char *ptr_tencode   )
//XData: 3095 收銀員帳表電文檔
int __fastcall REPORT::XDTReport(string &XData, string sPayStr, const char *ptr_tencode,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int AutoZflg )
{

    string str_tencode, StrLine, NowDDHHmm, PreDDHHmm, stmp;
    str_tencode = ptr_tencode;

    StrLine="";

	_Strsprintf(StrLine,"%s序號:%5s 店號:%s 機:%s", gchar_date_format,
                                               _StringSegment_EX(XData, SGM, 8).c_str(),
											   str_tencode.c_str(),
											   _StringSegment_EX(XData, SGM, 4).c_str() );
    to_ptr(StrLine);
    //to_ptr(gchar_date_format);
    //to_ptr(("序號：" + XData.SubString(7,2)));
    //to_ptr(("店號：" + str_tencode + "     " + "機號：" + XData.SubString(1,1)));
    to_ptr("");

    to_ptr("             收銀員明細表        ");
    to_ptr("");

    to_ptr( ("收銀員 NO " + _StringSegment_EX(XData, SGM, 9)  ) );

    //結帳日期+結帳時間
    NowDDHHmm=_StringSegment_EX(XData, SGM, 12).substr(6,2)+_StringSegment_EX(XData, SGM, 13).substr(0,4);
    //前次結帳日期+前次結帳時間
    PreDDHHmm=_StringSegment_EX(XData, SGM, 10).substr(6,2)+_StringSegment_EX(XData, SGM, 11).substr(0,4);

    //StrLine.sprintf("結帳時間    開始 %s 結束 %s",
	_Strsprintf(StrLine,"結帳時間    本次 %s 前次 %s",
                     ToDateFormat( NowDDHHmm ).c_str(),
                     ToDateFormat( PreDDHHmm ).c_str() );
    to_ptr(StrLine);
    //to_ptr(("收銀員 NO" + InsertSpace(XData.SubString(2,5),9)) );
    //to_ptr(("開始時間" + InsertSpace(ToDateFormat(XData.SubString(15,6)),8)));
    //to_ptr(("結束時間" + InsertSpace(ToDateFormat(XData.SubString(29,6)),8)));
    to_ptr("");


	_Strsprintf(StrLine,"交易品項    %8s  本日來客數%7s",
                     GetNumber(_StringSegment_EX(XData, SGM, 15) ).c_str(),
                     GetNumber(_StringSegment_EX(XData, SGM, 14) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    //to_ptr(("代收商品客數" + InsertSpace(GetNumber(XData.SubString(3293,8)),12)));

	_Strsprintf(StrLine,"發票結帳金額(GS)%14s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 20)).c_str());
    to_ptr(StrLine);
    //to_ptr("發票結帳金額(GS)");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(69,8)),0));

    //  FFU4  3293
    //  FFU4  3301 金額
	_Strsprintf(StrLine,"不開發票金額%18s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 74)).c_str());
    to_ptr(StrLine);
    //to_ptr("不開發票金額");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3301,8)),0));

	_Strsprintf(StrLine,"誤,退/重合計%18s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 23)).c_str());
    to_ptr(StrLine);
    //to_ptr("誤、退/重合計");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(101,8)),0));

	_Strsprintf(StrLine,"不開發票誤,退/重合計%10s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 75)).c_str());
    to_ptr(StrLine);
    //to_ptr("不開發票誤、退/重合計");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3189,8)),0));   //加值機 結帳金額, 不開發票金額誤退重

	_Strsprintf(StrLine,"實際銷貨收入(NS)%14s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 24)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine,"代售,手續費來客數%13s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 89)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費銷售金額%11s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 87)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費作廢金額%11s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 88)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售商品折讓金額   %11s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 43)).c_str());
    to_ptr(StrLine);

    //StrLine.sprintf("健康捐合計%20s %8s"," ",GetNumber(_StringSegment_EX(XData, SGM, 81) ).c_str() );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "代收合計%11s %8s   %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 69)).c_str(),
                                                  GetNumber(_StringSegment_EX(XData, SGM, 70) ).c_str() );
    to_ptr(StrLine);


    //StrLine.sprintf("代售合計%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
	_Strsprintf(StrLine, "代售商品合計%7s %8d   %8d", " ", iqt5_15217totrev, iam5_15217totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "即時購代售合計%16s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 79)).c_str());
    to_ptr(StrLine);

    //即時購合計
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XData, SGM, 81) );
    iVDCNx=_StrToInt( _StringSegment_EX(XData, SGM, 82) );
    //StrLine.sprintf("即時購與銷轉進值%14s %8s"," ",GetNumber( _StringSegment_EX(XData, "|", 204) ).c_str() );
	_Strsprintf(StrLine, "即時購與銷轉進值%14s %8d", " ", iVDCTx + iVDCNx);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "手續費合計%20s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 78)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代付合計%11s %8d   %8d", " ", iqt5_18219totrev, iam5_18219totrev);
    to_ptr(StrLine);


	_Strsprintf(StrLine, "溢收     %8s   %10s %8s", GetNumber(_StringSegment_EX(XData, SGM, 40)).c_str(),
                                             "禮券",
                                              GetNumber(_StringSegment_EX(XData, SGM, 30) ).c_str() );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "找餘額券 %8s   %10s %8s", GetNumber(_StringSegment_EX(XData, SGM, 38)).c_str(),
                                             "禮物卡",
                                              GetNumber(_StringSegment_EX(XData, SGM, 86) ).c_str()  );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "刷卡合計 %8s   %10s %8s", GetNumber(_StringSegment_EX(XData, SGM, 66)).c_str(),
                                             "折價券",
                                              GetNumber(_StringSegment_EX(XData, SGM, 34) ).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代現金合計%20s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 36)).c_str());
    to_ptr(StrLine);

   
    //20151208 全家需求變更，調整Z帳表食安顯示位置
	_Strsprintf(StrLine, "食安退款%11s     %15s", " ",
                                                 GetNumber(_StringSegment_EX(XData, SGM, 64) ).c_str() );
    to_ptr(StrLine);

    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XData, SGM, 83) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XData, SGM, 84) );

	_Strsprintf(StrLine, "發票兌獎 %8d   %10s %8s", iPayinvoamt, "現金合計", GetNumber(_StringSegment_EX(XData, SGM, 41)).c_str());
    to_ptr(StrLine);

    TStringList *tsl_drop;

    string sDrop;
    int iDropCnt=0;
	tsl_drop = new TStringList; //for (list<string>::iterator ls = gtsl_1051_sal->begin(); ls != gtsl_1051_sal->end(); ls++)
	list<string>::iterator ls;
    try
    {
		tsl_drop->clear();
        int int_position = 144;    //LOG中投庫開始位置

		for (int i = 0; i < 30; i++) // , ls++)
        {
            sDrop=_StringSegment_EX(XData, SGM, int_position)+_StringSegment_EX(XData, SGM, int_position+1);
            tsl_drop->push_back(sDrop);
            int_position += 2;
        }

        string str_drop_time,str_drop_money,str_drop_data;
        int int_drop_money = 0;

        to_ptr("投庫");
		
		ls = tsl_drop->begin();
		for (int i = 0; i < 30; i++, ls++) /// != tsl_drop->end()
        {
			stmp = *ls;
			if (ls == tsl_drop->end())
				break;

			str_drop_time = stmp.substr(8, 6); //  tsl_drop->Strings[i].SubString(9, 6);

            if (str_drop_time == "000000" || Trim(str_drop_time)=="" )
            {
                break;
            }
            else
            {
                str_drop_time = ToTimeFormat(str_drop_time);
				int_drop_money = _StrToInt(stmp.substr(0, 8), "");   //_StrToInt(tsl_drop->Strings[i].SubString(1, 8), tsl_drop->Strings[i]);
                str_drop_money = InsertSpace(int2str(int_drop_money),12);

                if (i < 9)
                    str_drop_data = ("     " + int2str(i + 1) + " " + str_drop_time + "     " + str_drop_money);
                else
					str_drop_data = ("    " + int2str(i + 1) + " " + str_drop_time + "     " + str_drop_money);


                to_ptr(str_drop_data);
                iDropCnt++;
            }
        }
    }
	catch (...)
    {
        ;
    }

	delete tsl_drop;

    //投庫次數
    //投庫合計金額
    //StrLine.sprintf("合計%15s %8s   %8s"," ",GetNumber(XData.SubString(3077,8)).c_str(),
    //                                             GetNumber(XData.SubString(3085,8)).c_str() );
	_Strsprintf(StrLine, "合計%8s         %8s", GetNumber(_StringSegment_EX(XData, SGM, 44)).c_str(),
                                          GetNumber(_StringSegment_EX(XData, SGM, 45) ).c_str() );
    to_ptr(StrLine);
    to_ptr("");


	_Strsprintf(StrLine, "更正合計%11s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 46) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 47) ).c_str() );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "交易取消合計%7s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 48) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 49) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "折扣%15s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 53) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 54) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "商品組合折扣/讓%4s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 55) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 56) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "加值機 開/售卡%5s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 57) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 58) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "加值機加值%9s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 59) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 60) ).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XData, "|", 206) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XData, "|", 207) );
    //StrLine.sprintf("發票兌獎%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "開錢櫃%13s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 52)).c_str());
    to_ptr(StrLine);

    //  FFU4  3293
    //  FFU4  3301

    //  FFU5  3308
    //  FFU5  3317
    //  FFU6  3325
    //  FFU6  3333

    // 2005/07/01
    //to_ptr( ("加值機 結帳" + InsertSpace(GetNumber(XData.SubString(3181,8)),11)) );
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3189,8)),0));

    // 2005/07/01
    //to_ptr( ("加值機 開卡/加值取消" + InsertSpace(GetNumber(XData.SubString(3341,8)),20)) );  //ffu7
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3349,8)),0));   //ffu7

    //2021/03/17 取消
	//_Strsprintf(StrLine, "e通卡現金加值%6s %8s   %8s", " ",
    //                                          GetNumber(_StringSegment_EX(XData, SGM, 64) ).c_str(),
    //                                          GetNumber(_StringSegment_EX(XData, SGM, 65) ).c_str() );
    //to_ptr(StrLine);

    //StrLine.sprintf("e通卡信用卡加值%4s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 66) ).c_str(),
    //                                          GetNumber(_StringSegment_EX(XData, "|", 67) ).c_str() );
    //to_ptr(StrLine);




   // to_ptr( ("e通卡結帳" + InsertSpace(GetNumber(XData.SubString(3389,8)),9)) );        //ffu10
   // to_ptr(InsertSpace(GetNumber(XData.SubString(3397,8)),0));                         //ffu10

	_Strsprintf(StrLine, "部門登錄%11s %8s   %8s", " ",
                                              GetNumber(_StringSegment_EX(XData, SGM, 50) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, SGM, 51) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "廢棄%15s %8s   %8s", " ",
                                              GetNumber(_StringSegment_EX(XData, SGM, 76) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, SGM, 77) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "金額０發票%9s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 73)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "結束發票%20s %10s", " ", _StringSegment_EX(XData, SGM, 17).c_str());
    to_ptr(StrLine);


    //int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
    string sCpnCnt, sCpnAmt;

    //icpn77cnt=_StrToInt(sPayStr.SubString(225,8));
    //icpn77amt=_StrToInt(sPayStr.SubString(233,8));
    //icpn78cnt=_StrToInt(sPayStr.SubString(241,8));
    //icpn78amt=_StrToInt(sPayStr.SubString(249,8));
   // sCpnCnt.sprintf("%8d",icpn77cnt+icpn78cnt);
    //sCpnAmt.sprintf("%8d",icpn77amt+icpn78amt);

    // to_ptr( ("提貨券次數" + InsertSpace(GetNumber(sCpnCnt),10)));
    //if( icpn77amt+icpn78amt > 0)
    //  {
   //    to_ptr("");
   //    to_ptr( ("提貨券金額" + InsertSpace(GetNumber(sCpnAmt),10)));
    //  }
      
    to_ptr("");
    to_ptr("");
    to_ptr("");

    //to_ptr( ("        ***     " + _StringSegment_EX(XData, "|", 74)+"."+ _StringSegment_EX(XData, "|", 75)+ "     ***") );
    to_ptr( ("        ***     " + _StringSegment_EX(XData, SGM, 71)+ _StringSegment_EX(XData, SGM, 72)+ "     ***") );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");

    return 0;
}

//3096
// int __fastcall REPORT::Z(string &Zdata, char *ptr_tencode, char *ptr_ecr_no)
int __fastcall REPORT::ZDTReport(string &Zdata, const char *ptr_tencode, const char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int rpt_type )
{

    string str_bg_noclose;         //序號

    if( rpt_type==21 || rpt_type==22)
		_Strsprintf(str_bg_noclose,"%08d", _StrToInt(_StringSegment_EX(Zdata, SGM, 7), "ZData"));
    else
		_Strsprintf(str_bg_noclose,"%05d", _StrToInt(_StringSegment_EX(Zdata, SGM, 6), "ZData")); 

    string str_tencode, str_ecr_no, StrLine, sTmp, sStDDHHMM, sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;

    StrLine="";
    if( rpt_type==21 || rpt_type==22)
		_Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_bg_noclose.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    else
		_Strsprintf(StrLine, "%s序號:%s店號:%s 機:%s", gchar_date_format, str_bg_noclose.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);
  
    to_ptr("");

    //21:簽到 22:簽退
    if( rpt_type ==21)
        to_ptr("          盤點人員帳表(簽到)     ");
    else if( rpt_type ==22)
        to_ptr("          盤點人員帳表(簽退)     ");
    else
        to_ptr("                Z帳表        ");

    to_ptr("");

    sTmp=_StringSegment_EX(Zdata, SGM, 10)+_StringSegment_EX(Zdata, SGM, 11);
    sStDDHHMM=sTmp.substr(6,6); //本次
    sTmp=_StringSegment_EX(Zdata, SGM, 8)+_StringSegment_EX(Zdata, SGM, 9);
    sPtDDHHMM=sTmp.substr(6,6); //前次

	_Strsprintf(StrLine, "結帳時間  本次 %s 前次 %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "本日交易品項%7s 本日來客數%7s",
                     GetNumber(_StringSegment_EX(Zdata, SGM, 13) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, SGM, 12) ).c_str() );
    to_ptr(StrLine);

   	_Strsprintf(StrLine, "本日結帳累計金額%13s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 14)).c_str());
    to_ptr(StrLine);
    	
	_Strsprintf(StrLine, "發票結帳金額%17s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 19)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "不開發票金額%17s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 82)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "發票結帳   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(Zdata, SGM, 17) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, SGM, 18) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "誤,退/重   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(Zdata, SGM, 21) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, SGM, 22) ).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "誤,退/重合計%17s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 23)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "不開發票誤,退/重合計%9s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 83)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "本日實際銷貨收入%13s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 24)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代售,手續費來客數%12s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 177)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費銷售金額(免)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 69)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費銷售金額(應)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 175)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費作廢金額(免)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 70)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費作廢金額(應)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 176)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售商品折讓金額       %6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 47)).c_str());
    to_ptr(StrLine);

  	_Strsprintf(StrLine, "代收合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 77)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 78)).c_str() );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "代售商品合計%6s%8d   %8d", " ", iqt5_15217totrev, iam5_15217totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "即時購代售合計%15s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 87)).c_str());
    to_ptr(StrLine);

    //即時購合計
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(Zdata, SGM, 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(Zdata, SGM, 170) );
    //StrLine.sprintf("即時購與銷轉進值%14s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 169) ).c_str() );
	_Strsprintf(StrLine, "即時購與銷轉進值%13s%8d", " ", iVDCTx + iVDCNx);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "手續費合計%19s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 86)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代付合計%10s%8d   %8d", " ", iqt5_18219totrev, iam5_18219totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "溢收    %8s  %10s %8s", GetNumber(_StringSegment_EX(Zdata, SGM, 40)).c_str(),
                                             "禮券",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 30)).c_str() );
    to_ptr(StrLine);
        
	_Strsprintf(StrLine, "找餘額券%8s  %10s %8s", GetNumber(_StringSegment_EX(Zdata, SGM, 38)).c_str(),
                                             "禮物卡",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 174)).c_str()  );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "刷卡合計%8s  %10s %8s", GetNumber(_StringSegment_EX(Zdata, SGM, 74)).c_str(),
                                             "折價券",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 34)).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代現金合計%19s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 36)).c_str());
    to_ptr(StrLine);

    //20151208 全家需求變更，調整Z帳表食安顯示位置
	_Strsprintf(StrLine, "食安退款%10s     %14s", " ",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 72)).c_str() );
    to_ptr(StrLine);

    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, SGM, 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, SGM, 172) );

	_Strsprintf(StrLine, "發票兌獎%8d  %10s %8s", iPayinvoamt, "現金合計", GetNumber(_StringSegment_EX(Zdata, SGM, 45)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "投庫合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 50)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 51)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "更正合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 52)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 53)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "交易取消合計%6s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 54)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 55)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "折扣(應稅)%8s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 61)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 62)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "商品組合折扣/讓%3s%8s   %8s", " ",
                                                  GetNumber(_StringSegment_EX(Zdata, SGM, 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(Zdata, SGM, 64)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "加值機 開/售卡%4s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 65)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 66)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "加值機加值%8s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 67)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, "|", 172) );
    //StrLine.sprintf("發票兌獎%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);


	_Strsprintf(StrLine, "開錢櫃%12s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 60)).c_str());
    to_ptr(StrLine);

   
	_Strsprintf(StrLine, "部門登錄%10s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 58)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 59)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "廢棄%14s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 84)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 85)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine,"金額０發票%8s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 81)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "訓練合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 56)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 57)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "結束發票%19s%10s", " ", _StringSegment_EX(Zdata, SGM, 16).c_str());
    to_ptr(StrLine);
     
    to_ptr("");
            
    to_ptr("      ***     "+_StringSegment_EX(Zdata, SGM, 79)+ _StringSegment_EX(Zdata, SGM, 80)+"     ***" );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");

    return 0;
}


void __fastcall REPORT::to_ptr(string S)
{
    //輸出檔案
    if (S == "Save")       //X帳報表
    {
		if (PathFileExists(StringToWString(string(char_filename)).c_str()))
			DeleteFile(StringToWString(string(char_filename)).c_str());

        /*if (FileExists(char_filename))
            DeleteFile(char_filename.c_str());*/

		//_ListToFile(tslSaveToFile, char_filename);  //tslSaveToFile->SaveToFile(char_filename);
        _ListToWFile(tslSaveToFile, string(char_filename) ); //Unicode
        tslSaveToFile->clear();
        logsprintf("REPORT::to_ptr: Rpt File=%s,", char_filename );
        return;     //to_ptr()
    }
    else    //儲存字串
    {
        //if (gbl_epson_prt == false)
        //    S.sprintf("%cPR1%s", ESC, S.c_str());

        tslSaveToFile->push_back(S);
    }
}


int REPORT::CreateReport001(string rpt_type, string sSal, string sPayStr, string str_tencode, string  str_ecr_no,
                            int iqt5_15217totrev , int iam5_15217totrev ,
                            int iqt5_18219totrev, int iam5_18219totrev , TStringList *tsl,
                            int AutoZflg, string s1093)
{

    //選擇報表 1部門 2X帳 3Z帳
    return 0;

}


int REPORT::CreateReport(string rpt_type, string sSal, string sPayStr,
                     string str_tencode, string  str_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev,
					 TStringList *tsl, int AutoZflg, string s1093 )
{
   
    //選擇報表 1部門 2X帳 3Z帳
  

    /***/
    //if (str_good_no.substr(0, 1) == "M")  //農會商品 CD_FMCODE => M開頭7碼
    if (rpt_type == "d")  //部門
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 1, NULL, "");
    }
    else  if (rpt_type == "E") //盤點人員部門   21:簽到 22:簽退
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 21, tsl, s1093);
    }
    else  if (rpt_type == "e") //盤點人員部門   21:簽到 22:簽退
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 22, tsl, s1093);
    }
    else  if (rpt_type == "D") //部門
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 0, NULL, "" );
    }
    else  if (rpt_type == "x" || rpt_type == "c") //收銀員帳/checkin
    {
        XDTReport(sSal, sPayStr, str_tencode.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, AutoZflg);
    }
    else  if (rpt_type == "Z")
    {
        ZDTReport(sSal, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 0);
    }
    else  if (rpt_type == "V")     //盤點人員   21:簽到 22:簽退
    {
        ZDTReport(sSal, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 21);
    }
    else  if (rpt_type == "v")
    {
        ZDTReport(sSal, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 22);
    }
    else  if (rpt_type == "X")
    {
         XReport(sSal, sPayStr, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev);
    }
    else  if (rpt_type == "s")
    {
        subsale(tsl, str_tencode.c_str(), str_ecr_no.c_str() );
    }
    else
    {
        ;;
    }
    /***/
    /******
    switch (rpt_type)
    {
      case 'd':                                 //部門
          Dept(sSal, ptr_tencode, ptr_ecr_no, 1, NULL, "");
          break;
      case 'E':                                 //盤點人員部門   21:簽到 22:簽退
		  Dept(sSal, ptr_tencode, ptr_ecr_no, 21, tsl, s1093);
          break;
      case 'e':                                 //盤點人員部門   21:簽到 22:簽退
		  Dept(sSal, ptr_tencode, ptr_ecr_no, 22, tsl, s1093);
          break;

      case 'D':                                 //部門
          Dept(sSal, ptr_tencode, ptr_ecr_no);
          break;
      case 'x':                                 //收銀員帳
      case 'c':                                 //checkin
          XDTReport(sSal, sPayStr, ptr_tencode,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, AutoZflg );
          break;
      case 'Z':     //Z帳
           ZDTReport(sSal, ptr_tencode, ptr_ecr_no,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, rpt_type );
          break;
      case 'V':    //盤點人員   21:簽到 22:簽退
          ZDTReport(sSal, ptr_tencode, ptr_ecr_no,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 21 );
          break;
      case 'v':
          ZDTReport(sSal, ptr_tencode, ptr_ecr_no,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 22 );
          break;

      case 'X':    //XReport 讀帳
          XReport(sSal, sPayStr, ptr_tencode, ptr_ecr_no,
                  iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev);
          break;
      case 's':                                 //代售報表
          subsale(tsl, ptr_tencode, ptr_ecr_no);
    }
******/
    return 0;
}


REPORT::~REPORT()
{
    delete tslSaveToFile;
}

/*
string __fastcall REPORT::GetNumber(string S)
{
    string str_num;

    try
    {
        str_num.sprintf("%d",S.ToInt());
    }
    catch(Exception &e)
    {
        writelog(("數值轉換發生錯誤 " + S));
        return "0";
    }

    return str_num;
}
*/


string __fastcall REPORT::ToDateFormat(string S)
{
    string str_date_format;

    str_date_format = S;
    str_date_format.insert(2, "-");
    str_date_format.insert(5, ":");
    return str_date_format;
}

string __fastcall REPORT::InsertSpace(string S, int i)
{
    int int_len = 0;

    int_len = S.length();

    int int_space = (24 - int_len - i);

    string str_space = S;

	//for (int i = 0; i<int_space; i++)  //str.insert(str.end(), 3, '.');
	//	str_space.insert(int_len, " ");

	str_space.insert(str_space.end(), int_space, ' ');

    return str_space;
}

//int __fastcall REPORT::XReport(string &XRData, string sPayStr, char *ptr_tencode, char *ptr_ecr_no)

// 讀取 1096 data
int __fastcall REPORT::XReport(string &XRData, string sPayStr, const char *ptr_tencode, const char *ptr_ecr_no,
                             int iqt5_15217totrev, int iam5_15217totrev,
                             int iqt5_18219totrev, int iam5_18219totrev )
{

    //string str_bg_noclose;         //序號
    //str_bg_noclose.sprintf("%06d", _StrToInt(XRData.SubString(2,5), "XRData"));

    string str_tencode,str_ecr_no,StrLine,sTmp,sStDDHHMM,sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no = ptr_ecr_no;


    StrLine="";
	_Strsprintf(StrLine,"%s序號:%s店號:%s 機:%s", gchar_date_format, "00000",
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("序號：000000"));
    //to_ptr(("店號：" + str_tencode + "     機號：" + str_ecr_no));
    to_ptr("");

    to_ptr("                X帳表        ");
    to_ptr("");

    sTmp=_StringSegment_EX(XRData, SGM, 10)+_StringSegment_EX(XRData, SGM, 11);
    sStDDHHMM=sTmp.substr(6,6); //本次
    sTmp=_StringSegment_EX(XRData, SGM, 8)+_StringSegment_EX(XRData, SGM, 9);
    sPtDDHHMM=sTmp.substr(6,6); //前次

	_Strsprintf(StrLine, "結帳時間  本次 %s 前次 %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );

    //StrLine.sprintf("結帳時間   本次 %s  前次 %s",
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 11) ).c_str(),
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 9) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "本日交易品項%7s 本日來客數%7s",
                     GetNumber(_StringSegment_EX(XRData, SGM, 13) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, SGM, 12) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    // to_ptr(("代收商品客數" + InsertSpace(GetNumber(XRData.SubString(1887,8)),12)));
    //to_ptr("");

	_Strsprintf(StrLine, "本日結帳累計金額%13s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 14)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "發票結帳金額%17s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 19)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "不開發票金額%17s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 82)).c_str());
    to_ptr(StrLine);

    //  FFU4  1887
    //  FFU4  1895 金額

	_Strsprintf(StrLine, "發票結帳   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(XRData, SGM, 17) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, SGM, 18) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "誤,退/重   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(XRData, SGM, 21) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, SGM, 22) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "誤,退/重合計%17s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 23)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "不開發票誤,退/重合計%9s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 83)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "本日實際銷貨收入%13s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 24)).c_str());
    to_ptr(StrLine);

    // 2015/08/19 Update Lu
	_Strsprintf(StrLine, "代售,手續費來客數%12s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 177)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費銷售金額(免)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 69)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費銷售金額(應)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 175)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費作廢金額(免)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 70)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售,手續費作廢金額(應)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 176)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "代售商品折讓金額       %6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 47)).c_str());
    to_ptr(StrLine);

    //StrLine.sprintf("健康捐合計%20s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 86)).c_str() );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "代收合計%11s%8s  %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 77)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 78)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("代售合計%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
	_Strsprintf(StrLine, "代售商品合計%7s%8d  %8d", " ", iqt5_15217totrev, iam5_15217totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "即時購代售合計%15s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 87)).c_str());
    to_ptr(StrLine);

    //即時購合計
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XRData, SGM, 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(XRData, SGM, 170) );
    //StrLine.sprintf("即時購與銷轉進值%14s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 169) ).c_str() );
	_Strsprintf(StrLine, "即時購與銷轉進值%13s%8d", " ", iVDCTx + iVDCNx);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "手續費合計%19s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 86)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代付合計%11s%8d  %8d", " ", iqt5_18219totrev, iam5_18219totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "溢收   %8s   %10s %8s", GetNumber(_StringSegment_EX(XRData, SGM, 40)).c_str(),
                                             "禮券",
                                              GetNumber(_StringSegment_EX(XRData, SGM, 30)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("找餘額券 %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 38)).c_str(),
    //                                         "折價券",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 34)).c_str()  );
	_Strsprintf(StrLine, "找餘額券%7s   %10s %8s", GetNumber(_StringSegment_EX(XRData, SGM, 38)).c_str(),
                                             "禮物卡",
                                              GetNumber(_StringSegment_EX(XRData, SGM, 174)).c_str()  );
    to_ptr(StrLine);


    //StrLine.sprintf("刷卡合計 %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 74)).c_str(),
    //                                         "代現金合計",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 36)).c_str()  );
	_Strsprintf(StrLine, "刷卡合計%7s   %10s %8s", GetNumber(_StringSegment_EX(XRData, SGM, 74)).c_str(),
                                             "折價券",
                                              GetNumber(_StringSegment_EX(XRData, SGM, 34)).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "代現金合計%19s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 36)).c_str());
    to_ptr(StrLine);


    //to_ptr("消費券合計");
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1871,8)),0));  //1871

    //StrLine.sprintf("現金合計%22s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 45)).c_str() );
    //to_ptr(StrLine);


     //20151208 全家需求變更，調整Z帳表食安顯示位置
	_Strsprintf(StrLine, "食安退款%11s    %14s", " ",
                                             //GetNumber(_StringSegment_EX(XRData, "|", 71)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 72)).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, SGM, 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, SGM, 172) );
	_Strsprintf(StrLine, "發票兌獎%8d  %10s %8s", iPayinvoamt, "現金合計", GetNumber(_StringSegment_EX(XRData, SGM, 45)).c_str());
    to_ptr(StrLine);

    //to_ptr( ("借零合計" + InsertSpace(GetNumber(XRData.SubString(1639,8)),8)) );
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1647,8)),0));



	_Strsprintf(StrLine, "投庫合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 50)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 51)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "更正合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 52)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 53)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "交易取消合計%6s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 54)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 55)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "折扣(應稅)%8s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 61)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 62)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "商品組合折扣/讓%3s%8s   %8s", " ",
                                                  GetNumber(_StringSegment_EX(XRData, SGM, 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(XRData, SGM, 64)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "加值機 開/售卡%4s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 65)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 66)).c_str() );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "加值機加值%8s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 67)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, "|", 172) );
    //StrLine.sprintf("發票兌獎%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
   // to_ptr(StrLine);

	_Strsprintf(StrLine, "開錢櫃%12s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 60)).c_str());
    to_ptr(StrLine);

    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e通卡現金加值%6s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 69)).c_str(),
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 70)).c_str() );
    //to_ptr(StrLine);
    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e通卡信用卡加值%4s %8s   %8s"," ",
    //                                           GetNumber(_StringSegment_EX(XRData, "|", 71)).c_str(),
    //                                           GetNumber(_StringSegment_EX(XRData, "|", 72)).c_str() );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "部門登錄%10s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 58)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 59)).c_str() );
    to_ptr(StrLine);    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1799,8)),0));


	_Strsprintf(StrLine, "廢棄%14s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 84)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 85)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "金額０發票%8s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 81)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "訓練合計%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 56)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 57)).c_str() );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "結束發票%19s%10s", " ", _StringSegment_EX(XRData, SGM, 16).c_str());
    to_ptr(StrLine);

   // int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
   // string sCpnCnt, sCpnAmt;

   // icpn77cnt=_StrToInt(sPayStr.SubString(225,8));
   // icpn77amt=_StrToInt(sPayStr.SubString(233,8));
   // icpn78cnt=_StrToInt(sPayStr.SubString(241,8));
   // icpn78amt=_StrToInt(sPayStr.SubString(249,8));
  //  sCpnCnt.sprintf("%8d",icpn77cnt+icpn78cnt);
   // sCpnAmt.sprintf("%8d",icpn77amt+icpn78amt);

  //  to_ptr("");
  //  to_ptr( ("提貨券金額" + InsertSpace(GetNumber(sCpnAmt),10)));

    to_ptr("");

    //to_ptr("        ***     "+_StringSegment_EX(XRData, "|", 79)+"."+ _StringSegment_EX(XRData, "|", 80)+"     ***");
    to_ptr("       ***     "+_StringSegment_EX(XRData, SGM, 79)+ _StringSegment_EX(XRData, SGM, 80)+"     ***");

    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");


    return 0;
}

string __fastcall REPORT::ToTimeFormat(string S)
{
    string str_time;

    str_time = (S.substr(0,4)).insert(2,":");
    return str_time;
}

REPORT::REPORT()
{
    tslSaveToFile = new TStringList;
    char_filename[0] = 0;
	sprintf_s(gchar_date_format, sizeof(gchar_date_format), "%10s", currentDateTime(4).c_str()); //    FormatDateTime("yyyy-mm-dd", Now()).c_str());
}

//代售報表
int __fastcall REPORT::subsale(TStringList *tsl, const char *ptr_tencode, const char *ptr_ecr_no)
{
    string str_tencode, str_ecr_no, StrLine;     //店號、機號
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;
    int int_amt = 0;                        //代售金額加總

    //單筆記錄、商品名稱、銷售金額
    string str_record, str_bar_name, str_item_amt;

    StrLine="";
	_Strsprintf(StrLine,"%s序號:%s店號:%s 機:%s", gchar_date_format, "00000",
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr("序號：000000");
    //to_ptr(("店號：" + str_tencode + "     機號：" + str_ecr_no));
    to_ptr("");

    to_ptr("           代售商品金額報表     ");
    to_ptr("");

	_Strsprintf(StrLine, "序%s商品名稱%17s銷售金額", " ", " ");
    to_ptr(StrLine);
    int int_space = 1;
	int i = 0;
    //for (int i=0; i<tsl->Count; i++)
	for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++, i++)
    {
		str_record = *ls; // tsl->Strings[i];
        str_bar_name = _StringSegment_EX(str_record, "|", 3);
        str_item_amt = _StringSegment_EX(str_record, "|", 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //刪除全形空白
        {
			int_space = _Pos(str_bar_name, "　");
            if (int_space)
            {
                str_bar_name.erase(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //刪除半形空白
        {
			int_space = _Pos(str_bar_name," ");
            if (int_space)
            {
                str_bar_name.erase(int_space,1);
            }

        }

		_Strsprintf(str_record,"%02d.%-26s%7s", i + 1, str_bar_name.substr(0, 26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);

        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
    }

    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    合計%16d"," ",int_amt);
	_Strsprintf(str_record,"合計%15s %16d", " ", int_amt);
    to_ptr(str_record);

	// 2019/11/06 For SCO
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");

    return 0;
}
