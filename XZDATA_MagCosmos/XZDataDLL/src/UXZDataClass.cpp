//---------------------------------------------------------------------------

#include <vcl.h>
#include "UXZDataClass.h"

//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)


//REPORT *report;     //����Class

int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo, String XDTsysDttm, String StrNxnoSeq,
                          String Version, String Z_date, String DDZ_1099, String ZNT_1099, String sIdx,
                          String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile )
{

    AnsiString sTmp, sSalesLoginDttm, str_x_first, ed_date_time, str_nz_cnt, str_YYYYMMDDZ, str_ver_num, SaleDataVer,
               str_date_z,str_sal_last_1099, str_tran_cnt, str_sum_tran, AutoXFg ;

    x->no_tm = gchar_ecr_no;                      //���Ⱦ�����
    x->no_casher = gchar_cashier;                 //���ȭ����X
    str_nz_cnt=StrZCnt;
    str_ver_num=Version;
    str_date_z=Z_date;                            //DDZ

    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���
    //Charles update 20160225 for 2016030100 - 1028 �l�[���p�d&Smart Pay���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
       SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    sTmp.printf("�� XDATA::GetXDTData �p��Z�� GetXDTData:StrZCnt=%s, StoreNO=%s, EcrNO=%s, SalesNo=%s, StrNxnoSeq=%s, Version=%s, Z_date=%s, DDZ_1099=%s, ZNT_1099=%s, RtnXRPFile=%s, RtnXRPFile=%s ",
                StrZCnt.c_str(), StoreNO.c_str(), EcrNO.c_str(), SalesNo.c_str(), StrNxnoSeq.c_str(),
                Version.c_str(), Z_date.c_str(), DDZ_1099.c_str(), ZNT_1099.c_str(), RtnXRPFile.c_str(), RtnCRPFile );

    writelog(sTmp);

    if (x->no_casher.Length() < 5)
        x->no_casher = "00001";

     sSalesLoginDttm=tif_ecr->ReadString("X","SALESLOGINDTTM","");

    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"�ثe�g�J����Ǹ�");
    str_tran_cnt.sprintf("%05d", int_sum_transcnt);

    if ((int_sum_transcnt + 1) > 99999)    //�������ȭ��b����Ǹ�
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );


    //X�b�Ǹ�  01~99
    x->no_seq = StrNxnoSeq;
    str_x_first = gpchar_x_first;    //X�b�Ĥ@��Sal(���N����SAL�Ĥ@��)

    //x->dt_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(1,8); //�e�����b���
    //x->tm_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(9,6); //�e�����b�ɶ�

    //sIdx::  01:�Ĥ@��, 10:�̫�@��,  11:�u���@��

    ed_date_time = FormatDateTime("yyyymmddhhnnss",Now());
    x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);
    x->tm_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(9,6);

    AutoXFg="1";   //0:���@�O�۰ʤ鵲����Z

    if( Trim(sIdx)=="01" )         //01:�Ĥ@��
       {
         if( sSalesLoginDttm.Trim()=="")
           sSalesLoginDttm=_StringSegment_EX(ZNT_1099, "|", 5);
         x->dt_begin=sSalesLoginDttm.SubString(1,8);       //�e�����b���
         x->tm_begin=sSalesLoginDttm.SubString(9,6);       //�e�����b�ɶ�
         x->dt_end = x->dt_begin;
         x->tm_end = "235959";
         AutoXFg="0";  //0:���@�O�۰ʤ鵲����Z

       }
    else if( Trim(sIdx)=="10" )  //10:�̫�@��
       {

              // 2018/08/20 make By Lu
              // x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);
              // x->tm_begin = "000001";
               x->dt_begin=sSalesLoginDttm.SubString(1,8);
               x->tm_begin=sSalesLoginDttm.SubString(9,6);       

               x->dt_end =  _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);  // x->dt_begin;  2018/08/20 make By Lu
               x->tm_end = ed_date_time.SubString(9,6);
               AutoXFg="1";  //0:���@�O�۰ʤ鵲����Z
               //sTmp.printf("�� XDATA::�p��Z�� 20180905 �̫�@��, (x->dt_end:%s) > (1099_Dttm:%s) �� 1099_Dttm 5����Z��END���.",
               //             x->dt_end, ZNT_1099.c_str()  );
               //writelog(sTmp);
        }
    else if( Trim(sIdx)=="11" )  //11:�u���@��
       {
         if( sSalesLoginDttm.Trim()=="")
           sSalesLoginDttm=_StringSegment_EX(ZNT_1099, "|", 5);

         // �w�������,���W�鵲  
         if( ed_date_time.SubString(1,8) >  _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8) )
           {
               sTmp.printf("�� XDATA::�p��Z�� �u���@��, (ed_date_time:%s) > (1099_Dttm:%s) �� 1099_Dttm ����Z�����.",
                            ed_date_time, _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8)  );
               writelog(sTmp);

               x->dt_begin=sSalesLoginDttm.SubString(1,8);       //�e�����b���
               x->tm_begin=sSalesLoginDttm.SubString(9,6);       //�e�����b�ɶ�
               x->dt_end = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);    // x->dt_begin;
               x->tm_end = "235959";
               AutoXFg="1";  //0:���@�O�۰ʤ鵲����Z
           }
         else
           {
             x->dt_begin=sSalesLoginDttm.SubString(1,8);
             x->tm_begin=sSalesLoginDttm.SubString(9,6);
             x->dt_end = ed_date_time.SubString(1,8);
             x->tm_end = ed_date_time.SubString(9,6);
             AutoXFg="1"; //0:���@�O�۰ʤ鵲����Z
           }
       }
    else
       {
         // 2018/08/20 make By Lu
         //x->dt_begin = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);     //�e�����b���
         //x->tm_begin = "000001";                                               //�e�����b�ɶ�
         x->dt_begin=sSalesLoginDttm.SubString(1,8);
         x->tm_begin=sSalesLoginDttm.SubString(9,6);

         x->dt_end = _StringSegment_EX(ZNT_1099, "|", 5).SubString(1,8);  // x->dt_begin;  2018/08/20 make By Lu
         x->tm_end = "235959";
         AutoXFg="0";  //0:���@�O�۰ʤ鵲����Z
       }


    AnsiString str_invo_char;                    //�o���^��r��
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.Length() < 2)
        str_invo_char = "XX";

    if (gbl_tot_haveline)
    {
        x->no_rcstart = invoice_start_end(gi_tot_line, true);    //�o���}�l���X
        x->no_rcend = invoice_start_end(gi_tot_line, false);     //�o���������X
    }
    else    //�L����o��
    {
        x->no_rcend   = "0000000000";              //�o���}�l���X
        x->no_rcstart = "0000000000";              //�o���������X
    }

    //�������b���
    //ed_date_time = FormatDateTime("yyyymmddhhnnss",Now());      //?????????
    //x->dt_end = ed_date_time.SubString(1,8);
    //x->tm_end = ed_date_time.SubString(9,6);

    //�L�}�l�ɶ��B�P�����ɶ��ۦP
    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000") || Trim(x->dt_begin) == "")
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }

    str_YYYYMMDDZ=ed_date_time.SubString(1,8)+StrNxnoSeq.Trim();

    AnsiString x_a;
    int iC0TotCount=0;
    x_a.sprintf("1095|%04d|%-6s|%-2s|%14s|%05d|%-10s|%2s|%5s|%8s|%6s|%8s|%6s|",1779,
                                        gchar_tencode,
                                        x->no_tm.c_str(),         //���Ⱦ�����
                                        x->dt_end+x->tm_end,  //ed_date_time.c_str(),   XDTsysDttm.c_str(),
                                        str_nz_cnt.ToInt(),
                                        str_YYYYMMDDZ.SubString(3,8),
                                        x->no_seq.c_str(),        //��Z�Ǹ�
                                        x->no_casher.c_str(),     //���ȭ����X
                                        x->dt_begin.c_str(),      //�e�����b���,
                                        x->tm_begin.c_str(),      //�e�����b�ɶ�
                                        x->dt_end.c_str(),        //�������b���
                                        x->tm_end.c_str()         //�������b���
                                     );

    //����ӫȼ� 20140101
    x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //����ӫȼ�
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~�� 20140101
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    x->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line) - giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;
    
     // �I�d����O���K�|
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt, R4_C01TaxAmt,R4_C01FTaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01TaxAmt=R4_C01FTaxAmt=0;
    //Sub_C0C1TaxInvoAmt(0, gi_rec_line);  2014/0315
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;


    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);
    x->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );        //�o���鵲�b�B(�K)
    x->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));           //�o���鵲�b�B(�|)
    x->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ) );        //�o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    TotMiss(0,gi_tot_line);
    x->tb_trans.qt_miss.sprintf("%08d",    giA);        //�~���o������
    x->tb_trans.am_missntx.sprintf("%08d", giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX );        //�~���o�����B(�K)
    x->tb_trans.am_misstax.sprintf("%08d", giC-RC01TaxAmt-R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //�~���o�����B(�|)
    x->tb_trans.am_miss.sprintf("%08d",    giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        //�~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //���}�o�����B, ���}�o�����B�~�h��
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtNTX + PreSal10_AmtTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    x->tb_trans.am_totsale.sprintf("%08d", int_totsale);

    AnsiString x_b;

    x_b  = (x->tb_trans.qt_tcust+"|");                      //����ӫȼ�
    x_b += (x->tb_trans.qt_trans+"|");                      //�������~��
    x_b += (x->no_rcstart+"|");                              //�o���}�l���X
    x_b += (x->no_rcend+"|");                                //�o���������X
    x_b += (x->tb_trans.am_trcpntx+"|");                    //�o���鵲�b�B(�K)
    x_b += (x->tb_trans.am_trcptax+"|");                    //�o���鵲�b�B(�|)
    x_b += (x->tb_trans.am_trcptot+"|");                    //�o���鵲�b���B
    x_b += (x->tb_trans.qt_miss+"|");                       //�~���o������
    x_b += (x->tb_trans.am_missntx+"|");                    //�~���o�����B(�K)
    x_b += (x->tb_trans.am_misstax+"|");                    //�~���o�����B(�|)
    x_b += (x->tb_trans.am_miss+"|");                       //�~���o�����B
    x_b += (x->tb_trans.am_totsale+"|");                    //�����ھP�f���J

    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC ;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    //�N��ITABLE===============================================================

    AnsiString x_c;
    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                          //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;         //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;         //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE;    //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);   //giH giI ���w�h�f���B�B����       //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    x->tb_newadd.qt_cetel.sprintf("%08d", giD);          //�N������
    x->tb_newadd.am_cetel.sprintf("%08d", giC);          //�N�����B


    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //x_c += (x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty)+"|");
            //x_c += (x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt)+"|");

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //x_c += (x->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty)+"|");
        //x_c += (x->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt)+"|");
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //�N��X�p����
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;   //2014/03/15
    //�N��X�p���B   �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);   //2014/03/15

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;

     x_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );




   // writelog( ("X:�N��I�������� �������� 720 �ثeData: " + z_c ) );
    /*******************************************************************
    for (int j=0; j<5; j++)
    {
        for (int k=0; k<9; k++)
        {
            if (k<8)    //�C��TABLE���K��ITEM
            {
                if (j == 0)     //�Ĥ@TABLE�s�N��
                {
                    if (k == 0)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giE);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giA);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else if (j == 3)    //�ĥ|TABLE�s�N��
                {
                    if (!k)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giF);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giB);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else
                {
                    x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                }
            }
            else
            {
                if (j == 0)     //�N���`�X
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giE);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giA);
                }
                else if (j == 3)    //�N�I�`�X
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giF);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giB);
                }
                else
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",0);
                }
            }
        }//for k
    }//for j
    *******************************************************************/

    //�N�{��TABLE===============================================================

    int iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);   //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩  �t������
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    x->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);       //§��^��
    x->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //§����B
    SumPresent41+=giB; //§��+�ӫ~§��
    SumPresent41+=iCSMAmt; //���O��   for  ���ȭ��汵�Z���Ӫ�

    x->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //�t�ӧ����^�� �ثe���ϥ�
    x->tb_cash.am_vndcpn.sprintf("%08d", 0);             //�t�ӧ������B

    x->tb_cash.qt_stocpn.sprintf("%08d", giC);           //�����^��  �����N�{���^��
    x->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //�������B  �����N�{�����B
    SumCpn41 +=  giD;                                    //�����+Other1+�������� CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(�D�������B:giU), �������B:giV)     25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    SumCpn41 -=  giU;


    //x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI - giT));      //�N�{���X�p�^��
    //x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ - iCSMAmt));  //�N�{���X�p���B

    x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //�N�{���X�p�^��
    x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //�N�{���X�p���B

    x->tb_cash.qt_bonout.sprintf("%08d", giG);           //�����X�p�^��
    x->tb_cash.am_bonout.sprintf("%08d", giH);           //�����X�p���B

    iTotOt2Rtn=giE;                                      //giE:�l�B����B / giF:�l�B�馸��
    iqt_ffu1=giI;                                        //�{���d�^��  �q�l������  3275
    iam_ffu1=giJ;                                        //�{���d���B  �q�l�����B  ��d�X�p 3261

    AnsiString x_d;

    x_d += (x->tb_cash.qt_bonus+"|");                //§��^��
    x_d += (x->tb_cash.am_bonus+"|");                //§����B
    x_d += (x->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��
    x_d += (x->tb_cash.am_vndcpn+"|");               //�t�ӧ������B
    x_d += (x->tb_cash.qt_stocpn+"|");               //�����^��
    x_d += (x->tb_cash.am_stocpn+"|");               //�������B
    x_d += (x->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��
    x_d += (x->tb_cash.am_totcash+"|");              //�N�{���X�p���B

    sTmp.sprintf("%08d",giF);
    x_d += (sTmp+"|");                         //�l�B�X�p�^��
    sTmp.sprintf("%08d",giE);
    x_d += (sTmp+"|");                         //�l�B�X�p���B

    x_d += (x->tb_cash.qt_bonout+"|");               //�����X�p�^��
    x_d += (x->tb_cash.am_bonout+"|");               //�����X�p���B

    //==========================================================================

    //�{���X�p �t ���O��-�l�B��
    SumCash41=SumCash(0,gi_tot_line);
    x->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    AnsiString x_e;

    x_e += (x->am_cash+"|");               //�{���X�p
    //x_e += (x->no_rcstart+"|");            //�o���}�l���X
    //x_e += (x->no_rcend+"|");              //�o���������X


    //���ȭ�����FIELD===========================================================

    AnsiString x_f;

    //�ɹs
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tb_vendfld[i].am_vend.sprintf("%08d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

        x->tb_casher.tb_vendfld[i].tm_vend.sprintf("%06d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

    x->tb_casher.qt_vendtot.sprintf("%08d",0);
    x->tb_casher.am_vendtot.sprintf("%08d",0);


    //x_f += (x->tb_casher.qt_vendtot+"|");      //�ɹs����
    sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");      //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)

    //x_f += (x->tb_casher.am_vendtot+"|");      //�ɹs�X�p���B
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    x_f += (sTmp+"|");                           // �N���ӫ~������馩  �ɹs�X�p���B

    AnsiString x_g;

    //��s
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_feedfld[i].am_feed.sprintf("%08d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

        x->tb_casher.tg_feedfld[i].tm_feed.sprintf("%06d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

    x->tb_casher.qt_feedtot.sprintf("%08d",0);
    x->tb_casher.am_feedtot.sprintf("%08d",0);

    x_g += (x->tb_casher.qt_feedtot+"|");      //��s����
    x_g += (x->tb_casher.am_feedtot+"|");      //��s�X�p���B


    AnsiString x_h;

    //��w
    int int_drop_line = (gtsl_drop->Count - 1);   //��w����

    sTmp.printf("���ͦ��ȭ���Z���Ӫ�:��w����(%d)",gtsl_drop->Count);
    writelog(sTmp );

    //��l�ƥ�����s
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_dropfld[i].am_drop.sprintf("%08d",0);    //��w���B
        x->tb_casher.tg_dropfld[i].tm_drop.sprintf("%-6s"," ");    //����ɶ�
    }

    int int_drops = 0;                            //��w����
    int int_drop_money = 0;                       //��w���B
    int int_drop_sum = 0;                         //��w�`�B
    int x_rtn_cnt=0;

    AnsiString str_drop_time, s;

    for (int i=0; i<=int_drop_line; i++)
    {
        s=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("���ͦ��ȭ���Z���Ӫ�:�D��w(%s)���",s);
             writelog(sTmp );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, "|", 11) );     //�{��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 12) );     //IC�I��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 13) );     //�H�Υd
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 14) );    //§��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 15) );    //�����
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 16) );    //�����
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 17) );    //���f��

        //�����o�����B  800   Cloud �����o�����B(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 18) );

        //�����o�����B  500
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 19) );
        //�����o�����B  1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 20) );
        //�����o�����B  200
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 22) );

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 23) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 25) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 27) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 29) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 31) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 33) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 35) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 37) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 39) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 41) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 43) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 45) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 47) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 49) );

        //�C����w���B
        x->tb_casher.tg_dropfld[int_drops].am_drop.sprintf("%08d", int_drop_money);

        int_drop_sum += int_drop_money;                         //��w�`�B

        str_drop_time = _StringSegment_EX(s, "|", 5).SubString(9,6);  //��w�ɶ�

        //�C����w�ɶ�
        x->tb_casher.tg_dropfld[int_drops].tm_drop.sprintf("%-6s", str_drop_time);

        int_drop_money = 0;
        ++int_drops;                             //��w����
        //sTmp.printf("���ͦ��ȭ���Z���Ӫ�:��w:int_drop_money=%d,int_drops=%d",
        //           int_drop_money,int_drops);
        //writelog(sTmp );

    }

    //for (int i=0; i<50; i++)
    //{
   //     x_h += (x->tb_casher.tg_dropfld[i].am_drop+"h");  //��w�`���`�X
   //     x_h += (x->tb_casher.tg_dropfld[i].tm_drop+"h");  //��w���B�`�X
    //}

    AnsiString x_i;

    x_i += (x->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        //��w����
    x_i += (x->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     //��w�X�p���B

    SumUpdat(0,gi_rec_line);
    x_i += (x->tb_casher.qt_update.sprintf("%08d", giA)+"|");               //�󥿦���
    x_i += (x->tb_casher.am_update.sprintf("%08d", giB)+"|");               //�󥿦X�p���B

    SumVoid(0,gi_tot_line);
    x_i += (x->tb_casher.qt_void.sprintf("%08d", giA)+"|");                 //�����������
    x_i += (x->tb_casher.am_void.sprintf("%08d", giB)+"|");                 //��������X�p���B

    SumInpdp(0,gi_rec_line);                                       //�����n��
    x_i += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //��ʿ�J�������ơB����13�B14
    x_i += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //��ʿ�J�������B

    x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //�}���d����

    //�p�p�馩  1011
    SumDisc(0,gi_pay_line);
    x_i += (x->tb_casher.qt_dise.sprintf("%08d", giA)+"|");                 //�馩����
    x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");                 //�馩���B ���t������ : iPreSalINVODisAmt

    pay_sub_amt(0,gi_pay_line);
    x_i += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //�P�P�馩/������
    x_i += (x->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");         //�P�P�馩/�����B


    easy_card(0,gi_easy_line);
    x_i += (x->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");                  //�[�Ⱦ��}��d����
    x_i += (x->tb_casher.am_disret1.sprintf("%08d", giB)+"|");                  //�[�Ⱦ��}��d���B
    x_i += (x->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");                  //�[�Ⱦ��[�Ȧ���
    x_i += (x->tb_casher.am_disret2.sprintf("%08d", giD)+"|");                  //�[�Ⱦ��[�Ȫ��B


    //x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //�}���d����
    //x_i += (x->tb_casher.qt_pack.sprintf("%08d" , iTotOt2Rtn)+"|"); //giE);     //�[�Ⱦ����b����   2007/12/01 ��l�B��
    //x_i += (x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"I");   //�[�Ⱦ����b���B, ���}�o�����B�~�h��

    //�l�[TABLE=================================================================


    AnsiString x_j;

    //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //��ʿ�J�������ơB����13�B14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //��ʿ�J�������B

    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //�ƥΤK����  �{�� �[�Ȧ���     2015/08/19 �N��,����O�P����B(�K)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B     2015/08/19 �N��,����O�@�o���B(�K)
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���     2015/08/19 �N��,����O�P����B(�K)   //2017/06/04 Update
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B     2015/08/19 �N��,����O�@�o���B(�K) //2017/06/04 Update

    //���w���ơB���B  int iqt_food,iamt_food;
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0

    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //�{���d����   �q�l������  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //�{���d���B   �q�l�����B  ��d�X�p 3261

    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����  �O�d
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"|"); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B  �O�d

    //�o�����B0����

    x_j += (x->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");         //�N�������O����
    x_j += (x->tb_newadd.am_parking.sprintf("%08d", 0)+"|");         //�N�������O���B
    x_j += (x->tb_newadd.qt_cetel+"|");                              //�N������FUNCTIO�b�e��
    x_j += (x->tb_newadd.am_cetel+"|");                              //�N�����B
    //x_j += (x->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix)+"|");       //�N���ӫ~�ȼ�
    //x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");     //�{���d����   �q�l������  3275
    //x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");     //�{���d���B   �q�l�����B  ��d�X�p 3261
    //x_j += (x->tb_newadd.qt_ffu2.sprintf("%08d", 0)+"|");            //���d�����d����                    3269
    //x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //���d�����d���B  2008/12/04 ���O��  3277
    x_j += (x->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //�����e�m�Ÿ�  3285
    x_j += (x->tb_newadd.tx_ver.sprintf("%6s" , str_ver_num)+"|");   //�������X
    x_j += (x->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");    //�o�����B0����

    //���}�o�����B
    x_j += (x->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //�ƥΥ|���B,  ���}�o�����B
    x_j +=(x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"|");   //���}�o���~�h���X�p

    SumLOSS(0,gi_rec_line);
    x_j += (x->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");         //TM�o��Loss����
    x_j += (x->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");         //TM�o��Loss���B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // ���d���X�p  2013/10/17  ����
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //��[�Ⱦ��[�Ȩ�������
    x_j += (x->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");   //��[�Ⱦ��[�Ȩ������B


    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��
    int iSumRelDimAgn=giB;

    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //��[�Ⱦ��[�Ȩ�������  �Y���ʥN���I�d
    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //��[�Ⱦ��[�Ȩ�������  �Y���ʥN���I�d

    x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //���O��


    //pay_sub_amt(0,gi_pay_line);
    //x_j += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //�P�P�馩/������
    //x_j += (x->tb_newadd.am_ffu6.sprintf("%08d", giB)+"|");         //�P�P�馩/�����B


    //fetc_card(0,gi_fetc_line);
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",giFetcCashAddCnt)+"|");  //�ƥΤK����  �{�� �[�Ȧ���
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",giFetcCashAddamt)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B
    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����  �O�d
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"J"); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B  �O�d

    // 2012/09/25  Update
    if( SaleDataVer >= "2012090100")
      {
          for (int i=0; i<60; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //��w���B
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //��w�ɶ�
            }

      }
    else
      {
        for (int i=0; i<30; i++)
            {
              x_h += ("00000000|");  //�ɹs���B
              x_h += ("      |");    //�ɹs�ɶ�
            }         //"000000|"

        for (int i=0; i<30; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //��w���B
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //��w�ɶ�
            }
      }

    
    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);  �Y���ʻP�P��i�����|
    x_h +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //�Y���ʻP�P��i�ȧK�|
    x_h +=sTmp;


    //�o���I�����B
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //�o���I���i��  �����o���i��
    x_h +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B  �����o�����B
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for ���ȭ���Z���Ӫ�
    //SumCash41+=giA;                // 2012/10/08 for ���ȭ���Z���Ӫ�

     //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    x_h +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�I�d����OgiD, �@�ogiE , �I�d����O�ȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt, iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)
    x_h +=sTmp;

    AnsiString str_xdt_path, str_xrp_path;   //X��ơB�b���s����|


    SumBill78(0, gi_bil_line);
    writelog("���ʹ��f����:" + StrBillPay);


    //AnsiString str_x_path;
    //DDZ_1099=str_date_z;

    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;


    //AutoXFg: 0:���@�O�۰ʤ鵲����Z
    //X����
    AnsiString xdt_1095_data;

    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%79s|\n",  2012/09/18
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%43s|\n",
    xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%8s%s%7s|\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),
                          x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          " ",
                          AutoXFg.c_str(),
                          "FFFFFFF"
                          );                      
                          
       

    sTmp.printf("XDATA::GetXDTData ���ͦ��ȭ���Z�� xdt_1095_data:(%s) Len=%d",xdt_1095_data, xdt_1095_data.Length() );
    writelog(sTmp);

    RtnXDTData=xdt_1095_data;

    //���ȭ��b��  *.XRP    String &RtnXRPData, String &RtnCRPData
    strcpy(report->char_filename, RtnXRPFile.c_str()); //  ChangeFileExt(str_x_filename,".xrp").c_str());
    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  ���ͦ��ȭ���Z��  *.XRP                ***************/
    sTmp.printf("���ͦ��ȭ���Z��:(%s)",report->char_filename );
    writelog(sTmp);

    DeleteFile(RtnXRPFile);
    report->CreateReport('x', xdt_1095_data, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 );

    //str_new_name = str_x_path;
    //str_new_name +=   ExtractFileName(report->char_filename) ;

    //DeleteFile(str_new_name);

    //if (RenameFile(report->char_filename,str_new_name))       //�h��
    //    writelog("���ͦ��ȭ��b��(XRP)�ɮ� " + str_new_name + " ->gchar_date_time="+gchar_date_time);
    //else
    //    writelog("���ͦ��ȭ��b��(XRP)�ɮץ��� " + str_new_name);


    //str_xrp_path = str_new_name;        //�g�JINI�ܼ�

    //�g�JSAL��1097Type
    AnsiString str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver,
               str_nxno_seq,
               str_ptran_cnt;

    str_rec_type = "1097";
    str_rec_len  = "0106";

    str_tencode = gchar_tencode;   //����

    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"����"));              //����
    str_sys_date = (x->dt_end + x->tm_end);                              //����ɶ�

    //int int_sum_transcnt = _StrToInt(str_tran_cnt,"�ثe�g�J1097����Ǹ�"); //�ثe�g�J97����Ǹ�
    //str_trans_cnt.sprintf("%05d", int_sum_transcnt);


    //20031215

    /**
    if ((int_sum_transcnt + 1) > 99999)
           int_sum_transcnt = 0;

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );
    str_ptran_cnt = str_sum_tran;
    **/

    //str_pxno_seq  //�W��x�b�ֿn����

    //str_nxno_seq  //�ثex����ĴX��

    // 2007/07/17
    str_z_cnt.sprintf("%02d", _StrToInt(str_nxno_seq,"��Z�Ǹ�"));

    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //���ȭ��N��

    str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //�}�l����Ǹ��U���b���}�l�Ǹ�

    str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //�����ھP�f���J
    str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF";


    AnsiString str_trans1_5, CasherRptFile, Head1041;    //����Ǹ��e5�X
    //str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    CasherRptFile=RtnCRPFile;  //ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    Head1041.sprintf("1041|0500|%-6s|%2s|%14s|%05d|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        XDTsysDttm.c_str(),                   //x->dt_begin+x->tm_begin,
                        str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );


     
    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   XDTsysDttm.c_str(),   //str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   str_cashier_no.c_str(),                //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   " ",
                   " ",
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );




    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);


    str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   XDTsysDttm.c_str(),   //str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   str_cashier_no.c_str(),                //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   0,
                   0,
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );


    AnsiString _str_tran_cnt;    //����Ǹ�
    _str_tran_cnt = (str_trans1_5 + str_sum_tran);


    //�g�J1097����Ǹ�
    AnsiString str_97="";

    str_97.sprintf("%4s|%4s|%-6s|%2s|%14s|%05d|%-10s|%8s|%10s|000%8s|%10s|%8s|\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        XDTsysDttm.c_str(),      //str_sys_date.c_str(),
                        str_nz_cnt.ToInt(),      //str_z_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );


    /***************  ���ͦ��ȭ���Z���Ӫ�  ���ȭ��汵�Z���Ӫ� ***************/

    AnsiString SumCasherRptData;
    DeleteFile(RtnCRPFile);
    SumCasherRptXDT(NAUTOZ, 0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt,Head1041, StrNxnoSeq, RtnCRPFile,
                    str_01, str_97, xdt_1095_data, SumCasherRptData, iamt_food, iqt_food, 1);
    sTmp.printf("���ͦ��ȭ���Z���Ӫ�:%s",CasherRptFile);
    writelog(sTmp );

    RtnXDTData+=SumCasherRptData;

    sTmp.printf("XDATA::GetXDTData End:StrZCnt=%s, StrNxnoSeq=%s, Z_date=%s, DDZ_1099=%s, ZNT_1099=%s, RtnXRPFile=%s, RtnXRPFile=%s  ",
                StrZCnt.c_str(), StrNxnoSeq.c_str(),  Z_date.c_str(), DDZ_1099.c_str(), ZNT_1099.c_str(), RtnXRPFile.c_str(), RtnCRPFile );

    writelog(sTmp );

     return 1;

}





//���ȭ�/��Z ��ʥ�Z
int __fastcall XDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version)
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName,true, "", "");               //���JSAL�ɡA�N�ɮר̫��A����


    AnsiString sLog;
    sLog.sprintf("XDATA::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    if (!int_reslut)
        return 0;

    AnsiString str_ver_num,str_nz_cnt, sTmp, sSalesLoginDttm;

    str_ver_num = get_version(Version);     //�����s��


    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����

    //SPOS_XRPDT *x = new SPOS_XRPDT;         //X�b�ɮ׵��c

    //�ƥ����J����l����
    Auto_Sal->Clear();
    Auto_Sal->LoadFromFile(SalFileName);
    Auto_Sal->SaveToFile("c:\\ftlpos\\xzdata\\Bak.sal");
    //���� loadsalfile(SalFileName,true, "", ""); ,�|�إ� Auto_Sal->Assign(tsl_x_data);

    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Auto_1051_sal->Assign(gtsl_1051_sal);
    Autotsl_Ticket->Assign(gtsl_Ticket);

    TStringList *Z_CntLst = new TStringList;
    TStringList *Z_CntLsttmp = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->Clear();
    ZDT_Lst->Clear();
    Z_CntLsttmp->Clear();

    if( !_GetZCntLst(Z_CntLsttmp, Auto_Sal  ) )
      {

        // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("XDATA::�]�w [X]->XOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }
        delete Z_CntLst;
        delete ZDT_Lst;
        delete Z_CntLsttmp;
        return 0;    //�d�U���i�^ return OK;
      }
    //Z_CntLst->Assign(Z_CntLsttmp);


    AnsiString str_x_filename,              //XDATA�ɦW
               str_ptran_cnt,               //�e�����b�Ǹ�
               str_date_z,                  //DataZ(����[X�b����)
               str_tran_cnt,                //�}���H�ӵ��b���B
               str_pxno_seq,                //X�b���b����
               str_nxno_seq,                //�U�����b��
               str_YYYYMMDDZ,
               SaleDataVer,
               str_xrp_filename,
               str_crp_filename,
               XDTYYYYMMDD;                 //XDTYYYYMMDD:��Z�ɶ�


    str_date_z = tif_ecr->ReadString("Z","DATEZ","");             //ddn
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );


    str_YYYYMMDDZ=tif_ecr->ReadString("Z","YYYYMMDDZ","201001010000");

    AnsiString str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;

#ifdef TM_20
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "00001");          //999999�ثe�鵲�Ǹ�  ���餣�k�s
    str_date_time = tif_ecr->ReadString("Z","pz_date", gchar_date_time);  //2003/03/08 2004/03/22 liu
    if (str_date_time.Length() < 8)
           str_date_time = FormatDateTime("yyyymmddhhnnss",Now());        //2003/03/08 liu
#else
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());        //2003/03/08 liu
#endif

    XDTYYYYMMDD=FormatDateTime("yyyymmddhhnnss",Now());
    if(str_date_z.Length() < 3)    //DATAZ=���+Z�b����  ddn
       str_date_z = (str_date_time.SubString(7,2) + "1");

    sLog.sprintf("XDATA::ECR->WriteData �����ثe XZDATA.INI ���鵲�Ǹ�:%s, ����Ǹ�:%s", str_nz_cnt, str_tran_cnt);
    writelog( sLog);

    AnsiString str_sal_last, str_sal_last_1099;        //*.sal �̫�@��
    str_sal_last = gpchar_sal_last;
    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
       SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    //if (str_tran_cnt.Length() < 5)  //�W������Ǹ�
    //2012/08/06 Mark �wINI ���̾�
    //if( Trim(str_tran_cnt)=="00000" )
    if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //����Ǹ�

    sLog.sprintf("XDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    writelog( sLog);

    str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","01");                  //�W��x�b�ֿn����

    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //�ثex����ĴX��
    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //�̦h99��X�b
        str_nxno_seq = "01";


    AnsiString str_pzno_seq;
    str_pzno_seq = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b����1~9   �����k1


    str_x_filename.sprintf("%s%02d%s%02d%02dx.xdt",
                                         XZDATADIR,
                                         atoi(gchar_ecr_no),
                                        (XDTYYYYMMDD.SubString(3,6)).c_str(),
                                         _StrToInt(str_pzno_seq),_StrToInt(str_nxno_seq) );

    str_xrp_filename=ChangeFileExt(str_x_filename,".xrp").c_str();
    str_crp_filename=ChangeFileExt(str_x_filename,".crp").c_str();


     AnsiString DDZ_1099, str_x_path;
    DDZ_1099=str_date_z;
    if(  _StrToInt(DDZ_1099.SubString(1,2),"DDZ_1099") < 1 || _StrToInt(DDZ_1099.SubString(1,2),"DDZ_1099") > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //���ȭ��b���ƥ����|
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.SubString(1,2) + "\\" + "0" + DDZ_1099.SubString(3,1) + "\\";


    //�b���ɦW
    AnsiString sTmpPzData;

    if ( Trim(str_x_path)=="" )
    {
        //�L���w���|�Ѩt����M�w

        str_x_path = "C:\\FTLPOS\\HISTORY\\";
        sTmpPzData=tif_ecr->ReadString("Z","PZ_DATE",FormatDateTime("yyyymmddhhnnss",Now())).Trim();

        if (sTmpPzData == NULL)
        {
            str_x_path += (str_date_time.SubString(7,2) + "\\01\\");
            writelog("�T�{���v�ؿ� 01:" + str_x_path +" ->str_date_time="+str_date_time);

        }
        else
        {
            str_x_path += ( sTmpPzData.SubString(7,2) + "\\0" );

            if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 1)
               { str_x_path += "1\\";
                writelog("�T�{���v�ؿ� 02:" + str_x_path  );
               }
            else
               {
                str_x_path += (tif_ecr->ReadString("Z","NZNO_SEQ","") + "\\");
                writelog("�T�{���v�ؿ� 03:" + str_x_path);
               }
        }

        tif_ecr->WriteString("Z","XPATH",str_x_path);
    }

    if (!DirectoryExists(str_x_path))
       {
        writelog("�إ߾��v�ؿ� :" + str_x_path);
        ForceDirectories(str_x_path);
       }

   AnsiString StrXDTFileName, StrXRPFileName, StrCRPFileName;
   StrXDTFileName = (str_x_path + ExtractFileName(str_x_filename) );

   StrXRPFileName = (str_x_path + ExtractFileName(str_xrp_filename) );
   if (FileExists( StrXRPFileName))
                DeleteFile( StrXRPFileName);

   StrCRPFileName = (str_x_path + ExtractFileName(str_crp_filename) );
   if (FileExists( StrCRPFileName))
                DeleteFile( StrCRPFileName);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //str_orgam_tclsacc=str_am_tclsacc;

  AnsiString SaleXDT, sRec, TmpZcnt, str_xrp, XRPFile, CRPFile, sIdx;
  SaleXDT="";
  //XRPFile=ChangeFileExt( str_x_filename, ".Xrp");     //ExtractFileName(str_z_filename)
  //CRPFile=ChangeFileExt( str_x_filename, ".Crp");


  for( int i=0; i< Z_CntLsttmp->Count; i++)
    {
       sRec=Z_CntLsttmp->Strings[i];
       TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
       GetZCNT_SalData(TmpZcnt) ;
       if( gtsl_tot->Count > 0 || gtsl_drop->Count > 0)
         {
           Z_CntLst->Add(sRec);
           sLog.sprintf("XDATA::�[�JZ_CntLst:�鵲�Ǹ�(%s)(%s)",TmpZcnt,sRec);
           writelog( sLog);
         }
    }

    if(  Z_CntLst->Count <= 0 )
    {
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("��Z���� Z_CntLst->Count <=%d, XDATA::�]�w [X]->XOPERATION=0..�w�� XZDATA.INI �Q�٭�", Z_CntLst->Count);
        writelog( sLog);
 
        sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }
           
        delete Z_CntLst;
        delete ZDT_Lst;
        delete Z_CntLsttmp;
        return 0;    //�d�U���i�^ return OK;
    }


  str_nxno_seq=str_pxno_seq;
  Z_CntLst->Sort();
  int iIdx=0;
  for( int i=0; i< Z_CntLst->Count; i++)
       {

         //sIdx::  01:�Ĥ@��, 10:�̫�@��,  11:�u���@��
         if( Z_CntLst->Count == 1 )
            sIdx="11";
         else if( i== 0 )
            sIdx="01";
         else if( i==Z_CntLst->Count-1)
            sIdx="10";
         else
            sIdx="09";

         sRec=Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);

         //XRPFile.sprintf("c:\\FtlPos\\XZDATA\\%s%s.Xrp",TmpZcnt.c_str(), str_nxno_seq.c_str() );
         //CRPFile.sprintf("c:\\FtlPos\\XZDATA\\%s%s.Crp",TmpZcnt.c_str(), str_nxno_seq.c_str() );
         XRPFile.sprintf("c:\\FtlPos\\XZDATA\\Tmp.Xrp");
         CRPFile.sprintf("c:\\FtlPos\\XZDATA\\Tmp.Crp");
         //CRPFile=ChangeFileExt( str_x_filename, ".Crp");
         GetZCNT_SalData(TmpZcnt) ;

         if( gtsl_tot->Count > 0 || gtsl_drop->Count > 0)
           {
             iIdx++;

             str_nxno_seq.sprintf("%02d", (_StrToInt(str_nxno_seq,"pxno_seq") + 1)); //�ثex����ĴX��
             if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //�̦h99��X�b
                  str_nxno_seq = "01";

             for( int i=0 ; i< 20; i++)     //INIT �N��I�������� 15-17 �N��, 18-19 �N�I
                {
                   BaseTbSubrev[i].iqt5_totrev=0;
                   BaseTbSubrev[i].iam5_totrev=0;
                   for( int j=0 ; j<10; j++)
                      {
                        BaseTbSubrev[i].it8_subrev[j].iqt_subrev=0;
                        BaseTbSubrev[i].it8_subrev[j].iam_subrev=0;
                      }
                 }

            GetXDTData(TmpZcnt, String(gchar_tencode),  String(gchar_ecr_no), String(gchar_cashier), XDTYYYYMMDD, str_nxno_seq,
                    String(str_ver_num), str_date_z, DDZ_1099, sRec, sIdx, SaleXDT, XRPFile, CRPFile  );

          // int __fastcall XDATA::GetXDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo , String StrNxnoSeq,
          //                String Version, String Z_date, String DDZ_1099, String ZNT_1099,
          //                String &RtnXDTData, String &RtnXRPFile, String &RtnCRPFile );

            sLog.sprintf("�� XDATA::���ͥ�Z��(%s, XRPFile=%s, CRPFile=%s)",SaleXDT.c_str(), XRPFile.c_str(), CRPFile.c_str() );
            writelog( sLog);

            ZDT_Lst->Add(SaleXDT);
            str_xrp+=SaleXDT;

            SaleXDT="";

            file_cat( XRPFile.c_str(), StrXRPFileName.c_str() );
            sLog.sprintf("XDATA::file_cat(%s, %s)",XRPFile.c_str(), StrXRPFileName.c_str() );
            writelog( sLog);

            file_cat( CRPFile.c_str(), StrCRPFileName.c_str() );
            sLog.sprintf("XDATA::file_cat(%s, %s)",CRPFile.c_str(), StrCRPFileName.c_str() );
            writelog( sLog);
          }
          else
          {
            sLog.sprintf("XDATA::(%s) ����X��Z��",sRec.c_str()  );
            writelog( sLog);
          }

       } // end of for( int i=0; i< Z_CntLst->Count; i++)

       /**
     sTmp=ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+ExtractFileName(ZrpFile);
     CopyFile(ZrpFile.c_str(), str_zrp_path.c_str(), false);
     sLog.sprintf("XDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
     writelog( sLog);
     DeleteFile( ZrpFile );
     sLog.sprintf("XDATA::DeleteFile(%s)",ZrpFile.c_str() );
     writelog( sLog);

     str_dpt_path=sTmp+ExtractFileName(DptFile);
     CopyFile(DptFile.c_str(), str_dpt_path.c_str(), false);
     sLog.sprintf("XDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
     writelog( sLog);
     DeleteFile( DptFile );
     sLog.sprintf("XDATA::DeleteFile(%s)",DptFile.c_str() );
     writelog( sLog);
      **/
     delete Z_CntLst;
     delete ZDT_Lst;
     delete Z_CntLsttmp;
     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    AnsiString str_xdt_path, str_xrp_path;   //X��ơB�b���s����|



    /***
    SumBill78(0, gi_bil_line);
    writelog("���ʹ��f����:" + StrBillPay);
    
    DDZ_1099=str_date_z;
    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //���ȭ��b���ƥ����|
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.SubString(1,2) + "\\" + "0" + DDZ_1099.SubString(3,1) + "\\";
    ****/


    /***
    DeleteFile(str_new_name);
    if (RenameFile(str_x_filename,str_new_name))      //�h�ɳ�����
        writelog("���ͦ��ȭ��b��(XDT)�ɮ� " + str_new_name);
    else
        writelog("���ͦ��ȭ��b��(XDT)�ɮץ��� " + str_new_name);
    ***/

    //str_xdt_path = str_new_name;                     //�g�JINI�ܼ�


    //strcpy(report->char_filename, ChangeFileExt(str_x_filename,".xrp").c_str());


    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  ���ͦ��ȭ���Z��  *.XRP                ***************/
    //report->CreateReport('x', xdt_1095_data, StrBillPay, gchar_tencode, gchar_ecr_no,
    //                     iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 );


     // ���ȭ���Z���Ӫ�
    //str_date_z          //ddn
    //str_tran_cnt    //�ثe����Ǹ�

   // AnsiString sTmp ;
   // sTmp.printf("CreateReport: ���ͦ��ȭ��b���ɮ� �N��X�p=%d",iam5_15217totrev);
   // writelog(sTmp);

   // str_new_name = str_x_path;
  //  str_new_name +=   ExtractFileName(report->char_filename) ;

  //  DeleteFile(str_new_name);

   // if (RenameFile(report->char_filename,str_new_name))       //�h��
   //     writelog("���ͦ��ȭ��b��(XRP)�ɮ� " + str_new_name + " ->gchar_date_time="+gchar_date_time);
   // else
   //     writelog("���ͦ��ȭ��b��(XRP)�ɮץ��� " + str_new_name);


    //str_xrp_path = str_new_name;        //�g�JINI�ܼ�

    //�g�JSAL��1097Type
    AnsiString str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver;

    str_rec_type = "1097";
    str_rec_len  = "0106";

    str_tencode = gchar_tencode;   //����

    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"����"));              //����
    str_sys_date = XDTYYYYMMDD; //(x->dt_end + x->tm_end);               //����ɶ�

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"�ثe�g�J1097����Ǹ�"); //�ثe�g�J97����Ǹ�
    str_trans_cnt.sprintf("%05d", int_sum_transcnt);

    AnsiString str_sum_tran;       //�������ȭ��b����Ǹ�
    //20031215


    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );

    //xxx
    //tif_tmp->WriteString("ECR","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_sum_tran;


    //str_pxno_seq  //�W��x�b�ֿn����

    //str_nxno_seq  //�ثex����ĴX��

    // 2007/07/17
    str_z_cnt.sprintf("%02d", _StrToInt(str_nxno_seq,"��Z�Ǹ�"));
    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //���ȭ��N��

    //str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //�}�l����Ǹ��U���b���}�l�Ǹ�

    str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //�����ھP�f���J
    str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF";


    AnsiString str_trans1_5, CasherRptFile;;    //����Ǹ��e5�X
    //str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;
    
    /******************
    CasherRptFile=ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    sTmp.sprintf("1041|0500|%-6s|%2s|%14s|%05d|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );

    ******************/

    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|\n",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   str_cashier_no.c_str(),                //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   " ",
                   " ",
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

    str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|\n",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   str_cashier_no.c_str(),                //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   0,
                   0,
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );


    AnsiString _str_tran_cnt;    //����Ǹ�

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);

    //�g�J1097����Ǹ�
    AnsiString str_97;
    /**
    str_97.sprintf("%4s|%4s|%-6s|%2s|%14s|%05d|%-10s|%8s|%10s|000%8s|%10s|%8s|",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.ToInt(),      //str_z_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );
    **/


    String str_sal;
    str_sal = str_01 +  str_xrp + str_99;  //str_97 + str_99;

    str_xdt_path=StrXDTFileName;
    FILE *fxdata;

    if ((fxdata = fopen(str_xdt_path.c_str(), "w")) != NULL)
    {

        fprintf(fxdata,"%s",str_sal.c_str());
        fclose(fxdata);
        writelog("���ͦ��ȭ� XDT " + str_xdt_path);
    }
    else
    {
        writelog("�L�k���ͦ��ȭ� XDT " + str_xdt_path);
        return 0;
    }

    /****   �w��� FTLPOS.exe �g�J ���nn.SAL��
     //�g�J���nn.SAL��
    if( file_cat( str_xdt_path.c_str(), SalFileName.c_str() ) < 0 )
       {
         sTmp.printf("file_cat %s to %s Error. ",str_xdt_path.c_str(), SalFileName.c_str() );
         writelog(sTmp );
         // return 0;
       }
    else
       {
         sTmp.printf("file_cat %s to %s OK. ",str_xdt_path.c_str(), SalFileName.c_str() );
         writelog(sTmp );

       }
    *****/


    //FILE *f_sale;
    //AnsiString str_write_sal;
    //str_write_sal = "C:\\FTLPOS\\XZDATA\\00000" + str_trans_cnt + ".sal";
    //if ((f_sale = fopen(str_write_sal.c_str(),"a")) != NULL)
    //{
    //    fprintf(f_sale,"%s",str_97.c_str());
    //    fclose(f_sale);

        //xdt_tmp.ini
    //    tif_tmp->WriteString("X","x0097filename", str_write_sal);
    //}
    //else
   // {
   //     tif_tmp->WriteString("X","x0097filename", " ");
   //     return 0;
   // }

    //writelog(sSal97);

    //xxx

    ///////////////////////////////////////////////////////////////////////////////////

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //�e���~��/�d�ȭ��L

    if ((int_sum_transcnt + 2) > 99999)
            int_sum_transcnt = 1;
    else
         int_sum_transcnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_transcnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    AnsiString sTtmpPTRAN_CNT;  //2012/06/04

    sTtmpPTRAN_CNT=str_trans1_5.SubString(3,3) + str_ptran_cnt;
    tif_tmp->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));

    tif_tmp->WriteString("X","PXNO_SEQ",   str_nxno_seq);
    tif_tmp->WriteString("X","SX_DATE",   (x->dt_begin + x->tm_begin));
    tif_tmp->WriteString("X","EX_DATE",   (x->dt_end + x->tm_end));
    tif_tmp->WriteString("ECR","tot_open_x", "0");
    tif_tmp->WriteString("ECR","x_pay_cnt" ,  "0");

    tif_tmp->WriteString("X","XFILEDTTM", str_sys_date);    // XDT ��DTTM
    tif_tmp->WriteString("X","XFILENAME", StrXDTFileName);  //str_xdt_path);
    tif_tmp->WriteString("X","XRPTNAME" , StrXRPFileName);
    tif_tmp->WriteString("X","CHRRPTNAME" , StrCRPFileName); //CasherRptFile);
    
    //tif_tmp->WriteString("X","SALESLOGINDTTM" , sSalesLoginDttm );

    //delete x,tif_ecr;

    sTmp.printf("��Z����:�g�J������Z����Ǹ� tif_tmp->X->PTRAN_CNT=%s",sTtmpPTRAN_CNT.c_str() );
    writelog(sTmp );

    return OK;

}





int __fastcall ZDATA::GetZDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo ,
                          String Version, String PZ_date, String Z_date, String DDZ_1099, String ZDT_1050,
                          String &RtnZDTData, AnsiString &Tclsacc,
                          String &str_zrp_path, String &str_dpt_path)
{
    int int_reslut = 0;
    AnsiString sLog, sTmp,str_ver_num, str_tran_cnt,ZDT_1050_DDZ;

    AnsiString str_z_date,                  //�{�b�ɶ�
               str_pz_date,                 //�e���ɶ�
               str_pzno_seq,                //�ثeZ�b����   �����k1
               str_pz_cnt,                  //�e���鵲�Ǹ�  ���餣�k�s
               str_nz_cnt,                  //�ثe�鵲�Ǹ�  ���餣�k�s
               str_am_tclsacc,              //�ֿnZ�b���B   ���餣�k�s
               sYYMMDDZZ,                   //Z�b���Ǹ�
               str_date_time, str_datez,    //�ثe�b���t�ήɶ�
               z_a;

    sTmp=_StringSegment_EX(ZDT_1050, "|", 7);  //����Ǹ�
    ZDT_1050_DDZ=sTmp.SubString(3,3);
    str_am_tclsacc=Tclsacc;
    str_z_date=Z_date;
    str_pz_date=PZ_date;

    str_ver_num=Version ;

    //sYYMMDDZZ.sprintf("%s%s", str_pz_date.SubString(3,6).c_str(),StrZCnt );
    sYYMMDDZZ.sprintf("%s%s", str_z_date.SubString(5,4).c_str(),StrZCnt );


    sTmp.printf("** ZDATA::GetZDTData ���ͤ鵲�b�� Version=%s, ZDT:StrZCnt=%s, PZ_date=%s, Z_date=%s, DDZ_1099=%s, Tclsacc=%s **",
                Version, StrZCnt, PZ_date, Z_date, DDZ_1099, Tclsacc  );
    writelog(sTmp );

    int i,j;
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






    z->no_tm = gchar_ecr_no;             //���Ⱦ�����

    //�鵲�Ǹ�
    z->bg_noclose.sprintf("%05d", _StrToInt(StrZCnt,"�鵲�Ǹ�"));


    AnsiString ssysdttm, s1050sysdttm;
    ssysdttm=gchar_date_time;
    s1050sysdttm=_StringSegment_EX(ZDT_1050, "|", 5);

    if( s1050sysdttm.SubString(1,8) < ssysdttm.SubString(1,8) )
      {
        z->bg_dtclose = s1050sysdttm.SubString(1,8);      //���b���
        z->bg_tmclose = "235959";                         //���b�ɶ�

        sTmp.printf("ZDATA::GetZDTData: s1050sysdttm=%s �p�� ssysdttm=%s, Set 1096 �鵲�Ǹ�=%s, ���b��� z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
        writelog(sTmp );
      }
    else
      {
        z->bg_dtclose = str_z_date.SubString(1,8);      //���b���
        z->bg_tmclose = str_z_date.SubString(9,6);      //���b�ɶ�

        sTmp.printf("ZDATA::GetZDTData: s1050sysdttm=%s �j���� ssysdttm=%s, Set 1096 �鵲�Ǹ�=%s, ���b��� z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
        writelog(sTmp );
      }



    //z->bg_dtclose = str_z_date.SubString(1,8);      //���b���
    //z->bg_tmclose = str_z_date.SubString(9,6);      //���b�ɶ�
    z->ed_dtclose = str_pz_date.SubString(1,8);     //�e�����b���
    z->ed_tmclose = str_pz_date.SubString(9,6);     //�e�����b�ɶ�


    z_a.sprintf("1096|%04d|%-6s|%-2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|",
                                      1593,
                                      gchar_tencode,
                                      z->no_tm.c_str(),           //���Ⱦ�����
                                      Z_date.c_str(),             //�{�b�ɶ�
                                      z->bg_noclose.c_str(),      //�鵲�Ǹ�
                                      sYYMMDDZZ.c_str(),          //Z�b���Ǹ�
                                      z->ed_dtclose.c_str(),      //�e�����b���
                                      z->ed_tmclose.c_str(),      //�e�����b�ɶ�
                                      z->bg_dtclose.c_str(),      //���b���
                                      z->bg_tmclose.c_str()      //���b�ɶ�
                                    );

    // ZDATA::GetZDTData 03 ������Ӣ�������  =============================================

    //����ӫȼ�
    int iC0TotCount=0; //�I�d����O�ӫȼ�
    z->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~��
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    z->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;


    // �I�d����O���K�|
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt, R4_C01TaxAmt,R4_C01FTaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01TaxAmt=R4_C01FTaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);
    z->tb_trans.am_trcpntx.sprintf("%08d",giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );       // �o���鵲�b�B(�K)
    z->tb_trans.am_trcptax.sprintf("%08d",giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));       // �o���鵲�b�B(�|)
    z->tb_trans.am_trcptot.sprintf("%08d",giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));       // �o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    sTmp.printf("ZDATA::GetZDTData: �o�����b�֭p���B=%s, �o���鵲�b���B=%d ", str_am_tclsacc.c_str(), int_tot_amt  );
    writelog(sTmp );

      //���鵲�b�֭p���B   //20150318 Mantis:0019359(�}�ʩ�)���׳q����---150315-0078���鵲�b�֭p���B�w�W�L-1000�U
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) < -9000000 )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p"));

    Tclsacc=IntToStr(int_acc_tot_amt);

    z->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")));

    TotMiss(0,gi_tot_line);
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
    z->tb_trans.qt_miss.sprintf("%08d",   giA);       //  �~���o������
    z->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX );       //  �~���o�����B(�K)
    z->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  �~���o�����B(�|)
    z->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );       //  �~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //���}�o�����B, ���}�o�����B�~�h��
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    //ZDATA::GetZDTData �����ھP�f���J
    z->tb_trans.am_totsale.sprintf("%08d", int_totsale );


    if (gbl_tot_haveline)
    {
        z->no_rcstart = invoice_start_end(gi_tot_line, true);    //�o���}�l���X
        z->no_rcend = invoice_start_end(gi_tot_line, false);     //�o���������X
    }
    else    //�L�o�����
    {
        z->no_rcstart = "0000000000";                            //�o���}�l���X
        z->no_rcend   = "0000000000";                            //�o���������X
    }


    AnsiString z_b;

    z_b += (z->tb_trans.qt_tcust+"|");             //����ӫȼ�
    z_b += (z->tb_trans.qt_trans+"|");             //�������~��
    z_b += (z->tb_trans.am_tclsacc+"|");           //���鵲�b�֭p���B
    z_b += (z->no_rcstart+"|");                   //�o���}�l���X
    z_b += (z->no_rcend+"|");                      //�o���������X
    z_b += (z->tb_trans.am_trcpntx+"|");          //�o���鵲�b�B(�K)
    z_b += (z->tb_trans.am_trcptax+"|");           //�o���鵲�b�B(�|)
    z_b += (z->tb_trans.am_trcptot+"|");          //�o���鵲�b���B
    z_b += (z->tb_trans.qt_miss+"|");              //�~���o������
    z_b += (z->tb_trans.am_missntx+"|");           //�~���o�����B(�K)
    z_b += (z->tb_trans.am_misstax+"|");           //�~���o�����B(�|)
    z_b += (z->tb_trans.am_miss+"|");              //�~���o�����B
    z_b += (z->tb_trans.am_totsale+"|");           //�����ھP�f���J


    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC ;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    /* ZDATA::GetZDTData �N��I��������       */

    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);
    int iC0TotAmt, iC0GivUpAmt;             //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);   //giH giI ���w�h�f���B�B����     //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    z->tb_newadd.qt_agnrcv.sprintf("%08d",giD);             //�N���O�Φ���
    z->tb_newadd.am_agnrcv.sprintf("%08d",giC);             //�N���O�Ϊ��B

    AnsiString z_c;
    int iAmt, iQty,k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //�N��X�p����
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //�N��X�p���B   �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;
    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

    z_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //�N�{����������=============================================
    int  iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);  //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B


    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    writelog("giS:���O����B , giT:���O��^��");

    z->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);            //§��^��
    z->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //§����B


    SumPresent41+=giB; //§��+�ӫ~§��
    SumPresent41+=iCSMAmt; //���O��   for  ���ȭ��汵�Z���Ӫ�

    z->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //�t�ӥN�{���^�� �ثe���ϥ�
    z->tb_cash.am_vndcpn.sprintf("%08d", 0);             //�t�ӥN�{�����B

    z->tb_cash.qt_stocpn.sprintf("%08d", giC);           //�����N�{���^��
    z->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //�������B �����N�{�����B
    SumCpn41 +=  giD;      //�����+Other1+�������� CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(�D�������B:giU), �������B:giV)     25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    SumCpn41 -=  giU;

    z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //�N�{���X�p�^��
    z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //�N�{���X�p���B

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //�N�{���X�p�^��
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //�N�{���X�p���B

    z->tb_cash.qt_bonout.sprintf("%08d", giG);           //�����X�p�^��
    z->tb_cash.am_bonout.sprintf("%08d", giH);           //�����X�p���B

    iTotOt2Rtn=giE;                                      //�l�B��
    iqt_ffu1=giI;                                        //�{���d�^��  �q�l�� 1847
    iam_ffu1=giJ;                                        //�{���d���B  �q�l�� 1855    ��d�X�p

    AnsiString z_d;

    z_d += (z->tb_cash.qt_bonus+"|");                //§��^��
    z_d += (z->tb_cash.am_bonus+"|");                //§����B
    z_d += (z->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��
    z_d += (z->tb_cash.am_vndcpn+"|");               //�t�ӧ������B
    z_d += (z->tb_cash.qt_stocpn+"|");               //�����^��
    z_d += (z->tb_cash.am_stocpn+"|");               //�������B
    z_d += (z->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��
    z_d += (z->tb_cash.am_totcash+"|");              //�N�{���X�p���B

    sTmp.sprintf("%08d",giF);
    z_d += (sTmp+"|");                               //�l�B�X�p�^��
    sTmp.sprintf("%08d",giE);
    z_d += (sTmp+"|");                               //�l�B�X�p���B

    z_d += (z->tb_cash.qt_bonout+"|");               //�����X�p�^��
    z_d += (z->tb_cash.am_bonout+"|");               //�����X�p���B

    //ZDATA::GetZDTData======================================================
    //�{���X�p �t ���O��

    SumCash41=SumCash(0,gi_tot_line);
    z->am_cash.sprintf("%08d",SumCash41-iTotOt2Rtn+iCSMAmt );

    // writelog("�{���X�p �t ���O��");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //�{���X�p
    //strcat(ze,z->no_rcstart.c_str());      //�o���}�l���X
    //strcat(ze,z->no_rcend.c_str());        //�o���������X
    //ze[28] = 0;


    //ZDATA::GetZDTData Z�b�����O���pTABLE      =============================================

    AnsiString z_f;

    //s1�����X�p�������
    //s2�����X�p�P����B
    //s3�����X�p�~�h����
    //s4�����X�p�~�h���B
    int s1, s2, s3, s4;

    s1 = s2 = s3 = s4 = 0;
    DEPT *dept;
    try
    {
        dept = new DEPT[20];
        //���N�ȶ�s
        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpsale.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmiss.sprintf("%08d",0);

            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //writelog("INIT ����������, �����b");
        //�p��Ҧ�����������, �����b


          if (gbl_rec_haveline)
            SumAllDept(dept);


        // writelog("�p��Ҧ�����������, �����b OK");


        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            z->tb_depcal[i].am_dpsale.sprintf("%08d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            z->tb_depcal[i].am_dpmitm.sprintf("%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            z->tb_depcal[i].am_dpmiss.sprintf("%08d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;
        }


    }
    __finally
    {
      delete [] dept;
    }

   //ZDATA::GetZDTData writelog("Z�b�����O���pTABLE OK");

    AnsiString z_g;

    z_g += (z->am_tdpsitm.sprintf("%08d", s1)+"|");             // �����X�p�������
    z_g += (z->am_dpstot.sprintf("%08d" , s2)+"|");             // �����X�p�P����B
    z_g += (z->am_tdpmitm.sprintf("%08d", s3)+"|");             // �����X�p�~�h����
    z_g += (z->am_dpmtot.sprintf("%08d" , s4)+"|");             // �����X�p�~�h���B
    z_g += (z->am_cash+"|");                                   // �{���X�p�t���O�� 45


    //ZDATA::GetZDTData ���ȭ����Ӣ�������=============================================

    AnsiString z_h;

    //z_h += (z->am_cash+"|");                                    //�{���X�p
    //z_h += (z->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");    // �ɹs����
    sTmp.sprintf("%08d",VisuilCpnAmt);
    z_h += (sTmp+"|");                                           //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // �ɹs�X�p���B
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    z_h += (sTmp+"|");                           // �N���ӫ~������馩  �ɹs�X�p���B

    z_h += (z->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");    // ��s����
    z_h += (z->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");    // ��s�X�p���B

    //��w
    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;           //��w����
    int int_drop_money = 0;       //��w���B
    int int_drop_sum = 0;    //��w�`�B
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        //StringSegment_EX(s, "|", 15)
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("���ͤ鵲�b��:�D��w(%s)���",s1041);
             writelog(sTmp );
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //�{��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC�I��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //�H�Υd
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //§��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //�����
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //���f��

        //ZDATA::GetZDTData�����o�����B 500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //ZDATA::GetZDTData�����o�����B 1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //ZDATA::GetZDTData�����o�����B 200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //�O�� 01���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //�O�� 02���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //�O�� 03���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //�O�� 04���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //�O�� 05���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //�O�� 06���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //�O�� 07���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //�O�� 08���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //�O�� 09���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //�O�� 10���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //�O�� 11���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //�O�� 12���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //�O�� 13���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //�O�� 14���B

        int_drop_sum += int_drop_money;          //��w�`�B
        int_drop_money = 0;
        ++int_drops;                             //��w����
    }

    z_h += (z->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        // ��w����
    z_h += (z->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     // ��w�X�p���B

    //writelog("��w�X�p���B OK");

    //ZDATA::GetZDTData �󥿦��ơB���B
    SumUpdat(0,gi_rec_line);
    z_h += (z->tb_casher.qt_update.sprintf("%08d", giA)+"|");          // �󥿦���
    z_h += (z->tb_casher.am_update.sprintf("%08d", giB)+"|");          // �󥿦X�p���B


    //�������
    SumVoid(0,gi_tot_line);
    z_h += (z->tb_casher.qt_void.sprintf("%08d", giA)+"|");            // �����������
    z_h += (z->tb_casher.am_void.sprintf("%08d", giB)+"|");            // ��������X�p���B


     //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
    }
    __finally
    {
        //delete ini_ecr;
        ;;
    }


    z_h += (z->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // �V�m�X�p����
    z_h += (z->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // �V�m�X�p���B

    //��ʿ�J����13�B14
    SumInpdp(0,gi_rec_line);
    z_h += (z->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");            //��ʿ�J��������
    z_h += (z->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");            //��ʿ�J�������B

    z_h += (z->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");     // �}���d����

    //�馩������   //�p�p�馩
    SumDisc(0,gi_pay_line);
    z_h += (z->tb_casher.qt_disc.sprintf("%08d", giA)+"|");             // �馩����(���|)
    z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");            // �馩���B(���|)

    pay_sub_amt(0,gi_pay_line);
    z_h += (z->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");            //�P�P�馩/������
    z_h += (z->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");             //�P�P�馩/�����B

    //�[�Ⱦ�
    easy_card(0,gi_easy_line);
    z_h += (z->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");          // �[�Ⱦ��}/��d����
    z_h += (z->tb_casher.am_disret1.sprintf("%08d", giB)+"|");         // �[�Ⱦ��}/��d���B
    z_h += (z->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");          // �[�Ⱦ��[�Ȧ���
    z_h += (z->tb_casher.am_disret2.sprintf("%08d", giD)+"|");          // �[�Ⱦ��[�Ȫ��B

    //fetc_card(0,gi_fetc_line);   //e�q�d   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //�ƥΤK����  �{�� �[�Ȧ���    2015/08/19 �N��,����O�P����B(�K)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //�ƥΤK���B  �{�� �[�Ȫ��B     2015/08/19 �N��,����O�@�o���B(�K)
    z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���    2015/08/19 �N��,����O�P����B(�K)
    z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|"); //�ƥΤK���B  �{�� �[�Ȫ��B     2015/08/19 �N��,����O�@�o���B(�K)

    //���w���ơB���B  int iqt_food,iamt_food;
    z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���  2015/08/19 0
    z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B  2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���  2015/08/19 0
    //z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B  2015/08/19 0


    //   05 �l�[ table        =============================================

    AnsiString z_i;

    z_i += (z->tb_newadd.qt_cashCard.sprintf("%08d", iqt_ffu1)+"|");    //��d/�{���d����    �q�l�� 1847
    z_i += (z->tb_newadd.am_cashCard.sprintf("%08d", iam_ffu1)+"|");    //��d/�{���d���B    �q�l��  ��d�X�p 1855
    z_i += (z->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");            //�N�������O����
    z_i += (z->tb_newadd.am_parking.sprintf("%08d", 0)+"|");            //�N�������O���B
    z_i += (z->tb_newadd.qt_agnrcv+"|");                                //�N���O�Φ���  1831
    z_i += (z->tb_newadd.am_agnrcv+"|");                                //�N���O�Ϊ��B  1839
    z_i += (z->tb_newadd.tx_mark.sprintf("%2s", str_ver_num.SubString(1,2) )+"|");      //�����e�m�Ÿ�
    z_i += (z->tb_newadd.tx_ver.sprintf("%6s",  str_ver_num.SubString(3,6) )+"|");      //�������X
    z_i += (z->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  //�o�����B0����

    //ZDATA::GetZDTData ���}�o�����B
    //SumNOVoiceAmt(0,gi_tot_line);

    z_i += (z->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //���}�o�����B
    z_i += (z->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");    //  ���}�o�����B�~�h��

    SumLOSS(0,gi_rec_line);
    z_i += (z->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");             // TM�o��Loss�Ȧ���
    z_i += (z->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");             // TM�o��Loss�Ȫ��B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);     // ���d���X�p
    z_i += (z->tb_newadd.am_ffu7.sprintf("%08d", iSub_C0InvoAmtA)+"|");             //��[�Ⱦ��[�Ȩ������B

    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��
    int iSumRelDimAgn=giB;

    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");             //��[�Ⱦ��[�Ȩ�������

    //���O��
    z_i += (z->tb_newadd.am_posZRpt.sprintf("%08d", iCSMAmt)+"|");      //���d�����d���B   ���O��

    AnsiString z_k;
    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);  //+iSumRelDimAgn+iC0InvoAmt4VDCN);    �Y���ʻP�P��i�����|
    z_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     // �Y���ʻP�P��i�ȧK�|
    z_k +=sTmp;

    //�o���I�����B
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //�o���I���i��
    z_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B
    z_k +=sTmp;


    //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    z_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�I�d����OgiD.�@�ogiE , �I�d����O�ȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt, iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)

    z_k +=sTmp;

    //AnsiString DDZ_1099;
    //DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
    //   DDZ_1099=str_datez;

    //ZDATA::GetZDTData Z����
    AnsiString str_zrp;

    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17   �I�d����O Update
    str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str(),
                            "FFFFFFFF"
                            );

    sTmp.printf("ZDATA::GetZDTData::(%s) Len=%d",str_zrp, str_zrp.Length() );
    writelog(sTmp);


    AnsiString  Zrpfilename;
    //sTmp = ExtractFileName(SalFileName);

    Zrpfilename.sprintf("%s%s_%s.zrp",
                                        XZDATADIR,
                                        sYYMMDDZZ.c_str(),
                                        ZDT_1050_DDZ.c_str()  );


    //ZDATA::GetZDTData Z����
    strcpy(report->char_filename, Zrpfilename.c_str()); //ChangeFileExt(str_z_filename,".zrp").c_str());
    report->CreateReport('Z', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
    //�ƥ�Z����
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //�����b��
    strcpy(report->char_filename, ChangeFileExt(Zrpfilename,".dpt").c_str() );
    report->CreateReport('D', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no);
    //�ƥ������b��
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    RtnZDTData=str_zrp;

  return(0);
}



//�鵲
int __fastcall ZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version, const String AutoZDttm)
{

    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //���JSAL�ɡA�N�ɮר̫��A����

    AnsiString sLog, sTmp, TmpZcnt,sRec;
    AnsiString str_ver_num, str_tran_cnt, SaleZDT;


    sLog.sprintf("ZDATA::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    if (!int_reslut)
        return 0;

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );
    str_ver_num = Version;    //get_version(Version);   �鵲

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����

    //�ƥ����J����l����
    Auto_Sal->Clear();
    Auto_Sal->LoadFromFile(SalFileName);
    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Autotsl_Ticket->Assign(gtsl_Ticket);
    //Auto_1051_sal->Assign(gtsl_1051_sal);       �۰ʤ鵲�L���ϥ�

    TStringList *Z_CntLst = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->Clear();
    ZDT_Lst->Clear();

    if( !_GetZCntLst(Z_CntLst, Auto_Sal ) )
        return 0;

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z�b���c

    //�ثeZ�b�ɶ�
    AnsiString str_z_date,                  //�{�b�ɶ�
               str_pz_date,                 //�e���ɶ�
               str_pzno_seq,                //�ثeZ�b����   �����k1
               str_pz_cnt,                  //�e���鵲�Ǹ�  ���餣�k�s
               str_nz_cnt,                  //�ثe�鵲�Ǹ�  ���餣�k�s
               str_am_tclsacc,              //�ֿnZ�b���B   ���餣�k�s
               sYYMMDDZZ,                   //Z�b���Ǹ�
               DDZ_1099,
               str_orgam_tclsacc,
               str_zrp,
               str_z_filename,               //Z�b��X�ɦW
               Lst_Zcnt_Dttm,
               str_sal_first;

    AnsiString str_date_time, str_datez;               //�ثe�b���t�ήɶ�
    AnsiString  Zrpfilename, str_zrp_path, str_dpt_path;


    str_date_time = gchar_date_time;
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("ZDATA::WriteData �����ثe������Ǹ�:%s", str_tran_cnt);
    writelog( sLog);



    str_sal_first = gpchar_sal_first;
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");       //�e���ɶ�

    if( _StringSegment_EX(str_sal_first, "|", 5) < str_pz_date )
        str_pz_date=_StringSegment_EX(str_sal_first, "|", 5);

    str_z_date  = str_date_time;                               //�{�b�ɶ�
    str_datez = tif_ecr->ReadString("Z","DATEZ","");           //DDZ
    Lst_Zcnt_Dttm = tif_ecr->ReadString("AUTOZ","LST_ZCNT_DTTM",str_z_date.SubString(1,8) );   //����ѦҤ�

    //�N�b�����Ƴƥ��w�Q�N�ӧP�_���ؿ����� james - 2003/02/07

     sTmp=tif_ecr->ReadString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100" ));  //�ۤ�ʤ鵲�n�p��

    //�N�b�����Ƴƥ��w�Q�N�ӧP�_���ؿ����� james - 2003/02/07
    tif_ecr->WriteString("Backup", "pz_date", sTmp);

    //tif_ecr->WriteString("Backup", "pz_date", str_pz_date);
    //�ƥ� sal8.TXT����
    //tif_ecr->WriteString("Backup", "datez", str_datez);

    if ((str_pz_date == "") || (str_pz_date.Length() < 8))  //�L������ơA����O���k�s
    {
        tif_ecr->WriteString("Z", "pz_date", str_z_date);
        tif_ecr->WriteString("Z", "pzno_seq", "0");
        tif_ecr->WriteString("Z", "nzno_seq", "1");
        tif_ecr->WriteString("Z", "pz_cnt", "00000");
        tif_ecr->WriteString("Z", "nz_cnt", "00001");
        tif_ecr->WriteString("Z", "datez", (str_date_time.SubString(7,2) + "1"));
        //20031225
        tif_ecr->WriteString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        tif_ecr->WriteString("Z", "zfilename", "");
        tif_ecr->WriteString("Z", "zrptname", "");
        tif_ecr->WriteString("Z", "deptname", "");
        tif_ecr->WriteString("Z", "inqtname", "");

        tif_ecr->WriteString("VXZ", "PZNO_SEQ", "0");
        tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");
        tif_ecr->WriteString("VXZ", "PZ_DATE", "");

        //tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        //tif_ecr->WriteString("VXZ","CS_ENTRY","");

        AnsiString str_xpath;               //���w���ȭ��b���ؿ�
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date;
        Lst_Zcnt_Dttm=str_z_date.SubString(1,8);

        sLog.sprintf("ZDATA::Z->PZ_DATE < 8Byte ==> init %s ", XZDATA_INI);
        writelog( sLog);
    }

    str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b���� 01~09  �����k1
    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s 00001~99999

    tif_ecr->WriteString("Z_OFF", "nz_cnt", str_nz_cnt  );            // ���u��ƨϥ�
    tif_ecr->WriteString("Z_OFF", "pz_cnt", tif_ecr->ReadString("Z","pz_cnt","00000")  );  // ���u��ƨϥ�

    str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //�ֿnZ�b���B   ���餣�k�s


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.Length() < 5)
        str_nz_cnt = "00001";

    sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_pzno_seq));

    sLog.sprintf("ZDATA::Get %s=> str_pz_date=%s, str_nz_cnt=%s, str_am_tclsacc=%s", XZDATA_INI, str_pz_date, str_nz_cnt, str_am_tclsacc);
    writelog( sLog);


#ifdef TM_20

    str_z_filename.sprintf("%s%02d%s%02d%s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        (str_pz_date.SubString(3,6)).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        str_pz_date.SubString(9,4).c_str()    );
#else
    //��X�ɮ� yyyymmddnn.zdt ex:2002010101.zdt
    //str_z_filename = (str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt");

    str_z_filename.sprintf("%s%02d%s%02d%s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        str_pz_date.SubString(3,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        str_pz_date.SubString(9,4).c_str()   );
#endif

    //����Flag
    bool bl_change_day;

    //�P�_����
    //if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
    if ( str_z_date.SubString(1,8) > str_pz_date.SubString(1,8) &&
         str_z_date.SubString(1,8) > Lst_Zcnt_Dttm  )
        bl_change_day = true;
    else
        bl_change_day = false;


    DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=str_datez;

    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //���v�ؿ����@��


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

     str_orgam_tclsacc=str_am_tclsacc;

     AnsiString ZrpFile, DptFile;

     ZrpFile=ChangeFileExt( str_z_filename, ".zrp");     //ExtractFileName(str_z_filename)
     DptFile=ChangeFileExt( str_z_filename, ".dpt");
     // sBackupPath=ExtractFilePath(BackupFileName);
     //   sFileNameNonPath = ExtractFileName(BackupFileName);    //�ɦW���]�t���|

     for( int i=0; i< Z_CntLst->Count; i++)
       {
         sRec=Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);
         GetZCNT_SalData(TmpZcnt) ;
         GetZDTData(TmpZcnt, String(gchar_tencode),  String(gchar_ecr_no), String(gchar_cashier),
                    String(str_ver_num), str_pz_date, str_z_date, DDZ_1099, sRec,
                    SaleZDT, str_am_tclsacc, str_zrp_path, str_dpt_path  );

         ZDT_Lst->Add(SaleZDT);
         str_zrp+=SaleZDT;

         SaleZDT="";

         file_cat( str_zrp_path.c_str(), ZrpFile.c_str() );
         sLog.sprintf("ZDATA::file_cat(%s, %s)",str_zrp_path.c_str(), ZrpFile.c_str() );
         writelog( sLog);

         file_cat( str_dpt_path.c_str(), DptFile.c_str() );
         sLog.sprintf("ZDATA::file_cat(%s, %s)",str_dpt_path.c_str(), DptFile.c_str() );
         writelog( sLog);

       } // end of for( int i=0; i< Z_CntLst->Count; i++)

     sTmp=ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+ExtractFileName(ZrpFile);
     CopyFile(ZrpFile.c_str(), str_zrp_path.c_str(), false);
     sLog.sprintf("ZDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
     writelog( sLog);
     DeleteFile( ZrpFile );
     sLog.sprintf("ZDATA::DeleteFile(%s)",ZrpFile.c_str() );
     writelog( sLog);

     str_dpt_path=sTmp+ExtractFileName(DptFile);
     CopyFile(DptFile.c_str(), str_dpt_path.c_str(), false);
     sLog.sprintf("ZDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
     writelog( sLog);
     DeleteFile( DptFile );
     sLog.sprintf("ZDATA::DeleteFile(%s)",DptFile.c_str() );
     writelog( sLog);

     delete Z_CntLst;
     delete ZDT_Lst;

     //�g�JSAL��98Type
     AnsiString str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_p_z_cnt,
               str_n_z_cnt,
               str_start_trans,            //�}�l����Ǹ�
               str_am_tclascc,
               str_am_totsale,
               str_zdt_no,
               str_ver,
               str_sal_last_1099,
               SaleDataVer,
               str_zdt_path;

    str_rec_type = "1098";
    str_rec_len = "0121";
    str_tencode = gchar_tencode;
    str_ecr_no.sprintf("%s",gchar_ecr_no);
    str_sys_date = str_z_date;      //����ɶ�

    //98����Ǹ� //20031215
    int int_sum_tran_cnt ;

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //����Ǹ�

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    sLog.sprintf("ZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    writelog( sLog);


    AnsiString str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
         str_sum_tran.sprintf("%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //�e���鵲�Ǹ��A�ثe�鵲�Ǹ�
    str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "�e���鵲�Ǹ�"));
    str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "�ثe�鵲�Ǹ�"));


    if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc, int_acc_tot_amt;

    int_acc_tot_amt=_StrToInt(str_am_tclsacc);
    //AnsiString str_sal_first;
    //str_sal_first = gpchar_sal_first;

     //�}�l����Ǹ�
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, "|", 7).SubString(TRANCNT_NO_POS,5));
    str_start_trans.sprintf("%05d", i_tran_cnt);

    //int_am_tclsacc=int_acc_tot_amt;
    //str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //���鵲�b�֭p���B  ????????????
    str_am_totsale = "0";   //z->tb_trans.am_totsale;         //�����ھP�f���J  ????????????
    str_zdt_no.sprintf("%010ld", _StrToInt(str_nz_cnt,"�鵲�Ǹ�"));

    str_ver = "FFFFFFFF|";

    AnsiString str_trans1_5;    //����Ǹ��e���X

    AnsiString str_sal_last;
    str_sal_last = gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    //if( Trim(str_trans1_5)=="")
    str_trans1_5="00"+DDZ_1099;

    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //���v�ؿ����@��  ���i�R

    AnsiString str_01,str_99;

    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   SalesNo.c_str(),                       //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   " ",
                   " ",
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );

    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
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

    AnsiString _str_tran_cnt;        //����Ǹ�
    AnsiString str_st_tran_cnt;     //�}�l����Ǹ�

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    AnsiString str_98;  //�g�J1098����Ǹ�
    str_98.sprintf("%4s|%4s|%6s|%2s|%14s|%-5s|%-10s|%5s|%5s|%10s|000%8s|000%8s|%10s|%9s\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_p_z_cnt.c_str(),
                        str_n_z_cnt.c_str(),
                        str_st_tran_cnt.c_str(),
                        str_am_tclascc.c_str(),
                        str_am_totsale.c_str(),
                        str_zdt_no.c_str(),
                        str_ver.c_str()
                        );



    String str_sal;
    //str_sal = str_01 + "\n"+ str_zrp + str_98 + str_99;
    str_sal = str_01 + "\n"+ str_zrp + str_99;


    if (WriteSAL(str_sal, str_z_filename) == OK)
       {
        //�ƥ�ZDT�b
        str_zdt_path = BackupFile(str_z_filename, "", DDZ_1099);
        tif_tmp->WriteString("Z","x1098filename", str_zdt_path);
       }
    else
      {
       tif_tmp->WriteString("Z","x1098filename", "");
      }


    /////////////////////////////////////////////////////////////////

    AnsiString StrZslFile;
    if (WriteSAL(str_sal, SalFileName) == OK)
       {
         //�ƥ�ZSL�b
         sTmp = ExtractFileName(SalFileName);
         StrZslFile.sprintf("%s%s_%s%s",XZDATASAL,str_sys_date.SubString(1,8),DDZ_1099,sTmp);
         if( CopyFile( SalFileName.c_str(), StrZslFile.c_str(), false)  )
           {
             sLog.sprintf("ZDATA:: �ƥ�SAL %s TO %s",
                  SalFileName, StrZslFile );
             writelog( sLog);

           }
         else
           {
             sLog.sprintf("ZDATA:: �ƥ�SAL %s TO %s ����!!",
                  SalFileName, StrZslFile );
             writelog( sLog);
           }

         StrZslFile = BackupFile(SalFileName, ChangeFileExt(str_zdt_path,".zsl"), DDZ_1099 );
         tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);

         sTmp=ChangeFileExt(StrZslFile,".asl");
         Auto_Sal->SaveToFile(sTmp);
         writelog("ZDATA::�ƥ����J��SAL Auto_Sal->SaveToFile=>" + sTmp);

       }
    else
       {
         tif_tmp->WriteString("Z","ZSLNAME","");
       }

    AnsiString str_ipt_path;

    //���XZDATA.INI
    int int_nzno_seq   = _StrToInt(str_pzno_seq, "pzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp->WriteString("Z","ZFILENAME" , str_zdt_path);
    tif_tmp->WriteString("Z","ZRPTNAME"  , str_zrp_path);
    tif_tmp->WriteString("Z","DEPTNAME"  , str_dpt_path);
    tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

    tif_tmp->WriteString("Z","PZ_DATE"     , str_z_date);
    tif_tmp->WriteString("Z","AM_TCLSACC" , IntToStr(int_am_tclsacc)) ;
    tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT ����Ǹ�

    tif_tmp->WriteString("ECR","tot_open_z", "0");
    tif_ecr->WriteString("ECR","tot_open_z", "0");

    tif_tmp->WriteString("VXZ", "NZNO_SEQ", "1");
    tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");

    tif_tmp->WriteString("VXZ", "PZ_DATE", "");
    tif_ecr->WriteString("VXZ", "PZ_DATE", "");


    tif_tmp->WriteString("AUTOZ","LST_AUTOZ_DTTM",str_z_date.SubString(1,8)); //AutoZ_Dt);   //�e���۰ʤ鵲�ɶ�
    //tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM","");  //�{�A�۰ʤ鵲�ɶ��L�@��
    tif_tmp->WriteString("AUTOZ","LST_ZCNT_DTTM",str_z_date.SubString(1,8));   //����ѦҤ�


    if (bl_change_day) //�P�_�O�_����
    {
        //sLog.sprintf("ZDATA::�鵲����  pz_date=%s, z_date=%s",str_pz_date, str_z_date);
        //writelog( sLog);

        tif_ecr->WriteString("Z","PZNO_SEQ", "0");
        tif_ecr->WriteString("Z","NZNO_SEQ", "1");
        tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);
        if (++int_nz_cnt > 99999)
            {
             tif_ecr->WriteString("Z","PZ_CNT", "00000");
             int_nz_cnt = 1;
            }

        str_nz_cnt.sprintf("%05d",int_nz_cnt);
        tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        sLog="ZDATA::����B�z DATEZ=" + str_z_date.SubString(7,2)+ "1" +  ", YYYYMMDDZ=" + str_z_date.SubString(7,2) + "0100";
        writelog( sLog);

    }
    else
    {

        tif_ecr->WriteString("Z","PZNO_SEQ", str_pzno_seq);

        if (++int_nzno_seq > 9)
           {
            tif_ecr->WriteString("Z","PZ_CNT", "00000");
            int_nzno_seq = 1;
           }

        str_pzno_seq.sprintf("%d", int_nzno_seq);

        tif_ecr->WriteString("Z","NZNO_SEQ", str_pzno_seq);
        tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);

        if (++int_nz_cnt > 99999)
            int_nz_cnt = 1;

        str_nz_cnt.sprintf("%05d", int_nz_cnt);

        tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
                              + FormatFloat("00",int_nzno_seq) + "00"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        //sLog.sprintf("ZDATA::�鵲������  pz_date=%s, z_date=%s, SET NZNO_SEQ=%s",str_pz_date, str_z_date, str_pzno_seq);
        //writelog( sLog);

        sLog="ZDATA::������B�z DATEZ=" + str_z_date.SubString(7,2) + IntToStr(int_nzno_seq) +  ", YYYYMMDDZ=" +  str_z_date.SubString(1,8) + FormatFloat("00",int_nzno_seq) + "00";
        writelog( sLog);


    }

    sLog.sprintf("ZDATA::�g�J XZDATAINI [AUTOZ]->LST_AUTOZ_DTTM=%s, [AUTOZ]->LST_AUTOZ_DTTM=%s, [AUTOZ]->LST_ZCNT_DTTM=%s, [Z]->NZ_CNT=%s",
                  str_z_date.SubString(1,8), str_z_date.SubString(1,8), str_z_date.SubString(1,8), str_nz_cnt);
    writelog( sLog);

    str_pz_date    = str_z_date;
    str_pzno_seq   = tif_ecr->ReadString("Z","NZNO_SEQ","");      //�ثeZ�b����   �����k1

    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";


    //���wX �bPATH
    AnsiString str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //�e���~��/�d�ȭ��L
    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_tran_cnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    // �۰ʤ鵲 ���NX�b�k�s    _WZ_OK �|�W0
    tif_ecr->WriteString("X", "PTRAN_CNT", "00000000");
    tif_ecr->WriteString("X", "SX_DATE"  , "");
    tif_ecr->WriteString("X", "EX_DATE"  , "");
    //�۰ʤ鵲�W������,��ʤ鵲�k0 , �ҥH�� �۰ʤ鵲/��ʤ鵲 �B�z
    tif_ecr->WriteString("X", "PXNO_SEQ" , "0");


    sLog.sprintf("ZDATA::�g�J XZDATAINI Z->NZ_CNT=%s, AM_TCLSACC=%d, ECR->TRAN_CNT=%s, XPATH=%s, �鵲 -> �NX�b�k�s",
                  str_nz_cnt, int_am_tclsacc, str_sum_tran, str_x_path );
    writelog( sLog);


    return OK;


}


//X�b��
int __fastcall XREPORT::WriteData( const String StoreNO, const String EcrNO, const String SalesNo ,
                                   const AnsiString SalFileName, const AnsiString Version)
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //���JSAL�ɡA�N�ɮר̫��A����


    AnsiString sLog;
    sLog.sprintf("X�b�� XREPORT::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    //if (!int_reslut)
    //    return 0;

    AnsiString str_ver_num, sTmp;

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����

    str_ver_num = get_version(Version);

    SPOS_ZRPDT *xr = new SPOS_ZRPDT;       //Z�b���c

    TIniFile *tif_ecr   = new TIniFile(XZDATA_INI);

    //�ثeZ�b�ɶ�
    AnsiString str_z_date;         //�{�b�ɶ�
    AnsiString str_pz_date;        //�e���ɶ�

    AnsiString str_date_time;
    str_date_time = gchar_date_time;

    str_z_date  = str_date_time;
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");

    if (str_pz_date.Length() < 6)
        str_pz_date = str_z_date;

    xr->no_tm = gchar_ecr_no;        //���Ⱦ�����

    //�鵲�Ǹ�
    AnsiString str_nz_cnt, sYYMMDDZZ, str_tran_cnt;
    str_nz_cnt = tif_ecr->ReadString("Z","nz_cnt","00001");

    xr->bg_noclose.sprintf("%05d", _StrToInt(str_nz_cnt,"nz_cnt"));

    AnsiString  str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b���� 01~09  �����k1
    //str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s 00001~99999
    //str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //�ֿnZ�b���B   ���餣�k�s


    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

    //if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
    //    str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //����Ǹ�

    sLog.sprintf("XREPORT::ECR->WriteData �����ثe������Ǹ�:%s", str_tran_cnt);
    writelog( sLog);


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_pzno_seq));


    xr->bg_dtclose = str_z_date.SubString(1,8);     //���b���
    xr->bg_tmclose = str_z_date.SubString(9,6);     //���b�ɶ�
    xr->ed_dtclose = str_pz_date.SubString(1,8);    //�e�����b���
    xr->ed_tmclose = str_pz_date.SubString(9,6);    //�e�����b�ɶ�


    AnsiString xr_a;
    AnsiString z_a;
    int iC0TotCount=0;

    xr_a.sprintf("1096|%04d|%6s|%2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|", 1594,
                                      gchar_tencode,
                                      xr->no_tm.c_str(),           //���Ⱦ�����
                                      str_date_time.c_str(),      //�{�b�ɶ�
                                      xr->bg_noclose.c_str(),      //�鵲�Ǹ�
                                      sYYMMDDZZ,                   //Z�b���Ǹ�
                                      xr->ed_dtclose.c_str(),      //�e�����b���
                                      xr->ed_tmclose.c_str(),      //�e�����b�ɶ�
                                      xr->bg_dtclose.c_str(),      //���b���
                                      xr->bg_tmclose.c_str()      //���b�ɶ�
                                    );



    //������Ӣ�������=============================================

    //����ӫȼ�
    xr->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~��
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    xr->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;

    // �I�d����O���K�|
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

     //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    //int iPreSalINVODisAmt ;
    //SumPreSal_INVO_DisAmt(0, gi_rec_line);
    //iPreSalINVODisAmt = giU;

    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);    //1050==> R4+Z0
    xr->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ));      //�o���鵲�b�B(�K)
    xr->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));      //�o���鵲�b�B(�|)
    xr->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );      //�o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ); // + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    sLog.sprintf("XREPORT::WriteData(%s, %s, %s)",
                  xr->tb_trans.am_trcpntx, xr->tb_trans.am_trcptax, xr->tb_trans.am_trcptot);
    writelog( sLog);


    //���鵲�b�֭p���B
    AnsiString str_am_tclsacc;     //�ֿnZ�b���B
    int int_acc_tot_amt;
    str_am_tclsacc = tif_ecr->ReadString("Z","AM_TCLSACC","");      //���餣�k�s

    if (str_am_tclsacc.Length() < 1)
        str_am_tclsacc = "0";

    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p"));

    xr->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    // xr->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")));


    TotMiss(0,gi_tot_line);       //1050 ==> R2R3R4
    // RC01FTaxAmt=giC; RC01TaxAmt=giD;
    xr->tb_trans.qt_miss.sprintf("%08d",   giA);        // �~���o������   1050 ==> R2R3R4
    xr->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        // �~���o�����B(�K)
    xr->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        // �~���o�����B(�|)
    xr->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        // �~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX);


    //���}�o�����B
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX - iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX - iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    xr->tb_trans.am_totsale.sprintf("%08d", (int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt));

    //�o���}�l���X
    //�o���������X
    if (gbl_tot_haveline)
    {
        xr->no_rcstart = invoice_start_end(gi_tot_line, true);
        xr->no_rcend = invoice_start_end(gi_tot_line, false);
    }
    else
    {
        xr->no_rcstart = "0000000000";
        xr->no_rcend   = "0000000000";
    }


    AnsiString xr_b;

    xr_b += (xr->tb_trans.qt_tcust+"|");             //����ӫȼ�
    xr_b += (xr->tb_trans.qt_trans+"|");             //�������~��
    xr_b += (xr->tb_trans.am_tclsacc+"|");          //���鵲�b�֭p���B

    xr_b += (xr->no_rcstart+"|");                   //�o���}�l���X
    xr_b += (xr->no_rcend+"|");                      //�o���������X
    xr_b += (xr->tb_trans.am_trcpntx+"|");           //�o���鵲�b�B(�K)
    xr_b += (xr->tb_trans.am_trcptax+"|");          //�o���鵲�b�B(�|)
    xr_b += (xr->tb_trans.am_trcptot+"|");           //�o���鵲�b���B

    xr_b += (xr->tb_trans.qt_miss+"|");             //�~���o������
    xr_b += (xr->tb_trans.am_missntx+"|");           //�~���o�����B(�K)
    xr_b += (xr->tb_trans.am_misstax+"|");           //�~���o�����B(�|)
    xr_b += (xr->tb_trans.am_miss+"|");              //�~���o�����B
    xr_b += (xr->tb_trans.am_totsale+"|");           //�����ھP�f���J   24


    //�N��I��������===========================================

    int iRentAmt, iRemtCnt;
    //�N��
    SumKamt(0,gi_tot_line);                 //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;             //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;                 //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);   //giH giI ���w�h�f���B�B����   //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    xr->tb_newadd.qt_agnrcv.sprintf("%08d",giD);               //�N���O�Φ���
    xr->tb_newadd.am_agnrcv.sprintf("%08d",giC);               //�N���O�Ϊ��B


    if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
        ForceDirectories("C:\\FTLPOS\\HISTORY\\");

    //���ͥN����� 2011/05/30
    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\subsale.rpt");
    report->CreateReport('s', " ", StrBillPay, gchar_tencode, gchar_ecr_no, 0,0,0,0,tsl_subsale);


    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;


    AnsiString xr_c;
    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    /****
    for (int j=0; j<5; j++)
    {
        for (int k=0; k<9; k++)
        {
            if (k<8)
            {
                if (j == 0)
                {
                    if (k == 0)
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giE);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giA);
                    }
                    else
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else if (j == 3)
                {
                    if (k == 0)
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giF);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giB);
                    }
                    else
                    {
                        xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else
                {
                    xr_c += xr->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                    xr_c += xr->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                }

            }
            else
            {
                if (j == 0)
                {
                    xr_c += xr->tb_subrev[j].qt_totrev.sprintf("%08d",giE);
                    xr_c += xr->tb_subrev[j].am_totrev.sprintf("%08d",giA);
                }
                else if (j == 3)
                {
                    xr_c += xr->tb_subrev[j].qt_totrev.sprintf("%08d",giF);
                    xr_c += xr->tb_subrev[j].am_totrev.sprintf("%08d",giB);
                }
                else
                {
                    xr_c += xr->tb_subrev[j].qt_totrev.sprintf("%08d",0);
                    xr_c += xr->tb_subrev[j].am_totrev.sprintf("%08d",0);
                }

            }
        }
    }
    ****/

    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

     Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //�N��X�p����  �N�����O�ӫ~�X�p�^��
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //�N��X�p���B  �N�����O�ӫ~�X�p���B �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

    //25~28
    xr_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

   //////////////////////////////////////////////////////////////////////////////


    // 03 �N�{����������=============================================
    int iqt_ffu1,iam_ffu1,iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);  //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B


    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;


    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    xr->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);               //§��^��
    xr->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);               //§����B

    xr->tb_cash.qt_vndcpn.sprintf("%08d", 0);                //�t�ӥN�{���^��
    xr->tb_cash.am_vndcpn.sprintf("%08d", 0);                //�t�ӥN�{�����B

    xr->tb_cash.qt_stocpn.sprintf("%08d", giC);              //�����N�{���^��
    xr->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt );              //�����N�{�����B  �����

    xr->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));     //�N�{���X�p�^��
    xr->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));     //�N�{���X�p���B

    //xr->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));     //�N�{���X�p�^��
    //xr->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));     //�N�{���X�p���B

    xr->tb_cash.qt_bonout.sprintf("%08d", giG);              //�����X�p�^��
    xr->tb_cash.am_bonout.sprintf("%08d", giH);              //�����X�p���B

    iTotOt2Rtn=giE;                                       //�l�B��
    iqt_ffu1=giI;                                         //�{���d�^��   �q�l���^��
    iam_ffu1=giJ;                                         //�{���d���B   �q�l�����B   ��d�X�p

    AnsiString xr_d;

    xr_d += (xr->tb_cash.qt_bonus+"|");               //§��^��      29
    xr_d += (xr->tb_cash.am_bonus+"|");                //§����B     30
    xr_d += (xr->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��   31
    xr_d += (xr->tb_cash.am_vndcpn+"|");               //�t�ӧ������B   32
    xr_d += (xr->tb_cash.qt_stocpn+"|");               //�����^��       33
    xr_d += (xr->tb_cash.am_stocpn+"|");               //�������B       34
    xr_d += (xr->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��   35
    xr_d += (xr->tb_cash.am_totcash+"|");              //�N�{���X�p���B   36

    sTmp.sprintf("%08d",giF);
    xr_d += (sTmp+"|");                               //�l�B�X�p�^��      37
    sTmp.sprintf("%08d",giE);
    xr_d += (sTmp+"|");                               //�l�B�X�p���B      38

    xr_d += (xr->tb_cash.qt_bonout+"|");                 //�����X�p�^��       39
    xr_d += (xr->tb_cash.am_bonout+"|");;                //�����X�p���B       40

    //==========================================================================

    //�{���X�p �t ���O��
    SumCash41=SumCash(0,gi_tot_line);
    xr->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    //sLog.sprintf("XREPORT::�{���X�p(%s)",xr->am_cash);
    //writelog( sLog);


    //char xre[29];
    //xre[0] = 0;

   //strcpy(xre, xr->am_cash.c_str());         //�{���X�p
    //strcat(xre, xr->no_rcstart.c_str());      //�o���}�l���X
    //strcat(xre, xr->no_rcend.c_str());        //�o���������X
    //xre[28] = 0;


    // Z�b�����O���pTABLE      =============================================
    AnsiString xr_f;

    int s1, s2, s3, s4;
    s1 = s2 = s3 = s4 = 0;

    DEPT *dept;

    try
    {

        dept = new DEPT[20];

        //���N�ȶ�s
        for (int i=0; i<20; i++)
        {
            xr->tb_depcal[i].am_dpsitm.sprintf("%08d", 0);
            xr->tb_depcal[i].am_dpsale.sprintf("%08d", 0);
            xr->tb_depcal[i].am_dpmitm.sprintf("%08d", 0);
            xr->tb_depcal[i].am_dpmiss.sprintf("%08d", 0);
            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //�p��Ҧ�����������, �����b
        if (gbl_rec_haveline)
            SumAllDept(dept);

        for (int i=0; i<20; i++)
        {
            xr->tb_depcal[i].am_dpsitm.sprintf("%08d",dept[i].am_dpsitm);
            xr_f += (xr->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            xr->tb_depcal[i].am_dpsale.sprintf("%08d",dept[i].am_dpsale);
            xr_f += (xr->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            xr->tb_depcal[i].am_dpmitm.sprintf("%08d",dept[i].am_dpmitm);
            xr_f += (xr->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            xr->tb_depcal[i].am_dpmiss.sprintf("%08d",dept[i].am_dpmiss);
            xr_f += (xr->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;
        }
    }
    __finally
    {
        delete [] dept;
    }



    AnsiString xr_g;

    xr_g += (xr->am_tdpsitm.sprintf("%08d", s1)+"|");     //�����X�p������� 41
    xr_g += (xr->am_dpstot.sprintf("%08d",  s2)+"|");     //�����X�p�P����B 42
    xr_g += (xr->am_tdpmitm.sprintf("%08d", s3)+"|");     //�����X�p�~�h���� 43
    xr_g += (xr->am_dpmtot.sprintf("%08d",  s4)+"|");     //�����X�p�~�h���B 44
    xr_g +=  (xr->am_cash+"|");                           //�{���X�p�t���O�� 45


    //���ȭ����Ӣ������� =============================================

    AnsiString xr_h;

    xr_h += (xr->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");         // �ɹs����      46
    //sTmp.sprintf("%08d",VisuilCpnAmt);                                   //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    //xr_h += (sTmp+"|");

    //xr_h += (xr->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");         // �ɹs�X�p���B  47
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    xr_h += (sTmp+"|");                           // �N���ӫ~������馩  �ɹs�X�p���B

    xr_h += (xr->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");         // ��s����      48
    xr_h += (xr->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");         // ��s�X�p���B  49

    //��w
    AnsiString x_h;

    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;                           //��w����
    int int_drop_money = 0;                       //��w���B
    int int_drop_sum = 0;                    //��w�`�B
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("����X�b��:�D��w(%s)���",s1041);
             writelog(sTmp );
             continue;
           }


        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //�{��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC�I��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //�H�Υd
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //§��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //�����
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //���f��

        //�����o�����B  800   Cloud �����o�����B(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s1041, "|", 18) );

        //�����o�����B 500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //�����o�����B 1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //�����o�����B 200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //�O�� 01���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //�O�� 02���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //�O�� 03���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //�O�� 04���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //�O�� 05���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //�O�� 06���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //�O�� 07���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //�O�� 08���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //�O�� 09���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //�O�� 10���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //�O�� 11���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //�O�� 12���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //�O�� 13���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //�O�� 14���B

        int_drop_sum += int_drop_money;          //��w�`�B
        int_drop_money = 0;
        ++int_drops;                             //��w����

    }

    xr_h += (xr->tb_casher.qt_droptot.sprintf("%08d",int_drops)+"|");          //��w����    50
    xr_h += (xr->tb_casher.am_droptot.sprintf("%08d",int_drop_sum)+"|");   //��w�X�p���B    51

    //�󥿦��ơB���B
    SumUpdat(0,gi_rec_line);
    xr_h += (xr->tb_casher.qt_update.sprintf("%08d",giA)+"|");            //�󥿦���         52
    xr_h += (xr->tb_casher.am_update.sprintf("%08d",giB)+"|");            //�󥿦X�p���B     53

    //�������
    SumVoid(0,gi_tot_line);
    xr_h += (xr->tb_casher.qt_void.sprintf("%08d",giA)+"|");               // �����������     54
    xr_h += (xr->tb_casher.am_void.sprintf("%08d",giB)+"|");               // ��������X�p���B 55

    //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

         //   tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
         //   tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

    }
    __finally
    {
          ;;
    }

    
    xr_h += (xr->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // �V�m�X�p����  56
    xr_h += (xr->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // �V�m�X�p���B  57


    //��鳡���[�`13�B14
    SumInpdp(0,gi_rec_line);
    xr_h += (xr->tb_newadd.qt_inpdp.sprintf("%08d",giA)+"|");             //��ʿ�J��������  58
    xr_h += (xr->tb_newadd.am_inpdp.sprintf("%08d",giB)+"|");            //��ʿ�J�������B   59

    xr_h += (xr->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");      // �}���d����      60

    //�馩����   //�p�p�馩
    SumDisc(0,gi_pay_line);
    xr_h += (xr->tb_casher.qt_disc.sprintf("%08d",giA)+"|");             // �馩����(���|)   61
    xr_h += (xr->tb_casher.am_disc.sprintf("%08d",giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");             // �馩���B(���|)   62

    pay_sub_amt(0,gi_pay_line);
    xr_h += (xr->tb_newadd.qt_ffu6.sprintf("%08d",giA)+"|");             //�P�P�馩/������   63
    xr_h += (xr->tb_newadd.am_ffu6.sprintf("%08d",giB+iPreSalMMDisAmt_TM)+"|");             //�P�P�馩/�����B   64


    //�[�Ⱦ�
    easy_card(0,gi_easy_line);
    xr_h += (xr->tb_casher.qt_disret1.sprintf("%08d",giA)+"|");            // �[�Ⱦ��}/��d����  65
    xr_h += (xr->tb_casher.am_disret1.sprintf("%08d",giB)+"|");            // �[�Ⱦ��}/��d���B  66
    xr_h += (xr->tb_casher.qt_disret2.sprintf("%08d",giC)+"|");            // �[�Ⱦ��[�Ȧ���     67
    xr_h += (xr->tb_casher.am_disret2.sprintf("%08d",giD)+"|");            // �[�Ⱦ��[�Ȫ��B     68

    //fetc_card(0,gi_fetc_line);   //e�q�d  // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;

    //xr_h += (xr->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx + PreSal09_AmtNTX )+"|");    //�ƥΤK����  �{�� �[�Ȧ���   69   2015/08/19 �N��,����O�P����B(�K)
    //xr_h += (xr->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B   70   2015/08/19 �N��,����O�@�o���B(�K)
    xr_h += (xr->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");    //�ƥΤK����  �{�� �[�Ȧ���   69   2015/08/19 �N��,����O�P����B(�K)
    xr_h += (xr->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B   70   2015/08/19 �N��,����O�@�o���B(�K)

    //���w���ơB���B  int iqt_food,iamt_food;
    xr_h += (xr->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ��� 71   2015/08/19 0
    xr_h += (xr->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B 72   2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //xr_h += (xr->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ��� 71   2015/08/19 0
    //xr_h += (xr->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B 72   2015/08/19 0


    //�l�[ table  =============================================

    AnsiString xr_i;

    xr_i += (xr->tb_newadd.qt_cashCard.sprintf("%08d",iqt_ffu1)+"|");         //��d/�{���d����  73
    xr_i += (xr->tb_newadd.am_cashCard.sprintf("%08d",iam_ffu1)+"|");         //��d/�{���d���B  74
    xr_i += (xr->tb_newadd.qt_parking.sprintf("%08d",0)+"|");           //�N�������O����         75
    xr_i += (xr->tb_newadd.am_parking.sprintf("%08d",0)+"|");           //�N�������O���B ,���}�o�����B�~�h��  76
    xr_i += (xr->tb_newadd.qt_agnrcv+"|");                              //�N���O�Φ���     77
    xr_i += (xr->tb_newadd.am_agnrcv+"|");                              //�N���O�Ϊ��B     78
    xr_i += (xr->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //�����e�m�Ÿ�      79
    xr_i += (xr->tb_newadd.tx_ver.sprintf("%6s", str_ver_num)+"|");    //�������X          80
    xr_i += (xr->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  // �o�����B0����

    //���}�o�����B
    //SumNOVoiceAmt(0,gi_tot_line);
    xr_i += (xr->tb_newadd.am_ffu4.sprintf("%08d",int_novoice_amt)+"|");        //�ƥΥ|���B , ���}�o�����B
    xr_i += (xr->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");   // �[�Ⱦ����b���B,���}�o�����B�~�h��

    SumLOSS(0,gi_rec_line);
    xr_i += (xr->tb_newadd.qt_ffu5.sprintf("%08d",giA)+"|");             //TM�o��Loss�Ȧ���
    xr_i += (xr->tb_newadd.am_ffu5.sprintf("%08d",giB)+"|");             //TM�o��Loss�Ȫ��B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // ���d���X�p
    xr_i += (xr->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");     //��[�Ⱦ��[�Ȩ������B

    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��
    int iSumRelDimAgn=giB;

    //xr_i += (xr->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //��[�Ⱦ��[�Ȩ�������
    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    xr_i += (xr->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //��[�Ⱦ��[�Ȩ�������

    xr_i += (xr->tb_newadd.am_posZRpt.sprintf("%08d",iCSMAmt)+"|");           //���d�����d���B  ���O��


    AnsiString xr_k;
    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    �Y���ʻP�P��i�����|
    xr_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //�Y���ʻP�P��i�ȧK�|
    xr_k +=sTmp;

    //�o���I�����B
    VoicePayAmt(0,gi_tot_line);
    sTmp.sprintf("%08d|",giA/200); //�o���I���i��  �����o���i��
    xr_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B  �����o�����B
    xr_k +=sTmp;


    //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    xr_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    xr_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�N�����O���|���B giD.  �N�����O���|�@�o���B giE , �N�����O�ӫȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)
    xr_k +=sTmp;

    //xr_i += xr->tb_newadd.qt_ffu4.sprintf("%08d",gi_mix);         //�N���ӫȼ�

    //xr_i += xr->tb_newadd.qt_posZRpt.sprintf("%08d",0);           //���d�����d����

    // xr_h += xr->tb_casher.qt_pack.sprintf("%08d",iTotOt2Rtn); //giE);     // �[�Ⱦ����b����  2007/12/01 ��l�B��

    //xr_i += xr->tb_newadd.qt_ffu10.sprintf("%08d",0); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����   �O�d
    //xr_i += xr->tb_newadd.am_ffu10.sprintf("%08d",0); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B   �O�d



    SumBill78(0, gi_bil_line);
    writelog("���ʹ��f����:" + StrBillPay);

     /***
    if (xr_a.Length()!=34)
    {
        writelog( ("xr_a �������� 34 �ثe���� " + IntToStr(xr_a.Length())) );
        xr_a = xr_a.SubString(1,34);
    }

    if (xr_b.Length()!=88)
    {
        writelog( ("xr_b �������� 88 �ثe���� " + IntToStr(xr_b.Length())) );
        xr_b = xr_b.SubString(1,88);
    }

    if (xr_c.Length()!=720)
    {
        writelog( ("xr_c �������� 720 �ثe���� " + IntToStr(xr_c.Length())) );
        xr_c = xr_c.SubString(1,720);
    }

    if (xr_d.Length()!=80)
    {
        writelog( ("xr_d �������� 80 �ثe���� " + IntToStr(xr_d.Length())) );
        xr_d = xr_d.SubString(1,80);
    }

    if (strlen(xre)!=28)
    {
        writelog( ("xr_e �������� 29 �ثe���� " + IntToStr(strlen(xre))) );
        xre[28] = 0;
    }

    if (xr_f.Length()!=640)
    {
        writelog( ("xr_f �������� 640 �ثe���� " + IntToStr(xr_f.Length())) );
        xr_f = xr_f.SubString(1,640);
    }

    if (xr_g.Length()!=32)
    {
        writelog( ("xr_g �������� 32 �ثe���� " + IntToStr(xr_g.Length())) );
        xr_g = xr_g.SubString(1,32);
    }

    if (xr_h.Length()!=168)
    {
        writelog( ("xr_h �������� 168 �ثe���� " + IntToStr(xr_h.Length())) );
        xr_h = xr_h.SubString(1,168);
    }

    if (xr_i.Length()!=208)
    {
        writelog( ("xr_i �������� 208 �ثe���� " + IntToStr(xr_i.Length())) );
        xr_i = xr_i.SubString(1,208);
    }
    ****/

    //XReportŪ�b����
    AnsiString str_xr;

    //str_xr.sprintf("%s%s%s%s%s%s%s%s%8s%88s|\n",
    //str_xr.sprintf("%s%s%s%s%s%s%s%s%8s%79s|\n",    //2012/09/18
    //str_xr.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",     //2013/03/15
    //str_xr.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",     //2013/10/17      �I�d����O Update
    str_xr.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                   xr_a.c_str(),
                   xr_b.c_str(),
                   xr_c.c_str(),
                   xr_d.c_str(),
                   xr_g.c_str(),
                   xr_h.c_str(),
                   xr_i.c_str(),
                   xr_f.c_str(),
                   xr_k.c_str(),
                   "FFFFFFFF"  );





    //report = new REPORT();      //Ū�b�b��

    sLog.sprintf("XREPORT::(%s)Len=%d",str_xr, str_xr.Length());

    writelog( sLog);


    if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
        ForceDirectories("C:\\FTLPOS\\HISTORY\\");

    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\XReport.xpp");
   // report->CreateReport('X', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no);

   // 2006/08/03 for �N������
   report->CreateReport('X', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no,
                        iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );


    sTmp.printf("CreateReport: ���Ͳ���Ū�b�b�� �N��X�p=(%d),(%d)",iqt5_15217totrev,iam5_15217totrev);
    writelog(sTmp);

    writelog("����Ū�b�b�� C:\\FTLPOS\\HISTORY\\XReport.xpp");

    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\XReport.xdp");
    report->CreateReport('d', str_xr, StrBillPay, gchar_tencode, gchar_ecr_no);
    writelog("����Ū�b�b��(����) C:\\FTLPOS\\HISTORY\\XReport.xdp");


    // �ˬd 1050 �� SAL 1050 ���ƬO�_�@�P
    int RtnZDTCnts, RtnSALCnts;
    AnsiString  strchkfile;
    strchkfile.sprintf("C:\\FTLPOS\\XZDATA\\SAL\\%s.sal",gchar_ecr_no);
    _GetZDTZSALCntCheck(gtsl_tot, strchkfile, str_nz_cnt,
                        RtnZDTCnts, RtnSALCnts );

    tif_ecr->WriteString("XREPORT","XREPORTDATE",str_date_time);
    tif_ecr->WriteString("XREPORT","ZDTCnts",IntToStr(RtnZDTCnts) );
    tif_ecr->WriteString("XREPORT","SALCnts",IntToStr(RtnSALCnts) );

    delete xr, tif_ecr;

    

    return 1;
}



AnsiString __fastcall ZDATA::write_xpath(AnsiString FileName)
{
    AnsiString str_file_dd,                //���
               str_file_num,               //����
               str_bak_path;                //�ƥ����|


    AnsiString str_his_folder = "C:\\FTLPOS\\HISTORY\\";
    AnsiString str_abs_file = ExtractFileName(FileName);

    str_file_dd  = str_abs_file.SubString(7,2);      //DD
    str_file_num = str_abs_file.SubString(9,2);      //01~09

    //�ƥ����|
    if ( (!str_file_dd.IsEmpty()) && (!str_file_num.IsEmpty()) )
        str_bak_path = ( str_his_folder + str_file_dd + "\\" + str_file_num + "\\" );
    else
        str_bak_path = str_his_folder;

    //�إߥؿ�
    if (!DirectoryExists(str_bak_path))
        ForceDirectories(str_bak_path);

    return str_bak_path;

}


//����P��Z
int __fastcall CHECKIN::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version)
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName,true, "", "");               //���JSAL�ɡA�N�ɮר̫��A����

    //if (!int_reslut)
    //    return 0;


    AnsiString sLog,sTmp;
    sLog.sprintf("CHECKIN::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);


    AnsiString str_ver_num;

    str_ver_num = get_version(Version);
    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����

    //SPOS_XRPDT *x = new SPOS_XRPDT;   //X�b�ɮ׵��c

    //TIniFile *tif_ecr   = new TIniFile(XZDATA_INI);

    AnsiString str_x_filename,              //XDATA�ɦW
               str_ptran_cnt,               //�e�����b�Ǹ�
               str_date_z,                  //DataZ(����[X�b����)
               str_tran_cnt,                //�}���H�ӵ��b���B
               str_pxno_seq,                //X�b���b����
               str_nxno_seq,                //�U�����b��
               str_YYYYMMDDZ,
               str_date_time,
               ed_date_time ,
               str_nz_cnt,
               SaleDataVer;


    str_date_z = tif_ecr->ReadString("Z","DATEZ","");             //ddn
    //str_tran_cnt = tif_ecr->ReadString("ECR","TRAN_CNT","");      //�ثe����Ǹ�
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("CHECKIN::ECR->WriteData �����ثe������Ǹ�:%s", str_tran_cnt);
    writelog( sLog);


    str_YYYYMMDDZ=tif_ecr->ReadString("Z","YYYYMMDDZ","201001010000");
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "00001");          //999999�ثe�鵲�Ǹ�  ���餣�k�s

    str_date_time = gchar_date_time;

    str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","");        //�W��x�b�ֿn����
                                                                                                              
    if ( str_pxno_seq.Length() < 2)                                                                                    
        str_pxno_seq = "00";                                                                                     
                                                                                                              
    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq) + 1) );   //�ثex����ĴX��


    //�̦h99��X�b
    if ( (_StrToInt(str_nxno_seq) > 99) )                                                                        
        str_nxno_seq = "01";                                                                                     
                                                                                                              
    x->no_tm = gchar_ecr_no;                           //���Ⱦ�����
    x->no_casher = gchar_cashier;                      //���ȭ����X

    if (!x->no_casher.Trim().Length())
        x->no_casher = "00001";
                                                                                                              
    //�鵲�Ǹ�                                                                                                
    x->no_seq = str_nxno_seq;

    AnsiString str_x_first, str_sal_last, str_sal_last_1099;        //*.sal �̫�@��
    str_x_first = gpchar_x_first;
    str_sal_last = gpchar_sal_last;
    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    if (str_x_first != "")
    {
        x->dt_begin = str_x_first.SubString(21,8);   //�}�l���
        x->tm_begin = str_x_first.SubString(29,6);   //�}�l�ɶ�
    }
    else
    {
        x->dt_begin = "00000000";
        x->tm_begin = "000000";
    }


    AnsiString str_invo_char;                    //�o���^��r��
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.Length() < 2)
        str_invo_char = "XX";
                                                                                                              
    if (gbl_tot_haveline)
    {                                                                                                         
        //�o���}�l���X�B�o���������X                                                                          
        x->no_rcstart = invoice_start_end(gi_tot_line, true);
        x->no_rcend   = invoice_start_end(gi_tot_line, false);
    }                                                                                                         
    else                                                                                                      
    {                                                                                                         
        x->no_rcend   = "0000000000";
        x->no_rcstart = "0000000000";                                                                         
    }

    //X������X�ɦW                                                                                           
    str_x_filename = "C:\\Abacus\\HISTORY\\checkin.xdt";
                                                                                                              
    //�����ɶ��ثe�ɶ����ثe���b�ɶ�                                                                          
    x->dt_end = str_date_time.SubString(1,8);
    x->tm_end = str_date_time.SubString(9,6);

    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000"))
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }
                                                                                                              
    AnsiString x_a;
    int iC0TotCount=0;
    x_a.sprintf("1095|%04d|%-6s|%-2s|%14s|%5s|%-10s|%2s|%5s|%8s|%6s|%8s|%6s|",1779,
                                        gchar_tencode,
                                        x->no_tm.c_str(),         //���Ⱦ�����
                                        ed_date_time.c_str(),
                                        str_nz_cnt.c_str(),
                                        str_YYYYMMDDZ.SubString(3,8),
                                        x->no_seq.c_str(),        //��Z�Ǹ�
                                        x->no_casher.c_str(),     //���ȭ����X
                                        x->dt_begin.c_str(),      //�}�l���
                                        x->tm_begin.c_str(),      //�}�l�ɶ�
                                        x->dt_end.c_str(),        //�������
                                        x->tm_end.c_str()         //�����ɶ�
                                     );
                                                                                                              
    //������Ӣ�������=====================================================

    //����ӫȼ�
    x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //����ӫȼ�
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~��
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    x->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;

    // �I�d����O���K�|
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);
    x->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );        //�o���鵲�b�B(�K)
    x->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));        //�o���鵲�b�B(�|)
    x->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));        //�o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    TotMiss(0,gi_tot_line);
    x->tb_trans.qt_miss.sprintf("%08d",    giA);        //�~���o������
    x->tb_trans.am_missntx.sprintf("%08d", giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //�~���o�����B(�K)
    x->tb_trans.am_misstax.sprintf("%08d", giC-RC01TaxAmt-R4_C01TaxAmt - R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //�~���o�����B(�|)
    x->tb_trans.am_miss.sprintf("%08d",    giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        //�~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt- ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //���}�o�����B, ���}�o�����B�~�h��
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX +PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    x->tb_trans.am_totsale.sprintf("%08d", int_totsale);

    AnsiString x_b;

    x_b  = (x->tb_trans.qt_tcust+"|");                      //����ӫȼ�
    x_b += (x->tb_trans.qt_trans+"|");                      //�������~��
    x_b += (x->no_rcstart+"|");                              //�o���}�l���X
    x_b += (x->no_rcend+"|");                                //�o���������X
    x_b += (x->tb_trans.am_trcpntx+"|");                    //�o���鵲�b�B(�K)
    x_b += (x->tb_trans.am_trcptax+"|");                    //�o���鵲�b�B(�|)
    x_b += (x->tb_trans.am_trcptot+"|");                    //�o���鵲�b���B
    x_b += (x->tb_trans.qt_miss+"|");                       //�~���o������
    x_b += (x->tb_trans.am_missntx+"|");                    //�~���o�����B(�K)
    x_b += (x->tb_trans.am_misstax+"|");                    //�~���o�����B(�|)
    x_b += (x->tb_trans.am_miss+"|");                       //�~���o�����B
    x_b += (x->tb_trans.am_totsale+"|");                    //�����ھP�f���J



    //�N��ITABLE===============================================================

    AnsiString x_c;
    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                     //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;             //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);    //giH giI ���w�h�f���B�B����    //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    x->tb_newadd.qt_cetel.sprintf("%08d", giD);          //�N������
    x->tb_newadd.am_cetel.sprintf("%08d", giC);          //�N�����B


    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //x_c += (x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty)+"|");
            //x_c += (x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt)+"|");

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //x_c += (x->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty)+"|");
        //x_c += (x->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt)+"|");
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //�N��X�p����
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //�N��X�p���B   �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN ; //+ ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;

     x_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );




    //�N�{��TABLE===============================================================
    int iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);   //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    x->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);       //§��^��
    x->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //§����B
    SumPresent41+=giB; //§��+�ӫ~§��
    SumPresent41+=iCSMAmt; //���O��   for  ���ȭ��汵�Z���Ӫ�

    x->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //�t�ӧ����^�� �ثe���ϥ�
    x->tb_cash.am_vndcpn.sprintf("%08d", 0);             //�t�ӧ������B

    x->tb_cash.qt_stocpn.sprintf("%08d", giC);           //�����^��
    x->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //�������B
    SumCpn41 +=  giD;      //�����+Other1+�������� CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(�D�������B:giU), �������B:giV)     25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    SumCpn41 -=  giU;
    //x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI - giT));      //�N�{���X�p�^��
    //x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ - iCSMAmt));  //�N�{���X�p���B

    x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //�N�{���X�p�^��
    x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //�N�{���X�p���B

    x->tb_cash.qt_bonout.sprintf("%08d", giG);           //�����X�p�^��
    x->tb_cash.am_bonout.sprintf("%08d", giH);           //�����X�p���B

    iTotOt2Rtn=giE;                                      //giE:�l�B����B / giF:�l�B�馸��
    iqt_ffu1=giI;                                        //�{���d�^��  �q�l������  3275
    iam_ffu1=giJ;                                        //�{���d���B  �q�l�����B  ��d�X�p 3261

    AnsiString x_d;

    x_d += (x->tb_cash.qt_bonus+"|");                //§��^��
    x_d += (x->tb_cash.am_bonus+"|");                //§����B
    x_d += (x->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��
    x_d += (x->tb_cash.am_vndcpn+"|");               //�t�ӧ������B
    x_d += (x->tb_cash.qt_stocpn+"|");               //�����^��
    x_d += (x->tb_cash.am_stocpn+"|");               //�������B
    x_d += (x->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��
    x_d += (x->tb_cash.am_totcash+"|");              //�N�{���X�p���B

    sTmp.sprintf("%08d",giF);
    x_d += (sTmp+"|");                         //�l�B�X�p�^��
    sTmp.sprintf("%08d",giE);
    x_d += (sTmp+"|");                         //�l�B�X�p���B

    x_d += (x->tb_cash.qt_bonout+"|");               //�����X�p�^��
    x_d += (x->tb_cash.am_bonout+"|");               //�����X�p���B


    //==========================================================================

    //�{���X�p �t ���O��
    SumCash41=SumCash(0,gi_tot_line);
    x->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    AnsiString x_e;

    x_e += (x->am_cash+"|");               //�{���X�p
    //x_e += (x->no_rcstart+"|");            //�o���}�l���X
    //x_e += (x->no_rcend+"|");              //�o���������X

    //���ȭ�����FIELD===========================================================
                                                                                                              
    AnsiString x_f;

    //�ɹs
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tb_vendfld[i].am_vend.sprintf("%08d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

        x->tb_casher.tb_vendfld[i].tm_vend.sprintf("%06d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

    x->tb_casher.qt_vendtot.sprintf("%08d",0);
    x->tb_casher.am_vendtot.sprintf("%08d",0);

    //x_f += (x->tb_casher.qt_vendtot+"|");      //�ɹs����
    sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");                          //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)

    //x_f += (x->tb_casher.am_vendtot+"|");      //�ɹs�X�p���B
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    x_f += (sTmp+"|");                           // �N���ӫ~������馩  �ɹs�X�p���B

    AnsiString x_g;

    //��s
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_feedfld[i].am_feed.sprintf("%08d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

        x->tb_casher.tg_feedfld[i].tm_feed.sprintf("%06d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

    x->tb_casher.qt_feedtot.sprintf("%08d",0);
    x->tb_casher.am_feedtot.sprintf("%08d",0);

    x_g += (x->tb_casher.qt_feedtot+"|");      //��s����
    x_g += (x->tb_casher.am_feedtot+"|");      //��s�X�p���B


    AnsiString x_h;
                                                                                                              
    //��w                                                                                                    
    int int_drop_line = (gtsl_drop->Count - 1);   //��w����

    //��l�ƥ�����s
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_dropfld[i].am_drop.sprintf("%08d",0);    //��w���B
        x->tb_casher.tg_dropfld[i].tm_drop.sprintf("%-6s"," ");    //����ɶ�
    }

    int int_drops = 0;                            //��w����
    int int_drop_money = 0;                       //��w���B
    int int_drop_sum = 0;                         //��w�`�B
    int x_rtn_cnt=0;                              //�����ȭ���w�֭p����
    AnsiString str_drop_time,s;

    for (int i=0; i<=int_drop_line; i++)
    {
        s=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("���� ���X �b��:�D��w(%s)���",s);
             writelog(sTmp );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, "|", 11) );     //�{��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 12) );     //IC�I��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 13) );     //�H�Υd
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 14) );    //§��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 15) );    //�����
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 16) );    //�����
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 17) );    //���f��

        //�����o�����B  800   Cloud �����o�����B(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 18) );

        //�����o�����B  500
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 19) );

        //�����o�����B  1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 20) );

        //�����o�����B  200
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 22) );

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 23) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 25) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 27) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 29) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 31) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 33) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 35) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 37) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 39) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 41) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 43) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 45) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 47) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 49) );

        //�C����w���B
        x->tb_casher.tg_dropfld[int_drops].am_drop.sprintf("%08d", int_drop_money);

        int_drop_sum += int_drop_money;                         //��w�`�B

        str_drop_time = _StringSegment_EX(s, "|", 5).SubString(9,6);  //��w�ɶ�

        //�C����w�ɶ�
        x->tb_casher.tg_dropfld[int_drops].tm_drop.sprintf("%-6s", str_drop_time);

        int_drop_money = 0;
        ++int_drops;                             //��w����
     }

    //for (int i=0; i<50; i++)
    //{
   //     x_h += (x->tb_casher.tg_dropfld[i].am_drop+"h");  //��w�`���`�X
   //     x_h += (x->tb_casher.tg_dropfld[i].tm_drop+"h");  //��w���B�`�X
    //}


    AnsiString x_i;

    x_i += (x->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        //��w����
    x_i += (x->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     //��w�X�p���B

    SumUpdat(0,gi_rec_line);
    x_i += (x->tb_casher.qt_update.sprintf("%08d", giA)+"|");               //�󥿦���
    x_i += (x->tb_casher.am_update.sprintf("%08d", giB)+"|");               //�󥿦X�p���B

    SumVoid(0,gi_tot_line);
    x_i += (x->tb_casher.qt_void.sprintf("%08d", giA)+"|");                 //�����������
    x_i += (x->tb_casher.am_void.sprintf("%08d", giB)+"|");                 //��������X�p���B

     SumInpdp(0,gi_rec_line);
    x_i += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //��ʿ�J�������ơB����13�B14
    x_i += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //��ʿ�J�������B

    x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //�}���d����

    //�p�p�馩  1011
    SumDisc(0,gi_pay_line);
    x_i += (x->tb_casher.qt_dise.sprintf("%08d", giA)+"|");                 //�馩����
    x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");                 //�馩���B

    pay_sub_amt(0,gi_pay_line);
    x_i += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //�P�P�馩/������
    x_i += (x->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");         //�P�P�馩/�����B


    easy_card(0,gi_easy_line);
    x_i += (x->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");                  //�[�Ⱦ��}��d����
    x_i += (x->tb_casher.am_disret1.sprintf("%08d", giB)+"|");                  //�[�Ⱦ��}��d���B
    x_i += (x->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");                  //�[�Ⱦ��[�Ȧ���
    x_i += (x->tb_casher.am_disret2.sprintf("%08d", giD)+"|");                  //�[�Ⱦ��[�Ȫ��B


    //x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //�}���d����
    //x_i += (x->tb_casher.qt_pack.sprintf("%08d" , iTotOt2Rtn)+"|"); //giE);     //�[�Ⱦ����b����   2007/12/01 ��l�B��
    //x_i += (x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"I");   //�[�Ⱦ����b���B, ���}�o�����B�~�h��



    //�l�[TABLE=================================================================

                                                                                                              
    AnsiString x_j;                                                                                           
                                                                                                              
   //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //��ʿ�J�������ơB����13�B14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //��ʿ�J�������B

    //fetc_card(0,gi_fetc_line);   // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //�ƥΤK����  �{�� �[�Ȧ���   2015/08/19 �N��,����O�P����B(�K)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B 2015/08/19 �N��,����O�@�o���B(�K)
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���   2015/08/19 �N��,����O�P����B(�K)
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B 2015/08/19 �N��,����O�@�o���B(�K)

    //���w���ơB���B  int iqt_food,iamt_food;
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0
    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //�{���d����   �q�l������  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //�{���d���B   �q�l�����B  ��d�X�p 3261

    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����  �O�d
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"|"); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B  �O�d

    //�o�����B0����

    x_j += (x->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");         //�N�������O����
    x_j += (x->tb_newadd.am_parking.sprintf("%08d", 0)+"|");         //�N�������O���B
    x_j += (x->tb_newadd.qt_cetel+"|");                              //�N������FUNCTIO�b�e��
    x_j += (x->tb_newadd.am_cetel+"|");                              //�N�����B
    //x_j += (x->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix)+"|");       //�N���ӫ~�ȼ�
    //x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");     //�{���d����   �q�l������  3275
    //x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");     //�{���d���B   �q�l�����B  ��d�X�p 3261
    //x_j += (x->tb_newadd.qt_ffu2.sprintf("%08d", 0)+"|");            //���d�����d����                    3269
    //x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //���d�����d���B  2008/12/04 ���O��  3277
    x_j += (x->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //�����e�m�Ÿ�  3285
    x_j += (x->tb_newadd.tx_ver.sprintf("%6s" , str_ver_num)+"|");   //�������X
    x_j += (x->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");    //�o�����B0����

    //���}�o�����B
    x_j += (x->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //�ƥΥ|���B,  ���}�o�����B
    x_j +=(x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"|");   //���}�o���~�h���X�p

    SumLOSS(0,gi_rec_line);
    x_j += (x->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");         //TM�o��Loss����
    x_j += (x->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");         //TM�o��Loss���B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);        // ���d���X�p
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //��[�Ⱦ��[�Ȩ�������
    x_j += (x->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");   //��[�Ⱦ��[�Ȩ������B

    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��
    int iSumRelDimAgn=giB;

    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",giB+iSub_C0InvoAmtC)+"|");   //��[�Ⱦ��[�Ȩ�������
    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //��[�Ⱦ��[�Ȩ�������

    x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //���O��


    //pay_sub_amt(0,gi_pay_line);
    //x_j += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //�P�P�馩/������
    //x_j += (x->tb_newadd.am_ffu6.sprintf("%08d", giB)+"|");         //�P�P�馩/�����B


    //fetc_card(0,gi_fetc_line);
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",giFetcCashAddCnt)+"|");  //�ƥΤK����  �{�� �[�Ȧ���
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",giFetcCashAddamt)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B
    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����  �O�d
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"J"); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B  �O�d


    // 2012/09/25  Update
    if( SaleDataVer >= "2012090100")
      {
          for (int i=0; i<60; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //��w���B
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //��w�ɶ�
            }

      }
    else
      {
          for (int i=0; i<30; i++)
            {
              x_h += ("00000000|");  //�ɹs���B
              x_h += ("      |");    //�ɹs�ɶ�
            }           //"000000|"

          for (int i=0; i<30; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|"; //("00000000|");  //��w���B
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|"; //("000000|");    //��w�ɶ�
            }
     }

    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);    �Y���ʻP�P��i�����|
    x_h +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //�Y���ʻP�P��i�ȧK�|
    x_h +=sTmp;

    //�o���I�����B �����o���i��  ���B
    VoicePayAmt(0,gi_tot_line);
    sTmp.sprintf("%08d|",giA/200); //�o���I���i��
    x_h +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for ���ȭ���Z���Ӫ�
    //SumCash41+=giA;                // 2012/10/08 for ���ȭ���Z���Ӫ�

    //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    x_h +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�I�d����OgiD.�@�ogiE , �I�d����O�ȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt, iC0GivUpAmt, iC0TotCount); //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)
    x_h +=sTmp;

    SumBill78(0, gi_bil_line);
    writelog("���ʹ��f����:" + StrBillPay);

    //X����
    AnsiString xdt_1095_data;
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%88s|\n",
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%79s|\n",   2012/09/18
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%61s|\n",   2013/03/15
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%43s|\n",   2013/10/17   �I�d����O Update
    xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%16s|\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),
                          x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          "1FFFFFFF"
                          );




    FILE *fxdata;
    if ((fxdata = fopen(str_x_filename.c_str(),"w")) != NULL)
    {
        fprintf(fxdata,"%s",xdt_1095_data.c_str() );
        fclose(fxdata);
    }
    else
    {
         sTmp.printf("File Open Error:%s",str_x_filename.c_str());
         writelog(sTmp);
    }

    //X����                                                                                                   
    AnsiString str_xrp_checkin,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_cashier_no,
               str_start_trans;
                                                                                                              
    str_xrp_checkin=xdt_1095_data;

    if (!DirectoryExists("C:\\FTLPOS\\HISTORY\\"))
        ForceDirectories("C:\\FTLPOS\\HISTORY\\");
        
    //report = new REPORT();      //����b��
    strcpy(report->char_filename, "C:\\FTLPOS\\HISTORY\\checkin.xrp");
    report->CreateReport('c', str_xrp_checkin, StrBillPay, gchar_tencode, gchar_ecr_no,
                              iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 0 );
    writelog("���ͳ���b�� C:\\FTLPOS\\HISTORY\\checkin.xrp");


    /**************************************************************************/


    str_tencode = gchar_tencode;   //����
    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"����"));              //����
    str_sys_date = (x->dt_end + x->tm_end);                              //����ɶ�

    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //���ȭ��N��

    str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //�}�l����Ǹ��U���b���}�l�Ǹ�

    AnsiString str_trans1_5, CasherRptFile;    //����Ǹ��e5�X
    //str_trans1_5 = str_sal_last.SubString(36,5);

    CasherRptFile=ChangeFileExt(report->char_filename,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    sTmp.sprintf("1041|0500|%6s|%2s|%14s|%5s|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        (str_trans1_5 + str_tran_cnt).c_str(),
                        str_cashier_no.c_str()
                        );


    //���ͦ��ȭ���Z���Ӫ�  ���ȭ��汵�Z���Ӫ�
    AnsiString SumCasherRptData;
    SumCasherRpt(NAUTOZ, 0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt, sTmp, "00",
                 CasherRptFile, "", "", "", "", SumCasherRptData, iamt_food, iqt_food);
    sTmp.printf("���ͦ��ȭ���Z���Ӫ�:%s",CasherRptFile);
    writelog(sTmp );

   // delete x, tif_ecr, report;
    return 1;
}

CHECKIN::CHECKIN()
{
    x = new SPOS_XRPDT;   //X�b�ɮ׵��c
    tif_ecr   = new TIniFile(XZDATA_INI);
    report = new REPORT();      //����b��
    writelog("���� �}�l");

}

CHECKIN::~CHECKIN()
{
    // delete report;      //����b��
    delete x, tif_ecr, report;
    writelog("���� ����");
}

XDATA::XDATA()
{
    //
    writelog("���� ���ȭ��b�� �}�l");
    x = new SPOS_XRPDT;         //X�b�ɮ׵��c
    tif_ecr = new TIniFile(XZDATA_INI);
    tif_tmp = new TIniFile(XDATATMP_INI);
    report = new REPORT();      //���ȭ��b��
}

XDATA::~XDATA()
{
    //
    delete report;      //���ȭ��b��
    delete x,tif_ecr;
    delete tif_tmp;
    writelog("���� ���ȭ��b�� ����");
}

XREPORT::XREPORT()
{
    //
    writelog("Ū�b �}�l");
    report = new REPORT();      //Ū�b�b��
}

XREPORT::~XREPORT()
{
    //
    delete report;      //Ū�b�b��

    writelog("Ū�b ����");
}

ZDATA::ZDATA()
{
    //
    writelog("Z�b �}�l");
    z = new SPOS_ZRPDT;         //Z�b���c
    tif_tmp = new TIniFile(ZDATATMP_INI);
    tif_ecr = new TIniFile(XZDATA_INI);
    report = new REPORT();      //�鵲�b��

    

}

ZDATA::~ZDATA()
{
    delete z, tif_ecr;

    delete report;      //�鵲�B�����b��
    delete tif_tmp;

    writelog("Z�b ����");

}

void __fastcall ZDATA::mtn_dat_folder() //���@DAT�U����30�餺
{
    String Dir;
    String FileName;
    TSearchRec SearchRec;
    int iAttributes = 0;
    iAttributes |= faAnyFile * true;

    AnsiString str_dat_path = "C:\\Abacus\\download\\dat\\";
    Dir = str_dat_path;

    TStringList *TempList = new TStringList;
    try
    {

        TempList->Clear();

        if (FindFirst(Dir+"*.tm", iAttributes, SearchRec) == 0)
        {
            do
            {
                if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
                {
                    TempList->Add(SearchRec.Name);
                }
            } while (FindNext(SearchRec) == 0);

            FindClose(SearchRec);
        }

        AnsiString str_dd_flag = FormatDateTime("yymmdd",Now()-30);
        AnsiString str_dd_file;

        for ( int i = 0; i <= (TempList->Count-1); i++)
        {
            FileName = TempList->Strings[i];

            if (FileName.Length() < 13)
            {
                TempList->Delete(i);
                i = 0;
                continue;
            }

            str_dd_file = FileName.SubString(3,6);

            if (str_dd_file.AnsiCompare(str_dd_flag) < 0)
            {
                FileName = (Dir + TempList->Strings[i]);
                DeleteFile(FileName);
                writelog("���@DAT�R���ɮ� " + FileName);
            }
        }
    }
    __finally
    {
        delete TempList; // destroy the list object
    }

}

void __fastcall ZDATA::del_list_file(TStringList *list, AnsiString path)
{
    AnsiString str_sal_00;

    for (int i=0; i<list->Count; i++)
    {
        str_sal_00 = (path + list->Strings[i]);
        DeleteFile(str_sal_00);
        writelog("�T�Q�ѥؿ����@�R���ɮ� " + str_sal_00);
    }

    list->Clear();
}

/////////////////////////////////////////////////////////////////////////////////////






SPCDATA::SPCDATA()
{
    //
    int i;
    writelog("���I�ӫ~�C�ޱb�� �}�l");
    tif_ecr = new TIniFile(XZDATA_INI);
    //tif_tmp = new TIniFile("C:\\ABACUS\\XDT_TMP.INI");
    //report = new REPORT();      //���ȭ��b��
    tb_setgrpcount=0;
    for(i=0; i<109; i++)
       {
        tb_setgrp[i].no_setgrp="";
        tb_setgrp[i].nm_setgrp="";
        tb_setgrp[i].no_setsubgrp="";
        tb_setgrp[i].qt_setgrp=0;
        tb_setgrp[i].am_setgrp=0;
        tb_setgrp[i].MustPrinted=0;
       }


    for(i=0; i<109; i++)
       {
               tb_30TMgrp[i].no_setgrp="";
               tb_30TMgrp[i].nm_setgrp="";
               tb_30TMgrp[i].amt_minpric=0;
               tb_30TMgrp[i].amt_maxpric=0;
               tb_30TMgrp[i].no_setsubgrp="";
       }

    Get30TMData();   

}



SPCDATA::~SPCDATA()
{

    //delete report;      //���ȭ��b��
    delete tif_ecr;
    //delete tif_tmp;
    writelog("���I�ӫ~�C�ޱb�� ����");
}



//���I�ӫ~�C�ޱb��
//iQrySw=1 ,�d��
int __fastcall SPCDATA::WriteSpcInqData(const AnsiString SalFileName, const AnsiString Version,
                                        const int iQrySw )
{


    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //���JSAL�ɡA�N�ɮר̫��A����

    if (!int_reslut)
        return 0;

    //TStringList  *Tm30Data = new TStringList;
    //TStringList  *tsl_Spcdata = new TStringList;

    //SPOS_XRPDT *x = new SPOS_XRPDT;         //X�b�ɮ׵��c

    AnsiString str_x_filename,              //XDATA�ɦW
               str_ptran_cnt,               //�e�����b�Ǹ�
               str_date_z,                  //DataZ(����[X�b����)
               str_tran_cnt,
               str_pxno_seq,                //X�b���b����
               str_nxno_seq,                //�U�����b��
               str_pSPCDttm,
               str_pSPC0010DataKey,
               str_pSpcno_seq,
               str_npSpcno_seq,
               str_nz_cnt,
               str_ecr_no,
               str_trans1_5,
               pzDate_date_time,
               str_tencode,
               sTmp,
               sLog;


    pzDate_date_time=tif_ecr->ReadString("Z","PZ_DATE", FormatDateTime("yyyymmddhhnnss",Now()) );
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s
    str_date_z = tif_ecr->ReadString("Z","DATEZ","");           //ddn
    //str_tran_cnt = tif_ecr->ReadString("ECR","TRAN_CNT","");    //�ثe����Ǹ�
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("SPCDATA::ECR->WriteData �����ثe������Ǹ�:%s", str_tran_cnt);
    writelog( sLog);


    AnsiString str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());
/***
#ifdef TM_20
    str_date_time = tif_ecr->ReadString("Z","pz_date", gchar_date_time);
    if (str_date_time.Length() < 8)
           str_date_time = FormatDateTime("yyyymmddhhnnss",Now());
#else
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());
#endif
****/

    if (str_date_z.Length() < 3)    //DATAZ=���+Z�b����  ddn
        str_date_z = (str_date_time.SubString(7,2) + "1");

    AnsiString str_sal_last;        //LOG�̫�@��
    str_sal_last = gpchar_sal_last;

    if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
        str_tran_cnt = _StringSegment_EX(str_sal_last, "|", 7).SubString(6,5);
        //str_sal_last.SubString(41,5);

    str_trans1_5 = _StringSegment_EX(str_sal_last, "|", 7).SubString(1,5);
        //str_sal_last.SubString(36,5);


    str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","");                  //�W�����I�ӫ~�C�ޱb���ֿn����
    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //�ثe���I�ӫ~�C�ޱb����ĴX��
    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //�̦h99��X�b
        str_nxno_seq = "01";


    str_pSpcno_seq = tif_ecr->ReadString("SPC","pSPCno_seq","");                //�W��x�b�ֿn����
    str_npSpcno_seq.sprintf("%02d", (_StrToInt(str_pSpcno_seq,"pSPCno_seq") + 1)); //�ثex����ĴX��

    str_pSPCDttm = tif_ecr->ReadString("SPC","pSPCDttm",pzDate_date_time);
    str_pSPC0010DataKey=tif_ecr->ReadString("SPC","pSPCData","");                //�W�����I�ӫ~�C�ޱb���̫�@��0010
    //str_pSPC0010DataKey:yyyymmddhhnnss|00ZZn99999

    if (_StrToInt(str_nxno_seq,"pSPCno_seq") > 99)  //�̦h99��X�b
        str_nxno_seq = "01";

    //x->no_tm = gchar_ecr_no;                      //���Ⱦ�����
    //x->no_casher = gchar_cashier;                 //���ȭ����X
    //if (x->no_casher.Length() < 5)
    //    x->no_casher = "00001";

    //�鵲�Ǹ�
    //x->no_seq = str_nxno_seq;

    //AnsiString str_x_first;                      //X�b�Ĥ@��LOG(���N����SAL�Ĥ@��)
    //str_x_first = gpchar_x_first;

    //x->dt_begin = tif_ecr->ReadString("X","login_dttm","").SubString(1,8); //�}�l���
    //x->tm_begin = tif_ecr->ReadString("X","login_dttm","").SubString(9,6); //�}�l�ɶ�

    AnsiString str_invo_char;                    //�o���^��r��
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "");

    if (str_invo_char.Length() < 2)
        str_invo_char = "XX";

    /***
    if (gbl_tot_haveline)
    {
        x->no_rcstart = invoice_start_end(gi_tot_line, true);    //�o���}�l���X
        x->no_rcend = invoice_start_end(gi_tot_line, false);     //�o���������X
    }
    else    //�L����o��
    {
        //x->no_rcend   = str_invo_char + "00000000";              //�o���}�l���X
        //x->no_rcstart = str_invo_char + "00000000";              //�o���������X
        x->no_rcend   = "0000000000";              //�o���}�l���X
        x->no_rcstart = "0000000000";              //�o���������X
    }
    ***/

    AnsiString str_pzno_seq;
    str_pzno_seq = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b����   �����k1

    //20031230
    //�����ɶ��ثe�ɶ����ثe���b�ɶ�

    str_ecr_no.sprintf("%02d", atoi(gchar_ecr_no) );              //����
    str_tencode = gchar_tencode;   //����
    ed_date_time = FormatDateTime("yyyymmddhhnnss",Now());

    AnsiString spc_a;
    /**
     spc_a.sprintf("1080|0189|%-6s|%2s|%14s|%5s|%10s|%2s|%-8s|%-2s|%14s|",
                                        gchar_tencode,
                                        str_ecr_no.c_str(),
                                        str_date_time.c_str(),
                                        str_nz_cnt.c_str(),       //�鵲�Ǹ�
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_nxno_seq.c_str(),
                                        gchar_cashier,
                                        str_npSpcno_seq.c_str(),
                                        str_pSPCDttm.c_str()
                                     );

    **/
    spc_a.sprintf("1080|0189|%-6s|%2s|%14s|%5s|%10s|%2s|%10s|%14s|",
                                        gchar_tencode,
                                        str_ecr_no.c_str(),
                                        str_date_time.c_str(),
                                        str_nz_cnt.c_str(),       //�鵲�Ǹ�
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_nxno_seq.c_str(),
                                        " ",                      //TM�έp����
                                        str_pSPCDttm.c_str()
                                     );

    //���I�ӫ~�C�ޱb��
    tb_setgrpcount=0;

    tb_setgrpcount=SumSPCData(0, gi_rec_line, str_pSPC0010DataKey);

    //�g�JSAL��97Type
    AnsiString str_rec_type,
               str_rec_len,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver,
               str_write_PrvRpt;

    str_rec_type = "1097";
    str_rec_len  = "0106";

    //str_sys_date = (x->dt_end + x->tm_end);                              //����ɶ�

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"�ثe�g�J97����Ǹ�"); //�ثe�g�J97����Ǹ�
    str_trans_cnt.sprintf("%05d", int_sum_transcnt);

    AnsiString str_sum_tran;       //�������ȭ��b����Ǹ�
    //20031215
    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1));

    //xxx
    //tif_tmp->WriteString("SUM","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_trans_cnt;

    // 2007/07/17
    str_z_cnt.sprintf("%05d", _StrToInt(str_nxno_seq,"��Z�Ǹ�"));
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s

    /************************
    //str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //���ȭ��N��

    //str_start_trans = str_x_first.SubString(36,10);                 //�}�l����Ǹ��U���b���}�l�Ǹ�

    //str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //�����ھP�f���J
    //str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF|";


    AnsiString sTmp, CasherRptFile;;    //����Ǹ��e5�X
    //str_trans1_5 = str_sal_last.SubString(36,5);
   // CasherRptFile=ChangeFileExt(str_xdt_path,".Crp");

   // sTmp.sprintf("0041|0471|%6s|%2s|%14s|%10s|%5s|%8s",
   //                     str_tencode.c_str(),
   //                    str_ecr_no.c_str(),
   //                     str_sys_date.c_str(),
   //                     (str_trans1_5 + str_tran_cnt).c_str(),
   //                     str_nz_cnt.c_str(),
   //                     str_cashier_no.c_str()
   //                     );
    
    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%6s|%2s|%14s|%10s|%5s|%2s|00|0000000000|FFFFFFFF|FFFFFFFF|",
                                        "0001",
                                        "0089",
                                        str_tencode.c_str(),
                                        str_ecr_no.c_str(),
                                        str_sys_date.c_str(),
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_z_cnt.c_str(),
                                        "Z0");

    str_99 = str_01;
    str_99 = "0099" + str_99.Delete(1,4);


    AnsiString _str_tran_cnt;    //����Ǹ�

    _str_tran_cnt = (str_trans1_5 + str_trans_cnt);

    //�g�J97����Ǹ�
    AnsiString str_97;
    str_97.sprintf("%4s|%4s|%6s|%2s|%14s|%10s|%5s|%8s|%10s|%8s|%10s|%9s\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        _str_tran_cnt.c_str(),
                        str_z_cnt.c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );



    String str_sal;
    str_sal = str_01 + "\n" +  str_97 + str_99;
    ************************/



    AnsiString str_write_sal, str_write_rpt, stmp;

    if( tb_setgrpcount > 0)
      {
       if( iQrySw == 1)   // �d��
         {
          str_write_sal = "c:\\abacus\\TmpSpc.sal";
          str_write_rpt = "c:\\abacus\\posdata\\TmpSpc.rpt";
          str_write_PrvRpt = "c:\\abacus\\posdata\\TmpPrvSpc.rpt";

          if (FileExists( str_write_sal))
                DeleteFile( str_write_sal);

          //if (FileExists( str_write_rpt))
          //      DeleteFile( str_write_rpt);

          if( WriteSPCSAL(spc_a,str_write_sal))
            {
             tif_ecr->WriteString("SPC","TMPSPfilename", str_write_sal);

             //if( WriteSPCReport(spc_a, str_write_rpt) )
             //    tif_ecr->WriteString("SPC","TMPSPRptname", str_write_rpt);
             //else
             //    tif_ecr->WriteString("SPC","TMPSPRptname", "");

            }
          else
            {
              tif_ecr->WriteString("SPC","TMPSPfilename", "");
              //tif_ecr->WriteString("SPC","TMPSPRptname", "");
            }

         }
       else
         {
           //�g�J���nn.SAL��

          if (WriteSPCSAL(spc_a,SalFileName))
             {
                 tif_ecr->WriteString("ECR","TRAN_CNT", str_sum_tran);
                 str_write_sal = "C:\\FTLPOS\\XZDATA\\00000" + str_trans_cnt + ".sal";
                 str_write_rpt = "C:\\FTLPOS\\XZDATA\\TmpSpc.rpt";
                 str_write_PrvRpt = "C:\\FTLPOS\\XZDATA\\TmpPrvSpc.rpt";

                if (FileExists( str_write_sal))
                    DeleteFile( str_write_sal);

                if (FileExists( str_write_rpt))
                    DeleteFile( str_write_rpt);

                if( WriteSPCSAL(spc_a,str_write_sal))
                    {
                     tif_ecr->WriteString("SPC","SPC0080filename", str_write_sal);
                     if( WriteSPCReport(spc_a, str_write_rpt) )
                        {
                          tif_ecr->WriteString("SPC","TMPSPRptname", str_write_rpt);
                          if( CopyFile( str_write_rpt.c_str(), str_write_PrvRpt.c_str(), false ) )
                              tif_ecr->WriteString("SPC","TMPPRVSPRptname", str_write_PrvRpt);
                        }
                     else
                        {
                          tif_ecr->WriteString("SPC","TMPSPRptname", "");
                        }

                    }
                else
                   {
                    tif_ecr->WriteString("SPC","SPC0080filename", "");
                    tif_ecr->WriteString("SPC","TMPSPRptname", "");
                   }

                //xxx
               //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));
               //tif_tmp->WriteString("X","PXNO_SEQ",   str_nxno_seq);

               tif_ecr->WriteString("SPC","pSPCDttm",str_date_time);
               tif_ecr->WriteString("SPC","pSPCTran_cnt",str_date_z + str_ptran_cnt );
               tif_ecr->WriteString("SPC","pSPCno_seq",str_npSpcno_seq.Trim() );
               tif_ecr->WriteString("SPC","pSPCData",str_pSPC0010DataKey.Trim() );
               stmp.sprintf("WriteSpcInqData: ���I�ӫ~�C�޸�ƳB�̧���, �]�w Ecr_no.ini->SPC->pSPCData=%s",
                             str_pSPC0010DataKey.c_str() );
               writelog(stmp);
            }
          else
            {
              tif_ecr->WriteString("SPC","SPC0080filename", "");
              tif_ecr->WriteString("SPC","TMPSPRptname", "");
              tb_setgrpcount=0;
            }

        } // end of else
    } // end of  if( tb_setgrpcount > 0)
   else
    {
       tif_ecr->WriteString("SPC","SPC0080filename", "");
       tif_ecr->WriteString("SPC","TMPSPfilename", "");
       tif_ecr->WriteString("SPC","TMPSPRptname", "");
       tb_setgrpcount=0;
       stmp.sprintf("WriteSpcInqData: �L���I�ӫ~�C�޸�� " );
       writelog(stmp);

    }

   //delete x,tif_ecr;

    return(tb_setgrpcount);
}




// ���I�ӫ~�C�ޱb��
//int __fastcall BASIC::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey)

int __fastcall SPCDATA::SumSPCData(int StartLine, int TotalLine, AnsiString &PrvSPCDataKey )
{
    AnsiString sRType,  //�h�f
               sType, stmp, sSpcType,s;
    AnsiString str_dept, tmpDttm_Tran, tmpStDttm_Tran, tmpEdDttm_Tran;

    int iDept, iDataCnt;

    iDept = iDataCnt = 0;      //�����O
    //�ƶq�B���B
    giA = giB = 0;

    tmpStDttm_Tran=PrvSPCDataKey;
    tmpDttm_Tran="";

    if (gbl_rec_haveline)
    {
        for (int i=StartLine; i<=TotalLine; i++)
        {
            s=gtsl_rec->Strings[i];

            //_StringSegment_EX(ss, "|", 7).SubString(6,5);

            //str_dept = gtsl_rec->Strings[i].SubString(119,2).Trim();  //����
            //str_dept = gtsl_rec->Strings[i].SubString(124,2).Trim();    //�P�O�C�޸s��
            str_dept = _StringSegment_EX(s, "|", 20).SubString(6,2);

            //tmpDttm_Tran=gtsl_rec->Strings[i].SubString(21,14).Trim()+"|";
            tmpDttm_Tran=_StringSegment_EX(s, "|", 5)+"|";

            //tmpDttm_Tran+=gtsl_rec->Strings[i].SubString(36,10).Trim()+"|";
            tmpDttm_Tran+=_StringSegment_EX(s, "|", 7)+"|";

            giA = giB = 0;


            if( tmpDttm_Tran <= PrvSPCDataKey )
              continue;

            if (str_dept.Length() > 0)
                iDept = _StrToInt(str_dept);
            else
                continue;

            if( iDept < 1 && iDept > 99 )
              {
                 stmp.sprintf("SumSPCData:1010->�C�޸s�� Error (%s-%s-%d)",
                               tmpDttm_Tran, str_dept, iDept );
                 writelog(stmp);
                 continue;
              }

            sRType = _StringSegment_EX(s, "|", 38).SubString(1,2);
            //gtsl_rec->Strings[i].SubString(370,2);

            sType = _StringSegment_EX(s, "|", 8);
            //gtsl_rec->Strings[i].SubString(68,2);


            sSpcType = _StringSegment_EX(s, "|", 10).Trim();
            //gtsl_rec->Strings[i].SubString(224,1);
            if ( sType.Pos("A0") || sType.Pos("A1") )
               { ;; }
            else if ( sType.Pos("S0") || sType.Pos("S1") )
                    {
                        if( sSpcType == "2" || sSpcType == "02" )  //�N��
                           { ;; }
                        else
                           { continue; }
                    }
            else
               { continue; }

            iDataCnt++;
            //���`�P��
            if (sRType.Pos("FF") || sRType.Pos("Z0") || sRType.Pos("Z1"))
            {
                    //sType = gtsl_rec->Strings[i].SubString(68,2);

                    if (sType.Pos("A0") || sType.Pos("S0") )        //�����P��
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        //_StrToInt(gtsl_rec->Strings[i].SubString(142,4));


                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
                        //_StrToInt(gtsl_rec->Strings[i].SubString(159,8));
                    }
                    else if (sType.Pos("A1") || sType.Pos("S1"))   //�P�����
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
                    }

                    //tb_setgrp[iDept].no_setgrp="";
                    //tb_setgrp[iDept].nm_setgrp="";
                    //tb_setgrp[iDept].no_setsubgrp="";
                    tb_setgrp[iDept].MustPrinted=1;
                    tb_setgrp[iDept].qt_setgrp+=giA;
                    tb_setgrp[iDept].am_setgrp+=giB;
                    tb_setgrp[iDept].no_setgrp=str_dept;
                    tb_setgrp[iDept].nm_setgrp=tb_30TMgrp[iDept].nm_setgrp;
                    if( Trim(tb_setgrp[iDept].nm_setgrp) == "")
                        tb_setgrp[iDept].nm_setgrp="�C�޸s "+str_dept;

                    tb_setgrp[iDept].no_setsubgrp=tb_30TMgrp[iDept].no_setsubgrp;


            }
            else if ( sRType.Pos("R2") || sRType.Pos("R3") )    //�e���~���B�h�f
            {

                    //sType = gtsl_rec->Strings[i].SubString(68,2);

                    if (sType.Pos("A0") || sType.Pos("S0"))        //�����P��
                    {
                        giA -= _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        giB -= _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
                    }
                    else if (sType.Pos("A1") || sType.Pos("S1"))   //�P�����
                    {
                        giA += _StrToInt(_StringSegment_EX(s, "|", 26));   // QT_ITEM
                        giB += _StrToInt(_StringSegment_EX(s, "|", 29));   //AM_ITEM
                    }

                    tb_setgrp[iDept].MustPrinted=1;
                    tb_setgrp[iDept].qt_setgrp+=giA;
                    tb_setgrp[iDept].am_setgrp+=giB;
                    tb_setgrp[iDept].no_setgrp=str_dept;
                    tb_setgrp[iDept].nm_setgrp=tb_30TMgrp[iDept].nm_setgrp;
                    if( Trim(tb_setgrp[iDept].nm_setgrp) == "")
                        tb_setgrp[iDept].nm_setgrp="�C�޸s "+str_dept;
                    tb_setgrp[iDept].no_setsubgrp=tb_30TMgrp[iDept].no_setsubgrp;
         
             }

        }
    }

    PrvSPCDataKey=tmpDttm_Tran;
    stmp.sprintf("SumSPCData:1010->���I�ӫ~�C�ޱb����Ƶ���(%d) ST_Dttm_Tran(%s)~~ED_Dttm_Tran(%s)",
                iDataCnt, tmpStDttm_Tran.c_str(), PrvSPCDataKey.c_str() );
    writelog(stmp);

    //PrvSPCDataKey=tmpDttm_Tran;
    return iDataCnt;
}






int __fastcall SPCDATA::WriteSPCSAL(AnsiString strHdata,  AnsiString SALFileName)
{
    FILE *f_write_sal;
    AnsiString tmpdata, str_01, str_99, tmp_str;
    int iCnt,i, cnt80, cnt81;

    iCnt=cnt80=cnt81=0;

    /******************************************/
    TStringList *tmpsl = new TStringList();
    tmpsl->Clear();
    tmp_str="c:\\FTLPOS\\XZDATA\\1081SpcTmp.txt";  // FTLPOS.exe ����
    if (FileExists(tmp_str) )
       {
        tmpsl->LoadFromFile(tmp_str);
        tmpdata.sprintf("WriteSPCSAL: Load �C�ްӫ~�P����Ӹ��(%s) (1081����=%d) OK.",
                        tmp_str.c_str(),tmpsl->Count  );
        writelog(tmpdata);
        //DeleteFile(tmp_str);    doont delete this file
       }
     else
       {
        tmpdata.sprintf("WriteSPCSAL: �C�ްӫ~�P����Ӹ��(%s) not find.",
                        tmp_str.c_str(),tmpsl->Count  );
        writelog(tmpdata);
       }
    /****************************************/


    for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp == 0 && tb_setgrp[i].MustPrinted==0)
         if(tb_setgrp[i].qt_setgrp == 0 )   // 2009/10/02
           continue;

         iCnt++;
       }

    if( (tb_setgrpcount <= 0 || iCnt <=0) && tmpsl->Count==0)
       {
        writelog(" �L��Ƽg�JSAL��(1080-1081) " + SALFileName);
        return 0;
      }

    if ((f_write_sal = fopen(SALFileName.c_str(), "a")) == NULL)
    {
        writelog("�g�JSAL��(1080-1081)�o�Ϳ��~ " + SALFileName);
        delete tmpsl;
        return 0;
    }


    // sprintf(sal_filename, "c:\\abacus\\sales\\00000%05ld.SAL",tran_cnt);
    // sprintf(sal_filename, "%s%s%3s%05ld.SAL",ABACUS_SALES,gchr_ecr_no,zini_datez,tran_cnt);
    // P_ecrno_sal_file=fopen(sal_filename, "w");



    /**
    tmpdata.sprintf("0080|0177|%-6s|%2s|%14s|%10s|%5s|%2s|%-8s|%-10s|%14s|",
                                        gchar_tencode,
                                        gchar_ecr_no,
                                        str_date_time.c_str(),
                                        (str_trans1_5 + str_tran_cnt).c_str(),
                                        str_nz_cnt.c_str(),       //�鵲�Ǹ�
                                        str_nxno_seq.c_str(),
                                        gchar_cashier,
                                        " ",
                                        str_pSPCDttm.c_str()
                                     );
   ***/



    str_01.sprintf("%4s|%4s|%9s|%31s|%2s|00|0000000000|FFFFFFFF|FFFFFFFF|",
                                        "0001",
                                        "0089",
                                        strHdata.SubString(11,9).c_str(),
                                        strHdata.SubString(21,31).c_str(),
                                        "Z0");

   fprintf(f_write_sal,"%s\n",str_01.c_str());

   /*********************1080 OutPut*****************************/
   for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp==0 && tb_setgrp[i].MustPrinted==0 )
         if(tb_setgrp[i].qt_setgrp==0 )  // 2009/10/02
           continue;

         cnt80++;
         tmpdata.sprintf("%s%-2s|%-30s|%-30s|%08ld|%08.0f|%8s|%8s|",
                           strHdata.c_str(),
                           tb_setgrp[i].no_setgrp.c_str(),
                           tb_setgrp[i].nm_setgrp.c_str(),
                           tb_setgrp[i].no_setsubgrp.c_str(),
                           tb_setgrp[i].qt_setgrp,
                           tb_setgrp[i].am_setgrp,
                           "FFFFFFFF",
                           "FFFFFFFF"  );
         fprintf(f_write_sal,"%s\n",tmpdata.c_str());
       }

   /*********************0081 OutPut*****************************/
   if( tmpsl->Count > 0)
     {
       for (int i=0; i<tmpsl->Count; i++)
           {
             cnt81++;
             tmpdata = tmpsl->Strings[i];
             fprintf(f_write_sal,"%s\n",tmpdata.c_str());
            }
     }
   delete tmpsl;
   /***********************************************************/

   str_99 = str_01;
   str_99 = "1099" + str_99.Delete(1,4);
   fprintf(f_write_sal,"%s\n",str_99.c_str());
   //tif_tmp->WriteString("X","x0097filename", str_write_sal);


   fclose(f_write_sal);

   tmpdata.sprintf("WriteSPCSAL: �C�ްӫ~�P��έp(%s) 0080����=%d, 0081����=%d OK. ",
                                 SALFileName.c_str(), cnt80, cnt81 );
   writelog(tmpdata);

   return iCnt;
}




int __fastcall SPCDATA::Get30TMData()
{

   FILE *plu_file;
   AnsiString sQryFileName,stmp,sData;
   int LineLength, igQryStatus, tot_cnt, itmpPrice;
   char tmp_buf[256],data_buffer[513],c;
   int handle, tmp_no_setgrp;

   LineLength=0;
   igQryStatus=0;
   tot_cnt=0;

  sQryFileName.sprintf("c:\\abacus\\posdata\\30.tm");

  if((handle=open(sQryFileName.c_str(), O_RDONLY )) == -1)
     {
      sprintf( tmp_buf,"Get30TMData: %s ��Ʀ��~. (file not find)",sQryFileName);
      writelog(AnsiString(tmp_buf));
      return(0);
     }
  else
     {
      LineLength = filelength(handle);
      close(handle);
     }



  if( LineLength < 80)
     {
      sprintf( tmp_buf,"Get30TMData: CheckLength %s ��Ʀ��~. DATA Length < %d",sQryFileName.c_str(), 80);
      writelog(AnsiString(tmp_buf));
      return(0);
     }

  if ((plu_file=fopen(sQryFileName.c_str(), "r")) == NULL)
       {
        sprintf( tmp_buf,"Get30TMData: %s ��Ʀ��~. (fopen error)",sQryFileName.c_str());
        writelog(AnsiString(tmp_buf));
        return(0);
       }


  while( !feof(plu_file) )
      {
        memset( data_buffer,'\0',sizeof(data_buffer));
        c=0;

        scan_fixstr(plu_file, data_buffer, 78);
        data_buffer[78] = 0;
        stmp=AnsiString(data_buffer);

        fscanf(plu_file,"%c",&c );

        tot_cnt++;
        if( c != 0x0d && c != 0x0a)
          {
            sprintf( tmp_buf,"Get30TMData: %s ���0x0d0x0a���~. (���=%ld)",sQryFileName.c_str(),tot_cnt);
            writelog(AnsiString(tmp_buf));
            //Post=false;
            igQryStatus=0;
            break;
            }

        //sData="|";
          tmp_no_setgrp=_StrToInt(stmp.SubString(1,2));
          if( tmp_no_setgrp > 0 &&  tmp_no_setgrp < 100)
            {
               tb_30TMgrp[tmp_no_setgrp].no_setgrp=stmp.SubString(1,2).Trim();
               tb_30TMgrp[tmp_no_setgrp].nm_setgrp=stmp.SubString(3,30).Trim();

               itmpPrice=_StrToInt(stmp.SubString(33,8));
               tb_30TMgrp[tmp_no_setgrp].amt_minpric=itmpPrice;

               itmpPrice=_StrToInt(stmp.SubString(41,8));
               tb_30TMgrp[tmp_no_setgrp].amt_maxpric=itmpPrice;

               tb_30TMgrp[tmp_no_setgrp].no_setsubgrp=stmp.SubString(49,30);

              //sData+=(stmp.SubString(1,2)+"|"+stmp.SubString(3,30)+"|"+stmp.SubString(33,8)+"|"+stmp.SubString(41,8)+"|");
             //sData+=(stmp.SubString(49,3)+" "+stmp.SubString(52,3)+" "+stmp.SubString(55,3)+" "+stmp.SubString(58,3)+" "+stmp.SubString(61,3)+" ");
             //sData+=(stmp.SubString(64,3)+" "+stmp.SubString(67,3)+" "+stmp.SubString(70,3)+" "+stmp.SubString(73,3)+" "+stmp.SubString(76,3)+" |FFFFFFFF|");
             //ls->Add(sData);
            }
          else
            {
              sprintf( tmp_buf,"Get30TMData: NO_SETGRP ��Ʀ��~. (%s)",stmp.c_str() );
              writelog(AnsiString(tmp_buf));
            }

        fscanf(plu_file,"\n");
   }  // end of while

  fclose(plu_file);

  sprintf( tmp_buf,"Get30TMData: %s (�`����=%ld)", sQryFileName.c_str(), tot_cnt );
  writelog(AnsiString(tmp_buf));

  return(tot_cnt);
}



int __fastcall SPCDATA::WriteSPCReport(AnsiString strHdata, AnsiString RptSALFileName)
{

   TStringList *tslCrp, *tsltmp;
   tslCrp = new TStringList;
   tsltmp = new TStringList;

   AnsiString nt,NowTime,PrvTime,  sStoreNo, sStoreName, stmp, strtmp, strQt;
   int i;

   tslCrp->Clear();
   tsltmp->Clear();
   for(i=0; i<109; i++)
       {
         //if(tb_setgrp[i].qt_setgrp==0 && tb_setgrp[i].MustPrinted==0)
         if(tb_setgrp[i].qt_setgrp==0 )     // 2009/10/02
           continue;

         if( tb_setgrp[i].qt_setgrp > 99999 )
            strQt="?????";
         else
            strQt.sprintf("%5ld",tb_setgrp[i].qt_setgrp);

         stmp.sprintf(" %-2s  %-20s %5s %8.0f",
                           tb_setgrp[i].no_setgrp.c_str(),
                           tb_setgrp[i].nm_setgrp.SubString(1,20).Trim().c_str(),
                           strQt.c_str(),
                           tb_setgrp[i].am_setgrp
                            );
         tsltmp->Add(stmp);
       }

   if( tsltmp->Count <= 0 )
     {
      delete tslCrp;
      delete tsltmp;
      return(0);
     }


   nt=strHdata.SubString(21,16);
   //NowTime=nt.SubString(3,2)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   NowTime=nt.SubString(1,4)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   nt=strHdata.SubString(68,16);
   //PrvTime=nt.SubString(3,2)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   PrvTime=nt.SubString(1,4)+"-"+nt.SubString(5,2)+"-"+nt.SubString(7,2)+" "+nt.SubString(9,2)+":"+nt.SubString(11,2);
   read_ten_name(sStoreNo, sStoreName);

   strtmp.sprintf("           �C�ްӫ~�P��έp��"); tslCrp->Add(strtmp);
   //strtmp.sprintf("%s  �e���έp:%s",NowTime,PrvTime ); tslCrp->Add(strtmp);
   strtmp.sprintf("�C�ްӫ~�έp�ɶ�:%s",NowTime); tslCrp->Add(strtmp);
   strtmp.sprintf("�e���C�޲έp�ɶ�:%s",PrvTime ); tslCrp->Add(strtmp);
   strtmp.sprintf("����:%s    ���W:%s  ����:%s",strHdata.SubString(11,6), sStoreName, strHdata.SubString(18,2) );
   tslCrp->Add(strtmp);

   //strtmp.sprintf("%s     ����:%s",NowTime,sTranCnt.SubString(18,2) ); tslCrp->Add(strtmp);

   strtmp.sprintf("���ȭ�:%s\n",strHdata.SubString(56,8));     tslCrp->Add(strtmp);
   strtmp.sprintf("�s�� �C�޸s�W��            �ƶq     ���B");   tslCrp->Add(strtmp);

   //tsltmp->Sort();
   for (int i=0; i<tsltmp->Count; i++)
    {
        stmp = tsltmp->Strings[i];
        tslCrp->Add(stmp);
    }

   strtmp.sprintf("----------------------------------------\n\n\n\n");  tslCrp->Add(strtmp);
   strtmp.sprintf("\x1b\x69");  tslCrp->Add(strtmp); //����

    //��X�ɮ�
   if (FileExists(RptSALFileName.c_str() ))
        DeleteFile(RptSALFileName.c_str());

   tslCrp->SaveToFile(RptSALFileName);
   //tslCrp->Clear();
   i=tsltmp->Count;
   delete tslCrp;
   delete tsltmp;

   stmp.sprintf("WriteSPCReport: �C�ްӫ~�P��έp�� OK. (%s)",RptSALFileName.c_str() );
   writelog(stmp);

   return(i);

}




//�L�I�H��Ū�b   call by _WVXZ
int __fastcall VXZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version)
{

    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false, "", "");               //���JSAL�ɡA�N�ɮר̫��A����

    AnsiString sLog, sTmp;
    sLog.sprintf("�L�I�H��Ū�b VXZDATA::WriteData(%s, %s, %s, %s, %s)",StoreNO,EcrNO,SalesNo,SalFileName, Version);
    writelog( sLog);

    if (!int_reslut)
        return 0;

    AnsiString str_ver_num, str_tran_cnt;

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );
    str_ver_num = get_version(Version);

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z�b���c
    AnsiString str_z_filename;              //Z�b��X�ɦW

    //�ثeZ�b�ɶ�
    AnsiString str_z_date,                  //�{�b�ɶ�
               str_pz_date,                 //�e���ɶ�
               str_pzno_seq,                //�ثeZ�b����   �����k1
               str_pz_cnt,                  //�e���鵲�Ǹ�  ���餣�k�s
               str_nz_cnt,                  //�ثe�鵲�Ǹ�  ���餣�k�s
               str_am_tclsacc,              //�ֿnZ�b���B   ���餣�k�s
               sYYMMDDZZ,                   //Z�b���Ǹ�
               str_vxpzno_seq;              //�L�I�H��Ū�b����


    AnsiString str_date_time, str_datez,str_cd_satncode,str_cs_entry;               //�ثe�b���t�ήɶ�
    str_date_time = gchar_date_time;


    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );


    str_z_date  = str_date_time;                                 //�{�b�ɶ�

    //str_pz_date = tif_ecr->ReadString("VXZ","PZ_DATE","");       //�e���ɶ�
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");       //�e���ɶ�  2013/08/26 Update
    
    str_datez = tif_ecr->ReadString("Z","DATEZ","");           //DDZ
    str_cd_satncode=tif_ecr->ReadString("VXZ","CD_SATNCODE","");
    str_cs_entry=tif_ecr->ReadString("VXZ","CS_ENTRY","");


    sLog.sprintf("VXZDATA:ECR->WriteData �����ثe������Ǹ�:%s, CD_SATNCODE=%s, CS_ENTRY=%s",
                  str_tran_cnt.c_str(), str_cd_satncode.c_str(), str_cs_entry.c_str() );
    writelog( sLog);



    //�N�b�����Ƴƥ��w�Q�N�ӧP�_���ؿ����� james - 2003/02/07
    //tif_ecr->WriteString("Backup", "pz_date", str_pz_date);
    //�ƥ� sal8.TXT����
    //tif_ecr->WriteString("Backup", "datez", str_datez);

    /**********************

    019915 TM1 5/27 ��TM130524R1����--�H��ñ��y���鵲�Ǹ����~�C

    �鵲�᦬�ȭ��n�J�鵲�Ǹ�00006�C
    �L�I�H���n�J��鵲�Ǹ��ܦ�00001�A
    �t�~�ټv�T����@�������鵲�Ǹ��]�O00001�H


    if ((str_pz_date == "") || (str_pz_date.Length() < 8))  //�L������ơA����O���k�s
    {
        tif_ecr->WriteString("Z", "pz_date", str_z_date);
        tif_ecr->WriteString("Z", "C", "0");
        tif_ecr->WriteString("Z", "nzno_seq", "1");
        tif_ecr->WriteString("Z", "pz_cnt", "00000");
        tif_ecr->WriteString("Z", "nz_cnt", "00001");
        tif_ecr->WriteString("Z", "datez", (str_date_time.SubString(7,2) + "1"));
        //20031225
        tif_ecr->WriteString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        tif_ecr->WriteString("Z", "zfilename", "");
        tif_ecr->WriteString("Z", "zrptname", "");
        tif_ecr->WriteString("Z", "deptname", "");
        tif_ecr->WriteString("Z", "inqtname", "");

        tif_ecr->WriteString("VXZ", "PZNO_SEQ", "0");
        tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");
        tif_ecr->WriteString("VXZ", "PZ_DATE", "");

        //tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        //tif_ecr->WriteString("VXZ","CS_ENTRY","");


        AnsiString str_xpath;               //���w���ȭ��b���ؿ�
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date;
    }
    ****************************/

    str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b���� 01~09  �����k1
    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s 00001~99999
    str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //�ֿnZ�b���B   ���餣�k�s
    str_vxpzno_seq = tif_ecr->ReadString("VXZ", "NZNO_SEQ", "1");


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.Length() < 5)
        str_nz_cnt = "00001";

    //sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_vxpzno_seq));
    sYYMMDDZZ.sprintf("%02d",_StrToInt(str_vxpzno_seq));
    
#ifdef TM_20

    str_z_filename.sprintf("%s%02d%s%02d%02d.vdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        (str_pz_date.SubString(3,6)).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        _StrToInt(str_vxpzno_seq)              );
#else
    //��X�ɮ� yyyymmddnn.zdt ex:2002010101.vdt
    //str_z_filename = (str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt");

    str_z_filename.sprintf("%s%02d%s%02d%02d.vdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        (str_pz_date.SubString(3,6)).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        _StrToInt(str_vxpzno_seq)              );



#endif

    //����Flag
    bool bl_change_day;

    //�P�_����
    if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
        bl_change_day = true;
    else
        bl_change_day = false;

    z->no_tm = gchar_ecr_no;             //���Ⱦ�����

    //�鵲�Ǹ�
    z->bg_noclose.sprintf("%05d", _StrToInt(str_nz_cnt,"�鵲�Ǹ�"));


    z->bg_dtclose = str_z_date.SubString(1,8);      //���b���
    z->bg_tmclose = str_z_date.SubString(9,6);      //���b�ɶ�
    z->ed_dtclose = str_pz_date.SubString(1,8);     //�e�����b���
    z->ed_tmclose = str_pz_date.SubString(9,6);     //�e�����b�ɶ�


    AnsiString z_a;
    int iC0TotCount=0;
    z_a.sprintf("1092|%04d|%-6s|%-2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|", 1593,
                                      gchar_tencode,
                                      z->no_tm.c_str(),           //���Ⱦ�����
                                      str_date_time.c_str(),      //�{�b�ɶ�
                                      z->bg_noclose.c_str(),      //�鵲�Ǹ�
                                      sYYMMDDZZ.c_str(),          //Z�b���Ǹ�
                                      z->ed_dtclose.c_str(),      //�e�����b���
                                      z->ed_tmclose.c_str(),      //�e�����b�ɶ�
                                      z->bg_dtclose.c_str(),      //���b���
                                      z->bg_tmclose.c_str()      //���b�ɶ�
                                    );


    // 03 ������Ӣ�������  =============================================

    //����ӫȼ�
    z->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~��
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    z->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt;

    // �I�d����O���K�|
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);
    z->tb_trans.am_trcpntx.sprintf("%08d",giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );       // �o���鵲�b�B(�K)
    z->tb_trans.am_trcptax.sprintf("%08d",giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));       // �o���鵲�b�B(�|)
    z->tb_trans.am_trcptot.sprintf("%08d",giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));       // �o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    //���鵲�b�֭p���B
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) > 99999999 ||(int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p"));
          //int_acc_tot_amt=_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p");


    z->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")));

    TotMiss(0,gi_tot_line);
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
    z->tb_trans.qt_miss.sprintf("%08d",   giA);       //  �~���o������
    z->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt - R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  �~���o�����B(�K)
    z->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  �~���o�����B(�|)
    z->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );       //  �~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //���}�o�����B, ���}�o�����B�~�h��
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+ PreSal10_AmtTX + PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    //�����ھP�f���J
    z->tb_trans.am_totsale.sprintf("%08d", int_totsale );


    if (gbl_tot_haveline)
    {
        z->no_rcstart = invoice_start_end(gi_tot_line, true);    //�o���}�l���X
        z->no_rcend = invoice_start_end(gi_tot_line, false);     //�o���������X
    }
    else    //�L�o�����
    {
        z->no_rcstart = "0000000000";                            //�o���}�l���X
        z->no_rcend   = "0000000000";                            //�o���������X
    }


    AnsiString z_b;

    z_b += (z->tb_trans.qt_tcust+"|");             //����ӫȼ�
    z_b += (z->tb_trans.qt_trans+"|");             //�������~��
    z_b += (z->tb_trans.am_tclsacc+"|");           //���鵲�b�֭p���B
    z_b += (z->no_rcstart+"|");                   //�o���}�l���X
    z_b += (z->no_rcend+"|");                      //�o���������X
    z_b += (z->tb_trans.am_trcpntx+"|");          //�o���鵲�b�B(�K)
    z_b += (z->tb_trans.am_trcptax+"|");           //�o���鵲�b�B(�|)
    z_b += (z->tb_trans.am_trcptot+"|");          //�o���鵲�b���B
    z_b += (z->tb_trans.qt_miss+"|");              //�~���o������
    z_b += (z->tb_trans.am_missntx+"|");           //�~���o�����B(�K)
    z_b += (z->tb_trans.am_misstax+"|");           //�~���o�����B(�|)
    z_b += (z->tb_trans.am_miss+"|");              //�~���o�����B
    z_b += (z->tb_trans.am_totsale+"|");           //�����ھP�f���J


    /* 03 �N��I��������       *///===========================================

    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                 //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;             //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);     //giH giI ���w�h�f���B�B����    //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    z->tb_newadd.qt_agnrcv.sprintf("%08d",giD);             //�N���O�Φ���
    z->tb_newadd.am_agnrcv.sprintf("%08d",giC);             //�N���O�Ϊ��B


    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    AnsiString z_c;
    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //�N��X�p����
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //�N��X�p���B   �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;
    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

    z_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //�N�{����������=============================================
    int  iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);   //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    writelog("giS:���O����B , giT:���O��^��");

    z->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);            //§��^��
    z->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //§����B


    SumPresent41+=giB; //§��+�ӫ~§��
    SumPresent41+=iCSMAmt; //���O��   for  ���ȭ��汵�Z���Ӫ�

    z->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //�t�ӥN�{���^�� �ثe���ϥ�
    z->tb_cash.am_vndcpn.sprintf("%08d", 0);             //�t�ӥN�{�����B

    z->tb_cash.qt_stocpn.sprintf("%08d", giC);           //�����N�{���^��
    z->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //�����N�{�����B
    SumCpn41 +=  giD;         //�����+Other1+�������� CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(�D�������B:giU), �������B:giV)     25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    SumCpn41 -=  giU;

    z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //�N�{���X�p�^��
    z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //�N�{���X�p���B

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //�N�{���X�p�^��
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //�N�{���X�p���B

    z->tb_cash.qt_bonout.sprintf("%08d", giG);           //�����X�p�^��
    z->tb_cash.am_bonout.sprintf("%08d", giH);           //�����X�p���B

    iTotOt2Rtn=giE;                                      //�l�B��
    iqt_ffu1=giI;                                        //�{���d�^��  �q�l�� 1847
    iam_ffu1=giJ;                                        //�{���d���B  �q�l�� 1855    ��d�X�p

    AnsiString z_d;

    z_d += (z->tb_cash.qt_bonus+"|");                //§��^��
    z_d += (z->tb_cash.am_bonus+"|");                //§����B
    z_d += (z->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��
    z_d += (z->tb_cash.am_vndcpn+"|");               //�t�ӧ������B
    z_d += (z->tb_cash.qt_stocpn+"|");               //�����^��
    z_d += (z->tb_cash.am_stocpn+"|");               //�������B
    z_d += (z->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��
    z_d += (z->tb_cash.am_totcash+"|");              //�N�{���X�p���B

    sTmp.sprintf("%08d",giF);
    z_d += (sTmp+"|");                               //�l�B�X�p�^��
    sTmp.sprintf("%08d",giE);
    z_d += (sTmp+"|");                               //�l�B�X�p���B

    z_d += (z->tb_cash.qt_bonout+"|");               //�����X�p�^��
    z_d += (z->tb_cash.am_bonout+"|");               //�����X�p���B

    //==========================================================================


    //�{���X�p �t ���O��

    SumCash41=SumCash(0,gi_tot_line);
    z->am_cash.sprintf("%08d",SumCash41-iTotOt2Rtn+iCSMAmt );

    // writelog("�{���X�p �t ���O��");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //�{���X�p
    //strcat(ze,z->no_rcstart.c_str());      //�o���}�l���X
    //strcat(ze,z->no_rcend.c_str());        //�o���������X
    //ze[28] = 0;


    // Z�b�����O���pTABLE      =============================================

    AnsiString z_f, z_a1093, z_ammdis;

    //s1�����X�p�������
    //s2�����X�p�P����B
    //s3�����X�p�~�h����
    //s4�����X�p�~�h���B
    int s1, s2, s3, s4;

    s1 = s2 = s3 = s4 = 0;
    DEPT *dept;
    try
    {
        dept = new DEPT[20];
        //���N�ȶ�s
        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpsale.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmiss.sprintf("%08d",0);

            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;

            dept[i].am_mmDis = 0;         // �����P�P�馩���B        Lu 2018/06/13
            dept[i].am_mmDismiss = 0;     // �����P�P�馩�~�h���B    Lu 2018/06/13
         }

        //writelog("INIT ����������, �����b");
        //�p��Ҧ�����������, �����b


          if (gbl_rec_haveline)
            SumAllDept(dept);


        // writelog("�p��Ҧ�����������, �����b OK");


       // �����P�P�馩���B        Lu 2018/06/13
        z_a1093.sprintf("1093|%04d|%-6s|%-2s|%14s|%5s|%-10s|", 275,
                                      gchar_tencode,
                                      z->no_tm.c_str(),           //���Ⱦ�����
                                      str_date_time.c_str(),      //�{�b�ɶ�
                                      z->bg_noclose.c_str(),      //�鵲�Ǹ�
                                      sYYMMDDZZ.c_str()          //Z�b���Ǹ�
                                      );
        z_ammdis="";

        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            z->tb_depcal[i].am_dpsale.sprintf("%08d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            z->tb_depcal[i].am_dpmitm.sprintf("%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            z->tb_depcal[i].am_dpmiss.sprintf("%08d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;

            // �����P�P�馩���B        Lu 2018/06/13
            sTmp.sprintf("%08d", dept[i].am_mmDis - dept[i].am_mmDismiss);
            z_a1093 += (sTmp+"|");

            sTmp.sprintf("%08d|%08d", dept[i].am_mmDis , dept[i].am_mmDismiss);
            z_ammdis += (sTmp+"|");

        }

        // �����P�P�馩���B        Lu 2018/06/13
        sTmp.sprintf("%40s|\n", " ");
        z_a1093 += (sTmp);
        z_ammdis += (sTmp);
    }
    __finally
    {
      delete [] dept;
    }

    //writelog("Z�b�����O���pTABLE OK");

    AnsiString z_g;

    z_g += (z->am_tdpsitm.sprintf("%08d", s1)+"|");             // �����X�p�������
    z_g += (z->am_dpstot.sprintf("%08d" , s2)+"|");             // �����X�p�P����B
    z_g += (z->am_tdpmitm.sprintf("%08d", s3)+"|");             // �����X�p�~�h����
    z_g += (z->am_dpmtot.sprintf("%08d" , s4)+"|");             // �����X�p�~�h���B
    z_g += (z->am_cash+"|");                                   // �{���X�p�t���O�� 45


    //���ȭ����Ӣ�������=============================================

    AnsiString z_h;

    //z_h += (z->am_cash+"|");                                    //�{���X�p
    //z_h += (z->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");    // �ɹs����
    sTmp.sprintf("%08d",VisuilCpnAmt);
    z_h += (sTmp+"|");                                            //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // �ɹs�X�p���B
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    z_h += (sTmp+"|");                                            // �N���ӫ~������馩  �ɹs�X�p���B

    z_h += (z->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");    // ��s����
    z_h += (z->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");    // ��s�X�p���B

    //��w
    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;           //��w����
    int int_drop_money = 0;       //��w���B
    int int_drop_sum = 0;    //��w�`�B
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        //StringSegment_EX(s, "|", 15)
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("���ͤ鵲�b��:�D��w(%s)���",s1041);
             writelog(sTmp );
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //�{��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC�I��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //�H�Υd
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //§��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //�����
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //���f��

        //�����o�����B  800   Cloud �����o�����B(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s1041, "|", 18) );

        //�����o�����B  500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //�����o�����B  1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //�����o�����B  200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //�O�� 01���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //�O�� 02���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //�O�� 03���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //�O�� 04���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //�O�� 05���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //�O�� 06���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //�O�� 07���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //�O�� 08���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //�O�� 09���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //�O�� 10���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //�O�� 11���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //�O�� 12���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //�O�� 13���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //�O�� 14���B

        int_drop_sum += int_drop_money;          //��w�`�B
        int_drop_money = 0;
        ++int_drops;                             //��w����
    }

    z_h += (z->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        // ��w����
    z_h += (z->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     // ��w�X�p���B

    //writelog("��w�X�p���B OK");

    //�󥿦��ơB���B
    SumUpdat(0,gi_rec_line);
    z_h += (z->tb_casher.qt_update.sprintf("%08d", giA)+"|");          // �󥿦���
    z_h += (z->tb_casher.am_update.sprintf("%08d", giB)+"|");          // �󥿦X�p���B


    //�������
    SumVoid(0,gi_tot_line);
    z_h += (z->tb_casher.qt_void.sprintf("%08d", giA)+"|");            // �����������
    z_h += (z->tb_casher.am_void.sprintf("%08d", giB)+"|");            // ��������X�p���B


    //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
    }
    __finally
    {
        //delete ini_ecr;
        ;;
    }


    z_h += (z->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // �V�m�X�p����
    z_h += (z->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // �V�m�X�p���B

    //��ʿ�J����13�B14
    SumInpdp(0,gi_rec_line);
    z_h += (z->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");            //��ʿ�J��������
    z_h += (z->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");            //��ʿ�J�������B

    z_h += (z->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");     // �}���d����

    //�馩������   //�p�p�馩
    SumDisc(0,gi_pay_line);
    z_h += (z->tb_casher.qt_disc.sprintf("%08d", giA)+"|");             // �馩����(���|)
    z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");            // �馩���B(���|)

    pay_sub_amt(0,gi_pay_line);
    z_h += (z->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");            //�P�P�馩/������
    z_h += (z->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");             //�P�P�馩/�����B

    //�[�Ⱦ�
    easy_card(0,gi_easy_line);
    z_h += (z->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");          // �[�Ⱦ��}/��d����
    z_h += (z->tb_casher.am_disret1.sprintf("%08d", giB)+"|");         // �[�Ⱦ��}/��d���B
    z_h += (z->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");          // �[�Ⱦ��[�Ȧ���
    z_h += (z->tb_casher.am_disret2.sprintf("%08d", giD)+"|");          // �[�Ⱦ��[�Ȫ��B

    //fetc_card(0,gi_fetc_line);   //e�q�d    2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //�ƥΤK����  �{�� �[�Ȧ���   2015/08/19 �N��,����O�P����B(�K)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //�ƥΤK���B  �{�� �[�Ȫ��B   2015/08/19 �N��,����O�@�o���B(�K)
    z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���   2015/08/19 �N��,����O�P����B(�K)
    z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|"); //�ƥΤK���B  �{�� �[�Ȫ��B   2015/08/19 �N��,����O�@�o���B(�K)


    //���w���ơB���B  int iqt_food,iamt_food;
    z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���   2015/08/19 0
    //z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B   2015/08/19 0


    //   05 �l�[ table        =============================================

    AnsiString z_i;

    z_i += (z->tb_newadd.qt_cashCard.sprintf("%08d", iqt_ffu1)+"|");    //��d/�{���d����    �q�l�� 1847
    z_i += (z->tb_newadd.am_cashCard.sprintf("%08d", iam_ffu1)+"|");    //��d/�{���d���B    �q�l��  ��d�X�p 1855
    z_i += (z->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");            //�N�������O����
    z_i += (z->tb_newadd.am_parking.sprintf("%08d", 0)+"|");            //�N�������O���B
    z_i += (z->tb_newadd.qt_agnrcv+"|");                                //�N���O�Φ���  1831
    z_i += (z->tb_newadd.am_agnrcv+"|");                                //�N���O�Ϊ��B  1839
    z_i += (z->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");      //�����e�m�Ÿ�
    z_i += (z->tb_newadd.tx_ver.sprintf("%6s", str_ver_num)+"|");      //�������X
    z_i += (z->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  //�o�����B0����

    //���}�o�����B
    //SumNOVoiceAmt(0,gi_tot_line);

    z_i += (z->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //���}�o�����B
    z_i += (z->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");    //  ���}�o�����B�~�h��

    SumLOSS(0,gi_rec_line);
    z_i += (z->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");             // TM�o��Loss�Ȧ���
    z_i += (z->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");             // TM�o��Loss�Ȫ��B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // ���d���X�p
    z_i += (z->tb_newadd.am_ffu7.sprintf("%08d", iSub_C0InvoAmtA)+"|");             //��[�Ⱦ��[�Ȩ������B

    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��
    int iSumRelDimAgn=giB;

    // z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", giB+iSub_C0InvoAmtC)+"|");             //��[�Ⱦ��[�Ȩ�������
    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");             //��[�Ⱦ��[�Ȩ�������

    //���O��
    z_i += (z->tb_newadd.am_posZRpt.sprintf("%08d", iCSMAmt)+"|");      //���d�����d���B   ���O��

    AnsiString z_k;
    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);      �Y���ʻP�P��i�����|
    z_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //�Y���ʻP�P��i�ȧK�|
    z_k +=sTmp;

    //�o���I�����B
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //�o���I���i��  �����o���i��
    z_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B  �����o�����B
    z_k +=sTmp;


    //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    z_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�I�d����OgiD.�@�ogiE , �I�d����O�ȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);  //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)
    z_k +=sTmp;

    //z_h += z->tb_casher.qt_pack.sprintf("%08d", iTotOt2Rtn); //giE);     // �[�Ⱦ����b����  2007/12/01 ��l�B��

    //z_i += z->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix);          //�N���ӫȼ�

    //z_i += z->tb_newadd.qt_posZRpt.sprintf("%08d", 0);            //���d�����d����

    //z_i += z->tb_newadd.qt_ffu10.sprintf("%08d",0); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b���� �O�d
    //z_i += z->tb_newadd.am_ffu10.sprintf("%08d",0); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B �O�d




    //if (z_a.Length()!=34)
    //{
    //    writelog( ("z_a �������� 34 �ثe���� " + IntToStr(z_a.Length())) );
    //    z_a = z_a.SubString(1,34);
    //}

    //if (z_b.Length()!=88)
    //{
    //    writelog( ("z_b �������� 88 �ثe���� " + IntToStr(z_b.Length())) );
    //    z_b = z_b.SubString(1,88);
    //}

    //if (z_c.Length()!=720)
    //{
    //    writelog( ("z_c �������� 720 �ثe���� " + IntToStr(z_c.Length())) );
    //    z_c = z_c.SubString(1,720);
    //}

    //if (z_d.Length()!=80)
    //{
    //    writelog( ("z_d �������� 80 �ثe���� " + IntToStr(z_d.Length())) );
    //    z_d = z_d.SubString(1,80);
   // }

   // if (strlen(ze)!=28)
   // {
   //     writelog( ("z_e �������� 29 �ثe���� " + IntToStr(strlen(ze))) );
   //     ze[28] = 0;
   // }

   // if (z_f.Length()!=640)
   // {
   //     writelog( ("z_f �������� 640 �ثe���� " + IntToStr(z_f.Length())) );
   //     z_f = z_f.SubString(1,640);
   // }

   // if (z_g.Length()!=32)
   // {
  //      writelog( ("z_g �������� 32 �ثe���� " + IntToStr(z_g.Length())) );
  //      z_g = z_g.SubString(1,32);
  //  }

  //  if (z_h.Length()!=168)
  //  {
  //      writelog( ("z_h �������� 168 �ثe���� " + IntToStr(z_h.Length())) );
  //      z_h = z_h.SubString(1,168);
  //  }

  //  if (z_i.Length()!=208)
  //  {
  //      writelog( ("z_i �������� 208 �ثe���� " + IntToStr(z_i.Length())) );
  //      z_i = z_i.SubString(1,208);
  //  }


    AnsiString DDZ_1099;
    DDZ_1099=str_datez;
    // 2013/08/26  Update
    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ; //DDZ_1099=str_datez;

    //�O��Z PATH��INI
    AnsiString str_zdt_path, str_zrp_path;


    //Z����
    AnsiString str_zrp;

    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%8s%88s|\n",
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%79s|\n",  2012/09/18
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17    �I�d����O Update
    str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str(),
                            "FFFFFFFF"
                            );

    sTmp.printf("VXZDATA::(%s) Len=%d",str_zrp, str_zrp.Length() );
    writelog(sTmp);
    //report = new REPORT();      //�鵲�b��

    //Z����
    strcpy(report->char_filename, ChangeFileExt(str_z_filename,".vrp").c_str());

    if( _StrToInt(str_cs_entry)==1)
        report->CreateReport('V', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
   else
        report->CreateReport('v', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
    //�ƥ�Z����
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //�����b��
    AnsiString str_dpt_path;
    strcpy(report->char_filename, ChangeFileExt(str_z_filename,".vpt").c_str() );

    if( _StrToInt(str_cs_entry)==1)
         report->CreateReport('E', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,0,0,0,0,tsl_subsale, 0, z_ammdis);
    else
         report->CreateReport('e', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,0,0,0,0,tsl_subsale, 0, z_ammdis);
    //�ƥ������b��
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    //�g�JSAL��98Type
    AnsiString str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_p_z_cnt,
               str_n_z_cnt,
               str_start_trans,            //�}�l����Ǹ�
               str_am_tclascc,
               str_am_totsale,
               str_zdt_no,
               str_ver,
               str_sal_last_1099,
               SaleDataVer;

    str_rec_type = "1015";
    str_rec_len = "0098";
    str_tencode = gchar_tencode;
    str_ecr_no.sprintf("%02d",_StrToInt(z->no_tm,"z->no_tm"));
    str_sys_date = (z->bg_dtclose + z->bg_tmclose);      //����ɶ�

    //98����Ǹ� //20031215
    int int_sum_tran_cnt ;

    //= tif_ecr->ReadInteger("ECR","TRAN_CNT",1);
    //str_tran_cnt.sprintf("%05d",int_sum_tran_cnt);

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    //if (str_tran_cnt.Length() < 5)
    //if( Trim(str_tran_cnt)=="00000")
    //if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
    //    str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //����Ǹ�

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    //sLog.sprintf("VXZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    //writelog( sLog);


    AnsiString str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
         str_sum_tran.sprintf("%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //�e���鵲�Ǹ��A�ثe�鵲�Ǹ�
    //str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "�e���鵲�Ǹ�"));
    //str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "�ثe�鵲�Ǹ�"));


    if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc;

    AnsiString str_sal_first;
    str_sal_first = gpchar_sal_first;

     //�}�l����Ǹ�
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, "|", 7).SubString(TRANCNT_NO_POS,5));
    str_start_trans.sprintf("%05d", i_tran_cnt);

    int_am_tclsacc=int_acc_tot_amt;
    str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //���鵲�b�֭p���B
    str_am_totsale = z->tb_trans.am_totsale;                //�����ھP�f���J
    str_zdt_no.sprintf("%010ld",z->bg_noclose.ToInt());     //ZDT�Ǹ��N�JZ�ɦW
    str_ver = "FFFFFFFF|";


    AnsiString str_trans1_5;    //����Ǹ��e���X

    AnsiString str_sal_last;
    str_sal_last = gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    AnsiString str_01,str_99;

    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",   //1097
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   SalesNo.c_str(),                       //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   " ",
                   " ",
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );



    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
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



    AnsiString _str_tran_cnt;        //����Ǹ�
    AnsiString str_st_tran_cnt;     //�}�l����Ǹ�

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    AnsiString str_98;  //�g�J1015����Ǹ�
    str_98.sprintf("%4s|%4s|%6s|%2s|%14s|%-5s|%-10s|%3s|%2s|%-8s|%1s|%-16s|%9s\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        "000",
                        "01",
                        SalesNo.c_str(),      //���ȭ��N�X
                        str_cs_entry.c_str(),            //1,2
                        str_cd_satncode.c_str(),
                        "        |"
                        );



    String str_sal;
    //str_sal = str_01 + "\n"+ str_zrp + str_98 + str_99;
    str_sal = str_01 + "\n"+ str_98+str_zrp + z_a1093 + str_99;    // z_a1093 �����P�P�馩���B        Lu 2018/06/13

    if (WriteSAL(str_sal, str_z_filename) == OK)
       {
        //�ƥ�ZDT�b
        str_zdt_path = BackupFile(str_z_filename,"", DDZ_1099);
        tif_tmp->WriteString("VXZ","x1092filename", str_zdt_path);
       }
    else
      {
       tif_tmp->WriteString("VXZ","x1092filename", "");
      }

    /////////////////////////////////////////////////////////////////

    AnsiString StrZslFile;
    if (WriteSAL_A(str_sal, SalFileName) == OK)
       {
         //�ƥ�ZSL�b
         StrZslFile = BackupFile(SalFileName, ChangeFileExt(SalFileName,".zzz"),DDZ_1099 ); // for test
         //tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);
         ;;
       }
    else
       {
         //tif_tmp->WriteString("Z","ZSLNAME","");
         ;;
       }


    AnsiString str_ipt_path;

    //���XZDATA.INI
    int int_vxnzno_seq   = _StrToInt(str_vxpzno_seq, "vxpzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp->WriteString("VXZ","ZFILENAME" , str_zdt_path);
    tif_ecr->WriteString("VXZ","ZFILENAME" , str_zdt_path);

    tif_tmp->WriteString("VXZ","ZRPTNAME"  , str_zrp_path);
    tif_ecr->WriteString("VXZ","ZRPTNAME"  , str_zrp_path);

    tif_tmp->WriteString("VXZ","DEPTNAME"  , str_dpt_path);
    tif_ecr->WriteString("VXZ","DEPTNAME"  , str_dpt_path);

    tif_tmp->WriteString("VXZ","PZ_DATE"     , str_z_date);
    tif_ecr->WriteString("VXZ","PZ_DATE"     , str_z_date);

    //tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

    //tif_tmp->WriteString("VXZ","PZ_DATE"     , str_z_date);
    //tif_tmp->WriteInteger("Z","AM_TCLSACC" , int_am_tclsacc);
    //tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT ����Ǹ�

    //tif_tmp->WriteString("ECR","tot_open_z", "0");
    //tif_ecr->WriteString("ECR","tot_open_z", "0");
    /*
    if (bl_change_day) //�P�_�O�_����
    {
        tif_ecr->WriteString("Z","PZNO_SEQ", "0");
        tif_ecr->WriteString("Z","NZNO_SEQ", "1");
        tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);
        if (++int_nz_cnt > 99999)
            int_nz_cnt = 1;

        str_nz_cnt.sprintf("%05d",int_nz_cnt);
        tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));
    }
    else
    {
    */
        tif_ecr->WriteString("VXZ","PZNO_SEQ", str_vxpzno_seq);

        if (++int_vxnzno_seq > 99)
            int_vxnzno_seq = 1;

        str_vxpzno_seq.sprintf("%d", int_vxnzno_seq);

        tif_ecr->WriteString("VXZ","NZNO_SEQ", str_vxpzno_seq);
        //tif_ecr->WriteString("Z","PZ_CNT", str_nz_cnt);

        //if (++int_nz_cnt > 99999)
        //    int_nz_cnt = 1;

        //str_nz_cnt.sprintf("%05d", int_nz_cnt);

        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);
        //tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
        //                      + FormatFloat("00",int_nzno_seq) + "00"));
        //tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));
    //}



    str_pz_date    = str_z_date;
    //str_pzno_seq   = tif_ecr->ReadString("Z","NZNO_SEQ","");      //�ثeZ�b����   �����k1

    // tif_ecr->WriteString("VXZ", "nzno_seq", "1");


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    //���wX �bPATH
    //AnsiString str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    //tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //�e���~��/�d�ȭ��L

    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_tran_cnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);
    tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    sLog.sprintf("VXZDATA::�g�J����Ǹ� ECR->TRAN_CNT=%s", str_sum_tran );
    writelog( sLog);



    return OK;
}



VXZDATA::VXZDATA()
{
    //
    writelog("�L�I�H��Ū�b �}�l");
    z = new SPOS_ZRPDT;         //Z�b���c
    tif_tmp = new TIniFile(ZDATATMP_INI);
    tif_ecr = new TIniFile(XZDATA_INI);
    report = new REPORT();      //�鵲�b��

}

VXZDATA::~VXZDATA()
{
    delete z, tif_ecr;

    delete report;      //�鵲�B�����b��
    delete tif_tmp;
    //mtn_dat_folder();   //���@DAT�ؿ��A�O�s�T�Q��H�����
 
}


///////////////////////////////////////////////////////////////////////////////
//�۰ʤ鵲Section


AnsiString  __fastcall AUTOZDATA::GetSysDttmMaxTransNo(AnsiString sDttm)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    AnsiString sTmp, sRec, sTmpDttm, RecNo, tmpTrans, rtns;

 try
   {
      rtns="";
      if( Auto_Sal->Count > 0)
       {
        for (int i=0;  i<Auto_Sal->Count; i++)
            {
             sRec=Auto_Sal->Strings[i];
             RecNo=_StringSegment_EX(sRec, "|", 1);
             if( RecNo!="1001")
                 continue;
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             tmpTrans=_StringSegment_EX(sRec, "|", 7);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( sDttm != "" && sTmpDttm.SubString(1,8) == sDttm )
                  {
                    rtns=tmpTrans;
                    break;
                  }
            }
        }

   }
 catch(...)
   {
     writelog("AUTOZDATA:: GetSysDttmMaxTransNo Auto_Sal Catch error...");
   }
return(rtns);

}




int __fastcall AUTOZDATA::GetYYYYMDD_SalData(AnsiString &AutoZCnt, AnsiString AutoZdt , int AutoZ)
{
    //Auto_Sal->LoadFromFile(SalFileName);
    AnsiString sTmp, sRec, sTmpDttm,TmpZcnt, SalZcnt, RecNo;
    int irtn=0;
    TStringList *sTmlLst = new TStringList();
    SalZcnt=AutoZCnt;

 try
   {
    if( AutoZ ==1)
      {
       sTmlLst->Assign(Auto_Sal);
       if( sTmlLst->Count > 0)
       {
        Auto_Sal->Clear();
        for (int i=0;  i<sTmlLst->Count; i++)
            {
             sRec=sTmlLst->Strings[i];
             RecNo=_StringSegment_EX(sRec, "|", 1);
             if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
               TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //�鵲�Ǹ�
             else
               TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�

             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt )
                  {
                    if( RecNo=="1001" )   //���o 1001 ���̤j�鵲�Ǹ�
                      {
                        SalZcnt=TmpZcnt;
                        AutoZCnt=SalZcnt;
                      }
                  }
             // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt )
               {
                 Auto_Sal->Add(sRec);
               }
             else
               {
                   ;;
               }
            }
        }

      } // end of  if( AutoZ ==1)
   }
 catch(...)
   {
     writelog("AUTOZDATA::GetYYYYMDD_SalData Auto_Sal Catch error...");
   }

  delete  sTmlLst;

  //////////////////////////////

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

             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                    //AutoZCnt=SalZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt )
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
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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
             TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }
              // 2013/10/24  Update  0013864: [TM�۰ʤ鵲] ���e�@��鵲�Ǹ�=99999,�����L�k����۰ʤ鵲..�� Release A/N
             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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

       //////////////////////////////////////////////////////////////////
       gtsl_Ticket->Clear();

      if( Autotsl_Ticket->Count > 0)
       {
            //  if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
            //   TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //�鵲�Ǹ�
        for (int i=0;  i<Autotsl_subsale->Count; i++)
            {
             sRec=Autotsl_Ticket->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, "|", 7);  //�鵲�Ǹ�
             sTmpDttm=_StringSegment_EX(sRec, "|", 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if(  AutoZdt != "" && sTmpDttm.SubString(1,8) <= AutoZdt)
                  {
                    SalZcnt=TmpZcnt;
                  }

             if( TmpZcnt<=SalZcnt && sTmpDttm.SubString(1,8) <= AutoZdt)
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



    } // end of try
  catch(...)
    {
      writelog("AUTOZDATA::GetYYYYMDD_SalData Catch error...");
      irtn=0;
    }

    return(irtn);

}




int __fastcall AUTOZDATA::GetZDTData(String StrZCnt, String StoreNO,  String EcrNO, String SalesNo ,
                          String Version, String PZ_date, String Z_date, String DDZ_1099, String ZDT_1050,
                          String &RtnZDTData, AnsiString &Tclsacc,
                          String &str_zrp_path, String &str_dpt_path)
{
    int int_reslut = 0;
    AnsiString sLog, sTmp,str_ver_num, str_tran_cnt, ZDT_1050_DDZ;

    AnsiString str_z_date,                  //�{�b�ɶ�
               str_pz_date,                 //�e���ɶ�
               str_pzno_seq,                //�ثeZ�b����   �����k1
               str_pz_cnt,                  //�e���鵲�Ǹ�  ���餣�k�s
               str_nz_cnt,                  //�ثe�鵲�Ǹ�  ���餣�k�s
               str_am_tclsacc,              //�ֿnZ�b���B   ���餣�k�s
               sYYMMDDZZ,                   //Z�b���Ǹ�
               str_date_time, str_datez,    //�ثe�b���t�ήɶ�
               z_a;

    sTmp=_StringSegment_EX(ZDT_1050, "|", 7);  //����Ǹ�
    ZDT_1050_DDZ=sTmp.SubString(3,3);
    str_am_tclsacc=Tclsacc;
    str_z_date=Z_date;
    str_pz_date=PZ_date;
    str_ver_num=Version ;
    //sYYMMDDZZ.sprintf("%s%s", str_pz_date.SubString(3,6).c_str(),StrZCnt );
    sYYMMDDZZ.sprintf("%s%s", str_z_date.SubString(5,4).c_str(),StrZCnt );


    sTmp.printf("** AUTOZDATA::GetZDTData ���ͤ鵲�b�� Version=%s, ZDT:StrZCnt=%s, PZ_date=%s, Z_date=%s, DDZ_1099=%s, Tclsacc=%s **",
                Version, StrZCnt, PZ_date, Z_date, DDZ_1099, Tclsacc  );
    writelog(sTmp );

    int i,j;
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



    z->no_tm = gchar_ecr_no;             //���Ⱦ�����

    //�鵲�Ǹ�
    z->bg_noclose.sprintf("%05d", _StrToInt(StrZCnt,"�鵲�Ǹ�"));


    AnsiString ssysdttm, s1050sysdttm;
    int iC0TotCount=0;

    ssysdttm=gchar_date_time;
    s1050sysdttm=_StringSegment_EX(ZDT_1050, "|", 5);

    if( s1050sysdttm.SubString(1,8) < ssysdttm.SubString(1,8) )
      {
        z->bg_dtclose = s1050sysdttm.SubString(1,8);      //���b���
        z->bg_tmclose = "235959";                         //���b�ɶ�
      }
    else
      {
        //z->bg_dtclose = str_z_date.SubString(1,8);      //���b���   �����ثe�̤j1050�����
        //z->bg_tmclose = str_z_date.SubString(9,6);      //���b�ɶ�

        z->bg_dtclose = s1050sysdttm.SubString(1,8);      //���b���  1090 ���
        z->bg_tmclose = "235959";                         //���b�ɶ�

      }

    sTmp.printf("AUTOZDATA::GetZDTData: s1050sysdttm=%s, ssysdttm=%s, Set 1096 �鵲�Ǹ�=%s, ���b��� z->bg_dtclose=%s, z->bg_tmclose=%s ",
                s1050sysdttm.c_str(), ssysdttm.c_str(), z->bg_noclose.c_str(), z->bg_dtclose.c_str(), z->bg_tmclose.c_str()  );
    writelog(sTmp );

    z->ed_dtclose = str_pz_date.SubString(1,8);     //�e�����b���
    z->ed_tmclose = str_pz_date.SubString(9,6);     //�e�����b�ɶ�


    z_a.sprintf("1096|%04d|%-6s|%-2s|%14s|%5s|%-10s|%8s|%6s|%8s|%6s|",
                                      1593,
                                      gchar_tencode,
                                      z->no_tm.c_str(),           //���Ⱦ�����
                                      Z_date.c_str(),             //�{�b�ɶ�
                                      z->bg_noclose.c_str(),      //�鵲�Ǹ�
                                      sYYMMDDZZ.c_str(),          //Z�b���Ǹ�
                                      z->ed_dtclose.c_str(),      //�e�����b���
                                      z->ed_tmclose.c_str(),      //�e�����b�ɶ�
                                      z->bg_dtclose.c_str(),      //���b���
                                      z->bg_tmclose.c_str()      //���b�ɶ�
                                    );

    // ZDATA::GetZDTData 03 ������Ӣ�������  =============================================

    //����ӫȼ�
    z->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~��
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    z->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    int int_acc_tot_amt, C01TaxAmt, C01FTaxAmt, RC01TaxAmt, RC01FTaxAmt, R4_C01FTaxAmt, R4_C01TaxAmt;
    
    // �I�d����O���K�|
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE;  R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);
    z->tb_trans.am_trcpntx.sprintf("%08d",giA-C01FTaxAmt-R4_C01FTaxAmt - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ) );       // �o���鵲�b�B(�K)
    z->tb_trans.am_trcptax.sprintf("%08d",giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));       // �o���鵲�b�B(�|)
    z->tb_trans.am_trcptot.sprintf("%08d",giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));       // �o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );


    sTmp.printf("ZDATA::GetZDTData: �o�����b�֭p���B=%s, �o���鵲�b���B=%d ", str_am_tclsacc.c_str(), int_tot_amt  );
    writelog(sTmp );

      //���鵲�b�֭p���B
    if( (int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) > 99999999||(int_tot_amt+_StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")) < -9000000  )
          int_acc_tot_amt=int_tot_amt;
    else
          int_acc_tot_amt=(int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p"));

    Tclsacc=IntToStr(int_acc_tot_amt);

    z->tb_trans.am_tclsacc.sprintf("%08d", int_acc_tot_amt);
    //z->tb_trans.am_tclsacc.sprintf("%08d", (int_tot_amt + _StrToInt(str_am_tclsacc,"���鵲�b���B�֭p")));

    TotMiss(0,gi_tot_line);
    //RC01FTaxAmt=giC; RC01TaxAmt=giD;
    z->tb_trans.qt_miss.sprintf("%08d",   giA);       //  �~���o������
    z->tb_trans.am_missntx.sprintf("%08d",giB-RC01FTaxAmt-R4_C01FTaxAmt- R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);       //  �~���o�����B(�K)
    z->tb_trans.am_misstax.sprintf("%08d",giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);       //  �~���o�����B(�|)
    z->tb_trans.am_miss.sprintf("%08d",   giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );       //  �~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //���}�o�����B, ���}�o�����B�~�h��
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+PreSal10_AmtTX+PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;

    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    //ZDATA::GetZDTData �����ھP�f���J
    z->tb_trans.am_totsale.sprintf("%08d", int_totsale );


    if (gbl_tot_haveline)
    {
        z->no_rcstart = invoice_start_end(gi_tot_line, true);    //�o���}�l���X
        z->no_rcend = invoice_start_end(gi_tot_line, false);     //�o���������X
    }
    else    //�L�o�����
    {
        z->no_rcstart = "0000000000";                            //�o���}�l���X
        z->no_rcend   = "0000000000";                            //�o���������X
    }


    AnsiString z_b;

    z_b += (z->tb_trans.qt_tcust+"|");             //����ӫȼ�
    z_b += (z->tb_trans.qt_trans+"|");             //�������~��
    z_b += (z->tb_trans.am_tclsacc+"|");           //���鵲�b�֭p���B
    z_b += (z->no_rcstart+"|");                   //�o���}�l���X
    z_b += (z->no_rcend+"|");                      //�o���������X
    z_b += (z->tb_trans.am_trcpntx+"|");          //�o���鵲�b�B(�K)
    z_b += (z->tb_trans.am_trcptax+"|");           //�o���鵲�b�B(�|)
    z_b += (z->tb_trans.am_trcptot+"|");          //�o���鵲�b���B
    z_b += (z->tb_trans.qt_miss+"|");              //�~���o������
    z_b += (z->tb_trans.am_missntx+"|");           //�~���o�����B(�K)
    z_b += (z->tb_trans.am_misstax+"|");           //�~���o�����B(�|)
    z_b += (z->tb_trans.am_miss+"|");              //�~���o�����B
    z_b += (z->tb_trans.am_totsale+"|");           //�����ھP�f���J

    /* ZDATA::GetZDTData �N��I��������       */

    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                 //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;             //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);     //giH giI ���w�h�f���B�B����     //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    z->tb_newadd.qt_agnrcv.sprintf("%08d",giD);             //�N���O�Φ���
    z->tb_newadd.am_agnrcv.sprintf("%08d",giC);             //�N���O�Ϊ��B

    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;

    AnsiString z_c;
    int iAmt, iQty, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //z_c += z->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty);
            //z_c += z->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt);

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //z_c += z->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty);
        //z_c += z->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt);
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;

    //�N��X�p����
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //�N��X�p���B   �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;
    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev; //+iRentAmt;

    z_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );

    //�N�{����������=============================================
    int  iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);   //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    writelog("giS:���O����B , giT:���O��^��");

    z->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);            //§��^��
    z->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //§����B


    SumPresent41+=giB; //§��+�ӫ~§��
    SumPresent41+=iCSMAmt; //���O��   for  ���ȭ��汵�Z���Ӫ�

    z->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //�t�ӥN�{���^�� �ثe���ϥ�
    z->tb_cash.am_vndcpn.sprintf("%08d", 0);             //�t�ӥN�{�����B

    z->tb_cash.qt_stocpn.sprintf("%08d", giC);           //�����N�{���^��
    z->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //�����N�{�����B
    SumCpn41+=giD;    //�����+Other1+�������� CPN


    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(�D�������B:giU), �������B:giV)     25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    SumCpn41 -=  giU;

    z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));  //�N�{���X�p�^��
    z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //�N�{���X�p���B

    //z->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI ));  //�N�{���X�p�^��
    //z->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ ));  //�N�{���X�p���B

    z->tb_cash.qt_bonout.sprintf("%08d", giG);           //�����X�p�^��
    z->tb_cash.am_bonout.sprintf("%08d", giH);           //�����X�p���B

    iTotOt2Rtn=giE;                                      //�l�B��
    iqt_ffu1=giI;                                        //�{���d�^��  �q�l�� 1847
    iam_ffu1=giJ;                                        //�{���d���B  �q�l�� 1855    ��d�X�p

    AnsiString z_d;

    z_d += (z->tb_cash.qt_bonus+"|");                //§��^��
    z_d += (z->tb_cash.am_bonus+"|");                //§����B
    z_d += (z->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��
    z_d += (z->tb_cash.am_vndcpn+"|");               //�t�ӧ������B
    z_d += (z->tb_cash.qt_stocpn+"|");               //�����^��
    z_d += (z->tb_cash.am_stocpn+"|");               //�������B
    z_d += (z->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��
    z_d += (z->tb_cash.am_totcash+"|");              //�N�{���X�p���B

    sTmp.sprintf("%08d",giF);
    z_d += (sTmp+"|");                               //�l�B�X�p�^��
    sTmp.sprintf("%08d",giE);
    z_d += (sTmp+"|");                               //�l�B�X�p���B

    z_d += (z->tb_cash.qt_bonout+"|");               //�����X�p�^��
    z_d += (z->tb_cash.am_bonout+"|");               //�����X�p���B

    //ZDATA::GetZDTData======================================================
    //�{���X�p �t ���O��

    SumCash41=SumCash(0,gi_tot_line);
    z->am_cash.sprintf("%08d",SumCash41-iTotOt2Rtn+iCSMAmt );

    // writelog("�{���X�p �t ���O��");


    //char ze[29];
    //ze[0] = 0;

    //strcpy(ze,z->am_cash.c_str());         //�{���X�p
    //strcat(ze,z->no_rcstart.c_str());      //�o���}�l���X
    //strcat(ze,z->no_rcend.c_str());        //�o���������X
    //ze[28] = 0;


    //ZDATA::GetZDTData Z�b�����O���pTABLE      =============================================

    AnsiString z_f;

    //s1�����X�p�������
    //s2�����X�p�P����B
    //s3�����X�p�~�h����
    //s4�����X�p�~�h���B
    int s1, s2, s3, s4;

    s1 = s2 = s3 = s4 = 0;
    DEPT *dept;
    try
    {
        dept = new DEPT[20];
        //���N�ȶ�s
        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpsale.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmitm.sprintf("%08d",0);
            z->tb_depcal[i].am_dpmiss.sprintf("%08d",0);

            dept[i].am_dpsitm = 0;
            dept[i].am_dpsale = 0;
            dept[i].am_dpmitm = 0;
            dept[i].am_dpmiss = 0;
        }

        //writelog("INIT ����������, �����b");
        //�p��Ҧ�����������, �����b


          if (gbl_rec_haveline)
            SumAllDept(dept);


        // writelog("�p��Ҧ�����������, �����b OK");


        for (int i=0; i<20; i++)
        {
            z->tb_depcal[i].am_dpsitm.sprintf("%08d", dept[i].am_dpsitm);
            z_f += (z->tb_depcal[i].am_dpsitm+"|");
            s1 += dept[i].am_dpsitm;

            z->tb_depcal[i].am_dpsale.sprintf("%08d", dept[i].am_dpsale);
            z_f += (z->tb_depcal[i].am_dpsale+"|");
            s2 += dept[i].am_dpsale;

            z->tb_depcal[i].am_dpmitm.sprintf("%08d", dept[i].am_dpmitm);
            z_f += (z->tb_depcal[i].am_dpmitm+"|");
            s3 += dept[i].am_dpmitm;

            z->tb_depcal[i].am_dpmiss.sprintf("%08d", dept[i].am_dpmiss);
            z_f += (z->tb_depcal[i].am_dpmiss+"|");
            s4 += dept[i].am_dpmiss;
        }


    }
    __finally
    {
      delete [] dept;
    }

   //ZDATA::GetZDTData writelog("Z�b�����O���pTABLE OK");

    AnsiString z_g;

    z_g += (z->am_tdpsitm.sprintf("%08d", s1)+"|");             // �����X�p�������
    z_g += (z->am_dpstot.sprintf("%08d" , s2)+"|");             // �����X�p�P����B
    z_g += (z->am_tdpmitm.sprintf("%08d", s3)+"|");             // �����X�p�~�h����
    z_g += (z->am_dpmtot.sprintf("%08d" , s4)+"|");             // �����X�p�~�h���B
    z_g += (z->am_cash+"|");                                   // �{���X�p�t���O�� 45


    //ZDATA::GetZDTData ���ȭ����Ӣ�������=============================================

    AnsiString z_h;

    //z_h += (z->am_cash+"|");                                    //�{���X�p
    //z_h += (z->tb_casher.qt_vendtot.sprintf("%08d", 0)+"|");    // �ɹs����
    sTmp.sprintf("%08d",VisuilCpnAmt);
    z_h += (sTmp+"|");                                            //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)

    //z_h += (z->tb_casher.am_vendtot.sprintf("%08d", 0)+"|");    // �ɹs�X�p���B
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    z_h += (sTmp+"|");                                            // �N���ӫ~������馩  �ɹs�X�p���B

    z_h += (z->tb_casher.qt_feedtot.sprintf("%08d", 0)+"|");    // ��s����
    z_h += (z->tb_casher.am_feedtot.sprintf("%08d", 0)+"|");    // ��s�X�p���B

    //��w
    int int_drop_line = (gtsl_drop->Count - 1);
    int int_drops = 0;           //��w����
    int int_drop_money = 0;       //��w���B
    int int_drop_sum = 0;    //��w�`�B
    int x_rtn_cnt=0;

    AnsiString   s1041;
    for (int i=0; i<=int_drop_line; i++)
    {
        //StringSegment_EX(s, "|", 15)
        s1041=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s1041, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("���ͤ鵲�b��:�D��w(%s)���",s1041);
             writelog(sTmp );
             continue;
           }

        int_drop_money  = _StrToInt(_StringSegment_EX(s1041, "|", 11));     //�{��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 12));     //IC�I��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 13));     //�H�Υd
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 14));     //§��
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 15));     //�����
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 17));     //���f��

        //�����o�����B  800   Cloud �����o�����B(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s1041, "|", 18) );

        //ZDATA::GetZDTData�����o�����B  500
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 19));

        //ZDATA::GetZDTData�����o�����B  1000
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 20));

        //ZDATA::GetZDTData�����o�����B  200
        int_drop_money += _StrToInt(_StringSegment_EX(s1041, "|", 22));

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 23)); //�O�� 01���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 25)); //�O�� 02���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 27)); //�O�� 03���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 29)); //�O�� 04���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 31)); //�O�� 05���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 33)); //�O�� 06���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 35)); //�O�� 07���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 37)); //�O�� 08���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 39)); //�O�� 09���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 41)); //�O�� 10���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 43)); //�O�� 11���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 45)); //�O�� 12���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 47)); //�O�� 13���B
        int_drop_money +=_StrToInt(_StringSegment_EX(s1041, "|", 49)); //�O�� 14���B

        int_drop_sum += int_drop_money;          //��w�`�B
        int_drop_money = 0;
        ++int_drops;                             //��w����
    }

    z_h += (z->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        // ��w����
    z_h += (z->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     // ��w�X�p���B

    //writelog("��w�X�p���B OK");

    //ZDATA::GetZDTData �󥿦��ơB���B
    SumUpdat(0,gi_rec_line);
    z_h += (z->tb_casher.qt_update.sprintf("%08d", giA)+"|");          // �󥿦���
    z_h += (z->tb_casher.am_update.sprintf("%08d", giB)+"|");          // �󥿦X�p���B


    //�������
    SumVoid(0,gi_tot_line);
    z_h += (z->tb_casher.qt_void.sprintf("%08d", giA)+"|");            // �����������
    z_h += (z->tb_casher.am_void.sprintf("%08d", giB)+"|");            // ��������X�p���B


     //TIniFile *ini_ecr;

    int int_ZINI_TRAN_MODE_CNT = 0;
    int int_ZINI_TRAN_MODE_AMT = 0;

    try
    {
        //ini_ecr = new TIniFile(XZDATA_INI);

        int_ZINI_TRAN_MODE_CNT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        int_ZINI_TRAN_MODE_AMT = tif_ecr->ReadInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);

        if( int_ZINI_TRAN_MODE_AMT > 99999999)
            int_ZINI_TRAN_MODE_AMT=99999999;

        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_CNT", 0);
        tif_ecr->WriteInteger("ECR", "ZINI_TRAN_MODE_AMT", 0);
    }
    __finally
    {
        //delete ini_ecr;
        ;;
    }


    z_h += (z->tb_casher.qt_train.sprintf("%08d", int_ZINI_TRAN_MODE_CNT)+"|"); // �V�m�X�p����
    z_h += (z->tb_casher.am_train.sprintf("%08d", int_ZINI_TRAN_MODE_AMT)+"|"); // �V�m�X�p���B

    //��ʿ�J����13�B14
    SumInpdp(0,gi_rec_line);
    z_h += (z->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");            //��ʿ�J��������
    z_h += (z->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");            //��ʿ�J�������B

    z_h += (z->tb_casher.qt_openbox.sprintf("%08d",gi_open_z)+"|");     // �}���d����

    //�馩������   //�p�p�馩
    SumDisc(0,gi_pay_line);
    z_h += (z->tb_casher.qt_disc.sprintf("%08d", giA)+"|");             // �馩����(���|)
    z_h += (z->tb_casher.am_disc.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");            // �馩���B(���|)

    pay_sub_amt(0,gi_pay_line);
    z_h += (z->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");            //�P�P�馩/������
    z_h += (z->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");             //�P�P�馩/�����B

    //�[�Ⱦ�
    easy_card(0,gi_easy_line);
    z_h += (z->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");          // �[�Ⱦ��}/��d����
    z_h += (z->tb_casher.am_disret1.sprintf("%08d", giB)+"|");         // �[�Ⱦ��}/��d���B
    z_h += (z->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");          // �[�Ⱦ��[�Ȧ���
    z_h += (z->tb_casher.am_disret2.sprintf("%08d", giD)+"|");          // �[�Ⱦ��[�Ȫ��B

    //fetc_card(0,gi_fetc_line);   //e�q�d  2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //�ƥΤK����  �{�� �[�Ȧ���  2015/08/19 �N��,����O�P����B(�K)
    //z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|"); //�ƥΤK���B  �{�� �[�Ȫ��B   2015/08/19 �N��,����O�@�o���B(�K)
    z_h += (z->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���  2015/08/19 �N��,����O�P����B(�K)
    z_h += (z->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|"); //�ƥΤK���B  �{�� �[�Ȫ��B   2015/08/19 �N��,����O�@�o���B(�K)

     //���w���ơB���B  int iqt_food,iamt_food;
    z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���  2015/08/19 0
    z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B  2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //z_h += (z->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���  2015/08/19 0
    //z_h += (z->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B  2015/08/19 0


    //   05 �l�[ table        =============================================

    AnsiString z_i;

    z_i += (z->tb_newadd.qt_cashCard.sprintf("%08d", iqt_ffu1)+"|");    //��d/�{���d����    �q�l�� 1847
    z_i += (z->tb_newadd.am_cashCard.sprintf("%08d", iam_ffu1)+"|");    //��d/�{���d���B    �q�l��  ��d�X�p 1855
    z_i += (z->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");            //�N�������O����
    z_i += (z->tb_newadd.am_parking.sprintf("%08d", 0)+"|");            //�N�������O���B
    z_i += (z->tb_newadd.qt_agnrcv+"|");                                //�N���O�Φ���  1831
    z_i += (z->tb_newadd.am_agnrcv+"|");                                //�N���O�Ϊ��B  1839
    z_i += (z->tb_newadd.tx_mark.sprintf("%2s", str_ver_num.SubString(1,2) )+"|");      //�����e�m�Ÿ�
    z_i += (z->tb_newadd.tx_ver.sprintf("%6s",  str_ver_num.SubString(3,6) )+"|");      //�������X
    z_i += (z->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");  //�o�����B0����

    //ZDATA::GetZDTData ���}�o�����B
    //SumNOVoiceAmt(0,gi_tot_line);

    z_i += (z->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //���}�o�����B
    z_i += (z->tb_casher.am_pack.sprintf("%08d",int_novoice_miss_amt)+"|");    //  ���}�o�����B�~�h��

    SumLOSS(0,gi_rec_line);
    z_i += (z->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");             // TM�o��Loss�Ȧ���
    z_i += (z->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");             // TM�o��Loss�Ȫ��B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    SumHealthAmt(0,gi_tot_line);         // ���d���X�p
    z_i += (z->tb_newadd.am_ffu7.sprintf("%08d", iSub_C0InvoAmtA)+"|");             //��[�Ⱦ��[�Ȩ������B

    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��
    int iSumRelDimAgn=giB;

    //z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", giB+iSub_C0InvoAmtC)+"|");             //��[�Ⱦ��[�Ȩ�������
    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    z_i += (z->tb_newadd.qt_ffu7.sprintf("%08d", iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");             //��[�Ⱦ��[�Ȩ�������


    //���O��
    z_i += (z->tb_newadd.am_posZRpt.sprintf("%08d", iCSMAmt)+"|");      //���d�����d���B   ���O��

    AnsiString z_k;
    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);     �Y���ʻP�P��i�����|
    z_k +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //�Y���ʻP�P��i�ȧK�|
    z_k +=sTmp;

    //�o���I�����B
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //�o���I���i��   �����o���i��
    z_k +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B   �����o�����B
    z_k +=sTmp;


    //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    z_k +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    z_k +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�N��,����OgiD. �N��,����O�@�ogiE , �I�d����O�ȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);   //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)
    z_k +=sTmp;

    //AnsiString DDZ_1099;
    //DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    //if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
    //   DDZ_1099=str_datez;

    //ZDATA::GetZDTData Z����
    AnsiString str_zrp;

    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17    �I�d����O Update
    str_zrp.sprintf("%s%s%s%s%s%s%s%s%s%16s|\n",
                            z_a.c_str(),
                            z_b.c_str(),
                            z_c.c_str(),
                            z_d.c_str(),
                            z_g.c_str(),
                            z_h.c_str(),
                            z_i.c_str(),
                            z_f.c_str(),
                            z_k.c_str(),
                            "FFFFFFFF"
                            );

    sTmp.printf("ZDATA::GetZDTData::(%s) Len=%d",str_zrp, str_zrp.Length() );
    writelog(sTmp);


    AnsiString  Zrpfilename;
    //sTmp = ExtractFileName(SalFileName);

    Zrpfilename.sprintf("%s%s_%s.zrp",
                                        XZDATADIR,
                                        sYYMMDDZZ.c_str(),
                                        ZDT_1050_DDZ.c_str()  );


    //ZDATA::GetZDTData Z����
    strcpy(report->char_filename, Zrpfilename.c_str()); //ChangeFileExt(str_z_filename,".zrp").c_str());
    report->CreateReport('Z', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev );
    //�ƥ�Z����
    str_zrp_path = BackupFile(report->char_filename, "", DDZ_1099);


    //�����b��
    strcpy(report->char_filename, ChangeFileExt(Zrpfilename,".dpt").c_str() );
    report->CreateReport('D', str_zrp, StrBillPay, gchar_tencode, gchar_ecr_no);
    //�ƥ������b��
    str_dpt_path = BackupFile(report->char_filename, "", DDZ_1099);


    RtnZDTData=str_zrp;

  return(0);
}


//�۰ʤ鵲
int __fastcall AUTOZDATA::WriteData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                    const String SalFileName, const String Version, const String AutoZDttm)
{

    AnsiString RtnAutoXData, sLog, sTmp,str_ver_num, str_tran_cnt;

    AnsiString str_z_filename;              //Z�b��X�ɦW


    sLog.sprintf("AUTOZDATA::�۰ʤ鵲 WriteData(StoreNO=%s, EcrNO=%s, SalesNo=%s, SalFileName=%s, Version=%s, AutoZDttm=)",StoreNO,EcrNO,SalesNo,SalFileName, Version, AutoZDttm);
    writelog( sLog);

    //int int_reslut = 0;
    //int_reslut = loadsalfile(SalFileName);               //���JSAL�ɡA�N�ɮר̫��A����
    //if (!int_reslut)
    //    return 0;

    //�ƥ����J����l����
    //Auto_Sal->LoadFromFile(SalFileName);
    //Autotsl_rec->Assign(gtsl_rec);
    //Autotsl_tot->Assign(gtsl_tot);
    //Autotsl_drop->Assign(gtsl_drop);
    //Autotsl_easy_card->Assign(gtsl_easy_card);
    //Autotsl_pay_sub->Assign(gtsl_pay_sub);
    //Autotsl_bil->Assign(gtsl_bil);
    //Autotsl_fetc_card->Assign(gtsl_fetc_card);
    //Autotsl_subsale->Assign(tsl_subsale);
    //

    //SPOS_ZRPDT *z = new SPOS_ZRPDT;         //Z�b���c


    //�ثeZ�b�ɶ�
    AnsiString str_z_date,                  //�{�b�ɶ�
               str_pz_date,                 //�e���ɶ�
               str_pzno_seq,                //�ثeZ�b����   �����k1
               str_pz_cnt,                  //�e���鵲�Ǹ�  ���餣�k�s
               str_nz_cnt,                  //�ثe�鵲�Ǹ�  ���餣�k�s
               str_am_tclsacc,              //�ֿnZ�b���B   ���餣�k�s
               sYYMMDDZZ,                   //Z�b���Ǹ�
               Lst_AutoZ_Dttm,              //�e���۰ʤ鵲�ɶ�
               Nst_AutoZ_Dttm,              //�{�A�۰ʤ鵲�ɶ�
               AutoZ_Dt,                    //�鵲�����
               str_date_time, str_datez,    //�ثe�b���t�ήɶ�
               AutoZ_Cnt,
               Lst_ZCNT_Dttm,
               str_orgam_tclsacc,
               DDZ_1099, sRec, TmpZcnt, SaleZDT,str_zrp_path, str_dpt_path,
               str_zrp,
               str_sal_first,
               AutoZstartTrans ;


    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "00001");
    int iTmpCnt =  _StrToInt( str_nz_cnt )-1;
    if( iTmpCnt <= 0)
        iTmpCnt=99999;
    AutoZ_Cnt.sprintf("%05d",iTmpCnt);

    Lst_AutoZ_Dttm=tif_ecr->ReadString("AUTOZ","LST_AUTOZ_DTTM","");              //�e���۰ʤ鵲�ɶ�
    Nst_AutoZ_Dttm=tif_ecr->ReadString("AUTOZ","NST_AUTOZ_DTTM","");              //�{�A�۰ʤ鵲�ɶ�
    Lst_ZCNT_Dttm=tif_ecr->ReadString("AUTOZ","LST_ZCNT_DTTM",FormatDateTime("yyyymmdd",Now()) );

    if( Trim(AutoZDttm)=="" || AutoZDttm.Length()!=8 )
        AutoZ_Dt = FormatDateTime("yyyymmdd",Now()-1);  //�鵲�����
    else
        AutoZ_Dt = AutoZDttm;

    //AutoZ_Dt="20130522" ;

    ////////////////////////////   �۰ʥ�Z   //////////////////////////////

     RtnAutoXData=AutoXData(StoreNO, EcrNO, SalesNo , SalFileName, Version, AutoZ_Cnt, AutoZ_Dt ) ;

     gtsl_rec->Clear();
     gtsl_tot->Clear();
     gtsl_drop->Clear();
     gtsl_easy_card->Clear();
     gtsl_pay_sub->Clear();
     gtsl_bil->Clear();
     gtsl_fetc_card->Clear();
     tsl_subsale->Clear();
     gtsl_1051_sal->Clear();
     gtsl_Ticket->Clear();

     str_date_time = gchar_date_time;  // ����ɶ����۰ʥ�Z


     //////////////////////////////////////////////////////////////////////

    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, false , AutoZ_Cnt, AutoZ_Dt);               //���JSAL�ɡA�N�ɮר̫��A����
    //int_reslut = loadsalfile(SalFileName);               //���JSAL�ɡA�N�ɮר̫��A����
    if (!int_reslut)
       {
         // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("AUTOZDATA::�]�w [X]->XOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

        tif_ecr->WriteString("Z","ZOPERATION","0");
        sLog.sprintf("AUTOZDATA::�]�w [Z]->ZOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }


         sTmp = FormatDateTime("yyyymmdd",Now());
         tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
         sLog.sprintf("AUTOZDATA::loadsalfile:%s Error...�L���i��鵲�ʧ@.", SalFileName.c_str() );
         writelog( sLog);
         sLog.sprintf("AUTOZDATA::�]�w�۰ʤ鵲�ɶ� AUTOZ->LST_AUTOZ_DTTM=%s..�۰ʤ鵲����.", sTmp.c_str() );
         writelog( sLog);
         //return 0;      //�d�U���i�^ return OK;
         return 2;      //2013/10/28
       }
 

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );
    sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );
    str_ver_num = Version;   //get_version(Version);       //�鵲

    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����


    //�ƥ����J����l����
    Auto_Sal->Clear();
    Auto_Sal->LoadFromFile(SalFileName);
    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Autotsl_Ticket->Assign(gtsl_Ticket);
    //Auto_1051_sal->Assign(gtsl_1051_sal);           �۰ʤ鵲�L���ϥ�

    AutoZstartTrans=GetSysDttmMaxTransNo(str_date_time.SubString(1,8) );

    if( !GetYYYYMDD_SalData(AutoZ_Cnt, AutoZ_Dt, 1) )    // ���o ����餧 sal ���
      {
        // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("AUTOZDATA::�]�w [X]->XOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

        tif_ecr->WriteString("Z","ZOPERATION","0");
        sLog.sprintf("AUTOZDATA::�]�w [Z]->ZOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp = FormatDateTime("yyyymmdd",Now());
        tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
        sLog.sprintf("AUTOZDATA::GetYYYYMDD_SalData:Auto_Sal ���d�L�����鵲���(%s)�s�@�b��...�L���i��鵲�ʧ@.", AutoZ_Dt.c_str() );
        writelog( sLog);
        sLog.sprintf("AUTOZDATA::�]�w�۰ʤ鵲�ɶ� AUTOZ->LST_AUTOZ_DTTM=%s..�۰ʤ鵲����.", sTmp.c_str() );
        writelog( sLog);
        //return 0;    //�d�U���i�^ return OK;
        return 2;      //2013/10/28
      }

    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");      //�ثe����Ǹ�,��SAL�̫�@������1
    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );
    sLog.sprintf("AUTOZDATA::�e���۰ʤ鵲�ɶ�=%s, �{�b�۰ʤ鵲�ɶ�=%s, �����=%s, �̤j�鵲�Ǹ�=%s, ECR->WriteData �����ثe������Ǹ�:%s, ����Ѥ�Lst_ZCNT_Dttm=%s, AutoZstartTrans=%s",
                  Lst_AutoZ_Dttm.c_str(),FormatDateTime("yyyymmdd",Now()), AutoZ_Dt.c_str(), AutoZ_Cnt.c_str(), str_tran_cnt, Lst_ZCNT_Dttm.c_str(), AutoZstartTrans.c_str() );
    writelog( sLog);


    TStringList *Z_CntLst = new TStringList;
    TStringList *ZDT_Lst = new TStringList;

    Z_CntLst->Clear();
    ZDT_Lst->Clear();

    if( !_GetZCntLst(Z_CntLst, Auto_Sal  ) )
      {
        // 2013/10/21 Update
        tif_ecr->WriteString("X","XOPERATION", "0");
        sLog.sprintf("AUTOZDATA::�]�w [X]->XOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

        tif_ecr->WriteString("Z","ZOPERATION","0");
        sLog.sprintf("AUTOZDATA::�]�w [Z]->ZOPERATION=0..�w�� XZDATA.INI �Q�٭�");
        writelog( sLog);

         sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp="C:\\FTLPOS\\INI\\XZDATA_ZBAK.INI";
        if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("AUTOZDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

        sTmp = FormatDateTime("yyyymmdd",Now());
        tif_ecr->WriteString("AUTOZ","LST_AUTOZ_DTTM", sTmp );
        sLog.sprintf("AUTOZDATA::_GetZCntLst:Auto_Sal ���d�L�����鵲�Ǹ��s�@�b��...�L���i��鵲�ʧ@.");
        writelog( sLog);
        sLog.sprintf("AUTOZDATA::�]�w�۰ʤ鵲�ɶ� AUTOZ->LST_AUTOZ_DTTM=%s..�۰ʤ鵲����.", sTmp.c_str() );
        writelog( sLog);
        //return 0;    //�d�U���i�^ return OK;
        return 2;      //2013/10/28
      }


    str_sal_first = gpchar_sal_first;
    str_pz_date = tif_ecr->ReadString("Z","PZ_DATE","");       //�e���ɶ�

    AnsiString sTmpFirstDttm;
    sTmpFirstDttm=_StringSegment_EX(str_sal_first, "|", 5);

    //���׳q����---131209-0153...FM-010171-ù�F�_���� SC �禬��12/7��TM2 Z�b�A�d��.ZRP�� �ɮצW�ٲ��`
    //if( _StringSegment_EX(str_sal_first, "|", 5) < str_pz_date )
    if( sTmpFirstDttm < str_pz_date  && Trim(sTmpFirstDttm).Length()==14 )
      {
        //sLog.sprintf("AUTOZDATA:: �����ثe�}�l�����:%s",str_sal_first.c_str() );
        //writelog( sLog);
        str_pz_date=sTmpFirstDttm; //_StringSegment_EX(str_sal_first, "|", 5);
        sLog.sprintf("AUTOZDATA:: �����ثe�}�l�����:%s Set str_pz_date=%s",str_sal_first.c_str(), str_pz_date.c_str() );
        writelog( sLog);
      }


     if( Autotsl_tot->Count > 0)
       {
            sTmp=Autotsl_tot->Strings[Autotsl_tot->Count-1];
            sLog.sprintf("AUTOZDATA:: �����ثe�̤j1050�����:%s",sTmp.c_str() );
            writelog( sLog);

            str_z_date=_StringSegment_EX(sTmp, "|", 5);
       }
     else
       {
        str_z_date = _StringSegment_EX(gsLast1099, "|", 5);  //str_date_time;   //�{�b�ɶ�
       }

    if( str_z_date.Trim() == "")
        str_z_date=str_date_time;   //�{�b�ɶ�

    str_datez = tif_ecr->ReadString("Z","DATEZ","");           //DDZ
    sTmp=tif_ecr->ReadString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100" ));  //�۰ʤ鵲�n�p��

    //�N�b�����Ƴƥ��w�Q�N�ӧP�_���ؿ�����, �R���ؿ��}�l��� james - 2003/02/07
    tif_ecr->WriteString("Backup", "pz_date", sTmp);


    //�ƥ� sal8.TXT����
    tif_ecr->WriteString("Backup", "datez", str_datez);

    if ((str_pz_date == "") || (str_pz_date.Length() < 8))  //�L������ơA����O���k�s
    {
        tif_ecr->WriteString("Z", "pz_date", str_z_date);
        tif_ecr->WriteString("Z", "pzno_seq", "0");
        tif_ecr->WriteString("Z", "nzno_seq", "1");
        tif_ecr->WriteString("Z", "pz_cnt", "00000");
        tif_ecr->WriteString("Z", "nz_cnt", "00001");
        tif_ecr->WriteString("Z", "datez", (str_date_time.SubString(7,2) + "1"));
        //20031225
        tif_ecr->WriteString("Z", "YYYYMMDDZ", (str_date_time.SubString(1,8) + "0100"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        tif_ecr->WriteString("Z", "zfilename", "");
        tif_ecr->WriteString("Z", "zrptname", "");
        tif_ecr->WriteString("Z", "deptname", "");
        tif_ecr->WriteString("Z", "inqtname", "");

        tif_ecr->WriteString("VXZ", "PZNO_SEQ", "0");
        tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");
        tif_ecr->WriteString("VXZ", "PZ_DATE", "");

        //tif_ecr->WriteString("VXZ","CD_SATNCODE","");
        //tif_ecr->WriteString("VXZ","CS_ENTRY","");

        AnsiString str_xpath;               //���w���ȭ��b���ؿ�
        str_xpath = ("C:\\FTLPOS\\HISTORY\\" + str_z_date.SubString(7,2) + "\\01\\");
        tif_ecr->WriteString("Z","XPATH",str_xpath);

        str_pz_date = str_z_date.SubString(1,8)+"000000";
    }


    str_pzno_seq   = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b���� 01~09  �����k1
    str_nz_cnt     = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s 00001~99999
    tif_ecr->WriteString("Z_OFF", "nz_cnt", str_nz_cnt  );            // ���u��ƨϥ�
    tif_ecr->WriteString("Z_OFF", "pz_cnt", tif_ecr->ReadString("Z","pz_cnt","00000")  );  // ���u��ƨϥ�

    str_am_tclsacc = tif_ecr->ReadString("Z", "AM_TCLSACC", "");      //�ֿnZ�b���B   ���餣�k�s


    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    if (str_nz_cnt.Length() < 5)
        str_nz_cnt = "00001";

    sYYMMDDZZ.sprintf("%s%02d", str_pz_date.SubString(3,6).c_str(),_StrToInt(str_pzno_seq));
    
#ifdef TM_20


    str_z_filename.sprintf("%s%02d%s%02d%05s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        str_pz_date.SubString(3,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        AutoZ_Cnt.c_str()              );
#else
    //��X�ɮ� yyyymmddnn.zdt ex:2002010101.zdt
    //str_z_filename = (str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt");

    str_z_filename.sprintf("%s%02d%s%02d%05s.zdt",
                                        XZDATADIR,
                                        atoi(gchar_ecr_no),
                                        str_pz_date.SubString(3,6).c_str(),
                                        _StrToInt(str_pzno_seq),
                                        AutoZ_Cnt.c_str()              );

#endif

    //����Flag
    bool bl_change_day;

    //�P�_����
    //if (str_pz_date.SubString(1,8).AnsiCompare(str_z_date.SubString(1,8)))
    if (str_pz_date.SubString(1,8).AnsiCompare(str_date_time.SubString(1,8)))
        {
          sLog.sprintf("AUTOZDATA::����P�_ pz_date=%s, date_time=%s.. �t�Τw���� Set bl_change_day = true",str_pz_date.c_str(), str_date_time.c_str() );
          writelog( sLog);
          bl_change_day = true;
        }
    else
        {
          sLog.sprintf("AUTOZDATA::����P�_ pz_date=%s, date_time=%s.. �t�Υ����� Set bl_change_day = true",str_pz_date.c_str(), str_date_time.c_str() );
          writelog( sLog);
          bl_change_day = false;
        }

    sLog.sprintf("AUTOZDATA::Last1099=(%s)",gsLast1099.c_str());
    writelog( sLog);

    DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    if(  _StrToInt( DDZ_1099.SubString(1,2) ) < 1 || _StrToInt( DDZ_1099.SubString(1,2) ) > 31 )
       DDZ_1099=str_datez;
   
    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //���v�ؿ����@�� ,���藍�i�R���ؿ�


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  str_orgam_tclsacc=str_am_tclsacc;

   AnsiString ZrpFile, DptFile;

   ZrpFile=ChangeFileExt( str_z_filename, ".zrp");     //ExtractFileName(str_z_filename)
   DptFile=ChangeFileExt( str_z_filename, ".dpt");
     // sBackupPath=ExtractFilePath(BackupFileName);
     //   sFileNameNonPath = ExtractFileName(BackupFileName);    //�ɦW���]�t���|
  for( int i=0; i< Z_CntLst->Count; i++)
       {
         sRec=Z_CntLst->Strings[i];
         TmpZcnt=_StringSegment_EX(sRec, "|", 6);  //�鵲�Ǹ�
         //sTmpDttm=_StringSegment_EX(sRec, "|", 5);
         GetZCNT_SalData(TmpZcnt) ;
         GetZDTData(TmpZcnt, String(gchar_tencode),  String(gchar_ecr_no), String(gchar_cashier),
                    String(str_ver_num), str_pz_date, str_z_date, DDZ_1099, sRec,
                    SaleZDT, str_am_tclsacc, str_zrp_path, str_dpt_path  );

         ZDT_Lst->Add(SaleZDT);
         str_zrp+=SaleZDT;

         SaleZDT="";

         file_cat( str_zrp_path.c_str(), ZrpFile.c_str() );
         sLog.sprintf("AUTOZDATA::file_cat(%s, %s)",str_zrp_path.c_str(), ZrpFile.c_str() );
         writelog( sLog);

         file_cat( str_dpt_path.c_str(), DptFile.c_str() );
         sLog.sprintf("AUTOZDATA::file_cat(%s, %s)",str_dpt_path.c_str(), DptFile.c_str() );
         writelog( sLog);


       } // end of for( int i=0; i< Z_CntLst->Count; i++)

     sTmp=ExtractFilePath(str_zrp_path);
     str_zrp_path=sTmp+ExtractFileName(ZrpFile);
     CopyFile(ZrpFile.c_str(), str_zrp_path.c_str(), false);
     sLog.sprintf("AUTOZDATA::CopyFile(%s, %s)",ZrpFile.c_str(), str_zrp_path.c_str() );
     writelog( sLog);
     DeleteFile( ZrpFile );
     sLog.sprintf("AUTOZDATA::DeleteFile(%s)",ZrpFile.c_str() );
     writelog( sLog);

     str_dpt_path=sTmp+ExtractFileName(DptFile);
     CopyFile(DptFile.c_str(), str_dpt_path.c_str(), false);
     sLog.sprintf("AUTOZDATA::CopyFile(%s, %s)",DptFile.c_str(), str_dpt_path.c_str() );
     writelog( sLog);
     DeleteFile( DptFile );
     sLog.sprintf("AUTOZDATA::DeleteFile(%s)",DptFile.c_str() );
     writelog( sLog);

     delete Z_CntLst;
     delete ZDT_Lst;


     //�g�JSAL��98Type
    AnsiString str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_p_z_cnt,
               str_n_z_cnt,
               str_start_trans,            //�}�l����Ǹ�
               str_am_tclascc,
               str_am_totsale,
               str_zdt_no,
               str_ver,
               str_sal_last_1099,
               SaleDataVer,
               str_zdt_path;



    str_rec_type = "1098";
    str_rec_len = "0121";
    str_tencode = gchar_tencode;
    str_ecr_no.sprintf("%02d",_StrToInt(z->no_tm,"z->no_tm"));
    str_sys_date = (z->bg_dtclose + z->bg_tmclose);      //����ɶ�

    //98����Ǹ� //20031215
    int int_sum_tran_cnt ;

    str_sal_last_1099=gsLast1099;
    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

    //if (str_tran_cnt.Length() < 5)
    //if( Trim(str_tran_cnt)=="00000")
    if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //����Ǹ�

    int_sum_tran_cnt=_StrToInt(str_tran_cnt);

    sLog.sprintf("AUTOZDATA::Last1099(%s), ECR->TRAN_CNT=%s",str_sal_last_1099, str_tran_cnt );
    writelog( sLog);


    AnsiString str_sum_tran;
    if( (int_sum_tran_cnt + 1 ) > 99999)
         str_sum_tran="00001";
    else
         str_sum_tran.sprintf("%05d", (int_sum_tran_cnt + 1));

   // tif_ecr->WriteString("ECR", "TRAN_CNT", str_sum_tran);

    //�e���鵲�Ǹ��A�ثe�鵲�Ǹ�
    str_p_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","PZNO_SEQ",""), "�e���鵲�Ǹ�"));
    str_n_z_cnt.sprintf("%05d",_StrToInt(tif_ecr->ReadString("Z","NZNO_SEQ",""), "�ثe�鵲�Ǹ�"));


    if (tif_ecr->ReadString("Z","PZNO_SEQ","").Length() < 5)
        str_p_z_cnt = "00000";

    if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 5)
        str_n_z_cnt = "00001";


    int i_tran_cnt, int_am_tclsacc;

    //str_sal_first = gpchar_sal_first;

     //�}�l����Ǹ�
    i_tran_cnt = _StrToInt(_StringSegment_EX(str_sal_first, "|", 7).SubString(TRANCNT_NO_POS,5));
    str_start_trans.sprintf("%05d", i_tran_cnt);

    //int_am_tclsacc=int_acc_tot_amt;
    //str_am_tclascc.sprintf("%08d",int_am_tclsacc);          //���鵲�b�֭p���B
    str_am_totsale = "0";                                      //z->tb_trans.am_totsale;                //�����ھP�f���J
    str_zdt_no.sprintf("%010ld",z->bg_noclose.ToInt());     //ZDT�Ǹ��N�JZ�ɦW
    str_ver = "FFFFFFFF|";


    AnsiString str_trans1_5;    //����Ǹ��e���X

    //str_sal_last = gpchar_sal_last;

    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    tif_ecr->WriteString("Backup", "datez",DDZ_1099);  //���v�ؿ����@��    ���i�R

    AnsiString str_01,str_99;

    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   AutoZ_Cnt.c_str(),                     // str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   SalesNo.c_str(),                       //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   " ",
                   " ",
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );

    //str_99 = str_01;      for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

     str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%5s|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   AutoZ_Cnt.c_str(),                     //str_nz_cnt.c_str(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
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

    AnsiString _str_tran_cnt;        //����Ǹ�
    AnsiString str_st_tran_cnt;     //�}�l����Ǹ�

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);
    str_st_tran_cnt = (str_trans1_5 + str_start_trans);

    AnsiString str_98;  //�g�J1098����Ǹ�
    str_98.sprintf("%4s|%4s|%6s|%2s|%14s|%-5s|%-10s|%5s|%5s|%10s|000%8s|000%8s|%10s|%9s\n",
                        str_rec_type.c_str(),
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        str_sys_date.c_str(),
                        AutoZ_Cnt.c_str(),          //str_nz_cnt.c_str(),
                        _str_tran_cnt.c_str(),
                        str_p_z_cnt.c_str(),
                        str_n_z_cnt.c_str(),
                        str_st_tran_cnt.c_str(),
                        str_am_tclascc.c_str(),
                        str_am_totsale.c_str(),
                        str_zdt_no.c_str(),
                        str_ver.c_str()
                        );

    String str_sal;
    //str_sal = str_01 + "\n"+ RtnAutoXData + "\n" +str_zrp + str_98 + str_99;

    if( RtnAutoXData.Trim()=="")
       str_sal = str_01 + "\n"+ str_zrp + str_99;
    else
       str_sal = str_01 + "\n"+ RtnAutoXData + "\n" +str_zrp + str_99;

    ///////////////////////

    if (WriteSAL(str_sal, str_z_filename) == OK)
       {
        //�ƥ�ZDT�b
        str_zdt_path = BackupFile(str_z_filename,"", DDZ_1099);
        tif_tmp->WriteString("Z","x1098filename", str_zdt_path);
       }
    else
      {
       tif_tmp->WriteString("Z","x1098filename", "");
      }

    /////////////////////////////////////////////////////////////////

    AnsiString StrZslFile;
    if (WriteSAL(str_sal, SalFileName) == OK)
       {
         //�ƥ�ZSL�b

         sTmp = ExtractFileName(SalFileName);
         StrZslFile.sprintf("%s%s_%s%s",XZDATASAL,str_sys_date.SubString(1,8),DDZ_1099,sTmp);
         if( CopyFile( SalFileName.c_str(), StrZslFile.c_str(), false)  )
           {
             sLog.sprintf("AUTOZDATA:: InPutFile SalFileName �ƥ�SAL %s TO %s",
                  SalFileName, StrZslFile );
             writelog( sLog);

           }
         else
           {
             sLog.sprintf("AUTOZDATA:: InPutFile SalFileName �ƥ�SAL %s TO %s ����!!",
                  SalFileName, StrZslFile );
             writelog( sLog);
           }


         StrZslFile = BackupFile(SalFileName, ChangeFileExt(str_zdt_path,".zsl"), DDZ_1099 );
         tif_tmp->WriteString("Z","ZSLNAME", StrZslFile);

         sTmp=ChangeFileExt(StrZslFile,".asl");
         Auto_Sal->SaveToFile(sTmp);
         writelog("AUTOZDATA::�ƥ����J��SAL Auto_Sal->SaveToFile=>" + sTmp);

       }
    else
       {
         tif_tmp->WriteString("Z","ZSLNAME","");
       }


    AnsiString str_ipt_path;

    //���XZDATA.INI
    int int_nzno_seq   = _StrToInt(str_pzno_seq, "pzno_seq");
    int int_nz_cnt     = _StrToInt(str_nz_cnt, "nz_cnt");
    int_am_tclsacc = _StrToInt(z->tb_trans.am_tclsacc, "am_tclsacc");

    tif_tmp->WriteString("Z","ZFILENAME" , str_zdt_path);
    tif_tmp->WriteString("Z","ZRPTNAME"  , str_zrp_path);
    tif_tmp->WriteString("Z","DEPTNAME"  , str_dpt_path);
    tif_tmp->WriteString("Z","INQTNAME"  , str_ipt_path);

    tif_tmp->WriteString("Z","PZ_DATE"     ,AutoZ_Dt+"235959" ); //str_z_date);         2013/09/12
    tif_tmp->WriteString("Z","AM_TCLSACC" , IntToStr(int_am_tclsacc) );
    tif_tmp->WriteString("Z","ZfileTranCnt" , _str_tran_cnt);  // ZDT ����Ǹ�

    tif_tmp->WriteString("AUTOZ","LST_AUTOZ_DTTM",str_date_time.SubString(1,8)); //AutoZ_Dt);   //�e���۰ʤ鵲�ɶ�
    //tif_tmp->WriteString("AUTOZ","NST_AUTOZ_DTTM","");  //�{�A�۰ʤ鵲�ɶ��L�@��
    tif_tmp->WriteString("AUTOZ","LST_ZCNT_DTTM",Lst_ZCNT_Dttm);   //����ѦҤ�

    tif_tmp->WriteString("ECR","tot_open_z", "0");
    tif_ecr->WriteString("ECR","tot_open_z", "0");

    tif_tmp->WriteString("VXZ", "NZNO_SEQ", "1");
    tif_ecr->WriteString("VXZ", "NZNO_SEQ", "1");

    tif_tmp->WriteString("VXZ", "PZ_DATE", "");
    tif_ecr->WriteString("VXZ", "PZ_DATE", "");


    if (bl_change_day) //�P�_�O�_����
    {
        tif_ecr->WriteString("Z","PZNO_SEQ", "0");
        tif_ecr->WriteString("Z","NZNO_SEQ", "1");
        tif_ecr->WriteString("Z","PZ_CNT", AutoZ_Cnt); // str_nz_cnt);

        //if (++int_nz_cnt > 99999) �۰ʤ鵲������
        //    int_nz_cnt = 1;
        //str_nz_cnt.sprintf("%05d",int_nz_cnt);
        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);

        //tif_ecr->WriteString("Z","DATEZ", ( str_z_date.SubString(7,2) + "1" ));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_z_date.SubString(1,8) + "0100" ));

        tif_ecr->WriteString("Z","DATEZ", ( str_date_time.SubString(7,2) + "1" ));
        tif_ecr->WriteString("Z","YYYYMMDDZ", ( str_date_time.SubString(1,8) + "0100" ));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));


        //tif_ecr->WriteString("Backup", "datez",( str_date_time.SubString(7,2) + "1" ) );  //���v�ؿ����@��

        sLog="AUTOZDATA::����B�z DATEZ=" + str_date_time.SubString(7,2)+ "1" +  ", YYYYMMDDZ=" + str_date_time.SubString(1,8)+"0100";
        writelog( sLog);



    }
    else
    {
        tif_ecr->WriteString("Z","PZNO_SEQ", str_pzno_seq);

        if (++int_nzno_seq > 9)
            int_nzno_seq = 1;

        str_pzno_seq.sprintf("%d", int_nzno_seq);

        tif_ecr->WriteString("Z","NZNO_SEQ", str_pzno_seq);
        tif_ecr->WriteString("Z","PZ_CNT", AutoZ_Cnt);  //str_nz_cnt);

        //if (++int_nz_cnt > 99999)       �۰ʤ鵲������
        //    int_nz_cnt = 1;
        //str_nz_cnt.sprintf("%05d", int_nz_cnt);
        //tif_ecr->WriteString("Z","NZ_CNT", str_nz_cnt);

        //tif_ecr->WriteString("Z","DATEZ", (str_z_date.SubString(7,2) + IntToStr(int_nzno_seq)));
        //tif_ecr->WriteString("Z","YYYYMMDDZ", (str_z_date.SubString(1,8)
        //                      + FormatFloat("00",int_nzno_seq) + "00"));

        tif_ecr->WriteString("Z","DATEZ", ( str_date_time.SubString(7,2) + IntToStr(int_nzno_seq)));
        tif_ecr->WriteString("Z","YYYYMMDDZ", (str_date_time.SubString(1,8)
                              + FormatFloat("00",int_nzno_seq) + "00"));
        tif_ecr->WriteString("Z", "monthz", FormatDateTime("mm",Now()));

        //tif_ecr->WriteString("Backup", "datez",( str_date_time.SubString(7,2) + "1" ) );  //���v�ؿ����@��

        sLog="AUTOZDATA::������B�z DATEZ=" + str_date_time.SubString(7,2)+ IntToStr(int_nzno_seq) +  ", YYYYMMDDZ=" + str_date_time.SubString(1,8) + FormatFloat("00",int_nzno_seq) + "00";
        writelog( sLog);

    }

    str_pz_date    = str_z_date;
    str_pzno_seq   = tif_ecr->ReadString("Z","NZNO_SEQ","");      //�ثeZ�b����   �����k1

    if (str_pzno_seq.Length() < 1)
        str_pzno_seq = "1";

    //���wX �bPATH
    AnsiString  str_x_path;
    //str_x_path = write_xpath((str_pz_date.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    str_x_path = write_xpath((str_date_time.SubString(1,8) + "0" + str_pzno_seq + ".zdt" ));
    tif_ecr->WriteString("Z","XPATH",str_x_path);

    tif_ecr->WriteString("ECR", "PRE_TRAN_CNT", str_sum_tran);   //�e���~��/�d�ȭ��L
    if ((int_sum_tran_cnt + 2) > 99999)
         int_sum_tran_cnt = 1;
    else
         int_sum_tran_cnt +=2;

    str_sum_tran.sprintf("%05d", int_sum_tran_cnt );

    tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    AnsiString LstXdtDttm;

    LstXdtDttm=tif_ecr->ReadString("X","EX_DATE","");    // �W���鵲�ɶ�
    if( LstXdtDttm.SubString(1,8) >= str_date_time.SubString(1,8) ) // 00:00~04:00 ����Z
      {

        sLog.sprintf("AUTOZDATA::�۰ʤ鵲����  LstXdtDttm=%s  �j�󵥩� %s  ���NX�b�k�s.", LstXdtDttm.c_str(), str_date_time.c_str() );
        writelog( sLog);

      }
    else
      {
        //tif_ecr->WriteString("X","PTRAN_CNT", AutoZstartTrans.SubString(3,8) );
        
        sLog.sprintf("AUTOZDATA::�۰ʤ鵲����  LstXdtDttm=%s  �p�� %s  �NX�b�k�s.", LstXdtDttm.c_str(), str_date_time.c_str() );
        writelog( sLog);
        // �۰ʤ鵲 ���NX�b�k�s    _WZ_OK �|�W0
        tif_ecr->WriteString("X", "PTRAN_CNT", "00000000");
        tif_ecr->WriteString("X", "SX_DATE"  , "");
        tif_ecr->WriteString("X", "EX_DATE"  , "");
      }

     //2015/03/11 Mantis:019302_��Z���ѡA����۰ʤ鵲XZDATA.INI�Q�^�g
     sTmp="C:\\FTLPOS\\INI\\XZDATA_XBAK.INI";
     if (FileExists(sTmp))
           {
               DeleteFile(sTmp);
               sLog.sprintf("XDATA::DeleteFile=%s.", sTmp );
               writelog( sLog);
           }

    sLog.sprintf("AUTOZDATA::�۰ʤ鵲���� DDZ_1099=%s, XZDATAINI Z->PZ_CNT=%s, ECR->TRAN_CNT=%s, AM_TCLSACC=%d, [AUTOZ]->Lst_ZCNT_Dttm=%s, XPATH=%s,  AutoZstartTrans=%s ",
                  DDZ_1099, AutoZ_Cnt, str_sum_tran,int_am_tclsacc, Lst_ZCNT_Dttm, str_x_path, AutoZstartTrans.SubString(3,8) );
    writelog( sLog);


    return OK;

}


AUTOZDATA::AUTOZDATA()
{
    //
    writelog("Z�b �}�l");
    z = new SPOS_ZRPDT;         //Z�b���c
    x = new SPOS_XRPDT;
    tif_tmp = new TIniFile(ZDATATMP_INI);
    tif_ecr = new TIniFile(XZDATA_INI);
    report = new REPORT();      //�鵲�b��


}

AUTOZDATA::~AUTOZDATA()
{
    delete z, tif_ecr;

    delete report;      //�鵲�B�����b��
    delete tif_tmp;

    writelog("AUTOZ�b ����");


}

////////////////////////////////////////////////////////////////////////////////
//�۰ʦ��ȭ���Z
AnsiString __fastcall AUTOZDATA::AutoXData(const String StoreNO, const String EcrNO, const String SalesNo ,
                                const String SalFileName, const String Version, const AnsiString AutoZ_cnt,
                                const AnsiString AutoZ_Dt )
{
    int int_reslut = 0;
    int_reslut = loadsalfile(SalFileName, true, AutoZ_cnt, AutoZ_Dt);               //���JSAL�ɡA�N�ɮר̫��A����

    AnsiString sLog, SalAutoZcnt;

    sLog.sprintf("AUTOZDATA::�۰ʦ��ȭ���Z AutoXData(%s, %s, %s, %s, %s, %s, %s)",
                  StoreNO,EcrNO,SalesNo,SalFileName, Version, AutoZ_cnt, AutoZ_Dt);
    writelog( sLog);
    SalAutoZcnt=AutoZ_cnt;

    if (!int_reslut)
        return "";

    if( gtsl_tot->Count > 0 || gtsl_drop->Count > 0 )
      {
       ;;
      }
    else
      {
        sLog.sprintf("AUTOZDATA::�۰ʦ��ȭ���Z =>�L����Χ�w���, �����ͦ��ȭ���Z��, gtsl_tot->Count=%d, gtsl_drop->Count=%d ",
                      gtsl_tot->Count, gtsl_drop->Count);
        writelog( sLog);
        return "";
      }

    AnsiString str_ver_num,str_nz_cnt, sTmp, sSalesLoginDttm;

    str_ver_num = get_version(Version);     //�����s��

    sprintf(gchar_tencode,"%6.6s",StoreNO.c_str() );
    sprintf(gchar_ecr_no,"%2s",EcrNO.c_str() );


    if( Trim(gchar_orgtencode)=="")
       sprintf(gchar_orgtencode,"%s",gchar_tencode);      //��l����

    AnsiString str_x_filename,              //XDATA�ɦW
               str_ptran_cnt,               //�e�����b�Ǹ�
               str_date_z,                  //DataZ(����[X�b����)
               str_tran_cnt,                //�}���H�ӵ��b���B
               str_pxno_seq,                //X�b���b����
               str_nxno_seq,                //�U�����b��
               str_YYYYMMDDZ,
               SaleDataVer;

    str_date_z = tif_ecr->ReadString("Z","DATEZ","");             //ddn
    sTmp = tif_ecr->ReadString("ECR","TRAN_CNT","00001");         //�ثe����Ǹ�,��SAL�̫�@������1

    //str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1);
    str_tran_cnt.sprintf("%05d",_StrToInt(sTmp)-1>0?_StrToInt(sTmp)-1:1  );

    sLog.sprintf("AUTOZDATA::AutoXData ECR->WriteData �����ثe������Ǹ�:%s", str_tran_cnt);
    writelog( sLog);

    str_YYYYMMDDZ=tif_ecr->ReadString("Z","YYYYMMDDZ","201001010000");

    AnsiString str_date_time, ed_date_time ;
    //str_date_time = gchar_date_time;

#ifdef TM_20
    str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "00001");             //999999�ثe�鵲�Ǹ�  ���餣�k�s
    str_date_time = tif_ecr->ReadString("Z","pz_date", gchar_date_time);  //2003/03/08 2004/03/22 liu
    if (str_date_time.Length() < 8)
           str_date_time = FormatDateTime("yyyymmddhhnnss",Now());        //2003/03/08 liu
#else
    str_date_time = FormatDateTime("yyyymmddhhnnss",Now());               //2003/03/08 liu
#endif

    if(str_date_z.Length() < 3)    //DATAZ=���+Z�b����  ddn
       str_date_z = (str_date_time.SubString(7,2) + "1");

    AnsiString str_sal_last, str_sal_last_1099, AutoZstartTrans ;        //*.sal �̫�@��
    str_sal_last = gpchar_sal_last;
    str_sal_last_1099=gsLast1099;

    SaleDataVer=_StringSegment_EX(str_sal_last_1099, "|", 9);    //��ƪ���

     //2016/11/03    Ver=2016100100
    if( Trim(SaleDataVer)=="")
        SaleDataVer="2021040100";  //Default:Ver     last:"2018060100"

     if( Trim(SalesNo)=="")
       {
         sTmp= _StringSegment_EX(str_sal_last_1099, "|", 14);
         sprintf(gchar_cashier,"%5.5s",sTmp.c_str() );
       }
     else
       {  sprintf(gchar_cashier,"%5.5s",SalesNo.c_str() );    }


    //if (str_tran_cnt.Length() < 5)  //�W������Ǹ�
    //2012/08/06 Mark �wINI ���̾�
    //if( Trim(str_tran_cnt)=="00000" )
    if ( _StrToInt(str_tran_cnt) < 0 )  //�W������Ǹ�
        str_tran_cnt = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(TRANCNT_NO_POS,5);  //����Ǹ�

    //SalAutoZcnt=_StringSegment_EX(str_sal_last_1099, "|", 6);

    sLog.sprintf("AUTOZDATA::AutoXData Last1099(%s), ECR->TRAN_CNT=%s, SalAutoZcnt=%s",
                  str_sal_last_1099, str_tran_cnt, SalAutoZcnt );
    writelog( sLog);

    //str_pxno_seq = tif_ecr->ReadString("X","pxno_seq","01");                  //�W��x�b�ֿn����
    str_pxno_seq = tif_ecr->ReadString("X","Autoxno_seq","01");                  //�۰ʤ鵲�W������,��ʤ鵲�k0 , �ҥH�� �۰ʤ鵲/��ʤ鵲 �B�z

    str_nxno_seq.sprintf("%02d", (_StrToInt(str_pxno_seq,"pxno_seq") + 1)); //�ثex����ĴX��

    if (_StrToInt(str_nxno_seq,"nxno_seq") > 99)  //�̦h99��X�b
        str_nxno_seq = "01";

    x->no_tm = gchar_ecr_no;                      //���Ⱦ�����
    x->no_casher = gchar_cashier;                 //���ȭ����X

    if (x->no_casher.Length() < 5)
        x->no_casher = "00001";

    //X�b�Ǹ�  01~99
    x->no_seq = str_nxno_seq;

    AnsiString str_x_first;                      //X�b�Ĥ@��Sal(���N����SAL�Ĥ@��)
    str_x_first = gpchar_x_first;

    //x->dt_begin = tif_ecr->ReadString("X","login_dttm","").SubString(1,8); //�}�l���
    //x->tm_begin = tif_ecr->ReadString("X","login_dttm","").SubString(9,6); //�}�l�ɶ�

    x->dt_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(1,8); //�e�����b���
    x->tm_begin = tif_ecr->ReadString("X","EX_DATE","").SubString(9,6); //�e�����b�ɶ�

    sSalesLoginDttm=tif_ecr->ReadString("X","SALESLOGINDTTM","");

    AnsiString str_invo_char;                    //�o���^��r��
    str_invo_char = tif_ecr->ReadString("ECR", "INVO_CHAR", "XX");

    if (str_invo_char.Length() < 2)
        str_invo_char = "XX";

    Auto_Sal->LoadFromFile(SalFileName);
    Autotsl_rec->Assign(gtsl_rec);
    Autotsl_tot->Assign(gtsl_tot);
    Autotsl_drop->Assign(gtsl_drop);
    Autotsl_easy_card->Assign(gtsl_easy_card);
    Autotsl_pay_sub->Assign(gtsl_pay_sub);
    Autotsl_bil->Assign(gtsl_bil);
    Autotsl_fetc_card->Assign(gtsl_fetc_card);
    Autotsl_subsale->Assign(tsl_subsale);
    Auto_1051_sal->Assign(gtsl_1051_sal);
    Autotsl_Ticket->Assign(gtsl_Ticket);

    if( !GetYYYYMDD_SalData(SalAutoZcnt,AutoZ_Dt,1) )
        return ""; // ���o ����餧 sal ���

    sLog.sprintf("AUTOZDATA::�۰ʦ��ȭ���Z .. �{�b�۰ʤ鵲�ɶ�=%s, �����=%s, �̤j�鵲�Ǹ�=%s, ",
                  FormatDateTime("yyyymmdd",Now()), AutoZ_Dt.c_str(), SalAutoZcnt.c_str() );
    writelog( sLog);


    if (gbl_tot_haveline)
    {
        x->no_rcstart = invoice_start_end(gi_tot_line, true);    //�o���}�l���X
        x->no_rcend = invoice_start_end(gi_tot_line, false);     //�o���������X
    }
    else    //�L����o��
    {
        x->no_rcend   = "0000000000";              //�o���}�l���X
        x->no_rcstart = "0000000000";              //�o���������X
    }

    AnsiString str_pzno_seq;
    str_pzno_seq = tif_ecr->ReadString("Z", "NZNO_SEQ", "");        //�ثeZ�b����1~9   �����k1


    str_x_filename.sprintf("%s%02d%s%02d%02d.xdt",
                                         XZDATADIR,
                                         atoi(gchar_ecr_no),
                                        (str_date_time.SubString(3,6)).c_str(),
                                         _StrToInt(str_pzno_seq),_StrToInt(str_nxno_seq) );


    //�������b���


    if( Autotsl_tot->Count > 0)
       {
            sTmp=Autotsl_tot->Strings[Autotsl_tot->Count-1];
            sLog.sprintf("AutoXData:: �����ثe�̤j1050�����:%s",sTmp.c_str() );
            writelog( sLog);
            ed_date_time=_StringSegment_EX(sTmp, "|", 5);
       }
    else
       {
        ed_date_time = _StringSegment_EX(str_sal_last_1099, "|", 5);  //����Ǹ� FormatDateTime("yyyymmddhhnnss",Now());
       }


    x->dt_end = AutoZ_Dt.SubString(1,8); // ed_date_time.SubString(1,8);
    x->tm_end = "235959";                //ed_date_time.SubString(9,6);

    if( ed_date_time.Trim()=="")
        ed_date_time = x->dt_end+x->tm_end;   //FormatDateTime("yyyymmddhhnnss",Now());


    //�L�}�l�ɶ��B�P�����ɶ��ۦP
    if ((x->dt_begin == "00000000") || (x->tm_begin == "000000") || Trim(x->dt_begin) == "")
    {
        x->dt_begin = x->dt_end;
        x->tm_begin = x->tm_end;
    }

    // 2012/03/30 Update
    if( sSalesLoginDttm.Trim()=="")
        sSalesLoginDttm=ed_date_time;

    x->dt_begin=sSalesLoginDttm.SubString(1,8);
    x->tm_begin=sSalesLoginDttm.SubString(9,6);

    AnsiString x_a;
    int iC0TotCount=0;
    // 2013/08/28 Update
    x_a.sprintf("1095|%04d|%-6s|%-2s|%8s%6s|%05d|%-10s|%2s|%5s|%8s|%6s|%8s|%6s|",1779,
                                        gchar_tencode,
                                        x->no_tm.c_str(),         //���Ⱦ�����
                                        x->dt_end.c_str(),        //�������b���  ed_date_time.c_str(),
                                        x->tm_end.c_str(),         //�������b���
                                        SalAutoZcnt.ToInt(),          //str_nz_cnt.ToInt(),
                                        str_YYYYMMDDZ.SubString(3,8),
                                        x->no_seq.c_str(),        //��Z�Ǹ�
                                        x->no_casher.c_str(),     //���ȭ����X
                                        x->dt_begin.c_str(),      //�e�����b���,
                                        x->tm_begin.c_str(),      //�e�����b�ɶ�
                                        x->dt_end.c_str(),        //�������b���
                                        x->tm_end.c_str()         //�������b���
                                     );
    


    //����ӫȼ�
    x->tb_trans.qt_tcust.sprintf("%08d", SumQt_tcust(0,gi_tot_line));   //����ӫȼ�
    iC0TotCount=giA; //�I�d����O�ӫȼ�

    //�������~��
    Sub_C0C1TaxInvoQty(0, gi_rec_line);  // �I�d����O�ƶq
    x->tb_trans.qt_trans.sprintf("%08d", SumQt_trans(0,gi_tot_line)-giC);

    int int_tot_amt = 0, int_miss_amt = 0;
    int int_novoice_amt=0, int_novoice_miss_amt=0;
    // �I�d����O���K�|
    int C01TaxAmt,C01FTaxAmt,RC01TaxAmt,RC01FTaxAmt,R4_C01FTaxAmt,R4_C01TaxAmt;
    C01TaxAmt=C01FTaxAmt=RC01TaxAmt=RC01FTaxAmt=R4_C01FTaxAmt=R4_C01TaxAmt=0;
    Sub_C0C1TaxInvoAmt(0, gi_rec_line);
    C01FTaxAmt=giA; C01TaxAmt=giB;
    RC01FTaxAmt=giC; RC01TaxAmt=giD;
    R4_C01FTaxAmt=giE; R4_C01TaxAmt=giF;

    /*  ST �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �w��I���J�禬(���}�o�����B)���}�o��  */
    SumPreSal_Amt(0, gi_rec_line);
    int PreSal09_AmtTX,  PreSal09_AmtNTX ;
    int PreSal10_AmtTX,  PreSal10_AmtNTX ;
    int R2R3PreSal09_AmtTX,  R2R3PreSal09_AmtNTX ;
    int R2R3PreSal10_AmtTX,  R2R3PreSal10_AmtNTX ;
    int R4PreSal09_AmtTX,  R4PreSal09_AmtNTX ;
    int R4PreSal10_AmtTX,  R4PreSal10_AmtNTX ;

    PreSal09_AmtTX = giV;        //�w�����|
    PreSal09_AmtNTX = giL;       //�w��K�|
    R2R3PreSal09_AmtTX = giI;    //R2R3�w�����|
    R2R3PreSal09_AmtNTX = giJ;  //R2R3�w��K�|
    R4PreSal09_AmtTX = giX;     //R4�w�����|
    R4PreSal09_AmtNTX= giN;     //R4�w��K�|

    PreSal10_AmtTX = giU;        //�I�����|
    PreSal10_AmtNTX = giK;       //�I���K�|
    R2R3PreSal10_AmtTX = giG;    //R2R�I�����|
    R2R3PreSal10_AmtNTX= giH;    //R2R�I���K�|
    R4PreSal10_AmtTX = giW;     //R4�I�����|
    R4PreSal10_AmtNTX= giM;     //R4�I���K�|

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalMMDisAmtZ0, iPreSalMMDisAmtR2R3;
    SumPreSal_MM_DisAmtTMMFP(0, gi_rec_line);
    iPreSalMMDisAmtZ0=giU;
    iPreSalMMDisAmtR2R3=giV;

    /* End �w��ΧI���ӫ~�έp : �w��}�o�����J�禬 ,  �I���J�禬���}�o��  */

    TotCount(0,gi_tot_line);
    x->tb_trans.am_trcpntx.sprintf("%08d", giA-C01FTaxAmt-R4_C01FTaxAmt  - ( PreSal09_AmtNTX+R4PreSal09_AmtNTX ));        //�o���鵲�b�B(�K)
    x->tb_trans.am_trcptax.sprintf("%08d", giB-C01TaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX+R4PreSal09_AmtTX ));        //�o���鵲�b�B(�|)
    x->tb_trans.am_trcptot.sprintf("%08d", giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX ));        //�o���鵲�b���B
    int_tot_amt = giC-C01FTaxAmt-C01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( PreSal09_AmtTX + PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX );

    TotMiss(0,gi_tot_line);
    x->tb_trans.qt_miss.sprintf("%08d",    giA);        //�~���o������
    x->tb_trans.am_missntx.sprintf("%08d", giB-RC01FTaxAmt-R4_C01FTaxAmt- R2R3PreSal09_AmtNTX - R4PreSal09_AmtNTX);        //�~���o�����B(�K)
    x->tb_trans.am_misstax.sprintf("%08d", giC-RC01TaxAmt-R4_C01TaxAmt- R2R3PreSal09_AmtTX - R4PreSal09_AmtTX);        //�~���o�����B(�|)
    x->tb_trans.am_miss.sprintf("%08d",    giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) );        //�~���o�����B
    int_miss_amt = giD-RC01FTaxAmt-RC01TaxAmt-R4_C01FTaxAmt-R4_C01TaxAmt - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX + R4PreSal09_AmtTX + R4PreSal09_AmtNTX) ;

    //���}�o�����B, ���}�o�����B�~�h��
    // + 10�w��I���J�禬���}�o�� (�I����/�K�|)
    SumNOVoiceAmt(0,gi_tot_line);
    //int_novoice_amt=giA+PreSal10_AmtTX+PreSal10_AmtNTX- iPreSalMMDisAmtZ0;
    int_novoice_amt=giA; // - iPreSalMMDisAmtZ0;
    //int_novoice_miss_amt=giB+ R2R3PreSal10_AmtTX + R2R3PreSal10_AmtNTX- iPreSalMMDisAmtR2R3;
    int_novoice_miss_amt=giB; // - iPreSalMMDisAmtR2R3;
    //�����ھP�f���J = �o�����b���B - �~���o�����B + ���}�o�����B-���}�o�����B�~�h��
    int int_totsale = int_tot_amt - int_miss_amt + int_novoice_amt-int_novoice_miss_amt;

    x->tb_trans.am_totsale.sprintf("%08d", int_totsale);

    AnsiString x_b;

    x_b  = (x->tb_trans.qt_tcust+"|");                      //����ӫȼ�
    x_b += (x->tb_trans.qt_trans+"|");                      //�������~��
    x_b += (x->no_rcstart+"|");                              //�o���}�l���X
    x_b += (x->no_rcend+"|");                                //�o���������X
    x_b += (x->tb_trans.am_trcpntx+"|");                    //�o���鵲�b�B(�K)
    x_b += (x->tb_trans.am_trcptax+"|");                    //�o���鵲�b�B(�|)
    x_b += (x->tb_trans.am_trcptot+"|");                    //�o���鵲�b���B
    x_b += (x->tb_trans.qt_miss+"|");                       //�~���o������
    x_b += (x->tb_trans.am_missntx+"|");                    //�~���o�����B(�K)
    x_b += (x->tb_trans.am_misstax+"|");                    //�~���o�����B(�|)
    x_b += (x->tb_trans.am_miss+"|");                       //�~���o�����B
    x_b += (x->tb_trans.am_totsale+"|");                    //�����ھP�f���J


    //�N��ITABLE===============================================================

    AnsiString x_c;
    int iRentAmt, iRemtCnt;
    SumKamt(0,gi_tot_line);                     //�N���B��B�I ,�I�d����OgiD.�@�ogiE
    int iC0TotAmt, iC0GivUpAmt;             //�N��,����O(��)giD, �N��,����O�@�o(��)giE , �I�d����O�ȼ�giF
    int iC0TotAmtNTx, iC0GivUpAmtNTx;       //�N��,����O(�K)giG, �N��,����O�@�o(�K)giH
    int iqt_food,iamt_food;   //giH giI ���w�h�f���B�B����

    iC0TotAmt=giD; iC0GivUpAmt=giE; //iC0TotCount=giF;
    iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;

    sub_times(0,gi_rec_line);     //giH giI ���w�h�f���B�B����    //�N������,  ���� �h�������(giB), �h���(giA) , �N�I(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_food=giI; iamt_food=giH;

    x->tb_newadd.qt_cetel.sprintf("%08d", giD);          //�N������
    x->tb_newadd.am_cetel.sprintf("%08d", giC);          //�N�����B

    Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    int iSub_C0InvoAmtA,iSub_C0InvoAmtC, iSub_C0InvoQtyC;
    iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;  iSub_C0InvoQtyC=giD;


    int iAmt, iQty, j, k;

    for ( j=0; j<5; j++)        //0->BaseTbSubrev[15], 1->BaseTbSubrev[16], 2->BaseTbSubrev[17] �N��
    {                              //3->BaseTbSubrev[18], 4->BaseTbSubrev[19]   �N�I
        for (k=0; k<8; k++)
        {
            iQty=BaseTbSubrev[j+15].it8_subrev[k].iqt_subrev;
            iAmt=BaseTbSubrev[j+15].it8_subrev[k].iam_subrev;
            //x_c += (x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",iQty)+"|");
            //x_c += (x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",iAmt)+"|");

        }//for k
        iQty=BaseTbSubrev[j+15].iqt5_totrev;
        iAmt=BaseTbSubrev[j+15].iam5_totrev;
        //x_c += (x->tb_subrev[j].it_subrev[8].qt_subrev.sprintf("%08d",iQty)+"|");
        //x_c += (x->tb_subrev[j].it_subrev[8].am_subrev.sprintf("%08d",iAmt)+"|");
    }//for j


    int iqt5_15217totrev, iam5_15217totrev,iqt5_18219totrev,iam5_18219totrev;

    Sub_C0InvoAmt4VDC(0, gi_rec_line);
    int  iC0InvoAmt4VDCN, iC0InvoQty4VDCN ;
    iC0InvoAmt4VDCN=giC;  iC0InvoQty4VDCN=giD;


    //�N��X�p����
    iqt5_15217totrev=BaseTbSubrev[15].iqt5_totrev+
                     BaseTbSubrev[16].iqt5_totrev+
                     BaseTbSubrev[17].iqt5_totrev+iSub_C0InvoQtyC-iC0InvoQty4VDCN;
    //�N��X�p���B   �L���[�J�w��ӫ~
    iam5_15217totrev=BaseTbSubrev[15].iam5_totrev+
                     BaseTbSubrev[16].iam5_totrev+
                     BaseTbSubrev[17].iam5_totrev+iSub_C0InvoAmtC-iC0InvoAmt4VDCN; // + ( PreSal09_AmtTX + PreSal09_AmtNTX ) - ( R2R3PreSal09_AmtTX +  R2R3PreSal09_AmtNTX);

    //�N�I�X�p����
    iqt5_18219totrev=BaseTbSubrev[18].iqt5_totrev+
                     BaseTbSubrev[19].iqt5_totrev; //-iRemtCnt;

    //�N�I�X�p���B
    iam5_18219totrev=BaseTbSubrev[18].iam5_totrev+
                     BaseTbSubrev[19].iam5_totrev;//+iRentAmt;

     x_c.sprintf("%08ld|%08ld|%08ld|%08ld|",
                 iqt5_15217totrev,iam5_15217totrev,
                 iqt5_18219totrev,iam5_18219totrev );




   // writelog( ("X:�N��I�������� �������� 720 �ثeData: " + z_c ) );
    /*******************************************************************
    for (int j=0; j<5; j++)
    {
        for (int k=0; k<9; k++)
        {
            if (k<8)    //�C��TABLE���K��ITEM
            {
                if (j == 0)     //�Ĥ@TABLE�s�N��
                {
                    if (k == 0)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giE);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giA);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else if (j == 3)    //�ĥ|TABLE�s�N��
                {
                    if (!k)
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",giF);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",giB);
                    }
                    else
                    {
                        x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                        x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                    }
                }
                else
                {
                    x_c += x->tb_subrev[j].it_subrev[k].qt_subrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].it_subrev[k].am_subrev.sprintf("%08d",0);
                }
            }
            else
            {
                if (j == 0)     //�N���`�X
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giE);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giA);
                }
                else if (j == 3)    //�N�I�`�X
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",giF);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",giB);
                }
                else
                {
                    x_c += x->tb_subrev[j].qt_totrev.sprintf("%08d",0);
                    x_c += x->tb_subrev[j].am_totrev.sprintf("%08d",0);
                }
            }
        }//for k
    }//for j
    *******************************************************************/

    //�N�{��TABLE===============================================================

    int iqt_ffu1, iam_ffu1, iTotOt2Rtn;
    int SumCash41, SumPresent41, SumCpn41, SumWinInvoAmt;
    SumCash41=SumPresent41=SumCpn41=SumWinInvoAmt=0;

    int EcPrecentCnt, EcPrecentAmt;
    SumECP2Orher2Amt(0, gi_bil_line);
    EcPrecentCnt=0;     //�q�l§��^��
    EcPrecentAmt=giA;  //�q�l§����B

    SumTbCash(0,gi_tot_line);    //�N�{��  ��d�X�p
    EcPrecentCnt=giL;  //�q�l§��^��
    EcPrecentAmt+=giM;  //�q�l§����B

    //�w����(�����S0 10/MMS0 11 )�馩
    int iPreSalCPNDisAmt, iPreSalMMDisAmt_MFP, iPreSalMMDisAmt_TM ;
    SumPreSal_MMCPN_DisAmt(0, gi_rec_line);
    iPreSalCPNDisAmt=giU;
    iPreSalMMDisAmt_MFP=giV;
    iPreSalMMDisAmt_TM=giX;

    //�w��I���ӫ~������馩/�N���ӫ~������馩
    int iPreSalINVODisAmt ;
    SumPreSal_INVO_DisAmt(0, gi_rec_line);
    iPreSalINVODisAmt = giU;

    int iCSMAmt=0;//���O��
    iCSMAmt=SumBillCSM(0, gi_bil_line);//�p�� 2008/12/04 ���O��
    //giS:���O����B , giT:���O��^��;

    x->tb_cash.qt_bonus.sprintf("%08d", giA); //-giT);       //§��^��
    x->tb_cash.am_bonus.sprintf("%08d", giB); //-iCSMAmt);            //§����B
    SumPresent41+=giB; //§��+�ӫ~§��
    SumPresent41+=iCSMAmt; //���O��   for  ���ȭ��汵�Z���Ӫ�

    x->tb_cash.qt_vndcpn.sprintf("%08d", 0);             //�t�ӧ����^�� �ثe���ϥ�
    x->tb_cash.am_vndcpn.sprintf("%08d", 0);             //�t�ӧ������B

    x->tb_cash.qt_stocpn.sprintf("%08d", giC);           //�����^��
    x->tb_cash.am_stocpn.sprintf("%08d", giD); //+iPreSalCPNDisAmt);           //�������B
    SumCpn41+=giD;    //�����+Other1+�������� CPN

    int VisuilCpnAmt=SumBillCPN(0, gi_bil_line);         //(�D�������B:giU), �������B:giV)     25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    SumCpn41 -=  giU;

    //x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI - giT));      //�N�{���X�p�^��
    //x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ - iCSMAmt));  //�N�{���X�p���B

    x->tb_cash.qt_totcash.sprintf("%08d", (giA + giC + giI + EcPrecentCnt)); // - giT));      //�N�{���X�p�^��
    x->tb_cash.am_totcash.sprintf("%08d", (giB + giD + giJ + EcPrecentAmt)); // - iCSMAmt));  //�N�{���X�p���B

    x->tb_cash.qt_bonout.sprintf("%08d", giG);           //�����X�p�^��
    x->tb_cash.am_bonout.sprintf("%08d", giH);           //�����X�p���B

    iTotOt2Rtn=giE;                                      //giE:�l�B����B / giF:�l�B�馸��
    iqt_ffu1=giI;                                        //�{���d�^��  �q�l������  3275
    iam_ffu1=giJ;                                        //�{���d���B  �q�l�����B  ��d�X�p 3261

    AnsiString x_d;

    x_d += (x->tb_cash.qt_bonus+"|");                //§��^��
    x_d += (x->tb_cash.am_bonus+"|");                //§����B
    x_d += (x->tb_cash.qt_vndcpn+"|");               //�t�ӧ����^��
    x_d += (x->tb_cash.am_vndcpn+"|");               //�t�ӧ������B
    x_d += (x->tb_cash.qt_stocpn+"|");               //�����^��
    x_d += (x->tb_cash.am_stocpn+"|");               //�������B
    x_d += (x->tb_cash.qt_totcash+"|");              //�N�{���X�p�^��
    x_d += (x->tb_cash.am_totcash+"|");              //�N�{���X�p���B

    sTmp.sprintf("%08d",giF);
    x_d += (sTmp+"|");                         //�l�B�X�p�^��
    sTmp.sprintf("%08d",giE);
    x_d += (sTmp+"|");                         //�l�B�X�p���B

    x_d += (x->tb_cash.qt_bonout+"|");               //�����X�p�^��
    x_d += (x->tb_cash.am_bonout+"|");               //�����X�p���B

    //==========================================================================

    //�{���X�p �t ���O��-�l�B��
    SumCash41=SumCash(0,gi_tot_line);
    x->am_cash.sprintf("%08d", SumCash41-iTotOt2Rtn+iCSMAmt);

    AnsiString x_e;

    x_e += (x->am_cash+"|");               //�{���X�p
    //x_e += (x->no_rcstart+"|");            //�o���}�l���X
    //x_e += (x->no_rcend+"|");              //�o���������X


    //���ȭ�����FIELD===========================================================

    AnsiString x_f;

    //�ɹs
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tb_vendfld[i].am_vend.sprintf("%08d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].am_vend+"|");

        x->tb_casher.tb_vendfld[i].tm_vend.sprintf("%06d",0);
        //x_f += (x->tb_casher.tb_vendfld[i].tm_vend+"|");
    }

    x->tb_casher.qt_vendtot.sprintf("%08d",0);
    x->tb_casher.am_vendtot.sprintf("%08d",0);

    //x_f += (x->tb_casher.qt_vendtot+"|");      //�ɹs����
    sTmp.sprintf("%08d",VisuilCpnAmt);
    x_f += (sTmp+"|");                          //�ɹs����  : VisuilCpnAmt :  �L�ȥ�������B   25176:TFM���D/�ݨD/�W����ܧ�q��(TMSC�L�ȥ������ק�)
    //x_f += (x->tb_casher.am_vendtot+"|");      //�ɹs�X�p���B
    sTmp.sprintf("%08d",iPreSalINVODisAmt);
    x_f += (sTmp+"|");                           // �N���ӫ~������馩  �ɹs�X�p���B


    AnsiString x_g;

    //��s
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_feedfld[i].am_feed.sprintf("%08d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].am_feed+"|");

        x->tb_casher.tg_feedfld[i].tm_feed.sprintf("%06d",0);
        //x_g += (x->tb_casher.tg_feedfld[i].tm_feed+"|");
    }

    x->tb_casher.qt_feedtot.sprintf("%08d",0);
    x->tb_casher.am_feedtot.sprintf("%08d",0);

    x_g += (x->tb_casher.qt_feedtot+"|");      //��s����
    x_g += (x->tb_casher.am_feedtot+"|");      //��s�X�p���B


    AnsiString x_h;

    //��w
    int int_drop_line = (gtsl_drop->Count - 1);   //��w����

    sTmp.printf("AUTOZDATA::AutoXData ���ͦ��ȭ���Z���Ӫ�:��w����(%d)",gtsl_drop->Count);
    writelog(sTmp );

    //��l�ƥ�����s
    for (int i=0; i<MaxDropRec; i++)
    {
        x->tb_casher.tg_dropfld[i].am_drop.sprintf("%08d",0);    //��w���B
        x->tb_casher.tg_dropfld[i].tm_drop.sprintf("%-6s"," ");    //����ɶ�
    }

    int int_drops = 0;                            //��w����
    int int_drop_money = 0;                       //��w���B
    int int_drop_sum = 0;                         //��w�`�B
    int x_rtn_cnt=0;

    AnsiString str_drop_time, s;

    for (int i=0; i<=int_drop_line; i++)
    {
        s=gtsl_drop->Strings[i];
        x_rtn_cnt=_StrToInt( _StringSegment_EX(s, "|", 10) );
        if( x_rtn_cnt <= 0)
           {
             sTmp.printf("���ͦ��ȭ���Z���Ӫ�:�D��w(%s)���",s);
             writelog(sTmp );
             continue;
           }

        //_StringSegment_EX(s, "|", 38).SubString(1,2)
        int_drop_money  = _StrToInt( _StringSegment_EX(s, "|", 11) );     //�{��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 12) );     //IC�I��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 13) );     //�H�Υd
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 14) );    //§��
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 15) );    //�����
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 16) );    //�����
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 17) );    //���f��

        //�����o�����B  800   Cloud �����o�����B(800)   20210601 Update
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 18) );

        //�����o�����B 500
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 19) );

        //�����o�����B 1000
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 20) );

        //�����o�����B 200
        int_drop_money += _StrToInt( _StringSegment_EX(s, "|", 22) );

        //�O�γ�ڪ��B iacc_amt01..iacc_cnt14
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 23) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 25) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 27) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 29) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 31) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 33) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 35) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 37) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 39) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 41) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 43) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 45) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 47) );
        int_drop_money +=_StrToInt( _StringSegment_EX(s, "|", 49) );

        //�C����w���B
        x->tb_casher.tg_dropfld[int_drops].am_drop.sprintf("%08d", int_drop_money);

        int_drop_sum += int_drop_money;                         //��w�`�B

        str_drop_time = _StringSegment_EX(s, "|", 5).SubString(9,6);  //��w�ɶ�

        //�C����w�ɶ�
        x->tb_casher.tg_dropfld[int_drops].tm_drop.sprintf("%-6s", str_drop_time);

        int_drop_money = 0;
        ++int_drops;                             //��w����
        //sTmp.printf("AUTOZDATA::AutoXData ���ͦ��ȭ���Z���Ӫ�:��w:int_drop_money=%d,int_drops=%d",
        //           int_drop_money,int_drops);
        //writelog(sTmp );

    }

    //for (int i=0; i<50; i++)
    //{
   //     x_h += (x->tb_casher.tg_dropfld[i].am_drop+"h");  //��w�`���`�X
   //     x_h += (x->tb_casher.tg_dropfld[i].tm_drop+"h");  //��w���B�`�X
    //}

    AnsiString x_i;

    x_i += (x->tb_casher.qt_droptot.sprintf("%08d", int_drops)+"|");        //��w����
    x_i += (x->tb_casher.am_droptot.sprintf("%08d", int_drop_sum)+"|");     //��w�X�p���B

    SumUpdat(0,gi_rec_line);
    x_i += (x->tb_casher.qt_update.sprintf("%08d", giA)+"|");               //�󥿦���
    x_i += (x->tb_casher.am_update.sprintf("%08d", giB)+"|");               //�󥿦X�p���B

    SumVoid(0,gi_tot_line);
    x_i += (x->tb_casher.qt_void.sprintf("%08d", giA)+"|");                 //�����������
    x_i += (x->tb_casher.am_void.sprintf("%08d", giB)+"|");                 //��������X�p���B

    SumInpdp(0,gi_rec_line);                                       //�����n��
    x_i += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //��ʿ�J�������ơB����13�B14
    x_i += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //��ʿ�J�������B

    x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //�}���d����

    //�p�p�馩  1011
    SumDisc(0,gi_pay_line);
    x_i += (x->tb_casher.qt_dise.sprintf("%08d", giA)+"|");                 //�馩����
    x_i += (x->tb_casher.am_dise.sprintf("%08d", giB+iPreSalMMDisAmt_MFP - iPreSalINVODisAmt)+"|");         //�馩���B

    pay_sub_amt(0,gi_pay_line);
    x_i += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //�P�P�馩/������
    x_i += (x->tb_newadd.am_ffu6.sprintf("%08d", giB+iPreSalMMDisAmt_TM)+"|");         //�P�P�馩/�����B


    easy_card(0,gi_easy_line);
    x_i += (x->tb_casher.qt_disret1.sprintf("%08d", giA)+"|");                  //�[�Ⱦ��}��d����
    x_i += (x->tb_casher.am_disret1.sprintf("%08d", giB)+"|");                  //�[�Ⱦ��}��d���B
    x_i += (x->tb_casher.qt_disret2.sprintf("%08d", giC)+"|");                  //�[�Ⱦ��[�Ȧ���
    x_i += (x->tb_casher.am_disret2.sprintf("%08d", giD)+"|");                  //�[�Ⱦ��[�Ȫ��B


    //x_i += (x->tb_casher.qt_openbox.sprintf("%08d", gi_open_x)+"|");            //�}���d����
    //x_i += (x->tb_casher.qt_pack.sprintf("%08d" , iTotOt2Rtn)+"|"); //giE);     //�[�Ⱦ����b����   2007/12/01 ��l�B��
    //x_i += (x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"I");   //�[�Ⱦ����b���B, ���}�o�����B�~�h��

    //�l�[TABLE=================================================================


    AnsiString x_j;

    //SumInpdp(0,gi_rec_line);
    //x_j += (x->tb_newadd.qt_inpdp.sprintf("%08d", giA)+"|");       //��ʿ�J�������ơB����13�B14
    //x_j += (x->tb_newadd.am_inpdp.sprintf("%08d", giB)+"|");       //��ʿ�J�������B

    //fetc_card(0,gi_fetc_line);  // 2015/08/19 Mark ,  Add iC0TotAmtNTx=giG; iC0GivUpAmtNTx=giH;
    giFetcCashAddCnt=giFetcCashAddamt=giFetcCredAddCnt=giFetcCredAddamt=0;
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx+ PreSal09_AmtNTX)+"|");  //�ƥΤK����  �{�� �[�Ȧ���   2015/08/19 �N��,����O�P����B(�K)
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx+R2R3PreSal09_AmtNTX)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B    2015/08/19 �N��,����O�@�o���B(�K)
    x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",iC0TotAmtNTx)+"|");  //�ƥΤK����  �{�� �[�Ȧ���   2015/08/19 �N��,����O�P����B(�K)
    x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",iC0GivUpAmtNTx)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B    2015/08/19 �N��,����O�@�o���B(�K)

    //���w���ơB���B  int iqt_food,iamt_food;
    x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",iqt_food)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���  2015/08/19 0
    x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",iamt_food)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B  2015/08/19 0

    // 20151123�ץ������w���ơB���B  Charles.Wang
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���  2015/08/19 0
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B  2015/08/19 0
    x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");         //�{���d����   �q�l������  3275
    x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");         //�{���d���B   �q�l�����B  ��d�X�p 3261

    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����  �O�d
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"|"); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B  �O�d

    //�o�����B0����

    x_j += (x->tb_newadd.qt_parking.sprintf("%08d", 0)+"|");         //�N�������O����
    x_j += (x->tb_newadd.am_parking.sprintf("%08d", 0)+"|");         //�N�������O���B
    x_j += (x->tb_newadd.qt_cetel+"|");                              //�N������FUNCTIO�b�e��
    x_j += (x->tb_newadd.am_cetel+"|");                              //�N�����B
    //x_j += (x->tb_newadd.qt_ffu4.sprintf("%08d", gi_mix)+"|");       //�N���ӫ~�ȼ�
    //x_j += (x->tb_newadd.qt_ffu1.sprintf("%08d", iqt_ffu1)+"|");     //�{���d����   �q�l������  3275
    //x_j += (x->tb_newadd.am_ffu1.sprintf("%08d", iam_ffu1)+"|");     //�{���d���B   �q�l�����B  ��d�X�p 3261
    //x_j += (x->tb_newadd.qt_ffu2.sprintf("%08d", 0)+"|");            //���d�����d����                    3269
    //x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //���d�����d���B  2008/12/04 ���O��  3277
    x_j += (x->tb_newadd.tx_mark.sprintf("%2s", Version.SubString(1,2) )+"|");    //�����e�m�Ÿ�  3285
    x_j += (x->tb_newadd.tx_ver.sprintf("%6s" , str_ver_num)+"|");   //�������X
    x_j += (x->tb_newadd.qt_rcp0.sprintf("%08d", TotZero(0,gi_tot_line))+"|");    //�o�����B0����

    //���}�o�����B

    x_j += (x->tb_newadd.am_ffu4.sprintf("%08d", int_novoice_amt)+"|");        //�ƥΥ|���B,  ���}�o�����B
    x_j +=(x->tb_casher.am_pack.sprintf("%08d" , int_novoice_miss_amt)+"|");   //���}�o���~�h���X�p

    SumLOSS(0,gi_rec_line);
    x_j += (x->tb_newadd.qt_ffu5.sprintf("%08d", giA)+"|");         //TM�o��Loss����
    x_j += (x->tb_newadd.am_ffu5.sprintf("%08d", giB)+"|");         //TM�o��Loss���B

    //Sub_C0InvoAmt(0,gi_rec_line);        // �~�[����OgiA,�Y���ʥN���I�d���p giC
    //int iSub_C0InvoAmtA,iSub_C0InvoAmtC;
    //iSub_C0InvoAmtA=giA;  iSub_C0InvoAmtC=giC;
    //SumHealthAmt(0,gi_tot_line);         // ���d���X�p
    //x_j += x->tb_newadd.qt_ffu7.sprintf("%08d",giA);   //��[�Ⱦ��[�Ȩ�������
    x_j += (x->tb_newadd.am_ffu7.sprintf("%08d",iSub_C0InvoAmtA)+"|");   //��[�Ⱦ��[�Ȩ������B


    SumRelDimAgn(0, gi_rec_line); //�Y���ʥN��   S0/S1   VDC > 0
    int iSumRelDimAgn=giB;


    //x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iSub_C0InvoAmtC)+"|");   //��[�Ⱦ��[�Ȩ�������
    //�Y���ʥN��(S0/S1���s�u�Ϥ�)+�I�d(C0/C1���s�u�Ϥ�)
    x_j += (x->tb_newadd.qt_ffu7.sprintf("%08d",iSumRelDimAgn+iC0InvoAmt4VDCN)+"|");   //��[�Ⱦ��[�Ȩ�������

    x_j += (x->tb_newadd.am_ffu2.sprintf("%08d", iCSMAmt)+"|");      //���O��


    //pay_sub_amt(0,gi_pay_line);
    //x_j += (x->tb_newadd.qt_ffu6.sprintf("%08d", giA)+"|");         //�P�P�馩/������
    //x_j += (x->tb_newadd.am_ffu6.sprintf("%08d", giB)+"|");         //�P�P�馩/�����B


    //fetc_card(0,gi_fetc_line);
    //x_j += (x->tb_newadd.qt_ffu8.sprintf("%08d",giFetcCashAddCnt)+"|");  //�ƥΤK����  �{�� �[�Ȧ���
    //x_j += (x->tb_newadd.am_ffu8.sprintf("%08d",giFetcCashAddamt)+"|");  //�ƥΤK���B  �{�� �[�Ȫ��B
    //x_j += (x->tb_newadd.qt_ffu9.sprintf("%08d",giFetcCredAddCnt)+"|");  //�ƥΤE����  �H�Υd �[�Ȧ���
    //x_j += (x->tb_newadd.am_ffu9.sprintf("%08d",giFetcCredAddamt)+"|");  //�ƥΤE���B  �H�Υd �[�Ȫ��B
    //x_j += (x->tb_newadd.qt_ffu10.sprintf("%08d",0)+"|"); //giFetcSumCnt);     //�ƥΤQ����  FETC ���b����  �O�d
    //x_j += (x->tb_newadd.am_ffu10.sprintf("%08d",0)+"J"); //giFetcSumAmt);     //�ƥΤQ���B  FETC ���b���B  �O�d

    // 2012/09/25  Update
    if( SaleDataVer >= "2012090100")
      {
          for (int i=0; i<60; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //��w���B
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //��w�ɶ�
            }

      }
    else
      {
        for (int i=0; i<30; i++)
            {
              x_h += ("00000000|");  //�ɹs���B
              x_h += ("      |");    //�ɹs�ɶ�
            }         //"000000|"

        for (int i=0; i<30; i++)
            {
              x_h += x->tb_casher.tg_dropfld[i].am_drop+"|";   //("00000000|");  //��w���B
              x_h += x->tb_casher.tg_dropfld[i].tm_drop+"|";   //("      |");    //��w�ɶ�
            }
      }

    VdcInvSumdat(0,gi_rec_line);   //�Y���ʻP�P��i��
    sTmp.sprintf("%08d|",giB);     //+iSumRelDimAgn+iC0InvoAmt4VDCN);     �Y���ʻP�P��i�����|
    x_h +=sTmp;   //"00000000|";
    sTmp.sprintf("%08d|",giD);     //�Y���ʻP�P��i�ȧK�|
    x_h +=sTmp;


    //�o���I�����B
    VoicePayAmt(0,gi_tot_line);

    sTmp.sprintf("%08d|",giA/200); //�o���I���i��    �����o���i��
    x_h +=sTmp;
    sTmp.sprintf("%08d|",giA);     //�o���I�����B    �����o�����B
    x_h +=sTmp;

    SumWinInvoAmt=giA;               // 2017/08/15 for ���ȭ���Z���Ӫ�
    //SumCash41+=giA;                // 2012/10/08 for ���ȭ���Z���Ӫ�

     //�p��q�l§����l�B��
    //SumECP2Orher2Amt(0, gi_bil_line);
    sTmp.sprintf("%08d|",EcPrecentCnt);  //�q�l§��^��
    x_h +=sTmp;
    sTmp.sprintf("%08d|",EcPrecentAmt);  //�q�l§����B
    x_h +=sTmp;

    //iC0TotAmt, iC0GivUpAmt, iC0TotCount;         //�I�d����OgiD.�@�ogiE , �I�d����O�ȼ�
    //sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt+PreSal09_AmtTX , iC0GivUpAmt+R2R3PreSal09_AmtTX, iC0TotCount);
    sTmp.sprintf("%08d|%08d|%08d|",iC0TotAmt , iC0GivUpAmt, iC0TotCount);  //2017/06/04 Update   �N��,����O�P����B(��)   �N��,����O�@�o���B(��)
    x_h +=sTmp;


    AnsiString str_xdt_path, str_xrp_path;   //X��ơB�b���s����|

    SumBill78(0, gi_bil_line);
    writelog("���ʹ��f����:" + StrBillPay);

    AnsiString DDZ_1099, str_x_path;
    DDZ_1099=_StringSegment_EX(gsLast1099, "|", 7).SubString(3,3) ;

    if(DDZ_1099.SubString(1,2) < 1 || DDZ_1099.SubString(1,2) > 31 )
       DDZ_1099=str_date_z;

    //str_x_path = tif_ecr->ReadString("Z", "XPATH", "");  //���ȭ��b���ƥ����|
    str_x_path = "C:\\FTLPOS\\HISTORY\\" + DDZ_1099.SubString(1,2) + "\\" + "0" + DDZ_1099.SubString(3,1) + "\\";



    //�b���ɦW
    AnsiString str_new_name, sTmpPzData;

    /******/
    if ( Trim(str_x_path)=="" )
    {
        //�L���w���|�Ѩt����M�w

        str_x_path = "C:\\FTLPOS\\HISTORY\\";
        sTmpPzData=tif_ecr->ReadString("Z","PZ_DATE",FormatDateTime("yyyymmddhhnnss",Now())).Trim();

        if (sTmpPzData == NULL)
        {
            str_x_path += (str_date_time.SubString(7,2) + "\\01\\");
            writelog("�T�{���v�ؿ� 01:" + str_x_path +" ->str_date_time="+str_date_time);

        }
        else
        {
            str_x_path += ( sTmpPzData.SubString(7,2) + "\\0" );

            if (tif_ecr->ReadString("Z","NZNO_SEQ","").Length() < 1)
               { str_x_path += "1\\";
                writelog("�T�{���v�ؿ� 02:" + str_x_path  );
               }
            else
               {
                str_x_path += (tif_ecr->ReadString("Z","NZNO_SEQ","") + "\\");
                writelog("�T�{���v�ؿ� 03:" + str_x_path);
               }
        }

        tif_ecr->WriteString("Z","XPATH",str_x_path);
    }


    if (!DirectoryExists(str_x_path))
       {
        writelog("�إ߾��v�ؿ� :" + str_x_path);
        ForceDirectories(str_x_path);
       }
    str_new_name = (str_x_path + ExtractFileName(str_x_filename) );
    
    /*******/

    str_xdt_path = str_new_name;                     //�g�JINI�ܼ�

    //X����
    AnsiString xdt_1095_data;


    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%88s|\n",
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%79s|\n",  2012/09/18
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%61s|\n",  2013/03/15
    //xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%43s|\n",  2013/10/17   �I�d����O Update
    xdt_1095_data.sprintf("%s%s%s%s%s%s%s%s%s%s%16s|\n",
                          x_a.c_str(),
                          x_b.c_str(),
                          x_c.c_str(),
                          x_d.c_str(),
                          x_e.c_str(),
                          x_f.c_str(),
                          x_g.c_str(),
                          x_i.c_str(),
                          x_j.c_str(),
                          x_h.c_str(),
                          "0FFFFFFF"
                          );

    sTmp.printf("AUTOZDATA::AutoXData CreateReport:(%s) Len=%d",xdt_1095_data, xdt_1095_data.Length() );
    writelog(sTmp);

    //report = new REPORT();      //���ȭ��b��

    strcpy(report->char_filename, ChangeFileExt(str_x_filename,".xrp").c_str());


    // UReport.cpp  int REPORT::CreateReport(....)  2011/05/Mark
    // /***************  ���ͦ��ȭ���Z��  *.XRP                ***************/
    report->CreateReport('x', xdt_1095_data, StrBillPay, gchar_tencode, gchar_ecr_no,
                         iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, NULL, 1 );


     // ���ȭ���Z���Ӫ�
    //str_date_z          //ddn
    //str_tran_cnt    //�ثe����Ǹ�

   // AnsiString sTmp ;
   // sTmp.printf("CreateReport: ���ͦ��ȭ��b���ɮ� �N��X�p=%d",iam5_15217totrev);
   // writelog(sTmp);

    str_new_name = str_x_path;
    str_new_name +=   ExtractFileName(report->char_filename) ;

    DeleteFile(str_new_name);

    if (RenameFile(report->char_filename,str_new_name))       //�h��
        writelog("AUTOZDATA::AutoXData ���ͦ��ȭ��b��(XRP)�ɮ� " + str_new_name + " ->gchar_date_time="+gchar_date_time);
    else
        writelog("AUTOZDATA::AutoXData ���ͦ��ȭ��b��(XRP)�ɮץ��� " + str_new_name);


    str_xrp_path = str_new_name;        //�g�JINI�ܼ�

    //�g�JSAL��1097Type
    AnsiString str_rec_type,
               str_rec_len,
               str_tencode,
               str_ecr_no,
               str_sys_date,
               str_trans_cnt,
               str_z_cnt,
               str_cashier_no,
               str_start_trans,
               str_totsale_sum,
               str_xdt_no,
               str_ver;

    str_rec_type = "1097";
    str_rec_len  = "0106";

    str_tencode = gchar_tencode;   //����

    str_ecr_no.sprintf("%02d", _StrToInt(x->no_tm,"����"));              //����
    str_sys_date = (x->dt_end + x->tm_end);                              //����ɶ�

    int int_sum_transcnt = _StrToInt(str_tran_cnt,"�ثe�g�J1097����Ǹ�"); //�ثe�g�J97����Ǹ�
    str_trans_cnt.sprintf("%05d", int_sum_transcnt);

    AnsiString str_sum_tran;       //�������ȭ��b����Ǹ�
    //20031215
    if ((int_sum_transcnt + 1) > 99999)
    {
        int_sum_transcnt = 0;
    }

    str_sum_tran.sprintf("%05d", (int_sum_transcnt + 1) );

    //xxx
    //tif_tmp->WriteString("ECR","TRAN_CNT", str_sum_tran);

    str_ptran_cnt = str_sum_tran;


    //str_pxno_seq  //�W��x�b�ֿn����

    //str_nxno_seq  //�ثex����ĴX��

    // 2007/07/17
    str_z_cnt.sprintf("%02d", _StrToInt(str_nxno_seq,"��Z�Ǹ�"));
    //str_nz_cnt = tif_ecr->ReadString("Z", "NZ_CNT", "");          //�ثe�鵲�Ǹ�  ���餣�k�s

    str_cashier_no.sprintf("%-8s", (Trim(x->no_casher).c_str()));   //���ȭ��N��

    str_start_trans = _StringSegment_EX(str_x_first, "|", 7); //str_x_first.SubString(36,10); //�}�l����Ǹ��U���b���}�l�Ǹ�

    str_totsale_sum.sprintf("%8s", x->tb_trans.am_totsale.c_str()); //�����ھP�f���J
    str_xdt_no.sprintf("%010ld",x->no_seq.ToInt());

    str_ver = "FFFFFFFF";


    AnsiString str_trans1_5, CasherRptFile;;    //����Ǹ��e5�X
    //str_trans1_5 = _StringSegment_EX(str_sal_last_1099, "|", 7).SubString(1,5);
    str_trans1_5="00"+DDZ_1099;

    CasherRptFile=ChangeFileExt(str_xdt_path,".Crp");
    // sTmp=str_date_z.Trim()+str_tran_cnt.Trim();
    sTmp.sprintf("1041|0500|%-6s|%2s|%8s%6s|%05d|%-10s|%8s",
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        x->dt_end.c_str(),        //�������b���  ed_date_time.c_str(),
                        x->tm_end.c_str(),         //�������b���
                        SalAutoZcnt.ToInt(),                    // str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str()
                        );



    AnsiString str_01,str_99;
    str_01.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%-5s|%-8s| |%-6s|%-20.20s|",
                   "1001",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   SalAutoZcnt.ToInt(),                     //str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   str_cashier_no.c_str(),                //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   " ",
                   " ",
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );


    //str_99 = str_01;     for MTS:4127
    //str_99 = "1099" + str_99.Delete(1,4);

    str_99.sprintf("%4s|%4s|%-6s|%-2s|%14s|%05d|%-10s|%2s|%-10s|%2s|%-2s|%-10s|%-10s|%-8s|%-4s|%-20s|%-20s|%05d|%08d| |%-6s|%-20.20s|",
                   "1099",
                   "0197",
                   str_tencode.c_str(),
                   str_ecr_no.c_str(),
                   str_sys_date.c_str(),
                   SalAutoZcnt.ToInt(),                     //str_nz_cnt.ToInt(),
                   (str_trans1_5 + str_sum_tran).c_str(),
                   "Z0",
                   SaleDataVer.c_str(),                   //SALVER,  "2012022100",
                   "00",     //�h�f�Ϥ�
                   str_ecr_no.c_str(),                    //��l�������
                   (str_trans1_5 + str_sum_tran).c_str(), //��l����Ǹ�
                   "0000000000",                          //�h�f����Ǹ�
                   str_cashier_no.c_str(),                //���ȭ��N��
                   "00",                                  //�ȼh�N�X
                   " ",                                   //�|���s��
                   " ",                                   //�d��
                   0,
                   0,
                   gchar_orgtencode,                     //��l����
                   " "                                   //�O�d
                                       );


    AnsiString _str_tran_cnt;    //����Ǹ�

    _str_tran_cnt = (str_trans1_5 + str_sum_tran);

    //�g�J1097����Ǹ�
    AnsiString str_97;
    str_97.sprintf("%4s|%4s|%-6s|%2s|%14s|%05d|%-10s|%8s|%10s|000%8s|%10s|%8s|",
                        str_rec_type.c_str(),     //1097
                        str_rec_len.c_str(),
                        str_tencode.c_str(),
                        str_ecr_no.c_str(),
                        ed_date_time.c_str(),                 //str_sys_date.c_str(),
                        SalAutoZcnt.ToInt(),                 //str_nz_cnt.ToInt(),
                        (str_trans1_5 + str_sum_tran).c_str(),
                        str_cashier_no.c_str(),
                        str_start_trans.c_str(),
                        str_totsale_sum.c_str(),
                        str_xdt_no.c_str(),
                        str_ver.c_str()
                        );

    String str_sal;
    //str_sal = str_01 +  str_97 + str_99;
    str_sal = str_97;

    FILE *fxdata;

    if ((fxdata = fopen(str_xdt_path.c_str(), "w")) != NULL)
    {

        fprintf(fxdata,"%s\n",str_01.c_str());
        //fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%79s|\n",
        //fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%61s|\n",
        //fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%43s|\n",   2013/10/17    �I�d����O Update
        fprintf(fxdata,"%s%s%s%s%s%s%s%s%s%s%16s|\n",
                            x_a.c_str(),
                            x_b.c_str(),
                            x_c.c_str(),
                            x_d.c_str(),
                            x_e.c_str(),
                            x_f.c_str(),
                            x_g.c_str(),
                            x_i.c_str(),
                            x_j.c_str(),
                            x_h.c_str(),
                            "0FFFFFFF"
                            );

        fclose(fxdata);
    }
    else
    {
        writelog("AUTOZDATA::AutoXData �L�k���ͦ��ȭ�DATA " + str_xdt_path);
        return "";
    }


    /***************  ���ͦ��ȭ���Z���Ӫ�  ���ȭ��汵�Z���Ӫ� ***************/
    AnsiString SumCasherRptData;
    SumCasherRpt(AUTOZ,0, SumCash41,SumPresent41,SumCpn41, iTotOt2Rtn, VisuilCpnAmt, SumWinInvoAmt, sTmp, str_nxno_seq,
                 CasherRptFile, str_xdt_path, str_01, str_97, str_99, SumCasherRptData, iamt_food, iqt_food, 1);

    xdt_1095_data+=SumCasherRptData;
    sTmp.printf("AUTOZDATA::AutoXData ���ͦ��ȭ���Z���Ӫ�:%s",CasherRptFile);
    writelog(sTmp );

    /*************************************************************************/
    // �۰ʥ�Z�����ʵL���ֶi����Ǹ�
    //if ((int_sum_transcnt + 2) > 99999)
    //        int_sum_transcnt = 1;
    //else
    //     int_sum_transcnt +=2;
    //str_sum_tran.sprintf("%05d", int_sum_transcnt );
   // tif_tmp->WriteString("ECR", "TRAN_CNT", str_sum_tran);


    AnsiString sTtmpPTRAN_CNT;  //2012/06/04
    sTtmpPTRAN_CNT=str_trans1_5.SubString(3,3) + str_ptran_cnt;
    //tif_ecr->WriteString("X","PTRAN_CNT", sTtmpPTRAN_CNT);

    //tif_tmp->WriteString("X","PTRAN_CNT", (str_date_z + str_ptran_cnt));

    //�۰ʤ鵲�W������,��ʤ鵲�k0 , �ҥH�� �۰ʤ鵲/��ʤ鵲 �B�z
    //tif_ecr->WriteString("X","PXNO_SEQ",   str_nxno_seq);

    tif_ecr->WriteString("X","SX_DATE",   (x->dt_begin + x->tm_begin));
    //tif_tmp->WriteString("X","EX_DATE",   (x->dt_end + x->tm_end));     //�����U����Z��(�e�����b����ɶ�) �۰ʥ�Z������

    tif_ecr->WriteString("ECR","tot_open_x", "0");
    tif_ecr->WriteString("ECR","x_pay_cnt" ,  "0");

    tif_ecr->WriteString("X","XFILEDTTM", str_sys_date);    // XDT ��DTTM
    tif_ecr->WriteString("X","XFILENAME", str_xdt_path);
    tif_ecr->WriteString("X","XRPTNAME" , str_xrp_path);
    tif_ecr->WriteString("X","CHRRPTNAME" , CasherRptFile);
    
    //tif_tmp->WriteString("X","SALESLOGINDTTM" , sSalesLoginDttm );

    //delete x,tif_ecr;

    sTmp.printf("AUTOZDATA::AutoXData ��Z����:�g�J������Z����Ǹ� tif_tmp->X->PTRAN_CNT=%s, AutoZ_cnt=%s",sTtmpPTRAN_CNT.c_str(), AutoZ_cnt.c_str() );
    writelog(sTmp );

    return( xdt_1095_data);
}




////////////////////////////////////////////////////////////////////////////////

void __fastcall AUTOZDATA::mtn_dat_folder() //���@DAT�U����30�餺
{
    String Dir;
    String FileName;
    TSearchRec SearchRec;
    int iAttributes = 0;
    iAttributes |= faAnyFile * true;

    AnsiString str_dat_path = "C:\\Abacus\\download\\dat\\";
    Dir = str_dat_path;

    TStringList *TempList = new TStringList;
    try
    {

        TempList->Clear();

        if (FindFirst(Dir+"*.tm", iAttributes, SearchRec) == 0)
        {
            do
            {
                if ((SearchRec.Attr & iAttributes) == SearchRec.Attr)
                {
                    TempList->Add(SearchRec.Name);
                }
            } while (FindNext(SearchRec) == 0);

            FindClose(SearchRec);
        }

        AnsiString str_dd_flag = FormatDateTime("yymmdd",Now()-30);
        AnsiString str_dd_file;

        for ( int i = 0; i <= (TempList->Count-1); i++)
        {
            FileName = TempList->Strings[i];

            if (FileName.Length() < 13)
            {
                TempList->Delete(i);
                i = 0;
                continue;
            }

            str_dd_file = FileName.SubString(3,6);

            if (str_dd_file.AnsiCompare(str_dd_flag) < 0)
            {
                FileName = (Dir + TempList->Strings[i]);
                DeleteFile(FileName);
                writelog("AUTOZDATA:���@DAT�R���ɮ� " + FileName);
            }
        }
    }
    __finally
    {
        delete TempList; // destroy the list object
    }

}

void __fastcall AUTOZDATA::del_list_file(TStringList *list, AnsiString path)
{
    AnsiString str_sal_00;

    for (int i=0; i<list->Count; i++)
    {
        str_sal_00 = (path + list->Strings[i]);
        DeleteFile(str_sal_00);
        writelog("AUTOZDATA:�T�Q�ѥؿ����@�R���ɮ� " + str_sal_00);
    }

    list->Clear();
}

AnsiString __fastcall AUTOZDATA::write_xpath(AnsiString FileName)
{
    AnsiString str_file_dd,                //���
               str_file_num,               //����
               str_bak_path;                //�ƥ����|


    AnsiString str_his_folder = "C:\\FTLPOS\\HISTORY\\";
    AnsiString str_abs_file = ExtractFileName(FileName);

    str_file_dd  = str_abs_file.SubString(7,2);
    str_file_num = str_abs_file.SubString(9,2);

    //�ƥ����|
    if ( (!str_file_dd.IsEmpty()) && (!str_file_num.IsEmpty()) )
        str_bak_path = ( str_his_folder + str_file_dd + "\\" + str_file_num + "\\" );
    else
        str_bak_path = str_his_folder;

    //�إߥؿ�
    if (!DirectoryExists(str_bak_path))
        ForceDirectories(str_bak_path);

    return str_bak_path;

}