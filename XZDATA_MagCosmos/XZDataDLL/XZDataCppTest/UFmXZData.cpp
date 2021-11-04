//---------------------------------------------------------------------------

#include <vcl.h>
#include <map.h>
#pragma hdrstop

#include "UFmXZData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFmMain *FmMain;
//---------------------------------------------------------------------------
__fastcall TFmMain::TFmMain(TComponent* Owner)
        : TForm(Owner)
{
}


void __fastcall TFmMain::mLog(AnsiString sLog)
{

    if( Memo1->Lines->Count > 100)
       Memo1->Lines->Clear();

    Memo1->Lines->Add(sLog);


}

//---------------------------------------------------------------------------
void __fastcall TFmMain::Button1Click(TObject *Sender)
{
    String  stmp;
    checkin = new CHECKIN();
    stmp=EdiSalFile->Text;
    //擔當同交班
    int i=checkin->WriteData("009986", "01", "99999" , stmp, "V99.9999");

    stmp.sprintf("checkin->WriteData: 擔當同交班 Rtn=%d",i );
    mLog(stmp);
    delete checkin;
}
//---------------------------------------------------------------------------


void __fastcall TFmMain::Button2Click(TObject *Sender)
{
    String  stmp, sExt;
    xreport = new XREPORT();
    stmp=EdiSalFile->Text;

    //sExt=ExtractFileExt(stmp);
    sExt=ExtractFileName(stmp);

    //X帳表 /讀帳
    int i=xreport->WriteData("009986", sExt.SubString(1,2)  , "99999" , stmp, "V99.9999");
    stmp.sprintf("xreport->WriteData: X帳表  Rtn=%d",i );
    mLog(stmp);

    delete xreport;
}
//---------------------------------------------------------------------------


void __fastcall TFmMain::Button3Click(TObject *Sender)
{
    String  stmp,Last1099, sStore,sEcr, sChr,strTranCnt, First1099, strEndTranCnt ;
    int i,TranCnt;
    stmp=EdiSalFile->Text;
    strTranCnt=Trim(Edit1->Text);
    strEndTranCnt=Trim(Edit2->Text);
    //strTranCnt="97578";
    //TranCnt=StrToInt(strTranCnt);
    loadsalfile(stmp, false,First1099, Last1099);
 
    sStore=Last1099.SubString(11,6);
    sEcr=Last1099.SubString(18,2);
    sChr=Last1099.SubString(95,5);
    //Clear_Tran_cnt_INI(TranCnt+1);
 
    TIniFile *MyIni;
    MyIni = new TIniFile("C:\\FTLPOS\\INI\\XZDATA.INI");
 
    MyIni->WriteInteger("ECR","EcrNO",sEcr.ToInt() );
    if(strEndTranCnt.SubString(4,5).ToInt() > 0)
      MyIni->WriteString("ECR","TRAN_CNT",strEndTranCnt.SubString(4,5) );
    MyIni->WriteString("X","ptran_cnt",strTranCnt );
    delete MyIni;

    //[XDT]:交班 請輸入交易序號 ddz99999 ~ ddz99999, 當輸日 00000000 ~ 00000000 表示全擋制作
 
    xdata = new XDATA();
 
    i = xdata->WriteData(sStore, sEcr, "99999" , stmp, "V99.9999");
    stmp.sprintf("xreport->WriteData: 交班帳表  Rtn=%d",i );
    mLog(stmp);
    delete xdata;
 
}

//---------------------------------------------------------------------------



AnsiString __fastcall TFmMain::GetUniqueSal(AnsiString sSalFileName)
{
  string   sRec ;
  String   stmp, OutSalFile;
  int i,TranCnt, NoUsedData;

  OutSalFile="";
  TStringList *tmpSal = new TStringList;
  TStringList *OutSal = new TStringList;
  tmpSal->LoadFromFile(sSalFileName);

  OutSal->Clear();

  //std::map<string, int, DisableCompare<string> > salmap;
  //std::pair<std::map<string,int, DisableCompare<string>  >::iterator,bool> ret;

  std::map<string, int > salmap;
  std::pair<std::map<string,int >::iterator,bool> ret;

  if( tmpSal->Count > 0 )
    {
       for (i=0;  i<tmpSal->Count; i++)
         {
          sRec=tmpSal->Strings[i].c_str();
          ret=salmap.insert( std::pair<string,int>(sRec,i) );

          if (ret.second==false)   //second
             {
              stmp.sprintf("GetUniqueSal:Data already existed->%s, Count=%d",sRec.c_str(), i );
              mLog(stmp);
             }
          else
             {
              OutSal->Add( AnsiString(sRec.c_str()) );
              }

         }

    }

    tmpSal->Clear();
    // second insert function version (with hint position):

    /****
    std::map<string, int, DisableCompare<string> >::iterator it = salmap.begin();
    // 由前往後找
    for (it=salmap.begin(); it!=salmap.end(); ++it)
        {
          sRec=it->first;; // << " => " << it->second << '\n';
          tmpSal->Add( AnsiString(sRec.c_str()) );
         }


    // 由後往前找
    std::map<string, int , DisableCompare<string> >::reverse_iterator  it = salmap.rbegin();

    for (it=salmap.rbegin(); it!=salmap.rend(); ++it)
        {
          sRec=it->first;; // << " => " << it->second << '\n';
          tmpSal->Add( AnsiString(sRec.c_str()) );
         }
    ****/

     //ExtractFileName(FileName)
    // sBackupPath=ExtractFilePath(BackupFileName);
    OutSalFile=ExtractFilePath(sSalFileName)+"__"+ExtractFileName(sSalFileName);
    OutSal->SaveToFile(OutSalFile);    //檔名不包含路徑;


  delete tmpSal;
  delete OutSal;
  return( OutSalFile);

}


void __fastcall TFmMain::Button5Click(TObject *Sender)
{

  /********

   String  stmp, Last1099, sStore,sEcr, sChr, strTranCnt, YYZ,First1099,str_now,
            pz_date,nz_cnt ,sRec, sRecNo,sTmpDttm,TmpZcnt, LoadSalName, StartZcnt,
            EndZcnt, saledata, AutoDt    ;

    int i,TranCnt;
    stmp=EdiSalFile->Text;
    strTranCnt=Edit1->Text;
    TranCnt=StrToInt(strTranCnt);




    loadsalfile(stmp, false,First1099, Last1099);

    pz_date=First1099.SubString(21,14);
    sStore=Last1099.SubString(11,6);
    sEcr=Last1099.SubString(18,2);
    nz_cnt=Last1099.SubString(36,5);

    sChr=Last1099.SubString(95,5);   //收銀員
    YYZ=Last1099.SubString(44,3);
    strTranCnt=Last1099.SubString(47,5);
    TranCnt=StrToInt(strTranCnt);



    TIniFile *MyIni;
    MyIni = new TIniFile("C:\\FTLPOS\\INI\\XZDATA.INI");

    MyIni->WriteInteger("ECR","TRAN_CNT",TranCnt+1 );

    MyIni->WriteString("Z","pz_date",pz_date );
    MyIni->WriteString("Z","nz_cnt",nz_cnt );
    MyIni->WriteString("Z","nzno_seq",YYZ.SubString(3,1) );
    MyIni->WriteString("Z","datez",YYZ );


    //Clear_Tran_cnt_INI(TranCnt+1);
    zdata = new ZDATA();

    i = zdata->WriteData(sStore, sEcr, "99999" , stmp, "V99.9999", EdAutoZDttm->Text   );
    stmp.sprintf("xreport->WriteData: 日結帳表  Rtn=%d",i );
    mLog(stmp);


    delete zdata;
    delete MyIni;


    MyIni = new TIniFile("C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI");
    stmp="日結帳表輸出:"+MyIni->ReadString("Z","ZFILENAME","" );
    mLog(stmp);
    delete MyIni;

    ************/

  

    String  stmp, Last1099, sStore,sEcr, sChr, strTranCnt, YYZ,First1099,str_now,
            pz_date,nz_cnt ,sRec, sRecNo,sTmpDttm,TmpZcnt, LoadSalName, StartZcnt,
            EndZcnt, saledata, AutoDt    ;
    int i,TranCnt, NoUsedData;
    LoadSalName=EdiSalFile->Text;
    strTranCnt=Edit1->Text;
    TranCnt=StrToInt(strTranCnt);

    
    if (RadioButton1 ->Checked==true)         //2015/12/28 新增功能:檢查重複資料
    {
    LoadSalName=GetUniqueSal(LoadSalName);
    }
    //return;


    StartZcnt=Edi_ST->Text;
    EndZcnt=Edi_ED->Text;
    TStringList *tmpSal = new TStringList;
    TStringList *outSal = new TStringList;


     outSal->Clear();


     tmpSal->LoadFromFile(LoadSalName);

     saledata="";
     NoUsedData=0;

     if( tmpSal->Count > 0 )
       {
        for (i=0;  i<tmpSal->Count; i++)
         {
          sRec=tmpSal->Strings[i];
          sRecNo=_StringSegment_EX(sRec, "|", 1);
          sTmpDttm=_StringSegment_EX(sRec, "|", 5);
          TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號

          if(sRecNo=="1096")
            {
              NoUsedData=1;
            }


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

                   if( _StrToInt(StartZcnt)>0 || _StrToInt(EndZcnt) > 0 )
                     {
                       if( TmpZcnt >= StartZcnt &&  TmpZcnt <= EndZcnt  )
                            outSal->Add(saledata); //.SubString(1, sRec.Length()-1) );
                     }
                   else
                     { outSal->Add(saledata);  }


                 }
               saledata="";
               NoUsedData=0;
            }
          else
            { stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
               saledata+=stmp;   }

         } //end of for
       }
    
    str_now = FormatDateTime("ddhhnnss",Now());
    CopyFile(LoadSalName.c_str(), Trim(LoadSalName+str_now).c_str() , false);
    outSal->SaveToFile(LoadSalName);

    //stmp.sprintf("tmpSal->SaveToFile: %s,   Count=%d",LoadSalName, outSal->Count  );
    //mLog(stmp);

    delete tmpSal, outSal;

   
    //Last1099=loadsalfile(stmp, false);
    loadsalfile(LoadSalName, false,First1099, Last1099);

    pz_date=First1099.SubString(21,14);
    sStore=Last1099.SubString(11,6);
    sEcr=Last1099.SubString(18,2);
    nz_cnt=Last1099.SubString(36,5);

    sChr=Last1099.SubString(95,5);   //收銀員
    YYZ=Last1099.SubString(44,3);
    strTranCnt=Last1099.SubString(47,5);
    TranCnt=StrToInt(strTranCnt);

    AutoDt=_StringSegment_EX(Last1099, "|", 5).SubString(1,8);

    if( Trim(EdAutoZDttm->Text)=="")
        AutoDt=_StringSegment_EX(Last1099, "|", 5).SubString(1,8);
    else
        AutoDt=EdAutoZDttm->Text;


    TIniFile *MyIni;
    MyIni = new TIniFile("C:\\FTLPOS\\INI\\XZDATA.INI");

    MyIni->WriteInteger("ECR","TRAN_CNT",TranCnt+1 );

    MyIni->WriteString("Z","pz_date",pz_date );
    MyIni->WriteString("Z","nz_cnt",nz_cnt );
    MyIni->WriteString("Z","nzno_seq",YYZ.SubString(3,1) );
    MyIni->WriteString("Z","datez",YYZ );


    //Clear_Tran_cnt_INI(TranCnt+1);



    zdata = new ZDATA();

    i = zdata->WriteData(sStore, sEcr, "99999" , LoadSalName, "V99.9999", AutoDt  );
    stmp.sprintf("xreport->WriteData: 日結帳表  Rtn=%d",i );
    mLog(stmp);

    autozdata = new AUTOZDATA();


    delete autozdata;
    delete MyIni;


    MyIni = new TIniFile("C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI");
    stmp="日結帳表輸出:"+MyIni->ReadString("Z","ZFILENAME","" );
    mLog(stmp);
    delete MyIni;


}
//---------------------------------------------------------------------------

void __fastcall TFmMain::Button7Click(TObject *Sender)
{
    String  stmp;
    stmp=EdiSalFile->Text;
    AllSpcData = new SPCDATA();
    int i=AllSpcData->WriteSpcInqData(stmp,"V99.9999", 0); //("009986", "01", "99999" , stmp, "V99.9999");
    stmp.sprintf("xreport->WriteData: 重點商品帳表  Rtn=%d",i );
    mLog(stmp);
    delete AllSpcData;
}
//---------------------------------------------------------------------------

void __fastcall TFmMain::FormShow(TObject *Sender)
{
   mLog( "C:\\FTLPOS\\XZDATA\\XZDATA.Log");
}
//---------------------------------------------------------------------------


int __fastcall TFmMain::loadsalfile(AnsiString FileName, bool for_x, AnsiString &First1099, AnsiString &Last1099)
{

    FILE *f_sal_file;                           //SAL檔指標

    AnsiString str_sal_data,stmp; // Last1099;                    //SAL每行字串
    int i_type, i_len;                          //資料TYEP 10,50..，長度
    char c_data_buf[10240];                       //讀出每行資料暫存字串


   First1099="";
   mLog("★★XZDATA.DLL Ver:V12.0920 For Magic 1000  ★★ InPut File:"+FileName  );

   AnsiString str_trans_now;                   //每行資料交易序號

   Last1099="";
   if ((f_sal_file = fopen(FileName.c_str(),"rb")) == NULL)
    {
        mLog("TFmMain::loadsalfile 開啟交易檔案發生錯誤 " + FileName);
        return(-1);
    }


    if (f_sal_file != NULL)
    {
        while (!feof(f_sal_file))               //讀SAL檔資料
        {
            memset(c_data_buf, 0x00, MAXDATALEN);
            fgets(c_data_buf, MAXDATALEN, f_sal_file);
            fscanf(f_sal_file,"\n");
            str_sal_data = c_data_buf;

            i_type = _StrToInt(str_sal_data.SubString(1,4));
            if( i_type==1099 ) //CheckSaleRecData(str_sal_data) )
            {

                Last1099.sprintf("%s", c_data_buf);                //SAL檔最後一筆資料

                if( Trim(First1099)=="")
                  First1099=Last1099;

            }
            else        //交易資料有誤
            {
                //if (!feof(f_sal_file))
               //    mLog("SAL檔交易資料異常 " + str_sal_data);
                continue;
            }

        }//while
    }//if (f_sal_file != NULL)

    fclose(f_sal_file);

    stmp.sprintf("取得本次First1099:%s",First1099  );
    mLog(stmp);

    stmp.sprintf("取得本次Last1099:%s",Last1099  );
    mLog(stmp);

    return(0);
}



int __fastcall TFmMain::Clear_Tran_cnt_INI(int TranCnt)
{

  TIniFile *MyIni;
  MyIni = new TIniFile("C:\\FTLPOS\\INI\\XZDATA.INI");

  MyIni->WriteInteger("ECR","TRAN_CNT",TranCnt );

  delete MyIni;
  return(0);
}



void __fastcall TFmMain::Button6Click(TObject *Sender)
{
  TIniFile *MyIni;
  AnsiString stmp;

  //_WZ_OK("00002", "02", "99999", "03.sal", "V99.9999");

  MyIni = new TIniFile("C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI");
    stmp="日結帳表輸出:"+MyIni->ReadString("Z","ZFILENAME","" );
    mLog(stmp);
    delete MyIni;



}
//---------------------------------------------------------------------------

void __fastcall TFmMain::Button8Click(TObject *Sender)
{

   String  stmp, Last1099, sStore,sEcr, sChr, strTranCnt, YYZ,First1099,
            pz_date,nz_cnt            ;
    int i,TranCnt;
    stmp=EdiSalFile->Text;
    strTranCnt=Edit1->Text;
    TranCnt=StrToInt(strTranCnt);

    //Last1099=loadsalfile(stmp, false);
    loadsalfile(stmp, false,First1099, Last1099);

    pz_date=First1099.SubString(21,14);
    sStore=Last1099.SubString(11,6);
    sEcr=Last1099.SubString(18,2);
    nz_cnt=Last1099.SubString(36,5);

    sChr=Last1099.SubString(95,5);   //收銀員
    YYZ=Last1099.SubString(44,3);
    strTranCnt=Last1099.SubString(47,5);
    TranCnt=StrToInt(strTranCnt);



    TIniFile *MyIni;
    MyIni = new TIniFile("C:\\FTLPOS\\INI\\XZDATA.INI");

    MyIni->WriteInteger("ECR","TRAN_CNT",TranCnt+1 );

    MyIni->WriteString("Z","pz_date",pz_date );
    MyIni->WriteString("Z","nz_cnt",nz_cnt );
    MyIni->WriteString("Z","nzno_seq",YYZ.SubString(3,1) );
    MyIni->WriteString("Z","datez",YYZ );


    //Clear_Tran_cnt_INI(TranCnt+1);
    vxzdata = new VXZDATA();

    i = vxzdata->WriteData(sStore, sEcr, "99999" , stmp, "V99.9999");
    stmp.sprintf("xreport->WriteData: 盤點讀帳  Rtn=%d",i );
    mLog(stmp);


    delete vxzdata;
    delete MyIni;


    MyIni = new TIniFile("C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI");
    stmp="盤點讀帳帳表輸出:"+MyIni->ReadString("VXZ","ZFILENAME","" );
    mLog(stmp);
    delete MyIni;


}
//---------------------------------------------------------------------------

// 自動日結
void __fastcall TFmMain::Button9Click(TObject *Sender)
{
   String  stmp, Last1099, sStore,sEcr, sChr, strTranCnt, YYZ,First1099,str_now,
            pz_date,nz_cnt ,sRec, sRecNo,sTmpDttm,TmpZcnt, LoadSalName, StartZcnt,
            EndZcnt, saledata, AutoDt    ;
    int i,TranCnt;
    LoadSalName=EdiSalFile->Text;
    strTranCnt=Edit1->Text;
    TranCnt=StrToInt(strTranCnt);

    StartZcnt=Edi_ST->Text;
    EndZcnt=Edi_ED->Text;
    TStringList *tmpSal = new TStringList;
    TStringList *outSal = new TStringList;

     outSal->Clear();
     tmpSal->LoadFromFile(LoadSalName);

     saledata="";
     if( tmpSal->Count > 0 )
       {
        for (i=0;  i<tmpSal->Count; i++)
         {
          sRec=tmpSal->Strings[i];
          sRecNo=_StringSegment_EX(sRec, "|", 1);
          sTmpDttm=_StringSegment_EX(sRec, "|", 5);
          TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //日結序號

          if(sRecNo=="1001")
            {
               saledata="";
               stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
               saledata+=stmp;
            }
          else if(sRecNo=="1099")
            {
               //stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
               stmp.sprintf("%s",sRec);
               saledata+=sRec;

               if( TmpZcnt >= StartZcnt &&  TmpZcnt <= EndZcnt  )
                   outSal->Add(saledata); //.SubString(1, sRec.Length()-1) );
               saledata="";

            }
          else
            { stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
               saledata+=stmp;   }

         } //end of for
       }
    str_now = FormatDateTime("ddhhnnss",Now());
    CopyFile(LoadSalName.c_str(), Trim(LoadSalName+str_now).c_str() , false);
    outSal->SaveToFile(LoadSalName);

    stmp.sprintf("tmpSal->SaveToFile: %s,   Count=%d",LoadSalName, outSal->Count  );
    mLog(stmp);

    delete tmpSal, outSal;

   
    //Last1099=loadsalfile(stmp, false);
    loadsalfile(LoadSalName, false,First1099, Last1099);

    pz_date=First1099.SubString(21,14);
    sStore=Last1099.SubString(11,6);
    sEcr=Last1099.SubString(18,2);
    nz_cnt=Last1099.SubString(36,5);

    sChr=Last1099.SubString(95,5);   //收銀員
    YYZ=Last1099.SubString(44,3);
    strTranCnt=Last1099.SubString(47,5);
    TranCnt=StrToInt(strTranCnt);

    AutoDt=_StringSegment_EX(Last1099, "|", 5).SubString(1,8);

    if( Trim(EdAutoZDttm->Text)=="")
        AutoDt=_StringSegment_EX(Last1099, "|", 5).SubString(1,8);
    else
        AutoDt=EdAutoZDttm->Text;


    TIniFile *MyIni;
    MyIni = new TIniFile("C:\\FTLPOS\\INI\\XZDATA.INI");

    MyIni->WriteInteger("ECR","TRAN_CNT",TranCnt+1 );

    MyIni->WriteString("Z","pz_date",pz_date );
    MyIni->WriteString("Z","nz_cnt",nz_cnt );
    MyIni->WriteString("Z","nzno_seq",YYZ.SubString(3,1) );
    MyIni->WriteString("Z","datez",YYZ );


    //Clear_Tran_cnt_INI(TranCnt+1);
    autozdata = new AUTOZDATA();

    i = autozdata->WriteData(sStore, sEcr, "99999" , LoadSalName, "V99.9999",AutoDt);
    stmp.sprintf("autozdata-->WriteData: Auto日結帳表  Rtn=%d",i );
    mLog(stmp);


    delete autozdata;
    delete MyIni;


    MyIni = new TIniFile("C:\\FTLPOS\\XZDATA\\ZDT_TMP.INI");
    stmp="Auto日結帳表輸出:"+MyIni->ReadString("Z","ZFILENAME","" );
    mLog(stmp);
    delete MyIni;
}
//---------------------------------------------------------------------------


void __fastcall TFmMain::Button10Click(TObject *Sender)
{

  AnsiString s, stmp;

  //sprintf( sFile,"%s",EdiSalFile->Text);
  //sprintf( sPath,"");

  stmp="";
  //s=File2String(EdiSalFile->Text.c_str(),stmp.c_str() );
  s=File2String(EdiSalFile->Text,stmp );

  stmp.sprintf("Button10Click:(%s), Len=%d",s, s.Length() );
  mLog(stmp);

  return;

  int iFileHandle;
  int iFileLength;
  int iBytesRead;

  char *pszBuffer;

    try
    {
      iFileHandle = FileOpen(EdiSalFile->Text, fmOpenRead);
      if( iFileHandle == -1)
          return;


      iFileLength = FileSeek(iFileHandle,0,2);
      FileSeek(iFileHandle,0,0);
      pszBuffer = new char[iFileLength+1];
      iBytesRead = FileRead(iFileHandle, pszBuffer, iFileLength);
      FileClose(iFileHandle);

      //memcpy(stmp.c_str(),pszBuffer, iFileLength );
     stmp.sprintf("FileRead: (%s)  Len=%d",pszBuffer, iFileLength );
      mLog(stmp);

      delete [] pszBuffer;
    }
    catch(...)
    {
      Application->MessageBox("Can't perform one of the following file operations: Open, Seek, Read, Close.", "File Error", IDOK);
    }


}
//---------------------------------------------------------------------------



AnsiString __fastcall TFmMain::File2String(AnsiString fn, AnsiString path)   //失敗則回傳:""
{
   AnsiString s,XmlData;

   int iFileHandle;
   int iFileLength;
   int iBytesRead;
   AnsiString stmp;
   char *pszBuffer;
   char rvcbuf[102400];

   //AnsiString t=(String)path+(String)fn;
   AnsiString t=path+fn;
   s="";
   XmlData="";
   memset( rvcbuf,0, sizeof(rvcbuf));

   try
    {
      iFileHandle = FileOpen(t, fmOpenRead);
      if( iFileHandle == -1)
          return (XmlData);


      iFileLength = FileSeek(iFileHandle,0,2);
      FileSeek(iFileHandle,0,0);
      pszBuffer = new char[iFileLength+1];
      memset(pszBuffer,0, sizeof(pszBuffer));
      iBytesRead = FileRead(iFileHandle, pszBuffer, iFileLength);
      FileClose(iFileHandle);

      memcpy( rvcbuf,pszBuffer, iFileLength);
      stmp.sprintf("File2String:(%s), Len=%d",rvcbuf, iFileLength );
      //memcpy(stmp.c_str(),pszBuffer, iFileLength );
      mLog(stmp);

      //delete [] pszBuffer;
    }
    catch(...)
    {
      //Application->MessageBox("Can't perform one of the following file operations: Open, Seek, Read, Close.", "File Error", IDOK);
      ;;
    }

    XmlData=AnsiString(rvcbuf) ;
    delete [] pszBuffer;
    return (XmlData);
}


/***
AnsiString __fastcall TFmMain::File2String(char *fn,char *path)   //失敗則回傳:""
{
   AnsiString s,XmlData;
   char c;
   FILE   *xmlfile;
   int    i;
    
   AnsiString t=(String)path+(String)fn;
   s="";
   XmlData="";
   
   if ((xmlfile=fopen(t.c_str(), "r"))==NULL)
      {
       //s="File2String(): File Not found "+t;    write_log_file(s.c_str());
       return (XmlData);
      }

   i=0;
   while (!feof(xmlfile))
         {

           fscanf(xmlfile, "%c", &c);
           //s=AnsiString(c);
           XmlData+=AnsiString(c); //s;
           i++;
           if( i>2048000)
              break;
         }
    fclose(xmlfile);

    return XmlData;
}
****/



void __fastcall TFmMain::Button4Click(TObject *Sender)
{
   _WX_OK("00002", "02", "99999", "03.sal", "V99.9999");
}
//---------------------------------------------------------------------------



int __fastcall TFmMain::_WX_OK(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
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





