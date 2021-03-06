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
//#define _ver "々々XZDATA.DLL FOR SC2 V93.1014々々"
#define _ver "々々XZDATA.DLL V2017.0426々々"
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

// 惜?d 1050 の SAL 1050 祈柴?O?_?@?P
int __fastcall _GetZDTZSALCntCheck(TStringList *ZCntLst, AnsiString SalFileName, AnsiString OrgZcnt,
                                   int &RtnZDTCnts, int &RtnSALCnts );


//魁???O彊?pTABLE
typedef struct
    {
        int am_dpsitm;              // 魁??ユ??教柴
        int am_dpsale;              // 魁???P謂???B
        int am_dpmitm;              // 魁???~?h教柴
        int am_dpmiss;              // 魁???~?h???B

        int am_mmDis;               // 魁???P?PчΙ???B        Lu 2018/06/13
        int am_mmDismiss;           // 魁???P?PчΙ?~?h???B    Lu 2018/06/13
    }DEPT;

//?Y?紐s?u尉?~彊?pTABLE
typedef struct
    {
        int qt_InqSale;              // ?Y?紐s?u尉?~ユ??柴?q
        int am_InqSale;              // ?Y?紐s?u尉?~ユ?????B
    }INQTBL;


typedef struct
    {
        int IndexNo;                //щ?w?埜?
        int PayCashAmt;           //щ?w?{?????B
    }PAYCASHREC;



class BASIC
{
public:                
    //瑚?銅筈s隻
    TStringList *gtsl_rec, *gtsl_tot, *gtsl_drop, *gtsl_easy_card, *gtsl_pay_sub;
    TStringList *tsl_subsale,*gtsl_bil, *gtsl_fetc_card, *gtsl_1051_sal;
    TStringList *Autotsl_rec, *Autotsl_tot, *Autotsl_drop, *Autotsl_easy_card, *Autotsl_pay_sub;
    TStringList *Autotsl_subsale,*Autotsl_bil, *Autotsl_fetc_card, *Auto_Sal, *Auto_1051_sal ;
    TStringList *Autotsl_Ticket, *gtsl_Ticket;

    char gchar_tencode[7];        //?宇O
    char gchar_ecr_no[5];         //穣弦
    char gchar_cashier[6];        //Μ使??
    char gchar_date_time[15];     //?{?b?俵?
    char gchar_orgtencode[7];        //?讒l?姥?


    char gchar_x_trans[6];        //?WΩΜ使?b?埜?
    char gpchar_x_first[MAXDATALEN];         //380 X?b可?@祈SAL LOG
    char gpchar_sal_last[MAXDATALEN];        //380 SAL殻?瓩@祈
    char gpchar_sal_first[MAXDATALEN];       //380 SAL可?@祈
    String gsLast1099;      //    SAL可?@祈 1099
    //20021223
    int pay_bill_cnt;              //??カ苅?f?薤玄?
    int pay_bill_amt;              //??カ苅?f?薨??B
    int mpay_bill_cnt;              //?t?V??カ苅?f?薤玄?
    int mpay_bill_amt;              //?t?V??カ苅?f?薨??B

    //20030125
    int Cpn71_cnt;                  //ч児??71Ω柴
    int Cpn71_amt;                  //ч児??71???B
    int Cpn72_cnt;                  //ч児??72Ω柴
    int Cpn72_amt;                  //ч児??72???B
    int Cpn73_cnt;                  //ч児??73Ω柴
    int Cpn73_amt;                  //ч児??73???B
    int Cpn74_cnt;                  //ч児??74Ω柴
    int Cpn74_amt;                  //ч児??74???B
    int Cpn75_cnt;                  //ч児??75Ω柴
    int Cpn75_amt;                  //ч児??75???B
    int Cpn76_cnt;                  //ч児??76Ω柴
    int Cpn76_amt;                  //ч児??76???B
    int Cpn77_cnt;                  //??カ苅?f??77Ω柴
    int Cpn77_amt;                  //??カ苅?f??77???B
    int Cpn78_cnt;                  //??カ苅?f??78Ω柴
    int Cpn78_amt;                  //??カ苅?f??78???B
    AnsiString StrBillPay ;

    int giA;  //屡?s怒柴ABCDEFGH
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


    struct {                            /* 03 ?N謂?I??∫∬♯??       */
          struct {                      /* 05 ?N謂?I?~教?顴??蹉?  */
                  int  iqt_subrev;      /*       07 教ヘユ???^柴     */
                  int  iam_subrev;      /*       07 教ヘユ?????B     */
          } it8_subrev[10];
          int  iqt5_totrev;             /*    05 ?N謂?I?暸哂暸礒X?p?^柴  */
          int  iam5_totrev;             /*    05 ?N謂?I?暸哂暸礒X?p???B  */
       } BaseTbSubrev[20];

    int gi_mix;                      //?V?n教ヘ(?]?t?N(Μ謂?I)?B?@?訃P謂)
    int gi_open_x;                   //?}椙?dΩ柴-Μ使???b??    
    int gi_open_z;                   //?}椙?dΩ柴-Z?b?蹇B的?b??

    bool gbl_tot_haveline;           //TOT?O?_Τユ??瑚??
    bool gbl_rec_haveline;           //REC
    bool gbl_bil_haveline;
    bool gbl_easy_haveline;           //easy card
    bool gbl_pay_haveline;
    bool gbl_fetc_haveline;           //Fetc card
    //bool gbl_0041_haveline;           //щ?w
    bool gbl_1051_haveline;           //1051?qゅ
    bool gbl_Ticket_haveline;

    int gi_tot_line;                 //TOT隻?`?羲?
    int gi_rec_line;                 //REC
    int gi_bil_line;                 //bil
    int gi_easy_line;                //EASY CARD
    int gi_pay_line;
    int gi_fetc_line;                //Fetc EASY CARD
    int gi_1051_line;                //?p財1051?羲?
    //int gi_0041_line;                //щ?w
    int gi_Ticket_Line;              //1805, 1806, 1807

    int __fastcall subsale_update(TStringList * tsl, AnsiString good_no,
                            AnsiString menu_no, AnsiString bar_name,
                            AnsiString item_amt, AnsiString sal_type);

   /********************
    //魁???O彊?pTABLE
    typedef struct
    {
        int am_dpsitm;              // 魁??ユ??教柴
        int am_dpsale;              // 魁???P謂???B
        int am_dpmitm;              // 魁???~?h教柴
        int am_dpmiss;              // 魁???~?h???B
    }DEPT;

    //?Y?紐s?u尉?~彊?pTABLE
    typedef struct
    {
        int qt_InqSale;              // ?Y?紐s?u尉?~ユ??柴?q
        int am_InqSale;              // ?Y?紐s?u尉?~ユ?????B
    }INQTBL;
  **********************/

protected:

    int __fastcall loadsalfile(AnsiString FileName, bool for_x = false, AnsiString AutoZ_Cnt = "",
                               AnsiString AutoZ_Dt = "" ); //?}衛SAL隻?A?Nず?eだ寵
    bool __fastcall CheckSaleRecData(AnsiString RecData) ;

    AnsiString __fastcall BackupFile(AnsiString FileName,  AnsiString BackupFileName="", AnsiString DDZ="");      //各??隻?廢?C:\HISTORY

    int __fastcall SumQt_tcust(int StartLine,int TotalLine);   //セら?哽伴?

    int __fastcall SumQt_trans(int StratLine, int TotalLine);  //ユ???~教柴

    void __fastcall TotCount(int StartLine, int TotalLine);    //?o下ら飢?b

    void __fastcall TotMiss(int StartLine, int TotalLine);     //?~ゴ?o下
    void __fastcall TotMissCnt(int StartLine, int TotalLine);   //?~ゴ?o下?i柴

    int __fastcall SumCash(int StartLine, int TotalLine);      //?{???X?p

    int __fastcall BASIC::SumNOVoiceAmt(int StartLine, int TotalLine);  //ぃ?}?o下???B

    int __fastcall BASIC::VoicePayAmt(int StartLine, int TotalLine);     //?o下?I綬???B

    int __fastcall SumInpdp(int StartLine, int TotalLine);     //も翠魁??

    int __fastcall BASIC::SumPreSal_MM_DisAmtTMMFP(int StartLine, int TotalLine);
    int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int StartLine, int TotalLine);   //?w謂寵(ч児??/MM)чΙ

    int __fastcall BASIC::SumPreSal_INVO_DisAmt(int StartLine, int TotalLine);    //?w謂?I勧尉?~ч統恰чΙ/?NΜ尉?~ч統恰чΙ

    int __fastcall BASIC::SumPreSal_Amt(int StartLine, int TotalLine); //?w謂の?I勧尉?~歌?p : ?w謂?}?o下ぃ?J仙Μ ,  ?I勧?J仙Μぃ?}?o下

    int __fastcall SumRelDimAgn(int StartLine, int TotalLine); //?Y?描淵N謂

    int __fastcall TotZero(int StartLine, int TotalLine);      //?o下???B?s

    int __fastcall SumVoid(int StartLine, int TotalLine);      //ユ??????

    int __fastcall SumUpdat(int StartLine, int TotalLine);     //?鵐?

    int __fastcall SumLOSS(int StartLine, int TotalLine);      //?o遠

    int __fastcall SumTbCash(int StartLine, int TotalLine);    //?N?{??  ?螢d?X?p

    int __fastcall BASIC::SumBill78(int StartLine, int TotalLine); //?p財??カ苅?f??

    int __fastcall SumKamt(int StartLine, int TotalLine);      //?NΜ?B謂?B?I ,?I?dも挺?OgiD.?@?ogiE  

    int __fastcall SumHealthAmt(int StartLine, int TotalLine);

    void __fastcall sub_times(int StartLine, int TotalLine);   //?NΜ?B謂?B?IΩ柴歌?p

    AnsiString __fastcall invoice_start_end(int tot_line_count, bool invoice_start);     //?o下飢??弦?X

    int __fastcall WriteSAL(AnsiString data, AnsiString SALFileName); //97,98
    int __fastcall WriteSAL_A(AnsiString data,AnsiString SALFileName);

    int __fastcall easy_card(int StartLine, int TotalLine);

    int __fastcall fetc_card(int StartLine, int TotalLine);

    void __fastcall SumDisc(int StartLine, int TotalLine);      //?p?pчΙ

    int __fastcall SumBillCSM(int StartLine, int TotalLine);    //?p財2008/12/04 ???O??

    int __fastcall BASIC::SumBillCPN(int StartLine, int TotalLine);  //?p財2017/03/08 CPN

    void __fastcall SumAllDept(DEPT *dept);                     //魁???b

    void __fastcall BASIC::INQSubTotal(INQTBL *inqtbl) ;         // ?Y?紐s?u尉?~彊?p

    void __fastcall init_data();                                //???oPOS芋セ瑚??

    void __fastcall list_filename(TStringList *tsl_file_list, AnsiString dir, AnsiString type);

    void __fastcall pay_sub_amt(int StartLine, int TotalLine);  //чΙ統

    //欝?Z??禾:?Hノ?d単恰?i柴
    //?s?W?G?Hノ?d単恰?i柴??1028.FG_SIGN=・1・
    void __fastcall Sig_Cnt(int StartLine, int TotalLine);

    //杭?豌示薜h下?i柴
    //??1805.FU_SPACE可3?X='1'の1806~1807.FU_SPACE 可1?X='1'?A?P?_杭?豌示薜h下?i柴?C
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

                                    //Μ使??ユ?Z??禾?? for ち?bノ
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


    // ???I尉?~?C剤?b??
    //int __fastcall BASIC::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey);

    int __fastcall BASIC::read_ten_name(AnsiString &sStoreNo, AnsiString &sStoreName);                               
    void __fastcall BASIC::scan_fixstr(FILE *f, register char *str, register int len);
    //void __fastcall _DelTree(AnsiString dir);
    
    //void __fastcall _DeleteAllFile(String Directory , String Mask);
    //?Y?描弗X?p
    int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine);

    void __fastcall Sum040RecycleDataCnt(int StartLine, int TotalLine);   //?o瑚圭?^Μ柴 /苅?f?薨??B/苅?f?蕷i柴

    int __fastcall BASIC::SumECP2Orher2Amt(int StartLine, int TotalLine);

    int __fastcall  BASIC::GetZCNT_SalData(AnsiString AutoZCnt);

    // ?~?[も挺?O,?Y?描淵N謂?I?d彊?p
    void __fastcall BASIC::Sub_C0InvoAmt(int StartLine, int TotalLine);
    // ?I?dも挺?O棲?K?|
    void __fastcall BASIC::Sub_C0C1TaxInvoAmt(int StartLine, int TotalLine);

    // ?I?dも挺?O柴?q
    void __fastcall BASIC::Sub_C0C1TaxInvoQty(int StartLine, int TotalLine);

    // ?~?[も挺?O,?Y?描淵N謂?I?d彊?p ?H?s?u囲だだ寵
    void __fastcall BASIC::Sub_C0InvoAmt4VDC(int StartLine, int TotalLine);


    ~BASIC();

    BASIC();

};


//---------------------------------------------------------------------------
#endif
