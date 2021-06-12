//---------------------------------------------------------------------------

#include "stdafx.h"
#include "UReport.h"

//---------------------------------------------------------------------------
//#pragma hdrstop
//#pragma package(smart_init)

//�����b�B�z�{��
int __fastcall REPORT::Dept(string &ZData, const char *ptr_tencode, const char *ptr_ecr_no, int XR, TStringList *tsl, string s1093)
{

    string str_nz_cnt;             //����Ǹ�
    string str_tencode;           //����
    string str_ecr_no;             //����
    string str_bg_noclose;
    int int_dept = 88;                //�e�@�ӳ����bZDATA���}�l��l

    //if( XR==21 || XR==22)
	_Strsprintf(str_bg_noclose, "%08d", _StrToInt(_StringSegment_EX(ZData, SGM, 7), "ZData") );

    //�N�����x�s��TStringList��K�B�z  for (list<string>::iterator s = S1050Lst->begin(); s != S1050Lst->end(); s++)
    TStringList *tsl_zdata = new TStringList;
	tsl_zdata->clear();
	list<string>::iterator ls;
	
    string sDpt,stmp;
    for (int i=0; i<20; i++)
    {
        int_dept++;    //89~168 �}�l
        sDpt=_StringSegment_EX(ZData, SGM, int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, SGM, int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, SGM, int_dept);
        int_dept++;
        sDpt+=_StringSegment_EX(ZData, SGM, int_dept);

        tsl_zdata->push_back(sDpt);        //�����C�@�q��Ƭ�32

    }

    if (XR==1)
    {
        str_nz_cnt = "00000";
    }
    else
    {
        str_nz_cnt=_StringSegment_EX(ZData, SGM, 6); // 2013/07/22
        
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
	ls = tsl_zdata->begin();;
    try
    {
        dept = new sut_dept[20];

        //�x�s��TStringList�������b�g�J���c��
        for (int i=0; i<20; i++, ls++)
        {
			if (ls != tsl_zdata->end())
			{
				stmp = *ls;
				//dept[i].qty = _StrToInt(stmp.substr(0, 8),"");
				//dept[i].amt = _StrToInt(stmp.substr(8, 8), "");
				//dept[i].rqty = _StrToInt(stmp.substr(16, 8), "");
				//dept[i].ramt = _StrToInt(stmp.substr(24, 8),"");

                dept[i].qty = _StrToInt(stmp.substr(0, 8), "");
                dept[i].amt = _StrToInt(stmp.substr(8, 10), "");
                dept[i].rqty = _StrToInt(stmp.substr(18, 8), "");
                dept[i].ramt = _StrToInt(stmp.substr(26, 10), "");

			}
			else
			{
				break;
			}
        }
    }
	catch(...)
    {
        
    }
	delete tsl_zdata;


    string qtyamt,  //�ƶq�[�`�ξP����B�[�`
		   ramt;    //�~���B�h�f/���L���B�[�`
    string   StrLine;

    StrLine="";

    if( XR==21 || XR==22)
		_Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_bg_noclose.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    else
		_Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_nz_cnt.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
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
    string sCount;
    int int_a = 0, int_b = 0;

	_Strsprintf(StrLine, "����   �P�� �P����B    �h�� �h�f���B");
    to_ptr(StrLine);
    for (int i=0; i<20; i++)
    {
        int_a -= dept[i].rqty;
        int_b -= dept[i].ramt;

		_Strsprintf(qtyamt,"%5d %8d", dept[i].qty, dept[i].amt);            //"%6d %8d"
		_Strsprintf(ramt,"%5d %8d", int_a, int_b);                         //"%6d %8d"
		_Strsprintf(sCount,"%02d", (i + 1));

      //StrLine.sprintf("%3s   %s     %s",sCount, qtyamt, ramt);
		_Strsprintf(StrLine, "%3s   %s  %s", sCount.c_str(), qtyamt.c_str(), ramt.c_str());
        to_ptr(StrLine);
       

        //to_ptr(("����" + sCount));
        //to_ptr(qtyamt);
        //to_ptr(ramt);
        //to_ptr("");

        int_a = 0;
        int_b = 0;
    }

    //to_ptr("�����X�p");

    int_a -= _StrToInt(_StringSegment_EX(ZData, SGM, 43));
    int_b -= _StrToInt(_StringSegment_EX(ZData, SGM, 44));

	_Strsprintf(qtyamt,"%5d %8d", _StrToInt(_StringSegment_EX(ZData, SGM, 41)),       //"%6d %8d"
                              _StrToInt(_StringSegment_EX(ZData, SGM, 42))   );   //"%6d %8d"
	_Strsprintf(ramt,"%5d %8d", int_a, int_b);

	_Strsprintf(StrLine,"--------------------------------------");
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�X�p %s   %s", qtyamt.c_str(), ramt.c_str());
    to_ptr(StrLine);

    //to_ptr("\n\n\n\n");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //�����X�p�[�`
    //to_ptr("�����X�p");
    //to_ptr(qtyamt);
    //to_ptr(ramt);

	/////////////////////////////////  �����P�P����  ////////////////////////////////////
	int int_a_Tot, int_b_Tot;
	if ( Trim(s1093) != "")
	{
		int_a = 0; int_b = 0; int_a_Tot = 0;  int_b_Tot = 0;

		to_ptr("");
		if (XR == 21 || XR == 22)
			_Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_bg_noclose.c_str(),
			str_tencode.c_str(), str_ecr_no.c_str());
		else
			_Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_nz_cnt.c_str(),
			str_tencode.c_str(), str_ecr_no.c_str());
		to_ptr(StrLine);

		if (XR == 21) //21:ñ�� 22:ñ�h
			to_ptr("        �����P�P����(ñ��)     ");
		else if (XR == 22) //21:ñ�� 22:ñ�h
			to_ptr("        �����P�P����(ñ�h)     ");
		else
			to_ptr("           �����P�P����     ");

		to_ptr("");

		_Strsprintf(StrLine, "����   �P��P�P���B   �h�f�P�P���B");
		to_ptr(StrLine);
		for (int i = 1; i<21; i++)
		{
			int_a = _StrToInt(_StringSegment_EX(s1093, SGM, (2 * i) - 1));
			int_a_Tot += int_a;  // total
			int_b = _StrToInt(_StringSegment_EX(s1093, SGM, (2 * i)));
			int_b_Tot += int_b;  // total

			_Strsprintf(qtyamt,"%8d", int_a);
			_Strsprintf(ramt,"%8d", int_b);
			_Strsprintf(sCount,"%02d", (i));

			_Strsprintf(StrLine, "%3s   %s      %s", sCount.c_str(), qtyamt.c_str() , ramt.c_str());
			to_ptr(StrLine);
			int_a = 0;
			int_b = 0;
		}

		_Strsprintf(ramt,"%8d      %8d", int_a_Tot, int_b_Tot);

		_Strsprintf(StrLine, "--------------------------------------");
		to_ptr(StrLine);

		_Strsprintf(StrLine, "�X�p  %s   ", ramt.c_str() );
		to_ptr(StrLine);

		//to_ptr("\n\n\n\n");
		to_ptr("");
		to_ptr("");
		to_ptr("");
		to_ptr("");
		// 2019/11/06 For SCO
		//_Strsprintf(StrLine, "\x1b\x69");
		//to_ptr(StrLine);
	}


    ///////////////////////�N��ӫ~���B����//////////////////////////////////

   if( ( XR==21 || XR==22) && tsl )
     {
       int int_amt = 0;                        //�N����B�[�`

       //�浧�O���B�ӫ~�W�١B�P����B
       string str_record, str_bar_name, str_item_amt;
	   // 2019/11/06 For SCO
	   to_ptr("---------------------------------------");
       StrLine="";
	   _Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_bg_noclose.c_str(),
		   str_tencode.c_str(), str_ecr_no.c_str());
       to_ptr(StrLine);

       to_ptr("");
       to_ptr("           �N��ӫ~���B����     ");
       to_ptr("");

	   _Strsprintf(StrLine, "��%s�ӫ~�W��%17s�P����B", " ", " ");
       to_ptr(StrLine);
       int int_space = 1;
	   int i = 0;
       //for (int i=0; i<tsl->Count; i++)
	   for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++, i++)
       {
		str_record = *ls; // tsl->Strings[i];
        str_bar_name = _StringSegment_EX(str_record, SGM, 3);
        str_item_amt = _StringSegment_EX(str_record, SGM, 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //�R�����Ϊť�
        {
            int_space = str_bar_name.find("�@");
            if (int_space)
            {
                str_bar_name.erase(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //�R���b�Ϊť�
        {
            int_space = str_bar_name.find(" ");
            if (int_space)
            {
                str_bar_name.erase(int_space,2);
            }
        }

		_Strsprintf(str_record,"%02d.%-26s%7s", i + 1, str_bar_name.substr(0, 26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);
 
        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
       }


    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    �X�p%16d"," ",int_amt);
	_Strsprintf(str_record,"�X�p%15s %16d", " ", int_amt);
    to_ptr(str_record);

    to_ptr("");

    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine,"\x1b\x69");
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
int __fastcall REPORT::INQReport(const char *ptr_tencode, const char *ptr_ecr_no,
                                 INQTBL *InqTbl)
{

    string str_nz_cnt;             //����Ǹ�
    string str_tencode;           //����
    string str_ecr_no;             //����
 
    str_tencode = ptr_tencode;                  //����
    str_ecr_no  = ptr_ecr_no;                    //����

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(XZDATA_INI);
	str_nz_cnt = m_IniReader.ReadString("Z", "NZ_CNT", "00001");   //tif_ecr->ReadString("Z", "NZ_CNT", "");     //�ֿn���b���ƧǸ�
	if (str_nz_cnt.length() < 5)
		str_nz_cnt = "00001";

 //   TIniFile *tif_ecr;
 //   try
 //       {
 //           tif_ecr = new TIniFile(XZDATA_INI);
 //           str_nz_cnt = tif_ecr->ReadString("Z","NZ_CNT","");     //�ֿn���b���ƧǸ�

 //           if (str_nz_cnt.Length() < 5)
 //               str_nz_cnt = "00001";
 //       }
	//catch (...)
 //       {
 //           delete tif_ecr;
 //       }
   
    string qtyamt,  //�ƶq�[�`
               ramt;    //���B�[�`
   
    to_ptr(gchar_date_format);
    to_ptr(("�Ǹ��G" + str_nz_cnt));
    to_ptr(("�����G" + str_tencode + "    " + "�����G" + str_ecr_no));
    to_ptr("");
 // to_ptr("       �����b��        ");
    to_ptr(" �Y�ɳs�u�ӫ~����έp�� ");
    to_ptr("");

    //��X���ɮ�
    string sCount;
    int int_a = 0, int_b = 0;

    for (int i=1; i<20; i++)
    {
        int_a = InqTbl[i].qt_InqSale;
        int_b = InqTbl[i].am_InqSale;
        if( int_a > 0 || int_b > 0)
          {
			_Strsprintf(qtyamt,"%6d%18d", int_a, int_b);
           //ramt.sprintf("%6d%18d", int_a, int_b);
			_Strsprintf(sCount,"%02d", i);

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


// int __fastcall REPORT::X(string &XData, string sPayStr, char *ptr_tencode   )
//XData: 3095 ���ȭ��b��q����
int __fastcall REPORT::XDTReport(string &XData, string sPayStr, const char *ptr_tencode,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int AutoZflg )
{

    string str_tencode, StrLine, NowDDHHmm, PreDDHHmm, stmp;
    str_tencode = ptr_tencode;

    StrLine="";

	_Strsprintf(StrLine,"%s�Ǹ�:%5s ����:%s ��:%s", gchar_date_format,
                                               _StringSegment_EX(XData, SGM, 8).c_str(),
											   str_tencode.c_str(),
											   _StringSegment_EX(XData, SGM, 4).c_str() );
    to_ptr(StrLine);
    //to_ptr(gchar_date_format);
    //to_ptr(("�Ǹ��G" + XData.SubString(7,2)));
    //to_ptr(("�����G" + str_tencode + "     " + "�����G" + XData.SubString(1,1)));
    to_ptr("");

    to_ptr("             ���ȭ����Ӫ�        ");
    to_ptr("");

    to_ptr( ("���ȭ� NO " + _StringSegment_EX(XData, SGM, 9)  ) );

    //���b���+���b�ɶ�
    NowDDHHmm=_StringSegment_EX(XData, SGM, 12).substr(6,2)+_StringSegment_EX(XData, SGM, 13).substr(0,4);
    //�e�����b���+�e�����b�ɶ�
    PreDDHHmm=_StringSegment_EX(XData, SGM, 10).substr(6,2)+_StringSegment_EX(XData, SGM, 11).substr(0,4);

    //StrLine.sprintf("���b�ɶ�    �}�l %s ���� %s",
	_Strsprintf(StrLine,"���b�ɶ�    ���� %s �e�� %s",
                     ToDateFormat( NowDDHHmm ).c_str(),
                     ToDateFormat( PreDDHHmm ).c_str() );
    to_ptr(StrLine);
    //to_ptr(("���ȭ� NO" + InsertSpace(XData.SubString(2,5),9)) );
    //to_ptr(("�}�l�ɶ�" + InsertSpace(ToDateFormat(XData.SubString(15,6)),8)));
    //to_ptr(("�����ɶ�" + InsertSpace(ToDateFormat(XData.SubString(29,6)),8)));
    to_ptr("");


	_Strsprintf(StrLine,"����~��    %8s  ����ӫȼ�%7s",
                     GetNumber(_StringSegment_EX(XData, SGM, 15) ).c_str(),
                     GetNumber(_StringSegment_EX(XData, SGM, 14) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    //to_ptr(("�N���ӫ~�ȼ�" + InsertSpace(GetNumber(XData.SubString(3293,8)),12)));

	_Strsprintf(StrLine,"�o�����b���B(GS)%14s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 20)).c_str());
    to_ptr(StrLine);
    //to_ptr("�o�����b���B(GS)");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(69,8)),0));

    //  FFU4  3293
    //  FFU4  3301 ���B
	_Strsprintf(StrLine,"���}�o�����B%18s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 74)).c_str());
    to_ptr(StrLine);
    //to_ptr("���}�o�����B");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3301,8)),0));

	_Strsprintf(StrLine,"�~,�h/���X�p%18s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 23)).c_str());
    to_ptr(StrLine);
    //to_ptr("�~�B�h/���X�p");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(101,8)),0));

	_Strsprintf(StrLine,"���}�o���~,�h/���X�p%10s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 75)).c_str());
    to_ptr(StrLine);
    //to_ptr("���}�o���~�B�h/���X�p");
    //to_ptr(InsertSpace(GetNumber(XData.SubString(3189,8)),0));   //�[�Ⱦ� ���b���B, ���}�o�����B�~�h��

	_Strsprintf(StrLine,"��ھP�f���J(NS)%14s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 24)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine,"�N��,����O�ӫȼ�%13s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 89)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�P����B%11s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 87)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�@�o���B%11s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 88)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��ӫ~�������B   %11s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 43)).c_str());
    to_ptr(StrLine);

    //StrLine.sprintf("���d���X�p%20s %8s"," ",GetNumber(_StringSegment_EX(XData, SGM, 81) ).c_str() );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "�N���X�p%11s %8s   %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 69)).c_str(),
                                                  GetNumber(_StringSegment_EX(XData, SGM, 70) ).c_str() );
    to_ptr(StrLine);


    //StrLine.sprintf("�N��X�p%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
	_Strsprintf(StrLine, "�N��ӫ~�X�p%7s %8d   %8d", " ", iqt5_15217totrev, iam5_15217totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�Y���ʥN��X�p%16s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 79)).c_str());
    to_ptr(StrLine);

    //�Y���ʦX�p
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XData, SGM, 81) );
    iVDCNx=_StrToInt( _StringSegment_EX(XData, SGM, 82) );
    //StrLine.sprintf("�Y���ʻP�P��i��%14s %8s"," ",GetNumber( _StringSegment_EX(XData, "|", 204) ).c_str() );
	_Strsprintf(StrLine, "�Y���ʻP�P��i��%14s %8d", " ", iVDCTx + iVDCNx);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "����O�X�p%20s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 78)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N�I�X�p%11s %8d   %8d", " ", iqt5_18219totrev, iam5_18219totrev);
    to_ptr(StrLine);


	_Strsprintf(StrLine, "����     %8s   %10s %8s", GetNumber(_StringSegment_EX(XData, SGM, 40)).c_str(),
                                             "§��",
                                              GetNumber(_StringSegment_EX(XData, SGM, 30) ).c_str() );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "��l�B�� %8s   %10s %8s", GetNumber(_StringSegment_EX(XData, SGM, 38)).c_str(),
                                             "§���d",
                                              GetNumber(_StringSegment_EX(XData, SGM, 86) ).c_str()  );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "��d�X�p %8s   %10s %8s", GetNumber(_StringSegment_EX(XData, SGM, 66)).c_str(),
                                             "�����",
                                              GetNumber(_StringSegment_EX(XData, SGM, 34) ).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N�{���X�p%20s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 36)).c_str());
    to_ptr(StrLine);

   
    //20151208 ���a�ݨD�ܧ�A�վ�Z�b���w��ܦ�m
	_Strsprintf(StrLine, "���w�h��%11s     %15s", " ",
                                                 GetNumber(_StringSegment_EX(XData, SGM, 64) ).c_str() );
    to_ptr(StrLine);

    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XData, SGM, 83) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XData, SGM, 84) );

	_Strsprintf(StrLine, "�o���I�� %8d   %10s %8s", iPayinvoamt, "�{���X�p", GetNumber(_StringSegment_EX(XData, SGM, 41)).c_str());
    to_ptr(StrLine);

    TStringList *tsl_drop;

    string sDrop;
    int iDropCnt=0;
	tsl_drop = new TStringList; //for (list<string>::iterator ls = gtsl_1051_sal->begin(); ls != gtsl_1051_sal->end(); ls++)
	list<string>::iterator ls;
    try
    {
		tsl_drop->clear();
        int int_position = 144;    //LOG����w�}�l��m

		for (int i = 0; i < 30; i++) // , ls++)
        {
            sDrop=_StringSegment_EX(XData, SGM, int_position)+_StringSegment_EX(XData, SGM, int_position+1);
            tsl_drop->push_back(sDrop);
            int_position += 2;
        }

        string str_drop_time,str_drop_money,str_drop_data;
        int int_drop_money = 0;

        to_ptr("��w");
		
		ls = tsl_drop->begin();
		for (int i = 0; i < 30; i++, ls++) /// != tsl_drop->end()
        {
			stmp = *ls;
			if (ls == tsl_drop->end())
				break;

			str_drop_time = stmp.substr(8, 6); //  tsl_drop->Strings[i].SubString(9, 6);

            if (str_drop_time == "000000" || Trim(str_drop_time)=="" )
            {
                break;
            }
            else
            {
                str_drop_time = ToTimeFormat(str_drop_time);
				int_drop_money = _StrToInt(stmp.substr(0, 8), "");   //_StrToInt(tsl_drop->Strings[i].SubString(1, 8), tsl_drop->Strings[i]);
                str_drop_money = InsertSpace(int2str(int_drop_money),12);

                if (i < 9)
                    str_drop_data = ("     " + int2str(i + 1) + " " + str_drop_time + "     " + str_drop_money);
                else
					str_drop_data = ("    " + int2str(i + 1) + " " + str_drop_time + "     " + str_drop_money);


                to_ptr(str_drop_data);
                iDropCnt++;
            }
        }
    }
	catch (...)
    {
        ;
    }

	delete tsl_drop;

    //��w����
    //��w�X�p���B
    //StrLine.sprintf("�X�p%15s %8s   %8s"," ",GetNumber(XData.SubString(3077,8)).c_str(),
    //                                             GetNumber(XData.SubString(3085,8)).c_str() );
	_Strsprintf(StrLine, "�X�p%8s         %8s", GetNumber(_StringSegment_EX(XData, SGM, 44)).c_str(),
                                          GetNumber(_StringSegment_EX(XData, SGM, 45) ).c_str() );
    to_ptr(StrLine);
    to_ptr("");


	_Strsprintf(StrLine, "�󥿦X�p%11s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 46) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 47) ).c_str() );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "��������X�p%7s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 48) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 49) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�馩%15s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 53) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 54) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�ӫ~�զX�馩/��%4s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 55) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 56) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�[�Ⱦ� �}/��d%5s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 57) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 58) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�[�Ⱦ��[��%9s %8s   %8s", " ",
                                         GetNumber(_StringSegment_EX(XData, SGM, 59) ).c_str(),
                                         GetNumber(_StringSegment_EX(XData, SGM, 60) ).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XData, "|", 206) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XData, "|", 207) );
    //StrLine.sprintf("�o���I��%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "�}���d%13s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 52)).c_str());
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

    //2021/03/17 ����
	//_Strsprintf(StrLine, "e�q�d�{���[��%6s %8s   %8s", " ",
    //                                          GetNumber(_StringSegment_EX(XData, SGM, 64) ).c_str(),
    //                                          GetNumber(_StringSegment_EX(XData, SGM, 65) ).c_str() );
    //to_ptr(StrLine);

    //StrLine.sprintf("e�q�d�H�Υd�[��%4s %8s   %8s"," ",
    //                                          GetNumber(_StringSegment_EX(XData, "|", 66) ).c_str(),
    //                                          GetNumber(_StringSegment_EX(XData, "|", 67) ).c_str() );
    //to_ptr(StrLine);




   // to_ptr( ("e�q�d���b" + InsertSpace(GetNumber(XData.SubString(3389,8)),9)) );        //ffu10
   // to_ptr(InsertSpace(GetNumber(XData.SubString(3397,8)),0));                         //ffu10

	_Strsprintf(StrLine, "�����n��%11s %8s   %8s", " ",
                                              GetNumber(_StringSegment_EX(XData, SGM, 50) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, SGM, 51) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�o��%15s %8s   %8s", " ",
                                              GetNumber(_StringSegment_EX(XData, SGM, 76) ).c_str(),
                                              GetNumber(_StringSegment_EX(XData, SGM, 77) ).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "���B���o��%9s %8s", " ", GetNumber(_StringSegment_EX(XData, SGM, 73)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�����o��%20s %10s", " ", _StringSegment_EX(XData, SGM, 17).c_str());
    to_ptr(StrLine);


    //int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
    string sCpnCnt, sCpnAmt;

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
    to_ptr( ("        ***     " + _StringSegment_EX(XData, SGM, 71)+ _StringSegment_EX(XData, SGM, 72)+ "     ***") );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");

    return 0;
}

//3096
// int __fastcall REPORT::Z(string &Zdata, char *ptr_tencode, char *ptr_ecr_no)
int __fastcall REPORT::ZDTReport(string &Zdata, const char *ptr_tencode, const char *ptr_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev, int rpt_type )
{

    string str_bg_noclose;         //�Ǹ�

    if( rpt_type==21 || rpt_type==22)
		_Strsprintf(str_bg_noclose,"%08d", _StrToInt(_StringSegment_EX(Zdata, SGM, 7), "ZData"));
    else
		_Strsprintf(str_bg_noclose,"%05d", _StrToInt(_StringSegment_EX(Zdata, SGM, 6), "ZData")); 

    string str_tencode, str_ecr_no, StrLine, sTmp, sStDDHHMM, sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;

    StrLine="";
    if( rpt_type==21 || rpt_type==22)
		_Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_bg_noclose.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    else
		_Strsprintf(StrLine, "%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, str_bg_noclose.c_str(),
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);
  
    to_ptr("");

    //21:ñ�� 22:ñ�h
    if( rpt_type ==21)
        to_ptr("          �L�I�H���b��(ñ��)     ");
    else if( rpt_type ==22)
        to_ptr("          �L�I�H���b��(ñ�h)     ");
    else
        to_ptr("                Z�b��        ");

    to_ptr("");

    sTmp=_StringSegment_EX(Zdata, SGM, 10)+_StringSegment_EX(Zdata, SGM, 11);
    sStDDHHMM=sTmp.substr(6,6); //����
    sTmp=_StringSegment_EX(Zdata, SGM, 8)+_StringSegment_EX(Zdata, SGM, 9);
    sPtDDHHMM=sTmp.substr(6,6); //�e��

	_Strsprintf(StrLine, "���b�ɶ�  ���� %s �e�� %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�������~��%7s ����ӫȼ�%7s",
                     GetNumber(_StringSegment_EX(Zdata, SGM, 13) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, SGM, 12) ).c_str() );
    to_ptr(StrLine);

   	_Strsprintf(StrLine, "���鵲�b�֭p���B%13s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 14)).c_str());
    to_ptr(StrLine);
    	
	_Strsprintf(StrLine, "�o�����b���B%17s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 19)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "���}�o�����B%17s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 82)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�o�����b   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(Zdata, SGM, 17) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, SGM, 18) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�~,�h/��   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(Zdata, SGM, 21) ).c_str(),
                     GetNumber(_StringSegment_EX(Zdata, SGM, 22) ).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�~,�h/���X�p%17s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 23)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "���}�o���~,�h/���X�p%9s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 83)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�����ھP�f���J%13s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 24)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N��,����O�ӫȼ�%12s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 177)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�P����B(�K)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 69)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�P����B(��)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 175)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�@�o���B(�K)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 70)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�@�o���B(��)%6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 176)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��ӫ~�������B       %6s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 47)).c_str());
    to_ptr(StrLine);

  	_Strsprintf(StrLine, "�N���X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 77)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 78)).c_str() );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "�N��ӫ~�X�p%6s%8d   %8d", " ", iqt5_15217totrev, iam5_15217totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�Y���ʥN��X�p%15s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 87)).c_str());
    to_ptr(StrLine);

    //�Y���ʦX�p
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(Zdata, SGM, 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(Zdata, SGM, 170) );
    //StrLine.sprintf("�Y���ʻP�P��i��%14s %8s"," ",GetNumber(_StringSegment_EX(Zdata, "|", 169) ).c_str() );
	_Strsprintf(StrLine, "�Y���ʻP�P��i��%13s%8d", " ", iVDCTx + iVDCNx);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "����O�X�p%19s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 86)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N�I�X�p%10s%8d   %8d", " ", iqt5_18219totrev, iam5_18219totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "����    %8s  %10s %8s", GetNumber(_StringSegment_EX(Zdata, SGM, 40)).c_str(),
                                             "§��",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 30)).c_str() );
    to_ptr(StrLine);
        
	_Strsprintf(StrLine, "��l�B��%8s  %10s %8s", GetNumber(_StringSegment_EX(Zdata, SGM, 38)).c_str(),
                                             "§���d",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 174)).c_str()  );
    to_ptr(StrLine);

    _Strsprintf(StrLine, "��d�X�p%8s  %10s %8s", GetNumber(_StringSegment_EX(Zdata, SGM, 74)).c_str(),
                                             "�����",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 34)).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N�{���X�p%19s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 36)).c_str());
    to_ptr(StrLine);

    //20151208 ���a�ݨD�ܧ�A�վ�Z�b���w��ܦ�m
	_Strsprintf(StrLine, "���w�h��%10s     %14s", " ",
                                              GetNumber(_StringSegment_EX(Zdata, SGM, 72)).c_str() );
    to_ptr(StrLine);

    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, SGM, 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, SGM, 172) );

	_Strsprintf(StrLine, "�o���I��%8d  %10s %8s", iPayinvoamt, "�{���X�p", GetNumber(_StringSegment_EX(Zdata, SGM, 45)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "��w�X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 50)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 51)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�󥿦X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 52)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 53)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "��������X�p%6s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 54)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 55)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�馩(���|)%8s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 61)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 62)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�ӫ~�զX�馩/��%3s%8s   %8s", " ",
                                                  GetNumber(_StringSegment_EX(Zdata, SGM, 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(Zdata, SGM, 64)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�[�Ⱦ� �}/��d%4s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 65)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 66)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�[�Ⱦ��[��%8s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 67)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(Zdata, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(Zdata, "|", 172) );
    //StrLine.sprintf("�o���I��%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
    //to_ptr(StrLine);


	_Strsprintf(StrLine, "�}���d%12s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 60)).c_str());
    to_ptr(StrLine);

   
	_Strsprintf(StrLine, "�����n��%10s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 58)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 59)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�o��%14s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 84)).c_str(),
                                               GetNumber(_StringSegment_EX(Zdata, SGM, 85)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine,"���B���o��%8s%8s", " ", GetNumber(_StringSegment_EX(Zdata, SGM, 81)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�V�m�X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 56)).c_str(),
                                             GetNumber(_StringSegment_EX(Zdata, SGM, 57)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�����o��%19s%10s", " ", _StringSegment_EX(Zdata, SGM, 16).c_str());
    to_ptr(StrLine);
     
    to_ptr("");
            
    to_ptr("      ***     "+_StringSegment_EX(Zdata, SGM, 79)+ _StringSegment_EX(Zdata, SGM, 80)+"     ***" );


    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");

    return 0;
}


void __fastcall REPORT::to_ptr(string S)
{
    //��X�ɮ�
    if (S == "Save")       //X�b����
    {
		if (PathFileExists(StringToWString(string(char_filename)).c_str()))
			DeleteFile(StringToWString(string(char_filename)).c_str());

        /*if (FileExists(char_filename))
            DeleteFile(char_filename.c_str());*/

		//_ListToFile(tslSaveToFile, char_filename);  //tslSaveToFile->SaveToFile(char_filename);
        _ListToWFile(tslSaveToFile, string(char_filename) ); //Unicode
        tslSaveToFile->clear();
        logsprintf("REPORT::to_ptr: Rpt File=%s,", char_filename );
        return;     //to_ptr()
    }
    else    //�x�s�r��
    {
        //if (gbl_epson_prt == false)
        //    S.sprintf("%cPR1%s", ESC, S.c_str());

        tslSaveToFile->push_back(S);
    }
}


int REPORT::CreateReport001(string rpt_type, string sSal, string sPayStr, string str_tencode, string  str_ecr_no,
                            int iqt5_15217totrev , int iam5_15217totrev ,
                            int iqt5_18219totrev, int iam5_18219totrev , TStringList *tsl,
                            int AutoZflg, string s1093)
{

    //��ܳ��� 1���� 2X�b 3Z�b
    return 0;

}


int REPORT::CreateReport(string rpt_type, string sSal, string sPayStr,
                     string str_tencode, string  str_ecr_no,
                     int iqt5_15217totrev, int iam5_15217totrev,
                     int iqt5_18219totrev, int iam5_18219totrev,
					 TStringList *tsl, int AutoZflg, string s1093 )
{
   
    //��ܳ��� 1���� 2X�b 3Z�b
  

    /***/
    //if (str_good_no.substr(0, 1) == "M")  //�A�|�ӫ~ CD_FMCODE => M�}�Y7�X
    if (rpt_type == "d")  //����
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 1, NULL, "");
    }
    else  if (rpt_type == "E") //�L�I�H������   21:ñ�� 22:ñ�h
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 21, tsl, s1093);
    }
    else  if (rpt_type == "e") //�L�I�H������   21:ñ�� 22:ñ�h
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 22, tsl, s1093);
    }
    else  if (rpt_type == "D") //����
    {
        Dept(sSal, str_tencode.c_str(), str_ecr_no.c_str(), 0, NULL, "" );
    }
    else  if (rpt_type == "x" || rpt_type == "c") //���ȭ��b/checkin
    {
        XDTReport(sSal, sPayStr, str_tencode.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, AutoZflg);
    }
    else  if (rpt_type == "Z")
    {
        ZDTReport(sSal, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 0);
    }
    else  if (rpt_type == "V")     //�L�I�H��   21:ñ�� 22:ñ�h
    {
        ZDTReport(sSal, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 21);
    }
    else  if (rpt_type == "v")
    {
        ZDTReport(sSal, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev, 22);
    }
    else  if (rpt_type == "X")
    {
         XReport(sSal, sPayStr, str_tencode.c_str(), str_ecr_no.c_str(), iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev);
    }
    else  if (rpt_type == "s")
    {
        subsale(tsl, str_tencode.c_str(), str_ecr_no.c_str() );
    }
    else
    {
        ;;
    }
    /***/
    /******
    switch (rpt_type)
    {
      case 'd':                                 //����
          Dept(sSal, ptr_tencode, ptr_ecr_no, 1, NULL, "");
          break;
      case 'E':                                 //�L�I�H������   21:ñ�� 22:ñ�h
		  Dept(sSal, ptr_tencode, ptr_ecr_no, 21, tsl, s1093);
          break;
      case 'e':                                 //�L�I�H������   21:ñ�� 22:ñ�h
		  Dept(sSal, ptr_tencode, ptr_ecr_no, 22, tsl, s1093);
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

      case 'X':    //XReport Ū�b
          XReport(sSal, sPayStr, ptr_tencode, ptr_ecr_no,
                  iqt5_15217totrev, iam5_15217totrev, iqt5_18219totrev, iam5_18219totrev);
          break;
      case 's':                                 //�N�����
          subsale(tsl, ptr_tencode, ptr_ecr_no);
    }
******/
    return 0;
}


REPORT::~REPORT()
{
    delete tslSaveToFile;
}

/*
string __fastcall REPORT::GetNumber(string S)
{
    string str_num;

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


string __fastcall REPORT::ToDateFormat(string S)
{
    string str_date_format;

    str_date_format = S;
    str_date_format.insert(2, "-");
    str_date_format.insert(5, ":");
    return str_date_format;
}

string __fastcall REPORT::InsertSpace(string S, int i)
{
    int int_len = 0;

    int_len = S.length();

    int int_space = (24 - int_len - i);

    string str_space = S;

	//for (int i = 0; i<int_space; i++)  //str.insert(str.end(), 3, '.');
	//	str_space.insert(int_len, " ");

	str_space.insert(str_space.end(), int_space, ' ');

    return str_space;
}

//int __fastcall REPORT::XReport(string &XRData, string sPayStr, char *ptr_tencode, char *ptr_ecr_no)

// Ū�� 1096 data
int __fastcall REPORT::XReport(string &XRData, string sPayStr, const char *ptr_tencode, const char *ptr_ecr_no,
                             int iqt5_15217totrev, int iam5_15217totrev,
                             int iqt5_18219totrev, int iam5_18219totrev )
{

    //string str_bg_noclose;         //�Ǹ�
    //str_bg_noclose.sprintf("%06d", _StrToInt(XRData.SubString(2,5), "XRData"));

    string str_tencode,str_ecr_no,StrLine,sTmp,sStDDHHMM,sPtDDHHMM;
    str_tencode = ptr_tencode;
    str_ecr_no = ptr_ecr_no;


    StrLine="";
	_Strsprintf(StrLine,"%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, "00000",
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr(("�Ǹ��G000000"));
    //to_ptr(("�����G" + str_tencode + "     �����G" + str_ecr_no));
    to_ptr("");

    to_ptr("                X�b��        ");
    to_ptr("");

    sTmp=_StringSegment_EX(XRData, SGM, 10)+_StringSegment_EX(XRData, SGM, 11);
    sStDDHHMM=sTmp.substr(6,6); //����
    sTmp=_StringSegment_EX(XRData, SGM, 8)+_StringSegment_EX(XRData, SGM, 9);
    sPtDDHHMM=sTmp.substr(6,6); //�e��

	_Strsprintf(StrLine, "���b�ɶ�  ���� %s �e�� %s",
                     ToDateFormat(sStDDHHMM).c_str(),
                     ToDateFormat(sPtDDHHMM).c_str()  );

    //StrLine.sprintf("���b�ɶ�   ���� %s  �e�� %s",
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 11) ).c_str(),
    //                 ToDateFormat(_StringSegment_EX(XRData, "|", 9) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�������~��%7s ����ӫȼ�%7s",
                     GetNumber(_StringSegment_EX(XRData, SGM, 13) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, SGM, 12) ).c_str() );
    to_ptr(StrLine);

    // 2005/07/01
    // to_ptr(("�N���ӫ~�ȼ�" + InsertSpace(GetNumber(XRData.SubString(1887,8)),12)));
    //to_ptr("");

	_Strsprintf(StrLine, "���鵲�b�֭p���B%13s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 14)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�o�����b���B%17s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 19)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "���}�o�����B%17s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 82)).c_str());
    to_ptr(StrLine);

    //  FFU4  1887
    //  FFU4  1895 ���B

	_Strsprintf(StrLine, "�o�����b   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(XRData, SGM, 17) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, SGM, 18) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�~,�h/��   %8s(�K)  %8s(��)",
                     GetNumber(_StringSegment_EX(XRData, SGM, 21) ).c_str(),
                     GetNumber(_StringSegment_EX(XRData, SGM, 22) ).c_str()  );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�~,�h/���X�p%17s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 23)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "���}�o���~,�h/���X�p%9s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 83)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�����ھP�f���J%13s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 24)).c_str());
    to_ptr(StrLine);

    // 2015/08/19 Update Lu
	_Strsprintf(StrLine, "�N��,����O�ӫȼ�%12s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 177)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�P����B(�K)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 69)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�P����B(��)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 175)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�@�o���B(�K)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 70)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��,����O�@�o���B(��)%6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 176)).c_str());
    to_ptr(StrLine);
	_Strsprintf(StrLine, "�N��ӫ~�������B       %6s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 47)).c_str());
    to_ptr(StrLine);

    //StrLine.sprintf("���d���X�p%20s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 86)).c_str() );
    //to_ptr(StrLine);

	_Strsprintf(StrLine, "�N���X�p%11s%8s  %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 77)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 78)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("�N��X�p%11s %8d   %8d"," ", iqt5_15217totrev, iam5_15217totrev );
	_Strsprintf(StrLine, "�N��ӫ~�X�p%7s%8d  %8d", " ", iqt5_15217totrev, iam5_15217totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�Y���ʥN��X�p%15s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 87)).c_str());
    to_ptr(StrLine);

    //�Y���ʦX�p
    int iVDCTx, iVDCNx;
    iVDCTx=_StrToInt( _StringSegment_EX(XRData, SGM, 169) );
    iVDCNx=_StrToInt( _StringSegment_EX(XRData, SGM, 170) );
    //StrLine.sprintf("�Y���ʻP�P��i��%14s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 169) ).c_str() );
	_Strsprintf(StrLine, "�Y���ʻP�P��i��%13s%8d", " ", iVDCTx + iVDCNx);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "����O�X�p%19s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 86)).c_str());
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N�I�X�p%11s%8d  %8d", " ", iqt5_18219totrev, iam5_18219totrev);
    to_ptr(StrLine);

	_Strsprintf(StrLine, "����   %8s   %10s %8s", GetNumber(_StringSegment_EX(XRData, SGM, 40)).c_str(),
                                             "§��",
                                              GetNumber(_StringSegment_EX(XRData, SGM, 30)).c_str() );
    to_ptr(StrLine);

    //StrLine.sprintf("��l�B�� %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 38)).c_str(),
    //                                         "�����",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 34)).c_str()  );
	_Strsprintf(StrLine, "��l�B��%7s   %10s %8s", GetNumber(_StringSegment_EX(XRData, SGM, 38)).c_str(),
                                             "§���d",
                                              GetNumber(_StringSegment_EX(XRData, SGM, 174)).c_str()  );
    to_ptr(StrLine);


    //StrLine.sprintf("��d�X�p %8s   %10s %8s",GetNumber(_StringSegment_EX(XRData, "|", 74)).c_str(),
    //                                         "�N�{���X�p",
    //                                          GetNumber(_StringSegment_EX(XRData, "|", 36)).c_str()  );
	_Strsprintf(StrLine, "��d�X�p%7s   %10s %8s", GetNumber(_StringSegment_EX(XRData, SGM, 74)).c_str(),
                                             "�����",
                                              GetNumber(_StringSegment_EX(XRData, SGM, 34)).c_str()  );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�N�{���X�p%19s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 36)).c_str());
    to_ptr(StrLine);


    //to_ptr("���O��X�p");
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1871,8)),0));  //1871

    //StrLine.sprintf("�{���X�p%22s %8s"," ",GetNumber(_StringSegment_EX(XRData, "|", 45)).c_str() );
    //to_ptr(StrLine);


     //20151208 ���a�ݨD�ܧ�A�վ�Z�b���w��ܦ�m
	_Strsprintf(StrLine, "���w�h��%11s    %14s", " ",
                                             //GetNumber(_StringSegment_EX(XRData, "|", 71)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 72)).c_str() );
    to_ptr(StrLine);



    int iPayinvoamt, iPayinvoqty;
    iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, SGM, 171) );
    iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, SGM, 172) );
	_Strsprintf(StrLine, "�o���I��%8d  %10s %8s", iPayinvoamt, "�{���X�p", GetNumber(_StringSegment_EX(XRData, SGM, 45)).c_str());
    to_ptr(StrLine);

    //to_ptr( ("�ɹs�X�p" + InsertSpace(GetNumber(XRData.SubString(1639,8)),8)) );
    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1647,8)),0));



	_Strsprintf(StrLine, "��w�X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 50)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 51)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�󥿦X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 52)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 53)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "��������X�p%6s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 54)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 55)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�馩(���|)%8s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 61)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 62)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�ӫ~�զX�馩/��%3s%8s   %8s", " ",
                                                  GetNumber(_StringSegment_EX(XRData, SGM, 63)).c_str(),
                                                  GetNumber(_StringSegment_EX(XRData, SGM, 64)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "�[�Ⱦ� �}/��d%4s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 65)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 66)).c_str() );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�[�Ⱦ��[��%8s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 67)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 68)).c_str() );
    to_ptr(StrLine);

    //int iPayinvoamt, iPayinvoqty;
    //iPayinvoqty=_StrToInt( _StringSegment_EX(XRData, "|", 171) );
    //iPayinvoamt=_StrToInt( _StringSegment_EX(XRData, "|", 172) );
    //StrLine.sprintf("�o���I��%11s %8d   %8d"," ", iPayinvoqty,iPayinvoamt );
   // to_ptr(StrLine);

	_Strsprintf(StrLine, "�}���d%12s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 60)).c_str());
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

	_Strsprintf(StrLine, "�����n��%10s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 58)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 59)).c_str() );
    to_ptr(StrLine);    //to_ptr(InsertSpace(GetNumber(XRData.SubString(1799,8)),0));


	_Strsprintf(StrLine, "�o��%14s%8s   %8s", " ",
                                               GetNumber(_StringSegment_EX(XRData, SGM, 84)).c_str(),
                                               GetNumber(_StringSegment_EX(XRData, SGM, 85)).c_str() );
    to_ptr(StrLine);

	_Strsprintf(StrLine, "���B���o��%8s%8s", " ", GetNumber(_StringSegment_EX(XRData, SGM, 81)).c_str());
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�V�m�X�p%10s%8s   %8s", " ",
                                             GetNumber(_StringSegment_EX(XRData, SGM, 56)).c_str(),
                                             GetNumber(_StringSegment_EX(XRData, SGM, 57)).c_str() );
    to_ptr(StrLine);


	_Strsprintf(StrLine, "�����o��%19s%10s", " ", _StringSegment_EX(XRData, SGM, 16).c_str());
    to_ptr(StrLine);

   // int icpn77cnt,icpn77amt,icpn78cnt,icpn78amt;
   // string sCpnCnt, sCpnAmt;

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
    to_ptr("       ***     "+_StringSegment_EX(XRData, SGM, 79)+ _StringSegment_EX(XRData, SGM, 80)+"     ***");

    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");


    return 0;
}

string __fastcall REPORT::ToTimeFormat(string S)
{
    string str_time;

    str_time = (S.substr(0,4)).insert(2,":");
    return str_time;
}

REPORT::REPORT()
{
    tslSaveToFile = new TStringList;
    char_filename[0] = 0;
	sprintf_s(gchar_date_format, sizeof(gchar_date_format), "%10s", currentDateTime(4).c_str()); //    FormatDateTime("yyyy-mm-dd", Now()).c_str());
}

//�N�����
int __fastcall REPORT::subsale(TStringList *tsl, const char *ptr_tencode, const char *ptr_ecr_no)
{
    string str_tencode, str_ecr_no, StrLine;     //�����B����
    str_tencode = ptr_tencode;
    str_ecr_no  = ptr_ecr_no;
    int int_amt = 0;                        //�N����B�[�`

    //�浧�O���B�ӫ~�W�١B�P����B
    string str_record, str_bar_name, str_item_amt;

    StrLine="";
	_Strsprintf(StrLine,"%s�Ǹ�:%s����:%s ��:%s", gchar_date_format, "00000",
		str_tencode.c_str(), str_ecr_no.c_str());
    to_ptr(StrLine);

    //to_ptr(gchar_date_format);
    //to_ptr("�Ǹ��G000000");
    //to_ptr(("�����G" + str_tencode + "     �����G" + str_ecr_no));
    to_ptr("");

    to_ptr("           �N��ӫ~���B����     ");
    to_ptr("");

	_Strsprintf(StrLine, "��%s�ӫ~�W��%17s�P����B", " ", " ");
    to_ptr(StrLine);
    int int_space = 1;
	int i = 0;
    //for (int i=0; i<tsl->Count; i++)
	for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++, i++)
    {
		str_record = *ls; // tsl->Strings[i];
        str_bar_name = _StringSegment_EX(str_record, "|", 3);
        str_item_amt = _StringSegment_EX(str_record, "|", 4);

        int_amt += _StrToInt(str_item_amt);

        int_space = 1;
        while (int_space != 0)  //�R�����Ϊť�
        {
			int_space = _Pos(str_bar_name, "�@");
            if (int_space)
            {
                str_bar_name.erase(int_space,2);
            }

        }
        int_space = 1;
        while (int_space != 0)  //�R���b�Ϊť�
        {
			int_space = _Pos(str_bar_name," ");
            if (int_space)
            {
                str_bar_name.erase(int_space,1);
            }

        }

		_Strsprintf(str_record,"%02d.%-26s%7s", i + 1, str_bar_name.substr(0, 26).c_str(),
                                                    str_item_amt.c_str());
        to_ptr(str_record);

        //str_record.sprintf("%02d. %-20s",i+1 ,str_bar_name.SubString(1,20).c_str());
        //to_ptr(str_record);
        //str_record.sprintf("%24s",str_item_amt.c_str());
        //to_ptr(str_record);
    }

    to_ptr("---------------------------------------");

    //str_record.sprintf("%15s    �X�p%16d"," ",int_amt);
	_Strsprintf(str_record,"�X�p%15s %16d", " ", int_amt);
    to_ptr(str_record);

	// 2019/11/06 For SCO
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
    to_ptr("");
	_Strsprintf(StrLine, "\x1b\x69");
    to_ptr(StrLine);

    //�g�J�ɮ�
    to_ptr("Save");

    return 0;
}
