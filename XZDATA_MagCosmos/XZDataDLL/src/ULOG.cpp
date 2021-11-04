//---------------------------------------------------------------------------

#include <vcl.h>
#include "ULOG.h"
#include <sys\stat.h>

#pragma hdrstop
#pragma package(smart_init)
//---------------------------------------------------------------------------

void __fastcall writelog(AnsiString S)       //�O��LOG
{
    AnsiString str_now, stmp;

    str_now = FormatDateTime("yyyy-mm-dd hh:nn:ss",Now());

    FILE *f_log;

    AnsiString str_path = XZDATADIR;
    AnsiString str_ecr;

    //int int_ecr = 0;

    //TIniFile *tifAbacus;

    try
    {
        //tifAbacus = new TIniFile("C:\\ABACUS\\ABACUS.INI");
        //int_ecr = tifAbacus->ReadInteger("ECR","gchr_ecr_no",0);
        str_ecr.sprintf("XZDATA"); //, atoi(gchar_ecr_no));
    }
    __finally
    {
        //delete tifAbacus;
        ;;
    }

    AnsiString str_file = (str_path + str_ecr + ".log");

    if ((f_log = fopen(str_file.c_str(), "a")) != NULL)
    {
        fprintf(f_log, "%s, %s\n", str_now.c_str(), S.c_str());
        fclose(f_log);
    }

}


int __stdcall file_cat( char *sour, char *targ )
{
   int in, out, buf_len;
   char *buf;

try
   {
   buf = (char *) malloc(1026);
   if(( in = open(sour, O_RDONLY | O_BINARY, S_IREAD)) == -1 )
     {
     //  ShowMessage("pos_cat: source file can't open ");
       return(-1);
     }
   if(( out = open( targ, O_CREAT | O_BINARY | O_WRONLY | O_APPEND, S_IWRITE))
       == -1)
     {
     //  ShowMessage("pos_cat :targ file cann't open" );
       return( -1);
      }

    while( !eof(in))
         {
           buf_len = read(in,buf,1024);
           write( out, buf, buf_len);
         }
   }
__finally
  {
    close(in);
    close(out);
    free(buf);

  }
    return(1);
 }



//---------------------------------------------------------------------------
//�̷Ӥ��j�Ÿ�,���X���j�Ÿ��e���r�ꤤ
// "AAAAAA,BBBBBB,CCCCCC,DDDDDD,EEEEE," ; Seg(1)=AAAAAA, ~ Seg(5)=EEEEE
//---------------------------------------------------------------------------
AnsiString __fastcall _StringSegment_EX(AnsiString &Str , AnsiString Comma , int Seg, int idelStr)
{
  if ((Str=="") || (Seg<1)) return "";

  String C=Comma; if (C=="") C=",";

  String s=Str;
  String sTmp;
  String r;
  int iPosComma, iSegCnt,i;
  bool brtn;

  iSegCnt=i=0;
  r="";

  while(s.Pos(C)>0)
        {
          i++;

          if(i>Str.Length() )
             break;

          brtn=false;
          brtn=s.IsDelimiter(C,i);
          if( brtn )
            {
             iSegCnt++;
             if( iSegCnt==Seg)
               {
                sTmp = s.SubString(1,i-1);
                r=sTmp;
                s = s.SubString(i + 1,s.Length()); // delete item from string
                i=0;
                break;
               }
             else
               {
                s = s.SubString(i + 1,s.Length()); // delete item from string
                i=0;
               }
            }
        }

  if( idelStr )
      Str=s;

  //if( r=="")
  //    r=s;

  return r;
}

/****
int __fastcall _GetZCntLst(TStringList *ZCntLst, TStringList *S1050Lst )
{

  AnsiString sRec, TmpZcnt, sTmpDttm, OrgZcnt, sRecNo, sTmp;

  try
  {
       //sTmlLst->Assign(Auto_Sal);
    if( S1050Lst->Count > 0)
      {
        ZCntLst->Clear();
        OrgZcnt="";
        for (int i=0;  i<S1050Lst->Count; i++)
            {
             sRec=S1050Lst->Strings[i];
             sRecNo=_StringSegment_EX(sRec, "|", 1);
             if( sRecNo != "1099" )
                continue;
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt !=  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    if( !_GetZCntLstCheck(ZCntLst, TmpZcnt) )
                      {
                        ZCntLst->Add(sRec);
                        OrgZcnt=TmpZcnt;
                        sTmp.sprintf("_GetZCntLst: �[�J�鵲�Ǹ� =%s",TmpZcnt);
                        writelog(sTmp);
                      }
                    else
                      {
                        // ZCntLst->Add(sRec);
                        OrgZcnt=TmpZcnt;
                        sTmp.sprintf("_GetZCntLst: �鵲�Ǹ����� =%s",TmpZcnt);
                        writelog(sTmp);
                      }
                 }
             }
       }
   }
 catch(...)
   {
     writelog("_GetZCntLst Catch error...");
   }

   return( ZCntLst->Count);

}
*****/

int __fastcall _GetZCntLst(TStringList *ZCntLst, TStringList *S1050Lst )
{

  AnsiString sRec, TmpZcnt, sTmpDttm, OrgZcnt, sRecNo, sTmp;

  try
  {
       //sTmlLst->Assign(Auto_Sal);
    if( S1050Lst->Count > 0)
      {
        ZCntLst->Clear();
        OrgZcnt="";
        for (int i=S1050Lst->Count-1; i>=0; i--)
            {
             sRec=S1050Lst->Strings[i];
             sRecNo=_StringSegment_EX(sRec, "|", 1);

             if( sRecNo != "1099" )     //if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
                  continue;

             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt !=  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    if( !_GetZCntLstCheck(ZCntLst, TmpZcnt) )
                      {
                        ZCntLst->Add(sRec);
                        OrgZcnt=TmpZcnt;
                        sTmp.sprintf("_GetZCntLst: �[�J�鵲�Ǹ� =%s,(%s)",TmpZcnt, sRec.c_str() );
                        writelog(sTmp);
                      }
                    else
                      {
                        // ZCntLst->Add(sRec);
                        OrgZcnt=TmpZcnt;
                        sTmp.sprintf("_GetZCntLst: �鵲�Ǹ����� =%s,(%s)",TmpZcnt, sRec.c_str());
                        writelog(sTmp);
                      }
                 }
             }
       }
   }
 catch(...)
   {
     writelog("_GetZCntLst Catch error...");
   }


 sTmp.sprintf("_GetZCntLst: �[�J�鵲�Ǹ����� =%d",ZCntLst->Count);
 writelog(sTmp);

 return( ZCntLst->Count);

}



int __fastcall _GetZCntLstCheck(TStringList *ZCntLst, AnsiString OrgZcnt)
{
      AnsiString  sRec, TmpZcnt,  sTmpDttm, sTmp;
      int ifind;
      ifind=0;
 try
   {
      for (int i=0;  i<ZCntLst->Count; i++)
            {
             sRec=ZCntLst->Strings[i];
             //sRecNo=_StringSegment_EX(sRec, "|", 1);
             //if( sRecNo != "1099" )
             //   continue;
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt ==  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    ifind=1;
                    sTmp.sprintf("GetZCntLstCheck: �鵲�Ǹ����� TmpZcnt=%s, OrgZcnt=%s ",TmpZcnt,OrgZcnt );
                    writelog(sTmp);
                    break;
                 }
             }
     }
 catch(...)
    {
       ifind=0;
       sTmp.sprintf("GetZCntLstCheck: Catch Error .. OrgZcnt=%s ",OrgZcnt );
       writelog(sTmp);
    }

  return( ifind );

}


// �ˬd 1050 �� SAL 1050 ���ƬO�_�@�P
int __fastcall _GetZDTZSALCntCheck(TStringList *ZCntLst, AnsiString SalFileName, AnsiString OrgZcnt,
                                   int &RtnZDTCnts, int &RtnSALCnts )
{
      AnsiString  sRec, TmpZcnt,  sTmpDttm, sTmp, sRecNo;
      int iZdt1050, iSal1050;
      iZdt1050=iSal1050=0;
      RtnZDTCnts=iZdt1050;
      RtnSALCnts=iSal1050;

 try
   {
      for (int i=0;  i<ZCntLst->Count; i++)
            {
             sRec=ZCntLst->Strings[i];
             sRecNo=_StringSegment_EX(sRec, "|", 1);
             if( sRecNo != "1050" )
                continue;
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt ==  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    iZdt1050++;
                    sTmp.sprintf("_GetZDTZSALCntCheck:ZDT(%6d)(%s)",iZdt1050, sRec.SubString(1,526) );
                    writelog(sTmp);

                 }
             }
     }
 catch(...)
    {
       sTmp.sprintf("_GetZDTZSALCntCheck: Catch Error .. OrgZcnt=%s , Zdt1050=%d",OrgZcnt, iZdt1050 );
       writelog(sTmp);
    }

  sTmp.sprintf("_GetZDTZSALCntCheck:ZDT Check (Z_CNT=%s) OK Total Cnts (%6d)",TmpZcnt,iZdt1050 );
  writelog(sTmp);


 ///////////////////////////////

  TStringList *TmpSalLst =  new TStringList;

  sTmp.sprintf("_GetZDTZSALCntCheck: LoadFromFile(%s)",SalFileName );
  writelog(sTmp);

  TmpSalLst->LoadFromFile(SalFileName);

  try
   {
      for (int i=0;  i<TmpSalLst->Count; i++)
            {
             sRec=TmpSalLst->Strings[i];
             sRecNo=_StringSegment_EX(sRec, "|", 1);
             if( sRecNo != "1050" )
                continue;
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt ==  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    iSal1050++;
                    sTmp.sprintf("_GetZDTZSALCntCheck:SAL(%6d)(%s)",iSal1050, sRec.SubString(1,526) );
                    writelog(sTmp);

                 }
             }
     }
 catch(...)
    {
       sTmp.sprintf("_GetZDTZSALCntCheck: Catch Error %s.. OrgZcnt=%s , iSal1050=%d",SalFileName, OrgZcnt, iSal1050 );
       writelog(sTmp);
    }

  sTmp.sprintf("_GetZDTZSALCntCheck:SAL Check (Z_CNT=%s) OK Total Cnts (%6d)",TmpZcnt,iSal1050 );
  writelog(sTmp);

  RtnZDTCnts=iZdt1050;
  RtnSALCnts=iSal1050;

  if( RtnZDTCnts == RtnSALCnts )
     sTmp.sprintf("_GetZDTZSALCntCheck: **** �ˬd 1050 �� SAL 1050 ���Ƥ@�P    (Z_CNT=%s) (ZDT=%6d), (SAL=%6d) ",TmpZcnt, iZdt1050, RtnZDTCnts );
  else
     sTmp.sprintf("_GetZDTZSALCntCheck: **** �ˬd 1050 �� SAL 1050 ���Ƥ��@�P  (Z_CNT=%s) (ZDT=%6d), (SAL=%6d) ",TmpZcnt, iZdt1050, RtnSALCnts );

  writelog(sTmp);

  delete TmpSalLst;
  return( 0 );


}




int __fastcall  BASIC::GetZCNT_SalData(AnsiString AutoZCnt)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    AnsiString sTmp, sRec, sTmpDttm,TmpZcnt, SalZcnt;
    int i_type, irtn=0;
    //TStringList *sTmlLst = new TStringList();
    SalZcnt=AutoZCnt;
   //delete  sTmlLst;


 try
   {
    gtsl_rec->Clear();
    //gtsl_rec->Assign(Autotsl_rec);
    if( Autotsl_rec->Count > 0)
       {
        for (int i=0;  i<Autotsl_rec->Count; i++)
            {
             sRec=Autotsl_rec->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_rec->Add(sRec);
               }
             else
               {
                 ;;//gtsl_rec->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_tot->Clear();
    //gtsl_tot->Assign(Autotsl_tot);
    if( Autotsl_tot->Count > 0)
       {

        for (int i=0;  i<Autotsl_tot->Count; i++)
            {
             sRec=Autotsl_tot->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_tot->Add(sRec);
                 //sTmp.sprintf("AUTOZDATA::GetYYYYMDD_SalData:(%s)", sRec.c_str() );
                 //writelog(sTmp);
               }
             else
               {
                 ;;//gtsl_tot->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_drop->Clear();
    //gtsl_drop->Assign(Autotsl_drop);
    if( Autotsl_drop->Count > 0)
       {

        for (int i=0;  i<Autotsl_drop->Count; i++)
            {
             sRec=Autotsl_drop->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_drop->Add(sRec);
               }
             else
               {
                 ;;//gtsl_drop->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_easy_card->Clear();
    //gtsl_easy_card->Assign(Autotsl_easy_card);
    if( Autotsl_easy_card->Count > 0)
       {

        for (int i=0;  i<Autotsl_easy_card->Count; i++)
            {
             sRec=Autotsl_easy_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_easy_card->Add(sRec);
               }
             else
               {
                 ;;//gtsl_easy_card->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_pay_sub->Clear();
    //gtsl_pay_sub->Assign(Autotsl_pay_sub);
    if( Autotsl_pay_sub->Count > 0)
       {

        for (int i=0;  i<Autotsl_pay_sub->Count; i++)
            {
             sRec=Autotsl_pay_sub->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_pay_sub->Add(sRec);
               }
             else
               {
                 ;; //gtsl_pay_sub->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_bil->Clear();
    //gtsl_bil->Assign(Autotsl_bil);
    if( Autotsl_bil->Count > 0)
       {

        for (int i=0;  i<Autotsl_bil->Count; i++)
            {
             sRec=Autotsl_bil->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                gtsl_bil->Add(sRec);
               }
             else
               {
                 ;;//gtsl_bil->Delete(i);
               }
            }
       }
    irtn+=1;

    gtsl_fetc_card->Clear();
    //gtsl_fetc_card->Assign(Autotsl_fetc_card);
    if( Autotsl_fetc_card->Count > 0)
       {

        for (int i=0;  i<Autotsl_fetc_card->Count; i++)
            {
             sRec=Autotsl_fetc_card->Strings[i];

             i_type = _StrToInt(sRec.SubString(1,4));

             if( i_type == 1028)
                    TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //�鵲�Ǹ�
             else
                    TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�

             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if( TmpZcnt==SalZcnt )
               {
                 gtsl_fetc_card->Add(sRec);
               }
             else
               {
                 ;;//gtsl_fetc_card->Delete(i);
               }
            }
       }
    irtn+=1;

    tsl_subsale->Clear();
    //tsl_subsale->Assign(Autotsl_subsale);
    if( Autotsl_subsale->Count > 0)
       {

        for (int i=0;  i<Autotsl_subsale->Count; i++)
            {
             sRec=Autotsl_subsale->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 tsl_subsale->Add(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;

    //////////////////////// 2014/12/9 ///////////////////////////////////////
    //�̤鵲�Ǹ���1051List
    gtsl_1051_sal->Clear();
    //tsl_subsale->Assign(Autotsl_subsale);
    if( Auto_1051_sal->Count > 0)
       {

        for (int i=0;  i<Auto_1051_sal->Count; i++)
            {
             sRec=Auto_1051_sal->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_1051_sal->Add(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;

     //////////////////////// 2021/6/15 ///////////////////////////////////////
    gtsl_Ticket->Clear();
    if( Autotsl_Ticket->Count > 0)
       {

        for (int i=0;  i<Autotsl_Ticket->Count; i++)
            {
             sRec=Autotsl_Ticket->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_Ticket->Add(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;
     ///////////////////////////////////////////////////////////////

    } // end of try
  catch(...)
    {
      writelog("GetZCNT_SalData Catch error...");
      irtn=0;
    }

    return(irtn);

}




//Ū�J���LOG�ɡA��TYPE����
int __fastcall BASIC::loadsalfile(AnsiString FileName, bool for_x, AnsiString AutoZ_Cnt,
                                  AnsiString AutoZ_Dt)
{
    init_data();                                //�]�w�򥻸��

    FILE *f_sal_file;                           //SAL�ɫ���

    AnsiString str_sal_data,stmp, TmpSalDttm;                    //SAL�C��r��
    int i_type, i_len;                                 //���TYEP 10,50..�A����
    char c_data_buf[MAXDATALEN];                       //Ū�X�C���ƼȦs�r��

//#ifdef TM_20    //�O������

    writelog("����XZDATA.DLL loadsalfile  ���� InPut File:"+FileName+", AutoZ_Cnt="+AutoZ_Cnt+", AutoZ_Dt="+AutoZ_Dt  );
//#else
//    writelog(_ver);
//#endif

    bool bl_x_start = false;                    //�}�l�x�s�������ȭ�������
    bool bl_x_into = true;                      //�}�l�䦹�����ȭ����

    AnsiString TmpZcnt, SalZcnt, str_trans_now;  //�C���ƥ���Ǹ�

    SalZcnt=AutoZ_Cnt;

    if ((f_sal_file = fopen(FileName.c_str(),"rb")) == NULL)
    {
        gbl_tot_haveline = false;
        gbl_rec_haveline = false;
        gbl_easy_haveline = false;
        gbl_pay_haveline = false;
        gbl_bil_haveline = false;
        gbl_fetc_haveline = false;
        gbl_1051_haveline = false;        //gbl_0041_haveline = false;
        gbl_Ticket_haveline = false;

        writelog("�}�ҥ���ɮ׵o�Ϳ��~ " + FileName);
        return 0;
    }

    TStringList *tsl_x_data = new TStringList; //�x�s�������ȭ�������

    AnsiString str_trans_next;                  //�������ȭ��b�}�l����Ǹ�
    int i_compare_trans = 0;                    //�W�����ȭ��b�̫����Ǹ�
    int i_now_trans=0;

    i_compare_trans = _StrToInt(AnsiString(gchar_x_trans),"�W�����ȭ��b�̫����Ǹ�");          //�W������Ǹ�

    bool bl_is_first = true;

    if (f_sal_file != NULL)
    {
        while (!feof(f_sal_file))               //ŪSAL�ɸ��
        {
            memset(c_data_buf, 0x00, MAXDATALEN);
            fgets(c_data_buf, MAXDATALEN, f_sal_file);
            fscanf(f_sal_file,"\n");  // for if ((f_sal_file = fopen(FileName.c_str(),"rb")) == NULL)

          //  i_len = strlen(c_data_buf);         //�C�����
          //  c_data_buf[i_len-1] = 0;            //�m��\n

            str_sal_data = c_data_buf;

            TmpZcnt=_StringSegment_EX(str_sal_data, "|", 6);  //�鵲�Ǹ�

            TmpSalDttm=_StringSegment_EX(str_sal_data, "|", 5).SubString(1,8);
            //if( AutoZ_Dt != "" )
            //  {
            //     if(  TmpSalDttm > AutoZ_Dt )
            //       continue;
            //  }

          //  if ( (!(*c_data_buf == 0x00) && (!(*c_data_buf == '\n')) && (i_len > 50) ) )
            if( CheckSaleRecData(str_sal_data) )
            {

                strcpy(gpchar_sal_last, c_data_buf);                //SAL�ɳ̫�@�����
                   //gpchar_sal_last = str_sal_data;

                if(  AutoZ_Dt != "" && TmpSalDttm <= AutoZ_Dt)
                  {
                    SalZcnt=TmpZcnt;
                  }

                if( AutoZ_Dt != "" )
                 {
                   if( TmpZcnt > SalZcnt )
                       continue;
                 }

               
                if (bl_is_first)
                {
                    strcpy(gpchar_sal_first, c_data_buf);           //SAL�ɲĤ@��
                  //  gpchar_sal_first = str_sal_data;
                    bl_is_first = false;
                }

                if (for_x)      //���ȭ��b��
                {
                  //  i_compare_trans = atoi(gchar_x_trans);          //�W������Ǹ�
                  //  str_trans_now = str_sal_data.SubString(41,5);   //�ثe����Ǹ�

                    i_type = _StrToInt(str_sal_data.SubString(1,4));

                    if( i_type == 1028 || i_type == 1805 || i_type == 1806 || i_type == 1807)
                        str_trans_now = _StringSegment_EX(str_sal_data, "|", 6).SubString(TRANCNT_NO_POS,5);
                    else
                        str_trans_now = _StringSegment_EX(str_sal_data, "|", 7).SubString(TRANCNT_NO_POS,5);

                    i_now_trans =  _StrToInt(str_trans_now,"�ثe����Ǹ�");
                    if (i_compare_trans == 0)       //�p�G�O0�A�N�q�Y�}�lŪ�J������
                    {
                        bl_x_start = true;
                        bl_x_into = false;
                    }
                    else if (bl_x_into)             //������Ǹ�
                    {
                       // str_trans_next.sprintf("%05d", ++i_compare_trans);
                       // if ((str_trans_now.AnsiCompare(str_trans_next)) == 0)

                       if( i_type != 1095 && i_type != 1096 )
                         {
                          //if (i_now_trans == i_compare_trans  )   2013/10/14 Update
                          // (Ī�w�Ӥl��)���׳q����---131012-0157...FM-012291-���ѤT�I��ZTM01 �e�����J��0 SC�S������ ���J��� ���o�����
                          if (i_now_trans >= i_compare_trans  &&  i_now_trans <= i_compare_trans+10 )
                             {
                               stmp.sprintf("���o�������ȭ��b�}�l����Ǹ� (%05d), �j�󦹧Ǹ��N�C�J���ȭ��b..", i_now_trans);
                               writelog(stmp);
                               bl_x_start = true;
                               bl_x_into = false;

                               //if (i_now_trans > i_compare_trans )   //2013/10/14 Update
                               //    tsl_x_data->Add(str_sal_data);

                               // if (i_now_trans == i_compare_trans) //2004/03/22
                               //     continue;
                             }
                         }     
                    }

                    if (bl_x_start)         //0�άO���������Ǹ��}�lŪ�J���
                    {
                        if (i_now_trans != i_compare_trans)
                              tsl_x_data->Add(str_sal_data);
                    }
                }
                else    //Ū�b�BZ�b��
                {                         
                    i_type = _StrToInt(str_sal_data.SubString(1,4));

                    switch (i_type)
                    {
                        //REC
                        //case 10:
                        case 1010:
                            gtsl_rec->Add(str_sal_data);
                            break;
                        //�馩��
                        //case 11:
                        case 1011:
                            gtsl_pay_sub->Add(str_sal_data);
                            break;
                        //easy card / 31:Kstc ����
                        //case 25:
                        case 1025:
                        case 1031:
                        case 1032:
                        case 1035:
                        case 1034:
                        case 1038:
                        case 1811:
                            gtsl_easy_card->Add(str_sal_data);
                            break;
                        case 1028:
                        case 1030:
                            gtsl_fetc_card->Add(str_sal_data);
                            break;
                        //Bil
                        //case 40:
                        case 1040:
                            gtsl_bil->Add(str_sal_data);
                            break;
                        //��w
                        //case 41:
                        case 1041:
                            gtsl_drop->Add(str_sal_data);
                            break;
                        //TOT
                        //case 50:
                        case 1050:
                            gtsl_tot->Add(str_sal_data);
                           // writelog(FileName+ ":SALTot " + str_sal_data);
                            break;
                        case 1805:
                        case 1806:
                        case 1807:
                            gtsl_Ticket->Add(str_sal_data);
                             break;
                        case 13:
                        case 14:
                        case 15:
                        case 16:
                        case 20:
                        case 27:
                        case 37:
                        //case 31:
                        case 51:
                        case 80:
                        case 90:
                        case 97:
                        case 1098:
                        case 1099:
                        case 1802:
                        case 1803:
                             break;
                        default :
                             //writelog(FileName+ ":Data Erroe " + str_sal_data);
                             break;
                    }
                }   // end of else Ū�b�BZ�b��
            }
            else        //�����Ʀ��~
            {
                if (!feof(f_sal_file))
                    writelog("SAL�ɥ����Ʋ��` " + str_sal_data);
                continue;
            }

        }//while
    }//if (f_sal_file != NULL)

    if (for_x)      //���ȭ��b��
    {
        stmp.sprintf("���o�W�����ȭ��b��Z��̫����Ǹ� (%s)", gchar_x_trans);
        writelog(stmp);
        stmp.sprintf("���o�������ȭ��b�̫����Ǹ� (%s), �Ǹ� (%s) ~ (%s) �N�C�J���ȭ��b.. ",
                      str_trans_now.c_str(), gchar_x_trans, str_trans_now.c_str() );
        writelog(stmp);

        for (int i=0; i<tsl_x_data->Count; i++)
        {
            i_type = _StrToInt(tsl_x_data->Strings[i].SubString(1,4), tsl_x_data->Strings[i]);
  
            switch (i_type)
            {
                //REC
                case 10:
                case 1010:
                    gtsl_rec->Add(tsl_x_data->Strings[i]);
                    break;
                //�馩��
                //case 11:
                case 1011:
                    gtsl_pay_sub->Add(tsl_x_data->Strings[i]);
                    break;
                //easy card
                //case 1025:
                case 1031:
                case 1032:
                case 1035:
                case 1034:
                case 1038:
                case 1811:
                    gtsl_easy_card->Add(tsl_x_data->Strings[i]);
                    break;

                case 1028:
                case 1030:
                     gtsl_fetc_card->Add(tsl_x_data->Strings[i]);
                     break;

                case 1040:
                    gtsl_bil->Add(tsl_x_data->Strings[i]);
                    break;
                //��w
                //case 41:
                case 1041:
                    gtsl_drop->Add(tsl_x_data->Strings[i]);
                    break;
                //TOT
                //case 50:
                case 1050:
                    gtsl_tot->Add(tsl_x_data->Strings[i]);
                    break;
                case 1051:
                    gtsl_1051_sal->Add(tsl_x_data->Strings[i]);
                    break;

                case 1805:
                case 1806:
                case 1807:
                    gtsl_Ticket->Add(tsl_x_data->Strings[i]);
                    break;


            }
        }

    }//for_x
    //else
    //{
       // memset(gpchar_sal_last, 0x00, 380);
        //memset(gpchar_sal_first, 0x00, 380);
    //}

    fclose(f_sal_file);


    if (bl_x_start && gtsl_tot->Count>0)
        strcpy(gpchar_x_first, tsl_x_data->Strings[0].c_str());
    else
        memset(gpchar_x_first, 0x00, MAXDATALEN);

    delete tsl_x_data;


    gi_tot_line = (gtsl_tot->Count - 1);            //TOT���`���
    gi_rec_line = (gtsl_rec->Count - 1);            //REC
    gi_easy_line = (gtsl_easy_card->Count - 1);     //EASY CARD
    gi_pay_line = (gtsl_pay_sub->Count - 1);        //�馩��
    gi_bil_line =  (gtsl_bil->Count - 1);           //BIL
    gi_fetc_line =  (gtsl_fetc_card->Count - 1);    //Fetc Card
    gi_1051_line = (gtsl_1051_sal->Count - 1);      //1051���
    gi_Ticket_Line = (gtsl_Ticket->Count - 1);


    stmp.sprintf("���o�����b����Ƶ��� 1010=%d, 1011=%d, 1031~1035=%d, 1040=%d, 1041=%d, 1050=%d, 1051=%d, 1028,1030=%d, Ticket_Line=%d",
                 gtsl_rec->Count,gtsl_pay_sub->Count, gtsl_easy_card->Count,
                 gtsl_bil->Count,gtsl_drop->Count, gtsl_tot->Count, gtsl_1051_sal->Count, gtsl_fetc_card->Count,
                 gtsl_Ticket->Count  );
    writelog(stmp);

    if (gi_tot_line > -1)
        gbl_tot_haveline = true;    //TOT���Ƭ�����
    else
        gbl_tot_haveline = false;

    if (gi_rec_line > -1)
        gbl_rec_haveline = true;    //REC���Ƭ�����
    else
        gbl_rec_haveline = false;

    if (gi_bil_line > -1)
        gbl_bil_haveline = true;    //Bil���Ƭ�����
    else
        gbl_bil_haveline = false;


    if (gi_easy_line > -1)
        gbl_easy_haveline = true;   //EASY CARD���Ƭ�����
    else
        gbl_easy_haveline = false;

    if (gi_pay_line > -1)
        gbl_pay_haveline = true;    //�馩�����Ƭ�����
    else
        gbl_pay_haveline = false;

    if (gi_fetc_line > -1)
        gbl_fetc_haveline = true;   //EASY CARD���Ƭ�����
    else
        gbl_fetc_haveline = false;

    if (gi_1051_line > -1)
        gbl_1051_haveline = true;   //1051���Ƭ�����
    else
        gbl_1051_haveline = false;

    if (gi_Ticket_Line > -1)
        gbl_Ticket_haveline = true;   //1805, 1806, 1807 ���Ƭ�����
    else
        gbl_Ticket_haveline = false;


    return OK;
}


bool __fastcall BASIC::CheckSaleRecData(AnsiString RecData)
{
    AnsiString strtmp, sRecType;
    bool blFileRecErr;
    int i,itype;

    blFileRecErr = false;
    i=RecData.Length() ;
    sRecType=RecData.SubString(1,4);
    try
      {
       itype=sRecType.ToInt();

        switch (itype)
            {
              case 1001:
              case 1099:
                    // if( i == REC0199LENGTH || i == 57)
                           blFileRecErr = true;
                     gsLast1099=RecData;
                        break;
              /**
              case 10:
                     if( i == REC10LENGTH )
                           blFileRecErr = true;
                        break;
              case 11:
                     if( i == REC11LENGTH )
                           blFileRecErr = true;
                        break;
              case 13:
                     if( i == REC13LENGTH )
                           blFileRecErr = true;
                        break;
              case 15:
                     if( i == REC15LENGTH )
                           blFileRecErr = true;
                        break;
              case 40:
                     if( i == REC40LENGTH )
                           blFileRecErr = true;
                        break;
              case 20:
                     if( i == REC20LENGTH )
                           blFileRecErr = true;
                        break;
              case 25:
                     if( i == REC25LENGTH )
                           blFileRecErr = true;
                        break;
              case 30:
                     if( i == REC30LENGTH )
                           blFileRecErr = true;
                        break;
              case 31:      //546
                     if( i == REC31LENGTH )
                           blFileRecErr = true;
                        break;
              case 35:
                     if( i == REC35LENGTH )
                           blFileRecErr = true;
                        break;
              case 41:
                     if( i == REC41LENGTH )
                           blFileRecErr = true;
                        break;
              case 50:
                     if( i == REC50LENGTH )
                           blFileRecErr = true;
                        break;
              case 51:
                     if( i == REC51LENGTH )
                           blFileRecErr = true;
                        break;
             

             **/
              default:
                      blFileRecErr = true;
                      break ;
            }
     }
     catch(...)
     {
       blFileRecErr == false ;
     }

     if( blFileRecErr == false)
        {
         strtmp.sprintf("CheckSaleRecData:  RecData(%s) �����Ʋ��` Length=%d ",
                                      RecData.c_str(),i  );
         writelog(strtmp);
        }
  return(blFileRecErr);
}





BASIC::~BASIC()
{
    delete gtsl_rec,
           gtsl_tot,
           gtsl_bil,
           gtsl_drop,
           gtsl_easy_card,
           gtsl_pay_sub,
           gtsl_1051_sal,
           tsl_subsale;

    delete Autotsl_rec ;
    delete Autotsl_tot ;
    delete Autotsl_drop ;
    delete Autotsl_easy_card ;
    delete Autotsl_pay_sub ;
    delete Autotsl_bil ;
    delete Autotsl_fetc_card ;
    delete Autotsl_subsale ;
    delete Auto_Sal ;
    delete Auto_1051_sal;

    delete gtsl_Ticket;
    delete Autotsl_Ticket;

}

AnsiString __fastcall BASIC::BackupFile(AnsiString FileName, AnsiString BackupFileName, AnsiString DDZ  )  //�ɮ׳ƥ�
{

    if (!FileExists(FileName))
    {
        writelog("�ƥ�����ɤ��s�b " + FileName);
        return "NONFILE";
    }
    else
    {
       writelog("�ƥ����=" + FileName + "  BackupFileName=" + BackupFileName + "  DDZ="+DDZ );
    }



    AnsiString sFilePathDD;                //���
    AnsiString sFilePathNum;               //����
    AnsiString sBackupPath;                //�ƥ����|
    AnsiString sBackupFileName;            //�ƥ��ɦW

    AnsiString sHistory = "C:\\FTLPOS\\HISTORY\\";
    AnsiString sFileNameNonPath = ExtractFileName(FileName);    //�ɦW���]�t���|

    if( Trim(DDZ) !="")
      {
         sFilePathDD  = DDZ.SubString(1,2);
         sFilePathNum = "0"+DDZ.SubString(3,1);
      }
    else
      {
        //020312250100.zdt
      sFilePathDD  = sFileNameNonPath.SubString(7,2);
      sFilePathNum = sFileNameNonPath.SubString(9,2);

      }


    if( Trim(BackupFileName) !="")
      {
        sBackupPath=ExtractFilePath(BackupFileName);
        sFileNameNonPath = ExtractFileName(BackupFileName);    //�ɦW���]�t���|
      }
     else
      {

        //�ƥ����|
        if ( (!sFilePathDD.IsEmpty()) && (!sFilePathNum.IsEmpty()) )
             sBackupPath = (sHistory + sFilePathDD + "\\" + sFilePathNum + "\\");
        else
             sBackupPath = sHistory;

      }

    //�إߥؿ�
    if (!DirectoryExists(sBackupPath))
        ForceDirectories(sBackupPath);


    sBackupFileName = (sBackupPath + sFileNameNonPath);             //�ƥ��ɦW���|�ɦW

    DeleteFile(sBackupFileName);                             

    if (RenameFile(FileName, sBackupFileName))              //�h��
        writelog("�ƥ�����ɦW OK : " + FileName + " To " +sBackupFileName);
    else
        writelog("�ƥ�����ɦW NG : " + FileName + " To " +sBackupFileName);

    return sBackupFileName;                                 //�^���ɮ׸��|
}

int __fastcall _StrToInt(AnsiString S, AnsiString Data)
{
    int i = 0;
    float amt;


    try
    {
        if(Trim(S)=="")
          S="0";  
        amt=StrToFloat(S);
        i = amt; //StrToInt(S);
    }
    catch(Exception &e)
    {
        //writelog("�ƭ��ഫ���~ " + S + " " + Data);
        return 0;
    }

    return i;
}

//����ӫȼ�
int __fastcall BASIC::SumQt_tcust(int StartLine,int TotalLine)
{
    AnsiString str_trans_type,s;       //�P�⫬�A
    int i_trans_cnt = 0;             //�ӫȼƭp��
    int i_invo_cnt = 0;              //�P��ƶq
    int iCOInvoAmtTxt=0;
    int iCOInvoAmtNxt=0;

    giA=0; //�I�d����O�ӫȼ�
    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=StartLine; i<gtsl_tot->Count ; i++)
        {
            s=gtsl_tot->Strings[i];
            str_trans_type = _StringSegment_EX(s, "|", 8);
            //i_invo_cnt = _StrToInt(gtsl_tot->Strings[i].SubString(220,5)); //pc_cnt
            iCOInvoAmtTxt=_StrToInt(_StringSegment_EX(s, "|", 49)) ;  //C0 00 ���|
            iCOInvoAmtNxt=_StrToInt(_StringSegment_EX(s, "|", 50)) ;  //C0 00 �K�| + S0 02 �K�|�N�� ,(�ثe���|�� C0 00 �K�| )

            //invo_amt+kcolx_amt+kcolf_amt  (32:�o�����B, 37:�馩\�����B, 44:���}�o�����B(��), 45:���}�o�����B(�K)
            //�o�����B  2019/02/13     �l�[ 37:�馩\�����B
            i_invo_cnt = _StrToInt(_StringSegment_EX(s, "|", 32))+
                         _StrToInt(_StringSegment_EX(s, "|", 37))+
                         _StrToInt(_StringSegment_EX(s, "|", 44))+
                         _StrToInt(_StringSegment_EX(s, "|", 45))-iCOInvoAmtTxt-iCOInvoAmtNxt;
                         //_StrToInt(gtsl_tot->Strings[i].SubString(345,8))+
                         //_StrToInt(gtsl_tot->Strings[i].SubString(354,8));

            if (str_trans_type.Pos("Z"))
            {
                //�P��ƶq����0�A�N���B��B�I�A�L�P��ƶq
                if (i_invo_cnt>0)
                   { ++i_trans_cnt; }
                else  if( i_invo_cnt<=0 && iCOInvoAmtTxt+iCOInvoAmtNxt>0 )
                   {  ++giA;  }

            }
            else if (str_trans_type.Pos("R2") || str_trans_type.Pos("R3"))
            {
                if (i_invo_cnt>0)
                    { --i_trans_cnt;  }
                else  if( i_invo_cnt<=0 && iCOInvoAmtTxt+iCOInvoAmtNxt>0 )
                    { --giA; }

            }
        }
    }

    return i_trans_cnt;
}

//����~����
int __fastcall BASIC::SumQt_trans(int StratLine, int TotalLine)
{

    AnsiString str_trans_type,s;  //�P�⫬�A
    int i_trans_num = 0;        //����~����
    int i_invo_cnt = 0;         //�P��ƶq����0

    if (gbl_tot_haveline)
    {
        //for (int i=StratLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_trans_type = _StringSegment_EX(s, "|", 8);

            i_invo_cnt = _StrToInt(_StringSegment_EX(s, "|", 30));


            if ( str_trans_type.Pos("Z") )
            {
                if (i_invo_cnt)     //�P��ƶq����0
                    i_trans_num += i_invo_cnt;
            }
            else if ( str_trans_type.Pos("R2") || str_trans_type.Pos("R3") )
            {
                i_trans_num -= i_invo_cnt;
            }
        }
    }

    return i_trans_num;
}



//�o���鵲�b���B
void __fastcall BASIC::TotCount(int StartLine, int TotalLine)
{

   //sThsrcStr=_StringSegment_EX(s, "|", 1);

    //�K�B �|�B ��
    giA = giB = giC = 0;

    AnsiString str_trans_type, tmpSer, strtmp, sSale;  //�P�⫬�A
    int iTmpA, iTmpB, iTmpC,iTmpD;

    iTmpA=iTmpB=iTmpC=iTmpD=0;
    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            sSale=gtsl_tot->Strings[i];

            //str_trans_type = gtsl_tot->Strings[i].SubString(67,2);
            str_trans_type = _StringSegment_EX(sSale, "|", 8);

            //tmpSer=gtsl_tot->Strings[i].SubString(36,10);  //����Ǹ�
            tmpSer=_StringSegment_EX(sSale, "|", 7);  //����Ǹ�

            iTmpA=iTmpB=iTmpC=0;

            //�o�����L�B���`�P��
            if ( str_trans_type.Pos("R4") || str_trans_type.Pos("Z0") )
            {
                //iTmpA=_StrToInt(gtsl_tot->Strings[i].SubString(309,8));   //�o���鵲�b�B(�K)
                iTmpA=_StrToInt(_StringSegment_EX(sSale, "|", 40));   //�o���鵲�b�B(�K)
                giA += iTmpA;
                // iTmpB=_StrToInt(gtsl_tot->Strings[i].SubString(300,8));   //�o���鵲�b�B(�|)
                iTmpB=_StrToInt(_StringSegment_EX(sSale, "|", 39));   //�o���鵲�b�B(�|)
                giB += iTmpB;
                //iTmpC=_StrToInt(gtsl_tot->Strings[i].SubString(236,9));   //�o���鵲�b���B
                iTmpC=_StrToInt(_StringSegment_EX(sSale, "|", 32));   //�o���鵲�b���B
                giC += iTmpC;

                //iTmpD=_StrToInt(_StringSegment_EX(sSale, "|", 49));   //�I�d����O  �� Sub_C0C1TaxInvoAmt �B�z
                iTmpD=0;
                giB -= iTmpD;
                giC -= iTmpD;

                if( iTmpC != (iTmpA+iTmpB) )
                  {
                    strtmp.sprintf("TotCount:�o���鵲�b, ����Ǹ�(%s) �o�����B���� , (�o�����B=%d) != (�o��(�K)=%d)+(�o��(�|=%d)-�I�d����O(%d) )",
                                      tmpSer,iTmpC, iTmpA, iTmpB, iTmpD );
                    writelog(strtmp);
                  }

            }
        }
    }

    strtmp.sprintf("TotCount:�o���鵲�b���B, (�o�����B=%d) => (�o��(�K=%d))+(�o��(�|=%d)���t�I�d����O(%d) )",
                    giC, giA, giB, iTmpD );
    writelog(strtmp);

}

//�~���o��
void __fastcall BASIC::TotMiss(int StartLine, int TotalLine)
{
    //���B�K�B  �|�B  ���B
    giA = giB = giC = giD = 0;

    AnsiString str_miss_type, tmpSer,strtmp, s ;
    int iTmpB, iTmpC, iTmpD, iTmpE ;

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
           s=gtsl_tot->Strings[i];
           str_miss_type = _StringSegment_EX(s, "|", 8);
           tmpSer=_StringSegment_EX(s, "|", 7);  //����Ǹ�
           iTmpD=iTmpB=iTmpC=iTmpE=0;

            if ( str_miss_type.Pos("R") )
            {
                ++giA; //�~���o������
                iTmpB=_StrToInt(_StringSegment_EX(s, "|", 40)); //�~���o�����B(�K)
                giB += iTmpB;
                iTmpC=_StrToInt(_StringSegment_EX(s, "|", 39)); //�~���o�����B(�|)
                giC += iTmpC;
                iTmpD=_StrToInt(_StringSegment_EX(s, "|", 32)); //�~���o�����B
                giD += iTmpD;

                //iTmpE=_StrToInt(_StringSegment_EX(s, "|", 49)); //�I�d����O �� Sub_C0C1TaxInvoAmt �B�z
                iTmpE=0;
                giC-=iTmpE;
                giD-=iTmpE;

                strtmp.sprintf("TotMiss:�~���o��, ����Ǹ�(%s) (�o�����B=%6d) => (�o��(�K%6d) )+(�o��(�|%6d) ���t�I�d����O(%d) )  =>>�֭p=%d",
                                      tmpSer,iTmpD, iTmpB, iTmpC, iTmpE, giD );
                writelog(strtmp);

                 if( iTmpD != (iTmpB+iTmpC-iTmpE) )
                  {
                    strtmp.sprintf("TotMiss:�~���o��, ����Ǹ�(%s) �o�����B���� , (�o�����B=%d) != (�o��(�K%d))+(�o��(�|%d) - �I�d����O(%d) )",
                                      tmpSer,iTmpD, iTmpB, iTmpC, iTmpE );
                    writelog(strtmp);
                  }
            }
        }
    }
}



//�~���o���i��
void __fastcall BASIC::TotMissCnt(int StartLine, int TotalLine)
{
    giA = giB = giC = giD = 0;

    AnsiString str_miss_type, tmpSer,strtmp, tmpVsNo, s ;
    int iTmpB, iTmpC, iTmpD;

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
       for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_miss_type = _StringSegment_EX(s, "|", 8); //gtsl_tot->Strings[i].SubString(67,2);
            tmpSer=_StringSegment_EX(s, "|", 7);          //gtsl_tot->Strings[i].SubString(36,10);  //����Ǹ�
            iTmpD=iTmpB=iTmpC=0;

            if ( str_miss_type.Pos("R") || str_miss_type.Pos("V") )
            {
                strtmp=_StringSegment_EX(s, "|", 9);
                tmpVsNo=strtmp.SubString(3,8);   //gtsl_tot->Strings[i].SubString(73,8); �o�����X�_
                iTmpB=_StrToInt(strtmp.SubString(8,3)); //�~���o�����X�_

                strtmp=_StringSegment_EX(s, "|",10);
                iTmpC=_StrToInt(strtmp.SubString(8,3)); //�~���o�����X��
                if(iTmpC < iTmpB)  // 2008/05/07 Update
                  {
                   strtmp.sprintf("TotMissCnt:����Ǹ�: %s �~���o�����X�����~, �o�����X�_(%03d),  �o�����X��(%03d)",
                                   tmpSer, iTmpB, iTmpC );
                   writelog(strtmp);
                   // iTmpC=999;
                  }

                //if( iTmpC>0 && iTmpB>0)
                // if(tmpVsNo != "00000000" )
                 if( Trim(tmpVsNo)!="" && tmpVsNo!="00000000" )
                   {
                     if ( str_miss_type.Pos("R") )
                        {
                          giA+=abs(iTmpC-iTmpB)+1; //�~���o���i��
                        }
                     else if ( str_miss_type.Pos("V") )
                        {
                          giB+=abs(iTmpC-iTmpB)+1; //�~���o���i��
                        }
                    }
                  else
                    {
                       strtmp.sprintf("TotMissCnt:����Ǹ�: %s �������~���o���i��, tmpVsNo=%s, �o�����X�_(%08ld), �o�����X��(%08ld)",
                                      tmpSer, tmpVsNo, iTmpB, iTmpC );
                       writelog(strtmp);
                    }
             }
         }
    }

    strtmp.sprintf("TotMissCnt:�~���o���i��, �h�fR2R3R4(%d), �������V(%d)", giA, giB );
    writelog(strtmp);

}



//�{���X�p
int __fastcall BASIC::SumCash(int StartLine, int TotalLine)
{
    AnsiString str_trans_type,s;      //�P�⫬�A
    int i_cash_z = 0;               //Z Type
    int i_cash_r = 0;               //R Type
    int i_cash_amt = 0;             //�{���[�`

    //20031223�������f��
    int pay_cnt = 0;    //����
    int pay_amt = 0;    //���B


    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
       for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_trans_type = _StringSegment_EX(s, "|", 8);

            if ( str_trans_type.Pos("Z") || str_trans_type.Pos("V") )
            {
                i_cash_z += _StrToInt(_StringSegment_EX(s, "|", 15));
            }
            else if ( str_trans_type.Pos("R2") || str_trans_type.Pos("R3") )
            {
                i_cash_r += _StrToInt(_StringSegment_EX(s, "|", 15));
            }

            //20031225�������f��
            pay_amt = _StrToInt(_StringSegment_EX(s, "|", 21));
            if (pay_amt != 0)
            {
                if ( str_trans_type.Pos("Z"))
                {
                    pay_bill_amt += pay_amt;
                    ++pay_bill_cnt;

                }
                if (str_trans_type.Pos("R4"))   //20040112 ���L�O�P�ɲ��ͤ@���@�t�V
                {                               //���V��B�t�V�[
                    pay_bill_amt += pay_amt;
                    ++pay_bill_cnt;
                    mpay_bill_amt -= pay_amt;
                    --mpay_bill_cnt;

                }
                
                if ( str_trans_type.Pos("R2") || str_trans_type.Pos("R3") )
                {
                    mpay_bill_amt -= pay_amt;
                    --mpay_bill_cnt;
                }
            }
        }
    }

    i_cash_amt = (i_cash_z - i_cash_r);

    s.sprintf("SumCash:�{���X�p, i_cash_z(%d), i_cash_r(%d), pay_bill_amt(%d)",
    i_cash_z, i_cash_r, pay_bill_amt );
    writelog(s);

    return i_cash_amt;
}



////////////////////////////////////////////////////////////////////////////
//���}�o�����B
int __fastcall BASIC::SumNOVoiceAmt(int StartLine, int TotalLine)
{
    AnsiString str_trans_type, temp, s, Am1, Am2;      //�P�⫬�A
    giA = giB = 0;
    //AnsiString str_miss_type, tmpSer,strtmp ;
    int iTmpA, iTmpB, iTmpC, iTmpD;

    iTmpA=iTmpB=0;
    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_trans_type=_StringSegment_EX(s, "|", 8);

            Am1=_StringSegment_EX(s, "|", 44);    //���}�o�����B(��)
            Am2=_StringSegment_EX(s, "|", 45);    //���}�o�����B(�K)

            if ( str_trans_type.Pos("Z") )
            {
                iTmpA += _StrToInt(Am1)+  _StrToInt(Am2);  //pay_add_pe

            }
            else if ( str_trans_type.Pos("R2") || str_trans_type.Pos("R3") )
            {
                //iTmpA += _StrToInt(gtsl_tot->Strings[i].SubString(345,8));
                iTmpB +=  _StrToInt(Am1)+  _StrToInt(Am2);

            }
            else if ( str_trans_type.Pos("R4"))
            {
               iTmpA +=  _StrToInt(Am1)+  _StrToInt(Am2);

               iTmpB +=  _StrToInt(Am1)+  _StrToInt(Am2);

            }
            else
             {
              ;;
             }
        }
    }

   // giA = (iTmpA - iTmpB);
    giA = iTmpA ;
    giB = iTmpB ;

    temp.sprintf("SumNOVoiceAmt:1050->(44,45)���}�o�����B,�X�p���B (Z0:%d),(R2R3R4:%d)",
                  iTmpA,iTmpB);
    writelog(temp);

    return giA;
}




//�o���I�����B  �����o�����B
int __fastcall BASIC::VoicePayAmt(int StartLine, int TotalLine)
{
    AnsiString str_trans_type, temp, s, Am1, Am2;      //�P�⫬�A
    giA = giB = 0;
    //AnsiString str_miss_type, tmpSer,strtmp ;
    int iTmpA, iTmpB, iTmpC, iTmpD;

    iTmpA=iTmpB=0;
    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_trans_type=_StringSegment_EX(s, "|", 8);

            Am1=_StringSegment_EX(s, "|", 25);    //��I����6 �o���I�����B

            if ( str_trans_type.Pos("Z") )
            {
                iTmpA += _StrToInt(Am1);

            }
            else if ( str_trans_type.Pos("R2") || str_trans_type.Pos("R3") )
            {
                //iTmpA += _StrToInt(gtsl_tot->Strings[i].SubString(345,8));
                iTmpB +=  _StrToInt(Am1);

            }
            else if ( str_trans_type.Pos("R4"))
            {
               iTmpA +=  _StrToInt(Am1);

               iTmpB +=  _StrToInt(Am1);

            }
            else
             {
              ;;
             }
        }
    }


    //giA = iTmpA ;
    //giB = iTmpB ;
    giA = (iTmpA - iTmpB);

    temp.sprintf("VoicePayAmt:1050->(25)�o���I�����B,�X�p���B (Z0:%d)-(R2R3R4:%d)=%d ",
                  iTmpA,iTmpB, giA);
    writelog(temp);

    return giA;
}







//��鳡��/�����n�J
int __fastcall BASIC::SumInpdp(int StartLine, int TotalLine)
{
    AnsiString sRType, sType;
    AnsiString str_dept,s ;

    int iDept = 0;      //�����O

    //�ƶq�B���B
    giA = giB = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            sType = _StringSegment_EX(s, "|", 8);

            //�P�O����
            str_dept = _StringSegment_EX(s, "|", 20).SubString(1,2).Trim();

            if (str_dept.Length() > 0)
                iDept = _StrToInt(str_dept);
            else
                iDept = 20;         //�L�����A�w�]20����

            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);

            //���`�P��
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {
                if ((iDept == 13) || (iDept == 14))
                {
                    // sType = _StringSegment_EX(s, "|", 8);

                    if (sType.Pos("B0"))        //�����P��
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));
                    }
                    else if (sType.Pos("B1"))   //�P�����
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));
                    }
                }
            }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {
                if ( (iDept==13) || (iDept==14) )
                {
                    // sType = gtsl_rec->Strings[i].SubString(68,2);

                    if (sType.Pos("B0"))        //�����P��
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));
                    }
                    else if (sType.Pos("B1"))   //�P�����
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));
                    }
                }
            }
        }
    }

    return 0;
}



//�Y���ʥN��
int __fastcall BASIC::SumRelDimAgn(int StartLine, int TotalLine)
{
    AnsiString sRelType,sRType,sType,temp,s , sFG_SPC;  //�s�u�ӫ~�Ϥ�
    int  iRelType ;

    //�ƶq�B���B
    giA = giB = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {

            s=gtsl_rec->Strings[i];
            //str_trans_type=_StringSegment_EX(s, "|", 8);
            sFG_SPC = _StringSegment_EX(s, "|", 10);
            //�s�u�ӫ~�Ϥ�
            sRelType = _StringSegment_EX(s, "|", 36).SubString(1,2).Trim();
            iRelType=_StrToInt(sRelType);

            //������A�Ϥ�
            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2).Trim();
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1") || Trim(sRType)=="" )
            {
                //2013/11/25  S0  VDC06                             $80 ==>���󬰦��N��I�s���M���s�u�Ϥ� => �Y���ʥN��X�p
                //if (iRelType==7 || iRelType==9)  //2006/12/20
                if (iRelType>0 )
                {
                    sType = _StringSegment_EX(s, "|", 8);
                    if ( sType.Pos("S0") || (sType.Pos("A0") && sFG_SPC.Pos("9")) )        //�P��   A0 09 �w��ӫ~
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));
                        temp.sprintf("SumRelDimAgn:�Y���ʥN��:%s-->+tot=%d", s.c_str(), giB);
                        writelog(temp);
                    }
                    else if (sType.Pos("S1") || (sType.Pos("A1") && sFG_SPC.Pos("9")) )   //��    A0 09 �w��ӫ~
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));
                        temp.sprintf("SumRelDimAgn:�Y���ʥN��:%s-->-tot=%d", s.c_str(), giB);
                        writelog(temp);
                    }


                }
            }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {
                //if (iRelType==7 || iRelType==9)
                if (iRelType>0 )
                {
                    sType = _StringSegment_EX(s, "|", 8);

                    if (sType.Pos("S0") || (sType.Pos("A0") && sFG_SPC.Pos("9")) )        //�����P��
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));
                        temp.sprintf("SumRelDimAgn:�Y���ʥN��:%s-->-tot=%d", s.c_str(), giB);
                         writelog(temp);
                    }
                    else if (sType.Pos("S1") || (sType.Pos("A1") && sFG_SPC.Pos("9")) )   //�P�����
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));
                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));
                        temp.sprintf("SumRelDimAgn:�Y���ʥN��:%s-->+tot=%d", s.c_str(), giB);
                         writelog(temp);
                    }

                }
            }
        } // end of for
    }     // if (gbl_rec_haveline)

    temp.sprintf("SumRelDimAgn:�Y���ʥN��,�X�p���B gbl_rec_haveline=%d, (%d,%d)",
                  gbl_rec_haveline, giA, giB);
    writelog(temp);

    return 0;
}




/////////////////////////////////

int __fastcall BASIC::SumPreSal_MM_DisAmtTMMFP(int StartLine, int TotalLine)
{
    AnsiString sRType, sType, temp, sFG_SPC, sDONGLE_TYPE;
    AnsiString str_dept,s ;

    temp.sprintf("SumPreSal_MMCPN_DisAmtTMMFP:�w����(MM)�馩  StartLine=%d, TotalLine=%d",StartLine, TotalLine );
    writelog(temp);

    //�ƶq�B���B
    giU = giV = giW = giX = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            sType = _StringSegment_EX(s, "|", 8);
            sFG_SPC = _StringSegment_EX(s, "|", 10);
            sDONGLE_TYPE = _StringSegment_EX(s, "|", 11);
            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);

            //���`�P��
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {

                   if (sType.Pos("S0") && sFG_SPC.Pos("11") )        //��(/MM)
                   {
                       giU += _StrToInt(_StringSegment_EX(s, "|", 29));   
                   }
                   else if (sType.Pos("S1") && sFG_SPC.Pos("11") )        //��(/MM)
                   {
                       giU -= _StrToInt(_StringSegment_EX(s, "|", 29));
                   }
             }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {
                   if (sType.Pos("S0") && sFG_SPC.Pos("11") )        //��(MM)
                   {
                      giV += _StrToInt(_StringSegment_EX(s, "|", 29));
                   }
                   else if (sType.Pos("S1") && sFG_SPC.Pos("11") )        //��(MM)
                   {
                      giV -= _StrToInt(_StringSegment_EX(s, "|", 29));
                   }

            }
        }
    }


    temp.sprintf("SumPreSal_MMCPN_DisAmtTMMFP:�w����(MM)�馩 =%d, Z0��(MM)=%d, R2R3��(MM)=%d",giU, giV  );
    writelog(temp);

    return 0;
}



//�w����(�������:giU , ��MM_MFP:giV , ��MM_TM:giX
int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int StartLine, int TotalLine)
{
    AnsiString sRType, sType, temp, sFG_SPC, sDONGLE_TYPE;
    AnsiString str_dept,s ;

    temp.sprintf("SumPreSal_MMCPN_DisAmt:�w����(�����/MM)�馩  StartLine=%d, TotalLine=%d",StartLine, TotalLine );
    writelog(temp);

    //�ƶq�B���B
    giU = giV = giW = giX = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            sType = _StringSegment_EX(s, "|", 8);
            sFG_SPC = _StringSegment_EX(s, "|", 10);
            sDONGLE_TYPE = _StringSegment_EX(s, "|", 11);
            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);

            //���`�P��
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {
                   if (sType.Pos("S0") && sFG_SPC.Pos("10") )        //��(�����)
                    {
                        giU += _StrToInt(_StringSegment_EX(s, "|", 29));
                    }
                   else if (sType.Pos("S1") && sFG_SPC.Pos("10") )        //��(�����)
                    {
                        giU -= _StrToInt(_StringSegment_EX(s, "|", 29));

                    }
                   else if (sType.Pos("S0") && sFG_SPC.Pos("11") )        //��(/MM)
                   {
                        /**  2019/03/27 Update
                        if (sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC") )
                             giV += _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_MFP
                        else
                             giX += _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_TM
                        **/
                        if (sDONGLE_TYPE.Pos("TM") )
                             giX += _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_TM
                        else
                             giV += _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_MFP




                   }
                   else if (sType.Pos("S1") && sFG_SPC.Pos("11") )        //��(/MM)
                   {
                        /**  2019/03/27 Update
                        if (sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC") )
                            giV -= _StrToInt(_StringSegment_EX(s, "|", 29));    //��MM_MFP
                        else
                            giX -= _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_TM
                        **/
                       if (sDONGLE_TYPE.Pos("TM") )
                             giX -= _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_TM
                        else
                             giV -= _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_MFP

                   }

            }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {

                   if (sType.Pos("S0") && sFG_SPC.Pos("10") )        //��(�����)
                    {
                        giU -= _StrToInt(_StringSegment_EX(s, "|", 29));
                    }
                   else if (sType.Pos("S1") && sFG_SPC.Pos("10") )        //��(�����)
                    {
                        giU += _StrToInt(_StringSegment_EX(s, "|", 29));

                    }
                   else if (sType.Pos("S0") && sFG_SPC.Pos("11") )        //��(MM)
                   {
                      /**  2019/03/27 Update
                      if (sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC"))
                          giV -= _StrToInt(_StringSegment_EX(s, "|", 29));  //��MM_MFP
                      else
                          giX -= _StrToInt(_StringSegment_EX(s, "|", 29));  //��MM_TM
                      **/
                      if (sDONGLE_TYPE.Pos("TM") )
                             giX -= _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_TM
                      else
                             giV -= _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_MFP

                   }
                   else if (sType.Pos("S1") && sFG_SPC.Pos("11") )        //��(MM)
                   {
                        /**  2019/03/27 Update
                        if (sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC") )
                            giV += _StrToInt(_StringSegment_EX(s, "|", 29));  //��MM_MFP
                        else
                            giX += _StrToInt(_StringSegment_EX(s, "|", 29));  //��MM_TM
                        **/
                        if (sDONGLE_TYPE.Pos("TM") )
                             giX += _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_TM
                        else
                             giV += _StrToInt(_StringSegment_EX(s, "|", 29));   //��MM_MFP
                   }

            }
        }
    }

    giV=giV*(-1);
    giX=giX*(-1);
    temp.sprintf("SumPreSal_MMCPN_DisAmt:�w����(�����/MM)�馩  ��(�����)=%d, ��(MM_MFP)=%d, ��(MM_TM)=%d",giU, giV, giX  );
    writelog(temp);

    return 0;
}



//�w��I���ӫ~������馩/�N���ӫ~������馩:giU
int __fastcall BASIC::SumPreSal_INVO_DisAmt(int StartLine, int TotalLine)
{
    AnsiString sRType, sType, temp, sFG_SPC, sDONGLE_TYPE;
    AnsiString str_dept,s ;

    temp.sprintf("SumPreSal_INVO_DisAmt:�N���ӫ~������馩  StartLine=%d, TotalLine=%d",StartLine, TotalLine );
    writelog(temp);

    //�ƶq�B���B
    giU = giV = giW = giX = 0;
    int iAM_SUB_SUB, iAM_ITEM, iQty;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            sType = _StringSegment_EX(s, "|", 8);
            sFG_SPC = _StringSegment_EX(s, "|", 10);
            sDONGLE_TYPE = _StringSegment_EX(s, "|", 11);
            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);
            iAM_ITEM = _StrToInt(_StringSegment_EX(s, "|", 29));
            iAM_SUB_SUB = _StrToInt(_StringSegment_EX(s, "|", 31));
            iQty = iAM_SUB_SUB/3;


            //���`�P��
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {
                   if (sType.Pos("A0") && sFG_SPC.Pos("10") )        //�w��I���ӫ~
                    {
                        giU += iAM_SUB_SUB;                          //��������B�u��
                        giV += iQty;
                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("10") )
                    {
                        giU -= iAM_SUB_SUB;                          //��������B�u��
                        giV -= iQty;

                    }
                    /*****
                   else if  (sType.Pos("S0") && sFG_SPC.Pos("10") )
                   {
                      giW += iAM_ITEM;                              //�� CPN
                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("10") )
                   {
                      giW -= iAM_ITEM;                              //�� CPN
                   }
                   else if (sType.Pos("S0") && sFG_SPC.Pos("11") )
                   {
                       //�� MM
                       //  2019/03/27 Update
                       //if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC") )
                       //     giX += iAM_ITEM;   //FP+MFP
                       //else
                       //     giY += iAM_ITEM;   //TM

                       if( sDONGLE_TYPE.Pos("TM") )
                            giY += iAM_ITEM;   //TM
                       else
                            giX += iAM_ITEM;   //FP+MFP

                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("11") )
                   {
                       //�� MM
                       //  2019/03/27 Update
                       //if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC"))
                       //     giX -= iAM_ITEM;   //FP+MFP
                       //else
                       //     giY -= iAM_ITEM;   //TM
                       if( sDONGLE_TYPE.Pos("TM") )
                            giY -= iAM_ITEM;   //TM
                       else
                            giX -= iAM_ITEM;   //FP+MFP


                   }
                  ******/
             }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {

                   if (sType.Pos("A0") && sFG_SPC.Pos("10") )        //�w��I���ӫ~
                    {
                        giU -= iAM_SUB_SUB;                         //��������B�u��
                        giV -= iQty;
                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("10") )
                    {
                        giU += iAM_SUB_SUB;                        //��������B�u��
                        giV += iQty;

                    }
                   /*******
                   else if  (sType.Pos("S0") && sFG_SPC.Pos("10") )
                   {
                      giW -= iAM_ITEM;                              //�� CPN
                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("10") )
                   {
                      giW += iAM_ITEM;                              //�� CPN
                   }
                   else if (sType.Pos("S0") && sFG_SPC.Pos("11") )
                   {
                       //�� MM
                       //if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC"))
                       //     giX -= iAM_ITEM;   //FP+MFP
                       //else
                       //     giY -= iAM_ITEM;   //TM

                       if( sDONGLE_TYPE.Pos("TM") )
                            giY -= iAM_ITEM;   //TM
                       else
                            giX -= iAM_ITEM;   //FP+MFP

                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("11") )
                   {
                       //�� MM
                       //if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") || sDONGLE_TYPE.Pos("EC"))
                       //     giX += iAM_ITEM;   //FP+MFP
                       //else
                       //     giY += iAM_ITEM;   //TM

                       if( sDONGLE_TYPE.Pos("TM") )
                            giY += iAM_ITEM;   //TM
                       else
                            giX += iAM_ITEM;   //FP+MFP
                   }
                   *******/

              }
        }
    }

    giU = giU*(-1);
    //temp.sprintf("SumPreSal_INVO_DisAmt:�N���ӫ~������馩(Qt=%d, AM=%d), ��CPN=%d, ��MM_MFP=%d , ��MM_TM=%d",giU, giV, giW, giX, giY   );
    temp.sprintf("SumPreSal_INVO_DisAmt:�N���ӫ~������馩(Qt=%d, AM=%d)",giU, giV   );
    writelog(temp);

    return 0;
}





//�w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��
int __fastcall BASIC::SumPreSal_Amt(int StartLine, int TotalLine)
{
    AnsiString sRType, sType, temp, sFG_SPC, sDONGLE_TYPE;
    AnsiString str_dept,s, sTx_Flg ;

    //temp.sprintf("SumPreSal_INVO_DisAmt:�N���ӫ~������馩  StartLine=%d, TotalLine=%d",StartLine, TotalLine );
    //writelog(temp);

    //�ƶq�B���B
    giU = giK = 0;     //�I���ӫ~     ���|/�K�|
    giG = giH = 0;     //R3R3�I���ӫ~ ���|/�K�|

    giV = giL = 0;     //�w��ӫ~     ���|/�K�|
    giI = giJ = 0;     //R2R3�w��ӫ~ ���|/�K�|

    giW = giM = 0;     //R4�I���ӫ~   ���|/�K�|
    giX = giN = 0;     //R4�w��ӫ~   ���|/�K�|


    //giO = giP = giQ = giR = 0;    //R2R3�K�|


    int iAM_SUB_SUB, iAM_ITEM, iQty;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            sType = _StringSegment_EX(s, "|", 8);
            sFG_SPC = _StringSegment_EX(s, "|", 10);
            sDONGLE_TYPE = _StringSegment_EX(s, "|", 11);
            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);
            iAM_ITEM = _StrToInt(_StringSegment_EX(s, "|", 29));
            iAM_SUB_SUB = _StrToInt(_StringSegment_EX(s, "|", 31));
            sTx_Flg = _StringSegment_EX(s, "|", 35);


            //���`�P��
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {
                   if (sType.Pos("A0") && sFG_SPC.Pos("10") )        //�w��I���ӫ~
                    {
                        if (sTx_Flg == "1")  //���|
                             giU += iAM_ITEM; // - iAM_SUB_SUB:��������B�u��
                        else
                             giK += iAM_ITEM; // - iAM_SUB_SUB:��������B�u��
                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("10") )
                    {
                        if (sTx_Flg == "1")  //���|
                             giU -= iAM_ITEM; // - iAM_SUB_SUB:��������B�u��
                        else
                             giK -= iAM_ITEM; // - iAM_SUB_SUB:��������B�u��
                    }
                   else if (sType.Pos("A0") && sFG_SPC.Pos("9") )        //�w��ӫ~
                    {
                        if (sTx_Flg == "1")  //���|
                            giV += iAM_ITEM;
                        else
                            giL += iAM_ITEM; //�K�|
                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("9") )        //�w��ӫ~
                    {
                        if (sTx_Flg == "1")  //���|
                            giV -= iAM_ITEM;
                        else
                            giL -= iAM_ITEM; //�K�|
                    }


             }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {

                   if (sType.Pos("A0") && sFG_SPC.Pos("10") )        //�w��I���ӫ~
                    {
                         if (sTx_Flg == "1")  //���|
                             giG -= iAM_ITEM;
                         else
                             giH -= iAM_ITEM;
                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("10") )
                    {
                         if (sTx_Flg == "1")  //���|
                             giG += iAM_ITEM;                        //��������B�u��
                         else
                             giH += iAM_ITEM;
                    }
                   else if (sType.Pos("A0") && sFG_SPC.Pos("9") )        //�w��ӫ~
                    {
                         if (sTx_Flg == "1")  //���|
                             giI -= iAM_ITEM;
                         else
                             giJ -= iAM_ITEM;

                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("9") )
                    {
                        if (sTx_Flg == "1")  //���|
                            giI += iAM_ITEM;
                        else
                            giJ += iAM_ITEM;
                    }

            }
            else if ( sRType.Pos("R4")  )    //�d�ȭ��L
            {

                   if (sType.Pos("A0") && sFG_SPC.Pos("10") )        //�w��I���ӫ~
                    {
                        if (sTx_Flg == "1")  //���|
                            giW += iAM_ITEM;                         //��������B�u��
                        else
                            giM += iAM_ITEM;
                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("10") )
                    {
                        if (sTx_Flg == "1")  //���|
                            giW -= iAM_ITEM;                        //��������B�u��
                        else
                            giM -= iAM_ITEM;
                    }
                   else if (sType.Pos("A0") && sFG_SPC.Pos("9") )        //�w��ӫ~
                    {
                        if (sTx_Flg == "1")  //���|
                            giX += iAM_ITEM;
                        else
                            giN += iAM_ITEM;

                    }
                   else if (sType.Pos("A1") && sFG_SPC.Pos("9") )
                    {
                        if (sTx_Flg == "1")  //���|
                            giX -= iAM_ITEM;
                        else
                            giN -= iAM_ITEM;
                    }

              }


        }
    }

    temp.sprintf("SumPreSal_Amt:�w��I���ӫ~     (�I���ӫ~���|=%d, �I���ӫ~�K�|=%d, �w��ӫ~���|=%d,�w��ӫ~�K�|=%d )",giU, giK, giV, giL   );
    writelog(temp);
    temp.sprintf("SumPreSal_Amt:�w��I���ӫ~ R2R3(�I���ӫ~���|=%d, �I���ӫ~�K�|=%d, �w��ӫ~���|=%d,�w��ӫ~�K�|=%d )",giG, giH, giI, giJ   );
    writelog(temp);
    temp.sprintf("SumPreSal_Amt:�w��I���ӫ~   R4(�I���ӫ~���|=%d, �I���ӫ~�K�|=%d, �w��ӫ~���|=%d,�w��ӫ~�K�|=%d )",giW, giM, giX ,giN   );
    writelog(temp);

    //R3R3�I���ӫ~ ���|/�K�|
    giG =  giG *(-1);
    giH =  giH *(-1);

    //R2R3�w��ӫ~ ���|/�K�|
    giI = giI *(-1);
    giJ = giJ *(-1);

    //R4�I���ӫ~   ���|/�K�|
    giW = giW; // * (-1);
    giM = giM; // * (-1);

    //R4�w��ӫ~   ���|/�K�|
    giX = giX; // * (-1);
    giN = giN; // * (-1);



    return 0;
}



////////////////////////////////










//�o�����B�s����
int __fastcall BASIC::TotZero(int StartLine, int TotalLine)
{
    int iCashZero = 0;      //�o�����B��0����
    int i_invo_amt = 0;     //�o�����B
    AnsiString str_type,s;

    unsigned int str_invoice_start = 0, str_invoice_end = 0;

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];

            i_invo_amt = _StrToInt(_StringSegment_EX(s, "|", 32));

            str_invoice_start = _StrToInt(_StringSegment_EX(s, "|", 9).SubString(3,8));
            str_invoice_end   = _StrToInt(_StringSegment_EX(s, "|", 10).SubString(3,8));
            str_type = _StringSegment_EX(s, "|", 8);

            if (str_type.Pos("X"))      //LOG�X�{�������p���B�z
                continue;
            // 2007/09/18 Update Lu
            //if (str_type=="R2" || str_type=="R3" || str_type=="R4")      //LOG�X�{�������p���B�z
            if (str_type=="R2" || str_type=="R3" || str_type.Pos("V"))      //LOG�X�{�������p���B�z
                continue;


            if (!i_invo_amt )
            {
                if (str_invoice_start != 0 && str_invoice_end != 0)
                    ++iCashZero;
            }
        }
    }

    return iCashZero;
}

//�������
int __fastcall BASIC::SumVoid(int StartLine, int TotalLine)
{
    AnsiString str_type,s;    //�P�⫬�A

    //���ơB���B
    giA = giB = 0;

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_type=_StringSegment_EX(s, "|", 8);

            if (str_type.Pos("V"))      //V���������
            {
                ++giA;
                giB += _StrToInt(_StringSegment_EX(s, "|", 32));
            }
        }
    }

    if(  abs(giB) > 9999999 )
       giB=9999999;

    s.sprintf("SumVoid:�������,�X�p(����=%d, ���B=%d) ",giA, giB);
    writelog(s);

    return 0;
}

//97,98
int __fastcall BASIC::WriteSAL(AnsiString data,AnsiString SALFileName)
{
    FILE *f_write_sal;

    writelog("WriteSAL:�g�J�ɮ�: " + SALFileName + "<" + data + ">");

    if ((f_write_sal = fopen(SALFileName.c_str(), "a")) == NULL)
    {
        writelog("WriteSAL:�g�JSAL�ɵo�Ϳ��~ " + SALFileName);
        return 0;
    }


    fprintf(f_write_sal,"%s\n",data.c_str());
    fclose(f_write_sal);

    return OK;
}


int __fastcall BASIC::WriteSAL_A(AnsiString data,AnsiString SALFileName)
{
    FILE *f_write_sal;

    writelog("WriteSAL_A:�g�J�ɮ�: " + SALFileName + "<" + data + ">");

    if ((f_write_sal = fopen(SALFileName.c_str(), "a+")) == NULL)
    {
        writelog("WriteSAL:�g�JSAL�ɵo�Ϳ��~ " + SALFileName);
        return 0;
    }


    fprintf(f_write_sal,"%s\n",data.c_str());
    fclose(f_write_sal);

    return OK;
}




//�� �X�p
int __fastcall BASIC::SumUpdat(int StartLine, int TotalLine)
{

    AnsiString str_sale_type;       //�P�f���A
    AnsiString str_r_type;          //�~���B�h�f���A
    AnsiString str_spc_flag,s;        //������O
    int ifg_spc;

    //���ơB���B
    giA = giB = 0;

    //TStringList *t = new TStringList();
    if (gbl_rec_haveline)
    {

        //for (int i=StartLine; i<=TotalLine; i++)
        //for (int i=0; i<gtsl_rec->Count; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            str_r_type = _StringSegment_EX(s, "|", 38);     // ������A�Ϥ�

            //A1:���`�P�����    B1:�����P�����   M1:Menu�P�����
            //S0:�S�����     S1:�S���������

            if (str_r_type.Pos("X"))
                continue;

            //if (str_sale_type.Pos("1"))
            if (str_sale_type.Pos("A1") || str_sale_type.Pos("B1") )
            {
                str_spc_flag = _StringSegment_EX(s, "|", 10).Trim();   //������O
                ifg_spc=_StrToInt(str_spc_flag);

                //str_spc_flag.Pos("4") || str_spc_flag.Pos("5") ||  str_spc_flag.Pos("6") || str_spc_flag.Pos("9") )
                if ( ifg_spc==4 || ifg_spc==5 || ifg_spc==6 || ifg_spc==9 )
                    continue;

                if(str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
                {
                    ++giA;
                    giB += _StrToInt(_StringSegment_EX(s, "|", 29));
                    //t->Add(gtsl_rec->Strings[i]);
                }
                else if ( str_r_type.Pos("R2") || str_r_type.Pos("R3"))
                {
                    --giA;
                    giB -= _StrToInt(_StringSegment_EX(s, "|", 29));
                    //t->Add(gtsl_rec->Strings[i]);
                }
            }
        }
    }

    //t->SaveToFile("e:\\a.txt");
    //delete t;
    return 0;
}

//�o��
int __fastcall BASIC::SumLOSS(int StartLine, int TotalLine)
{
    AnsiString str_sale_type,str_r_type,s;    //�P�f���A

    //���ơB���B
    giA = giB = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            str_r_type = _StringSegment_EX(s, "|", 38);
            str_sale_type = _StringSegment_EX(s, "|", 8);

            if (str_r_type.Pos("X"))
                continue;

            if (str_sale_type.Pos("I0"))          //���`
            {
                giA += _StrToInt(_StringSegment_EX(s, "|", 26));
                giB += _StrToInt(_StringSegment_EX(s, "|", 29));
            }
            else if (str_sale_type.Pos("I1"))    //���P
            {
                giA -= _StrToInt(_StringSegment_EX(s, "|", 26));
                giB -= _StrToInt(_StringSegment_EX(s, "|", 29));
            }
        }
    }

    return 0;
}

//�p��N�{�� ��d�X�p
int __fastcall BASIC::SumTbCash(int StartLine, int TotalLine)
{
    //§���B��  �馸�B��   �����B��
    giA = giB = giC = giD = giG = giH = giI = giJ = giK = 0;
    giE=giF=giL=giM=0;

    int iT1, iT2, iT3, iT4, iT5, iT6, iT7, iT8, iTT, iT9, iT10, iT11, iT12, iT13,
        iTOt3, iTOt2Rtn, iTOt2RtnOld, iT26, iIPss, iT28, iT52;
    iT1=iT2=iT3=iT4=iT5=iT6=iT7=iT8=iT9=iT10=iTT=iT11=iT12=iT13=iT26=iIPss=iT28=iT52=0;
    iTOt3=iTOt2Rtn=iTOt2RtnOld=0;                 // 2015/02/12 �s�W�����IiT12  2015/12/25 �s�WHappyCash��IiT13

    AnsiString str_sale_type, strtmp,tmpSer, s;       //���b���A

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            iTOt2Rtn=0;
            iTOt2RtnOld=0;

            s=gtsl_tot->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            tmpSer=_StringSegment_EX(s, "|", 7);    //����Ǹ�
            if (str_sale_type.Pos("Z0") || str_sale_type.Pos("V1") )
            {


               if(str_sale_type.Pos("V1"))   // 2012/02/02
                 {
                  //�q�l�����B

                  iT5 = _StrToInt(_StringSegment_EX(s, "|", 16));
                  if (iT5)
                     {
                      ++giI;
                      giJ += iT5;
                      strtmp.sprintf("SumTbCash:��d�X�p:%s-->V1�q�l�����B:+%d, tot=%d", s.c_str(), iT5 , giJ);
                      writelog(strtmp);
                     }

                  // iIPass ����
                  iIPss = _StrToInt(_StringSegment_EX(s, "|", 27));
                  if (iIPss)
                     {
                       ++giI;
                       giJ += iIPss;
                       strtmp.sprintf("SumTbCash:��d�X�p:%s(iIPass V1����:%d, %d)", s.c_str(), iIPss , giJ);
                       writelog(strtmp);

                     }


                  // HappyCash
                  iT13 = _StrToInt(_StringSegment_EX(s, "|", 51));
                  if (iT13)
                     {
                       ++giI;
                       giJ += iT13;
                       strtmp.sprintf("SumTbCash:��d�X�p:%s(iT13 V1HappyCash:%d, %d)", s.c_str(), iT13 , giJ);
                       writelog(strtmp);

                     }



                  //�y�C�d�I�I���  iT28
                  iT28 = _StrToInt(_StringSegment_EX(s, "|", 28));
                  if (iT28)
                     {
                       ++giI;
                       giJ += iT28;
                       strtmp.sprintf("SumTbCash:��d�X�p:%s(iT28 V1�y�C�d�I�I���:%d, %d)", s.c_str(), iT28 , giJ);
                       writelog(strtmp);

                     }

                  continue;
                 }  // end of   if(str_sale_type.Pos("V1"))


               //�{��
               iT6 = _StrToInt(_StringSegment_EX(s, "|", 15));

               //HappyGo���B
                iT11 = _StrToInt(_StringSegment_EX(s, "|", 12));
                if (iT11)
                {
                    ++giI;
                    giJ += iT11;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->HappyGo���B:+%d, tot=%d", s.c_str(), iT11 , giJ);
                    writelog(strtmp);

                }

                //�ӫ~§���I�b�B  �l�B��ץ� 2007/10/01
                iTOt3 = _StrToInt(_StringSegment_EX(s, "|", 13));

               //�y�C�d���B
                iT5 = _StrToInt(_StringSegment_EX(s, "|", 16));
                if (iT5)
                {
                    ++giI;
                    giJ += iT5;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->�y�C�d���B:+%d, tot=%d", s.c_str(), iT5 , giJ);
                    writelog(strtmp);

                }

                // �H�Υd
                iT7 = _StrToInt(_StringSegment_EX(s, "|", 17));
                if (iT7)
                {
                    ++giI;
                    giJ += iT7;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s(�H�Υd���B:%d, %d)", s.c_str(), iT7 , giJ);
                    writelog(strtmp);

                }

                //§��
                iT1 = _StrToInt(_StringSegment_EX(s, "|", 18));
                if (iT1)
                {
                    ++giA;
                    giB += iT1;

                }

                //����� �}�o�������I��
                iT2 = _StrToInt(_StringSegment_EX(s, "|", 19));
                if (iT2)
                {
                    ++giC;
                    giD += iT2;
                }

                //�����  Other1     ���}�o��������I
                iT9 = _StrToInt(_StringSegment_EX(s, "|", 23));
                if (iT9)
                {
                    ++giC;
                    giD += iT9;
                }

                //�������� CPN    �}�o���t�ӧ����I��
                iT3 = _StrToInt(_StringSegment_EX(s, "|", 20));
                if (iT3)
                {
                    ++giC;
                    giD += iT3;
                }

                //���Q�I��
                iT10 = _StrToInt(_StringSegment_EX(s, "|", 22));
                if (iT10)
                {
                    ++giC;
                    giD += iT10;
                }

                //�ӫ~§��,�X�֨�§��
                iT8 = _StrToInt(_StringSegment_EX(s, "|", 24));
                //if (iT8)
                //{
                    ++giA;
                    giB += iT8;
                    //giB += iTOt3;

                    if( (iT8-iTOt3) > 0 ) //�l�B��ץ�  ���� iTo3 �O�ӫ~§��b�B
                      {
                          iTOt2RtnOld=iT8-iTOt3;
                          //giE+=iTOt2Rtn;
                      }

                    iTOt2Rtn=_StrToInt(_StringSegment_EX(s, "|", 14));
                    giE+=iTOt2Rtn;        //�l�B��
                    if( iTOt2Rtn > 0)
                       ++giF;
                //}

                //�q�l§��§���d
                iT26 = _StrToInt(_StringSegment_EX(s, "|", 26));
                if (iT26)
                {
                    ++giL;
                    giM += iT26;
                }


                // iIPass ����
                iIPss = _StrToInt(_StringSegment_EX(s, "|", 27));
                if (iIPss)
                {
                    ++giI;
                    giJ += iIPss;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s(iIPass ����:%d, %d)", s.c_str(), iIPss , giJ);
                    writelog(strtmp);

                }

                //�y�C�d�I�I���  iT28
                iT28 = _StrToInt(_StringSegment_EX(s, "|", 28));
                if (iT28)
                {
                     ++giI;
                     giJ += iT28;
                     strtmp.sprintf("SumTbCash:��d�X�p:%s(iT28 �y�C�d�I�I���:%d, %d)", s.c_str(), iT28 , giJ);
                     writelog(strtmp);
                }

                //�����I���B
                iT12 = _StrToInt(_StringSegment_EX(s, "|", 29));
                if (iT12)
                {
                    ++giI;
                    giJ += iT12;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->�����I���B:+%d, tot=%d", s.c_str(), iT12 , giJ);
                    writelog(strtmp);

                }

                //HappyCash��I���B
                iT13 = _StrToInt(_StringSegment_EX(s, "|", 51));
                if (iT13)
                {
                    ++giI;
                    giJ += iT13;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->HappyCash��I���B:+%d, tot=%d", s.c_str(), iT13 , giJ);
                    writelog(strtmp);

                }

                // FamiPoint  2019/12/25    TM_PG_�s�WFami Point��I�b��
                  iT52 = _StrToInt(_StringSegment_EX(s, "|", 52));    //�ƥ�
                if (iT52)
                {
                       ++giI;
                       giJ += iT52;
                       strtmp.sprintf("SumTbCash:��d�X�p:%s(iT52 FamiPoint :%d, %d)", s.c_str(), iT52 , giJ);
                       writelog(strtmp);

                }


                //TOT_AMT
                iTT = _StrToInt(_StringSegment_EX(s, "|", 31));

                //�������B
                iT4 = _StrToInt(_StringSegment_EX(s, "|", 34));
                if (iT4)
                {
                    ++giG;
                    giH += iT4;
                    //writelog(gtsl_tot->Strings[i]);
                }

                /***
                if( iTT != (iT6+iT5+iT1+iT2+iT3+iT7+iT8+iT9+iT10-iT4-iTOt2Rtn) )
                  {
                   strtmp.sprintf("SumTbCash: ����Ǹ�(%s) �ɶU���� , ��:�P����B=%d, �U:(�{=%d)+(IC=%d)+(�H=%d)+(§=%d)+(�ӫ~§��=%d)+(��=%d)+(CPN=%d)+(OT1=%d)+(���Q=%d)-(��=%d)-(�l�B��=%d)",
                                         tmpSer,iTT, iT6, iT5, iT7, iT1,iT8,iT2,iT3,iT9,iT10,iT4,iTOt2Rtn);
                   writelog(strtmp);
                  }
                ***/
                if( iTT != (iT6+iT5+iT1+iT2+iT3+iT7+iT8+iT9+iT10+iT26+iT11+iT12+iT13+iIPss-iT4-iTOt2Rtn) )
                  {
                   strtmp.sprintf("SumTbCash: ����Ǹ�(%s) �ɶU���� , ��:�P����B=%d, �U:(�{=%d)+(IC=%d)+(�H=%d)+(§=%d)+(�ӫ~§��=%d)+(��=%d)+(CPN=%d)+(OT1=%d)+(���Q=%d)+(HPG=%d)+(�����I=%d)+(HappyCash��I=%d)+(iIPss=%d)-(��=%d)",
                                         tmpSer,iTT, iT6, iT5, iT7, iT1,iT8+iT26,iT2,iT3,iT9,iT10,iT11,iT12,iT13,iIPss, iT4+iTOt2Rtn );
                   writelog(strtmp);
                  }

                //if( iTOt2RtnOld != iTOt2Rtn )
                //  {
                //   strtmp.sprintf("SumTbCash: ����Ǹ�(%s) ��l�B����B���` iTOt2RtnOld=%d ������ iTOt2Rtn=%d ",
                //                         tmpSer,iTOt2RtnOld, iTOt2Rtn);
                //   writelog(strtmp);
                //  }


            }
            else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
            {
               //�{��
               iT6 = _StrToInt(_StringSegment_EX(s, "|", 15));

               //HappyGo���B
               iT11 = _StrToInt(_StringSegment_EX(s, "|", 12));
               if (iT11)
                {
                    --giI;
                    giJ -= iT11;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->HappyGo���B:-%d, tot=%d", s.c_str(), iT11 , giJ);
                    writelog(strtmp);


                }

                //�ӫ~§���I�b�B  �l�B��ץ� 2007/10/01
                iTOt3 = _StrToInt(_StringSegment_EX(s, "|", 13));

                //�y�C�d���B
                iT5 = _StrToInt(_StringSegment_EX(s, "|", 16));
                if (iT5)
                {
                    --giI;
                    giJ -= iT5;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->�y�C�d���B:-%d, tot=%d", s.c_str(), iT5 , giJ);
                    writelog(strtmp);

                }

                // �H�Υd
                iT7 = _StrToInt(_StringSegment_EX(s, "|", 17));
                if (iT7)
                {
                    --giI;
                    giJ -= iT7;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->�H�Υd���B:-%d, tot=%d", s.c_str(), iT7 , giJ);
                    writelog(strtmp);

                }

                //§��
                iT1 = _StrToInt(_StringSegment_EX(s, "|", 18));
                if (iT1)
                {
                    --giA;
                    giB -= iT1;
                }


                //����� �}�o�������I��
                iT2 = _StrToInt(_StringSegment_EX(s, "|", 19));
                if (iT2)
                {
                    --giC;
                    giD -= iT2;
                }

                //�����  Other1
                iT9 = _StrToInt(_StringSegment_EX(s, "|", 23));
                if (iT9)
                {
                    --giC;
                    giD -= iT9;
                }

                 //�������� CPN
                iT3 = _StrToInt(_StringSegment_EX(s, "|", 20));
                if (iT3)
                {
                    --giC;
                    giD -= iT3;

                }

                //���Q�I��
                iT10 = _StrToInt(_StringSegment_EX(s, "|", 22));
                if (iT10)
                {
                    --giC;
                    giD -= iT10;
                }

                //�ӫ~§��,�X�֨�§��
                iT8 = _StrToInt(_StringSegment_EX(s, "|", 24));
                //if (iT8)
                //{
                    --giA;
                    giB -= iT8;
                    //giB -= iTOt3;

                    if( (iT8-iTOt3) > 0 )  //�l�B��ץ�  ���� iTo3 �O�ӫ~§��b�B
                      {
                          iTOt2RtnOld=iT8-iTOt3;
                    //      giE-=iTOt2Rtn;
                      }
                   iTOt2Rtn=_StrToInt(_StringSegment_EX(s, "|", 14));
                   giE-=iTOt2Rtn;        // �l�B��
                   if( iTOt2Rtn > 0)
                       ++giF;

                //}


                //�q�l§��§���d
                iT26 = _StrToInt(_StringSegment_EX(s, "|", 26));
                if (iT26)
                {
                    --giL;
                    giM -= iT26;
                }


                 // iIPass ����
                iIPss = _StrToInt(_StringSegment_EX(s, "|", 27));
                if (iIPss)
                {
                    --giI;
                    giJ -= iIPss;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->iIPass ����:-%d, tot=%d", s.c_str(), iIPss , giJ);
                    writelog(strtmp);

                }

                //�y�C�d�I�I���  iT28
                iT28 = _StrToInt(_StringSegment_EX(s, "|", 28));
                if (iT28)
                {
                     --giI;
                     giJ -= iT28;
                     strtmp.sprintf("SumTbCash:��d�X�p:%s-->iT28 �y�C�d�I�I���:-%d, tot=%d", s.c_str(), iT28 , giJ);
                     writelog(strtmp);
                }

                //�����I���B
                iT12 = _StrToInt(_StringSegment_EX(s, "|", 29));
                if (iT12)
                {
                    --giI;
                    giJ -= iT12;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->�����I���B:-%d, tot=%d", s.c_str(), iT12 , giJ);
                    writelog(strtmp);

                }

                //HappyCash��I���B
                iT13 = _StrToInt(_StringSegment_EX(s, "|", 51));
                if (iT13)
                {
                    --giI;
                    giJ -= iT13;
                    strtmp.sprintf("SumTbCash:��d�X�p:%s-->HapyCash��I���B:-%d, tot=%d", s.c_str(), iT13 , giJ);
                    writelog(strtmp);

                }

                // FamiPoint  2019/12/25    TM_PG_�s�WFami Point��I�b��
                iT52 = _StrToInt(_StringSegment_EX(s, "|", 52));    //�ƥ�
                if (iT52)
                     {
                       --giI;
                       giJ -= iT52;
                       strtmp.sprintf("SumTbCash:��d�X�p:%s(iT52 FamiPoint :%d, %d)", s.c_str(), iT52 , giJ);
                       writelog(strtmp);

                     }


                //TOT_AMT
                iTT = _StrToInt(_StringSegment_EX(s, "|", 31));

                //�������B
                iT4 = _StrToInt(_StringSegment_EX(s, "|", 34));

                if (iT4)
                {
                    --giG;
                    giH -= iT4;
                    //writelog(gtsl_tot->Strings[i]);
                }

                //if( iTT != (iT6+iT5+iT1+iT2+iT3+iT7+iT8+iT9+iT10-iT4-iTOt2Rtn) )
                //  {
                //   strtmp.sprintf("SumTbCash: ����Ǹ�(%s) R�ɶU���� , ��:�P����B=%d, �U:(�{=%d)+(IC=%d)+(�H=%d)+(§=%d)+(�ӫ~§��=%d)+(��=%d)+(CPN=%d)+(OT1=%d)+(���Q=%d)-(��=%d)-(�l�B��=%d)",
                //                         tmpSer,iTT, iT6, iT5, iT7, iT1,iT8,iT2,iT3,iT9,iT10,iT4,iTOt2Rtn);
                //   writelog(strtmp);
                //  }

                if( iTT != (iT6+iT5+iT1+iT2+iT3+iT7+iT8+iT9+iT10+iT26+iT11+iT12+iT13+iIPss-iT4-iTOt2Rtn) )
                  {
                   strtmp.sprintf("SumTbCash: ����Ǹ�(%s) R�ɶU���� , ��:�P����B=%d, �U:(�{=%d)+(IC=%d)+(�H=%d)+(§=%d)+(�ӫ~§��=%d)+(��=%d)+(CPN=%d)+(OT1=%d)+(���Q=%d)+(HPG=%d)+(�����I=%d)+(HappyCash��I=%d)+(iIPss=%d)-(��=%d)",
                                         tmpSer,iTT, iT6, iT5, iT7, iT1,iT8+iT26,iT2,iT3,iT9,iT10,iT11,iT12,iT13,iIPss, iT4+iTOt2Rtn);
                   writelog(strtmp);
                  }

               // if( iTOt2RtnOld != iTOt2Rtn )
               //   {
               //    strtmp.sprintf("SumTbCash: ����Ǹ�(%s) ��l�B����B���` iTOt2RtnOld=%d ������ iTOt2Rtn=%d ",
               //                          tmpSer,iTOt2RtnOld, iTOt2Rtn);
               //    writelog(strtmp);
               //   }


            }
        }
    }

    return 0;
}

//�p��������f��
int __fastcall BASIC::SumBill78(int StartLine, int TotalLine)
{


    int iTCnt, iTAmt, iT4, iPayType,iOtCnt,iOtAmt;

    AnsiString str_sale_type,stmp,s;       //���b���A

    iTCnt = iTAmt = iT4 = iOtCnt = iOtAmt = 0;

    Cpn71_cnt=0;                  //�����71����
    Cpn71_amt=0;                  //�����71���B
    Cpn72_cnt=0;                  //�����72����
    Cpn72_amt=0;                  //�����72���B
    Cpn73_cnt=0;                  //�����73����
    Cpn73_amt=0;                  //�����73���B
    Cpn74_cnt=0;                  //�����74����
    Cpn74_amt=0;                  //�����74���B
    Cpn75_cnt=0;                  //�����75����
    Cpn75_amt=0;                  //�����75���B
    Cpn76_cnt=0;                  //�����76����
    Cpn76_amt=0;                  //�����76���B

    Cpn77_cnt=0;                  //�������f��77����
    Cpn77_amt=0;                  //�������f��77���B
    Cpn78_cnt=0;                  //�������f��78����
    Cpn78_amt=0;                  //�������f��78���B

   // writelog("�R���ɮ� " + str_tmp_ini);
    if (gbl_bil_haveline)
    {
       // writelog("SumBill78: Have Bil Data" );
       // for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_bil->Count; i++)
        {
            s=gtsl_bil->Strings[i];;
            str_sale_type = _StringSegment_EX(s, "|", 8);
            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"��I���O");
            iTAmt = _StrToInt(_StringSegment_EX(s, "|", 13), "��I���B");
           // if (iTAmt)
               iTCnt=1;
          //  writelog("SumBill78: Bil Data str_sale_type,PayType ,"+ str_sale_type + stmp);
            if (str_sale_type.Pos("Z0"))
            {
                 switch(iPayType)
                       {
                         case 71:
                               Cpn71_cnt+=iTCnt;                  //�����71����
                               Cpn71_amt+=iTAmt;                  //�����71���B
                              break;
                         case 72:
                               Cpn72_cnt+=iTCnt;                  //�����71����
                               Cpn72_amt+=iTAmt;                  //�����71���B
                              break;
                         case 73:
                               Cpn73_cnt+=iTCnt;                  //�����71����
                               Cpn73_amt+=iTAmt;                  //�����71���B
                              break;
                         case 74:
                               Cpn74_cnt+=iTCnt;                  //�����71����
                               Cpn74_amt+=iTAmt;                  //�����71���B
                              break;
                         case 75:
                               Cpn75_cnt+=iTCnt;                  //�����71����
                               Cpn75_amt+=iTAmt;                  //�����71���B
                              break;
                         case 76:
                               Cpn76_cnt+=iTCnt;;                  //�����71����
                               Cpn76_amt+=iTAmt;                  //�����71���B
                              break;
                         case 77:
                               Cpn77_cnt+=iTCnt;                  //�����71����
                               Cpn77_amt+=iTAmt;                  //�����71���B
                              break;
                         case 78:
                               Cpn78_cnt+=iTCnt;                  //�����71����
                               Cpn78_amt+=iTAmt;                  //�����71���B
                              break;
                         default:
                              iOtCnt+=iTCnt;;
                              iOtAmt+=iTAmt;

                       }     // END OF SWITCH
              } // end of  if (str_sale_type.Pos("Z0"))

            else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
            {
                 switch(iPayType)
                       {
                         case 71:
                               Cpn71_cnt-=iTCnt;                  //�����71����
                               Cpn71_amt-=iTAmt;                  //�����71���B
                              break;
                         case 72:
                               Cpn72_cnt-=iTCnt;                  //�����71����
                               Cpn72_amt-=iTAmt;                  //�����71���B
                              break;
                         case 73:
                               Cpn73_cnt-=iTCnt;                  //�����71����
                               Cpn73_amt-=iTAmt;                  //�����71���B
                              break;
                         case 74:
                               Cpn74_cnt-=iTCnt;                  //�����71����
                               Cpn74_amt-=iTAmt;                  //�����71���B
                              break;
                         case 75:
                               Cpn75_cnt-=iTCnt;                  //�����71����
                               Cpn75_amt-=iTAmt;                  //�����71���B
                              break;
                         case 76:
                               Cpn76_cnt-=iTCnt;;                  //�����71����
                               Cpn76_amt-=iTAmt;                  //�����71���B
                              break;
                         case 77:
                               Cpn77_cnt-=iTCnt;                  //�����71����
                               Cpn77_amt-=iTAmt;                  //�����71���B
                              break;
                         case 78:
                               Cpn78_cnt-=iTCnt;                  //�����71����
                               Cpn78_amt-=iTAmt;                  //�����71���B
                              break;
                         default:
                              iOtCnt-=iTCnt;;
                              iOtAmt-=iTAmt;

                       }     // END OF SWITCH
            }   // end of  else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)


    StrBillPay.sprintf("%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       Cpn71_cnt,Cpn71_amt,Cpn72_cnt,Cpn72_amt,Cpn73_cnt,Cpn73_amt,Cpn74_cnt,Cpn74_amt,
                       Cpn75_cnt,Cpn75_amt,Cpn76_cnt,Cpn76_amt,Cpn77_cnt,Cpn77_amt,Cpn78_cnt,Cpn78_amt   );


    return 0;
}



//�N���B��B�I ,�I�d����OgiD.�@�ogiE For TM3.0    2003/02/20 , 2013/10/17

int __fastcall BASIC::SumKamt(int StartLine, int TotalLine)
{
     writelog("�N���B��B�I ���p���B For TM3.0 ");
    //���B��B�I
    giA=giB=giC=giD=giE=giF=giG=giH=0;
    gi_mix = 0;                 //�V�n����    �N���ӫ~�ȼ�

    bool bl_mix = false;        //�O�_�V�n
    int i_invo_cnt = 0;         //�P��ƶq����0

    int iT1, iT2, iT3,kcolx_amt,kcolf_amt, iT4, iT5, iT6, iT7;
    iT1 = iT2 = iT3 = kcolx_amt=kcolf_amt=iT4=iT5=iT6=iT7=0;

    AnsiString str_sale_type, temp, s;   //�P�f���A

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {

            s=gtsl_tot->Strings[i];
            str_sale_type=_StringSegment_EX(s, "|", 8);

            i_invo_cnt = _StrToInt(_StringSegment_EX(s, "|", 30));
            //str_sale_type = gtsl_tot->Strings[i].SubString(67,2);
            //kcolx_amt=_StrToInt(gtsl_tot->Strings[i].SubString(345,8)); //�N�����|
            //kcolf_amt=_StrToInt(gtsl_tot->Strings[i].SubString(354,8)); //�N���K�|
             kcolx_amt=_StrToInt(_StringSegment_EX(s, "|", 41)); //�N��

            if ( str_sale_type.Pos("Z0") )
            {
                //�N��
                iT1 = _StrToInt(_StringSegment_EX(s, "|", 42));
                if (iT1)
                {
                    giA += iT1;
                }

                //�N�I
                iT2 = _StrToInt(_StringSegment_EX(s, "|", 43));
                if (iT2)
                {
                    giB += iT2;
                }

                //�N��
                iT3 =  kcolx_amt+kcolf_amt ;
                if (iT3)
                {
                    giC += iT3;
                    bl_mix = true;      //���N��
                }

                //�I�d�N�����O���|:giD    Lu Update 2015/0819
                iT4 = iT6 = 0;
                iT4 = _StrToInt(_StringSegment_EX(s, "|", 49));
                if (iT4)
                {
                    giD += iT4;
                    //giF++;     //�I�d����O�ӫȼ�
                }
                // �I�d�N�����O�K�|:giG   Lu Update 2015/0819
                iT6 = _StrToInt(_StringSegment_EX(s, "|", 50));
                if (iT6)
                {
                    giG += iT6;
                }

                if (iT4>0 || iT6>0)
                    giF++;     //�I�d����O�ӫȼ�



                //�V�n
                if (bl_mix)             //�p��N���V�n����
                {
                    if (i_invo_cnt)     //�P��ƶq����0�A�]���N����V�n����
                        ++gi_mix;       //�N���ӫ~�ȼ�

                    bl_mix = false;
                }
            }
            else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
            {
                //�N��
                iT1 = _StrToInt(_StringSegment_EX(s, "|", 42));
                if (iT1)
                {
                    giA -= iT1;
                }

                //�N�I
                iT2 = _StrToInt(_StringSegment_EX(s, "|", 43));
                if (iT2)
                {
                    giB -= iT2;
                }

                //�N��
                iT3 = kcolx_amt+kcolf_amt;
                if (iT3)
                {
                    giC -= iT3;
                }

                //�I�d�N�����O���|�@�o giE   Lu Update 2015/0819
                iT5 = _StrToInt(_StringSegment_EX(s, "|", 49));
                if (iT5)
                {
                    giE -= iT5;
                }

                 //�I�d�N�����O�K�|�@�o giH  Lu Update 2015/0819
                iT7 = _StrToInt(_StringSegment_EX(s, "|", 50));
                if (iT7)
                {
                    giH -= iT7;
                }


                //���� �V�n���i�H���h�f ����
            }
          else if ( str_sale_type.Pos("R4") )
            {
                //�I�d�N�����O���|�@�o giE   Lu Update 2015/0819
                iT5 = _StrToInt(_StringSegment_EX(s, "|", 49));
                if (iT5)
                {
                    giD += iT5;
                    giE -= iT5;
                }

                //�I�d�N�����O�K�|�@�o giH   Lu Update 2015/0819
                iT7 = _StrToInt(_StringSegment_EX(s, "|", 50));
                if (iT7)
                {
                    giG += iT7;
                    giH -= iT7;
                }

            }

        }   // end of for
    }

    giE=giE*(-1);
    giH=giH*(-1);
    temp.sprintf("SumKamt:1050->(�N��:41,�N��:42,�N�I:43) �N���X�p���B(%d,%d), �N��X�p���B(%d), �N�I�X�p���B(%d), �I�d�N�����O���|(%d),�I�d�N�����O�K�|(%d),�I�d�N�����O���|�@�o(%d),�I�d�N�����O�K�|�@�o(%d)",
                                       kcolx_amt, kcolf_amt, giA, giB, giD, giG, giE, giH);
    writelog(temp);

    gi_mix=0;       //�N���ӫ~�ȼ�    2005/06/17
    return 0;
}



// ���d���X�p
int __fastcall BASIC::SumHealthAmt(int StartLine, int TotalLine)
{

    giA = giB = giC = 0;

    int iHealthCnt, iHealthAmt;
    AnsiString temp;

    iHealthCnt=iHealthAmt =0;

    AnsiString str_sale_type;   //�P�f���A

    return 0;

    if (gbl_tot_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_tot->Count; i++)
        {
            iHealthCnt = _StrToInt(gtsl_tot->Strings[i].SubString(273,8));
            str_sale_type = gtsl_tot->Strings[i].SubString(67,2);
            if ( str_sale_type.Pos("Z0") )
              {
                if (iHealthCnt>0)
                {
                    giA ++;
                    giB += iHealthCnt;
                }

            }
            else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
            {
                if (iHealthCnt>0)
                {
                    giA --;
                    giB -= iHealthCnt;
                }

                //���� �V�n���i�H���h�f ����

            }
        }
    }
    temp.sprintf("SumHealthamt:0050->(273,8)���d���X�p���B (%d,%d)",giA, giB);
    writelog(temp);

    return 0;
}


//�����b  2005/10/01
void __fastcall BASIC::SumAllDept(DEPT *dept)
{

    AnsiString str_r_type, str_sale_type,s, slog, sUNIT;   //�~���B�h�f�A���`�P�f
    int iDept , iItemAmt, iDisSubAmt, iSubSubAmt,iItemQty, iNetItemAmt,
         ifoodSubSubAmt, iDisSubAmt_SPC, iSubSubAmt_SPC  ;

    AnsiString str_dept, str_spc;

    iDept=iItemAmt=iDisSubAmt=iSubSubAmt=iItemQty=iNetItemAmt=ifoodSubSubAmt=0;

    slog.sprintf("�����b : gbl_rec_haveline=%d", gi_rec_line);
    writelog(slog);
    /**
    for (int i=0; i<20; i++)
        {
          dept[i].am_mmDis = 0;         // �����P�P�馩���B        Lu 2018/06/13
          dept[i].am_mmDismiss = 0;     // �����P�P�馩�~�h���B    Lu 2018/06/13
        }
    **/
    if (gbl_rec_haveline)
    {

        for (int i=0; i<20; i++)
        {
          dept[i].am_mmDis = 0;         // �����P�P�馩���B        Lu 2018/06/13
          dept[i].am_mmDismiss = 0;     // �����P�P�馩�~�h���B    Lu 2018/06/13
        }


        //for (int i=0; i<=gi_rec_line; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            //slog.sprintf("�����b :%d(%s)",i, s);
            //writelog(slog);
            //�P�O���ӳ���
            // 2020/11/03 Update by Lu
            sUNIT=Trim(_StringSegment_EX(s, "|", 27));
            if( sUNIT.UpperCase()=="VS" )
              continue;

            str_dept = Trim(_StringSegment_EX(s, "|", 20).SubString(1,2));

            if ( _StrToInt(str_dept)>0)  //            >.Length() > 0)
            {
                iDept = _StrToInt(str_dept);
                --iDept;    //�}�C��0�}�l
            }
            else
                iDept = 12; //��20�����A�}�C��0�}�l

            if( iDept > 19 )
                iDept=12;

            str_r_type = _StringSegment_EX(s, "|", 38).SubString(1,2);
            str_sale_type = _StringSegment_EX(s, "|", 8);
            str_spc = _StringSegment_EX(s, "|", 10);

            if (str_sale_type.Pos("S") && str_spc.Pos("11") )    // ��MM
               { ;; }
            else if (str_sale_type.Pos("S") || str_sale_type.Pos("I") || str_sale_type.Pos("C") || str_sale_type.Pos("L") )
               {
                 //�N��I,�o���k���� , L �~��� 2020/08/31
                 continue;
                }

            if ( str_sale_type.Pos("A") && str_spc.Pos("9") )     //�w��ӫ~
                continue;

            //if ( str_sale_type.Pos("A") && str_spc.Pos("10") )     //�w��I���ӫ~
            //    continue;

            //A0:���`�P��       B0:�����P��   M0:Menu�P��
            //I0:�o��    I1:�o�����
            //A1:���`�P�����    B1:�����P�����   M1:Menu�P�����
            //S0:�S�����     S1:�S���������

            iItemQty=_StrToInt(_StringSegment_EX(s, "|", 26));
            iItemAmt=_StrToInt(_StringSegment_EX(s, "|", 29));
            iDisSubAmt=_StrToInt(_StringSegment_EX(s, "|", 30));      //MM  �馩 �u�D
            iSubSubAmt=_StrToInt(_StringSegment_EX(s, "|", 31));      //�p�p�馩 �u�D
            ifoodSubSubAmt=_StrToInt(_StringSegment_EX(s, "|", 32));  //������馩 �u�D


            /**********************   �w��S�O�B�z  **********************/
            iDisSubAmt_SPC =  iDisSubAmt;     //iDisSubAmt_SPC �M�γ����P�P�馩���B        Lu 2018/06/13
            iSubSubAmt_SPC =  iSubSubAmt;     //iSubSubAmt_SPC �M�γ����P�P�馩���B        Lu 2018/06/13

            if (str_sale_type.Pos("S") && str_spc.Pos("11") )    // ��MM
                {
                  iItemQty=0;
                  iDisSubAmt=0;
                  iSubSubAmt=0;
                  iItemAmt=iItemAmt*(-1);
                }

            if ( str_sale_type.Pos("A") && str_spc.Pos("10") )     //�w��I���ӫ~ ������������
               {
                 //    iSubSubAmt=0;                               // 2017/07/10 �n���������� ,�ҥH Mark
                 iDisSubAmt=0;                                     // 2017/09/21 �]�w���� ��MM, �ҥH�L���A��  MM �馩 �u�D , �]��MM=MM �馩
                 iSubSubAmt_SPC=0;                                  //�n����������   Lu 2018/06/13
               }
           /*************************************************************/

            iNetItemAmt=iItemAmt-iDisSubAmt-iSubSubAmt-ifoodSubSubAmt;

            if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )
            {
                if (str_sale_type.Pos("0"))             //���V
                {
                    dept[iDept].am_dpsitm += iItemQty;
                    dept[iDept].am_dpsale += iNetItemAmt;

                    dept[iDept].am_mmDis += (iDisSubAmt_SPC+iSubSubAmt_SPC);    // �����P�P�馩���B        Lu 2018/06/13
                }
                else if (str_sale_type.Pos("1"))        //�t�V
                {
                    dept[iDept].am_dpsitm -= iItemQty;
                    dept[iDept].am_dpsale -= iNetItemAmt;

                    dept[iDept].am_mmDis -= (iDisSubAmt_SPC+iSubSubAmt_SPC);    // �����P�P�馩���B        Lu 2018/06/13
                }
            }
            else if (str_r_type == "R2" || str_r_type == "R3" )
            {

                //�����~�h���ơB���B
                if ( str_sale_type.Pos("0") )             //���V
                {
                    dept[iDept].am_dpmitm += iItemQty;
                    dept[iDept].am_dpmiss += iNetItemAmt;

                    dept[iDept].am_mmDismiss += (iDisSubAmt_SPC+iSubSubAmt_SPC);    // �����P�P�馩�~�h���B        Lu 2018/06/13
                }
                else if ( str_sale_type.Pos("1") )        //�t�V
                {
                    dept[iDept].am_dpmitm -= iItemQty;
                    dept[iDept].am_dpmiss -= iNetItemAmt;

                    dept[iDept].am_mmDismiss -= (iDisSubAmt_SPC+iSubSubAmt_SPC);    // �����P�P�馩�~�h���B        Lu 2018/06/13
                }

            }
            else if (str_r_type.Pos("R4"))            //�d�ȭ��L
            {
                if ( str_sale_type.Pos("0") )           //���V
                {
                    dept[iDept].am_dpsitm += iItemQty;
                    dept[iDept].am_dpsale += iNetItemAmt;

                    dept[iDept].am_dpmitm += iItemQty;
                    dept[iDept].am_dpmiss += iNetItemAmt;
                }
                else if ( str_sale_type.Pos("1") )      //�t�V
                {
                    dept[iDept].am_dpsitm -= iItemQty;
                    dept[iDept].am_dpsale -= iNetItemAmt;

                    dept[iDept].am_dpmitm -= iItemQty;
                    dept[iDept].am_dpmiss -= iNetItemAmt;
                }
            }

        }
    }


    /*****   2005/10/01 Mark   �馩���[�J����13 14  , 2009/04/28 �l�[  0011 H0  */
    /***************/
    int int_sale_price = 0;         //���|���B
    int int_sub_price = 0;          //�K�|���B

    int int_sale_times = 0;         //���|����
    int int_sub_times = 0;          //�K�|����
    int int_acc_amt;


    if (gbl_pay_haveline)      // 1011 ����
    {
        //for (int i=0; i<=gi_pay_line; i++)
        for (int i=0; i<gtsl_pay_sub->Count; i++)
        {
            s=gtsl_pay_sub->Strings[i];

            //slog.sprintf("�����b H0:%d(%s)",i, s);
            //writelog(slog);

            str_r_type = _StringSegment_EX(s, "|", 22).SubString(1,2);
            str_sale_type = _StringSegment_EX(s, "|", 8);

            int_sale_price = _StrToInt(_StringSegment_EX(s, "|", 17));   //�馩/�����B
            int_sub_price  = 0;

            //int_sale_price = _StrToInt(_StringSegment_EX(s, "|", 18));   //���|���B
            //int_sub_price = _StrToInt(_StringSegment_EX(s, "|", 19));    //�K�|���B

            //if (int_sale_price)
                int_sale_times = _StrToInt(_StringSegment_EX(s, "|", 16));   //����

            //if (int_sub_price)
            //    int_sub_times = _StrToInt(_StringSegment_EX(s, "|", 16));    //����

            if( !str_sale_type.Pos("H") )  // H0 || H1
               continue ;

            //12:�w�ʧ����ӫ~�f��
            str_dept = Trim(_StringSegment_EX(s, "|", 14).SubString(1,2));   //�馩����

            //if (str_dept.Length() > 0)
            //{
            //    iDept = _StrToInt(str_dept, "1011 ����");
            //    --iDept;    //�}�C��0�}�l
            //}
            //else
            //    iDept = 19; //��20�����A�}�C��0�}�l

            if( iDept > 19 )
                iDept=19;

            iDept=12; // 13����;


            if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )        //���`�P��
            {
                if ( str_sale_type.Pos("0") )           //���V
                {
                    dept[iDept].am_dpsitm += int_sale_times;
                    dept[iDept].am_dpsale -= int_sale_price;   //���|���B

                    //dept[iDept].am_dpsitm += int_sub_times;
                    //dept[iDept].am_dpsale -= int_sub_price;    //�K�|���B
                    int_acc_amt -= int_sub_price;
                }
                else if ( str_sale_type.Pos("1") )      //�t�V
                {
                    dept[iDept].am_dpsitm -= int_sale_times;
                    dept[iDept].am_dpsale += int_sale_price;

                    //dept[iDept].am_dpsitm -= int_sub_times;
                    //dept[iDept].am_dpsale += int_sub_price;
                    int_acc_amt += int_sub_price;
                }
            }
            else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))  //�~�h
            {
                if ( str_sale_type.Pos("0") )           //���V
                {
                    dept[iDept].am_dpmitm += int_sale_times;
                    dept[iDept].am_dpmiss -= int_sale_price;

                    //dept[iDept].am_dpmitm += int_sub_times;
                    //dept[iDept].am_dpmiss -= int_sub_price;
                    int_acc_amt -= int_sub_price;
                }
                else if ( str_sale_type.Pos("1") )      //�t�V
                {
                    dept[iDept].am_dpmitm -= int_sale_times;
                    dept[iDept].am_dpmiss += int_sale_price;

                    //dept[iDept].am_dpmitm -= int_sub_times;
                    //dept[iDept].am_dpmiss += int_sub_price;
                    int_acc_amt += int_sub_price;
                }
            }


            else if (str_r_type.Pos("R4"))          //�o�����L
            {
                if ( str_sale_type.Pos("0") )           //���V
                {
                    dept[iDept].am_dpsitm -= int_sale_times;
                    dept[iDept].am_dpsale -= int_sale_price;

                    //dept[iDept].am_dpsitm -= int_sub_times;
                    //dept[iDept].am_dpsale -= int_sub_price;

                    //

                    dept[iDept].am_dpmitm -= int_sale_times;
                    dept[iDept].am_dpmiss -= int_sale_price;

                    //dept[iDept].am_dpmitm -= int_sub_times;
                    //dept[iDept].am_dpmiss -= int_sub_price;
                }
                else if ( str_sale_type.Pos("1") )      //�t�V
                {
                    dept[iDept].am_dpsitm += int_sale_times;
                    dept[iDept].am_dpsale += int_sale_price;

                    //dept[iDept].am_dpsitm += int_sub_times;
                    //dept[iDept].am_dpsale += int_sub_price;

                    //

                    dept[iDept].am_dpmitm += int_sale_times;
                    dept[iDept].am_dpmiss += int_sale_price;

                    //dept[iDept].am_dpmitm += int_sub_times;
                    //dept[iDept].am_dpmiss += int_sub_price;
                }

            }

            int_sale_price = 0;
            int_sub_price = 0;
        }

        slog.sprintf("�����b H0,13���� �w�ʧ���� (���B:%d) .",int_acc_amt);
        writelog(slog);

    }

   /***************/
   /**********************  2009/04/29 **********/

   
    //20040112�������f�� ��13����
    dept[12].am_dpsitm += pay_bill_cnt;    //���V
    dept[12].am_dpsale -= pay_bill_amt;
    dept[12].am_dpmitm += mpay_bill_cnt;   //�t�V
    dept[12].am_dpmiss += mpay_bill_amt;


    slog.sprintf("�����b END,13���� �������f��(�ƶq:%d, ���B:%d) (�ƶq:%d, ���B:%d), �`:���A�B�z 1011 ���.",
                  pay_bill_cnt, pay_bill_amt, mpay_bill_cnt, mpay_bill_amt);
    writelog(slog);


}


// �Y�ɳs�u�ӫ~���p
void __fastcall BASIC::INQSubTotal(INQTBL *inqtbl)
{

    AnsiString str_r_type, str_sale_type,s;   //�~���B�h�f�A���`�P�f
    int iDept = 0;                          //�Y�ɳs�u�Ϥ�

    AnsiString str_dept;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<=gi_rec_line; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            //�P�O���ӧY�ɳs�u�Ϥ�
            s=gtsl_rec->Strings[i];
            //Trim(gtsl_rec->Strings[i].SubString(334,4)); //�Y�ɳs�u�Ϥ� ,�c�ƶq.
            str_dept = Trim(_StringSegment_EX(s, "|", 36).SubString(1,2)); //�Y�ɳs�u�Ϥ� ,�c�ƶq.
            if (str_dept.Length() > 0)
            {
                iDept = _StrToInt(str_dept);
                //--iDept;    //�}�C��0�}�l
            }
            else
                iDept = 19; //��19�Y�ɳs�u�Ϥ��A�}�C��0�}�l

            if ( iDept == 0)     //�D�Y�ɳs�u�ӫ~���p�� 
                continue;    

            str_r_type = _StringSegment_EX(s, "|", 38).SubString(1,2);
            str_sale_type = _StringSegment_EX(s, "|", 8);

            // if (str_sale_type.Pos("S") || str_sale_type.Pos("I"))     //�N��I,�o���k����
            //    continue;
            if ( str_sale_type.Pos("I"))     //�o�󤣭p�� 
                continue;    

            //A0:���`�P��       B0:�����P��   M0:Menu�P��
            //I0:�o��    I1:�o�����
            //A1:���`�P�����    B1:�����P�����   M1:Menu�P�����
            //S0:�S�����     S1:�S���������

            if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
            {
                if (str_sale_type.Pos("0"))             //���V
                {
                    inqtbl[iDept].qt_InqSale += _StrToInt(_StringSegment_EX(s, "|", 26));
                    inqtbl[iDept].am_InqSale += _StrToInt(_StringSegment_EX(s, "|", 29));
                }
                else if (str_sale_type.Pos("1"))        //�t�V
                {
                    inqtbl[iDept].qt_InqSale -= _StrToInt(_StringSegment_EX(s, "|", 26));
                    inqtbl[iDept].am_InqSale -= _StrToInt(_StringSegment_EX(s, "|", 29));
                }
            }
            else if (str_r_type == "R2" || str_r_type == "R3" )
            {

                //�Y�ɳs�u�Ϥ��~�h�ƶq�B���B
                if ( str_sale_type.Pos("0") )             //�t�V
                {
                    inqtbl[iDept].qt_InqSale -= _StrToInt(_StringSegment_EX(s, "|", 26));
                    inqtbl[iDept].am_InqSale -= _StrToInt(_StringSegment_EX(s, "|", 29));
                }
                else if ( str_sale_type.Pos("1") )       //���V 
                {
                    inqtbl[iDept].qt_InqSale += _StrToInt(_StringSegment_EX(s, "|", 26));
                    inqtbl[iDept].am_InqSale += _StrToInt(_StringSegment_EX(s, "|", 29));
                }

            }
            else if (str_r_type.Pos("R4"))
            {
                if ( str_sale_type.Pos("0") )           //���V
                {
                    ;;
                }
                else if ( str_sale_type.Pos("1") )      //�t�V
                {
                    ;;
                }
            }

        }  // end of for
    }  // end of if (gbl_rec_haveline)
}



void __fastcall BASIC::init_data()
{

    //TIniFile *tifAbacus;
    TIniFile *tif_ecr;

    //AnsiString str_ten_code, str_ecr_no, str_cashier, str_x_trans;

    int int_ten_code, int_ecr_no, int_cashier,i,j;
    AnsiString str_x_trans, stmp,OrgStoreNo;


    stmp.sprintf("%sXZDATA\\",WORKDIR);
    if (!DirectoryExists(stmp))
       {
        // writelog("�إ߾��v�ؿ� :" + stmp);
        ForceDirectories(stmp);
       }


    try
    {
        //tifAbacus = new TIniFile(".\\ABACUS.INI");
        tif_ecr   = new TIniFile(XZDATA_INI);

        int_ten_code = tif_ecr->ReadInteger("ECR", "StoreNO", 9913);
        int_ecr_no   = tif_ecr->ReadInteger("ECR", "EcrNO"  , 1);

        int_cashier  = tif_ecr->ReadInteger("ECR", "SalesNo" , 99999);
        str_x_trans  = tif_ecr->ReadString("X"  , "ptran_cnt", "00000000");

        OrgStoreNo = tif_ecr->ReadString("ECR","OrgStoreNO","");     //��l����
        tif_ecr->WriteString("ECR","OrgStoreNO",OrgStoreNo);


        sprintf(gchar_tencode,"%06d", int_ten_code);
        sprintf(gchar_ecr_no, "%1d",  int_ecr_no);
        sprintf(gchar_cashier,"%05d", int_cashier);
        sprintf(gchar_orgtencode,"%s",OrgStoreNo.c_str());      //��l����


        gi_open_x = tif_ecr->ReadInteger("ECR", "tot_open_x", 0);
        gi_open_z = tif_ecr->ReadInteger("ECR", "tot_open_z", 0);

        sprintf(gchar_date_time, "%14s", FormatDateTime("yyyymmddhhnnss", Now()).c_str());


        //���ȭ��b����Ǹ����   2012/06/04

        if( str_x_trans.Length() > 5)
           stmp=str_x_trans.SubString(str_x_trans.Length()-4,5);
        else
           stmp=str_x_trans;

        sprintf(gchar_x_trans, "%5.5s", stmp.c_str());
        //sprintf(gchar_x_trans, "%5s", str_x_trans.SubString(4,5).c_str());

        //
        if (tif_ecr->SectionExists("PRINT") == false)
            tif_ecr->WriteBool("PRINT","EPSON",false);

        int int_epson = tif_ecr->ReadString("PRINT","EPSON","").Length();

        if (int_epson != 1)
            tif_ecr->WriteBool("PRINT","EPSON",false);

        for( i=0 ; i< 20; i++)     //INIT �N��I�������� 15-17 �N��, 18-19 �N�I
           {
             BaseTbSubrev[i].iqt5_totrev=0;
             BaseTbSubrev[i].iam5_totrev=0;
             for( j=0 ; j<10; j++)
                {
                  BaseTbSubrev[i].it8_subrev[j].iqt_subrev=0;
                  BaseTbSubrev[i].it8_subrev[j].iam_subrev=0;
                }
           }

        //
    }
    __finally
    {
        delete tif_ecr;// tifAbacus;
    }
}



void __fastcall BASIC::sub_times(int StartLine, int TotalLine)
{
    //�N���B��B�I���Ʋέp

    AnsiString str_is_rtype, str_trans_flag,Stmp, str_fmcode, s;        //�P��κA�A����κA�A�ӫ~�N��
    int i_trans_num = 0;                            //�P��ƶq

    //0:�@��ӫ~    1:�N��   2:�N��   3:�N�I�B���w   4:�[�Ⱦ�(�},��)�d   5:�[�Ⱦ�(�[��)

    int i_trans_flag = 0;                          //������A
    int iRelType;

    //���B��B�I
    giA=giB=giD=giE=giF=giG=0;
    //giH giI ���w�h�f���B�B����
    giH=giI=0;

    AnsiString str_good_no, str_menu_no, str_bar_name, str_item_amt;
    //TStringList *tsl_subsale = new TStringList();
    AnsiString str_type, str_item_qty, strRelType;        //��

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<=gi_rec_line; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            //gtsl_rec->Strings[i].SubString(370,2);
            str_is_rtype = _StringSegment_EX(s, "|", 38).SubString(1,2);   // ������A�Ϥ�

            //_StrToInt(gtsl_rec->Strings[i].SubString(142,4));
            i_trans_num = _StrToInt(_StringSegment_EX(s, "|", 26) );  //�P��ƶq
            str_type = _StringSegment_EX(s, "|", 8);                  //�P�f���A

            //�s�u�ӫ~�Ϥ�
            strRelType = _StringSegment_EX(s, "|", 36).SubString(1,2);   // �s�u�ӫ~���A
            iRelType=_StrToInt(strRelType);

            str_item_qty=IntToStr(i_trans_num);
            str_trans_flag = _StringSegment_EX(s, "|", 10); //�N��I�Ϥ�       //SPC_FLG  ������O

            str_fmcode = _StringSegment_EX(s, "|", 17); //�ӫ~�N��

            str_good_no  = _StringSegment_EX(s, "|", 17).Trim();
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //�N����,�I���O
            str_menu_no = "0000";
            str_bar_name = _StringSegment_EX(s, "|", 19).Trim();
            str_item_amt = _StringSegment_EX(s, "|", 29).Trim();;

            if(iRelType == 11 && _StrToInt(str_item_amt)==0) //���� �h��� 2012/03/26 Update
               i_trans_num=0;

            // 2006/12/20
            if ( Trim(str_trans_flag)=="" || iRelType == 7 || iRelType == 9)      //�L���A ;�s�u�ӫ~�Ϥ�:�N�Ⲽ�� 07, 09
               {
                Stmp.sprintf("sub_times:���B�z�N�N��X�p, str_good_no=(%d),�s�u�ӫ~�Ϥ�=(%s)",
                                        str_good_no.c_str(), strRelType.c_str());
                writelog(Stmp);                        
                continue;
               }
           // else
          //     {
          //       Stmp.sprintf("sub_times:�N��X�p, str_good_no=(%d), �s�u�ӫ~�Ϥ�=(%s)",
          //                              str_good_no.c_str(), strRelType.c_str());
          //       writelog(Stmp);
          //     }

            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  ������O

            if (str_is_rtype.Pos("FF") || str_is_rtype.Pos("Z0") || str_is_rtype.Pos("Z1") || Trim(str_is_rtype)=="" )
            {
                if (str_type.Pos("S0"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�N��
                            if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //���B
                                   giB+=i_trans_num;               //����
                                }  // end of   if(iRelType =

                            giD += i_trans_num;
                        break;
                        case 2:     //�N��   2006/12/20
                              //2013/11/25  S0  VDC06                             $80 ==>���󬰦��N��I�s���M���s�u�Ϥ� => �Y���ʥN��X�p
                              //if( iRelType != 7 && iRelType != 9)
                              if( iRelType > 0 )
                                 {  ;;   }
                              else
                                {
                                 str_menu_no = "1801";
                                 giE += i_trans_num;
                                 subsale_update(tsl_subsale,str_good_no,str_menu_no,str_bar_name,str_item_amt,"Z");
                                 Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,1 );
                                }
                        break;
                        case 3:     //�N�I�B���w

                            if (str_good_no == "9390027")        //���wFMCODE
                                {
                                giI += i_trans_num;
                                giH += _StrToInt(str_item_amt);   //���B

                                }
                            else
                                {
                                str_menu_no = "2001";
                                giF += i_trans_num;
                                giG += _StrToInt(str_item_amt);   //���B
                                Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,1 );
                                }
                        break;
                        case 0:     //�h���
                              if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //���B
                                   giB+=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
                else if (str_type.Pos("S1"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�N��
                             if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //���B
                                   giB-=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                            giD -= i_trans_num;
                        break;
                        case 2:     //�N��  2006/12/20
                              //2013/11/25  S0  VDC06                             $80 ==>���󬰦��N��I�s���M���s�u�Ϥ� => �Y���ʥN��X�p
                              //if( iRelType != 7 && iRelType != 9)
                               if( iRelType > 0 )
                                 {  ;;   }
                               else
                                {
                                  str_menu_no = "1801";
                                  giE -= i_trans_num;
                                  subsale_update(tsl_subsale,str_good_no,str_menu_no,str_bar_name,str_item_amt,"R");
                                  Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,-1 );
                                }
                        break;
                        case 3:     //�N�I
                            if (str_good_no == "9390027")        //���wFMCODE
                                {
                                giI -= i_trans_num;
                                giH -= _StrToInt(str_item_amt);   //���B

                                }
                            else
                                {
                                str_menu_no = "2001";
                                giF -= i_trans_num;
                                giG -= _StrToInt(str_item_amt);   //���B
                                Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,-1 );
                                }
                        break;
                        case 0:     //�h���
                              if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //���B
                                   giB-=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
            }
            else if ( str_is_rtype.Pos("R2") || str_is_rtype.Pos("R3") )
            {
                if (str_type.Pos("S0"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�N��
                            if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //���B
                                   giB-=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                            giD -= i_trans_num;
                        break;
                        case 2:     //�N�� 2006/12/20
                            //2013/11/25  S0  VDC06                             $80 ==>���󬰦��N��I�s���M���s�u�Ϥ� => �Y���ʥN��X�p
                            //if( iRelType != 7 && iRelType != 9)
                            if( iRelType > 0 )
                                 {  ;;   }
                            else
                              {
                               str_menu_no = "1801";
                               giE -= i_trans_num;
                               subsale_update(tsl_subsale,str_good_no,str_menu_no,str_bar_name,str_item_amt,"R");
                               Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,-1 );
                              }
                        break;
                        case 3:     //�N�I
                            if (str_good_no == "9390027")        //���wFMCODE
                                {
                                giI -= i_trans_num;
                                giH -= _StrToInt(str_item_amt);   //���B

                                }
                            else
                                {
                                str_menu_no = "2001";
                                giF -= i_trans_num;
                                giG -= _StrToInt(str_item_amt);   //���B
                                Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,-1 );
                                }
                        break;
                        case 0:     //�h���
                              if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //���B
                                   giB-=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
                else if (str_type.Pos("S1"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�N��
                            if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //���B
                                   giB+=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                            giD += i_trans_num;
                        break;
                        case 2:     //�N��  2006/12/20
                              //if( iRelType != 7 && iRelType != 9)
                              if( iRelType > 0 )
                                 {  ;;   }
                              else
                                {
                                 str_menu_no = "1801";
                                 giE += i_trans_num;
                                 subsale_update(tsl_subsale,str_good_no,str_menu_no,str_bar_name,str_item_amt,"Z");
                                 Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,1 );
                                }
                        break;
                        case 3:     //�N�I
                              if (str_good_no == "9390027")        //���wFMCODE
                                {
                                giI += i_trans_num;
                                giH += _StrToInt(str_item_amt);   //���B

                                }
                              else
                              {
                               str_menu_no = "2001";
                               giF += i_trans_num;
                               giG += _StrToInt(str_item_amt);   //���B
                               Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,1 );
                               }
                        break;
                        case 0:     //�h���
                              if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //���B
                                   giB+=i_trans_num;               //����
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
            }
        }
    }

    //DeleteFile("e:\\subsal.txt");
    //tsl_subsale->SaveToFile("e:\\subsal.txt");
    //delete tsl_subsale;
    Stmp.sprintf("sub_times: ���� �h�������(%d), �h���(%d), �N�I����(%d), �N�I(%d), �N������(%d)",
                             giB, giA, giF, giG, giD );

    writelog(Stmp);

}


void __fastcall BASIC::Sum_BaseTbSubrev(AnsiString menu_no, AnsiString item_amt,
                                    AnsiString item_qty,int NegTive )
{
  int MemuNo, MenuItem, iamt, iqty;
  AnsiString Stmp;

  try
    {
     Stmp=menu_no.SubString(1,2).Trim();
     MemuNo=Stmp.ToInt();
     Stmp=menu_no.SubString(3,2).Trim();
     MenuItem=Stmp.ToInt();
     iamt=(item_amt.Trim()).ToInt();
     iqty=(item_qty.Trim()).ToInt();

     if(  MemuNo < 1 || MemuNo > 20 )
       return;

     if(  MenuItem < 1 || MenuItem > 8 )
          MenuItem=1;

          BaseTbSubrev[MemuNo-1].iqt5_totrev += iqty*NegTive;
          BaseTbSubrev[MemuNo-1].iam5_totrev += iamt*NegTive;

          BaseTbSubrev[MemuNo-1].it8_subrev[MenuItem-1].iqt_subrev += iqty*NegTive;
          BaseTbSubrev[MemuNo-1].it8_subrev[MenuItem-1].iam_subrev += iamt*NegTive;

        //  Stmp.sprintf("Sum_BaseTbSubrev: Sum menu_no[%d].iqt5_totrev=%d, menu_no[%d].iam5_totrev=%d, 
        //               \it8_subrev[%d].iqt_subrev=%d, it8_subrev[%d].iam_subrev=%d",
        //           MemuNo-1, BaseTbSubrev[MemuNo-1].iqt5_totrev,
        //           MemuNo-1, BaseTbSubrev[MemuNo-1].iam5_totrev,
       //            MenuItem-1, BaseTbSubrev[MemuNo-1].it8_subrev[MenuItem-1].iqt_subrev,
       //           MenuItem-1, BaseTbSubrev[MemuNo-1].it8_subrev[MenuItem-1].iam_subrev  );
        //  writelog(Stmp);
     }
  catch(...)
    {
     Stmp.sprintf("Sum_BaseTbSubrev: Error menu_no=(%s), item_amt=(%s), item_qty=(%s)",
                   menu_no.c_str(), item_amt.c_str(), item_qty.c_str());
     writelog(Stmp);

    }

}




//�o���������X
AnsiString __fastcall BASIC::invoice_start_end(int tot_line_count, bool invoice_start)
{
    AnsiString str_invoice_no;      //�o�����X
    AnsiString str_invoice_char;    //�r�y
    AnsiString str_type,sTmp,s;            //�P�f���A

    bool bl_normal_type = false;    //�L�o�����X

    try
      {
       if (invoice_start)
        {
          //for (int i=0; i<=tot_line_count; i++)
          for (int i=0; i<gtsl_tot->Count; i++)
           {
            s=gtsl_tot->Strings[i];

            str_type = _StringSegment_EX(s, "|", 8);

            sTmp=_StringSegment_EX(s, "|", 9);      //�o���_��
            str_invoice_char =  sTmp.SubString(1,2);
            str_invoice_no = sTmp.SubString(3,8);

            if (_StrToInt(str_invoice_no) == 0)
                continue;

            if ( str_type.Pos("Z0") || str_type.Pos("R4") || str_type.Pos("V"))
            {
                if (str_invoice_no == "00000000")
                    str_invoice_char = "00";

                str_invoice_no = (str_invoice_char + str_invoice_no);
                bl_normal_type = true;
                break;
            }
          }
       }
      else
       {

        for (int i=gtsl_tot->Count-1; i>=0; i--)
        //for (int i=0; i<gtsl_tot->Count; i++)
        {
            s=gtsl_tot->Strings[i];
            str_type = str_type = _StringSegment_EX(s, "|", 8); //gtsl_tot->Strings[i].SubString(67,2);

            sTmp=_StringSegment_EX(s, "|", 10);      //�o������
            str_invoice_char =  sTmp.SubString(1,2);
            str_invoice_no = sTmp.SubString(3,8);

            if (_StrToInt(str_invoice_no) == 0)
                continue;

            if ( str_type.Pos("Z0") || str_type.Pos("R4") || str_type.Pos("V"))
            {
                if (str_invoice_no == "00000000")
                    str_invoice_char = "00";

                str_invoice_no = (str_invoice_char + str_invoice_no);
                bl_normal_type = true;
                break;
            }

          }
       }
    } // end of try
    catch (Exception &e)
    {
         sTmp.sprintf("invoice_start_end: Error %s",
                     e.Message);
         writelog(sTmp);
         str_invoice_no="0000000000";
    }
    catch(...)
    {
       sTmp.sprintf("invoice_start_end: catch Error, tot_line_count=%d",
                     tot_line_count);
       writelog(sTmp);
       str_invoice_no="0000000000";
    }

    if (bl_normal_type)
        return str_invoice_no;
    else
        return "0000000000";

}



BASIC::BASIC()
{
    gtsl_rec = new TStringList();
    gtsl_tot = new TStringList();
    gtsl_drop = new TStringList();
    gtsl_easy_card = new TStringList();
    gtsl_pay_sub = new TStringList();
    gtsl_bil = new TStringList();
    gtsl_fetc_card = new TStringList();
    tsl_subsale = new TStringList();
    gtsl_1051_sal = new TStringList();
    gtsl_Ticket = new TStringList();

    Autotsl_rec = new TStringList();
    Autotsl_tot = new TStringList();
    Autotsl_drop = new TStringList();
    Autotsl_easy_card = new TStringList();
    Autotsl_pay_sub = new TStringList();
    Autotsl_bil = new TStringList();
    Autotsl_fetc_card = new TStringList();
    Autotsl_subsale = new TStringList();
    Auto_1051_sal = new TStringList();
    Auto_Sal= new TStringList();
    Autotsl_Ticket = new TStringList();

    gchar_tencode[0] = 0;
    gchar_ecr_no[0] = 0;
    gchar_cashier[0] = 0;
    gchar_date_time[0] = 0;
    gchar_x_trans[0] = 0;
    memset(gpchar_x_first, 0x00, MAXDATALEN); // gpchar_x_first[0] = 0;
    memset(gpchar_sal_last, 0x00, MAXDATALEN);       //gpchar_sal_last[0] = 0;
    memset(gpchar_sal_first, 0x00, MAXDATALEN);     //gpchar_sal_first[0] = 0;


    pay_bill_cnt = 0;     //�������f�馸�� 20031223
    pay_bill_amt = 0;     //�������f����B
    mpay_bill_cnt = 0;     //�������f�馸�� 20031223
    mpay_bill_amt = 0;     //�������f����B

    Cpn71_cnt=0;                  //�����71����
    Cpn71_amt=0;                  //�����71���B
    Cpn72_cnt=0;                  //�����72����
    Cpn72_amt=0;                  //�����72���B
    Cpn73_cnt=0;                  //�����73����
    Cpn73_amt=0;                  //�����73���B
    Cpn74_cnt=0;                  //�����74����
    Cpn74_amt=0;                  //�����74���B
    Cpn75_cnt=0;                  //�����75����
    Cpn75_amt=0;                  //�����75���B
    Cpn76_cnt=0;                  //�����76����
    Cpn76_amt=0;                  //�����76���B

    Cpn77_cnt=0;                  //�������f��77����
    Cpn77_amt=0;                  //�������f��77���B
    Cpn78_cnt=0;                  //�������f��78����
    Cpn78_amt=0;                  //�������f��78���B
                                                                                        //->
    StrBillPay.sprintf("%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       Cpn71_cnt,Cpn71_amt,Cpn72_cnt,Cpn72_amt,Cpn73_cnt,Cpn73_amt,Cpn74_cnt,Cpn74_amt,
                       Cpn75_cnt,Cpn75_amt,Cpn76_cnt,Cpn76_amt,Cpn77_cnt,Cpn77_amt,Cpn78_cnt,Cpn78_amt   );
}


//�p�p�馩    1011
void __fastcall BASIC::SumDisc(int StartLine, int TotalLine)
{
    AnsiString str_tot_type,s;
    AnsiString str_pay_type;

    giA = giB = 0;

    if (gbl_pay_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_pay_sub->Count; i++)
        {
            s=gtsl_pay_sub->Strings[i];
            str_pay_type = _StringSegment_EX(s, "|", 8);
            str_tot_type = _StringSegment_EX(s, "|", 22).SubString(1,2);

            if (str_pay_type.Pos("F"))  //F���馩��
                continue;


            if (str_tot_type.Pos("FF") || str_tot_type.Pos("Z0") || str_tot_type.Pos("Z1") || Trim(str_tot_type)=="")
            {
                if (str_pay_type.Pos("E0"))
                {
                    giA += _StrToInt(_StringSegment_EX(s, "|", 16));      //����
                    giB += _StrToInt(_StringSegment_EX(s, "|", 17));      //�p�p�馩/�馩/�����B
                }
                else if (str_pay_type.Pos("E1"))
                {
                    giA -= _StrToInt(_StringSegment_EX(s, "|", 16));      //����
                    giB -= _StrToInt(_StringSegment_EX(s, "|", 17));      //�p�p�馩/�馩/�����B
                }
            }
            else if (str_tot_type.Pos("R2") || str_tot_type.Pos("R3"))
            {
                if (str_pay_type.Pos("E0"))
                {
                    giA -= _StrToInt(_StringSegment_EX(s, "|", 16));     //����
                    giB -= _StrToInt(_StringSegment_EX(s, "|", 17));     //�p�p�馩/�馩/�����B
                }
                else if (str_pay_type.Pos("E1"))
                {
                    giA += _StrToInt(_StringSegment_EX(s, "|", 16));    //����
                    giB += _StrToInt(_StringSegment_EX(s, "|", 17));    //�p�p�馩/�馩/�����B
                }
            }
        }
    }
    giB = (0 - giB);


    //giA = abs(giA - pay_bill_cnt - mpay_bill_cnt);
    //giB = (giB - pay_bill_amt - mpay_bill_amt);
    //giA += pay_bill_cnt;
    //giB -= pay_bill_amt;
}

//�[�Ⱦ�
int __fastcall BASIC::easy_card(int StartLine, int TotalLine)
{
    AnsiString str_type, str_sale_type, str_r_type, StrTranType,s;        //�[�ȥ�����O
    int iTmpAmt,int_type = 0;

    int int_0_time  = 0, int_0_money  = 0;
    int int_2_time  = 0, int_2_money  = 0;
    int int_7_time  = 0, int_7_money  = 0;
    int int_9_time  = 0, int_9_money  = 0;
    int int_11_time = 0, int_11_money = 0;

//  �}��d    //�[��      //���b      //����
    giA = giB = giC = giD = giE = giF = giG = giH = 0;
    iTmpAmt=0;
    //return 0;
    if (gbl_easy_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<
        gtsl_easy_card->Count; i++)
        {
            s=gtsl_easy_card->Strings[i];


            StrTranType=gtsl_easy_card->Strings[i].SubString(1,4);

            if( StrTranType == "0025")
              {
                 str_sale_type = gtsl_easy_card->Strings[i].SubString(57,2);
                 str_type = gtsl_easy_card->Strings[i].SubString(132,2);
                 str_r_type = gtsl_easy_card->Strings[i].SubString(178,2);
                 int_type = _StrToInt(str_type);
                 //iTmpAmt=

                 if (str_sale_type.Pos("X"))         //LOG���~���B�z
                    continue;

                 if (str_type != "00" && int_type == 0)  //���~������O
                    continue;

		 if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="")
		    {

	             switch (int_type)
	              {
	                //���b = �}�d + �[��
	                case 0:
	                    ++int_0_time;
	                    //int_0_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[��
	                case 2:
	                    ++int_2_time;
	                    int_2_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�}��d
	                case 7:
	                    ++int_7_time;
	                    int_7_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�}�d����
	                case 9:
	                    ++int_9_time;
	                    int_9_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[�Ȩ���
	                case 11:
	                    ++int_11_time;
	                    int_11_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	               }
	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
	          else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	           {
	            switch (int_type)
	              {
	                //���b
	                case 0:
	                    --int_0_time;
	                    //int_0_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[��
	                case 2:
	                    --int_2_time;
	                    int_2_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�}��d
	                case 7:
	                    --int_7_time;
	                    int_7_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�}�d����
	                case 9:
	                    --int_9_time;
	                    int_9_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[�Ȩ���
	                case 11:
	                    --int_11_time;
	                    int_11_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	              }
	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	       }  // end of if( StrTranType == "0025")
            else if( StrTranType == "1031")  //����
               {
                 //str_sale_type = gtsl_easy_card->Strings[i].SubString(57,2);
                 // 'A101'(�[��) 'A102'(�[�Ȩ���) 'A103'(���Q���I) 'A104'(�������Q���I) 'A105'(���Q�x�I) 'A108'(�I���) 'A991'(�[�Ȳ��`�J�b)
                 //�x�W�q �[��:30   �[�I:36
                 str_type = _StringSegment_EX(s, "|", 10).Trim(); //gtsl_easy_card->Strings[i].SubString(71,4); //A101, A102,
                 str_r_type = _StringSegment_EX(s, "|", 28).SubString(1,2);  //gtsl_easy_card->Strings[i].SubString(536,2);
                 str_sale_type=str_r_type;
                 //int_type = _StrToInt(str_type);
                 //iTmpAmt=
                 if (str_sale_type.Pos("X") || str_sale_type.Pos("V") )         //LOG���~���B�z
                    continue;

                 //if (str_type != "00" && int_type == 0)  //���~������O
                 //   continue;

		 if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )
		    {
                     if(str_type=="A101" || str_type=="A991" || str_type=="30" || str_type=="36") // �[��  ���`�J�b"A991"
                       {
                          ++int_2_time;
                          int_2_money += _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else if(str_type=="A102" || str_type=="70" || str_type=="7C")     //�[�Ȩ���       �x�W�q  �[�Ȩ���:70   �[�I:7c
                       {
                          ++int_11_time;
                           int_11_money += _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else
                       {
                         ++int_2_time;
	                int_2_money += _StrToInt(_StringSegment_EX(s, "|", 18));

                       }

	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
	          else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	           {
                     if(str_type=="A101" || str_type=="A991" || str_type=="30" || str_type=="36") // �[��  ���`�J�b"A991"
                       {
                          --int_2_time;
                          int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else  if(str_type=="A102" || str_type=="70" || str_type=="7C")     //�[�Ȩ���   �x�W�q  �[�Ȩ���:70   �[�I:7c
                       {
                          --int_11_time;
                           int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else
                       {
                         --int_2_time;
	                int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 18));

                       }

	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))

               } // end of if( StrTranType == "0031")
          else if( StrTranType == "1032" && _StringSegment_EX(s, "|", 9).Trim()=="K2" )  //New Dongle ����
               {
                 str_type = _StringSegment_EX(s, "|", 10).Trim();
                 str_r_type = _StringSegment_EX(s, "|", 28).SubString(1,2);
                 str_sale_type=str_r_type;
                 //int_type = _StrToInt(str_type);
                 //iTmpAmt=
                 if (str_sale_type.Pos("X") || str_sale_type.Pos("V") )         //LOG���~���B�z
                    continue;

                 //if (str_type != "00" && int_type == 0)  //���~������O
                 //   continue;

		 if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )
		    {
                     if(str_type=="05" ) // �[��
                       {
                          ++int_2_time;
                          int_2_money += _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else if(str_type=="06")     //�[�Ȩ���
                       {
                          ++int_11_time;
                           int_11_money += _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else
                       {
                         //++int_2_time;
	                //int_2_money += _StrToInt(_StringSegment_EX(s, "|", 18));
                         ;;
                       }

	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
	          else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	           {
                     if(str_type=="05") // �[��
                       {
                          --int_2_time;
                          int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else  if(str_type=="06")     //�[�Ȩ���
                       {
                          --int_11_time;
                           int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
                       }
                     else
                       {
                       //  --int_2_time;
	               // int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
                         ;;
                       }

	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))

               } // end of if( StrTranType == "0032")

          else if( StrTranType == "1034" && _StringSegment_EX(s, "|", 10).Trim()=="K3" )  //New Dongle �K�X�@ for HappyCash(����)
               {
                 str_type = _StringSegment_EX(s, "|", 13).Trim();
                 str_r_type = _StringSegment_EX(s, "|", 31).SubString(1,2);
                 str_sale_type=str_r_type;
                 //int_type = _StrToInt(str_type);
                 //iTmpAmt=
                 if (str_sale_type.Pos("X") || str_sale_type.Pos("V") )         //LOG���~���B�z
                    continue;

                 //if (str_type != "00" && int_type == 0)  //���~������O
                 //   continue;

		 if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )
		    {
                     if(str_type=="7707" ) // �[��
                       {
                          ++int_2_time;
                          int_2_money += _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                       }
                     else if(str_type=="7708")     //�[�Ȩ���
                       {
                          ++int_11_time;
                           int_11_money += _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                       }
                     else
                       {
                         //++int_2_time;
	                //int_2_money += _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                         ;;
                       }

	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
	          else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	           {
                     if(str_type=="7707") // �[��
                       {
                          --int_2_time;
                          int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                       }
                     else  if(str_type=="7708")     //�[�Ȩ���
                       {
                          --int_11_time;
                           int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                       }
                     else
                       {
                       //  --int_2_time;
	               // int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                         ;;
                       }

	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))

               } // end of if( StrTranType == "0032")
             else if( StrTranType == "1811"  )
               {
                 str_sale_type = _StringSegment_EX(s, "|", 9).Trim();   //SALE_TYPE
                 str_r_type = _StringSegment_EX(s, "|", 10).Trim();     //FG_SaleType  S0,S1
                 str_type =  _StringSegment_EX(s, "|", 12).Trim();      //Tran_type  310/320

                 //str_sale_type=str_r_type;
                 //int_type = _StrToInt(str_type);
                 //iTmpAmt=
                 if (str_sale_type.Pos("X") || str_sale_type.Pos("V") )
                     continue;

		 if (str_sale_type.Pos("Z0") || str_sale_type.Pos("Z1")  )   // str_sale_type
		    {
                     if(str_type=="310" ) // �[��
                       {
                          ++int_2_time;
                          int_2_money += _StrToInt(_StringSegment_EX(s, "|", 21));
                       }
                     else if(str_type=="320")     //�[�Ȩ���
                       {
                          ++int_11_time;
                           int_11_money += _StrToInt(_StringSegment_EX(s, "|", 21));
                       }
                     else
                       {
                         //++int_2_time;
	                //int_2_money += _StrToInt(_StringSegment_EX(s, "|", 21));
                         ;;
                       }

	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
                  else if (str_sale_type.Pos("R2") ||str_sale_type.Pos("R3"))
	           {
                     if(str_type=="310") // �[��
                       {
                          --int_2_time;
                          int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 21));
                       }
                     else  if(str_type=="320")     //�[�Ȩ���
                       {
                          --int_11_time;
                           int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 21));
                       }
                     else
                       {
                       //  --int_2_time;
	               // int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 21));
                         ;;
                       }

	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))

               } // end of if( StrTranType == "1811")
         else if( StrTranType == "1038")
              {

                 str_sale_type = _StringSegment_EX(s, "|", 9); //gtsl_easy_card->Strings[i].SubString(57,2);
                 // '811799'(�{���[��) '810899'(�[�Ȩ���) 'X10799'(�[�Ȳ��`) 'X10899'(�[�Ȩ������`) '850799'(�}��d) '850899'(�}��d����) 'X50799'(�}��d���`)


                 str_type = _StringSegment_EX(s, "|", 18).SubString(2,3);  // gtsl_easy_card->Strings[i].SubString(116,3);    //107, 108, 507, 508        �y�C�T���N��
                 str_r_type = _StringSegment_EX(s, "|", 20).SubString(1,2);  //gtsl_easy_card->Strings[i].SubString(225,2);
                 int_type = _StrToInt(str_type);



                 //iTmpAmt=

                 if (str_sale_type.Pos("X") || str_sale_type.Pos("Z") || str_sale_type.Pos("V") )         //LOG���~���B�z
                    continue;

                 //if (str_type != "00" && int_type == 0)  //���~������O
                 //   continue;

		 if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="")
		    {
	             switch (int_type)
	              {
	                //���b = �}�d + �[��
	                case 0:
	                    ++int_0_time;
	                    //int_0_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[��
                        case 417:     //841799
	                case 117:     //811799
	                    ++int_2_time;
	                    int_2_money += _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�}��d
	                case 517:
	                    ++int_7_time;
	                    //int_7_money += (   _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //                 + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) );
                            //2008/06/28 TFM �T�{ sAuthCardSalePHDongleOutData.n_Deposit=remain+iDeposit
                            int_7_money +=  _StrToInt(_StringSegment_EX(s, "|", 14));


	                    break;
	                //�}�d����
	                case 518:
	                    ++int_9_time;
	                    //int_9_money += (   _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //                 + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) );
                            //2008/06/28 TFM �T�{ sAuthCardSalePHDongleOutData.n_Deposit=remain+iDeposit
                            int_9_money +=  _StrToInt(_StringSegment_EX(s, "|", 14));

	                    break;

	                //�[�Ȩ���
                        //case 408:
	                case 118:          //811899
	                    ++int_11_time;
	                    int_11_money += _StrToInt(_StringSegment_EX(s, "|", 14));
                            break;

                        // 824799  �H�Υ[�� AutoLoad  837799 847799  825799
                        // �ǥͥd�i���[��:816799    �ǥͥd�i��:813399 // �i��:811399
                        case 247:
                        case 257:
                        case 167:
                        case 133:
                        case 113:
                             break;

                        // �p�B����
                        //case 105:
                        //     break;

                        // �h�f
                        //case 509:
                        //     break;

                        default:
                            ++int_2_time;
	                    int_2_money += _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	               }
	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
	          else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	           {
	            switch (int_type)
	              {
	                //���b
	                case 0:
	                    --int_0_time;
	                    //int_0_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[��
                        case 417:
	                case 117:
	                    --int_2_time;
	                    int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�}��d
	                case 517:
	                    --int_7_time;
	                    //int_7_money -=(  _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //               + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) ) ;
                            int_7_money -=  _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�}�d����
	                case 518:
	                    --int_9_time;
	                    //int_9_money -= (  _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //                + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) ) ;
                            int_9_money -=  _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�[�Ȩ���
	                case 118:
	                    --int_11_time;
	                    int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 14));
                            break;
                        case 509:
                             break;
                             
                        // 824799  �H�Υ[�� AutoLoad
                        // �ǥͥd�i���[��:816799    �ǥͥd�i��:813399 // �i��:811399
                        case 247:
                        case 257:
                        case 167:
                        case 133:
                        case 113:
                            break;

                        default:
                            --int_2_time;
	                    int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;

	              }
	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))


              }// end of   else if( StrTranType == "1038")
            else // ( StrTranType == "1035") OR 1038
              {

                 str_sale_type = _StringSegment_EX(s, "|", 9); //gtsl_easy_card->Strings[i].SubString(57,2);
                 // '810799'(�{���[��) '810899'(�[�Ȩ���) 'X10799'(�[�Ȳ��`) 'X10899'(�[�Ȩ������`) '850799'(�}��d) '850899'(�}��d����) 'X50799'(�}��d���`)

                 str_type = _StringSegment_EX(s, "|", 16).SubString(2,3);  // gtsl_easy_card->Strings[i].SubString(116,3);    //107, 108, 507, 508        �y�C�T���N��
                 str_r_type = _StringSegment_EX(s, "|", 18).SubString(1,2);  //gtsl_easy_card->Strings[i].SubString(225,2);
                 int_type = _StrToInt(str_type);

                 //iTmpAmt=

                 if (str_sale_type.Pos("X") || str_sale_type.Pos("Z") || str_sale_type.Pos("V") )         //LOG���~���B�z
                    continue;

                 //if (str_type != "00" && int_type == 0)  //���~������O
                 //   continue;

		 if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="")
		    {
	             switch (int_type)
	              {
	                //���b = �}�d + �[��
	                case 0:
	                    ++int_0_time;
	                    //int_0_money += _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[��
                        case 407:
	                case 107:
	                    ++int_2_time;
	                    int_2_money += _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�}��d
	                case 507:
	                    ++int_7_time;
	                    //int_7_money += (   _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //                 + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) );
                            //2008/06/28 TFM �T�{ sAuthCardSalePHDongleOutData.n_Deposit=remain+iDeposit
                            int_7_money +=  _StrToInt(_StringSegment_EX(s, "|", 14));


	                    break;
	                //�}�d����
	                case 508:
	                    ++int_9_time;
	                    //int_9_money += (   _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //                 + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) );
                            //2008/06/28 TFM �T�{ sAuthCardSalePHDongleOutData.n_Deposit=remain+iDeposit
                            int_9_money +=  _StrToInt(_StringSegment_EX(s, "|", 14));

	                    break;

	                //�[�Ȩ���
                        case 408:
	                case 108:
	                    ++int_11_time;
	                    int_11_money += _StrToInt(_StringSegment_EX(s, "|", 14));
                            break;

                        // 824799 ,825799 �H�Υ[�� AutoLoad
                        case 247:
                        case 257:
                             break;

                        // �p�B����
                        //case 105:
                        //     break;

                        // �h�f
                        //case 509:
                        //     break;

                        default:
                            ++int_2_time;
	                    int_2_money += _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	               }
	           }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
	          else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	           {
	            switch (int_type)
	              {
	                //���b
	                case 0:
	                    --int_0_time;
	                    //int_0_money -= _StrToInt(gtsl_easy_card->Strings[i].SubString(125,6));
	                    break;
	                //�[��
                        case 407:
	                case 107:
	                    --int_2_time;
	                    int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�}��d
	                case 507:
	                    --int_7_time;
	                    //int_7_money -=(  _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //               + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) ) ;
                            int_7_money -=  _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�}�d����
	                case 508:
	                    --int_9_time;
	                    //int_9_money -= (  _StrToInt(gtsl_easy_card->Strings[i].SubString(103,5))
                            //                + _StrToInt(gtsl_easy_card->Strings[i].SubString(109,5)) ) ;
                            int_9_money -=  _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;
	                //�[�Ȩ���
	                case 108:
	                    --int_11_time;
	                    int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 14));
                            break;
                        case 509:
                             break;
                             
                        // 824799  �H�Υ[�� AutoLoad
                        case 247:
                        case 257:
                             break;

                        default:
                            --int_2_time;
	                    int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 14));
	                    break;

	              }
	           }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))


              }// end of   if( StrTranType == "0025") else

        }
    }

    //�}��d
    giA = (int_7_time  - int_9_time);
    giB = (int_7_money - int_9_money);

    //�[��
    giC = (int_2_time  - int_11_time);
    giD = (int_2_money - int_11_money);

    //���b
    if (int_0_time)
    {
        giE = int_0_time;     //2003/04/01 liu
        giF = (giB + giD);
    }
    else
    {
       // giE = 1;           //2003/04/02 liu
       // giF = (giB + giD); //2003/04/02 liu
        giE = 0;         //2003/04/02 liu
        giF = 0;         //2003/04/02 liu
    }

    //����
    giG = (int_9_time + int_11_time);   //2005/07/01
    giH = 0 - (int_9_money + int_11_money);  //2005/07/01

    //����
    giG = 0;     //2005/07/01
    giH = 0;     //2005/07/01
    //���b
    giE = 0;         //2005/07/01 liu
    giF = 0;         //2005/07/01 liu

    return 0;
}


//FETC �[�Ⱦ�   2005/07/01   2021/6/15 Add 1028
int __fastcall BASIC::fetc_card(int StartLine, int TotalLine)
{
    AnsiString str_type, str_sale_type, str_r_type,sCardNo,sTmp,s, sFG_SIGN;        //Fetc �[�ȥ�����O
    int iFG_SIGN_Cnt, iTmpAmt,int_type = 0;

    sTmp.sprintf("FETC �[�Ⱦ� fetc_card:(%d,%d)",StartLine,TotalLine);
    writelog(sTmp);

    int int_0_time  = 0, int_0_money  = 0;
    int int_2_time  = 0, int_2_money  = 0;
    int int_7_time  = 0, int_7_money  = 0;
    int int_9_time  = 0, int_9_money  = 0;
    int int_1_time = 0,  int_1_money = 0;

    giA = giB = 0;
    //�[��      //���b      //����
    giFetcCashAddCnt=0;
    giFetcCashAddamt=0;
    giFetcCredAddCnt=0;
    giFetcCredAddamt=0;
    giFetcSumCnt=0;
    giFetcSumAmt=0;
    iTmpAmt=0;
    iFG_SIGN_Cnt=0;
    sFG_SIGN="";
    //_StrToInt(_StringSegment_EX(s, "|", 17));
    if (gbl_fetc_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_fetc_card->Count; i++)
        {
            s=gtsl_fetc_card->Strings[i];

            int_type = _StrToInt(s.SubString(1,4));
            if( int_type == 1028)
             {
                //�H�Υdñ��i��  FG_SIGN==1
                sFG_SIGN=_StringSegment_EX(s, "|", 35);
                str_r_type=_StringSegment_EX(s, "|", 9);
                if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )
                   {
                       iFG_SIGN_Cnt++;   //�H�Υdñ��i��
                   }
                 continue;
             } // end of   if( int_type == 1028)
            else // else of   if( int_type == 1028)
             {

            str_sale_type = _StringSegment_EX(s, "|", 9); //gtsl_fetc_card->Strings[i].SubString(57,2);
            str_type = _StringSegment_EX(s, "|",10);      //gtsl_fetc_card->Strings[i].SubString(69,4);    //��d��������O ����N�X
            sCardNo = _StringSegment_EX(s, "|", 19); //gtsl_fetc_card->Strings[i].SubString(164,30);   //�d��
            str_r_type = _StringSegment_EX(s, "|", 31).SubString(1,2);    //������A�Ϥ�
            iTmpAmt=_StrToInt(_StringSegment_EX(s, "|", 22));
            // int_type = _StrToInt(str_type);
           // sTmp.sprintf("fetc_card:str_sale_type=%s, str_type=%s, sCardNo=%s, str_r_type=%s ",
           //               str_sale_type,str_type, sCardNo, str_r_type);
           // writelog(sTmp);

            if (str_sale_type.Pos("X"))         //LOG���~���B�z
                continue;

            if (str_type == "0000" )  //����N�X
                continue;

            if( str_type == "A090" )   //���b
              {
               ++int_0_time;
	      // int_0_money += _StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
               continue;
              }

            if (str_type == "N092" )  //����N�X
               {
                sTmp.sprintf("fetc_card:str_sale_type=%s, str_type=%s, sCardNo=%s, str_r_type=%s ���`���B�z",
                         str_sale_type,str_type, sCardNo, str_r_type);
                writelog(sTmp);
                continue;
               }

	    if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") || Trim(str_r_type)=="" )
	       {
                 // 2012/08/29 Mts:7787
                 //iTmpAmt=_StrToInt(_StringSegment_EX(s, "|", 22));

                 if(sCardNo.SubString(1,4) == "0999") //�{���d
                    {
                     // if( str_type == "A140"   )   //�{�� �[��
                      if( str_sale_type == "S0"   )   //�{�� �[��
                        {
                          ++int_7_time;
                          //��d��ڥ�����B
	                  int_7_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                         // sTmp.sprintf("fetc_card:�{�� �[�� int_7_money=%s ",
                         // gtsl_fetc_card->Strings[i].SubString(215,10)  );
                        //  writelog(sTmp);

                        }
                      else if( str_sale_type == "S1"   )   // else if( str_type == "A120"  )   //�{�� �[�Ȩ���
                        {
                          ++int_9_time;
	                  int_9_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                         // sTmp.sprintf("fetc_card:�{�� �[�Ȩ��� int_9_money=%s ",
                         // gtsl_fetc_card->Strings[i].SubString(215,10)  );
                         // writelog(sTmp);

                        }

                    }
                 else   //�H�Υd
                    {
                       //if( str_type == "A140"  )   //�H�Υd �[��
                       if( str_sale_type == "S0"   )   //�H�Υd �[��
                        {
                          ++int_1_time;
	                  int_1_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                          //sTmp.sprintf("fetc_card:�H�Υd �[�� int_1_money=%s ",
                          //gtsl_fetc_card->Strings[i].SubString(215,10)  );
                          //writelog(sTmp);
                        }
                      else if( str_sale_type == "S1"   )   //if(  str_type == "A120"  )   //�H�Υd �[�Ȩ���
                        {
                          ++int_2_time;
	                  int_2_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                          //sTmp.sprintf("fetc_card:�H�Υd �[�Ȩ��� int_2_money=%s ",
                          //gtsl_fetc_card->Strings[i].SubString(215,10)  );
                          //writelog(sTmp);

                        }

                    }

	        }
             else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
	        {
                   if(sCardNo.SubString(1,4) == "0999") //�{���d
                    {
                      if( str_sale_type == "S0"  )   //�{�� �[��
                        {
                          --int_7_time;
	                  int_7_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                        }
                      else if(  str_sale_type == "S1"  )   //�{�� �[�Ȩ���
                           {
                            --int_9_time;
	                    int_9_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                           }

                    }
                  else   //�H�Υd
                    {
                       if( str_sale_type == "S0" )   //�H�Υd �[��
                        {
                          --int_1_time;
	                  int_1_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                        }
                       else if(   str_sale_type == "S1"  )   //�H�Υd �[�Ȩ���
                         {
                          --int_2_time;
	                  int_2_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                         }

                    }


	        }    // end of else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))

           } //end  else of  if( int_type == 1028)

        }       // end of for
    }

     //���b
  //  if (int_0_time)
  //  {
        giFetcSumCnt = int_0_time;
        giFetcSumAmt = int_0_money;
  //  }

    giFetcCashAddCnt=int_7_time-int_9_time;   //�{�� �[��
    giFetcCashAddamt=int_7_money-int_9_money; //�{�� �[��

    giFetcCredAddCnt=int_1_time-int_2_time;   //�H�Υd �[��
    giFetcCredAddamt=int_1_money-int_2_money;  //�H�Υd �[��

    giA= iFG_SIGN_Cnt;   //�H�Υdñ��i��

    sTmp.sprintf("fetc_card: e�q�d���b(%d,%d),�{�� �[��(%d,%d),�{�� �[�Ȩ���(%d,%d),�H�Υd �[��(%d,%d),�H�Υd �[�Ȩ���(%d,%d), �H�Υdñ��i��=%d",
                  giFetcSumCnt,giFetcSumAmt,
                  int_7_time, int_7_money, int_9_time, int_9_money,
                  int_1_time, int_1_money, int_2_time, int_2_money, iFG_SIGN_Cnt );
    writelog(sTmp);
    return 0;
}


//���Z����:�H�Υdñ��i��
//�s�W�G�H�Υdñ��i�ƨ�1028.(35)FG_SIGN=��1��
void __fastcall BASIC::Sig_Cnt(int StartLine, int TotalLine)
{
    AnsiString str_type, sTmp,s, sFG_SIGN;        //Fetc �[�ȥ�����O
    int iFG_SIGN_Cnt, int_type = 0;
    iFG_SIGN_Cnt=giA=0;

     if (gbl_fetc_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_fetc_card->Count; i++)
        {
            s=gtsl_fetc_card->Strings[i];

            int_type = _StrToInt(s.SubString(1,4));
            if( int_type == 1028)
             {
                //�H�Υdñ��i��  FG_SIGN==1
                sFG_SIGN=_StringSegment_EX(s, "|", 35);
                str_type=_StringSegment_EX(s, "|", 9);
                if (str_type.Pos("FF") || str_type.Pos("Z0") || str_type.Pos("Z1") || Trim(str_type)=="" )
                   {
                       if( sFG_SIGN == "1")
                           iFG_SIGN_Cnt++;   //�H�Υdñ��i��
                   }
                 continue;
             } // end of   if( int_type == 1028)

        }

    }

    giA= iFG_SIGN_Cnt;   //�H�Υdñ��i��

    sTmp.sprintf("Sig_Cnt: �H�Υdñ��i��=%d", iFG_SIGN_Cnt );
    writelog(sTmp);
    
}



//���Z����:���첼��h���i��
//��1805.FU_SPACE��3�X='1'��1806~1807.FU_SPACE ��1�X='1'�A�P�_���첼��h���i�ơC
void __fastcall BASIC::Rtn_Ticket(int StartLine, int TotalLine)
{
    AnsiString str_type;
    AnsiString str_Space,s, sTmp;
    int int_type=0;
    giA = giB = giC = 0;

    if (gbl_Ticket_haveline)
    {
        for (int i=0; i<gtsl_Ticket->Count; i++)
        {
            s=gtsl_Ticket->Strings[i];

            int_type = _StrToInt(s.SubString(1,4));

            switch( int_type )
            {
            case 1805:
                 str_type = _StringSegment_EX(s, "|", 9);  //S0
                 str_Space = _StringSegment_EX(s, "|", 26).SubString(3,1);
                 if (str_type.Pos("S0")  && str_Space=="1" )
                    {
                       giA++;
                    }
                 break;
            case 1806:
                 str_type = _StringSegment_EX(s, "|", 9); //S0
                 str_Space = _StringSegment_EX(s, "|", 27).SubString(1,1);
                  if (str_type.Pos("S0") && str_Space=="1"  )
                    {
                       giB++;
                    }
                 break;
            case 1807:
                 str_type = _StringSegment_EX(s, "|", 9); //S0
                 str_Space = _StringSegment_EX(s, "|", 28).SubString(1,1);
                 if (str_type.Pos("S0") && str_Space=="1"   )
                    {
                       giC++;
                    }
                 break;
             default:
                  break;
            }



        }    //for (int i=0; i<gtsl_Ticket->Count; i++)
    }


    sTmp.sprintf("Rtn_Ticket:���첼��h���i��:�X�p(1805=%d,1806=%d,1807=%d)",giA, giB, giC  );
    writelog(sTmp);

}



//�馩��  for 1011   "G0" ������馩
void __fastcall BASIC::pay_sub_amt(int StartLine, int TotalLine)
{
    AnsiString str_tot_type;
    AnsiString str_pay_type,s, sTmp;

    giA = giB = 0;

    if (gbl_pay_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_pay_sub->Count; i++)
        {
            s=gtsl_pay_sub->Strings[i];

            str_pay_type = _StringSegment_EX(s, "|", 8);
            str_tot_type = _StringSegment_EX(s, "|", 22).SubString(1,2);

            if (str_pay_type.Pos("E"))  //�p�p�馩
                continue;


            if (str_tot_type.Pos("FF") || str_tot_type.Pos("Z0") || str_tot_type.Pos("Z1") || Trim(str_tot_type)=="" )
            {
                if ( str_pay_type.Pos("F0") || str_pay_type.Pos("G0") || str_pay_type.Pos("H0"))
                {
                    giA += _StrToInt(_StringSegment_EX(s, "|", 16));     //����
                    giB += _StrToInt(_StringSegment_EX(s, "|", 17));     //�p�p�馩/�馩/�����B
                    sTmp.sprintf("�p�p�馩/�馩/�����B:%s+(%d,%d)",s.c_str(),giA, giB );
                }
                else if (str_pay_type.Pos("F1") || str_pay_type.Pos("G1") || str_pay_type.Pos("H1") )
                {
                    giA -= _StrToInt(_StringSegment_EX(s, "|", 16));
                    giB -= _StrToInt(_StringSegment_EX(s, "|", 17));     //�p�p�馩/�馩/�����B
                    sTmp.sprintf("�p�p�馩/�馩/�����B:%s-(%d,%d)",s.c_str(),giA, giB );
                }
                //writelog(sTmp);
            }
            else if (str_tot_type.Pos("R2") || str_tot_type.Pos("R3"))
            {
                if (str_pay_type.Pos("F0") || str_pay_type.Pos("G0") || str_pay_type.Pos("H0") )
                {
                    giA -= _StrToInt(_StringSegment_EX(s, "|", 16));
                    giB -= _StrToInt(_StringSegment_EX(s, "|", 17));     //�p�p�馩/�馩/�����B
                    sTmp.sprintf("�p�p�馩/�馩/�����B:%s-(%d,%d)",s.c_str(),giA, giB );
                }
                else if (str_pay_type.Pos("F1") || str_pay_type.Pos("G1") || str_pay_type.Pos("H1"))
                {
                    giA += _StrToInt(_StringSegment_EX(s, "|", 16));
                    giB += _StrToInt(_StringSegment_EX(s, "|", 17));     //�p�p�馩/�馩/�����B
                    sTmp.sprintf("�p�p�馩/�馩/�����B:%s+(%d,%d)",s.c_str(),giA, giB );
                }
                //writelog(sTmp);
           }
        }
    }

    giB = (0 - giB);
    sTmp.sprintf("�p�p�馩/�馩/�����B:�X�p(%d,%d)",giA, giB );
    writelog(sTmp);

}



AnsiString __fastcall get_version(AnsiString S)
{
    AnsiString str_ver;

    if (S.Length() == 8)
        str_ver = S.SubString(3,6); //(S.SubString(2,2) + S.SubString(5,4));
    else
        str_ver = "999999";

    return str_ver;
}
//------------------------------------------------------------------------------
//�R����w�ؿ��U�Ҧ����
/*
void __fastcall BASIC::_DelTree(AnsiString dir)
{
  return;

  String d=dir;
  String FileName;
  TSearchRec SearchRec;

  String temp;

  temp="�����i�`�N�j_DelTree() �R�O�}�l , �R���ؿ� -> "+dir;
  writelog(temp);


  if (!DirectoryExists(dir))
     {
       temp="�����i���~�j�ؿ����s�b!! -> "+dir;
       writelog(temp);
       return;  // �n #include "FileCtrl.hpp" �~���o�Ө禡
     }

  if (d.SubString(d.Length(),1)!="\\") d=d+"\\";

  if (FindFirst(d+"*.*", faAnyFile, SearchRec) == 0)
     {
       do {  if ((SearchRec.Attr == (faDirectory | faArchive)) ||
                (SearchRec.Attr == (faDirectory)))
                {
                  if ((SearchRec.Name!=".") && (SearchRec.Name!=".."))
                     {
                       _DelTree(d+SearchRec.Name);  //���j�I�s
                       if (RemoveDir(d+SearchRec.Name))
                          {
                            temp="_DelTree() �R�ؿ����\!! -> "+d+SearchRec.Name;
                            writelog(temp);
                          }
                       else
                          {
                            temp="_DelTree() �R�ؿ�����!! -> "+d+SearchRec.Name;
                            writelog(temp);
                          }
                     }
                 }
             else
                 {
                   FileSetAttr(d+SearchRec.Name,faArchive);
                   if (DeleteFile(d+SearchRec.Name))
                      {
                        temp="_DelTree() �R�ɦ��\!! -> "+d+SearchRec.Name;
                        writelog(temp);
                      }
                   else
                      {
                        temp="_DelTree() �R�ɥ���!! -> "+d+SearchRec.Name;
                        writelog(temp);
                      }
                 }
          } while (FindNext(SearchRec) == 0);

       FindClose(SearchRec);

     }

  if (RemoveDir(d))
     {
       temp="_DelTree() �R�ؿ����\!! -> "+d;
       writelog(temp);
     }
  else
     {
       temp="_DelTree() �R�ؿ�����!! -> "+d;
       writelog(temp);
     }
}
*/

//------------------------------------------------------------------------------
// �ϥΤ�k _DeleteAllFile("c:\\abcpsv\\sale\\06\\","*.*");
/*
void __fastcall BASIC::_DeleteAllFile(String Directory , String Mask)
{
  String temp;
  String Dir=Directory;
  String M=Mask;
  if (Dir.SubString(Dir.Length(),1)!="\\") Dir=Dir+"\\";
  if (Trim(M)=="") M="*.*";


  TSearchRec SearchRec;
  if (FindFirst(Dir+M, faAnyFile,SearchRec)==0)
     {
       do {
            if (DeleteFile(Dir+SearchRec.Name))
                      {
                        temp="_DeleteAllFile() �R�ɦ��\!! -> "+Dir+SearchRec.Name;
                        writelog(temp);
                      }
                   else
                      {
                        temp="_DeleteAllFile() �R�ɥ���!! -> "+Dir+SearchRec.Name;
                        writelog(temp);
                      }

          } while (FindNext(SearchRec) == 0);

       FindClose(SearchRec);
     }
}
*/

//�C�X�ؿ����Ҧ��ɮ�
void __fastcall BASIC::list_filename(TStringList *tsl_file_list, AnsiString dir, AnsiString type)
{
    AnsiString str_dir = dir;
    TSearchRec SearchRec;
    int iAttributes = 0;
    iAttributes |= faAnyFile * true;

    tsl_file_list->Clear();

    if (FindFirst((str_dir + type), iAttributes, SearchRec) == 0)
    {
        do
        {
            if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
                tsl_file_list->Add(SearchRec.Name);

        }while (FindNext(SearchRec) == 0);

        FindClose(SearchRec);
    }
}

AnsiString __fastcall _StringSegment(AnsiString Str , AnsiString Comma , int Seg)
{
  if ((Str=="") || (Seg<1)) return "";

  String C=Comma; if (C=="") C=",";

  String s=Str;
  String sTmp;
  String r;
  int iPosComma;

  TStringList *TempList = new TStringList; // declare the list

  TempList->Clear();
  while (s.Pos(C)>0)
        {
          iPosComma = s.Pos(C); // locate commas
          sTmp = s.SubString(1,iPosComma - 1); // copy item to tmp string
          TempList->Add(sTmp); // add to list
          s = s.SubString(iPosComma + 1,s.Length()); // delete item from string
        }
  // trap for trailing filename
  if (s.Length()!=0) TempList->Add(s);

  if (Seg > TempList->Count)
      r="";
  else
      r= TempList->Strings[Seg-1];

  delete TempList; // destroy the list object

  return r;
}

int __fastcall BASIC::subsale_update(TStringList * tsl, AnsiString good_no,
                                     AnsiString menu_no, AnsiString bar_name,
                                     AnsiString item_amt, AnsiString sal_type)
{
    AnsiString str_record;      //�浧�O��
    AnsiString _good_no, _menu_no, _bar_name;   //�f���BMenu�N���B�ӫ~�W��
    int _item_amt = 0;                          //�C�˥N��ӫ~���B�`�X
    int int_record = 0;                         //�浧�O��index
    bool bl_find_record = false;                //�O�_�����ۦP�ӫ~

    if (tsl->Count) //�w���N��O���A���O�_���ۦP����
    {
        bl_find_record = false;
        for (int i=0; i<tsl->Count; i++)
        {
            str_record = tsl->Strings[i];
            if (str_record.Pos(good_no))
            {
                int_record = i;
                bl_find_record = true;
                break;
            }
        }

        if (bl_find_record)     //���ۦP�N�ⶵ��
        {
            str_record = tsl->Strings[int_record];
            tsl->Delete(int_record);
            _good_no = _StringSegment(str_record, "|", 1);
            _menu_no = _StringSegment(str_record, "|", 2);
            _bar_name = _StringSegment(str_record, "|", 3);
            _item_amt = _StrToInt(_StringSegment(str_record, "|", 4));

            if (sal_type == "Z")
            {
                _item_amt = _item_amt + _StrToInt(item_amt.Trim());
            }
            else
            {
                _item_amt = _item_amt - _StrToInt(item_amt.Trim());
            }



            str_record = _good_no + "|" + _menu_no + "|" + _bar_name + "|" + IntToStr(_item_amt);
            tsl->Add(str_record);
        }
        else    //�S���ۦP�N�ⶵ��
        {
            if (sal_type == "Z")
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|" + GetNumber(item_amt);
            else
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|-" + GetNumber(item_amt);

            tsl->Add(str_record);
        }

    }
    else    //�L�O������
    {
        if (sal_type == "Z")
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|" + GetNumber(item_amt);
            else
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|-" + GetNumber(item_amt);

        tsl->Add(str_record);

    }

    tsl->Sort();

  return(0);
}

AnsiString __fastcall GetNumber(AnsiString S)
{
    AnsiString str_num;

    try
    {
        if(Trim(S)=="")
          S="0";

        str_num.sprintf("%d",S.ToInt());
    }
    catch(Exception &e)
    {
        writelog(("�ƭ��ഫ�o�Ϳ��~ " + S));
        return "0";
    }

    return str_num;
}











/**   �O�d�w�ƥ�
bool __stdcall TForm1::Scan_SalLine(FILE *f,  char *pstr)
 {
    char cData[2049],cTmpData[2049], cHead[128],clen[5],c;
    int ilen,j;
    bool brtn,bErr;
    AnsiString sLog,sSaleData;

    brtn=false;
    bErr=false;
    while(!feof(f))
         {
           ilen=0;
           scan_fixstr(f, cHead, 9);
           strncpy(clen,&cHead[5],4 ); clen[4]=0;
           ilen=atoi(clen);
           if( ilen > 2048)
               ilen=512;

           scan_fixstr(f, cData, ilen-11);  //11=9(cHead)+2(0D,0A)
           fscanf(f,"%c",&c );
           if( c != 0x0d && c != 0x0a)
               {
                 j=0;
                 while(!feof(f))
                    {
                       fscanf(f,"%c",&c );
                        j++;
                        if( c == 0x0d || c == 0x0a)
                          {
                           fscanf(f,"\n");
                           break;
                          }
                        if( j > 512 || feof(f))
                          {
                            bErr=true;
                            brtn=false;
                            break;
                          }
                     }   // end of   while(!feof(f))
               }
           else
               {
                 fscanf(f,"\n");
                 sSaleData.sprintf("%s%s",cHead,cData);
                 if( sSaleData.Length() == ilen-2 )
                   {
                     brtn=true;
                   }
                 else
                   {
                     sLog.sprintf("Scan_SalLine:Error Data=%s" , sSaleData.c_str());
                     writelog(sLog);
                   }
               }

            if( bErr==true ||  brtn==true)
              break;

         }   // end of while



   if( bErr==true)
     {
        sLog.sprintf("Scan_SalLine:Error Data=%s" , cHead);
        writelog(sLog);
     }
   if( brtn==false)
     {
        sLog.sprintf("Scan_SalLine: Error ");
        writelog(sLog);
        pstr[0]=0;
     }
   else
     {
    //  strncpy(pstr,sSaleData.c_str(), sSaleData.Length());
        sprintf(pstr,"%s\n", sSaleData.c_str());

     }

   return( brtn );
 }

**/

void __fastcall BASIC::scan_fixstr(FILE *f, register char *str, register int len)
  {
    register int    i;
    register char   *ptr;

    ptr = str;
    for (i=0; i<len; i++)
    {
        fscanf(f, "%c", str);
       // if (*str=='\n') break;
        str++;
       // if (*(str-1)!=' ') ptr = str;
    }
    *str = '\0';
 }






//���ȭ��汵�Z��, ���ȭ���Z���Ӫ�
//iSumCash41:1050�{����w,  iSumPresent41:1050§��+�ӫ~§��, iSumCpn41:1050�����+Other1+�������� CPN
//iSumPsRtn41:1050�l�B�� ,  iSumVisualCpnAmt:1040�L�ȥ�������B, iWinnInvoAmt:1050�o���I�����B
// int __fastcall BASIC::SumCasherRp  &&  BASIC::SumCasherRptXDT
int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sXdtFile,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1099,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update )
{
    int iacc_invo_amt, iacc_amt01, iacc_amt02, iacc_amt03, iacc_amt04, iacc_amt05,
        iacc_amt06, iacc_amt07, iacc_amt08, iacc_amt09, iacc_amt10, iacc_amt11,
        iacc_amt12, iacc_amt13, iacc_amt14;
    int iacc_invo_cnt, iacc_cnt01, iacc_cnt02, iacc_cnt03, iacc_cnt04, iacc_cnt05,
        iacc_cnt06, iacc_cnt07, iacc_cnt08, iacc_cnt09, iacc_cnt10, iacc_cnt11,
        iacc_cnt12, iacc_cnt13, iacc_cnt14;
    int icash_short_amt, ipresent_short_amt, icpn_short_amt, itot_short_amt,
        ircv_cnt, iinvo_rtn_cnt, iinvo_void_cnt;

    int ipay_cash_amt, ipay_present_amt, ipay_cpn_amt,cashi, iTmp;

    int pay_other1_amt, pay_other2_amt,ipay_CldInvo800_amt;

    PAYCASHREC pay_cash_rec[100];


    iacc_invo_amt=iacc_amt01=iacc_amt02=iacc_amt03=iacc_amt04=iacc_amt05=
        iacc_amt06=iacc_amt07=iacc_amt08=iacc_amt09=iacc_amt10=iacc_amt11=
        iacc_amt12=iacc_amt13=iacc_amt14=0;

    iacc_invo_cnt=iacc_cnt01=iacc_cnt02=iacc_cnt03=iacc_cnt04=iacc_cnt05=
        iacc_cnt06=iacc_cnt07=iacc_cnt08=iacc_cnt09=iacc_cnt10=iacc_cnt11=
        iacc_cnt12=iacc_cnt13=iacc_cnt14=0;

    icash_short_amt=ipresent_short_amt=icpn_short_amt=itot_short_amt=
        ircv_cnt=iinvo_rtn_cnt=iinvo_void_cnt=0;

    ipay_cash_amt=ipay_present_amt=ipay_cpn_amt=0;

    //Cloud �����o�����B(800) ipay_CldInvo800_amt 20210601 Update
    pay_other1_amt=pay_other2_amt=ipay_CldInvo800_amt=0;

    AnsiString str_sale_type, strtmp, tmpSer, tmp0041Date1, tmp0041ChrName,s, sLog;       //���b���A


    sLog.printf("SumCasherRpt: ���ȭ���Z���Ӫ� StartLine=%d, iSumCash41=%d, iSumPsRtn41=%d, iSumVisualCpnAmt=%d, ifoodamt=%d, ifoodcnt=%d",
                StartLine, iSumCash41, iSumPsRtn41, iSumVisualCpnAmt, ifoodamt, ifoodcnt);

    writelog(sLog);

    for( cashi=0; cashi<100; cashi++)
        {
               pay_cash_rec[cashi].IndexNo=-1;
               pay_cash_rec[cashi].PayCashAmt=-9999;
        }

    int int_drop_line = (gtsl_drop->Count - 1);   //��w����
    for (int i=StartLine, cashi=0; i<=int_drop_line; i++, cashi++)  //1040��w
        {
           s=gtsl_drop->Strings[i];
           _StringSegment_EX(s, "|", 1);

           iTmp=_StrToInt(_StringSegment_EX(s, "|", 9));   //�����Y�w�ֿn���� ��Z�ɲ���
           if( iTmp == 0)
             {
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=0;
               continue;
              }

           //tmp0041Date1=gtsl_drop->Strings[i].SubString(1,60); // Rec_Type~sale_no
           tmp0041ChrName=_StringSegment_EX(s, "|", 58);  //  gtsl_drop->Strings[i].SubString(450,10);

           tmpSer=_StringSegment_EX(s, "|", 7);    //����Ǹ�
            //�{��
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=_StrToInt(_StringSegment_EX(s, "|", 11));
               if( pay_cash_rec[cashi].PayCashAmt >= 0)
                   ipay_cash_amt +=pay_cash_rec[cashi].PayCashAmt;
            //§��
               ipay_present_amt += _StrToInt(_StringSegment_EX(s, "|", 14));

            //�����
               ipay_cpn_amt += _StrToInt(_StringSegment_EX(s, "|", 15));

            //Cloud �����o�����B(800) ipay_CldInvo800_amt 20210601 Update
               ipay_CldInvo800_amt +=  _StrToInt(_StringSegment_EX(s, "|", 18));

            //�����o�����B  500
               pay_other1_amt +=  _StrToInt(_StringSegment_EX(s, "|", 19));

            //�����o�����B  1000
               pay_other2_amt +=  _StrToInt(_StringSegment_EX(s, "|", 20));

            //�����o�����B 200
               iacc_invo_amt += _StrToInt(_StringSegment_EX(s, "|", 22));

            //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
               iacc_amt01+=_StrToInt(_StringSegment_EX(s, "|", 23));
               iacc_cnt01+=_StrToInt(_StringSegment_EX(s, "|", 24));

               iacc_amt02+=_StrToInt(_StringSegment_EX(s, "|", 25));
               iacc_cnt02+=_StrToInt(_StringSegment_EX(s, "|", 26));

               iacc_amt03+=_StrToInt(_StringSegment_EX(s, "|", 27));
               iacc_cnt03+=_StrToInt(_StringSegment_EX(s, "|", 28));

               iacc_amt04+=_StrToInt(_StringSegment_EX(s, "|", 29));
               iacc_cnt04+=_StrToInt(_StringSegment_EX(s, "|", 30));

               iacc_amt05+=_StrToInt(_StringSegment_EX(s, "|", 31));
               iacc_cnt05+=_StrToInt(_StringSegment_EX(s, "|", 32));

               iacc_amt06+=_StrToInt(_StringSegment_EX(s, "|", 33));
               iacc_cnt06+=_StrToInt(_StringSegment_EX(s, "|", 34));

               iacc_amt07+=_StrToInt(_StringSegment_EX(s, "|", 35));
               iacc_cnt07+=_StrToInt(_StringSegment_EX(s, "|", 36));

               iacc_amt08+=_StrToInt(_StringSegment_EX(s, "|", 37));
               iacc_cnt08+=_StrToInt(_StringSegment_EX(s, "|", 38));

               iacc_amt09+=_StrToInt(_StringSegment_EX(s, "|", 39));
               iacc_cnt09+=_StrToInt(_StringSegment_EX(s, "|", 40));

               iacc_amt10+=_StrToInt(_StringSegment_EX(s, "|", 41));
               iacc_cnt10+=_StrToInt(_StringSegment_EX(s, "|", 42));

               iacc_amt11+=_StrToInt(_StringSegment_EX(s, "|", 43));
               iacc_cnt11+=_StrToInt(_StringSegment_EX(s, "|", 44));

               iacc_amt12+=_StrToInt(_StringSegment_EX(s, "|", 45));
               iacc_cnt12+=_StrToInt(_StringSegment_EX(s, "|", 46));

               iacc_amt13+=_StrToInt(_StringSegment_EX(s, "|", 47));
               iacc_cnt13+=_StrToInt(_StringSegment_EX(s, "|", 48));

               iacc_amt14+=_StrToInt(_StringSegment_EX(s, "|", 49));
               iacc_cnt14+=_StrToInt(_StringSegment_EX(s, "|", 50));


        }     //for (int i=StartLine; i<=TotalLine; i++)

     int itot_acc_amt;   //�O�γ�ڪ��B
     itot_acc_amt=iacc_amt01+iacc_amt02+iacc_amt03+iacc_amt04+iacc_amt05+
                iacc_amt06+iacc_amt07+iacc_amt08+iacc_amt09+iacc_amt10+
                iacc_amt11+iacc_amt12;//+iacc_amt13+iacc_amt14;   20151123 �אּ���w�P���f��  Charles.Wang

     iacc_amt13=ifoodamt; //���w�h�ڪ��B ;
     iacc_cnt13=ifoodcnt;//���w�h�ڦ��� ;

     //�{���u����=(�{��+�����o�����B(200+1000)+�O�γ�ڪ��B+��l�B��)-XDT�{�� ,��acc_type > 01�ɤ~����
     //�{���u����=(�{��+�O�γ�ڪ��B+��l�B��)-XDT�{�� ,��acc_type > 01�ɤ~����  // 2017/0815 Update
    //int itotSumCash41=(ipay_cash_amt+iacc_invo_amt+pay_other2_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);
    int itotSumCash41=(ipay_cash_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);     // 2017/0815 Update
    int itotSumPresent41=ipay_present_amt-iSumPresent41; //§��u����,��acc_type > 01�ɤ~����
    int itotSumCpn41=ipay_cpn_amt-iSumCpn41;
    int itotWinInvoAmt41=(iacc_invo_amt+pay_other1_amt+pay_other2_amt+ipay_CldInvo800_amt)-iWinnInvoAmt;    // 2017/08/15 Update �����o�����B

    int iqt_cetel, iqt_R23Cnt, iqt_VCnt;
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����
    int iqt_RTicketCnt, iqt_SigCnt;
    int iRentAmt, iRemtCnt;
    sub_times(0,gi_rec_line);   //giH giI ���w�h�f���B�B����       //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_cetel=giD;
    iqt_food=giI; iamt_food=giH;

    TotMissCnt(0,gi_tot_line);

    Sum040RecycleDataCnt(0, gi_bil_line);      //�o�귽�^���� /���f����B/ ���f��i��  �ȥ������i��  �ȥ������i��
    iacc_amt14=giE; //���f����B
    iacc_cnt14=giF; //���f��i��

    // 2018/12/07 Lu Update
    int iPaCpnAmt = giM;         //�ȥ��������B
    int iPaCpnCnt = giN;         //�ȥ������i��
    int iPaPresentAmt = giO;    //�ȥ�§����B
    int iPaPresentCnt = giP;    //�ȥ�§��i��

    iqt_R23Cnt=giA;
    iqt_VCnt=giB;

    Rtn_Ticket(0, gbl_Ticket_haveline);
    iqt_RTicketCnt=giA+giB+giC;  //1805, 1806, 1807 ���Ƭ�����

    Sig_Cnt(0, gi_fetc_line); //�H�Υdñ��i��
    iqt_SigCnt = giA;


    strtmp.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�:���ȭ���Z(%s) ��w����(%d), �N���i��(%d), �@�o�o���i��(%d), ��������@�o�o���i��(%d), ��l�B��(%d),���f����B(%d),���f��i��(%d)",
                   sTranCnt.SubString(53,8), int_drop_line+1, iqt_cetel, iqt_R23Cnt, iqt_VCnt, iSumPsRtn41, iacc_amt14, iacc_cnt14 );
    writelog(strtmp);

    FILE *fxdata;


   if( Trim(sXdtFile)!="")
     {
      if ((fxdata = fopen(sXdtFile.c_str(), "a+")) != NULL)
       {
        try
         {
          //1041|�q��
          RtnSumCasherData.sprintf("%s|%04d|%04d|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%2s|%010ld|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%010ld|%010ld|%010ld|%04d|%04d|%04d|%-10.10s|%8s|",
                                         sTranCnt.c_str(),
                                         0,
                                         0,
                                         ipay_cash_amt,
                                         0,   //pay_ic_amt
                                         0,   // pay_credit_amt
                                         ipay_present_amt,
                                         ipay_cpn_amt,
                                         0,
                                         0, //pay_bill_amt
                                         ipay_CldInvo800_amt, //pay_group_amt, //Cloud �����o�����B(800) ipay_CldInvo800_amt 20210601 Update
                                         pay_other1_amt,  //�����o�� 500
                                         pay_other2_amt,  //�����o�� 1000
                                         sXdtNo.c_str(),   //[00]:��w, [>01]:��Z�Ǹ�
                                         iacc_invo_amt,    //�����o�� 200
                                         iacc_amt01,
                                         iacc_cnt01,
                                         iacc_amt02,
                                         iacc_cnt02,
                                         iacc_amt03,
                                         iacc_cnt03,
                                         iacc_amt04,
                                         iacc_cnt04,
                                         iacc_amt05,
                                         iacc_cnt05,
                                         iacc_amt06,
                                         iacc_cnt06,
                                         iacc_amt07,
                                         iacc_cnt07,
                                         iacc_amt08,
                                         iacc_cnt08,
                                         iacc_amt09,
                                         iacc_cnt09,
                                         iacc_amt10,
                                         iacc_cnt10,
                                         iacc_amt11,
                                         iacc_cnt11,
                                         iacc_amt12,
                                         iacc_cnt12,
                                         iacc_amt13,     //���w�h�ڪ��B
                                         iacc_cnt13,     //���w�h�ڵ���
                                         iacc_amt14,     //���f����B
                                         iacc_cnt14,     //���f��i��
                                         itotSumCash41, //�{���u����,��acc_type > 01�ɤ~����
                                         itotSumPresent41, //§��u����,��acc_type > 01�ɤ~����
                                         itotSumCpn41, //�����u����,��acc_type > 01�ɤ~����
                                         itotSumCash41+itotSumPresent41+itotSumCpn41+itotWinInvoAmt41, //�u�����X�p,��acc_type > 01�ɤ~����
                                         iqt_cetel, //�N���i��,��acc_type > 01�ɤ~����
                                         iqt_R23Cnt, //�@�o�o���i��,��acc_type > 01�ɤ~����
                                         iqt_VCnt, //��������@�o�o���i��,��acc_type > 01�ɤ~����
                                         tmp0041ChrName.c_str(),  //10,���ȭ�
                                         "Z0FFFFFF" );  //�O�d,ver1 9byte
                                         
          fprintf(fxdata,"%s",RtnSumCasherData.c_str());

          //RtnSumCasherData+=sStr1097;
          //fprintf(fxdata,"%s\n",sStr1097);
          fprintf(fxdata,"%s\n",sStr1099);


          strtmp.sprintf("���ȭ���ZDATA->0041:%s OK",sXdtFile);
          writelog(strtmp);
        }
      catch(...)
       {
          strtmp.sprintf("���ȭ���ZDATA->0041:%s Catch Error",sXdtFile);
          writelog(strtmp);
       }

      fclose(fxdata);

      if( iAutoZ )
        DeleteFile(sXdtFile.c_str());

      }   // end of if ((fxdata = fopen(sXdtFile.c_str(), "a+")) != NULL)
   } //end of  if( Trim(sXdtFile)!="")


   TStringList *tslCrp;
   tslCrp = new TStringList;
   AnsiString nt,NowTime, sStoreNo, sStoreName;

   TIniFile *AutoCrpIni, *xzdata_ecr;
   AutoCrpIni = new TIniFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");
   int i1,i2,iFTLSysPrinterInitial;

   try
        {
            xzdata_ecr = new TIniFile(XZDATA_INI);
            iFTLSysPrinterInitial = xzdata_ecr->ReadInteger("ECR","FTLSysPrinterInitial",0);     //�ֿn���b���ƧǸ�
        }
    __finally
        {
            delete xzdata_ecr;
        }
   AnsiString CmdStr="";
   if( iFTLSysPrinterInitial == 1)
       CmdStr.sprintf("%c%cM080801", 27, 27);    //ESC, ESC, M , �r��[2]:08, SIZE[2]:08, �˦�[2]:00

    strtmp.sprintf("SumCasherRpt: ���ȭ��汵�Z���Ӫ�:XZDATA_INI->ECR->FTLSysPrinterInitial=%d, CmdStr(%s)", iFTLSysPrinterInitial, CmdStr.c_str());
   writelog(strtmp);

   sTranCnt +="|";
   nt=  _StringSegment_EX(sTranCnt, "|", 5); // sTranCnt.SubString(21,14);  //1041
   NowTime=nt.SubString(1,4)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+"  "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   read_ten_name(sStoreNo, sStoreName);


   strtmp.sprintf("%s         ���ȭ��汵�Z���Ӫ�",CmdStr.c_str() ); tslCrp->Add(strtmp);
   strtmp.sprintf("%s%s     ����:%s",CmdStr.c_str(), NowTime,_StringSegment_EX(sTranCnt, "|", 4) );   tslCrp->Add(strtmp);
   strtmp.sprintf("%s����:%s    ���W:%s",CmdStr.c_str(),_StringSegment_EX(sTranCnt, "|", 3), sStoreName);  tslCrp->Add(strtmp);
   strtmp.sprintf("%s���ȭ��s��:%s",CmdStr.c_str(),_StringSegment_EX(sTranCnt, "|", 8) );   tslCrp->Add(strtmp);
   strtmp.sprintf("%s��w����:",CmdStr.c_str());   tslCrp->Add(strtmp);

   AnsiString ReadCashStr,CashStr;
   int  ReadCashCnt=0;
   int  ReadCashtot=0;
   int  ReadCashCntIdx=0;
   CashStr="�{��|";
   //strtmp.sprintf("      �ϲ{           ��:%8ld",ipay_cash_amt);   tslCrp->Add(strtmp);

   ReadCashStr=AutoCrpIni->ReadString("AutoX","Cashdrp","0");
   ReadCashCnt=AutoCrpIni->ReadInteger("AutoX","Cashdrpcnt",0);
   ReadCashtot=AutoCrpIni->ReadInteger("AutoX","Cashdrptot",0);

   if(ReadCashCnt > 0)
     {
       //strtmp.sprintf("      �ϲ{           ��:%2d.%8ld",  1, ReadCashtot );
       strtmp.sprintf("%s  �ϲ{           ��:��.%8ld",CmdStr.c_str(), ReadCashtot );
       tslCrp->Add(strtmp);
       ReadCashCnt=1;
       ReadCashCntIdx=0;
     }


   for(cashi=0; cashi<100; cashi++)
      {
       //pay_cash_rec[cashi].IndexNo=cashi;
       //        pay_cash_rec[cashi].PayCashAmt

       if( pay_cash_rec[cashi].PayCashAmt < 0)
           break;

       if( cashi==0 && ReadCashCnt==0 )
          {

            strtmp.sprintf("%s  �ϲ{           ��:%2d.%8ld",CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt );
            tslCrp->Add(strtmp);
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
                  strtmp.sprintf("%02d|%8ld|",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
                  strtmp.sprintf("%02d|%8ld|",0 , 0);

               CashStr+=strtmp;
              }

          }
       else
          {

            strtmp.sprintf("%s                   :%2d.%8ld",CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt);
            tslCrp->Add(strtmp);
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
                 strtmp.sprintf("%02d|%8ld|",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
                      strtmp.sprintf("%02d|%8ld|",0 , 0);
               CashStr+=strtmp;
              }
          }


      }   // end of for(cashi=0; cashi<100; cashi++)

   if( Update )
         {
               if(!iAutoZ)
                  {
                    ReadCashtot=0;
                    ReadCashCnt=0;
                    CashStr="";
                  }
               AutoCrpIni->WriteString("AutoX","Cashdrp",CashStr);
               AutoCrpIni->WriteInteger("AutoX","Cashdrpcnt",ReadCashCnt);
               AutoCrpIni->WriteInteger("AutoX","Cashdrptot",ReadCashtot);
         }


   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","pres","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s�@��§�� / �ӫ~§��     :%7ld",CmdStr.c_str(),ipay_present_amt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("§��|%02d|%8ld|", 0, ipay_present_amt+i2);
      else
        strtmp.sprintf("§��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","pres",strtmp);
     }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","cpn","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �ϧ����/���Q�I��     :%7ld",CmdStr.c_str(),ipay_cpn_amt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���|%02d|%8ld|", 0, ipay_cpn_amt+i2);
      else
       strtmp.sprintf("���|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","cpn",strtmp);
     }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","invo","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(200��) :%7ld",CmdStr.c_str(),iacc_invo_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����|%02d|%8ld|", 0, iacc_invo_amt+i2);
      else
        strtmp.sprintf("����|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo",strtmp);
    }


   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","invo500","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(500��) :%7ld",CmdStr.c_str(),pay_other1_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����500|%02d|%8ld|", 0, pay_other1_amt+i2);
      else
        strtmp.sprintf("����500|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo500",strtmp);
    }

    //Cloud �����o�����B(800) ipay_CldInvo800_amt  20210601 Update
   strtmp=AutoCrpIni->ReadString("AutoX","invo800","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(800��) :%7ld",CmdStr.c_str(),ipay_CldInvo800_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����800|%02d|%8ld|", 0, ipay_CldInvo800_amt+i2);
      else
        strtmp.sprintf("����800|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo800",strtmp);
    }


    ////////
   strtmp=AutoCrpIni->ReadString("AutoX","invo1000","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(1000��):%7ld",CmdStr.c_str(),pay_other2_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����1000|%02d|%8ld|", 0, pay_other2_amt+i2);
      else
        strtmp.sprintf("����1000|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo1000",strtmp);
    }


   ///////
   strtmp=AutoCrpIni->ReadString("AutoX","acc","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �϶O�γ�ڪ��B        :%7ld",CmdStr.c_str(),itot_acc_amt+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�O��|%02d|%8ld|", 0, itot_acc_amt+i2);
      else
       strtmp.sprintf("�O��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","acc",strtmp);
     }

   ///////

   ///////  ���w�h��  20151202 ���a��������p��A���ܰh�ڵ��ƫe
   /*strtmp=AutoCrpIni->ReadString("AutoX","food","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("      �Э��w�h��       :   %8ld",iacc_amt13+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���w|%02d|%8ld|", 0, iacc_amt13+i2);
      else
       strtmp.sprintf("���w|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","food",strtmp);
     }
   */
   ///////



   strtmp=AutoCrpIni->ReadString("AutoX","drpacc","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   //sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> drpacc = %s ,��w�p�p  :%8ld", strtmp,
   //                                               ipay_cash_amt+ipay_present_amt+ipay_cpn_amt+iacc_invo_amt+itot_acc_amt+i2 );
   //writelog(sLog);


   strtmp.sprintf("------------------------------------"); tslCrp->Add(strtmp);
   strtmp.sprintf("%s  ��w�p�p         :   %8ld\n",CmdStr.c_str(),ipay_cash_amt+ipay_present_amt+ipay_cpn_amt+
                   ipay_CldInvo800_amt+iacc_invo_amt+pay_other1_amt+pay_other2_amt+itot_acc_amt+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("��w|%02d|%8ld|", 0, ipay_cash_amt+ipay_present_amt+ipay_cpn_amt+iacc_invo_amt+pay_other2_amt+itot_acc_amt+i2);
      else
        strtmp.sprintf("��w|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","drpacc",strtmp);
     }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","Cashtot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> Cashtot = %s ,�{���u����  :%8ld", strtmp, itotSumCash41+i2);
   writelog(sLog);

 //strtmp.sprintf("�{���u����  :01234567  ���ȭ�ñ��",itotSumCash41);   tslCrp->Add(strtmp);
   strtmp.sprintf("%s�{���u����  :%7ld ���ȭ�ñ��",CmdStr.c_str(),itotSumCash41+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
         strtmp.sprintf("�{�u|%02d|%8ld|", 0, itotSumCash41+i2);
     else
         strtmp.sprintf("�{�u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","Cashtot",strtmp);
    }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","prestot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> prestot = %s ,§��u����  :%8ld", strtmp, itotSumPresent41+i2);
   writelog(sLog);

   strtmp.sprintf("%s§��u����  :%7ld �z�w�w�w�w �{",CmdStr.c_str(),itotSumPresent41+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("§�u|%02d|%8ld|", 0, itotSumPresent41+i2);
      else
       strtmp.sprintf("§�u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","prestot",strtmp);
    }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","cpntot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> cpntot = %s , �����u����:%8ld", strtmp, itotSumCpn41+i2 );
   writelog(sLog);

   strtmp.sprintf("%s�����u����:%7ld �x         �x",CmdStr.c_str(),itotSumCpn41+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("��u|%02d|%8ld|", 0, itotSumCpn41+i2);
      else
       strtmp.sprintf("��u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","cpntot",strtmp);
     }

   ////////   �����o�����B  ////////////
   strtmp=AutoCrpIni->ReadString("AutoX","wininvotot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> wininvotot = %s , �����o���u����:%8ld", strtmp, itotWinInvoAmt41+i2 );
   writelog(sLog);

   strtmp.sprintf("%s�����o���u����:%5ld �x         �x",CmdStr.c_str(),itotWinInvoAmt41+i2);   tslCrp->Add(strtmp);
   int iTmptotWinInvoAmt41 = itotWinInvoAmt41+i2;
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���u|%02d|%8ld|", 0, itotWinInvoAmt41+i2);
      else
       strtmp.sprintf("���u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","wininvotot",strtmp);
     }

   ///////////////////////////



   strtmp=AutoCrpIni->ReadString("AutoX","subtot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> subtot = %s , �u�����X�p  :%8ld", strtmp,
                                                  itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41 );
   writelog(sLog);

   strtmp.sprintf("%s�u�����X�p  :%7ld �x         �x",CmdStr.c_str(),itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("�u�X|%02d|%8ld|", 0, itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41);
      else
        strtmp.sprintf("�u�X|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","subtot",strtmp);
     }

   ///////
   strtmp=AutoCrpIni->ReadString("AutoX","agncnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> agncnt = %s , �N���i��    :%8ld", strtmp, iqt_cetel+i2 );
   writelog(sLog);

   strtmp.sprintf("%s�N���i��    :%7ld �x         �x",CmdStr.c_str(),iqt_cetel+i2);   tslCrp->Add(strtmp);
   if( iAutoZ )
       strtmp.sprintf("�N�i|%02d|%8ld|", 0, iqt_cetel+i2);  // 2013/10/17 Update Lu CodeView
   else
       strtmp.sprintf("�N�i|%02d|%8ld|", 0, 0);
   AutoCrpIni->WriteString("AutoX","agncnt",strtmp);

   
/////////////////////////// 2014/12/10 //////////////////////////////////////////////////////////////
   strtmp=AutoCrpIni->ReadString("AutoX","s1051cnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   AnsiString sMissRec, AutoXsMissRec, PrintAutoXsMissRec;
   AutoXsMissRec=AutoCrpIni->ReadString("AutoX","s1051cntMissRec","");

   S1051MissCnt(0,gi_1051_line,sMissRec);
   int  sMissRecCnt=giC;
   AutoXsMissRec+=sMissRec;
   PrintAutoXsMissRec=AutoXsMissRec;

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> s1051cnt = %s ,�ȥ��@�o�o���i��  :%8ld, AutoXsMissRec(%s)",
                                                  strtmp, giA+i2,AutoXsMissRec.c_str());
   writelog(sLog);

   strtmp.sprintf("%s�ȥ��@�o�o��:%7ld �x         �x",CmdStr.c_str(), giA+i2 );   tslCrp->Add(strtmp);   //�|�w�w�w�w�w�}


     if( Update )
     {
      if( iAutoZ )
       {
        strtmp.sprintf("�ȼo|%02d|%8ld|", 0, giA+i2);
        }
      else
       {
         strtmp.sprintf("�ȼo|%02d|%8ld|", 0, 0);
         AutoXsMissRec="";
       }

      AutoCrpIni->WriteString("AutoX","s1051cnt",strtmp);
      AutoCrpIni->WriteString("AutoX","s1051cntMissRec",AutoXsMissRec);
     }
///////////////////////////////////////////////////////////////////////////////////////////////////////

    // 2018/12/07 Lu Update
    //iPaCpnAmt �ȥ��������B
    //iPaCpnCnt �ȥ������i��
    //iPaPresentAmt �ȥ�§����B
    //iPaPresentCnt �ȥ�§��i��

    strtmp=AutoCrpIni->ReadString("AutoX","iPaPresentCnt","0");  //�ȥ������i��
    i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
    i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

    sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> iPaPresentCnt = %s ,�ȥ�§��i��  :%8ld", strtmp,
                                                     iPaPresentCnt+i2);
    writelog(sLog);

    strtmp.sprintf("%s�ȥ�§��i��:%7ld �x         �x",CmdStr.c_str(), iPaPresentCnt+i2 );   tslCrp->Add(strtmp);   //�|�w�w�w�w�w�}


     if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("��§|%02d|%8ld|", 0, iPaPresentCnt+i2);
      else
       strtmp.sprintf("��§|%02d|%8ld|", 0, 0);

      AutoCrpIni->WriteString("AutoX","iPaPresentCnt",strtmp);
     }

    /////////////////////////////////////////////////

    strtmp=AutoCrpIni->ReadString("AutoX","iPaCpnCnt","0");  //�ȥ������i��
    i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
    i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

    sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> iPaCpnCnt = %s ,�ȥ������i��  :%8ld", strtmp,
                                                     iPaCpnCnt+i2);
    writelog(sLog);

    strtmp.sprintf("%s�ȥ������i��:%5ld �x         �x",CmdStr.c_str(), iPaCpnCnt+i2 );   tslCrp->Add(strtmp);   //�|�w�w�w�w�w�}


     if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�ȧ�|%02d|%8ld|", 0, iPaCpnCnt+i2);
      else
       strtmp.sprintf("�ȧ�|%02d|%8ld|", 0, 0);

      AutoCrpIni->WriteString("AutoX","iPaCpnCnt",strtmp);
     }


///////////////////////////////////////////////////////////////////////////////////////////////////////


   strtmp=AutoCrpIni->ReadString("AutoX","r23cnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> r23cnt = %s , �@�o�o���i��    :%8ld", strtmp, iqt_R23Cnt+iqt_VCnt+i2 );
   writelog(sLog);

   strtmp.sprintf("%s�@�o�o���i��:%7ld �x         �x",CmdStr.c_str(),iqt_R23Cnt+iqt_VCnt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�o�i|%02d|%8ld|", 0, iqt_R23Cnt+iqt_VCnt+i2);
      else
       strtmp.sprintf("�o�i|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","r23cnt",strtmp);
     }




////////////////////���w�h�ڵ���2015/12/02///////////////////////////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","Foodcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt13,     //���w�h�ڪ��B
   //iacc_cnt13,     //���w�h�ڵ���
   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> Foodcnt = %s ,���w�h�ڵ��� :%4ld, ���w�h�ڪ��B :%8ld,", strtmp,
                                                      iacc_cnt13+i1, iacc_amt13+i2);
   writelog(sLog);
   strtmp.sprintf("%s���w�h�ڪ��B:%7ld �x         �x",CmdStr.c_str(), iacc_amt13+i2 );   tslCrp->Add(strtmp);
   //strtmp.sprintf("���w�h���`��:%8ld  ", iacc_cnt13+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���h|%04d|%8ld|",iacc_cnt13+i1, iacc_amt13+i2);
      else
       strtmp.sprintf("���h|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","Foodcnt",strtmp);
     }



  /////////////////////////// 2015/10/22 //////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","BillAmt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt14,     //���f����B
   //iacc_cnt14,     //���f��i��

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> BillAmt = %s ,���f��i�� :%4ld, ���f����B :%8ld,", strtmp,
                                                      iacc_cnt14+i1, iacc_amt14+i2);
   writelog(sLog);
   strtmp.sprintf("%s���f����B  :%7ld �x         �x",CmdStr.c_str(), iacc_amt14+i2 );   tslCrp->Add(strtmp);
   strtmp.sprintf("%s���f��i��  :%7ld �x         �x",CmdStr.c_str(), iacc_cnt14+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���f|%04d|%8ld|",iacc_cnt14+i1, iacc_amt14+i2);
      else
       strtmp.sprintf("���f|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","BillAmt",strtmp);
     }


  ////////////////////////////// ����h���i�� //////////////////////////////////////
   strtmp=AutoCrpIni->ReadString("AutoX","rTickcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> rTickcnt = %s ,����h���i��  :%8ld", strtmp,
                                                     iqt_RTicketCnt+i2);
   writelog(sLog);

   strtmp.sprintf("%s���첼��h���i��:%3ld �x         �x",CmdStr.c_str(),iqt_RTicketCnt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�h��|%02d|%8ld|", 0, iqt_RTicketCnt+i2);
      else
       strtmp.sprintf("�h��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","rTickcnt",strtmp);
     }

   ////////////////////////////// �H�Υdñ��i�� //////////////////////////////////////
   strtmp=AutoCrpIni->ReadString("AutoX","rSigcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> rTickcnt = %s ,�H�Υdñ��i��  :%8ld", strtmp,
                                                     iqt_SigCnt+i2);
   writelog(sLog);

   strtmp.sprintf("%s�H�Υdñ��i��:%5ld �|�w�w�w �w�}",CmdStr.c_str(),iqt_SigCnt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("ñ��|%02d|%8ld|", 0, iqt_SigCnt+i2);
      else
       strtmp.sprintf("ñ��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","rSigcnt",strtmp);
     }

 /////////////////////////////////////////////////////////////////////////////////////////////////


   if( Cpn71_amt )
      { strtmp.sprintf("%s�o�q���U��  :%7ld  ",CmdStr.c_str(),Cpn71_amt);   tslCrp->Add(strtmp);  }
   if( Cpn72_amt )
      { strtmp.sprintf("%s�o����U��  :%7ld  ",CmdStr.c_str(),Cpn72_amt);   tslCrp->Add(strtmp);  }
   if( Cpn73_amt )
      { strtmp.sprintf("%s�o���q�U��  :%7ld  ",CmdStr.c_str(),Cpn73_amt);   tslCrp->Add(strtmp);  }
   if( Cpn74_amt )
      { strtmp.sprintf("%s�o���гU��  :%7ld  ",CmdStr.c_str(),Cpn74_amt);   tslCrp->Add(strtmp);  }
   if( Cpn75_amt )
      {
        // 2019/11/27 Update  20200409 �U��
        //strtmp.sprintf("%s�o�����X�U��:%7ld  ",CmdStr.c_str(),Cpn75_amt);   tslCrp->Add(strtmp);
          strtmp.sprintf("%s�o���O�U��  :%7ld  ",CmdStr.c_str(),Cpn75_amt);   tslCrp->Add(strtmp);
        }

    if( Cpn76_amt )  //Update  20200409
      {
          strtmp.sprintf("%s�o��ʹq���U��:%5ld  ",CmdStr.c_str(),Cpn76_amt);   tslCrp->Add(strtmp);
      }


   /***
   strtmp.sprintf("                             ���ȭ�ñ��");  tslCrp->Add(strtmp);
   strtmp.sprintf("                            �z�w�w�w�w-�{");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �x         �x");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �x         �x");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �x         �x");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �|�w�w�w�w-�}");   tslCrp->Add(strtmp);
   ****/

   if( PrintAutoXsMissRec.Trim() != "")
     {
     //�ȥ��@�o�o ����  2019/04/26
     strtmp.sprintf("--------�ȥ��@�o�o������----------");  tslCrp->Add(strtmp);
     strtmp.sprintf("��  ����Ǹ�     �o�����X");  tslCrp->Add(strtmp);
     i2=0;
     while(1)
     {
     i2++;
     tmpSer=_StringSegment_EX(PrintAutoXsMissRec, "|", i2);
     if(tmpSer.Trim()=="" || i2>500 ) break;
     strtmp.sprintf("%d.%24s",i2, tmpSer );  tslCrp->Add(strtmp);

     }  // end of while

     strtmp.sprintf("(�@�o�o�����Ӥ��]�t�d�ȭ��L)");  tslCrp->Add(strtmp);
   }
   strtmp.sprintf("\n\n\n\n");  tslCrp->Add(strtmp);
   strtmp.sprintf("\x1b\x69");  tslCrp->Add(strtmp); //����

    //��X�ɮ�
   if (FileExists(SCasherRpt.c_str() ))
        DeleteFile(SCasherRpt.c_str());

   tslCrp->SaveToFile(SCasherRpt);
   //tslCrp->Clear();
   delete tslCrp;
   delete AutoCrpIni;

   //if( iAutoZ==0 ) //�D�۰ʤ鵲
   //     DeleteFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");

   return 0;
}



int __fastcall BASIC::read_ten_name(AnsiString &sStoreNo, AnsiString &sStoreName)
{
  FILE  *code_file;
  char   tmp_buf[81],tmp_data[31];
  AnsiString strtmp;

  sprintf(tmp_buf,"C:\\FTLPOS\\DATA\\INI\\Storedata_19.tm");
  // if ((code_file=fopen("c:\\abacus\\19.tm", "r")) == NULL)
  if ((code_file=fopen(tmp_buf, "r")) == NULL)
     {

      //memset(gchr_company,' ',10);
      //gchr_ten_code[10]=0;
      strtmp.sprintf("read_ten_name:�������� File Open Error %s ",tmp_buf);
      writelog(strtmp);
      sStoreNo="999999";
      sStoreName="StNotFind";
      return(0);
     }
  else
     {
      scan_fixstr(code_file, tmp_data, 16);
      tmp_data[16] = 0;

      strncpy(tmp_buf,tmp_data,10);
      tmp_buf[10]=0;
      sStoreName=tmp_buf;

      strncpy(tmp_buf,&tmp_data[10],6);
      tmp_buf[6]=0;
      sStoreNo=tmp_buf;

     }
   fclose(code_file);

 return(0);
}




//�p��2008/12/04 ���O��
int __fastcall BASIC::SumBillCSM(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt;
    AnsiString str_sale_type, stmp, s;       //���b���A

    iPayType = iPayID = iTAmt = iTCnt = 0;
    giS=giT=0;

    stmp.sprintf("SumBillCSM:���O��X�p���B�p�� (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);
    writelog(stmp);

    if (gbl_bil_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_bil->Count; i++)
        {
            s=gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"��I���O");

            iTAmt = _StrToInt( _StringSegment_EX(s, "|", 13), "��I���B" );
            stmp=_StringSegment_EX(s, "|", 12);  //��I���O�Ǹ�
            iPayID=_StrToInt(stmp,"��I���O�Ǹ�");

            if( iPayType==63 && (iPayID==8 || iPayID==9) )
              {
                stmp.sprintf("SumBillCSM:1040->%s",gtsl_bil->Strings[i] );
                writelog(stmp);
                if (str_sale_type.Pos("Z0"))
                    {
                     giT+=iTCnt;                  //���O��71����
                     giS+=iTAmt;                  //���O��71���B
                    }
                else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
                    {
                      giT-=iTCnt;                  //���O��71����
                      giS-=iTAmt;                  //���O��71���B
                    }
              }

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)


    stmp.sprintf("SumBillCSM:1040->���O��X�p���B (���B:%d)",giS);
    writelog(stmp);
    return giS;
}



//�p��2017/03/08 CPN
int __fastcall BASIC::SumBillCPN(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt, iRecycleFg;     //iRecycleFg:�^�����O
    AnsiString str_sale_type, stmp, s, sPAY_TYPE;       //sPAY_TYPE: ���b���A

    iPayType = iPayID = iTAmt = iTCnt = iRecycleFg = 0;
    giU=giV=0;

    stmp.sprintf("SumBillCPN: �D�������B�����X�p���B�p�� (1040:StartLine:%d,TotalLine:%d)",StartLine, TotalLine);
    writelog(stmp);

    if (gbl_bil_haveline)
    {

        for (int i=0; i<gtsl_bil->Count; i++)
        {
            s=gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            sPAY_TYPE = _StringSegment_EX(s, "|", 10); //��I�N��
            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"��I���O");
            stmp=_StringSegment_EX(s, "|", 12);  //��I���O�Ǹ�
            iPayID=_StrToInt(stmp,"��I���O�Ǹ�");
            iTAmt = _StrToInt( _StringSegment_EX(s, "|", 13), "��I���B" );
            iRecycleFg=_StrToInt(_StringSegment_EX(s, "|", 29), "�o�귽�^fg_29");  //�^�����O

            if( sPAY_TYPE.SubString(1,1)=="C" )
            {
                //stmp.sprintf("SumBillCPN:1040->%s",gtsl_bil->Strings[i] );
                //writelog(stmp);
                if (str_sale_type.Pos("Z0"))
                    {
                     //if( iPayID == 109 || iPayID == 001 || iPayID == 003 || (iPayID >= 901 && iPayID <= 999 ) )     //�D�������B   2017/06/26 �l�[ 001, 003 2019/12/07 (iPayID >= 901 && iPayID <= 999 )
                     if( iRecycleFg == 6 || iRecycleFg == 7)   //2018/12/12   2020/3/4  �ثe�u�w��1040  �����=6�ư���Z�u�����p��A�ݷs�W7
                        giU +=iTAmt;
                     else
                        giV +=iTAmt;
                    }
                else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
                    {
                      //if( iPayID == 109 || iPayID == 001 || iPayID == 003 || (iPayID >= 901 && iPayID <= 999 ) )    //�D�������B   2017/06/26 �l�[ 001, 003 2019/12/07 (iPayID >= 901 && iPayID <= 999 )
                      if( iRecycleFg == 6 || iRecycleFg == 7 )   //2018/12/12   2020/3/4  �ثe�u�w��1040  �����=6�ư���Z�u�����p��A�ݷs�W7
                        giU -=iTAmt;
                      else
                        giV -=iTAmt;
                    }
            }   // end of if( sPAY_TYPE.SubString(0,1)=="C" )

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)

    stmp.sprintf("SumBillCPN: �D�������B�����X�p���B�p�� 1040 (�D�������B:%d), �������B:%d)",giU, giV);
    writelog(stmp);
    return giU;
}





//�Y���ʦX�p   �Y���ʻP�P��i��
int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine)
{

    AnsiString str_sale_type;       //�P�f���A
    AnsiString str_r_type;          //�~���B�h�f���A
    AnsiString str_spc_flag,s;        //������O
    AnsiString VdcStatus,stmp,TaxFlg, sRelType, TmpBarcode2;
    int ifg_spc, tmpQty, tmpAmt, iRelType, iTmpAmt, iVDC99Cnt, iVDC99Amt;

    //���ơB���B
    giA = 0;
    giB = 0;
    giC = 0;   //(�K�|����)
    giD = 0;   //(�K�|���B)
    giE = 0;

    iVDC99Cnt=iVDC99Amt=0;
    //TStringList *t = new TStringList();
    if (gbl_rec_haveline)
    {

        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            str_r_type = _StringSegment_EX(s, "|", 38);     // ������A�Ϥ�
            TaxFlg=_StringSegment_EX(s, "|", 35);           // �|�O  '0'(�K�|) '1'(���|)

            //A1:���`�P�����    B1:�����P�����   M1:Menu�P�����
            //S0:�S�����     S1:�S���������
            tmpQty=0;
            tmpAmt=0;
            if (str_r_type.Pos("X"))
                continue;

            if( str_sale_type=="B0" || str_sale_type=="B1" || str_sale_type=="I0" || str_sale_type=="I1")   // 2012/04/17
               continue;

            str_spc_flag = _StringSegment_EX(s, "|", 10).Trim();   //������O
            ifg_spc=_StrToInt(str_spc_flag);
            //�s�u�ӫ~���A
            VdcStatus=_StringSegment_EX(s, "|", 36).Trim();

            //�s�u�ӫ~�Ϥ�
            iRelType=0;
            sRelType = VdcStatus.SubString(1,2).Trim();
            iRelType=_StrToInt(sRelType);
            TmpBarcode2="";

            /************    2012/04/01 Update,  2012/04/04 ReCover  ******************/
            if( VdcStatus.SubString(3,1).Trim()=="0" || VdcStatus.SubString(3,1).Trim()=="")  //�s�f����FG
                    continue;

            if ( str_sale_type.Pos("A") && ifg_spc==9 )  //�w��ӫ~
               { ;;}
            else if ( ifg_spc==4 || ifg_spc==5 || ifg_spc==6 || ifg_spc==9 )
               {     continue;   }

            /************             2012/04/01  Update            ******************/


            //if (iRelType==0 || iRelType==3 || iRelType==4 )
            //    continue;


            tmpQty= _StrToInt(_StringSegment_EX(s, "|", 26));      //�ƶq;
            tmpAmt= _StrToInt(_StringSegment_EX(s, "|", 29));      //���B

            if ( iRelType==11 ) // ����
               {
                 if( str_sale_type=="A0" || str_sale_type=="A1" )
                     {
                        TmpBarcode2=_StringSegment_EX(s, "|", 14);
                        if(TmpBarcode2.SubString(15,2)=="02") //��
                          {
                           iTmpAmt=_StrToInt(_StringSegment_EX(s, "|", 23));
                           tmpAmt=iTmpAmt * tmpQty;      //���

                           //stmp.sprintf("VdcInvSumdat:�Y���ʻP�P��i����:%s->���B:+%d",s.c_str(),tmpAmt);
                           //writelog(stmp);
                          }
                        else
                          {
                            continue;
                          }
                     }
                  else
                     {
                      continue;
                     }

               }



            if (str_sale_type.Pos("0"))
            {

                if(str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") )
                {
                   if( Trim(TaxFlg)=="0" )
                     {
                       giC += tmpQty;    //(�K�|����)
                       giD += tmpAmt;   //(�K�|���B)
                     }
                   else
                     {
                       giA += tmpQty; //_StrToInt(_StringSegment_EX(s, "|", 26));      //(���|����)
                       giB += tmpAmt; //_StrToInt(_StringSegment_EX(s, "|", 29));      //(���|���B)
                     }

                  if (iRelType==99)
                     {
                       iVDC99Cnt+=tmpQty;
                       iVDC99Amt+=tmpAmt;
                     }

                   //stmp.sprintf("VdcInvSumdat:�Y���ʻP�P��i��:%s->���B:+%d, Acc=%d ... VDC99(%d,%d)",
                   //              s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt );
                   //writelog(stmp);

                }
                else if ( str_r_type.Pos("R2") || str_r_type.Pos("R3"))
                {
                    if( Trim(TaxFlg)=="0" )
                     {
                       giC -= tmpQty;   //(�K�|����)
                       giD -= tmpAmt;   //(�K�|���B)
                     }
                    else
                     {
                       giA -= tmpQty; //_StrToInt(_StringSegment_EX(s, "|", 26));      //(���|����)
                       giB -= tmpAmt; //_StrToInt(_StringSegment_EX(s, "|", 29));      //(���|���B)
                     }

                    if (iRelType==99)
                     {
                       iVDC99Cnt-=tmpQty;
                       iVDC99Amt-=tmpAmt;
                     }

                   //stmp.sprintf("VdcInvSumdat:�Y���ʻP�P��i��:%s->���B:-%d, Acc=%d ... VDC99(%d,%d)",
                   //            s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt);
                   //writelog(stmp);

                }
            }    //if (str_sale_type.Pos("1"))
           else  //0001    ��
            {
               if(str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1") )
                {
                   if( Trim(TaxFlg)=="0" )
                     {
                       giC -= tmpQty;    //(�K�|����)
                       giD -= tmpAmt;   //(�K�|���B)
                     }
                   else
                     {
                       giA -= tmpQty; //_StrToInt(_StringSegment_EX(s, "|", 26));      //(���|����)
                       giB -= tmpAmt; //_StrToInt(_StringSegment_EX(s, "|", 29));      //(���|���B)
                     }


                   if (iRelType==99)
                     {
                       iVDC99Cnt-=tmpQty;
                       iVDC99Amt-=tmpAmt;
                     }

                  //stmp.sprintf("VdcInvSumdat:�Y���ʻP�P��i��:%s->���B:-%d, Acc=%d ... VDC99(%d,%d)",
                  //              s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt);
                  //writelog(stmp);

                    //t->Add(gtsl_rec->Strings[i]);
                }
                else if ( str_r_type.Pos("R2") || str_r_type.Pos("R3"))
                {
                    if( Trim(TaxFlg)=="0" )
                     {
                       giC += tmpQty;   //(�K�|����)
                       giD += tmpAmt;   //(�K�|���B)
                     }
                    else
                     {
                       giA += tmpQty; //_StrToInt(_StringSegment_EX(s, "|", 26));      //(���|����)
                       giB += tmpAmt; //_StrToInt(_StringSegment_EX(s, "|", 29));      //(���|���B)
                     }
                   if (iRelType==99)
                     {
                       iVDC99Cnt+=tmpQty;
                       iVDC99Amt+=tmpAmt;
                     }

                   //stmp.sprintf("VdcInvSumdat:�Y���ʻP�P��i��:%s->���B:+%d, Acc=%d ... VDC99(%d,%d)",
                   //              s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt );
                   //writelog(stmp);

                    //t->Add(gtsl_rec->Strings[i]);
                }

            }  // end of else  0001

        }
    }

    giE=giB+giD;

    stmp.sprintf("VdcInvSumdat:1010->�Y���ʻP�P��i��(���t03/04) (���|����/���B:%d, %d), (�K�|����/���B:%d, %d), ... VDC99Acc(%d,%d)  TotAmt=%d",
                                     giA, giB, giC, giD, iVDC99Cnt, iVDC99Amt, giB+giD );
    writelog(stmp);

    //t->SaveToFile("e:\\a.txt");
    //delete t;
    return 0;
}




//�o�귽�^���� /���f����B/���f��i��
void __fastcall BASIC::Sum040RecycleDataCnt(int StartLine, int TotalLine)
{

    int iTCnt, iTAmt, iT4, iPayType,iOtCnt,iOtAmt, iRecyClePackCnt,iRecycleFg;

    AnsiString str_sale_type,stmp,s, sPayType ;       //���b���A

    iTCnt = iTAmt = iT4 = iOtCnt = iOtAmt = 0;

    Cpn71_cnt=0;
    Cpn71_amt=0;
    Cpn72_cnt=0;
    Cpn72_amt=0;
    Cpn73_cnt=0;
    Cpn73_amt=0;
    Cpn74_cnt=0;
    Cpn74_amt=0;
    Cpn75_cnt=0;
    Cpn75_amt=0;
    Cpn76_cnt=0;
    Cpn76_amt=0;
    iRecyClePackCnt=0;
    iRecycleFg=0;

    giE = 0;    //���f����B
    giF = 0;    //���f��i��

    // 2018/12/07 Lu Update
    int iPaCpnAmt = 0;         //�ȥ��������B
    int iPaCpnCnt = 0;         //�ȥ������i��
    int iPaPresentAmt = 0;    //�ȥ�§����B
    int iPaPresentCnt = 0;    //�ȥ�§��i��

    giM=iPaCpnAmt;         //�ȥ��������B
    giN=iPaCpnCnt;         //�ȥ������i��
    giO=iPaPresentAmt;         //�ȥ��������B
    giP=iPaPresentCnt;         //�ȥ������i��

   // writelog("�R���ɮ� " + str_tmp_ini);
    if (gbl_bil_haveline)
    {
       // writelog("SumBill78: Have Bil Data" );
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_bil->Count; i++)
        {
            iRecyClePackCnt=0;
            iRecycleFg=0;
            s=gtsl_bil->Strings[i];;
            str_sale_type = _StringSegment_EX(s, "|", 8);
            sPayType=_StringSegment_EX(s, "|", 10);   //��I�N��

            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"��I���O");
            iTAmt = _StrToInt(_StringSegment_EX(s, "|", 13), "��I���B");

            iRecycleFg=_StrToInt(_StringSegment_EX(s, "|", 29), "�o�귽�^fg_29");
            iRecyClePackCnt=_StrToInt(_StringSegment_EX(s, "|", 30), "�o�귽�^����_30");

           // if (iTAmt)
             iTCnt=1;
          //  writelog("SumBill78: Bil Data str_sale_type,PayType ,"+ str_sale_type + stmp);
            if (str_sale_type.Pos("Z0"))
            {

                 if( sPayType  == "E0" ) //���f��
                   {
                      giE += iTAmt;    //���f����B
                      giF ++;          //���f��i��
                   }

                 // 2018/12/07 Lu Update   , 2019/01/14 Update
                 // if( ( sPayType  == "C0" || sPayType  == "C9" ) && iRecycleFg != 6  ) //  �ȥ������
                 if( ( sPayType  == "C0" || sPayType  == "C9" ) && iRecycleFg == 0  ) //  �ȥ������
                   {
                      iPaCpnAmt += iTAmt;    //�ȥ��������B
                      iPaCpnCnt ++;          //�ȥ������i��
                   }

                  // 2018/12/07 Lu Update
                 if( ( sPayType  == "B1" || sPayType  == "B2" ) ) //  �ȥ�§��
                   {
                      iPaPresentAmt += iTAmt;    //�ȥ�§����B
                      iPaPresentCnt ++;          //�ȥ�§��i��
                   }

                 switch(iRecycleFg)
                       {
                         //case 71:
                         //      Cpn71_cnt+=iTCnt;
                         //      Cpn71_amt+=iTAmt;
                         //     break;
                         case 1:   //�o�q��
                               //Cpn71_cnt+=iTCnt;
                               Cpn71_amt+=iRecyClePackCnt;
                              break;
                         case 2:   //�o����
                               //Cpn72_cnt+=iTCnt;
                               Cpn72_amt+=iRecyClePackCnt;
                              break;
                         case 3:   //�o���q
                               //Cpn73_cnt+=iTCnt;
                               Cpn73_amt+=iRecyClePackCnt;
                              break;
                         case 4:   //�o����
                               //Cpn74_cnt+=iTCnt;
                               Cpn74_amt+=iRecyClePackCnt;
                              break;
                         case 5:   //�o�����X
                               //Cpn75_cnt+=iTCnt;;
                               Cpn75_amt+=iRecyClePackCnt;
                              break;
                         case 8:   //�o��ʹq��  20200409 Lu
                               //Cpn75_cnt+=iTCnt;;
                               Cpn76_amt+=iRecyClePackCnt;
                              break;
                         default:
                               //Cpn71_cnt+=iTCnt;
                               //Cpn71_amt+=iRecyClePackCnt;
                               break;

                       }     // END OF SWITCH
              } // end of  if (str_sale_type.Pos("Z0"))

            else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
            {
                  if( sPayType  == "E0" ) //���f��
                   {
                      giE -= iTAmt;    //���f����B
                      giF --;          //���f��i��
                   }

                   // 2018/12/07 Lu Update   , 2019/01/14 Update
                 //if( ( sPayType  == "C0" || sPayType  == "C9" ) && iRecycleFg != 6  ) //  �ȥ������
                 if( ( sPayType  == "C0" || sPayType  == "C9" ) && iRecycleFg == 0  ) //  �ȥ������
                   {
                      iPaCpnAmt -= iTAmt;    //�ȥ��������B
                      iPaCpnCnt --;          //�ȥ������i��
                   }

                  // 2018/12/07 Lu Update
                 if( ( sPayType  == "B1" || sPayType  == "B2" ) ) //  �ȥ�§��
                   {
                      iPaPresentAmt -= iTAmt;    //�ȥ�§����B
                      iPaPresentCnt --;          //�ȥ�§��i��
                   }


                 switch(iPayType)
                       {
                         //case 71:
                         //      Cpn71_cnt-=iTCnt;                  //�����71����
                         //      Cpn71_amt-=iTAmt;                  //�����71���B
                         //     break;
                         case 1:
                               //Cpn71_cnt-=iTCnt;                  //�����71����
                               Cpn71_amt-=iRecyClePackCnt;                  //�����71���B
                              break;
                         case 2:
                               //Cpn72_cnt-=iTCnt;                  //�����71����
                               Cpn72_amt-=iRecyClePackCnt;                  //�����71���B
                              break;
                         case 3:
                               //Cpn73_cnt-=iTCnt;                  //�����71����
                               Cpn73_amt-=iRecyClePackCnt;                  //�����71���B
                              break;
                         case 4:
                               //Cpn74_cnt-=iTCnt;                  //�����71����
                               Cpn74_amt-=iRecyClePackCnt;                  //�����71���B
                              break;
                         case 5:
                               //Cpn75_cnt-=iTCnt;;                  //�����71����
                               Cpn75_amt-=iRecyClePackCnt;                  //�����71���B
                              break;
                         case 8:   //�o��ʹq��  20200409 Lu
                               //Cpn75_cnt+=iTCnt;;
                               Cpn76_amt-=iRecyClePackCnt;
                              break;

                         default:
                              //Cpn71_cnt-=iTCnt;                  //�����71����
                              //Cpn71_amt-=iRecyClePackCnt;                  //�����71���B
                              break;

                       }     // END OF SWITCH
            }   // end of  else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)


    giM=iPaCpnAmt;         //�ȥ��������B
    giN=iPaCpnCnt;         //�ȥ������i��
    giO=iPaPresentAmt;         //�ȥ��������B
    giP=iPaPresentCnt;         //�ȥ������i��
    
    StrBillPay.sprintf("%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|",
                       Cpn71_cnt,Cpn71_amt,Cpn72_cnt,Cpn72_amt,Cpn73_cnt,Cpn73_amt,
                       Cpn74_cnt,Cpn74_amt,Cpn75_cnt,Cpn75_amt,Cpn76_cnt,Cpn76_amt );


    return ;
}



//�p��q�l§��§���d��l�B��
int __fastcall BASIC::SumECP2Orher2Amt(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt, iOt2Amt;
    AnsiString str_sale_type, stmp, s, sPayType, PrcCode;       //���b���A

    iPayType = iPayID = iTAmt = iTCnt = 0;
    giA=giB=iOt2Amt=0;

    stmp.sprintf("SumECP2Orher2Amt:�p��§���d��l�B�� (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);
    writelog(stmp);

    if (gbl_bil_haveline)
    {
        //for (int i=StartLine; i<=TotalLine; i++)
        for (int i=0; i<gtsl_bil->Count; i++)
        {
            s=gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, "|", 8);
            sPayType=_StringSegment_EX(s, "|", 10);    //B3
            PrcCode=_StringSegment_EX(s, "|", 16);     //1Q04

            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"��I���O");

            iTAmt = _StrToInt( _StringSegment_EX(s, "|", 13), "��I���B" );
            iOt2Amt=_StrToInt( _StringSegment_EX(s, "|", 32), "��l�B��" );


            stmp=_StringSegment_EX(s, "|", 12);  //��I���O�Ǹ�
            iPayID=_StrToInt(stmp,"��I���O�Ǹ�");

            //if( str_sale_type=="R2" || str_sale_type=="R3" )
            //   continue;

            if( sPayType=="B3" && ( PrcCode=="1X00" || PrcCode=="1Q04" ) )   //"1Q04"
               {
                 giA+=iOt2Amt;
                 stmp.sprintf("SumECP2Orher2Amt:1040->(§���d��l�B����B:%s)",s.c_str() );
                 writelog(stmp);
               }

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)

    //giA=iOt2Amt;
    stmp.sprintf("SumECP2Orher2Amt:1040->(§���d��l�B����B:%d)",giA);
    writelog(stmp);
    return giA;
}



// �~�[����O,�Y���ʥN���I�d���p �p�n���u���ѳ����b
void __fastcall BASIC::Sub_C0InvoAmt(int StartLine, int TotalLine)
{
    //�~�[����O,�Y���ʥN���I�d���B���Ʋέp

    AnsiString str_is_rtype, str_trans_flag,Stmp, s;        //�P��κA�A����κA
    int i_trans_num = 0;                            //�P��ƶq

    //0:�@��ӫ~    1:�N��   2:�N��   3:�N�I   4:�[�Ⱦ�(�},��)�d   5:�[�Ⱦ�(�[��)

    int i_trans_flag = 0;                          //������A
    int iRelType;

    //�~�[����O, �Y���ʥN��
    giA=giB=giC=giD=giE=giF=giG=0;

    AnsiString str_good_no, str_menu_no, str_bar_name, str_item_amt;
    AnsiString str_type, str_item_qty, strRelType;

    if (gbl_rec_haveline)
    {
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            str_is_rtype = _StringSegment_EX(s, "|", 38).SubString(1,2);   // ������A�Ϥ�

            i_trans_num = _StrToInt(_StringSegment_EX(s, "|", 26) );  //�P��ƶq
            str_type = _StringSegment_EX(s, "|", 8);                  //�P�f���A

            //�s�u�ӫ~�Ϥ�
            strRelType = _StringSegment_EX(s, "|", 36).SubString(1,2);   // �s�u�ӫ~���A
            iRelType=_StrToInt(strRelType);
        
            str_item_qty=IntToStr(i_trans_num);
            str_trans_flag = _StringSegment_EX(s, "|", 10); //�N��I�Ϥ�       //SPC_FLG  ������O

            str_good_no  = _StringSegment_EX(s, "|", 17).Trim();
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //�N����,�I���O
            str_menu_no = "0000";
            str_bar_name = _StringSegment_EX(s, "|", 19).Trim();
            str_item_amt = _StringSegment_EX(s, "|", 29).Trim();;

            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  ������O

            // TaxFlg=_StringSegment_EX(s, "|", 35);           // �|�O  '0'(�K�|) '1'(���|)

            if (str_is_rtype.Pos("FF") || str_is_rtype.Pos("Z0") || str_is_rtype.Pos("Z1") || Trim(str_is_rtype)=="" )
            {
                if (str_type.Pos("C0"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�~�[����O
                             giA+=_StrToInt(str_item_amt);   //���B
                             giB+=i_trans_num;               //����
                             break;
                        case 2:     //�Y���ʥN���I�d
                        case 14:    //2019/05/21 Add
                             giC+=_StrToInt(str_item_amt);   //���B
                             giD+=i_trans_num;               //����
                             break;

                    }
                }
                else if (str_type.Pos("C1"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�~�[����O
                             giA-=_StrToInt(str_item_amt);   //���B
                             giB-=i_trans_num;               //����
                             break;
                        case 2:     //�Y���ʥN���I�d
                        case 14:    //2019/05/21 Add
                             giC-=_StrToInt(str_item_amt);   //���B
                             giD-=i_trans_num;               //����
                             break;
                    }
                }
            }
            else if ( str_is_rtype.Pos("R2") || str_is_rtype.Pos("R3") )
            {
                if (str_type.Pos("C0"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //�~�[����O
                             giA-=_StrToInt(str_item_amt);   //���B
                             giB-=i_trans_num;               //����
                             break;
                        case 2:     //�Y���ʥN���I�d
                        case 14:    //2019/05/21 Add
                             giC-=_StrToInt(str_item_amt);   //���B
                             giD-=i_trans_num;               //����
                             break;
                    }
                }
                else if (str_type.Pos("C1"))
                {
                    switch (i_trans_flag)
                    {
                       case 1:     //�~�[����O
                             giA+=_StrToInt(str_item_amt);   //���B
                             giB+=i_trans_num;               //����
                             break;
                        case 2:     //�Y���ʥN���I�d
                        case 14:    //2019/05/21 Add
                             giC+=_StrToInt(str_item_amt);   //���B
                             giD+=i_trans_num;               //����
                             break;
                    }
                }
            }
        }
    }

    
    Stmp.sprintf("Sub_C0InvoAmt C0C1: �~�[����O������(%d)(%d), �Y���ʥN���I�d(%d)(%d)",
                             giA, giB, giC, giD );

    writelog(Stmp);

}



// �I�d����O���K�|
void __fastcall BASIC::Sub_C0C1TaxInvoAmt(int StartLine, int TotalLine)
{
    //�~�[����O,�Y���ʥN���I�d���B���Ʋέp

    AnsiString str_is_rtype, str_trans_flag,Stmp, TaxFlg, s;        //�P��κA�A����κA

    //0:�@��ӫ~    1:�N��   2:�N��   3:�N�I   4:�[�Ⱦ�(�},��)�d   5:�[�Ⱦ�(�[��)

    int i_trans_flag = 0;                          //������A
    int iRelType;
    int iItemAmt,iDisSubAmt,iSubSubAmt,ifoodSubSubAmt,iItemQty,iNetItemAmt;
    //�~�[����O
    giA=giB=giC=giD=giE=giF=giG=0;

    AnsiString str_good_no, str_menu_no, str_bar_name;
    AnsiString str_type, strRelType;

    if (gbl_rec_haveline)
    {
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            str_is_rtype = _StringSegment_EX(s, "|", 38).SubString(1,2);   // ������A�Ϥ�

            str_type = _StringSegment_EX(s, "|", 8);                  //�P�f���A

            str_trans_flag = _StringSegment_EX(s, "|", 10); //�N��I�Ϥ�       //SPC_FLG  ������O

            str_good_no  = _StringSegment_EX(s, "|", 17).Trim();
            str_menu_no = "0000";
            str_bar_name = _StringSegment_EX(s, "|", 19).Trim();

            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  ������O

            iItemQty=_StrToInt(_StringSegment_EX(s, "|", 26));
            iItemAmt=_StrToInt(_StringSegment_EX(s, "|", 29));
            iDisSubAmt=_StrToInt(_StringSegment_EX(s, "|", 30));      //MM  �馩 �u�D
            iSubSubAmt=_StrToInt(_StringSegment_EX(s, "|", 31));      //�p�p�馩 �u�D
            ifoodSubSubAmt=_StrToInt(_StringSegment_EX(s, "|", 32));  //������馩 �u�D

            iNetItemAmt=iItemAmt-iDisSubAmt-iSubSubAmt-ifoodSubSubAmt;

            TaxFlg=_StringSegment_EX(s, "|", 35);           // �|�O  '0'(�K�|) '1'(���|)
            //�s�u�ӫ~�Ϥ�
            strRelType = _StringSegment_EX(s, "|", 36).SubString(1,2);   // �s�u�ӫ~���A
            iRelType=_StrToInt(strRelType);


             //���t R4
            if (str_is_rtype.Pos("FF") || str_is_rtype.Pos("Z0") || str_is_rtype.Pos("Z1") ||
                Trim(str_is_rtype)=="" )
            {
                if (str_type.Pos("C0"))
                {
                   if( Trim(TaxFlg)=="0" )
                    {
                        giA+=iNetItemAmt;   //���B   (�K�|)
                        //giB+=i_trans_num;               //�P��ƶq
                    }
                   else
                    {
                        giB+=iNetItemAmt;   //���B (���|)
                        //giD+=i_trans_num;               //�P��ƶq
                    }
                }
                else if (str_type.Pos("C1"))
                {
                    if( Trim(TaxFlg)=="0" )
                    {
                        giA-=iNetItemAmt;   //���B   (�K�|)
                        //giB+=i_trans_num;               //����
                    }
                   else
                    {
                        giB-=iNetItemAmt;   //���B (���|)
                        //giD+=i_trans_num;               //����
                    }
                }
            }
            else if ( str_is_rtype.Pos("R2") || str_is_rtype.Pos("R3") )
            {

               if (str_type.Pos("C0"))
                {
                   if( Trim(TaxFlg)=="0" )
                    {
                        giC+=iNetItemAmt;   //���B (�K�|)
                        //giB+=i_trans_num;               //����
                    }
                   else
                    {
                        giD+=iNetItemAmt;   //���B (���|)
                        //giD+=i_trans_num;               //����
                    }
                }
                else if(str_type.Pos("C1"))
                {
                    if( Trim(TaxFlg)=="0" )
                    {
                        giC-=iNetItemAmt;   //���B   (�K�|)
                        //giB+=i_trans_num;               //����
                    }
                   else
                    {
                        giD-=iNetItemAmt;   //���B (���|)
                        //giD+=i_trans_num;               //����
                    }
                }

            }
          else if ( str_is_rtype.Pos("R4")  )
            {
                if (str_type.Pos("C0"))
                {
                   if( Trim(TaxFlg)=="0" )
                    {
                        giE+=iNetItemAmt;   //���B (�K�|)

                    }
                   else
                    {
                        giF+=iNetItemAmt;   //���B (���|)

                    }
                }
                else if(str_type.Pos("C1"))
                {
                    if( Trim(TaxFlg)=="0" )
                    {
                        giE-=iNetItemAmt;   //���B   (�K�|)

                    }
                   else
                    {
                        giF-=iNetItemAmt;   //���B (���|)

                    }
                }
            }

        }
    }


    Stmp.sprintf("Sub_C0C1TaxInvoAmt C0C1: �I�d����O(�K�|:%d)(���|%d), �~�h(�K�|:%d)(���|:%d), ���L(�K�|:%d),(���|:%d) ",
                             giA, giB, giC, giD, giE, giF );

    writelog(Stmp);

}



// �I�d����O�ƶq
void __fastcall BASIC::Sub_C0C1TaxInvoQty(int StartLine, int TotalLine)
{
    //�~�[����O,�Y���ʥN���I�d���B���Ʋέp

    AnsiString str_is_rtype, str_trans_flag,Stmp, TaxFlg, s;        //�P��κA�A����κA
    int i_trans_num = 0;                            //�P��ƶq

    //0:�@��ӫ~    1:�N��   2:�N��   3:�N�I   4:�[�Ⱦ�(�},��)�d   5:�[�Ⱦ�(�[��)

    int i_trans_flag = 0;                          //������A
    int iRelType;

    //�~�[����O
    giA=giB=giC=giD=giE=giF=giG=0;

    AnsiString str_good_no, str_menu_no, str_bar_name, str_item_amt;
    AnsiString str_type, str_item_qty, strRelType;

    if (gbl_rec_haveline)
    {
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            str_is_rtype = _StringSegment_EX(s, "|", 38).SubString(1,2);   // ������A�Ϥ�

            i_trans_num = _StrToInt(_StringSegment_EX(s, "|", 26) );  //�P��ƶq
            str_type = _StringSegment_EX(s, "|", 8);                  //�P�f���A

            //�s�u�ӫ~�Ϥ�
            strRelType = _StringSegment_EX(s, "|", 36).SubString(1,2);   // �s�u�ӫ~���A
            iRelType=_StrToInt(strRelType);
        
            str_item_qty=IntToStr(i_trans_num);
            str_trans_flag = _StringSegment_EX(s, "|", 10); //�N��I�Ϥ�       //SPC_FLG  ������O

            str_good_no  = _StringSegment_EX(s, "|", 17).Trim();
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //�N����,�I���O
            str_menu_no = "0000";
            str_bar_name = _StringSegment_EX(s, "|", 19).Trim();
            str_item_amt = _StringSegment_EX(s, "|", 29).Trim();;

            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  ������O

            TaxFlg=_StringSegment_EX(s, "|", 35);           // �|�O  '0'(�K�|) '1'(���|)


            //���t R4
            if (str_is_rtype.Pos("FF") || str_is_rtype.Pos("Z0") || str_is_rtype.Pos("Z1") || Trim(str_is_rtype)=="" )
            {
                if (str_type.Pos("C0"))
                {
                   //if( Trim(TaxFlg)=="0" )
                   // {
                        //giA+=_StrToInt(str_item_amt);   //���B   (�K�|)
                        giA+=i_trans_num;               //����
                   // }

                }
                else if (str_type.Pos("C1"))
                {
                   // if( Trim(TaxFlg)=="0" )
                   // {
                        //giA-=_StrToInt(str_item_amt);   //���B   (�K�|)
                        giA-=i_trans_num;               //����
                   // }

                }
            }
            else if ( str_is_rtype.Pos("R2") || str_is_rtype.Pos("R3") )
            {

               if (str_type.Pos("C0"))
                {
                   //if( Trim(TaxFlg)=="0" )
                   // {
                        //giB+=_StrToInt(str_item_amt);   //���B (�K�|)
                        giB+=i_trans_num;               //����
                   // }

                }
                else if(str_type.Pos("C1"))
                {
                   // if( Trim(TaxFlg)=="0" )
                   // {
                        //giB-=_StrToInt(str_item_amt);   //���B   (�K�|)
                        giB-=i_trans_num;               //����
                   // }

                }

            }
        }
    }

    giC=giA-giB;
    Stmp.sprintf("Sub_C0C1TaxInvoQty C0C1: �I�d����O�ƶq(%d), �~�h(%d) Tot=%d",
                             giA, giB, giC);

    writelog(Stmp);

}



// �~�[����O,�Y���ʥN���I�d���p �H�s�u�Ϥ�����

void __fastcall BASIC::Sub_C0InvoAmt4VDC(int StartLine, int TotalLine)
{
    //�~�[����O,�Y���ʥN���I�d���B���Ʋέp

    AnsiString str_is_rtype, str_trans_flag,Stmp, s;        //�P��κA�A����κA
    int i_trans_num = 0;                            //�P��ƶq

    //0:�@��ӫ~    1:�N��   2:�N��   3:�N�I   4:�[�Ⱦ�(�},��)�d   5:�[�Ⱦ�(�[��)

    int i_trans_flag = 0;                          //������A
    int iRelType;

    //�~�[����O, �Y���ʥN��
    giA=giB=giC=giD=giE=giF=giG=0;

    AnsiString str_good_no, str_menu_no, str_bar_name, str_item_amt;
    AnsiString str_type, str_item_qty, strRelType;

    if (gbl_rec_haveline)
    {
        for (int i=0; i<gtsl_rec->Count; i++)
        {
            s=gtsl_rec->Strings[i];

            str_is_rtype = _StringSegment_EX(s, "|", 38).SubString(1,2);   // ������A�Ϥ�

            i_trans_num = _StrToInt(_StringSegment_EX(s, "|", 26) );  //�P��ƶq
            str_type = _StringSegment_EX(s, "|", 8);                  //�P�f���A

            //�s�u�ӫ~�Ϥ�
            strRelType = _StringSegment_EX(s, "|", 36).SubString(1,2);   // �s�u�ӫ~���A
            iRelType=_StrToInt(strRelType);
        
            str_item_qty=IntToStr(i_trans_num);
            str_trans_flag = _StringSegment_EX(s, "|", 10); //�N��I�Ϥ�       //SPC_FLG  ������O

            str_good_no  = _StringSegment_EX(s, "|", 17).Trim();
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //�N����,�I���O
            str_menu_no = "0000";
            str_bar_name = _StringSegment_EX(s, "|", 19).Trim();
            str_item_amt = _StringSegment_EX(s, "|", 29).Trim();;

            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  ������O

            // TaxFlg=_StringSegment_EX(s, "|", 35);           // �|�O  '0'(�K�|) '1'(���|)

            if (str_is_rtype.Pos("FF") || str_is_rtype.Pos("Z0") || str_is_rtype.Pos("Z1") || Trim(str_is_rtype)=="" )
            {
                if (str_type.Pos("C0"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA+=_StrToInt(str_item_amt);   //���B
                             giB+=i_trans_num;               //����
                             break;
                        default:
                             giC+=_StrToInt(str_item_amt);   //���B
                             giD+=i_trans_num;               //����
                             break;

                    }
                }
                else if (str_type.Pos("C1"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA-=_StrToInt(str_item_amt);   //���B
                             giB-=i_trans_num;               //����
                             break;
                        default:
                             giC-=_StrToInt(str_item_amt);   //���B
                             giD-=i_trans_num;               //����
                             break;
                    }
                }
            }
            else if ( str_is_rtype.Pos("R2") || str_is_rtype.Pos("R3") )
            {
                if (str_type.Pos("C0"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA-=_StrToInt(str_item_amt);   //���B
                             giB-=i_trans_num;               //����
                             break;
                        default:
                             giC-=_StrToInt(str_item_amt);   //���B
                             giD-=i_trans_num;               //����
                             break;
                    }
                }
                else if (str_type.Pos("C1"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA+=_StrToInt(str_item_amt);   //���B
                             giB+=i_trans_num;               //����
                             break;
                        default:
                             giC+=_StrToInt(str_item_amt);   //���B
                             giD+=i_trans_num;               //����
                             break;
                    }
                }
            }
        }
    }

    Stmp.sprintf("Sub_C0InvoAmt4VDC C0C1: C0_NVDC���B����(%d)(%d), C0_VDC���B����(%d)(%d)",
                             giA, giB, giC, giD );
    writelog(Stmp);

}

//���ȭ��汵�Z��, ���ȭ���Z���Ӫ� for ���b��
//iSumCash41:1050�{��,  iSumPresent41:1050§��+�ӫ~§��, iSumCpn41:1050�����+Other1+�������� CPN
//iSumPsRtn41:1050�l�B�� ,  iSumVisualCpnAmt:1040�L�ȥ�������B, iWinnInvoAmt:1050�o���I�����B

// int __fastcall BASIC::SumCasherRp  &&  BASIC::SumCasherRptXDT

int __fastcall BASIC::SumCasherRptXDT(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1095,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update )
{

    int iacc_invo_amt, iacc_amt01, iacc_amt02, iacc_amt03, iacc_amt04, iacc_amt05,
        iacc_amt06, iacc_amt07, iacc_amt08, iacc_amt09, iacc_amt10, iacc_amt11,
        iacc_amt12, iacc_amt13, iacc_amt14;
    int iacc_invo_cnt, iacc_cnt01, iacc_cnt02, iacc_cnt03, iacc_cnt04, iacc_cnt05,
        iacc_cnt06, iacc_cnt07, iacc_cnt08, iacc_cnt09, iacc_cnt10, iacc_cnt11,
        iacc_cnt12, iacc_cnt13, iacc_cnt14;
    int icash_short_amt, ipresent_short_amt, icpn_short_amt, itot_short_amt,
        ircv_cnt, iinvo_rtn_cnt, iinvo_void_cnt;

    int ipay_cash_amt, ipay_present_amt, ipay_cpn_amt,cashi,ipay_CldInvo800_amt, iTmp;

    int pay_other1_amt, pay_other2_amt;

    PAYCASHREC pay_cash_rec[100];


    iacc_invo_amt=iacc_amt01=iacc_amt02=iacc_amt03=iacc_amt04=iacc_amt05=
        iacc_amt06=iacc_amt07=iacc_amt08=iacc_amt09=iacc_amt10=iacc_amt11=
        iacc_amt12=iacc_amt13=iacc_amt14=0;

    iacc_invo_cnt=iacc_cnt01=iacc_cnt02=iacc_cnt03=iacc_cnt04=iacc_cnt05=
        iacc_cnt06=iacc_cnt07=iacc_cnt08=iacc_cnt09=iacc_cnt10=iacc_cnt11=
        iacc_cnt12=iacc_cnt13=iacc_cnt14=0;

    icash_short_amt=ipresent_short_amt=icpn_short_amt=itot_short_amt=
        ircv_cnt=iinvo_rtn_cnt=iinvo_void_cnt=0;

    ipay_cash_amt=ipay_present_amt=ipay_cpn_amt=0;

    pay_other1_amt=pay_other2_amt=ipay_CldInvo800_amt=0;

    AnsiString str_sale_type, strtmp, tmpSer, tmp0041Date1, tmp0041ChrName,s;       //���b���A


    s.printf("SumCasherRptXDT: ���ȭ���Z���Ӫ� StartLine=%d, iSumCash41=%d, iSumPsRtn41=%d, iSumVisualCpnAmt=%d, ifoodamt=%d, ifoodcnt=%d",
                StartLine, iSumCash41, iSumPsRtn41, iSumVisualCpnAmt, ifoodamt, ifoodcnt);

    writelog(s);

    for( cashi=0; cashi<100; cashi++)
        {
               pay_cash_rec[cashi].IndexNo=-1;
               pay_cash_rec[cashi].PayCashAmt=-9999;
        }

    int int_drop_line = (gtsl_drop->Count - 1);   //��w����
    for (int i=StartLine, cashi=0; i<=int_drop_line; i++, cashi++)
        {
           s=gtsl_drop->Strings[i];
           _StringSegment_EX(s, "|", 1);

           iTmp=_StrToInt(_StringSegment_EX(s, "|", 9));   //�����Y�w�ֿn���� ��Z�ɲ���
           if( iTmp == 0)
             {
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=0;
               continue;
              }

           //tmp0041Date1=gtsl_drop->Strings[i].SubString(1,60); // Rec_Type~sale_no
           tmp0041ChrName=_StringSegment_EX(s, "|", 58);  //  gtsl_drop->Strings[i].SubString(450,10);

           tmpSer=_StringSegment_EX(s, "|", 7);    //����Ǹ�
            //�{��
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=_StrToInt(_StringSegment_EX(s, "|", 11));
               if( pay_cash_rec[cashi].PayCashAmt >= 0)
                   ipay_cash_amt +=pay_cash_rec[cashi].PayCashAmt;
            //§��
               ipay_present_amt += _StrToInt(_StringSegment_EX(s, "|", 14));

            //�����
               ipay_cpn_amt += _StrToInt(_StringSegment_EX(s, "|", 15));

            //Cloud �����o�����B(800) ipay_CldInvo800_amt  20210601 Update
            ipay_CldInvo800_amt  += _StrToInt(_StringSegment_EX(s, "|", 18));

            //�����o�����B(500)
               pay_other1_amt += _StrToInt(_StringSegment_EX(s, "|", 19));

            //�����o�����B(1000)
               pay_other2_amt += _StrToInt(_StringSegment_EX(s, "|", 20));

            //�����o�����B(200)
               iacc_invo_amt += _StrToInt(_StringSegment_EX(s, "|", 22));

            //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
               iacc_amt01+=_StrToInt(_StringSegment_EX(s, "|", 23));
               iacc_cnt01+=_StrToInt(_StringSegment_EX(s, "|", 24));

               iacc_amt02+=_StrToInt(_StringSegment_EX(s, "|", 25));
               iacc_cnt02+=_StrToInt(_StringSegment_EX(s, "|", 26));

               iacc_amt03+=_StrToInt(_StringSegment_EX(s, "|", 27));
               iacc_cnt03+=_StrToInt(_StringSegment_EX(s, "|", 28));

               iacc_amt04+=_StrToInt(_StringSegment_EX(s, "|", 29));
               iacc_cnt04+=_StrToInt(_StringSegment_EX(s, "|", 30));

               iacc_amt05+=_StrToInt(_StringSegment_EX(s, "|", 31));
               iacc_cnt05+=_StrToInt(_StringSegment_EX(s, "|", 32));

               iacc_amt06+=_StrToInt(_StringSegment_EX(s, "|", 33));
               iacc_cnt06+=_StrToInt(_StringSegment_EX(s, "|", 34));

               iacc_amt07+=_StrToInt(_StringSegment_EX(s, "|", 35));
               iacc_cnt07+=_StrToInt(_StringSegment_EX(s, "|", 36));

               iacc_amt08+=_StrToInt(_StringSegment_EX(s, "|", 37));
               iacc_cnt08+=_StrToInt(_StringSegment_EX(s, "|", 38));

               iacc_amt09+=_StrToInt(_StringSegment_EX(s, "|", 39));
               iacc_cnt09+=_StrToInt(_StringSegment_EX(s, "|", 40));

               iacc_amt10+=_StrToInt(_StringSegment_EX(s, "|", 41));
               iacc_cnt10+=_StrToInt(_StringSegment_EX(s, "|", 42));

               iacc_amt11+=_StrToInt(_StringSegment_EX(s, "|", 43));
               iacc_cnt11+=_StrToInt(_StringSegment_EX(s, "|", 44));

               iacc_amt12+=_StrToInt(_StringSegment_EX(s, "|", 45));
               iacc_cnt12+=_StrToInt(_StringSegment_EX(s, "|", 46));

               iacc_amt13+=_StrToInt(_StringSegment_EX(s, "|", 47));
               iacc_cnt13+=_StrToInt(_StringSegment_EX(s, "|", 48));

               iacc_amt14+=_StrToInt(_StringSegment_EX(s, "|", 49));
               iacc_cnt14+=_StrToInt(_StringSegment_EX(s, "|", 50));


        }     //for (int i=StartLine; i<=TotalLine; i++)

     int itot_acc_amt;   //�O�γ�ڪ��B
     itot_acc_amt=iacc_amt01+iacc_amt02+iacc_amt03+iacc_amt04+iacc_amt05+
                iacc_amt06+iacc_amt07+iacc_amt08+iacc_amt09+iacc_amt10+
                iacc_amt11+iacc_amt12; //+iacc_amt13+iacc_amt14;  20151123 �אּ���w�P���f��  Charles.Wang

     iacc_amt13=ifoodamt; //���w�h�ڪ��B ;
     iacc_cnt13=ifoodcnt; //���w�h�ڦ��� ;


     //�{���u����=(�{��+�����o�����B(200+1000)+�O�γ�ڪ��B+��l�B��)-XDT�{�� ,��acc_type > 01�ɤ~����
     //�{���u����=(�{��+�O�γ�ڪ��B+��l�B��)-XDT�{�� ,��acc_type > 01�ɤ~����  // 2017/0815 Update
    //int itotSumCash41=(ipay_cash_amt+iacc_invo_amt+pay_other2_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);
    int itotSumCash41=(ipay_cash_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);
    int itotSumPresent41=ipay_present_amt-iSumPresent41; //§��u����,��acc_type > 01�ɤ~����
    int itotSumCpn41=ipay_cpn_amt-iSumCpn41;
    int itotWinInvoAmt41=(iacc_invo_amt+pay_other1_amt+pay_other2_amt+ipay_CldInvo800_amt)-iWinnInvoAmt;    // 2017/08/15 Update �����o�����B
    int iqt_cetel, iqt_R23Cnt, iqt_VCnt;
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����
    int iqt_RTicketCnt, iqt_SigCnt;
    int iRentAmt, iRemtCnt;
    sub_times(0,gi_rec_line);   //giH giI ���w�h�f���B�B����      //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_cetel=giD;
    iqt_food=giI; iamt_food=giH;

    TotMissCnt(0,gi_tot_line);

    Sum040RecycleDataCnt(0, gi_bil_line);      //�o�귽�^���� /���f����B/���f��i�� �ȥ������i��  �ȥ������i��
    iacc_amt14=giE; //���f����B
    iacc_cnt14=giF; //���f��i��

    // 2018/12/07 Lu Update
    int iPaCpnAmt = giM;         //�ȥ��������B
    int iPaCpnCnt = giN;         //�ȥ������i��
    int iPaPresentAmt = giO;    //�ȥ�§����B
    int iPaPresentCnt = giP;    //�ȥ�§��i��

    iqt_R23Cnt=giA;
    iqt_VCnt=giB;

    Rtn_Ticket(0, gbl_Ticket_haveline);
    iqt_RTicketCnt=giA+giB+giC;  ///1805, 1806, 1807 ���Ƭ�����

    Sig_Cnt(0, gi_fetc_line); //�H�Υdñ��i��
    iqt_SigCnt = giA;

    strtmp.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�:���ȭ���Z(%s) ��w����(%d), �N���i��(%d), �@�o�o���i��(%d), ��������@�o�o���i��(%d), ��l�B��(%d),���f����B(%d),���f��i��(%d)",
                   sTranCnt.SubString(53,8), int_drop_line+1, iqt_cetel, iqt_R23Cnt, iqt_VCnt, iSumPsRtn41, iacc_amt14, iacc_cnt14);
    writelog(strtmp);



     // Head1041.sprintf("1041|0500|%-6s|%2s|%14s|%05d|%-10s|%8s",
     //                   str_tencode.c_str(),
     //                   str_ecr_no.c_str(),
     //                  XDTsysDttm.c_str(),                   //x->dt_begin+x->tm_begin,
     //                   str_nz_cnt.ToInt(),
     //                   (str_trans1_5 + str_sum_tran).c_str(),
     //                   str_cashier_no.c_str()
     //                   );

    AnsiString nXDTdttb = _StringSegment_EX(sStr1095, "|", 10) + _StringSegment_EX(sStr1095, "|", 11);  //�e����Z�ɶ�
    AnsiString nXDTdttn = _StringSegment_EX(sStr1095, "|", 12) + _StringSegment_EX(sStr1095, "|", 13);  //������Z�ɶ�

    try
       {
        //1041|�q��
        RtnSumCasherData.sprintf("%s%14s%s|%04d|%04d|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%010ld|%2s|%010ld|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%04d|%010ld|%010ld|%010ld|%010ld|%04d|%04d|%04d|%-10.10s|%8s|\n",
                                         sTranCnt.SubString(1,20).c_str(),
                                         nXDTdttn.c_str(),
                                         sTranCnt.SubString(35,26).c_str(),
                                         0,
                                         0,
                                         ipay_cash_amt,
                                         0,   //pay_ic_amt
                                         0,   // pay_credit_amt
                                         ipay_present_amt,
                                         ipay_cpn_amt,
                                         0,
                                         0, //pay_bill_amt
                                         ipay_CldInvo800_amt, //pay_group_amt,//Cloud �����o�����B(800) ipay_CldInvo800_amt  20210601 Update
                                         pay_other1_amt,      //�����o�����B(500)
                                         pay_other2_amt,      //�����o�����B(1000)
                                         sXdtNo.c_str(),      //[00]:��w, [>01]:��Z�Ǹ�
                                         iacc_invo_amt,       //�����o�����B(200)
                                         iacc_amt01,
                                         iacc_cnt01,
                                         iacc_amt02,
                                         iacc_cnt02,
                                         iacc_amt03,
                                         iacc_cnt03,
                                         iacc_amt04,
                                         iacc_cnt04,
                                         iacc_amt05,
                                         iacc_cnt05,
                                         iacc_amt06,
                                         iacc_cnt06,
                                         iacc_amt07,
                                         iacc_cnt07,
                                         iacc_amt08,
                                         iacc_cnt08,
                                         iacc_amt09,
                                         iacc_cnt09,
                                         iacc_amt10,
                                         iacc_cnt10,
                                         iacc_amt11,
                                         iacc_cnt11,
                                         iacc_amt12,
                                         iacc_cnt12,
                                         iacc_amt13,    //���w�h�ڪ��B
                                         iacc_cnt13,    //���w�h�ڵ���
                                         iacc_amt14,    //���f����B
                                         iacc_cnt14,    //���f��i��
                                         itotSumCash41, //�{���u����,��acc_type > 01�ɤ~����
                                         itotSumPresent41, //§��u����,��acc_type > 01�ɤ~����
                                         itotSumCpn41, //�����u����,��acc_type > 01�ɤ~����
                                         itotSumCash41+itotSumPresent41+itotSumCpn41+itotWinInvoAmt41, //�u�����X�p,��acc_type > 01�ɤ~����
                                         iqt_cetel, //�N���i��,��acc_type > 01�ɤ~����
                                         iqt_R23Cnt, //�@�o�o���i��,��acc_type > 01�ɤ~����
                                         iqt_VCnt, //��������@�o�o���i��,��acc_type > 01�ɤ~����
                                         tmp0041ChrName.c_str(),  //10,���ȭ�
                                         "Z0FFFFFF" );  //�O�d,ver1 9byte



          //RtnSumCasherData+=sStr1097;

          strtmp.sprintf("SumCasherRptXDT::���ȭ���ZDATA->0041:%s OK",RtnSumCasherData.c_str() );
          writelog(strtmp);
        }
      catch(...)
       {
          strtmp.sprintf("SumCasherRptXDT::���ȭ���ZDATA->0041: Catch Error");
          writelog(strtmp);
       }

     // if( iAutoZ )
     //   DeleteFile(sXdtFile.c_str());



   TStringList *tslCrp;
   tslCrp = new TStringList;
   AnsiString bt,BefTime,nt,NowTime, sStoreNo, sStoreName,sLog;

   TIniFile *AutoCrpIni, *xzdata_ecr;
   AutoCrpIni = new TIniFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");
   int i1,i2, iFTLSysPrinterInitial;

    try
        {
            xzdata_ecr = new TIniFile(XZDATA_INI);
            iFTLSysPrinterInitial = xzdata_ecr->ReadInteger("ECR","FTLSysPrinterInitial",0);     //�ֿn���b���ƧǸ�
        }
    __finally
        {
            delete xzdata_ecr;
        }
   AnsiString CmdStr="";
   if( iFTLSysPrinterInitial == 1)
       CmdStr.sprintf("%c%cM080801", 27, 27);    //ESC, ESC, M , �r��[2]:08, SIZE[2]:08, �˦�[2]:00

   strtmp.sprintf("SumCasherRptXDT: ���ȭ��汵�Z���Ӫ�:XZDATA_INI->ECR->FTLSysPrinterInitial=%d, CmdStr(%s)", iFTLSysPrinterInitial, CmdStr.c_str());
   writelog(strtmp);

   sTranCnt +="|";
   bt=  nXDTdttb;
   BefTime=bt.SubString(1,4)+"-"+bt.SubString(5,2)+"-"+bt.SubString(7,2)+"  "+bt.SubString(9,2)+":"+bt.SubString(11,2);

   nt=  nXDTdttn;  //_StringSegment_EX(sTranCnt, "|", 5); // sTranCnt.SubString(21,14);  1041
   NowTime=nt.SubString(1,4)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+"  "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   read_ten_name(sStoreNo, sStoreName);

   strtmp.sprintf("%s        ���ȭ��汵�Z���Ӫ�",CmdStr.c_str()); tslCrp->Add(strtmp);
   // Vita 29175:TM_���ȭ��汵�Z���s�W�C�ܸӯZ�}�l�ɶ�
   strtmp.sprintf("%s%s �}�l",CmdStr.c_str(), BefTime);   tslCrp->Add(strtmp);
   strtmp.sprintf("%s%s ����  ����:%s",CmdStr.c_str(), NowTime,_StringSegment_EX(sTranCnt, "|", 4) );   tslCrp->Add(strtmp);
   //strtmp.sprintf("%s%s     ����:%s",CmdStr.c_str(),NowTime,_StringSegment_EX(sTranCnt, "|", 4) );   tslCrp->Add(strtmp);
   strtmp.sprintf("%s����:%s    ���W:%s",CmdStr.c_str(),_StringSegment_EX(sTranCnt, "|", 3), sStoreName);  tslCrp->Add(strtmp);
   strtmp.sprintf("%s���ȭ��s��:%s",CmdStr.c_str(),_StringSegment_EX(sTranCnt, "|", 8) );   tslCrp->Add(strtmp);
   strtmp.sprintf("%s��w����:",CmdStr.c_str());   tslCrp->Add(strtmp);

   AnsiString ReadCashStr,CashStr;
   int  ReadCashCnt=0;
   int  ReadCashtot=0;
   int  ReadCashCntIdx=0;
   CashStr="�{��|";
   //strtmp.sprintf("      �ϲ{           ��:%8ld",ipay_cash_amt);   tslCrp->Add(strtmp);

   ReadCashStr=AutoCrpIni->ReadString("AutoX","Cashdrp","0");
   ReadCashCnt=AutoCrpIni->ReadInteger("AutoX","Cashdrpcnt",0);
   ReadCashtot=AutoCrpIni->ReadInteger("AutoX","Cashdrptot",0);

   if(ReadCashCnt > 0)
     {
       strtmp.sprintf("%s  �ϲ{           ��:��.%8ld",CmdStr.c_str(), ReadCashtot );
       tslCrp->Add(strtmp);
       ReadCashCnt=1;
       ReadCashCntIdx=0;
     }


   for(cashi=0; cashi<100; cashi++)
      {
       //pay_cash_rec[cashi].IndexNo=cashi;
       //        pay_cash_rec[cashi].PayCashAmt

       if( pay_cash_rec[cashi].PayCashAmt < 0)
           break;

       if( cashi==0 && ReadCashCnt==0 )
          {

            strtmp.sprintf("%s  �ϲ{           ��:%2d.%8ld",CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt );
            tslCrp->Add(strtmp);
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
                  strtmp.sprintf("%02d|%8ld|",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
                  strtmp.sprintf("%02d|%8ld|",0 , 0);

               CashStr+=strtmp;
              }

          }
       else
          {

            strtmp.sprintf("%s                   :%2d.%8ld",CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt);
            tslCrp->Add(strtmp);
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
                 strtmp.sprintf("%02d|%8ld|",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
                      strtmp.sprintf("%02d|%8ld|",0 , 0);
               CashStr+=strtmp;
              }
          }


      }   // end of for(cashi=0; cashi<100; cashi++)

   if( Update )
         {
               if(!iAutoZ)
                  {
                    ReadCashtot=0;
                    ReadCashCnt=0;
                    CashStr="";
                  }
               AutoCrpIni->WriteString("AutoX","Cashdrp",CashStr);
               AutoCrpIni->WriteInteger("AutoX","Cashdrpcnt",ReadCashCnt);
               AutoCrpIni->WriteInteger("AutoX","Cashdrptot",ReadCashtot);
         }


   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","pres","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s�@��§�� / �ӫ~§��     :%7ld",CmdStr.c_str(),ipay_present_amt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("§��|%02d|%8ld|", 0, ipay_present_amt+i2);
      else
        strtmp.sprintf("§��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","pres",strtmp);
     }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","cpn","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �ϧ����/���Q�I��     :%7ld",CmdStr.c_str(),ipay_cpn_amt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���|%02d|%8ld|", 0, ipay_cpn_amt+i2);
      else
       strtmp.sprintf("���|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","cpn",strtmp);
     }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","invo","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(200��) :%7ld",CmdStr.c_str(),iacc_invo_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����|%02d|%8ld|", 0, iacc_invo_amt+i2);
      else
        strtmp.sprintf("����|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo",strtmp);
    }

    ////////
   strtmp=AutoCrpIni->ReadString("AutoX","invo500","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(500��) :%7ld",CmdStr.c_str(),pay_other1_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����500|%02d|%8ld|", 0, pay_other1_amt+i2);
      else
        strtmp.sprintf("����500|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo500",strtmp);
    }

    //Cloud �����o�����B(800) ipay_CldInvo800_amt  20210601 Update
   strtmp=AutoCrpIni->ReadString("AutoX","invo800","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(800��) :%7ld",CmdStr.c_str(),ipay_CldInvo800_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����800|%02d|%8ld|", 0, ipay_CldInvo800_amt+i2);
      else
        strtmp.sprintf("����800|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo800",strtmp);
    }


    ////////
   strtmp=AutoCrpIni->ReadString("AutoX","invo1000","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �Ϥ����o�����B(1000��):%7ld",CmdStr.c_str(),pay_other2_amt+i2);  tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("����1000|%02d|%8ld|", 0, pay_other2_amt+i2);
      else
        strtmp.sprintf("����1000|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","invo1000",strtmp);
    }


   ///////
   strtmp=AutoCrpIni->ReadString("AutoX","acc","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("%s  �϶O�γ�ڪ��B        :%7ld",CmdStr.c_str(),itot_acc_amt+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�O��|%02d|%8ld|", 0, itot_acc_amt+i2);
      else
       strtmp.sprintf("�O��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","acc",strtmp);
     }

   ///////

   ///////  ���w�h��    20151202 ���a��������p��A���ܰh�ڵ��ƫe
   /*strtmp=AutoCrpIni->ReadString("AutoX","food","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("      �Э��w�h��       :   %8ld",iacc_amt13+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���w|%02d|%8ld|", 0, iacc_amt13+i2);
      else
       strtmp.sprintf("���w|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","food",strtmp);
     }
   */
   ///////


   strtmp=AutoCrpIni->ReadString("AutoX","drpacc","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("------------------------------------"); tslCrp->Add(strtmp);
   strtmp.sprintf("%s  ��w�p�p         :   %8ld\n",CmdStr.c_str(),ipay_cash_amt+ipay_present_amt+ipay_cpn_amt+
                  ipay_CldInvo800_amt+iacc_invo_amt+pay_other1_amt+pay_other2_amt+itot_acc_amt+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("��w|%02d|%8ld|", 0, ipay_cash_amt+ipay_present_amt+ipay_cpn_amt+iacc_invo_amt+pay_other2_amt+itot_acc_amt+i2);
      else
        strtmp.sprintf("��w|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","drpacc",strtmp);
     }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","Cashtot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> Cashtot = %s ,�{���u����  :%8ld", strtmp, itotSumCash41+i2);
   writelog(sLog);

 //strtmp.sprintf("�{���u����  :01234567  ���ȭ�ñ��",itotSumCash41);   tslCrp->Add(strtmp);
   strtmp.sprintf("%s�{���u����  :%7ld ���ȭ�ñ��",CmdStr.c_str(),itotSumCash41+i2);   tslCrp->Add(strtmp);


   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�{�u|%02d|%8ld|", 0, itotSumCash41+i2);
      else
       strtmp.sprintf("�{�u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","Cashtot",strtmp);
    }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","prestot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> prestot = %s ,§��u����  :%8ld", strtmp, itotSumPresent41+i2);
   writelog(sLog);

   strtmp.sprintf("%s§��u����  :%7ld �z�w�w�w�w �{",CmdStr.c_str(),itotSumPresent41+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("§�u|%02d|%8ld|", 0, itotSumPresent41+i2);
      else
       strtmp.sprintf("§�u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","prestot",strtmp);
    }

   ////////
   strtmp=AutoCrpIni->ReadString("AutoX","cpntot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> cpntot = %s ,�����u����  :%8ld", strtmp, itotSumCpn41+i2);
   writelog(sLog);

   strtmp.sprintf("%s�����u����:%7ld �x         �x",CmdStr.c_str(),itotSumCpn41+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("��u|%02d|%8ld|", 0, itotSumCpn41+i2);
      else
       strtmp.sprintf("��u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","cpntot",strtmp);
     }

    ////////   �����o�����B  ////////////
   strtmp=AutoCrpIni->ReadString("AutoX","wininvotot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> wininvotot = %s , �����o���u����:%8ld", strtmp, itotWinInvoAmt41+i2 );
   writelog(sLog);

   strtmp.sprintf("%s�����o���u����:%5ld �x         �x",CmdStr.c_str(),itotWinInvoAmt41+i2);   tslCrp->Add(strtmp);
   int iTmptotWinInvoAmt41 = itotWinInvoAmt41+i2;
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���u|%02d|%8ld|", 0, itotWinInvoAmt41+i2);
      else
       strtmp.sprintf("���u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","wininvotot",strtmp);
     }

  ////////

   strtmp=AutoCrpIni->ReadString("AutoX","subtot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> subtot = %s ,�u�����X�p  :%8ld", strtmp,
                                                     itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41);
   writelog(sLog);

   strtmp.sprintf("%s�u�����X�p  :%7ld �x         �x",CmdStr.c_str(),itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
        strtmp.sprintf("�u�X|%02d|%8ld|", 0, itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41);
      else
        strtmp.sprintf("�u�X|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","subtot",strtmp);
     }

   ///////
   strtmp=AutoCrpIni->ReadString("AutoX","agncnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> agncnt = %s ,�N���i��  :%8ld", strtmp,
                                                     iqt_cetel+i2);
   writelog(sLog);

   strtmp.sprintf("%s�N���i��    :%7ld �x         �x",CmdStr.c_str(),iqt_cetel+i2);   tslCrp->Add(strtmp);
   if( Update )
      {
       if( iAutoZ )
          strtmp.sprintf("�N�i|%02d|%8ld|", 0, iqt_cetel+i2);  // 2013/10/17 Update Lu CodeView
       else
          strtmp.sprintf("�N�i|%02d|%8ld|", 0, 0);
       AutoCrpIni->WriteString("AutoX","agncnt",strtmp);
      }

    //////////////////////// 2014/12/09  ////////////////////////////////////////
   strtmp=AutoCrpIni->ReadString("AutoX","s1051cnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   AnsiString sMissRec, AutoXsMissRec, PrintAutoXsMissRec;
   AutoXsMissRec=AutoCrpIni->ReadString("AutoX","s1051cntMissRec","");

   S1051MissCnt(0,gi_1051_line, sMissRec);
   int  sMissRecCnt=giC;
   AutoXsMissRec+=sMissRec;
   PrintAutoXsMissRec=AutoXsMissRec;
   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> s1051cnt = %s ,�ȥ��@�o�o���i��  :%8ld ,AutoXsMissRec(%s)",
                                                     strtmp, giA+i2, AutoXsMissRec.c_str() );
   writelog(sLog);

   strtmp.sprintf("%s�ȥ��@�o�o��:%7ld �x         �x",CmdStr.c_str(), giA+i2 );   tslCrp->Add(strtmp);


     if( Update )
     {
      if( iAutoZ )
       {
          strtmp.sprintf("�ȼo|%02d|%8ld|", 0, giA+i2);
       }
      else
       {
         strtmp.sprintf("�ȼo|%02d|%8ld|", 0, 0);
         AutoXsMissRec="";
       }

      AutoCrpIni->WriteString("AutoX","s1051cnt",strtmp);
      AutoCrpIni->WriteString("AutoX","s1051cntMissRec",AutoXsMissRec);
     }
   ///////////////////////////////////////////////////////////////////////////
   // 2018/12/07 Lu Update
    //iPaCpnAmt �ȥ��������B
    //iPaCpnCnt �ȥ������i��
    //iPaPresentAmt �ȥ�§����B
    //iPaPresentCnt �ȥ�§��i��

    strtmp=AutoCrpIni->ReadString("AutoX","iPaPresentCnt","0");  //�ȥ�§��i��
    i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
    i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

    sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> iPaPresentCnt = %s ,�ȥ�§��i��  :%8ld", strtmp,
                                                     iPaPresentCnt+i2);
    writelog(sLog);

    strtmp.sprintf("%s�ȥ�§��i��:%7ld �x         �x",CmdStr.c_str(), iPaPresentCnt+i2 );   tslCrp->Add(strtmp);   //�|�w�w�w�w�w�}


     if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("��§|%02d|%8ld|", 0, iPaPresentCnt+i2);
      else
       strtmp.sprintf("��§|%02d|%8ld|", 0, 0);

      AutoCrpIni->WriteString("AutoX","iPaPresentCnt",strtmp);
     }

    /////////////////////////////////////////////////

    strtmp=AutoCrpIni->ReadString("AutoX","iPaCpnCnt","0");  //�ȥ������i��
    i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
    i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

    sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> iPaCpnCnt = %s ,�ȥ������i��  :%8ld", strtmp,
                                                     iPaCpnCnt+i2);
    writelog(sLog);

    strtmp.sprintf("%s�ȥ������i��:%5ld �x         �x",CmdStr.c_str(), iPaCpnCnt+i2 );   tslCrp->Add(strtmp);   //�|�w�w�w�w�w�}


     if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�ȧ�|%02d|%8ld|", 0, iPaCpnCnt+i2);
      else
       strtmp.sprintf("�ȧ�|%02d|%8ld|", 0, 0);

      AutoCrpIni->WriteString("AutoX","iPaCpnCnt",strtmp);
     }


///////////////////////////////////////////////////////////////////////////////////////////////////////


   strtmp=AutoCrpIni->ReadString("AutoX","r23cnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> r23cnt = %s ,�@�o�o���i��  :%8ld", strtmp,
                                                     iqt_R23Cnt+iqt_VCnt+i2);
   writelog(sLog);

   strtmp.sprintf("%s�@�o�o���i��:%7ld �x         �x",CmdStr.c_str(),iqt_R23Cnt+iqt_VCnt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�o�i|%02d|%8ld|", 0, iqt_R23Cnt+iqt_VCnt+i2);
      else
       strtmp.sprintf("�o�i|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","r23cnt",strtmp);
     }


 ////////////////////���w�h�ڵ���2015/12/02/////////////////////////////////////////////////


   strtmp=AutoCrpIni->ReadString("AutoX","Foodcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt13,     //���w�h�ڪ��B
   //iacc_cnt13,     //���w�h�ڵ���
   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> Foodcnt = %s ,���w�h�ڵ��� :%4ld, ���w�h�ڪ��B :%8ld,", strtmp,
                                                      iacc_cnt13+i1, iacc_amt13+i2);
   writelog(sLog);
   strtmp.sprintf("%s���w�h�ڪ��B:%7ld �x         �x",CmdStr.c_str(), iacc_amt13+i2 );   tslCrp->Add(strtmp);
   //strtmp.sprintf("���w�h���`��:%8ld  ", iacc_cnt13+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���h|%04d|%8ld|",iacc_cnt13+i1, iacc_amt13+i2);
      else
       strtmp.sprintf("���h|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","Foodcnt",strtmp);
     }


   /////////////////////////// 2015/10/22 //////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","BillAmt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt14,     //���f����B
   //iacc_cnt14,     //���f��i��

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> BillAmt = %s ,���f��i�� :%4ld, ���f����B :%8ld,", strtmp,
                                                      iacc_cnt14+i1, iacc_amt14+i2);
   writelog(sLog);
   strtmp.sprintf("%s���f����B  :%7ld �x         �x",CmdStr.c_str(), iacc_amt14+i2 );   tslCrp->Add(strtmp);
   strtmp.sprintf("%s���f��i��  :%7ld �x         �x",CmdStr.c_str(), iacc_cnt14+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���f|%04d|%8ld|", iacc_cnt14+i1, iacc_amt14+i2);
      else
       strtmp.sprintf("���f|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","BillAmt",strtmp);
     }


 ////////////////////////////// ����h���i�� //////////////////////////////////////
   strtmp=AutoCrpIni->ReadString("AutoX","rTickcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> rTickcnt = %s ,����h���i��  :%8ld", strtmp,
                                                     iqt_RTicketCnt+i2);
   writelog(sLog);

   strtmp.sprintf("%s���첼��h���i��:%3ld �x         �x",CmdStr.c_str(),iqt_RTicketCnt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("�h��|%02d|%8ld|", 0, iqt_RTicketCnt+i2);
      else
       strtmp.sprintf("�h��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","rTickcnt",strtmp);
     }

   ////////////////////////////// �H�Υdñ��i�� //////////////////////////////////////
   strtmp=AutoCrpIni->ReadString("AutoX","rSigcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> rTickcnt = %s ,�H�Υdñ��i��  :%8ld", strtmp,
                                                     iqt_SigCnt+i2);
   writelog(sLog);

   strtmp.sprintf("%s�H�Υdñ��i��:%5ld �|�w�w�w �w�}",CmdStr.c_str(),iqt_SigCnt+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("ñ��|%02d|%8ld|", 0, iqt_SigCnt+i2);
      else
       strtmp.sprintf("ñ��|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","rSigcnt",strtmp);
     }


 /////////////////////////////////////////////////////////////////////////////////////////////////


   strtmp=AutoCrpIni->ReadString("AutoX","xBtrPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   if( Cpn71_amt+i2 )
      {
         strtmp.sprintf("%s�o�q���U��  :%7ld  ",CmdStr.c_str(),Cpn71_amt+i2);   tslCrp->Add(strtmp);

         if( Update )
          {
             if( iAutoZ )
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, Cpn71_amt+i2);
             else
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, 0);
             AutoCrpIni->WriteString("AutoX","xBtrPaks",strtmp);
          }
      }

   strtmp=AutoCrpIni->ReadString("AutoX","xPhoPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   if( Cpn72_amt+i2 )
      {
         strtmp.sprintf("%s�o����U��  :%7ld  ",CmdStr.c_str(),Cpn72_amt+i2);   tslCrp->Add(strtmp);
         if( Update )
          {
             if( iAutoZ )
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, Cpn72_amt+i2);
             else
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, 0);
             AutoCrpIni->WriteString("AutoX","xPhoPaks",strtmp);
          }
      }

   strtmp=AutoCrpIni->ReadString("AutoX","xNotPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   if( Cpn73_amt+i2 )
      {
         strtmp.sprintf("%s�o���q�U��  :%7ld  ",CmdStr.c_str(),Cpn73_amt+i2);   tslCrp->Add(strtmp);
         if( Update )
          {
             if( iAutoZ )
                  strtmp.sprintf("�o�q|%02d|%8ld|", 0, Cpn73_amt+i2);
             else
                  strtmp.sprintf("�o�q|%02d|%8ld|", 0, 0);
             AutoCrpIni->WriteString("AutoX","xNotPaks",strtmp);
          }
      }

   strtmp=AutoCrpIni->ReadString("AutoX","xDskPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   if( Cpn74_amt+i2 )
      {
        strtmp.sprintf("%s�o���гU��  :%7ld  ",CmdStr.c_str(),Cpn74_amt+i2);   tslCrp->Add(strtmp);
        if( Update )
          {
             if( iAutoZ )
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, Cpn74_amt+i2);
             else
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, 0);
             AutoCrpIni->WriteString("AutoX","xDskPaks",strtmp);
          }
      }

   strtmp=AutoCrpIni->ReadString("AutoX","xInkPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   if( Cpn75_amt+i2 )
      {
         // 2019/11/27 Update  20200409 �U��
         //strtmp.sprintf("%s�o�����X�U��:%7ld  ",CmdStr.c_str(),Cpn75_amt+i2);   tslCrp->Add(strtmp);
         strtmp.sprintf("%s�o���O�U��  :%7ld  ",CmdStr.c_str(),Cpn75_amt+i2);   tslCrp->Add(strtmp);
        if( Update )
          {
             if( iAutoZ )
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, Cpn75_amt+i2);
             else
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, 0);
             AutoCrpIni->WriteString("AutoX","xInkPaks",strtmp);
          }

      }

   //Update  20200409
   strtmp=AutoCrpIni->ReadString("AutoX","xPwrPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   if( Cpn76_amt+i2 )
      {

        strtmp.sprintf("%s�o��ʹq���U��:%5ld  ",CmdStr.c_str(),Cpn76_amt+i2);   tslCrp->Add(strtmp);
        if( Update )
          {
             if( iAutoZ )
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, Cpn76_amt+i2);
             else
                  strtmp.sprintf("�o��|%02d|%8ld|", 0, 0);
             AutoCrpIni->WriteString("AutoX","xPwrPaks",strtmp);
          }

      }
 

   /***
   strtmp.sprintf("                             ���ȭ�ñ��");  tslCrp->Add(strtmp);
   strtmp.sprintf("                            �z�w�w�w�w-�{");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �x         �x");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �x         �x");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �x         �x");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            �|�w�w�w�w-�}");   tslCrp->Add(strtmp);
   ****/
   //sprintf(outfile,"  �i����ڽЩ�J��w�U�j");  msg_to_tkt(outfile);

   if( PrintAutoXsMissRec.Trim() != "")
    {
     //�ȥ��@�o�o ����  2019/04/26
     strtmp.sprintf("--------�ȥ��@�o�o������----------");  tslCrp->Add(strtmp);
     strtmp.sprintf("��  ����Ǹ�     �o�����X");  tslCrp->Add(strtmp);
     i2=0;
     while(1)
     {
     i2++;
     tmpSer=_StringSegment_EX(PrintAutoXsMissRec, "|", i2);
     if(tmpSer.Trim()=="" || i2>500 ) break;
     strtmp.sprintf("%d.%24s",i2, tmpSer );  tslCrp->Add(strtmp);

     }  // end of while

     strtmp.sprintf("(�@�o�o�����Ӥ��]�t�d�ȭ��L)");  tslCrp->Add(strtmp);
   }


   strtmp.sprintf("\n\n\n\n");  tslCrp->Add(strtmp);
   strtmp.sprintf("\x1b\x69");  tslCrp->Add(strtmp); //����

    //��X�ɮ�
   if (FileExists(SCasherRpt.c_str() ))
       DeleteFile(SCasherRpt.c_str());

   tslCrp->SaveToFile(SCasherRpt);

   /**
    RtnSumCasherCrp="";
    for (int i=0;  i<tslCrp->Count; i++)
          {
           strtmp=tslCrp->Strings[i]+"\n";
           RtnSumCasherCrp+=strtmp;
           }
    **/

   delete tslCrp;
   delete AutoCrpIni;

   //if( iAutoZ==0 ) //�D�۰ʤ鵲
   //     DeleteFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");

   return 0;
}



// 1051�q��
// �ȥ��@�o�o�����p  2019/04/23 , 2020/05/25 Update
void __fastcall BASIC::S1051MissCnt(int StartLine, int TotalLine, AnsiString &sMissRec )
{
     giA = giB = giC = giD = 0;

     AnsiString str_miss_type, tmpSer, strtmp, tmpVsNo, s ;
     int iTmpB,iTmpC,iTmpD;

     sMissRec="";
     if (gbl_1051_haveline)
     {
        for(int i=0; i<gtsl_1051_sal->Count; i++)
        {
            iTmpB = iTmpC = 0;
            s=gtsl_1051_sal->Strings[i];
            str_miss_type = _StringSegment_EX(s, "|",8);  //���b���A(V1,Z0,R2,R3...)

            tmpSer=_StringSegment_EX(s, "|",11);     //�@�o����Ǹ� 10 Byte

            strtmp=_StringSegment_EX(s, "|", 12);   //�@�o�o�����X�_ 10 Byte
            tmpVsNo=strtmp;  //.SubString(3,8);

            //  2020/05/25 Update
            strtmp=_StringSegment_EX(s, "|", 18);   //�ƥ� 20 Byte
            if(  strtmp.SubString(1,1)=="1" )
            continue;
            //////////////////////

            if( Trim(tmpVsNo)=="" || tmpVsNo.SubString(3,8)=="00000000" )
              {
               if( str_miss_type.Pos("R2") || str_miss_type.Pos("R3") )
                 {
                   giC++;
                   strtmp.sprintf("%10s   %-11s|",tmpSer.SubString(3,8) ," " );   //(0�����)
                   sMissRec += strtmp;
                 }
               //strtmp.sprintf("���}�o���@�o���:(%s)..",tmpVsNo );
               //writelog(strtmp);
               continue;
              }


            if( str_miss_type.Pos("R") )
            {
                //if( Trim(tmpVsNo)!="" && tmpVsNo!="00000000" )
                //{
                        strtmp=_StringSegment_EX(s, "|",15);
                        iTmpB=_StrToInt(strtmp.Trim() );     //�ϥθ���Ϥ�  '0'���ϥθ��� '1'�ϥθ���
                        
                        if( iTmpB ==0)
                        {
                         giA++;
                         if( str_miss_type.Pos("R2") || str_miss_type.Pos("R3") )
                         {
                            giC++;
                            strtmp.sprintf("%10s   %-2s-%-8s|",tmpSer.SubString(3,8) ,tmpVsNo.SubString(1,2),tmpVsNo.SubString(3,8)  );
                            sMissRec += strtmp;
                         }
                        }
                        else
                        {
                         giB++;
                        }
                //}
            }


        }    // end for

        strtmp.sprintf("S1051MissCnt:(R)�ȥ��@�o�o���i��(%d),(R2,R3)(%d), ����@�o�o���i��(%d), (%s)",giA,giC,giB,sMissRec.c_str() );
        writelog(strtmp);
    }
 }
