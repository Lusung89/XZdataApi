//---------------------------------------------------------------------------

#ifndef ULOGH
#define ULOGH


#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  

#include <codecvt>
#include <atlstr.h> 
#include <tchar.h>
#include <string>
//#include <cstring>
#include <sstream> 
#include <fstream>
#include <vector>
//#include <experimental/filesystem> // or #include <filesystem>

#include <list>
#include <iostream>
#include "Shlwapi.h"
#include "Shlobj.h"
#include <time.h>
#include "INI.h"
//#include <locale.h>
//#include <mbstring.h>


using namespace std;
//using namespace boost;
//using namespace System::IO;
//using namespace
//using std::list;

typedef std::list<string> TStringList;    //for (list<string>::iterator ls = xxList->begin(); ls != xxList->end(); ls++)  string str=*ls;

//#define const_mark " V"
#define _ver "★★XZdataApi.DLL V2018.0101.01★★"
#define OK 1
#define ZERO 0
#define MAXDATALEN 4096
#define SALVER "2012090100"               
#define MaxDropRec     60


#define REC0199LENGTH  89    
#define REC10LENGTH    380
#define REC11LENGTH    327
#define REC13LENGTH    324
#define REC15LENGTH    98
#define REC40LENGTH    248
#define REC20LENGTH    242
#define REC25LENGTH    188
#define REC30LENGTH    308   
#define REC31LENGTH    546   
#define REC35LENGTH    235   
#define REC41LENGTH    471   
#define REC50LENGTH    373
#define REC51LENGTH    130
#define REC97LENGTH    103
#define REC98LENGTH    109

#define XZDATA_INI    "C:\\FTLPOS\\INI\\XZDATA.INI"
#define WORKDIR       "C:\\FTLPOS\\"
#define XZDATADIR     "C:\\FTLPOS\\XZDATA\\"
#define XDATATMP_INI  "C:\\FTLPOS\\XZDATA\\XDT_TMP.INI"
#define ZDATATMP_INI  "C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI"
#define XZDATASAL     "C:\\FTLPOS\\XZDATA\\SAL\\"
#define SGM           "||" 

#define TRANCNT_NO_POS 5   // ex:0002159803  substr(..) start from 0 ~ N-1 ,,  BCB start from 1~N


#define _IntToStr             int2str
#define _StringSegment_EX    _WStringSegment

string  __stdcall RTrim(const string& str);
string  __stdcall LTrim(const string& str);
string  __stdcall Trim(const string& str);
string  __stdcall Trim(const LPSTR  str);

string __stdcall int2str(int i);
string __stdcall TcharToString(TCHAR *pTstr);
bool __stdcall _StrFind(const std::string &str, const std::string &sfind);
int __stdcall _Pos(const std::string &str, const std::string &sfind);

string __stdcall WStringToString(const std::wstring& s);
wstring __stdcall StringToWString(const std::string& s);

std::wstring __stdcall s2ws(const std::string& str);
std::string __stdcall ws2s(const std::wstring& wstr);


int __stdcall _StrToInt(string S, string Data = " ");  //stoi(S); //stof(s)
string __stdcall GetNumber(string S);
string  __stdcall currentDateTime(int iDttmTypw, int OffsetDay = 0);

void  __stdcall writelog(LPSTR pstr);
void  __stdcall writelog(string S);
void __stdcall logsprintf(char* format, ...);
string __stdcall _Strsprintf(std::string &str, char* format, ...);

int __stdcall file_cat(const char *sour, const char *targ);
string __stdcall _ChangeFileExt(const std::string& str, string& newExt);
bool __stdcall DirExists(const std::string& dirName_in);
string __stdcall _ExtractFilePath(string str);
string __stdcall _ExtractFile(string str);


int __stdcall GetTextFileType(const std::string& FileName);

template< typename T, typename A >
void _ListToFile(const std::list<T, A>* seq, const char* path2file);

//for Unicode file
void _ListToWFile(TStringList* seq, string path2file);

int _wListLoadFromFile(TStringList* seq, string path2file);

template< typename T, typename A >
int _ListLoadFromFile(std::list<T, A>* seq, const char* path2file);
int __stdcall _SaveZCntToFile(string InPutSalFile, int Z_Cnt, string OutPutSalFile);

string __stdcall get_version(string S);

//std::vector<std::string> split(const std::string& s, char delimiter);
string __stdcall _StringSegment(string &Str, string delimiter, int Seg, int idelStr = 0);
string __stdcall _WStringSegment(string Str, string delimiter, int Seg, int idelStr = 0);
wstring __stdcall _WStringSegmentW(wstring Str, wstring delimiter, int Seg, int idelSt = 0);

int __stdcall _GetZCntLst(TStringList *ZCntLst, TStringList *S1050Lst );
int __stdcall _GetZCntLstCheck(TStringList *ZCntLst, string OrgZcnt);
//int __stdcall _GetZCNT_SalData(string AutoZCnt);

//// 檢查 1050 及 SAL 1050 筆數是否一致
int __fastcall _GetZDTZSALCntCheck(TStringList *ZCntLst, string SalFileName, string OrgZcnt,
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

	CIniReader m_IniReader;
	
    //資料暫存檔
    TStringList *gtsl_rec, *gtsl_tot, *gtsl_drop, *gtsl_easy_card, *gtsl_pay_sub;
    TStringList *tsl_subsale,*gtsl_bil, *gtsl_fetc_card, *gtsl_1051_sal;
    TStringList *Autotsl_rec, *Autotsl_tot, *Autotsl_drop, *Autotsl_easy_card, *Autotsl_pay_sub;
    TStringList *Autotsl_subsale,*Autotsl_bil, *Autotsl_fetc_card, *Auto_Sal, *Auto_1051_sal ;


    //char gchar_tencode[7];        //店別
    string g_tencode;        //店別
    //char gchar_ecr_no[5];         //機號
    string g_ecr_no;         //機號
    //char gchar_cashier[6];        //收銀員
    string g_cashier;
    //char gchar_date_time[15];     //現在時間
    string g_date_time;             //現在時間
    //char gchar_orgtencode[7];        //原始店號
    string g_orgtencode;
    //char gchar_x_trans[6];        //上次收銀帳序號
    string gx_trans;

    //char gpchar_x_first[MAXDATALEN];         //380 X帳第一筆SAL LOG
    //char gpchar_sal_last[MAXDATALEN];        //380 SAL最後一筆
    //char gpchar_sal_first[MAXDATALEN];       //380 SAL第一筆
    
    string gsalX_first;
    string gsal_last;
    string gsal_first;
    string gsLast1099;      //    SAL第一筆 1099
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
    string StrBillPay ;

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

    int gi_tot_line;                 //TOT檔總行數
    int gi_rec_line;                 //REC
    int gi_bil_line;                 //bil
    int gi_easy_line;                //EASY CARD
    int gi_pay_line;
    int gi_fetc_line;                //Fetc EASY CARD
    int gi_1051_line;                //計算1051行數
    //int gi_0041_line;                //投庫

    int __fastcall subsale_update(TStringList * tsl, string good_no,
                            string menu_no, string bar_name,
                            string item_amt, string sal_type);

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



	//void __fastcall BASIC::init_data();                                //取得POS基本資料
	//int __fastcall BASIC::loadsalfile(string FileName, bool for_x = false, string AutoZ_Cnt = "", string AutoZ_Dt = ""); //開啟SAL檔，將內容分類

	~BASIC();
	BASIC();

protected:
	void __fastcall BASIC::init_data();                                //取得POS基本資料
	int __fastcall BASIC::loadsalfile(string FileName, bool for_x = false, string AutoZ_Cnt = "", string AutoZ_Dt = ""); //開啟SAL檔，將內容分類
    int __fastcall BASIC::wloadsalfile(string FileName, bool for_x, string AutoZ_Cnt, string AutoZ_Dt); 
    bool __fastcall BASIC::CheckSaleRecData(string RecData);
	string __fastcall BASIC::BackupFile(string FileName, string BackupFileName = "", string DDZ = "");      //備份檔案至C:\HISTORY
    //int __fastcall BASIC::wloadsalfile(); // string FileName, bool for_x, string AutoZ_Cnt, string AutoZ_Dt); 
	////////////////// 1050 ///////////////////////

	int __fastcall BASIC::SumQt_tcust(int startLine, int TotalLine);   //本日來客數

	int __fastcall BASIC::SumQt_trans(int stratLine, int TotalLine);  //交易品項數

	void __fastcall BASIC::TotCount(int startLine, int TotalLine);    //發票日結帳

	void __fastcall BASIC::TotMiss(int startLine, int TotalLine);     //誤打發票
	void __fastcall BASIC::TotMissCnt(int startLine, int TotalLine);   //誤打發票張數

	int __fastcall BASIC::SumCash(int startLine, int TotalLine);      //現金合計

    int __fastcall BASIC::SumNOVoiceAmt(int StartLine, int TotalLine);  //不開發票金額

    int __fastcall BASIC::VoicePayAmt(int startLine, int TotalLine);     //發票兌獎金額

	int __fastcall BASIC::SumInpdp(int startLine, int TotalLine);     //手輸部門

    int __fastcall BASIC::SumPreSal_MM_DisAmtTMMFP(int startLine, int TotalLine);
    int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int startLine, int TotalLine);   //預售類(折價券/MM)折扣

    int __fastcall BASIC::SumPreSal_INVO_DisAmt(int startLine, int TotalLine);    //預售兌換商品折讓單折扣/代收商品折讓單折扣

    int __fastcall BASIC::SumPreSal_Amt(int startLine, int TotalLine); //預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票

	int __fastcall BASIC::SumRelDimAgn(int startLine, int TotalLine); //即時購代售

	int __fastcall BASIC::TotZero(int startLine, int TotalLine);      //發票金額零

	int __fastcall BASIC::SumVoid(int startLine, int TotalLine);      //交易取消

	int __fastcall BASIC::SumUpdat(int startLine, int TotalLine);     //更正

	int __fastcall BASIC::SumLOSS(int startLine, int TotalLine);      //廢棄

	int __fastcall BASIC::SumTbCash(int startLine, int TotalLine);    //代現金  刷卡合計

    int __fastcall BASIC::SumBill78(int startLine, int TotalLine); //計算門市提貨券

	int __fastcall BASIC::SumKamt(int startLine, int TotalLine);      //代收、售、付 ,點卡手續費giD.作廢giE  

	int __fastcall BASIC::SumHealthAmt(int startLine, int TotalLine);

	////////////////// 1010 ///////////////////////

	void __fastcall BASIC::sub_times(int startLine, int TotalLine);   //代收、售、付次數統計  

    string __fastcall invoice_start_end(int tot_line_count, bool invoice_start);     //發票結束號碼
		
	int __fastcall BASIC::easy_card(int startLine, int TotalLine);

	int __fastcall BASIC::fetc_card(int startLine, int TotalLine);

	void __fastcall BASIC::SumDisc(int startLine, int TotalLine);      //小計折扣

	int __fastcall BASIC::SumBillCSM(int startLine, int TotalLine);    //計算2008/12/04 消費券

    int __fastcall BASIC::SumBillCPN(int startLine, int TotalLine);  //計算2017/03/08 CPN

	void __fastcall BASIC::SumAllDept(DEPT *dept);                     //部門帳

    void __fastcall BASIC::INQSubTotal(INQTBL *inqtbl) ;         // 即時連線商品集計

    void __fastcall pay_sub_amt(int StartLine, int TotalLine);  //折扣讓 

	void __fastcall BASIC::Sum_BaseTbSubrev(string menu_no, string item_amt, string item_qty,int NegTive=1 );

    int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   string sTranCnt, string sXdtNo,
                                   string SCasherRpt,
                                   string sXdtFile,
                                   string sStr1001,
                                   string sStr1097,
                                   string sStr1099,
                                   string &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update=0);

                                    //收銀員交班明細表 for 切帳用
	int __fastcall BASIC::SumCasherRptXDT(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   string sTranCnt, string sXdtNo,
                                   string SCasherRpt,
                                   string sStr1001,
                                   string sStr1097,
                                   string sStr1095,
                                   string &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update );

	void __fastcall BASIC::S1051MissCnt(int StartLine, int TotalLine, string &sMissRec);
   
    //即時購合計
    int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine);

	void __fastcall BASIC::Sum040RecycleDataCnt(int StartLine, int TotalLine);   //廢資源回收數 /提貨券金額/提貨券張數

    int __fastcall BASIC::SumECP2Orher2Amt(int StartLine, int TotalLine);

    int __fastcall  BASIC::GetZCNT_SalData(string AutoZCnt);

    // 外加手續費,即時購代售點卡集計
    //void __fastcall BASIC::Sub_C0InvoAmt(int StartLine, int TotalLine);
    // 點卡手續費應免稅
    void __fastcall BASIC::Sub_C0C1TaxInvoAmt(int StartLine, int TotalLine);

    // 點卡手續費數量 Sub_C0C1TaxInvoQty(0, gi_rec_line) ==> 併入 Sub_C0C1TaxInvoAmt
    //void __fastcall BASIC::Sub_C0C1TaxInvoQty(int StartLine, int TotalLine);

    // 外加手續費,即時購代售點卡集計 以連線區分分類
    // Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    void __fastcall BASIC::Sub_C0InvoAmt4VDC(int StartLine, int TotalLine);

	int __fastcall BASIC::read_ten_name(string &sStoreNo, string &sStoreName);
	void __fastcall BASIC::scan_fixstr(FILE *f, register char *str, register int len);

    int __fastcall BASIC::w_WriteSAL(string data, string SALFileName, int mode);
	int __fastcall BASIC::WriteSAL(string data, string SALFileName); //97,98
	int __fastcall BASIC::WriteSAL_A(string data, string SALFileName);

};


//---------------------------------------------------------------------------
#endif
