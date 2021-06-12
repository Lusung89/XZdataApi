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
/// 交班  Data strature
/// </summary>
class XDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 03 收銀機機號           */
       string  no_casher;           /* 03 收銀員號碼           */
       string  no_seq;              /* 03 日結序號             */
       string  dt_begin;            /* 03 開始日期             */
       string  tm_begin;            /* 03 開始時間             */
       string  dt_end;              /* 03 結束日期             */
       string  tm_end;              /* 03 結束時間             */

       struct {                   /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {        /* 03 代售付ｔａｂｌｅ       */
          struct {                      /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];
   
       struct {      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;             /* 03 現金合計             */
       string     no_rcstart;         /* 03 發票開始號碼         */
       string     no_rcend;           /* 03 發票結束號碼         */

       struct {   /* 03 收銀員明細ｆｉｅｌｄ */
          struct {                      /*   05 借零 table        */
             string  am_vend;          /*      07 借零金額       */
             string  tm_vend;          /*      07 借零執行時間   */
          } tb_vendfld[MaxDropRec];
          string  qt_vendtot;          /*   05 借零次數          */
          string  am_vendtot;          /*   05 借零合計金額      */

          struct {                          /*   05 投零 table        */
             string  am_feed;          /*      07 投零金額       */
             string  tm_feed;          /*      07 投零執行時間   */
          } tg_feedfld[MaxDropRec];
          string  qt_feedtot;          /*   05 投零次數          */
          string  am_feedtot;          /*   05 投零合計金額      */

          struct {                      /*   05 投庫 table        */
             string  am_drop;          /*      07 投庫金額       */
             string  tm_drop;          /*      07 投庫執行時間   */
          } tg_dropfld[MaxDropRec];
          string  qt_droptot;          /*   05 投庫次數          */
          string  am_droptot;          /*   05 投庫合計金額      */

          string  qt_update;           /*   05 更正次數          */
          string  am_update;           /*   05 更正合計金額      */
          string  qt_void;             /*   05 交易取消次數      */
          string  am_void;             /*   05 交易取消合計金額  */
          string  qt_dise;             /*   05 折扣次數(應稅)    */
          string  am_dise;             /*   05 折扣金額(應稅)    */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          string  am_pack;             /*   05 加值機結帳金額     */
       } tb_casher;

       struct {                         /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_cetel;            /*   05 代收電信費次數     */
          string  am_cetel;            /*   05 代收電信費金額     */
          string  qt_ffu1;             /*   05 現金卡次數         */
          string  am_ffu1;             /*   05 現金卡金額         */
          string  qt_ffu2;             /*   05 扣卡機扣卡次數     */
          string  am_ffu2;             /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收商品客數       */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss次數     */
          string  am_ffu5;             /*   05 TM廢棄Loss金額     */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;                //X帳檔案結構
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 
    REPORT *report;               //報表Class
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
/// 日結  Data strature
/// </summary>
class ZDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 收銀機機號    31H-39H */
       string  bg_noclose;          /* 日結序號      00001-99999 */
       string  bg_dtclose;          /* 結帳日期              */
       string  bg_tmclose;          /* 結帳時間              */
       string  ed_dtclose;          /* 前次結帳日期          */
       string  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_tclsacc;          /*    05 本日結帳累計金額  */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;          /* 03 現金合計             */
       string     no_rcstart;       /* 03 發票開始號碼         */
       string     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          string  am_dpsitm;           /*   05 部門交易項數       */
          string  am_dpsale;           /*   05 部門銷售金額       */
          string  am_dpmitm;           /*   05 部門誤退項數       */
          string  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 部門合計交易項數     */
       string     am_dpstot;       /* 03 部門合計銷售金額     */
       string     am_tdpmitm;      /* 03 部門合計誤退項數     */
       string     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          string  qt_vendtot;          /*   05 借零次數           */
          string  am_vendtot;          /*   05 借零合計金額       */
          string  qt_feedtot;          /*   05 投零次數           */
          string  am_feedtot;          /*   05 投零合計金額       */
          string  qt_droptot;          /*   05 投庫次數           */
          string  am_droptot;          /*   05 投庫合計金額       */
          string  qt_update;           /*   05 更正次數           */
          string  am_update;           /*   05 更正合計金額       */
          string  qt_void;             /*   05 交易取消次數       */
          string  am_void;             /*   05 交易取消合計金額   */
          string  qt_disc;             /*   05 折扣次數(應稅)     */
          string  am_disc;             /*   05 折扣金額(應稅)     */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          string  am_pack;             /*   05 加值機結帳金額     */
          string  qt_train;            /*   05 訓練合計次數       */
          string  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_agnrcv;           /*   05 代收費用次數       */
          string  am_agnrcv;           /*   05 代收費用金額       */
          string  qt_cashCard;         /*   05 現金卡次數         */
          string  am_cashCard;         /*   05 現金卡金額         */
          string  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          string  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收來客數         */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          string  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;                //Z帳結構
    
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 
    REPORT *report;               //報表Class
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
//class XREPORT : public BASIC 讀帳
/// <summary> class XREPORT
/// 讀帳  Data strature
/// </summary>
class XREPORT : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 收銀機機號    31H-39H */
       string  bg_noclose;          /* 日結序號      00001-99999 */
       string  bg_dtclose;          /* 結帳日期              */
       string  bg_tmclose;          /* 結帳時間              */
       string  ed_dtclose;          /* 前次結帳日期          */
       string  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_tclsacc;          /*    05 本日結帳累計金額  */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;          /* 03 現金合計             */
       string     no_rcstart;       /* 03 發票開始號碼         */
       string     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          string  am_dpsitm;           /*   05 部門交易項數       */
          string  am_dpsale;           /*   05 部門銷售金額       */
          string  am_dpmitm;           /*   05 部門誤退項數       */
          string  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 部門合計交易項數     */
       string     am_dpstot;       /* 03 部門合計銷售金額     */
       string     am_tdpmitm;      /* 03 部門合計誤退項數     */
       string     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          string  qt_vendtot;          /*   05 借零次數           */
          string  am_vendtot;          /*   05 借零合計金額       */
          string  qt_feedtot;          /*   05 投零次數           */
          string  am_feedtot;          /*   05 投零合計金額       */
          string  qt_droptot;          /*   05 投庫次數           */
          string  am_droptot;          /*   05 投庫合計金額       */
          string  qt_update;           /*   05 更正次數           */
          string  am_update;           /*   05 更正合計金額       */
          string  qt_void;             /*   05 交易取消次數       */
          string  am_void;             /*   05 交易取消合計金額   */
          string  qt_disc;             /*   05 折扣次數(應稅)     */
          string  am_disc;             /*   05 折扣金額(應稅)     */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券    */
          string  am_pack;             /*   05 加值機結帳金額     */
          string  qt_train;            /*   05 訓練合計次數       */
          string  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_agnrcv;           /*   05 代收費用次數       */
          string  am_agnrcv;           /*   05 代收費用金額       */
          string  qt_cashCard;         /*   05 現金卡次數         */
          string  am_cashCard;         /*   05 現金卡金額         */
          string  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          string  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收來客數         */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          string  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */
	CIniReader tif_ecr;               // only for XZDATA.Ini 
	CIniReader tif_tmp;               // only for XZDATA_Tmp.Ini 
    REPORT *report;               //報表Class
public:
    int __fastcall WriteData( const string StoreNO, const string EcrNO, const string SalesNo ,
                              const string SalFileName, const string Version);
    XREPORT();
    ~XREPORT();

};


 
/// <summary>  class CHECKIN 
/// 擔當登入 Data strature
/// </summary>
class CHECKIN : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 03 收銀機機號           */
       string  no_casher;           /* 03 收銀員號碼           */
       string  no_seq;              /* 03 日結序號             */
       string  dt_begin;            /* 03 開始日期             */
       string  tm_begin;            /* 03 開始時間             */
       string  dt_end;              /* 03 結束日期             */
       string  tm_end;              /* 03 結束時間             */

       struct {                   /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {        /* 03 代售付ｔａｂｌｅ       */
          struct {                      /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];
   
       struct {      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;             /* 03 現金合計             */
       string     no_rcstart;         /* 03 發票開始號碼         */
       string     no_rcend;           /* 03 發票結束號碼         */

       struct {   /* 03 收銀員明細ｆｉｅｌｄ */
          struct {                      /*   05 借零 table        */
             string  am_vend;          /*      07 借零金額       */
             string  tm_vend;          /*      07 借零執行時間   */
          } tb_vendfld[MaxDropRec];
          string  qt_vendtot;          /*   05 借零次數          */
          string  am_vendtot;          /*   05 借零合計金額      */

          struct {                          /*   05 投零 table        */
             string  am_feed;          /*      07 投零金額       */
             string  tm_feed;          /*      07 投零執行時間   */
          } tg_feedfld[MaxDropRec];
          string  qt_feedtot;          /*   05 投零次數          */
          string  am_feedtot;          /*   05 投零合計金額      */

          struct {                      /*   05 投庫 table        */
             string  am_drop;       /*      07 投庫金額       */
             string  tm_drop;          /*      07 投庫執行時間   */
          } tg_dropfld[MaxDropRec];
          string  qt_droptot;          /*   05 投庫次數          */
          string  am_droptot;          /*   05 投庫合計金額      */

          string  qt_update;           /*   05 更正次數          */
          string  am_update;           /*   05 更正合計金額      */
          string  qt_void;             /*   05 交易取消次數      */
          string  am_void;             /*   05 交易取消合計金額  */
          string  qt_dise;             /*   05 折扣次數(應稅)    */
          string  am_dise;             /*   05 折扣金額(應稅)    */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          string  am_pack;             /*   05 加值機結帳金額     */
       } tb_casher;

       struct {                         /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_cetel;            /*   05 代收電信費次數     */
          string  am_cetel;            /*   05 代收電信費金額     */
          string  qt_ffu1;             /*   05 現金卡次數         */
          string  am_ffu1;             /*   05 現金卡金額         */
          string  qt_ffu2;             /*   05 扣卡機扣卡次數     */
          string  am_ffu2;             /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收商品客數       */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss次數     */
          string  am_ffu5;             /*   05 TM廢棄Loss金額     */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;                 //X帳檔案結構
    REPORT *report;                //報表Class
	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;           // only for XZDATA_Tmp.Ini 
public:
    int __fastcall WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version);
    CHECKIN();
    ~CHECKIN();

};



//class SPCDATA : public BASIC  重點商品
/// <summary> class SPCDATA
/// 重點商品  Data strature
/// </summary>
class SPCDATA : public BASIC
{
private:
    //typedef struct {
       string  no_store;
       string  no_tm;               // 03 收銀機機號
       string  no_Dttm;
       string  no_tran;
       string  no_seq;              // 03 日結序號
       string  no_xseq;             //    交班序號
       string  no_casher;           // 03 收銀員號碼
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
    //REPORT *report;			 //報表Class
public:
    //重點商品列管帳表
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
//class VXZDATA : public BASIC 盤點人員讀帳
/// <summary>  class VXZDATA
/// 盤點人員讀帳 Data strature
/// </summary>
class VXZDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 收銀機機號    31H-39H */
       string  bg_noclose;          /* 日結序號      00001-99999 */
       string  bg_dtclose;          /* 結帳日期              */
       string  bg_tmclose;          /* 結帳時間              */
       string  ed_dtclose;          /* 前次結帳日期          */
       string  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_tclsacc;          /*    05 本日結帳累計金額  */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;          /* 03 現金合計             */
       string     no_rcstart;       /* 03 發票開始號碼         */
       string     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          string  am_dpsitm;           /*   05 部門交易項數       */
          string  am_dpsale;           /*   05 部門銷售金額       */
          string  am_dpmitm;           /*   05 部門誤退項數       */
          string  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 部門合計交易項數     */
       string     am_dpstot;       /* 03 部門合計銷售金額     */
       string     am_tdpmitm;      /* 03 部門合計誤退項數     */
       string     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          string  qt_vendtot;          /*   05 借零次數           */
          string  am_vendtot;          /*   05 借零合計金額       */
          string  qt_feedtot;          /*   05 投零次數           */
          string  am_feedtot;          /*   05 投零合計金額       */
          string  qt_droptot;          /*   05 投庫次數           */
          string  am_droptot;          /*   05 投庫合計金額       */
          string  qt_update;           /*   05 更正次數           */
          string  am_update;           /*   05 更正合計金額       */
          string  qt_void;             /*   05 交易取消次數       */
          string  am_void;             /*   05 交易取消合計金額   */
          string  qt_disc;             /*   05 折扣次數(應稅)     */
          string  am_disc;             /*   05 折扣金額(應稅)     */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          string  am_pack;             /*   05 加值機結帳金額     */
          string  qt_train;            /*   05 訓練合計次數       */
          string  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_agnrcv;           /*   05 代收費用次數       */
          string  am_agnrcv;           /*   05 代收費用金額       */
          string  qt_cashCard;         /*   05 現金卡次數         */
          string  am_cashCard;         /*   05 現金卡金額         */
          string  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          string  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收來客數         */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          string  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;				 //Z帳結構
 	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;       // only for XZDATA_Tmp.Ini 
    REPORT *report;			 //報表Class
public:

    int __fastcall WriteData(const string StoreNO, const string EcrNO, const string SalesNo ,
                                const string SalFileName, const string Version);
    VXZDATA();
    ~VXZDATA();
    //void __fastcall mtn_dat_folder();
    //void __fastcall del_list_file(TStringList *list, string path);

};



//class AUTOZDATA : public BASIC 自動日結
/// <summary> class AUTOZDATA
/// 自動日結  Data strature
/// </summary>
class AUTOZDATA : public BASIC
{
private:
    typedef struct {
       string  no_tm;               /* 收銀機機號    31H-39H */
       string  bg_noclose;          /* 日結序號      00001-99999 */
       string  bg_dtclose;          /* 結帳日期              */
       string  bg_tmclose;          /* 結帳時間              */
       string  ed_dtclose;          /* 前次結帳日期          */
       string  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_tclsacc;          /*    05 本日結帳累計金額  */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;          /* 03 現金合計             */
       string     no_rcstart;       /* 03 發票開始號碼         */
       string     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          string  am_dpsitm;           /*   05 部門交易項數       */
          string  am_dpsale;           /*   05 部門銷售金額       */
          string  am_dpmitm;           /*   05 部門誤退項數       */
          string  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       string     am_tdpsitm;      /* 03 部門合計交易項數     */
       string     am_dpstot;       /* 03 部門合計銷售金額     */
       string     am_tdpmitm;      /* 03 部門合計誤退項數     */
       string     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          string  qt_vendtot;          /*   05 借零次數           */
          string  am_vendtot;          /*   05 借零合計金額       */
          string  qt_feedtot;          /*   05 投零次數           */
          string  am_feedtot;          /*   05 投零合計金額       */
          string  qt_droptot;          /*   05 投庫次數           */
          string  am_droptot;          /*   05 投庫合計金額       */
          string  qt_update;           /*   05 更正次數           */
          string  am_update;           /*   05 更正合計金額       */
          string  qt_void;             /*   05 交易取消次數       */
          string  am_void;             /*   05 交易取消合計金額   */
          string  qt_disc;             /*   05 折扣次數(應稅)     */
          string  am_disc;             /*   05 折扣金額(應稅)     */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          string  am_pack;             /*   05 加值機結帳金額     */
          string  qt_train;            /*   05 訓練合計次數       */
          string  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_agnrcv;           /*   05 代收費用次數       */
          string  am_agnrcv;           /*   05 代收費用金額       */
          string  qt_cashCard;         /*   05 現金卡次數         */
          string  am_cashCard;         /*   05 現金卡金額         */
          string  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          string  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收來客數         */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          string  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;          //Z帳結構

    ///////////////////////////////////////
     typedef struct {
       string  no_tm;               /* 03 收銀機機號           */
       string  no_casher;           /* 03 收銀員號碼           */
       string  no_seq;              /* 03 日結序號             */
       string  dt_begin;            /* 03 開始日期             */
       string  tm_begin;            /* 03 開始時間             */
       string  dt_end;              /* 03 結束日期             */
       string  tm_end;              /* 03 結束時間             */

       struct {                   /* 03 交易明細ｆｉｅｌｄ   */
          string  qt_tcust;            /*    05 本日來客數        */
          string  qt_trans;            /*    05 本日交易品項      */
          string  am_trcpntx;          /*    05 發票日結帳額(免)  */
          string  am_trcptax;          /*    05 發票日結帳額(稅)  */
          string  am_trcptot;          /*    05 發票日結帳金額    */
          string  qt_miss;             /*    05 誤打發票次數      */
          string  am_missntx;          /*    05 誤打發票金額(免)  */
          string  am_misstax;          /*    05 誤打發票金額(稅)  */
          string  am_miss;             /*    05 誤打發票金額      */
          string  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {        /* 03 代售付ｔａｂｌｅ       */
          struct {                      /*    05 代售付品項ｉｔｅｍ  */
             string  qt_subrev;        /*       07 項目交易回數     */
             string  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          string  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          string  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];
   
       struct {      /* 03 代現金ｔａｂｌｅ     */
          string  qt_bonus;            /*    05 禮券回數          */
          string  am_bonus;            /*    05 禮券金額          */
          string  qt_vndcpn;           /*    05 廠商折價券回數    */
          string  am_vndcpn;           /*    05 廠商折價券金額    */
          string  qt_stocpn;           /*    05 折價券回數        */
          string  am_stocpn;           /*    05 折價券金額        */
          string  qt_totcash;          /*    05 代現金合計回數    */
          string  am_totcash;          /*    05 代現金合計金額    */
          string  qt_bonout;           /*    05 溢收合計回數      */
          string  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       string     am_cash;             /* 03 現金合計             */
       string     no_rcstart;         /* 03 發票開始號碼         */
       string     no_rcend;           /* 03 發票結束號碼         */

       struct {   /* 03 收銀員明細ｆｉｅｌｄ */
          struct {                      /*   05 借零 table        */
             string  am_vend;          /*      07 借零金額       */
             string  tm_vend;          /*      07 借零執行時間   */
          } tb_vendfld[MaxDropRec];
          string  qt_vendtot;          /*   05 借零次數          */
          string  am_vendtot;          /*   05 借零合計金額      */

          struct {                          /*   05 投零 table        */
             string  am_feed;          /*      07 投零金額       */
             string  tm_feed;          /*      07 投零執行時間   */
          } tg_feedfld[MaxDropRec];
          string  qt_feedtot;          /*   05 投零次數          */
          string  am_feedtot;          /*   05 投零合計金額      */

          struct {                      /*   05 投庫 table        */
             string  am_drop;       /*      07 投庫金額       */
             string  tm_drop;          /*      07 投庫執行時間   */
          } tg_dropfld[MaxDropRec];
          string  qt_droptot;          /*   05 投庫次數          */
          string  am_droptot;          /*   05 投庫合計金額      */

          string  qt_update;           /*   05 更正次數          */
          string  am_update;           /*   05 更正合計金額      */
          string  qt_void;             /*   05 交易取消次數      */
          string  am_void;             /*   05 交易取消合計金額  */
          string  qt_dise;             /*   05 折扣次數(應稅)    */
          string  am_dise;             /*   05 折扣金額(應稅)    */
          string  qt_disret1;          /*   05 加值機開/售卡次數  */
          string  am_disret1;          /*   05 加值機開/售卡金額  */
          string  qt_disret2;          /*   05 加值機加值次數     */
          string  am_disret2;          /*   05 加值機加值金額     */
          string  qt_openbox;          /*   05 開錢櫃次數         */
          string  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          string  am_pack;             /*   05 加值機結帳金額     */
       } tb_casher;

       struct {                         /*   05 追加 table        */
          string  qt_inpdp;            /*   05 手動輸入部門次數   */
          string  am_inpdp;            /*   05 手動輸入部門金額   */
          string  qt_rcp0;             /*   05 發票金額0次數      */
          string  qt_parking;          /*   05 代收停車費次數     */
          string  am_parking;          /*   05 代收停車費金額     */
          string  qt_cetel;            /*   05 代收電信費次數     */
          string  am_cetel;            /*   05 代收電信費金額     */
          string  qt_ffu1;             /*   05 現金卡次數         */
          string  am_ffu1;             /*   05 現金卡金額         */
          string  qt_ffu2;             /*   05 扣卡機扣卡次數     */
          string  am_ffu2;             /*   05 扣卡機扣卡金額     */
          string  tx_mark;             /*   05 版本前置符號       */
          string  tx_ver;              /*   05 版本號碼           */
          string  qt_ffu4;             /*   05 代收商品客數       */
          string  am_ffu4;             /*   05 備用四金額         */
          string  qt_ffu5;             /*   05 TM廢棄Loss次數     */
          string  am_ffu5;             /*   05 TM廢棄Loss金額     */
          string  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          string  am_ffu6;             /*   05 促銷折扣/讓金額    */
          string  qt_ffu7;             /*   05 加值機加值取消次數 */
          string  am_ffu7;             /*   05 加值機加值取消金額 */
          string  qt_ffu8;             /*   05 備用八次數         */
          string  am_ffu8;             /*   05 備用八金額         */
          string  qt_ffu9;             /*   05 備用九次數         */
          string  am_ffu9;             /*   05 備用九金額         */
          string  qt_ffu10;            /*   05 備用十次數         */
          string  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;      //X帳檔案結構

	CIniReader tif_ecr;           // only for XZDATA.Ini 
	CIniReader tif_tmp;       // only for XZDATA_Tmp.Ini 
    REPORT *report;     //報表Class
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


extern  XDATA *xdata;         //交班 OK
extern  ZDATA *zdata;         //日結  
extern  XREPORT *xreport;     //讀帳 OK
extern  CHECKIN *checkin;     //擔當 OK
extern  SPCDATA *AllSpcData;  //重點商品 OK
extern  VXZDATA *vxzdata;     //盤點人員讀帳  OK
extern  AUTOZDATA *autozdata; //自動日結


#endif
