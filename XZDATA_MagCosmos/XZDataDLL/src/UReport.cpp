//---------------------------------------------------------------------------

#include <vcl.h>
#include "UReport.h"

//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)

//部門帳處理程序
// 部門促銷折扣金額        Lu 2018/06/13
int __fastcall REPORT::Dept(AnsiString &ZData, char *ptr_tencode, char *ptr_ecr_no,  int XR,
                            TStringList *tsl, AnsiString s1093 )
{

    AnsiString str_nz_cnt;             //交易序號
    AnsiString str_tencode;           //店號
    AnsiString str_ecr_no;             //機號
    AnsiString str_bg_noclose;

    int int_dept = 88;                //前一個部門在ZDATA中開始位子



    //if( XR==21 || XR==22)
       str_bg_noclose.sprintf("%08d", _StrToInt(_StringSegment_EX(ZData, "|", 7), "ZData"));


    //將部門儲存至TStringList方便處理
    TStringList *tsl_zdata = new TStringList;

    AnsiString sDpt;
    for (int i=0; i<20; i++)
    {
        int_dept++;    //89 開始
        sDpt=_StringSegment_EX(ZData, "|", int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, "|", int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, "|", int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, "|", int_dept);

        tsl_zdata->Add(sDpt);        //部門每一段資料為32

    }

    if (XR==1)
    {
        str_nz_cnt = "00000";
    }
    else
    {
        str_nz_cnt=_StringSegment_EX(ZData, "|", 6); // 2013/07/22
        /***
        TIniFile *tif_ecr;
        try
        {
            tif_ecr = new TIniFile(XZDATA_INI);
            str_nz_cnt = tif_ecr->ReadString("Z","NZ_CNT","");     //累積結帳次數序號

            if (str_nz_cnt.Length() < 5)
                str_nz_cnt = "00001";
        }
        __finally
        {
            delete tif_ecr;
        }
        ***/

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

    try
    {
        dept = new sut_dept[20];

        //儲存於TStringList中部門帳寫入結構中
        for (int i=0; i<20; i++)
        {
            dept[i].qty = _StrToInt(tsl_zdata->Strings[i].SubString(1,8), tsl_zdata->Strings[i]);
            dept[i].amt = _StrToInt(tsl_zdata->Strings[i].SubString(9,8), tsl_zdata->Strings[i]);
            dept[i].rqty = _StrToInt(tsl_zdata->Strings[i].SubString(17,8), tsl_zdata->Strings[i]);
            dept[i].ramt = _StrToInt(tsl_zdata->Strings[i].SubString(25,8), tsl_zdata->Strings[i]);
        }
    }
    __finally
    {
        delete tsl_zdata;
    }



    AnsiString qtyamt,  //數量加總及銷售金額加總
               ramt;    //誤打、退貨/重印金額加總
    AnsiString   StrLine;

    StrLine="";

    if( XR==21 || XR==22)
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
    else
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_nz_cnt,
                                           str_tencode,str_ecr_no);
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
    AnsiString sCount;
    int int_a = 0, int_b = 0;

  //StrLine.sprintf("部門   銷數 銷售金額      退數 退貨金額");
    StrLine.sprintf("部門   銷數 銷售金額    退數 退貨金額");
    to_ptr(StrLine);
    for (int i=0; i<20; i++)
    {
        int_a -= dept[i].rqty;
        int_b -= dept[i].ramt;

        qtyamt.sprintf("%5d %8d", dept[i].qty,dept[i].amt);            //"%6d %8d"
        ramt.sprintf("%5d %8d", int_a, int_b);                         //"%6d %8d"
        sCount.sprintf("%02d", (i + 1));

      //StrLine.sprintf("%3s   %s     %s",sCount, qtyamt, ramt);
        StrLine.sprintf("%3s   %s  %s",sCount, qtyamt, ramt);
        to_ptr(StrLine);


        //to_ptr(("部門" + sCount));
        //to_ptr(qtyamt);
        //to_ptr(ramt);
        //to_ptr("");

        int_a = 0;
        int_b = 0;
    }

    //to_ptr("部門合計");

    int_a -= _StrToInt(_StringSegment_EX(ZData, "|", 43));
    int_b -= _StrToInt(_StringSegment_EX(ZData, "|", 44));

    qtyamt.sprintf("%5d %8d", _StrToInt(_StringSegment_EX(ZData, "|", 41)),       //"%6d %8d"
                              _StrToInt(_StringSegment_EX(ZData, "|", 42))   );   //"%6d %8d"
    ramt.sprintf("%5d %8d", int_a, int_b);

    StrLine.sprintf("--------------------------------------");
    to_ptr(StrLine);

    StrLine.sprintf("合計 %s   %s", qtyamt, ramt);
    to_ptr(StrLine);

    //to_ptr("\n\n\n\n");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //部門合計加總
    //to_ptr("部門合計");
    //to_ptr(qtyamt);
    //to_ptr(ramt);

    /////////////////////////////////  部門促銷報表  ////////////////////////////////////
    int int_a_Tot,  int_b_Tot;
    if( s1093 != NULL &&  s1093.Trim() != "")
      {
       int_a = 0; int_b = 0; int_a_Tot=0;  int_b_Tot =0;

       to_ptr("");

       if( XR==21 || XR==22)
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
       else
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_nz_cnt,
                                           str_tencode,str_ecr_no);
       to_ptr(StrLine);

       if (XR==21) //21:簽到 22:簽退
            to_ptr("        部門促銷報表(簽到)     ");
       else if (XR==22) //21:簽到 22:簽退
            to_ptr("        部門促銷報表(簽退)     ");
       else
            to_ptr("           部門促銷報表     ");

       to_ptr("");

       StrLine.sprintf("部門   銷售促銷金額   退貨促銷金額");
       to_ptr(StrLine);
       for (int i=1; i<21; i++)
         {
             int_a = _StrToInt(_StringSegment_EX(s1093, "|", (2*i)-1 ) );
             int_a_Tot += int_a  ;  // total
             int_b = _StrToInt(_StringSegment_EX(s1093, "|", (2*i) ) );
             int_b_Tot += int_b  ;  // total

             qtyamt.sprintf("%8d", int_a);
             ramt.sprintf("%8d", int_b);
             sCount.sprintf("%02d", (i));

             StrLine.sprintf("%3s   %s      %s",sCount, qtyamt, ramt);
             to_ptr(StrLine);
             int_a = 0;
             int_b = 0;
         }

      ramt.sprintf("%8d      %8d",  int_a_Tot, int_b_Tot );

      StrLine.sprintf("--------------------------------------");
      to_ptr(StrLine);

      StrLine.sprintf("合計  %s   ", ramt);
      to_ptr(StrLine);

      //to_ptr("\n\n\n\n");
      to_ptr("");
      to_ptr("");
      to_ptr("");
      to_ptr("");
      // 2019/11/06 For SCO
      //StrLine.sprintf("\x1b\x69");
      //to_ptr(StrLine);
    }
    ///////////////////////代售商品金額報表//////////////////////////////////

   if( ( XR==21 || XR==22) && tsl )
     {
       int int_amt = 0;                        //代售金額加總

       //單筆記錄、商品名稱、銷售金額
       AnsiString str_record, str_bar_name, str_item_amt;
        // 2019/11/06 For SCO
       to_ptr("---------------------------------------");
       StrLine="";
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
       to_ptr(StrLine);

       to_ptr("");
       to_ptr("           代售商品金額報表     ");
       to_ptr("");

       StrLine.sprintf("序%s商品名稱%17s銷售金額"," ", " ");
       to_ptr(StrLine);
       int int_space = 1;
       for (int i=0; i<tsl->Count; i++)
       {
        str_record = tsl->Strings[i];
        str_bar_name = _StringSegment(str_record, "|", 3);
        str_item_amt = _StringSegment(str_record, "|", 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //刪除全形空白
        {
            int_space = str_bar_name.Pos("　");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //刪除半形空白
        {
            int_space = str_bar_name.Pos(" ");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }
        }

        str_record.sprintf("%02d.%-26s%7s",i+1 ,str_bar_name.SubString(1,26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);
 
        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
       }


    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    合計%16d"," ",int_amt);
    str_record.sprintf("合計%15s %16d"," ",int_amt);
    to_ptr(str_record);

    // 2019/11/06 For SCO
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");

    StrLine.sprintf("\x1b\x69");
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
int __fastcall REPORT::INQReport( char *ptr_tencode, char *ptr_ecr_no,
                                 INQTBL *InqTbl)
{

    AnsiString str_nz_cnt;             //交易序號
    AnsiString str_tencode;           //店號
    AnsiString str_ecr_no;             //機號
      

    str_tencode = ptr_tencode;                  //店號
    str_ecr_no  = ptr_ecr_no;                    //機號

    TIniFile *tif_ecr;
    try
        {
            tif_ecr = new TIniFile(XZDATA_INI);
            str_nz_cnt = tif_ecr->ReadString("Z","NZ_CNT","");     //累積結帳次數序號

            if (str_nz_cnt.Length() < 5)
                str_nz_cnt = "00001";
        }
    __finally
        {
            delete tif_ecr;
        }
   
    AnsiString qtyamt,  //數量加總
               ramt;    //金額加總
   
    to_ptr(gchar_date_format);
    to_ptr(("序號：" + str_nz_cnt));
    to_ptr(("店號：" + str_tencode + "    " + "機號：" + str_ecr_no));
    to_ptr("");
 // to_ptr("       部門帳表        ");
    to_ptr(" 即時連線商品交易統計表 ");
    to_ptr("");

    //輸出至檔案
    AnsiString sCount;
    int int_a = 0, int_b = 0;

    for (int i=1; i<20; i++)
    {
        int_a = InqTbl[i].qt_InqSale;
        int_b = InqTbl[i].am_InqSale;
        if( int_a > 0 || int_b > 0)
          {
           qtyamt.sprintf("%6d%18d", int_a,int_b);
           //ramt.sprintf("%6d%18d", int_a, int_b);
           sCount.sprintf("%02d", i);

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


// int __fastcall REPORT::X(AnsiString &XData, AnsiString sPayStr, char *ptr_tencode   )
//XData: 1095 收銀員帳表電文檔
int __fastcall REPORT::XDTReport(AnsiString &XData, AnsiString sPayStr, char *ptr_tencode,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int AutoZflg )
{

    AnsiString str_tencode, StrLine, NowDDHHmm, PreDDHHmm;
    str_tencode = ptr_tencode;

    StrLine="";

    StrLine.sprintf("%s序號:%5s 店號:%s 機:%s",gchar_date_format,
                                               _StringSegment_EX(XData, "|", 8),
                                               str_tencode,
                                               _StringSegment_EX(XData, "|", 4) );
    to_ptr(StrLine);
    //to_ptr(gchar_date_format);
    //to_ptr(("序號：" + XData.SubString(7,2)));
    //to_ptr(("店號：" + str_tencode + "     " + "機號：" + XData.SubString(1,1)));
    to_ptr("");

    to_ptr("             收銀員明細表        ");
    to_ptr("");

    to_ptr( ("收銀員 NO " + _StringSegment_EX(XData, "|", 9)  ) );

    //結帳日期+結帳時間
    NowDDHHmm=_StringSegment_EX(XData, "|", 12).SubString(7,2)+_StringSegment_EX(XData, "|", 13).SubString(1,4);
    //前次結帳日期+前次結帳時間
    PreDDHHmm=_StringSegment_EX(XData, "|", 10).SubString(7,2)+_StringSegment_EX(XData, "|", 11).SubString(1,4);

    //StrLine.sprintf("結帳時間    開始 %s 結束 %s",
    StrLine.sprintf("結帳時間    本次 %s 前次 %s",
                     ToDateFormat( NowDDHHmm ).c_str(),
                     ToDateFormat( PreDDHHmm ).c_str() );
    to_ptr(StrLine);
    //to_ptr(("收銀員 NO" + InsertSpace(XData.SubString(2,5),9)) );
    //to_ptr(("開始時間" + InsertSpace(ToDateFormat(XData.SubString(15,6)),8)));
    //to_ptr(("結束時間" + InsertSpace(ToDateFormat(XData.SubString(29,6)),8)));
    to_ptr("");


    StrLine.sprintf("交易品項    %8s  本日來客數%7s",
                     GetNumber(_StringSegment_EX(XData, "|", 15) ).c_str(),
                     GetNumber(_StringSegment_EX(XData, "|", 14) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    //to_ptr(("代收商品客數" + InsertSpace(GetNumber(XData.SubString(3293,8)),12)));

    StrLine.sprintf("發票結帳金額(GS)%14s %8s"," ", GetNumber(_StringSegment_EX(XData, "|", 20)).c_str() );
    to_ptr(StrLine);
    //to_ptr("發票結帳金額(GS)");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(69,8)),0));

    //  FFU4  3293
    //  FFU4  3301 金額
    StrLine.sprintf("不開發票金額%18s %8s"," ", GetNumber(_StringSegment_EX(XData, "|", 77) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("不開發票金額");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3301,8)),0));

    StrLine.sprintf("誤,退/重合計%18s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 24) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("誤、退/重合計");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(101,8)),0));

    StrLine.sprintf("不開發票誤,退/重合計%10s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 78) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("不開發票誤、退/重合計");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3189,8)),0));   //加值機 結帳金額, 不開發票金額誤退重

    StrLine.sprintf("實際銷貨收入(NS)%14s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 25) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("代售,手續費來客數%13s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 212) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額%11s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 210) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額%11s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 211) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售商品折讓金額   %11s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 44) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("健康捐合計%20s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 81) ).c_str() );
    //to_ptr(StrLine);

    StrLine.sprintf("代收合計%11s %8s   %8s"," ", GetNumber(_StringSegment_EX(XData, "|", 72) ).c_str(),
                                                  GetNumber(_StringSegment_EX(XData, "|", 73) ).c_str() );
    to_ptr(StrLine);


    //StrLine.sprintf("代售合計%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    StrLine.sprintf("代售商品合計%7s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    to_ptr(StrLine);

    StrLine.sprintf("即時購代售合計%16s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 82) ).c_str() );
    to_ptr(StrLine);

    //即時購合計
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XData, "|", 204) );
    iVDCNx=_StrToInt( _StringSegment_EX(XData, "|", 205) );
    //StrLine.sprintf("即時購與銷轉進值%14s %8s"," ",GetNumber( _StringSegment_EX(XData, "|", 204) ).c_str() );
    StrLine.sprintf("即時購與銷轉進值%14s %8d"," ",iVDCTx+iVDCNx );
    to_ptr(StrLine);

    StrLine.sprintf("手續費合計%20s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 81)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("代付合計%11s %8d   %8d"," ", iqt5_18219totrev,iam5_18219totrev );
    to_ptr(StrLine);


    StrLine.sprintf("溢收     %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 41) ).c_str(),
                                             "禮券",
                                              GetNumber(_StringSegment_EX(XData, "|", 31) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("找餘額券 %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 39) ).c_str(),
    //                                         "折價券",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 35) ).c_str()  );
    StrLine.sprintf("找餘額券 %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 39) ).c_str(),
                                             "禮物卡",
                                              GetNumber(_StringSegment_EX(XData, "|", 209) ).c_str()  );
    to_ptr(StrLine);

    //StrLine.sprintf("刷卡合計 %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 69) ).c_str(),
    //                                         "代現金合計",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 37) ).c_str()  );
    StrLine.sprintf("刷卡合計 %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 69) ).c_str(),
                                             "折價券",
                                              GetNumber(_StringSegment_EX(XData, "|", 35) ).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("代現金合計%20s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 37) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("現金合計%22s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 42) ).c_str() );
    //to_ptr(StrLine);


    //20151208 全家需求變更，調整Z帳表食安顯示位置
    StrLine.sprintf("食安退款%11s     %15s"," ",
                                              //GetNumber(_StringSegment_EX(XData, "|", 66) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 67) ).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XData, "|", 206) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XData, "|", 207) );

    StrLine.sprintf("發票兌獎 %8d   %10s %8s", iPayinvoamt, "現金合計", GetNumber(_StringSegment_EX(XData, "|", 42) ).c_str() );
    to_ptr(StrLine);

    TStringList *tsl_drop;

    AnsiString sDrop;
    int iDropCnt=0;
    try
    {
        tsl_drop = new TStringList;

        int int_position = 144;    //LOG中投庫開始位置


        for (int i=0; i<30; i++)
        {
            sDrop=_StringSegment_EX(XData, "|", int_position)+_StringSegment_EX(XData, "|", int_position+1);
            tsl_drop->Add(sDrop);
            int_position += 2;
        }

        AnsiString str_drop_time,str_drop_money,str_drop_data;
        int int_drop_money = 0;

        to_ptr("投庫");

        for (int i=0; i<30; i++)
        {
            str_drop_time = tsl_drop->Strings[i].SubString(9,6);

            if (str_drop_time == "000000" || Trim(str_drop_time)=="" )
            {
                break;
            }
            else
            {
                str_drop_time = ToTimeFormat(str_drop_time);
                int_drop_money = _StrToInt(tsl_drop->Strings[i].SubString(1,8),tsl_drop->Strings[i]);
                str_drop_money = InsertSpace(IntToStr(int_drop_money),12);

                if (i < 9)
                    str_drop_data = ("     " + IntToStr(i + 1) + " " + str_drop_time + "     " + str_drop_money);
                else
                    str_drop_data = ("    " + IntToStr(i + 1) + " " + str_drop_time + "     " + str_drop_money);


                to_ptr(str_drop_data);
                iDropCnt++;
            }
        }
    }
    __finally
    {
        delete tsl_drop;
    }

    //投庫次數
    //投庫合計金額
    //StrLine.sprintf("合計%15s %8s   %8s"," ",GetNumber(XData.SubString(3077,8)).c_str(),
    //                                             GetNumber(XData.SubString(3085,8)).c_str() );
    StrLine.sprintf("合計%8s         %8s",GetNumber(_StringSegment_EX(XData, "|", 47) ).c_str(),
                                          GetNumber(_StringSegment_EX(XData, "|", 48) ).c_str() );
    to_ptr(StrLine);
    to_ptr("");


    StrLine.sprintf("更正合計%11s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 49) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 50) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("交易取消合計%7s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 51) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 52) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("折扣%15s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 56) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 57) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("商品組合折扣/讓%4s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 58) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 59) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("加值機 開/售卡%5s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 60) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 61) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("加值機加值%9s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 62) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 63) ).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XData, "|", 206) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XData, "|", 207) );
    //StrLine.sprintf("發票兌獎%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);

    StrLine.sprintf("開錢櫃%13s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 55) ).c_str() );
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

    StrLine.sprintf("e通卡現金加值%6s %8s   %8s"," ",
                                              GetNumber(_StringSegment_EX(XData, "|", 64) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 65) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("e通卡信用卡加值%4s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 66) ).c_str(),
    //                                          GetNumber(_StringSegment_EX(XData, "|", 67) ).c_str() );
    //to_ptr(StrLine);




   // to_ptr( ("e通卡結帳" + InsertSpace(GetNumber(XData.SubString(3389,8)),9)) );        //ffu10
   // to_ptr(InsertSpace(GetNumber(XData.SubString(3397,8)),0));                         //ffu10

    StrLine.sprintf("部門登錄%11s %8s   %8s"," ",
                                              GetNumber(_StringSegment_EX(XData, "|", 53) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 54) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("廢棄%15s %8s   %8s"," ",
                                              GetNumber(_StringSegment_EX(XData, "|", 79) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 80) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("金額０發票%9s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 76) ).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("結束發票%20s %10s"," ",_StringSegment_EX(XData, "|", 17).c_str()  );
    to_ptr(StrLine);


    int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
    AnsiString sCpnCnt, sCpnAmt;

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
    to_ptr( ("        ***     " + _StringSegment_EX(XData, "|", 74)+ _StringSegment_EX(XData, "|", 75)+ "     ***") );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");

    return 0;
}

//1096
// int __fastcall REPORT::Z(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no)
int __fastcall REPORT::ZDTReport(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int rpt_type )
{

    AnsiString str_bg_noclose;         //序號

    if( rpt_type==21 || rpt_type==22)
       str_bg_noclose.sprintf("%08d", _StrToInt(_StringSegment_EX(Zdata, "|", 7), "ZData"));
    else
      str_bg_noclose.sprintf("%05d", _StrToInt(_StringSegment_EX(Zdata, "|", 6), "ZData"));

    AnsiString str_tencode, str_ecr_no, StrLine, sTmp, sStDDHHMM, sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;

    StrLine="";
    if( rpt_type==21 || rpt_type==22)
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
    else
       StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("序號：" + str_bg_noclose));
    //to_ptr(("店號：" + str_tencode + "     機號：" + str_ecr_no));
    to_ptr("");


    //21:簽到 22:簽退
    if( rpt_type ==21)
        to_ptr("          盤點人員帳表(簽到)     ");
    else if( rpt_type ==22)
        to_ptr("          盤點人員帳表(簽退)     ");
    else
        to_ptr("                Z帳表        ");

    to_ptr("");

    sTmp=_StringSegment_EX(Zdata, "|", 10)+_StringSegment_EX(Zdata, "|", 11);
    sStDDHHMM=sTmp.SubString(7,6); //本次
    sTmp=_StringSegment_EX(Zdata, "|", 8)+_StringSegment_EX(Zdata, "|", 9);
    sPtDDHHMM=sTmp.SubString(7,6); //前次

    StrLine.sprintf("結帳時間  本次 %s 前次 %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("本日交易品項%7s 本日來客數%7s",
                     GetNumber(_StringSegment_EX(Zdata, "|", 13) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, "|", 12) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    //to_ptr( ("代收商品客數" + InsertSpace(GetNumber(Zdata.SubString(1887,8)),12)) );
    //to_ptr("");

    StrLine.sprintf("本日結帳累計金額%13s%8s"," ", GetNumber(_StringSegment_EX(Zdata, "|", 14) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("本日結帳累計金額");
    //to_ptr(InsertSpace(GetNumber(Zdata.SubString(51,8)),0));


    StrLine.sprintf("發票結帳金額%17s%8s"," ", GetNumber(_StringSegment_EX(Zdata, "|", 19) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("不開發票金額%17s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 82) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("發票結帳   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(Zdata, "|", 17) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, "|", 18) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("誤,退/重   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(Zdata, "|", 21) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, "|", 22) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("誤,退/重合計%17s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 23) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("不開發票誤,退/重合計%9s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 83) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("本日實際銷貨收入%13s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 24) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("代售,手續費來客數%12s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(免)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(應)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(免)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(應)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 176) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售商品折讓金額       %6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 47) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("健康捐合計%20s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 86)).c_str() );
    //to_ptr(StrLine);

    StrLine.sprintf("代收合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 77)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 78)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("代售合計%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    StrLine.sprintf("代售商品合計%6s%8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    to_ptr(StrLine);

    StrLine.sprintf("即時購代售合計%15s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 87)).c_str() );
    to_ptr(StrLine);

    //即時購合計
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(Zdata, "|", 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(Zdata, "|", 170) );
    //StrLine.sprintf("即時購與銷轉進值%14s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 169) ).c_str() );
    StrLine.sprintf("即時購與銷轉進值%13s%8d"," ",iVDCTx+iVDCNx );
    to_ptr(StrLine);

    StrLine.sprintf("手續費合計%19s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 86)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("代付合計%10s%8d   %8d"," ", iqt5_18219totrev,iam5_18219totrev );
    to_ptr(StrLine);


    StrLine.sprintf("溢收    %8s  %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 40)).c_str(),
                                             "禮券",
                                              GetNumber(_StringSegment_EX(Zdata, "|", 30)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("找餘額券 %8s   %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 38)).c_str(),
    //                                         "折價券",
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 34)).c_str()  );
    StrLine.sprintf("找餘額券%8s  %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 38)).c_str(),
                                             "禮物卡",
                                              GetNumber(_StringSegment_EX(Zdata, "|", 174)).c_str()  );
    to_ptr(StrLine);



    //StrLine.sprintf("刷卡合計 %8s   %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 74)).c_str(),
    //                                         "代現金合計",
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 36)).c_str()  );
    StrLine.sprintf("刷卡合計%8s  %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 74)).c_str(),
                                             "折價券",
                                              GetNumber(_StringSegment_EX(Zdata, "|", 34)).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("代現金合計%19s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 36)).c_str() );
    to_ptr(StrLine);


    //to_ptr("消費券合計");
    //to_ptr(InsertSpace(GetNumber(Zdata.SubString(1871,8)),0));

    //StrLine.sprintf("現金合計%22s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 45)).c_str() );
    //to_ptr(StrLine);


    //20151208 全家需求變更，調整Z帳表食安顯示位置
    StrLine.sprintf("食安退款%10s     %14s"," ",
                                             //GetNumber(_StringSegment_EX(Zdata, "|", 71)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 72)).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, "|", 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, "|", 172) );

    StrLine.sprintf("發票兌獎%8d  %10s %8s", iPayinvoamt, "現金合計", GetNumber(_StringSegment_EX(Zdata, "|", 45)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("投庫合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 50)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 51)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("更正合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 52)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 53)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("交易取消合計%6s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 54)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 55)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("折扣(應稅)%8s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 61)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 62)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("商品組合折扣/讓%3s%8s   %8s"," ",
                                                  GetNumber(_StringSegment_EX(Zdata, "|", 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(Zdata, "|", 64)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("加值機 開/售卡%4s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 65)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 66)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("加值機加值%8s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 67)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, "|", 172) );
    //StrLine.sprintf("發票兌獎%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);


    StrLine.sprintf("開錢櫃%12s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 60)).c_str() );
    to_ptr(StrLine);

    //  FFU4  1887
    //  FFU4  1895

    //  FFU5  1903
    //  FFU5  1911
    //  FFU6  1919
    //  FFU6  1927

    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e通卡現金加值%6s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 69)).c_str(),
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 70)).c_str() );
    //to_ptr(StrLine);
    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e通卡信用卡加值%4s %8s   %8s"," ",
    //                                           GetNumber(_StringSegment_EX(Zdata, "|", 71)).c_str(),
    //                                           GetNumber(_StringSegment_EX(Zdata, "|", 72)).c_str() );
    //to_ptr(StrLine);



    StrLine.sprintf("部門登錄%10s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 58)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 59)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("廢棄%14s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 84)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 85)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("金額０發票%8s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 81)).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("訓練合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 56)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 57)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("結束發票%19s%10s"," ",_StringSegment_EX(Zdata, "|", 16).c_str()  );
    to_ptr(StrLine);
     
    to_ptr("");

    //to_ptr("        ***     "+_StringSegment_EX(Zdata, "|", 79)+"."+ _StringSegment_EX(Zdata, "|", 80)+"     ***" );
    to_ptr("      ***     "+_StringSegment_EX(Zdata, "|", 79)+ _StringSegment_EX(Zdata, "|", 80)+"     ***" );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");

    return 0;
}


void __fastcall REPORT::to_ptr(AnsiString S)
{
    //輸出檔案
    if (S == "Save")       //X帳報表
    {
        if (FileExists(char_filename))
            DeleteFile(char_filename);

        tslSaveToFile->SaveToFile(char_filename);

        tslSaveToFile->Clear();

        return;     //to_ptr()
    }
    else    //儲存字串
    {
        //if (gbl_epson_prt == false)
        //    S.sprintf("%cPR1%s", ESC, S.c_str());

        tslSaveToFile->Add(S);
    }
}


//int REPORT::CreateReport(char rpt_type, AnsiString sSal, AnsiString sPayStr,
//                         char *ptr_tencode, char *ptr_ecr_no,TStringList *tsl)
// 部門促銷折扣金額        Lu 2018/06/13
int REPORT::CreateReport(char rpt_type, AnsiString sSal, AnsiString sPayStr,
                     char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev,
                     TStringList *tsl, int AutoZflg, AnsiString s1093 )
{
    //選擇報表 1部門 2X帳 3Z帳
    switch (rpt_type)
    {
      case 'd':                                 //部門
          Dept(sSal, ptr_tencode, ptr_ecr_no,1, NULL, "");
          break;
      case 'E':                                 //盤點人員部門   21:簽到 22:簽退
          Dept(sSal, ptr_tencode, ptr_ecr_no,21, tsl, s1093);
          break;
      case 'e':                                 //盤點人員部門   21:簽到 22:簽退
          Dept(sSal, ptr_tencode, ptr_ecr_no,22, tsl, s1093);
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

      case 'X':                                 //XReport
          XReport(sSal, sPayStr, ptr_tencode, ptr_ecr_no,
                  iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev);
          break;
      case 's':                                 //代售報表
          subsale(tsl, ptr_tencode, ptr_ecr_no);
    }

    return 0;
}


REPORT::~REPORT()
{
    delete tslSaveToFile;
}

/*
AnsiString __fastcall REPORT::GetNumber(AnsiString S)
{
    AnsiString str_num;

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

AnsiString __fastcall REPORT::ToDateFormat(AnsiString S)
{
    AnsiString str_date_format;

    str_date_format = S;
    str_date_format.Insert("-", 3);
    str_date_format.Insert(":", 6);

    return str_date_format;
}

AnsiString __fastcall REPORT::InsertSpace(AnsiString S, int i)
{
    int int_len = 0;

    int_len = S.Length();

    int int_space = (24 - int_len - i);

    AnsiString str_space = S;

    for (int i=0; i<int_space; i++)
        str_space.Insert(" ", 1);

    return str_space;
}

//int __fastcall REPORT::XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no)

// 讀取 1096 data
int __fastcall REPORT::XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no,
                             int iqt5_15217totrev, int iam5_15217totrev,
                             int iqt5_18219totrev, int iam5_18219totrev )
{

    //AnsiString str_bg_noclose;         //序號
    //str_bg_noclose.sprintf("%06d", _StrToInt(XRData.SubString(2,5), "XRData"));

    AnsiString str_tencode,str_ecr_no,StrLine,sTmp,sStDDHHMM,sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no = ptr_ecr_no;


    StrLine="";
    StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format, "00000",
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("序號：000000"));
    //to_ptr(("店號：" + str_tencode + "     機號：" + str_ecr_no));
    to_ptr("");

    to_ptr("                X帳表        ");
    to_ptr("");

    sTmp=_StringSegment_EX(XRData, "|", 10)+_StringSegment_EX(XRData, "|", 11);
    sStDDHHMM=sTmp.SubString(7,6); //本次
    sTmp=_StringSegment_EX(XRData, "|", 8)+_StringSegment_EX(XRData, "|", 9);
    sPtDDHHMM=sTmp.SubString(7,6); //前次

    StrLine.sprintf("結帳時間  本次 %s 前次 %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );

    //StrLine.sprintf("結帳時間   本次 %s  前次 %s",
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 11) ).c_str(),
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 9) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("本日交易品項%7s 本日來客數%7s",
                     GetNumber(_StringSegment_EX(XRData, "|", 13) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, "|", 12) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    // to_ptr(("代收商品客數" + InsertSpace(GetNumber(XRData.SubString(1887,8)),12)));
    //to_ptr("");

    StrLine.sprintf("本日結帳累計金額%13s%8s"," ", GetNumber(_StringSegment_EX(XRData, "|", 14) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("發票結帳金額%17s%8s"," ", GetNumber(_StringSegment_EX(XRData, "|", 19) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("不開發票金額%17s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 82) ).c_str() );
    to_ptr(StrLine);

    //  FFU4  1887
    //  FFU4  1895 金額

    StrLine.sprintf("發票結帳   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(XRData, "|", 17) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, "|", 18) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("誤,退/重   %8s(免)  %8s(應)",
                     GetNumber(_StringSegment_EX(XRData, "|", 21) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, "|", 22) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("誤,退/重合計%17s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 23) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("不開發票誤,退/重合計%9s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 83) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("本日實際銷貨收入%13s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 24) ).c_str() );
    to_ptr(StrLine);

    // 2015/08/19 Update Lu
    StrLine.sprintf("代售,手續費來客數%12s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(免)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(應)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(免)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(應)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 176) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售商品折讓金額       %6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 47) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("健康捐合計%20s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 86)).c_str() );
    //to_ptr(StrLine);

    StrLine.sprintf("代收合計%11s%8s  %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 77)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 78)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("代售合計%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    StrLine.sprintf("代售商品合計%7s%8d  %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    to_ptr(StrLine);

    StrLine.sprintf("即時購代售合計%15s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 87)).c_str() );
    to_ptr(StrLine);

    //即時購合計
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XRData, "|", 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(XRData, "|", 170) );
    //StrLine.sprintf("即時購與銷轉進值%14s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 169) ).c_str() );
    StrLine.sprintf("即時購與銷轉進值%13s%8d"," ",iVDCTx+iVDCNx );
    to_ptr(StrLine);

    StrLine.sprintf("手續費合計%19s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 86)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("代付合計%11s%8d  %8d"," ", iqt5_18219totrev,iam5_18219totrev );
    to_ptr(StrLine);

    StrLine.sprintf("溢收   %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 40)).c_str(),
                                             "禮券",
                                              GetNumber(_StringSegment_EX(XRData, "|", 30)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("找餘額券 %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 38)).c_str(),
    //                                         "折價券",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 34)).c_str()  );
    StrLine.sprintf("找餘額券%7s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 38)).c_str(),
                                             "禮物卡",
                                              GetNumber(_StringSegment_EX(XRData, "|", 174)).c_str()  );
    to_ptr(StrLine);


    //StrLine.sprintf("刷卡合計 %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 74)).c_str(),
    //                                         "代現金合計",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 36)).c_str()  );
    StrLine.sprintf("刷卡合計%7s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 74)).c_str(),
                                             "折價券",
                                              GetNumber(_StringSegment_EX(XRData, "|", 34)).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("代現金合計%19s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 36)).c_str() );
    to_ptr(StrLine);


    //to_ptr("消費券合計");
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1871,8)),0));  //1871

    //StrLine.sprintf("現金合計%22s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 45)).c_str() );
    //to_ptr(StrLine);


     //20151208 全家需求變更，調整Z帳表食安顯示位置
    StrLine.sprintf("食安退款%11s    %14s"," ",
                                             //GetNumber(_StringSegment_EX(XRData, "|", 71)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 72)).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, "|", 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, "|", 172) );
    StrLine.sprintf("發票兌獎%8d  %10s %8s",iPayinvoamt, "現金合計", GetNumber(_StringSegment_EX(XRData, "|", 45)).c_str() );
    to_ptr(StrLine);

    //to_ptr( ("借零合計" + InsertSpace(GetNumber(XRData.SubString(1639,8)),8)) );
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1647,8)),0));



    StrLine.sprintf("投庫合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 50)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 51)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("更正合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 52)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 53)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("交易取消合計%6s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 54)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 55)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("折扣(應稅)%8s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 61)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 62)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("商品組合折扣/讓%3s%8s   %8s"," ",
                                                  GetNumber(_StringSegment_EX(XRData, "|", 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(XRData, "|", 64)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("加值機 開/售卡%4s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 65)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 66)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("加值機加值%8s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 67)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, "|", 172) );
    //StrLine.sprintf("發票兌獎%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
   // to_ptr(StrLine);

    StrLine.sprintf("開錢櫃%12s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 60)).c_str() );
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

    StrLine.sprintf("部門登錄%10s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 58)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 59)).c_str() );
    to_ptr(StrLine);    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1799,8)),0));


    StrLine.sprintf("廢棄%14s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 84)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 85)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("金額０發票%8s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 81)).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("訓練合計%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 56)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 57)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("結束發票%19s%10s"," ",_StringSegment_EX(XRData, "|", 16).c_str()  );
    to_ptr(StrLine);

   // int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
   // AnsiString sCpnCnt, sCpnAmt;

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
    to_ptr("       ***     "+_StringSegment_EX(XRData, "|", 79)+ _StringSegment_EX(XRData, "|", 80)+"     ***");


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");


    return 0;
}

AnsiString __fastcall REPORT::ToTimeFormat(AnsiString S)
{
    AnsiString str_time;

    str_time = (S.SubString(1,4)).Insert(":",3);
    return str_time;
}

REPORT::REPORT()
{
    tslSaveToFile = new TStringList;
    char_filename[0] = 0;
    sprintf(gchar_date_format, "%10s", FormatDateTime("yyyy-mm-dd",Now()).c_str());
}

//代售報表
int __fastcall REPORT::subsale(TStringList *tsl, char *ptr_tencode, char *ptr_ecr_no)
{
    AnsiString str_tencode, str_ecr_no, StrLine;     //店號、機號
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;
    int int_amt = 0;                        //代售金額加總

    //單筆記錄、商品名稱、銷售金額
    AnsiString str_record, str_bar_name, str_item_amt;


    StrLine="";
    StrLine.sprintf("%s序號:%s店號:%s 機:%s",gchar_date_format,"00000",
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr("序號：000000");
    //to_ptr(("店號：" + str_tencode + "     機號：" + str_ecr_no));
    to_ptr("");

    to_ptr("           代售商品金額報表     ");
    to_ptr("");

    StrLine.sprintf("序%s商品名稱%17s銷售金額"," ", " ");
    to_ptr(StrLine);
    int int_space = 1;
    for (int i=0; i<tsl->Count; i++)
    {
        str_record = tsl->Strings[i];
        str_bar_name = _StringSegment(str_record, "|", 3);
        str_item_amt = _StringSegment(str_record, "|", 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //刪除全形空白
        {
            int_space = str_bar_name.Pos("　");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //刪除半形空白
        {
            int_space = str_bar_name.Pos(" ");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }

        }


        str_record.sprintf("%02d.%-26s%7s",i+1 ,str_bar_name.SubString(1,26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);


        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
    }


    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    合計%16d"," ",int_amt);
    str_record.sprintf("合計%15s %16d"," ",int_amt);
    to_ptr(str_record);

    // 2019/11/06 For SCO
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    //to_ptr("");

    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //寫入檔案
    to_ptr("Save");

    return 0;
}
