//---------------------------------------------------------------------------

#include <vcl.h>
#include "UReport.h"

//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)

//�����b�B�z�{��
// �����P�P�馩���B        Lu 2018/06/13
int __fastcall REPORT::Dept(AnsiString &ZData, char *ptr_tencode, char *ptr_ecr_no,  int XR,
                            TStringList *tsl, AnsiString s1093 )
{

    AnsiString str_nz_cnt;             //����Ǹ�
    AnsiString str_tencode;           //����
    AnsiString str_ecr_no;             //����
    AnsiString str_bg_noclose;

    int int_dept = 88;                //�e�@�ӳ����bZDATA���}�l��l



    //if( XR==21 || XR==22)
       str_bg_noclose.sprintf("%08d", _StrToInt(_StringSegment_EX(ZData, "|", 7), "ZData"));


    //�N�����x�s��TStringList��K�B�z
    TStringList *tsl_zdata = new TStringList;

    AnsiString sDpt;
    for (int i=0; i<20; i++)
    {
        int_dept++;    //89 �}�l
        sDpt=_StringSegment_EX(ZData, "|", int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, "|", int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, "|", int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, "|", int_dept);

        tsl_zdata->Add(sDpt);        //�����C�@�q��Ƭ�32

    }

    if (XR==1)
    {
        str_nz_cnt = "00000";
    }
    else
    {
        str_nz_cnt=_StringSegment_EX(ZData, "|", 6); // 2013/07/22
        /***
        TIniFile *tif_ecr;
        try
        {
            tif_ecr = new TIniFile(XZDATA_INI);
            str_nz_cnt = tif_ecr->ReadString("Z","NZ_CNT","");     //�ֿn���b���ƧǸ�

            if (str_nz_cnt.Length() < 5)
                str_nz_cnt = "00001";
        }
        __finally
        {
            delete tif_ecr;
        }
        ***/

    }



    str_tencode = ptr_tencode;                  //����
    str_ecr_no  = ptr_ecr_no;                    //����

    //�����b���c
    typedef struct
    {
        int qty;
        int amt;
        int rqty;
        int ramt;

    }sut_dept;


    sut_dept *dept;

    try
    {
        dept = new sut_dept[20];

        //�x�s��TStringList�������b�g�J���c��
        for (int i=0; i<20; i++)
        {
            dept[i].qty = _StrToInt(tsl_zdata->Strings[i].SubString(1,8), tsl_zdata->Strings[i]);
            dept[i].amt = _StrToInt(tsl_zdata->Strings[i].SubString(9,8), tsl_zdata->Strings[i]);
            dept[i].rqty = _StrToInt(tsl_zdata->Strings[i].SubString(17,8), tsl_zdata->Strings[i]);
            dept[i].ramt = _StrToInt(tsl_zdata->Strings[i].SubString(25,8), tsl_zdata->Strings[i]);
        }
    }
    __finally
    {
        delete tsl_zdata;
    }



    AnsiString qtyamt,  //�ƶq�[�`�ξP����B�[�`
               ramt;    //�~���B�h�f/���L���B�[�`
    AnsiString   StrLine;

    StrLine="";

    if( XR==21 || XR==22)
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
    else
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_nz_cnt,
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("�Ǹ��G" + str_nz_cnt));
    //to_ptr(("�����G" + str_tencode + "     " + "�����G" + str_ecr_no));
    to_ptr("");

    if (XR==21) //21:ñ�� 22:ñ�h
        to_ptr("       �L�I�H�������b��(ñ��)   ");
    else if (XR==22) //21:ñ�� 22:ñ�h
        to_ptr("       �L�I�H�������b��(ñ�h)   ");
    else
        to_ptr("               �����b��        ");

    to_ptr("");


    //��X���ɮ�
    AnsiString sCount;
    int int_a = 0, int_b = 0;

  //StrLine.sprintf("����   �P�� �P����B      �h�� �h�f���B");
    StrLine.sprintf("����   �P�� �P����B    �h�� �h�f���B");
    to_ptr(StrLine);
    for (int i=0; i<20; i++)
    {
        int_a -= dept[i].rqty;
        int_b -= dept[i].ramt;

        qtyamt.sprintf("%5d %8d", dept[i].qty,dept[i].amt);            //"%6d %8d"
        ramt.sprintf("%5d %8d", int_a, int_b);                         //"%6d %8d"
        sCount.sprintf("%02d", (i + 1));

      //StrLine.sprintf("%3s   %s     %s",sCount, qtyamt, ramt);
        StrLine.sprintf("%3s   %s  %s",sCount, qtyamt, ramt);
        to_ptr(StrLine);


        //to_ptr(("����" + sCount));
        //to_ptr(qtyamt);
        //to_ptr(ramt);
        //to_ptr("");

        int_a = 0;
        int_b = 0;
    }

    //to_ptr("�����X�p");

    int_a -= _StrToInt(_StringSegment_EX(ZData, "|", 43));
    int_b -= _StrToInt(_StringSegment_EX(ZData, "|", 44));

    qtyamt.sprintf("%5d %8d", _StrToInt(_StringSegment_EX(ZData, "|", 41)),       //"%6d %8d"
                              _StrToInt(_StringSegment_EX(ZData, "|", 42))   );   //"%6d %8d"
    ramt.sprintf("%5d %8d", int_a, int_b);

    StrLine.sprintf("--------------------------------------");
    to_ptr(StrLine);

    StrLine.sprintf("�X�p %s   %s", qtyamt, ramt);
    to_ptr(StrLine);

    //to_ptr("\n\n\n\n");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //�����X�p�[�`
    //to_ptr("�����X�p");
    //to_ptr(qtyamt);
    //to_ptr(ramt);

    /////////////////////////////////  �����P�P����  ////////////////////////////////////
    int int_a_Tot,  int_b_Tot;
    if( s1093 != NULL &&  s1093.Trim() != "")
      {
       int_a = 0; int_b = 0; int_a_Tot=0;  int_b_Tot =0;

       to_ptr("");

       if( XR==21 || XR==22)
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
       else
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_nz_cnt,
                                           str_tencode,str_ecr_no);
       to_ptr(StrLine);

       if (XR==21) //21:ñ�� 22:ñ�h
            to_ptr("        �����P�P����(ñ��)     ");
       else if (XR==22) //21:ñ�� 22:ñ�h
            to_ptr("        �����P�P����(ñ�h)     ");
       else
            to_ptr("           �����P�P����     ");

       to_ptr("");

       StrLine.sprintf("����   �P��P�P���B   �h�f�P�P���B");
       to_ptr(StrLine);
       for (int i=1; i<21; i++)
         {
             int_a = _StrToInt(_StringSegment_EX(s1093, "|", (2*i)-1 ) );
             int_a_Tot += int_a  ;  // total
             int_b = _StrToInt(_StringSegment_EX(s1093, "|", (2*i) ) );
             int_b_Tot += int_b  ;  // total

             qtyamt.sprintf("%8d", int_a);
             ramt.sprintf("%8d", int_b);
             sCount.sprintf("%02d", (i));

             StrLine.sprintf("%3s   %s      %s",sCount, qtyamt, ramt);
             to_ptr(StrLine);
             int_a = 0;
             int_b = 0;
         }

      ramt.sprintf("%8d      %8d",  int_a_Tot, int_b_Tot );

      StrLine.sprintf("--------------------------------------");
      to_ptr(StrLine);

      StrLine.sprintf("�X�p  %s   ", ramt);
      to_ptr(StrLine);

      //to_ptr("\n\n\n\n");
      to_ptr("");
      to_ptr("");
      to_ptr("");
      to_ptr("");
      // 2019/11/06 For SCO
      //StrLine.sprintf("\x1b\x69");
      //to_ptr(StrLine);
    }
    ///////////////////////�N��ӫ~���B����//////////////////////////////////

   if( ( XR==21 || XR==22) && tsl )
     {
       int int_amt = 0;                        //�N����B�[�`

       //�浧�O���B�ӫ~�W�١B�P����B
       AnsiString str_record, str_bar_name, str_item_amt;
        // 2019/11/06 For SCO
       to_ptr("---------------------------------------");
       StrLine="";
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
       to_ptr(StrLine);

       to_ptr("");
       to_ptr("           �N��ӫ~���B����     ");
       to_ptr("");

       StrLine.sprintf("��%s�ӫ~�W��%17s�P����B"," ", " ");
       to_ptr(StrLine);
       int int_space = 1;
       for (int i=0; i<tsl->Count; i++)
       {
        str_record = tsl->Strings[i];
        str_bar_name = _StringSegment(str_record, "|", 3);
        str_item_amt = _StringSegment(str_record, "|", 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //�R�����Ϊť�
        {
            int_space = str_bar_name.Pos("�@");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //�R���b�Ϊť�
        {
            int_space = str_bar_name.Pos(" ");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }
        }

        str_record.sprintf("%02d.%-26s%7s",i+1 ,str_bar_name.SubString(1,26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);
 
        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
       }


    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    �X�p%16d"," ",int_amt);
    str_record.sprintf("�X�p%15s %16d"," ",int_amt);
    to_ptr(str_record);

    // 2019/11/06 For SCO
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");

    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    //to_ptr("Save");
    //return 0;

    }

    ///////////////////////////////////////////////////////////////////////////


    //�g�J�ɮ�
    to_ptr("Save");

    delete [] dept;
    return 0;
}




//�Y�ɳs�u�ӫ~���p�b
int __fastcall REPORT::INQReport( char *ptr_tencode, char *ptr_ecr_no,
                                 INQTBL *InqTbl)
{

    AnsiString str_nz_cnt;             //����Ǹ�
    AnsiString str_tencode;           //����
    AnsiString str_ecr_no;             //����
      

    str_tencode = ptr_tencode;                  //����
    str_ecr_no  = ptr_ecr_no;                    //����

    TIniFile *tif_ecr;
    try
        {
            tif_ecr = new TIniFile(XZDATA_INI);
            str_nz_cnt = tif_ecr->ReadString("Z","NZ_CNT","");     //�ֿn���b���ƧǸ�

            if (str_nz_cnt.Length() < 5)
                str_nz_cnt = "00001";
        }
    __finally
        {
            delete tif_ecr;
        }
   
    AnsiString qtyamt,  //�ƶq�[�`
               ramt;    //���B�[�`
   
    to_ptr(gchar_date_format);
    to_ptr(("�Ǹ��G" + str_nz_cnt));
    to_ptr(("�����G" + str_tencode + "    " + "�����G" + str_ecr_no));
    to_ptr("");
 // to_ptr("       �����b��        ");
    to_ptr(" �Y�ɳs�u�ӫ~����έp�� ");
    to_ptr("");

    //��X���ɮ�
    AnsiString sCount;
    int int_a = 0, int_b = 0;

    for (int i=1; i<20; i++)
    {
        int_a = InqTbl[i].qt_InqSale;
        int_b = InqTbl[i].am_InqSale;
        if( int_a > 0 || int_b > 0)
          {
           qtyamt.sprintf("%6d%18d", int_a,int_b);
           //ramt.sprintf("%6d%18d", int_a, int_b);
           sCount.sprintf("%02d", i);

           to_ptr(("�s�u�ӫ~  " + sCount));
           to_ptr(qtyamt);
           // to_ptr(ramt);
           to_ptr("");
          }
        int_a = 0;
        int_b = 0;
    }

    //�����X�p�[�`
    //to_ptr(qtyamt);
    //to_ptr(ramt);

    //�g�J�ɮ�
    to_ptr("Save");
    return 0;
}


// int __fastcall REPORT::X(AnsiString &XData, AnsiString sPayStr, char *ptr_tencode   )
//XData: 1095 ���ȭ��b��q����
int __fastcall REPORT::XDTReport(AnsiString &XData, AnsiString sPayStr, char *ptr_tencode,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int AutoZflg )
{

    AnsiString str_tencode, StrLine, NowDDHHmm, PreDDHHmm;
    str_tencode = ptr_tencode;

    StrLine="";

    StrLine.sprintf("%s�Ǹ�:%5s ����:%s ��:%s",gchar_date_format,
                                               _StringSegment_EX(XData, "|", 8),
                                               str_tencode,
                                               _StringSegment_EX(XData, "|", 4) );
    to_ptr(StrLine);
    //to_ptr(gchar_date_format);
    //to_ptr(("�Ǹ��G" + XData.SubString(7,2)));
    //to_ptr(("�����G" + str_tencode + "     " + "�����G" + XData.SubString(1,1)));
    to_ptr("");

    to_ptr("             ���ȭ����Ӫ�        ");
    to_ptr("");

    to_ptr( ("���ȭ� NO " + _StringSegment_EX(XData, "|", 9)  ) );

    //���b���+���b�ɶ�
    NowDDHHmm=_StringSegment_EX(XData, "|", 12).SubString(7,2)+_StringSegment_EX(XData, "|", 13).SubString(1,4);
    //�e�����b���+�e�����b�ɶ�
    PreDDHHmm=_StringSegment_EX(XData, "|", 10).SubString(7,2)+_StringSegment_EX(XData, "|", 11).SubString(1,4);

    //StrLine.sprintf("���b�ɶ�    �}�l %s ���� %s",
    StrLine.sprintf("���b�ɶ�    ���� %s �e�� %s",
                     ToDateFormat( NowDDHHmm ).c_str(),
                     ToDateFormat( PreDDHHmm ).c_str() );
    to_ptr(StrLine);
    //to_ptr(("���ȭ� NO" + InsertSpace(XData.SubString(2,5),9)) );
    //to_ptr(("�}�l�ɶ�" + InsertSpace(ToDateFormat(XData.SubString(15,6)),8)));
    //to_ptr(("�����ɶ�" + InsertSpace(ToDateFormat(XData.SubString(29,6)),8)));
    to_ptr("");


    StrLine.sprintf("����~��    %8s  ����ӫȼ�%7s",
                     GetNumber(_StringSegment_EX(XData, "|", 15) ).c_str(),
                     GetNumber(_StringSegment_EX(XData, "|", 14) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    //to_ptr(("�N���ӫ~�ȼ�" + InsertSpace(GetNumber(XData.SubString(3293,8)),12)));

    StrLine.sprintf("�o�����b���B(GS)%14s %8s"," ", GetNumber(_StringSegment_EX(XData, "|", 20)).c_str() );
    to_ptr(StrLine);
    //to_ptr("�o�����b���B(GS)");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(69,8)),0));

    //  FFU4  3293
    //  FFU4  3301 ���B
    StrLine.sprintf("���}�o�����B%18s %8s"," ", GetNumber(_StringSegment_EX(XData, "|", 77) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("���}�o�����B");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3301,8)),0));

    StrLine.sprintf("�~,�h/���X�p%18s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 24) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("�~�B�h/���X�p");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(101,8)),0));

    StrLine.sprintf("���}�o���~,�h/���X�p%10s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 78) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("���}�o���~�B�h/���X�p");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3189,8)),0));   //�[�Ⱦ� ���b���B, ���}�o�����B�~�h��

    StrLine.sprintf("��ھP�f���J(NS)%14s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 25) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�N��,����O�ӫȼ�%13s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 212) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B%11s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 210) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B%11s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 211) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��ӫ~�������B   %11s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 44) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("���d���X�p%20s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 81) ).c_str() );
    //to_ptr(StrLine);

    StrLine.sprintf("�N���X�p%11s %8s   %8s"," ", GetNumber(_StringSegment_EX(XData, "|", 72) ).c_str(),
                                                  GetNumber(_StringSegment_EX(XData, "|", 73) ).c_str() );
    to_ptr(StrLine);


    //StrLine.sprintf("�N��X�p%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    StrLine.sprintf("�N��ӫ~�X�p%7s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    to_ptr(StrLine);

    StrLine.sprintf("�Y���ʥN��X�p%16s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 82) ).c_str() );
    to_ptr(StrLine);

    //�Y���ʦX�p
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XData, "|", 204) );
    iVDCNx=_StrToInt( _StringSegment_EX(XData, "|", 205) );
    //StrLine.sprintf("�Y���ʻP�P��i��%14s %8s"," ",GetNumber( _StringSegment_EX(XData, "|", 204) ).c_str() );
    StrLine.sprintf("�Y���ʻP�P��i��%14s %8d"," ",iVDCTx+iVDCNx );
    to_ptr(StrLine);

    StrLine.sprintf("����O�X�p%20s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 81)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�N�I�X�p%11s %8d   %8d"," ", iqt5_18219totrev,iam5_18219totrev );
    to_ptr(StrLine);


    StrLine.sprintf("����     %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 41) ).c_str(),
                                             "§��",
                                              GetNumber(_StringSegment_EX(XData, "|", 31) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("��l�B�� %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 39) ).c_str(),
    //                                         "�����",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 35) ).c_str()  );
    StrLine.sprintf("��l�B�� %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 39) ).c_str(),
                                             "§���d",
                                              GetNumber(_StringSegment_EX(XData, "|", 209) ).c_str()  );
    to_ptr(StrLine);

    //StrLine.sprintf("��d�X�p %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 69) ).c_str(),
    //                                         "�N�{���X�p",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 37) ).c_str()  );
    StrLine.sprintf("��d�X�p %8s   %10s %8s",GetNumber(_StringSegment_EX(XData, "|", 69) ).c_str(),
                                             "�����",
                                              GetNumber(_StringSegment_EX(XData, "|", 35) ).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("�N�{���X�p%20s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 37) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("�{���X�p%22s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 42) ).c_str() );
    //to_ptr(StrLine);


    //20151208 ���a�ݨD�ܧ�A�վ�Z�b���w��ܦ�m
    StrLine.sprintf("���w�h��%11s     %15s"," ",
                                              //GetNumber(_StringSegment_EX(XData, "|", 66) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 67) ).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XData, "|", 206) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XData, "|", 207) );

    StrLine.sprintf("�o���I�� %8d   %10s %8s", iPayinvoamt, "�{���X�p", GetNumber(_StringSegment_EX(XData, "|", 42) ).c_str() );
    to_ptr(StrLine);

    TStringList *tsl_drop;

    AnsiString sDrop;
    int iDropCnt=0;
    try
    {
        tsl_drop = new TStringList;

        int int_position = 144;    //LOG����w�}�l��m


        for (int i=0; i<30; i++)
        {
            sDrop=_StringSegment_EX(XData, "|", int_position)+_StringSegment_EX(XData, "|", int_position+1);
            tsl_drop->Add(sDrop);
            int_position += 2;
        }

        AnsiString str_drop_time,str_drop_money,str_drop_data;
        int int_drop_money = 0;

        to_ptr("��w");

        for (int i=0; i<30; i++)
        {
            str_drop_time = tsl_drop->Strings[i].SubString(9,6);

            if (str_drop_time == "000000" || Trim(str_drop_time)=="" )
            {
                break;
            }
            else
            {
                str_drop_time = ToTimeFormat(str_drop_time);
                int_drop_money = _StrToInt(tsl_drop->Strings[i].SubString(1,8),tsl_drop->Strings[i]);
                str_drop_money = InsertSpace(IntToStr(int_drop_money),12);

                if (i < 9)
                    str_drop_data = ("     " + IntToStr(i + 1) + " " + str_drop_time + "     " + str_drop_money);
                else
                    str_drop_data = ("    " + IntToStr(i + 1) + " " + str_drop_time + "     " + str_drop_money);


                to_ptr(str_drop_data);
                iDropCnt++;
            }
        }
    }
    __finally
    {
        delete tsl_drop;
    }

    //��w����
    //��w�X�p���B
    //StrLine.sprintf("�X�p%15s %8s   %8s"," ",GetNumber(XData.SubString(3077,8)).c_str(),
    //                                             GetNumber(XData.SubString(3085,8)).c_str() );
    StrLine.sprintf("�X�p%8s         %8s",GetNumber(_StringSegment_EX(XData, "|", 47) ).c_str(),
                                          GetNumber(_StringSegment_EX(XData, "|", 48) ).c_str() );
    to_ptr(StrLine);
    to_ptr("");


    StrLine.sprintf("�󥿦X�p%11s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 49) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 50) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("��������X�p%7s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 51) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 52) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�馩%15s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 56) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 57) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�ӫ~�զX�馩/��%4s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 58) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 59) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�[�Ⱦ� �}/��d%5s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 60) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 61) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�[�Ⱦ��[��%9s %8s   %8s"," ",
                                         GetNumber(_StringSegment_EX(XData, "|", 62) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, "|", 63) ).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XData, "|", 206) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XData, "|", 207) );
    //StrLine.sprintf("�o���I��%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);

    StrLine.sprintf("�}���d%13s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 55) ).c_str() );
    to_ptr(StrLine);

    //  FFU4  3293
    //  FFU4  3301

    //  FFU5  3308
    //  FFU5  3317
    //  FFU6  3325
    //  FFU6  3333

    // 2005/07/01
    //to_ptr( ("�[�Ⱦ� ���b" + InsertSpace(GetNumber(XData.SubString(3181,8)),11)) );
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3189,8)),0));

    // 2005/07/01
    //to_ptr( ("�[�Ⱦ� �}�d/�[�Ȩ���" + InsertSpace(GetNumber(XData.SubString(3341,8)),20)) );  //ffu7
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3349,8)),0));   //ffu7

    StrLine.sprintf("e�q�d�{���[��%6s %8s   %8s"," ",
                                              GetNumber(_StringSegment_EX(XData, "|", 64) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 65) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("e�q�d�H�Υd�[��%4s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 66) ).c_str(),
    //                                          GetNumber(_StringSegment_EX(XData, "|", 67) ).c_str() );
    //to_ptr(StrLine);




   // to_ptr( ("e�q�d���b" + InsertSpace(GetNumber(XData.SubString(3389,8)),9)) );        //ffu10
   // to_ptr(InsertSpace(GetNumber(XData.SubString(3397,8)),0));                         //ffu10

    StrLine.sprintf("�����n��%11s %8s   %8s"," ",
                                              GetNumber(_StringSegment_EX(XData, "|", 53) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 54) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�o��%15s %8s   %8s"," ",
                                              GetNumber(_StringSegment_EX(XData, "|", 79) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, "|", 80) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("���B���o��%9s %8s"," ",GetNumber(_StringSegment_EX(XData, "|", 76) ).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("�����o��%20s %10s"," ",_StringSegment_EX(XData, "|", 17).c_str()  );
    to_ptr(StrLine);


    int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
    AnsiString sCpnCnt, sCpnAmt;

    //icpn77cnt=_StrToInt(sPayStr.SubString(225,8));
    //icpn77amt=_StrToInt(sPayStr.SubString(233,8));
    //icpn78cnt=_StrToInt(sPayStr.SubString(241,8));
    //icpn78amt=_StrToInt(sPayStr.SubString(249,8));
   // sCpnCnt.sprintf("%8d",icpn77cnt+icpn78cnt);
    //sCpnAmt.sprintf("%8d",icpn77amt+icpn78amt);

    // to_ptr( ("���f�馸��" + InsertSpace(GetNumber(sCpnCnt),10)));
    //if( icpn77amt+icpn78amt > 0)
    //  {
   //    to_ptr("");
   //    to_ptr( ("���f����B" + InsertSpace(GetNumber(sCpnAmt),10)));
    //  }
      
    to_ptr("");
    to_ptr("");
    to_ptr("");

    //to_ptr( ("        ***     " + _StringSegment_EX(XData, "|", 74)+"."+ _StringSegment_EX(XData, "|", 75)+ "     ***") );
    to_ptr( ("        ***     " + _StringSegment_EX(XData, "|", 74)+ _StringSegment_EX(XData, "|", 75)+ "     ***") );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");

    return 0;
}

//1096
// int __fastcall REPORT::Z(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no)
int __fastcall REPORT::ZDTReport(AnsiString &Zdata, char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int rpt_type )
{

    AnsiString str_bg_noclose;         //�Ǹ�

    if( rpt_type==21 || rpt_type==22)
       str_bg_noclose.sprintf("%08d", _StrToInt(_StringSegment_EX(Zdata, "|", 7), "ZData"));
    else
      str_bg_noclose.sprintf("%05d", _StrToInt(_StringSegment_EX(Zdata, "|", 6), "ZData"));

    AnsiString str_tencode, str_ecr_no, StrLine, sTmp, sStDDHHMM, sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;

    StrLine="";
    if( rpt_type==21 || rpt_type==22)
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
    else
       StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,str_bg_noclose,
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("�Ǹ��G" + str_bg_noclose));
    //to_ptr(("�����G" + str_tencode + "     �����G" + str_ecr_no));
    to_ptr("");


    //21:ñ�� 22:ñ�h
    if( rpt_type ==21)
        to_ptr("          �L�I�H���b��(ñ��)     ");
    else if( rpt_type ==22)
        to_ptr("          �L�I�H���b��(ñ�h)     ");
    else
        to_ptr("                Z�b��        ");

    to_ptr("");

    sTmp=_StringSegment_EX(Zdata, "|", 10)+_StringSegment_EX(Zdata, "|", 11);
    sStDDHHMM=sTmp.SubString(7,6); //����
    sTmp=_StringSegment_EX(Zdata, "|", 8)+_StringSegment_EX(Zdata, "|", 9);
    sPtDDHHMM=sTmp.SubString(7,6); //�e��

    StrLine.sprintf("���b�ɶ�  ���� %s �e�� %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("�������~��%7s ����ӫȼ�%7s",
                     GetNumber(_StringSegment_EX(Zdata, "|", 13) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, "|", 12) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    //to_ptr( ("�N���ӫ~�ȼ�" + InsertSpace(GetNumber(Zdata.SubString(1887,8)),12)) );
    //to_ptr("");

    StrLine.sprintf("���鵲�b�֭p���B%13s%8s"," ", GetNumber(_StringSegment_EX(Zdata, "|", 14) ).c_str() );
    to_ptr(StrLine);
    //to_ptr("���鵲�b�֭p���B");
    //to_ptr(InsertSpace(GetNumber(Zdata.SubString(51,8)),0));


    StrLine.sprintf("�o�����b���B%17s%8s"," ", GetNumber(_StringSegment_EX(Zdata, "|", 19) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("���}�o�����B%17s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 82) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("�o�����b   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(Zdata, "|", 17) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, "|", 18) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�~,�h/��   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(Zdata, "|", 21) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, "|", 22) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�~,�h/���X�p%17s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 23) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("���}�o���~,�h/���X�p%9s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 83) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�����ھP�f���J%13s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 24) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�N��,����O�ӫȼ�%12s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(�K)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(��)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(�K)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(��)%6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 176) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��ӫ~�������B       %6s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 47) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("���d���X�p%20s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 86)).c_str() );
    //to_ptr(StrLine);

    StrLine.sprintf("�N���X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 77)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 78)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("�N��X�p%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    StrLine.sprintf("�N��ӫ~�X�p%6s%8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    to_ptr(StrLine);

    StrLine.sprintf("�Y���ʥN��X�p%15s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 87)).c_str() );
    to_ptr(StrLine);

    //�Y���ʦX�p
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(Zdata, "|", 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(Zdata, "|", 170) );
    //StrLine.sprintf("�Y���ʻP�P��i��%14s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 169) ).c_str() );
    StrLine.sprintf("�Y���ʻP�P��i��%13s%8d"," ",iVDCTx+iVDCNx );
    to_ptr(StrLine);

    StrLine.sprintf("����O�X�p%19s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 86)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�N�I�X�p%10s%8d   %8d"," ", iqt5_18219totrev,iam5_18219totrev );
    to_ptr(StrLine);


    StrLine.sprintf("����    %8s  %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 40)).c_str(),
                                             "§��",
                                              GetNumber(_StringSegment_EX(Zdata, "|", 30)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("��l�B�� %8s   %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 38)).c_str(),
    //                                         "�����",
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 34)).c_str()  );
    StrLine.sprintf("��l�B��%8s  %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 38)).c_str(),
                                             "§���d",
                                              GetNumber(_StringSegment_EX(Zdata, "|", 174)).c_str()  );
    to_ptr(StrLine);



    //StrLine.sprintf("��d�X�p %8s   %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 74)).c_str(),
    //                                         "�N�{���X�p",
    //                                          GetNumber(_StringSegment_EX(Zdata, "|", 36)).c_str()  );
    StrLine.sprintf("��d�X�p%8s  %10s %8s",GetNumber(_StringSegment_EX(Zdata, "|", 74)).c_str(),
                                             "�����",
                                              GetNumber(_StringSegment_EX(Zdata, "|", 34)).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("�N�{���X�p%19s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 36)).c_str() );
    to_ptr(StrLine);


    //to_ptr("���O��X�p");
    //to_ptr(InsertSpace(GetNumber(Zdata.SubString(1871,8)),0));

    //StrLine.sprintf("�{���X�p%22s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 45)).c_str() );
    //to_ptr(StrLine);


    //20151208 ���a�ݨD�ܧ�A�վ�Z�b���w��ܦ�m
    StrLine.sprintf("���w�h��%10s     %14s"," ",
                                             //GetNumber(_StringSegment_EX(Zdata, "|", 71)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 72)).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, "|", 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, "|", 172) );

    StrLine.sprintf("�o���I��%8d  %10s %8s", iPayinvoamt, "�{���X�p", GetNumber(_StringSegment_EX(Zdata, "|", 45)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("��w�X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 50)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 51)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�󥿦X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 52)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 53)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("��������X�p%6s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 54)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 55)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�馩(���|)%8s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 61)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 62)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�ӫ~�զX�馩/��%3s%8s   %8s"," ",
                                                  GetNumber(_StringSegment_EX(Zdata, "|", 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(Zdata, "|", 64)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�[�Ⱦ� �}/��d%4s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 65)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 66)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�[�Ⱦ��[��%8s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 67)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, "|", 172) );
    //StrLine.sprintf("�o���I��%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);


    StrLine.sprintf("�}���d%12s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 60)).c_str() );
    to_ptr(StrLine);

    //  FFU4  1887
    //  FFU4  1895

    //  FFU5  1903
    //  FFU5  1911
    //  FFU6  1919
    //  FFU6  1927

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



    StrLine.sprintf("�����n��%10s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 58)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 59)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�o��%14s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(Zdata, "|", 84)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, "|", 85)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("���B���o��%8s%8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 81)).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�V�m�X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(Zdata, "|", 56)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, "|", 57)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("�����o��%19s%10s"," ",_StringSegment_EX(Zdata, "|", 16).c_str()  );
    to_ptr(StrLine);
     
    to_ptr("");

    //to_ptr("        ***     "+_StringSegment_EX(Zdata, "|", 79)+"."+ _StringSegment_EX(Zdata, "|", 80)+"     ***" );
    to_ptr("      ***     "+_StringSegment_EX(Zdata, "|", 79)+ _StringSegment_EX(Zdata, "|", 80)+"     ***" );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");

    return 0;
}


void __fastcall REPORT::to_ptr(AnsiString S)
{
    //��X�ɮ�
    if (S == "Save")       //X�b����
    {
        if (FileExists(char_filename))
            DeleteFile(char_filename);

        tslSaveToFile->SaveToFile(char_filename);

        tslSaveToFile->Clear();

        return;     //to_ptr()
    }
    else    //�x�s�r��
    {
        //if (gbl_epson_prt == false)
        //    S.sprintf("%cPR1%s", ESC, S.c_str());

        tslSaveToFile->Add(S);
    }
}


//int REPORT::CreateReport(char rpt_type, AnsiString sSal, AnsiString sPayStr,
//                         char *ptr_tencode, char *ptr_ecr_no,TStringList *tsl)
// �����P�P�馩���B        Lu 2018/06/13
int REPORT::CreateReport(char rpt_type, AnsiString sSal, AnsiString sPayStr,
                     char *ptr_tencode, char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev,
                     TStringList *tsl, int AutoZflg, AnsiString s1093 )
{
    //��ܳ��� 1���� 2X�b 3Z�b
    switch (rpt_type)
    {
      case 'd':                                 //����
          Dept(sSal, ptr_tencode, ptr_ecr_no,1, NULL, "");
          break;
      case 'E':                                 //�L�I�H������   21:ñ�� 22:ñ�h
          Dept(sSal, ptr_tencode, ptr_ecr_no,21, tsl, s1093);
          break;
      case 'e':                                 //�L�I�H������   21:ñ�� 22:ñ�h
          Dept(sSal, ptr_tencode, ptr_ecr_no,22, tsl, s1093);
          break;

      case 'D':                                 //����
          Dept(sSal, ptr_tencode, ptr_ecr_no);
          break;
      case 'x':                                 //���ȭ��b
      case 'c':                                 //checkin
          XDTReport(sSal, sPayStr, ptr_tencode,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, AutoZflg );
          break;
      case 'Z':     //Z�b
           ZDTReport(sSal, ptr_tencode, ptr_ecr_no,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, rpt_type );
          break;
      case 'V':    //�L�I�H��   21:ñ�� 22:ñ�h
          ZDTReport(sSal, ptr_tencode, ptr_ecr_no,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 21 );
          break;
      case 'v':
          ZDTReport(sSal, ptr_tencode, ptr_ecr_no,
            iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 22 );
          break;

      case 'X':                                 //XReport
          XReport(sSal, sPayStr, ptr_tencode, ptr_ecr_no,
                  iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev);
          break;
      case 's':                                 //�N�����
          subsale(tsl, ptr_tencode, ptr_ecr_no);
    }

    return 0;
}


REPORT::~REPORT()
{
    delete tslSaveToFile;
}

/*
AnsiString __fastcall REPORT::GetNumber(AnsiString S)
{
    AnsiString str_num;

    try
    {
        str_num.sprintf("%d",S.ToInt());
    }
    catch(Exception &e)
    {
        writelog(("�ƭ��ഫ�o�Ϳ��~ " + S));
        return "0";
    }

    return str_num;
}
*/

AnsiString __fastcall REPORT::ToDateFormat(AnsiString S)
{
    AnsiString str_date_format;

    str_date_format = S;
    str_date_format.Insert("-", 3);
    str_date_format.Insert(":", 6);

    return str_date_format;
}

AnsiString __fastcall REPORT::InsertSpace(AnsiString S, int i)
{
    int int_len = 0;

    int_len = S.Length();

    int int_space = (24 - int_len - i);

    AnsiString str_space = S;

    for (int i=0; i<int_space; i++)
        str_space.Insert(" ", 1);

    return str_space;
}

//int __fastcall REPORT::XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no)

// Ū�� 1096 data
int __fastcall REPORT::XReport(AnsiString &XRData, AnsiString sPayStr, char *ptr_tencode, char *ptr_ecr_no,
                             int iqt5_15217totrev, int iam5_15217totrev,
                             int iqt5_18219totrev, int iam5_18219totrev )
{

    //AnsiString str_bg_noclose;         //�Ǹ�
    //str_bg_noclose.sprintf("%06d", _StrToInt(XRData.SubString(2,5), "XRData"));

    AnsiString str_tencode,str_ecr_no,StrLine,sTmp,sStDDHHMM,sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no = ptr_ecr_no;


    StrLine="";
    StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format, "00000",
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("�Ǹ��G000000"));
    //to_ptr(("�����G" + str_tencode + "     �����G" + str_ecr_no));
    to_ptr("");

    to_ptr("                X�b��        ");
    to_ptr("");

    sTmp=_StringSegment_EX(XRData, "|", 10)+_StringSegment_EX(XRData, "|", 11);
    sStDDHHMM=sTmp.SubString(7,6); //����
    sTmp=_StringSegment_EX(XRData, "|", 8)+_StringSegment_EX(XRData, "|", 9);
    sPtDDHHMM=sTmp.SubString(7,6); //�e��

    StrLine.sprintf("���b�ɶ�  ���� %s �e�� %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );

    //StrLine.sprintf("���b�ɶ�   ���� %s  �e�� %s",
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 11) ).c_str(),
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 9) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�������~��%7s ����ӫȼ�%7s",
                     GetNumber(_StringSegment_EX(XRData, "|", 13) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, "|", 12) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    // to_ptr(("�N���ӫ~�ȼ�" + InsertSpace(GetNumber(XRData.SubString(1887,8)),12)));
    //to_ptr("");

    StrLine.sprintf("���鵲�b�֭p���B%13s%8s"," ", GetNumber(_StringSegment_EX(XRData, "|", 14) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�o�����b���B%17s%8s"," ", GetNumber(_StringSegment_EX(XRData, "|", 19) ).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("���}�o�����B%17s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 82) ).c_str() );
    to_ptr(StrLine);

    //  FFU4  1887
    //  FFU4  1895 ���B

    StrLine.sprintf("�o�����b   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(XRData, "|", 17) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, "|", 18) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�~,�h/��   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(XRData, "|", 21) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, "|", 22) ).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�~,�h/���X�p%17s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 23) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("���}�o���~,�h/���X�p%9s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 83) ).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("�����ھP�f���J%13s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 24) ).c_str() );
    to_ptr(StrLine);

    // 2015/08/19 Update Lu
    StrLine.sprintf("�N��,����O�ӫȼ�%12s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 177) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(�K)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 69) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�P����B(��)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 175) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(�K)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 70) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��,����O�@�o���B(��)%6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 176) ).c_str() );
    to_ptr(StrLine);
    StrLine.sprintf("�N��ӫ~�������B       %6s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 47) ).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("���d���X�p%20s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 86)).c_str() );
    //to_ptr(StrLine);

    StrLine.sprintf("�N���X�p%11s%8s  %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 77)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 78)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("�N��X�p%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    StrLine.sprintf("�N��ӫ~�X�p%7s%8d  %8d"," ", iqt5_15217totrev, iam5_15217totrev );
    to_ptr(StrLine);

    StrLine.sprintf("�Y���ʥN��X�p%15s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 87)).c_str() );
    to_ptr(StrLine);

    //�Y���ʦX�p
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XRData, "|", 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(XRData, "|", 170) );
    //StrLine.sprintf("�Y���ʻP�P��i��%14s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 169) ).c_str() );
    StrLine.sprintf("�Y���ʻP�P��i��%13s%8d"," ",iVDCTx+iVDCNx );
    to_ptr(StrLine);

    StrLine.sprintf("����O�X�p%19s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 86)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�N�I�X�p%11s%8d  %8d"," ", iqt5_18219totrev,iam5_18219totrev );
    to_ptr(StrLine);

    StrLine.sprintf("����   %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 40)).c_str(),
                                             "§��",
                                              GetNumber(_StringSegment_EX(XRData, "|", 30)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("��l�B�� %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 38)).c_str(),
    //                                         "�����",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 34)).c_str()  );
    StrLine.sprintf("��l�B��%7s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 38)).c_str(),
                                             "§���d",
                                              GetNumber(_StringSegment_EX(XRData, "|", 174)).c_str()  );
    to_ptr(StrLine);


    //StrLine.sprintf("��d�X�p %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 74)).c_str(),
    //                                         "�N�{���X�p",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 36)).c_str()  );
    StrLine.sprintf("��d�X�p%7s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 74)).c_str(),
                                             "�����",
                                              GetNumber(_StringSegment_EX(XRData, "|", 34)).c_str()  );
    to_ptr(StrLine);

    StrLine.sprintf("�N�{���X�p%19s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 36)).c_str() );
    to_ptr(StrLine);


    //to_ptr("���O��X�p");
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1871,8)),0));  //1871

    //StrLine.sprintf("�{���X�p%22s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 45)).c_str() );
    //to_ptr(StrLine);


     //20151208 ���a�ݨD�ܧ�A�վ�Z�b���w��ܦ�m
    StrLine.sprintf("���w�h��%11s    %14s"," ",
                                             //GetNumber(_StringSegment_EX(XRData, "|", 71)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 72)).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, "|", 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, "|", 172) );
    StrLine.sprintf("�o���I��%8d  %10s %8s",iPayinvoamt, "�{���X�p", GetNumber(_StringSegment_EX(XRData, "|", 45)).c_str() );
    to_ptr(StrLine);

    //to_ptr( ("�ɹs�X�p" + InsertSpace(GetNumber(XRData.SubString(1639,8)),8)) );
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1647,8)),0));



    StrLine.sprintf("��w�X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 50)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 51)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�󥿦X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 52)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 53)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("��������X�p%6s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 54)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 55)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�馩(���|)%8s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 61)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 62)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�ӫ~�զX�馩/��%3s%8s   %8s"," ",
                                                  GetNumber(_StringSegment_EX(XRData, "|", 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(XRData, "|", 64)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("�[�Ⱦ� �}/��d%4s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 65)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 66)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("�[�Ⱦ��[��%8s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 67)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, "|", 172) );
    //StrLine.sprintf("�o���I��%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
   // to_ptr(StrLine);

    StrLine.sprintf("�}���d%12s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 60)).c_str() );
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

    StrLine.sprintf("�����n��%10s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 58)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 59)).c_str() );
    to_ptr(StrLine);    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1799,8)),0));


    StrLine.sprintf("�o��%14s%8s   %8s"," ",
                                               GetNumber(_StringSegment_EX(XRData, "|", 84)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, "|", 85)).c_str() );
    to_ptr(StrLine);

    StrLine.sprintf("���B���o��%8s%8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 81)).c_str()  );
    to_ptr(StrLine);


    StrLine.sprintf("�V�m�X�p%10s%8s   %8s"," ",
                                             GetNumber(_StringSegment_EX(XRData, "|", 56)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, "|", 57)).c_str() );
    to_ptr(StrLine);


    StrLine.sprintf("�����o��%19s%10s"," ",_StringSegment_EX(XRData, "|", 16).c_str()  );
    to_ptr(StrLine);

   // int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
   // AnsiString sCpnCnt, sCpnAmt;

   // icpn77cnt=_StrToInt(sPayStr.SubString(225,8));
   // icpn77amt=_StrToInt(sPayStr.SubString(233,8));
   // icpn78cnt=_StrToInt(sPayStr.SubString(241,8));
   // icpn78amt=_StrToInt(sPayStr.SubString(249,8));
  //  sCpnCnt.sprintf("%8d",icpn77cnt+icpn78cnt);
   // sCpnAmt.sprintf("%8d",icpn77amt+icpn78amt);

  //  to_ptr("");
  //  to_ptr( ("���f����B" + InsertSpace(GetNumber(sCpnAmt),10)));

    to_ptr("");

    //to_ptr("        ***     "+_StringSegment_EX(XRData, "|", 79)+"."+ _StringSegment_EX(XRData, "|", 80)+"     ***");
    to_ptr("       ***     "+_StringSegment_EX(XRData, "|", 79)+ _StringSegment_EX(XRData, "|", 80)+"     ***");


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");


    return 0;
}

AnsiString __fastcall REPORT::ToTimeFormat(AnsiString S)
{
    AnsiString str_time;

    str_time = (S.SubString(1,4)).Insert(":",3);
    return str_time;
}

REPORT::REPORT()
{
    tslSaveToFile = new TStringList;
    char_filename[0] = 0;
    sprintf(gchar_date_format, "%10s", FormatDateTime("yyyy-mm-dd",Now()).c_str());
}

//�N�����
int __fastcall REPORT::subsale(TStringList *tsl, char *ptr_tencode, char *ptr_ecr_no)
{
    AnsiString str_tencode, str_ecr_no, StrLine;     //�����B����
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;
    int int_amt = 0;                        //�N����B�[�`

    //�浧�O���B�ӫ~�W�١B�P����B
    AnsiString str_record, str_bar_name, str_item_amt;


    StrLine="";
    StrLine.sprintf("%s�Ǹ�:%s����:%s ��:%s",gchar_date_format,"00000",
                                           str_tencode,str_ecr_no);
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr("�Ǹ��G000000");
    //to_ptr(("�����G" + str_tencode + "     �����G" + str_ecr_no));
    to_ptr("");

    to_ptr("           �N��ӫ~���B����     ");
    to_ptr("");

    StrLine.sprintf("��%s�ӫ~�W��%17s�P����B"," ", " ");
    to_ptr(StrLine);
    int int_space = 1;
    for (int i=0; i<tsl->Count; i++)
    {
        str_record = tsl->Strings[i];
        str_bar_name = _StringSegment(str_record, "|", 3);
        str_item_amt = _StringSegment(str_record, "|", 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //�R�����Ϊť�
        {
            int_space = str_bar_name.Pos("�@");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //�R���b�Ϊť�
        {
            int_space = str_bar_name.Pos(" ");
            if (int_space)
            {
                str_bar_name.Delete(int_space,2);
            }

        }


        str_record.sprintf("%02d.%-26s%7s",i+1 ,str_bar_name.SubString(1,26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);


        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
    }


    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    �X�p%16d"," ",int_amt);
    str_record.sprintf("�X�p%15s %16d"," ",int_amt);
    to_ptr(str_record);

    // 2019/11/06 For SCO
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    //to_ptr("");

    StrLine.sprintf("\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");

    return 0;
}
