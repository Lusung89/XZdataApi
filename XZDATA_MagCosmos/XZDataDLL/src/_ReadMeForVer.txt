■ XzDATA.dll



XZDATA.DLL Ver: 3.18.101.1

if( Trim(SaleDataVer)=="")
        SaleDataVer="2017120100";  //Default     last:"2017080100"




XZDATA.DLL Ver: 3.17.815.1
1. if( Trim(SaleDataVer)=="")
        SaleDataVer="2017080100";  //Default     last:"2017060100"
2.
  ////////   中獎發票短溢收  ////////////
   strtmp=AutoCrpIni->ReadString("AutoX","wininvotot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> wininvotot = %s , 中獎發票短溢收:%8ld", strtmp, itotWinInvoAmt41+i2 );
   writelog(sLog);

   strtmp.sprintf("%s中獎發票短溢收:%5ld │         │",CmdStr.c_str(),itotWinInvoAmt41+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("獎短|%02d|%8ld|", 0, itotWinInvoAmt41+i2);
      else
       strtmp.sprintf("獎短|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","wininvotot",strtmp);
     }

  ////////

3.
   else if ( str_sale_type.Pos("R4") )
            {
                //點卡代售手續費應稅作廢 giE   Lu Update 2015/0819
                iT5 = _StrToInt(_StringSegment_EX(s, "|", 49));
                if (iT5)
                {
                    giD += iT5;
                    giE -= iT5;
                }

                //點卡代售手續費免稅作廢 giH   Lu Update 2015/0819
                iT7 = _StrToInt(_StringSegment_EX(s, "|", 50));
                if (iT7)
                {
                    giG += iT7;
                    giH -= iT7;
                }

            }


4.
   if ( str_sale_type.Pos("A") && str_spc.Pos("10") )     //預售兌換商品 不扣除折讓單
               {
                 //    iSubSubAmt=0;                               // 2017/07/10 要扣除折讓單 ,所以 Mark
                 iDisSubAmt=0;                                     // 2017/09/21 因已扣除 類MM, 所以無須再扣  MM 折扣 攤題 , 因類MM=MM 折扣
               }








XZDATA.DLL Ver: 3.17.308.1

if( Trim(SaleDataVer)=="")
        SaleDataVer="2017010100";  //Default     last:"2016100100"

/////
SumCpn41 +=  giD;         //折價券+Other1+有價票券 CPN
int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);


//預售類(折價券S0 10/MMS0 11 )折扣
    int iPreSalCPNDisAmt, iPreSalMMDisAmt ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giA;
    iPreSalMMDisAmt=giB;

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giA;


sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");      //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    x_f += (x->tb_casher.am_vendtot+"|");      //借零合計金額




//收銀員交班明細表
int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sXdtFile,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1099,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update )



int __fastcall BASIC::SumCasherRptXDT(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1095,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update )




//計算2017/03/08 CPN
int __fastcall BASIC::SumBillCPN(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt;
    AnsiString str_sale_type, stmp, s, sPAY_TYPE;       //結帳型態

    iPayType = iPayID = iTAmt = iTCnt = 0;
    giU=giV=0;

    stmp.sprintf("SumBillCPN:CPN合計金額計算 (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);
    writelog(stmp);

    if (gbl_bil_haveline)
    {

        for (int i=0; i<gtsl_bil->Count; i++)
        {
            s=gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            sPAY_TYPE = _StringSegment_EX(s, "|", 10); //支付代號
            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"支付類別");
            stmp=_StringSegment_EX(s, "|", 12);  //支付類別序號
            iPayID=_StrToInt(stmp,"支付類別序號");
            iTAmt = _StrToInt( _StringSegment_EX(s, "|", 13), "支付金額" );

            if( sPAY_TYPE.SubString(0,1)=="C" )
            {
                stmp.sprintf("SumBillCPN:1040->%s",gtsl_bil->Strings[i] );
                writelog(stmp);
                if (str_sale_type.Pos("Z0"))
                    {
                     if( iPayID == 109)     //非實體券金額
                        giU +=iTAmt;
                     else
                        giV +=iTAmt;
                    }
                else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
                    {
                      if( iPayID == 9)      //非實體券金額
                        giU -=iTAmt;
                     else
                        giV -=iTAmt;
                    }
            }   // end of if( sPAY_TYPE.SubString(0,1)=="C" )

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)

    stmp.sprintf("SumBillCPN:1040->CPN合計金額計算 (非實體券金額:%d), 實體券金額:%d)",giU, giV);
    writelog(stmp);
    return giS;
}



//預售類(折價券/MM)折扣
int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int StartLine, int TotalLine);














XZDATA.DLL Ver: 3.17.105.1

收銀員交接班明細表 放大 8



XZDATA.DLL Ver: 3.16.1130.1

0024992: TFM問題/需求/規格書變更通知(105111801(TM_合流樣張調整))
收銀員交接班明細表




XZDATA.DLL Ver: 3.16.1001.1

 //2016/11/03    Ver=2016100100  Sal 版號更新


XZDATA.DLL Ver: 3.16.0318.1
BASIC::SumCasherRptXDT
BASIC::SumCasherRpt
中獎發票金額(1000)  收銀員交接班明細表
中獎發票金額(200)   收銀員交接班明細表


XZDATA.DLL Ver: 3.16.0225.1
銀聯卡&Smartpay  電文版本號變更


XZDATA.DLL Ver: 3.15.1223.2
遠鑫

XZDATA.DLL Ver: 3.15.1123.2

////////////////////食安退款筆數2015/12/02///////////////////////////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","Foodcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt13,     //食安退款金額
   //iacc_cnt13,     //食安退款筆數
   sLog.sprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> Foodcnt = %s ,食安退款筆數 :%4ld, 食安退款金額 :%8ld,", strtmp,
                                                      iacc_cnt13+i1, iacc_amt13+i2);
   writelog(sLog);
   strtmp.sprintf("食安退款金額:%8ld  ", iacc_amt13+i2 );   tslCrp->Add(strtmp);
   //strtmp.sprintf("食安退款總數:%8ld  ", iacc_cnt13+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("食退|%04d|%8ld|",iacc_cnt13+i1, iacc_amt13+i2);
      else
       strtmp.sprintf("食退|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","Foodcnt",strtmp);
     }




XZDATA.DLL Ver: 3.15.1022.x

//收銀員交班明細表
int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,..
{

Sum040RecycleDataCnt(0, gi_bil_line);      //廢資源回收數 /提貨券金額/ 提貨券張數


/////////////////////////// 2015/10/22 //////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","BillAmt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt14,     //提貨券金額
   //iacc_cnt14,     //提貨券張數

   sLog.sprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> BillAmt = %s ,提貨券張數 :%4ld, 提貨券金額 :%8ld,", strtmp,
                                                      iacc_cnt14+i1, iacc_amt14+i2);
   writelog(sLog);
   strtmp.sprintf("提貨券金額  :%8ld  ", iacc_amt14+i2 );   tslCrp->Add(strtmp);
   strtmp.sprintf("提貨券張數  :%8ld  ", iacc_cnt14+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("提貨|%04d|%8ld|", iacc_cnt14+i1,iacc_amt14+i2);
      else
       strtmp.sprintf("提貨|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","BillAmt",strtmp);
     }
}

 /////////////////////////////////////////////////////////////////////////////////////////////////


XZDATA.DLL Ver: 3.15.819.2

1. //Lu update 20150819 for 2015070100 - 1050 追加代售手續費免稅欄位
    if( Trim(SaleDataVer)=="")
       SaleDataVer="2015070100";  //Default     last:"2014050100"

2. int __fastcall BASIC::SumKamt(int StartLine, int TotalLine)
    //點卡代售手續費應稅:giD    Lu Update 2015/0819 
    //點卡代售手續費免稅:giG    Lu Update 2015/0819
    //點卡代售手續費應稅作廢 giE   Lu Update 2015/0819
    //點卡代售手續費免稅作廢 giH  Lu Update 2015/0819

3.int __fastcall REPORT::ZDTReport(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int rpt_type )

    // 2015/08/19 Update Lu
    StrLine.sprintf("代售,手續費來客數%13s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(免)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(應)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(免)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(應)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 176) ).c_str() );
    to_ptr(StrLine);

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

/ 讀取 1096 data
int __fastcall REPORT::XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no,
                             int iqt5_15217totrev, int iam5_15217totrev,
                             int iqt5_18219totrev, int iam5_18219totrev )

    // 2015/08/19 Update Lu
    StrLine.sprintf("代售,手續費來客數%13s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(免)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費銷售金額(應)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(免)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("代售,手續費作廢金額(應)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 176) ).c_str() );
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





int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo, String XDTsysDttm, String StrNxnoSeq,
                          String Version, String Z_date, String DDZ_1099, String ZNT_1099, String sIdx,
                          String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile )

    SumKamt(0,gi_tot_line);                          //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;         //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;         //代售,手續費(免)giG, 代售,手續費作廢(免)giH

    iC0TotAmt=giD; iC0GivUpAmt=giE;    //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;


    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數     2015/08/19 代售,手續費銷售金額(免)
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //備用八金額  現金 加值金額     2015/08/19 代售,手續費作廢金額(免)
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0
    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //現金卡次數   電子錢次數  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //現金卡金額   電子錢金額  刷卡合計 3261



//本日來客數
int __fastcall BASIC::SumQt_tcust(int StartLine,int TotalLine)
{

  iCOInvoAmt=_StrToInt(_StringSegment_EX(s, "|", 49)) + _StrToInt(_StringSegment_EX(s, "|", 50)) ;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 





XZDATA.DLL Ver:3.15.212.3
//本日結帳累計金額   //20150318 Mantis:0019359(開封店)維修通知函---150315-0078本日結帳累計金額已超過-1000萬
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 || (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000 )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));



XZDATA.DLL Ver:3.15.212.2
     //2015/03/11 Mantis:019302_交班失敗，防止自動日結XZDATA.INI被回寫
     sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
     if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }



XZDATA.DLL Ver:3.13.702.22
0014037: (蘆洲太子店)維修通知函---131012-0157...FM-012291-今天三點交班TM01 前場收入為0 SC沒有收到 收入資料 有發票資料

if( i_type != 1095 && i_type != 1096 )
                         {
                          //if (i_now_trans == i_compare_trans  )   2013/10/14 Update
                          // (蘆洲太子店)維修通知函---131012-0157...FM-012291-今天三點交班TM01 前場收入為0 SC沒有收到 收入資料 有發票資料
                          if (i_now_trans >= i_compare_trans  &&  i_now_trans <= i_compare_trans+10 )
                             {
                               stmp.sprintf("取得本次收銀員帳開始交易序號 (%05d), 大於此序號將列入收銀員帳..", i_now_trans);
                               writelog(stmp);
                               bl_x_start = true;
                               bl_x_into = false;

                               //if (i_now_trans > i_compare_trans )   //2013/10/14 Update
                               //    tsl_x_data->Add(str_sal_data);

                               // if (i_now_trans == i_compare_trans) //2004/03/22
                               //     continue;
                             }
                         }     






XZDATA.DLL Ver:3.12.710.1
1. 
   0008079: (高雄光興店) 維修通知函---120906-0394...FM-009859-店鋪詢問SC常模分析資料異常
   投庫後若接交班，交班原始交易序號會填投庫交易。交易應可不調原始交易序號。
    1. 交班電文不會被用於分析,其原始交易序號無作用
    2. 交班原始交易序號=交易序號

2.發票兌獎金額
  int __fastcall BASIC::VoicePayAmt(int StartLine, int TotalLine)


3. 投庫 Max 60 次






XZDATA.DLL Ver:3.12.705.10
0006881: 台灣通智慧卡_加值取消有帳差，Z帳表「加值機加值」未扣除反增加
else  if(str_type=="A102" || str_type=="70" || str_type=="7C")     //加值取消   台灣通  加值取消:70   加點:7c
        {
           --int_11_time;
           int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
        }



XZDATA.DLL Ver:3.12.705.8  930 KB (952,832 位元組)  2012年8月29日, 下午 09:01:58

#7787: E通卡加值_原交易加值，部退後再加值，帳表上僅記著部退加值金額導致帳差


////////////


XZDATA.DLL Ver:3.12.705.7  930 KB (952,832 位元組)  2012年8月10日, 下午 04:31:47

#6413  訓練教室(嘉義會議室)維修通知函---120719-0209...FM-019940- 店號:019871TM2做Z帳資料沒有進去SC，查詢退貨部分沒有交易資料 查詢發票也無交易資料 
#6283   (新竹東品店)維修通知函---120723-0386...FM-004362-7/22 TM1交接班的銷售收入 禮卷 代收金額都是0(營收日7/23)
#6881  台灣通智慧卡_加值取消有帳差，Z帳表「加值機加值」未扣除反增加



XZDATA.DLL Ver:3.12.701.5  , 2012年8月3日, 下午 04:18:10   926 KB (948,736 位元組)  

//交易取消
int __fastcall BASIC::SumVoid(int StartLine, int TotalLine)
{
    AnsiString str_type,s;    //銷售型態

    //次數、金額
    giA = giB = 0;

    if (gbl_tot_haveline)
    {
        for (int i=StartLine; i<=TotalLine; i++)
        {
            s=gtsl_tot->Strings[i];
            str_type=_StringSegment_EX(s, "|", 8);

            if (str_type.Pos("V"))      //V為交易取消
            {
                ++giA;
                giB += _StrToInt(_StringSegment_EX(s, "|", 32));
            }
        }
    }

    if(  giB > 9999999 )
       giB=9999999;

    s.sprintf("SumVoid:交易取消,合計(次數=%d, 金額=%d) ",giA, giB);
    writelog(s);

    return 0;
}





XZDATA.DLL Ver:3.12.401.28   2012年6月25日, 下午 12:11:41   926 KB (948,224 位元組)

1. if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;


XZDATA.DLL Ver:3.12.401.27   2012年6月11日, 下午 02:57:14   926 KB (948,224 位元組)
1. 
//即時購合計   即時購與銷轉進值
int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine)
if( str_sale_type=="B0" || str_sale_type=="B1" || str_sale_type=="I0" || str_sale_type=="I1")   // 2012/04/17
               continue;



XZDATA.DLL Ver:3.12.401.25   2012年6月4日, 下午 11:08:28   925 KB (947,712 位元組)

void __fastcall BASIC::init_data()
  {

        //收銀員帳交易序號比對   2012/06/04

        if( str_x_trans.Length() > 5)
           stmp=str_x_trans.SubString(str_x_trans.Length()-4,5);
        else
           stmp=str_x_trans;

        sprintf(gchar_x_trans, "%5.5s", stmp.c_str());
        //sprintf(gchar_x_trans, "%5s", str_x_trans.SubString(4,5).c_str());

}


int __fastcall XDATA::WriteData
    AnsiString sTtmpPTRAN_CNT;  //2012/06/04

    sTtmpPTRAN_CNT=str_trans1_5.SubString(3,3) + str_ptran_cnt;
    tif_tmp->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt)); 

       sTmp.printf("交班完成:寫入本次交班交易序號 tif_tmp->X->PTRAN_CNT=%s",sTtmpPTRAN_CNT.c_str() );
       writelog(sTmp );
    
//////////////////


XZDATA.DLL Ver:V3.12.401.23  2012年5月23日, 下午 03:20:58
1. MTS: 4127
    
   //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SALVER,                                //"2012022100",
                   "00",     //退貨區分
                   str_ecr_no.c_str(),                    //原始交易機號
                   (str_trans1_5 + str_sum_tran).c_str(), //原始交易序號
                   "0000000000",                          //退貨交易序號
                   SalesNo.c_str(),                       //收銀員代號
                   "00",                                  //客層代碼
                   " ",                                   //會員編號
                   " ",                                   //卡號
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );



XZDATA.DLL Ver:V3.12.401.17
1. 預購折價券部門帳修正
   //部門帳  2005/10/01
   void __fastcall BASIC::SumAllDept(DEPT *dept)


XZDATA.DLL Ver:V3.12.401.17
//即時購合計   即時購與銷轉進值
int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine)

   if( str_sale_type=="B0" || str_sale_type=="B1" )
               continue;



XZDATA.DLL Ver:V3.12.401.15  
1. 
    sRtnSaleDataPath="C:\\FTLPOS\\MMK\\Telegram\\";
    MtnDirFile(sRtnSaleDataPath, 30);



