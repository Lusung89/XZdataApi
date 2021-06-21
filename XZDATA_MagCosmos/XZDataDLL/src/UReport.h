//---------------------------------------------------------------------------

#ifndef UReportH
#define UReportH
#include "ULOG.h"
#define ESC 0x1B

class REPORT
{
public:
    //bool gbl_epson_prt;

    TStringList *tslSaveToFile;                             //�����X�Ȧs

    char char_filename[50];                                  //�����x�s��X�ɮצW

    char gchar_date_format[11];   //�{�b���

    REPORT();

    ~REPORT();

    // �����P�P�馩���B        Lu 2018/06/13
    int __fastcall Dept(AnsiString &ZData, char *ptr_tencode, char *ptr_ecr_no, int XR=0,
                        TStringList *tsl=NULL, AnsiString s1093=NULL);   //�����b��

    int __fastcall XDTReport(AnsiString &XData, AnsiString sPayStr, char *ptr_tencode,
                     int iqt5_15217totrev=0, int iam5_15217totrev=0,
                     int iqt5_18219totrev=0, int iam5_18219totrev=0, int AutoZflg=0 );                    //���ȭ��b��

    int __fastcall ZDTReport(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev ,int rpt_type );                    //Z�b��

    int __fastcall XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no,
                            int iqt5_15217totrev=0, int iam5_15217totrev=0,
                            int iqt5_18219totrev=0, int iam5_18219totrev=0  );             //Ū�b

    int __fastcall INQReport( char *ptr_tencode, char *ptr_ecr_no,
                                 INQTBL *InqTbl);


    void __fastcall to_ptr(AnsiString S);                   //�����X���ɮ�

    //AnsiString __fastcall GetNumber(AnsiString S);          //���o�ƭ�

    AnsiString __fastcall ToDateFormat(AnsiString S);       //�ഫ������榡

    AnsiString __fastcall InsertSpace(AnsiString S,int i);  //��ť�

    AnsiString __fastcall ToTimeFormat(AnsiString S);      //�ഫ���ɶ��榡

    // �����P�P�馩���B        Lu 2018/06/13
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
