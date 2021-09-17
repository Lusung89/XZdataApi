//---------------------------------------------------------------------------

#ifndef UReportH
#define UReportH
#include "ULOG.h"
#define ESC 0x1B

class REPORT
{
public:
    //bool gbl_epson_prt;

    TStringList *tslSaveToFile;                             //報表輸出暫存

    char char_filename[50];                                  //報表儲存輸出檔案名

    char gchar_date_format[11];   //現在日期

    REPORT();

    ~REPORT();

    // 部門促銷折扣金額        Lu 2018/06/13
    int __fastcall Dept(AnsiString &ZData, char *ptr_tencode, char *ptr_ecr_no, int XR=0,
                        TStringList *tsl=NULL, AnsiString s1093=NULL);   //部門帳表

    int __fastcall XDTReport(AnsiString &XData, AnsiString sPayStr, char *ptr_tencode,
                     int iqt5_15217totrev=0, int iam5_15217totrev=0,
                     int iqt5_18219totrev=0, int iam5_18219totrev=0, int AutoZflg=0 );                    //收銀員帳表

    int __fastcall ZDTReport(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev ,int rpt_type );                    //Z帳表

    int __fastcall XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no,
                            int iqt5_15217totrev=0, int iam5_15217totrev=0,
                            int iqt5_18219totrev=0, int iam5_18219totrev=0  );             //讀帳

    int __fastcall INQReport( char *ptr_tencode, char *ptr_ecr_no,
                                 INQTBL *InqTbl);


    void __fastcall to_ptr(AnsiString S);                   //報表輸出至檔案

    //AnsiString __fastcall GetNumber(AnsiString S);          //取得數值

    AnsiString __fastcall ToDateFormat(AnsiString S);       //轉換為日期格式

    AnsiString __fastcall InsertSpace(AnsiString S,int i);  //填空白

    AnsiString __fastcall ToTimeFormat(AnsiString S);      //轉換成時間格式

    // 部門促銷折扣金額        Lu 2018/06/13
    int CreateReport(char rpt_type, AnsiString sSal, AnsiString sPayStr,
                     char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev=0, int iam5_15217totrev=0,
                     int iqt5_18219totrev=0, int iam5_18219totrev=0,
                     TStringList *tsl = NULL, int AutoZflg=0, AnsiString s1093=NULL);

    int __fastcall subsale(TStringList *tsl, char *ptr_tencode, char *ptr_ecr_no);

};

//---------------------------------------------------------------------------
extern REPORT *report;
#endif
