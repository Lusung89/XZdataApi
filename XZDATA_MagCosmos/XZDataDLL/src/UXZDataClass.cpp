//---------------------------------------------------------------------------

#include <vcl.h>
#include "UXZDataClass.h"

//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)


//REPORT *report;     //報表Class

int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo, String XDTsysDttm, String StrNxnoSeq,
                          String Version, String Z_date, String DDZ_1099, String ZNT_1099, String sIdx,
                          String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile )
{

    AnsiString sTmp, sSalesLoginDttm, str_x_first, ed_date_time, str_nz_cnt, str_YYYYMMDDZ, str_ver_num, SaleDataVer,
               str_date_z,str_sal_last_1099, str_tran_cnt, str_sum_tran, AutoXFg ;

    x->no_tm = gchar_ecr_no;                      //收銀機機號
    x->no_casher = gchar_cashier;                 //收銀員號碼
    str_nz_cnt=StrZCnt;
    str_ver_num=Version;
    str_date_z=Z_date;                            //DDZ

    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本
    //Charles update 20160225 for 2016030100 - 1028 追加銀聯卡&Smart Pay欄位

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
       SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    sTmp.printf("★ XDATA::GetXDTData 計算班表 GetXDTData:StrZCnt=%s, StoreNO=%s, EcrNO=%s, SalesNo=%s, StrNxnoSeq=%s, Version=%s, Z_date=%s, DDZ_1099=%s, ZNT_1099=%s, RtnXRPFile=%s, RtnXRPFile=%s ",
                StrZCnt.c_str(), StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), StrNxnoSeq.c_str(),
                Version.c_str(), Z_date.c_str(), DDZ_1099.c_str(), ZNT_1099.c_str(), RtnXRPFile.c_str(), RtnCRPFile );

    writelog(sTmp);

    if (x->no_casher.Length() < 5)
        x->no_casher = "00001";

     sSalesLoginDttm=tif_ecr->ReadString("X","SALESLOGINDTTM","");

    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須減1
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入交易序號");
    str_tran_cnt.sprintf("%05d", int_sum_transcnt);

    if ((int_sum_transcnt + 1) > 99999)    //此次收銀員帳交易序號
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );


    //X帳序號  01~99
    x->no_seq = StrNxnoSeq;
    str_x_first = gpchar_x_first;    //X帳第一筆Sal(不代表為SAL第一筆)

    //x->dt_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(1,8); //前次結帳日期
    //x->tm_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(9,6); //前次結帳時間

    //sIdx::  01:第一筆, 10:最後一筆,  11:只有一筆

    ed_date_time = FormatDateTime("yyyymmddhhnnss",Now());
    x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);
    x->tm_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(9,6);

    AutoXFg="1";   //0:當作是自動日結之交班

    if( Trim(sIdx)=="01" )         //01:第一筆
       {
         if( sSalesLoginDttm.Trim()=="")
           sSalesLoginDttm=_StringSegment_EX(ZNT_1099, "|", 5);
         x->dt_begin=sSalesLoginDttm.SubString(1,8);       //前次結帳日期
         x->tm_begin=sSalesLoginDttm.SubString(9,6);       //前次結帳時間
         x->dt_end = x->dt_begin;
         x->tm_end = "235959";
         AutoXFg="0";  //0:當作是自動日結之交班

       }
    else if( Trim(sIdx)=="10" )  //10:最後一筆
       {

              // 2018/08/20 make By Lu
              // x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);
              // x->tm_begin = "000001";
               x->dt_begin=sSalesLoginDttm.SubString(1,8);
               x->tm_begin=sSalesLoginDttm.SubString(9,6);       

               x->dt_end =  _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);  // x->dt_begin;  2018/08/20 make By Lu
               x->tm_end = ed_date_time.SubString(9,6);
               AutoXFg="1";  //0:當作是自動日結之交班
               //sTmp.printf("★ XDATA::計算班表 20180905 最後一筆, (x->dt_end:%s) > (1099_Dttm:%s) 取 1099_Dttm 5為交班之END日期.",
               //             x->dt_end, ZNT_1099.c_str()  );
               //writelog(sTmp);
        }
    else if( Trim(sIdx)=="11" )  //11:只有一筆
       {
         if( sSalesLoginDttm.Trim()=="")
           sSalesLoginDttm=_StringSegment_EX(ZNT_1099, "|", 5);

         // 預防換日後,馬上日結  
         if( ed_date_time.SubString(1,8) >  _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8) )
           {
               sTmp.printf("★ XDATA::計算班表 只有一筆, (ed_date_time:%s) > (1099_Dttm:%s) 取 1099_Dttm 為交班之日期.",
                            ed_date_time, _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8)  );
               writelog(sTmp);

               x->dt_begin=sSalesLoginDttm.SubString(1,8);       //前次結帳日期
               x->tm_begin=sSalesLoginDttm.SubString(9,6);       //前次結帳時間
               x->dt_end = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);    // x->dt_begin;
               x->tm_end = "235959";
               AutoXFg="1";  //0:當作是自動日結之交班
           }
         else
           {
             x->dt_begin=sSalesLoginDttm.SubString(1,8);
             x->tm_begin=sSalesLoginDttm.SubString(9,6);
             x->dt_end = ed_date_time.SubString(1,8);
             x->tm_end = ed_date_time.SubString(9,6);
             AutoXFg="1"; //0:當作是自動日結之交班
           }
       }
    else
       {
         // 2018/08/20 make By Lu
         //x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);     //前次結帳日期
         //x->tm_begin = "000001";                                               //前次結帳時間
         x->dt_begin=sSalesLoginDttm.SubString(1,8);
         x->tm_begin=sSalesLoginDttm.SubString(9,6);

         x->dt_end = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);  // x->dt_begin;  2018/08/20 make By Lu
         x->tm_end = "235959";
         AutoXFg="0";  //0:當作是自動日結之交班
       }


    AnsiString str_invo_char;                    //發票英文字母
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.Length() < 2)
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

    str_YYYYMMDDZ=ed_date_time.SubString(1,8)+StrNxnoSeq.Trim();

    AnsiString x_a;
    int iC0TotCount=0;
    x_a.sprintf("1095|%04d|%-6s|%-2s|%14s|%05d|%-10s|%2s|%5s|%8s|%6s|%8s|%6s|",1779,
                                        gchar_tencode,
                                        x->no_tm.c_str(),         //收銀機機號
                                        x->dt_end+x->tm_end,  //ed_date_time.c_str(),   XDTsysDttm.c_str(),
                                        str_nz_cnt.ToInt(),
                                        str_YYYYMMDDZ.SubString(3,8),
                                        x->no_seq.c_str(),        //交班序號
                                        x->no_casher.c_str(),     //收銀員號碼
                                        x->dt_begin.c_str(),      //前次結帳日期,
                                        x->tm_begin.c_str(),      //前次結帳時間
                                        x->dt_end.c_str(),        //本次結帳日期
                                        x->tm_end.c_str()         //本次結帳日期
                                     );

    //本日來客數 20140101
    x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //本日來客數
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項 20140101
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    x->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line) - giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;
    
     // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt, R4_C01TaxAmt,R4_C01FTaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01TaxAmt=R4_C01FTaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  2014/0315
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    TotCount(0,gi_tot_line);
    x->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );        //發票日結帳額(免)
    x->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));           //發票日結帳額(稅)
    x->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ) );        //發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    TotMiss(0,gi_tot_line);
    x->tb_trans.qt_miss.sprintf("%08d",    giA);        //誤打發票次數
    x->tb_trans.am_missntx.sprintf("%08d", giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX );        //誤打發票金額(免)
    x->tb_trans.am_misstax.sprintf("%08d", giC-RC01TaxAmt-R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //誤打發票金額(稅)
    x->tb_trans.am_miss.sprintf("%08d",    giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        //誤打發票金額
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

    x->tb_trans.am_totsale.sprintf("%08d", int_totsale);

    AnsiString x_b;

    x_b  = (x->tb_trans.qt_tcust+"|");                      //本日來客數
    x_b += (x->tb_trans.qt_trans+"|");                      //本日交易品項
    x_b += (x->no_rcstart+"|");                              //發票開始號碼
    x_b += (x->no_rcend+"|");                                //發票結束號碼
    x_b += (x->tb_trans.am_trcpntx+"|");                    //發票日結帳額(免)
    x_b += (x->tb_trans.am_trcptax+"|");                    //發票日結帳額(稅)
    x_b += (x->tb_trans.am_trcptot+"|");                    //發票日結帳金額
    x_b += (x->tb_trans.qt_miss+"|");                       //誤打發票次數
    x_b += (x->tb_trans.am_missntx+"|");                    //誤打發票金額(免)
    x_b += (x->tb_trans.am_misstax+"|");                    //誤打發票金額(稅)
    x_b += (x->tb_trans.am_miss+"|");                       //誤打發票金額
    x_b += (x->tb_trans.am_totsale+"|");                    //本日實際銷貨收入

    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC ;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    //代售付TABLE===============================================================

    AnsiString x_c;
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

    x->tb_newadd.qt_cetel.sprintf("%08d", giD);          //代收次數
    x->tb_newadd.am_cetel.sprintf("%08d", giC);          //代收金額


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

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //代售合計次數
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;   //2014/03/15
    //代售合計金額   無須加入預售商品
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);   //2014/03/15

    //代付合計次數
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //代付合計金額
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;

     x_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );




   // writelog( ("X:代售付ｔａｂｌｅ 長度應為 720 目前Data: " + z_c ) );
    /*******************************************************************
    for (int j=0; j<5; j++)
    {
        for (int k=0; k<9; k++)
        {
            if (k<8)    //每個TABLE有八個ITEM
            {
                if (j == 0)     //第一TABLE存代售
                {
                    if (k == 0)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giE);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giA);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else if (j == 3)    //第四TABLE存代售
                {
                    if (!k)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giF);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giB);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else
                {
                    x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                }
            }
            else
            {
                if (j == 0)     //代售總合
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giE);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giA);
                }
                else if (j == 3)    //代付總合
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giF);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giB);
                }
                else
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",0);
                }
            }
        }//for k
    }//for j
    *******************************************************************/

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

    x->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);       //禮券回數
    x->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //禮券金額
    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

    x->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //廠商折價券回數 目前不使用
    x->tb_cash.am_vndcpn.sprintf("%08d", 0);             //廠商折價券金額

    x->tb_cash.qt_stocpn.sprintf("%08d", giC);           //折價券回數  門市代現金回數
    x->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //折價券金額  門市代現金金額
    SumCpn41 +=  giD;                                    //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;


    //x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI - giT));      //代現金合計回數
    //x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ - iCSMAmt));  //代現金合計金額

    x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //代現金合計回數
    x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    x->tb_cash.qt_bonout.sprintf("%08d", giG);           //溢收合計回數
    x->tb_cash.am_bonout.sprintf("%08d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //giE:餘額券金額 / giF:餘額券次數
    iqt_ffu1=giI;                                        //現金卡回數  電子錢次數  3275
    iam_ffu1=giJ;                                        //現金卡金額  電子錢金額  刷卡合計 3261

    AnsiString x_d;

    x_d += (x->tb_cash.qt_bonus+"|");                //禮券回數
    x_d += (x->tb_cash.am_bonus+"|");                //禮券金額
    x_d += (x->tb_cash.qt_vndcpn+"|");               //廠商折價券回數
    x_d += (x->tb_cash.am_vndcpn+"|");               //廠商折價券金額
    x_d += (x->tb_cash.qt_stocpn+"|");               //折價券回數
    x_d += (x->tb_cash.am_stocpn+"|");               //折價券金額
    x_d += (x->tb_cash.qt_totcash+"|");              //代現金合計回數
    x_d += (x->tb_cash.am_totcash+"|");              //代現金合計金額

    sTmp.sprintf("%08d",giF);
    x_d += (sTmp+"|");                         //餘額合計回數
    sTmp.sprintf("%08d",giE);
    x_d += (sTmp+"|");                         //餘額合計金額

    x_d += (x->tb_cash.qt_bonout+"|");               //溢收合計回數
    x_d += (x->tb_cash.am_bonout+"|");               //溢收合計金額

    //==========================================================================

    //現金合計 含 消費券-餘額券
    SumCash41=SumCash(0,gi_tot_line);
    x->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    AnsiString x_e;

    x_e += (x->am_cash+"|");               //現金合計
    //x_e += (x->no_rcstart+"|");            //發票開始號碼
    //x_e += (x->no_rcend+"|");              //發票結束號碼


    //收銀員明細FIELD===========================================================

    AnsiString x_f;

    //借零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tb_vendfld[i].am_vend.sprintf("%08d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

        x->tb_casher.tb_vendfld[i].tm_vend.sprintf("%06d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

    x->tb_casher.qt_vendtot.sprintf("%08d",0);
    x->tb_casher.am_vendtot.sprintf("%08d",0);


    //x_f += (x->tb_casher.qt_vendtot+"|");      //借零次數
    sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");      //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)

    //x_f += (x->tb_casher.am_vendtot+"|");      //借零合計金額
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);                     //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    x_f += (sTmp+"|");                           // 代售商品折讓金額  借零合計金額

    AnsiString x_g;

    //投零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_feedfld[i].am_feed.sprintf("%08d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

        x->tb_casher.tg_feedfld[i].tm_feed.sprintf("%06d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

    x->tb_casher.qt_feedtot.sprintf("%08d",0);
    x->tb_casher.am_feedtot.sprintf("%08d",0);

    x_g += (x->tb_casher.qt_feedtot+"|");      //投零次數
    x_g += (x->tb_casher.am_feedtot+"|");      //投零合計金額


    AnsiString x_h;

    //投庫
    int int_drop_line = (gtsl_drop->Count - 1);   //投庫次數

    sTmp.printf("產生收銀員交班明細表:投庫筆數(%d)",gtsl_drop->Count);
    writelog(sTmp );

    //初始化全部填零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_dropfld[i].am_drop.sprintf("%08d",0);    //投庫金額
        x->tb_casher.tg_dropfld[i].tm_drop.sprintf("%-6s"," ");    //執行時間
    }

    int int_drops = 0;                            //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                         //投庫總額
    int x_rtn_cnt=0;

    AnsiString str_drop_time, s;

    for (int i=0; i<=int_drop_line; i++)
    {
        s=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生收銀員交班明細表:非投庫(%s)資料",s);
             writelog(sTmp );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, "|", 11) );     //現金
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 12) );     //IC付款
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 13) );     //信用卡
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 14) );    //禮卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 15) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 16) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 17) );    //提貨單

        //中獎發票金額  800   Cloud 中獎發票金額(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 18) );

        //中獎發票金額  500
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 19) );
        //中獎發票金額  1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 20) );
        //中獎發票金額  200
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 22) );

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 23) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 25) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 27) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 29) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 31) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 33) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 35) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 37) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 39) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 41) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 43) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 45) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 47) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 49) );

        //每次投庫金額
        x->tb_casher.tg_dropfld[int_drops].am_drop.sprintf("%08d", int_drop_money);

        int_drop_sum += int_drop_money;                         //投庫總額

        str_drop_time = _StringSegment_EX(s, "|", 5).SubString(9,6);  //投庫時間

        //每次投庫時間
        x->tb_casher.tg_dropfld[int_drops].tm_drop.sprintf("%-6s", str_drop_time);

        int_drop_money = 0;
        ++int_drops;                             //投庫次數
        //sTmp.printf("產生收銀員交班明細表:投庫:int_drop_money=%d,int_drops=%d",
        //           int_drop_money,int_drops);
        //writelog(sTmp );

    }

    //for (int i=0; i<50; i++)
    //{
   //     x_h += (x->tb_casher.tg_dropfld[i].am_drop+"h");  //投庫總次總合
   //     x_h += (x->tb_casher.tg_dropfld[i].tm_drop+"h");  //投庫金額總合
    //}

    AnsiString x_i;

    x_i += (x->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        //投庫次數
    x_i += (x->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     //投庫合計金額

    SumUpdat(0,gi_rec_line);
    x_i += (x->tb_casher.qt_update.sprintf("%08d", giA)+"|");               //更正次數
    x_i += (x->tb_casher.am_update.sprintf("%08d", giB)+"|");               //更正合計金額

    SumVoid(0,gi_tot_line);
    x_i += (x->tb_casher.qt_void.sprintf("%08d", giA)+"|");                 //交易取消次數
    x_i += (x->tb_casher.am_void.sprintf("%08d", giB)+"|");                 //交易取消合計金額

    SumInpdp(0,gi_rec_line);                                       //部門登錄
    x_i += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    x_i += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //開錢櫃次數

    //小計折扣  1011
    SumDisc(0,gi_pay_line);
    x_i += (x->tb_casher.qt_dise.sprintf("%08d", giA)+"|");                 //折扣次數
    //x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");                 //折扣金額 不含折讓單 : iPreSalINVODisAmt
    x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP)+"|");                 //折扣金額,改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    x_i += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //促銷折扣/讓次數
    x_i += (x->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");         //促銷折扣/讓金額


    easy_card(0,gi_easy_line);
    x_i += (x->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");                  //加值機開售卡次數
    x_i += (x->tb_casher.am_disret1.sprintf("%08d", giB)+"|");                  //加值機開售卡金額
    x_i += (x->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");                  //加值機加值次數
    x_i += (x->tb_casher.am_disret2.sprintf("%08d", giD)+"|");                  //加值機加值金額


    //x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //開錢櫃次數
    //x_i += (x->tb_casher.qt_pack.sprintf("%08d" , iTotOt2Rtn)+"|"); //giE);     //加值機結帳次數   2007/12/01 找餘額券
    //x_i += (x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"I");   //加值機結帳金額, 不開發票金額誤退重

    //追加TABLE=================================================================


    AnsiString x_j;

    //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數     2015/08/19 代售,手續費銷售金額(免)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //備用八金額  現金 加值金額     2015/08/19 代售,手續費作廢金額(免)
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數     2015/08/19 代售,手續費銷售金額(免)   //2017/06/04 Update
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //備用八金額  現金 加值金額     2015/08/19 代售,手續費作廢金額(免) //2017/06/04 Update

    //食安次數、金額  int iqt_food,iamt_food;
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0

    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //現金卡次數   電子錢次數  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //現金卡金額   電子錢金額  刷卡合計 3261

    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //備用十次數  FETC 結帳次數  保留
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"|"); //giFetcSumAmt);     //備用十金額  FETC 結帳金額  保留

    //發票金額0次數

    x_j += (x->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");         //代收停車費次數
    x_j += (x->tb_newadd.am_parking.sprintf("%08d", 0)+"|");         //代收停車費金額
    x_j += (x->tb_newadd.qt_cetel+"|");                              //代收次數FUNCTIO在前面
    x_j += (x->tb_newadd.am_cetel+"|");                              //代收金額
    //x_j += (x->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix)+"|");       //代收商品客數
    //x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");     //現金卡次數   電子錢次數  3275
    //x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");     //現金卡金額   電子錢金額  刷卡合計 3261
    //x_j += (x->tb_newadd.qt_ffu2.sprintf("%08d", 0)+"|");            //扣卡機扣卡次數                    3269
    //x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //扣卡機扣卡金額  2008/12/04 消費券  3277
    x_j += (x->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //版本前置符號  3285
    x_j += (x->tb_newadd.tx_ver.sprintf("%6s" , str_ver_num)+"|");   //版本號碼
    x_j += (x->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");    //發票金額0次數

    //不開發票金額
    x_j += (x->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //備用四金額,  不開發票金額
    x_j +=(x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"|");   //不開發票誤退重合計

    SumLOSS(0,gi_rec_line);
    x_j += (x->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");         //TM廢棄Loss次數
    x_j += (x->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");         //TM廢棄Loss金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計  2013/10/17  取消
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //原加值機加值取消次數
    x_j += (x->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");   //原加值機加值取消金額


    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數  即時購代售點卡
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //原加值機加值取消次數  即時購代售點卡

    x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //消費券


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

    // 2012/09/25  Update
    if( SaleDataVer >= "2012090100")
      {
          for (int i=0; i<60; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //投庫金額
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //投庫時間
            }

      }
    else
      {
        for (int i=0; i<30; i++)
            {
              x_h += ("00000000|");  //借零金額
              x_h += ("      |");    //借零時間
            }         //"000000|"

        for (int i=0; i<30; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //投庫金額
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //投庫時間
            }
      }

    
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);  即時購與銷轉進值應稅
    x_h +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //即時購與銷轉進值免稅
    x_h +=sTmp;


    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數  中獎發票張數
    x_h +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額  中獎發票金額
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for 收銀員交班明細表
    //SumCash41+=giA;                // 2012/10/08 for 收銀員交班明細表

     //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    x_h +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD, 作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt, iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    x_h +=sTmp;

    AnsiString str_xdt_path, str_xrp_path;   //X資料、帳表存放路徑


    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);


    //AnsiString str_x_path;
    //DDZ_1099=str_date_z;

    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;


    //AutoXFg: 0:當作是自動日結之交班
    //X報表
    AnsiString xdt_1095_data;

    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%79s|\n",  2012/09/18
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%43s|\n",
    xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%8s%s%7s|\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),
                          x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          " ",
                          AutoXFg.c_str(),
                          "FFFFFFF"
                          );                      
                          
       

    sTmp.printf("XDATA::GetXDTData 產生收銀員交班表 xdt_1095_data:(%s) Len=%d",xdt_1095_data, xdt_1095_data.Length() );
    writelog(sTmp);

    RtnXDTData=xdt_1095_data;

    //收銀員帳表  *.XRP    String &RtnXRPData, String &RtnCRPData
    strcpy(report->char_filename, RtnXRPFile.c_str()); //  ChangeFileExt(str_x_filename,".xrp").c_str());
    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  產生收銀員交班表  *.XRP                ***************/
    sTmp.printf("產生收銀員交班表:(%s)",report->char_filename );
    writelog(sTmp);

    DeleteFile(RtnXRPFile);
    report->CreateReport('x', xdt_1095_data, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 );

    //str_new_name = str_x_path;
    //str_new_name +=   ExtractFileName(report->char_filename) ;

    //DeleteFile(str_new_name);

    //if (RenameFile(report->char_filename,str_new_name))       //搬檔
    //    writelog("產生收銀員帳表(XRP)檔案 " + str_new_name + " ->gchar_date_time="+gchar_date_time);
    //else
    //    writelog("產生收銀員帳表(XRP)檔案失敗 " + str_new_name);


    //str_xrp_path = str_new_name;        //寫入INI變數

    //寫入SAL檔1097Type
    AnsiString str_rec_type,
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

    str_tencode = gchar_tencode;   //店號

    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"機號"));              //機號
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
    str_z_cnt.sprintf("%02d", _StrToInt(str_nxno_seq,"交班序號"));

    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

    str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
    str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF";


    AnsiString str_trans1_5, CasherRptFile, Head1041;    //交易序號前5碼
    //str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    CasherRptFile=RtnCRPFile;  //ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    Head1041.sprintf("1041|0500|%-6s|%2s|%14s|%05d|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        XDTsysDttm.c_str(),                   //x->dt_begin+x->tm_begin,
                        str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );


     
    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   XDTsysDttm.c_str(),   //str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
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
                   " ",
                   " ",
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );




    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);


    str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   XDTsysDttm.c_str(),   //str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
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
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );


    AnsiString _str_tran_cnt;    //交易序號
    _str_tran_cnt = (str_trans1_5 + str_sum_tran);


    //寫入1097交易序號
    AnsiString str_97="";

    str_97.sprintf("%4s|%4s|%-6s|%2s|%14s|%05d|%-10s|%8s|%10s|000%8s|%10s|%8s|\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        XDTsysDttm.c_str(),      //str_sys_date.c_str(),
                        str_nz_cnt.ToInt(),      //str_z_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );


    /***************  產生收銀員交班明細表  收銀員交接班明細表 ***************/

    AnsiString SumCasherRptData;
    DeleteFile(RtnCRPFile);
    SumCasherRptXDT(NAUTOZ, 0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt,Head1041, StrNxnoSeq, RtnCRPFile,
                    str_01, str_97, xdt_1095_data, SumCasherRptData, iamt_food, iqt_food, 1);
    sTmp.printf("產生收銀員交班明細表:%s",CasherRptFile);
    writelog(sTmp );

    RtnXDTData+=SumCasherRptData;

    sTmp.printf("XDATA::GetXDTData End:StrZCnt=%s, StrNxnoSeq=%s, Z_date=%s, DDZ_1099=%s, ZNT_1099=%s, RtnXRPFile=%s, RtnXRPFile=%s  ",
                StrZCnt.c_str(), StrNxnoSeq.c_str(),  Z_date.c_str(), DDZ_1099.c_str(), ZNT_1099.c_str(), RtnXRPFile.c_str(), RtnCRPFile );

    writelog(sTmp );

     return 1;

}





//收銀員/交班 手動交班
int __fastcall XDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version)
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName,true, "", "");               //載入SAL檔，將檔案依型態分類


    AnsiString sLog;
    sLog.sprintf("XDATA::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    if (!int_reslut)
        return 0;

    AnsiString str_ver_num,str_nz_cnt, sTmp, sSalesLoginDttm;

    str_ver_num = get_version(Version);     //版本編號


    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號

    //SPOS_XRPDT *x = new SPOS_XRPDT;         //X帳檔案結構

    //備份載入之原始之料
    Auto_Sal->Clear();
    Auto_Sal->LoadFromFile(SalFileName);
    Auto_Sal->SaveToFile("c:\\ftlpos\\xzdata\\Bak.sal");
    //執行 loadsalfile(SalFileName,true, "", ""); ,會建立 Auto_Sal->Assign(tsl_x_data);

    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Auto_1051_sal->Assign(gtsl_1051_sal);
    Autotsl_Ticket->Assign(gtsl_Ticket);

    TStringList *Z_CntLst = new TStringList;
    TStringList *Z_CntLsttmp = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->Clear();
    ZDT_Lst->Clear();
    Z_CntLsttmp->Clear();

    if( !_GetZCntLst(Z_CntLsttmp, Auto_Sal  ) )
      {

        // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("XDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }
        delete Z_CntLst;
        delete ZDT_Lst;
        delete Z_CntLsttmp;
        return 0;    //千萬不可回 return OK;
      }
    //Z_CntLst->Assign(Z_CntLsttmp);


    AnsiString str_x_filename,              //XDATA檔名
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


    str_date_z = tif_ecr->ReadString("Z","DATEZ","");             //ddn
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );


    str_YYYYMMDDZ=tif_ecr->ReadString("Z","YYYYMMDDZ","201001010000");

    AnsiString str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;

#ifdef TM_20
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "00001");          //999999目前日結序號  換日不歸零
    str_date_time = tif_ecr->ReadString("Z","pz_date", gchar_date_time);  //2003/03/08 2004/03/22 liu
    if (str_date_time.Length() < 8)
           str_date_time = FormatDateTime("yyyymmddhhnnss",Now());        //2003/03/08 liu
#else
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());        //2003/03/08 liu
#endif

    XDTYYYYMMDD=FormatDateTime("yyyymmddhhnnss",Now());
    if(str_date_z.Length() < 3)    //DATAZ=日期+Z帳次數  ddn
       str_date_z = (str_date_time.SubString(7,2) + "1");

    sLog.sprintf("XDATA::ECR->WriteData 取的目前 XZDATA.INI 之日結序號:%s, 交易序號:%s", str_nz_cnt, str_tran_cnt);
    writelog( sLog);

    AnsiString str_sal_last, str_sal_last_1099;        //*.sal 最後一筆
    str_sal_last = gpchar_sal_last;
    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
       SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    //if (str_tran_cnt.Length() < 5)  //上次交易序號
    //2012/08/06 Mark 已INI 為依據
    //if( Trim(str_tran_cnt)=="00000" )
    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    sLog.sprintf("XDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    writelog( sLog);

    str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","01");                  //上次x帳累積次數

    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //目前x次到第幾次
    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";


    AnsiString str_pzno_seq;
    str_pzno_seq = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數1~9   換日歸1


    str_x_filename.sprintf("%s%02d%s%02d%02dx.xdt",
                                         XZDATADIR,
                                         atoi(gchar_ecr_no),
                                        (XDTYYYYMMDD.SubString(3,6)).c_str(),
                                         _StrToInt(str_pzno_seq),_StrToInt(str_nxno_seq) );

    str_xrp_filename=ChangeFileExt(str_x_filename,".xrp").c_str();
    str_crp_filename=ChangeFileExt(str_x_filename,".crp").c_str();


     AnsiString DDZ_1099, str_x_path;
    DDZ_1099=str_date_z;
    if(  _StrToInt(DDZ_1099.SubString(1,2),"DDZ_1099") < 1 || _StrToInt(DDZ_1099.SubString(1,2),"DDZ_1099") > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //收銀員帳表備份路徑
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.SubString(1,2) + "\\" + "0" + DDZ_1099.SubString(3,1) + "\\";


    //帳表檔名
    AnsiString sTmpPzData;

    if ( Trim(str_x_path)=="" )
    {
        //無指定路徑由系繰日期決定

        str_x_path = "C:\\FTLPOS\\HISTORY\\";
        sTmpPzData=tif_ecr->ReadString("Z","PZ_DATE",FormatDateTime("yyyymmddhhnnss",Now())).Trim();

        if (sTmpPzData == NULL)
        {
            str_x_path += (str_date_time.SubString(7,2) + "\\01\\");
            writelog("確認歷史目錄 01:" + str_x_path +" ->str_date_time="+str_date_time);

        }
        else
        {
            str_x_path += ( sTmpPzData.SubString(7,2) + "\\0" );

            if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 1)
               { str_x_path += "1\\";
                writelog("確認歷史目錄 02:" + str_x_path  );
               }
            else
               {
                str_x_path += (tif_ecr->ReadString("Z","NZNO_SEQ","") + "\\");
                writelog("確認歷史目錄 03:" + str_x_path);
               }
        }

        tif_ecr->WriteString("Z","XPATH",str_x_path);
    }

    if (!DirectoryExists(str_x_path))
       {
        writelog("建立歷史目錄 :" + str_x_path);
        ForceDirectories(str_x_path);
       }

   AnsiString StrXDTFileName, StrXRPFileName, StrCRPFileName;
   StrXDTFileName = (str_x_path + ExtractFileName(str_x_filename) );

   StrXRPFileName = (str_x_path + ExtractFileName(str_xrp_filename) );
   if (FileExists( StrXRPFileName))
                DeleteFile( StrXRPFileName);

   StrCRPFileName = (str_x_path + ExtractFileName(str_crp_filename) );
   if (FileExists( StrCRPFileName))
                DeleteFile( StrCRPFileName);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //str_orgam_tclsacc=str_am_tclsacc;

  AnsiString SaleXDT, sRec, TmpZcnt, str_xrp, XRPFile, CRPFile, sIdx;
  SaleXDT="";
  //XRPFile=ChangeFileExt( str_x_filename, ".Xrp");     //ExtractFileName(str_z_filename)
  //CRPFile=ChangeFileExt( str_x_filename, ".Crp");


  for( int i=0; i< Z_CntLsttmp->Count; i++)
    {
       sRec=Z_CntLsttmp->Strings[i];
       TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
       GetZCNT_SalData(TmpZcnt) ;
       if( gtsl_tot->Count > 0 || gtsl_drop->Count > 0)
         {
           Z_CntLst->Add(sRec);
           sLog.sprintf("XDATA::加入Z_CntLst:日結序號(%s)(%s)",TmpZcnt,sRec);
           writelog( sLog);
         }
    }

    if(  Z_CntLst->Count <= 0 )
    {
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("交班失敗 Z_CntLst->Count <=%d, XDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原", Z_CntLst->Count);
        writelog( sLog);
 
        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }
           
        delete Z_CntLst;
        delete ZDT_Lst;
        delete Z_CntLsttmp;
        return 0;    //千萬不可回 return OK;
    }


  str_nxno_seq=str_pxno_seq;
  Z_CntLst->Sort();
  int iIdx=0;
  for( int i=0; i< Z_CntLst->Count; i++)
       {

         //sIdx::  01:第一筆, 10:最後一筆,  11:只有一筆
         if( Z_CntLst->Count == 1 )
            sIdx="11";
         else if( i== 0 )
            sIdx="01";
         else if( i==Z_CntLst->Count-1)
            sIdx="10";
         else
            sIdx="09";

         sRec=Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);

         //XRPFile.sprintf("c:\\FtlPos\\XZDATA\\%s%s.Xrp",TmpZcnt.c_str(), str_nxno_seq.c_str() );
         //CRPFile.sprintf("c:\\FtlPos\\XZDATA\\%s%s.Crp",TmpZcnt.c_str(), str_nxno_seq.c_str() );
         XRPFile.sprintf("c:\\FtlPos\\XZDATA\\Tmp.Xrp");
         CRPFile.sprintf("c:\\FtlPos\\XZDATA\\Tmp.Crp");
         //CRPFile=ChangeFileExt( str_x_filename, ".Crp");
         GetZCNT_SalData(TmpZcnt) ;

         if( gtsl_tot->Count > 0 || gtsl_drop->Count > 0)
           {
             iIdx++;

             str_nxno_seq.sprintf("%02d", (_StrToInt(str_nxno_seq,"pxno_seq") + 1)); //目前x次到第幾次
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

            GetXDTData(TmpZcnt, String(gchar_tencode),  String(gchar_ecr_no), String(gchar_cashier), XDTYYYYMMDD, str_nxno_seq,
                    String(str_ver_num), str_date_z, DDZ_1099, sRec, sIdx, SaleXDT, XRPFile, CRPFile  );

          // int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo , String StrNxnoSeq,
          //                String Version, String Z_date, String DDZ_1099, String ZNT_1099,
          //                String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile );

            sLog.sprintf("★ XDATA::產生交班表(%s, XRPFile=%s, CRPFile=%s)",SaleXDT.c_str(), XRPFile.c_str(), CRPFile.c_str() );
            writelog( sLog);

            ZDT_Lst->Add(SaleXDT);
            str_xrp+=SaleXDT;

            SaleXDT="";

            file_cat( XRPFile.c_str(), StrXRPFileName.c_str() );
            sLog.sprintf("XDATA::file_cat(%s, %s)",XRPFile.c_str(), StrXRPFileName.c_str() );
            writelog( sLog);

            file_cat( CRPFile.c_str(), StrCRPFileName.c_str() );
            sLog.sprintf("XDATA::file_cat(%s, %s)",CRPFile.c_str(), StrCRPFileName.c_str() );
            writelog( sLog);
          }
          else
          {
            sLog.sprintf("XDATA::(%s) 不輸出交班表",sRec.c_str()  );
            writelog( sLog);
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



    AnsiString str_xdt_path, str_xrp_path;   //X資料、帳表存放路徑



    /***
    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);
    
    DDZ_1099=str_date_z;
    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //收銀員帳表備份路徑
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.SubString(1,2) + "\\" + "0" + DDZ_1099.SubString(3,1) + "\\";
    ****/


    /***
    DeleteFile(str_new_name);
    if (RenameFile(str_x_filename,str_new_name))      //搬檔報表檔
        writelog("產生收銀員帳表(XDT)檔案 " + str_new_name);
    else
        writelog("產生收銀員帳表(XDT)檔案失敗 " + str_new_name);
    ***/

    //str_xdt_path = str_new_name;                     //寫入INI變數


    //strcpy(report->char_filename, ChangeFileExt(str_x_filename,".xrp").c_str());


    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  產生收銀員交班表  *.XRP                ***************/
    //report->CreateReport('x', xdt_1095_data, StrBillPay, gchar_tencode, gchar_ecr_no,
    //                     iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 );


     // 收銀員交班明細表
    //str_date_z          //ddn
    //str_tran_cnt    //目前交易序號

   // AnsiString sTmp ;
   // sTmp.printf("CreateReport: 產生收銀員帳表檔案 代售合計=%d",iam5_15217totrev);
   // writelog(sTmp);

   // str_new_name = str_x_path;
  //  str_new_name +=   ExtractFileName(report->char_filename) ;

  //  DeleteFile(str_new_name);

   // if (RenameFile(report->char_filename,str_new_name))       //搬檔
   //     writelog("產生收銀員帳表(XRP)檔案 " + str_new_name + " ->gchar_date_time="+gchar_date_time);
   // else
   //     writelog("產生收銀員帳表(XRP)檔案失敗 " + str_new_name);


    //str_xrp_path = str_new_name;        //寫入INI變數

    //寫入SAL檔1097Type
    AnsiString str_rec_type,
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

    str_rec_type = "1097";
    str_rec_len  = "0106";

    str_tencode = gchar_tencode;   //店號

    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"機號"));              //機號
    str_sys_date = XDTYYYYMMDD; //(x->dt_end + x->tm_end);               //日期時間

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入1097交易序號"); //目前寫入97交易序號
    str_trans_cnt.sprintf("%05d", int_sum_transcnt);

    AnsiString str_sum_tran;       //此次收銀員帳交易序號
    //20031215


    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );

    //xxx
    //tif_tmp->WriteString("ECR","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_sum_tran;


    //str_pxno_seq  //上次x帳累積次數

    //str_nxno_seq  //目前x次到第幾次

    // 2007/07/17
    str_z_cnt.sprintf("%02d", _StrToInt(str_nxno_seq,"交班序號"));
    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    //str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

    str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
    str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF";


    AnsiString str_trans1_5, CasherRptFile;;    //交易序號前5碼
    //str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;
    
    /******************
    CasherRptFile=ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    sTmp.sprintf("1041|0500|%-6s|%2s|%14s|%05d|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );

    ******************/

    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|\n",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
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
                   " ",
                   " ",
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

    str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|\n",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
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
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );


    AnsiString _str_tran_cnt;    //交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);

    //寫入1097交易序號
    AnsiString str_97;
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


    String str_sal;
    str_sal = str_01 +  str_xrp + str_99;  //str_97 + str_99;

    str_xdt_path=StrXDTFileName;
    FILE *fxdata;

    if ((fxdata = fopen(str_xdt_path.c_str(), "w")) != NULL)
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


    //FILE *f_sale;
    //AnsiString str_write_sal;
    //str_write_sal = "C:\\FTLPOS\\XZDATA\\00000" + str_trans_cnt + ".sal";
    //if ((f_sale = fopen(str_write_sal.c_str(),"a")) != NULL)
    //{
    //    fprintf(f_sale,"%s",str_97.c_str());
    //    fclose(f_sale);

        //xdt_tmp.ini
    //    tif_tmp->WriteString("X","x0097filename", str_write_sal);
    //}
    //else
   // {
   //     tif_tmp->WriteString("X","x0097filename", " ");
   //     return 0;
   // }

    //writelog(sSal97);

    //xxx

    ///////////////////////////////////////////////////////////////////////////////////

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印

    if ((int_sum_transcnt + 2) > 99999)
            int_sum_transcnt = 1;
    else
         int_sum_transcnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_transcnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    AnsiString sTtmpPTRAN_CNT;  //2012/06/04

    sTtmpPTRAN_CNT=str_trans1_5.SubString(3,3) + str_ptran_cnt;
    tif_tmp->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));

    tif_tmp->WriteString("X","PXNO_SEQ",   str_nxno_seq);
    tif_tmp->WriteString("X","SX_DATE",   (x->dt_begin + x->tm_begin));
    tif_tmp->WriteString("X","EX_DATE",   (x->dt_end + x->tm_end));
    tif_tmp->WriteString("ECR","tot_open_x", "0");
    tif_tmp->WriteString("ECR","x_pay_cnt" ,  "0");

    tif_tmp->WriteString("X","XFILEDTTM", str_sys_date);    // XDT 之DTTM
    tif_tmp->WriteString("X","XFILENAME", StrXDTFileName);  //str_xdt_path);
    tif_tmp->WriteString("X","XRPTNAME" , StrXRPFileName);
    tif_tmp->WriteString("X","CHRRPTNAME" , StrCRPFileName); //CasherRptFile);
    
    //tif_tmp->WriteString("X","SALESLOGINDTTM" , sSalesLoginDttm );

    //delete x,tif_ecr;

    sTmp.printf("交班完成:寫入本次交班交易序號 tif_tmp->X->PTRAN_CNT=%s",sTtmpPTRAN_CNT.c_str() );
    writelog(sTmp );

    return OK;

}





int __fastcall ZDATA::GetZDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo ,
                          String Version, String PZ_date, String Z_date, String DDZ_1099, String ZDT_1050,
                          String &RtnZDTData, AnsiString &Tclsacc,
                          String &str_zrp_path, String &str_dpt_path)
{
    int int_reslut = 0;
    AnsiString sLog, sTmp,str_ver_num, str_tran_cnt,ZDT_1050_DDZ;

    AnsiString str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               str_date_time, str_datez,    //目前帳表系統時間
               z_a;

    sTmp=_StringSegment_EX(ZDT_1050, "|", 7);  //交易序號
    ZDT_1050_DDZ=sTmp.SubString(3,3);
    str_am_tclsacc=Tclsacc;
    str_z_date=Z_date;
    str_pz_date=PZ_date;

    str_ver_num=Version ;

    //sYYMMDDZZ.sprintf("%s%s", str_pz_date.SubString(3,6).c_str(),StrZCnt );
    sYYMMDDZZ.sprintf("%s%s", str_z_date.SubString(5,4).c_str(),StrZCnt );


    sTmp.printf("** ZDATA::GetZDTData 產生日結帳表 Version=%s, ZDT:StrZCnt=%s, PZ_date=%s, Z_date=%s, DDZ_1099=%s, Tclsacc=%s **",
                Version, StrZCnt, PZ_date, Z_date, DDZ_1099, Tclsacc  );
    writelog(sTmp );

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






    z->no_tm = gchar_ecr_no;             //收銀機機號

    //日結序號
    z->bg_noclose.sprintf("%05d", _StrToInt(StrZCnt,"日結序號"));


    AnsiString ssysdttm, s1050sysdttm;
    ssysdttm=gchar_date_time;
    s1050sysdttm=_StringSegment_EX(ZDT_1050, "|", 5);

    if( s1050sysdttm.SubString(1,8) < ssysdttm.SubString(1,8) )
      {
        z->bg_dtclose = s1050sysdttm.SubString(1,8);      //結帳日期
        z->bg_tmclose = "235959";                         //結帳時間

        sTmp.printf("ZDATA::GetZDTData: s1050sysdttm=%s 小於 ssysdttm=%s, Set 1096 日結序號=%s, 結帳日期 z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
        writelog(sTmp );
      }
    else
      {
        z->bg_dtclose = str_z_date.SubString(1,8);      //結帳日期
        z->bg_tmclose = str_z_date.SubString(9,6);      //結帳時間

        sTmp.printf("ZDATA::GetZDTData: s1050sysdttm=%s 大等於 ssysdttm=%s, Set 1096 日結序號=%s, 結帳日期 z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
        writelog(sTmp );
      }



    //z->bg_dtclose = str_z_date.SubString(1,8);      //結帳日期
    //z->bg_tmclose = str_z_date.SubString(9,6);      //結帳時間
    z->ed_dtclose = str_pz_date.SubString(1,8);     //前次結帳日期
    z->ed_tmclose = str_pz_date.SubString(9,6);     //前次結帳時間


    z_a.sprintf("1096|%04d|%-6s|%-2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|",
                                      1593,
                                      gchar_tencode,
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

    //本日來客數
    int iC0TotCount=0; //點卡手續費來客數
    z->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    z->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;


    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt, R4_C01TaxAmt,R4_C01FTaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01TaxAmt=R4_C01FTaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    TotCount(0,gi_tot_line);
    z->tb_trans.am_trcpntx.sprintf("%08d",giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );       // 發票日結帳額(免)
    z->tb_trans.am_trcptax.sprintf("%08d",giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));       // 發票日結帳額(稅)
    z->tb_trans.am_trcptot.sprintf("%08d",giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));       // 發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    sTmp.printf("ZDATA::GetZDTData: 發票結帳累計金額=%s, 發票日結帳金額=%d ", str_am_tclsacc.c_str(), int_tot_amt  );
    writelog(sTmp );

      //本日結帳累計金額   //20150318 Mantis:0019359(開封店)維修通知函---150315-0078本日結帳累計金額已超過-1000萬
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000 )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));

    Tclsacc=IntToStr(int_acc_tot_amt);

    z->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    TotMiss(0,gi_tot_line);
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
    z->tb_trans.qt_miss.sprintf("%08d",   giA);       //  誤打發票次數
    z->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX );       //  誤打發票金額(免)
    z->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  誤打發票金額(稅)
    z->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );       //  誤打發票金額
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
    z->tb_trans.am_totsale.sprintf("%08d", int_totsale );


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


    AnsiString z_b;

    z_b += (z->tb_trans.qt_tcust+"|");             //本日來客數
    z_b += (z->tb_trans.qt_trans+"|");             //本日交易品項
    z_b += (z->tb_trans.am_tclsacc+"|");           //本日結帳累計金額
    z_b += (z->no_rcstart+"|");                   //發票開始號碼
    z_b += (z->no_rcend+"|");                      //發票結束號碼
    z_b += (z->tb_trans.am_trcpntx+"|");          //發票日結帳額(免)
    z_b += (z->tb_trans.am_trcptax+"|");           //發票日結帳額(稅)
    z_b += (z->tb_trans.am_trcptot+"|");          //發票日結帳金額
    z_b += (z->tb_trans.qt_miss+"|");              //誤打發票次數
    z_b += (z->tb_trans.am_missntx+"|");           //誤打發票金額(免)
    z_b += (z->tb_trans.am_misstax+"|");           //誤打發票金額(稅)
    z_b += (z->tb_trans.am_miss+"|");              //誤打發票金額
    z_b += (z->tb_trans.am_totsale+"|");           //本日實際銷貨收入


    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC ;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

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

    z->tb_newadd.qt_agnrcv.sprintf("%08d",giD);             //代收費用次數
    z->tb_newadd.am_agnrcv.sprintf("%08d",giC);             //代收費用金額

    AnsiString z_c;
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

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
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

    z_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
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

    z->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);            //禮券回數
    z->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //禮券金額


    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

    z->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //廠商代現金回數 目前不使用
    z->tb_cash.am_vndcpn.sprintf("%08d", 0);             //廠商代現金金額

    z->tb_cash.qt_stocpn.sprintf("%08d", giC);           //門市代現金回數
    z->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //折價券金額 門市代現金金額
    SumCpn41 +=  giD;      //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

    z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //代現金合計回數
    z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //代現金合計回數
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //代現金合計金額

    z->tb_cash.qt_bonout.sprintf("%08d", giG);           //溢收合計回數
    z->tb_cash.am_bonout.sprintf("%08d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //餘額券
    iqt_ffu1=giI;                                        //現金卡回數  電子錢 1847
    iam_ffu1=giJ;                                        //現金卡金額  電子錢 1855    刷卡合計

    AnsiString z_d;

    z_d += (z->tb_cash.qt_bonus+"|");                //禮券回數
    z_d += (z->tb_cash.am_bonus+"|");                //禮券金額
    z_d += (z->tb_cash.qt_vndcpn+"|");               //廠商折價券回數
    z_d += (z->tb_cash.am_vndcpn+"|");               //廠商折價券金額
    z_d += (z->tb_cash.qt_stocpn+"|");               //折價券回數
    z_d += (z->tb_cash.am_stocpn+"|");               //折價券金額
    z_d += (z->tb_cash.qt_totcash+"|");              //代現金合計回數
    z_d += (z->tb_cash.am_totcash+"|");              //代現金合計金額

    sTmp.sprintf("%08d",giF);
    z_d += (sTmp+"|");                               //餘額合計回數
    sTmp.sprintf("%08d",giE);
    z_d += (sTmp+"|");                               //餘額合計金額

    z_d += (z->tb_cash.qt_bonout+"|");               //溢收合計回數
    z_d += (z->tb_cash.am_bonout+"|");               //溢收合計金額

    //ZDATA::GetZDTData======================================================
    //現金合計 含 消費券

    SumCash41=SumCash(0,gi_tot_line);
    z->am_cash.sprintf("%08d",SumCash41-iTotOt2Rtn+iCSMAmt );

    // writelog("現金合計 含 消費券");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //現金合計
    //strcat(ze,z->no_rcstart.c_str());      //發票開始號碼
    //strcat(ze,z->no_rcend.c_str());        //發票結束號碼
    //ze[28] = 0;


    //ZDATA::GetZDTData Z帳部門別集計TABLE      =============================================

    AnsiString z_f;

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
            z->tb_depcal[i].am_dpsitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpsale.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmiss.sprintf("%08d",0);

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


        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            z->tb_depcal[i].am_dpsale.sprintf("%08d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            z->tb_depcal[i].am_dpmitm.sprintf("%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            z->tb_depcal[i].am_dpmiss.sprintf("%08d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;
        }


    }
    __finally
    {
      delete [] dept;
    }

   //ZDATA::GetZDTData writelog("Z帳部門別集計TABLE OK");

    AnsiString z_g;

    z_g += (z->am_tdpsitm.sprintf("%08d", s1)+"|");             // 部門合計交易項數
    z_g += (z->am_dpstot.sprintf("%08d" , s2)+"|");             // 部門合計銷售金額
    z_g += (z->am_tdpmitm.sprintf("%08d", s3)+"|");             // 部門合計誤退項數
    z_g += (z->am_dpmtot.sprintf("%08d" , s4)+"|");             // 部門合計誤退金額
    z_g += (z->am_cash+"|");                                   // 現金合計含消費券 45


    //ZDATA::GetZDTData 收銀員明細ｆｉｅｌｄ=============================================

    AnsiString z_h;

    //z_h += (z->am_cash+"|");                                    //現金合計
    //z_h += (z->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");    // 借零次數
    sTmp.sprintf("%08d",VisuilCpnAmt);
    z_h += (sTmp+"|");                                           //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // 借零合計金額
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);                       //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    z_h += (sTmp+"|");                           // 代售商品折讓單金額

    z_h += (z->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");    // 投零次數
    z_h += (z->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");    // 投零合計金額

    //投庫
    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;           //投庫次數
    int int_drop_money = 0;       //投庫金額
    int int_drop_sum = 0;    //投庫總額
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        //StringSegment_EX(s, "|", 15)
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生日結帳表:非投庫(%s)資料",s1041);
             writelog(sTmp );
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC付款
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //信用卡
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //提貨單

        //ZDATA::GetZDTData中獎發票金額 500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //ZDATA::GetZDTData中獎發票金額 1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //ZDATA::GetZDTData中獎發票金額 200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //費用 13金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //費用 14金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數
    }

    z_h += (z->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        // 投庫次數
    z_h += (z->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     // 投庫合計金額

    //writelog("投庫合計金額 OK");

    //ZDATA::GetZDTData 更正次數、金額
    SumUpdat(0,gi_rec_line);
    z_h += (z->tb_casher.qt_update.sprintf("%08d", giA)+"|");          // 更正次數
    z_h += (z->tb_casher.am_update.sprintf("%08d", giB)+"|");          // 更正合計金額


    //交易取消
    SumVoid(0,gi_tot_line);
    z_h += (z->tb_casher.qt_void.sprintf("%08d", giA)+"|");            // 交易取消次數
    z_h += (z->tb_casher.am_void.sprintf("%08d", giB)+"|");            // 交易取消合計金額


     //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
    }
    __finally
    {
        //delete ini_ecr;
        ;;
    }


    z_h += (z->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // 訓練合計次數
    z_h += (z->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // 訓練合計金額

    //手動輸入部門13、14
    SumInpdp(0,gi_rec_line);
    z_h += (z->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");            //手動輸入部門次數
    z_h += (z->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");            //手動輸入部門金額

    z_h += (z->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");     // 開錢櫃次數

    //折扣讓次數   //小計折扣
    SumDisc(0,gi_pay_line);
    z_h += (z->tb_casher.qt_disc.sprintf("%08d", giA)+"|");             // 折扣次數(應稅)
    //z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");            // 折扣金額(應稅)
    z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP)+"|");            // 折扣金額(應稅),改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    z_h += (z->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");            //促銷折扣/讓次數
    z_h += (z->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");             //促銷折扣/讓金額

    //加值機
    easy_card(0,gi_easy_line);
    z_h += (z->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");          // 加值機開/售卡次數
    z_h += (z->tb_casher.am_disret1.sprintf("%08d", giB)+"|");         // 加值機開/售卡金額
    z_h += (z->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");          // 加值機加值次數
    z_h += (z->tb_casher.am_disret2.sprintf("%08d", giD)+"|");          // 加值機加值金額

    //fetc_card(0,gi_fetc_line);   //e通卡   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數    2015/08/19 代售,手續費銷售金額(免)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //備用八金額  現金 加值金額     2015/08/19 代售,手續費作廢金額(免)
    z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數    2015/08/19 代售,手續費銷售金額(免)
    z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|"); //備用八金額  現金 加值金額     2015/08/19 代售,手續費作廢金額(免)

    //食安次數、金額  int iqt_food,iamt_food;
    z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數  2015/08/19 0
    z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額  2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數  2015/08/19 0
    //z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額  2015/08/19 0


    //   05 追加 table        =============================================

    AnsiString z_i;

    z_i += (z->tb_newadd.qt_cashCard.sprintf("%08d", iqt_ffu1)+"|");    //刷卡/現金卡次數    電子錢 1847
    z_i += (z->tb_newadd.am_cashCard.sprintf("%08d", iam_ffu1)+"|");    //刷卡/現金卡金額    電子錢  刷卡合計 1855
    z_i += (z->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");            //代收停車費次數
    z_i += (z->tb_newadd.am_parking.sprintf("%08d", 0)+"|");            //代收停車費金額
    z_i += (z->tb_newadd.qt_agnrcv+"|");                                //代收費用次數  1831
    z_i += (z->tb_newadd.am_agnrcv+"|");                                //代收費用金額  1839
    z_i += (z->tb_newadd.tx_mark.sprintf("%2s", str_ver_num.SubString(1,2) )+"|");      //版本前置符號
    z_i += (z->tb_newadd.tx_ver.sprintf("%6s",  str_ver_num.SubString(3,6) )+"|");      //版本號碼
    z_i += (z->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  //發票金額0次數

    //ZDATA::GetZDTData 不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);

    z_i += (z->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //不開發票金額
    z_i += (z->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");    //  不開發票金額誤退重

    SumLOSS(0,gi_rec_line);
    z_i += (z->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");             // TM廢棄Loss值次數
    z_i += (z->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");             // TM廢棄Loss值金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);     // 健康捐合計
    z_i += (z->tb_newadd.am_ffu7.sprintf("%08d", iSub_C0InvoAmtA)+"|");             //原加值機加值取消金額

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");             //原加值機加值取消次數

    //消費券
    z_i += (z->tb_newadd.am_posZRpt.sprintf("%08d", iCSMAmt)+"|");      //扣卡機扣卡金額   消費券

    AnsiString z_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);  //+iSumRelDimAgn+iC0InvoAmt4VDCN);    即時購與銷轉進值應稅
    z_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     // 即時購與銷轉進值免稅
    z_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數
    z_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額
    z_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    z_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt, iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)

    z_k +=sTmp;

    //AnsiString DDZ_1099;
    //DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
    //   DDZ_1099=str_datez;

    //ZDATA::GetZDTData Z報表
    AnsiString str_zrp;

    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17   點卡手續費 Update
    str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str(),
                            "FFFFFFFF"
                            );

    sTmp.printf("ZDATA::GetZDTData::(%s) Len=%d",str_zrp, str_zrp.Length() );
    writelog(sTmp);


    AnsiString  Zrpfilename;
    //sTmp = ExtractFileName(SalFileName);

    Zrpfilename.sprintf("%s%s_%s.zrp",
                                        XZDATADIR,
                                        sYYMMDDZZ.c_str(),
                                        ZDT_1050_DDZ.c_str()  );


    //ZDATA::GetZDTData Z報表
    strcpy(report->char_filename, Zrpfilename.c_str()); //ChangeFileExt(str_z_filename,".zrp").c_str());
    report->CreateReport('Z', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
    //備份Z報表
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //部門帳表
    strcpy(report->char_filename, ChangeFileExt(Zrpfilename,".dpt").c_str() );
    report->CreateReport('D', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no);
    //備份部門帳表
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    RtnZDTData=str_zrp;

  return(0);
}



//日結
int __fastcall ZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version, const String AutoZDttm)
{

    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    AnsiString sLog, sTmp, TmpZcnt,sRec;
    AnsiString str_ver_num, str_tran_cnt, SaleZDT;


    sLog.sprintf("ZDATA::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    if (!int_reslut)
        return 0;

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );
    str_ver_num = Version;    //get_version(Version);   日結

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號

    //備份載入之原始之料
    Auto_Sal->Clear();
    Auto_Sal->LoadFromFile(SalFileName);
    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Autotsl_Ticket->Assign(gtsl_Ticket);
    //Auto_1051_sal->Assign(gtsl_1051_sal);       自動日結無須使用

    TStringList *Z_CntLst = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->Clear();
    ZDT_Lst->Clear();

    if( !_GetZCntLst(Z_CntLst, Auto_Sal ) )
        return 0;

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z帳結構

    //目前Z帳時間
    AnsiString str_z_date,                  //現在時間
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

    AnsiString str_date_time, str_datez;               //目前帳表系統時間
    AnsiString  Zrpfilename, str_zrp_path, str_dpt_path;


    str_date_time = gchar_date_time;
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("ZDATA::WriteData 取的目前之交易序號:%s", str_tran_cnt);
    writelog( sLog);



    str_sal_first = gpchar_sal_first;
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");       //前次時間

    if( _StringSegment_EX(str_sal_first, "|", 5) < str_pz_date )
        str_pz_date=_StringSegment_EX(str_sal_first, "|", 5);

    str_z_date  = str_date_time;                               //現在時間
    str_datez = tif_ecr->ReadString("Z","DATEZ","");           //DDZ
    Lst_Zcnt_Dttm = tif_ecr->ReadString("AUTOZ","LST_ZCNT_DTTM",str_z_date.SubString(1,8) );   //換日參考日

    //將帳表日資料備份已被將來判斷殺目錄條件 james - 2003/02/07

     sTmp=tif_ecr->ReadString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100" ));  //自手動日結要如此

    //將帳表日資料備份已被將來判斷殺目錄條件 james - 2003/02/07
    tif_ecr->WriteString("Backup", "pz_date", sTmp);

    //tif_ecr->WriteString("Backup", "pz_date", str_pz_date);
    //備份 sal8.TXT條件
    //tif_ecr->WriteString("Backup", "datez", str_datez);

    if ((str_pz_date == "") || (str_pz_date.Length() < 8))  //無相關資料，交易記錄歸零
    {
        tif_ecr->WriteString("Z", "pz_date", str_z_date);
        tif_ecr->WriteString("Z", "pzno_seq", "0");
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

        AnsiString str_xpath;               //指定收銀員帳表目錄
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date;
        Lst_Zcnt_Dttm=str_z_date.SubString(1,8);

        sLog.sprintf("ZDATA::Z->PZ_DATE < 8Byte ==> init %s ", XZDATA_INI);
        writelog( sLog);
    }

    str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數 01~09  換日歸1
    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999

    tif_ecr->WriteString("Z_OFF", "nz_cnt", str_nz_cnt  );            // 離線資料使用
    tif_ecr->WriteString("Z_OFF", "pz_cnt", tif_ecr->ReadString("Z","pz_cnt","00000")  );  // 離線資料使用

    str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.Length() < 5)
        str_nz_cnt = "00001";

    sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_pzno_seq));

    sLog.sprintf("ZDATA::Get %s=> str_pz_date=%s, str_nz_cnt=%s, str_am_tclsacc=%s", XZDATA_INI, str_pz_date, str_nz_cnt, str_am_tclsacc);
    writelog( sLog);


#ifdef TM_20

    str_z_filename.sprintf("%s%02d%s%02d%s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        (str_pz_date.SubString(3,6)).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        str_pz_date.SubString(9,4).c_str()    );
#else
    //輸出檔案 yyyymmddnn.zdt ex:2002010101.zdt
    //str_z_filename = (str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt");

    str_z_filename.sprintf("%s%02d%s%02d%s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        str_pz_date.SubString(3,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        str_pz_date.SubString(9,4).c_str()   );
#endif

    //換日Flag
    bool bl_change_day;

    //判斷換日
    //if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
    if ( str_z_date.SubString(1,8) > str_pz_date.SubString(1,8) &&
         str_z_date.SubString(1,8) > Lst_Zcnt_Dttm  )
        bl_change_day = true;
    else
        bl_change_day = false;


    DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=str_datez;

    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //歷史目錄維護用


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

     str_orgam_tclsacc=str_am_tclsacc;

     AnsiString ZrpFile, DptFile;

     ZrpFile=ChangeFileExt( str_z_filename, ".zrp");     //ExtractFileName(str_z_filename)
     DptFile=ChangeFileExt( str_z_filename, ".dpt");
     // sBackupPath=ExtractFilePath(BackupFileName);
     //   sFileNameNonPath = ExtractFileName(BackupFileName);    //檔名不包含路徑

     for( int i=0; i< Z_CntLst->Count; i++)
       {
         sRec=Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);
         GetZCNT_SalData(TmpZcnt) ;
         GetZDTData(TmpZcnt, String(gchar_tencode),  String(gchar_ecr_no), String(gchar_cashier),
                    String(str_ver_num), str_pz_date, str_z_date, DDZ_1099, sRec,
                    SaleZDT, str_am_tclsacc, str_zrp_path, str_dpt_path  );

         ZDT_Lst->Add(SaleZDT);
         str_zrp+=SaleZDT;

         SaleZDT="";

         file_cat( str_zrp_path.c_str(), ZrpFile.c_str() );
         sLog.sprintf("ZDATA::file_cat(%s, %s)",str_zrp_path.c_str(), ZrpFile.c_str() );
         writelog( sLog);

         file_cat( str_dpt_path.c_str(), DptFile.c_str() );
         sLog.sprintf("ZDATA::file_cat(%s, %s)",str_dpt_path.c_str(), DptFile.c_str() );
         writelog( sLog);

       } // end of for( int i=0; i< Z_CntLst->Count; i++)

     sTmp=ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+ExtractFileName(ZrpFile);
     CopyFile(ZrpFile.c_str(), str_zrp_path.c_str(), false);
     sLog.sprintf("ZDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
     writelog( sLog);
     DeleteFile( ZrpFile );
     sLog.sprintf("ZDATA::DeleteFile(%s)",ZrpFile.c_str() );
     writelog( sLog);

     str_dpt_path=sTmp+ExtractFileName(DptFile);
     CopyFile(DptFile.c_str(), str_dpt_path.c_str(), false);
     sLog.sprintf("ZDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
     writelog( sLog);
     DeleteFile( DptFile );
     sLog.sprintf("ZDATA::DeleteFile(%s)",DptFile.c_str() );
     writelog( sLog);

     delete Z_CntLst;
     delete ZDT_Lst;

     //寫入SAL檔98Type
     AnsiString str_rec_type,
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

    str_rec_type = "1098";
    str_rec_len = "0121";
    str_tencode = gchar_tencode;
    str_ecr_no.sprintf("%s",gchar_ecr_no);
    str_sys_date = str_z_date;      //日期時間

    //98交易序號 //20031215
    int int_sum_tran_cnt ;

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    sLog.sprintf("ZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    writelog( sLog);


    AnsiString str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
         str_sum_tran.sprintf("%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //前次日結序號，目前日結序號
    str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "前次日結序號"));
    str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "目前日結序號"));


    if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc, int_acc_tot_amt;

    int_acc_tot_amt=_StrToInt(str_am_tclsacc);
    //AnsiString str_sal_first;
    //str_sal_first = gpchar_sal_first;

     //開始交易序號
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, "|", 7).SubString(TRANCNT_NO_POS,5));
    str_start_trans.sprintf("%05d", i_tran_cnt);

    //int_am_tclsacc=int_acc_tot_amt;
    //str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //本日結帳累計金額  ????????????
    str_am_totsale = "0";   //z->tb_trans.am_totsale;         //本日實際銷貨收入  ????????????
    str_zdt_no.sprintf("%010ld", _StrToInt(str_nz_cnt,"日結序號"));

    str_ver = "FFFFFFFF|";

    AnsiString str_trans1_5;    //交易序號前五碼

    AnsiString str_sal_last;
    str_sal_last = gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    //if( Trim(str_trans1_5)=="")
    str_trans1_5="00"+DDZ_1099;

    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //歷史目錄維護用  不可刪

    AnsiString str_01,str_99;

    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
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
                   " ",
                   " ",
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );

    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
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
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );

    AnsiString _str_tran_cnt;        //交易序號
    AnsiString str_st_tran_cnt;     //開始交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    AnsiString str_98;  //寫入1098交易序號
    str_98.sprintf("%4s|%4s|%6s|%2s|%14s|%-5s|%-10s|%5s|%5s|%10s|000%8s|000%8s|%10s|%9s\n",
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



    String str_sal;
    //str_sal = str_01 + "\n"+ str_zrp + str_98 + str_99;
    str_sal = str_01 + "\n"+ str_zrp + str_99;


    if (WriteSAL(str_sal, str_z_filename) == OK)
       {
        //備份ZDT帳
        str_zdt_path = BackupFile(str_z_filename, "", DDZ_1099);
        tif_tmp->WriteString("Z","x1098filename", str_zdt_path);
       }
    else
      {
       tif_tmp->WriteString("Z","x1098filename", "");
      }


    /////////////////////////////////////////////////////////////////

    AnsiString StrZslFile;
    if (WriteSAL(str_sal, SalFileName) == OK)
       {
         //備份ZSL帳
         sTmp = ExtractFileName(SalFileName);
         StrZslFile.sprintf("%s%s_%s%s",XZDATASAL,str_sys_date.SubString(1,8),DDZ_1099,sTmp);
         if( CopyFile( SalFileName.c_str(), StrZslFile.c_str(), false)  )
           {
             sLog.sprintf("ZDATA:: 備份SAL %s TO %s",
                  SalFileName, StrZslFile );
             writelog( sLog);

           }
         else
           {
             sLog.sprintf("ZDATA:: 備份SAL %s TO %s 失敗!!",
                  SalFileName, StrZslFile );
             writelog( sLog);
           }

         StrZslFile = BackupFile(SalFileName, ChangeFileExt(str_zdt_path,".zsl"), DDZ_1099 );
         tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);

         sTmp=ChangeFileExt(StrZslFile,".asl");
         Auto_Sal->SaveToFile(sTmp);
         writelog("ZDATA::備份載入之SAL Auto_Sal->SaveToFile=>" + sTmp);

       }
    else
       {
         tif_tmp->WriteString("Z","ZSLNAME","");
       }

    AnsiString str_ipt_path;

    //更改XZDATA.INI
    int int_nzno_seq   = _StrToInt(str_pzno_seq, "pzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp->WriteString("Z","ZFILENAME" , str_zdt_path);
    tif_tmp->WriteString("Z","ZRPTNAME"  , str_zrp_path);
    tif_tmp->WriteString("Z","DEPTNAME"  , str_dpt_path);
    tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

    tif_tmp->WriteString("Z","PZ_DATE"     , str_z_date);
    tif_tmp->WriteString("Z","AM_TCLSACC" , IntToStr(int_am_tclsacc)) ;
    tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT 交易序號

    tif_tmp->WriteString("ECR","tot_open_z", "0");
    tif_ecr->WriteString("ECR","tot_open_z", "0");

    tif_tmp->WriteString("VXZ", "NZNO_SEQ", "1");
    tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");

    tif_tmp->WriteString("VXZ", "PZ_DATE", "");
    tif_ecr->WriteString("VXZ", "PZ_DATE", "");


    tif_tmp->WriteString("AUTOZ","LST_AUTOZ_DTTM",str_z_date.SubString(1,8)); //AutoZ_Dt);   //前次自動日結時間
    //tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM","");  //現再自動日結時間無作用
    tif_tmp->WriteString("AUTOZ","LST_ZCNT_DTTM",str_z_date.SubString(1,8));   //換日參考日


    if (bl_change_day) //判斷是否換日
    {
        //sLog.sprintf("ZDATA::日結換日  pz_date=%s, z_date=%s",str_pz_date, str_z_date);
        //writelog( sLog);

        tif_ecr->WriteString("Z","PZNO_SEQ", "0");
        tif_ecr->WriteString("Z","NZNO_SEQ", "1");
        tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);
        if (++int_nz_cnt > 99999)
            {
             tif_ecr->WriteString("Z","PZ_CNT", "00000");
             int_nz_cnt = 1;
            }

        str_nz_cnt.sprintf("%05d",int_nz_cnt);
        tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        sLog="ZDATA::換日處理 DATEZ=" + str_z_date.SubString(7,2)+ "1" +  ", YYYYMMDDZ=" + str_z_date.SubString(7,2) + "0100";
        writelog( sLog);

    }
    else
    {

        tif_ecr->WriteString("Z","PZNO_SEQ", str_pzno_seq);

        if (++int_nzno_seq > 9)
           {
            tif_ecr->WriteString("Z","PZ_CNT", "00000");
            int_nzno_seq = 1;
           }

        str_pzno_seq.sprintf("%d", int_nzno_seq);

        tif_ecr->WriteString("Z","NZNO_SEQ", str_pzno_seq);
        tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);

        if (++int_nz_cnt > 99999)
            int_nz_cnt = 1;

        str_nz_cnt.sprintf("%05d", int_nz_cnt);

        tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
                              + FormatFloat("00",int_nzno_seq) + "00"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        //sLog.sprintf("ZDATA::日結不換日  pz_date=%s, z_date=%s, SET NZNO_SEQ=%s",str_pz_date, str_z_date, str_pzno_seq);
        //writelog( sLog);

        sLog="ZDATA::未換日處理 DATEZ=" + str_z_date.SubString(7,2) + IntToStr(int_nzno_seq) +  ", YYYYMMDDZ=" +  str_z_date.SubString(1,8) + FormatFloat("00",int_nzno_seq) + "00";
        writelog( sLog);


    }

    sLog.sprintf("ZDATA::寫入 XZDATAINI [AUTOZ]->LST_AUTOZ_DTTM=%s, [AUTOZ]->LST_AUTOZ_DTTM=%s, [AUTOZ]->LST_ZCNT_DTTM=%s, [Z]->NZ_CNT=%s",
                  str_z_date.SubString(1,8), str_z_date.SubString(1,8), str_z_date.SubString(1,8), str_nz_cnt);
    writelog( sLog);

    str_pz_date    = str_z_date;
    str_pzno_seq   = tif_ecr->ReadString("Z","NZNO_SEQ","");      //目前Z帳次數   換日歸1

    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";


    //指定X 帳PATH
    AnsiString str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印
    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_tran_cnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    // 自動日結 不將X帳歸零    _WZ_OK 會規0
    tif_ecr->WriteString("X", "PTRAN_CNT", "00000000");
    tif_ecr->WriteString("X", "SX_DATE"  , "");
    tif_ecr->WriteString("X", "EX_DATE"  , "");
    //自動日結規不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理
    tif_ecr->WriteString("X", "PXNO_SEQ" , "0");


    sLog.sprintf("ZDATA::寫入 XZDATAINI Z->NZ_CNT=%s, AM_TCLSACC=%d, ECR->TRAN_CNT=%s, XPATH=%s, 日結 -> 將X帳歸零",
                  str_nz_cnt, int_am_tclsacc, str_sum_tran, str_x_path );
    writelog( sLog);


    return OK;


}


//X帳表  讀帳
int __fastcall XREPORT::WriteData( const String StoreNO, const String EcrNO, const String SalesNo ,
                                   const AnsiString SalFileName, const AnsiString Version)
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類


    AnsiString sLog;
    sLog.sprintf("X帳表 XREPORT::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    //if (!int_reslut)
    //    return 0;

    AnsiString str_ver_num, sTmp;

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號

    str_ver_num = get_version(Version);

    SPOS_ZRPDT *xr = new SPOS_ZRPDT;       //Z帳結構

    TIniFile *tif_ecr   = new TIniFile(XZDATA_INI);

    //目前Z帳時間
    AnsiString str_z_date;         //現在時間
    AnsiString str_pz_date;        //前次時間

    AnsiString str_date_time;
    str_date_time = gchar_date_time;

    str_z_date  = str_date_time;
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");

    if (str_pz_date.Length() < 6)
        str_pz_date = str_z_date;

    xr->no_tm = gchar_ecr_no;        //收銀機機號

    //日結序號
    AnsiString str_nz_cnt, sYYMMDDZZ, str_tran_cnt;
    str_nz_cnt = tif_ecr->ReadString("Z","nz_cnt","00001");

    xr->bg_noclose.sprintf("%05d", _StrToInt(str_nz_cnt,"nz_cnt"));

    AnsiString  str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數 01~09  換日歸1
    //str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999
    //str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零


    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

    //if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
    //    str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    sLog.sprintf("XREPORT::ECR->WriteData 取的目前之交易序號:%s", str_tran_cnt);
    writelog( sLog);


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_pzno_seq));


    xr->bg_dtclose = str_z_date.SubString(1,8);     //結帳日期
    xr->bg_tmclose = str_z_date.SubString(9,6);     //結帳時間
    xr->ed_dtclose = str_pz_date.SubString(1,8);    //前次結帳日期
    xr->ed_tmclose = str_pz_date.SubString(9,6);    //前次結帳時間


    AnsiString xr_a;
    AnsiString z_a;
    int iC0TotCount=0;

    xr_a.sprintf("1096|%04d|%6s|%2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|", 1594,
                                      gchar_tencode,
                                      xr->no_tm.c_str(),           //收銀機機號
                                      str_date_time.c_str(),      //現在時間
                                      xr->bg_noclose.c_str(),      //日結序號
                                      sYYMMDDZZ,                   //Z帳表序號
                                      xr->ed_dtclose.c_str(),      //前次結帳日期
                                      xr->ed_tmclose.c_str(),      //前次結帳時間
                                      xr->bg_dtclose.c_str(),      //結帳日期
                                      xr->bg_tmclose.c_str()      //結帳時間
                                    );



    //交易明細ｆｉｅｌｄ=============================================

    //本日來客數
    xr->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    xr->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;

    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    //預售兌換商品折讓單折扣/代收商品折讓單折扣
    //int iPreSalINVODisAmt ;
    //SumPreSal_INVO_DisAmt(0, gi_rec_line);
    //iPreSalINVODisAmt = giU;

    /* End 預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票  */

    TotCount(0,gi_tot_line);    //1050==> R4+Z0
    xr->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ));      //發票日結帳額(免)
    xr->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));      //發票日結帳額(稅)
    xr->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );      //發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ); // + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    sLog.sprintf("XREPORT::WriteData(%s, %s, %s)",
                  xr->tb_trans.am_trcpntx, xr->tb_trans.am_trcptax, xr->tb_trans.am_trcptot);
    writelog( sLog);


    //本日結帳累計金額
    AnsiString str_am_tclsacc;     //累積Z帳金額
    int int_acc_tot_amt;
    str_am_tclsacc = tif_ecr->ReadString("Z","AM_TCLSACC","");      //換日不歸零

    if (str_am_tclsacc.Length() < 1)
        str_am_tclsacc = "0";

    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));

    xr->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    // xr->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));


    TotMiss(0,gi_tot_line);       //1050 ==> R2R3R4
    // RC01FTaxAmt=giC; RC01TaxAmt=giD;
    xr->tb_trans.qt_miss.sprintf("%08d",   giA);        // 誤打發票次數   1050 ==> R2R3R4
    xr->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        // 誤打發票金額(免)
    xr->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        // 誤打發票金額(稅)
    xr->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        // 誤打發票金額
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX);


    //不開發票金額
    // + 10預售兌換入營收不開發票 (兌換應/免稅)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //本日實際銷貨收入 = 發票結帳金額 - 誤打發票金額 + 不開發票金額-不開發票金額誤退重
    xr->tb_trans.am_totsale.sprintf("%08d", (int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt));

    //發票開始號碼
    //發票結束號碼
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


    AnsiString xr_b;

    xr_b += (xr->tb_trans.qt_tcust+"|");             //本日來客數
    xr_b += (xr->tb_trans.qt_trans+"|");             //本日交易品項
    xr_b += (xr->tb_trans.am_tclsacc+"|");          //本日結帳累計金額

    xr_b += (xr->no_rcstart+"|");                   //發票開始號碼
    xr_b += (xr->no_rcend+"|");                      //發票結束號碼
    xr_b += (xr->tb_trans.am_trcpntx+"|");           //發票日結帳額(免)
    xr_b += (xr->tb_trans.am_trcptax+"|");          //發票日結帳額(稅)
    xr_b += (xr->tb_trans.am_trcptot+"|");           //發票日結帳金額

    xr_b += (xr->tb_trans.qt_miss+"|");             //誤打發票次數
    xr_b += (xr->tb_trans.am_missntx+"|");           //誤打發票金額(免)
    xr_b += (xr->tb_trans.am_misstax+"|");           //誤打發票金額(稅)
    xr_b += (xr->tb_trans.am_miss+"|");              //誤打發票金額
    xr_b += (xr->tb_trans.am_totsale+"|");           //本日實際銷貨收入   24


    //代售付ｔａｂｌｅ===========================================

    int iRentAmt, iRemtCnt;
    //代收
    SumKamt(0,gi_tot_line);                 //代收、售、付 ,點卡手續費giD.作廢giE
    int iC0TotAmt, iC0GivUpAmt;             //代售,手續費(應)giD, 代售,手續費作廢(應)giE , 點卡手續費客數giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //代售,手續費(免)giG, 代售,手續費作廢(免)giH
    int iqt_food,iamt_food;                 //giH giI 食安退貨金額、次數

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);   //giH giI 食安退貨金額、次數   //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    xr->tb_newadd.qt_agnrcv.sprintf("%08d",giD);               //代收費用次數
    xr->tb_newadd.am_agnrcv.sprintf("%08d",giC);               //代收費用金額


    if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
        ForceDirectories("C:\\FTLPOS\\HISTORY\\");

    //產生代售報表 2011/05/30
    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\subsale.rpt");
    report->CreateReport('s', " ", StrBillPay, gchar_tencode, gchar_ecr_no, 0,0,0,0,tsl_subsale);


    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;


    AnsiString xr_c;
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


    /****
    for (int j=0; j<5; j++)
    {
        for (int k=0; k<9; k++)
        {
            if (k<8)
            {
                if (j == 0)
                {
                    if (k == 0)
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giE);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giA);
                    }
                    else
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else if (j == 3)
                {
                    if (k == 0)
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giF);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giB);
                    }
                    else
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else
                {
                    xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                    xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                }

            }
            else
            {
                if (j == 0)
                {
                    xr_c += xr->tb_subrev[j].qt_totrev.sprintf("%08d",giE);
                    xr_c += xr->tb_subrev[j].am_totrev.sprintf("%08d",giA);
                }
                else if (j == 3)
                {
                    xr_c += xr->tb_subrev[j].qt_totrev.sprintf("%08d",giF);
                    xr_c += xr->tb_subrev[j].am_totrev.sprintf("%08d",giB);
                }
                else
                {
                    xr_c += xr->tb_subrev[j].qt_totrev.sprintf("%08d",0);
                    xr_c += xr->tb_subrev[j].am_totrev.sprintf("%08d",0);
                }

            }
        }
    }
    ****/

    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

     Sub_C0InvoAmt4VDC(0, gi_rec_line);
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

    //25~28
    xr_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

   //////////////////////////////////////////////////////////////////////////////


    // 03 代現金ｔａｂｌｅ=============================================
    int iqt_ffu1,iam_ffu1,iTotOt2Rtn;
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

    xr->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);               //禮券回數
    xr->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);               //禮券金額

    xr->tb_cash.qt_vndcpn.sprintf("%08d", 0);                //廠商代現金回數
    xr->tb_cash.am_vndcpn.sprintf("%08d", 0);                //廠商代現金金額

    xr->tb_cash.qt_stocpn.sprintf("%08d", giC);              //門市代現金回數
    xr->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt );              //門市代現金金額  折價券

    xr->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));     //代現金合計回數
    xr->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));     //代現金合計金額

    //xr->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));     //代現金合計回數
    //xr->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));     //代現金合計金額

    xr->tb_cash.qt_bonout.sprintf("%08d", giG);              //溢收合計回數
    xr->tb_cash.am_bonout.sprintf("%08d", giH);              //溢收合計金額

    iTotOt2Rtn=giE;                                       //餘額券
    iqt_ffu1=giI;                                         //現金卡回數   電子錢回數
    iam_ffu1=giJ;                                         //現金卡金額   電子錢金額   刷卡合計

    AnsiString xr_d;

    xr_d += (xr->tb_cash.qt_bonus+"|");               //禮券回數      29
    xr_d += (xr->tb_cash.am_bonus+"|");                //禮券金額     30
    xr_d += (xr->tb_cash.qt_vndcpn+"|");               //廠商折價券回數   31
    xr_d += (xr->tb_cash.am_vndcpn+"|");               //廠商折價券金額   32
    xr_d += (xr->tb_cash.qt_stocpn+"|");               //折價券回數       33
    xr_d += (xr->tb_cash.am_stocpn+"|");               //折價券金額       34
    xr_d += (xr->tb_cash.qt_totcash+"|");              //代現金合計回數   35
    xr_d += (xr->tb_cash.am_totcash+"|");              //代現金合計金額   36

    sTmp.sprintf("%08d",giF);
    xr_d += (sTmp+"|");                               //餘額合計回數      37
    sTmp.sprintf("%08d",giE);
    xr_d += (sTmp+"|");                               //餘額合計金額      38

    xr_d += (xr->tb_cash.qt_bonout+"|");                 //溢收合計回數       39
    xr_d += (xr->tb_cash.am_bonout+"|");;                //溢收合計金額       40

    //==========================================================================

    //現金合計 含 消費券
    SumCash41=SumCash(0,gi_tot_line);
    xr->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    //sLog.sprintf("XREPORT::現金合計(%s)",xr->am_cash);
    //writelog( sLog);


    //char xre[29];
    //xre[0] = 0;

   //strcpy(xre, xr->am_cash.c_str());         //現金合計
    //strcat(xre, xr->no_rcstart.c_str());      //發票開始號碼
    //strcat(xre, xr->no_rcend.c_str());        //發票結束號碼
    //xre[28] = 0;


    // Z帳部門別集計TABLE      =============================================
    AnsiString xr_f;

    int s1, s2, s3, s4;
    s1 = s2 = s3 = s4 = 0;

    DEPT *dept;

    try
    {

        dept = new DEPT[20];

        //先將值填零
        for (int i=0; i<20; i++)
        {
            xr->tb_depcal[i].am_dpsitm.sprintf("%08d", 0);
            xr->tb_depcal[i].am_dpsale.sprintf("%08d", 0);
            xr->tb_depcal[i].am_dpmitm.sprintf("%08d", 0);
            xr->tb_depcal[i].am_dpmiss.sprintf("%08d", 0);
            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //計算所有部門交易資料, 部門帳
        if (gbl_rec_haveline)
            SumAllDept(dept);

        for (int i=0; i<20; i++)
        {
            xr->tb_depcal[i].am_dpsitm.sprintf("%08d",dept[i].am_dpsitm);
            xr_f += (xr->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            xr->tb_depcal[i].am_dpsale.sprintf("%08d",dept[i].am_dpsale);
            xr_f += (xr->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            xr->tb_depcal[i].am_dpmitm.sprintf("%08d",dept[i].am_dpmitm);
            xr_f += (xr->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            xr->tb_depcal[i].am_dpmiss.sprintf("%08d",dept[i].am_dpmiss);
            xr_f += (xr->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;
        }
    }
    __finally
    {
        delete [] dept;
    }



    AnsiString xr_g;

    xr_g += (xr->am_tdpsitm.sprintf("%08d", s1)+"|");     //部門合計交易項數 41
    xr_g += (xr->am_dpstot.sprintf("%08d",  s2)+"|");     //部門合計銷售金額 42
    xr_g += (xr->am_tdpmitm.sprintf("%08d", s3)+"|");     //部門合計誤退項數 43
    xr_g += (xr->am_dpmtot.sprintf("%08d",  s4)+"|");     //部門合計誤退金額 44
    xr_g +=  (xr->am_cash+"|");                           //現金合計含消費券 45


    //收銀員明細ｆｉｅｌｄ =============================================

    AnsiString xr_h;

    xr_h += (xr->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");         // 借零次數      46
    //sTmp.sprintf("%08d",VisuilCpnAmt);                                   //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    //xr_h += (sTmp+"|");

    //xr_h += (xr->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");         // 借零合計金額  47
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);                       //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    xr_h += (sTmp+"|");                           // 代收商品折讓單折扣  借零合計金額

    xr_h += (xr->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");         // 投零次數      48
    xr_h += (xr->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");         // 投零合計金額  49

    //投庫
    AnsiString x_h;

    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;                           //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                    //投庫總額
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生X帳表:非投庫(%s)資料",s1041);
             writelog(sTmp );
             continue;
           }


        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC付款
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //信用卡
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //提貨單

        //中獎發票金額  800   Cloud 中獎發票金額(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s1041, "|", 18) );

        //中獎發票金額 500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //中獎發票金額 1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //中獎發票金額 200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //費用 13金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //費用 14金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數

    }

    xr_h += (xr->tb_casher.qt_droptot.sprintf("%08d",int_drops)+"|");          //投庫次數    50
    xr_h += (xr->tb_casher.am_droptot.sprintf("%08d",int_drop_sum)+"|");   //投庫合計金額    51

    //更正次數、金額
    SumUpdat(0,gi_rec_line);
    xr_h += (xr->tb_casher.qt_update.sprintf("%08d",giA)+"|");            //更正次數         52
    xr_h += (xr->tb_casher.am_update.sprintf("%08d",giB)+"|");            //更正合計金額     53

    //交易取消
    SumVoid(0,gi_tot_line);
    xr_h += (xr->tb_casher.qt_void.sprintf("%08d",giA)+"|");               // 交易取消次數     54
    xr_h += (xr->tb_casher.am_void.sprintf("%08d",giB)+"|");               // 交易取消合計金額 55

    //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

         //   tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
         //   tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

    }
    __finally
    {
          ;;
    }

    
    xr_h += (xr->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // 訓練合計次數  56
    xr_h += (xr->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // 訓練合計金額  57


    //手輸部門加總13、14
    SumInpdp(0,gi_rec_line);
    xr_h += (xr->tb_newadd.qt_inpdp.sprintf("%08d",giA)+"|");             //手動輸入部門次數  58
    xr_h += (xr->tb_newadd.am_inpdp.sprintf("%08d",giB)+"|");            //手動輸入部門金額   59

    xr_h += (xr->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");      // 開錢櫃次數      60

    //折扣次數   //小計折扣
    SumDisc(0,gi_pay_line);
    xr_h += (xr->tb_casher.qt_disc.sprintf("%08d",giA)+"|");             // 折扣次數(應稅)   61
    //xr_h += (xr->tb_casher.am_disc.sprintf("%08d",giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");             // 折扣金額(應稅)   62
    xr_h += (xr->tb_casher.am_disc.sprintf("%08d",giB+iPreSalMMDisAmt_MFP)+"|");             // 折扣金額(應稅)   62 ,改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    xr_h += (xr->tb_newadd.qt_ffu6.sprintf("%08d",giA)+"|");             //促銷折扣/讓次數   63
    xr_h += (xr->tb_newadd.am_ffu6.sprintf("%08d",giB+iPreSalMMDisAmt_TM)+"|");             //促銷折扣/讓金額   64


    //加值機
    easy_card(0,gi_easy_line);
    xr_h += (xr->tb_casher.qt_disret1.sprintf("%08d",giA)+"|");            // 加值機開/售卡次數  65
    xr_h += (xr->tb_casher.am_disret1.sprintf("%08d",giB)+"|");            // 加值機開/售卡金額  66
    xr_h += (xr->tb_casher.qt_disret2.sprintf("%08d",giC)+"|");            // 加值機加值次數     67
    xr_h += (xr->tb_casher.am_disret2.sprintf("%08d",giD)+"|");            // 加值機加值金額     68

    //fetc_card(0,gi_fetc_line);   //e通卡  // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;

    //xr_h += (xr->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx + PreSal09_AmtNTX )+"|");    //備用八次數  現金 加值次數   69   2015/08/19 代售,手續費銷售金額(免)
    //xr_h += (xr->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //備用八金額  現金 加值金額   70   2015/08/19 代售,手續費作廢金額(免)
    xr_h += (xr->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");    //備用八次數  現金 加值次數   69   2015/08/19 代售,手續費銷售金額(免)
    xr_h += (xr->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //備用八金額  現金 加值金額   70   2015/08/19 代售,手續費作廢金額(免)

    //食安次數、金額  int iqt_food,iamt_food;
    xr_h += (xr->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數 71   2015/08/19 0
    xr_h += (xr->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額 72   2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //xr_h += (xr->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數 71   2015/08/19 0
    //xr_h += (xr->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額 72   2015/08/19 0


    //追加 table  =============================================

    AnsiString xr_i;

    xr_i += (xr->tb_newadd.qt_cashCard.sprintf("%08d",iqt_ffu1)+"|");         //刷卡/現金卡次數  73
    xr_i += (xr->tb_newadd.am_cashCard.sprintf("%08d",iam_ffu1)+"|");         //刷卡/現金卡金額  74
    xr_i += (xr->tb_newadd.qt_parking.sprintf("%08d",0)+"|");           //代收停車費次數         75
    xr_i += (xr->tb_newadd.am_parking.sprintf("%08d",0)+"|");           //代收停車費金額 ,不開發票金額誤退重  76
    xr_i += (xr->tb_newadd.qt_agnrcv+"|");                              //代收費用次數     77
    xr_i += (xr->tb_newadd.am_agnrcv+"|");                              //代收費用金額     78
    xr_i += (xr->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //版本前置符號      79
    xr_i += (xr->tb_newadd.tx_ver.sprintf("%6s", str_ver_num)+"|");    //版本號碼          80
    xr_i += (xr->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  // 發票金額0次數

    //不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);
    xr_i += (xr->tb_newadd.am_ffu4.sprintf("%08d",int_novoice_amt)+"|");        //備用四金額 , 不開發票金額
    xr_i += (xr->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");   // 加值機結帳金額,不開發票金額誤退重

    SumLOSS(0,gi_rec_line);
    xr_i += (xr->tb_newadd.qt_ffu5.sprintf("%08d",giA)+"|");             //TM廢棄Loss值次數
    xr_i += (xr->tb_newadd.am_ffu5.sprintf("%08d",giB)+"|");             //TM廢棄Loss值金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計
    xr_i += (xr->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");     //原加值機加值取消金額

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //xr_i += (xr->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    xr_i += (xr->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //原加值機加值取消次數

    xr_i += (xr->tb_newadd.am_posZRpt.sprintf("%08d",iCSMAmt)+"|");           //扣卡機扣卡金額  消費券


    AnsiString xr_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    即時購與銷轉進值應稅
    xr_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //即時購與銷轉進值免稅
    xr_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);
    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數  中獎發票張數
    xr_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額  中獎發票金額
    xr_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    xr_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    xr_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //代售手續費應稅金額 giD.  代售手續費應稅作廢金額 giE , 代售手續費來客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    xr_k +=sTmp;

    //xr_i += xr->tb_newadd.qt_ffu4.sprintf("%08d",gi_mix);         //代收來客數

    //xr_i += xr->tb_newadd.qt_posZRpt.sprintf("%08d",0);           //扣卡機扣卡次數

    // xr_h += xr->tb_casher.qt_pack.sprintf("%08d",iTotOt2Rtn); //giE);     // 加值機結帳次數  2007/12/01 找餘額券

    //xr_i += xr->tb_newadd.qt_ffu10.sprintf("%08d",0); //giFetcSumCnt);     //備用十次數  FETC 結帳次數   保留
    //xr_i += xr->tb_newadd.am_ffu10.sprintf("%08d",0); //giFetcSumAmt);     //備用十金額  FETC 結帳金額   保留



    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);

     /***
    if (xr_a.Length()!=34)
    {
        writelog( ("xr_a 長度應為 34 目前長度 " + IntToStr(xr_a.Length())) );
        xr_a = xr_a.SubString(1,34);
    }

    if (xr_b.Length()!=88)
    {
        writelog( ("xr_b 長度應為 88 目前長度 " + IntToStr(xr_b.Length())) );
        xr_b = xr_b.SubString(1,88);
    }

    if (xr_c.Length()!=720)
    {
        writelog( ("xr_c 長度應為 720 目前長度 " + IntToStr(xr_c.Length())) );
        xr_c = xr_c.SubString(1,720);
    }

    if (xr_d.Length()!=80)
    {
        writelog( ("xr_d 長度應為 80 目前長度 " + IntToStr(xr_d.Length())) );
        xr_d = xr_d.SubString(1,80);
    }

    if (strlen(xre)!=28)
    {
        writelog( ("xr_e 長度應為 29 目前長度 " + IntToStr(strlen(xre))) );
        xre[28] = 0;
    }

    if (xr_f.Length()!=640)
    {
        writelog( ("xr_f 長度應為 640 目前長度 " + IntToStr(xr_f.Length())) );
        xr_f = xr_f.SubString(1,640);
    }

    if (xr_g.Length()!=32)
    {
        writelog( ("xr_g 長度應為 32 目前長度 " + IntToStr(xr_g.Length())) );
        xr_g = xr_g.SubString(1,32);
    }

    if (xr_h.Length()!=168)
    {
        writelog( ("xr_h 長度應為 168 目前長度 " + IntToStr(xr_h.Length())) );
        xr_h = xr_h.SubString(1,168);
    }

    if (xr_i.Length()!=208)
    {
        writelog( ("xr_i 長度應為 208 目前長度 " + IntToStr(xr_i.Length())) );
        xr_i = xr_i.SubString(1,208);
    }
    ****/

    //XReport讀帳報表
    AnsiString str_xr;

    //str_xr.sprintf("%s%s%s%s%s%s%s%s%8s%88s|\n",
    //str_xr.sprintf("%s%s%s%s%s%s%s%s%8s%79s|\n",    //2012/09/18
    //str_xr.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",     //2013/03/15
    //str_xr.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",     //2013/10/17      點卡手續費 Update
    str_xr.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                   xr_a.c_str(),
                   xr_b.c_str(),
                   xr_c.c_str(),
                   xr_d.c_str(),
                   xr_g.c_str(),
                   xr_h.c_str(),
                   xr_i.c_str(),
                   xr_f.c_str(),
                   xr_k.c_str(),
                   "FFFFFFFF"  );





    //report = new REPORT();      //讀帳帳表

    sLog.sprintf("XREPORT::(%s)Len=%d",str_xr, str_xr.Length());

    writelog( sLog);


    if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
        ForceDirectories("C:\\FTLPOS\\HISTORY\\");

    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\XReport.xpp");
   // report->CreateReport('X', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no);

   // 2006/08/03 for 代收票券
   report->CreateReport('X', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no,
                        iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );


    sTmp.printf("CreateReport: 產生產生讀帳帳表 代售合計=(%d),(%d)",iqt5_15217totrev,iam5_15217totrev);
    writelog(sTmp);

    writelog("產生讀帳帳表 C:\\FTLPOS\\HISTORY\\XReport.xpp");

    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\XReport.xdp");
    report->CreateReport('d', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no);
    writelog("產生讀帳帳表(部門) C:\\FTLPOS\\HISTORY\\XReport.xdp");


    // 檢查 1050 及 SAL 1050 筆數是否一致
    int RtnZDTCnts, RtnSALCnts;
    AnsiString  strchkfile;
    strchkfile.sprintf("C:\\FTLPOS\\XZDATA\\SAL\\%s.sal",gchar_ecr_no);
    _GetZDTZSALCntCheck(gtsl_tot, strchkfile, str_nz_cnt,
                        RtnZDTCnts, RtnSALCnts );

    tif_ecr->WriteString("XREPORT","XREPORTDATE",str_date_time);
    tif_ecr->WriteString("XREPORT","ZDTCnts",IntToStr(RtnZDTCnts) );
    tif_ecr->WriteString("XREPORT","SALCnts",IntToStr(RtnSALCnts) );

    delete xr, tif_ecr;

    

    return 1;
}



AnsiString __fastcall ZDATA::write_xpath(AnsiString FileName)
{
    AnsiString str_file_dd,                //日期
               str_file_num,               //次數
               str_bak_path;                //備份路徑


    AnsiString str_his_folder = "C:\\FTLPOS\\HISTORY\\";
    AnsiString str_abs_file = ExtractFileName(FileName);

    str_file_dd  = str_abs_file.SubString(7,2);      //DD
    str_file_num = str_abs_file.SubString(9,2);      //01~09

    //備份路徑
    if ( (!str_file_dd.IsEmpty()) && (!str_file_num.IsEmpty()) )
        str_bak_path = ( str_his_folder + str_file_dd + "\\" + str_file_num + "\\" );
    else
        str_bak_path = str_his_folder;

    //建立目錄
    if (!DirectoryExists(str_bak_path))
        ForceDirectories(str_bak_path);

    return str_bak_path;

}


//擔當同交班
int __fastcall CHECKIN::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version)
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName,true, "", "");               //載入SAL檔，將檔案依型態分類

    //if (!int_reslut)
    //    return 0;


    AnsiString sLog,sTmp;
    sLog.sprintf("CHECKIN::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);


    AnsiString str_ver_num;

    str_ver_num = get_version(Version);
    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號

    //SPOS_XRPDT *x = new SPOS_XRPDT;   //X帳檔案結構

    //TIniFile *tif_ecr   = new TIniFile(XZDATA_INI);

    AnsiString str_x_filename,              //XDATA檔名
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


    str_date_z = tif_ecr->ReadString("Z","DATEZ","");             //ddn
    //str_tran_cnt = tif_ecr->ReadString("ECR","TRAN_CNT","");      //目前交易序號
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("CHECKIN::ECR->WriteData 取的目前之交易序號:%s", str_tran_cnt);
    writelog( sLog);


    str_YYYYMMDDZ=tif_ecr->ReadString("Z","YYYYMMDDZ","201001010000");
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "00001");          //999999目前日結序號  換日不歸零

    str_date_time = gchar_date_time;

    str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","");        //上次x帳累積次數
                                                                                                              
    if ( str_pxno_seq.Length() < 2)                                                                                    
        str_pxno_seq = "00";                                                                                     
                                                                                                              
    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq) + 1) );   //目前x次到第幾次


    //最多99次X帳
    if ( (_StrToInt(str_nxno_seq) > 99) )                                                                        
        str_nxno_seq = "01";                                                                                     
                                                                                                              
    x->no_tm = gchar_ecr_no;                           //收銀機機號
    x->no_casher = gchar_cashier;                      //收銀員號碼

    if (!x->no_casher.Trim().Length())
        x->no_casher = "00001";
                                                                                                              
    //日結序號                                                                                                
    x->no_seq = str_nxno_seq;

    AnsiString str_x_first, str_sal_last, str_sal_last_1099;        //*.sal 最後一筆
    str_x_first = gpchar_x_first;
    str_sal_last = gpchar_sal_last;
    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    if (str_x_first != "")
    {
        x->dt_begin = str_x_first.SubString(21,8);   //開始日期
        x->tm_begin = str_x_first.SubString(29,6);   //開始時間
    }
    else
    {
        x->dt_begin = "00000000";
        x->tm_begin = "000000";
    }


    AnsiString str_invo_char;                    //發票英文字母
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.Length() < 2)
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
    str_x_filename = "C:\\Abacus\\HISTORY\\checkin.xdt";
                                                                                                              
    //結束時間目前時間為目前結帳時間                                                                          
    x->dt_end = str_date_time.SubString(1,8);
    x->tm_end = str_date_time.SubString(9,6);

    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000"))
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }
                                                                                                              
    AnsiString x_a;
    int iC0TotCount=0;
    x_a.sprintf("1095|%04d|%-6s|%-2s|%14s|%5s|%-10s|%2s|%5s|%8s|%6s|%8s|%6s|",1779,
                                        gchar_tencode,
                                        x->no_tm.c_str(),         //收銀機機號
                                        ed_date_time.c_str(),
                                        str_nz_cnt.c_str(),
                                        str_YYYYMMDDZ.SubString(3,8),
                                        x->no_seq.c_str(),        //交班序號
                                        x->no_casher.c_str(),     //收銀員號碼
                                        x->dt_begin.c_str(),      //開始日期
                                        x->tm_begin.c_str(),      //開始時間
                                        x->dt_end.c_str(),        //結束日期
                                        x->tm_end.c_str()         //結束時間
                                     );
                                                                                                              
    //交易明細ｆｉｅｌｄ=====================================================

    //本日來客數
    x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //本日來客數
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    x->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;

    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    TotCount(0,gi_tot_line);
    x->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );        //發票日結帳額(免)
    x->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));        //發票日結帳額(稅)
    x->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));        //發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    TotMiss(0,gi_tot_line);
    x->tb_trans.qt_miss.sprintf("%08d",    giA);        //誤打發票次數
    x->tb_trans.am_missntx.sprintf("%08d", giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //誤打發票金額(免)
    x->tb_trans.am_misstax.sprintf("%08d", giC-RC01TaxAmt-R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //誤打發票金額(稅)
    x->tb_trans.am_miss.sprintf("%08d",    giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        //誤打發票金額
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

    x->tb_trans.am_totsale.sprintf("%08d", int_totsale);

    AnsiString x_b;

    x_b  = (x->tb_trans.qt_tcust+"|");                      //本日來客數
    x_b += (x->tb_trans.qt_trans+"|");                      //本日交易品項
    x_b += (x->no_rcstart+"|");                              //發票開始號碼
    x_b += (x->no_rcend+"|");                                //發票結束號碼
    x_b += (x->tb_trans.am_trcpntx+"|");                    //發票日結帳額(免)
    x_b += (x->tb_trans.am_trcptax+"|");                    //發票日結帳額(稅)
    x_b += (x->tb_trans.am_trcptot+"|");                    //發票日結帳金額
    x_b += (x->tb_trans.qt_miss+"|");                       //誤打發票次數
    x_b += (x->tb_trans.am_missntx+"|");                    //誤打發票金額(免)
    x_b += (x->tb_trans.am_misstax+"|");                    //誤打發票金額(稅)
    x_b += (x->tb_trans.am_miss+"|");                       //誤打發票金額
    x_b += (x->tb_trans.am_totsale+"|");                    //本日實際銷貨收入



    //代售付TABLE===============================================================

    AnsiString x_c;
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

    x->tb_newadd.qt_cetel.sprintf("%08d", giD);          //代收次數
    x->tb_newadd.am_cetel.sprintf("%08d", giC);          //代收金額


    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

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

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
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

     x_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
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

    x->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);       //禮券回數
    x->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //禮券金額
    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

    x->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //廠商折價券回數 目前不使用
    x->tb_cash.am_vndcpn.sprintf("%08d", 0);             //廠商折價券金額

    x->tb_cash.qt_stocpn.sprintf("%08d", giC);           //折價券回數
    x->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //折價券金額
    SumCpn41 +=  giD;      //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;
    //x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI - giT));      //代現金合計回數
    //x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ - iCSMAmt));  //代現金合計金額

    x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //代現金合計回數
    x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    x->tb_cash.qt_bonout.sprintf("%08d", giG);           //溢收合計回數
    x->tb_cash.am_bonout.sprintf("%08d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //giE:餘額券金額 / giF:餘額券次數
    iqt_ffu1=giI;                                        //現金卡回數  電子錢次數  3275
    iam_ffu1=giJ;                                        //現金卡金額  電子錢金額  刷卡合計 3261

    AnsiString x_d;

    x_d += (x->tb_cash.qt_bonus+"|");                //禮券回數
    x_d += (x->tb_cash.am_bonus+"|");                //禮券金額
    x_d += (x->tb_cash.qt_vndcpn+"|");               //廠商折價券回數
    x_d += (x->tb_cash.am_vndcpn+"|");               //廠商折價券金額
    x_d += (x->tb_cash.qt_stocpn+"|");               //折價券回數
    x_d += (x->tb_cash.am_stocpn+"|");               //折價券金額
    x_d += (x->tb_cash.qt_totcash+"|");              //代現金合計回數
    x_d += (x->tb_cash.am_totcash+"|");              //代現金合計金額

    sTmp.sprintf("%08d",giF);
    x_d += (sTmp+"|");                         //餘額合計回數
    sTmp.sprintf("%08d",giE);
    x_d += (sTmp+"|");                         //餘額合計金額

    x_d += (x->tb_cash.qt_bonout+"|");               //溢收合計回數
    x_d += (x->tb_cash.am_bonout+"|");               //溢收合計金額


    //==========================================================================

    //現金合計 含 消費券
    SumCash41=SumCash(0,gi_tot_line);
    x->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    AnsiString x_e;

    x_e += (x->am_cash+"|");               //現金合計
    //x_e += (x->no_rcstart+"|");            //發票開始號碼
    //x_e += (x->no_rcend+"|");              //發票結束號碼

    //收銀員明細FIELD===========================================================
                                                                                                              
    AnsiString x_f;

    //借零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tb_vendfld[i].am_vend.sprintf("%08d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

        x->tb_casher.tb_vendfld[i].tm_vend.sprintf("%06d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

    x->tb_casher.qt_vendtot.sprintf("%08d",0);
    x->tb_casher.am_vendtot.sprintf("%08d",0);

    //x_f += (x->tb_casher.qt_vendtot+"|");      //借零次數
    sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");                          //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)

    //x_f += (x->tb_casher.am_vendtot+"|");      //借零合計金額
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);             //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    x_f += (sTmp+"|");                           // 代售商品折讓金額  借零合計金額

    AnsiString x_g;

    //投零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_feedfld[i].am_feed.sprintf("%08d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

        x->tb_casher.tg_feedfld[i].tm_feed.sprintf("%06d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

    x->tb_casher.qt_feedtot.sprintf("%08d",0);
    x->tb_casher.am_feedtot.sprintf("%08d",0);

    x_g += (x->tb_casher.qt_feedtot+"|");      //投零次數
    x_g += (x->tb_casher.am_feedtot+"|");      //投零合計金額


    AnsiString x_h;
                                                                                                              
    //投庫                                                                                                    
    int int_drop_line = (gtsl_drop->Count - 1);   //投庫次數

    //初始化全部填零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_dropfld[i].am_drop.sprintf("%08d",0);    //投庫金額
        x->tb_casher.tg_dropfld[i].tm_drop.sprintf("%-6s"," ");    //執行時間
    }

    int int_drops = 0;                            //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                         //投庫總額
    int x_rtn_cnt=0;                              //本收銀員投庫累計次數
    AnsiString str_drop_time,s;

    for (int i=0; i<=int_drop_line; i++)
    {
        s=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生 擔當X 帳表:非投庫(%s)資料",s);
             writelog(sTmp );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, "|", 11) );     //現金
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 12) );     //IC付款
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 13) );     //信用卡
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 14) );    //禮卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 15) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 16) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 17) );    //提貨單

        //中獎發票金額  800   Cloud 中獎發票金額(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 18) );

        //中獎發票金額  500
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 19) );

        //中獎發票金額  1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 20) );

        //中獎發票金額  200
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 22) );

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 23) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 25) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 27) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 29) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 31) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 33) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 35) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 37) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 39) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 41) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 43) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 45) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 47) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 49) );

        //每次投庫金額
        x->tb_casher.tg_dropfld[int_drops].am_drop.sprintf("%08d", int_drop_money);

        int_drop_sum += int_drop_money;                         //投庫總額

        str_drop_time = _StringSegment_EX(s, "|", 5).SubString(9,6);  //投庫時間

        //每次投庫時間
        x->tb_casher.tg_dropfld[int_drops].tm_drop.sprintf("%-6s", str_drop_time);

        int_drop_money = 0;
        ++int_drops;                             //投庫次數
     }

    //for (int i=0; i<50; i++)
    //{
   //     x_h += (x->tb_casher.tg_dropfld[i].am_drop+"h");  //投庫總次總合
   //     x_h += (x->tb_casher.tg_dropfld[i].tm_drop+"h");  //投庫金額總合
    //}


    AnsiString x_i;

    x_i += (x->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        //投庫次數
    x_i += (x->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     //投庫合計金額

    SumUpdat(0,gi_rec_line);
    x_i += (x->tb_casher.qt_update.sprintf("%08d", giA)+"|");               //更正次數
    x_i += (x->tb_casher.am_update.sprintf("%08d", giB)+"|");               //更正合計金額

    SumVoid(0,gi_tot_line);
    x_i += (x->tb_casher.qt_void.sprintf("%08d", giA)+"|");                 //交易取消次數
    x_i += (x->tb_casher.am_void.sprintf("%08d", giB)+"|");                 //交易取消合計金額

     SumInpdp(0,gi_rec_line);
    x_i += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    x_i += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //開錢櫃次數

    //小計折扣  1011
    SumDisc(0,gi_pay_line);
    x_i += (x->tb_casher.qt_dise.sprintf("%08d", giA)+"|");                 //折扣次數
    //x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");                 //折扣金額
    x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP)+"|");        //折扣金額(應稅),改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    x_i += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //促銷折扣/讓次數
    x_i += (x->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");         //促銷折扣/讓金額


    easy_card(0,gi_easy_line);
    x_i += (x->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");                  //加值機開售卡次數
    x_i += (x->tb_casher.am_disret1.sprintf("%08d", giB)+"|");                  //加值機開售卡金額
    x_i += (x->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");                  //加值機加值次數
    x_i += (x->tb_casher.am_disret2.sprintf("%08d", giD)+"|");                  //加值機加值金額


    //x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //開錢櫃次數
    //x_i += (x->tb_casher.qt_pack.sprintf("%08d" , iTotOt2Rtn)+"|"); //giE);     //加值機結帳次數   2007/12/01 找餘額券
    //x_i += (x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"I");   //加值機結帳金額, 不開發票金額誤退重



    //追加TABLE=================================================================

                                                                                                              
    AnsiString x_j;                                                                                           
                                                                                                              
   //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //備用八金額  現金 加值金額 2015/08/19 代售,手續費作廢金額(免)
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //備用八金額  現金 加值金額 2015/08/19 代售,手續費作廢金額(免)

    //食安次數、金額  int iqt_food,iamt_food;
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0
    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //現金卡次數   電子錢次數  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //現金卡金額   電子錢金額  刷卡合計 3261

    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //備用十次數  FETC 結帳次數  保留
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"|"); //giFetcSumAmt);     //備用十金額  FETC 結帳金額  保留

    //發票金額0次數

    x_j += (x->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");         //代收停車費次數
    x_j += (x->tb_newadd.am_parking.sprintf("%08d", 0)+"|");         //代收停車費金額
    x_j += (x->tb_newadd.qt_cetel+"|");                              //代收次數FUNCTIO在前面
    x_j += (x->tb_newadd.am_cetel+"|");                              //代收金額
    //x_j += (x->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix)+"|");       //代收商品客數
    //x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");     //現金卡次數   電子錢次數  3275
    //x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");     //現金卡金額   電子錢金額  刷卡合計 3261
    //x_j += (x->tb_newadd.qt_ffu2.sprintf("%08d", 0)+"|");            //扣卡機扣卡次數                    3269
    //x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //扣卡機扣卡金額  2008/12/04 消費券  3277
    x_j += (x->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //版本前置符號  3285
    x_j += (x->tb_newadd.tx_ver.sprintf("%6s" , str_ver_num)+"|");   //版本號碼
    x_j += (x->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");    //發票金額0次數

    //不開發票金額
    x_j += (x->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //備用四金額,  不開發票金額
    x_j +=(x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"|");   //不開發票誤退重合計

    SumLOSS(0,gi_rec_line);
    x_j += (x->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");         //TM廢棄Loss次數
    x_j += (x->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");         //TM廢棄Loss金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);        // 健康捐合計
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //原加值機加值取消次數
    x_j += (x->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");   //原加值機加值取消金額

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //原加值機加值取消次數

    x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //消費券


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


    // 2012/09/25  Update
    if( SaleDataVer >= "2012090100")
      {
          for (int i=0; i<60; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //投庫金額
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //投庫時間
            }

      }
    else
      {
          for (int i=0; i<30; i++)
            {
              x_h += ("00000000|");  //借零金額
              x_h += ("      |");    //借零時間
            }           //"000000|"

          for (int i=0; i<30; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|"; //("00000000|");  //投庫金額
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|"; //("000000|");    //投庫時間
            }
     }

    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    即時購與銷轉進值應稅
    x_h +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //即時購與銷轉進值免稅
    x_h +=sTmp;

    //發票兌獎金額 中獎發票張數  金額
    VoicePayAmt(0,gi_tot_line);
    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數
    x_h +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for 收銀員交班明細表
    //SumCash41+=giA;                // 2012/10/08 for 收銀員交班明細表

    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    x_h +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt, iC0GivUpAmt, iC0TotCount); //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    x_h +=sTmp;

    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);

    //X報表
    AnsiString xdt_1095_data;
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%88s|\n",
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%79s|\n",   2012/09/18
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%61s|\n",   2013/03/15
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%43s|\n",   2013/10/17   點卡手續費 Update
    xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%16s|\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),
                          x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          "1FFFFFFF"
                          );




    FILE *fxdata;
    if ((fxdata = fopen(str_x_filename.c_str(),"w")) != NULL)
    {
        fprintf(fxdata,"%s",xdt_1095_data.c_str() );
        fclose(fxdata);
    }
    else
    {
         sTmp.printf("File Open Error:%s",str_x_filename.c_str());
         writelog(sTmp);
    }

    //X報表                                                                                                   
    AnsiString str_xrp_checkin,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_cashier_no,
               str_start_trans;
                                                                                                              
    str_xrp_checkin=xdt_1095_data;

    if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
        ForceDirectories("C:\\FTLPOS\\HISTORY\\");
        
    //report = new REPORT();      //報到帳表
    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\checkin.xrp");
    report->CreateReport('c', str_xrp_checkin, StrBillPay, gchar_tencode, gchar_ecr_no,
                              iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 );
    writelog("產生報到帳表 C:\\FTLPOS\\HISTORY\\checkin.xrp");


    /**************************************************************************/


    str_tencode = gchar_tencode;   //店號
    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"機號"));              //機號
    str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

    AnsiString str_trans1_5, CasherRptFile;    //交易序號前5碼
    //str_trans1_5 = str_sal_last.SubString(36,5);

    CasherRptFile=ChangeFileExt(report->char_filename,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    sTmp.sprintf("1041|0500|%6s|%2s|%14s|%5s|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        (str_trans1_5 + str_tran_cnt).c_str(),
                        str_cashier_no.c_str()
                        );


    //產生收銀員交班明細表  收銀員交接班明細表
    AnsiString SumCasherRptData;
    SumCasherRpt(NAUTOZ, 0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt, sTmp, "00",
                 CasherRptFile, "", "", "", "", SumCasherRptData, iamt_food, iqt_food);
    sTmp.printf("產生收銀員交班明細表:%s",CasherRptFile);
    writelog(sTmp );

   // delete x, tif_ecr, report;
    return 1;
}

CHECKIN::CHECKIN()
{
    x = new SPOS_XRPDT;   //X帳檔案結構
    tif_ecr   = new TIniFile(XZDATA_INI);
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
    tif_ecr = new TIniFile(XZDATA_INI);
    tif_tmp = new TIniFile(XDATATMP_INI);
    report = new REPORT();      //收銀員帳表
}

XDATA::~XDATA()
{
    //
    delete report;      //收銀員帳表
    delete x,tif_ecr;
    delete tif_tmp;
    writelog("★★ 收銀員帳表 結束");
}

XREPORT::XREPORT()
{
    //
    writelog("讀帳 開始");
    report = new REPORT();      //讀帳帳表
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
    tif_tmp = new TIniFile(ZDATATMP_INI);
    tif_ecr = new TIniFile(XZDATA_INI);
    report = new REPORT();      //日結帳表

    

}

ZDATA::~ZDATA()
{
    delete z, tif_ecr;

    delete report;      //日結、部門帳表
    delete tif_tmp;

    writelog("Z帳 結束");

}

void __fastcall ZDATA::mtn_dat_folder() //維護DAT下傳檔30日內
{
    String Dir;
    String FileName;
    TSearchRec SearchRec;
    int iAttributes = 0;
    iAttributes |= faAnyFile * true;

    AnsiString str_dat_path = "C:\\Abacus\\download\\dat\\";
    Dir = str_dat_path;

    TStringList *TempList = new TStringList;
    try
    {

        TempList->Clear();

        if (FindFirst(Dir+"*.tm", iAttributes, SearchRec) == 0)
        {
            do
            {
                if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
                {
                    TempList->Add(SearchRec.Name);
                }
            } while (FindNext(SearchRec) == 0);

            FindClose(SearchRec);
        }

        AnsiString str_dd_flag = FormatDateTime("yymmdd",Now()-30);
        AnsiString str_dd_file;

        for ( int i = 0; i <= (TempList->Count-1); i++)
        {
            FileName = TempList->Strings[i];

            if (FileName.Length() < 13)
            {
                TempList->Delete(i);
                i = 0;
                continue;
            }

            str_dd_file = FileName.SubString(3,6);

            if (str_dd_file.AnsiCompare(str_dd_flag) < 0)
            {
                FileName = (Dir + TempList->Strings[i]);
                DeleteFile(FileName);
                writelog("維護DAT刪除檔案 " + FileName);
            }
        }
    }
    __finally
    {
        delete TempList; // destroy the list object
    }

}

void __fastcall ZDATA::del_list_file(TStringList *list, AnsiString path)
{
    AnsiString str_sal_00;

    for (int i=0; i<list->Count; i++)
    {
        str_sal_00 = (path + list->Strings[i]);
        DeleteFile(str_sal_00);
        writelog("三十天目錄維護刪除檔案 " + str_sal_00);
    }

    list->Clear();
}

/////////////////////////////////////////////////////////////////////////////////////






SPCDATA::SPCDATA()
{
    //
    int i;
    writelog("重點商品列管帳表 開始");
    tif_ecr = new TIniFile(XZDATA_INI);
    //tif_tmp = new TIniFile("C:\\ABACUS\\XDT_TMP.INI");
    //report = new REPORT();      //收銀員帳表
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
    delete tif_ecr;
    //delete tif_tmp;
    writelog("重點商品列管帳表 結束");
}



//重點商品列管帳表
//iQrySw=1 ,查詢
int __fastcall SPCDATA::WriteSpcInqData(const AnsiString SalFileName, const AnsiString Version,
                                        const int iQrySw )
{


    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    if (!int_reslut)
        return 0;

    //TStringList  *Tm30Data = new TStringList;
    //TStringList  *tsl_Spcdata = new TStringList;

    //SPOS_XRPDT *x = new SPOS_XRPDT;         //X帳檔案結構

    AnsiString str_x_filename,              //XDATA檔名
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


    pzDate_date_time=tif_ecr->ReadString("Z","PZ_DATE", FormatDateTime("yyyymmddhhnnss",Now()) );
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零
    str_date_z = tif_ecr->ReadString("Z","DATEZ","");           //ddn
    //str_tran_cnt = tif_ecr->ReadString("ECR","TRAN_CNT","");    //目前交易序號
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("SPCDATA::ECR->WriteData 取的目前之交易序號:%s", str_tran_cnt);
    writelog( sLog);


    AnsiString str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());
/***
#ifdef TM_20
    str_date_time = tif_ecr->ReadString("Z","pz_date", gchar_date_time);
    if (str_date_time.Length() < 8)
           str_date_time = FormatDateTime("yyyymmddhhnnss",Now());
#else
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());
#endif
****/

    if (str_date_z.Length() < 3)    //DATAZ=日期+Z帳次數  ddn
        str_date_z = (str_date_time.SubString(7,2) + "1");

    AnsiString str_sal_last;        //LOG最後一筆
    str_sal_last = gpchar_sal_last;

    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last, "|", 7).SubString(6,5);
        //str_sal_last.SubString(41,5);

    str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
        //str_sal_last.SubString(36,5);


    str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","");                  //上次重點商品列管帳表累積次數
    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //目前重點商品列管帳表到第幾次
    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";


    str_pSpcno_seq = tif_ecr->ReadString("SPC","pSPCno_seq","");                //上次x帳累積次數
    str_npSpcno_seq.sprintf("%02d", (_StrToInt(str_pSpcno_seq,"pSPCno_seq") + 1)); //目前x次到第幾次

    str_pSPCDttm = tif_ecr->ReadString("SPC","pSPCDttm",pzDate_date_time);
    str_pSPC0010DataKey=tif_ecr->ReadString("SPC","pSPCData","");                //上次重點商品列管帳表最後一筆0010
    //str_pSPC0010DataKey:yyyymmddhhnnss|00ZZn99999

    if (_StrToInt(str_nxno_seq,"pSPCno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";

    //x->no_tm = gchar_ecr_no;                      //收銀機機號
    //x->no_casher = gchar_cashier;                 //收銀員號碼
    //if (x->no_casher.Length() < 5)
    //    x->no_casher = "00001";

    //日結序號
    //x->no_seq = str_nxno_seq;

    //AnsiString str_x_first;                      //X帳第一筆LOG(不代表為SAL第一筆)
    //str_x_first = gpchar_x_first;

    //x->dt_begin = tif_ecr->ReadString("X","login_dttm","").SubString(1,8); //開始日期
    //x->tm_begin = tif_ecr->ReadString("X","login_dttm","").SubString(9,6); //開始時間

    AnsiString str_invo_char;                    //發票英文字母
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "");

    if (str_invo_char.Length() < 2)
        str_invo_char = "XX";

    /***
    if (gbl_tot_haveline)
    {
        x->no_rcstart = invoice_start_end(gi_tot_line, true);    //發票開始號碼
        x->no_rcend = invoice_start_end(gi_tot_line, false);     //發票結束號碼
    }
    else    //無交易發票
    {
        //x->no_rcend   = str_invo_char + "00000000";              //發票開始號碼
        //x->no_rcstart = str_invo_char + "00000000";              //發票結束號碼
        x->no_rcend   = "0000000000";              //發票開始號碼
        x->no_rcstart = "0000000000";              //發票結束號碼
    }
    ***/

    AnsiString str_pzno_seq;
    str_pzno_seq = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數   換日歸1

    //20031230
    //結束時間目前時間為目前結帳時間

    str_ecr_no.sprintf("%02d", atoi(gchar_ecr_no) );              //機號
    str_tencode = gchar_tencode;   //店號
    ed_date_time = FormatDateTime("yyyymmddhhnnss",Now());

    AnsiString spc_a;
    /**
     spc_a.sprintf("1080|0189|%-6s|%2s|%14s|%5s|%10s|%2s|%-8s|%-2s|%14s|",
                                        gchar_tencode,
                                        str_ecr_no.c_str(),
                                        str_date_time.c_str(),
                                        str_nz_cnt.c_str(),       //日結序號
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_nxno_seq.c_str(),
                                        gchar_cashier,
                                        str_npSpcno_seq.c_str(),
                                        str_pSPCDttm.c_str()
                                     );

    **/
    spc_a.sprintf("1080|0189|%-6s|%2s|%14s|%5s|%10s|%2s|%10s|%14s|",
                                        gchar_tencode,
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
    AnsiString str_rec_type,
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

    str_rec_type = "1097";
    str_rec_len  = "0106";

    //str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入97交易序號"); //目前寫入97交易序號
    str_trans_cnt.sprintf("%05d", int_sum_transcnt);

    AnsiString str_sum_tran;       //此次收銀員帳交易序號
    //20031215
    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1));

    //xxx
    //tif_tmp->WriteString("SUM","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_trans_cnt;

    // 2007/07/17
    str_z_cnt.sprintf("%05d", _StrToInt(str_nxno_seq,"交班序號"));
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

    /************************
    //str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    //str_start_trans = str_x_first.SubString(36,10);                 //開始交易序號下次帳表開始序號

    //str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
    //str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF|";


    AnsiString sTmp, CasherRptFile;;    //交易序號前5碼
    //str_trans1_5 = str_sal_last.SubString(36,5);
   // CasherRptFile=ChangeFileExt(str_xdt_path,".Crp");

   // sTmp.sprintf("0041|0471|%6s|%2s|%14s|%10s|%5s|%8s",
   //                     str_tencode.c_str(),
   //                    str_ecr_no.c_str(),
   //                     str_sys_date.c_str(),
   //                     (str_trans1_5 + str_tran_cnt).c_str(),
   //                     str_nz_cnt.c_str(),
   //                     str_cashier_no.c_str()
   //                     );
    
    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%6s|%2s|%14s|%10s|%5s|%2s|00|0000000000|FFFFFFFF|FFFFFFFF|",
                                        "0001",
                                        "0089",
                                        str_tencode.c_str(),
                                        str_ecr_no.c_str(),
                                        str_sys_date.c_str(),
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_z_cnt.c_str(),
                                        "Z0");

    str_99 = str_01;
    str_99 = "0099" + str_99.Delete(1,4);


    AnsiString _str_tran_cnt;    //交易序號

    _str_tran_cnt = (str_trans1_5 + str_trans_cnt);

    //寫入97交易序號
    AnsiString str_97;
    str_97.sprintf("%4s|%4s|%6s|%2s|%14s|%10s|%5s|%8s|%10s|%8s|%10s|%9s\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        _str_tran_cnt.c_str(),
                        str_z_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );



    String str_sal;
    str_sal = str_01 + "\n" +  str_97 + str_99;
    ************************/



    AnsiString str_write_sal, str_write_rpt, stmp;

    if( tb_setgrpcount > 0)
      {
       if( iQrySw == 1)   // 查詢
         {
          str_write_sal = "c:\\abacus\\TmpSpc.sal";
          str_write_rpt = "c:\\abacus\\posdata\\TmpSpc.rpt";
          str_write_PrvRpt = "c:\\abacus\\posdata\\TmpPrvSpc.rpt";

          if (FileExists( str_write_sal))
                DeleteFile( str_write_sal);

          //if (FileExists( str_write_rpt))
          //      DeleteFile( str_write_rpt);

          if( WriteSPCSAL(spc_a,str_write_sal))
            {
             tif_ecr->WriteString("SPC","TMPSPfilename", str_write_sal);

             //if( WriteSPCReport(spc_a, str_write_rpt) )
             //    tif_ecr->WriteString("SPC","TMPSPRptname", str_write_rpt);
             //else
             //    tif_ecr->WriteString("SPC","TMPSPRptname", "");

            }
          else
            {
              tif_ecr->WriteString("SPC","TMPSPfilename", "");
              //tif_ecr->WriteString("SPC","TMPSPRptname", "");
            }

         }
       else
         {
           //寫入交易nn.SAL檔

          if (WriteSPCSAL(spc_a,SalFileName))
             {
                 tif_ecr->WriteString("ECR","TRAN_CNT", str_sum_tran);
                 str_write_sal = "C:\\FTLPOS\\XZDATA\\00000" + str_trans_cnt + ".sal";
                 str_write_rpt = "C:\\FTLPOS\\XZDATA\\TmpSpc.rpt";
                 str_write_PrvRpt = "C:\\FTLPOS\\XZDATA\\TmpPrvSpc.rpt";

                if (FileExists( str_write_sal))
                    DeleteFile( str_write_sal);

                if (FileExists( str_write_rpt))
                    DeleteFile( str_write_rpt);

                if( WriteSPCSAL(spc_a,str_write_sal))
                    {
                     tif_ecr->WriteString("SPC","SPC0080filename", str_write_sal);
                     if( WriteSPCReport(spc_a, str_write_rpt) )
                        {
                          tif_ecr->WriteString("SPC","TMPSPRptname", str_write_rpt);
                          if( CopyFile( str_write_rpt.c_str(), str_write_PrvRpt.c_str(), false ) )
                              tif_ecr->WriteString("SPC","TMPPRVSPRptname", str_write_PrvRpt);
                        }
                     else
                        {
                          tif_ecr->WriteString("SPC","TMPSPRptname", "");
                        }

                    }
                else
                   {
                    tif_ecr->WriteString("SPC","SPC0080filename", "");
                    tif_ecr->WriteString("SPC","TMPSPRptname", "");
                   }

                //xxx
               //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));
               //tif_tmp->WriteString("X","PXNO_SEQ",   str_nxno_seq);

               tif_ecr->WriteString("SPC","pSPCDttm",str_date_time);
               tif_ecr->WriteString("SPC","pSPCTran_cnt",str_date_z + str_ptran_cnt );
               tif_ecr->WriteString("SPC","pSPCno_seq",str_npSpcno_seq.Trim() );
               tif_ecr->WriteString("SPC","pSPCData",str_pSPC0010DataKey.Trim() );
               stmp.sprintf("WriteSpcInqData: 重點商品列管資料處裡完成, 設定 Ecr_no.ini->SPC->pSPCData=%s",
                             str_pSPC0010DataKey.c_str() );
               writelog(stmp);
            }
          else
            {
              tif_ecr->WriteString("SPC","SPC0080filename", "");
              tif_ecr->WriteString("SPC","TMPSPRptname", "");
              tb_setgrpcount=0;
            }

        } // end of else
    } // end of  if( tb_setgrpcount > 0)
   else
    {
       tif_ecr->WriteString("SPC","SPC0080filename", "");
       tif_ecr->WriteString("SPC","TMPSPfilename", "");
       tif_ecr->WriteString("SPC","TMPSPRptname", "");
       tb_setgrpcount=0;
       stmp.sprintf("WriteSpcInqData: 無重點商品列管資料 " );
       writelog(stmp);

    }

   //delete x,tif_ecr;

    return(tb_setgrpcount);
}




// 重點商品列管帳表
//int __fastcall BASIC::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey)

int __fastcall SPCDATA::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey )
{
    AnsiString sRType,  //退貨
               sType, stmp, sSpcType,s;
    AnsiString str_dept, tmpDttm_Tran, tmpStDttm_Tran, tmpEdDttm_Tran;

    int iDept, iDataCnt;

    iDept = iDataCnt = 0;      //部門別
    //數量、金額
    giA = giB = 0;

    tmpStDttm_Tran=PrvSPCDataKey;
    tmpDttm_Tran="";

    if (gbl_rec_haveline)
    {
        for (int i=StartLine; i<=TotalLine; i++)
        {
            s=gtsl_rec->Strings[i];

            //_StringSegment_EX(ss, "|", 7).SubString(6,5);

            //str_dept = gtsl_rec->Strings[i].SubString(119,2).Trim();  //部門
            //str_dept = gtsl_rec->Strings[i].SubString(124,2).Trim();    //判別列管群號
            str_dept = _StringSegment_EX(s, "|", 20).SubString(6,2);

            //tmpDttm_Tran=gtsl_rec->Strings[i].SubString(21,14).Trim()+"|";
            tmpDttm_Tran=_StringSegment_EX(s, "|", 5)+"|";

            //tmpDttm_Tran+=gtsl_rec->Strings[i].SubString(36,10).Trim()+"|";
            tmpDttm_Tran+=_StringSegment_EX(s, "|", 7)+"|";

            giA = giB = 0;


            if( tmpDttm_Tran <= PrvSPCDataKey )
              continue;

            if (str_dept.Length() > 0)
                iDept = _StrToInt(str_dept);
            else
                continue;

            if( iDept < 1 && iDept > 99 )
              {
                 stmp.sprintf("SumSPCData:1010->列管群號 Error (%s-%s-%d)",
                               tmpDttm_Tran, str_dept, iDept );
                 writelog(stmp);
                 continue;
              }

            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);
            //gtsl_rec->Strings[i].SubString(370,2);

            sType = _StringSegment_EX(s, "|", 8);
            //gtsl_rec->Strings[i].SubString(68,2);


            sSpcType = _StringSegment_EX(s, "|", 10).Trim();
            //gtsl_rec->Strings[i].SubString(224,1);
            if ( sType.Pos("A0") || sType.Pos("A1") )
               { ;; }
            else if ( sType.Pos("S0") || sType.Pos("S1") )
                    {
                        if( sSpcType == "2" || sSpcType == "02" )  //代售
                           { ;; }
                        else
                           { continue; }
                    }
            else
               { continue; }

            iDataCnt++;
            //正常銷售
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {
                    //sType = gtsl_rec->Strings[i].SubString(68,2);

                    if (sType.Pos("A0") || sType.Pos("S0") )        //部門銷售
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        //_StrToInt(gtsl_rec->Strings[i].SubString(142,4));


                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
                        //_StrToInt(gtsl_rec->Strings[i].SubString(159,8));
                    }
                    else if (sType.Pos("A1") || sType.Pos("S1"))   //銷售取消
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
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
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //前筆誤打、退貨
            {

                    //sType = gtsl_rec->Strings[i].SubString(68,2);

                    if (sType.Pos("A0") || sType.Pos("S0"))        //部門銷售
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
                    }
                    else if (sType.Pos("A1") || sType.Pos("S1"))   //銷售取消
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
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
    stmp.sprintf("SumSPCData:1010->重點商品列管帳表資料筆數(%d) ST_Dttm_Tran(%s)~~ED_Dttm_Tran(%s)",
                iDataCnt, tmpStDttm_Tran.c_str(), PrvSPCDataKey.c_str() );
    writelog(stmp);

    //PrvSPCDataKey=tmpDttm_Tran;
    return iDataCnt;
}






int __fastcall SPCDATA::WriteSPCSAL(AnsiString strHdata,  AnsiString SALFileName)
{
    FILE *f_write_sal;
    AnsiString tmpdata, str_01, str_99, tmp_str;
    int iCnt,i, cnt80, cnt81;

    iCnt=cnt80=cnt81=0;

    /******************************************/
    TStringList *tmpsl = new TStringList();
    tmpsl->Clear();
    tmp_str="c:\\FTLPOS\\XZDATA\\1081SpcTmp.txt";  // FTLPOS.exe 產生
    if (FileExists(tmp_str) )
       {
        tmpsl->LoadFromFile(tmp_str);
        tmpdata.sprintf("WriteSPCSAL: Load 列管商品銷售明細資料(%s) (1081筆數=%d) OK.",
                        tmp_str.c_str(),tmpsl->Count  );
        writelog(tmpdata);
        //DeleteFile(tmp_str);    doont delete this file
       }
     else
       {
        tmpdata.sprintf("WriteSPCSAL: 列管商品銷售明細資料(%s) not find.",
                        tmp_str.c_str(),tmpsl->Count  );
        writelog(tmpdata);
       }
    /****************************************/


    for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp == 0 && tb_setgrp[i].MustPrinted==0)
         if(tb_setgrp[i].qt_setgrp == 0 )   // 2009/10/02
           continue;

         iCnt++;
       }

    if( (tb_setgrpcount <= 0 || iCnt <=0) && tmpsl->Count==0)
       {
        writelog(" 無資料寫入SAL檔(1080-1081) " + SALFileName);
        return 0;
      }

    if ((f_write_sal = fopen(SALFileName.c_str(), "a")) == NULL)
    {
        writelog("寫入SAL檔(1080-1081)發生錯誤 " + SALFileName);
        delete tmpsl;
        return 0;
    }


    // sprintf(sal_filename, "c:\\abacus\\sales\\00000%05ld.SAL",tran_cnt);
    // sprintf(sal_filename, "%s%s%3s%05ld.SAL",ABACUS_SALES,gchr_ecr_no,zini_datez,tran_cnt);
    // P_ecrno_sal_file=fopen(sal_filename, "w");



    /**
    tmpdata.sprintf("0080|0177|%-6s|%2s|%14s|%10s|%5s|%2s|%-8s|%-10s|%14s|",
                                        gchar_tencode,
                                        gchar_ecr_no,
                                        str_date_time.c_str(),
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_nz_cnt.c_str(),       //日結序號
                                        str_nxno_seq.c_str(),
                                        gchar_cashier,
                                        " ",
                                        str_pSPCDttm.c_str()
                                     );
   ***/



    str_01.sprintf("%4s|%4s|%9s|%31s|%2s|00|0000000000|FFFFFFFF|FFFFFFFF|",
                                        "0001",
                                        "0089",
                                        strHdata.SubString(11,9).c_str(),
                                        strHdata.SubString(21,31).c_str(),
                                        "Z0");

   fprintf(f_write_sal,"%s\n",str_01.c_str());

   /*********************1080 OutPut*****************************/
   for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp==0 && tb_setgrp[i].MustPrinted==0 )
         if(tb_setgrp[i].qt_setgrp==0 )  // 2009/10/02
           continue;

         cnt80++;
         tmpdata.sprintf("%s%-2s|%-30s|%-30s|%08ld|%08.0f|%8s|%8s|",
                           strHdata.c_str(),
                           tb_setgrp[i].no_setgrp.c_str(),
                           tb_setgrp[i].nm_setgrp.c_str(),
                           tb_setgrp[i].no_setsubgrp.c_str(),
                           tb_setgrp[i].qt_setgrp,
                           tb_setgrp[i].am_setgrp,
                           "FFFFFFFF",
                           "FFFFFFFF"  );
         fprintf(f_write_sal,"%s\n",tmpdata.c_str());
       }

   /*********************0081 OutPut*****************************/
   if( tmpsl->Count > 0)
     {
       for (int i=0; i<tmpsl->Count; i++)
           {
             cnt81++;
             tmpdata = tmpsl->Strings[i];
             fprintf(f_write_sal,"%s\n",tmpdata.c_str());
            }
     }
   delete tmpsl;
   /***********************************************************/

   str_99 = str_01;
   str_99 = "1099" + str_99.Delete(1,4);
   fprintf(f_write_sal,"%s\n",str_99.c_str());
   //tif_tmp->WriteString("X","x0097filename", str_write_sal);


   fclose(f_write_sal);

   tmpdata.sprintf("WriteSPCSAL: 列管商品銷售統計(%s) 0080筆數=%d, 0081筆數=%d OK. ",
                                 SALFileName.c_str(), cnt80, cnt81 );
   writelog(tmpdata);

   return iCnt;
}




int __fastcall SPCDATA::Get30TMData()
{

   FILE *plu_file;
   AnsiString sQryFileName,stmp,sData;
   int LineLength, igQryStatus, tot_cnt, itmpPrice;
   char tmp_buf[256],data_buffer[513],c;
   int handle, tmp_no_setgrp;

   LineLength=0;
   igQryStatus=0;
   tot_cnt=0;

  sQryFileName.sprintf("c:\\abacus\\posdata\\30.tm");

  if((handle=open(sQryFileName.c_str(), O_RDONLY )) == -1)
     {
      sprintf( tmp_buf,"Get30TMData: %s 資料有誤. (file not find)",sQryFileName);
      writelog(AnsiString(tmp_buf));
      return(0);
     }
  else
     {
      LineLength = filelength(handle);
      close(handle);
     }



  if( LineLength < 80)
     {
      sprintf( tmp_buf,"Get30TMData: CheckLength %s 資料有誤. DATA Length < %d",sQryFileName.c_str(), 80);
      writelog(AnsiString(tmp_buf));
      return(0);
     }

  if ((plu_file=fopen(sQryFileName.c_str(), "r")) == NULL)
       {
        sprintf( tmp_buf,"Get30TMData: %s 資料有誤. (fopen error)",sQryFileName.c_str());
        writelog(AnsiString(tmp_buf));
        return(0);
       }


  while( !feof(plu_file) )
      {
        memset( data_buffer,'\0',sizeof(data_buffer));
        c=0;

        scan_fixstr(plu_file, data_buffer, 78);
        data_buffer[78] = 0;
        stmp=AnsiString(data_buffer);

        fscanf(plu_file,"%c",&c );

        tot_cnt++;
        if( c != 0x0d && c != 0x0a)
          {
            sprintf( tmp_buf,"Get30TMData: %s 資料0x0d0x0a有誤. (行數=%ld)",sQryFileName.c_str(),tot_cnt);
            writelog(AnsiString(tmp_buf));
            //Post=false;
            igQryStatus=0;
            break;
            }

        //sData="|";
          tmp_no_setgrp=_StrToInt(stmp.SubString(1,2));
          if( tmp_no_setgrp > 0 &&  tmp_no_setgrp < 100)
            {
               tb_30TMgrp[tmp_no_setgrp].no_setgrp=stmp.SubString(1,2).Trim();
               tb_30TMgrp[tmp_no_setgrp].nm_setgrp=stmp.SubString(3,30).Trim();

               itmpPrice=_StrToInt(stmp.SubString(33,8));
               tb_30TMgrp[tmp_no_setgrp].amt_minpric=itmpPrice;

               itmpPrice=_StrToInt(stmp.SubString(41,8));
               tb_30TMgrp[tmp_no_setgrp].amt_maxpric=itmpPrice;

               tb_30TMgrp[tmp_no_setgrp].no_setsubgrp=stmp.SubString(49,30);

              //sData+=(stmp.SubString(1,2)+"|"+stmp.SubString(3,30)+"|"+stmp.SubString(33,8)+"|"+stmp.SubString(41,8)+"|");
             //sData+=(stmp.SubString(49,3)+" "+stmp.SubString(52,3)+" "+stmp.SubString(55,3)+" "+stmp.SubString(58,3)+" "+stmp.SubString(61,3)+" ");
             //sData+=(stmp.SubString(64,3)+" "+stmp.SubString(67,3)+" "+stmp.SubString(70,3)+" "+stmp.SubString(73,3)+" "+stmp.SubString(76,3)+" |FFFFFFFF|");
             //ls->Add(sData);
            }
          else
            {
              sprintf( tmp_buf,"Get30TMData: NO_SETGRP 資料有誤. (%s)",stmp.c_str() );
              writelog(AnsiString(tmp_buf));
            }

        fscanf(plu_file,"\n");
   }  // end of while

  fclose(plu_file);

  sprintf( tmp_buf,"Get30TMData: %s (總筆數=%ld)", sQryFileName.c_str(), tot_cnt );
  writelog(AnsiString(tmp_buf));

  return(tot_cnt);
}



int __fastcall SPCDATA::WriteSPCReport(AnsiString strHdata, AnsiString RptSALFileName)
{

   TStringList *tslCrp, *tsltmp;
   tslCrp = new TStringList;
   tsltmp = new TStringList;

   AnsiString nt,NowTime,PrvTime,  sStoreNo, sStoreName, stmp, strtmp, strQt;
   int i;

   tslCrp->Clear();
   tsltmp->Clear();
   for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp==0 && tb_setgrp[i].MustPrinted==0)
         if(tb_setgrp[i].qt_setgrp==0 )     // 2009/10/02
           continue;

         if( tb_setgrp[i].qt_setgrp > 99999 )
            strQt="?????";
         else
            strQt.sprintf("%5ld",tb_setgrp[i].qt_setgrp);

         stmp.sprintf(" %-2s  %-20s %5s %8.0f",
                           tb_setgrp[i].no_setgrp.c_str(),
                           tb_setgrp[i].nm_setgrp.SubString(1,20).Trim().c_str(),
                           strQt.c_str(),
                           tb_setgrp[i].am_setgrp
                            );
         tsltmp->Add(stmp);
       }

   if( tsltmp->Count <= 0 )
     {
      delete tslCrp;
      delete tsltmp;
      return(0);
     }


   nt=strHdata.SubString(21,16);
   //NowTime=nt.SubString(3,2)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   NowTime=nt.SubString(1,4)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   nt=strHdata.SubString(68,16);
   //PrvTime=nt.SubString(3,2)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   PrvTime=nt.SubString(1,4)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   read_ten_name(sStoreNo, sStoreName);

   strtmp.sprintf("           列管商品銷售統計表"); tslCrp->Add(strtmp);
   //strtmp.sprintf("%s  前次統計:%s",NowTime,PrvTime ); tslCrp->Add(strtmp);
   strtmp.sprintf("列管商品統計時間:%s",NowTime); tslCrp->Add(strtmp);
   strtmp.sprintf("前次列管統計時間:%s",PrvTime ); tslCrp->Add(strtmp);
   strtmp.sprintf("店號:%s    店名:%s  機號:%s",strHdata.SubString(11,6), sStoreName, strHdata.SubString(18,2) );
   tslCrp->Add(strtmp);

   //strtmp.sprintf("%s     機號:%s",NowTime,sTranCnt.SubString(18,2) ); tslCrp->Add(strtmp);

   strtmp.sprintf("收銀員:%s\n",strHdata.SubString(56,8));     tslCrp->Add(strtmp);
   strtmp.sprintf("群號 列管群名稱            數量     金額");   tslCrp->Add(strtmp);

   //tsltmp->Sort();
   for (int i=0; i<tsltmp->Count; i++)
    {
        stmp = tsltmp->Strings[i];
        tslCrp->Add(stmp);
    }

   strtmp.sprintf("----------------------------------------\n\n\n\n");  tslCrp->Add(strtmp);
   strtmp.sprintf("\x1b\x69");  tslCrp->Add(strtmp); //切紙

    //輸出檔案
   if (FileExists(RptSALFileName.c_str() ))
        DeleteFile(RptSALFileName.c_str());

   tslCrp->SaveToFile(RptSALFileName);
   //tslCrp->Clear();
   i=tsltmp->Count;
   delete tslCrp;
   delete tsltmp;

   stmp.sprintf("WriteSPCReport: 列管商品銷售統計表 OK. (%s)",RptSALFileName.c_str() );
   writelog(stmp);

   return(i);

}




//盤點人員讀帳   call by _WVXZ
int __fastcall VXZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version)
{

    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //載入SAL檔，將檔案依型態分類

    AnsiString sLog, sTmp;
    sLog.sprintf("盤點人員讀帳 VXZDATA::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    if (!int_reslut)
        return 0;

    AnsiString str_ver_num, str_tran_cnt;

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );
    str_ver_num = get_version(Version);

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z帳結構
    AnsiString str_z_filename;              //Z帳輸出檔名

    //目前Z帳時間
    AnsiString str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               str_vxpzno_seq;              //盤點人員讀帳次數


    AnsiString str_date_time, str_datez,str_cd_satncode,str_cs_entry;               //目前帳表系統時間
    str_date_time = gchar_date_time;


    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須減1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );


    str_z_date  = str_date_time;                                 //現在時間

    //str_pz_date = tif_ecr->ReadString("VXZ","PZ_DATE","");       //前次時間
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");       //前次時間  2013/08/26 Update
    
    str_datez = tif_ecr->ReadString("Z","DATEZ","");           //DDZ
    str_cd_satncode=tif_ecr->ReadString("VXZ","CD_SATNCODE","");
    str_cs_entry=tif_ecr->ReadString("VXZ","CS_ENTRY","");


    sLog.sprintf("VXZDATA:ECR->WriteData 取的目前之交易序號:%s, CD_SATNCODE=%s, CS_ENTRY=%s",
                  str_tran_cnt.c_str(), str_cd_satncode.c_str(), str_cs_entry.c_str() );
    writelog( sLog);



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


        AnsiString str_xpath;               //指定收銀員帳表目錄
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date;
    }
    ****************************/

    str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數 01~09  換日歸1
    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999
    str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零
    str_vxpzno_seq = tif_ecr->ReadString("VXZ", "NZNO_SEQ", "1");


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.Length() < 5)
        str_nz_cnt = "00001";

    //sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_vxpzno_seq));
    sYYMMDDZZ.sprintf("%02d",_StrToInt(str_vxpzno_seq));
    
#ifdef TM_20

    str_z_filename.sprintf("%s%02d%s%02d%02d.vdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        (str_pz_date.SubString(3,6)).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        _StrToInt(str_vxpzno_seq)              );
#else
    //輸出檔案 yyyymmddnn.zdt ex:2002010101.vdt
    //str_z_filename = (str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt");

    str_z_filename.sprintf("%s%02d%s%02d%02d.vdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        (str_pz_date.SubString(3,6)).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        _StrToInt(str_vxpzno_seq)              );



#endif

    //換日Flag
    bool bl_change_day;

    //判斷換日
    if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
        bl_change_day = true;
    else
        bl_change_day = false;

    z->no_tm = gchar_ecr_no;             //收銀機機號

    //日結序號
    z->bg_noclose.sprintf("%05d", _StrToInt(str_nz_cnt,"日結序號"));


    z->bg_dtclose = str_z_date.SubString(1,8);      //結帳日期
    z->bg_tmclose = str_z_date.SubString(9,6);      //結帳時間
    z->ed_dtclose = str_pz_date.SubString(1,8);     //前次結帳日期
    z->ed_tmclose = str_pz_date.SubString(9,6);     //前次結帳時間


    AnsiString z_a;
    int iC0TotCount=0;
    z_a.sprintf("1092|%04d|%-6s|%-2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|", 1593,
                                      gchar_tencode,
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

    //本日來客數
    z->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    z->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;

    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    TotCount(0,gi_tot_line);
    z->tb_trans.am_trcpntx.sprintf("%08d",giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );       // 發票日結帳額(免)
    z->tb_trans.am_trcptax.sprintf("%08d",giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));       // 發票日結帳額(稅)
    z->tb_trans.am_trcptot.sprintf("%08d",giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));       // 發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    //本日結帳累計金額
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));
          //int_acc_tot_amt=_StrToInt(str_am_tclsacc,"本日結帳金額累計");


    z->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    TotMiss(0,gi_tot_line);
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
    z->tb_trans.qt_miss.sprintf("%08d",   giA);       //  誤打發票次數
    z->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  誤打發票金額(免)
    z->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  誤打發票金額(稅)
    z->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );       //  誤打發票金額
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
    z->tb_trans.am_totsale.sprintf("%08d", int_totsale );


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


    AnsiString z_b;

    z_b += (z->tb_trans.qt_tcust+"|");             //本日來客數
    z_b += (z->tb_trans.qt_trans+"|");             //本日交易品項
    z_b += (z->tb_trans.am_tclsacc+"|");           //本日結帳累計金額
    z_b += (z->no_rcstart+"|");                   //發票開始號碼
    z_b += (z->no_rcend+"|");                      //發票結束號碼
    z_b += (z->tb_trans.am_trcpntx+"|");          //發票日結帳額(免)
    z_b += (z->tb_trans.am_trcptax+"|");           //發票日結帳額(稅)
    z_b += (z->tb_trans.am_trcptot+"|");          //發票日結帳金額
    z_b += (z->tb_trans.qt_miss+"|");              //誤打發票次數
    z_b += (z->tb_trans.am_missntx+"|");           //誤打發票金額(免)
    z_b += (z->tb_trans.am_misstax+"|");           //誤打發票金額(稅)
    z_b += (z->tb_trans.am_miss+"|");              //誤打發票金額
    z_b += (z->tb_trans.am_totsale+"|");           //本日實際銷貨收入


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

    z->tb_newadd.qt_agnrcv.sprintf("%08d",giD);             //代收費用次數
    z->tb_newadd.am_agnrcv.sprintf("%08d",giC);             //代收費用金額


    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    AnsiString z_c;
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

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
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

    z_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
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

    z->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);            //禮券回數
    z->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //禮券金額


    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

    z->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //廠商代現金回數 目前不使用
    z->tb_cash.am_vndcpn.sprintf("%08d", 0);             //廠商代現金金額

    z->tb_cash.qt_stocpn.sprintf("%08d", giC);           //門市代現金回數
    z->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //門市代現金金額
    SumCpn41 +=  giD;         //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

    z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //代現金合計回數
    z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //代現金合計回數
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //代現金合計金額

    z->tb_cash.qt_bonout.sprintf("%08d", giG);           //溢收合計回數
    z->tb_cash.am_bonout.sprintf("%08d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //餘額券
    iqt_ffu1=giI;                                        //現金卡回數  電子錢 1847
    iam_ffu1=giJ;                                        //現金卡金額  電子錢 1855    刷卡合計

    AnsiString z_d;

    z_d += (z->tb_cash.qt_bonus+"|");                //禮券回數
    z_d += (z->tb_cash.am_bonus+"|");                //禮券金額
    z_d += (z->tb_cash.qt_vndcpn+"|");               //廠商折價券回數
    z_d += (z->tb_cash.am_vndcpn+"|");               //廠商折價券金額
    z_d += (z->tb_cash.qt_stocpn+"|");               //折價券回數
    z_d += (z->tb_cash.am_stocpn+"|");               //折價券金額
    z_d += (z->tb_cash.qt_totcash+"|");              //代現金合計回數
    z_d += (z->tb_cash.am_totcash+"|");              //代現金合計金額

    sTmp.sprintf("%08d",giF);
    z_d += (sTmp+"|");                               //餘額合計回數
    sTmp.sprintf("%08d",giE);
    z_d += (sTmp+"|");                               //餘額合計金額

    z_d += (z->tb_cash.qt_bonout+"|");               //溢收合計回數
    z_d += (z->tb_cash.am_bonout+"|");               //溢收合計金額

    //==========================================================================


    //現金合計 含 消費券

    SumCash41=SumCash(0,gi_tot_line);
    z->am_cash.sprintf("%08d",SumCash41-iTotOt2Rtn+iCSMAmt );

    // writelog("現金合計 含 消費券");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //現金合計
    //strcat(ze,z->no_rcstart.c_str());      //發票開始號碼
    //strcat(ze,z->no_rcend.c_str());        //發票結束號碼
    //ze[28] = 0;


    // Z帳部門別集計TABLE      =============================================

    AnsiString z_f, z_a1093, z_ammdis;

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
            z->tb_depcal[i].am_dpsitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpsale.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmiss.sprintf("%08d",0);

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


       // 部門促銷折扣金額        Lu 2018/06/13
        z_a1093.sprintf("1093|%04d|%-6s|%-2s|%14s|%5s|%-10s|", 275,
                                      gchar_tencode,
                                      z->no_tm.c_str(),           //收銀機機號
                                      str_date_time.c_str(),      //現在時間
                                      z->bg_noclose.c_str(),      //日結序號
                                      sYYMMDDZZ.c_str()          //Z帳表序號
                                      );
        z_ammdis="";

        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            z->tb_depcal[i].am_dpsale.sprintf("%08d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            z->tb_depcal[i].am_dpmitm.sprintf("%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            z->tb_depcal[i].am_dpmiss.sprintf("%08d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;

            // 部門促銷折扣金額        Lu 2018/06/13
            sTmp.sprintf("%08d", dept[i].am_mmDis - dept[i].am_mmDismiss);
            z_a1093 += (sTmp+"|");

            sTmp.sprintf("%08d|%08d", dept[i].am_mmDis , dept[i].am_mmDismiss);
            z_ammdis += (sTmp+"|");

        }

        // 部門促銷折扣金額        Lu 2018/06/13
        sTmp.sprintf("%40s|\n", " ");
        z_a1093 += (sTmp);
        z_ammdis += (sTmp);
    }
    __finally
    {
      delete [] dept;
    }

    //writelog("Z帳部門別集計TABLE OK");

    AnsiString z_g;

    z_g += (z->am_tdpsitm.sprintf("%08d", s1)+"|");             // 部門合計交易項數
    z_g += (z->am_dpstot.sprintf("%08d" , s2)+"|");             // 部門合計銷售金額
    z_g += (z->am_tdpmitm.sprintf("%08d", s3)+"|");             // 部門合計誤退項數
    z_g += (z->am_dpmtot.sprintf("%08d" , s4)+"|");             // 部門合計誤退金額
    z_g += (z->am_cash+"|");                                   // 現金合計含消費券 45


    //收銀員明細ｆｉｅｌｄ=============================================

    AnsiString z_h;

    //z_h += (z->am_cash+"|");                                    //現金合計
    //z_h += (z->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");    // 借零次數
    sTmp.sprintf("%08d",VisuilCpnAmt);
    z_h += (sTmp+"|");                                            //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // 借零合計金額
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);                                       //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    z_h += (sTmp+"|");                                            // 代售商品折讓金額  借零合計金額

    z_h += (z->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");    // 投零次數
    z_h += (z->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");    // 投零合計金額

    //投庫
    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;           //投庫次數
    int int_drop_money = 0;       //投庫金額
    int int_drop_sum = 0;    //投庫總額
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        //StringSegment_EX(s, "|", 15)
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生日結帳表:非投庫(%s)資料",s1041);
             writelog(sTmp );
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC付款
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //信用卡
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //提貨單

        //中獎發票金額  800   Cloud 中獎發票金額(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s1041, "|", 18) );

        //中獎發票金額  500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //中獎發票金額  1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //中獎發票金額  200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //費用 13金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //費用 14金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數
    }

    z_h += (z->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        // 投庫次數
    z_h += (z->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     // 投庫合計金額

    //writelog("投庫合計金額 OK");

    //更正次數、金額
    SumUpdat(0,gi_rec_line);
    z_h += (z->tb_casher.qt_update.sprintf("%08d", giA)+"|");          // 更正次數
    z_h += (z->tb_casher.am_update.sprintf("%08d", giB)+"|");          // 更正合計金額


    //交易取消
    SumVoid(0,gi_tot_line);
    z_h += (z->tb_casher.qt_void.sprintf("%08d", giA)+"|");            // 交易取消次數
    z_h += (z->tb_casher.am_void.sprintf("%08d", giB)+"|");            // 交易取消合計金額


    //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
    }
    __finally
    {
        //delete ini_ecr;
        ;;
    }


    z_h += (z->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // 訓練合計次數
    z_h += (z->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // 訓練合計金額

    //手動輸入部門13、14
    SumInpdp(0,gi_rec_line);
    z_h += (z->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");            //手動輸入部門次數
    z_h += (z->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");            //手動輸入部門金額

    z_h += (z->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");     // 開錢櫃次數

    //折扣讓次數   //小計折扣
    SumDisc(0,gi_pay_line);
    z_h += (z->tb_casher.qt_disc.sprintf("%08d", giA)+"|");             // 折扣次數(應稅)
    //z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");            // 折扣金額(應稅)
    z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP)+"|");            // 折扣金額(應稅),改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    z_h += (z->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");            //促銷折扣/讓次數
    z_h += (z->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");             //促銷折扣/讓金額

    //加值機
    easy_card(0,gi_easy_line);
    z_h += (z->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");          // 加值機開/售卡次數
    z_h += (z->tb_casher.am_disret1.sprintf("%08d", giB)+"|");         // 加值機開/售卡金額
    z_h += (z->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");          // 加值機加值次數
    z_h += (z->tb_casher.am_disret2.sprintf("%08d", giD)+"|");          // 加值機加值金額

    //fetc_card(0,gi_fetc_line);   //e通卡    2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //備用八金額  現金 加值金額   2015/08/19 代售,手續費作廢金額(免)
    z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|"); //備用八金額  現金 加值金額   2015/08/19 代售,手續費作廢金額(免)


    //食安次數、金額  int iqt_food,iamt_food;
    z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數   2015/08/19 0
    //z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額   2015/08/19 0


    //   05 追加 table        =============================================

    AnsiString z_i;

    z_i += (z->tb_newadd.qt_cashCard.sprintf("%08d", iqt_ffu1)+"|");    //刷卡/現金卡次數    電子錢 1847
    z_i += (z->tb_newadd.am_cashCard.sprintf("%08d", iam_ffu1)+"|");    //刷卡/現金卡金額    電子錢  刷卡合計 1855
    z_i += (z->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");            //代收停車費次數
    z_i += (z->tb_newadd.am_parking.sprintf("%08d", 0)+"|");            //代收停車費金額
    z_i += (z->tb_newadd.qt_agnrcv+"|");                                //代收費用次數  1831
    z_i += (z->tb_newadd.am_agnrcv+"|");                                //代收費用金額  1839
    z_i += (z->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");      //版本前置符號
    z_i += (z->tb_newadd.tx_ver.sprintf("%6s", str_ver_num)+"|");      //版本號碼
    z_i += (z->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  //發票金額0次數

    //不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);

    z_i += (z->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //不開發票金額
    z_i += (z->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");    //  不開發票金額誤退重

    SumLOSS(0,gi_rec_line);
    z_i += (z->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");             // TM廢棄Loss值次數
    z_i += (z->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");             // TM廢棄Loss值金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計
    z_i += (z->tb_newadd.am_ffu7.sprintf("%08d", iSub_C0InvoAmtA)+"|");             //原加值機加值取消金額

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    // z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", giB+iSub_C0InvoAmtC)+"|");             //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");             //原加值機加值取消次數

    //消費券
    z_i += (z->tb_newadd.am_posZRpt.sprintf("%08d", iCSMAmt)+"|");      //扣卡機扣卡金額   消費券

    AnsiString z_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);      即時購與銷轉進值應稅
    z_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //即時購與銷轉進值免稅
    z_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數  中獎發票張數
    z_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額  中獎發票金額
    z_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    z_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);  //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
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


    AnsiString DDZ_1099;
    DDZ_1099=str_datez;
    // 2013/08/26  Update
    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ; //DDZ_1099=str_datez;

    //記錄Z PATH於INI
    AnsiString str_zdt_path, str_zrp_path;


    //Z報表
    AnsiString str_zrp;

    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%8s%88s|\n",
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%79s|\n",  2012/09/18
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17    點卡手續費 Update
    str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str(),
                            "FFFFFFFF"
                            );

    sTmp.printf("VXZDATA::(%s) Len=%d",str_zrp, str_zrp.Length() );
    writelog(sTmp);
    //report = new REPORT();      //日結帳表

    //Z報表
    strcpy(report->char_filename, ChangeFileExt(str_z_filename,".vrp").c_str());

    if( _StrToInt(str_cs_entry)==1)
        report->CreateReport('V', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
   else
        report->CreateReport('v', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
    //備份Z報表
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //部門帳表
    AnsiString str_dpt_path;
    strcpy(report->char_filename, ChangeFileExt(str_z_filename,".vpt").c_str() );

    if( _StrToInt(str_cs_entry)==1)
         report->CreateReport('E', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,0,0,0,0,tsl_subsale, 0, z_ammdis);
    else
         report->CreateReport('e', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,0,0,0,0,tsl_subsale, 0, z_ammdis);
    //備份部門帳表
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    //寫入SAL檔98Type
    AnsiString str_rec_type,
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

    str_rec_type = "1015";
    str_rec_len = "0098";
    str_tencode = gchar_tencode;
    str_ecr_no.sprintf("%02d",_StrToInt(z->no_tm,"z->no_tm"));
    str_sys_date = (z->bg_dtclose + z->bg_tmclose);      //日期時間

    //98交易序號 //20031215
    int int_sum_tran_cnt ;

    //= tif_ecr->ReadInteger("ECR","TRAN_CNT",1);
    //str_tran_cnt.sprintf("%05d",int_sum_tran_cnt);

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    //if (str_tran_cnt.Length() < 5)
    //if( Trim(str_tran_cnt)=="00000")
    //if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
    //    str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    //sLog.sprintf("VXZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    //writelog( sLog);


    AnsiString str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
         str_sum_tran.sprintf("%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //前次日結序號，目前日結序號
    //str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "前次日結序號"));
    //str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "目前日結序號"));


    if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc;

    AnsiString str_sal_first;
    str_sal_first = gpchar_sal_first;

     //開始交易序號
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, "|", 7).SubString(TRANCNT_NO_POS,5));
    str_start_trans.sprintf("%05d", i_tran_cnt);

    int_am_tclsacc=int_acc_tot_amt;
    str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //本日結帳累計金額
    str_am_totsale = z->tb_trans.am_totsale;                //本日實際銷貨收入
    str_zdt_no.sprintf("%010ld",z->bg_noclose.ToInt());     //ZDT序號代入Z檔名
    str_ver = "FFFFFFFF|";


    AnsiString str_trans1_5;    //交易序號前五碼

    AnsiString str_sal_last;
    str_sal_last = gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    AnsiString str_01,str_99;

    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",   //1097
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
                   " ",
                   " ",
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );



    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
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
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );



    AnsiString _str_tran_cnt;        //交易序號
    AnsiString str_st_tran_cnt;     //開始交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    AnsiString str_98;  //寫入1015交易序號
    str_98.sprintf("%4s|%4s|%6s|%2s|%14s|%-5s|%-10s|%3s|%2s|%-8s|%1s|%-16s|%9s\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        "000",
                        "01",
                        SalesNo.c_str(),      //收銀員代碼
                        str_cs_entry.c_str(),            //1,2
                        str_cd_satncode.c_str(),
                        "        |"
                        );



    String str_sal;
    //str_sal = str_01 + "\n"+ str_zrp + str_98 + str_99;
    str_sal = str_01 + "\n"+ str_98+str_zrp + z_a1093 + str_99;    // z_a1093 部門促銷折扣金額        Lu 2018/06/13

    if (WriteSAL(str_sal, str_z_filename) == OK)
       {
        //備份ZDT帳
        str_zdt_path = BackupFile(str_z_filename,"", DDZ_1099);
        tif_tmp->WriteString("VXZ","x1092filename", str_zdt_path);
       }
    else
      {
       tif_tmp->WriteString("VXZ","x1092filename", "");
      }

    /////////////////////////////////////////////////////////////////

    AnsiString StrZslFile;
    if (WriteSAL_A(str_sal, SalFileName) == OK)
       {
         //備份ZSL帳
         StrZslFile = BackupFile(SalFileName, ChangeFileExt(SalFileName,".zzz"),DDZ_1099 ); // for test
         //tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);
         ;;
       }
    else
       {
         //tif_tmp->WriteString("Z","ZSLNAME","");
         ;;
       }


    AnsiString str_ipt_path;

    //更改XZDATA.INI
    int int_vxnzno_seq   = _StrToInt(str_vxpzno_seq, "vxpzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp->WriteString("VXZ","ZFILENAME" , str_zdt_path);
    tif_ecr->WriteString("VXZ","ZFILENAME" , str_zdt_path);

    tif_tmp->WriteString("VXZ","ZRPTNAME"  , str_zrp_path);
    tif_ecr->WriteString("VXZ","ZRPTNAME"  , str_zrp_path);

    tif_tmp->WriteString("VXZ","DEPTNAME"  , str_dpt_path);
    tif_ecr->WriteString("VXZ","DEPTNAME"  , str_dpt_path);

    tif_tmp->WriteString("VXZ","PZ_DATE"     , str_z_date);
    tif_ecr->WriteString("VXZ","PZ_DATE"     , str_z_date);

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
        tif_ecr->WriteString("VXZ","PZNO_SEQ", str_vxpzno_seq);

        if (++int_vxnzno_seq > 99)
            int_vxnzno_seq = 1;

        str_vxpzno_seq.sprintf("%d", int_vxnzno_seq);

        tif_ecr->WriteString("VXZ","NZNO_SEQ", str_vxpzno_seq);
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


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    //指定X 帳PATH
    //AnsiString str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    //tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印

    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_tran_cnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);
    tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    sLog.sprintf("VXZDATA::寫入交易序號 ECR->TRAN_CNT=%s", str_sum_tran );
    writelog( sLog);



    return OK;
}



VXZDATA::VXZDATA()
{
    //
    writelog("盤點人員讀帳 開始");
    z = new SPOS_ZRPDT;         //Z帳結構
    tif_tmp = new TIniFile(ZDATATMP_INI);
    tif_ecr = new TIniFile(XZDATA_INI);
    report = new REPORT();      //日結帳表

}

VXZDATA::~VXZDATA()
{
    delete z, tif_ecr;

    delete report;      //日結、部門帳表
    delete tif_tmp;
    //mtn_dat_folder();   //維護DAT目錄，保存三十日以內資料
 
}


///////////////////////////////////////////////////////////////////////////////
//自動日結Section


AnsiString  __fastcall AUTOZDATA::GetSysDttmMaxTransNo(AnsiString sDttm)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    AnsiString sTmp, sRec, sTmpDttm, RecNo, tmpTrans, rtns;

 try
   {
      rtns="";
      if( Auto_Sal->Count > 0)
       {
        for (int i=0;  i<Auto_Sal->Count; i++)
            {
             sRec=Auto_Sal->Strings[i];
             RecNo=_StringSegment_EX(sRec, "|", 1);
             if( RecNo!="1001")
                 continue;
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             tmpTrans=_StringSegment_EX(sRec, "|", 7);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( sDttm != "" && sTmpDttm.SubString(1,8) == sDttm )
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




int __fastcall AUTOZDATA::GetYYYYMDD_SalData(AnsiString &AutoZCnt, AnsiString AutoZdt , int AutoZ)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    AnsiString sTmp, sRec, sTmpDttm,TmpZcnt, SalZcnt, RecNo;
    int irtn=0;
    TStringList *sTmlLst = new TStringList();
    SalZcnt=AutoZCnt;

 try
   {
    if( AutoZ ==1)
      {
       sTmlLst->Assign(Auto_Sal);
       if( sTmlLst->Count > 0)
       {
        Auto_Sal->Clear();
        for (int i=0;  i<sTmlLst->Count; i++)
            {
             sRec=sTmlLst->Strings[i];
             RecNo=_StringSegment_EX(sRec, "|", 1);
             if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
               TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //日結序號
             else
               TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號

             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt )
                  {
                    if( RecNo=="1001" )   //取得 1001 中最大日結序號
                      {
                        SalZcnt=TmpZcnt;
                        AutoZCnt=SalZcnt;
                      }
                  }
             // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt )
               {
                 Auto_Sal->Add(sRec);
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
    gtsl_rec->Clear();
    //gtsl_rec->Assign(Autotsl_rec);
    if( Autotsl_rec->Count > 0)
       {
        for (int i=0;  i<Autotsl_rec->Count; i++)
            {
             sRec=Autotsl_rec->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 gtsl_rec->Add(sRec);
               }
             else
               {
                 ;;//gtsl_rec->Delete(i);
               }


            }
       }
    irtn+=1;

    gtsl_tot->Clear();
    //gtsl_tot->Assign(Autotsl_tot);
    if( Autotsl_tot->Count > 0)
       {

        for (int i=0;  i<Autotsl_tot->Count; i++)
            {
             sRec=Autotsl_tot->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                    //AutoZCnt=SalZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 gtsl_tot->Add(sRec);
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

    gtsl_drop->Clear();
    //gtsl_drop->Assign(Autotsl_drop);
    if( Autotsl_drop->Count > 0)
       {

        for (int i=0;  i<Autotsl_drop->Count; i++)
            {
             sRec=Autotsl_drop->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 gtsl_drop->Add(sRec);
               }
             else
               {
                 ;;//gtsl_drop->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_easy_card->Clear();
    //gtsl_easy_card->Assign(Autotsl_easy_card);
    if( Autotsl_easy_card->Count > 0)
       {

        for (int i=0;  i<Autotsl_easy_card->Count; i++)
            {
             sRec=Autotsl_easy_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt )
               {
                 gtsl_easy_card->Add(sRec);
               }
             else
               {
                 ;;//gtsl_easy_card->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_pay_sub->Clear();
    //gtsl_pay_sub->Assign(Autotsl_pay_sub);
    if( Autotsl_pay_sub->Count > 0)
       {

        for (int i=0;  i<Autotsl_pay_sub->Count; i++)
            {
             sRec=Autotsl_pay_sub->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 gtsl_pay_sub->Add(sRec);
               }
             else
               {
                 ;; //gtsl_pay_sub->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_bil->Clear();
    //gtsl_bil->Assign(Autotsl_bil);
    if( Autotsl_bil->Count > 0)
       {

        for (int i=0;  i<Autotsl_bil->Count; i++)
            {
             sRec=Autotsl_bil->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                gtsl_bil->Add(sRec);
               }
             else
               {
                 ;;//gtsl_bil->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_fetc_card->Clear();
    //gtsl_fetc_card->Assign(Autotsl_fetc_card);
    if( Autotsl_fetc_card->Count > 0)
       {

        for (int i=0;  i<Autotsl_fetc_card->Count; i++)
            {
             sRec=Autotsl_fetc_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 gtsl_fetc_card->Add(sRec);
               }
             else
               {
                 ;;//gtsl_fetc_card->Delete(i);
               }
            }
       }
    irtn+=1;

    tsl_subsale->Clear();
    //tsl_subsale->Assign(Autotsl_subsale);
    if( Autotsl_subsale->Count > 0)
       {

        for (int i=0;  i<Autotsl_subsale->Count; i++)
            {
             sRec=Autotsl_subsale->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM自動日結] 當前一日日結序號=99999,換日後無法執行自動日結..待 Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 tsl_subsale->Add(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;

       //////////////////////////////////////////////////////////////////
       gtsl_Ticket->Clear();

      if( Autotsl_Ticket->Count > 0)
       {
            //  if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
            //   TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //日結序號
        for (int i=0;  i<Autotsl_subsale->Count; i++)
            {
             sRec=Autotsl_Ticket->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }

             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
               {
                 gtsl_Ticket->Add(sRec);
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




int __fastcall AUTOZDATA::GetZDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo ,
                          String Version, String PZ_date, String Z_date, String DDZ_1099, String ZDT_1050,
                          String &RtnZDTData, AnsiString &Tclsacc,
                          String &str_zrp_path, String &str_dpt_path)
{
    int int_reslut = 0;
    AnsiString sLog, sTmp,str_ver_num, str_tran_cnt, ZDT_1050_DDZ;

    AnsiString str_z_date,                  //現在時間
               str_pz_date,                 //前次時間
               str_pzno_seq,                //目前Z帳次數   換日歸1
               str_pz_cnt,                  //前次日結序號  換日不歸零
               str_nz_cnt,                  //目前日結序號  換日不歸零
               str_am_tclsacc,              //累積Z帳金額   換日不歸零
               sYYMMDDZZ,                   //Z帳表序號
               str_date_time, str_datez,    //目前帳表系統時間
               z_a;

    sTmp=_StringSegment_EX(ZDT_1050, "|", 7);  //交易序號
    ZDT_1050_DDZ=sTmp.SubString(3,3);
    str_am_tclsacc=Tclsacc;
    str_z_date=Z_date;
    str_pz_date=PZ_date;
    str_ver_num=Version ;
    //sYYMMDDZZ.sprintf("%s%s", str_pz_date.SubString(3,6).c_str(),StrZCnt );
    sYYMMDDZZ.sprintf("%s%s", str_z_date.SubString(5,4).c_str(),StrZCnt );


    sTmp.printf("** AUTOZDATA::GetZDTData 產生日結帳表 Version=%s, ZDT:StrZCnt=%s, PZ_date=%s, Z_date=%s, DDZ_1099=%s, Tclsacc=%s **",
                Version, StrZCnt, PZ_date, Z_date, DDZ_1099, Tclsacc  );
    writelog(sTmp );

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



    z->no_tm = gchar_ecr_no;             //收銀機機號

    //日結序號
    z->bg_noclose.sprintf("%05d", _StrToInt(StrZCnt,"日結序號"));


    AnsiString ssysdttm, s1050sysdttm;
    int iC0TotCount=0;

    ssysdttm=gchar_date_time;
    s1050sysdttm=_StringSegment_EX(ZDT_1050, "|", 5);

    if( s1050sysdttm.SubString(1,8) < ssysdttm.SubString(1,8) )
      {
        z->bg_dtclose = s1050sysdttm.SubString(1,8);      //結帳日期
        z->bg_tmclose = "235959";                         //結帳時間
      }
    else
      {
        //z->bg_dtclose = str_z_date.SubString(1,8);      //結帳日期   取的目前最大1050之交易
        //z->bg_tmclose = str_z_date.SubString(9,6);      //結帳時間

        z->bg_dtclose = s1050sysdttm.SubString(1,8);      //結帳日期  1090 日期
        z->bg_tmclose = "235959";                         //結帳時間

      }

    sTmp.printf("AUTOZDATA::GetZDTData: s1050sysdttm=%s, ssysdttm=%s, Set 1096 日結序號=%s, 結帳日期 z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
    writelog(sTmp );

    z->ed_dtclose = str_pz_date.SubString(1,8);     //前次結帳日期
    z->ed_tmclose = str_pz_date.SubString(9,6);     //前次結帳時間


    z_a.sprintf("1096|%04d|%-6s|%-2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|",
                                      1593,
                                      gchar_tencode,
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

    //本日來客數
    z->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    z->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt, C01TaxAmt, C01FTaxAmt, RC01TaxAmt, RC01FTaxAmt, R4_C01FTaxAmt, R4_C01TaxAmt;
    
    // 點卡手續費應免稅
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    TotCount(0,gi_tot_line);
    z->tb_trans.am_trcpntx.sprintf("%08d",giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );       // 發票日結帳額(免)
    z->tb_trans.am_trcptax.sprintf("%08d",giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));       // 發票日結帳額(稅)
    z->tb_trans.am_trcptot.sprintf("%08d",giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));       // 發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );


    sTmp.printf("ZDATA::GetZDTData: 發票結帳累計金額=%s, 發票日結帳金額=%d ", str_am_tclsacc.c_str(), int_tot_amt  );
    writelog(sTmp );

      //本日結帳累計金額
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) > 99999999||(int_tot_amt+_StrToInt(str_am_tclsacc,"本日結帳金額累計")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計"));

    Tclsacc=IntToStr(int_acc_tot_amt);

    z->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"本日結帳金額累計")));

    TotMiss(0,gi_tot_line);
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
    z->tb_trans.qt_miss.sprintf("%08d",   giA);       //  誤打發票次數
    z->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt- R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  誤打發票金額(免)
    z->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  誤打發票金額(稅)
    z->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );       //  誤打發票金額
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
    z->tb_trans.am_totsale.sprintf("%08d", int_totsale );


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


    AnsiString z_b;

    z_b += (z->tb_trans.qt_tcust+"|");             //本日來客數
    z_b += (z->tb_trans.qt_trans+"|");             //本日交易品項
    z_b += (z->tb_trans.am_tclsacc+"|");           //本日結帳累計金額
    z_b += (z->no_rcstart+"|");                   //發票開始號碼
    z_b += (z->no_rcend+"|");                      //發票結束號碼
    z_b += (z->tb_trans.am_trcpntx+"|");          //發票日結帳額(免)
    z_b += (z->tb_trans.am_trcptax+"|");           //發票日結帳額(稅)
    z_b += (z->tb_trans.am_trcptot+"|");          //發票日結帳金額
    z_b += (z->tb_trans.qt_miss+"|");              //誤打發票次數
    z_b += (z->tb_trans.am_missntx+"|");           //誤打發票金額(免)
    z_b += (z->tb_trans.am_misstax+"|");           //誤打發票金額(稅)
    z_b += (z->tb_trans.am_miss+"|");              //誤打發票金額
    z_b += (z->tb_trans.am_totsale+"|");           //本日實際銷貨收入

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

    z->tb_newadd.qt_agnrcv.sprintf("%08d",giD);             //代收費用次數
    z->tb_newadd.am_agnrcv.sprintf("%08d",giC);             //代收費用金額

    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    AnsiString z_c;
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

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
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

    z_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
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

    z->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);            //禮券回數
    z->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //禮券金額


    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

    z->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //廠商代現金回數 目前不使用
    z->tb_cash.am_vndcpn.sprintf("%08d", 0);             //廠商代現金金額

    z->tb_cash.qt_stocpn.sprintf("%08d", giC);           //門市代現金回數
    z->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //門市代現金金額
    SumCpn41+=giD;    //折價券+Other1+有價票券 CPN


    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

    z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //代現金合計回數
    z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //代現金合計回數
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //代現金合計金額

    z->tb_cash.qt_bonout.sprintf("%08d", giG);           //溢收合計回數
    z->tb_cash.am_bonout.sprintf("%08d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //餘額券
    iqt_ffu1=giI;                                        //現金卡回數  電子錢 1847
    iam_ffu1=giJ;                                        //現金卡金額  電子錢 1855    刷卡合計

    AnsiString z_d;

    z_d += (z->tb_cash.qt_bonus+"|");                //禮券回數
    z_d += (z->tb_cash.am_bonus+"|");                //禮券金額
    z_d += (z->tb_cash.qt_vndcpn+"|");               //廠商折價券回數
    z_d += (z->tb_cash.am_vndcpn+"|");               //廠商折價券金額
    z_d += (z->tb_cash.qt_stocpn+"|");               //折價券回數
    z_d += (z->tb_cash.am_stocpn+"|");               //折價券金額
    z_d += (z->tb_cash.qt_totcash+"|");              //代現金合計回數
    z_d += (z->tb_cash.am_totcash+"|");              //代現金合計金額

    sTmp.sprintf("%08d",giF);
    z_d += (sTmp+"|");                               //餘額合計回數
    sTmp.sprintf("%08d",giE);
    z_d += (sTmp+"|");                               //餘額合計金額

    z_d += (z->tb_cash.qt_bonout+"|");               //溢收合計回數
    z_d += (z->tb_cash.am_bonout+"|");               //溢收合計金額

    //ZDATA::GetZDTData======================================================
    //現金合計 含 消費券

    SumCash41=SumCash(0,gi_tot_line);
    z->am_cash.sprintf("%08d",SumCash41-iTotOt2Rtn+iCSMAmt );

    // writelog("現金合計 含 消費券");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //現金合計
    //strcat(ze,z->no_rcstart.c_str());      //發票開始號碼
    //strcat(ze,z->no_rcend.c_str());        //發票結束號碼
    //ze[28] = 0;


    //ZDATA::GetZDTData Z帳部門別集計TABLE      =============================================

    AnsiString z_f;

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
            z->tb_depcal[i].am_dpsitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpsale.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmiss.sprintf("%08d",0);

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


        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            z->tb_depcal[i].am_dpsale.sprintf("%08d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            z->tb_depcal[i].am_dpmitm.sprintf("%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            z->tb_depcal[i].am_dpmiss.sprintf("%08d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;
        }


    }
    __finally
    {
      delete [] dept;
    }

   //ZDATA::GetZDTData writelog("Z帳部門別集計TABLE OK");

    AnsiString z_g;

    z_g += (z->am_tdpsitm.sprintf("%08d", s1)+"|");             // 部門合計交易項數
    z_g += (z->am_dpstot.sprintf("%08d" , s2)+"|");             // 部門合計銷售金額
    z_g += (z->am_tdpmitm.sprintf("%08d", s3)+"|");             // 部門合計誤退項數
    z_g += (z->am_dpmtot.sprintf("%08d" , s4)+"|");             // 部門合計誤退金額
    z_g += (z->am_cash+"|");                                   // 現金合計含消費券 45


    //ZDATA::GetZDTData 收銀員明細ｆｉｅｌｄ=============================================

    AnsiString z_h;

    //z_h += (z->am_cash+"|");                                    //現金合計
    //z_h += (z->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");    // 借零次數
    sTmp.sprintf("%08d",VisuilCpnAmt);
    z_h += (sTmp+"|");                                            //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)

    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // 借零合計金額
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);                                       //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    z_h += (sTmp+"|");                                            // 代售商品折讓金額  借零合計金

    z_h += (z->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");    // 投零次數
    z_h += (z->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");    // 投零合計金額

    //投庫
    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;           //投庫次數
    int int_drop_money = 0;       //投庫金額
    int int_drop_sum = 0;    //投庫總額
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        //StringSegment_EX(s, "|", 15)
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生日結帳表:非投庫(%s)資料",s1041);
             writelog(sTmp );
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //現金
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC付款
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //信用卡
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //禮卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //折價卷
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //提貨單

        //中獎發票金額  800   Cloud 中獎發票金額(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s1041, "|", 18) );

        //ZDATA::GetZDTData中獎發票金額  500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //ZDATA::GetZDTData中獎發票金額  1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //ZDATA::GetZDTData中獎發票金額  200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //費用 01金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //費用 02金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //費用 03金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //費用 04金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //費用 05金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //費用 06金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //費用 07金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //費用 08金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //費用 09金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //費用 10金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //費用 11金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //費用 12金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //費用 13金額
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //費用 14金額

        int_drop_sum += int_drop_money;          //投庫總額
        int_drop_money = 0;
        ++int_drops;                             //投庫次數
    }

    z_h += (z->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        // 投庫次數
    z_h += (z->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     // 投庫合計金額

    //writelog("投庫合計金額 OK");

    //ZDATA::GetZDTData 更正次數、金額
    SumUpdat(0,gi_rec_line);
    z_h += (z->tb_casher.qt_update.sprintf("%08d", giA)+"|");          // 更正次數
    z_h += (z->tb_casher.am_update.sprintf("%08d", giB)+"|");          // 更正合計金額


    //交易取消
    SumVoid(0,gi_tot_line);
    z_h += (z->tb_casher.qt_void.sprintf("%08d", giA)+"|");            // 交易取消次數
    z_h += (z->tb_casher.am_void.sprintf("%08d", giB)+"|");            // 交易取消合計金額


     //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
    }
    __finally
    {
        //delete ini_ecr;
        ;;
    }


    z_h += (z->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // 訓練合計次數
    z_h += (z->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // 訓練合計金額

    //手動輸入部門13、14
    SumInpdp(0,gi_rec_line);
    z_h += (z->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");            //手動輸入部門次數
    z_h += (z->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");            //手動輸入部門金額

    z_h += (z->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");     // 開錢櫃次數

    //折扣讓次數   //小計折扣
    SumDisc(0,gi_pay_line);
    z_h += (z->tb_casher.qt_disc.sprintf("%08d", giA)+"|");             // 折扣次數(應稅)
    //z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");            // 折扣金額(應稅)
    z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP)+"|");            // 折扣金額(應稅),改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    z_h += (z->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");            //促銷折扣/讓次數
    z_h += (z->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");             //促銷折扣/讓金額

    //加值機
    easy_card(0,gi_easy_line);
    z_h += (z->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");          // 加值機開/售卡次數
    z_h += (z->tb_casher.am_disret1.sprintf("%08d", giB)+"|");         // 加值機開/售卡金額
    z_h += (z->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");          // 加值機加值次數
    z_h += (z->tb_casher.am_disret2.sprintf("%08d", giD)+"|");          // 加值機加值金額

    //fetc_card(0,gi_fetc_line);   //e通卡  2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數  2015/08/19 代售,手續費銷售金額(免)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //備用八金額  現金 加值金額   2015/08/19 代售,手續費作廢金額(免)
    z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數  2015/08/19 代售,手續費銷售金額(免)
    z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|"); //備用八金額  現金 加值金額   2015/08/19 代售,手續費作廢金額(免)

     //食安次數、金額  int iqt_food,iamt_food;
    z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數  2015/08/19 0
    z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額  2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數  2015/08/19 0
    //z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額  2015/08/19 0


    //   05 追加 table        =============================================

    AnsiString z_i;

    z_i += (z->tb_newadd.qt_cashCard.sprintf("%08d", iqt_ffu1)+"|");    //刷卡/現金卡次數    電子錢 1847
    z_i += (z->tb_newadd.am_cashCard.sprintf("%08d", iam_ffu1)+"|");    //刷卡/現金卡金額    電子錢  刷卡合計 1855
    z_i += (z->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");            //代收停車費次數
    z_i += (z->tb_newadd.am_parking.sprintf("%08d", 0)+"|");            //代收停車費金額
    z_i += (z->tb_newadd.qt_agnrcv+"|");                                //代收費用次數  1831
    z_i += (z->tb_newadd.am_agnrcv+"|");                                //代收費用金額  1839
    z_i += (z->tb_newadd.tx_mark.sprintf("%2s", str_ver_num.SubString(1,2) )+"|");      //版本前置符號
    z_i += (z->tb_newadd.tx_ver.sprintf("%6s",  str_ver_num.SubString(3,6) )+"|");      //版本號碼
    z_i += (z->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  //發票金額0次數

    //ZDATA::GetZDTData 不開發票金額
    //SumNOVoiceAmt(0,gi_tot_line);

    z_i += (z->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //不開發票金額
    z_i += (z->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");    //  不開發票金額誤退重

    SumLOSS(0,gi_rec_line);
    z_i += (z->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");             // TM廢棄Loss值次數
    z_i += (z->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");             // TM廢棄Loss值金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    SumHealthAmt(0,gi_tot_line);         // 健康捐合計
    z_i += (z->tb_newadd.am_ffu7.sprintf("%08d", iSub_C0InvoAmtA)+"|");             //原加值機加值取消金額

    SumRelDimAgn(0, gi_rec_line); //即時購代售
    int iSumRelDimAgn=giB;

    //z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", giB+iSub_C0InvoAmtC)+"|");             //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");             //原加值機加值取消次數


    //消費券
    z_i += (z->tb_newadd.am_posZRpt.sprintf("%08d", iCSMAmt)+"|");      //扣卡機扣卡金額   消費券

    AnsiString z_k;
    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);     即時購與銷轉進值應稅
    z_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //即時購與銷轉進值免稅
    z_k +=sTmp;

    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數   中獎發票張數
    z_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額   中獎發票金額
    z_k +=sTmp;


    //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    z_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //代售,手續費giD. 代售,手續費作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    z_k +=sTmp;

    //AnsiString DDZ_1099;
    //DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
    //   DDZ_1099=str_datez;

    //ZDATA::GetZDTData Z報表
    AnsiString str_zrp;

    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17    點卡手續費 Update
    str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str(),
                            "FFFFFFFF"
                            );

    sTmp.printf("ZDATA::GetZDTData::(%s) Len=%d",str_zrp, str_zrp.Length() );
    writelog(sTmp);


    AnsiString  Zrpfilename;
    //sTmp = ExtractFileName(SalFileName);

    Zrpfilename.sprintf("%s%s_%s.zrp",
                                        XZDATADIR,
                                        sYYMMDDZZ.c_str(),
                                        ZDT_1050_DDZ.c_str()  );


    //ZDATA::GetZDTData Z報表
    strcpy(report->char_filename, Zrpfilename.c_str()); //ChangeFileExt(str_z_filename,".zrp").c_str());
    report->CreateReport('Z', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
    //備份Z報表
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //部門帳表
    strcpy(report->char_filename, ChangeFileExt(Zrpfilename,".dpt").c_str() );
    report->CreateReport('D', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no);
    //備份部門帳表
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    RtnZDTData=str_zrp;

  return(0);
}


//自動日結
int __fastcall AUTOZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                    const String SalFileName, const String Version, const String AutoZDttm)
{

    AnsiString RtnAutoXData, sLog, sTmp,str_ver_num, str_tran_cnt;

    AnsiString str_z_filename;              //Z帳輸出檔名


    sLog.sprintf("AUTOZDATA::自動日結 WriteData(StoreNO=%s, EcrNO=%s, SalesNo=%s, SalFileName=%s, Version=%s, AutoZDttm=)",StoreNO,EcrNO,SalesNo,SalFileName, Version, AutoZDttm);
    writelog( sLog);

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
    //

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z帳結構


    //目前Z帳時間
    AnsiString str_z_date,                  //現在時間
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


    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "00001");
    int iTmpCnt =  _StrToInt( str_nz_cnt )-1;
    if( iTmpCnt <= 0)
        iTmpCnt=99999;
    AutoZ_Cnt.sprintf("%05d",iTmpCnt);

    Lst_AutoZ_Dttm=tif_ecr->ReadString("AUTOZ","LST_AUTOZ_DTTM","");              //前次自動日結時間
    Nst_AutoZ_Dttm=tif_ecr->ReadString("AUTOZ","NST_AUTOZ_DTTM","");              //現再自動日結時間
    Lst_ZCNT_Dttm=tif_ecr->ReadString("AUTOZ","LST_ZCNT_DTTM",FormatDateTime("yyyymmdd",Now()) );

    if( Trim(AutoZDttm)=="" || AutoZDttm.Length()!=8 )
        AutoZ_Dt = FormatDateTime("yyyymmdd",Now()-1);  //日結日歷日
    else
        AutoZ_Dt = AutoZDttm;

    //AutoZ_Dt="20130522" ;

    ////////////////////////////   自動交班   //////////////////////////////

     RtnAutoXData=AutoXData(StoreNO, EcrNO, SalesNo , SalFileName, Version, AutoZ_Cnt, AutoZ_Dt ) ;

     gtsl_rec->Clear();
     gtsl_tot->Clear();
     gtsl_drop->Clear();
     gtsl_easy_card->Clear();
     gtsl_pay_sub->Clear();
     gtsl_bil->Clear();
     gtsl_fetc_card->Clear();
     tsl_subsale->Clear();
     gtsl_1051_sal->Clear();
     gtsl_Ticket->Clear();

     str_date_time = gchar_date_time;  // 交易時間取自動交班


     //////////////////////////////////////////////////////////////////////

    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false , AutoZ_Cnt, AutoZ_Dt);               //載入SAL檔，將檔案依型態分類
    //int_reslut = loadsalfile(SalFileName);               //載入SAL檔，將檔案依型態分類
    if (!int_reslut)
       {
         // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("AUTOZDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

        tif_ecr->WriteString("Z","ZOPERATION","0");
        sLog.sprintf("AUTOZDATA::設定 [Z]->ZOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }


         sTmp = FormatDateTime("yyyymmdd",Now());
         tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
         sLog.sprintf("AUTOZDATA::loadsalfile:%s Error...無須進行日結動作.", SalFileName.c_str() );
         writelog( sLog);
         sLog.sprintf("AUTOZDATA::設定自動日結時間 AUTOZ->LST_AUTOZ_DTTM=%s..自動日結結束.", sTmp.c_str() );
         writelog( sLog);
         //return 0;      //千萬不可回 return OK;
         return 2;      //2013/10/28
       }
 

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );
    str_ver_num = Version;   //get_version(Version);       //日結

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號


    //備份載入之原始之料
    Auto_Sal->Clear();
    Auto_Sal->LoadFromFile(SalFileName);
    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Autotsl_Ticket->Assign(gtsl_Ticket);
    //Auto_1051_sal->Assign(gtsl_1051_sal);           自動日結無須使用

    AutoZstartTrans=GetSysDttmMaxTransNo(str_date_time.SubString(1,8) );

    if( !GetYYYYMDD_SalData(AutoZ_Cnt, AutoZ_Dt, 1) )    // 取得 日歷日之 sal 資料
      {
        // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("AUTOZDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

        tif_ecr->WriteString("Z","ZOPERATION","0");
        sLog.sprintf("AUTOZDATA::設定 [Z]->ZOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp = FormatDateTime("yyyymmdd",Now());
        tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
        sLog.sprintf("AUTOZDATA::GetYYYYMDD_SalData:Auto_Sal 中查無相關日結資料(%s)製作帳表...無須進行日結動作.", AutoZ_Dt.c_str() );
        writelog( sLog);
        sLog.sprintf("AUTOZDATA::設定自動日結時間 AUTOZ->LST_AUTOZ_DTTM=%s..自動日結結束.", sTmp.c_str() );
        writelog( sLog);
        //return 0;    //千萬不可回 return OK;
        return 2;      //2013/10/28
      }

    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //目前交易序號,但SAL最後一筆須檢1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("AUTOZDATA::前次自動日結時間=%s, 現在自動日結時間=%s, 日歷日=%s, 最大日結序號=%s, ECR->WriteData 取的目前之交易序號:%s, 換日參日Lst_ZCNT_Dttm=%s, AutoZstartTrans=%s",
                  Lst_AutoZ_Dttm.c_str(),FormatDateTime("yyyymmdd",Now()), AutoZ_Dt.c_str(), AutoZ_Cnt.c_str(), str_tran_cnt, Lst_ZCNT_Dttm.c_str(), AutoZstartTrans.c_str() );
    writelog( sLog);


    TStringList *Z_CntLst = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->Clear();
    ZDT_Lst->Clear();

    if( !_GetZCntLst(Z_CntLst, Auto_Sal  ) )
      {
        // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("AUTOZDATA::設定 [X]->XOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

        tif_ecr->WriteString("Z","ZOPERATION","0");
        sLog.sprintf("AUTOZDATA::設定 [Z]->ZOPERATION=0..預防 XZDATA.INI 被還原");
        writelog( sLog);

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp = FormatDateTime("yyyymmdd",Now());
        tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
        sLog.sprintf("AUTOZDATA::_GetZCntLst:Auto_Sal 中查無相關日結序號製作帳表...無須進行日結動作.");
        writelog( sLog);
        sLog.sprintf("AUTOZDATA::設定自動日結時間 AUTOZ->LST_AUTOZ_DTTM=%s..自動日結結束.", sTmp.c_str() );
        writelog( sLog);
        //return 0;    //千萬不可回 return OK;
        return 2;      //2013/10/28
      }


    str_sal_first = gpchar_sal_first;
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");       //前次時間

    AnsiString sTmpFirstDttm;
    sTmpFirstDttm=_StringSegment_EX(str_sal_first, "|", 5);

    //維修通知函---131209-0153...FM-010171-羅東北城店 SC 營收日12/7缺TM2 Z帳，查看.ZRP檔 檔案名稱異常
    //if( _StringSegment_EX(str_sal_first, "|", 5) < str_pz_date )
    if( sTmpFirstDttm < str_pz_date  && Trim(sTmpFirstDttm).Length()==14 )
      {
        //sLog.sprintf("AUTOZDATA:: 取的目前開始之交易:%s",str_sal_first.c_str() );
        //writelog( sLog);
        str_pz_date=sTmpFirstDttm; //_StringSegment_EX(str_sal_first, "|", 5);
        sLog.sprintf("AUTOZDATA:: 取的目前開始之交易:%s Set str_pz_date=%s",str_sal_first.c_str(), str_pz_date.c_str() );
        writelog( sLog);
      }


     if( Autotsl_tot->Count > 0)
       {
            sTmp=Autotsl_tot->Strings[Autotsl_tot->Count-1];
            sLog.sprintf("AUTOZDATA:: 取的目前最大1050之交易:%s",sTmp.c_str() );
            writelog( sLog);

            str_z_date=_StringSegment_EX(sTmp, "|", 5);
       }
     else
       {
        str_z_date = _StringSegment_EX(gsLast1099, "|", 5);  //str_date_time;   //現在時間
       }

    if( str_z_date.Trim() == "")
        str_z_date=str_date_time;   //現在時間

    str_datez = tif_ecr->ReadString("Z","DATEZ","");           //DDZ
    sTmp=tif_ecr->ReadString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100" ));  //自動日結要如此

    //將帳表日資料備份已被將來判斷殺目錄條件, 刪除目錄開始日期 james - 2003/02/07
    tif_ecr->WriteString("Backup", "pz_date", sTmp);


    //備份 sal8.TXT條件
    tif_ecr->WriteString("Backup", "datez", str_datez);

    if ((str_pz_date == "") || (str_pz_date.Length() < 8))  //無相關資料，交易記錄歸零
    {
        tif_ecr->WriteString("Z", "pz_date", str_z_date);
        tif_ecr->WriteString("Z", "pzno_seq", "0");
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

        AnsiString str_xpath;               //指定收銀員帳表目錄
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date.SubString(1,8)+"000000";
    }


    str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數 01~09  換日歸1
    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零 00001~99999
    tif_ecr->WriteString("Z_OFF", "nz_cnt", str_nz_cnt  );            // 離線資料使用
    tif_ecr->WriteString("Z_OFF", "pz_cnt", tif_ecr->ReadString("Z","pz_cnt","00000")  );  // 離線資料使用

    str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //累積Z帳金額   換日不歸零


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.Length() < 5)
        str_nz_cnt = "00001";

    sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_pzno_seq));
    
#ifdef TM_20


    str_z_filename.sprintf("%s%02d%s%02d%05s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        str_pz_date.SubString(3,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        AutoZ_Cnt.c_str()              );
#else
    //輸出檔案 yyyymmddnn.zdt ex:2002010101.zdt
    //str_z_filename = (str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt");

    str_z_filename.sprintf("%s%02d%s%02d%05s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        str_pz_date.SubString(3,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        AutoZ_Cnt.c_str()              );

#endif

    //換日Flag
    bool bl_change_day;

    //判斷換日
    //if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
    if (str_pz_date.SubString(1,8).AnsiCompare(str_date_time.SubString(1,8)))
        {
          sLog.sprintf("AUTOZDATA::換日判斷 pz_date=%s, date_time=%s.. 系統已換日 Set bl_change_day = true",str_pz_date.c_str(), str_date_time.c_str() );
          writelog( sLog);
          bl_change_day = true;
        }
    else
        {
          sLog.sprintf("AUTOZDATA::換日判斷 pz_date=%s, date_time=%s.. 系統未換日 Set bl_change_day = true",str_pz_date.c_str(), str_date_time.c_str() );
          writelog( sLog);
          bl_change_day = false;
        }

    sLog.sprintf("AUTOZDATA::Last1099=(%s)",gsLast1099.c_str());
    writelog( sLog);

    DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    if(  _StrToInt( DDZ_1099.SubString(1,2) ) < 1 || _StrToInt( DDZ_1099.SubString(1,2) ) > 31 )
       DDZ_1099=str_datez;
   
    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //歷史目錄維護用 ,絕對不可刪除目錄


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  str_orgam_tclsacc=str_am_tclsacc;

   AnsiString ZrpFile, DptFile;

   ZrpFile=ChangeFileExt( str_z_filename, ".zrp");     //ExtractFileName(str_z_filename)
   DptFile=ChangeFileExt( str_z_filename, ".dpt");
     // sBackupPath=ExtractFilePath(BackupFileName);
     //   sFileNameNonPath = ExtractFileName(BackupFileName);    //檔名不包含路徑
  for( int i=0; i< Z_CntLst->Count; i++)
       {
         sRec=Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);
         GetZCNT_SalData(TmpZcnt) ;
         GetZDTData(TmpZcnt, String(gchar_tencode),  String(gchar_ecr_no), String(gchar_cashier),
                    String(str_ver_num), str_pz_date, str_z_date, DDZ_1099, sRec,
                    SaleZDT, str_am_tclsacc, str_zrp_path, str_dpt_path  );

         ZDT_Lst->Add(SaleZDT);
         str_zrp+=SaleZDT;

         SaleZDT="";

         file_cat( str_zrp_path.c_str(), ZrpFile.c_str() );
         sLog.sprintf("AUTOZDATA::file_cat(%s, %s)",str_zrp_path.c_str(), ZrpFile.c_str() );
         writelog( sLog);

         file_cat( str_dpt_path.c_str(), DptFile.c_str() );
         sLog.sprintf("AUTOZDATA::file_cat(%s, %s)",str_dpt_path.c_str(), DptFile.c_str() );
         writelog( sLog);


       } // end of for( int i=0; i< Z_CntLst->Count; i++)

     sTmp=ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+ExtractFileName(ZrpFile);
     CopyFile(ZrpFile.c_str(), str_zrp_path.c_str(), false);
     sLog.sprintf("AUTOZDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
     writelog( sLog);
     DeleteFile( ZrpFile );
     sLog.sprintf("AUTOZDATA::DeleteFile(%s)",ZrpFile.c_str() );
     writelog( sLog);

     str_dpt_path=sTmp+ExtractFileName(DptFile);
     CopyFile(DptFile.c_str(), str_dpt_path.c_str(), false);
     sLog.sprintf("AUTOZDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
     writelog( sLog);
     DeleteFile( DptFile );
     sLog.sprintf("AUTOZDATA::DeleteFile(%s)",DptFile.c_str() );
     writelog( sLog);

     delete Z_CntLst;
     delete ZDT_Lst;


     //寫入SAL檔98Type
    AnsiString str_rec_type,
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



    str_rec_type = "1098";
    str_rec_len = "0121";
    str_tencode = gchar_tencode;
    str_ecr_no.sprintf("%02d",_StrToInt(z->no_tm,"z->no_tm"));
    str_sys_date = (z->bg_dtclose + z->bg_tmclose);      //日期時間

    //98交易序號 //20031215
    int int_sum_tran_cnt ;

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    //if (str_tran_cnt.Length() < 5)
    //if( Trim(str_tran_cnt)=="00000")
    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    sLog.sprintf("AUTOZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    writelog( sLog);


    AnsiString str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
         str_sum_tran.sprintf("%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //前次日結序號，目前日結序號
    str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "前次日結序號"));
    str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "目前日結序號"));


    if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc;

    //str_sal_first = gpchar_sal_first;

     //開始交易序號
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, "|", 7).SubString(TRANCNT_NO_POS,5));
    str_start_trans.sprintf("%05d", i_tran_cnt);

    //int_am_tclsacc=int_acc_tot_amt;
    //str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //本日結帳累計金額
    str_am_totsale = "0";                                      //z->tb_trans.am_totsale;                //本日實際銷貨收入
    str_zdt_no.sprintf("%010ld",z->bg_noclose.ToInt());     //ZDT序號代入Z檔名
    str_ver = "FFFFFFFF|";


    AnsiString str_trans1_5;    //交易序號前五碼

    //str_sal_last = gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //歷史目錄維護用    不可刪

    AnsiString str_01,str_99;

    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
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
                   " ",
                   " ",
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );

    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
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
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );

    AnsiString _str_tran_cnt;        //交易序號
    AnsiString str_st_tran_cnt;     //開始交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    AnsiString str_98;  //寫入1098交易序號
    str_98.sprintf("%4s|%4s|%6s|%2s|%14s|%-5s|%-10s|%5s|%5s|%10s|000%8s|000%8s|%10s|%9s\n",
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

    String str_sal;
    //str_sal = str_01 + "\n"+ RtnAutoXData + "\n" +str_zrp + str_98 + str_99;

    if( RtnAutoXData.Trim()=="")
       str_sal = str_01 + "\n"+ str_zrp + str_99;
    else
       str_sal = str_01 + "\n"+ RtnAutoXData + "\n" +str_zrp + str_99;

    ///////////////////////

    if (WriteSAL(str_sal, str_z_filename) == OK)
       {
        //備份ZDT帳
        str_zdt_path = BackupFile(str_z_filename,"", DDZ_1099);
        tif_tmp->WriteString("Z","x1098filename", str_zdt_path);
       }
    else
      {
       tif_tmp->WriteString("Z","x1098filename", "");
      }

    /////////////////////////////////////////////////////////////////

    AnsiString StrZslFile;
    if (WriteSAL(str_sal, SalFileName) == OK)
       {
         //備份ZSL帳

         sTmp = ExtractFileName(SalFileName);
         StrZslFile.sprintf("%s%s_%s%s",XZDATASAL,str_sys_date.SubString(1,8),DDZ_1099,sTmp);
         if( CopyFile( SalFileName.c_str(), StrZslFile.c_str(), false)  )
           {
             sLog.sprintf("AUTOZDATA:: InPutFile SalFileName 備份SAL %s TO %s",
                  SalFileName, StrZslFile );
             writelog( sLog);

           }
         else
           {
             sLog.sprintf("AUTOZDATA:: InPutFile SalFileName 備份SAL %s TO %s 失敗!!",
                  SalFileName, StrZslFile );
             writelog( sLog);
           }


         StrZslFile = BackupFile(SalFileName, ChangeFileExt(str_zdt_path,".zsl"), DDZ_1099 );
         tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);

         sTmp=ChangeFileExt(StrZslFile,".asl");
         Auto_Sal->SaveToFile(sTmp);
         writelog("AUTOZDATA::備份載入之SAL Auto_Sal->SaveToFile=>" + sTmp);

       }
    else
       {
         tif_tmp->WriteString("Z","ZSLNAME","");
       }


    AnsiString str_ipt_path;

    //更改XZDATA.INI
    int int_nzno_seq   = _StrToInt(str_pzno_seq, "pzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp->WriteString("Z","ZFILENAME" , str_zdt_path);
    tif_tmp->WriteString("Z","ZRPTNAME"  , str_zrp_path);
    tif_tmp->WriteString("Z","DEPTNAME"  , str_dpt_path);
    tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

    tif_tmp->WriteString("Z","PZ_DATE"     ,AutoZ_Dt+"235959" ); //str_z_date);         2013/09/12
    tif_tmp->WriteString("Z","AM_TCLSACC" , IntToStr(int_am_tclsacc) );
    tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT 交易序號

    tif_tmp->WriteString("AUTOZ","LST_AUTOZ_DTTM",str_date_time.SubString(1,8)); //AutoZ_Dt);   //前次自動日結時間
    //tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM","");  //現再自動日結時間無作用
    tif_tmp->WriteString("AUTOZ","LST_ZCNT_DTTM",Lst_ZCNT_Dttm);   //換日參考日

    tif_tmp->WriteString("ECR","tot_open_z", "0");
    tif_ecr->WriteString("ECR","tot_open_z", "0");

    tif_tmp->WriteString("VXZ", "NZNO_SEQ", "1");
    tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");

    tif_tmp->WriteString("VXZ", "PZ_DATE", "");
    tif_ecr->WriteString("VXZ", "PZ_DATE", "");


    if (bl_change_day) //判斷是否換日
    {
        tif_ecr->WriteString("Z","PZNO_SEQ", "0");
        tif_ecr->WriteString("Z","NZNO_SEQ", "1");
        tif_ecr->WriteString("Z","PZ_CNT", AutoZ_Cnt); // str_nz_cnt);

        //if (++int_nz_cnt > 99999) 自動日結不異動
        //    int_nz_cnt = 1;
        //str_nz_cnt.sprintf("%05d",int_nz_cnt);
        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);

        //tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));

        tif_ecr->WriteString("Z","DATEZ", ( str_date_time.SubString(7,2) + "1" ));
        tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_date_time.SubString(1,8) + "0100" ));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));


        //tif_ecr->WriteString("Backup", "datez",( str_date_time.SubString(7,2) + "1" ) );  //歷史目錄維護用

        sLog="AUTOZDATA::換日處理 DATEZ=" + str_date_time.SubString(7,2)+ "1" +  ", YYYYMMDDZ=" + str_date_time.SubString(1,8)+"0100";
        writelog( sLog);



    }
    else
    {
        tif_ecr->WriteString("Z","PZNO_SEQ", str_pzno_seq);

        if (++int_nzno_seq > 9)
            int_nzno_seq = 1;

        str_pzno_seq.sprintf("%d", int_nzno_seq);

        tif_ecr->WriteString("Z","NZNO_SEQ", str_pzno_seq);
        tif_ecr->WriteString("Z","PZ_CNT", AutoZ_Cnt);  //str_nz_cnt);

        //if (++int_nz_cnt > 99999)       自動日結不異動
        //    int_nz_cnt = 1;
        //str_nz_cnt.sprintf("%05d", int_nz_cnt);
        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);

        //tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
        //                      + FormatFloat("00",int_nzno_seq) + "00"));

        tif_ecr->WriteString("Z","DATEZ", ( str_date_time.SubString(7,2) + IntToStr(int_nzno_seq)));
        tif_ecr->WriteString("Z","YYYYMMDDZ", (str_date_time.SubString(1,8)
                              + FormatFloat("00",int_nzno_seq) + "00"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        //tif_ecr->WriteString("Backup", "datez",( str_date_time.SubString(7,2) + "1" ) );  //歷史目錄維護用

        sLog="AUTOZDATA::未換日處理 DATEZ=" + str_date_time.SubString(7,2)+ IntToStr(int_nzno_seq) +  ", YYYYMMDDZ=" + str_date_time.SubString(1,8) + FormatFloat("00",int_nzno_seq) + "00";
        writelog( sLog);

    }

    str_pz_date    = str_z_date;
    str_pzno_seq   = tif_ecr->ReadString("Z","NZNO_SEQ","");      //目前Z帳次數   換日歸1

    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    //指定X 帳PATH
    AnsiString  str_x_path;
    //str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    str_x_path = write_xpath((str_date_time.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //前筆誤打/卡紙重印
    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_tran_cnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    AnsiString LstXdtDttm;

    LstXdtDttm=tif_ecr->ReadString("X","EX_DATE","");    // 上次日結時間
    if( LstXdtDttm.SubString(1,8) >= str_date_time.SubString(1,8) ) // 00:00~04:00 有交班
      {

        sLog.sprintf("AUTOZDATA::自動日結完成  LstXdtDttm=%s  大於等於 %s  不將X帳歸零.", LstXdtDttm.c_str(), str_date_time.c_str() );
        writelog( sLog);

      }
    else
      {
        //tif_ecr->WriteString("X","PTRAN_CNT", AutoZstartTrans.SubString(3,8) );
        
        sLog.sprintf("AUTOZDATA::自動日結完成  LstXdtDttm=%s  小於 %s  將X帳歸零.", LstXdtDttm.c_str(), str_date_time.c_str() );
        writelog( sLog);
        // 自動日結 不將X帳歸零    _WZ_OK 會規0
        tif_ecr->WriteString("X", "PTRAN_CNT", "00000000");
        tif_ecr->WriteString("X", "SX_DATE"  , "");
        tif_ecr->WriteString("X", "EX_DATE"  , "");
      }

     //2015/03/11 Mantis:019302_交班失敗，防止自動日結XZDATA.INI被回寫
     sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
     if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

    sLog.sprintf("AUTOZDATA::自動日結完成 DDZ_1099=%s, XZDATAINI Z->PZ_CNT=%s, ECR->TRAN_CNT=%s, AM_TCLSACC=%d, [AUTOZ]->Lst_ZCNT_Dttm=%s, XPATH=%s,  AutoZstartTrans=%s ",
                  DDZ_1099, AutoZ_Cnt, str_sum_tran,int_am_tclsacc, Lst_ZCNT_Dttm, str_x_path, AutoZstartTrans.SubString(3,8) );
    writelog( sLog);


    return OK;

}


AUTOZDATA::AUTOZDATA()
{
    //
    writelog("Z帳 開始");
    z = new SPOS_ZRPDT;         //Z帳結構
    x = new SPOS_XRPDT;
    tif_tmp = new TIniFile(ZDATATMP_INI);
    tif_ecr = new TIniFile(XZDATA_INI);
    report = new REPORT();      //日結帳表


}

AUTOZDATA::~AUTOZDATA()
{
    delete z, tif_ecr;

    delete report;      //日結、部門帳表
    delete tif_tmp;

    writelog("AUTOZ帳 結束");


}

////////////////////////////////////////////////////////////////////////////////
//自動收銀員交班
AnsiString __fastcall AUTOZDATA::AutoXData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version, const AnsiString AutoZ_cnt,
                                const AnsiString AutoZ_Dt )
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, true, AutoZ_cnt, AutoZ_Dt);               //載入SAL檔，將檔案依型態分類

    AnsiString sLog, SalAutoZcnt;

    sLog.sprintf("AUTOZDATA::自動收銀員交班 AutoXData(%s, %s, %s, %s, %s, %s, %s)",
                  StoreNO,EcrNO,SalesNo,SalFileName, Version, AutoZ_cnt, AutoZ_Dt);
    writelog( sLog);
    SalAutoZcnt=AutoZ_cnt;

    if (!int_reslut)
        return "";

    if( gtsl_tot->Count > 0 || gtsl_drop->Count > 0 )
      {
       ;;
      }
    else
      {
        sLog.sprintf("AUTOZDATA::自動收銀員交班 =>無交易及投庫資料, 不產生收銀員交班表, gtsl_tot->Count=%d, gtsl_drop->Count=%d ",
                      gtsl_tot->Count, gtsl_drop->Count);
        writelog( sLog);
        return "";
      }

    AnsiString str_ver_num,str_nz_cnt, sTmp, sSalesLoginDttm;

    str_ver_num = get_version(Version);     //版本編號

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );


    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //原始店號

    AnsiString str_x_filename,              //XDATA檔名
               str_ptran_cnt,               //前次結帳序號
               str_date_z,                  //DataZ(日期加X帳次數)
               str_tran_cnt,                //開店以來結帳金額
               str_pxno_seq,                //X帳結帳次數
               str_nxno_seq,                //下次結帳數
               str_YYYYMMDDZ,
               SaleDataVer;

    str_date_z = tif_ecr->ReadString("Z","DATEZ","");             //ddn
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");         //目前交易序號,但SAL最後一筆須檢1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

    sLog.sprintf("AUTOZDATA::AutoXData ECR->WriteData 取的目前之交易序號:%s", str_tran_cnt);
    writelog( sLog);

    str_YYYYMMDDZ=tif_ecr->ReadString("Z","YYYYMMDDZ","201001010000");

    AnsiString str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;

#ifdef TM_20
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "00001");             //999999目前日結序號  換日不歸零
    str_date_time = tif_ecr->ReadString("Z","pz_date", gchar_date_time);  //2003/03/08 2004/03/22 liu
    if (str_date_time.Length() < 8)
           str_date_time = FormatDateTime("yyyymmddhhnnss",Now());        //2003/03/08 liu
#else
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());               //2003/03/08 liu
#endif

    if(str_date_z.Length() < 3)    //DATAZ=日期+Z帳次數  ddn
       str_date_z = (str_date_time.SubString(7,2) + "1");

    AnsiString str_sal_last, str_sal_last_1099, AutoZstartTrans ;        //*.sal 最後一筆
    str_sal_last = gpchar_sal_last;
    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //資料版本

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

     if( Trim(SalesNo)=="")
       {
         sTmp= _StringSegment_EX(str_sal_last_1099, "|", 14);
         sprintf(gchar_cashier,"%5.5s",sTmp.c_str() );
       }
     else
       {  sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );    }


    //if (str_tran_cnt.Length() < 5)  //上次交易序號
    //2012/08/06 Mark 已INI 為依據
    //if( Trim(str_tran_cnt)=="00000" )
    if ( _StrToInt(str_tran_cnt) < 0 )  //上次交易序號
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //交易序號

    //SalAutoZcnt=_StringSegment_EX(str_sal_last_1099, "|", 6);

    sLog.sprintf("AUTOZDATA::AutoXData Last1099(%s), ECR->TRAN_CNT=%s, SalAutoZcnt=%s",
                  str_sal_last_1099, str_tran_cnt, SalAutoZcnt );
    writelog( sLog);

    //str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","01");                  //上次x帳累積次數
    str_pxno_seq = tif_ecr->ReadString("X","Autoxno_seq","01");                  //自動日結規不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理

    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //目前x次到第幾次

    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //最多99次X帳
        str_nxno_seq = "01";

    x->no_tm = gchar_ecr_no;                      //收銀機機號
    x->no_casher = gchar_cashier;                 //收銀員號碼

    if (x->no_casher.Length() < 5)
        x->no_casher = "00001";

    //X帳序號  01~99
    x->no_seq = str_nxno_seq;

    AnsiString str_x_first;                      //X帳第一筆Sal(不代表為SAL第一筆)
    str_x_first = gpchar_x_first;

    //x->dt_begin = tif_ecr->ReadString("X","login_dttm","").SubString(1,8); //開始日期
    //x->tm_begin = tif_ecr->ReadString("X","login_dttm","").SubString(9,6); //開始時間

    x->dt_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(1,8); //前次結帳日期
    x->tm_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(9,6); //前次結帳時間

    sSalesLoginDttm=tif_ecr->ReadString("X","SALESLOGINDTTM","");

    AnsiString str_invo_char;                    //發票英文字母
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.Length() < 2)
        str_invo_char = "XX";

    Auto_Sal->LoadFromFile(SalFileName);
    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Auto_1051_sal->Assign(gtsl_1051_sal);
    Autotsl_Ticket->Assign(gtsl_Ticket);

    if( !GetYYYYMDD_SalData(SalAutoZcnt,AutoZ_Dt,1) )
        return ""; // 取得 日歷日之 sal 資料

    sLog.sprintf("AUTOZDATA::自動收銀員交班 .. 現在自動日結時間=%s, 日歷日=%s, 最大日結序號=%s, ",
                  FormatDateTime("yyyymmdd",Now()), AutoZ_Dt.c_str(), SalAutoZcnt.c_str() );
    writelog( sLog);


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

    AnsiString str_pzno_seq;
    str_pzno_seq = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //目前Z帳次數1~9   換日歸1


    str_x_filename.sprintf("%s%02d%s%02d%02d.xdt",
                                         XZDATADIR,
                                         atoi(gchar_ecr_no),
                                        (str_date_time.SubString(3,6)).c_str(),
                                         _StrToInt(str_pzno_seq),_StrToInt(str_nxno_seq) );


    //本次結帳日期


    if( Autotsl_tot->Count > 0)
       {
            sTmp=Autotsl_tot->Strings[Autotsl_tot->Count-1];
            sLog.sprintf("AutoXData:: 取的目前最大1050之交易:%s",sTmp.c_str() );
            writelog( sLog);
            ed_date_time=_StringSegment_EX(sTmp, "|", 5);
       }
    else
       {
        ed_date_time = _StringSegment_EX(str_sal_last_1099, "|", 5);  //交易序號 FormatDateTime("yyyymmddhhnnss",Now());
       }


    x->dt_end = AutoZ_Dt.SubString(1,8); // ed_date_time.SubString(1,8);
    x->tm_end = "235959";                //ed_date_time.SubString(9,6);

    if( ed_date_time.Trim()=="")
        ed_date_time = x->dt_end+x->tm_end;   //FormatDateTime("yyyymmddhhnnss",Now());


    //無開始時間、與結束時間相同
    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000") || Trim(x->dt_begin) == "")
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }

    // 2012/03/30 Update
    if( sSalesLoginDttm.Trim()=="")
        sSalesLoginDttm=ed_date_time;

    x->dt_begin=sSalesLoginDttm.SubString(1,8);
    x->tm_begin=sSalesLoginDttm.SubString(9,6);

    AnsiString x_a;
    int iC0TotCount=0;
    // 2013/08/28 Update
    x_a.sprintf("1095|%04d|%-6s|%-2s|%8s%6s|%05d|%-10s|%2s|%5s|%8s|%6s|%8s|%6s|",1779,
                                        gchar_tencode,
                                        x->no_tm.c_str(),         //收銀機機號
                                        x->dt_end.c_str(),        //本次結帳日期  ed_date_time.c_str(),
                                        x->tm_end.c_str(),         //本次結帳日期
                                        SalAutoZcnt.ToInt(),          //str_nz_cnt.ToInt(),
                                        str_YYYYMMDDZ.SubString(3,8),
                                        x->no_seq.c_str(),        //交班序號
                                        x->no_casher.c_str(),     //收銀員號碼
                                        x->dt_begin.c_str(),      //前次結帳日期,
                                        x->tm_begin.c_str(),      //前次結帳時間
                                        x->dt_end.c_str(),        //本次結帳日期
                                        x->tm_end.c_str()         //本次結帳日期
                                     );
    


    //本日來客數
    x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //本日來客數
    iC0TotCount=giA; //點卡手續費來客數

    //本日交易品項
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // 點卡手續費數量
    x->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    // 點卡手續費應免稅
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
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

    TotCount(0,gi_tot_line);
    x->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt  - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ));        //發票日結帳額(免)
    x->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));        //發票日結帳額(稅)
    x->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));        //發票日結帳金額
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    TotMiss(0,gi_tot_line);
    x->tb_trans.qt_miss.sprintf("%08d",    giA);        //誤打發票次數
    x->tb_trans.am_missntx.sprintf("%08d", giB-RC01FTaxAmt-R4_C01FTaxAmt- R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //誤打發票金額(免)
    x->tb_trans.am_misstax.sprintf("%08d", giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //誤打發票金額(稅)
    x->tb_trans.am_miss.sprintf("%08d",    giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        //誤打發票金額
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

    x->tb_trans.am_totsale.sprintf("%08d", int_totsale);

    AnsiString x_b;

    x_b  = (x->tb_trans.qt_tcust+"|");                      //本日來客數
    x_b += (x->tb_trans.qt_trans+"|");                      //本日交易品項
    x_b += (x->no_rcstart+"|");                              //發票開始號碼
    x_b += (x->no_rcend+"|");                                //發票結束號碼
    x_b += (x->tb_trans.am_trcpntx+"|");                    //發票日結帳額(免)
    x_b += (x->tb_trans.am_trcptax+"|");                    //發票日結帳額(稅)
    x_b += (x->tb_trans.am_trcptot+"|");                    //發票日結帳金額
    x_b += (x->tb_trans.qt_miss+"|");                       //誤打發票次數
    x_b += (x->tb_trans.am_missntx+"|");                    //誤打發票金額(免)
    x_b += (x->tb_trans.am_misstax+"|");                    //誤打發票金額(稅)
    x_b += (x->tb_trans.am_miss+"|");                       //誤打發票金額
    x_b += (x->tb_trans.am_totsale+"|");                    //本日實際銷貨收入


    //代售付TABLE===============================================================

    AnsiString x_c;
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

    x->tb_newadd.qt_cetel.sprintf("%08d", giD);          //代收次數
    x->tb_newadd.am_cetel.sprintf("%08d", giC);          //代收金額

    Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;


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

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
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

     x_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );




   // writelog( ("X:代售付ｔａｂｌｅ 長度應為 720 目前Data: " + z_c ) );
    /*******************************************************************
    for (int j=0; j<5; j++)
    {
        for (int k=0; k<9; k++)
        {
            if (k<8)    //每個TABLE有八個ITEM
            {
                if (j == 0)     //第一TABLE存代售
                {
                    if (k == 0)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giE);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giA);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else if (j == 3)    //第四TABLE存代售
                {
                    if (!k)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giF);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giB);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else
                {
                    x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                }
            }
            else
            {
                if (j == 0)     //代售總合
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giE);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giA);
                }
                else if (j == 3)    //代付總合
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giF);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giB);
                }
                else
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",0);
                }
            }
        }//for k
    }//for j
    *******************************************************************/

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

    x->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);       //禮券回數
    x->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //禮券金額
    SumPresent41+=giB; //禮券+商品禮券
    SumPresent41+=iCSMAmt; //消費券   for  收銀員交接班明細表

    x->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //廠商折價券回數 目前不使用
    x->tb_cash.am_vndcpn.sprintf("%08d", 0);             //廠商折價券金額

    x->tb_cash.qt_stocpn.sprintf("%08d", giC);           //折價券回數
    x->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //折價券金額
    SumCpn41+=giD;    //折價券+Other1+有價票券 CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(非實體券金額:giU), 實體券金額:giV)     25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    SumCpn41 -=  giU;

    //x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI - giT));      //代現金合計回數
    //x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ - iCSMAmt));  //代現金合計金額

    x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //代現金合計回數
    x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //代現金合計金額

    x->tb_cash.qt_bonout.sprintf("%08d", giG);           //溢收合計回數
    x->tb_cash.am_bonout.sprintf("%08d", giH);           //溢收合計金額

    iTotOt2Rtn=giE;                                      //giE:餘額券金額 / giF:餘額券次數
    iqt_ffu1=giI;                                        //現金卡回數  電子錢次數  3275
    iam_ffu1=giJ;                                        //現金卡金額  電子錢金額  刷卡合計 3261

    AnsiString x_d;

    x_d += (x->tb_cash.qt_bonus+"|");                //禮券回數
    x_d += (x->tb_cash.am_bonus+"|");                //禮券金額
    x_d += (x->tb_cash.qt_vndcpn+"|");               //廠商折價券回數
    x_d += (x->tb_cash.am_vndcpn+"|");               //廠商折價券金額
    x_d += (x->tb_cash.qt_stocpn+"|");               //折價券回數
    x_d += (x->tb_cash.am_stocpn+"|");               //折價券金額
    x_d += (x->tb_cash.qt_totcash+"|");              //代現金合計回數
    x_d += (x->tb_cash.am_totcash+"|");              //代現金合計金額

    sTmp.sprintf("%08d",giF);
    x_d += (sTmp+"|");                         //餘額合計回數
    sTmp.sprintf("%08d",giE);
    x_d += (sTmp+"|");                         //餘額合計金額

    x_d += (x->tb_cash.qt_bonout+"|");               //溢收合計回數
    x_d += (x->tb_cash.am_bonout+"|");               //溢收合計金額

    //==========================================================================

    //現金合計 含 消費券-餘額券
    SumCash41=SumCash(0,gi_tot_line);
    x->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    AnsiString x_e;

    x_e += (x->am_cash+"|");               //現金合計
    //x_e += (x->no_rcstart+"|");            //發票開始號碼
    //x_e += (x->no_rcend+"|");              //發票結束號碼


    //收銀員明細FIELD===========================================================

    AnsiString x_f;

    //借零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tb_vendfld[i].am_vend.sprintf("%08d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

        x->tb_casher.tb_vendfld[i].tm_vend.sprintf("%06d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

    x->tb_casher.qt_vendtot.sprintf("%08d",0);
    x->tb_casher.am_vendtot.sprintf("%08d",0);

    //x_f += (x->tb_casher.qt_vendtot+"|");      //借零次數
    sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");                          //借零次數  : VisuilCpnAmt :  無紙本折價金額   25176:TFM問題/需求/規格書變更通知(TMSC無紙本折價券修改)
    //x_f += (x->tb_casher.am_vendtot+"|");      //借零合計金額
    //sTmp.sprintf("%08d",iPreSalINVODisAmt);
    sTmp.sprintf("%08d",0);                      //代售商品折讓金額 0034576: (TMSC環保杯轉儲fami錢包):預售兌換的環保杯原計入代售商品折讓金額,改計入折扣(應稅)
    x_f += (sTmp+"|");                           // 代售商品折讓金額  借零合計金額


    AnsiString x_g;

    //投零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_feedfld[i].am_feed.sprintf("%08d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

        x->tb_casher.tg_feedfld[i].tm_feed.sprintf("%06d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

    x->tb_casher.qt_feedtot.sprintf("%08d",0);
    x->tb_casher.am_feedtot.sprintf("%08d",0);

    x_g += (x->tb_casher.qt_feedtot+"|");      //投零次數
    x_g += (x->tb_casher.am_feedtot+"|");      //投零合計金額


    AnsiString x_h;

    //投庫
    int int_drop_line = (gtsl_drop->Count - 1);   //投庫次數

    sTmp.printf("AUTOZDATA::AutoXData 產生收銀員交班明細表:投庫筆數(%d)",gtsl_drop->Count);
    writelog(sTmp );

    //初始化全部填零
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_dropfld[i].am_drop.sprintf("%08d",0);    //投庫金額
        x->tb_casher.tg_dropfld[i].tm_drop.sprintf("%-6s"," ");    //執行時間
    }

    int int_drops = 0;                            //投庫次數
    int int_drop_money = 0;                       //投庫金額
    int int_drop_sum = 0;                         //投庫總額
    int x_rtn_cnt=0;

    AnsiString str_drop_time, s;

    for (int i=0; i<=int_drop_line; i++)
    {
        s=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("產生收銀員交班明細表:非投庫(%s)資料",s);
             writelog(sTmp );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, "|", 11) );     //現金
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 12) );     //IC付款
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 13) );     //信用卡
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 14) );    //禮卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 15) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 16) );    //折價卷
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 17) );    //提貨單

        //中獎發票金額  800   Cloud 中獎發票金額(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 18) );

        //中獎發票金額 500
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 19) );

        //中獎發票金額 1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 20) );

        //中獎發票金額 200
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 22) );

        //費用單據金額 iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 23) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 25) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 27) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 29) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 31) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 33) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 35) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 37) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 39) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 41) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 43) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 45) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 47) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 49) );

        //每次投庫金額
        x->tb_casher.tg_dropfld[int_drops].am_drop.sprintf("%08d", int_drop_money);

        int_drop_sum += int_drop_money;                         //投庫總額

        str_drop_time = _StringSegment_EX(s, "|", 5).SubString(9,6);  //投庫時間

        //每次投庫時間
        x->tb_casher.tg_dropfld[int_drops].tm_drop.sprintf("%-6s", str_drop_time);

        int_drop_money = 0;
        ++int_drops;                             //投庫次數
        //sTmp.printf("AUTOZDATA::AutoXData 產生收銀員交班明細表:投庫:int_drop_money=%d,int_drops=%d",
        //           int_drop_money,int_drops);
        //writelog(sTmp );

    }

    //for (int i=0; i<50; i++)
    //{
   //     x_h += (x->tb_casher.tg_dropfld[i].am_drop+"h");  //投庫總次總合
   //     x_h += (x->tb_casher.tg_dropfld[i].tm_drop+"h");  //投庫金額總合
    //}

    AnsiString x_i;

    x_i += (x->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        //投庫次數
    x_i += (x->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     //投庫合計金額

    SumUpdat(0,gi_rec_line);
    x_i += (x->tb_casher.qt_update.sprintf("%08d", giA)+"|");               //更正次數
    x_i += (x->tb_casher.am_update.sprintf("%08d", giB)+"|");               //更正合計金額

    SumVoid(0,gi_tot_line);
    x_i += (x->tb_casher.qt_void.sprintf("%08d", giA)+"|");                 //交易取消次數
    x_i += (x->tb_casher.am_void.sprintf("%08d", giB)+"|");                 //交易取消合計金額

    SumInpdp(0,gi_rec_line);                                       //部門登錄
    x_i += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    x_i += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //開錢櫃次數

    //小計折扣  1011
    SumDisc(0,gi_pay_line);
    x_i += (x->tb_casher.qt_dise.sprintf("%08d", giA)+"|");                 //折扣次數
    //x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");         //折扣金額(應稅)
    x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP)+"|");         //折扣金額(應稅),改成包含預售兌換環保杯折讓(0034576: (TMSC環保杯轉儲fami錢包))

    pay_sub_amt(0,gi_pay_line);
    x_i += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //促銷折扣/讓次數
    x_i += (x->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");         //促銷折扣/讓金額


    easy_card(0,gi_easy_line);
    x_i += (x->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");                  //加值機開售卡次數
    x_i += (x->tb_casher.am_disret1.sprintf("%08d", giB)+"|");                  //加值機開售卡金額
    x_i += (x->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");                  //加值機加值次數
    x_i += (x->tb_casher.am_disret2.sprintf("%08d", giD)+"|");                  //加值機加值金額


    //x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //開錢櫃次數
    //x_i += (x->tb_casher.qt_pack.sprintf("%08d" , iTotOt2Rtn)+"|"); //giE);     //加值機結帳次數   2007/12/01 找餘額券
    //x_i += (x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"I");   //加值機結帳金額, 不開發票金額誤退重

    //追加TABLE=================================================================


    AnsiString x_j;

    //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //手動輸入部門次數、部門13、14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //手動輸入部門金額

    //fetc_card(0,gi_fetc_line);  // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //備用八金額  現金 加值金額    2015/08/19 代售,手續費作廢金額(免)
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //備用八次數  現金 加值次數   2015/08/19 代售,手續費銷售金額(免)
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //備用八金額  現金 加值金額    2015/08/19 代售,手續費作廢金額(免)

    //食安次數、金額  int iqt_food,iamt_food;
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //備用九次數  信用卡 加值次數  2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //備用九金額  信用卡 加值金額  2015/08/19 0

    // 20151123修正成食安次數、金額  Charles.Wang
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //備用九次數  信用卡 加值次數  2015/08/19 0
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //備用九金額  信用卡 加值金額  2015/08/19 0
    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //現金卡次數   電子錢次數  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //現金卡金額   電子錢金額  刷卡合計 3261

    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //備用十次數  FETC 結帳次數  保留
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"|"); //giFetcSumAmt);     //備用十金額  FETC 結帳金額  保留

    //發票金額0次數

    x_j += (x->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");         //代收停車費次數
    x_j += (x->tb_newadd.am_parking.sprintf("%08d", 0)+"|");         //代收停車費金額
    x_j += (x->tb_newadd.qt_cetel+"|");                              //代收次數FUNCTIO在前面
    x_j += (x->tb_newadd.am_cetel+"|");                              //代收金額
    //x_j += (x->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix)+"|");       //代收商品客數
    //x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");     //現金卡次數   電子錢次數  3275
    //x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");     //現金卡金額   電子錢金額  刷卡合計 3261
    //x_j += (x->tb_newadd.qt_ffu2.sprintf("%08d", 0)+"|");            //扣卡機扣卡次數                    3269
    //x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //扣卡機扣卡金額  2008/12/04 消費券  3277
    x_j += (x->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //版本前置符號  3285
    x_j += (x->tb_newadd.tx_ver.sprintf("%6s" , str_ver_num)+"|");   //版本號碼
    x_j += (x->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");    //發票金額0次數

    //不開發票金額

    x_j += (x->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //備用四金額,  不開發票金額
    x_j +=(x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"|");   //不開發票誤退重合計

    SumLOSS(0,gi_rec_line);
    x_j += (x->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");         //TM廢棄Loss次數
    x_j += (x->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");         //TM廢棄Loss金額

    //Sub_C0InvoAmt(0,gi_rec_line);        // 外加手續費giA,即時購代售點卡集計 giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // 健康捐合計
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //原加值機加值取消次數
    x_j += (x->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");   //原加值機加值取消金額


    SumRelDimAgn(0, gi_rec_line); //即時購代售   S0/S1   VDC > 0
    int iSumRelDimAgn=giB;


    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iSub_C0InvoAmtC)+"|");   //原加值機加值取消次數
    //即時購代售(S0/S1有連線區分)+點卡(C0/C1有連線區分)
    x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //原加值機加值取消次數

    x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //消費券


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

    // 2012/09/25  Update
    if( SaleDataVer >= "2012090100")
      {
          for (int i=0; i<60; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //投庫金額
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //投庫時間
            }

      }
    else
      {
        for (int i=0; i<30; i++)
            {
              x_h += ("00000000|");  //借零金額
              x_h += ("      |");    //借零時間
            }         //"000000|"

        for (int i=0; i<30; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //投庫金額
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //投庫時間
            }
      }

    VdcInvSumdat(0,gi_rec_line);   //即時購與銷轉進值
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);     即時購與銷轉進值應稅
    x_h +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //即時購與銷轉進值免稅
    x_h +=sTmp;


    //發票兌獎金額
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //發票兌獎張數    中獎發票張數
    x_h +=sTmp;
    sTmp.sprintf("%08d|",giA);     //發票兌獎金額    中獎發票金額
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for 收銀員交班明細表
    //SumCash41+=giA;                // 2012/10/08 for 收銀員交班明細表

     //計算電子禮券轉餘額券
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //電子禮券回數
    x_h +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //電子禮券金額
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //點卡手續費giD.作廢giE , 點卡手續費客數
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);  //2017/06/04 Update   代售,手續費銷售金額(應)   代售,手續費作廢金額(應)
    x_h +=sTmp;


    AnsiString str_xdt_path, str_xrp_path;   //X資料、帳表存放路徑

    SumBill78(0, gi_bil_line);
    writelog("產生提貨券資料:" + StrBillPay);

    AnsiString DDZ_1099, str_x_path;
    DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=str_date_z;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //收銀員帳表備份路徑
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.SubString(1,2) + "\\" + "0" + DDZ_1099.SubString(3,1) + "\\";



    //帳表檔名
    AnsiString str_new_name, sTmpPzData;

    /******/
    if ( Trim(str_x_path)=="" )
    {
        //無指定路徑由系繰日期決定

        str_x_path = "C:\\FTLPOS\\HISTORY\\";
        sTmpPzData=tif_ecr->ReadString("Z","PZ_DATE",FormatDateTime("yyyymmddhhnnss",Now())).Trim();

        if (sTmpPzData == NULL)
        {
            str_x_path += (str_date_time.SubString(7,2) + "\\01\\");
            writelog("確認歷史目錄 01:" + str_x_path +" ->str_date_time="+str_date_time);

        }
        else
        {
            str_x_path += ( sTmpPzData.SubString(7,2) + "\\0" );

            if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 1)
               { str_x_path += "1\\";
                writelog("確認歷史目錄 02:" + str_x_path  );
               }
            else
               {
                str_x_path += (tif_ecr->ReadString("Z","NZNO_SEQ","") + "\\");
                writelog("確認歷史目錄 03:" + str_x_path);
               }
        }

        tif_ecr->WriteString("Z","XPATH",str_x_path);
    }


    if (!DirectoryExists(str_x_path))
       {
        writelog("建立歷史目錄 :" + str_x_path);
        ForceDirectories(str_x_path);
       }
    str_new_name = (str_x_path + ExtractFileName(str_x_filename) );
    
    /*******/

    str_xdt_path = str_new_name;                     //寫入INI變數

    //X報表
    AnsiString xdt_1095_data;


    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%88s|\n",
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%79s|\n",  2012/09/18
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17   點卡手續費 Update
    xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%16s|\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),
                          x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          "0FFFFFFF"
                          );

    sTmp.printf("AUTOZDATA::AutoXData CreateReport:(%s) Len=%d",xdt_1095_data, xdt_1095_data.Length() );
    writelog(sTmp);

    //report = new REPORT();      //收銀員帳表

    strcpy(report->char_filename, ChangeFileExt(str_x_filename,".xrp").c_str());


    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  產生收銀員交班表  *.XRP                ***************/
    report->CreateReport('x', xdt_1095_data, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 1 );


     // 收銀員交班明細表
    //str_date_z          //ddn
    //str_tran_cnt    //目前交易序號

   // AnsiString sTmp ;
   // sTmp.printf("CreateReport: 產生收銀員帳表檔案 代售合計=%d",iam5_15217totrev);
   // writelog(sTmp);

    str_new_name = str_x_path;
    str_new_name +=   ExtractFileName(report->char_filename) ;

    DeleteFile(str_new_name);

    if (RenameFile(report->char_filename,str_new_name))       //搬檔
        writelog("AUTOZDATA::AutoXData 產生收銀員帳表(XRP)檔案 " + str_new_name + " ->gchar_date_time="+gchar_date_time);
    else
        writelog("AUTOZDATA::AutoXData 產生收銀員帳表(XRP)檔案失敗 " + str_new_name);


    str_xrp_path = str_new_name;        //寫入INI變數

    //寫入SAL檔1097Type
    AnsiString str_rec_type,
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

    str_rec_type = "1097";
    str_rec_len  = "0106";

    str_tencode = gchar_tencode;   //店號

    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"機號"));              //機號
    str_sys_date = (x->dt_end + x->tm_end);                              //日期時間

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"目前寫入1097交易序號"); //目前寫入97交易序號
    str_trans_cnt.sprintf("%05d", int_sum_transcnt);

    AnsiString str_sum_tran;       //此次收銀員帳交易序號
    //20031215
    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );

    //xxx
    //tif_tmp->WriteString("ECR","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_sum_tran;


    //str_pxno_seq  //上次x帳累積次數

    //str_nxno_seq  //目前x次到第幾次

    // 2007/07/17
    str_z_cnt.sprintf("%02d", _StrToInt(str_nxno_seq,"交班序號"));
    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //目前日結序號  換日不歸零

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //收銀員代號

    str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //開始交易序號下次帳表開始序號

    str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //本日實際銷貨收入
    str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF";


    AnsiString str_trans1_5, CasherRptFile;;    //交易序號前5碼
    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    CasherRptFile=ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    sTmp.sprintf("1041|0500|%-6s|%2s|%8s%6s|%05d|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        x->dt_end.c_str(),        //本次結帳日期  ed_date_time.c_str(),
                        x->tm_end.c_str(),         //本次結帳日期
                        SalAutoZcnt.ToInt(),                    // str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );



    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   SalAutoZcnt.ToInt(),                     //str_nz_cnt.ToInt(),
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
                   " ",
                   " ",
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

    str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   SalAutoZcnt.ToInt(),                     //str_nz_cnt.ToInt(),
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
                   0,
                   0,
                   gchar_orgtencode,                     //原始店號
                   " "                                   //保留
                                       );


    AnsiString _str_tran_cnt;    //交易序號

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);

    //寫入1097交易序號
    AnsiString str_97;
    str_97.sprintf("%4s|%4s|%-6s|%2s|%14s|%05d|%-10s|%8s|%10s|000%8s|%10s|%8s|",
                        str_rec_type.c_str(),     //1097
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        ed_date_time.c_str(),                 //str_sys_date.c_str(),
                        SalAutoZcnt.ToInt(),                 //str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );

    String str_sal;
    //str_sal = str_01 +  str_97 + str_99;
    str_sal = str_97;

    FILE *fxdata;

    if ((fxdata = fopen(str_xdt_path.c_str(), "w")) != NULL)
    {

        fprintf(fxdata,"%s\n",str_01.c_str());
        //fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%79s|\n",
        //fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%61s|\n",
        //fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%43s|\n",   2013/10/17    點卡手續費 Update
        fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%16s|\n",
                            x_a.c_str(),
                            x_b.c_str(),
                            x_c.c_str(),
                            x_d.c_str(),
                            x_e.c_str(),
                            x_f.c_str(),
                            x_g.c_str(),
                            x_i.c_str(),
                            x_j.c_str(),
                            x_h.c_str(),
                            "0FFFFFFF"
                            );

        fclose(fxdata);
    }
    else
    {
        writelog("AUTOZDATA::AutoXData 無法產生收銀員DATA " + str_xdt_path);
        return "";
    }


    /***************  產生收銀員交班明細表  收銀員交接班明細表 ***************/
    AnsiString SumCasherRptData;
    SumCasherRpt(AUTOZ,0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt, sTmp, str_nxno_seq,
                 CasherRptFile, str_xdt_path, str_01, str_97, str_99, SumCasherRptData, iamt_food, iqt_food, 1);

    xdt_1095_data+=SumCasherRptData;
    sTmp.printf("AUTOZDATA::AutoXData 產生收銀員交班明細表:%s",CasherRptFile);
    writelog(sTmp );

    /*************************************************************************/
    // 自動交班不異動無須累進交易序號
    //if ((int_sum_transcnt + 2) > 99999)
    //        int_sum_transcnt = 1;
    //else
    //     int_sum_transcnt +=2;
    //str_sum_tran.sprintf("%05d", int_sum_transcnt );
   // tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    AnsiString sTtmpPTRAN_CNT;  //2012/06/04
    sTtmpPTRAN_CNT=str_trans1_5.SubString(3,3) + str_ptran_cnt;
    //tif_ecr->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));

    //自動日結規不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理
    //tif_ecr->WriteString("X","PXNO_SEQ",   str_nxno_seq);

    tif_ecr->WriteString("X","SX_DATE",   (x->dt_begin + x->tm_begin));
    //tif_tmp->WriteString("X","EX_DATE",   (x->dt_end + x->tm_end));     //此為下次交班之(前次結帳日期時間) 自動交班不異動

    tif_ecr->WriteString("ECR","tot_open_x", "0");
    tif_ecr->WriteString("ECR","x_pay_cnt" ,  "0");

    tif_ecr->WriteString("X","XFILEDTTM", str_sys_date);    // XDT 之DTTM
    tif_ecr->WriteString("X","XFILENAME", str_xdt_path);
    tif_ecr->WriteString("X","XRPTNAME" , str_xrp_path);
    tif_ecr->WriteString("X","CHRRPTNAME" , CasherRptFile);
    
    //tif_tmp->WriteString("X","SALESLOGINDTTM" , sSalesLoginDttm );

    //delete x,tif_ecr;

    sTmp.printf("AUTOZDATA::AutoXData 交班完成:寫入本次交班交易序號 tif_tmp->X->PTRAN_CNT=%s, AutoZ_cnt=%s",sTtmpPTRAN_CNT.c_str(), AutoZ_cnt.c_str() );
    writelog(sTmp );

    return( xdt_1095_data);
}




////////////////////////////////////////////////////////////////////////////////

void __fastcall AUTOZDATA::mtn_dat_folder() //維護DAT下傳檔30日內
{
    String Dir;
    String FileName;
    TSearchRec SearchRec;
    int iAttributes = 0;
    iAttributes |= faAnyFile * true;

    AnsiString str_dat_path = "C:\\Abacus\\download\\dat\\";
    Dir = str_dat_path;

    TStringList *TempList = new TStringList;
    try
    {

        TempList->Clear();

        if (FindFirst(Dir+"*.tm", iAttributes, SearchRec) == 0)
        {
            do
            {
                if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
                {
                    TempList->Add(SearchRec.Name);
                }
            } while (FindNext(SearchRec) == 0);

            FindClose(SearchRec);
        }

        AnsiString str_dd_flag = FormatDateTime("yymmdd",Now()-30);
        AnsiString str_dd_file;

        for ( int i = 0; i <= (TempList->Count-1); i++)
        {
            FileName = TempList->Strings[i];

            if (FileName.Length() < 13)
            {
                TempList->Delete(i);
                i = 0;
                continue;
            }

            str_dd_file = FileName.SubString(3,6);

            if (str_dd_file.AnsiCompare(str_dd_flag) < 0)
            {
                FileName = (Dir + TempList->Strings[i]);
                DeleteFile(FileName);
                writelog("AUTOZDATA:維護DAT刪除檔案 " + FileName);
            }
        }
    }
    __finally
    {
        delete TempList; // destroy the list object
    }

}

void __fastcall AUTOZDATA::del_list_file(TStringList *list, AnsiString path)
{
    AnsiString str_sal_00;

    for (int i=0; i<list->Count; i++)
    {
        str_sal_00 = (path + list->Strings[i]);
        DeleteFile(str_sal_00);
        writelog("AUTOZDATA:三十天目錄維護刪除檔案 " + str_sal_00);
    }

    list->Clear();
}

AnsiString __fastcall AUTOZDATA::write_xpath(AnsiString FileName)
{
    AnsiString str_file_dd,                //日期
               str_file_num,               //次數
               str_bak_path;                //備份路徑


    AnsiString str_his_folder = "C:\\FTLPOS\\HISTORY\\";
    AnsiString str_abs_file = ExtractFileName(FileName);

    str_file_dd  = str_abs_file.SubString(7,2);
    str_file_num = str_abs_file.SubString(9,2);

    //備份路徑
    if ( (!str_file_dd.IsEmpty()) && (!str_file_num.IsEmpty()) )
        str_bak_path = ( str_his_folder + str_file_dd + "\\" + str_file_num + "\\" );
    else
        str_bak_path = str_his_folder;

    //建立目錄
    if (!DirectoryExists(str_bak_path))
        ForceDirectories(str_bak_path);

    return str_bak_path;

}
