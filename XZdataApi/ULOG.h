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
#define _ver "����XZdataApi.DLL V2018.0101.01����"
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

//// �ˬd 1050 �� SAL 1050 ���ƬO�_�@�P
int __fastcall _GetZDTZSALCntCheck(TStringList *ZCntLst, string SalFileName, string OrgZcnt,
                                   int &RtnZDTCnts, int &RtnSALCnts );


//�����O���pTABLE
typedef struct
    {
        int am_dpsitm;              // �����������
        int am_dpsale;              // �����P����B
        int am_dpmitm;              // �����~�h����
        int am_dpmiss;              // �����~�h���B

		int am_mmDis;               // �����P�P�馩���B        Lu 2018/06/13
		int am_mmDismiss;           // �����P�P�馩�~�h���B    Lu 2018/06/13
    }DEPT;

//�Y�ɳs�u�ӫ~���pTABLE
typedef struct
    {
        int qt_InqSale;              // �Y�ɳs�u�ӫ~����ƶq
        int am_InqSale;              // �Y�ɳs�u�ӫ~������B
    }INQTBL;


typedef struct
    {
        int IndexNo;                //��w�Ǹ�
        int PayCashAmt;           //��w�{�����B
    }PAYCASHREC;



class BASIC
{
public:   

	CIniReader m_IniReader;
	
    //��ƼȦs��
    TStringList *gtsl_rec, *gtsl_tot, *gtsl_drop, *gtsl_easy_card, *gtsl_pay_sub;
    TStringList *tsl_subsale,*gtsl_bil, *gtsl_fetc_card, *gtsl_1051_sal;
    TStringList *Autotsl_rec, *Autotsl_tot, *Autotsl_drop, *Autotsl_easy_card, *Autotsl_pay_sub;
    TStringList *Autotsl_subsale,*Autotsl_bil, *Autotsl_fetc_card, *Auto_Sal, *Auto_1051_sal ;


    //char gchar_tencode[7];        //���O
    string g_tencode;        //���O
    //char gchar_ecr_no[5];         //����
    string g_ecr_no;         //����
    //char gchar_cashier[6];        //���ȭ�
    string g_cashier;
    //char gchar_date_time[15];     //�{�b�ɶ�
    string g_date_time;             //�{�b�ɶ�
    //char gchar_orgtencode[7];        //��l����
    string g_orgtencode;
    //char gchar_x_trans[6];        //�W�����ȱb�Ǹ�
    string gx_trans;

    //char gpchar_x_first[MAXDATALEN];         //380 X�b�Ĥ@��SAL LOG
    //char gpchar_sal_last[MAXDATALEN];        //380 SAL�̫�@��
    //char gpchar_sal_first[MAXDATALEN];       //380 SAL�Ĥ@��
    
    string gsalX_first;
    string gsal_last;
    string gsal_first;
    string gsLast1099;      //    SAL�Ĥ@�� 1099
    //20021223
    int pay_bill_cnt;              //�������f�馸��
    int pay_bill_amt;              //�������f����B
    int mpay_bill_cnt;              //�t�V�������f�馸��
    int mpay_bill_amt;              //�t�V�������f����B

    //20030125
    int Cpn71_cnt;                  //�����71����
    int Cpn71_amt;                  //�����71���B
    int Cpn72_cnt;                  //�����72����
    int Cpn72_amt;                  //�����72���B
    int Cpn73_cnt;                  //�����73����
    int Cpn73_amt;                  //�����73���B
    int Cpn74_cnt;                  //�����74����
    int Cpn74_amt;                  //�����74���B
    int Cpn75_cnt;                  //�����75����
    int Cpn75_amt;                  //�����75���B
    int Cpn76_cnt;                  //�����76����
    int Cpn76_amt;                  //�����76���B
    int Cpn77_cnt;                  //�������f��77����
    int Cpn77_amt;                  //�������f��77���B
    int Cpn78_cnt;                  //�������f��78����
    int Cpn78_amt;                  //�������f��78���B
    string StrBillPay ;

    int giA;  //�Ȧs�ܼ�ABCDEFGH
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


    struct {                            /* 03 �N��I��������       */
          struct {                      /* 05 �N��I�~��������  */
                  int  iqt_subrev;      /*       07 ���إ���^��     */
                  int  iam_subrev;      /*       07 ���إ�����B     */
          } it8_subrev[10];
          int  iqt5_totrev;             /*    05 �N��I�ۢӢۢ�X�p�^��  */
          int  iam5_totrev;             /*    05 �N��I�ۢӢۢ�X�p���B  */
       } BaseTbSubrev[20];

    int gi_mix;                      //�V�n����(�]�t�N(����I)�B�@��P��)
    int gi_open_x;                   //�}���d����-���ȭ��b��    
    int gi_open_z;                   //�}���d����-Z�b��BŪ�b��

    bool gbl_tot_haveline;           //TOT�O�_��������
    bool gbl_rec_haveline;           //REC
    bool gbl_bil_haveline;
    bool gbl_easy_haveline;           //easy card
    bool gbl_pay_haveline;
    bool gbl_fetc_haveline;           //Fetc card
    //bool gbl_0041_haveline;           //��w
    bool gbl_1051_haveline;           //1051�q��

    int gi_tot_line;                 //TOT���`���
    int gi_rec_line;                 //REC
    int gi_bil_line;                 //bil
    int gi_easy_line;                //EASY CARD
    int gi_pay_line;
    int gi_fetc_line;                //Fetc EASY CARD
    int gi_1051_line;                //�p��1051���
    //int gi_0041_line;                //��w

    int __fastcall subsale_update(TStringList * tsl, string good_no,
                            string menu_no, string bar_name,
                            string item_amt, string sal_type);

   /********************
    //�����O���pTABLE
    typedef struct
    {
        int am_dpsitm;              // �����������
        int am_dpsale;              // �����P����B
        int am_dpmitm;              // �����~�h����
        int am_dpmiss;              // �����~�h���B
    }DEPT;

    //�Y�ɳs�u�ӫ~���pTABLE
    typedef struct
    {
        int qt_InqSale;              // �Y�ɳs�u�ӫ~����ƶq
        int am_InqSale;              // �Y�ɳs�u�ӫ~������B
    }INQTBL;
  **********************/



	//void __fastcall BASIC::init_data();                                //���oPOS�򥻸��
	//int __fastcall BASIC::loadsalfile(string FileName, bool for_x = false, string AutoZ_Cnt = "", string AutoZ_Dt = ""); //�}��SAL�ɡA�N���e����

	~BASIC();
	BASIC();

protected:
	void __fastcall BASIC::init_data();                                //���oPOS�򥻸��
	int __fastcall BASIC::loadsalfile(string FileName, bool for_x = false, string AutoZ_Cnt = "", string AutoZ_Dt = ""); //�}��SAL�ɡA�N���e����
    int __fastcall BASIC::wloadsalfile(string FileName, bool for_x, string AutoZ_Cnt, string AutoZ_Dt); 
    bool __fastcall BASIC::CheckSaleRecData(string RecData);
	string __fastcall BASIC::BackupFile(string FileName, string BackupFileName = "", string DDZ = "");      //�ƥ��ɮצ�C:\HISTORY
    //int __fastcall BASIC::wloadsalfile(); // string FileName, bool for_x, string AutoZ_Cnt, string AutoZ_Dt); 
	////////////////// 1050 ///////////////////////

	int __fastcall BASIC::SumQt_tcust(int startLine, int TotalLine);   //����ӫȼ�

	int __fastcall BASIC::SumQt_trans(int stratLine, int TotalLine);  //����~����

	void __fastcall BASIC::TotCount(int startLine, int TotalLine);    //�o���鵲�b

	void __fastcall BASIC::TotMiss(int startLine, int TotalLine);     //�~���o��
	void __fastcall BASIC::TotMissCnt(int startLine, int TotalLine);   //�~���o���i��

	int __fastcall BASIC::SumCash(int startLine, int TotalLine);      //�{���X�p

    int __fastcall BASIC::SumNOVoiceAmt(int StartLine, int TotalLine);  //���}�o�����B

    int __fastcall BASIC::VoicePayAmt(int startLine, int TotalLine);     //�o���I�����B

	int __fastcall BASIC::SumInpdp(int startLine, int TotalLine);     //��鳡��

    int __fastcall BASIC::SumPreSal_MM_DisAmtTMMFP(int startLine, int TotalLine);
    int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int startLine, int TotalLine);   //�w����(�����/MM)�馩

    int __fastcall BASIC::SumPreSal_INVO_DisAmt(int startLine, int TotalLine);    //�w��I���ӫ~������馩/�N���ӫ~������馩

    int __fastcall BASIC::SumPreSal_Amt(int startLine, int TotalLine); //�w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��

	int __fastcall BASIC::SumRelDimAgn(int startLine, int TotalLine); //�Y���ʥN��

	int __fastcall BASIC::TotZero(int startLine, int TotalLine);      //�o�����B�s

	int __fastcall BASIC::SumVoid(int startLine, int TotalLine);      //�������

	int __fastcall BASIC::SumUpdat(int startLine, int TotalLine);     //��

	int __fastcall BASIC::SumLOSS(int startLine, int TotalLine);      //�o��

	int __fastcall BASIC::SumTbCash(int startLine, int TotalLine);    //�N�{��  ��d�X�p

    int __fastcall BASIC::SumBill78(int startLine, int TotalLine); //�p��������f��

	int __fastcall BASIC::SumKamt(int startLine, int TotalLine);      //�N���B��B�I ,�I�d����OgiD.�@�ogiE  

	int __fastcall BASIC::SumHealthAmt(int startLine, int TotalLine);

	////////////////// 1010 ///////////////////////

	void __fastcall BASIC::sub_times(int startLine, int TotalLine);   //�N���B��B�I���Ʋέp  

    string __fastcall invoice_start_end(int tot_line_count, bool invoice_start);     //�o���������X
		
	int __fastcall BASIC::easy_card(int startLine, int TotalLine);

	int __fastcall BASIC::fetc_card(int startLine, int TotalLine);

	void __fastcall BASIC::SumDisc(int startLine, int TotalLine);      //�p�p�馩

	int __fastcall BASIC::SumBillCSM(int startLine, int TotalLine);    //�p��2008/12/04 ���O��

    int __fastcall BASIC::SumBillCPN(int startLine, int TotalLine);  //�p��2017/03/08 CPN

	void __fastcall BASIC::SumAllDept(DEPT *dept);                     //�����b

    void __fastcall BASIC::INQSubTotal(INQTBL *inqtbl) ;         // �Y�ɳs�u�ӫ~���p

    void __fastcall pay_sub_amt(int StartLine, int TotalLine);  //�馩�� 

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

                                    //���ȭ���Z���Ӫ� for ���b��
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
   
    //�Y���ʦX�p
    int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine);

	void __fastcall BASIC::Sum040RecycleDataCnt(int StartLine, int TotalLine);   //�o�귽�^���� /���f����B/���f��i��

    int __fastcall BASIC::SumECP2Orher2Amt(int StartLine, int TotalLine);

    int __fastcall  BASIC::GetZCNT_SalData(string AutoZCnt);

    // �~�[����O,�Y���ʥN���I�d���p
    //void __fastcall BASIC::Sub_C0InvoAmt(int StartLine, int TotalLine);
    // �I�d����O���K�|
    void __fastcall BASIC::Sub_C0C1TaxInvoAmt(int StartLine, int TotalLine);

    // �I�d����O�ƶq Sub_C0C1TaxInvoQty(0, gi_rec_line) ==> �֤J Sub_C0C1TaxInvoAmt
    //void __fastcall BASIC::Sub_C0C1TaxInvoQty(int StartLine, int TotalLine);

    // �~�[����O,�Y���ʥN���I�d���p �H�s�u�Ϥ�����
    // Sub_C0InvoAmt4VDC NextPOS �X�� Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
    void __fastcall BASIC::Sub_C0InvoAmt4VDC(int StartLine, int TotalLine);

	int __fastcall BASIC::read_ten_name(string &sStoreNo, string &sStoreName);
	void __fastcall BASIC::scan_fixstr(FILE *f, register char *str, register int len);

    int __fastcall BASIC::w_WriteSAL(string data, string SALFileName, int mode);
	int __fastcall BASIC::WriteSAL(string data, string SALFileName); //97,98
	int __fastcall BASIC::WriteSAL_A(string data, string SALFileName);

};


//---------------------------------------------------------------------------
#endif
