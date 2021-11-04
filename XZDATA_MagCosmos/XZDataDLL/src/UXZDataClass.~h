//---------------------------------------------------------------------------

#ifndef UXZDataClassH
#define UXZDataClassH
#include "UReport.h"

#define AUTOZ  1
#define NAUTOZ 0

//---------------------------------------------------------------------------

class XDATA : public BASIC
{
private:
    typedef struct {
       AnsiString  no_tm;               /* 03 收銀機機號           */
       AnsiString  no_casher;           /* 03 收銀員號碼           */
       AnsiString  no_seq;              /* 03 日結序號             */
       AnsiString  dt_begin;            /* 03 開始日期             */
       AnsiString  tm_begin;            /* 03 開始時間             */
       AnsiString  dt_end;              /* 03 結束日期             */
       AnsiString  tm_end;              /* 03 結束時間             */

       struct {                   /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {        /* 03 代售付ｔａｂｌｅ       */
          struct {                      /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];
   
       struct {      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;             /* 03 現金合計             */
       AnsiString     no_rcstart;         /* 03 發票開始號碼         */
       AnsiString     no_rcend;           /* 03 發票結束號碼         */

       struct {   /* 03 收銀員明細ｆｉｅｌｄ */
          struct {                      /*   05 借零 table        */
             AnsiString  am_vend;          /*      07 借零金額       */
             AnsiString  tm_vend;          /*      07 借零執行時間   */
          } tb_vendfld[MaxDropRec];
          AnsiString  qt_vendtot;          /*   05 借零次數          */
          AnsiString  am_vendtot;          /*   05 借零合計金額      */

          struct {                          /*   05 投零 table        */
             AnsiString  am_feed;          /*      07 投零金額       */
             AnsiString  tm_feed;          /*      07 投零執行時間   */
          } tg_feedfld[MaxDropRec];
          AnsiString  qt_feedtot;          /*   05 投零次數          */
          AnsiString  am_feedtot;          /*   05 投零合計金額      */

          struct {                      /*   05 投庫 table        */
             AnsiString  am_drop;       /*      07 投庫金額       */
             AnsiString  tm_drop;          /*      07 投庫執行時間   */
          } tg_dropfld[MaxDropRec];
          AnsiString  qt_droptot;          /*   05 投庫次數          */
          AnsiString  am_droptot;          /*   05 投庫合計金額      */

          AnsiString  qt_update;           /*   05 更正次數          */
          AnsiString  am_update;           /*   05 更正合計金額      */
          AnsiString  qt_void;             /*   05 交易取消次數      */
          AnsiString  am_void;             /*   05 交易取消合計金額  */
          AnsiString  qt_dise;             /*   05 折扣次數(應稅)    */
          AnsiString  am_dise;             /*   05 折扣金額(應稅)    */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
       } tb_casher;

       struct {                         /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_cetel;            /*   05 代收電信費次數     */
          AnsiString  am_cetel;            /*   05 代收電信費金額     */
          AnsiString  qt_ffu1;             /*   05 現金卡次數         */
          AnsiString  am_ffu1;             /*   05 現金卡金額         */
          AnsiString  qt_ffu2;             /*   05 扣卡機扣卡次數     */
          AnsiString  am_ffu2;             /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收商品客數       */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss次數     */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss金額     */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;      //X帳檔案結構
    TIniFile *tif_ecr;
    TIniFile *tif_tmp;
    REPORT *report;     //報表Class
public:
    //int __fastcall WriteData(const AnsiString SalFileName, const AnsiString Version);
    int __fastcall WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version);

    int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo, String XDTsysDttm, String StrNxnoSeq,
                          String Version, String Z_date, String DDZ_1099, String ZNT_1099, String sIdx,
                          String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile );

   

    XDATA();
    ~XDATA();
};

class ZDATA : public BASIC
{
private:
    typedef struct {
       AnsiString  no_tm;               /* 收銀機機號    31H-39H */
       AnsiString  bg_noclose;          /* 日結序號      00001-99999 */
       AnsiString  bg_dtclose;          /* 結帳日期              */
       AnsiString  bg_tmclose;          /* 結帳時間              */
       AnsiString  ed_dtclose;          /* 前次結帳日期          */
       AnsiString  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_tclsacc;          /*    05 本日結帳累計金額  */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;          /* 03 現金合計             */
       AnsiString     no_rcstart;       /* 03 發票開始號碼         */
       AnsiString     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          AnsiString  am_dpsitm;           /*   05 部門交易項數       */
          AnsiString  am_dpsale;           /*   05 部門銷售金額       */
          AnsiString  am_dpmitm;           /*   05 部門誤退項數       */
          AnsiString  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       AnsiString     am_tdpsitm;      /* 03 部門合計交易項數     */
       AnsiString     am_dpstot;       /* 03 部門合計銷售金額     */
       AnsiString     am_tdpmitm;      /* 03 部門合計誤退項數     */
       AnsiString     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          AnsiString  qt_vendtot;          /*   05 借零次數           */
          AnsiString  am_vendtot;          /*   05 借零合計金額       */
          AnsiString  qt_feedtot;          /*   05 投零次數           */
          AnsiString  am_feedtot;          /*   05 投零合計金額       */
          AnsiString  qt_droptot;          /*   05 投庫次數           */
          AnsiString  am_droptot;          /*   05 投庫合計金額       */
          AnsiString  qt_update;           /*   05 更正次數           */
          AnsiString  am_update;           /*   05 更正合計金額       */
          AnsiString  qt_void;             /*   05 交易取消次數       */
          AnsiString  am_void;             /*   05 交易取消合計金額   */
          AnsiString  qt_disc;             /*   05 折扣次數(應稅)     */
          AnsiString  am_disc;             /*   05 折扣金額(應稅)     */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
          AnsiString  qt_train;            /*   05 訓練合計次數       */
          AnsiString  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_agnrcv;           /*   05 代收費用次數       */
          AnsiString  am_agnrcv;           /*   05 代收費用金額       */
          AnsiString  qt_cashCard;         /*   05 現金卡次數         */
          AnsiString  am_cashCard;         /*   05 現金卡金額         */
          AnsiString  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          AnsiString  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收來客數         */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;          //Z帳結構
    TIniFile *tif_ecr;
    TIniFile *tif_tmp;
    REPORT *report;     //報表Class
public:

    //TStringList *Autotsl_rec, *Autotsl_tot, *Autotsl_drop, *Autotsl_easy_card, *Autotsl_pay_sub;
    //TStringList *Autotsl_subsale,*Autotsl_bil, *Autotsl_fetc_card, *Auto_Sal;

    int __fastcall ZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version, const String AutoZDttm);

    int __fastcall ZDATA::GetZDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo ,
                          String Version, String PZ_date, String Z_date, String DDZ_1099, String ZDT_1050,
                          String &RtnZDTData, AnsiString &Tclsacc,
                          String &str_zrp_path, String &str_dpt_path  );

    AnsiString __fastcall write_xpath(AnsiString);
    
    ZDATA();
    ~ZDATA();
    void __fastcall mtn_dat_folder();
    void __fastcall del_list_file(TStringList *list, AnsiString path);

};



class XREPORT : public BASIC
{
private:
    typedef struct {
       AnsiString  no_tm;               /* 收銀機機號    31H-39H */
       AnsiString  bg_noclose;          /* 日結序號      00001-99999 */
       AnsiString  bg_dtclose;          /* 結帳日期              */
       AnsiString  bg_tmclose;          /* 結帳時間              */
       AnsiString  ed_dtclose;          /* 前次結帳日期          */
       AnsiString  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_tclsacc;          /*    05 本日結帳累計金額  */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;          /* 03 現金合計             */
       AnsiString     no_rcstart;       /* 03 發票開始號碼         */
       AnsiString     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          AnsiString  am_dpsitm;           /*   05 部門交易項數       */
          AnsiString  am_dpsale;           /*   05 部門銷售金額       */
          AnsiString  am_dpmitm;           /*   05 部門誤退項數       */
          AnsiString  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       AnsiString     am_tdpsitm;      /* 03 部門合計交易項數     */
       AnsiString     am_dpstot;       /* 03 部門合計銷售金額     */
       AnsiString     am_tdpmitm;      /* 03 部門合計誤退項數     */
       AnsiString     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          AnsiString  qt_vendtot;          /*   05 借零次數           */
          AnsiString  am_vendtot;          /*   05 借零合計金額       */
          AnsiString  qt_feedtot;          /*   05 投零次數           */
          AnsiString  am_feedtot;          /*   05 投零合計金額       */
          AnsiString  qt_droptot;          /*   05 投庫次數           */
          AnsiString  am_droptot;          /*   05 投庫合計金額       */
          AnsiString  qt_update;           /*   05 更正次數           */
          AnsiString  am_update;           /*   05 更正合計金額       */
          AnsiString  qt_void;             /*   05 交易取消次數       */
          AnsiString  am_void;             /*   05 交易取消合計金額   */
          AnsiString  qt_disc;             /*   05 折扣次數(應稅)     */
          AnsiString  am_disc;             /*   05 折扣金額(應稅)     */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券    */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
          AnsiString  qt_train;            /*   05 訓練合計次數       */
          AnsiString  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_agnrcv;           /*   05 代收費用次數       */
          AnsiString  am_agnrcv;           /*   05 代收費用金額       */
          AnsiString  qt_cashCard;         /*   05 現金卡次數         */
          AnsiString  am_cashCard;         /*   05 現金卡金額         */
          AnsiString  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          AnsiString  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收來客數         */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    REPORT *report;     //報表Class
public:
    int __fastcall WriteData( const String StoreNO, const String EcrNO, const String SalesNo ,
                              const AnsiString SalFileName, const AnsiString Version);
    XREPORT();
    ~XREPORT();

};


class CHECKIN : public BASIC
{
private:
    typedef struct {
       AnsiString  no_tm;               /* 03 收銀機機號           */
       AnsiString  no_casher;           /* 03 收銀員號碼           */
       AnsiString  no_seq;              /* 03 日結序號             */
       AnsiString  dt_begin;            /* 03 開始日期             */
       AnsiString  tm_begin;            /* 03 開始時間             */
       AnsiString  dt_end;              /* 03 結束日期             */
       AnsiString  tm_end;              /* 03 結束時間             */

       struct {                   /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {        /* 03 代售付ｔａｂｌｅ       */
          struct {                      /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];
   
       struct {      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;             /* 03 現金合計             */
       AnsiString     no_rcstart;         /* 03 發票開始號碼         */
       AnsiString     no_rcend;           /* 03 發票結束號碼         */

       struct {   /* 03 收銀員明細ｆｉｅｌｄ */
          struct {                      /*   05 借零 table        */
             AnsiString  am_vend;          /*      07 借零金額       */
             AnsiString  tm_vend;          /*      07 借零執行時間   */
          } tb_vendfld[MaxDropRec];
          AnsiString  qt_vendtot;          /*   05 借零次數          */
          AnsiString  am_vendtot;          /*   05 借零合計金額      */

          struct {                          /*   05 投零 table        */
             AnsiString  am_feed;          /*      07 投零金額       */
             AnsiString  tm_feed;          /*      07 投零執行時間   */
          } tg_feedfld[MaxDropRec];
          AnsiString  qt_feedtot;          /*   05 投零次數          */
          AnsiString  am_feedtot;          /*   05 投零合計金額      */

          struct {                      /*   05 投庫 table        */
             AnsiString  am_drop;       /*      07 投庫金額       */
             AnsiString  tm_drop;          /*      07 投庫執行時間   */
          } tg_dropfld[MaxDropRec];
          AnsiString  qt_droptot;          /*   05 投庫次數          */
          AnsiString  am_droptot;          /*   05 投庫合計金額      */

          AnsiString  qt_update;           /*   05 更正次數          */
          AnsiString  am_update;           /*   05 更正合計金額      */
          AnsiString  qt_void;             /*   05 交易取消次數      */
          AnsiString  am_void;             /*   05 交易取消合計金額  */
          AnsiString  qt_dise;             /*   05 折扣次數(應稅)    */
          AnsiString  am_dise;             /*   05 折扣金額(應稅)    */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
       } tb_casher;

       struct {                         /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_cetel;            /*   05 代收電信費次數     */
          AnsiString  am_cetel;            /*   05 代收電信費金額     */
          AnsiString  qt_ffu1;             /*   05 現金卡次數         */
          AnsiString  am_ffu1;             /*   05 現金卡金額         */
          AnsiString  qt_ffu2;             /*   05 扣卡機扣卡次數     */
          AnsiString  am_ffu2;             /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收商品客數       */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss次數     */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss金額     */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;      //X帳檔案結構
    TIniFile *tif_ecr;
    REPORT *report;     //報表Class
public:
    int __fastcall WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version);
    CHECKIN();
    ~CHECKIN();

};


///////////////////////////////////////////////////////////////////////////////
class SPCDATA : public BASIC
{
private:
    //typedef struct {
       AnsiString  no_store;
       AnsiString  no_tm;               // 03 收銀機機號
       AnsiString  no_Dttm;
       AnsiString  no_tran;
       AnsiString  no_seq;              // 03 日結序號
       AnsiString  no_xseq;             //    交班序號
       AnsiString  no_casher;           // 03 收銀員號碼
       AnsiString  no_spcSta;           //
       AnsiString  no_PrvDttm;
       int  tb_setgrpcount;
        struct {
               AnsiString  no_setgrp;
               AnsiString  nm_setgrp;
               int         amt_minpric;
               int         amt_maxpric;
               AnsiString  no_setsubgrp;

          } tb_30TMgrp[110];


       struct {
               AnsiString  no_setgrp;
               AnsiString  nm_setgrp;
               AnsiString  no_setsubgrp;
               int         qt_setgrp;
               float       am_setgrp;
               int         MustPrinted;
          } tb_setgrp[110];

   // } SPOS_SPCRPDT;

    TIniFile *tif_ecr;
    TIniFile *tif_tmp;
    //REPORT *report;     //報表Class
public:
    //重點商品列管帳表
    int __fastcall SPCDATA::WriteSpcInqData(const AnsiString SalFileName, const AnsiString Version,
                                        const int iQrySw );
    int __fastcall SPCDATA::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey );
    int __fastcall SPCDATA::WriteSPCSAL(AnsiString strHdata,  AnsiString SALFileName);
    int __fastcall SPCDATA::Get30TMData();
    int __fastcall SPCDATA::WriteSPCReport(AnsiString strHdata, AnsiString RptSALFileName);
    SPCDATA();
    ~SPCDATA();
};



//////////////////////////////////////////////////////////////
//盤點人員讀帳
class VXZDATA : public BASIC
{
private:
    typedef struct {
       AnsiString  no_tm;               /* 收銀機機號    31H-39H */
       AnsiString  bg_noclose;          /* 日結序號      00001-99999 */
       AnsiString  bg_dtclose;          /* 結帳日期              */
       AnsiString  bg_tmclose;          /* 結帳時間              */
       AnsiString  ed_dtclose;          /* 前次結帳日期          */
       AnsiString  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_tclsacc;          /*    05 本日結帳累計金額  */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;          /* 03 現金合計             */
       AnsiString     no_rcstart;       /* 03 發票開始號碼         */
       AnsiString     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          AnsiString  am_dpsitm;           /*   05 部門交易項數       */
          AnsiString  am_dpsale;           /*   05 部門銷售金額       */
          AnsiString  am_dpmitm;           /*   05 部門誤退項數       */
          AnsiString  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       AnsiString     am_tdpsitm;      /* 03 部門合計交易項數     */
       AnsiString     am_dpstot;       /* 03 部門合計銷售金額     */
       AnsiString     am_tdpmitm;      /* 03 部門合計誤退項數     */
       AnsiString     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          AnsiString  qt_vendtot;          /*   05 借零次數           */
          AnsiString  am_vendtot;          /*   05 借零合計金額       */
          AnsiString  qt_feedtot;          /*   05 投零次數           */
          AnsiString  am_feedtot;          /*   05 投零合計金額       */
          AnsiString  qt_droptot;          /*   05 投庫次數           */
          AnsiString  am_droptot;          /*   05 投庫合計金額       */
          AnsiString  qt_update;           /*   05 更正次數           */
          AnsiString  am_update;           /*   05 更正合計金額       */
          AnsiString  qt_void;             /*   05 交易取消次數       */
          AnsiString  am_void;             /*   05 交易取消合計金額   */
          AnsiString  qt_disc;             /*   05 折扣次數(應稅)     */
          AnsiString  am_disc;             /*   05 折扣金額(應稅)     */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
          AnsiString  qt_train;            /*   05 訓練合計次數       */
          AnsiString  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_agnrcv;           /*   05 代收費用次數       */
          AnsiString  am_agnrcv;           /*   05 代收費用金額       */
          AnsiString  qt_cashCard;         /*   05 現金卡次數         */
          AnsiString  am_cashCard;         /*   05 現金卡金額         */
          AnsiString  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          AnsiString  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收來客數         */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;          //Z帳結構
    TIniFile *tif_ecr;
    TIniFile *tif_tmp;
    REPORT *report;     //報表Class
public:

    int __fastcall WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version);


    //AnsiString __fastcall write_xpath(AnsiString);
    
    VXZDATA();
    ~VXZDATA();
    //void __fastcall mtn_dat_folder();
    //void __fastcall del_list_file(TStringList *list, AnsiString path);

};

////////////////////////////////////////////////////////////////////////////////
//自動日結
class AUTOZDATA : public BASIC
{
private:
    typedef struct {
       AnsiString  no_tm;               /* 收銀機機號    31H-39H */
       AnsiString  bg_noclose;          /* 日結序號      00001-99999 */
       AnsiString  bg_dtclose;          /* 結帳日期              */
       AnsiString  bg_tmclose;          /* 結帳時間              */
       AnsiString  ed_dtclose;          /* 前次結帳日期          */
       AnsiString  ed_tmclose;          /* 前次結帳時間          */

       struct {                      /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_tclsacc;          /*    05 本日結帳累計金額  */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {                      /* 03 代售付ｔａｂｌｅ       */
          struct {                   /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];

       struct {                      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;          /* 03 現金合計             */
       AnsiString     no_rcstart;       /* 03 發票開始號碼         */
       AnsiString     no_rcend;         /* 03 發票結束號碼         */

       struct {                      /* 03 部門別集計TABLE      */
          AnsiString  am_dpsitm;           /*   05 部門交易項數       */
          AnsiString  am_dpsale;           /*   05 部門銷售金額       */
          AnsiString  am_dpmitm;           /*   05 部門誤退項數       */
          AnsiString  am_dpmiss;           /*   05 部門誤退金額       */
       } tb_depcal[20];
       AnsiString     am_tdpsitm;      /* 03 部門合計交易項數     */
       AnsiString     am_dpstot;       /* 03 部門合計銷售金額     */
       AnsiString     am_tdpmitm;      /* 03 部門合計誤退項數     */
       AnsiString     am_dpmtot;       /* 03 部門合計誤退金額     */

       struct {                     /* 03 收銀員明細ｆｉｅｌｄ */
          AnsiString  qt_vendtot;          /*   05 借零次數           */
          AnsiString  am_vendtot;          /*   05 借零合計金額       */
          AnsiString  qt_feedtot;          /*   05 投零次數           */
          AnsiString  am_feedtot;          /*   05 投零合計金額       */
          AnsiString  qt_droptot;          /*   05 投庫次數           */
          AnsiString  am_droptot;          /*   05 投庫合計金額       */
          AnsiString  qt_update;           /*   05 更正次數           */
          AnsiString  am_update;           /*   05 更正合計金額       */
          AnsiString  qt_void;             /*   05 交易取消次數       */
          AnsiString  am_void;             /*   05 交易取消合計金額   */
          AnsiString  qt_disc;             /*   05 折扣次數(應稅)     */
          AnsiString  am_disc;             /*   05 折扣金額(應稅)     */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
          AnsiString  qt_train;            /*   05 訓練合計次數       */
          AnsiString  am_train;            /*   05 訓練合計金額       */
       } tb_casher;

       struct {                   /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_agnrcv;           /*   05 代收費用次數       */
          AnsiString  am_agnrcv;           /*   05 代收費用金額       */
          AnsiString  qt_cashCard;         /*   05 現金卡次數         */
          AnsiString  am_cashCard;         /*   05 現金卡金額         */
          AnsiString  qt_posZRpt;          /*   05 扣卡機扣卡次數     */
          AnsiString  am_posZRpt;          /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收來客數         */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss值次數   */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss值金額   */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_ZRPDT;                     /* Recoder Length = 1998 */

    SPOS_ZRPDT *z;          //Z帳結構

    ///////////////////////////////////////
     typedef struct {
       AnsiString  no_tm;               /* 03 收銀機機號           */
       AnsiString  no_casher;           /* 03 收銀員號碼           */
       AnsiString  no_seq;              /* 03 日結序號             */
       AnsiString  dt_begin;            /* 03 開始日期             */
       AnsiString  tm_begin;            /* 03 開始時間             */
       AnsiString  dt_end;              /* 03 結束日期             */
       AnsiString  tm_end;              /* 03 結束時間             */

       struct {                   /* 03 交易明細ｆｉｅｌｄ   */
          AnsiString  qt_tcust;            /*    05 本日來客數        */
          AnsiString  qt_trans;            /*    05 本日交易品項      */
          AnsiString  am_trcpntx;          /*    05 發票日結帳額(免)  */
          AnsiString  am_trcptax;          /*    05 發票日結帳額(稅)  */
          AnsiString  am_trcptot;          /*    05 發票日結帳金額    */
          AnsiString  qt_miss;             /*    05 誤打發票次數      */
          AnsiString  am_missntx;          /*    05 誤打發票金額(免)  */
          AnsiString  am_misstax;          /*    05 誤打發票金額(稅)  */
          AnsiString  am_miss;             /*    05 誤打發票金額      */
          AnsiString  am_totsale;          /*    05 本日實際銷貨收入  */
       } tb_trans;

       struct {        /* 03 代售付ｔａｂｌｅ       */
          struct {                      /*    05 代售付品項ｉｔｅｍ  */
             AnsiString  qt_subrev;        /*       07 項目交易回數     */
             AnsiString  am_subrev;        /*       07 項目交易金額     */
          } it_subrev[8];
          AnsiString  qt_totrev;           /*    05 代售付ＭＥＭＵ合計回數  */
          AnsiString  am_totrev;           /*    05 代售付ＭＥＭＵ合計金額  */
       } tb_subrev[5];
   
       struct {      /* 03 代現金ｔａｂｌｅ     */
          AnsiString  qt_bonus;            /*    05 禮券回數          */
          AnsiString  am_bonus;            /*    05 禮券金額          */
          AnsiString  qt_vndcpn;           /*    05 廠商折價券回數    */
          AnsiString  am_vndcpn;           /*    05 廠商折價券金額    */
          AnsiString  qt_stocpn;           /*    05 折價券回數        */
          AnsiString  am_stocpn;           /*    05 折價券金額        */
          AnsiString  qt_totcash;          /*    05 代現金合計回數    */
          AnsiString  am_totcash;          /*    05 代現金合計金額    */
          AnsiString  qt_bonout;           /*    05 溢收合計回數      */
          AnsiString  am_bonout;           /*    05 溢收合計金額      */
       } tb_cash;

       AnsiString     am_cash;             /* 03 現金合計             */
       AnsiString     no_rcstart;         /* 03 發票開始號碼         */
       AnsiString     no_rcend;           /* 03 發票結束號碼         */

       struct {   /* 03 收銀員明細ｆｉｅｌｄ */
          struct {                      /*   05 借零 table        */
             AnsiString  am_vend;          /*      07 借零金額       */
             AnsiString  tm_vend;          /*      07 借零執行時間   */
          } tb_vendfld[MaxDropRec];
          AnsiString  qt_vendtot;          /*   05 借零次數          */
          AnsiString  am_vendtot;          /*   05 借零合計金額      */

          struct {                          /*   05 投零 table        */
             AnsiString  am_feed;          /*      07 投零金額       */
             AnsiString  tm_feed;          /*      07 投零執行時間   */
          } tg_feedfld[MaxDropRec];
          AnsiString  qt_feedtot;          /*   05 投零次數          */
          AnsiString  am_feedtot;          /*   05 投零合計金額      */

          struct {                      /*   05 投庫 table        */
             AnsiString  am_drop;       /*      07 投庫金額       */
             AnsiString  tm_drop;          /*      07 投庫執行時間   */
          } tg_dropfld[MaxDropRec];
          AnsiString  qt_droptot;          /*   05 投庫次數          */
          AnsiString  am_droptot;          /*   05 投庫合計金額      */

          AnsiString  qt_update;           /*   05 更正次數          */
          AnsiString  am_update;           /*   05 更正合計金額      */
          AnsiString  qt_void;             /*   05 交易取消次數      */
          AnsiString  am_void;             /*   05 交易取消合計金額  */
          AnsiString  qt_dise;             /*   05 折扣次數(應稅)    */
          AnsiString  am_dise;             /*   05 折扣金額(應稅)    */
          AnsiString  qt_disret1;          /*   05 加值機開/售卡次數  */
          AnsiString  am_disret1;          /*   05 加值機開/售卡金額  */
          AnsiString  qt_disret2;          /*   05 加值機加值次數     */
          AnsiString  am_disret2;          /*   05 加值機加值金額     */
          AnsiString  qt_openbox;          /*   05 開錢櫃次數         */
          AnsiString  qt_pack;             /*   05 加值機結帳次數  2007/12/01 找餘額券   */
          AnsiString  am_pack;             /*   05 加值機結帳金額     */
       } tb_casher;

       struct {                         /*   05 追加 table        */
          AnsiString  qt_inpdp;            /*   05 手動輸入部門次數   */
          AnsiString  am_inpdp;            /*   05 手動輸入部門金額   */
          AnsiString  qt_rcp0;             /*   05 發票金額0次數      */
          AnsiString  qt_parking;          /*   05 代收停車費次數     */
          AnsiString  am_parking;          /*   05 代收停車費金額     */
          AnsiString  qt_cetel;            /*   05 代收電信費次數     */
          AnsiString  am_cetel;            /*   05 代收電信費金額     */
          AnsiString  qt_ffu1;             /*   05 現金卡次數         */
          AnsiString  am_ffu1;             /*   05 現金卡金額         */
          AnsiString  qt_ffu2;             /*   05 扣卡機扣卡次數     */
          AnsiString  am_ffu2;             /*   05 扣卡機扣卡金額     */
          AnsiString  tx_mark;             /*   05 版本前置符號       */
          AnsiString  tx_ver;              /*   05 版本號碼           */
          AnsiString  qt_ffu4;             /*   05 代收商品客數       */
          AnsiString  am_ffu4;             /*   05 備用四金額         */
          AnsiString  qt_ffu5;             /*   05 TM廢棄Loss次數     */
          AnsiString  am_ffu5;             /*   05 TM廢棄Loss金額     */
          AnsiString  qt_ffu6;             /*   05 促銷折扣/讓次數    */
          AnsiString  am_ffu6;             /*   05 促銷折扣/讓金額    */
          AnsiString  qt_ffu7;             /*   05 加值機加值取消次數 */
          AnsiString  am_ffu7;             /*   05 加值機加值取消金額 */
          AnsiString  qt_ffu8;             /*   05 備用八次數         */
          AnsiString  am_ffu8;             /*   05 備用八金額         */
          AnsiString  qt_ffu9;             /*   05 備用九次數         */
          AnsiString  am_ffu9;             /*   05 備用九金額         */
          AnsiString  qt_ffu10;            /*   05 備用十次數         */
          AnsiString  am_ffu10;            /*   05 備用十金額         */
       } tb_newadd;
    } SPOS_XRPDT;                       /* Recoder Length = 3404 */

    SPOS_XRPDT *x;      //X帳檔案結構

    //////////////////////////////////////////

    TIniFile *tif_ecr;
    TIniFile *tif_tmp;
    REPORT *report;     //報表Class
public:


    //TStringList *Autotsl_rec, *Autotsl_tot, *Autotsl_drop, *Autotsl_easy_card, *Autotsl_pay_sub;
    //TStringList *Autotsl_subsale,*Autotsl_bil, *Autotsl_fetc_card, *Auto_Sal;

    int __fastcall WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version, const String AutoZDttm);

    AnsiString __fastcall write_xpath(AnsiString);
    int __fastcall GetYYYYMDD_SalData(AnsiString &AutoZCnt, AnsiString AutoZdt , int AutoZ);
    AnsiString  __fastcall AUTOZDATA::GetSysDttmMaxTransNo(AnsiString sDttm);

    AnsiString __fastcall AutoXData(const String StoreNO, const String EcrNO, const String SalesNo ,
                             const String SalFileName, const String Version, const AnsiString AutoZ_cnt,
                             const AnsiString AutoZ_Dt );

    int __fastcall AUTOZDATA::GetZDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo ,
                          String Version, String PZ_date, String Z_date, String DDZ_1099, String ZDT_1050,
                          String &RtnZDTData, AnsiString &Tclsacc,
                          String &str_zrp_path, String &str_dpt_path  );

    AUTOZDATA();
    ~AUTOZDATA();
    void __fastcall mtn_dat_folder();
    void __fastcall del_list_file(TStringList *list, AnsiString path);

};


extern PACKAGE XDATA *xdata;
extern PACKAGE ZDATA *zdata;
extern PACKAGE XREPORT *xreport;
extern PACKAGE CHECKIN *checkin;
extern PACKAGE SPCDATA *AllSpcData;
extern PACKAGE VXZDATA *vxzdata;     //盤點人員讀帳
extern PACKAGE AUTOZDATA *autozdata; //自動日結

#endif
