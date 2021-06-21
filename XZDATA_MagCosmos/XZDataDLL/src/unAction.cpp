//---------------------------------------------------------------------------

#include <sstream>
#include <algorithm>
#include <cassert>
#include <fstream>

#include <Filectrl.hpp>
#include <stdio.h>
#include <inifiles.hpp>


#include <vcl.h>
#pragma hdrstop

#include "unAction.h"
#include "ULOG.h"
#include "UXZDataClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAction *frmAction;

XDATA *xdata;
ZDATA *zdata;
XREPORT *xreport;
CHECKIN *checkin;
SPCDATA *AllSpcData;
VXZDATA *vxzdata;     //盤點人員讀帳
AUTOZDATA *autozdata; //自動日結
//---------------------------------------------------------------------------
__fastcall TfrmAction::TfrmAction(TComponent* Owner)
        : TForm(Owner)
{
}


String __stdcall _ByteArrayToAnsiString(byte *bary)
{
        String s;
        int ilen;

        char *ptr;
        ptr=(char *)bary;
        s=Trim(ptr);
         //ilen=strlen(ptr);


        //ilen=sizeof bary/sizeof *bary;
        //ilen=50; //sizeof *bary;

        //for (int i=0; i<ilen; i++)    //for (int i=0; i<50; i++)
        //     s += Char(bary[i]);
        //memcpy( s.c_str(), &bary[0], 25 );

        return s;
}


//------------------------------------------------------------------------------
void __fastcall _DelTree(AnsiString dir)
{
  //_ReNameTree(dir);  //用更名備份來代替刪目錄
  //return;

  String d=dir;
  String FileName;
  TSearchRec SearchRec;

  String temp;

  temp="★★【注意】_DelTree() 命令開始 , 刪除目錄 -> "+dir;
  writelog(temp);


  if (!DirectoryExists(dir))
     {
       temp="★★【錯誤】目錄不存在!! -> "+dir;
       writelog(temp);
       return;  // 要 #include "FileCtrl.hpp" 才有這個函式
     }

  if (d.SubString(d.Length(),1)!="\\") d=d+"\\";

  if (FindFirst(d+"*.*", faAnyFile, SearchRec) == 0)
     {
       do {  if ((SearchRec.Attr == (faDirectory | faArchive)) ||
                (SearchRec.Attr == (faDirectory)))
                {
                  if ((SearchRec.Name!=".") && (SearchRec.Name!=".."))
                     {
                       _DelTree(d+SearchRec.Name);  //遞迴呼叫
                       if (RemoveDir(d+SearchRec.Name))
                          {
                            temp="_DelTree() 刪目錄成功!! -> "+d+SearchRec.Name;
                            writelog(temp);
                          }
                       else
                          {
                            temp="_DelTree() 刪目錄失敗!! -> "+d+SearchRec.Name;
                            writelog(temp);
                          }
                     }
                 }
             else
                 {
                   FileSetAttr(d+SearchRec.Name,faArchive);
                   if (DeleteFile(d+SearchRec.Name))
                      {
                        temp="_DelTree() 刪檔成功!! -> "+d+SearchRec.Name;
                        writelog(temp);
                      }
                   else
                      {
                        temp="_DelTree() 刪檔失敗!! -> "+d+SearchRec.Name;
                        writelog(temp);
                      }
                 }
          } while (FindNext(SearchRec) == 0);

       FindClose(SearchRec);

     }

  if (RemoveDir(d))
     {
       temp="_DelTree() 刪目錄成功!! -> "+d;
       writelog(temp);
     }
  else
     {
       temp="_DelTree() 刪目錄失敗!! -> "+d;
       writelog(temp);
     }
}


void __fastcall GetFileList(TStringList *lsFileList, AnsiString sDir,
                                         int iBefDay)
{
    AnsiString str_dir,stmp;
    str_dir = sDir;

    TSearchRec SearchRec;
    int iAttributes = 0;
    iAttributes |= faAnyFile * true;

   // lsFileList->Clear();
    lsFileList->Duplicates = dupIgnore;
   // if (FindFirst((str_dir + "*.fd"), iAttributes, SearchRec) == 0)
    if (FindFirst((str_dir + "*.*"), iAttributes, SearchRec) == 0)
    {
        do
        {
            if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
            {
                if( SearchRec.Attr == faHidden	|| SearchRec.Attr==faDirectory )
                  {
                   ;;
                  }
                else
                  {
                    if( SearchRec.Name != "." && SearchRec.Name != ".."  )
                      {
                       if( iBefDay == 0)
                         {
                          lsFileList->Add(SearchRec.Name);
                         }
                       else
                         {
                          if( FileDateToDateTime(SearchRec.Time) < ( Now()-iBefDay ) )
                            { // 比對檔案日期,多少iBefDay天之前
                              // stmp=DateToStr( FileDateToDateTime(SearchRec.Time));
                              lsFileList->Add(SearchRec.Name);
                            }
                         }
                      }
                  }
            }

         } while (FindNext(SearchRec) == 0);

        FindClose(SearchRec);
    }
}

void __fastcall MtnDirFile(AnsiString sDir, int MtnDay)
{

    AnsiString mtn_file,FileExt,stmp ;

    TStringList *tsl = new TStringList();

    stmp.sprintf("MtnDirFile 刪除 %s 目錄下,%d天前的檔案",
                          sDir,MtnDay);
    writelog(stmp);

    tsl->Clear();
    GetFileList(tsl, sDir , MtnDay );
   // tsl->Sort();

    for (int i=0; i<tsl->Count; i++)
    {
        mtn_file = ExtractFileName(tsl->Strings[i]);
        FileExt=ExtractFileExt(mtn_file);

        DeleteFile(sDir + mtn_file);
        stmp.sprintf("MtnSalFile : DeleteFile(%s%s)",
                      sDir.c_str(),mtn_file.c_str());
        writelog(stmp);
    }
    delete tsl;

}

void __fastcall MtnDirFile_Ex(AnsiString sDir, int MtnDay, int FileCnt)
{

    AnsiString mtn_file,FileExt,stmp ;

    TStringList *tsl = new TStringList();

    stmp.sprintf("MtnDirFile_Ex 刪除 %s 目錄下,%d天前的檔案",
                          sDir,MtnDay);
    writelog(stmp);

    tsl->Clear();
    GetFileList(tsl, sDir , MtnDay );
   // tsl->Sort();
    int icnt=0;
    for (int i=0; i<tsl->Count; i++)
    {
        mtn_file = ExtractFileName(tsl->Strings[i]);
        FileExt=ExtractFileExt(mtn_file);

        DeleteFile(sDir + mtn_file);
        stmp.sprintf("MtnSalFile :%d DeleteFile(%s%s)",
                       i, sDir.c_str(),mtn_file.c_str());
        writelog(stmp);

        icnt++;
        if( (FileCnt > 0) && (icnt >= FileCnt) )
           break;
    }

    delete tsl;

}





//---------------------------------------------------------------------------
int __fastcall TfrmAction::_INITXZDATA(byte *OrgStoreNO, byte *StoreNO, byte *EcrNO, byte *Version)
{
 int int_result, i,j, z_cnt, zini_nzno_seq;
 String  stmp, dd, ddzz; //tmp_buf[181], xzini[51];
 TIniFile *MyIni;

  int_result = 0;

  stmp.sprintf("%s",XZDATADIR);
  if (!DirectoryExists(stmp))
       {
        //write_log_file("XZDATA:建立目錄 "+stmp)
        ForceDirectories(stmp);
       }

  if (!DirectoryExists("C:\\FTLPOS\\XZDATA\\WX\\"))
        ForceDirectories("C:\\FTLPOS\\XZDATA\\WX\\");

 MyIni = new TIniFile(XZDATA_INI);

 //[ECR}
 MyIni->WriteString("ECR","OrgStoreNO",_ByteArrayToAnsiString(OrgStoreNO));  //Jack Add _ByteArrayToAnsiString
 MyIni->WriteString("ECR","StoreNO",_ByteArrayToAnsiString(StoreNO));
 MyIni->WriteString("ECR","EcrNO",_ByteArrayToAnsiString(EcrNO));
 MyIni->WriteString("ECR","Version",_ByteArrayToAnsiString(Version));


 //[X] 區段
 stmp=MyIni->ReadString("X","ptran_cnt","00000000");
 MyIni->WriteString("X","ptran_cnt",stmp);

 stmp=MyIni->ReadString("X","sx_date","");
 MyIni->WriteString("X","sx_date",stmp);

 stmp=MyIni->ReadString("X","ex_date","");
 MyIni->WriteString("X","ex_date",stmp);

 i=MyIni->ReadInteger("X","pxno_seq",0);
 MyIni->WriteInteger("X","pxno_seq",i);

 j=MyIni->ReadInteger("X","Autoxno_seq",i);    //自動日結用
 MyIni->WriteInteger("X","Autoxno_seq",j);

 stmp=MyIni->ReadString("X","xfilename","");
 MyIni->WriteString("X","xfilename",stmp);

 stmp=MyIni->ReadString("X","xrptname","");
 MyIni->WriteString("X","xrptname",stmp);

 stmp=MyIni->ReadString("X","CHRRPTNAME","");
 MyIni->WriteString("X","CHRRPTNAME",stmp);



 //[Z] 區段
 stmp=MyIni->ReadString("Z","pz_date",FormatDateTime("yyyymmddhhnnss",Now()));
 MyIni->WriteString("Z","pz_date",stmp);
 dd=stmp;

 i=MyIni->ReadInteger("Z","pzno_seq",0);
 MyIni->WriteInteger("Z","pzno_seq",i);

 i=MyIni->ReadInteger("Z","nzno_seq",1);
 MyIni->WriteInteger("Z","nzno_seq",i);
 zini_nzno_seq=i;

 stmp=MyIni->ReadString("Z","pz_cnt","00000");
 MyIni->WriteString("Z","pz_cnt",stmp);

 stmp=MyIni->ReadString("Z","nz_cnt","00001");
 MyIni->WriteString("Z","nz_cnt",stmp);
 z_cnt=stmp.ToInt();

 //z_cnt = zini_nz_cnt;
 //String dd=zini_pz_date;
 stmp=MyIni->ReadString( "Z","datez",dd.SubString(7,2)+IntToStr(zini_nzno_seq) );
 MyIni->WriteString("Z","datez",stmp);


 stmp=MyIni->ReadString("Z","zfilename","");
 MyIni->WriteString("Z","zfilename",stmp);

 stmp=MyIni->ReadString("Z","zrptname","");
 MyIni->WriteString("Z","zrptname",stmp);

 stmp=MyIni->ReadString("Z","deptname","");
 MyIni->WriteString("Z","deptname",stmp);

 //strcpy(zini_inqtname,MyIni->ReadString("Z","inqtname","").c_str());
 i=MyIni->ReadInteger("Z","am_tclsacc",0);
 MyIni->WriteInteger("Z","am_tclsacc",i);


 ddzz.sprintf("%2s%02d",dd.SubString(7,2),IntToStr(zini_nzno_seq));
 stmp=MyIni->ReadString("Z","yyyymmddz",dd.SubString(1,6)+String(ddzz));
 MyIni->WriteString("Z","yyyymmddz",stmp);

 ////////////////////////////////
 stmp=MyIni->ReadString("VXZ", "nzno_seq", "1");
 MyIni->WriteString("VXZ", "nzno_seq", stmp);

 stmp=MyIni->ReadString("VXZ", "PZ_DATE", "");
 MyIni->WriteString("VXZ", "PZ_DATE", stmp);

 stmp=MyIni->ReadString("VXZ","CD_SATNCODE","");
 MyIni->WriteString("VXZ", "CD_SATNCODE", stmp);

 stmp=MyIni->ReadString("VXZ","CS_ENTRY","");
 MyIni->WriteString("VXZ","CS_ENTRY",stmp);

 stmp=MyIni->ReadString("AUTOZ","LST_ZCNT_DTTM",FormatDateTime("yyyymmdd",Now()));   //換日參考日
 MyIni->WriteString("AUTOZ","LST_ZCNT_DTTM",stmp);     //換日參考日

 stmp=MyIni->ReadString("AUTOZ","LST_AUTOZ_DTTM",FormatDateTime("yyyymmdd",Now()));  //前次自動日結時間
 MyIni->WriteString("AUTOZ","LST_AUTOZ_DTTM",stmp);     //前次自動日結時間

 stmp=MyIni->ReadString("AUTOZ","NST_AUTOZ_DTTM","");
 if( Trim(stmp)=="")
     MyIni->WriteString("AUTOZ","NST_AUTOZ_DTTM","20131015");  //自動日結時間啟用日期
 //if( Trim(stmp)="")
 //   tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM",dd.SubString(1,8));  //現再自動日結時間

 stmp=MyIni->ReadString("XREPORT","XREPORTDATE",FormatDateTime("yyyymmdd",Now()));
 MyIni->WriteString("XREPORT","XREPORTDATE",stmp);
 stmp=MyIni->ReadString("XREPORT","ZDTCnts","0" );
 MyIni->WriteString("XREPORT","ZDTCnts",stmp);
 stmp=MyIni->ReadString("XREPORT","SALCnts","0" );
 MyIni->WriteString("XREPORT","SALCnts",stmp);
 
 delete MyIni;
 return int_result;

}

//---------------------------------------------------------------------------
int __fastcall TfrmAction::_WX(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
    int int_result = 0;
    xdata = new XDATA();
    int_result = xdata->WriteData( _ByteArrayToAnsiString(StoreNO),
                                   _ByteArrayToAnsiString(EcrNO),
                                   _ByteArrayToAnsiString(SalesNo),
                                   _ByteArrayToAnsiString(SalFileName),
                                   _ByteArrayToAnsiString(Version) );
    delete xdata;

    return int_result;
}


//---------------------------------------------------------------------------
int __fastcall TfrmAction::_WZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int AutoZ)
{
    int int_result = 0;
    AnsiString stmp;

    if( AutoZ == 1)
      {

         autozdata = new AUTOZDATA();
         int_result = autozdata->WriteData( _ByteArrayToAnsiString(StoreNO),
                                   _ByteArrayToAnsiString(EcrNO),
                                   _ByteArrayToAnsiString(SalesNo),
                                   _ByteArrayToAnsiString(SalFileName),
                                   _ByteArrayToAnsiString(Version),
                                   "" );
         delete autozdata;

         // 2013/10/21 Update
         if( int_result==0 || int_result==2 )
           {
             if (FileExists(ZDATATMP_INI))
                {
                  DeleteFile(ZDATATMP_INI);
                  stmp.sprintf("TfrmAction::_WZ Return=%d, 刪除檔案=%s ", int_result, ZDATATMP_INI);
                  writelog(stmp);

                }
           }

         if( int_result!=0 )
             int_result=1; // 日結 OK
         
      }
    else
      {
       zdata = new ZDATA();
       int_result = zdata->WriteData( _ByteArrayToAnsiString(StoreNO),
                                   _ByteArrayToAnsiString(EcrNO),
                                   _ByteArrayToAnsiString(SalesNo),
                                   _ByteArrayToAnsiString(SalFileName),
                                   _ByteArrayToAnsiString(Version),
                                   "" );
       delete zdata;
     }
    return int_result;
}

//---------------------------------------------------------------------------
int __fastcall TfrmAction::_WXReport(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
    xreport = new XREPORT();
    xreport->WriteData( _ByteArrayToAnsiString(StoreNO),
                        _ByteArrayToAnsiString(EcrNO),
                        _ByteArrayToAnsiString(SalesNo),
                        _ByteArrayToAnsiString(SalFileName),
                        _ByteArrayToAnsiString(Version) );
    delete xreport;

    return 0;
}

//---------------------------------------------------------------------------
int __fastcall TfrmAction::_Checkin(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
    checkin = new CHECKIN();
    checkin->WriteData( _ByteArrayToAnsiString(StoreNO),
                        _ByteArrayToAnsiString(EcrNO),
                        _ByteArrayToAnsiString(SalesNo),
                        _ByteArrayToAnsiString(SalFileName),
                        _ByteArrayToAnsiString(Version) );
    delete checkin;

    return 0;

}

//---------------------------------------------------------------------------
int __fastcall TfrmAction::_WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
    AnsiString str_ecr_ini, str_tmp_ini, stmp;

    str_ecr_ini = XZDATA_INI;
    str_tmp_ini = XDATATMP_INI;

    stmp.sprintf("_WX_OK : (%s, %s, %s, %s)",
                      StoreNO, EcrNO, SalesNo, SalFileName);
    writelog(stmp);

    TIniFile *tif_ecr;
    TIniFile *tif_tmp;

    AnsiString str_xdt, str_xrp, str_97, str_crp,
               str_tran_cnt,
               str_ptran_cnt,
               str_pxno_seq,
               str_sx_date,
               str_ex_date,
               str_tot_opne_x,
               str_x_pay_cnt,
               str_sys_date;

    // 2013/10/21 Update
    if (!FileExists(str_tmp_ini))
       {
         stmp.sprintf("_WX_OK :(%s) 不存在...Return 1.",str_tmp_ini );
         writelog(stmp);
         return 1;
       }


    try
    {
        tif_ecr = new TIniFile(XZDATA_INI);
        tif_tmp = new TIniFile(XDATATMP_INI);

        //READ X INI
        str_97  = tif_tmp->ReadString("X","x0097filename", " ");
        str_xdt = tif_tmp->ReadString("X","XFILENAME"    , " ");
        str_xrp = tif_tmp->ReadString("X","XRPTNAME"     , " ");
        str_crp = tif_tmp->ReadString("X","CHRRPTNAME"     , " ");
        str_sys_date=tif_tmp->ReadString("X","XFILEDTTM", "");

        str_tran_cnt  = tif_tmp->ReadString("ECR","TRAN_CNT"  , " ");
        str_ptran_cnt = tif_tmp->ReadString("X","PTRAN_CNT"   , " ");
        str_pxno_seq  = tif_tmp->ReadString("X","PXNO_SEQ"    , " ");
        str_sx_date   = tif_tmp->ReadString("X","SX_DATE"     , " ");
        str_ex_date   = tif_tmp->ReadString("X","EX_DATE"     , " ");
        //str_tot_opne_x= tif_tmp->ReadString("SUM","tot_open_x", " ");
        //str_x_pay_cnt = tif_tmp->ReadString("PAY","x_pay_cnt" , "0");



        //READ END
        //WRITE X INI
        tif_ecr->WriteString("X","x0097filename", str_97);
        tif_ecr->WriteString("X","XFILENAME", str_xdt);
        tif_ecr->WriteString("X","XRPTNAME" , str_xrp);
        tif_ecr->WriteString("X","CHRRPTNAME" , str_crp);
        tif_ecr->WriteString("X","XFILEDTTM", str_sys_date);
        tif_ecr->WriteString("X","SALESLOGINDTTM" , "");


        tif_ecr->WriteString("ECR","TRAN_CNT"  , str_tran_cnt);
        tif_ecr->WriteString("X","PTRAN_CNT"   , str_ptran_cnt);
        tif_ecr->WriteString("X","PXNO_SEQ"    , str_pxno_seq);
        tif_ecr->WriteString("X","SX_DATE"     , str_sx_date);
        tif_ecr->WriteString("X","EX_DATE"     , str_ex_date);
        tif_ecr->WriteString("ECR","tot_open_x", "0");
        tif_ecr->WriteString("PAY","x_pay_cnt" , "0");


        //WRITE END
    }
    __finally
    {
        delete tif_ecr, tif_tmp;
    }

    DeleteFile(str_tmp_ini);
    writelog("刪除檔案 " + str_tmp_ini);

    if (!DirectoryExists("C:\\FTLPOS\\XZDATA\\WX\\"))
        ForceDirectories("C:\\FTLPOS\\XZDATA\\WX\\");

    DeleteFile(str_tmp_ini);
    writelog("WX_OK 完成: 刪除檔案 " + str_tmp_ini);

    return 1;


}


//---------------------------------------------------------------------------
int __fastcall TfrmAction::_WZ_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, bool delFolder)
{
//Jack Note: 傳入的 delFolder 若為 false 也是會判斷出 NULL

    AnsiString str_ecr_ini, str_tmp_ini, str_zini_datez, sInvoFile,
               sInvoFilePath, stmp, sTmpInvoFile, sInvoTransFilePath;

    AnsiString str_sal_file, str_log_file, str_ecr_file, str_mwlog_file,
                   str_zsl_file,str_dbtxt_file,str_dbtxt2_file, str_802log_file;
    int OutNZCnt=0;

    str_ecr_ini = XZDATA_INI;
    str_tmp_ini = ZDATATMP_INI;  //".\\ZDT_TMP.INI";


    stmp.sprintf("_WZ_OK : (%s, %s, %s, %s)",
                      StoreNO, EcrNO, SalesNo, SalFileName);
    writelog(stmp);


    // 2013/10/21 Update
    if (!FileExists(ZDATATMP_INI))
       {
         stmp.sprintf("_WZ_OK :(%s) 不存在...Return 1.",ZDATATMP_INI );
         writelog(stmp);
         return 1;
       }


    AnsiString str_folder_path;

    TIniFile *tif_ecr;
    TIniFile *tif_tmp;

    AnsiString str_zdt, str_zrp, str_dpt, str_98, str_pz_date, str_am_tclsacc,
               str_ipt, str_zsl, str_tran_cnt, ZfileTranCnt;
    try
    {
        tif_ecr = new TIniFile(XZDATA_INI);
        tif_tmp = new TIniFile(ZDATATMP_INI);

        //READ Z INI
        //str_zini_datez=tif_tmp->ReadString("Z", "DATEZ", "");
        str_tran_cnt  = tif_tmp->ReadString("ECR","TRAN_CNT"  , " ");
        str_zdt = tif_tmp->ReadString("Z", "ZFILENAME"     , " ");
        str_zrp = tif_tmp->ReadString("Z", "ZRPTNAME"      , " ");
        str_dpt = tif_tmp->ReadString("Z", "DEPTNAME"      , " ");
        str_ipt = tif_tmp->ReadString("Z", "INQTNAME"      , " ");
        str_zsl = tif_tmp->ReadString("Z", "ZSLNAME"     , " ");
        str_98  = tif_tmp->ReadString("Z", "x0098filename" , " ");

        ZfileTranCnt=tif_tmp->ReadString("Z","ZfileTranCnt" , "");  // ZDT 交易序號


        str_pz_date    = tif_tmp->ReadString("Z","PZ_DATE"     , " ");


        str_am_tclsacc = tif_tmp->ReadString("Z","AM_TCLSACC"  , " ");
        //READ END

        //WRITE Z INI
        tif_ecr->WriteString("Z", "ZFILENAME"     , str_zdt);
        tif_ecr->WriteString("Z", "ZRPTNAME"      , str_zrp);
        tif_ecr->WriteString("Z", "DEPTNAME"      , str_dpt);
        tif_ecr->WriteString("Z", "INQTNAME"      , str_ipt);
        tif_ecr->WriteString("Z", "ZSLNAME"       , str_zsl);
        tif_ecr->WriteString("Z", "x0098filename" , str_98);

        stmp=tif_tmp->ReadString("AUTOZ","LST_AUTOZ_DTTM","");   //前次自動日結時間
        tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM",stmp);   //前次自動日結時間

        //stmp=tif_tmp->ReadString("AUTOZ","NST_AUTOZ_DTTM","");
        //tif_ecr->WriteString("AUTOZ","NST_AUTOZ_DTTM",stmp);  //現再自動日結時間

        stmp=tif_tmp->ReadString("AUTOZ","LST_ZCNT_DTTM","");   //換日參考日
        tif_ecr->WriteString("AUTOZ","LST_ZCNT_DTTM",stmp);     //換日參考日


        //將X帳歸零 2013/09/17 Update 由日結/自動日結處理
        //tif_ecr->WriteString("X", "PTRAN_CNT", "00000000");
        //tif_ecr->WriteString("X", "SX_DATE"  , "");
        //tif_ecr->WriteString("X", "EX_DATE"  , "");
        /////////////////////////////////////////////////////////

        //tif_ecr->WriteString("X", "PXNO_SEQ" , "0");


        tif_ecr->WriteString("Z","PZ_DATE"     , str_pz_date   );
        tif_ecr->WriteString("Z","AM_TCLSACC" , str_am_tclsacc);

        tif_ecr->WriteString("ECR","TRAN_CNT"  , str_tran_cnt);
        tif_ecr->WriteString("ECR","tot_open_x", "0");
        tif_ecr->WriteString("ECR","tot_open_z", "0");
        tif_ecr->WriteString("PAY","x_pay_cnt" , "0");
        tif_ecr->WriteString("PAY","z_pay_cnt" , "0");

        tif_ecr->WriteString("Z", "ZfileTranCnt" , ZfileTranCnt);    // ZDT 交易序號

        writelog("■WZ_OK:Clear tot_open_x, tot_open_z  ■");

        //巡迴驗收
        tif_ecr->WriteString("SPC","pSPCno_seq","0" );
        //tif_ecr->WriteString("SPC","pSPCData","" );     2012/04/05 Update
        tif_ecr->WriteString("SPC","SPC0080filename", "");

        //tif_ecr->WriteString("SPC","TMPSPRptname", "");
        //tif_ecr->WriteString("SPC","TMPSPfilename", "");
        //tif_ecr->WriteString("SPC","pSPCDttm","");

        tif_ecr->WriteString("VXZ", "PZNO_SEQ", "0");
        tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");
        tif_ecr->WriteString("VXZ", "PZ_DATE", "");

        tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        tif_ecr->WriteString("VXZ","CS_ENTRY","");

        OutNZCnt=tif_ecr->ReadInteger("Z", "nz_cnt", 0);


        //WRITE END

        //if (delFolder)
        //{
            //James
            //TIniFile *ini_ecr = new TIniFile(XZDATA_INI);
            String report_year_month=tif_ecr->ReadString("Backup", "PZ_DATE","").SubString(1,6); // 前次結帳系統年月  前次日結
            String today_year_month=Date().FormatString("yyyymm"); // 系統年月


            int iMainTainHistory=0;
            iMainTainHistory=tif_ecr->ReadInteger("Backup", "MAINTAINHISTORY", 1);
            tif_ecr->WriteInteger("Backup", "MAINTAINHISTORY",iMainTainHistory );

            str_zini_datez=tif_ecr->ReadString("Backup", "DATEZ", "");    //備份 sal*.TXT條件  日結 ddz

            int report_day=_StrToInt(tif_ecr->ReadString("Backup", "PZ_DATE","").SubString(7,2)); //前次結帳系統日  前次日結
            //int report_day=_StrToInt( report_year_month.SubString(7,2) ); //帳表日

            AnsiString syyyymmdd = FormatDateTime("yyyymmdd",Now()+1);
            //int today_day=_StrToInt(Date().FormatString("dd"));
            int today_day=_StrToInt(syyyymmdd.SubString(7,2)  );                              //系統日

            //delete ini_ecr;  //2003/03/06

        sInvoTransFilePath="C:\\FTLPOS\\RCP\\TRANS\\";
        sInvoFilePath="C:\\FTLPOS\\RCP\\";
        sTmpInvoFile=ExtractFileName(str_zdt);
        sInvoFile=sInvoFilePath+ChangeFileExt(sTmpInvoFile,".VOL");

        if (iMainTainHistory && Trim(str_zini_datez)!="" ) //delFolder)
        {
            // 刪除本次日結與系統日間的目錄
            writelog("■WZ_OK: 歷史檔案維護功能開始 ■"); //2003/02/07

            writelog("前次日結YYYYMM："+report_year_month);
            writelog("系統年月："+today_year_month);
            writelog("前次日結 Backup->PZ_DATE..DD："+IntToStr(report_day));
            writelog("系統日+1："+IntToStr(today_day));

            try
              {
                if (Trim(report_year_month)!="" && report_year_month<=today_year_month)
                   {
                       if (report_year_month==today_year_month) //帳表年月及系統年月相同
                          {
                            if (report_day>=today_day)          // 帳表DD>=系統DD
                               {
                                  //;; //do nothing ...
                                  String del_dir="C:\\FTLPOS\\History\\"+FormatFloat("00",today_day);
                                  if( FormatFloat("00",today_day)==str_zini_datez.SubString(1,2))
                                    {
                                    stmp.sprintf("_WZ_OK 歷史檔案維護 : 帳表日(%d)  >= 系統日(%d) => 不執行",report_day, today_day );
                                    writelog(stmp);
                                   }
                                  else
                                    { _DelTree(del_dir); }

                              }
                            else
                               {
                                  for (int i=report_day+1; i<=today_day ; i++)
                                      {
                                         String del_dir="C:\\FTLPOS\\History\\"+FormatFloat("00",i);

                                         if( FormatFloat("00",i)==str_zini_datez.SubString(1,2))
                                            { ;; }
                                         else
                                            { _DelTree(del_dir); }  //2003/03/27

                                      }

                               }
                          }
                       else  //帳表年月及系統年月不同
                          {
                            for (int i=report_day+1; i<=31 ; i++)  //1. 刪 (帳表日+1 -> 31)
                                {
                                  String del_dir="C:\\FTLPOS\\History\\"+FormatFloat("00",i);
                                  if( FormatFloat("00",i)==str_zini_datez.SubString(1,2))
                                    { ;; }
                                  else
                                    { _DelTree(del_dir); }

                                }

                            for (int i=1; i<=today_day ; i++)      //2. 刪 (1 -> 系統日)
                               {
                                //if ((report_day==today_day)&&(i==today_day)) break; //2003/02/19 帳表日等於系統日則不刪
                                //if (i==report_day) break;   //20030924修正，永不得刪帳表日當天資料
                                String del_dir="C:\\FTLPOS\\History\\"+FormatFloat("00",i);
                                if( FormatFloat("00",i)==str_zini_datez.SubString(1,2))
                                    { ;; }
                                  else
                                    { _DelTree(del_dir); }

                              }
                          }

                       writelog("□WZ_OK:歷史檔案維護功能結束 □"); //2003/02/07
                   }   // end of if (Trim(report_year_month)!="" && report_year_month<=today_year_month)
                 else
                  {
                    writelog("□WZ_OK:帳表年月資料空白或大於系統日,放棄執行歷史檔案維護功能 □");
                  }

            } // end of    try
         catch(...)
            {
              writelog("□WZ_OK: 歷史檔案維護功能 Catch error □");

            }

        }    // if (iMainTainHistory) //delFolder)


        //備份相關檔案
        str_folder_path = ExtractFilePath(str_zrp);

        if (!DirectoryExists(str_folder_path) && Trim(str_folder_path)!="")
            {
              writelog("WZ_OK:建立目錄 "+str_folder_path);
              ForceDirectories(str_folder_path);
            }

        //所有備份檔新路徑
        //str_zdt.SubString(1, str_zdt.Pos(".")) + "zsl";
        str_zsl_file=ChangeFileExt(str_zdt,".ZSL");

        str_sal_file = str_folder_path + ExtractFileName( _ByteArrayToAnsiString(SalFileName) );
        str_log_file.sprintf("%s%s", XZDATADIR,"XZDATA.LOG");   //SalFileName.SubString(1, SalFileName.Pos(".")) + "log";
        str_ecr_file = str_folder_path + ExtractFileName(XZDATA_INI); // "XZDATA.INI";

        //str_mwlog_file = SalFileName.SubString(1, SalFileName.Pos(".")) + "mw";

        str_mwlog_file = _ByteArrayToAnsiString(SalFileName).SubString(1, _ByteArrayToAnsiString(SalFileName).Pos(".")) + "mw";


        writelog("WZ_OK: 備份LOG檔 ------- " + str_log_file + " TO " + str_folder_path + ExtractFileName(str_log_file));
        DeleteFile(str_folder_path + ExtractFileName(str_log_file));
        if (RenameFile(str_log_file, str_folder_path + ExtractFileName(str_log_file)))
        {
            writelog("WZ_OK: 備份LOG檔 完成" + str_log_file + " TO " + str_folder_path + ExtractFileName(str_log_file));
        }
        else
        {
            writelog("WZ_OK: 備份LOG檔 失敗" + str_log_file + " TO " + str_folder_path + ExtractFileName(str_log_file));
        }


        /****
        DeleteFile(str_sal_file);
        if (RenameFile(_ByteArrayToAnsiString(SalFileName), str_sal_file))
        {
            writelog("WZ_OK:備份SAL檔 " + str_sal_file);
        }
        else
        {
            writelog("WZ_OK:備份SAL檔失敗 " + str_sal_file);
            return 0;
        }


       if( CopyFile( str_sal_file.c_str(), str_zsl_file.c_str(), false)  )
         {
           writelog("WZ_OK:備份ZSL檔 " + str_zsl_file);
         }
       else
         {
           writelog("WZ_OK:備份ZSL檔失敗 " + str_zsl_file);
         }


        ****/
        
        /**
        DeleteFile(str_folder_path + ExtractFileName(str_mwlog_file));
        if (RenameFile(str_mwlog_file, str_folder_path + ExtractFileName(str_mwlog_file)))
        {
            writelog("備份MW檔 " + str_mwlog_file+ " To " + str_folder_path + ExtractFileName(str_mwlog_file));
            DeleteFile(str_mwlog_file);
        }
        else
        {
            writelog("備份MW檔失敗 " + str_folder_path + ExtractFileName(str_mwlog_file));
        }

        //NN802.MMK Log file
        DeleteFile(str_folder_path + ExtractFileName(str_802log_file));
        if (RenameFile(str_802log_file, str_folder_path + ExtractFileName(str_802log_file)))
        {
            writelog("備份802.log檔 " + str_802log_file+ " To " + str_folder_path + ExtractFileName(str_802log_file));
            DeleteFile(str_mwlog_file);
        }
        else
        {
            writelog("備份802.log檔失敗 " + str_folder_path + ExtractFileName(str_802log_file));
        }
       **/

        /**
        if ( CopyFile(str_mwlog_file.c_str(), (str_folder_path + ExtractFileName(str_mwlog_file)).c_str(), false) )
        {
            writelog("備份MW檔 " + str_mwlog_file+ " To" + str_folder_path + ExtractFileName(str_mwlog_file));
            DeleteFile(str_mwlog_file);
        }
        else
        {
            writelog("備份MW檔失敗 " + str_folder_path + ExtractFileName(str_mwlog_file));
        }
        **/


        /***
        if ( CopyFile(str_dbtxt_file.c_str(), (str_folder_path + ExtractFileName(str_dbtxt_file)).c_str(), false) )
        {
            writelog("備份SAL_TXT檔 " + str_dbtxt_file+ " To " + str_folder_path + ExtractFileName(str_dbtxt_file));
        }
        else
        {
            writelog("備份SAL_TXT檔失敗 " + str_dbtxt_file+ " To " + str_folder_path + ExtractFileName(str_dbtxt_file));
        }

        if ( CopyFile(str_dbtxt2_file.c_str(), (str_folder_path + ExtractFileName(str_dbtxt2_file)).c_str(), false) )
        {
            writelog("備份SAL2_TXT檔 " + str_dbtxt2_file+ " To " + str_folder_path + ExtractFileName(str_dbtxt2_file));
        }
        else
        {
            writelog("備份SAL2_TXT檔失敗 " + str_dbtxt2_file+ " To " + str_folder_path + ExtractFileName(str_dbtxt2_file));
        }
       ***/

        String InputFile, OutputFile;
        str_log_file.sprintf("C:\\FTLPOS\\XZDATA\\SAL\\%s.sal",EcrNO );
        OutputFile=str_log_file;

        writelog("WZ_OK: 備份SAL檔 ------- " + str_log_file + " TO " + str_folder_path + ExtractFileName(str_log_file));
        DeleteFile(str_folder_path + ExtractFileName(str_log_file));
        if (RenameFile(str_log_file, str_folder_path + ExtractFileName(str_log_file)))
        {
            writelog("WZ_OK: 備份SAL檔 完成" + str_log_file + " TO " + str_folder_path + ExtractFileName(str_log_file));

            InputFile== str_folder_path + ExtractFileName(str_log_file);
            //if( OutNZCnt > 0)      //如無需要就 MARK
            //    _SaveZCntToFile( InputFile ,OutNZCnt , OutputFile ) ;

        }
        else
        {
            writelog("WZ_OK: 備份SAL檔 失敗" + str_log_file + " TO " + str_folder_path + ExtractFileName(str_log_file));
        }

        if (DeleteFile(str_tmp_ini))
           {
              writelog("刪除檔案 : " + str_tmp_ini);
              //自動日結規不異動,手動日結歸0 , 所以由 自動日結/手動日結 處理
              // tif_ecr->WriteString("X", "PXNO_SEQ" , "0");
           }
    }
    __finally
    {
        delete tif_ecr, tif_tmp;

        if (CopyFile(XZDATA_INI, str_ecr_file.c_str(), false))
                  writelog("備份XZDATA_INI檔 " + str_ecr_file);
        else
                  writelog("備份XZDATA_INI檔失敗 " + str_ecr_file);
       
        str_ecr_file = str_folder_path + "Application.INI";
        if (CopyFile("C:\\FTLPOS\\INI\\Application.INI", str_ecr_file.c_str(), false))
                    writelog("備份Application.INI檔 " + str_ecr_file);
        else
                    writelog("備份Application.INI檔失敗 " + str_ecr_file);



    }

    //writelog("刪除檔案 " + str_tmp_ini);
    //writelog("維護 InvoFilePath ");
    AnsiString sRtnSaleDataPath;

    sRtnSaleDataPath="C:\\FTLPOS\\MMK\\Telegram\\";
    MtnDirFile(sRtnSaleDataPath, 30);

    sRtnSaleDataPath="C:\\FTLPOS\\MMK\\RCV\\";
    MtnDirFile_Ex(sRtnSaleDataPath, 30, 250);

    sRtnSaleDataPath="C:\\FTLPOS\\MMK\\SEND\\";
    MtnDirFile_Ex(sRtnSaleDataPath, 30, 250);

    MtnDirFile(sInvoFilePath, 90);

    sRtnSaleDataPath="C:\\FTLPOS\\RCP\\RcpUsed\\";
    MtnDirFile_Ex(sRtnSaleDataPath, 90, 100);

    MtnDirFile(sInvoTransFilePath,7);
    MtnDirFile(XZDATASAL,60);

    DeleteFile(str_tmp_ini);
    writelog("WZ_OK 完成: 刪除檔案 " + str_tmp_ini);

    return 1;


}



//---------------------------------------------------------------------------
int __fastcall TfrmAction::_WSPC(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version, int iQryType)
{
   int iRtn;

   AllSpcData = new SPCDATA();
   iRtn=AllSpcData->WriteSpcInqData( _ByteArrayToAnsiString(SalFileName), "V01.9999", iQryType);
   delete AllSpcData;

   // iRtn > 0 : SUCCEED
   return iRtn;

}



int __fastcall TfrmAction::_WVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
    int int_result = 0;
    vxzdata = new VXZDATA();
    int_result = vxzdata->WriteData( _ByteArrayToAnsiString(StoreNO),
                                   _ByteArrayToAnsiString(EcrNO),
                                   _ByteArrayToAnsiString(SalesNo),
                                   _ByteArrayToAnsiString(SalFileName),
                                   _ByteArrayToAnsiString(Version) );
    delete vxzdata;

    return int_result;
}



int __fastcall TfrmAction::_SaveZCntToFile( String InPutSalFile ,int Z_Cnt , String OutPutSalFile )
{

    TStringList *tmpSal = new TStringList;
    TStringList *outSal = new TStringList;

    AnsiString sRec, saledata, sRecNo, sTmpDttm, TmpZcnt, stmp;
    saledata="";
    int NoUsedData=0;
    int i;

    stmp.sprintf("_SaveZCntToFile: InPutSalFile=%s,  Z_Cnt=%d, OutPutSalFile=%s", InPutSalFile.c_str(), Z_Cnt, OutPutSalFile.c_str() );
    writelog(stmp);

    outSal->Clear();
    tmpSal->LoadFromFile(InPutSalFile);
 
    if( tmpSal->Count > 0 )
       {
        for (i=0;  i<tmpSal->Count; i++)
         {
          sRec=tmpSal->Strings[i];
          sRecNo=_StringSegment_EX(sRec, "|", 1);
          sTmpDttm=_StringSegment_EX(sRec, "|", 5);
          TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號

          //if(sRecNo=="1096")
          //  {
          //    NoUsedData=1;
          //  }

          if(sRecNo=="1001")
            {
               saledata="";
               stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
               saledata+=stmp;
            }
          else if(sRecNo=="1099")
            {
               //stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
               if( NoUsedData != 1)
                 {
                   stmp.sprintf("%s",sRec);
                   saledata+=sRec;
                   try
                     {
                       if( TmpZcnt.ToInt() == Z_Cnt  )
                           outSal->Add(saledata);
                     }
                    catch(...)
                    {
                      saledata="";
                    }
                 }
               saledata="";
               NoUsedData=0;
            }
          else
            { stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
              saledata+=stmp;   }

         } //end of for
       }

    //str_now = FormatDateTime("ddhhnnss",Now());
    //CopyFile(LoadSalName.c_str(), Trim(LoadSalName+str_now).c_str() , false);
    outSal->SaveToFile(OutPutSalFile);

    stmp.sprintf("_SaveZCntToFile: SaveToFile(%s) OK", OutPutSalFile.c_str() );
    writelog(stmp);

    delete tmpSal, outSal;

    return 1;
}









