//---------------------------------------------------------------------------

#ifndef ULOGH
#define ULOGH

#include <stdio.h>
#include <FileCtrl.hpp>
#include <inifiles.hpp>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>

#define const_mark " V"
//#define _ver "★★XZDATA.DLL FOR SC2 V93.1014★★"
#define _ver "★★XZDATA.DLL V2017.0426★★"
#define OK 1
#define ZERO 0
#define MAXDATALEN 2048
#define SALVER "2012090100"              //"2012070100"
#define MaxDropRec     60


#define REC0199LENGTH  89   //57
#define REC10LENGTH    380
#define REC11LENGTH    327
#define REC13LENGTH    324
#define REC15LENGTH    98
#define REC40LENGTH    248
#define REC20LENGTH    242
#define REC25LENGTH    188
#define REC30LENGTH    308  //2005/07/01
#define REC31LENGTH    546   
#define REC35LENGTH    235  //2005/07/01
#define REC41LENGTH    471  //182
#define REC50LENGTH    373
#define REC51LENGTH    130
#define REC97LENGTH    103
#define REC98LENGTH    109

#define XZDATA_INI "C:\\FTLPOS\\INI\\XZDATA.INI"
#define WORKDIR "C:\\FTLPOS\\"
#define XZDATADIR "C:\\FTLPOS\\XZDATA\\"
#define XDATATMP_INI  "C:\\FTLPOS\\XZDATA\\XDT_TMP.INI"
#define ZDATATMP_INI  "C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI"
#define XZDATASAL "C:\\FTLPOS\\XZDATA\\SAL\\"

#define TRANCNT_NO_POS 6 

int __fastcall _StrToInt(AnsiString S, AnsiString Data = " ");

void __fastcall writelog(AnsiString S);
int __stdcall file_cat( char *sour, char *targ );

AnsiString __fastcall get_version(AnsiString S);

AnsiString __fastcall _StringSegment(AnsiString Str , AnsiString Comma , int Seg);

AnsiString __fastcall GetNumber(AnsiString S);

AnsiString __fastcall _StringSegment_EX(AnsiString &Str , AnsiString Comma , int Seg, int idelStr=0);

int __fastcall _GetZCntLst(TStringList *ZCntLst, TStringList *S1050Lst );
int __fastcall _GetZCntLstCheck(TStringList *ZCntLst, AnsiString OrgZcnt);
int __fastcall _GetZCNT_SalData(AnsiString AutoZCnt);

// 檢查 1050 及 SAL 1050 筆數是否一致
int __fastcall _GetZDTZSALCntCheck(TStringList *ZCntLst, AnsiString SalFileName, AnsiString OrgZcnt,
                                   int &RtnZDTCnts, int &RtnSALCnts );


//部門別集計TABLE
typedef struct
    {
        int am_dpsitm;              // 部門交易項數
        int am_dpsale;              // 部門銷售金額
        int am_dpmitm;              // 部門誤退項數
        int am_dpmiss;              // 部門誤退金額

        int am_mmDis;               // 部門促銷折扣金額        Lu 2018/06/13
        int am_mmDismiss;           // 部門促銷折扣誤退金額    Lu 2018/06/13
    }DEPT;

//即時連線商品集計TABLE
typedef struct
    {
        int qt_InqSale;              // 即時連線商品交易數量
        int am_InqSale;              // 即時連線商品交易金額
    }INQTBL;


typedef struct
    {
        int IndexNo;                //投庫序號
        int PayCashAmt;           //投庫現金金額
    }PAYCASHREC;



class BASIC
{
public:                
    //資料暫存檔
    TStringList *gtsl_rec, *gtsl_tot, *gtsl_drop, *gtsl_easy_card, *gtsl_pay_sub;
    TStringList *tsl_subsale,*gtsl_bil, *gtsl_fetc_card, *gtsl_1051_sal;
    TStringList *Autotsl_rec, *Autotsl_tot, *Autotsl_drop, *Autotsl_easy_card, *Autotsl_pay_sub;
    TStringList *Autotsl_subsale,*Autotsl_bil, *Autotsl_fetc_card, *Auto_Sal, *Auto_1051_sal ;
    TStringList *Autotsl_Ticket, *gtsl_Ticket;

    char gchar_tencode[7];        //店別
    char gchar_ecr_no[5];         //機號
    char gchar_cashier[6];        //收銀員
    char gchar_date_time[15];     //現在時間
    char gchar_orgtencode[7];        //原始店號


    char gchar_x_trans[6];        //上次收銀帳序號
    char gpchar_x_first[MAXDATALEN];         //380 X帳第一筆SAL LOG
    char gpchar_sal_last[MAXDATALEN];        //380 SAL最後一筆
    char gpchar_sal_first[MAXDATALEN];       //380 SAL第一筆
    String gsLast1099;      //    SAL第一筆 1099
    //20021223
    int pay_bill_cnt;              //門市提貨券次數
    int pay_bill_amt;              //門市提貨券金額
    int mpay_bill_cnt;              //負向門市提貨券次數
    int mpay_bill_amt;              //負向門市提貨券金額

    //20030125
    int Cpn71_cnt;                  //折價券71次數
    int Cpn71_amt;                  //折價券71金額
    int Cpn72_cnt;                  //折價券72次數
    int Cpn72_amt;                  //折價券72金額
    int Cpn73_cnt;                  //折價券73次數
    int Cpn73_amt;                  //折價券73金額
    int Cpn74_cnt;                  //折價券74次數
    int Cpn74_amt;                  //折價券74金額
    int Cpn75_cnt;                  //折價券75次數
    int Cpn75_amt;                  //折價券75金額
    int Cpn76_cnt;                  //折價券76次數
    int Cpn76_amt;                  //折價券76金額
    int Cpn77_cnt;                  //門市提貨券77次數
    int Cpn77_amt;                  //門市提貨券77金額
    int Cpn78_cnt;                  //門市提貨券78次數
    int Cpn78_amt;                  //門市提貨券78金額
    AnsiString StrBillPay ;

    int giA;  //暫存變數ABCDEFGH
    int giB;
    int giC;
    int giD;
    int giE;
    int giF;
    int giG;
    int giH;
    int giI;
    int giJ;

    int giK;
    int giL;
    int giM;
    int giN;

    int giO;
    int giP;
    int giQ;
    int giR;

    int giS;
    int giT;
    int giU;
    int giV;
    int giW;
    int giX;
    int giY;
    int giZ;

    int giFetcCashAddCnt;
    int giFetcCashAddamt;
    int giFetcCredAddCnt;
    int giFetcCredAddamt;
    int giFetcSumCnt;
    int giFetcSumAmt;


    struct {                            /* 03 代售付ｔａｂｌｅ       */
          struct {                      /* 05 代售付品項ｉｔｅｍ  */
                  int  iqt_subrev;      /*       07 項目交易回數     */
                  int  iam_subrev;      /*       07 項目交易金額     */
          } it8_subrev[10];
          int  iqt5_totrev;             /*    05 代售付ＭＥＭＵ合計回數  */
          int  iam5_totrev;             /*    05 代售付ＭＥＭＵ合計金額  */
       } BaseTbSubrev[20];

    int gi_mix;                      //混登項目(包含代(收售付)、一般銷售)
    int gi_open_x;                   //開錢櫃次數-收銀員帳表    
    int gi_open_z;                   //開錢櫃次數-Z帳表、讀帳表

    bool gbl_tot_haveline;           //TOT是否有交易資料
    bool gbl_rec_haveline;           //REC
    bool gbl_bil_haveline;
    bool gbl_easy_haveline;           //easy card
    bool gbl_pay_haveline;
    bool gbl_fetc_haveline;           //Fetc card
    //bool gbl_0041_haveline;           //投庫
    bool gbl_1051_haveline;           //1051電文
    bool gbl_Ticket_haveline;

    int gi_tot_line;                 //TOT檔總行數
    int gi_rec_line;                 //REC
    int gi_bil_line;                 //bil
    int gi_easy_line;                //EASY CARD
    int gi_pay_line;
    int gi_fetc_line;                //Fetc EASY CARD
    int gi_1051_line;                //計算1051行數
    //int gi_0041_line;                //投庫
    int gi_Ticket_Line;              //1805, 1806, 1807

    int __fastcall subsale_update(TStringList * tsl, AnsiString good_no,
                            AnsiString menu_no, AnsiString bar_name,
                            AnsiString item_amt, AnsiString sal_type);

   /********************
    //部門別集計TABLE
    typedef struct
    {
        int am_dpsitm;              // 部門交易項數
        int am_dpsale;              // 部門銷售金額
        int am_dpmitm;              // 部門誤退項數
        int am_dpmiss;              // 部門誤退金額
    }DEPT;

    //即時連線商品集計TABLE
    typedef struct
    {
        int qt_InqSale;              // 即時連線商品交易數量
        int am_InqSale;              // 即時連線商品交易金額
    }INQTBL;
  **********************/

protected:

    int __fastcall loadsalfile(AnsiString FileName, bool for_x = false, AnsiString AutoZ_Cnt = "",
                               AnsiString AutoZ_Dt = "" ); //開啟SAL檔，將內容分類
    bool __fastcall CheckSaleRecData(AnsiString RecData) ;

    AnsiString __fastcall BackupFile(AnsiString FileName,  AnsiString BackupFileName="", AnsiString DDZ="");      //備份檔案至C:\HISTORY

    int __fastcall SumQt_tcust(int StartLine,int TotalLine);   //本日來客數

    int __fastcall SumQt_trans(int StratLine, int TotalLine);  //交易品項數

    void __fastcall TotCount(int StartLine, int TotalLine);    //發票日結帳

    void __fastcall TotMiss(int StartLine, int TotalLine);     //誤打發票
    void __fastcall TotMissCnt(int StartLine, int TotalLine);   //誤打發票張數

    int __fastcall SumCash(int StartLine, int TotalLine);      //現金合計

    int __fastcall BASIC::SumNOVoiceAmt(int StartLine, int TotalLine);  //不開發票金額

    int __fastcall BASIC::VoicePayAmt(int StartLine, int TotalLine);     //發票兌獎金額

    int __fastcall SumInpdp(int StartLine, int TotalLine);     //手輸部門

    int __fastcall BASIC::SumPreSal_MM_DisAmtTMMFP(int StartLine, int TotalLine);
    int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int StartLine, int TotalLine);   //預售類(折價券/MM)折扣

    int __fastcall BASIC::SumPreSal_INVO_DisAmt(int StartLine, int TotalLine);    //預售兌換商品折讓單折扣/代收商品折讓單折扣

    int __fastcall BASIC::SumPreSal_Amt(int StartLine, int TotalLine); //預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票

    int __fastcall SumRelDimAgn(int StartLine, int TotalLine); //即時購代售

    int __fastcall TotZero(int StartLine, int TotalLine);      //發票金額零

    int __fastcall SumVoid(int StartLine, int TotalLine);      //交易取消

    int __fastcall SumUpdat(int StartLine, int TotalLine);     //更正

    int __fastcall SumLOSS(int StartLine, int TotalLine);      //廢棄

    int __fastcall SumTbCash(int StartLine, int TotalLine);    //代現金  刷卡合計

    int __fastcall BASIC::SumBill78(int StartLine, int TotalLine); //計算門市提貨券

    int __fastcall SumKamt(int StartLine, int TotalLine);      //代收、售、付 ,點卡手續費giD.作廢giE  

    int __fastcall SumHealthAmt(int StartLine, int TotalLine);

    void __fastcall sub_times(int StartLine, int TotalLine);   //代收、售、付次數統計

    AnsiString __fastcall invoice_start_end(int tot_line_count, bool invoice_start);     //發票結束號碼

    int __fastcall WriteSAL(AnsiString data, AnsiString SALFileName); //97,98
    int __fastcall WriteSAL_A(AnsiString data,AnsiString SALFileName);

    int __fastcall easy_card(int StartLine, int TotalLine);

    int __fastcall fetc_card(int StartLine, int TotalLine);

    void __fastcall SumDisc(int StartLine, int TotalLine);      //小計折扣

    int __fastcall SumBillCSM(int StartLine, int TotalLine);    //計算2008/12/04 消費券

    int __fastcall BASIC::SumBillCPN(int StartLine, int TotalLine);  //計算2017/03/08 CPN

    void __fastcall SumAllDept(DEPT *dept);                     //部門帳

    void __fastcall BASIC::INQSubTotal(INQTBL *inqtbl) ;         // 即時連線商品集計

    void __fastcall init_data();                                //取得POS基本資料

    void __fastcall list_filename(TStringList *tsl_file_list, AnsiString dir, AnsiString type);

    void __fastcall pay_sub_amt(int StartLine, int TotalLine);  //折扣讓

    //接班明細:信用卡簽單張數
    //新增：信用卡簽單張數依1028.FG_SIGN=’1’
    void __fastcall Sig_Cnt(int StartLine, int TotalLine);

    //劃位票券退票張數
    //依1805.FU_SPACE第3碼='1'及1806~1807.FU_SPACE 第1碼='1'，判斷劃位票券退票張數。
    void __fastcall Rtn_Ticket(int StartLine, int TotalLine);

    void __fastcall Sum_BaseTbSubrev(AnsiString menu_no, AnsiString item_amt,
                                    AnsiString item_qty,int NegTive=1 );

    int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sXdtFile,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1099,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update=0);

                                    //收銀員交班明細表 for 切帳用
    int __fastcall BASIC::SumCasherRptXDT(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1095,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update );

    void __fastcall BASIC::S1051MissCnt(int StartLine, int TotalLine, AnsiString &sMissRec);


    // 重點商品列管帳表
    //int __fastcall BASIC::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey);

    int __fastcall BASIC::read_ten_name(AnsiString &sStoreNo, AnsiString &sStoreName);                               
    void __fastcall BASIC::scan_fixstr(FILE *f, register char *str, register int len);
    //void __fastcall _DelTree(AnsiString dir);
    
    //void __fastcall _DeleteAllFile(String Directory , String Mask);
    //即時購合計
    int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine);

    void __fastcall Sum040RecycleDataCnt(int StartLine, int TotalLine);   //廢資源回收數 /提貨券金額/提貨券張數

    int __fastcall BASIC::SumECP2Orher2Amt(int StartLine, int TotalLine);

    int __fastcall  BASIC::GetZCNT_SalData(AnsiString AutoZCnt);

    // 外加手續費,即時購代售點卡集計
    void __fastcall BASIC::Sub_C0InvoAmt(int StartLine, int TotalLine);
    // 點卡手續費應免稅
    void __fastcall BASIC::Sub_C0C1TaxInvoAmt(int StartLine, int TotalLine);

    // 點卡手續費數量
    void __fastcall BASIC::Sub_C0C1TaxInvoQty(int StartLine, int TotalLine);

    // 外加手續費,即時購代售點卡集計 以連線區分分類
    void __fastcall BASIC::Sub_C0InvoAmt4VDC(int StartLine, int TotalLine);


    ~BASIC();

    BASIC();

};


//---------------------------------------------------------------------------
#endif
