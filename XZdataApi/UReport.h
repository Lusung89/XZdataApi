//---------------------------------------------------------------------------

//#ifndef UReportH
//#define UReportH

#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  

#include <atlstr.h> 
#include <tchar.h>
#include <string>
#include <sstream> 
#include <fstream>
#include <iostream>

#include "ULOG.h"
#define ESC 0x1B

class REPORT
{
public:
    //bool gbl_epson_prt;

    TStringList *tslSaveToFile;                             //�����X�Ȧs

    char char_filename[100];                                  //�����x�s��X�ɮצW

    char gchar_date_format[11];   //�{�b���

    REPORT();

    ~REPORT();

	int __fastcall Dept(string &ZData, const char *ptr_tencode, const char *ptr_ecr_no, int XR = 0, TStringList *tsl = NULL, string s1093 = NULL);   //�����b��

    int __fastcall XDTReport(string &XData, string sPayStr, const char *ptr_tencode,
                     int iqt5_15217totrev=0, int iam5_15217totrev=0,
                     int iqt5_18219totrev=0, int iam5_18219totrev=0, int AutoZflg=0 );                    //���ȭ��b��

    int __fastcall ZDTReport(string &Zdata, const char *ptr_tencode, const char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev ,int rpt_type );                    //Z�b��

    int __fastcall XReport(string &XRData, string sPayStr, const char *ptr_tencode, const char *ptr_ecr_no,
                            int iqt5_15217totrev, int iam5_15217totrev,
                            int iqt5_18219totrev, int iam5_18219totrev  );             //Ū�b

    int __fastcall INQReport(const char *ptr_tencode, const char *ptr_ecr_no,
                                 INQTBL *InqTbl);


    void __fastcall to_ptr(string S);                   //�����X���ɮ�

    //AnsiString __fastcall GetNumber(AnsiString S);          //���o�ƭ�

    string __fastcall ToDateFormat(string S);       //�ഫ������榡

    string __fastcall InsertSpace(string S,int i);  //��ť�

    string __fastcall ToTimeFormat(string S);      //�ഫ���ɶ��榡

    int CreateReport(string rpt_type, string sSal, string sPayStr,
                     string str_tencode, string  str_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev,
					 TStringList *tsl, int AutoZflg, string s1093);

    int CreateReport001(string rpt_type, string sSal, string sPayStr, string str_tencode, string  str_ecr_no,
        int iqt5_15217totrev, int iam5_15217totrev,
        int iqt5_18219totrev, int iam5_18219totrev, 
        TStringList *tsl , int AutoZflg, string s1093);

    int __fastcall subsale(TStringList *tsl, const char *ptr_tencode, const char *ptr_ecr_no);

};

//---------------------------------------------------------------------------
//extern REPORT *report;
//#endif
