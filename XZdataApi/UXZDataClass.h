//---------------------------------------------------------------------------

#ifndef UXZDataClassH
#define UXZDataClassH
#include "UReport.h"

#define AUTOZ  1
#define NAUTOZ 0

int __stdcall wx_ok(string StoreNO, string EcrNO, string SalesNo, string SalFileName, string Version);
int __stdcall wz_ok(string StoreNO, string EcrNO, string SalesNo, string SalFileName, string Version); // , bool delFolder);
int __stdcall MtnDirHistory(string RptYYYYMMDD, string SysYYYYMMDD, string RptDatez);
//---------------------------------------------------------------------------
//class XREPORT  
/// <summary> class XDATA
/// ��Z  Data strature
/// </summary>
class XDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 03 ���Ⱦ�����           */
       string  no_casher;           /* 03 ���ȭ����X           */
       string  no_seq;              /* 03 �鵲�Ǹ�             */
       string  dt_begin;            /* 03 �}�l���             */
       string  tm_begin;            /* 03 �}�l�ɶ�             */
       string  dt_end;              /* 03 �������             */
       string  tm_end;              /* 03 �����ɶ�             */

       struct {                   /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {        /* 03 �N��I��������       */
          struct {                      /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];
   
       struct {      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;             /* 03 �{���X�p             */
       string     no_rcstart;         /* 03 �o���}�l���X         */
       string     no_rcend;           /* 03 �o���������X         */

       struct {   /* 03 ���ȭ����Ӣ������ */
          struct {                      /*   05 �ɹs table        */
             string  am_vend;          /*      07 �ɹs���B       */
             string  tm_vend;          /*      07 �ɹs����ɶ�   */
          } tb_vendfld[MaxDropRec];
          string  qt_vendtot;          /*   05 �ɹs����          */
          string  am_vendtot;          /*   05 �ɹs�X�p���B      */

          struct {                          /*   05 ��s table        */
             string  am_feed;          /*      07 ��s���B       */
             string  tm_feed;          /*      07 ��s����ɶ�   */
          } tg_feedfld[MaxDropRec];
          string  qt_feedtot;          /*   05 ��s����          */
          string  am_feedtot;          /*   05 ��s�X�p���B      */

          struct {                      /*   05 ��w table        */
             string  am_drop;          /*      07 ��w���B       */
             string  tm_drop;          /*      07 ��w����ɶ�   */
          } tg_dropfld[MaxDropRec];
          string  qt_droptot;          /*   05 ��w����          */
          string  am_droptot;          /*   05 ��w�X�p���B      */

          string  qt_update;           /*   05 �󥿦���          */
          string  am_update;           /*   05 �󥿦X�p���B      */
          string  qt_void;             /*   05 �����������      */
          string  am_void;             /*   05 ��������X�p���B  */
          string  qt_dise;             /*   05 �馩����(���|)    */
          string  am_dise;             /*   05 �馩���B(���|)    */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��   */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
       } tb_casher;

       struct {                         /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_cetel;            /*   05 �N���q�H�O����     */
          string  am_cetel;            /*   05 �N���q�H�O���B     */
          string  qt_ffu1;             /*   05 �{���d����         */
          string  am_ffu1;             /*   05 �{���d���B         */
          string  qt_ffu2;             /*   05 ���d�����d����     */
          string  am_ffu2;             /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫ~�ȼ�       */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss����     */
          string  am_ffu5;             /*   05 TM�o��Loss���B     */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;                //X�b�ɮ׵��c
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 
    REPORT *report;               //����Class
public:
     
    int __fastcall WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version);

    int __fastcall XDATA::GetXDTData(string StrZCnt, string StoreNO,  string EcrNO, string SalesNo, string XDTsysDttm, string StrNxnoSeq,
                          string Version, string Z_date, string DDZ_1099, string ZNT_1099, string sIdx,
                          string &RtnXDTData, string &RtnXRPFile, string &RtnCRPFile );
  
    XDATA();
    ~XDATA();
};



//class ZDATA  
/// <summary> class ZDATA
/// �鵲  Data strature
/// </summary>
class ZDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* ���Ⱦ�����    31H-39H */
       string  bg_noclose;          /* �鵲�Ǹ�      00001-99999 */
       string  bg_dtclose;          /* ���b���              */
       string  bg_tmclose;          /* ���b�ɶ�              */
       string  ed_dtclose;          /* �e�����b���          */
       string  ed_tmclose;          /* �e�����b�ɶ�          */

       struct {                      /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_tclsacc;          /*    05 ���鵲�b�֭p���B  */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {                      /* 03 �N��I��������       */
          struct {                   /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];

       struct {                      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;          /* 03 �{���X�p             */
       string     no_rcstart;       /* 03 �o���}�l���X         */
       string     no_rcend;         /* 03 �o���������X         */

       struct {                      /* 03 �����O���pTABLE      */
          string  am_dpsitm;           /*   05 �����������       */
          string  am_dpsale;           /*   05 �����P����B       */
          string  am_dpmitm;           /*   05 �����~�h����       */
          string  am_dpmiss;           /*   05 �����~�h���B       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 �����X�p�������     */
       string     am_dpstot;       /* 03 �����X�p�P����B     */
       string     am_tdpmitm;      /* 03 �����X�p�~�h����     */
       string     am_dpmtot;       /* 03 �����X�p�~�h���B     */

       struct {                     /* 03 ���ȭ����Ӣ������ */
          string  qt_vendtot;          /*   05 �ɹs����           */
          string  am_vendtot;          /*   05 �ɹs�X�p���B       */
          string  qt_feedtot;          /*   05 ��s����           */
          string  am_feedtot;          /*   05 ��s�X�p���B       */
          string  qt_droptot;          /*   05 ��w����           */
          string  am_droptot;          /*   05 ��w�X�p���B       */
          string  qt_update;           /*   05 �󥿦���           */
          string  am_update;           /*   05 �󥿦X�p���B       */
          string  qt_void;             /*   05 �����������       */
          string  am_void;             /*   05 ��������X�p���B   */
          string  qt_disc;             /*   05 �馩����(���|)     */
          string  am_disc;             /*   05 �馩���B(���|)     */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��   */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
          string  qt_train;            /*   05 �V�m�X�p����       */
          string  am_train;            /*   05 �V�m�X�p���B       */
       } tb_casher;

       struct {                   /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_agnrcv;           /*   05 �N���O�Φ���       */
          string  am_agnrcv;           /*   05 �N���O�Ϊ��B       */
          string  qt_cashCard;         /*   05 �{���d����         */
          string  am_cashCard;         /*   05 �{���d���B         */
          string  qt_posZRpt;          /*   05 ���d�����d����     */
          string  am_posZRpt;          /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫȼ�         */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss�Ȧ���   */
          string  am_ffu5;             /*   05 TM�o��Loss�Ȫ��B   */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;                //Z�b���c
    
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 
    REPORT *report;               //����Class
public:
	  
    int __fastcall ZDATA::WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version, const string AutoZDttm);

    int __fastcall ZDATA::GetZDTData(string StrZCnt, string StoreNO,  string EcrNO, string SalesNo ,
                          string Version, string PZ_date, string Z_date, string DDZ_1099, string ZDT_1050,
                          string &RtnZDTData, string &Tclsacc,
                          string &str_zrp_path, string &str_dpt_path  );

    string __fastcall write_xpath(string);
    
    ZDATA();
    ~ZDATA();
    void __fastcall mtn_dat_folder();
    void __fastcall del_list_file(TStringList *slist, string path);

};


//////////////////////////////////////////////////////////////
//class XREPORT : public BASIC Ū�b
/// <summary> class XREPORT
/// Ū�b  Data strature
/// </summary>
class XREPORT : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* ���Ⱦ�����    31H-39H */
       string  bg_noclose;          /* �鵲�Ǹ�      00001-99999 */
       string  bg_dtclose;          /* ���b���              */
       string  bg_tmclose;          /* ���b�ɶ�              */
       string  ed_dtclose;          /* �e�����b���          */
       string  ed_tmclose;          /* �e�����b�ɶ�          */

       struct {                      /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_tclsacc;          /*    05 ���鵲�b�֭p���B  */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {                      /* 03 �N��I��������       */
          struct {                   /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];

       struct {                      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;          /* 03 �{���X�p             */
       string     no_rcstart;       /* 03 �o���}�l���X         */
       string     no_rcend;         /* 03 �o���������X         */

       struct {                      /* 03 �����O���pTABLE      */
          string  am_dpsitm;           /*   05 �����������       */
          string  am_dpsale;           /*   05 �����P����B       */
          string  am_dpmitm;           /*   05 �����~�h����       */
          string  am_dpmiss;           /*   05 �����~�h���B       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 �����X�p�������     */
       string     am_dpstot;       /* 03 �����X�p�P����B     */
       string     am_tdpmitm;      /* 03 �����X�p�~�h����     */
       string     am_dpmtot;       /* 03 �����X�p�~�h���B     */

       struct {                     /* 03 ���ȭ����Ӣ������ */
          string  qt_vendtot;          /*   05 �ɹs����           */
          string  am_vendtot;          /*   05 �ɹs�X�p���B       */
          string  qt_feedtot;          /*   05 ��s����           */
          string  am_feedtot;          /*   05 ��s�X�p���B       */
          string  qt_droptot;          /*   05 ��w����           */
          string  am_droptot;          /*   05 ��w�X�p���B       */
          string  qt_update;           /*   05 �󥿦���           */
          string  am_update;           /*   05 �󥿦X�p���B       */
          string  qt_void;             /*   05 �����������       */
          string  am_void;             /*   05 ��������X�p���B   */
          string  qt_disc;             /*   05 �馩����(���|)     */
          string  am_disc;             /*   05 �馩���B(���|)     */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��    */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
          string  qt_train;            /*   05 �V�m�X�p����       */
          string  am_train;            /*   05 �V�m�X�p���B       */
       } tb_casher;

       struct {                   /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_agnrcv;           /*   05 �N���O�Φ���       */
          string  am_agnrcv;           /*   05 �N���O�Ϊ��B       */
          string  qt_cashCard;         /*   05 �{���d����         */
          string  am_cashCard;         /*   05 �{���d���B         */
          string  qt_posZRpt;          /*   05 ���d�����d����     */
          string  am_posZRpt;          /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫȼ�         */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss�Ȧ���   */
          string  am_ffu5;             /*   05 TM�o��Loss�Ȫ��B   */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */
	CIniReader tif_ecr;               // only for XZDATA.Ini 
	CIniReader tif_tmp;               // only for XZDATA_Tmp.Ini 
    REPORT *report;               //����Class
public:
    int __fastcall WriteData( const string StoreNO, const string EcrNO, const string SalesNo ,
                              const string SalFileName, const string Version);
    XREPORT();
    ~XREPORT();

};


 
/// <summary>  class CHECKIN 
/// ���n�J Data strature
/// </summary>
class CHECKIN : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 03 ���Ⱦ�����           */
       string  no_casher;           /* 03 ���ȭ����X           */
       string  no_seq;              /* 03 �鵲�Ǹ�             */
       string  dt_begin;            /* 03 �}�l���             */
       string  tm_begin;            /* 03 �}�l�ɶ�             */
       string  dt_end;              /* 03 �������             */
       string  tm_end;              /* 03 �����ɶ�             */

       struct {                   /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {        /* 03 �N��I��������       */
          struct {                      /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];
   
       struct {      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;             /* 03 �{���X�p             */
       string     no_rcstart;         /* 03 �o���}�l���X         */
       string     no_rcend;           /* 03 �o���������X         */

       struct {   /* 03 ���ȭ����Ӣ������ */
          struct {                      /*   05 �ɹs table        */
             string  am_vend;          /*      07 �ɹs���B       */
             string  tm_vend;          /*      07 �ɹs����ɶ�   */
          } tb_vendfld[MaxDropRec];
          string  qt_vendtot;          /*   05 �ɹs����          */
          string  am_vendtot;          /*   05 �ɹs�X�p���B      */

          struct {                          /*   05 ��s table        */
             string  am_feed;          /*      07 ��s���B       */
             string  tm_feed;          /*      07 ��s����ɶ�   */
          } tg_feedfld[MaxDropRec];
          string  qt_feedtot;          /*   05 ��s����          */
          string  am_feedtot;          /*   05 ��s�X�p���B      */

          struct {                      /*   05 ��w table        */
             string  am_drop;       /*      07 ��w���B       */
             string  tm_drop;          /*      07 ��w����ɶ�   */
          } tg_dropfld[MaxDropRec];
          string  qt_droptot;          /*   05 ��w����          */
          string  am_droptot;          /*   05 ��w�X�p���B      */

          string  qt_update;           /*   05 �󥿦���          */
          string  am_update;           /*   05 �󥿦X�p���B      */
          string  qt_void;             /*   05 �����������      */
          string  am_void;             /*   05 ��������X�p���B  */
          string  qt_dise;             /*   05 �馩����(���|)    */
          string  am_dise;             /*   05 �馩���B(���|)    */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��   */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
       } tb_casher;

       struct {                         /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_cetel;            /*   05 �N���q�H�O����     */
          string  am_cetel;            /*   05 �N���q�H�O���B     */
          string  qt_ffu1;             /*   05 �{���d����         */
          string  am_ffu1;             /*   05 �{���d���B         */
          string  qt_ffu2;             /*   05 ���d�����d����     */
          string  am_ffu2;             /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫ~�ȼ�       */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss����     */
          string  am_ffu5;             /*   05 TM�o��Loss���B     */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;                 //X�b�ɮ׵��c
    REPORT *report;                //����Class
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 
public:
    int __fastcall WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version);
    CHECKIN();
    ~CHECKIN();

};



//class SPCDATA : public BASIC  ���I�ӫ~
/// <summary> class SPCDATA
/// ���I�ӫ~  Data strature
/// </summary>
class SPCDATA : public BASIC
{
private:
    //typedef struct {
       string  no_store;
       string  no_tm;               // 03 ���Ⱦ�����
       string  no_Dttm;
       string  no_tran;
       string  no_seq;              // 03 �鵲�Ǹ�
       string  no_xseq;             //    ��Z�Ǹ�
       string  no_casher;           // 03 ���ȭ����X
       string  no_spcSta;           //
       string  no_PrvDttm;
       int  tb_setgrpcount;
        struct {
               string  no_setgrp;
               string  nm_setgrp;
               int         amt_minpric;
               int         amt_maxpric;
               string  no_setsubgrp;

          } tb_30TMgrp[110];


       struct {
               string  no_setgrp;
               string  nm_setgrp;
               string  no_setsubgrp;
               int         qt_setgrp;
               float       am_setgrp;
               int         MustPrinted;
          } tb_setgrp[110];

   // } SPOS_SPCRPDT;
		     
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;			  // only for XZDATA_Tmp.Ini 
    //REPORT *report;			 //����Class
public:
    //���I�ӫ~�C�ޱb��
    int __fastcall SPCDATA::WriteSpcInqData(const string SalFileName, const string Version,
                                        const int iQrySw );
    int __fastcall SPCDATA::SumSPCData(int StartLine, int TotalLine, string &PrvSPCDataKey );
    int __fastcall SPCDATA::WriteSPCSAL(string strHdata,  string SALFileName);
    int __fastcall SPCDATA::Get30TMData();
    int __fastcall SPCDATA::WriteSPCReport(string strHdata, string RptSALFileName);
    SPCDATA();
    ~SPCDATA();
};



//////////////////////////////////////////////////////////////
//class VXZDATA : public BASIC �L�I�H��Ū�b
/// <summary>  class VXZDATA
/// �L�I�H��Ū�b Data strature
/// </summary>
class VXZDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* ���Ⱦ�����    31H-39H */
       string  bg_noclose;          /* �鵲�Ǹ�      00001-99999 */
       string  bg_dtclose;          /* ���b���              */
       string  bg_tmclose;          /* ���b�ɶ�              */
       string  ed_dtclose;          /* �e�����b���          */
       string  ed_tmclose;          /* �e�����b�ɶ�          */

       struct {                      /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_tclsacc;          /*    05 ���鵲�b�֭p���B  */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {                      /* 03 �N��I��������       */
          struct {                   /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];

       struct {                      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;          /* 03 �{���X�p             */
       string     no_rcstart;       /* 03 �o���}�l���X         */
       string     no_rcend;         /* 03 �o���������X         */

       struct {                      /* 03 �����O���pTABLE      */
          string  am_dpsitm;           /*   05 �����������       */
          string  am_dpsale;           /*   05 �����P����B       */
          string  am_dpmitm;           /*   05 �����~�h����       */
          string  am_dpmiss;           /*   05 �����~�h���B       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 �����X�p�������     */
       string     am_dpstot;       /* 03 �����X�p�P����B     */
       string     am_tdpmitm;      /* 03 �����X�p�~�h����     */
       string     am_dpmtot;       /* 03 �����X�p�~�h���B     */

       struct {                     /* 03 ���ȭ����Ӣ������ */
          string  qt_vendtot;          /*   05 �ɹs����           */
          string  am_vendtot;          /*   05 �ɹs�X�p���B       */
          string  qt_feedtot;          /*   05 ��s����           */
          string  am_feedtot;          /*   05 ��s�X�p���B       */
          string  qt_droptot;          /*   05 ��w����           */
          string  am_droptot;          /*   05 ��w�X�p���B       */
          string  qt_update;           /*   05 �󥿦���           */
          string  am_update;           /*   05 �󥿦X�p���B       */
          string  qt_void;             /*   05 �����������       */
          string  am_void;             /*   05 ��������X�p���B   */
          string  qt_disc;             /*   05 �馩����(���|)     */
          string  am_disc;             /*   05 �馩���B(���|)     */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��   */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
          string  qt_train;            /*   05 �V�m�X�p����       */
          string  am_train;            /*   05 �V�m�X�p���B       */
       } tb_casher;

       struct {                   /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_agnrcv;           /*   05 �N���O�Φ���       */
          string  am_agnrcv;           /*   05 �N���O�Ϊ��B       */
          string  qt_cashCard;         /*   05 �{���d����         */
          string  am_cashCard;         /*   05 �{���d���B         */
          string  qt_posZRpt;          /*   05 ���d�����d����     */
          string  am_posZRpt;          /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫȼ�         */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss�Ȧ���   */
          string  am_ffu5;             /*   05 TM�o��Loss�Ȫ��B   */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;				 //Z�b���c
 	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;       // only for XZDATA_Tmp.Ini 
    REPORT *report;			 //����Class
public:

    int __fastcall WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version);
    VXZDATA();
    ~VXZDATA();
    //void __fastcall mtn_dat_folder();
    //void __fastcall del_list_file(TStringList *list, string path);

};



//class AUTOZDATA : public BASIC �۰ʤ鵲
/// <summary> class AUTOZDATA
/// �۰ʤ鵲  Data strature
/// </summary>
class AUTOZDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* ���Ⱦ�����    31H-39H */
       string  bg_noclose;          /* �鵲�Ǹ�      00001-99999 */
       string  bg_dtclose;          /* ���b���              */
       string  bg_tmclose;          /* ���b�ɶ�              */
       string  ed_dtclose;          /* �e�����b���          */
       string  ed_tmclose;          /* �e�����b�ɶ�          */

       struct {                      /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_tclsacc;          /*    05 ���鵲�b�֭p���B  */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {                      /* 03 �N��I��������       */
          struct {                   /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];

       struct {                      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;          /* 03 �{���X�p             */
       string     no_rcstart;       /* 03 �o���}�l���X         */
       string     no_rcend;         /* 03 �o���������X         */

       struct {                      /* 03 �����O���pTABLE      */
          string  am_dpsitm;           /*   05 �����������       */
          string  am_dpsale;           /*   05 �����P����B       */
          string  am_dpmitm;           /*   05 �����~�h����       */
          string  am_dpmiss;           /*   05 �����~�h���B       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 �����X�p�������     */
       string     am_dpstot;       /* 03 �����X�p�P����B     */
       string     am_tdpmitm;      /* 03 �����X�p�~�h����     */
       string     am_dpmtot;       /* 03 �����X�p�~�h���B     */

       struct {                     /* 03 ���ȭ����Ӣ������ */
          string  qt_vendtot;          /*   05 �ɹs����           */
          string  am_vendtot;          /*   05 �ɹs�X�p���B       */
          string  qt_feedtot;          /*   05 ��s����           */
          string  am_feedtot;          /*   05 ��s�X�p���B       */
          string  qt_droptot;          /*   05 ��w����           */
          string  am_droptot;          /*   05 ��w�X�p���B       */
          string  qt_update;           /*   05 �󥿦���           */
          string  am_update;           /*   05 �󥿦X�p���B       */
          string  qt_void;             /*   05 �����������       */
          string  am_void;             /*   05 ��������X�p���B   */
          string  qt_disc;             /*   05 �馩����(���|)     */
          string  am_disc;             /*   05 �馩���B(���|)     */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��   */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
          string  qt_train;            /*   05 �V�m�X�p����       */
          string  am_train;            /*   05 �V�m�X�p���B       */
       } tb_casher;

       struct {                   /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_agnrcv;           /*   05 �N���O�Φ���       */
          string  am_agnrcv;           /*   05 �N���O�Ϊ��B       */
          string  qt_cashCard;         /*   05 �{���d����         */
          string  am_cashCard;         /*   05 �{���d���B         */
          string  qt_posZRpt;          /*   05 ���d�����d����     */
          string  am_posZRpt;          /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫȼ�         */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss�Ȧ���   */
          string  am_ffu5;             /*   05 TM�o��Loss�Ȫ��B   */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;          //Z�b���c

    ///////////////////////////////////////
     typedef struct {
       string  no_tm;               /* 03 ���Ⱦ�����           */
       string  no_casher;           /* 03 ���ȭ����X           */
       string  no_seq;              /* 03 �鵲�Ǹ�             */
       string  dt_begin;            /* 03 �}�l���             */
       string  tm_begin;            /* 03 �}�l�ɶ�             */
       string  dt_end;              /* 03 �������             */
       string  tm_end;              /* 03 �����ɶ�             */

       struct {                   /* 03 ������Ӣ������   */
          string  qt_tcust;            /*    05 ����ӫȼ�        */
          string  qt_trans;            /*    05 �������~��      */
          string  am_trcpntx;          /*    05 �o���鵲�b�B(�K)  */
          string  am_trcptax;          /*    05 �o���鵲�b�B(�|)  */
          string  am_trcptot;          /*    05 �o���鵲�b���B    */
          string  qt_miss;             /*    05 �~���o������      */
          string  am_missntx;          /*    05 �~���o�����B(�K)  */
          string  am_misstax;          /*    05 �~���o�����B(�|)  */
          string  am_miss;             /*    05 �~���o�����B      */
          string  am_totsale;          /*    05 �����ھP�f���J  */
       } tb_trans;

       struct {        /* 03 �N��I��������       */
          struct {                      /*    05 �N��I�~��������  */
             string  qt_subrev;        /*       07 ���إ���^��     */
             string  am_subrev;        /*       07 ���إ�����B     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 �N��I�ۢӢۢ�X�p�^��  */
          string  am_totrev;           /*    05 �N��I�ۢӢۢ�X�p���B  */
       } tb_subrev[5];
   
       struct {      /* 03 �N�{����������     */
          string  qt_bonus;            /*    05 §��^��          */
          string  am_bonus;            /*    05 §����B          */
          string  qt_vndcpn;           /*    05 �t�ӧ����^��    */
          string  am_vndcpn;           /*    05 �t�ӧ������B    */
          string  qt_stocpn;           /*    05 �����^��        */
          string  am_stocpn;           /*    05 �������B        */
          string  qt_totcash;          /*    05 �N�{���X�p�^��    */
          string  am_totcash;          /*    05 �N�{���X�p���B    */
          string  qt_bonout;           /*    05 �����X�p�^��      */
          string  am_bonout;           /*    05 �����X�p���B      */
       } tb_cash;

       string     am_cash;             /* 03 �{���X�p             */
       string     no_rcstart;         /* 03 �o���}�l���X         */
       string     no_rcend;           /* 03 �o���������X         */

       struct {   /* 03 ���ȭ����Ӣ������ */
          struct {                      /*   05 �ɹs table        */
             string  am_vend;          /*      07 �ɹs���B       */
             string  tm_vend;          /*      07 �ɹs����ɶ�   */
          } tb_vendfld[MaxDropRec];
          string  qt_vendtot;          /*   05 �ɹs����          */
          string  am_vendtot;          /*   05 �ɹs�X�p���B      */

          struct {                          /*   05 ��s table        */
             string  am_feed;          /*      07 ��s���B       */
             string  tm_feed;          /*      07 ��s����ɶ�   */
          } tg_feedfld[MaxDropRec];
          string  qt_feedtot;          /*   05 ��s����          */
          string  am_feedtot;          /*   05 ��s�X�p���B      */

          struct {                      /*   05 ��w table        */
             string  am_drop;       /*      07 ��w���B       */
             string  tm_drop;          /*      07 ��w����ɶ�   */
          } tg_dropfld[MaxDropRec];
          string  qt_droptot;          /*   05 ��w����          */
          string  am_droptot;          /*   05 ��w�X�p���B      */

          string  qt_update;           /*   05 �󥿦���          */
          string  am_update;           /*   05 �󥿦X�p���B      */
          string  qt_void;             /*   05 �����������      */
          string  am_void;             /*   05 ��������X�p���B  */
          string  qt_dise;             /*   05 �馩����(���|)    */
          string  am_dise;             /*   05 �馩���B(���|)    */
          string  qt_disret1;          /*   05 �[�Ⱦ��}/��d����  */
          string  am_disret1;          /*   05 �[�Ⱦ��}/��d���B  */
          string  qt_disret2;          /*   05 �[�Ⱦ��[�Ȧ���     */
          string  am_disret2;          /*   05 �[�Ⱦ��[�Ȫ��B     */
          string  qt_openbox;          /*   05 �}���d����         */
          string  qt_pack;             /*   05 �[�Ⱦ����b����  2007/12/01 ��l�B��   */
          string  am_pack;             /*   05 �[�Ⱦ����b���B     */
       } tb_casher;

       struct {                         /*   05 �l�[ table        */
          string  qt_inpdp;            /*   05 ��ʿ�J��������   */
          string  am_inpdp;            /*   05 ��ʿ�J�������B   */
          string  qt_rcp0;             /*   05 �o�����B0����      */
          string  qt_parking;          /*   05 �N�������O����     */
          string  am_parking;          /*   05 �N�������O���B     */
          string  qt_cetel;            /*   05 �N���q�H�O����     */
          string  am_cetel;            /*   05 �N���q�H�O���B     */
          string  qt_ffu1;             /*   05 �{���d����         */
          string  am_ffu1;             /*   05 �{���d���B         */
          string  qt_ffu2;             /*   05 ���d�����d����     */
          string  am_ffu2;             /*   05 ���d�����d���B     */
          string  tx_mark;             /*   05 �����e�m�Ÿ�       */
          string  tx_ver;              /*   05 �������X           */
          string  qt_ffu4;             /*   05 �N���ӫ~�ȼ�       */
          string  am_ffu4;             /*   05 �ƥΥ|���B         */
          string  qt_ffu5;             /*   05 TM�o��Loss����     */
          string  am_ffu5;             /*   05 TM�o��Loss���B     */
          string  qt_ffu6;             /*   05 �P�P�馩/������    */
          string  am_ffu6;             /*   05 �P�P�馩/�����B    */
          string  qt_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������� */
          string  am_ffu7;             /*   05 �[�Ⱦ��[�Ȩ������B */
          string  qt_ffu8;             /*   05 �ƥΤK����         */
          string  am_ffu8;             /*   05 �ƥΤK���B         */
          string  qt_ffu9;             /*   05 �ƥΤE����         */
          string  am_ffu9;             /*   05 �ƥΤE���B         */
          string  qt_ffu10;            /*   05 �ƥΤQ����         */
          string  am_ffu10;            /*   05 �ƥΤQ���B         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;      //X�b�ɮ׵��c

	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;       // only for XZDATA_Tmp.Ini 
    REPORT *report;     //����Class
public:
   
    int __fastcall WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version, const string AutoZDttm);

    string __fastcall write_xpath(string);
    int __fastcall GetYYYYMDD_SalData(string &AutoZCnt, string AutoZdt , int AutoZ);
    string  __fastcall AUTOZDATA::GetSysDttmMaxTransNo(string sDttm);

    string __fastcall AutoXData(const string StoreNO, const string EcrNO, const string SalesNo ,
                             const string SalFileName, const string Version, const string AutoZ_cnt,
                             const string AutoZ_Dt );

    int __fastcall AUTOZDATA::GetZDTData(string StrZCnt, string StoreNO,  string EcrNO, string SalesNo ,
                          string Version, string PZ_date, string Z_date, string DDZ_1099, string ZDT_1050,
                          string &RtnZDTData, string &Tclsacc,
                          string &str_zrp_path, string &str_dpt_path  );

    AUTOZDATA();
    ~AUTOZDATA();
    void __fastcall mtn_dat_folder();
    void __fastcall del_list_file(TStringList *slist, string path);

};


extern  XDATA *xdata;         //��Z OK
extern  ZDATA *zdata;         //�鵲  
extern  XREPORT *xreport;     //Ū�b OK
extern  CHECKIN *checkin;     //��� OK
extern  SPCDATA *AllSpcData;  //���I�ӫ~ OK
extern  VXZDATA *vxzdata;     //�L�I�H��Ū�b  OK
extern  AUTOZDATA *autozdata; //�۰ʤ鵲


#endif
