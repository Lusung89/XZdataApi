�� XzDATA.dll



XZDATA.DLL Ver: 3.18.101.1

if( Trim(SaleDataVer)=="")
        SaleDataVer="2017120100";  //Default     last:"2017080100"




XZDATA.DLL Ver: 3.17.815.1
1. if( Trim(SaleDataVer)=="")
        SaleDataVer="2017080100";  //Default     last:"2017060100"
2.
  ////////   �����o���u����  ////////////
   strtmp=AutoCrpIni->ReadString("AutoX","wininvotot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> wininvotot = %s , �����o���u����:%8ld", strtmp, itotWinInvoAmt41+i2 );
   writelog(sLog);

   strtmp.sprintf("%s�����o���u����:%5ld �x         �x",CmdStr.c_str(),itotWinInvoAmt41+i2);   tslCrp->Add(strtmp);

   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���u|%02d|%8ld|", 0, itotWinInvoAmt41+i2);
      else
       strtmp.sprintf("���u|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","wininvotot",strtmp);
     }

  ////////

3.
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


4.
   if ( str_sale_type.Pos("A") && str_spc.Pos("10") )     //�w��I���ӫ~ ������������
               {
                 //    iSubSubAmt=0;                               // 2017/07/10 �n���������� ,�ҥH Mark
                 iDisSubAmt=0;                                     // 2017/09/21 �]�w���� ��MM, �ҥH�L���A��  MM �馩 �u�D , �]��MM=MM �馩
               }








XZDATA.DLL Ver: 3.17.308.1

if( Trim(SaleDataVer)=="")
        SaleDataVer="2017010100";  //Default     last:"2016100100"

/////
SumCpn41 +=  giD;         //�����+Other1+�������� CPN
int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);


//�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giA;
    iPreSalMMDisAmt=giB;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giA;


sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");      //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    x_f += (x->tb_casher.am_vendtot+"|");      //�ɹs�X�p���B




//���ȭ���Z���Ӫ�
int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt,
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



int __fastcall BASIC::SumCasherRptXDT(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt,
                                   AnsiString sTranCnt, AnsiString sXdtNo,
                                   AnsiString SCasherRpt,
                                   AnsiString sStr1001,
                                   AnsiString sStr1097,
                                   AnsiString sStr1095,
                                   AnsiString &RtnSumCasherData,
                                   int ifoodamt,
                                   int ifoodcnt,
                                   int Update )




//�p��2017/03/08 CPN
int __fastcall BASIC::SumBillCPN(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt;
    AnsiString str_sale_type, stmp, s, sPAY_TYPE;       //���b���A

    iPayType = iPayID = iTAmt = iTCnt = 0;
    giU=giV=0;

    stmp.sprintf("SumBillCPN:CPN�X�p���B�p�� (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);
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

            if( sPAY_TYPE.SubString(0,1)=="C" )
            {
                stmp.sprintf("SumBillCPN:1040->%s",gtsl_bil->Strings[i] );
                writelog(stmp);
                if (str_sale_type.Pos("Z0"))
                    {
                     if( iPayID == 109)     //�D�������B
                        giU +=iTAmt;
                     else
                        giV +=iTAmt;
                    }
                else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
                    {
                      if( iPayID == 9)      //�D�������B
                        giU -=iTAmt;
                     else
                        giV -=iTAmt;
                    }
            }   // end of if( sPAY_TYPE.SubString(0,1)=="C" )

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)

    stmp.sprintf("SumBillCPN:1040->CPN�X�p���B�p�� (�D�������B:%d), �������B:%d)",giU, giV);
    writelog(stmp);
    return giS;
}



//�w����(�����/MM)�馩
int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int StartLine, int TotalLine);














XZDATA.DLL Ver: 3.17.105.1

���ȭ��汵�Z���Ӫ� ��j 8



XZDATA.DLL Ver: 3.16.1130.1

0024992: TFM���D/�ݨD/�W����ܧ�q��(105111801(TM_�X�y�˱i�վ�))
���ȭ��汵�Z���Ӫ�




XZDATA.DLL Ver: 3.16.1001.1

 //2016/11/03    Ver=2016100100  Sal ������s


XZDATA.DLL Ver: 3.16.0318.1
BASIC::SumCasherRptXDT
BASIC::SumCasherRpt
�����o�����B(1000)  ���ȭ��汵�Z���Ӫ�
�����o�����B(200)   ���ȭ��汵�Z���Ӫ�


XZDATA.DLL Ver: 3.16.0225.1
���p�d&Smartpay  �q�媩�����ܧ�


XZDATA.DLL Ver: 3.15.1223.2
����

XZDATA.DLL Ver: 3.15.1123.2

////////////////////���w�h�ڵ���2015/12/02///////////////////////////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","Foodcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt13,     //���w�h�ڪ��B
   //iacc_cnt13,     //���w�h�ڵ���
   sLog.sprintf("SumCasherRpt ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> Foodcnt = %s ,���w�h�ڵ��� :%4ld, ���w�h�ڪ��B :%8ld,", strtmp,
                                                      iacc_cnt13+i1, iacc_amt13+i2);
   writelog(sLog);
   strtmp.sprintf("���w�h�ڪ��B:%8ld  ", iacc_amt13+i2 );   tslCrp->Add(strtmp);
   //strtmp.sprintf("���w�h���`��:%8ld  ", iacc_cnt13+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���h|%04d|%8ld|",iacc_cnt13+i1, iacc_amt13+i2);
      else
       strtmp.sprintf("���h|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","Foodcnt",strtmp);
     }




XZDATA.DLL Ver: 3.15.1022.x

//���ȭ���Z���Ӫ�
int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,..
{

Sum040RecycleDataCnt(0, gi_bil_line);      //�o�귽�^���� /���f����B/ ���f��i��


/////////////////////////// 2015/10/22 //////////////////////////////////////////////////////////////

   strtmp=AutoCrpIni->ReadString("AutoX","BillAmt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));
   //iacc_amt14,     //���f����B
   //iacc_cnt14,     //���f��i��

   sLog.sprintf("SumCasherRptXDT ���ȭ��汵�Z���Ӫ�: AutoCrp.ini:: AutoX -> BillAmt = %s ,���f��i�� :%4ld, ���f����B :%8ld,", strtmp,
                                                      iacc_cnt14+i1, iacc_amt14+i2);
   writelog(sLog);
   strtmp.sprintf("���f����B  :%8ld  ", iacc_amt14+i2 );   tslCrp->Add(strtmp);
   strtmp.sprintf("���f��i��  :%8ld  ", iacc_cnt14+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("���f|%04d|%8ld|", iacc_cnt14+i1,iacc_amt14+i2);
      else
       strtmp.sprintf("���f|%04d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","BillAmt",strtmp);
     }
}

 /////////////////////////////////////////////////////////////////////////////////////////////////


XZDATA.DLL Ver: 3.15.819.2

1. //Lu update 20150819 for 2015070100 - 1050 �l�[�N�����O�K�|���
    if( Trim(SaleDataVer)=="")
       SaleDataVer="2015070100";  //Default     last:"2014050100"

2. int __fastcall BASIC::SumKamt(int StartLine, int TotalLine)
    //�I�d�N�����O���|:giD    Lu Update 2015/0819 
    //�I�d�N�����O�K�|:giG    Lu Update 2015/0819
    //�I�d�N�����O���|�@�o giE   Lu Update 2015/0819
    //�I�d�N�����O�K�|�@�o giH  Lu Update 2015/0819

3.int __fastcall REPORT::ZDTReport(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int rpt_type )

    // 2015/08/19 Update Lu
    StrLine.sprintf("�N��,����O�ӫȼ�%13s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(�K)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(��)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(�K)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(��)%7s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 176) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e�q�d�{���[��%6s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 69)).c_str(),
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 70)).c_str() );
    //to_ptr(StrLine);
    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e�q�d�H�Υd�[��%4s %8s   %8s"," ",
    //                                           GetNumber(_StringSegment_EX(Zdata, "|", 71)).c_str(),
    //                                           GetNumber(_StringSegment_EX(Zdata, "|", 72)).c_str() );
    //to_ptr(StrLine);

/ Ū�� 1096 data
int __fastcall REPORT::XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no,
                             int iqt5_15217totrev, int iam5_15217totrev,
                             int iqt5_18219totrev, int iam5_18219totrev )

    // 2015/08/19 Update Lu
    StrLine.sprintf("�N��,����O�ӫȼ�%13s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(�K)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(��)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(�K)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(��)%7s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 176) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e�q�d�{���[��%6s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 69)).c_str(),
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 70)).c_str() );
    //to_ptr(StrLine);
    // 2005/07/01  2015/08/19 Mark Lu
    //StrLine.sprintf("e�q�d�H�Υd�[��%4s %8s   %8s"," ",
    //                                           GetNumber(_StringSegment_EX(XRData, "|", 71)).c_str(),
    //                                           GetNumber(_StringSegment_EX(XRData, "|", 72)).c_str() );
    //to_ptr(StrLine);





int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo, String XDTsysDttm, String StrNxnoSeq,
                          String Version, String Z_date, String DDZ_1099, String ZNT_1099, String sIdx,
                          String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile )

    SumKamt(0,gi_tot_line);                          //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;         //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;         //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH

    iC0TotAmt=giD; iC0GivUpAmt=giE;    //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;


    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���     2015/08/19 �N��,����O�P����B(�K)
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B     2015/08/19 �N��,����O�@�o���B(�K)
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0
    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //�{���d����   �q�l������  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //�{���d���B   �q�l�����B  ��d�X�p 3261



//����ӫȼ�
int __fastcall BASIC::SumQt_tcust(int StartLine,int TotalLine)
{

  iCOInvoAmt=_StrToInt(_StringSegment_EX(s, "|", 49)) + _StrToInt(_StringSegment_EX(s, "|", 50)) ;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 





XZDATA.DLL Ver:3.15.212.3
//���鵲�b�֭p���B   //20150318 Mantis:0019359(�}�ʩ�)���׳q����---150315-0078���鵲�b�֭p���B�w�W�L-1000�U
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) > 99999999 || (int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) < -9000000 )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p"));



XZDATA.DLL Ver:3.15.212.2
     //2015/03/11 Mantis:019302_��Z���ѡA����۰ʤ鵲XZDATA.INI�Q�^�g
     sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
     if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }



XZDATA.DLL Ver:3.13.702.22
0014037: (Ī�w�Ӥl��)���׳q����---131012-0157...FM-012291-���ѤT�I��ZTM01 �e�����J��0 SC�S������ ���J��� ���o�����

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






XZDATA.DLL Ver:3.12.710.1
1. 
   0008079: (����������) ���׳q����---120906-0394...FM-009859-���Q�߰�SC�`�Ҥ��R��Ʋ��`
   ��w��Y����Z�A��Z��l����Ǹ��|���w����C������i���խ�l����Ǹ��C
    1. ��Z�q�夣�|�Q�Ω���R,���l����Ǹ��L�@��
    2. ��Z��l����Ǹ�=����Ǹ�

2.�o���I�����B
  int __fastcall BASIC::VoicePayAmt(int StartLine, int TotalLine)


3. ��w Max 60 ��






XZDATA.DLL Ver:3.12.705.10
0006881: �x�W�q���z�d_�[�Ȩ������b�t�AZ�b��u�[�Ⱦ��[�ȡv�������ϼW�[
else  if(str_type=="A102" || str_type=="70" || str_type=="7C")     //�[�Ȩ���   �x�W�q  �[�Ȩ���:70   �[�I:7c
        {
           --int_11_time;
           int_11_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
        }



XZDATA.DLL Ver:3.12.705.8  930 KB (952,832 �줸��)  2012�~8��29��, �U�� 09:01:58

#7787: E�q�d�[��_�����[�ȡA���h��A�[�ȡA�b��W�ȰO�۳��h�[�Ȫ��B�ɭP�b�t


////////////


XZDATA.DLL Ver:3.12.705.7  930 KB (952,832 �줸��)  2012�~8��10��, �U�� 04:31:47

#6413  �V�m�Ы�(�Ÿq�|ĳ��)���׳q����---120719-0209...FM-019940- ����:019871TM2��Z�b��ƨS���i�hSC�A�d�߰h�f�����S�������� �d�ߵo���]�L������ 
#6283   (�s�˪F�~��)���׳q����---120723-0386...FM-004362-7/22 TM1�汵�Z���P�⦬�J §�� �N�����B���O0(�禬��7/23)
#6881  �x�W�q���z�d_�[�Ȩ������b�t�AZ�b��u�[�Ⱦ��[�ȡv�������ϼW�[



XZDATA.DLL Ver:3.12.701.5  , 2012�~8��3��, �U�� 04:18:10   926 KB (948,736 �줸��)  

//�������
int __fastcall BASIC::SumVoid(int StartLine, int TotalLine)
{
    AnsiString str_type,s;    //�P�⫬�A

    //���ơB���B
    giA = giB = 0;

    if (gbl_tot_haveline)
    {
        for (int i=StartLine; i<=TotalLine; i++)
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

    if(  giB > 9999999 )
       giB=9999999;

    s.sprintf("SumVoid:�������,�X�p(����=%d, ���B=%d) ",giA, giB);
    writelog(s);

    return 0;
}





XZDATA.DLL Ver:3.12.401.28   2012�~6��25��, �U�� 12:11:41   926 KB (948,224 �줸��)

1. if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;


XZDATA.DLL Ver:3.12.401.27   2012�~6��11��, �U�� 02:57:14   926 KB (948,224 �줸��)
1. 
//�Y���ʦX�p   �Y���ʻP�P��i��
int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine)
if( str_sale_type=="B0" || str_sale_type=="B1" || str_sale_type=="I0" || str_sale_type=="I1")   // 2012/04/17
               continue;



XZDATA.DLL Ver:3.12.401.25   2012�~6��4��, �U�� 11:08:28   925 KB (947,712 �줸��)

void __fastcall BASIC::init_data()
  {

        //���ȭ��b����Ǹ����   2012/06/04

        if( str_x_trans.Length() > 5)
           stmp=str_x_trans.SubString(str_x_trans.Length()-4,5);
        else
           stmp=str_x_trans;

        sprintf(gchar_x_trans, "%5.5s", stmp.c_str());
        //sprintf(gchar_x_trans, "%5s", str_x_trans.SubString(4,5).c_str());

}


int __fastcall XDATA::WriteData
    AnsiString sTtmpPTRAN_CNT;  //2012/06/04

    sTtmpPTRAN_CNT=str_trans1_5.SubString(3,3) + str_ptran_cnt;
    tif_tmp->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt)); 

       sTmp.printf("��Z����:�g�J������Z����Ǹ� tif_tmp->X->PTRAN_CNT=%s",sTtmpPTRAN_CNT.c_str() );
       writelog(sTmp );
    
//////////////////


XZDATA.DLL Ver:V3.12.401.23  2012�~5��23��, �U�� 03:20:58
1. MTS: 4127
    
   //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SALVER,                                //"2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   SalesNo.c_str(),                       //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   0,
                   0,
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );



XZDATA.DLL Ver:V3.12.401.17
1. �w�ʧ���鳡���b�ץ�
   //�����b  2005/10/01
   void __fastcall BASIC::SumAllDept(DEPT *dept)


XZDATA.DLL Ver:V3.12.401.17
//�Y���ʦX�p   �Y���ʻP�P��i��
int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine)

   if( str_sale_type=="B0" || str_sale_type=="B1" )
               continue;



XZDATA.DLL Ver:V3.12.401.15  
1. 
    sRtnSaleDataPath="C:\\FTLPOS\\MMK\\Telegram\\";
    MtnDirFile(sRtnSaleDataPath, 30);



