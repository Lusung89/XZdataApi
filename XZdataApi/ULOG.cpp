//---------------------------------------------------------------------------


#include "stdafx.h"
#include <iterator>
#include "ULOG.h"


//#include <sys\stat.h>

string  __stdcall LTrim(const string& str)
{
	
	string rtns = str;
	try
	{
		rtns.erase(0, rtns.find_first_not_of(" "));
		//rtns = str.substr(str.find_first_not_of(" \n\r\t"));
	}
	catch (...)
	{
		rtns = str;
	}
	return rtns; 
}

string  __stdcall RTrim(const string& str)
{
	string rtns = str;
	try
	{
		//str.substr(0, str.find_last_not_of(" \n\r\t") + 1);
		//rtns = str;
		rtns.erase(rtns.find_last_not_of(" \n\r\t") + 1);
	}
	catch (...)
	{
		rtns = str;
	}
	return rtns;
	
}


string  __stdcall Trim(const string& str)
{
	string lstr = str; 
	if (str.empty() || str.length() <= 0)
		return str;
	else
		return LTrim(RTrim(lstr));
}

string  __stdcall Trim(const LPSTR  str)
{
	return(string(str));
}



string __stdcall int2str(int i)
{
	string s;
	stringstream ss(s);
	ss << i;
	return ss.str();
}



bool __stdcall DirExists(const std::string& dirName_in)
{
  int ftyp = _access(dirName_in.c_str(), 0);
  if (0 == ftyp)
		return true;   // this is a directory!  
  else
		return false;    // this is not a directory!  
}

string __stdcall TcharToString(TCHAR *pTstr)
{
	std::wstring wStr = pTstr;
	std::string str = std::string(wStr.begin(), wStr.end());
	return str;

}

bool __stdcall _StrFind(const std::string &str, const std::string &sfind)
{
	bool brtn = false;
	if (str.find(sfind) != std::string::npos)
		brtn = true;
	return brtn;
}

int __stdcall _Pos(const std::string &str, const std::string &sfind)
{
	std::size_t found;

	found = str.find(sfind);

	if (str.find(sfind) == std::string::npos)
		found = 0;
	return found;
}




std::wstring __stdcall s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string __stdcall ws2s(const std::wstring& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    std::string r(len, '\0');
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
    return r;
}



string __stdcall WStringToString(const std::wstring& wstr)
{
    /***/
  std::string str;
  CW2A cw2a(wstr.c_str());
  str = cw2a;
  return str;
  /***/

    /**
    LPCWSTR pwszSrc = wstr.c_str();
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen == 0)
        return std::string("");

    char* pszDst = new char[nLen];
    if (!pszDst)
        return std::string("");

    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    std::string str(pszDst);
    delete[] pszDst;
    pszDst = NULL;
     return str;
    **/
   

}


wstring __stdcall StringToWString(const std::string& s)
{
	//fun1  無法對中文進行轉換, 因為是以 Byte ->  Unicode
	//std::wstring wsTmp(s.begin(), s.end());
	
	//fun2 Word To Unicode
	CA2W ca2w(s.c_str()); 
	wstring wsTmp = ca2w;
	return wsTmp;

}



//變更副檔名
string __stdcall _ChangeFileExt(const std::string& str, string& newExt)
{
	string s = str;

	string::size_type i = s.rfind('.', s.length());

	if (i != string::npos) {
		s.replace(i , newExt.length(), newExt);
	}

	return s;
}



// 取得檔案不含路徑 
string __stdcall _ExtractFile(string str)
{
	std::wstring wStr = L"";
	string sFileNameNonPath = "";
	wStr = PathFindFileName(StringToWString(str).c_str());     //檔名不包含路徑
	CW2A cw2a(wStr.c_str());
	sFileNameNonPath = cw2a;
	return sFileNameNonPath;
}




// 取得檔案路徑 
string __stdcall _ExtractFilePath(string str)
{
	string strrtn = "";
	try
	{
	    const size_t last_slash_idx = str.find_last_of('\\');
    	if (std::string::npos != last_slash_idx)
	       {
		      strrtn = str.substr(0, last_slash_idx) + "\\";
	      }
    }
	catch (...)
	{
		writelog("_ExtractFilePath: Catch error =>" + str);
	}
	

	/*DWORD  retval = 0;
	TCHAR  buffer[1024] = TEXT("");
	TCHAR** lppPart = { NULL };
	retval = GetFullPathName(StringToWString(str).c_str(),
		1024,
		buffer,
		lppPart);

	if (retval == 0)
	{
		strrtn = "";
	}
	else
	{
		
		strrtn = TcharToString(buffer);
	}*/

	return(strrtn);

}




//#pragma hdrstop
//#pragma package(smart_init)
//---------------------------------------------------------------------------

string  __stdcall currentDateTime(int iDttmTypw, int OffsetDay) 
{
	time_t   result, now;
	struct tm  tstruct;
	char       buf[80];
	
	now = time(0);
	localtime_s(&tstruct, &now);

	
	if (OffsetDay != 0)
	{
		tstruct.tm_mday += OffsetDay;
		if ((result = mktime(&tstruct)) != (time_t)-1)
		{
			localtime_s(&tstruct, &result);
		}
		else
		{ 
			 localtime_s(&tstruct, &now);
		}
	}
			
    // for more information about date/time format
	switch (iDttmTypw)
	{
	case 0:
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		break;
	case 1:
		strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);  //%H 24小時制的小時 %M 十時製表示的分鐘數 %S 十進制的秒數
		break;
	case 2:
		strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
		break;
	case 3:
		strftime(buf, sizeof(buf), "%X", &tstruct);      
		break;
	case 4:
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		break;
	
	default:
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	}


	return buf;
	/******************

	struct tm when;
	time_t now, result;
	int    days;

	time( &now );
	when = *localtime( &now );
	printf( "Current time is %s\n", asctime( &when ) );
	printf( "How many days to look ahead: " );
	scanf( "%d", &days );

	when.tm_mday = when.tm_mday + days;
	if( (result = mktime( &when )) != (time_t)-1 )
	printf( "In %d days the time will be %s\n",
	days, asctime( &when ) );
	else
	perror( "mktime failed" );




    	%a 星期幾的簡寫

		%A 星期幾的全稱

		%b 月分的簡寫

		%B 月份的全稱

		%c 標準的日期的時間串

		%C 年份的後兩位數字

		%d 十進製表示的每月的第幾天

		%D 月 / 天 / 年

		%e 在兩字符域中，十進製表示的每月的第幾天

		%F 年 - 月 - 日

		%g 年份的後兩位數字，使用基於周的年

		%G 年分，使用基於周的年

		%h 簡寫的月份名

		%H 24小時制的小時

		% I 12小時制的小時

		% j 十進製表示的每年的第幾天

		%m 十進製表示的月份

		%M 十時製表示的分鐘數

		%n 新行符

		%p 本地的AM或PM的等價顯示

		%r 12小時的時間

		% R 顯示小時和分鐘：hh : mm

		%S 十進制的秒數

		%t 水平製表符

		%T 顯示時分秒：hh : mm : ss

		%u 每週的第幾天，星期一為第一天 （值從0到6，星期一為0）

		%U 第年的第幾周，把星期日做為第一天（值從0到53）

		%V 每年的第幾周，使用基於周的年

		%w 十進製表示的星期幾（值從0到6，星期天為0）

		%W 每年的第幾周，把星期一做為第一天（值從0到53）

		%x 標準的日期串

		%X 標準的時間串

		%y 不帶世紀的十進制年份（值從0到99）

		%Y 帶世紀部分的十制年份

		%z，%Z 時區名稱，如果不能得到時區名稱則返回空字符。

		%% 百分號

**************/

}





void  __stdcall writelog(LPSTR pstr)
{
	writelog(string(pstr));

}

void  __stdcall writelog(string S)       //記錄LOG
{
    string str_now, stmp;

	str_now = currentDateTime(0); // FormatDateTime("yyyy-mm-dd hh:nn:ss", Now());
	   
	fstream file;

	string str_path = XZDATADIR;
	string str_ecr;

   
    try
    {
        //tifAbacus = new TIniFile("C:\\ABACUS\\ABACUS.INI");
        //int_ecr = tifAbacus->ReadInteger("ECR","gchr_ecr_no",0);
		str_ecr = "XZDATA"; //   sprintf("XZDATA"); //, atoi(gchar_ecr_no));

		string str_file = str_path + str_ecr + ".log";

		file.open(str_file.c_str(), ios::out | ios::app);
		if (file)
		{
			//file.open(log_name,ios::out | ios::app /*| ios::binary*/ );
			//dttm2 += " " +  string(pStr) + "\a\n";
			//dttm2.append(" ");
			//dttm2.append(string(pStr) );
			file << str_now.c_str() << " " << S.c_str() << "\n";
		}

    }
	catch (...)
    {
		;;
		
    }

	file.close();
}



string __stdcall _Strsprintf(std::string &str, char* format, ...)
{
	char buffer[10240];
	va_list argList;

	va_start(argList, format);
	vsnprintf_s(buffer, sizeof(buffer), format, argList);
	va_end(argList);

	str = string(buffer);
	return(str);
}


//void __stdcall logsprintf(const string& format, ...)
void __stdcall logsprintf(char* format, ...)
{
	char buffer[10240];
	va_list argList;

	va_start(argList, format);
	vsnprintf_s(buffer, 10240, format, argList);
	va_end(argList);

	writelog(buffer);
}

template< typename T, typename A >
void _ListToFile(const std::list<T, A>* seq, const char* path2file)
{
	try
	{
		std::ofstream file(path2file);
		std::copy(seq->begin(), seq->end(), std::ostream_iterator<T>(file, "\r"));
		file.close();
	}
	catch (...)
	{
		logsprintf("_ListToFile catch error ...");
	}

    //_ListToWFile( (TStringList *) seq , string(path2file));

}

// for Unicode file #include <codecvt>
void _ListToWFile(TStringList* seq, string path2file)
{
    try
    {
        string s;
        wstring wfile;
        wfile=StringToWString(path2file);

        // Write file in UTF-8  Start Byte 嚜?:EF BB BF
        std::wofstream wof;
        wof.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
        wof.open(wfile.c_str() ); // (L"example.txt");
       // wof << L"This is a test. \n";
       // wof << L"XXXXXXXXX \n";
       // wof << L"XXXXXXXXXXXXXXXXXX \n";

        for (list<string>::iterator ls = seq->begin(); ls != seq->end(); ls++)
        {
            s = *ls + "\n"; 
            wof << StringToWString(s);
        }
        wof.close();
    }
    catch (...)
    {
        logsprintf("_ListToWFile catch error ...");
    }

}


// for Unicode file #include <codecvt>
int _wListLoadFromFile(TStringList* seq, string path2file)
{
    string str_sal_data, sfname;
    wstring MEcode;
    wstring wLine;

    sfname = path2file;
    int ifileType;
    //TypeANSI = 0,
    //TypeUNICODE = 1,
    //TypeUTF8 = 2,
    ifileType = GetTextFileType(sfname);
    if (ifileType == 1)
        MEcode = L"r, ccs = UNICODE";
    else if (ifileType == 2)
        MEcode = L"r, ccs = UTF-8";
    else
        MEcode = L"r, ccs = UTF-8";

    writelog("_wListLoadFromFile: InPut File:" + sfname + ", FILETYPE=" + WStringToString(MEcode));

    std::wifstream wif; // StringToWString(FileName).c_str(), std::ios::binary);
   
    try
    {
        wif.open(StringToWString(sfname).c_str());
        if (ifileType == 1)
            wif.imbue(std::locale(std::locale(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
        else
            wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

        if (wif.is_open())
        {
            wLine.clear();
            while (std::getline(wif, wLine))                //讀SAL檔資料
            {
                str_sal_data = WStringToString(wLine);
                wLine.clear();
                seq->push_back(str_sal_data);

            }

        }

        wif.close();
    }
    catch (...)
    {
        logsprintf("_wListLoadFromFile: catch error ...");
    }

    return seq->size();
}



template< typename T, typename A >
int _ListLoadFromFile(std::list<T, A>* seq, const char* path2file)
{
	FILE *f_sal_file;
	//CHAR c_data_buf[MAXDATALEN];                       //讀出每行資料暫存字串
	string str_sal_data, sfname;
    wstring MEcode;
    CHAR* c_data_buf;
    c_data_buf = (CHAR*)malloc(MAXDATALEN);
  
    sfname = string(path2file);
    writelog("_ListLoadFromFile: InPut File:" + sfname );

	try
	{

		errno_t err;
        if ((err = fopen_s(&f_sal_file, path2file, "rb")) != 0)
           {
                writelog("_ListLoadFromFile:開啟交易檔案發生錯誤 " + string(path2file));
                free(c_data_buf);
                return 0;
           }


		if (f_sal_file != NULL)
		{
            while (!feof(f_sal_file))               //讀SAL檔資料
            {
               
                  memset(c_data_buf, 0x00, MAXDATALEN);
                  fgets(c_data_buf, MAXDATALEN, f_sal_file);
                  fscanf_s(f_sal_file, "\n");
                  seq->push_back(string(c_data_buf));
               
			}
		}

        fclose(f_sal_file); 

	}
	catch (...)
	{
		logsprintf("_ListLoadFromFile: catch error ...");
	}

    free(c_data_buf);
	return seq->size();

}



int __stdcall _SaveZCntToFile(string InPutSalFile, int Z_Cnt, string OutPutSalFile)
{

	TStringList *tmpSal = new TStringList;
	TStringList *outSal = new TStringList;
	string sRec, saledata, sRecNo, sTmpDttm, TmpZcnt, stmp;
	saledata = "";
	int NoUsedData = 0;
	logsprintf("_SaveZCntToFile: InPutSalFile=%s,  Z_Cnt=%d, OutPutSalFile=%s", InPutSalFile.c_str(), Z_Cnt, OutPutSalFile.c_str());
	outSal->clear();

	_ListLoadFromFile(tmpSal, InPutSalFile.c_str());  //LoadFromFile(tmpSa, InPutSalFile);

	if (tmpSal->size() > 0)
	{
		//for (i = 0; i<tmpSal->Count; i++)
		for (list<string>::iterator ls = tmpSal->begin(); ls != tmpSal->end(); ls++)
		{
			sRec = *ls; // tmpSal->Strings[i];
			sRecNo = _StringSegment_EX(sRec, "|", 1);
			sTmpDttm = _StringSegment_EX(sRec, "|", 5);
			TmpZcnt = _StringSegment_EX(sRec, "|", 6);  //日結序號
			//if(sRecNo=="1096")
			//  {
			//    NoUsedData=1;
			//  }
			if (sRecNo == "1001")
			{
				saledata = "";
				_Strsprintf(stmp, "%s%c%c", sRec, 0x0d, 0x0a);
				saledata += stmp;
			}
			else if (sRecNo == "1099")
			{
				//stmp.sprintf("%s%c%c",sRec, 0x0d, 0x0a);
				if (NoUsedData != 1)
				{
					_Strsprintf(stmp, "%s", sRec);
					saledata += sRec;
					try
					{
						if (_StrToInt(TmpZcnt) == Z_Cnt)
							outSal->push_back(saledata);
					}
					catch (...)
					{
						saledata = "";
					}
				}
				saledata = "";
				NoUsedData = 0;
			}
			else
			{
				_Strsprintf(stmp, "%s%c%c", sRec, 0x0d, 0x0a);
				saledata += stmp;
			}
		} //end of for
	}
	//str_now = FormatDateTime("ddhhnnss",Now());
	//CopyFile(LoadSalName.c_str(), Trim(LoadSalName+str_now).c_str() , false);
	//_ListToFile(outSal, OutPutSalFile.c_str());   //outSal->SaveToFile(OutPutSalFile);
    _ListToWFile(outSal, OutPutSalFile); //Unicode

	logsprintf("_SaveZCntToFile: SaveToFile(%s) OK", OutPutSalFile.c_str());

	delete tmpSal, outSal;

	return 1;
}



int __stdcall file_cat(const char *sour, const char *targ)
{
   int in, out, buf_len;
   char *buf;
   buf = (char *)malloc(1026);

__try
   {
   //if(( in = _sopen_s(sour, O_RDONLY | O_BINARY, S_IREAD)) == -1 )
   //if ((_sopen_s(&in, sour, _O_RDWR | _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE)) == -1)
   if ((_sopen_s(&in, sour, O_RDONLY | O_BINARY, _SH_DENYNO, _S_IREAD )) == -1)
     {
     //  ShowMessage("pos_cat: source file can't open ");
       return(-1);
     }

   //if(( out = open( targ, O_CREAT | O_BINARY | O_WRONLY | O_APPEND, S_IWRITE)) == -1)
   if ((_sopen_s(&out, targ, _O_CREAT | O_BINARY | O_WRONLY | O_APPEND, _SH_DENYNO, _S_IREAD | _S_IWRITE)) == -1)
     {
     //  ShowMessage("pos_cat :targ file cann't open" );
       return( -1);
      }

    while( !_eof(in))
         {
           buf_len = _read(in,buf,1024);
           _write( out, buf, buf_len);
         }
   }
__finally
  {
    _close(in);
    _close(out);
    free(buf);

  }
    return(1);
 }


//TypeANSI = 0,
//TypeUNICODE = 1,
//TypeUTF8 = 2,
int __stdcall GetTextFileType(const std::string & FileName)
{
	int ifileType = 0;
	std::ifstream file;
	file.open(FileName.c_str(), std::ios_base::in);
	bool bUnicodeFile = false;
	if (file.good())
	{
		char szFlag[3] = { 0 };
		file.read(szFlag, sizeof(char) * 3);
		if ((unsigned char)szFlag[0] == 0xFF && (unsigned char)szFlag[1] == 0xFE)
		{
			ifileType = 1;
		}
		else
			if ((unsigned char)szFlag[0] == 0xEF && (unsigned char)szFlag[1] == 0xBB && (unsigned char)szFlag[2] == 0xBF)
			{
				ifileType = 2;
			}
	}
	file.close();
	return ifileType;
}


//---------------------------------------------------------------------------
//依照分隔符號,取出分隔符號前的字串中
// "AAAAAA,BBBBBB,CCCCCC,DDDDDD,EEEEE," ; Seg(1)=AAAAAA, ~ Seg(5)=EEEEE
/// <summary>  _StringSegment
///  擷取字串 Str , 分割字串 Comma: ..,|,... Seg(1~N) 不適用含 UTF8, Big5
/// </summary>
/// <param name="Str"></param>
/// <returns>Void</returns>
string __stdcall _StringSegment(string &Str, string delimiter, int Seg, int idelStr)
{
	if ((Str.empty()) || (delimiter.empty()) || (Seg<1)) 
		return "";
	
	size_t pos = 0;
	string token = "";
	string s = Str;
	int idx = 1;      // start index=1 EX:Seg(1)=AAAAAA
	int iSegCnt = (-1 + idx);

	if (delimiter == "") delimiter = "|";

	try
	{
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			s.erase(0, pos + delimiter.length());
			iSegCnt++;
			if (iSegCnt == Seg)
				break;
		}

		if (iSegCnt != Seg)
			token = s;

		if (idelStr)
			Str = s;
	}
	catch (...)
	{
		token = "";
	}

	return token;
}



//vector <wstring> __stdcall _StringSegment_EXVT(string s, string delimiter, int Seg, int idelSt)
//---------------------------------------------------------------------------
//依照分隔符號,取出分隔符號前的字串中
// "AAAAAA,BBBBBB,CCCCCC,DDDDDD,EEEEE," ; Seg(1)=AAAAAA, ~ Seg(5)=EEEEE
/// <summary>  _WStringSegment
///  擷取字串 Str , 分割字串 delimiter: ..,|,...  Seg(1~N)  適用含 UTF8, Big5
/// </summary>
/// <param name="Str"></param>
/// <returns>Void</returns>
string __stdcall _WStringSegment(string Str, string delimiter, int Seg, int idelSt)
{
	if ((Str.empty()) || (delimiter.empty()) || (Seg<1)) return "";
	
	wstring wtoken;
	int Stidx = 1;      // start index=1 EX:Seg(1)=AAAAAA
	try
	{
		wstring ws = StringToWString(Str);
		wstring wdelimiter = StringToWString(delimiter);
		//size_t pos_start = 0, pos_end, delim_len = wdelimiter.length();
		wstring::size_type  pos_start = 0, pos_end, delim_len = wdelimiter.length();
		
		vector<wstring> wres;
		wres.clear();
		while ((pos_end = ws.find(wdelimiter, pos_start)) != wstring::npos) {
			wtoken = ws.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			wres.push_back(wtoken);
		}
		wres.push_back(ws.substr(pos_start));
		if ( (Seg - Stidx) >= wres.size() || wres.size() == 0 )
			wtoken = L"";
		else
		    wtoken = wres[Seg - Stidx];
	}
	catch (...)
	{
		wtoken = L"";
	}
	return (WStringToString(wtoken));
	
}



wstring __stdcall _WStringSegmentW(wstring Str, wstring delimiter, int Seg, int idelSt)
{
    if ((Str.empty()) || (delimiter.empty()) || (Seg < 1)) return L"";

    wstring wtoken;
    int Stidx = 1;      // start index=1 EX:Seg(1)=AAAAAA
    try
    {
        wstring ws = Str;
        wstring wdelimiter = delimiter;
        wstring::size_type  pos_start = 0, pos_end, delim_len = wdelimiter.length();

        vector<wstring> wres;
        wres.clear();
        while ((pos_end = ws.find(wdelimiter, pos_start)) != wstring::npos) {
            wtoken = ws.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            wres.push_back(wtoken);
        }
        wres.push_back(ws.substr(pos_start));
        if ((Seg - Stidx) >= wres.size() || wres.size() == 0)
            wtoken = L"";
        else
            wtoken = wres[Seg - Stidx];
    }
    catch (...)
    {
        wtoken = L"";
    }
    return (wtoken);

}


//std::vector<std::string> split(const std::string& s, char delimiter)
//{
//	std::vector<std::string> tokens;
//	std::string token;
//	std::istringstream tokenStream(s);
//	while (std::getline(tokenStream, token, delimiter))
//	{
//		tokens.push_back(token);
//	}
//	return tokens;
//}




//std::string s = "scott>=tiger>=mushroom";
//std::string delimiter = ">=";
//
//size_t pos = 0;
//std::string token;
//while ((pos = s.find(delimiter)) != std::string::npos) {
//	token = s.substr(0, pos);
//	std::cout << token << std::endl;
//	s.erase(0, pos + delimiter.length());
//}
//std::cout << s << std::endl;

//OUTPut
//scott
//tiger
//mushroom
//


int __stdcall _GetZCntLst(TStringList *ZCntLst, TStringList *S1050Lst )
{

  string sRec, TmpZcnt, sTmpDttm, OrgZcnt, sRecNo, sTmp;
  //CHAR cLog[1024];

  try
  {
       //sTmlLst->Assign(Auto_Sal);
    if( S1050Lst->size() > 0)
      {
		ZCntLst->clear();
        OrgZcnt="";

		for (list<string>::iterator s = S1050Lst->begin(); s != S1050Lst->end(); s++)
		    {
			 sRec = *s; //S1050Lst[i];
             sRecNo=_StringSegment_EX(sRec, SGM, 1);

             if( sRecNo != "3099" )     //if( RecNo=="1802" || RecNo=="1805" || RecNo=="1807" )
                  continue;

             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt !=  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    if( !_GetZCntLstCheck(ZCntLst, TmpZcnt) )
                      {
						ZCntLst->push_back(sRec);   //Add(sRec);
                        OrgZcnt=TmpZcnt;
						logsprintf("_GetZCntLst: 加入日結序號 =%s,(%s)", TmpZcnt.c_str(), sRec.c_str());
						//writelog(string(cLog));
                      }
                    else
                      {
                        // ZCntLst->Add(sRec);
                        OrgZcnt=TmpZcnt;
						logsprintf("_GetZCntLst: 日結序號重複 =%s,(%s)", TmpZcnt.c_str(), sRec.c_str());
						//writelog( string(cLog) );
                      }
                 }
             }
       }
   }
 catch(...)
   {
     writelog("_GetZCntLst Catch error...");
   }


 logsprintf("_GetZCntLst: 加入日結序號筆數 =%d", ZCntLst->size());
 //writelog(cLog);

 return(ZCntLst->size());

}



int __stdcall _GetZCntLstCheck(TStringList *ZCntLst, string OrgZcnt)
{
 string  sRec, TmpZcnt, sTmpDttm;
 int ifind;
 ifind=0;
 //CHAR cLog[1024];
 try
   {

	   /*for (list<string>::iterator s = S1050Lst->begin(); s != S1050Lst->end(); s++)
	   {
	   cout << " " << *s;
	   }*/


	   for (list<string>::iterator s = ZCntLst->begin(); s != ZCntLst->end(); s++) //for (int i = 0; i<ZCntLst->Count; i++)
            {
			 sRec = *s;  //ZCntLst->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);

             if(  TmpZcnt ==  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    ifind=1;
					logsprintf("GetZCntLstCheck: 日結序號重複 TmpZcnt=%s, OrgZcnt=%s ", TmpZcnt.c_str(), OrgZcnt.c_str());
					//writelog(cLog);
                    break;
                 }
             }
     }
 catch(...)
    {
       ifind=0;
	   logsprintf("GetZCntLstCheck: Catch Error .. OrgZcnt=%s ", OrgZcnt.c_str());
	   //writelog(cLog);
    }

  return( ifind );

}


//// 檢查 1050 及 SAL 1050 筆數是否一致
int __fastcall _GetZDTZSALCntCheck(TStringList *ZCntLst, string SalFileName, string OrgZcnt,
                                   int &RtnZDTCnts, int &RtnSALCnts )
{
      string  sRec, TmpZcnt,  sTmpDttm, sTmp, sRecNo;
      int iZdt1050, iSal1050;
      iZdt1050=iSal1050=0;
      RtnZDTCnts=iZdt1050;
      RtnSALCnts=iSal1050;

#pragma region ZCntLst Process
 try
   {
      //for (int i=0;  i<ZCntLst->Count; i++)
	   for (list<string>::iterator ls = ZCntLst->begin(); ls != ZCntLst->end(); ls++)
            {
        	 sRec = *ls;     // ZCntLst->Strings[i];
             sRecNo=_StringSegment_EX(sRec, SGM, 1);
             if( sRecNo != "3050" )
                continue;
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);

             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if(  TmpZcnt ==  OrgZcnt ) // sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    iZdt1050++;
					logsprintf("_GetZDTZSALCntCheck:ZDT(%6d)(%s)", iZdt1050, sRec.substr(0, 526).c_str());
                    //writelog(sTmp);
                 }
             }
     }
 catch(...)
    {
       logsprintf("_GetZDTZSALCntCheck: Catch Error .. OrgZcnt=%s , Zdt1050=%d",OrgZcnt.c_str(), iZdt1050 );
     }
 logsprintf("_GetZDTZSALCntCheck: ZDT Check .. (Z_CNT=%s) OK Total Cnts (%6d)", TmpZcnt.c_str(), iZdt1050);
#pragma endregion


#pragma region  TmpSalLst Process

  TStringList *TmpSalLst =  new TStringList;
  logsprintf("_GetZDTZSALCntCheck: LoadFromFile(%s)", SalFileName.c_str());
  _ListLoadFromFile(TmpSalLst, SalFileName.c_str());               // TmpSalLst->LoadFromFile(SalFileName);

  try
   {
       //  for (int i=0;  i<TmpSalLst->Count; i++)
	   for (list<string>::iterator ls = TmpSalLst->begin(); ls != TmpSalLst->end(); ls++)
            {
				sRec = *ls;  //TmpSalLst->Strings[i];
                sRecNo=_StringSegment_EX(sRec, SGM, 1);
                if( sRecNo != "3050" )
                  continue;
                TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
                sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
                //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
                if(  TmpZcnt ==  OrgZcnt )                //sTmpDttm.SubString(1,8) <= AutoZdt)
                 {
                    iSal1050++;
					logsprintf("_GetZDTZSALCntCheck:SAL(%6d)(%s)", iSal1050, sRec.substr(0, 526).c_str());
                 }
             }
     }
 catch(...)
    {
		logsprintf("_GetZDTZSALCntCheck: Catch Error %s.. OrgZcnt=%s , iSal1050=%d", SalFileName.c_str(), OrgZcnt.c_str(), iSal1050);
    }

 logsprintf("_GetZDTZSALCntCheck:SAL Check (Z_CNT=%s) OK Total Cnts (%6d)", TmpZcnt.c_str(), iSal1050);
#pragma endregion

  RtnZDTCnts=iZdt1050;
  RtnSALCnts=iSal1050;

  if( RtnZDTCnts == RtnSALCnts )
	  logsprintf("_GetZDTZSALCntCheck: **** 檢查 1050 及 SAL 1050 筆數一致    (Z_CNT=%s) (ZDT=%6d), (SAL=%6d) ", TmpZcnt.c_str(), iZdt1050, RtnZDTCnts);
  else
	  logsprintf("_GetZDTZSALCntCheck: **** 檢查 1050 及 SAL 1050 筆數不一致  (Z_CNT=%s) (ZDT=%6d), (SAL=%6d) ", TmpZcnt.c_str(), iZdt1050, RtnSALCnts);

  delete TmpSalLst;
  return( 0 );


}



/// <summary>  BASIC::GetZCNT_SalData    
///  依 ZCnt 將 SAL 載入 List
/// </summary>
/// <param name="AutoZCnt"></param>
/// <returns>int</returns>
int __fastcall  BASIC::GetZCNT_SalData(string AutoZCnt)
{
  string sTmp, sRec, sTmpDttm,TmpZcnt, SalZcnt;
  int irtn=0;
  SalZcnt=AutoZCnt;
	
 try
   {
#pragma region  Autotsl_rec to gtsl_rec
    gtsl_rec->clear();
    if( Autotsl_rec->size() > 0)
       {
        //for (int i=0;  i<Autotsl_rec->Count; i++)
		for (list<string>::iterator s = Autotsl_rec->begin(); s != Autotsl_rec->end(); s++)
            {
			 sRec = *s;  //Autotsl_rec->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
            
             if( TmpZcnt==SalZcnt )
               {
                 gtsl_rec->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_rec->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion

#pragma region Autotsl_tot to gtsl_tot
    gtsl_tot->clear();
    if( Autotsl_tot->size() > 0)
       {
        //for (int i=0;  i<Autotsl_tot->Count; i++)
	   for (list<string>::iterator s = Autotsl_tot->begin(); s != Autotsl_tot->end(); s++)
            {
			 sRec = *s;  //Autotsl_tot->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
 
             if( TmpZcnt==SalZcnt )
               {
                 gtsl_tot->push_back(sRec);
                 //logsprintf("AUTOZDATA::GetYYYYMDD_SalData:(%s)", sRec.c_str() );
               }
             else
               {
                 ;;//gtsl_tot->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion


#pragma region Autotsl_drop to gtsl_drop

    gtsl_drop->clear();
    if( Autotsl_drop->size() > 0)
       {
	   for (list<string>::iterator s = Autotsl_drop->begin(); s != Autotsl_drop->end(); s++) 
            {
			 sRec = *s; // Autotsl_drop->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
            
             if( TmpZcnt==SalZcnt )
               {
                 gtsl_drop->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_drop->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion


#pragma region Autotsl_easy_card to gtsl_easy_card
    gtsl_easy_card->clear();
	if (Autotsl_easy_card->size() > 0)
       {
		   for (list<string>::iterator s = Autotsl_easy_card->begin(); s != Autotsl_easy_card->end(); s++)
            {
			 sRec = *s; // Autotsl_easy_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_easy_card->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_easy_card->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion

#pragma region   Autotsl_pay_sub to gtsl_pay_sub 
    gtsl_pay_sub->clear();
    if( Autotsl_pay_sub->size() > 0)
       {
    	   for (list<string>::iterator s = Autotsl_pay_sub->begin(); s != Autotsl_pay_sub->end(); s++)
            {
			 sRec = *s; // Autotsl_pay_sub->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_pay_sub->push_back(sRec);
               }
             else
               {
                 ;; //gtsl_pay_sub->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion

#pragma region   Autotsl_bil to gtsl_bil 
    gtsl_bil->clear();
    if( Autotsl_bil->size() > 0)
       {
		   for (list<string>::iterator s = Autotsl_bil->begin(); s != Autotsl_bil->end(); s++) //for (int i = 0; i<Autotsl_bil->Count; i++)
            {
			 sRec = *s; // Autotsl_bil->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                gtsl_bil->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_bil->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion

#pragma region   Autotsl_fetc_card to gtsl_fetc_card
    gtsl_fetc_card->clear();
    //gtsl_fetc_card->Assign(Autotsl_fetc_card);
    if( Autotsl_fetc_card->size() > 0)
       {
		   for (list<string>::iterator s = Autotsl_fetc_card->begin(); s != Autotsl_fetc_card->end(); s++) //for (int i=0;  i<Autotsl_fetc_card->Count; i++)
            {
				sRec = *s; // Autotsl_fetc_card->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )
             if( TmpZcnt==SalZcnt )
               {
                 gtsl_fetc_card->push_back(sRec);
               }
             else
               {
                 ;;//gtsl_fetc_card->Delete(i);
               }
            }
       }
    irtn+=1;
#pragma endregion
	
#pragma region   Autotsl_subsale to tsl_subsale
	tsl_subsale->clear();
    //tsl_subsale->Assign(Autotsl_subsale);
    if( Autotsl_subsale->size() > 0)
       {
		   for (list<string>::iterator s = Autotsl_subsale->begin(); s != Autotsl_subsale->end(); s++) //for (int i = 0; i<Autotsl_subsale->Count; i++)
            {
			 sRec = *s; // Autotsl_subsale->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 tsl_subsale->push_back(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;
#pragma endregion

#pragma region   Autotsl_subsale to gtsl_1051_sal  依日結序號切 1051 List
   
    gtsl_1051_sal->clear();
    //tsl_subsale->Assign(Autotsl_subsale);
    if( Auto_1051_sal->size() > 0)
       {
		   for (list<string>::iterator s = Auto_1051_sal->begin(); s != Auto_1051_sal->end(); s++)  //for (int i = 0; i<Auto_1051_sal->Count; i++)
            {
			sRec = *s; // Auto_1051_sal->Strings[i];
             TmpZcnt=_StringSegment_EX(sRec, SGM, 6);  //日結序號
             sTmpDttm=_StringSegment_EX(sRec, SGM, 5);
             //if( sTmpDttm.SubString(1,8)<=sYYYYMMDD )

             if( TmpZcnt==SalZcnt )
               {
                 gtsl_1051_sal->push_back(sRec);
               }
             else
               {
                 ;;//tsl_subsale->Delete(i);
               }
            }
       }
       irtn+=1;
#pragma endregion

    } // end of try
  catch(...)
    {
      writelog("GetZCNT_SalData Catch error...");
      irtn=0;
    }

    return(irtn);

}



 
//讀入交易LOG檔，依TYPE分類  for UniCode
int __fastcall BASIC::wloadsalfile(string FileName, bool for_x, string AutoZ_Cnt, string AutoZ_Dt)
{
 
    init_data();                                //設定基本資料

    string str_sal_data, stmp, TmpSalDttm;                    //SAL每行字串
    int i_type, ifileType;                                 //資料TYEP 10,50..，長度
    wstring MEcode;

    //TypeANSI = 0,
    //TypeUNICODE = 1,
    //TypeUTF8 = 2,
    ifileType = GetTextFileType(FileName);
    if (ifileType == 1)
        MEcode = L"r, ccs = UNICODE";
    else if (ifileType == 2)
        MEcode = L"r, ccs = UTF-8";
    else
        MEcode = L"r, ccs = UTF-8";
    
    writelog("★★XZDATA.DLL wloadsalfile  ★★ InPut File:" + FileName + ", FILETYPE=" + WStringToString(MEcode) + ", AutoZ_Cnt=" + AutoZ_Cnt + ", AutoZ_Dt=" + AutoZ_Dt);

    bool bl_x_start = false;                    //開始儲存此次收銀員交易資料
    bool bl_x_into = true;                      //開始找此次收銀員資料

    string TmpZcnt, SalZcnt, str_trans_now;  //每行資料交易序號
    SalZcnt = AutoZ_Cnt;

    TStringList* tsl_x_data = new TStringList; //儲存本次收銀員交易資料

    string str_trans_next;                  //本次收銀員帳開始交易序號
    int i_compare_trans = 0;                    //上次收銀員帳最後交易序號
    int i_now_trans = 0;

    i_compare_trans = _StrToInt(gx_trans, "上次收銀員帳最後交易序號");          //上次交易序號

    bool bl_is_first = true;
    //wchar_t LineOfChars[MAXDATALEN];

    // open as a byte stream

    //std::wofstream wof;
    //wof.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
    //wof.open(wfile.c_str()); // (L"example.txt");

    std::wifstream wif; // StringToWString(FileName).c_str(), std::ios::binary);
    wif.open(StringToWString(FileName).c_str() );
    if( ifileType == 1)
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
    else
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    std::wstring wLine;
    if (wif.is_open())
    {
        wLine.clear();
        while ( std::getline(wif, wLine) )                //讀SAL檔資料
        {
            //wmemset(LineOfChars, 0x00, MAXDATALEN);
            str_sal_data = WStringToString(wLine);
            wLine.clear();
                 
            TmpZcnt = _StringSegment_EX(str_sal_data, SGM, 6);  //日結序號

            TmpSalDttm = _StringSegment_EX(str_sal_data, SGM, 5).substr(0, 8);
            
            if (CheckSaleRecData(str_sal_data))
            {
                //strcpy_s(gpchar_sal_last, sizeof(gpchar_sal_last), c_data_buf);                
                gsal_last = str_sal_data;  //SAL檔最後一筆資料

                if (AutoZ_Dt != "" && TmpSalDttm <= AutoZ_Dt)  // 以 SAL 內容為依據
                {
                    SalZcnt = TmpZcnt;
                }

                if (AutoZ_Dt != "")
                {
                    if (TmpZcnt > SalZcnt)
                        continue;
                }

                if (bl_is_first)
                {
                    // strcpy_s(gpchar_sal_first, sizeof(gpchar_sal_first), c_data_buf);           //SAL檔第一筆
                    gsal_first = str_sal_data;
                    bl_is_first = false;
                }

                if (for_x)      //收銀員帳表
                {
                    
                    i_type = _StrToInt(_StringSegment_EX(str_sal_data, SGM, 1) );  // str_sal_data.substr(0, 4));

                    str_trans_now = _StringSegment_EX(str_sal_data, SGM, 7).substr(TRANCNT_NO_POS, 5);

                    i_now_trans = _StrToInt(str_trans_now, "目前交易序號");
                    if (i_compare_trans == 0)       //如果是0，就從頭開始讀入交易資料
                    {
                        bl_x_start = true;
                        bl_x_into = false;
                    }
                    else if (bl_x_into)             //比對交易序號
                    {
                        
                        if (i_type != 1095 && i_type != 1096)
                        {
                           
                            if (i_now_trans >= i_compare_trans && i_now_trans <= i_compare_trans + 10)
                            {
                                logsprintf("取得本次收銀員帳開始交易序號 (%05d), 大於此序號將列入收銀員帳..", i_now_trans);

                                bl_x_start = true;
                                bl_x_into = false;

                            }
                        }
                    }

                    if (bl_x_start)         //0或是有比對到交易序號開始讀入資料
                    {
                        if (i_now_trans != i_compare_trans)
                            tsl_x_data->push_back(str_sal_data);
                    }
                }
                else    //讀帳、Z帳表
                {
                    //i_type = _StrToInt(str_sal_data.substr(0, 4)) % 1000;
                    i_type = _StrToInt( _StringSegment_EX(str_sal_data, SGM, 1) ) % 1000;

                    switch (i_type)
                    {
                        //REC
                    case 10:
                    case 110:
                    case 113:
                    case 120:
                        gtsl_rec->push_back(str_sal_data);
                        break;
                        //折扣讓
                    case 311:
                    case 11:
                        gtsl_pay_sub->push_back(str_sal_data);
                        break;
                        //easy card / 31:Kstc 高捷
                        //case 25:
                        //case 31:
                    case 32:  //TM一卡通加值(支付)明細電文檔(3232)
                    case 34:  //TM遠鑫加值(支付)明細電文檔 (3234)
                    case 38:  //TM二代悠遊卡加值 二代悠遊加值明細 3238
                    case 811: //plus Pay
                        gtsl_easy_card->push_back(str_sal_data);
                        break;
                    case 30:
                        gtsl_fetc_card->push_back(str_sal_data);
                        break;
                        //Bil
                    case 40:
                    case 240:
                        gtsl_bil->push_back(str_sal_data);
                        break;
                        //投庫
                    case 41:
                    case 241:
                        gtsl_drop->push_back(str_sal_data);
                        break;
                        //TOT
                    case 50:
                        gtsl_tot->push_back(str_sal_data);
                        // writelog(FileName+ ":SALTot " + str_sal_data);
                        break;
                    case 51:
                        gtsl_1051_sal->push_back(stmp);
                        break;

                    case 14:
                    case 15:
                    case 16:
                    case 20:
                    case 27:
                    case 37:
                        //case 31:
                    case 80:
                    case 90:
                    case 97:
                    case 1098:
                    case 1099:
                    case 1802:
                    case 1803:
                        break;
                    default:
                        //writelog(FileName+ ":Data Erroe " + str_sal_data);
                        break;
                    }
                }   // end of else 讀帳、Z帳表
            }
           else        //交易資料有誤
            {
                writelog("SAL檔交易資料異常 " + str_sal_data);
                continue;
            }

        }//while
    }//if (f_sal_file != NULL)
    else
    {
        gbl_tot_haveline = false;
        gbl_rec_haveline = false;
        gbl_easy_haveline = false;
        gbl_pay_haveline = false;
        gbl_bil_haveline = false;
        gbl_fetc_haveline = false;
        gbl_1051_haveline = false;        //gbl_0041_haveline = false;

        writelog("開啟交易檔案發生錯誤 " + FileName);
        return 0;
    }

    wif.close();

    if (for_x)      //收銀員帳表
    {
        logsprintf("取得上次收銀員帳交班後最後交易序號 (%s)", gx_trans.c_str() );

        logsprintf("取得本次收銀員帳最後交易序號 (%s), 序號 (%s) ~ (%s) 將列入收銀員帳.. ",
            str_trans_now.c_str(), gx_trans.c_str(), str_trans_now.c_str());

        for (list<string>::iterator s = tsl_x_data->begin(); s != tsl_x_data->end(); s++) //for (int i = 0; i<tsl_x_data->Count; i++)
        {
            stmp = *s;
            //i_type = _StrToInt(stmp.substr(0, 4), stmp);
            i_type = _StrToInt(_StringSegment_EX(stmp, SGM, 1), stmp) % 1000;
           
            switch (i_type)
            {
                //REC
            case 10:
            case 110:
            case 113:
            case 120:
                gtsl_rec->push_back(stmp);
                break;
                //折扣讓
            case 11:
            case 311:
                gtsl_pay_sub->push_back(stmp);
                break;
                //easy card
                //case 1025:
                //easy card / 31:Kstc 高捷
                //case 31:
            case 32:  //TM一卡通加值(支付)明細電文檔(3232)
            case 34:  //TM遠鑫加值(支付)明細電文檔 (3234)
            case 38:  //TM二代悠遊卡加值 二代悠遊加值明細 3238
            case 811: //plus Pay
                gtsl_easy_card->push_back(stmp);
                break;
            case 30:
                gtsl_fetc_card->push_back(stmp);
                break;
                // BIL
                //case 40:
            case 40:
            case 240:
                gtsl_bil->push_back(stmp);
                break;
                //投庫
                //case 41:
            case 41:
            case 241:
                gtsl_drop->push_back(stmp);
                break;
                //TOT
                //case 50:
            case 50:
                gtsl_tot->push_back(stmp);
                break;
            case 51:
                gtsl_1051_sal->push_back(stmp);
                break;
            default:
                //writelog(FileName+ "for_x :Data Erroe " + str_sal_data);
                break;
            }
        }

    }//for_x
    

    if (bl_x_start && gtsl_tot->size() > 0)
    {
        list<string>::iterator s = tsl_x_data->begin();
        gsalX_first = *s;
        //stmp = *s;
        //strcpy_s(gpchar_x_first, sizeof(gpchar_x_first), stmp.c_str());
    }
    else
    {
        //memset(gpchar_x_first, 0x00, MAXDATALEN);
        gsalX_first = "";
    }

    delete tsl_x_data;

    gi_tot_line = (gtsl_tot->size() - 1);            //TOT檔總行數
    gi_rec_line = (gtsl_rec->size() - 1);            //REC
    gi_easy_line = (gtsl_easy_card->size() - 1);     //EASY CARD
    gi_pay_line = (gtsl_pay_sub->size() - 1);        //折扣讓
    gi_bil_line = (gtsl_bil->size() - 1);           //BIL
    gi_fetc_line = (gtsl_fetc_card->size() - 1);    //Fetc Card
    gi_1051_line = (gtsl_1051_sal->size() - 1);      //1051行數

    logsprintf("取得本次帳表資料筆數 3110,3113=%d, 3011,3311=%d, 1031~1035=%d, 3240=%d, 3241=%d, 3050=%d, 3051=%d ",
        gtsl_rec->size(), gtsl_pay_sub->size(), gtsl_easy_card->size(), gtsl_bil->size(), gtsl_drop->size(), gtsl_tot->size(), gtsl_1051_sal->size());

    if (gi_tot_line > -1)
        gbl_tot_haveline = true;    //TOT筆數為正值
    else
        gbl_tot_haveline = false;

    if (gi_rec_line > -1)
        gbl_rec_haveline = true;    //REC筆數為正值
    else
        gbl_rec_haveline = false;

    if (gi_bil_line > -1)
        gbl_bil_haveline = true;    //Bil筆數為正值
    else
        gbl_bil_haveline = false;


    if (gi_easy_line > -1)
        gbl_easy_haveline = true;   //EASY CARD筆數為正值
    else
        gbl_easy_haveline = false;

    if (gi_pay_line > -1)
        gbl_pay_haveline = true;    //折扣讓筆數為正值
    else
        gbl_pay_haveline = false;

    if (gi_fetc_line > -1)
        gbl_fetc_haveline = true;   //EASY CARD筆數為正值
    else
        gbl_fetc_haveline = false;

    if (gi_1051_line > -1)
        gbl_1051_haveline = true;   //1051筆數為正值
    else
        gbl_1051_haveline = false;


    return OK;
}
 

//讀入交易LOG檔，依TYPE分類  for Ansi
int __fastcall BASIC::loadsalfile(string FileName, bool for_x, string AutoZ_Cnt, string AutoZ_Dt)
{
    init_data();                                //設定基本資料
	
    FILE *f_sal_file;                           //SAL檔指標

    string str_sal_data,stmp, TmpSalDttm;                    //SAL每行字串
    int i_type, ifileType;                                 //資料TYEP 10,50..，長度
    //CHAR cLog[2049];
	wstring MEcode;

    //CHAR c_data_buf[MAXDATALEN];                       //讀出每行資料暫存字串
    CHAR *c_data_buf;
    c_data_buf = (CHAR*)malloc(MAXDATALEN);
    //free(c_data_buf);

    //MEcode = L"1010 || M999999 || M121704 ||堃 六龜 - 農會測試商品０元 ||\n";
    //stmp =  WStringToString(MEcode);  //ws2s(MEcode);

	//TypeANSI = 0,
	//TypeUNICODE = 1,
	//TypeUTF8 = 2,
	ifileType=GetTextFileType(FileName);
	if (ifileType == 1)
			MEcode = L"r, ccs = UNICODE";
	else if (ifileType == 2)
	    	MEcode = L"r, ccs = UTF-8";
	else
		MEcode = L"r, ccs = UTF-8";

//#ifdef TM_20    //記錄版本
	writelog("★★XZDATA.DLL loadsalfile  ★★ InPut File:" + FileName + ", FILETYPE=" + WStringToString(MEcode) + ", AutoZ_Cnt=" + AutoZ_Cnt + ", AutoZ_Dt=" + AutoZ_Dt);
//#else
//    writelog(_ver);
//#endif

    bool bl_x_start = false;                    //開始儲存此次收銀員交易資料
    bool bl_x_into = true;                      //開始找此次收銀員資料

    string TmpZcnt, SalZcnt, str_trans_now;  //每行資料交易序號

    SalZcnt=AutoZ_Cnt;

#ifdef WIN32
	errno_t err;
	if (ifileType == 0) // ANSI File
	{
		err = fopen_s(&f_sal_file, FileName.c_str(), "rb");
	}
	else
	{
		err = _wfopen_s(&f_sal_file, StringToWString(FileName).c_str(), MEcode.c_str() );
	}

	
#else
	if ((f_sal_file = fopen(FileName.c_str(),"rb")) == NULL)
#endif
	if (err != 0)
    {
        gbl_tot_haveline = false;
        gbl_rec_haveline = false;
        gbl_easy_haveline = false;
        gbl_pay_haveline = false;
        gbl_bil_haveline = false;
        gbl_fetc_haveline = false;
        gbl_1051_haveline = false;        //gbl_0041_haveline = false;

        writelog("開啟交易檔案發生錯誤 " + FileName);
        return 0;
    }

    TStringList *tsl_x_data = new TStringList; //儲存本次收銀員交易資料

    string str_trans_next;                  //本次收銀員帳開始交易序號
    int i_compare_trans = 0;                    //上次收銀員帳最後交易序號
    int i_now_trans=0;

    i_compare_trans = _StrToInt(gx_trans,"上次收銀員帳最後交易序號");          //上次交易序號

    bool bl_is_first = true;
	//wchar_t LineOfChars[MAXDATALEN];
    wchar_t *LineOfChars;
    LineOfChars = (wchar_t*)malloc(MAXDATALEN);

	wstring wLine;

    if (f_sal_file != NULL)
    {
        while (!feof(f_sal_file))               //讀SAL檔資料
        {
			// 注意 必須有 0D0A
			if (ifileType == 0) // ANSI File
			{
				memset(c_data_buf, 0x00, MAXDATALEN);
				fgets(c_data_buf, MAXDATALEN, f_sal_file);
				fscanf_s(f_sal_file,"\n");    // for if ((f_sal_file = fopen(FileName.c_str(),"rb")) == NULL)
				str_sal_data = c_data_buf;
			}
			else
			{
				wmemset(LineOfChars, 0x00, MAXDATALEN);
				fgetws(LineOfChars, MAXDATALEN, f_sal_file);
				wLine.clear();
				wLine.append(LineOfChars);
                str_sal_data = WStringToString(wLine);
                //str_sal_data = ws2s(wLine.c_str() ); 
				//strcpy_s(c_data_buf, sizeof(c_data_buf), str_sal_data.c_str() );
			}
            //MEcode = _WStringSegmentW(wLine, L"||", 6);

            TmpZcnt=_StringSegment_EX(str_sal_data, SGM, 6);  //日結序號

            TmpSalDttm=_StringSegment_EX(str_sal_data, SGM, 5).substr(0,8);
            //if( AutoZ_Dt != "" )
            //  {
            //     if(  TmpSalDttm > AutoZ_Dt )
            //       continue;
            //  }

          //  if ( (!(*c_data_buf == 0x00) && (!(*c_data_buf == '\n')) && (i_len > 50) ) )
            if( CheckSaleRecData(str_sal_data) )
            {
				//strcpy_s(gpchar_sal_last, sizeof(gpchar_sal_last), c_data_buf);                
                gsal_last = str_sal_data;  //SAL檔最後一筆資料

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
					// strcpy_s(gpchar_sal_first, sizeof(gpchar_sal_first), c_data_buf);           //SAL檔第一筆
                    gsal_first = str_sal_data;
                    bl_is_first = false;
                }

                if (for_x)      //收銀員帳表
                {
                  //  i_compare_trans = atoi(gchar_x_trans);          //上次交易序號
                  //  str_trans_now = str_sal_data.SubString(41,5);   //目前交易序號

                    i_type = _StrToInt(str_sal_data.substr(0,4));

					str_trans_now = _StringSegment_EX(str_sal_data, SGM, 7).substr(TRANCNT_NO_POS, 5);

                    i_now_trans =  _StrToInt(str_trans_now,"目前交易序號");
                    if (i_compare_trans == 0)       //如果是0，就從頭開始讀入交易資料
                    {
                        bl_x_start = true;
                        bl_x_into = false;
                    }
                    else if (bl_x_into)             //比對交易序號
                    {
                       // str_trans_next.sprintf("%05d", ++i_compare_trans);
                       // if ((str_trans_now.AnsiCompare(str_trans_next)) == 0)

                       if( i_type != 1095 && i_type != 1096 )
                         {
                          //if (i_now_trans == i_compare_trans  )   2013/10/14 Update
                            if (i_now_trans >= i_compare_trans  &&  i_now_trans <= i_compare_trans+10 )
                             {
							   logsprintf("取得本次收銀員帳開始交易序號 (%05d), 大於此序號將列入收銀員帳..", i_now_trans);
							   
                               bl_x_start = true;
                               bl_x_into = false;

                               //if (i_now_trans > i_compare_trans )   //2013/10/14 Update
                               //    tsl_x_data->Add(str_sal_data);

                               // if (i_now_trans == i_compare_trans) //2004/03/22
                               //     continue;
                             }
                         }     
                    }

                    if (bl_x_start)         //0或是有比對到交易序號開始讀入資料
                    {
                        if (i_now_trans != i_compare_trans)
                              tsl_x_data->push_back(str_sal_data);
                    }
                }
                else    //讀帳、Z帳表
                {                         
                    i_type = _StrToInt(str_sal_data.substr(0,4))%1000;

                    switch (i_type)
                    {
                        //REC
                        case 110:
                        case 113:
                        case 10:
                       		gtsl_rec->push_back(str_sal_data);
                            break;
                        //折扣讓
                        case 311:
                        case 11:
							gtsl_pay_sub->push_back(str_sal_data);
                            break;
                        //easy card / 31:Kstc 高捷
                        //case 25:
                        //case 31:
                        case 32:  //TM一卡通加值(支付)明細電文檔(3232)
                        //case 35:
                        case 34:  //TM遠鑫加值(支付)明細電文檔 (3234)
                        case 38:  //TM二代悠遊卡加值 二代悠遊加值明細 3238
							gtsl_easy_card->push_back(str_sal_data);
                            break;
                        case 30:
							gtsl_fetc_card->push_back(str_sal_data);
                            break;
                        //Bil
                        case 40:
                        case 240:
							gtsl_bil->push_back(str_sal_data);
                            break;
                        //投庫
                        case 41:
                        case 241:
							gtsl_drop->push_back(str_sal_data);
                            break;
                        //TOT
                        case 50:
                        	gtsl_tot->push_back(str_sal_data);
                           // writelog(FileName+ ":SALTot " + str_sal_data);
                            break;
                        case 51:
                            gtsl_1051_sal->push_back(stmp);
                            break;

                        case 14:
                        case 15:
                        case 16:
                        case 20:
                        case 27:
                        case 37:
                        //case 31:
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
                }   // end of else 讀帳、Z帳表
            }
            else        //交易資料有誤
            {
                if (!feof(f_sal_file))
                    writelog("SAL檔交易資料異常 " + str_sal_data);
                continue;
            }

        }//while
    }//if (f_sal_file != NULL)

    if (for_x)      //收銀員帳表
    {
        logsprintf("取得上次收銀員帳交班後最後交易序號 (%s)", gx_trans.c_str());
		
        logsprintf("取得本次收銀員帳最後交易序號 (%s), 序號 (%s) ~ (%s) 將列入收銀員帳.. ",
                      str_trans_now.c_str(), gx_trans.c_str(), str_trans_now.c_str() );
		
		for (list<string>::iterator s = tsl_x_data->begin(); s != tsl_x_data->end(); s++) //for (int i = 0; i<tsl_x_data->Count; i++)
        {
			stmp = *s;
			i_type = _StrToInt(stmp.substr(0, 4), stmp);

            switch (i_type)
            {
                //REC
                case 10:
                case 1010:
					gtsl_rec->push_back(stmp);
                    break;
                //折扣讓
                case 11:
                case 1011:
					gtsl_pay_sub->push_back(stmp);
                    break;
                //easy card
                //case 1025:
                //easy card / 31:Kstc 高捷
                //case 31:
                case 32:  //TM一卡通加值(支付)明細電文檔(3232)
                case 34:  //TM遠鑫加值(支付)明細電文檔 (3234)
                case 38:  //TM二代悠遊卡加值 二代悠遊加值明細 3238
					gtsl_easy_card->push_back(stmp);
                    break;
                case 1030:
					gtsl_fetc_card->push_back(stmp);
                    break;
                // BIL
                //case 40:
                case 1040:
					gtsl_bil->push_back(stmp);
                    break;
                //投庫
                //case 41:
                case 1041:
					gtsl_drop->push_back(stmp);
                    break;
                //TOT
                //case 50:
                case 1050:
					gtsl_tot->push_back(stmp);
                    break;
                case 1051:
					gtsl_1051_sal->push_back(stmp);
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
    free(c_data_buf);
    free(LineOfChars);

	if (bl_x_start && gtsl_tot->size() > 0)
	{
		list<string>::iterator s = tsl_x_data->begin();
        gsalX_first = *s;
        //stmp = *s;
		//strcpy_s(gpchar_x_first, sizeof(gpchar_x_first), stmp.c_str());
	}
    else
	{
		//memset(gpchar_x_first, 0x00, MAXDATALEN);
        gsalX_first = "";
	}

    delete tsl_x_data;

    gi_tot_line = (gtsl_tot->size() - 1);            //TOT檔總行數
	gi_rec_line = (gtsl_rec->size() - 1);            //REC
	gi_easy_line = (gtsl_easy_card->size() - 1);     //EASY CARD
	gi_pay_line = (gtsl_pay_sub->size() - 1);        //折扣讓
	gi_bil_line = (gtsl_bil->size() - 1);           //BIL
	gi_fetc_line = (gtsl_fetc_card->size() - 1);    //Fetc Card
	gi_1051_line = (gtsl_1051_sal->size() - 1);      //1051行數

	logsprintf("取得本次帳表資料筆數 1010=%d, 1011=%d, 1031~1035=%d, 1040=%d, 1041=%d, 1050=%d, 1051=%d ",
		gtsl_rec->size(), gtsl_pay_sub->size(), gtsl_easy_card->size(),	gtsl_bil->size(), gtsl_drop->size(), gtsl_tot->size(), gtsl_1051_sal->size());

    if (gi_tot_line > -1)
        gbl_tot_haveline = true;    //TOT筆數為正值
    else
        gbl_tot_haveline = false;

    if (gi_rec_line > -1)
        gbl_rec_haveline = true;    //REC筆數為正值
    else
        gbl_rec_haveline = false;

    if (gi_bil_line > -1)
        gbl_bil_haveline = true;    //Bil筆數為正值
    else
        gbl_bil_haveline = false;


    if (gi_easy_line > -1)
        gbl_easy_haveline = true;   //EASY CARD筆數為正值
    else
        gbl_easy_haveline = false;

    if (gi_pay_line > -1)
        gbl_pay_haveline = true;    //折扣讓筆數為正值
    else
        gbl_pay_haveline = false;

    if (gi_fetc_line > -1)
        gbl_fetc_haveline = true;   //EASY CARD筆數為正值
    else
        gbl_fetc_haveline = false;

    if (gi_1051_line > -1)
        gbl_1051_haveline = true;   //1051筆數為正值
    else
        gbl_1051_haveline = false;


    return OK;
}



//97,98
int __fastcall BASIC::w_WriteSAL(string data, string SALFileName, int mode)
{
    int ifileType = 2;
    FILE* f_write_sal;
    wstring MEcode;

    writelog("w_WriteSAL:寫入檔案: " + SALFileName + "<" + data + ">");

        //TypeANSI = 0,
        //TypeUNICODE = 1,
        //TypeUTF8 = 2,
        
    if (ifileType == 1)
    {
        if (mode == 0)
            MEcode = L"a, ccs = UTF-8";
        else if (mode == 1)
            MEcode = L"a+, ccs = UTF-8";
        else
            MEcode = L"w, ccs = UTF-8";
    }
    else if (ifileType == 2)
    {
        if (mode == 0)
            MEcode = L"a, ccs = UTF-8";
        else if (mode == 1)
            MEcode = L"a+, ccs = UTF-8";
        else
            MEcode = L"w, ccs = UTF-8";
    }
    else
    {
        if (mode == 0)
            MEcode = L"a, ccs = UTF-8";
        else if (mode == 1)
            MEcode = L"a+, ccs = UTF-8";
        else
            MEcode = L"w, ccs = UTF-8";
    }

   // a:   Opens for writing at the end of the file(appending) without removing the EOF marker 
   //      before writing new data to the file.Creates the file if it doesn't exist.

   // a+:  Opens for readingand appending.The appending operation includes the removal of the EOF 
   //      marker before new data is written to the fileand the EOF marker is restored after writing 
   //      is complete.Creates the file if it doesn't exist.

    errno_t err;
   // if ((err = fopen_s(&f_write_sal, SALFileName.c_str(), "a")) != 0)
   if (ifileType == 0) // ANSI File
        {
            err = fopen_s(&f_write_sal, SALFileName.c_str(), "a");
        }
    else
        {
            err = _wfopen_s(&f_write_sal, StringToWString(SALFileName).c_str(), MEcode.c_str());
        }

    if (err != 0)
        {
            writelog("開啟交易檔案發生錯誤 " + SALFileName);
            return 0;
        }


    fwprintf(f_write_sal, L"%s\n", StringToWString(data).c_str());
    fclose(f_write_sal);

    return OK;
}


//97,98
int __fastcall BASIC::WriteSAL(string data, string SALFileName)
{
	FILE *f_write_sal;
	writelog("WriteSAL:寫入檔案: " + SALFileName + "<" + data + ">");


#ifdef WIN32
	errno_t err;
	if ((err = fopen_s(&f_write_sal, SALFileName.c_str(), "a")) != 0)
#else
	if ((f_write_sal = fopen(SALFileName.c_str(), "a")) == NULL)
#endif
	{
		writelog("WriteSAL:寫入SAL檔發生錯誤 " + SALFileName);
		return 0;
	}


	fprintf(f_write_sal, "%s\n", data.c_str());
	fclose(f_write_sal);

	return OK;
}


int __fastcall BASIC::WriteSAL_A(string data, string SALFileName)
{
	FILE *f_write_sal;

	writelog("WriteSAL_A:寫入檔案: " + SALFileName + "<" + data + ">");

#ifdef WIN32
	errno_t err;
	if ((err = fopen_s(&f_write_sal, SALFileName.c_str(), "a+")) != 0)
#else
	if ((f_write_sal = fopen(SALFileName.c_str(), "a+")) == NULL)
#endif
	{
		writelog("WriteSAL:寫入SAL檔發生錯誤 " + SALFileName);
		return 0;
	}


	fprintf(f_write_sal, "%s\n", data.c_str());
	fclose(f_write_sal);

	return OK;
}




bool __fastcall BASIC::CheckSaleRecData(string RecData)
{
    string strtmp, sRecType;
    bool blFileRecErr;
    int i,itype;
	//CHAR cLog[2049];

    blFileRecErr = false;
    i=RecData.length() ;
    sRecType=RecData.substr(0,4);
    try
      {
		  itype = stoi(sRecType)%100;

        switch (itype)
            {
              case 01:
              case 99:
                    // if( i == REC0199LENGTH || i == 57)
                           blFileRecErr = true;
                     gsLast1099=RecData;
                        break;
               
              //case 10:
              //       if( i == REC10LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 11:
              //       if( i == REC11LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 13:
              //       if( i == REC13LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 15:
              //       if( i == REC15LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 40:
              //       if( i == REC40LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 20:
              //       if( i == REC20LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 25:
              //       if( i == REC25LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 30:
              //       if( i == REC30LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 31:      //546
              //       if( i == REC31LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 35:
              //       if( i == REC35LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 41:
              //       if( i == REC41LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 50:
              //       if( i == REC50LENGTH )
              //             blFileRecErr = true;
              //          break;
              //case 51:
              //       if( i == REC51LENGTH )
              //             blFileRecErr = true;
              //          break;
             

              
              default:
                      blFileRecErr = true;
                      break ;
            }
     }
     catch(...)
     {
       blFileRecErr = false ;
     }

     if( blFileRecErr == false)
        {
			logsprintf("CheckSaleRecData:  RecData(%s) 交易資料異常 Length=%d ", RecData.c_str(),i  );
			//writelog(cLog);
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

}

string __fastcall BASIC::BackupFile(string FileName, string BackupFileName, string DDZ)  //檔案備份
{

	
	if (!PathFileExists(StringToWString(FileName).c_str()))
    {
        writelog("備份資料檔不存在 " + FileName);
        return("NONFILE");
    }
    else
    {
       writelog("備份資料=" + FileName + "  BackupFileName=" + BackupFileName + "  DDZ="+DDZ );
    }

    string sFilePathDD;                //日期
	string sFilePathNum;               //次數
	string sBackupPath;                //備份路徑
	string sBackupFileName;            //備份檔名

	string sHistory = "C:\\FTLPOS\\HISTORY\\";
	string sFileNameNonPath = "";

	sFileNameNonPath = _ExtractFile(FileName);    //檔名不包含路徑

    if( Trim(DDZ) !="")
      {
		  sFilePathDD = DDZ.substr(0, 2);
		  sFilePathNum = "0" + DDZ.substr(2, 1);
      }
    else
      {
        //020312250100.zdt
		  sFilePathDD = sFileNameNonPath.substr(5, 2);
		  sFilePathNum = sFileNameNonPath.substr(7, 2);

      }


    if( Trim(BackupFileName) !="")
      {
        sBackupPath = _ExtractFilePath(BackupFileName.c_str() );                           //檔名路徑 
		sFileNameNonPath = _ExtractFile(BackupFileName);                                  //檔名不包含路徑
      }
     else
      {

        //備份路徑
		  if ((!sFilePathDD.empty()) && (!sFilePathNum.empty()))
             sBackupPath = (sHistory + sFilePathDD + "\\" + sFilePathNum + "\\");
        else
             sBackupPath = sHistory;

      }

    //建立目錄
	if (!DirExists(sBackupPath))
		SHCreateDirectory(NULL, StringToWString(sBackupPath).c_str());  

	
    sBackupFileName = (sBackupPath + sFileNameNonPath);             //備份檔名路徑檔名

	DeleteFile(StringToWString(sBackupFileName).c_str());

	if (MoveFile( StringToWString(FileName).c_str(), StringToWString(sBackupFileName).c_str() ))              //搬檔
        writelog("備份資料檔名 OK : " + FileName + " To " +sBackupFileName);
    else
        writelog("備份資料檔名 NG : " + FileName + " To " +sBackupFileName);

    return sBackupFileName;                                 //回傳檔案路徑
}

int  __stdcall _StrToInt(string S, string Data)
{
    int i = 0;
    int amt;
	
    try
    {
		if (S.empty() || Trim(S) == "")
          S="0";  
		amt = stoi(S); //stof(s)
        i = amt; //StrToInt(S);
    }
	catch (...)
    {
        //writelog("數值轉換錯誤 " + S + " " + Data);
        return 0;
    }

    return i;
}

//本日來客數 / 交易品項數   Data 3050 
int __fastcall BASIC::SumQt_tcust(int StartLine,int TotalLine)
{
    string str_trans_type,s;       //銷售型態
    int i_trans_cnt = 0;             //來客數計算
    int i_invo_cnt = 0;              //銷售數量
    int iCOInvoAmtTxt=0;
    int iCOInvoAmtNxt=0;
    int  i_trans_num = 0;

    giA = 0; //點卡手續費來客數
    giB = 0; //交易品項數
    if (gbl_tot_haveline)
    {
		//for (int i = StartLine; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
         {
			s = *ls; // gtsl_tot->Strings[i];
            str_trans_type = _StringSegment_EX(s, SGM, 8);                         //SALE_TYPE
            //i_invo_cnt = _StrToInt(gtsl_tot->Strings[i].SubString(220,5));       //pc_cnt
           
            i_trans_num = _StrToInt(_StringSegment_EX(s, SGM, 30));               //PC_CNT

            iCOInvoAmtTxt = _StrToInt(_StringSegment_EX(s, SGM, 49));  //AM_ACC76 C0 00 應稅   
            iCOInvoAmtNxt = _StrToInt(_StringSegment_EX(s, SGM, 50));  //AM_ACC78 C0 00 免稅 + S0 02 免稅代售 ,(目前不會有 C0 00 免稅 )
			//invo_amt+kcolx_amt+kcolf_amt  (32:發票金額, 37:折扣\讓金額, 44:不開發票金額(應), 45:不開發票金額(免)
			//發票金額  
            i_invo_cnt = _StrToInt(_StringSegment_EX(s, SGM, 32))+
				         _StrToInt(_StringSegment_EX(s, SGM, 37)) +
                         _StrToInt(_StringSegment_EX(s, SGM, 44))+
                         _StrToInt(_StringSegment_EX(s, SGM, 45))-iCOInvoAmtTxt-iCOInvoAmtNxt;
                         //_StrToInt(gtsl_tot->Strings[i].SubString(345,8))+
                         //_StrToInt(gtsl_tot->Strings[i].SubString(354,8));

           

			if (_StrFind(str_trans_type,"Z"))
            {
                
                if (i_invo_cnt)     //發票金額>0
                    giB += i_trans_num;   //交易品項數

                //銷售數量不為0，代收、售、付，無銷售數量
                if (i_invo_cnt>0)
                   { ++i_trans_cnt; }
                else  if (i_invo_cnt <= 0 && iCOInvoAmtTxt + iCOInvoAmtNxt > 0)
                {
                    ++giA; // 點卡手續費來客數
                }    

            }
			else if (_StrFind(str_trans_type, "R2") || _StrFind(str_trans_type,"R3"))  //if (found!=std::string::npos)
            {
                if (i_invo_cnt)     //發票金額>0
                    giB -= i_trans_num;   //交易品項數

                if (i_invo_cnt>0)
                    { --i_trans_cnt;  }
                else  if (i_invo_cnt <= 0 && iCOInvoAmtTxt + iCOInvoAmtNxt > 0)
                {
                    --giA; // 點卡手續費來客數
                } 

            }
        }
    }

    return i_trans_cnt; //本日來客數
}

////交易品項數
int __fastcall BASIC::SumQt_trans(int StratLine, int TotalLine)
{

    string str_trans_type,s;  //銷售型態
    int i_trans_num = 0;        //交易品項數
    int i_invo_cnt = 0;         //銷售數量不為0

    if (gbl_tot_haveline)
    {
        
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];
            str_trans_type = _StringSegment_EX(s, SGM, 8);

            i_invo_cnt = _StrToInt(_StringSegment_EX(s, SGM, 30));


			if (_StrFind(str_trans_type,"Z"))
            {
                if (i_invo_cnt)     //銷售數量不為0
                    i_trans_num += i_invo_cnt;
            }
			else if (_StrFind(str_trans_type, "R2") || _StrFind(str_trans_type,"R3"))
            {
                i_trans_num -= i_invo_cnt;
            }
        }
    }

    return i_trans_num;
}


////發票日結帳金額  3050
void __fastcall BASIC::TotCount(int StartLine, int TotalLine)
{

	//sThsrcStr=_StringSegment_EX(s, "|", 1);

	//免、 稅、 金
	giA = giB = giC = 0;

	string str_trans_type, tmpSer, strtmp, sSale;  //銷售型態
	int iTmpA, iTmpB, iTmpC, iTmpD;

	iTmpA = iTmpB = iTmpC = iTmpD = 0;
	if (gbl_tot_haveline)
	{
		//for (int i = 0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
		{
			sSale = *ls; // gtsl_tot->Strings[i];
			//str_trans_type = gtsl_tot->Strings[i].SubString(67,2);
			str_trans_type = _StringSegment_EX(sSale, SGM, 8);  //SALE_TYPE

			//tmpSer=gtsl_tot->Strings[i].SubString(36,10);  //交易序號
			tmpSer = _StringSegment_EX(sSale, SGM, 7);       //交易序號

			iTmpA = iTmpB = iTmpC = 0;

			//發票重印、正常銷售
			if (_StrFind(str_trans_type, "R4") || _StrFind(str_trans_type,"Z0"))
			{
				//iTmpA=_StrToInt(gtsl_tot->Strings[i].SubString(309,8));   //發票日結帳額(免)
				iTmpA = _StrToInt(_StringSegment_EX(sSale, SGM, 40));   //發票日結帳額(免) AM_FTAX
				giA += iTmpA;
				// iTmpB=_StrToInt(gtsl_tot->Strings[i].SubString(300,8));   //發票日結帳額(稅) 
				iTmpB = _StrToInt(_StringSegment_EX(sSale, SGM, 39));   //發票日結帳額(稅) AM_TAX
				giB += iTmpB;
				//iTmpC=_StrToInt(gtsl_tot->Strings[i].SubString(236,9));   //發票日結帳金額
				iTmpC = _StrToInt(_StringSegment_EX(sSale, SGM, 32));   //發票日結帳金額  AM_INVO
				giC += iTmpC;

				//iTmpD=_StrToInt(_StringSegment_EX(sSale, SGM, 49));   //點卡手續費  由 Sub_C0C1TaxInvoAmt 處理
				iTmpD = 0;
				giB -= iTmpD;
				giC -= iTmpD;

				if (iTmpC != (iTmpA + iTmpB))
				{
					logsprintf("TotCount:發票日結帳, 交易序號(%s) 發票金額不平 , (發票金額=%d) != (發票(免)=%d)+(發票(稅=%d)-點卡手續費(%d) )",
						tmpSer.c_str(), iTmpC, iTmpA, iTmpB, iTmpD);
				}

			}
		}
	}

	logsprintf("TotCount:發票日結帳金額, (發票金額=%d) => (發票(免=%d))+(發票(稅=%d)不含點卡手續費(%d) )",
		giC, giA, giB, iTmpD);

}


////誤打發票   3050
void __fastcall BASIC::TotMiss(int StartLine, int TotalLine)
{
    //次、免、  稅、  金額
    giA = giB = giC = giD = 0;
    giE = giF = giG = giI= 0;   //for 發票日結帳金額 
   

    string str_miss_type, tmpSer, s ;
    int iTmpB, iTmpC, iTmpD, iTmpE ;
    int iTmpI,iTmpF,iTmpG, iTmpH;

    if (gbl_tot_haveline)
    {
        // for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];
           str_miss_type = _StringSegment_EX(s, SGM, 8);   //SALE_TYPE
           tmpSer=_StringSegment_EX(s, SGM, 7);  //交易序號
           iTmpD=iTmpB=iTmpC=iTmpE=0;
           iTmpI = iTmpF = iTmpG = 0;

           //發票重印、正常銷售  來源 TotCount
           if (_StrFind(str_miss_type, "R4") || _StrFind(str_miss_type, "Z0"))
           {
               
               iTmpE = _StrToInt(_StringSegment_EX(s, SGM, 40));   //發票日結帳額(免) AM_FTAX I
               giI += iTmpE;
               
               iTmpF = _StrToInt(_StringSegment_EX(s, SGM, 39));   //發票日結帳額(稅) AM_TAX  B
               giF += iTmpF;
              
               iTmpG = _StrToInt(_StringSegment_EX(s, SGM, 32));   //發票日結帳金額  AM_INVO  C
               giG += iTmpG;

               iTmpH = 0;   //D
               giF -= iTmpH;
               giG -= iTmpH;

               if (iTmpG != (iTmpI + iTmpF))
               {
                   logsprintf("TotMiss:發票日結帳, 交易序號(%s) 發票金額不平 , (發票金額=%d) != (發票(免)=%d)+(發票(稅=%d)-點卡手續費(%d) )",
                       tmpSer.c_str(), iTmpG, iTmpI, iTmpF, iTmpH);
               }
 
           }


		   if (_StrFind(str_miss_type,"R"))
            {
                ++giA; //誤打發票次數
                iTmpB=_StrToInt(_StringSegment_EX(s, SGM, 40)); //誤打發票金額(免)
                giB += iTmpB;
                iTmpC=_StrToInt(_StringSegment_EX(s, SGM, 39)); //誤打發票金額(稅)
                giC += iTmpC;
                iTmpD=_StrToInt(_StringSegment_EX(s, SGM, 32)); //誤打發票金額
                giD += iTmpD;

                //iTmpE=_StrToInt(_StringSegment_EX(s, SGM, 49)); //點卡手續費 由 Sub_C0C1TaxInvoAmt 處理
                iTmpE=0;
                giC-=iTmpE;
                giD-=iTmpE;

                logsprintf("TotMiss:誤打發票, 交易序號(%s) (發票金額=%6d) => (發票(免%6d) )+(發票(稅%6d) 不含點卡手續費(%d) )  =>>累計=%d",
                                      tmpSer.c_str(),iTmpD, iTmpB, iTmpC, iTmpE, giD );
                 if( iTmpD != (iTmpB+iTmpC) )
                  {
                    logsprintf("TotMiss:誤打發票, 交易序號(%s) 發票金額不平 , (發票金額=%d) != (發票(免%d))+(發票(稅%d) - 點卡手續費(%d) )",
						tmpSer.c_str(), iTmpD, iTmpB, iTmpC, iTmpE);
                  }
            }
        }
    }
}



////誤打發票張數
void __fastcall BASIC::TotMissCnt(int StartLine, int TotalLine)
{
    giA = giB = giC = giD = 0;

	string str_miss_type, tmpSer, tmpVsNo, s, strtmp;
    int iTmpB, iTmpC, iTmpD;

    if (gbl_tot_haveline)
    {
       //for (int i=0; i<gtsl_tot->Count; i++)
	   for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];
            str_miss_type = _StringSegment_EX(s, SGM, 8);
            tmpSer=_StringSegment_EX(s, SGM, 7);             //交易序號
            iTmpD=iTmpB=iTmpC=0;

			if (_StrFind(str_miss_type, "R") || _StrFind(str_miss_type,"V"))
            {
                strtmp=_StringSegment_EX(s, SGM, 9);
                if (Trim(strtmp) == "" || tmpVsNo == "00000000")
                   {
                      logsprintf("TotMissCnt:交易序號: %s 不紀錄誤打發票張數, tmpVsNo=%s, 發票號碼起(%08ld), 發票號碼迄(%08ld)",
                      tmpSer.c_str(), tmpVsNo.c_str(), iTmpB, iTmpC);
                   }
                else
                {
                    tmpVsNo = strtmp.substr(2, 8);   //gtsl_tot->Strings[i].SubString(73,8); 發票號碼起
                    iTmpB = _StrToInt(strtmp.substr(7, 3)); //誤打發票號碼起

                    strtmp = _StringSegment_EX(s, SGM, 10);
                    iTmpC = _StrToInt(strtmp.substr(7, 3)); //誤打發票號碼迄
                    if (iTmpC < iTmpB)  // 2008/05/07 Update
                    {
                        logsprintf("TotMissCnt:交易序號: %s 誤打發票號碼迄有誤, 發票號碼起(%03d),  發票號碼迄(%03d)",
                            tmpSer.c_str(), iTmpB, iTmpC);
                        
                    }
                    if (_StrFind(str_miss_type, "R"))
                        {
                            giA+=abs(iTmpC-iTmpB)+1; //誤打發票張數
                        }
                     else if (_StrFind(str_miss_type,"V"))
                        {
                          giB+=abs(iTmpC-iTmpB)+1; //誤打發票張數
                        }
                }
         
                // 2021/05/31 Update 交班當機.
                 //if( Trim(tmpVsNo)!="" && tmpVsNo!="00000000" )
                 //  {
				//	   if (_StrFind(str_miss_type,"R"))
                //        {
                //          giA+=abs(iTmpC-iTmpB)+1; //誤打發票張數
                //        }
				//	   else if (_StrFind(str_miss_type,"V"))
                //        {
                //          giB+=abs(iTmpC-iTmpB)+1; //誤打發票張數
                //        }
                //    }
                  //else
                  //  {
                  //     logsprintf("TotMissCnt:交易序號: %s 不紀錄誤打發票張數, tmpVsNo=%s, 發票號碼起(%08ld), 發票號碼迄(%08ld)",
				  //	   tmpSer.c_str(), tmpVsNo, iTmpB, iTmpC);
                 //   }
             }
         }
    }

    logsprintf("TotMissCnt:誤打發票張數, 退貨R2R3R4(%d), 交易取消V(%d)", giA, giB );
    
}



////現金合計 3050
int __fastcall BASIC::SumCash(int StartLine, int TotalLine)
{
    string str_trans_type,s;      //銷售型態
    int i_cash_z = 0;               //Z Type
    int i_cash_r = 0;               //R Type
    int i_cash_amt = 0;             //現金加總

    //20031223門市提貨券
    int pay_cnt = 0;    //次數
    int pay_amt = 0;    //金額


    if (gbl_tot_haveline)
    {
      //for (int i=0; i<gtsl_tot->Count; i++)
	  for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];
            str_trans_type = _StringSegment_EX(s, SGM, 8);

			if (_StrFind(str_trans_type, "Z") || _StrFind(str_trans_type,"V"))
            {
                i_cash_z += _StrToInt(_StringSegment_EX(s, SGM, 15));
            }
			else if (_StrFind(str_trans_type, "R2") || _StrFind(str_trans_type,"R3"))
            {
                i_cash_r += _StrToInt(_StringSegment_EX(s, SGM, 15));
            }

            //20031225門市提貨券
            pay_amt = _StrToInt(_StringSegment_EX(s, SGM, 21));
            if (pay_amt != 0)
            {
				if (_StrFind(str_trans_type,"Z"))
                {
                    pay_bill_amt += pay_amt;
                    ++pay_bill_cnt;

                }
				if (_StrFind(str_trans_type,"R4"))   //20040112 重印是同時產生一正一負向
                {                               //正向減、負向加
                    pay_bill_amt += pay_amt;
                    ++pay_bill_cnt;
                    mpay_bill_amt -= pay_amt;
                    --mpay_bill_cnt;

                }
                
				if (_StrFind(str_trans_type, "R2") || _StrFind(str_trans_type,"R3"))
                {
                    mpay_bill_amt -= pay_amt;
                    --mpay_bill_cnt;
                }
            }
        }
    }

    i_cash_amt = (i_cash_z - i_cash_r);

    logsprintf("SumCash:現金合計, i_cash_z(%d), i_cash_r(%d), pay_bill_amt(%d)", i_cash_z, i_cash_r, pay_bill_amt );
   
    return i_cash_amt;
}




////不開發票金額  3050
int __fastcall BASIC::SumNOVoiceAmt(int StartLine, int TotalLine)
{
    string str_trans_type, temp, s, Am1, Am2;      //銷售型態
    giA = giB = 0;
	int iTmpA, iTmpB; // , iTmpC, iTmpD;
    iTmpA=iTmpB=0;

    if (gbl_tot_haveline)
    {
        //for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];
            str_trans_type=_StringSegment_EX(s, SGM, 8);

            Am1=_StringSegment_EX(s, SGM, 44);    //不開發票金額(應)
            Am2=_StringSegment_EX(s, SGM, 45);    //不開發票金額(免)

			if (_StrFind(str_trans_type,"Z"))
            {
                iTmpA += _StrToInt(Am1)+  _StrToInt(Am2);  //pay_add_pe
            }
			else if (_StrFind(str_trans_type, "R2") || _StrFind(str_trans_type,"R3"))
            {
                //iTmpA += _StrToInt(gtsl_tot->Strings[i].SubString(345,8));
                iTmpB +=  _StrToInt(Am1)+  _StrToInt(Am2);
            }
			else if (_StrFind(str_trans_type,"R4"))
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

    logsprintf("SumNOVoiceAmt:1050->(44,45)不開發票金額,合計金額 (Z0:%d),(R2R3R4:%d)", iTmpA,iTmpB);
  
    return giA;
}




////發票兌獎金額 3050
int __fastcall BASIC::VoicePayAmt(int StartLine, int TotalLine)
{
    string str_trans_type, temp, s, Am1, Am2;      //銷售型態
    giA = giB = 0;
	int iTmpA, iTmpB; // , iTmpC, iTmpD;
    iTmpA=iTmpB=0;

    if (gbl_tot_haveline)
    {
      // for (int i=0; i<gtsl_tot->Count; i++)
	   for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; //gtsl_tot->Strings[i];
            str_trans_type=_StringSegment_EX(s, SGM, 8);

            Am1=_StringSegment_EX(s, SGM, 25);    //支付項目6 發票兌獎金額

			if (_StrFind(str_trans_type,"Z"))
            {
                iTmpA += _StrToInt(Am1);
            }
			else if (_StrFind(str_trans_type, "R2") || _StrFind(str_trans_type,"R3"))
            {
                //iTmpA += _StrToInt(gtsl_tot->Strings[i].SubString(345,8));
                iTmpB +=  _StrToInt(Am1);
            }
			else if (_StrFind(str_trans_type,"R4"))
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

    logsprintf("VoicePayAmt:1050->(25)發票兌獎金額,合計金額 (Z0:%d)-(R2R3R4:%d)=%d ", iTmpA,iTmpB, giA);
   
    return giA;
}



//手輸部門/部門登入  3110
int __fastcall BASIC::SumInpdp(int StartLine, int TotalLine)
{
    string sRType, sType, str_dept,s ;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;
    int iDept = 0, iQty=0, iAmt=0;      //部門別
    //數量、金額
    giA = giB = 0;

    if (gbl_rec_haveline)
    {
       // for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;

            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                //seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                //seg9 = 38, seg10 = 19, seg11 = 82;
                continue;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            sType = _StringSegment_EX(s, SGM, seg1);
           
            iQty = _StrToInt(_StringSegment_EX(s, SGM, seg4));
            iAmt = _StrToInt(_StringSegment_EX(s, SGM, seg5));

            //判別部門
            str_dept = _StringSegment_EX(s, SGM, 20); // .substr(0, 2);  CD_DEP

            if (str_dept.length() > 0)
                iDept = _StrToInt(str_dept);
            else
                iDept = 20;         //無部門，預設20部門

            sRType = _StringSegment_EX(s, SGM, seg11).substr(0,2);

            //正常銷售
			if (_StrFind(sRType, "FF") || _StrFind(sRType, "Z0") || _StrFind(sRType,"Z1"))
            {
                if ((iDept == 13) || (iDept == 14))
                {
                    // sType = _StringSegment_EX(s, "|", 8);

					if (_StrFind(sType,"B0"))        //部門銷售
                    {
                        giA += iQty;
                        giB += iAmt;
                    }
					else if (_StrFind(sType,"B1"))   //銷售取消
                    {
                        giA -= iQty;
                        giB -= iAmt;
                    }
                }
            }
			else if (_StrFind(sRType, "R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {
                if ( (iDept==13) || (iDept==14) )
                {
                    // sType = gtsl_rec->Strings[i].SubString(68,2);

					if (_StrFind(sType,"B0"))        //部門銷售
                    {
                        giA -= iQty;
                        giB -= iAmt;
                    }
					else if (_StrFind(sType,"B1"))   //銷售取消
                    {
                        giA += iQty;
                        giB += iAmt;
                    }
                }
            }
        }
    }

    return 0;
}



//即時購代售  3110
int __fastcall BASIC::SumRelDimAgn(int StartLine, int TotalLine)
{
    string sRelType,sRType,sType,temp,s , sFG_SPC;  //連線商品區分
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;
    int  iRelType , iQty, iAmt;

    //數量、金額
    giA = giB = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                segDONGLE = 62, segNam = 18;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            //str_trans_type=_StringSegment_EX(s, "|", 8);
            sFG_SPC = _StringSegment_EX(s, SGM, seg2);

            iQty = _StrToInt(_StringSegment_EX(s, SGM, 26));
            iAmt = _StrToInt(_StringSegment_EX(s, SGM, 29));

            //連線商品區分
            sRelType = _StringSegment_EX(s, SGM, seg10).substr(0,2);  //CS_PRODTYPE
            iRelType=_StrToInt(sRelType);

            //交易狀態區分
            sRType = _StringSegment_EX(s, SGM, seg11).substr(0,2);
			if (_StrFind(sRType, "FF") || _StrFind(sRType, "Z0") || _StrFind(sRType,"Z1") || Trim(sRType) == "")
            {
                //2013/11/25  S0  VDC06                             $80 ==>條件為有代售付編號和有連線區分 => 即時購代售合計
                //if (iRelType==7 || iRelType==9)  //2006/12/20
                if (iRelType>0 )
                {
                    sType = _StringSegment_EX(s, SGM, 8);
					if (_StrFind(sType, "S0") || (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"9")))        //銷售   A0 09 預售商品
                    {
                        giA += iQty;
                        giB += iAmt;
                        logsprintf("SumRelDimAgn:即時購代售:%s-->+tot=%d", s.c_str(), giB);
                     }
					else if (_StrFind(sType, "S1") || (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"9")))   //更正    A0 09 預售商品
                    {
                        giA -= iQty;
                        giB -= iAmt;
                        logsprintf("SumRelDimAgn:即時購代售:%s-->-tot=%d", s.c_str(), giB);
                    }

                }
            }
			else if (_StrFind(sRType, "R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {
                //if (iRelType==7 || iRelType==9)
                if (iRelType>0 )
                {
                    sType = _StringSegment_EX(s, SGM, 8);
					if (_StrFind(sType, "S0") || (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"9")))        //部門銷售
                    {
                        giA -= iQty;
                        giB -= iAmt;
                        logsprintf("SumRelDimAgn:即時購代售:%s-->-tot=%d", s.c_str(), giB);
                    }
					else if (_StrFind(sType, "S1") || (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"9")))   //銷售取消
                    {
                        giA += iQty;
                        giB += iAmt;
                        logsprintf("SumRelDimAgn:即時購代售:%s-->+tot=%d", s.c_str(), giB);
                    }

                }
            }
        } // end of for
    }     // if (gbl_rec_haveline)

    logsprintf("SumRelDimAgn:即時購代售,合計金額 gbl_rec_haveline=%d, (%d,%d)", gbl_rec_haveline, giA, giB);
    return 0;
}


////預售類(折價券S0 10/MMS0 11 )折扣 3010
int __fastcall BASIC::SumPreSal_MM_DisAmtTMMFP(int StartLine, int TotalLine)
{
    string sRType, sType, temp, sFG_SPC, sDONGLE_TYPE, str_dept,s ;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE;

    logsprintf("SumPreSal_MMCPN_DisAmtTMMFP:預售類(MM)折扣  StartLine=%d, TotalLine=%d",StartLine, TotalLine );
   
    //數量、金額
    giU = giV = giW = giX = 0;
    int iAmt;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;

            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
               // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
               // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }


            sType = _StringSegment_EX(s, SGM, seg1);                          //銷貨型態
            sFG_SPC = _StringSegment_EX(s, SGM, seg2);                        //FG_SPC
            sDONGLE_TYPE = _StringSegment_EX(s, SGM, segDONGLE);              //DONGLE_TYPE
            iAmt=_StrToInt(_StringSegment_EX(s, SGM, seg5));                  //AM_ITEM
            sRType = _StringSegment_EX(s, SGM, seg11).substr(0,2);            //seg11.交易狀態區分 

            //正常銷售
			if (_StrFind(sRType, "FF") || _StrFind(sRType, "Z0") || _StrFind(sRType,"Z1"))
            {

				if (_StrFind(sType, "S0") && _StrFind(sFG_SPC,"11"))        //類(/MM)
                   {
                    giU += iAmt;
                   }
				else if (_StrFind(sType, "S1") && _StrFind(sFG_SPC,"11"))        //類(/MM)
                   {
                       giU -= iAmt;
                   }
             }
			else if (_StrFind(sRType, "R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {
				if (_StrFind(sType, "S0") && _StrFind(sFG_SPC,"11"))        //類(MM)
                   {
                      giV += iAmt;
                   }
				else if (_StrFind(sType, "S1") && _StrFind(sFG_SPC,"11"))        //類(MM)
                   {
                      giV -= iAmt;
                   }

            }
        }
    }

    logsprintf("SumPreSal_MMCPN_DisAmtTMMFP:預售類(MM)折扣 Z0類(MM)=%d, R2R3類(MM)=%d",giU, giV  );

    return 0;
}



//預售類(類折價券:giU , 類MM_MFP:giV , 類MM_TM:giX
int __fastcall BASIC::SumPreSal_MMCPN_DisAmt(int StartLine, int TotalLine)
{
    string sRType, sType, temp, sFG_SPC, sDONGLE_TYPE, str_dept,s ;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;
    logsprintf("SumPreSal_MMCPN_DisAmt:預售類(折價券/MM)折扣  StartLine=%d, TotalLine=%d",StartLine, TotalLine );

    //數量、金額
    giU = giV = giW = giX = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            sType = _StringSegment_EX(s, SGM, seg1);
            sFG_SPC = _StringSegment_EX(s, SGM, seg2);
            sDONGLE_TYPE = _StringSegment_EX(s, SGM, segDONGLE);
            sRType = _StringSegment_EX(s, SGM, seg11).substr(0,2);

            //正常銷售
			if (_StrFind(sRType, "FF") || _StrFind(sRType, "Z0") || _StrFind(sRType,"Z1"))
            {
				if (_StrFind(sType, "S0") && _StrFind(sFG_SPC,"10"))        //類(折價券)
                    {
                        giU += _StrToInt(_StringSegment_EX(s, SGM, seg5));
                    }
				else if (_StrFind(sType, "S1") && _StrFind(sFG_SPC,"10"))        //類(折價券)
                    {
                        giU -= _StrToInt(_StringSegment_EX(s, SGM, seg5));
                    }
				else if (_StrFind(sType, "S0") && _StrFind(sFG_SPC,"11"))        //類(/MM)
                   {
					   
					   if (_StrFind(sDONGLE_TYPE, "TM") )
						   giX += _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_TM
   				       else
						   giV += _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_MFP

                   }
				else if (_StrFind(sType, "S1") && _StrFind(sFG_SPC, SGM))        //類(/MM)
                   {
					   
					   if (_StrFind(sDONGLE_TYPE, "TM"))
						   giX -= _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_TM
					   else
						   giV -= _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_MFP

                   }

            }
			else if (_StrFind(sRType, "R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {

				if (_StrFind(sType, "S0") && _StrFind(sFG_SPC,"10"))        //類(折價券)
                    {
                        giU -= _StrToInt(_StringSegment_EX(s, SGM, seg5));
                    }
				else if (_StrFind(sType, "S1") && _StrFind(sFG_SPC,"10"))        //類(折價券)
                    {
                        giU += _StrToInt(_StringSegment_EX(s, SGM, seg5));

                    }
				else if (_StrFind(sType,"S0") && _StrFind(sFG_SPC,"11"))        //類(MM)
                   {
					   
					   if (_StrFind(sDONGLE_TYPE, "TM"))
						   giX -= _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_TM
					   else
						   giV -= _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_MFP

                   }
				else if (_StrFind(sType, "S1") && _StrFind(sFG_SPC,"11"))        //類(MM)
                   {
					   
					   if (_StrFind(sDONGLE_TYPE, "TM"))
						   giX += _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_TM
					   else
						   giV += _StrToInt(_StringSegment_EX(s, SGM, seg5));   //類MM_MFP
                   }

            }
        }
    }

    giV=giV*(-1);
    giX=giX*(-1);
    logsprintf("SumPreSal_MMCPN_DisAmt:預售類(折價券/MM)折扣  類(折價券)=%d, 類(MM_MFP)=%d, 類(MM_TM)=%d",giU, giV, giX  );

    return 0;
}



//預售兌換商品折讓單折扣/代收商品折讓單折扣:giU  3110
int __fastcall BASIC::SumPreSal_INVO_DisAmt(int StartLine, int TotalLine)
{
    string sRType, sType, temp, sFG_SPC, sDONGLE_TYPE, str_dept,s ;

    logsprintf("SumPreSal_INVO_DisAmt:代收商品折讓單折扣  StartLine=%d, TotalLine=%d",StartLine, TotalLine );

    //數量、金額
    giU = giV = giW = giX = 0;
    int iAM_SUB_SUB, iAM_ITEM, iQty;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            sType = _StringSegment_EX(s, SGM, seg1);
            sFG_SPC = _StringSegment_EX(s, SGM, seg2);
            sDONGLE_TYPE = _StringSegment_EX(s, SGM, segDONGLE);
            sRType = _StringSegment_EX(s, SGM, seg11).substr(0,2);
            iAM_ITEM = _StrToInt(_StringSegment_EX(s, SGM, seg5));
            iAM_SUB_SUB = _StrToInt(_StringSegment_EX(s, SGM, seg7));
            iQty = iAM_SUB_SUB/3;

            //正常銷售
			if (_StrFind(sRType, "FF") || _StrFind(sRType, "Z0") || _StrFind(sRType,"Z1"))
            {
				if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"10"))        //預售兌換商品
                    {
                        giU += iAM_SUB_SUB;                          //折讓單金額攤提
                        giV += iQty;
                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"10"))
                    {
                        giU -= iAM_SUB_SUB;                          //折讓單金額攤提
                        giV -= iQty;

                    }
                    /*****
                   else if  (sType.Pos("S0") && sFG_SPC.Pos("10") )
                   {
                      giW += iAM_ITEM;                              //類 CPN
                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("10") )
                   {
                      giW -= iAM_ITEM;                              //類 CPN
                   }
                   else if (sType.Pos("S0") && sFG_SPC.Pos("11") )
                   {
                       //類 MM
                       if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") )
                            giX += iAM_ITEM;   //FP+MFP
                       else
                            giY += iAM_ITEM;   //TM

                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("11") )
                   {
                       //類 MM
                       if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") )
                            giX -= iAM_ITEM;   //FP+MFP
                       else
                            giY -= iAM_ITEM;   //TM
                   }
                  ******/
             }
			else if (_StrFind(sRType, "R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {

				if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"10"))        //預售兌換商品
                    {
                        giU -= iAM_SUB_SUB;                         //折讓單金額攤提
                        giV -= iQty;
                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"10"))
                    {
                        giU += iAM_SUB_SUB;                        //折讓單金額攤提
                        giV += iQty;
                    }
                   /*******
                   else if  (sType.Pos("S0") && sFG_SPC.Pos("10") )
                   {
                      giW -= iAM_ITEM;                              //類 CPN
                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("10") )
                   {
                      giW += iAM_ITEM;                              //類 CPN
                   }
                   else if (sType.Pos("S0") && sFG_SPC.Pos("11") )
                   {
                       //類 MM
                       if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") )
                            giX -= iAM_ITEM;   //FP+MFP
                       else
                            giY -= iAM_ITEM;   //TM

                   }
                   else if  (sType.Pos("S1") && sFG_SPC.Pos("11") )
                   {
                       //類 MM
                       if( sDONGLE_TYPE.Pos("FP") || sDONGLE_TYPE.Pos("MFP") )
                            giX += iAM_ITEM;   //FP+MFP
                       else
                            giY += iAM_ITEM;   //TM
                   }
                   *******/

              }
        }
    }

    giU = giU*(-1);
    //temp.sprintf("SumPreSal_INVO_DisAmt:代收商品折讓單折扣(Qt=%d, AM=%d), 類CPN=%d, 類MM_MFP=%d , 類MM_TM=%d",giU, giV, giW, giX, giY   );
    logsprintf("SumPreSal_INVO_DisAmt:代收商品折讓單折扣(Qt=%d, AM=%d)",giU, giV   );
   
    return 0;
}



//預售及兌換商品統計 : 預售開發票不入營收 ,  兌換入營收不開發票 3010
int __fastcall BASIC::SumPreSal_Amt(int StartLine, int TotalLine)
{
    string sRType, sType, temp, sFG_SPC, sDONGLE_TYPE, str_dept,s, sTx_Flg ;

    //temp.sprintf("SumPreSal_INVO_DisAmt:代收商品折讓單折扣  StartLine=%d, TotalLine=%d",StartLine, TotalLine );
    //writelog(temp);

    //數量、金額
    giU = giK = 0;     //兌換商品     應稅/免稅
    giG = giH = 0;     //R3R3兌換商品 應稅/免稅

    giV = giL = 0;     //預售商品     應稅/免稅
    giI = giJ = 0;     //R2R3預售商品 應稅/免稅

    giW = giM = 0;     //R4兌換商品   應稅/免稅
    giX = giN = 0;     //R4預售商品   應稅/免稅


    //giO = giP = giQ = giR = 0;    //R2R3免稅


    int iAM_SUB_SUB, iAM_ITEM, i_type;
    int seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE;
    if (gbl_rec_haveline)
    {
       // for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
                // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
                // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11;

                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }


            sType = _StringSegment_EX(s, SGM, seg1);                   //銷貨型態
            sFG_SPC = _StringSegment_EX(s, SGM, seg2);                 //FG_SPC   
            sDONGLE_TYPE = _StringSegment_EX(s, SGM, segDONGLE);       //DONGLE_TYPE
           
            iAM_ITEM = _StrToInt(_StringSegment_EX(s, SGM, seg5));     //AM_ITEM
            iAM_SUB_SUB = _StrToInt(_StringSegment_EX(s, SGM, seg7));  //AM_SUB_SUB 
            sTx_Flg = _StringSegment_EX(s, SGM, seg9);
            sRType = _StringSegment_EX(s, SGM, seg11).substr(0, 2);      //seg11.交易狀態區分 

            //正常銷售
			if (_StrFind(sRType, "FF") || _StrFind(sRType, "Z0") || _StrFind(sRType,"Z1"))
            {
				if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"10"))        //預售兌換商品
                    {
                        if (sTx_Flg == "1")  //應稅
                             giU += iAM_ITEM; // - iAM_SUB_SUB:折讓單金額攤提
                        else
                             giK += iAM_ITEM; // - iAM_SUB_SUB:折讓單金額攤提
                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"10"))
                    {
                        if (sTx_Flg == "1")  //應稅
                             giU -= iAM_ITEM; // - iAM_SUB_SUB:折讓單金額攤提
                        else
                             giK -= iAM_ITEM; // - iAM_SUB_SUB:折讓單金額攤提
                    }
				else if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"9"))        //預售商品
                    {
                        if (sTx_Flg == "1")  //應稅
                            giV += iAM_ITEM;
                        else
                            giL += iAM_ITEM; //免稅
                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"9"))        //預售商品
                    {
                        if (sTx_Flg == "1")  //應稅
                            giV -= iAM_ITEM;
                        else
                            giL -= iAM_ITEM; //免稅
                    }


             }
			else if (_StrFind(sRType, "R2") || _StrFind(sRType,"R3"))    //前筆誤打、退貨
            {

				if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"10"))        //預售兌換商品
                    {
                         if (sTx_Flg == "1")  //應稅
                             giG -= iAM_ITEM;
                         else
                             giH -= iAM_ITEM;
                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"10"))
                    {
                         if (sTx_Flg == "1")  //應稅
                             giG += iAM_ITEM;                        //折讓單金額攤提
                         else
                             giH += iAM_ITEM;
                    }
				else if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"9"))        //預售商品
                    {
                         if (sTx_Flg == "1")  //應稅
                             giI -= iAM_ITEM;
                         else
                             giJ -= iAM_ITEM;

                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"9"))
                    {
                        if (sTx_Flg == "1")  //應稅
                            giI += iAM_ITEM;
                        else
                            giJ += iAM_ITEM;
                    }

            }
			else if (_StrFind(sRType,"R4"))    //卡紙重印
            {

				if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"10"))        //預售兌換商品
                    {
                        if (sTx_Flg == "1")  //應稅
                            giW += iAM_ITEM;                         //折讓單金額攤提
                        else
                            giM += iAM_ITEM;
                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"10"))
                    {
                        if (sTx_Flg == "1")  //應稅
                            giW -= iAM_ITEM;                        //折讓單金額攤提
                        else
                            giM -= iAM_ITEM;
                    }
				else if (_StrFind(sType, "A0") && _StrFind(sFG_SPC,"9"))        //預售商品
                    {
                        if (sTx_Flg == "1")  //應稅
                            giX += iAM_ITEM;
                        else
                            giN += iAM_ITEM;

                    }
				else if (_StrFind(sType, "A1") && _StrFind(sFG_SPC,"9"))
                    {
                        if (sTx_Flg == "1")  //應稅
                            giX -= iAM_ITEM;
                        else
                            giN -= iAM_ITEM;
                    }

              }


        }
    }

    logsprintf("SumPreSal_Amt:預售兌換商品     (兌換商品應稅=%d, 兌換商品免稅=%d, 預售商品應稅=%d,預售商品免稅=%d )",giU, giK, giV, giL   );
   
    logsprintf("SumPreSal_Amt:預售兌換商品 R2R3(兌換商品應稅=%d, 兌換商品免稅=%d, 預售商品應稅=%d,預售商品免稅=%d )",giG, giH, giI, giJ   );
   
    logsprintf("SumPreSal_Amt:預售兌換商品   R4(兌換商品應稅=%d, 兌換商品免稅=%d, 預售商品應稅=%d,預售商品免稅=%d )",giW, giM, giX ,giN   );
    
    //R3R3兌換商品 應稅/免稅
    giG =  giG *(-1);
    giH =  giH *(-1);

    //R2R3預售商品 應稅/免稅
    giI = giI *(-1);
    giJ = giJ *(-1);

    //R4兌換商品   應稅/免稅
    giW = giW; // * (-1);
    giM = giM; // * (-1);

    //R4預售商品   應稅/免稅
    giX = giX; // * (-1);
    giN = giN; // * (-1);

    return 0;
}




//發票金額零次數
int __fastcall BASIC::TotZero(int StartLine, int TotalLine)
{
    int iCashZero = 0;      //發票金額為0次數
    int i_invo_amt = 0;     //發票金額
    string str_type,s;

    unsigned int str_invoice_start = 0, str_invoice_end = 0;

    if (gbl_tot_haveline)
    {
        //for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];

            i_invo_amt = _StrToInt(_StringSegment_EX(s, SGM, 32));

            str_invoice_start = _StrToInt(_StringSegment_EX(s, SGM, 9).substr(2,8));
            str_invoice_end   = _StrToInt(_StringSegment_EX(s, SGM, 10).substr(2,8));
            str_type = _StringSegment_EX(s, "|", 8);

			if (_StrFind(str_type,"X"))      //LOG出現不表狀況不處理
                continue;
            // 2007/09/18 Update Lu
            //if (str_type=="R2" || str_type=="R3" || str_type=="R4")      //LOG出現不表狀況不處理
			if (str_type == "R2" || str_type == "R3" || _StrFind(str_type,"V"))      //LOG出現不表狀況不處理
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

//交易取消  3050
int __fastcall BASIC::SumVoid(int StartLine, int TotalLine)
{
    string str_type,s;    //銷售型態

    //次數、金額
    giA = giB = 0;

    if (gbl_tot_haveline)
    {
        //for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
			s = *ls; // gtsl_tot->Strings[i];
            str_type=_StringSegment_EX(s, SGM, 8);  //SALE_TYPE

			if (_StrFind(str_type,"V"))      //V為交易取消
            {
                ++giA;
                giB += _StrToInt(_StringSegment_EX(s, SGM, 32));  //AM_INVO
            }
        }
    }

    if(  abs(giB) > 9999999 )
       giB=9999999;

    logsprintf("SumVoid:交易取消,合計(次數=%d, 金額=%d) ",giA, giB);
    
    return 0;
}




//更正 合計  3010
int __fastcall BASIC::SumUpdat(int StartLine, int TotalLine)
{

    string str_sale_type;       //銷貨型態
    string str_r_type;          //誤打、退貨型態
    string str_spc_flag,s;        //交易類別
    int ifg_spc;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;
    //次數、金額
    giA = giB = 0;

    //TStringList *t = new TStringList();
    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                segDONGLE = 11, segNam = 18;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            str_sale_type = _StringSegment_EX(s, SGM, seg1);
            str_r_type = _StringSegment_EX(s, SGM, seg11);     // 交易狀態區分

            //A1:正常銷售取消    B1:部門銷售取消   M1:Menu銷售取消
            //S0:特殊交易     S1:特殊交易取消

			if (_StrFind(str_r_type,"X"))
                continue;

            //if (str_sale_type.Pos("1"))
			if (_StrFind(str_sale_type, "A1") || _StrFind(str_sale_type,"B1"))
            {
				str_spc_flag = _StringSegment_EX(s, SGM, seg2);   //交易類別
                ifg_spc=_StrToInt(str_spc_flag);

                //str_spc_flag.Pos("4") || str_spc_flag.Pos("5") ||  str_spc_flag.Pos("6") || str_spc_flag.Pos("9") )
                if ( ifg_spc==4 || ifg_spc==5 || ifg_spc==6 || ifg_spc==9 )
                    continue;

				if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1"))
                {
                    ++giA;
                    giB += _StrToInt(_StringSegment_EX(s, SGM, seg5));  //seg5.AM_ITEM:31,
                    //t->Add(gtsl_rec->Strings[i]);
                }
				else if (_StrFind(str_r_type, "R2") || _StrFind(str_r_type,"R3"))
                {
                    --giA;
                    giB -= _StrToInt(_StringSegment_EX(s, SGM, seg5)); //seg5.AM_ITEM:31,
                    //t->Add(gtsl_rec->Strings[i]);
                }
            }
        }
    }

    //t->SaveToFile("e:\\a.txt");  _ListToFile(tslCrp, SCasherRpt.c_str());
    //delete t;
    return 0;
}

//廢棄 3110
int __fastcall BASIC::SumLOSS(int StartLine, int TotalLine)
{
    string str_sale_type,str_r_type,s;    //銷貨型態
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;
    int iQty, iAmt;
    //次數、金額
    giA = giB = 0;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                //seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                //seg9 = 38, seg10 = 19, seg11 = 82;
                continue;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            str_sale_type = _StringSegment_EX(s, SGM, seg1);

            iQty = _StrToInt(_StringSegment_EX(s, SGM, seg4));
            iAmt = _StrToInt(_StringSegment_EX(s, SGM, seg5));

            str_r_type = _StringSegment_EX(s, SGM, seg11);
 
			if (_StrFind(str_r_type,"X"))
                continue;

			if (_StrFind(str_sale_type,"I0"))          //正常
            {
                giA += iQty;
                giB += iAmt;
            }
			else if (_StrFind(str_sale_type,"I1"))    //取銷
            {
                giA -= iQty;
                giB -= iAmt;
            }
        }
    }

    return 0;
}

//計算代現金 刷卡合計  3050
int __fastcall BASIC::SumTbCash(int StartLine, int TotalLine)
{
    //禮次、金  折次、金   溢次、金
    giA = giB = giC = giD = giG = giH = giI = giJ = giK = 0;
    giE=giF=giL=giM=0;

    int iT1, iT2, iT3, iT4, iT5, iT6, iT7, iT8, iTT, iT9, iT10, iT11, iT12, iT13,
        iTOt3, iTOt2Rtn, iTOt2RtnOld, iT26, iIPss, iT28, iT52;
    iT1=iT2=iT3=iT4=iT5=iT6=iT7=iT8=iT9=iT10=iTT=iT11=iT12=iT13=iT26=iIPss=iT28=iT52=0;
    iTOt3=iTOt2Rtn=iTOt2RtnOld=0;                 // 2015/02/12 新增手機支付iT12  2015/12/25 新增HappyCash支付iT13

    string str_sale_type, strtmp,tmpSer, s;       //結帳型態

    if (gbl_tot_haveline)
    {
        //for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {
            iTOt2Rtn=0;
            iTOt2RtnOld=0;
			s = *ls; // gtsl_tot->Strings[i];
            str_sale_type = _StringSegment_EX(s, SGM, 8);
            tmpSer=_StringSegment_EX(s, "|", 7);    //交易序號

			if (_StrFind(str_sale_type, "Z0") || _StrFind(str_sale_type,"V1"))
            {
				if (_StrFind(str_sale_type,"V1"))   // 2012/02/02
                 {
                  //電子錢金額

                  iT5 = _StrToInt(_StringSegment_EX(s, SGM, 16));
                  if (iT5)
                     {
                      ++giI;
                      giJ += iT5;
                      logsprintf("SumTbCash:刷卡合計:%s-->V1電子錢金額:+%d, tot=%d", s.c_str(), iT5 , giJ);
                     }

                  // iIPass 高捷
                  iIPss = _StrToInt(_StringSegment_EX(s, SGM, 27));
                  if (iIPss)
                     {
                       ++giI;
                       giJ += iIPss;
                       logsprintf("SumTbCash:刷卡合計:%s(iIPass V1高捷:%d, %d)", s.c_str(), iIPss , giJ);
                     }

                  // HappyCash
                  iT13 = _StrToInt(_StringSegment_EX(s, SGM, 51));
                  if (iT13)
                     {
                       ++giI;
                       giJ += iT13;
                       logsprintf("SumTbCash:刷卡合計:%s(iT13 V1HappyCash:%d, %d)", s.c_str(), iT13 , giJ);
                     }

                  //悠遊卡兌點折抵  iT28
                  iT28 = _StrToInt(_StringSegment_EX(s, SGM, 28));
                  if (iT28)
                     {
                       ++giI;
                       giJ += iT28;
                       logsprintf("SumTbCash:刷卡合計:%s(iT28 V1悠遊卡兌點折抵:%d, %d)", s.c_str(), iT28 , giJ);
                     }

                  continue;
                 }  // end of   if(str_sale_type.Pos("V1"))

               //現金
               iT6 = _StrToInt(_StringSegment_EX(s, SGM, 15));
               //HappyGo金額
                iT11 = _StrToInt(_StringSegment_EX(s, SGM, 12));
                if (iT11)
                {
                    ++giI;
                    giJ += iT11;
                    logsprintf("SumTbCash:刷卡合計:%s-->HappyGo金額:+%d, tot=%d", s.c_str(), iT11 , giJ);
                }

                //商品禮券支付淨額  餘額券修正 2007/10/01
                iTOt3 = _StrToInt(_StringSegment_EX(s, SGM, 13));
               //悠遊卡金額
                iT5 = _StrToInt(_StringSegment_EX(s, SGM, 16));
                if (iT5)
                {
                    ++giI;
                    giJ += iT5;
                    logsprintf("SumTbCash:刷卡合計:%s-->悠遊卡金額:+%d, tot=%d", s.c_str(), iT5 , giJ);
                }

                // 信用卡
                iT7 = _StrToInt(_StringSegment_EX(s, SGM, 17));
                if (iT7)
                {
                    ++giI;
                    giJ += iT7;
                    logsprintf("SumTbCash:刷卡合計:%s(信用卡金額:%d, %d)", s.c_str(), iT7 , giJ);
                }

                //禮券
                iT1 = _StrToInt(_StringSegment_EX(s, SGM, 18));
                if (iT1)
                {
                    ++giA;
                    giB += iT1;
                }

                //折價券 開發票折價券付款
                iT2 = _StrToInt(_StringSegment_EX(s, SGM, 19));
                if (iT2)
                {
                    ++giC;
                    giD += iT2;
                }

                //折價券  Other1     不開發票折價券支付
                iT9 = _StrToInt(_StringSegment_EX(s, SGM, 23));
                if (iT9)
                {
                    ++giC;
                    giD += iT9;
                }

                //有價票券 CPN    開發票廠商折價券付款
                iT3 = _StrToInt(_StringSegment_EX(s, SGM, 20));
                if (iT3)
                {
                    ++giC;
                    giD += iT3;
                }

                //紅利兌換
                iT10 = _StrToInt(_StringSegment_EX(s, SGM, 22));
                if (iT10)
                {
                    ++giC;
                    giD += iT10;
                }

                //商品禮券,合併到禮券
                iT8 = _StrToInt(_StringSegment_EX(s, SGM, 24));
                //if (iT8)
                //{
                    ++giA;
                    giB += iT8;

                    if( (iT8-iTOt3) > 0 ) //餘額券修正  表示 iTo3 是商品禮券淨額
                      {
                          iTOt2RtnOld=iT8-iTOt3;
                          //giE+=iTOt2Rtn;
                      }

                    iTOt2Rtn=_StrToInt(_StringSegment_EX(s, SGM, 14));
                    giE+=iTOt2Rtn;        //餘額券
                    if( iTOt2Rtn > 0)
                       ++giF;
                //}

                //電子禮券禮物卡
                iT26 = _StrToInt(_StringSegment_EX(s, SGM, 26));
                if (iT26)
                {
                    ++giL;
                    giM += iT26;
                }

                // iIPass 高捷
                iIPss = _StrToInt(_StringSegment_EX(s, SGM, 27));
                if (iIPss)
                {
                    ++giI;
                    giJ += iIPss;
                    logsprintf("SumTbCash:刷卡合計:%s(iIPass 高捷:%d, %d)", s.c_str(), iIPss , giJ);
                }

                //悠遊卡兌點折抵  iT28
                iT28 = _StrToInt(_StringSegment_EX(s, SGM, 28));
                if (iT28)
                {
                     ++giI;
                     giJ += iT28;
                     logsprintf("SumTbCash:刷卡合計:%s(iT28 悠遊卡兌點折抵:%d, %d)", s.c_str(), iT28 , giJ);
                }

                //手機支付金額
                iT12 = _StrToInt(_StringSegment_EX(s, SGM, 29));
                if (iT12)
                {
                    ++giI;
                    giJ += iT12;
                   logsprintf("SumTbCash:刷卡合計:%s-->手機支付金額:+%d, tot=%d", s.c_str(), iT12 , giJ);
                }

                //HappyCash支付金額
                iT13 = _StrToInt(_StringSegment_EX(s, SGM, 51));
                if (iT13)
                {
                    ++giI;
                    giJ += iT13;
                    logsprintf("SumTbCash:刷卡合計:%s-->HappyCash支付金額:+%d, tot=%d", s.c_str(), iT13 , giJ);
                }

				// FamiPoint  2019/12/25    TM_PG_新增Fami Point支付帳表
				iT52 = _StrToInt(_StringSegment_EX(s, SGM, 52));    //備用
				if (iT52)
				{
					++giI;
					giJ += iT52;
					logsprintf("SumTbCash:刷卡合計:%s-->FamiPoint支付金額:+%d, tot=%d", s.c_str(), iT52, giJ);
				}



                //TOT_AMT
                iTT = _StrToInt(_StringSegment_EX(s, SGM, 31));

                //溢收金額
                iT4 = _StrToInt(_StringSegment_EX(s, SGM, 34));
                if (iT4)
                {
                    ++giG;
                    giH += iT4;
                    //writelog(gtsl_tot->Strings[i]);
                }

                
                if( iTT != (iT6+iT5+iT1+iT2+iT3+iT7+iT8+iT9+iT10+iT26+iT11+iT12+iT13+iIPss-iT4-iTOt2Rtn) )
                  {
                   logsprintf("SumTbCash: 交易序號(%s) 借貸不平 , 借:銷售金額=%d, 貸:(現=%d)+(IC=%d)+(信=%d)+(禮=%d)+(商品禮券=%d)+(折=%d)+(CPN=%d)+(OT1=%d)+(紅利=%d)+(HPG=%d)+(手機支付=%d)+(HappyCash支付=%d)+(iIPss=%d)-(溢=%d)",
					   tmpSer.c_str(), iTT, iT6, iT5, iT7, iT1, iT8 + iT26, iT2, iT3, iT9, iT10, iT11, iT12, iT13, iIPss, iT4 + iTOt2Rtn);
                  }

                //if( iTOt2RtnOld != iTOt2Rtn )
                //  {
                //   strtmp.sprintf("SumTbCash: 交易序號(%s) 找餘額券金額異常 iTOt2RtnOld=%d 不等於 iTOt2Rtn=%d ",
                //                         tmpSer,iTOt2RtnOld, iTOt2Rtn);
                //   writelog(strtmp);
                //  }


            }
			else if (_StrFind(str_sale_type, "R2") || _StrFind(str_sale_type,"R3"))
            {
               //現金
               iT6 = _StrToInt(_StringSegment_EX(s, SGM, 15));
               //HappyGo金額
               iT11 = _StrToInt(_StringSegment_EX(s, SGM, 12));
               if (iT11)
                {
                    --giI;
                    giJ -= iT11;
                    logsprintf("SumTbCash:刷卡合計:%s-->HappyGo金額:-%d, tot=%d", s.c_str(), iT11 , giJ);
                }

                //商品禮券支付淨額  餘額券修正 2007/10/01
                iTOt3 = _StrToInt(_StringSegment_EX(s, SGM, 13));

                //悠遊卡金額
                iT5 = _StrToInt(_StringSegment_EX(s, SGM, 16));
                if (iT5)
                {
                    --giI;
                    giJ -= iT5;
                    logsprintf("SumTbCash:刷卡合計:%s-->悠遊卡金額:-%d, tot=%d", s.c_str(), iT5 , giJ);
                }

                // 信用卡
                iT7 = _StrToInt(_StringSegment_EX(s, SGM, 17));
                if (iT7)
                {
                    --giI;
                    giJ -= iT7;
                   logsprintf("SumTbCash:刷卡合計:%s-->信用卡金額:-%d, tot=%d", s.c_str(), iT7 , giJ);
                }

                //禮券
                iT1 = _StrToInt(_StringSegment_EX(s, SGM, 18));
                if (iT1)
                {
                    --giA;
                    giB -= iT1;
                }


                //折價券 開發票折價券付款
                iT2 = _StrToInt(_StringSegment_EX(s, SGM, 19));
                if (iT2)
                {
                    --giC;
                    giD -= iT2;
                }

                //折價券  Other1
                iT9 = _StrToInt(_StringSegment_EX(s, SGM, 23));
                if (iT9)
                {
                    --giC;
                    giD -= iT9;
                }

                 //有價票券 CPN
                iT3 = _StrToInt(_StringSegment_EX(s, SGM, 20));
                if (iT3)
                {
                    --giC;
                    giD -= iT3;

                }

                //紅利兌換
                iT10 = _StrToInt(_StringSegment_EX(s, SGM, 22));
                if (iT10)
                {
                    --giC;
                    giD -= iT10;
                }

                //商品禮券,合併到禮券
                iT8 = _StrToInt(_StringSegment_EX(s, SGM, 24));
                //if (iT8)
                //{
                    --giA;
                    giB -= iT8;
                    //giB -= iTOt3;

                    if( (iT8-iTOt3) > 0 )  //餘額券修正  表示 iTo3 是商品禮券淨額
                      {
                          iTOt2RtnOld=iT8-iTOt3;
                    //      giE-=iTOt2Rtn;
                      }
                   iTOt2Rtn=_StrToInt(_StringSegment_EX(s, SGM, 14));
                   giE-=iTOt2Rtn;        // 餘額券
                   if( iTOt2Rtn > 0)
                       ++giF;
                //}


                //電子禮券禮物卡
                iT26 = _StrToInt(_StringSegment_EX(s, SGM, 26));
                if (iT26)
                {
                    --giL;
                    giM -= iT26;
                }

                 // iIPass 高捷
                iIPss = _StrToInt(_StringSegment_EX(s, SGM, 27));
                if (iIPss)
                {
                    --giI;
                    giJ -= iIPss;
                    logsprintf("SumTbCash:刷卡合計:%s-->iIPass 高捷:-%d, tot=%d", s.c_str(), iIPss , giJ);
                }

                //悠遊卡兌點折抵  iT28
                iT28 = _StrToInt(_StringSegment_EX(s, SGM, 28));
                if (iT28)
                {
                     --giI;
                     giJ -= iT28;
                     logsprintf("SumTbCash:刷卡合計:%s-->iT28 悠遊卡兌點折抵:-%d, tot=%d", s.c_str(), iT28 , giJ);
                }

                //手機支付金額
                iT12 = _StrToInt(_StringSegment_EX(s, SGM, 29));
                if (iT12)
                {
                    --giI;
                    giJ -= iT12;
                    logsprintf("SumTbCash:刷卡合計:%s-->手機支付金額:-%d, tot=%d", s.c_str(), iT12 , giJ);
                }

                //HappyCash支付金額
                iT13 = _StrToInt(_StringSegment_EX(s, SGM, 51));
                if (iT13)
                {
                    --giI;
                    giJ -= iT13;
                    logsprintf("SumTbCash:刷卡合計:%s-->HapyCash支付金額:-%d, tot=%d", s.c_str(), iT13 , giJ);
                }


				// FamiPoint  2019/12/25    TM_PG_新增Fami Point支付帳表
				iT52 = _StrToInt(_StringSegment_EX(s, SGM, 52));    //備用
				if (iT52)
				{
					--giI;
					giJ -= iT52;
					logsprintf("SumTbCash:刷卡合計:%s-->R2R3FamiPoint支付金額:+%d, tot=%d", s.c_str(), iT52, giJ);
				}


                //TOT_AMT
                iTT = _StrToInt(_StringSegment_EX(s, SGM, 31));

                //溢收金額
                iT4 = _StrToInt(_StringSegment_EX(s, SGM, 34));

                if (iT4)
                {
                    --giG;
                    giH -= iT4;
                    //writelog(gtsl_tot->Strings[i]);
                }
				                

                if( iTT != (iT6+iT5+iT1+iT2+iT3+iT7+iT8+iT9+iT10+iT26+iT11+iT12+iT13+iIPss-iT4-iTOt2Rtn) )
                  {
                   logsprintf("SumTbCash: 交易序號(%s) R借貸不平 , 借:銷售金額=%d, 貸:(現=%d)+(IC=%d)+(信=%d)+(禮=%d)+(商品禮券=%d)+(折=%d)+(CPN=%d)+(OT1=%d)+(紅利=%d)+(HPG=%d)+(手機支付=%d)+(HappyCash支付=%d)+(iIPss=%d)-(溢=%d)",
					   tmpSer.c_str(), iTT, iT6, iT5, iT7, iT1, iT8 + iT26, iT2, iT3, iT9, iT10, iT11, iT12, iT13, iIPss, iT4 + iTOt2Rtn);
                  }

            }
        }
    }

    return 0;
}

//計算門市提貨券  3240
int __fastcall BASIC::SumBill78(int StartLine, int TotalLine)
{


    int iTCnt, iTAmt, iT4, iPayType,iOtCnt,iOtAmt;

    string str_sale_type,stmp,s;       //結帳型態

    iTCnt = iTAmt = iT4 = iOtCnt = iOtAmt = 0;

    Cpn71_cnt=0;                  //折價券71次數
    Cpn71_amt=0;                  //折價券71金額
    Cpn72_cnt=0;                  //折價券72次數
    Cpn72_amt=0;                  //折價券72金額
    Cpn73_cnt=0;                  //折價券73次數
    Cpn73_amt=0;                  //折價券73金額
    Cpn74_cnt=0;                  //折價券74次數
    Cpn74_amt=0;                  //折價券74金額
    Cpn75_cnt=0;                  //折價券75次數
    Cpn75_amt=0;                  //折價券75金額
    Cpn76_cnt=0;                  //折價券76次數
    Cpn76_amt=0;                  //折價券76金額

    Cpn77_cnt=0;                  //門市提貨券77次數
    Cpn77_amt=0;                  //門市提貨券77金額
    Cpn78_cnt=0;                  //門市提貨券78次數
    Cpn78_amt=0;                  //門市提貨券78金額

   // writelog("刪除檔案 " + str_tmp_ini);
    if (gbl_bil_haveline)
    {
       //for (int i=0; i<gtsl_bil->Count; i++)
		for (list<string>::iterator ls = gtsl_bil->begin(); ls != gtsl_bil->end(); ls++)
        {
			s = *ls; // gtsl_bil->Strings[i];;
            str_sale_type = _StringSegment_EX(s, SGM, 9);
            stmp=_StringSegment_EX(s, SGM, 11);      //PAY_TYPE_ID
            iPayType=_StrToInt(stmp,"支付類別");
            iTAmt = _StrToInt(_StringSegment_EX(s, SGM, 13), "支付金額");  //AM_BILL_REC
           // if (iTAmt)
               iTCnt=1;
			   if (_StrFind(str_sale_type,"Z0"))
            {
                 switch(iPayType)
                       {
                         case 71:
                               Cpn71_cnt+=iTCnt;                  //折價券71次數
                               Cpn71_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 72:
                               Cpn72_cnt+=iTCnt;                  //折價券71次數
                               Cpn72_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 73:
                               Cpn73_cnt+=iTCnt;                  //折價券71次數
                               Cpn73_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 74:
                               Cpn74_cnt+=iTCnt;                  //折價券71次數
                               Cpn74_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 75:
                               Cpn75_cnt+=iTCnt;                  //折價券71次數
                               Cpn75_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 76:
                               Cpn76_cnt+=iTCnt;;                  //折價券71次數
                               Cpn76_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 77:
                               Cpn77_cnt+=iTCnt;                  //折價券71次數
                               Cpn77_amt+=iTAmt;                  //折價券71金額
                              break;
                         case 78:
                               Cpn78_cnt+=iTCnt;                  //折價券71次數
                               Cpn78_amt+=iTAmt;                  //折價券71金額
                              break;
                         default:
                              iOtCnt+=iTCnt;;
                              iOtAmt+=iTAmt;

                       }     // END OF SWITCH
              } // end of  if (str_sale_type.Pos("Z0"))

			   else if (_StrFind(str_sale_type,"R2") || _StrFind(str_sale_type,"R3"))
            {
                 switch(iPayType)
                       {
                         case 71:
                               Cpn71_cnt-=iTCnt;                  //折價券71次數
                               Cpn71_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 72:
                               Cpn72_cnt-=iTCnt;                  //折價券71次數
                               Cpn72_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 73:
                               Cpn73_cnt-=iTCnt;                  //折價券71次數
                               Cpn73_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 74:
                               Cpn74_cnt-=iTCnt;                  //折價券71次數
                               Cpn74_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 75:
                               Cpn75_cnt-=iTCnt;                  //折價券71次數
                               Cpn75_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 76:
                               Cpn76_cnt-=iTCnt;;                  //折價券71次數
                               Cpn76_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 77:
                               Cpn77_cnt-=iTCnt;                  //折價券71次數
                               Cpn77_amt-=iTAmt;                  //折價券71金額
                              break;
                         case 78:
                               Cpn78_cnt-=iTCnt;                  //折價券71次數
                               Cpn78_amt-=iTAmt;                  //折價券71金額
                              break;
                         default:
                              iOtCnt-=iTCnt;;
                              iOtAmt-=iTAmt;

                       }     // END OF SWITCH
            }   // end of  else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)


    logsprintf("%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       Cpn71_cnt,Cpn71_amt,Cpn72_cnt,Cpn72_amt,Cpn73_cnt,Cpn73_amt,Cpn74_cnt,Cpn74_amt,
                       Cpn75_cnt,Cpn75_amt,Cpn76_cnt,Cpn76_amt,Cpn77_cnt,Cpn77_amt,Cpn78_cnt,Cpn78_amt   );


    return 0;
}



//代收、售、付 ,點卡手續費giD.作廢giE For TM3.0     3050

int __fastcall BASIC::SumKamt(int StartLine, int TotalLine)
{
     writelog("代收、售、付 集計金額 For TM3.0 ");
    //收、售、付
    giA=giB=giC=giD=giE=giF=giG=giH=0;
    gi_mix = 0;                 //混登次數    代收商品客數

    bool bl_mix = false;        //是否混登
    int i_invo_cnt = 0;         //銷售數量不為0

    int iT1, iT2, iT3,kcolx_amt,kcolf_amt, iT4, iT5, iT6, iT7;
    iT1 = iT2 = iT3 = kcolx_amt=kcolf_amt=iT4=iT5=iT6=iT7=0;

    string str_sale_type, temp, s;   //銷貨型態

    if (gbl_tot_haveline)
    {
        //for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
        {

			s = *ls; // gtsl_tot->Strings[i];
            str_sale_type=_StringSegment_EX(s, SGM, 8);

            i_invo_cnt = _StrToInt(_StringSegment_EX(s, SGM, 30));
            //str_sale_type = gtsl_tot->Strings[i].SubString(67,2);
            //kcolx_amt=_StrToInt(gtsl_tot->Strings[i].SubString(345,8)); //代收應稅
            //kcolf_amt=_StrToInt(gtsl_tot->Strings[i].SubString(354,8)); //代收免稅
             kcolx_amt=_StrToInt(_StringSegment_EX(s, SGM, 41)); //代收

			 if (_StrFind(str_sale_type,"Z0"))
            {
                //代售
                iT1 = _StrToInt(_StringSegment_EX(s, SGM, 42));
                if (iT1)
                {
                    giA += iT1;
                }

                //代付
                iT2 = _StrToInt(_StringSegment_EX(s, SGM, 43));
                if (iT2)
                {
                    giB += iT2;
                }

                //代收
                iT3 =  kcolx_amt+kcolf_amt ;
                if (iT3)
                {
                    giC += iT3;
                    bl_mix = true;      //有代收
                }

                //點卡代售手續費應稅:giD    Lu Update 2015/0819
                iT4 = iT6 = 0;
                iT4 = _StrToInt(_StringSegment_EX(s, SGM, 49));
                if (iT4)
                {
                    giD += iT4;
                    //giF++;     //點卡手續費來客數
                }
                // 點卡代售手續費免稅:giG   Lu Update 2015/0819
                iT6 = _StrToInt(_StringSegment_EX(s, SGM, 50));
                if (iT6)
                {
                    giG += iT6;
                }

                if (iT4>0 || iT6>0)
                    giF++;     //點卡手續費來客數



                //混登
                if (bl_mix)             //計算代收混登次數
                {
                    if (i_invo_cnt)     //銷售數量不為0，也有代收算混登次數
                        ++gi_mix;       //代收商品客數

                    bl_mix = false;
                }
            }
			 else if (_StrFind(str_sale_type, "R2") || _StrFind(str_sale_type,"R3"))
            {
                //代售
                iT1 = _StrToInt(_StringSegment_EX(s, SGM, 42));
                if (iT1)
                {
                    giA -= iT1;
                }

                //代付
                iT2 = _StrToInt(_StringSegment_EX(s, SGM, 43));
                if (iT2)
                {
                    giB -= iT2;
                }

                //代收
                iT3 = kcolx_amt+kcolf_amt;
                if (iT3)
                {
                    giC -= iT3;
                }

                //點卡代售手續費應稅作廢 giE   Lu Update 2015/0819
                iT5 = _StrToInt(_StringSegment_EX(s, SGM, 49));
                if (iT5)
                {
                    giE -= iT5;
                }

                 //點卡代售手續費免稅作廢 giH  Lu Update 2015/0819
                iT7 = _StrToInt(_StringSegment_EX(s, SGM, 50));
                if (iT7)
                {
                    giH -= iT7;
                }


                //☆☆ 混登不可以全退貨 ☆☆
            }
			 else if (_StrFind(str_sale_type,"R4"))
            {
                //點卡代售手續費應稅作廢 giE   Lu Update 2015/0819
                iT5 = _StrToInt(_StringSegment_EX(s, SGM, 49));
                if (iT5)
                {
                    giD += iT5;
                    giE -= iT5;
                }

                //點卡代售手續費免稅作廢 giH   Lu Update 2015/0819
                iT7 = _StrToInt(_StringSegment_EX(s, SGM, 50));
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
    logsprintf("SumKamt:1050->(代收:41,代售:42,代付:43) 代收合計金額(%d,%d), 代售合計金額(%d), 代付合計金額(%d), 點卡代售手續費應稅(%d),點卡代售手續費免稅(%d),點卡代售手續費應稅作廢(%d),點卡代售手續費免稅作廢(%d)",
                                       kcolx_amt, kcolf_amt, giA, giB, giD, giG, giE, giH);

    gi_mix=0;       //代收商品客數    2005/06/17
    return 0;
}



// 健康捐合計
int __fastcall BASIC::SumHealthAmt(int StartLine, int TotalLine)
{

    giA = giB = giC = 0;

    int iHealthCnt, iHealthAmt;
    string temp;

    iHealthCnt=iHealthAmt =0;

    string str_sale_type;   //銷貨型態

    return 0;

  //  if (gbl_tot_haveline)
  //  {
  //      //for (int i=0; i<gtsl_tot->Count; i++)
		//for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
  //      {
  //          iHealthCnt = _StrToInt(gtsl_tot->Strings[i].SubString(273,8));
  //          str_sale_type = gtsl_tot->Strings[i].SubString(67,2);
  //          if ( str_sale_type.Pos("Z0") )
  //            {
  //              if (iHealthCnt>0)
  //              {
  //                  giA ++;
  //                  giB += iHealthCnt;
  //              }

  //          }
  //          else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
  //          {
  //              if (iHealthCnt>0)
  //              {
  //                  giA --;
  //                  giB -= iHealthCnt;
  //              }

  //              //☆☆ 混登不可以全退貨 ☆☆

  //          }
  //      }
  //  }
  //  logsprintf("SumHealthamt:0050->(273,8)健康捐合計金額 (%d,%d)",giA, giB);

  //  return 0;
}


//部門帳  2005/10/01
void __fastcall BASIC::SumAllDept(DEPT *dept)
{

    string str_r_type, str_sale_type,s, slog, sUNIT;   //誤打、退貨，正常銷貨
    int iDept , iItemAmt, iDisSubAmt, iSubSubAmt,iItemQty, iNetItemAmt,
		ifoodSubSubAmt, iDisSubAmt_SPC, iSubSubAmt_SPC;

	string str_dept, str_spc, str_good_no ;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam, segDEP, segUNIT;

    iDept=iItemAmt=iDisSubAmt=iSubSubAmt=iItemQty=iNetItemAmt=ifoodSubSubAmt=0;

    logsprintf("部門帳 : gbl_rec_haveline=%d", gi_rec_line);

	
    if (gbl_rec_haveline)
    {
		for (int i = 0; i<20; i++)
		{
			dept[i].am_mmDis = 0;         // 部門促銷折扣金額         
			dept[i].am_mmDismiss = 0;     // 部門促銷折扣誤退金額     
		}

        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;

            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
                // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
                // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19, segDEP.CD_DEP=20, segUNIT:29;
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19, segDEP = 20, segUNIT=29;
                break;
            case 113:
                 //seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                 //seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                segDONGLE = 11, segNam = 18, segDEP = 41, segUNIT = 30;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            //判別那個部門
            str_dept = Trim(_StringSegment_EX(s, SGM, segDEP)); // .substr(0, 2));  //CD_DEP

            // 2020/11/03 Update by Lu
            sUNIT = Trim(_StringSegment_EX(s, SGM, segUNIT));
            if (sUNIT == "VS" || sUNIT == "vs" )
                continue;


            if ( _StrToInt(str_dept)>0)  //            >.Length() > 0)
            {
                iDept = _StrToInt(str_dept);
                --iDept;    //陣列由0開始
            }
            else
                iDept = 12; //第13部門，陣列由0開始

            if( iDept > 19 )
                iDept=12;

            
            str_sale_type = _StringSegment_EX(s, SGM, seg1);
            if (i_type == 110)
                str_spc = _StringSegment_EX(s, SGM, seg2);
            else
                str_spc = "00";

            str_r_type = _StringSegment_EX(s, SGM, seg11).substr(0, 2);


			if (_StrFind(str_sale_type, "S") && _StrFind(str_spc,"11"))    // 類MM
               { ;; }
			else if (_StrFind(str_sale_type, "S") || _StrFind(str_sale_type, "I") || _StrFind(str_sale_type,"C") || _StrFind(str_sale_type, "L"))
               { 
                //代售付,廢棄不歸部門 , L 洗選但 2020/08/31
                continue;  
                }

			if (_StrFind(str_sale_type, "A") && _StrFind(str_spc,"9"))     //預售商品
                continue;

#pragma region   農會商品 CD_FMCODE => M開頭7碼 
			str_good_no = Trim(_StringSegment_EX(s, SGM, seg3));
			if (str_good_no.substr(0, 1) == "M")  //農會商品 CD_FMCODE => M開頭7碼
				continue;
#pragma endregion


            //if ( str_sale_type.Pos("A") && str_spc.Pos("10") )     //預售兌換商品
            //    continue;

            //A0:正常銷售       B0:部門銷售   M0:Menu銷售
            //I0:廢棄    I1:廢棄取消
            //A1:正常銷售取消    B1:部門銷售取消   M1:Menu銷售取消
            //S0:特殊交易     S1:特殊交易取消

            iItemQty=_StrToInt(_StringSegment_EX(s, SGM, seg4));
            iItemAmt=_StrToInt(_StringSegment_EX(s, SGM, seg5));        //銷售金額
            iDisSubAmt=_StrToInt(_StringSegment_EX(s, SGM, seg6));      //MM  折扣 攤題
            iSubSubAmt=_StrToInt(_StringSegment_EX(s, SGM, seg7));      //小計折扣 攤題
            ifoodSubSubAmt=_StrToInt(_StringSegment_EX(s, SGM, seg8));  //食物券折扣 攤題


            /**********************   預售特別處理  **********************/
			iDisSubAmt_SPC = iDisSubAmt;     //iDisSubAmt_SPC 專用部門促銷折扣金額        Lu 2018/06/13
			iSubSubAmt_SPC = iSubSubAmt;     //iSubSubAmt_SPC 專用部門促銷折扣金額        Lu 2018/06/13
			if (_StrFind(str_sale_type, "S") && _StrFind(str_spc,"11"))    // 類MM
                {
                  iItemQty=0;
                  iDisSubAmt=0;
                  iSubSubAmt=0;
                  iItemAmt=iItemAmt*(-1);
                }

			if (_StrFind(str_sale_type, "A") && _StrFind(str_spc,"10"))     //預售兌換商品 不扣除折讓單
               {
                 //    iSubSubAmt=0;                               // 2017/07/10 要扣除折讓單 ,所以 Mark
                 iDisSubAmt=0;                                     // 2017/09/21 因已扣除 類MM, 所以無須再扣  MM 折扣 攤題 , 因類MM=MM 折扣
				 iSubSubAmt_SPC = 0;                                  //要扣除折讓單   Lu 2018/06/13   
			}
           /*************************************************************/

            iNetItemAmt=iItemAmt-iDisSubAmt-iSubSubAmt-ifoodSubSubAmt;

			if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1") || Trim(str_r_type) == "")
            {
				if (_StrFind(str_sale_type,"0"))             //正向
                {
                    dept[iDept].am_dpsitm += iItemQty;
                    dept[iDept].am_dpsale += iNetItemAmt;
					dept[iDept].am_mmDis += (iDisSubAmt_SPC + iSubSubAmt_SPC);    // 部門促銷折扣金額        Lu 2018/06/13
                }
				else if (_StrFind(str_sale_type,"1"))        //負向
                {
                    dept[iDept].am_dpsitm -= iItemQty;
                    dept[iDept].am_dpsale -= iNetItemAmt;
					dept[iDept].am_mmDis -= (iDisSubAmt_SPC + iSubSubAmt_SPC);    // 部門促銷折扣金額        Lu 2018/06/13
                }
            }
            else if (str_r_type == "R2" || str_r_type == "R3" )
            {

                //部門誤退項數、金額
				if (_StrFind(str_sale_type,"0"))             //正向
                {
                    dept[iDept].am_dpmitm += iItemQty;
                    dept[iDept].am_dpmiss += iNetItemAmt;
					dept[iDept].am_mmDismiss += (iDisSubAmt_SPC + iSubSubAmt_SPC);    // 部門促銷折扣誤退金額        Lu 2018/06/13
                }
				else if (_StrFind(str_sale_type,"1"))        //負向
                {
                    dept[iDept].am_dpmitm -= iItemQty;
                    dept[iDept].am_dpmiss -= iNetItemAmt;
					dept[iDept].am_mmDismiss -= (iDisSubAmt_SPC + iSubSubAmt_SPC);    // 部門促銷折扣誤退金額        Lu 2018/06/13
                }

            }
			else if (_StrFind(str_r_type,"R4"))            //卡紙重印
            {
				if (_StrFind(str_sale_type,"0"))           //正向
                {
                    dept[iDept].am_dpsitm += iItemQty;
                    dept[iDept].am_dpsale += iNetItemAmt;

                    dept[iDept].am_dpmitm += iItemQty;
                    dept[iDept].am_dpmiss += iNetItemAmt;
                }
				else if (_StrFind(str_sale_type,"1"))      //負向
                {
                    dept[iDept].am_dpsitm -= iItemQty;
                    dept[iDept].am_dpsale -= iNetItemAmt;

                    dept[iDept].am_dpmitm -= iItemQty;
                    dept[iDept].am_dpmiss -= iNetItemAmt;
                }
            }

        }
    }

    /*****   2005/10/01 Mark   折扣讓加入部門13 14  , 2009/04/28 追加  0011 H0  */
    /***************/
    int int_sale_price = 0;         //應稅金額
    int int_sub_price = 0;          //免稅金額

    int int_sale_times = 0;         //應稅次數
    int int_sub_times = 0;          //免稅次數
    int int_acc_amt=0;


    if (gbl_pay_haveline)      // 3311 筆數
    {
        //for (int i=0; i<gtsl_pay_sub->Count; i++)
		for (list<string>::iterator ls = gtsl_pay_sub->begin(); ls != gtsl_pay_sub->end(); ls++)
        {
			s = *ls; // gtsl_pay_sub->Strings[i];

            str_r_type = _StringSegment_EX(s, SGM, 22).substr(0,2);      //TRAN_STAT
            str_sale_type = _StringSegment_EX(s, SGM, 9);                //FG_SALETYPE

            int_sale_price = _StrToInt(_StringSegment_EX(s, SGM, 17));   //折扣/讓金額 AM_ITEM
            int_sub_price  = 0;

            //int_sale_price = _StrToInt(_StringSegment_EX(s, SGM, 18));   //應稅金額
            //int_sub_price = _StrToInt(_StringSegment_EX(s, SGM, 19));    //免稅金額

            //if (int_sale_price)
                int_sale_times = _StrToInt(_StringSegment_EX(s, SGM, 16));   //次數 QT_ITEM

            //if (int_sub_price)
            //    int_sub_times = _StrToInt(_StringSegment_EX(s, SGM", 16));    //次數

				if (!_StrFind(str_sale_type,"H"))  // H0 || H1
               continue ;

            //12:預購折價券商品貨號
            str_dept = Trim(_StringSegment_EX(s, SGM, 14).substr(0,2));   //折扣分類 NO_DEPT

            //if (str_dept.Length() > 0)
            //{
            //    iDept = _StrToInt(str_dept, "1011 部門");
            //    --iDept;    //陣列由0開始
            //}
            //else
            //    iDept = 19; //第20部門，陣列由0開始

            if( iDept > 19 )
                iDept=19;

            iDept=12; // 13部門;


			if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1") || Trim(str_r_type) == "")        //正常銷售
            {
				if (_StrFind(str_sale_type,"0"))           //正向
                {
                    dept[iDept].am_dpsitm += int_sale_times;
                    dept[iDept].am_dpsale -= int_sale_price;   //應稅金額

                    //dept[iDept].am_dpsitm += int_sub_times;
                    //dept[iDept].am_dpsale -= int_sub_price;    //免稅金額
                    int_acc_amt -= int_sub_price;
                }
				else if (_StrFind(str_sale_type,"1"))      //負向
                {
                    dept[iDept].am_dpsitm -= int_sale_times;
                    dept[iDept].am_dpsale += int_sale_price;

                    //dept[iDept].am_dpsitm -= int_sub_times;
                    //dept[iDept].am_dpsale += int_sub_price;
                    int_acc_amt += int_sub_price;
                }
            }
			else if (_StrFind(str_r_type, "R2") || _StrFind(str_r_type,"R3"))  //誤退
            {
				if (_StrFind(str_sale_type,"0"))           //正向
                {
                    dept[iDept].am_dpmitm += int_sale_times;
                    dept[iDept].am_dpmiss -= int_sale_price;

                    //dept[iDept].am_dpmitm += int_sub_times;
                    //dept[iDept].am_dpmiss -= int_sub_price;
                    int_acc_amt -= int_sub_price;
                }
				else if (_StrFind(str_sale_type,"1"))      //負向
                {
                    dept[iDept].am_dpmitm -= int_sale_times;
                    dept[iDept].am_dpmiss += int_sale_price;

                    //dept[iDept].am_dpmitm -= int_sub_times;
                    //dept[iDept].am_dpmiss += int_sub_price;
                    int_acc_amt += int_sub_price;
                }
            }


			else if (_StrFind(str_r_type,"R4"))          //發票重印
            {
				if (_StrFind(str_sale_type,"0"))           //正向
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
				else if (_StrFind(str_sale_type,"1"))      //負向
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

        logsprintf("部門帳 H0,13部門 預購折價券 (金額:%d) .",int_acc_amt);

    }

   /***************/
     
    //20040112門市提貨券 屬13部門
    dept[12].am_dpsitm += pay_bill_cnt;    //正向
    dept[12].am_dpsale -= pay_bill_amt;
    dept[12].am_dpmitm += mpay_bill_cnt;   //負向
    dept[12].am_dpmiss += mpay_bill_amt;


   logsprintf("部門帳 END,13部門 門市提貨券(數量:%d, 金額:%d) (數量:%d, 金額:%d), 注:不再處理 1011 資料.", pay_bill_cnt, pay_bill_amt, mpay_bill_cnt, mpay_bill_amt);


}


// 即時連線商品集計
void __fastcall BASIC::INQSubTotal(INQTBL *inqtbl)
{

    string str_r_type, str_sale_type,s;   //誤打、退貨，正常銷貨
    int iDept = 0;                          //即時連線區分

    string str_dept;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
            //判別那個即時連線區分
			s = *ls; // gtsl_rec->Strings[i];
            //Trim(gtsl_rec->Strings[i].SubString(334,4)); //即時連線區分 ,箱數量.
            str_dept = Trim(_StringSegment_EX(s, SGM, 36).substr(0,2)); //即時連線區分 ,箱數量.
            if (str_dept.length() > 0)
            {
                iDept = _StrToInt(str_dept);
                //--iDept;    //陣列由0開始
            }
            else
                iDept = 19; //第19即時連線區分，陣列由0開始

            if ( iDept == 0)     //非即時連線商品不計算 
                continue;    

            str_r_type = _StringSegment_EX(s, SGM, 38).substr(0,2);
            str_sale_type = _StringSegment_EX(s, SGM, 8);

            // if (str_sale_type.Pos("S") || str_sale_type.Pos("I"))     //代售付,廢棄不歸部門
            //    continue;
			if (_StrFind(str_sale_type,"I"))     //廢棄不計算 
                continue;    

            //A0:正常銷售       B0:部門銷售   M0:Menu銷售
            //I0:廢棄    I1:廢棄取消
            //A1:正常銷售取消    B1:部門銷售取消   M1:Menu銷售取消
            //S0:特殊交易     S1:特殊交易取消

			if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1"))
            {
				if (_StrFind(str_sale_type,"0"))             //正向
                {
                    inqtbl[iDept].qt_InqSale += _StrToInt(_StringSegment_EX(s, SGM, 26));
                    inqtbl[iDept].am_InqSale += _StrToInt(_StringSegment_EX(s, SGM, 29));
                }
				else if (_StrFind(str_sale_type,"1"))        //負向
                {
                    inqtbl[iDept].qt_InqSale -= _StrToInt(_StringSegment_EX(s, SGM, 26));
                    inqtbl[iDept].am_InqSale -= _StrToInt(_StringSegment_EX(s, SGM, 29));
                }
            }
            else if (str_r_type == "R2" || str_r_type == "R3" )
            {

                //即時連線區分誤退數量、金額
				if (_StrFind(str_sale_type,"0"))             //負向
                {
                    inqtbl[iDept].qt_InqSale -= _StrToInt(_StringSegment_EX(s, SGM, 26));
                    inqtbl[iDept].am_InqSale -= _StrToInt(_StringSegment_EX(s, SGM, 29));
                }
				else if (_StrFind(str_sale_type,"1"))       //正向 
                {
                    inqtbl[iDept].qt_InqSale += _StrToInt(_StringSegment_EX(s, SGM, 26));
                    inqtbl[iDept].am_InqSale += _StrToInt(_StringSegment_EX(s, SGM, 29));
                }

            }
			else if (_StrFind(str_r_type,"R4"))
            {
				if (_StrFind(str_sale_type,"0"))           //正向
                {
                    ;;
                }
				else if (_StrFind(str_sale_type,"1"))      //負向
                {
                    ;;
                }
            }

        }  // end of for
    }  // end of if (gbl_rec_haveline)
}



void __fastcall BASIC::init_data()
{

    int int_ten_code, int_ecr_no, int_cashier,i,j;
    string str_x_trans,OrgStoreNo;
	   
	string stmp = string(WORKDIR) + "XZDATA";
	
	//CA2W ca2w(stmp.c_str());
	//wstring wstmp = ca2w;  
	//CW2A cw2a(wstmp.c_str());
	
	wstring wstmp = StringToWString(stmp);
	//建立目錄
	//if (!PathFileExists((LPCWSTR)stmp.c_str()))         
	if (!DirExists(stmp))
		SHCreateDirectory(NULL, wstmp.c_str());

	try
	{ 
	//m_IniReader.WriteString(m_strKeyValue, m_strKeyName, m_strSectionName);
	m_IniReader.setINIFileName(XZDATA_INI);
	int_ten_code = stoi(m_IniReader.ReadString("ECR", "StoreNO", "009999"));
	int_ecr_no = stoi(m_IniReader.ReadString("ECR", "EcrNO", "01"));
	int_cashier = stoi(m_IniReader.ReadString("ECR", "SalesNo", "00000"));

	str_x_trans = m_IniReader.ReadString("X", "ptran_cnt", "00000000");
	OrgStoreNo = m_IniReader.ReadString("ECR", "OrgStoreNO", "009999"); //原始店號

	//sprintf_s(gchar_tencode, sizeof(gchar_tencode), "%06d", int_ten_code);
    _Strsprintf(g_tencode, "%06d", int_ten_code);
	//sprintf_s(gchar_ecr_no, sizeof(gchar_ecr_no), "%1d", int_ecr_no);
    _Strsprintf(g_ecr_no, "%02d", int_ecr_no);
	//sprintf_s(gchar_cashier, sizeof(gchar_cashier), "%05d", int_cashier);
    _Strsprintf(g_cashier, "%05d", int_cashier);

	//sprintf_s(gchar_orgtencode, sizeof(gchar_orgtencode), "%s", OrgStoreNo.c_str());      //原始店號
    g_orgtencode = OrgStoreNo;   //原始店號

	gi_open_x = stoi(m_IniReader.ReadString("ECR", "tot_open_x", "0"));  //tif_ecr->ReadInteger("ECR", "tot_open_x", 0);
	gi_open_z = stoi(m_IniReader.ReadString("ECR", "tot_open_z", "0"));  //tif_ecr->ReadInteger("ECR", "tot_open_z", 0);
    
    g_date_time = currentDateTime(1);
	//sprintf_s(gchar_date_time, sizeof(gchar_date_time), "%14s", currentDateTime(1).c_str());

	if (str_x_trans.length() > 5)
		  stmp=str_x_trans.substr(str_x_trans.length()-5,5);
    else
		  stmp=str_x_trans;
    gx_trans = stmp;
	//sprintf_s(gchar_x_trans, sizeof(gchar_x_trans), "%5.5s", stmp.c_str());
		
	int int_epson = stoi(m_IniReader.ReadString("PRINT", "EPSON", "1"));   //tif_ecr->ReadString("PRINT", "EPSON", "").Length();

	for( i=0 ; i< 20; i++)     //INIT 代售付ｔａｂｌｅ 15-17 代售, 18-19 代付
	          {
	            BaseTbSubrev[i].iqt5_totrev=0;
	            BaseTbSubrev[i].iam5_totrev=0;
	            for( j=0 ; j<10; j++)
	               {
	                 BaseTbSubrev[i].it8_subrev[j].iqt_subrev=0;
	                 BaseTbSubrev[i].it8_subrev[j].iam_subrev=0;
	               }
	          }
	}
	catch (...)
	{
		;;
	}

    g_store_name = ""; // GetStoreName();

	//m_IniReader.getSectionNames(); // getSectionData("ECR");

	writelog("★ BASIC::init_data() OK ★ ");

}


//3010 
void __fastcall BASIC::sub_times(int StartLine, int TotalLine)
{
    //代收、售、付次數統計

    string str_is_rtype, str_trans_flag,Stmp, s;        //銷售形態，交易形態，商品代號 str_fmcode,
    int i_trans_num = 0;                            //銷售數量

    //0:一般商品    1:代收   2:代售   3:代付、食安   4:加值機(開,售)卡   5:加值機(加值)

    int i_trans_flag = 0;                          //交易型態
    int iRelType;

    //收、售、付
    giA=giB=giD=giE=giF=giG=0;
    //giH giI 食安退貨金額、次數
    giH=giI=0;

    string str_good_no, str_menu_no, str_bar_name, str_item_amt;
    //TStringList *tsl_subsale = new TStringList();
    string str_type, str_item_qty, strRelType;        //更正
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;

            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
               // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
               // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:14, seg4.銷售數量:35, seg5.AM_ITEM:38, 
                // seg6.AM_DIS_SUB:39, seg7.AM_SUB_SUB:40, seg8.AM_PAY_SUB:41, seg9.FG_TAX:44  seg10.連線商品狀態:45, 
                // seg11.交易狀態區分:50 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 14, seg4 = 35, seg5 = 38, seg6 = 39, seg7 = 40, seg8 = 41;
                seg9 = 44, seg10 = 45, seg11 = 50;
                break;
            default:
                continue;

            }

            str_type = _StringSegment_EX(s, SGM, seg1);                  //銷貨型態
            str_trans_flag = _StringSegment_EX(s, SGM, seg2);           //代售付區分 SPC_FLG  交易類別
            //str_fmcode = _StringSegment_EX(s, SGM, 17);               //商品代號
            str_good_no = _StringSegment_EX(s, SGM, seg3);

            str_menu_no = "0000";
            str_bar_name = _StringSegment_EX(s, SGM, segNam);          //NM_PLU
            
            
            str_item_amt = _StringSegment_EX(s, SGM, seg5);            //AM_ITEM 


            i_trans_num = _StrToInt(_StringSegment_EX(s, SGM, seg4));  //銷售數量
            str_item_qty = int2str(i_trans_num); //_IntToStr(i_trans_num);

            //連線商品區分
            strRelType = _StringSegment_EX(s, SGM, seg10).substr(0, 2);   // 連線商品狀態
            iRelType = _StrToInt(strRelType);

            str_is_rtype = _StringSegment_EX(s, SGM, seg11).substr(0,2);   // 交易狀態區分
 
            if(iRelType == 11 && _StrToInt(str_item_amt)==0) //租賃 退押金 2012/03/26 Update
               i_trans_num=0;

            // 2006/12/20
            if ( Trim(str_trans_flag)=="" || iRelType == 7 || iRelType == 9)      //無型態 ;連線商品區分:代售票券 07, 09
               {
                //logsprintf("sub_times:不處理代代售合計, str_good_no=(%d),連線商品區分=(%s)", str_good_no.c_str(), strRelType.c_str());
                continue;
               }
           // else
          //     {
          //       Stmp.sprintf("sub_times:代售合計, str_good_no=(%d), 連線商品區分=(%s)",
          //                              str_good_no.c_str(), strRelType.c_str());
          //       writelog(Stmp);
          //     }
			//!= std::string::npos
            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  交易類別
			
			if (_StrFind(str_is_rtype, "FF") || _StrFind(str_is_rtype, "Z0") || _StrFind(str_is_rtype, "Z1") || Trim(str_is_rtype) == "")
            {
				if (_StrFind(str_type, "S0") )  
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //代收
                            if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //金額
                                   giB+=i_trans_num;               //次數
                                }  // end of   if(iRelType =

                            giD += i_trans_num;
                        break;
                        case 2:     //代售   2006/12/20
                              //2013/11/25  S0  VDC06                             $80 ==>條件為有代售付編號和有連線區分 => 即時購代售合計
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
                        case 3:     //代付、食安

                            if (str_good_no == "9390027")        //食安FMCODE
                                {
                                giI += i_trans_num;
                                giH += _StrToInt(str_item_amt);   //金額

                                }
                            else
                                {
                                str_menu_no = "2001";
                                giF += i_trans_num;
                                giG += _StrToInt(str_item_amt);   //金額
                                Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,1 );
                                }
                        break;
                        case 0:     //退押金
                              if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //金額
                                   giB+=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
				else if (_StrFind(str_type,"S1"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //代收
                             if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //金額
                                   giB-=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                            giD -= i_trans_num;
                        break;
                        case 2:     //代售  2006/12/20
                              //2013/11/25  S0  VDC06                             $80 ==>條件為有代售付編號和有連線區分 => 即時購代售合計
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
                        case 3:     //代付
                            if (str_good_no == "9390027")        //食安FMCODE
                                {
                                giI -= i_trans_num;
                                giH -= _StrToInt(str_item_amt);   //金額

                                }
                            else
                                {
                                str_menu_no = "2001";
                                giF -= i_trans_num;
                                giG -= _StrToInt(str_item_amt);   //金額
                                Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,-1 );
                                }
                        break;
                        case 0:     //退押金
                              if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //金額
                                   giB-=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
            }
			else if (_StrFind(str_is_rtype, "R2") || _StrFind(str_is_rtype,"R3") )
            {
				if (_StrFind(str_type,"S0") )
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //代收
                            if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //金額
                                   giB-=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                            giD -= i_trans_num;
                        break;
                        case 2:     //代售 2006/12/20
                            //2013/11/25  S0  VDC06                             $80 ==>條件為有代售付編號和有連線區分 => 即時購代售合計
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
                        case 3:     //代付
                            if (str_good_no == "9390027")        //食安FMCODE
                                {
                                giI -= i_trans_num;
                                giH -= _StrToInt(str_item_amt);   //金額

                                }
                            else
                                {
                                str_menu_no = "2001";
                                giF -= i_trans_num;
                                giG -= _StrToInt(str_item_amt);   //金額
                                Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,-1 );
                                }
                        break;
                        case 0:     //退押金
                              if(iRelType == 11)
                                {
                                   giA-=_StrToInt(str_item_amt);   //金額
                                   giB-=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
				else if (_StrFind(str_type,"S1"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //代收
                            if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //金額
                                   giB+=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                            giD += i_trans_num;
                        break;
                        case 2:     //代售  2006/12/20
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
                        case 3:     //代付
                              if (str_good_no == "9390027")        //食安FMCODE
                                {
                                giI += i_trans_num;
                                giH += _StrToInt(str_item_amt);   //金額

                                }
                              else
                              {
                               str_menu_no = "2001";
                               giF += i_trans_num;
                               giG += _StrToInt(str_item_amt);   //金額
                               Sum_BaseTbSubrev(str_menu_no, str_item_amt, str_item_qty,1 );
                               }
                        break;
                        case 0:     //退押金
                              if(iRelType == 11)
                                {
                                   giA+=_StrToInt(str_item_amt);   //金額
                                   giB+=i_trans_num;               //次數
                                }  // end of   if(iRelType == 11)
                        break;
                    }
                }
            }
        }
    }

    //DeleteFile("e:\\subsal.txt");
    //tsl_subsale->   SaveToFile("e:\\subsal.txt");  _ListToFile(tslCrp, SCasherRpt.c_str());
    //delete tsl_subsale;
    logsprintf("sub_times: 租賃 退押金次數(%d), 退押金(%d), 代付次數(%d), 代付(%d), 代收次數(%d)", giB, giA, giF, giG, giD );


}


void __fastcall BASIC::Sum_BaseTbSubrev(string menu_no, string item_amt,  string item_qty,int NegTive )
{
  int MemuNo, MenuItem, iamt, iqty;
  string Stmp;

  try
    {
     Stmp=menu_no.substr(0,2);
	 MemuNo = _StrToInt(Stmp);
     Stmp=menu_no.substr(2,2);
	 MenuItem = _StrToInt(Stmp);
	 iamt = _StrToInt(item_amt);  
	 iqty = _StrToInt(item_qty);  

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
     logsprintf("Sum_BaseTbSubrev: Error menu_no=(%s), item_amt=(%s), item_qty=(%s)",
                   menu_no.c_str(), item_amt.c_str(), item_qty.c_str());
    }

}




////發票結束號碼  3050**
/// <summary>  BASIC::invoice_start_end
///   取得發票號碼 開始:invoice_start=true, /結束:invoice_start=false  
/// </summary>
/// <param name="tot_line_coun"></param>
/// <param name="invoice_star"></param>
/// <returns>string</returns>
string __fastcall BASIC::invoice_start_end(int tot_line_count, bool invoice_start)
{
    string str_invoice_no;      //發票號碼
    string str_invoice_char;    //字軌
    string str_type,sTmp,s;            //銷貨型態

    bool bl_normal_type = false;    //無發票號碼

	if (gtsl_tot->size() <= 0)
		return "0000000000";

    try
      {
       if (invoice_start)
        {
          //for (int i=0; i<gtsl_tot->Count; i++)
		for (list<string>::iterator ls = gtsl_tot->begin(); ls != gtsl_tot->end(); ls++)
           {
		    s = *ls; // gtsl_tot->Strings[i];
            str_type = _StringSegment_EX(s, SGM, 8);

            sTmp=_StringSegment_EX(s, SGM, 9);      //發票起號
            str_invoice_char =  sTmp.substr(0,2);
            str_invoice_no = sTmp.substr(2,8);

            if (_StrToInt(str_invoice_no) == 0)
                continue;

			if (_StrFind(str_type, "Z0") || _StrFind(str_type, "R4") || _StrFind(str_type,"V"))
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
        //for (int i=gtsl_tot->Count-1; i>=0; i--)
		//for (list<string>::iterator ls = gtsl_tot->end(); ls != gtsl_tot->begin(); --ls)
		list<string>::iterator ls = gtsl_tot->end();
		while ( 1 )
         {
			 --ls;
		    s = *ls; // gtsl_tot->Strings[i];
            str_type = str_type = _StringSegment_EX(s, SGM, 8); //gtsl_tot->Strings[i].SubString(67,2);

            sTmp=_StringSegment_EX(s, SGM, 10);      //發票迄號
            str_invoice_char =  sTmp.substr(0,2);
            str_invoice_no = sTmp.substr(2,8);

			if (_StrToInt(str_invoice_no) == 0)
			{
				if (ls == gtsl_tot->begin())
					break;
				else
				    continue;  // 發票號碼為0時，不儲存，開始下個迴圈 
			}

			if (_StrFind(str_type, "Z0") || _StrFind(str_type, "R4") || _StrFind(str_type,"V"))
            {
				if (str_invoice_no == "00000000")
				{
					//str_invoice_char = "00";
					continue;
				}

                str_invoice_no = (str_invoice_char + str_invoice_no);
                bl_normal_type = true;
                break;
            }

			if (ls == gtsl_tot->begin())
				break;

          }
       }
    } // end of try
    catch(...)
    {
       logsprintf("invoice_start_end: catch Error, tot_line_count=%d", tot_line_count);
       //writelog(sTmp);
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

    //gchar_tencode[0] = 0;
    //gchar_ecr_no[0] = 0;
    //gchar_cashier[0] = 0;
    //gchar_date_time[0] = 0;
    //gchar_x_trans[0] = 0;
    //memset(gpchar_x_first, 0x00, MAXDATALEN); // gpchar_x_first[0] = 0;
    //memset(gpchar_sal_last, 0x00, MAXDATALEN);       //gpchar_sal_last[0] = 0;
    //memset(gpchar_sal_first, 0x00, MAXDATALEN);     //gpchar_sal_first[0] = 0;

    gsalX_first="";
    gsal_last="";
    gsal_first="";
    gsLast1099="";      //    SAL第一筆 1099

    pay_bill_cnt = 0;     //門市提貨券次數 20031223
    pay_bill_amt = 0;     //門市提貨券金額
    mpay_bill_cnt = 0;     //門市提貨券次數 20031223
    mpay_bill_amt = 0;     //門市提貨券金額

    Cpn71_cnt=0;                  //折價券71次數
    Cpn71_amt=0;                  //折價券71金額
    Cpn72_cnt=0;                  //折價券72次數
    Cpn72_amt=0;                  //折價券72金額
    Cpn73_cnt=0;                  //折價券73次數
    Cpn73_amt=0;                  //折價券73金額
    Cpn74_cnt=0;                  //折價券74次數
    Cpn74_amt=0;                  //折價券74金額
    Cpn75_cnt=0;                  //折價券75次數
    Cpn75_amt=0;                  //折價券75金額
    Cpn76_cnt=0;                  //折價券76次數
    Cpn76_amt=0;                  //折價券76金額

    Cpn77_cnt=0;                  //門市提貨券77次數
    Cpn77_amt=0;                  //門市提貨券77金額
    Cpn78_cnt=0;                  //門市提貨券78次數
    Cpn78_amt=0;                  //門市提貨券78金額
       
		//->
	char  buffer[10240];
	sprintf_s(buffer, sizeof(buffer), "%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		Cpn71_cnt, Cpn71_amt, Cpn72_cnt, Cpn72_amt, Cpn73_cnt, Cpn73_amt, Cpn74_cnt, Cpn74_amt,
		Cpn75_cnt, Cpn75_amt, Cpn76_cnt, Cpn76_amt, Cpn77_cnt, Cpn77_amt, Cpn78_cnt, Cpn78_amt);

	StrBillPay = string(buffer);

    /*StrBillPay.sprintf("%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       Cpn71_cnt,Cpn71_amt,Cpn72_cnt,Cpn72_amt,Cpn73_cnt,Cpn73_amt,Cpn74_cnt,Cpn74_amt,
                       Cpn75_cnt,Cpn75_amt,Cpn76_cnt,Cpn76_amt,Cpn77_cnt,Cpn77_amt,Cpn78_cnt,Cpn78_amt   );*/
}


//小計折扣    1011  3311
void __fastcall BASIC::SumDisc(int StartLine, int TotalLine)
{
    string str_tot_type,s;
    string str_pay_type;
    int iQty, iAmt;
    giA = giB = 0;

    if (gbl_pay_haveline)
    {
       // for (int i=0; i<gtsl_pay_sub->Count; i++)
    	for (list<string>::iterator ls = gtsl_pay_sub->begin(); ls != gtsl_pay_sub->end(); ls++)
        {
			s = *ls; // gtsl_pay_sub->Strings[i];
            str_pay_type = _StringSegment_EX(s, SGM, 9);
            
            iQty = _StrToInt(_StringSegment_EX(s, SGM, 16));      //次數
            iAmt = _StrToInt(_StringSegment_EX(s, SGM, 17));      //小計折扣/折扣/讓金額

            str_tot_type = _StringSegment_EX(s, SGM, 22).substr(0,2);

            if (_StrFind(str_pay_type,"F"))  //F為折扣讓
                continue;

			if (_StrFind(str_tot_type, "FF") || _StrFind(str_tot_type, "Z0") || _StrFind(str_tot_type,"Z1") || Trim(str_tot_type) == "")
            {
				if (_StrFind(str_pay_type,"E0"))
                {
                    giA += iQty;      //次數
                    giB += iAmt;      //小計折扣/折扣/讓金額
                }
				else if (_StrFind(str_pay_type,"E1"))
                {
                    giA -= iQty;      //次數
                    giB -= iAmt;      //小計折扣/折扣/讓金額
                }
            }
			else if (_StrFind(str_tot_type, "R2") || _StrFind(str_tot_type,"R3"))
            {
				if (_StrFind(str_pay_type,"E0"))
                {
                    giA -= iQty;     //次數
                    giB -= iAmt;     //小計折扣/折扣/讓金額
                }
				else if (_StrFind(str_pay_type,"E1"))
                {
                    giA += iQty;    //次數
                    giB += iAmt;    //小計折扣/折扣/讓金額
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

//加值機  
 //TM一卡通加值(支付)明細電文檔(3432)
 //TM遠鑫加值(支付)明細電文檔 (3234)
 //TM二代悠遊卡加值 二代悠遊加值明細 3238
int __fastcall BASIC::easy_card(int StartLine, int TotalLine)
{
    string str_type, str_sale_type, str_r_type, StrTranType,s;        //加值交易類別
    int iTmpAmt,int_type = 0;

    int int_0_time  = 0, int_0_money  = 0;
    int int_2_time  = 0, int_2_money  = 0;
    int int_7_time  = 0, int_7_money  = 0;
    int int_9_time  = 0, int_9_money  = 0;
    int int_11_time = 0, int_11_money = 0;

//  開售卡    //加值      //結帳      //取消
    giA = giB = giC = giD = giE = giF = giG = giH = 0;
    iTmpAmt=0;
    //return 0;
    if (gbl_easy_haveline)
    {
       // for (int i=0; i< gtsl_easy_card->Count; i++)
		for (list<string>::iterator ls = gtsl_easy_card->begin(); ls != gtsl_easy_card->end(); ls++)
        {
			s = *ls; // gtsl_easy_card->Strings[i];
            
			StrTranType = _StringSegment_EX(s, SGM, 1);    //gtsl_easy_card->Strings[i].SubString(1,4);
            
            if( StrTranType == "1031")   
               {
                ;;
               } // end of if( StrTranType == "0031")
			else if (StrTranType == "3432" &&  Trim(_StringSegment_EX(s, SGM, 9)) == "K2")  //New Dongle 高捷
               {
#pragma region  New Dongle 高捷 
				 str_type = Trim(_StringSegment_EX(s, SGM, 10));
                 str_r_type = _StringSegment_EX(s, SGM, 28).substr(0,2);
                 str_sale_type=str_r_type;
                 //int_type = _StrToInt(str_type);
                 //iTmpAmt=
				 if (_StrFind(str_sale_type, "X") || _StrFind(str_sale_type,"V"))         //LOG錯誤不處理
                    continue;

                 //if (str_type != "00" && int_type == 0)  //錯誤交易類別
                 //   continue;

				 if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1") || Trim(str_r_type) == "")
		          {
                     if(str_type=="05" ) // 加值
                       {
                          ++int_2_time;
                          int_2_money += _StrToInt(_StringSegment_EX(s, SGM, 18));
                       }
                     else if(str_type=="06")     //加值取消
                       {
                          ++int_11_time;
                           int_11_money += _StrToInt(_StringSegment_EX(s, SGM, 18));
                       }
                     else
                       {
                         //++int_2_time;
	                //int_2_money += _StrToInt(_StringSegment_EX(s, "|", 18));
                         ;;
                       }

	              }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
				 else if (_StrFind(str_r_type, "R2") || _StrFind(str_r_type,"R3"))
	              {
                     if(str_type=="05") // 加值
                       {
                          --int_2_time;
                          int_2_money -= _StrToInt(_StringSegment_EX(s, SGM, 18));
                       }
                     else  if(str_type=="06")     //加值取消
                       {
                          --int_11_time;
                           int_11_money -= _StrToInt(_StringSegment_EX(s, SGM, 18));
                       }
                     else
                       {
                       //  --int_2_time;
	               // int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 18));
                         ;;
                       }

	             }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
#pragma endregion
               } // end of if( StrTranType == "0032")
			else if (StrTranType == "3234" && Trim(_StringSegment_EX(s, SGM, 10)) == "K3")  //New Dongle 八合一 for HappyCash(遠鑫)
               {
#pragma region  New Dongle 八合一 for HappyCash(遠鑫) 
				 str_type = Trim(_StringSegment_EX(s, SGM, 13));           // Processing_Code
                 str_r_type = _StringSegment_EX(s, SGM, 31).substr(0,2);
                 str_sale_type=str_r_type;
                 //int_type = _StrToInt(str_type);
                 //iTmpAmt=
				 if (_StrFind(str_sale_type, "X") || _StrFind(str_sale_type,"V"))         //LOG錯誤不處理
                    continue;

                 //if (str_type != "00" && int_type == 0)  //錯誤交易類別
                 //   continue;

				 if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1") || Trim(str_r_type) == "")
		            {
                     if(str_type=="7707" ) // 加值
                       {
                          ++int_2_time;
                          int_2_money += _StrToInt(_StringSegment_EX(s, SGM, 23).substr(0,10));
                       }
                     else if(str_type=="7708")     //加值取消
                       {
                          ++int_11_time;
                           int_11_money += _StrToInt(_StringSegment_EX(s, SGM, 23).substr(0,10));
                       }
                     else
                       {
                         //++int_2_time;
	                //int_2_money += _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                         ;;
                       }

	                }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
				 else if (_StrFind(str_r_type, "R2") || _StrFind(str_r_type,"R3"))
	                {
                     if(str_type=="7707") // 加值
                       {
                          --int_2_time;
                          int_2_money -= _StrToInt(_StringSegment_EX(s, SGM, 23).substr(0,10));
                       }
                     else  if(str_type=="7708")     //加值取消
                       {
                          --int_11_time;
                           int_11_money -= _StrToInt(_StringSegment_EX(s, SGM, 23).substr(0,10));
                       }
                     else
                       {
                       //  --int_2_time;
	               // int_2_money -= _StrToInt(_StringSegment_EX(s, "|", 23).SubString(1,10));
                         ;;
                       }
	               }   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
#pragma endregion
             } // end of if( StrTranType == "1034")
         else if( StrTranType == "3238")
              {
#pragma region  New Dongle 八合一 for (悠遊卡) 
                 str_sale_type = _StringSegment_EX(s, SGM, 9); //FG_SALETYPE
                 // '811799'(現金加值) '810899'(加值取消) 'X10799'(加值異常) 'X10899'(加值取消異常) '850799'(開售卡) '850899'(開售卡取消) 'X50799'(開售卡異常)
                 str_type = _StringSegment_EX(s, SGM, 18).substr(1,3);  // ID_MESSAGE    悠遊訊息代號
                 str_r_type = _StringSegment_EX(s, SGM, 20).substr(0,2);  //gtsl_easy_card->Strings[i].SubString(225,2);
                 int_type = _StrToInt(str_type);
                
				 if (_StrFind(str_sale_type, "X") || _StrFind(str_sale_type, "Z") || _StrFind(str_sale_type,"V"))         //LOG錯誤不處理
                    continue;
 
				 if (_StrFind(str_r_type,"FF") || _StrFind(str_r_type,"Z0") || _StrFind(str_r_type,"Z1") || Trim(str_r_type) == "")
		          {
	              switch (int_type)
	              {
	                //結帳 = 開卡 + 加值
	                case 0:
	                    ++int_0_time;
	                    break;
	                //加值
                        case 417:     //841799
	                case 117:     //811799
	                    ++int_2_time;
	                    int_2_money += _StrToInt(_StringSegment_EX(s, SGM, 14));
	                    break;
	                //開售卡
	                case 517:
	                    ++int_7_time;
                        int_7_money +=  _StrToInt(_StringSegment_EX(s, SGM, 14));
	                    break;
	                //開卡取消
	                case 518:
	                    ++int_9_time;
                        int_9_money +=  _StrToInt(_StringSegment_EX(s, SGM, 14));
	                    break;

	                //加值取消
                        //case 408:
	                case 118:          //811899
	                    ++int_11_time;
	                    int_11_money += _StrToInt(_StringSegment_EX(s, SGM, 14));
                            break;
                        // 824799  信用加值 AutoLoad  837799 847799  825799
                        // 學生卡展期加值:816799    學生卡展期:813399 // 展期:811399
                        case 247:
                        case 257:
                        case 167:
                        case 133:
                        case 113:
                             break;

                        // 小額扣款
                        //case 105:
                        //     break;

                        // 退貨
                        //case 509:
                        //     break;

                        default:
                            ++int_2_time;
	                    int_2_money += _StrToInt(_StringSegment_EX(s, SGM, 14));
	                    break;
	                  }
	                }   // end if (str_r_type.Pos("FF") || str_r_type.Pos("Z0") || str_r_type.Pos("Z1"))
				 else if (_StrFind(str_r_type, "R2") || _StrFind(str_r_type,"R3"))
	                {
						switch (int_type)
							{
							//結帳
							case 0:
								--int_0_time;
			                    break;
			                //加值
	                        case 417:
			                case 117:
			                    --int_2_time;
			                    int_2_money -= _StrToInt(_StringSegment_EX(s, SGM, 14));
			                    break;
			                //開售卡
			                case 517:
			                    --int_7_time;
	                            int_7_money -=  _StrToInt(_StringSegment_EX(s, SGM, 14));
			                    break;
			                //開卡取消
			                case 518:
			                    --int_9_time;
	                            int_9_money -=  _StrToInt(_StringSegment_EX(s, SGM, 14));
			                    break;
			                //加值取消
			                case 118:
			                    --int_11_time;
			                    int_11_money -= _StrToInt(_StringSegment_EX(s, SGM, 14));
	                            break;
	                        case 509:
	                             break;
	                            
		                       // 824799  信用加值 AutoLoad
		                       // 學生卡展期加值:816799    學生卡展期:813399 // 展期:811399
		                    case 247:
							case 257:
							case 167:
							case 133:
							case 113:
								break;

							default:
								--int_2_time;
								int_2_money -= _StrToInt(_StringSegment_EX(s, SGM, 14));
								break;
					}
				}   // end else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))
#pragma endregion
			  }// end of   else if( StrTranType == "1038")
            else // ( StrTranType == "1035") OR 1038
              {
                 ;;
              }// end of   if( StrTranType == "1035") else
        }
    }

    //開售卡
    giA = (int_7_time  - int_9_time);
    giB = (int_7_money - int_9_money);

    //加值
    giC = (int_2_time  - int_11_time);
    giD = (int_2_money - int_11_money);

    //結帳
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

    //取消
    giG = (int_9_time + int_11_time);   //2005/07/01
    giH = 0 - (int_9_money + int_11_money);  //2005/07/01

    //取消
    giG = 0;     //2005/07/01
    giH = 0;     //2005/07/01
    //結帳
    giE = 0;         //2005/07/01 liu
    giF = 0;         //2005/07/01 liu

    return 0;
}


//FETC 加值機   2005/07/01
int __fastcall BASIC::fetc_card(int StartLine, int TotalLine)
{
    string str_type, str_sale_type, str_r_type,sCardNo,sTmp,s;        //Fetc 加值交易類別
    int iTmpAmt,int_type = 0;

    logsprintf("FETC 加值機 fetc_card:(%d,%d)",StartLine,TotalLine);
    //writelog(sTmp);

    int int_0_time  = 0, int_0_money  = 0;
    int int_2_time  = 0, int_2_money  = 0;
    int int_7_time  = 0, int_7_money  = 0;
    int int_9_time  = 0, int_9_money  = 0;
    int int_1_time = 0,  int_1_money = 0;

    //加值      //結帳      //取消
    giFetcCashAddCnt=0;
    giFetcCashAddamt=0;
    giFetcCredAddCnt=0;
    giFetcCredAddamt=0;
    giFetcSumCnt=0;
    giFetcSumAmt=0;
    iTmpAmt=0;
    //_StrToInt(_StringSegment_EX(s, "|", 17));
    if (gbl_fetc_haveline)
    {
        //for (int i=0; i<gtsl_fetc_card->Count; i++)
		for (list<string>::iterator ls = gtsl_fetc_card->begin(); ls != gtsl_fetc_card->end(); ls++)
        {
			s = *ls; // gtsl_fetc_card->Strings[i];

            str_sale_type = _StringSegment_EX(s, "|", 9); //gtsl_fetc_card->Strings[i].SubString(57,2);
            str_type = _StringSegment_EX(s, "|",10);      //gtsl_fetc_card->Strings[i].SubString(69,4);    //刷卡機交易類別 交易代碼
            sCardNo = _StringSegment_EX(s, "|", 19); //gtsl_fetc_card->Strings[i].SubString(164,30);   //卡號
            str_r_type = _StringSegment_EX(s, "|", 31).substr(0,2);    //交易狀態區分
            iTmpAmt=_StrToInt(_StringSegment_EX(s, "|", 22));
            // int_type = _StrToInt(str_type);
           // sTmp.sprintf("fetc_card:str_sale_type=%s, str_type=%s, sCardNo=%s, str_r_type=%s ",
           //               str_sale_type,str_type, sCardNo, str_r_type);
           // writelog(sTmp);

            if (_StrFind(str_sale_type,"X"))         //LOG錯誤不處理
                continue;

            if (str_type == "0000" )  //交易代碼
                continue;

            if( str_type == "A090" )   //結帳
              {
               ++int_0_time;
	      // int_0_money += _StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
               continue;
              }

            if (str_type == "N092" )  //交易代碼
               {
                logsprintf("fetc_card:str_sale_type=%s, str_type=%s, sCardNo=%s, str_r_type=%s 異常不處理",
					str_sale_type.c_str(), str_type.c_str(), sCardNo.c_str(), str_r_type.c_str());
                //writelog(sTmp);
                continue;
               }

			if (_StrFind(str_r_type, "FF") || _StrFind(str_r_type, "Z0") || _StrFind(str_r_type,"Z1") || Trim(str_r_type) == "")
	       {
                 // 2012/08/29 Mts:7787
                 //iTmpAmt=_StrToInt(_StringSegment_EX(s, "|", 22));

                 if(sCardNo.substr(1,4) == "0999") //現金卡
                    {
                     // if( str_type == "A140"   )   //現金 加值
                      if( str_sale_type == "S0"   )   //現金 加值
                        {
                          ++int_7_time;
                          //刷卡實際交易金額
	                  int_7_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                         // sTmp.sprintf("fetc_card:現金 加值 int_7_money=%s ",
                         // gtsl_fetc_card->Strings[i].SubString(215,10)  );
                        //  writelog(sTmp);
                        }
                      else if( str_sale_type == "S1"   )   // else if( str_type == "A120"  )   //現金 加值取消
                        {
                          ++int_9_time;
	                  int_9_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                         // sTmp.sprintf("fetc_card:現金 加值取消 int_9_money=%s ",
                         // gtsl_fetc_card->Strings[i].SubString(215,10)  );
                         // writelog(sTmp);
                        }

                    }
                 else   //信用卡
                    {
                       //if( str_type == "A140"  )   //信用卡 加值
                       if( str_sale_type == "S0"   )   //信用卡 加值
                        {
                          ++int_1_time;
	                  int_1_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                          //sTmp.sprintf("fetc_card:信用卡 加值 int_1_money=%s ",
                          //gtsl_fetc_card->Strings[i].SubString(215,10)  );
                          //writelog(sTmp);
                        }
                      else if( str_sale_type == "S1"   )   //if(  str_type == "A120"  )   //信用卡 加值取消
                        {
                          ++int_2_time;
	                  int_2_money += iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                          //sTmp.sprintf("fetc_card:信用卡 加值取消 int_2_money=%s ",
                          //gtsl_fetc_card->Strings[i].SubString(215,10)  );
                          //writelog(sTmp);
                        }

                    }

	        }
			else if (_StrFind(str_r_type, "R2") || _StrFind(str_r_type,"R3"))
	        {
                   if(sCardNo.substr(1,4) == "0999") //現金卡
                    {
                      if( str_sale_type == "S0"  )   //現金 加值
                        {
                          --int_7_time;
	                  int_7_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                        }
                      else if(  str_sale_type == "S1"  )   //現金 加值取消
                           {
                            --int_9_time;
	                    int_9_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                           }

                    }
                  else   //信用卡
                    {
                       if( str_sale_type == "S0" )   //信用卡 加值
                        {
                          --int_1_time;
	                  int_1_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                        }
                       else if(   str_sale_type == "S1"  )   //信用卡 加值取消
                         {
                          --int_2_time;
	                  int_2_money -= iTmpAmt; //_StrToInt(gtsl_fetc_card->Strings[i].SubString(215,10));
                         }

                    }


	        }    // end of else if (str_r_type.Pos("R2") || str_r_type.Pos("R3"))

        }       // end of for
    }

     //結帳
  //  if (int_0_time)
  //  {
        giFetcSumCnt = int_0_time;
        giFetcSumAmt = int_0_money;
  //  }

    giFetcCashAddCnt=int_7_time-int_9_time;   //現金 加值
    giFetcCashAddamt=int_7_money-int_9_money; //現金 加值

    giFetcCredAddCnt=int_1_time-int_2_time;   //信用卡 加值
    giFetcCredAddamt=int_1_money-int_2_money;  //信用卡 加值

    logsprintf("fetc_card: e通卡結帳(%d,%d),現金 加值(%d,%d),現金 加值取消(%d,%d),信用卡 加值(%d,%d),信用卡 加值取消(%d,%d)",
                  giFetcSumCnt,giFetcSumAmt,
                  int_7_time, int_7_money, int_9_time, int_9_money,
                  int_1_time, int_1_money, int_2_time, int_2_money );
    //writelog(sTmp);
    return 0;
}


//折扣讓  for 1011   "G0" 食物券折扣    3011  NextPOS 合併 SumDisc 
void __fastcall BASIC::pay_sub_amt(int StartLine, int TotalLine)
{
    string str_tot_type;
    string str_pay_type,s, sTmp;
    int iQty, iAmt;
    giA = giB = 0;
    giC = giD = 0;

    if (gbl_pay_haveline)
    {
        //for (int i=0; i<gtsl_pay_sub->Count; i++)
		for (list<string>::iterator ls = gtsl_pay_sub->begin(); ls != gtsl_pay_sub->end(); ls++)
        {
			s = *ls; // gtsl_pay_sub->Strings[i];

            str_pay_type = _StringSegment_EX(s, SGM, 9);
            iQty = _StrToInt(_StringSegment_EX(s, SGM, 16));     //次數
            iAmt = _StrToInt(_StringSegment_EX(s, SGM, 17));     //小計折扣/折扣/讓金額

            str_tot_type = _StringSegment_EX(s, SGM, 22).substr(0,2);

            if (_StrFind(str_pay_type, "E"))  //小計折扣
            {
                if (_StrFind(str_tot_type, "FF") || _StrFind(str_tot_type, "Z0") || _StrFind(str_tot_type, "Z1") || Trim(str_tot_type) == "")
                {
                    if (_StrFind(str_pay_type, "E0"))
                    {
                        giC += iQty;      //次數
                        giD += iAmt;      //小計折扣/折扣/讓金額
                    }
                    else if (_StrFind(str_pay_type, "E1"))
                    {
                        giC -= iQty;      //次數
                        giD -= iAmt;      //小計折扣/折扣/讓金額
                    }
                }
                else if (_StrFind(str_tot_type, "R2") || _StrFind(str_tot_type, "R3"))
                {
                    if (_StrFind(str_pay_type, "E0"))
                    {
                        giC -= iQty;     //次數
                        giD -= iAmt;     //小計折扣/折扣/讓金額
                    }
                    else if (_StrFind(str_pay_type, "E1"))
                    {
                        giC += iQty;    //次數
                        giD += iAmt;    //小計折扣/折扣/讓金額
                    }
                }
                 
            }
            else                              //F為折扣讓   
            {
                if (_StrFind(str_tot_type, "FF") || _StrFind(str_tot_type, "Z0") || _StrFind(str_tot_type, "Z1") || Trim(str_tot_type) == "")
                {
                    if (_StrFind(str_pay_type, "F0") || _StrFind(str_pay_type, "G0") || _StrFind(str_pay_type, "H0"))
                    {
                        giA += iQty;     //次數
                        giB += iAmt;     //小計折扣/折扣/讓金額
                        //logsprintf("小計折扣/折扣/讓金額:%s+(%d,%d)",s.c_str(),giA, giB );
                    }
                    else if (_StrFind(str_pay_type, "F1") || _StrFind(str_pay_type, "G1") || _StrFind(str_pay_type, "H1"))
                    {
                        giA -= iQty;
                        giB -= iAmt;     //小計折扣/折扣/讓金額
                        //logsprintf("小計折扣/折扣/讓金額:%s-(%d,%d)",s.c_str(),giA, giB );
                    }
                    //writelog(sTmp);
                }
                else if (_StrFind(str_tot_type, "R2") || _StrFind(str_tot_type, "R3"))
                {
                    if (_StrFind(str_pay_type, "F0") || _StrFind(str_pay_type, "G0") || _StrFind(str_pay_type, "H0"))
                    {
                        giA -= iQty;
                        giB -= iAmt;     //小計折扣/折扣/讓金額
                        //logsprintf("小計折扣/折扣/讓金額:%s-(%d,%d)",s.c_str(),giA, giB );
                    }
                    else if (_StrFind(str_pay_type, "F1") || _StrFind(str_pay_type, "G1") || _StrFind(str_pay_type, "H1"))
                    {
                        giA += iQty;
                        giB += iAmt;     //小計折扣/折扣/讓金額
                        //logsprintf("小計折扣/折扣/讓金額:%s+(%d,%d)",s.c_str(),giA, giB );
                    }
                    //writelog(sTmp);
                }
            }
        }
    }
   
    giB = (0 - giB);
    giD = (0 - giD);

    logsprintf("小計折扣(%d,%d), 折扣/讓金額:合計(%d,%d)", giC, giD, giA, giB );

}



string __stdcall get_version(string S)
{
    string str_ver;

    if (S.length() == 8)
        str_ver = S.substr(2,6); //(S.SubString(2,2) + S.SubString(5,4));
    else
        str_ver = "999999";

	return S; // str_ver;
}



int __fastcall BASIC::subsale_update(TStringList * tsl, string good_no,
                                     string menu_no, string bar_name,
                                     string item_amt, string sal_type)
{
	string str_record, sfindRec;      //單筆記錄
    string _good_no, _menu_no, _bar_name;   //貨號、Menu代號、商品名稱
    int _item_amt = 0;                          //每樣代售商品金額總合
    int int_record = 0;                         //單筆記錄index
    bool bl_find_record = false;                //是否有找到相同商品


	//for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
	list<string>::iterator lsFund;
    if (tsl->size()) //已有代售記錄，比對是否有相同項目
    {
        bl_find_record = false;

        //for (int i=0; i<tsl->Count; i++)
		for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++)
        {
			str_record = *ls; // tsl->Strings[i];
			sfindRec = "";
            if ( _StrFind(str_record,good_no) )
            {
                //int_record = i;
				//sfindRec = str_record;
				lsFund = ls;
                bl_find_record = true;
                break;
            }
        }

        if (bl_find_record)     //有相同代售項目
        {
			str_record = *lsFund; // sfindRec; // tsl->Strings[int_record];
			tsl->erase(lsFund);  //Delete(int_record);
			
            _good_no = _StringSegment_EX(str_record, "|", 1);
            _menu_no = _StringSegment_EX(str_record, "|", 2);
            _bar_name = _StringSegment_EX(str_record, "|", 3);
            _item_amt = _StrToInt(_StringSegment_EX(str_record, "|", 4));

            if (sal_type == "Z")
            {
                _item_amt = _item_amt + _StrToInt(item_amt);
            }
            else
            {
                _item_amt = _item_amt - _StrToInt(item_amt);
            }

            str_record = _good_no + "|" + _menu_no + "|" + _bar_name + "|" + int2str(_item_amt);
			tsl->push_back(str_record); //  Add(str_record);
        }
        else    //沒有相同代售項目
        {
            if (sal_type == "Z")
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|" + GetNumber(item_amt);
            else
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|-" + GetNumber(item_amt);

			tsl->push_back(str_record); //  Add(str_record);
        }

    }
    else    //無記錄項目
    {
        if (sal_type == "Z")
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|" + GetNumber(item_amt);
            else
                str_record = good_no + "|" + menu_no + "|" + bar_name + "|-" + GetNumber(item_amt);

		tsl->push_back(str_record); //  Add(str_record);

    }

	// 清除 data 為 NULL 的 node
	//tsl->remove((string *)NULL);
	tsl->sort();
	 

  return(0);
}


string __stdcall GetNumber(string S)
{
    string str_num;

    try
    {
        if(Trim(S)=="")
          S="0";
		str_num = to_string(stoi(S));   //stof(s)();
    }
    catch(...)
    {
        writelog(("數值轉換發生錯誤 " + S));
        return "0";
    }

    return str_num;
}



/**   保留預備用
bool __stdcall TForm1::Scan_SalLine(FILE *f,  char *pstr)
 {
    char cData[2049],cTmpData[2049], cHead[128],clen[5],c;
    int ilen,j;
    bool brtn,bErr;
    String sLog,sSaleData;

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
        fscanf_s(f, "%c", str);
       // if (*str=='\n') break;
        str++;
       // if (*(str-1)!=' ') ptr = str;
    }
    *str = '\0';
 }






//收銀員交班明細表
//iSumCash41:1050現金投庫,  iSumPresent41:1050禮券+商品禮券, iSumCpn41:1050折價券+Other1+有價票券 CPN
//iSumPsRtn41:1050餘額券 ,  iSumVisualCpnAmt:1040無紙本折價金額, iWinnInvoAmt:1050發票兌獎金額
int __fastcall BASIC::SumCasherRpt(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   string sTranCnt, string sXdtNo,
                                   string SCasherRpt,
                                   string sXdtFile,
                                   string sStr1001,
                                   string sStr1097,
                                   string sStr1099,
                                   string &RtnSumCasherData,
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
    char  buffer[10240];

    int ipay_cash_amt, ipay_present_amt, ipay_cpn_amt,cashi, iTmp;

    int pay_other1_amt, pay_other2_amt;

    //PAYCASHREC pay_cash_rec[100];
	PAYCASHREC pay_cash_rec[101];

    iacc_invo_amt=iacc_amt01=iacc_amt02=iacc_amt03=iacc_amt04=iacc_amt05=
        iacc_amt06=iacc_amt07=iacc_amt08=iacc_amt09=iacc_amt10=iacc_amt11=
        iacc_amt12=iacc_amt13=iacc_amt14=0;

    iacc_invo_cnt=iacc_cnt01=iacc_cnt02=iacc_cnt03=iacc_cnt04=iacc_cnt05=
        iacc_cnt06=iacc_cnt07=iacc_cnt08=iacc_cnt09=iacc_cnt10=iacc_cnt11=
        iacc_cnt12=iacc_cnt13=iacc_cnt14=0;

    icash_short_amt=ipresent_short_amt=icpn_short_amt=itot_short_amt=
        ircv_cnt=iinvo_rtn_cnt=iinvo_void_cnt=0;

    ipay_cash_amt=ipay_present_amt=ipay_cpn_amt=0;

	pay_other1_amt=pay_other2_amt=0;

    string str_sale_type, strtmp, tmpSer, tmp0041Date1, tmp0041ChrName,s, sLog;       //結帳型態


    logsprintf("SumCasherRpt: 收銀員交班明細表 StartLine=%d, iSumCash41=%d, iSumPsRtn41=%d, iSumVisualCpnAmt=%d, ifoodamt=%d, ifoodcnt=%d",
                StartLine, iSumCash41, iSumPsRtn41, iSumVisualCpnAmt, ifoodamt, ifoodcnt);

    //writelog(sLog);

    for( cashi=0; cashi<100; cashi++)
        {
               pay_cash_rec[cashi].IndexNo=-1;
               pay_cash_rec[cashi].PayCashAmt=-9999;
        }

    int int_drop_line = gtsl_drop->size() - 1;   //投庫次數
    //for (int i=StartLine, cashi=0; i<=int_drop_line; i++, cashi++)  //1040投庫
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
        {
			s = *ls; // gtsl_drop->Strings[i];
           _StringSegment_EX(s, SGM, 1);

           iTmp=_StrToInt(_StringSegment_EX(s, SGM, 9));   //本日頭庫累積次數 交班時產生
           if( iTmp == 0)
             {
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=0;
               continue;
              }

           //tmp0041Date1=gtsl_drop->Strings[i].SubString(1,60); // Rec_Type~sale_no
           tmp0041ChrName=_StringSegment_EX(s, SGM, 58);  //  gtsl_drop->Strings[i].SubString(450,10);

           tmpSer=_StringSegment_EX(s, SGM, 7);    //交易序號
            //現金
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=_StrToInt(_StringSegment_EX(s, SGM, 11));
               if( pay_cash_rec[cashi].PayCashAmt >= 0)
                   ipay_cash_amt +=pay_cash_rec[cashi].PayCashAmt;
            //禮券
               ipay_present_amt += _StrToInt(_StringSegment_EX(s, SGM, 14));

            //折價券
               ipay_cpn_amt += _StrToInt(_StringSegment_EX(s, SGM, 15));

			//中獎發票金額  500
			   pay_other1_amt += _StrToInt(_StringSegment_EX(s, SGM, 19));

            //中獎發票金額  1000
               pay_other2_amt +=  _StrToInt(_StringSegment_EX(s, SGM, 20));

            //中獎發票金額 200
               iacc_invo_amt += _StrToInt(_StringSegment_EX(s, SGM, 22));

            //費用單據金額 iacc_amt01..iacc_cnt14
               iacc_amt01+=_StrToInt(_StringSegment_EX(s, SGM, 23));
               iacc_cnt01+=_StrToInt(_StringSegment_EX(s, SGM, 24));

               iacc_amt02+=_StrToInt(_StringSegment_EX(s, SGM, 25));
               iacc_cnt02+=_StrToInt(_StringSegment_EX(s, SGM, 26));

               iacc_amt03+=_StrToInt(_StringSegment_EX(s, SGM, 27));
               iacc_cnt03+=_StrToInt(_StringSegment_EX(s, SGM, 28));

               iacc_amt04+=_StrToInt(_StringSegment_EX(s, SGM, 29));
               iacc_cnt04+=_StrToInt(_StringSegment_EX(s, SGM, 30));

               iacc_amt05+=_StrToInt(_StringSegment_EX(s, SGM, 31));
               iacc_cnt05+=_StrToInt(_StringSegment_EX(s, SGM, 32));

               iacc_amt06+=_StrToInt(_StringSegment_EX(s, SGM, 33));
               iacc_cnt06+=_StrToInt(_StringSegment_EX(s, SGM, 34));

               iacc_amt07+=_StrToInt(_StringSegment_EX(s, SGM, 35));
               iacc_cnt07+=_StrToInt(_StringSegment_EX(s, SGM, 36));

               iacc_amt08+=_StrToInt(_StringSegment_EX(s, SGM, 37));
               iacc_cnt08+=_StrToInt(_StringSegment_EX(s, SGM, 38));

               iacc_amt09+=_StrToInt(_StringSegment_EX(s, SGM, 39));
               iacc_cnt09+=_StrToInt(_StringSegment_EX(s, SGM, 40));

               iacc_amt10+=_StrToInt(_StringSegment_EX(s, SGM, 41));
               iacc_cnt10+=_StrToInt(_StringSegment_EX(s, SGM, 42));

               iacc_amt11+=_StrToInt(_StringSegment_EX(s, SGM, 43));
               iacc_cnt11+=_StrToInt(_StringSegment_EX(s, SGM, 44));

               iacc_amt12+=_StrToInt(_StringSegment_EX(s, SGM, 45));
               iacc_cnt12+=_StrToInt(_StringSegment_EX(s, SGM, 46));

               iacc_amt13+=_StrToInt(_StringSegment_EX(s, SGM, 47));
               iacc_cnt13+=_StrToInt(_StringSegment_EX(s, SGM, 48));

               iacc_amt14+=_StrToInt(_StringSegment_EX(s, SGM, 49));
               iacc_cnt14+=_StrToInt(_StringSegment_EX(s, SGM, 50));


        }     //for (int i=StartLine; i<=TotalLine; i++)

     int itot_acc_amt;   //費用單據金額
     itot_acc_amt=iacc_amt01+iacc_amt02+iacc_amt03+iacc_amt04+iacc_amt05+
                iacc_amt06+iacc_amt07+iacc_amt08+iacc_amt09+iacc_amt10+
                iacc_amt11+iacc_amt12;//+iacc_amt13+iacc_amt14;   20151123 改為食安與提貨券  Charles.Wang

     iacc_amt13=ifoodamt; //食安退款金額 ;
     iacc_cnt13=ifoodcnt;//食安退款次數 ;

     //現金短溢收=(現金+中獎發票金額(200+1000)+費用單據金額+找餘額券)-XDT現金 ,當acc_type > 01時才有值
     //現金短溢收=(現金+費用單據金額+找餘額券)-XDT現金 ,當acc_type > 01時才有值  // 2017/0815 Update
    //int itotSumCash41=(ipay_cash_amt+iacc_invo_amt+pay_other2_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);
    int itotSumCash41=(ipay_cash_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);     // 2017/0815 Update
    int itotSumPresent41=ipay_present_amt-iSumPresent41; //禮券短溢收,當acc_type > 01時才有值
    int itotSumCpn41=ipay_cpn_amt-iSumCpn41;
    int itotWinInvoAmt41=(iacc_invo_amt+ pay_other1_amt+pay_other2_amt)-iWinnInvoAmt;    // 2017/08/15 Update 中獎發票金額

    int iqt_cetel, iqt_R23Cnt, iqt_VCnt;
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    int iRentAmt, iRemtCnt;
    sub_times(0,gi_rec_line);   //giH giI 食安退貨金額、次數       //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_cetel=giD;
    iqt_food=giI; iamt_food=giH;

    TotMissCnt(0,gi_tot_line);

    Sum040RecycleDataCnt(0, gi_bil_line);      //廢資源回收數 /提貨券金額/ 提貨券張數
    iacc_amt14=giE; //提貨券金額
    iacc_cnt14=giF; //提貨券張數

	// 2018/12/07 Lu Update
	int iPaCpnAmt = giM;         //紙本折價券金額
	int iPaCpnCnt = giN;         //紙本折價券張數
	int iPaPresentAmt = giO;    //紙本禮券金額
	int iPaPresentCnt = giP;    //紙本禮券張數

    iqt_R23Cnt=giA;
    iqt_VCnt=giB;

    logsprintf("SumCasherRpt 收銀員交接班明細表:收銀員交班(%s) 投庫筆數(%d), 代收張數(%d), 作廢發票張數(%d), 交易取消作廢發票張數(%d), 找餘額券(%d),提貨券金額(%d),提貨券張數(%d)",
        _StringSegment_EX(sTranCnt, SGM, 8).c_str(), int_drop_line + 1, iqt_cetel, iqt_R23Cnt, iqt_VCnt, iSumPsRtn41, iacc_amt14, iacc_cnt14);
    //writelog(strtmp);
     
    FILE *fxdata;
   if( Trim(sXdtFile)!="")
     {

#ifdef WIN32
		 errno_t err;
		 if ((err = fopen_s(&fxdata, sXdtFile.c_str(), "a+")) == 0)
#else
		 if ((fxdata = fopen(sXdtFile.c_str(), "a+")) != NULL)
#endif
       {
        try
         {
          //3041|電文  RtnSumCasherData.sprintf
		 sprintf_s(buffer, sizeof(buffer),"%s||%04d||%04d||%010ld||%010ld||%010ld||%2s||%010ld||%010ld||%010ld  ||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||  %010ld||%010ld||%010ld||%010ld||%04d||%04d||%04d||%-10.10s||%s||",
                                         sTranCnt.c_str(),  // ID_RECTYPE :1  ~ NO_CASHER:8
                                         0,                 //本日投庫累計次數  
                                         0,                 //本收銀員投庫累計次數
                                         ipay_cash_amt,     //現金付款
                                         ipay_present_amt,  //禮卷付款
                                         ipay_cpn_amt,      //折價券付款
                                         sXdtNo.c_str(),   //[00]:投庫, [>01]:交班序號                 
                                         pay_other1_amt,   //中獎發票 500
                                         pay_other2_amt,  //中獎發票 1000
                                         iacc_invo_amt,    //中獎發票 200
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
                                         iacc_amt13,     //食安退款金額  42
                                         iacc_cnt13,     //食安退款筆數  43
                                         iacc_amt14,     //提貨券金額
                                         iacc_cnt14,     //提貨券張數
                                         itotSumCash41, //現金短溢收,當acc_type > 01時才有值
                                         itotSumPresent41, //禮券短溢收,當acc_type > 01時才有值
                                         itotSumCpn41, //折價券短溢收,當acc_type > 01時才有值
                                         itotSumCash41+itotSumPresent41+itotSumCpn41+itotWinInvoAmt41, //短溢收合計,當acc_type > 01時才有值
                                         iqt_cetel, //代收張數,當acc_type > 01時才有值
                                         iqt_R23Cnt, //作廢發票張數,當acc_type > 01時才有值
                                         iqt_VCnt, //交易取消作廢發票張數,當acc_type > 01時才有值
                                         tmp0041ChrName.c_str(),  //10,收銀員 53
                                         "1||2||3||4||5||6||7||8||9||10" );  //保留,ver1 9byte
		  RtnSumCasherData = string(buffer);
          fprintf(fxdata,"%s",RtnSumCasherData.c_str());

          //RtnSumCasherData+=sStr1097;
          //fprintf(fxdata,"%s\n",sStr1097);
          fprintf(fxdata,"%s\n",sStr1099.c_str());


		  logsprintf("收銀員交班DATA->0041:%s OK", sXdtFile.c_str());
          //writelog(strtmp);
        }
      catch(...)
       {
		   logsprintf("收銀員交班DATA->0041:%s Catch Error", sXdtFile.c_str());
          //writelog(strtmp);
       }

      fclose(fxdata);

      if( iAutoZ )
        DeleteFile(StringToWString(sXdtFile).c_str());

      }   // end of if ((fxdata = fopen(sXdtFile.c_str(), "a+")) != NULL)
   } //end of  if( Trim(sXdtFile)!="")


   TStringList *tslCrp;
   tslCrp = new TStringList;
   tslCrp->clear();
   string nt,NowTime, sStoreNo, sStoreName;

   //TIniFile *AutoCrpIni, *xzdata_ecr;
   // AutoCrpIni = new TIniFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");
   int i1,i2,iFTLSysPrinterInitial;
   
   //xzdata_ecr = new TIniFile(XZDATA_INI);
   m_IniReader.setINIFileName(XZDATA_INI);
   iFTLSysPrinterInitial = stoi(m_IniReader.ReadString("ECR", "FTLSysPrinterInitial", "0"));
   //xzdata_ecr->ReadInteger("ECR", "FTLSysPrinterInitial", 0);     //累積結帳次數序號
   //delete xzdata_ecr;
 
   string CmdStr="";
   if (iFTLSysPrinterInitial == 1)
   {
	   sprintf_s(buffer, sizeof(buffer), "%c%cM080801", 27, 27);    //ESC, ESC, M , 字型[2]:08, SIZE[2]:08, 樣式[2]:00
	   CmdStr = buffer;
   }
   logsprintf("SumCasherRpt: 收銀員交接班明細表:XZDATA_INI->ECR->FTLSysPrinterInitial=%d, CmdStr(%s)", iFTLSysPrinterInitial, CmdStr.c_str());

   sTranCnt +=SGM;
   nt=  _StringSegment_EX(sTranCnt, SGM, 5); // sTranCnt.SubString(21,14);  //1041
   NowTime=nt.substr(0,4)+"-"+nt.substr(4,2)+"-"+nt.substr(6,2)+"  "+nt.substr(8,2)+":"+nt.substr(10,2);
   read_ten_name(sStoreNo, sStoreName);

   sprintf_s(buffer, sizeof(buffer), "%s         收銀員交接班明細表", CmdStr.c_str() ); tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "%s%s     機號:%s", CmdStr.c_str(), NowTime.c_str(), _StringSegment_EX(sTranCnt, SGM, 4).c_str() );   tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "%s店號:%s    店名:%s", CmdStr.c_str(), _StringSegment_EX(sTranCnt, SGM, 3).c_str(), sStoreName.c_str());  tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "%s收銀員編號:%s", CmdStr.c_str(), Trim(_StringSegment_EX(sTranCnt, SGM, 8)).c_str());   tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "%s投庫明細:", CmdStr.c_str());   tslCrp->push_back(buffer);

   string ReadCashStr,CashStr;
   int  ReadCashCnt=0;
   int  ReadCashtot=0;
   int  ReadCashCntIdx=0;
   CashStr="現金|";
   //strtmp.sprintf("      ＋現           金:%8ld",ipay_cash_amt);   tslCrp->Add(strtmp);

   m_IniReader.setINIFileName("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");
   ReadCashStr = m_IniReader.ReadString("AutoX", "Cashdrp", "0");   //AutoCrpIni->ReadString("AutoX", "Cashdrp", "0");
   ReadCashCnt = stoi(m_IniReader.ReadString("AutoX", "Cashdrpcnt", "0"));   //AutoCrpIni->ReadInteger("AutoX", "Cashdrpcnt", 0);
   ReadCashtot = stoi(m_IniReader.ReadString("AutoX", "Cashdrptot", "0"));  //AutoCrpIni->ReadInteger("AutoX", "Cashdrptot", 0);

   if(ReadCashCnt > 0)
     {
       sprintf_s(buffer, sizeof(buffer), "%s  ＋現           金:自.%8ld", CmdStr.c_str(), ReadCashtot);
	   tslCrp->push_back(buffer); //tslCrp->Add(strtmp);
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
		    sprintf_s(buffer, sizeof(buffer), "%s  ＋現           金:%2d.%8ld", CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt );
			tslCrp->push_back(buffer); //tslCrp->Add(strtmp);
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||", 0, 0);

			   CashStr += string(buffer);  
              }

          }
       else
          {

			sprintf_s(buffer, sizeof(buffer), "%s                   :%2d.%8ld", CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt);
			tslCrp->push_back(buffer); //tslCrp->Add(strtmp);
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||", 0, 0);
			   CashStr += string(buffer);
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

			   //m_IniReader.WriteString("0", "Cashdrptot", "AutoX");   // AutoCrpIni->WriteString("AutoX", "Cashdrp", "0");
			   m_IniReader.WriteString("AutoX", "Cashdrptot", "0");
			   m_IniReader.WriteString("AutoX", "Cashdrpcnt", int2str(ReadCashCnt));    // AutoCrpIni->WriteInteger("AutoX", "Cashdrpcnt", ReadCashCnt);
			   m_IniReader.WriteString("AutoX", "Cashdrptot", int2str(ReadCashtot));    //AutoCrpIni->WriteInteger("AutoX", "Cashdrptot", ReadCashtot);
         }


   ////////
   strtmp = m_IniReader.ReadString("AutoX", "pres", "0");    //strtmp = AutoCrpIni->ReadString("AutoX", "pres", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s　＋禮券 / 商品禮券     :%7ld", CmdStr.c_str(), ipay_present_amt + i2);   tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "禮券||%02d||%8ld||", 0, ipay_present_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "禮券||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "pres", string(buffer));   //AutoCrpIni->WriteString("AutoX", "pres", strtmp);
     }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "cpn", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "cpn", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋折價券/紅利點數     :%7ld", CmdStr.c_str(), ipay_cpn_amt + i2);   tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "折價||%02d||%8ld||", 0, ipay_cpn_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "折價||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "cpn", string(buffer));   //AutoCrpIni->WriteString("AutoX", "cpn", strtmp);
     }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "invo", "0"); // strtmp = AutoCrpIni->ReadString("AutoX", "invo", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋中獎發票金額(200元) :%7ld", CmdStr.c_str(), iacc_invo_amt + i2);  tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "中獎||%02d||%8ld||", 0, iacc_invo_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "中獎||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "invo", string(buffer));  //AutoCrpIni->WriteString("AutoX", "invo", strtmp);
    }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "invo500", "0");  //strtmp = AutoCrpIni->ReadString("AutoX", "invo1000", "0");
   i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋中獎發票金額(500元) :%7ld", CmdStr.c_str(), pay_other1_amt + i2);  tslCrp->push_back(buffer);

   if (Update)
   {
	   if (iAutoZ)
		   sprintf_s(buffer, sizeof(buffer), "中獎500||%02d||%8ld||", 0, pay_other1_amt + i2);
	   else
		   sprintf_s(buffer, sizeof(buffer), "中獎500||%02d||%8ld||", 0, 0);
	   m_IniReader.WriteString("AutoX", "invo500", string(buffer));   //AutoCrpIni->WriteString("AutoX", "invo500", strtmp);
   }

    ////////
   strtmp = m_IniReader.ReadString("AutoX", "invo1000", "0");  //strtmp = AutoCrpIni->ReadString("AutoX", "invo1000", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋中獎發票金額(1000元):%7ld", CmdStr.c_str(), pay_other2_amt + i2);  tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "中獎1000||%02d||%8ld||", 0, pay_other2_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "中獎1000||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "invo1000", string(buffer));   //AutoCrpIni->WriteString("AutoX", "invo1000", strtmp);
    }


   ///////
   strtmp = m_IniReader.ReadString("AutoX", "acc", "0");  //strtmp = AutoCrpIni->ReadString("AutoX", "acc", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋費用單據金額        :%7ld", CmdStr.c_str(), itot_acc_amt + i2);  tslCrp->push_back(buffer);
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "費用||%02d||%8ld||", 0, itot_acc_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "費用||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "acc", string(buffer));   //AutoCrpIni->WriteString("AutoX", "acc", strtmp);
     }

   ///////

   ///////  食安退款  20151202 全家取消此行計算，移至退款筆數前
   /*strtmp=AutoCrpIni->ReadString("AutoX","food","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("      －食安退款       :   %8ld",iacc_amt13+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("食安|%02d|%8ld|", 0, iacc_amt13+i2);
      else
       strtmp.sprintf("食安|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","food",strtmp);
     }
   */
   ///////



   strtmp = m_IniReader.ReadString("AutoX", "drpacc", "0");  //strtmp = AutoCrpIni->ReadString("AutoX", "drpacc", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   //sLog.sprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> drpacc = %s ,投庫小計  :%8ld", strtmp,
   //                                               ipay_cash_amt+ipay_present_amt+ipay_cpn_amt+iacc_invo_amt+itot_acc_amt+i2 );
   //writelog(sLog);


   sprintf_s(buffer, sizeof(buffer), "------------------------------------"); tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "%s  投庫小計         :   %8ld\n", CmdStr.c_str(), ipay_cash_amt + ipay_present_amt + ipay_cpn_amt +
	   iacc_invo_amt + pay_other1_amt + pay_other2_amt + itot_acc_amt + i2);                   tslCrp->push_back(buffer);
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "投庫||%02d||%8ld||", 0, ipay_cash_amt + ipay_present_amt + ipay_cpn_amt + iacc_invo_amt + pay_other2_amt + itot_acc_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "投庫||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "drpacc", string(buffer));   //AutoCrpIni->WriteString("AutoX", "drpacc", strtmp);
     }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "Cashtot", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "Cashtot", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> Cashtot = %s ,現金短溢收  :%8ld", strtmp.c_str(), itotSumCash41 + i2);
   
 //strtmp.sprintf("現金短溢收  :01234567  收銀員簽章",itotSumCash41);   tslCrp->Add(strtmp);
   sprintf_s(buffer, sizeof(buffer), "%s現金短溢收  :%7ld 收銀員簽章", CmdStr.c_str(), itotSumCash41 + i2);   tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		 sprintf_s(buffer, sizeof(buffer), "現短||%02d||%8ld||", 0, itotSumCash41 + i2);
     else
		 sprintf_s(buffer, sizeof(buffer), "現短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "Cashtot", string(buffer));   //AutoCrpIni->WriteString("AutoX", "Cashtot", strtmp);
    }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "prestot", "0");     //strtmp = AutoCrpIni->ReadString("AutoX", "prestot", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> prestot = %s ,禮券短溢收  :%8ld", strtmp.c_str(), itotSumPresent41 + i2);
   //writelog(sLog);

   sprintf_s(buffer, sizeof(buffer), "%s禮券短溢收  :%7ld ┌──── ┐", CmdStr.c_str(), itotSumPresent41 + i2);   tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "禮短||%02d||%8ld||", 0, itotSumPresent41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "禮短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "prestot", string(buffer));    //AutoCrpIni->WriteString("AutoX", "prestot", strtmp);
    }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "cpntot", "0");    //strtmp = AutoCrpIni->ReadString("AutoX", "cpntot", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> cpntot = %s , 折價券短溢收:%8ld", strtmp.c_str(), itotSumCpn41 + i2);
   
   sprintf_s(buffer, sizeof(buffer), "%s折價券短溢收:%7ld │         │", CmdStr.c_str(), itotSumCpn41 + i2);   tslCrp->push_back(buffer);
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "折短||%02d||%8ld||", 0, itotSumCpn41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "折短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "cpntot", string(buffer));   //AutoCrpIni->WriteString("AutoX", "cpntot", strtmp);
     }

   ////////   中獎發票金額  ////////////
   strtmp = m_IniReader.ReadString("AutoX", "wininvotot", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "wininvotot", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> wininvotot = %s , 中獎發票短溢收:%8ld", strtmp.c_str(), itotWinInvoAmt41 + i2);
   
   sprintf_s(buffer, sizeof(buffer), "%s中獎發票短溢收:%5ld │         │", CmdStr.c_str(), itotWinInvoAmt41 + i2);   tslCrp->push_back(buffer);
   int iTmptotWinInvoAmt41 = itotWinInvoAmt41+i2;
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "獎短||%02d||%8ld||", 0, itotWinInvoAmt41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "獎短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "wininvotot", string(buffer));   //AutoCrpIni->WriteString("AutoX", "wininvotot", strtmp);
     }

   ///////////////////////////
   strtmp = m_IniReader.ReadString("AutoX", "subtot", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "subtot", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> subtot = %s , 短溢收合計  :%8ld", strtmp.c_str(),
                                                  itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41 );
   
   sprintf_s(buffer, sizeof(buffer), "%s短溢收合計  :%7ld │         │", CmdStr.c_str(), itotSumCash41 + itotSumPresent41 + itotSumCpn41 + i2 + itotWinInvoAmt41);   
   tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "短合||%02d||%8ld||", 0, itotSumCash41 + itotSumPresent41 + itotSumCpn41 + i2 + itotWinInvoAmt41);
      else
		  sprintf_s(buffer, sizeof(buffer), "短合||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "subtot", string(buffer));   //AutoCrpIni->WriteString("AutoX", "subtot", strtmp);
     }

   ///////
   strtmp = m_IniReader.ReadString("AutoX", "agncnt", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "agncnt", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> agncnt = %s , 代收張數    :%8ld", strtmp.c_str(), iqt_cetel + i2);
  
   sprintf_s(buffer, sizeof(buffer), "%s代收張數    :%7ld │         │", CmdStr.c_str(), iqt_cetel + i2);  tslCrp->push_back(buffer);
   if( iAutoZ )
	   sprintf_s(buffer, sizeof(buffer), "代張||%02d||%8ld||", 0, iqt_cetel + i2);  // 2013/10/17 Update Lu CodeView
   else
	   sprintf_s(buffer, sizeof(buffer), "代張||%02d||%8ld||", 0, 0);
   m_IniReader.WriteString("AutoX", "agncnt", string(buffer));     //AutoCrpIni->WriteString("AutoX", "agncnt", strtmp);
   
/////////////////////////// 2014/12/10 //////////////////////////////////////////////////////////////
   strtmp = m_IniReader.ReadString("AutoX", "s1051cnt", "0");    //strtmp = AutoCrpIni->ReadString("AutoX", "s1051cnt", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   string sMissRec, AutoXsMissRec, PrintAutoXsMissRec;
   AutoXsMissRec = m_IniReader.ReadString("AutoX", "s1051cntMissRec", "");
   S1051MissCnt(0, gi_1051_line, sMissRec);
   int  sMissRecCnt = giC;
   AutoXsMissRec += sMissRec;
   PrintAutoXsMissRec = AutoXsMissRec;

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> s1051cnt = %s ,紙本作廢發票張數  :%8ld, AutoXsMissRec(%s)", strtmp.c_str(), giA + i2, AutoXsMissRec.c_str() );
  
   sprintf_s(buffer, sizeof(buffer), "%s紙本作廢發票:%7ld │         │", CmdStr.c_str(), giA + i2);   tslCrp->push_back(buffer);

   if (Update)
   {
	   if (iAutoZ)
	   {
		   sprintf_s(buffer, sizeof(buffer), "紙廢||%02d||%8ld||", 0, giA + i2);
	   }
	   else
	   {
	   	   sprintf_s(buffer, sizeof(buffer), "紙廢||%02d||%8ld||", 0, 0);
		   AutoXsMissRec = "";
       }

	  m_IniReader.WriteString("AutoX", "s1051cnt", string(buffer));   //AutoCrpIni->WriteString("AutoX", "s1051cnt", strtmp);
	  m_IniReader.WriteString("AutoX", "s1051cntMissRec", AutoXsMissRec);
   }

///////////////////////////////////////////////////////////////////////////////////////////////////////
	 // 2018/12/07 Lu Update
	 //iPaCpnAmt 紙本折價券金額
	 //iPaCpnCnt 紙本折價券張數
	 //iPaPresentAmt 紙本禮券金額
	 //iPaPresentCnt 紙本禮券張數

	 strtmp = m_IniReader.ReadString("AutoX", "iPaPresentCnt", "0");  //紙本折禮券張數
	 i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
	 i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));
	 
	 logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> iPaPresentCnt = %s ,紙本禮券張數  :%8ld", strtmp.c_str(),  iPaPresentCnt + i2);

	 sprintf_s(buffer, sizeof(buffer), "%s紙本禮券張數:%7ld │         │", CmdStr.c_str(), iPaPresentCnt + i2);   tslCrp->push_back(buffer); 

	 if (Update)
	 {
		 if (iAutoZ)
			 sprintf_s(buffer, sizeof(buffer), "紙禮||%02d||%8ld||", 0, iPaPresentCnt + i2);
		 else
			 sprintf_s(buffer, sizeof(buffer), "紙禮||%02d||%8ld||", 0, 0);

		 m_IniReader.WriteString("AutoX", "iPaPresentCnt", string(buffer));
	 }

	 /////////////////////////////////////////////////
	 strtmp = m_IniReader.ReadString("AutoX", "iPaCpnCnt", "0");  //紙本折折價券張數
	 i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
	 i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));

	 logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> iPaCpnCnt = %s ,紙本折價券張數  :%8ld", strtmp.c_str(), iPaCpnCnt + i2);

	 sprintf_s(buffer, sizeof(buffer), "%s紙本折價券張數:%5ld │         │", CmdStr.c_str(), iPaCpnCnt + i2);   tslCrp->push_back(buffer);

	 if (Update)
	 {
		 if (iAutoZ)
			 sprintf_s(buffer, sizeof(buffer), "紙折||%02d||%8ld||", 0, iPaCpnCnt + i2);
		 else
			 sprintf_s(buffer, sizeof(buffer), "紙折||%02d||%8ld||", 0, 0);

		 m_IniReader.WriteString("AutoX", "iPaCpnCnt", string(buffer));
	 }

	 ///////////////////////////////////////////////////////////////////////////////////////////////////////
   strtmp = m_IniReader.ReadString("AutoX", "r23cnt", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "r23cnt", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> r23cnt = %s , 作廢發票張數    :%8ld", strtmp.c_str(), iqt_R23Cnt + iqt_VCnt + i2);

   sprintf_s(buffer, sizeof(buffer), "%s作廢發票總數:%7ld │         │", CmdStr.c_str(), iqt_R23Cnt + iqt_VCnt + i2);   tslCrp->push_back(buffer);

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "廢張||%02d||%8ld||", 0, iqt_R23Cnt + iqt_VCnt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "廢張||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "r23cnt", string(buffer));   //AutoCrpIni->WriteString("AutoX","r23cnt",strtmp);
     }

////////////////////食安退款筆數2015/12/02///////////////////////////////////////////////////////////////////////////////////

   strtmp = m_IniReader.ReadString("AutoX", "Foodcnt", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "Foodcnt", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   //iacc_amt13,     //食安退款金額
   //iacc_cnt13,     //食安退款筆數
   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> Foodcnt = %s ,食安退款筆數 :%4ld, 食安退款金額 :%8ld,", strtmp.c_str(),
                                                      iacc_cnt13+i1, iacc_amt13+i2);
   sprintf_s(buffer, sizeof(buffer), "%s食安退款金額:%7ld │         │", CmdStr.c_str(), iacc_amt13 + i2);   tslCrp->push_back(buffer);
    
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "食退||%04d||%8ld||", iacc_cnt13 + i1, iacc_amt13 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "食退||%04d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "Foodcnt", string(buffer));   //AutoCrpIni->WriteString("AutoX","Foodcnt",strtmp);
     }

  /////////////////////////// 2015/10/22 //////////////////////////////////////////////////////////////

   strtmp = m_IniReader.ReadString("AutoX", "BillAmt", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","BillAmt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   //iacc_amt14,     //提貨券金額
   //iacc_cnt14,     //提貨券張數

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> BillAmt = %s ,提貨券張數 :%4ld, 提貨券金額 :%8ld,", strtmp.c_str(),
                                                      iacc_cnt14+i1, iacc_amt14+i2);
   sprintf_s(buffer, sizeof(buffer), "%s提貨券金額  :%7ld │         │", CmdStr.c_str(), iacc_amt14 + i2);   tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "%s提貨券張數  :%7ld └─── ─┘", CmdStr.c_str(), iacc_cnt14 + i1);   tslCrp->push_back(buffer);
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "提貨||%04d||%8ld||", iacc_cnt14 + i1, iacc_amt14 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "提貨||%04d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "BillAmt", string(buffer));   //AutoCrpIni->WriteString("AutoX","BillAmt",strtmp);
     }

 /////////////////////////////////////////////////////////////////////////////////////////////////

   if( Cpn71_amt )
   {
	   sprintf_s(buffer, sizeof(buffer), "%s廢電池袋數  :%7ld  ", CmdStr.c_str(), Cpn71_amt);   tslCrp->push_back(buffer);
   }
   if( Cpn72_amt )
   {
	   sprintf_s(buffer, sizeof(buffer), "%s廢手機袋數  :%7ld  ", CmdStr.c_str(), Cpn72_amt);   tslCrp->push_back(buffer);
   }
   if( Cpn73_amt )
   {
	   sprintf_s(buffer, sizeof(buffer), "%s廢筆電袋數  :%7ld  ", CmdStr.c_str(), Cpn73_amt);   tslCrp->push_back(buffer);
   }
   if( Cpn74_amt )
   {
	   sprintf_s(buffer, sizeof(buffer), "%s廢光碟袋數  :%7ld  ", CmdStr.c_str(), Cpn74_amt);   tslCrp->push_back(buffer);
   }
   if( Cpn75_amt )
   {
	   // 2019/11/27 Update
	  // sprintf_s(buffer, sizeof(buffer), "%s廢墨水匣袋數:%7ld  ", CmdStr.c_str(), Cpn75_amt);   tslCrp->push_back(buffer);
	   sprintf_s(buffer, sizeof(buffer), "%s廢平板袋數  :%7ld  ", CmdStr.c_str(), Cpn75_amt);   tslCrp->push_back(buffer);
   }
   if (Cpn76_amt)  //Update  20200409
   {
       sprintf_s(buffer, sizeof(buffer), "%s廢行動電源袋數:%5ld  ", CmdStr.c_str(), Cpn76_amt);   tslCrp->push_back(buffer);
   }



   /***
   strtmp.sprintf("                             收銀員簽章");  tslCrp->Add(strtmp);
   strtmp.sprintf("                            ┌────-┐");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            │         │");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            │         │");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            │         │");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            └────-┘");   tslCrp->Add(strtmp);
   ****/
   //sprintf(outfile,"  【此單據請放入投庫袋】");  msg_to_tkt(outfile);
   if (Trim(PrintAutoXsMissRec) != "")
   {
	   //紙本作廢發 明細  2019/04/26
	   sprintf_s(buffer, sizeof(buffer), "--------紙本作廢發票明細----------");  tslCrp->push_back(string(buffer));
	   sprintf_s(buffer, sizeof(buffer), "序  交易序號     發票號碼");  tslCrp->push_back(string(buffer));
	   i2 = 0;
	   while (1)
	   {
		   i2++;
		   tmpSer = _StringSegment_EX(PrintAutoXsMissRec, SGM, i2 - 1);
		   if (Trim(tmpSer) == "" || i2>500) break;
		   sprintf_s(buffer, sizeof(buffer), "%d.%s", i2, tmpSer.c_str());  tslCrp->push_back(string(buffer));

	   }  // end of while
	   sprintf_s(buffer, sizeof(buffer), "(作廢發票明細不包含卡紙重印)");  tslCrp->push_back(string(buffer));

   }

   sprintf_s(buffer, sizeof(buffer), "\n\n\n\n");  tslCrp->push_back(buffer);
   sprintf_s(buffer, sizeof(buffer), "\x1b\x69");  tslCrp->push_back(buffer); //切紙

    //輸出檔案

   //if (FileExists(SCasherRpt.c_str() ))
   if (PathFileExists(StringToWString(SCasherRpt).c_str()))
	   DeleteFile(StringToWString(SCasherRpt).c_str());

   //tslCrp->SaveToFile(SCasherRpt);
   //_ListToFile(tslCrp, SCasherRpt.c_str());  //Ansi File
   _ListToWFile(tslCrp, SCasherRpt);  //Unicode

   //tslCrp->Clear();
   delete tslCrp;
   //delete AutoCrpIni;

   //if( iAutoZ==0 ) //非自動日結
   //     DeleteFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");

   return 0;
}



int __fastcall BASIC::read_ten_name(string &sStoreNo, string &sStoreName)
{
    sStoreNo = g_tencode;
    sStoreName = g_store_name;
    return(0);

 /****************************************

  FILE  *code_file;
  char   tmp_buf[81],tmp_data[31];
  string strtmp;

  sprintf_s(tmp_buf,sizeof(tmp_buf),"C:\\FTLPOS\\DATA\\INI\\Storedata_19.tm");
 
#ifdef WIN32
  errno_t err;
  if ((err = fopen_s(&code_file, tmp_buf, "r")) != 0)
#else
  if ((code_file=fopen(tmp_buf, "r")) == NULL)
#endif
     {

      //memset(gchr_company,' ',10);
      //gchr_ten_code[10]=0;
      logsprintf("read_ten_name:門市基本檔 File Open Error %s ",tmp_buf);
      sStoreNo="999999";
      sStoreName="StNotFind";
      return(0);
     }
  else
     {
      scan_fixstr(code_file, tmp_data, 16);
      tmp_data[16] = 0;

      strncpy_s(tmp_buf,sizeof(tmp_buf),tmp_data,10);
      tmp_buf[10]=0;
      sStoreName=tmp_buf;

	  strncpy_s(tmp_buf, sizeof(tmp_buf), &tmp_data[10], 6);
      tmp_buf[6]=0;
      sStoreNo=tmp_buf;

     }
   fclose(code_file);
   return(0);
 *******************************************/
 
}




////計算2008/12/04 消費券 3240
int __fastcall BASIC::SumBillCSM(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt;
    string str_sale_type, stmp, s;       //結帳型態

    iPayType = iPayID = iTAmt = iTCnt = 0;
    giS=giT=0;

    logsprintf("SumBillCSM:消費券合計金額計算 (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);

    if (gbl_bil_haveline)
    {
        //for (int i=0; i<gtsl_bil->Count; i++)
		for (list<string>::iterator ls = gtsl_bil->begin(); ls != gtsl_bil->end(); ls++)
        {
			s = *ls; // gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, SGM, 8);
            stmp=_StringSegment_EX(s, SGM, 11);
            iPayType=_StrToInt(stmp,"支付類別");

            iTAmt = _StrToInt( _StringSegment_EX(s, SGM, 13), "支付金額" );
            stmp=_StringSegment_EX(s, SGM, 12);  //支付類別序號
            iPayID=_StrToInt(stmp,"支付類別序號");

            if( iPayType==63 && (iPayID==8 || iPayID==9) )
              {
                //stmp.sprintf("SumBillCSM:1040->%s",gtsl_bil->Strings[i] );
                //writelog(stmp);
                if (_StrFind(str_sale_type,"Z0"))
                    {
                     giT+=iTCnt;                  //消費券71次數
                     giS+=iTAmt;                  //消費券71金額
                    }
				else if (_StrFind(str_sale_type, "R2") || _StrFind(str_sale_type,"R3"))
                    {
                      giT-=iTCnt;                  //消費券71次數
                      giS-=iTAmt;                  //消費券71金額
                    }
              }

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)


    logsprintf("SumBillCSM:3240->消費券合計金額 (金額:%d)",giS);
    //writelog(stmp);
    return giS;
}



////計算3240 CPN
int __fastcall BASIC::SumBillCPN(int StartLine, int TotalLine)
{
	int iPayType, iPayID, iTCnt, iTAmt, iRecycleFg;
    string str_sale_type, stmp, s, sPAY_TYPE;       //結帳型態

	iPayType = iPayID = iTAmt = iTCnt = iRecycleFg = 0;
    giU=giV=0;

    logsprintf("SumBillCPN: 非實體券金額折價券合計金額計算 3240 (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);

    if (gbl_bil_haveline)
    {
        //for (int i=0; i<gtsl_bil->Count; i++)
        for (list<string>::iterator ls = gtsl_bil->begin(); ls != gtsl_bil->end(); ls++)
        {
			s = *ls; // gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, SGM, 9);
            sPAY_TYPE = _StringSegment_EX(s, SGM, 10); //支付代號
            stmp=_StringSegment_EX(s, SGM, 11);        //支付類別 PAY_TYPE_ID   
            iPayType=_StrToInt(stmp,"支付類別");
            stmp=_StringSegment_EX(s, SGM, 12);  //支付類別序號 ID_PAYNO
            iPayID=_StrToInt(stmp,"支付類別序號");
            iTAmt = _StrToInt( _StringSegment_EX(s, SGM, 13), "支付金額" );
			iRecycleFg = _StrToInt(_StringSegment_EX(s, SGM, 29), "廢資源回fg_29");  //回收註記

            if( sPAY_TYPE.substr(0,1)=="C" )
            {
                /*stmp.sprintf("SumBillCPN:1040->%s",gtsl_bil->Strings[i] );
                writelog(stmp);*/
				if (_StrFind(str_sale_type,"Z0"))
                    {
					//	if (iPayID == 109 || iPayID == 001 || iPayID == 003 || (iPayID >= 901 && iPayID <= 999) )     //非實體券金額   2017/06/26 追加 001, 003
					 if (iRecycleFg == 6 || iRecycleFg == 7)   //2018/12/12 2020/3/4  目前只針對1040  該欄位=6排除交班短溢收計算，需新增7
						giU +=iTAmt;
                     else
                        giV +=iTAmt;
                    }
				else if (_StrFind(str_sale_type, "R2") || _StrFind(str_sale_type,"R3"))
                    {
					//if (iPayID == 109 || iPayID == 001 || iPayID == 003 || (iPayID >= 901 && iPayID <= 999) )      //非實體券金額  2017/06/26 追加 001, 003
					 if (iRecycleFg == 6 || iRecycleFg == 7)   //2018/12/12  2020/3/4  目前只針對1040  該欄位=6排除交班短溢收計算，需新增7 
						giU -=iTAmt;
                     else
                        giV -=iTAmt;
                    }
            }   // end of if( sPAY_TYPE.SubString(0,1)=="C" )

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)

    logsprintf("SumBillCPN: 非實體券金額折價券合計金額計算 3240 (非實體券金額:%d, 實體券金額:%d)",giU, giV);
    return giU;
}



////即時購合計   即時購與銷轉進值 3110, 3113
int __fastcall BASIC::VdcInvSumdat(int StartLine, int TotalLine)
{

    string str_sale_type;       //銷貨型態
    string str_r_type;          //誤打、退貨型態
    string str_spc_flag,s;        //交易類別
    string VdcStatus,stmp,TaxFlg, sRelType, TmpBarcode2;
    int ifg_spc, tmpQty, tmpAmt, iRelType, iTmpAmt, iVDC99Cnt, iVDC99Amt;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;

    //次數、金額
    giA = 0;
    giB = 0;
    giC = 0;   //(免稅次數)
    giD = 0;   //(免稅金額)
    giE = 0;

    iVDC99Cnt=iVDC99Amt=0;
    //TStringList *t = new TStringList();
    if (gbl_rec_haveline)
    {

      //  for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
              // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
              // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }


            str_sale_type = _StringSegment_EX(s, SGM, seg1);
            TaxFlg = Trim(_StringSegment_EX(s, SGM, seg9));           // 稅別  '0'(免稅) '1'(應稅)

            str_r_type = Trim( _StringSegment_EX(s, SGM, seg11) );     // 交易狀態區分
           

            //A1:正常銷售取消    B1:部門銷售取消   M1:Menu銷售取消
            //S0:特殊交易     S1:特殊交易取消
            tmpQty=0;
            tmpAmt=0;
            if (_StrFind(str_r_type,"X"))
                continue;

			if (str_sale_type == "B0" || str_sale_type == "B1" || str_sale_type == "I0" || str_sale_type == "I1")   // 2012/04/17
               continue;

			str_spc_flag = Trim(_StringSegment_EX(s, SGM, seg2));   //交易類別
            ifg_spc=_StrToInt(str_spc_flag);
            //連線商品狀態
			VdcStatus = _StringSegment_EX(s, SGM, seg10);

            //連線商品區分
            iRelType=0;
			sRelType = Trim(VdcStatus.substr(0, 2));
            iRelType=_StrToInt(sRelType);
            TmpBarcode2="";

            /************    2012/04/01 Update,  2012/04/04 ReCover  ******************/
			if (Trim(VdcStatus.substr(1, 1)) == "0" || Trim(VdcStatus.substr(1, 1)) == "")  //存貨推移FG
                    continue;

            //if ( _StrFind(str_sale_type,"A") && ifg_spc==9 )  //預售商品
            //   { ;; }
            //else if ( ifg_spc==4 || ifg_spc==5 || ifg_spc==6 || ifg_spc==9 )
            //   {     continue;   }

            if (ifg_spc == 4 || ifg_spc == 5 || ifg_spc == 6 || ifg_spc == 9)
               {     continue;   }

            /************             2012/04/01  Update            ******************/


            //if (iRelType==0 || iRelType==3 || iRelType==4 )
            //    continue;


            tmpQty= _StrToInt(_StringSegment_EX(s, SGM, seg4));      //數量;
            tmpAmt= _StrToInt(_StringSegment_EX(s, SGM, seg5));      //金額
            /*****   
            if ( iRelType==11 ) // 租賃
               {
                 if( str_sale_type=="A0" || str_sale_type=="A1" )
                     {
                        TmpBarcode2=_StringSegment_EX(s, "|", 14);  //CD_BARCODE2
                        if(TmpBarcode2.substr(14,2)=="02") //繯片
                          {
                           iTmpAmt=_StrToInt(_StringSegment_EX(s, "|", 25));  // 單價 AM_NOWSALE
                           tmpAmt=iTmpAmt * tmpQty;      

                           //stmp.sprintf("VdcInvSumdat:即時購與銷轉進環片:%s->金額:+%d",s.c_str(),tmpAmt);
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
             ********/   

      	   if ( _StrFind(str_sale_type,"0") )
            {

				if (str_r_type == "FF" || str_r_type=="Z0" || str_r_type=="Z1" )
                {
                    
                    if (TaxFlg == "0")
                    {
                        giC += tmpQty;
                        giD += tmpAmt;
                    }
                    else
                    {
                        giA += tmpQty;
                        giB += tmpAmt;
                    }

                    if (iRelType == 99)
                    {
                        iVDC99Cnt += tmpQty;
                        iVDC99Amt += tmpAmt;
                    }
                                    
                  //logsprintf("VdcInvSumdat:即時購與銷轉進值:%s->金額:+%d, Acc=%d ... VDC99(%d,%d)", s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt );

                }
               else if (str_r_type=="R2" || str_r_type=="R3" )
                {
                    if( TaxFlg == "0" )
                     {
                       giC -= tmpQty;   //(免稅次數)
                       giD -= tmpAmt;   //(免稅金額)
                     }
                    else
                     {
                       giA -= tmpQty;       //(應稅次數)
                       giB -= tmpAmt;       //(應稅金額)
                     }

                    if (iRelType == 99)
                     {
                       iVDC99Cnt-=tmpQty;
                       iVDC99Amt-=tmpAmt;
                     }

                   //logsprintf("VdcInvSumdat:即時購與銷轉進值:%s->金額:-%d, Acc=%d ... VDC99(%d,%d)", s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt);

                }
                              
            }    //if (str_sale_type.Pos("1"))
           else  //1    更正
            {
				if (str_r_type=="FF" || str_r_type=="Z0" || str_r_type=="Z1" )
                {
                   if( TaxFlg == "0" )
                     {
                       giC -= tmpQty;    //(免稅次數)
                       giD -= tmpAmt;   //(免稅金額)
                     }
                   else
                     {
                       giA -= tmpQty;      //(應稅次數)
                       giB -= tmpAmt;      //(應稅金額)
                     }


                   if (iRelType==99)
                     {
                       iVDC99Cnt-=tmpQty;
                       iVDC99Amt-=tmpAmt;
                     }

                  //logsprintf("VdcInvSumdat:即時購與銷轉進值:%s->金額:-%d, Acc=%d ... VDC99(%d,%d)",
                  //              s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt);
                  
                }
				else if (str_r_type=="R2" || str_r_type=="R3")
                {
                    if( TaxFlg=="0" )
                     {
                       giC += tmpQty;   //(免稅次數)
                       giD += tmpAmt;   //(免稅金額)
                     }
                    else
                     {
                       giA += tmpQty;       //(應稅次數)
                       giB += tmpAmt;       //(應稅金額)
                     }
                   if (iRelType==99)
                     {
                       iVDC99Cnt+=tmpQty;
                       iVDC99Amt+=tmpAmt;
                     }

                  // logsprintf("VdcInvSumdat:即時購與銷轉進值:%s->金額:+%d, Acc=%d ... VDC99(%d,%d)", s.c_str(),tmpAmt, giB+giD, iVDC99Cnt, iVDC99Amt );
                   //t->Add(gtsl_rec->Strings[i]);
                }

            }  // end of else  0001

        }
    }

    giE=giB+giD;

    logsprintf("VdcInvSumdat:1010->即時購與銷轉進值(不含03/04) (應稅次數/金額:%d, %d), (免稅次數/金額:%d, %d), ... VDC99Acc(%d,%d)  TotAmt=%d",
                                     giA, giB, giC, giD, iVDC99Cnt, iVDC99Amt, giB+giD );
    //t->SaveToFile("e:\\a.txt");  _ListToFile(tslCrp, SCasherRpt.c_str());
    //delete t;
    return 0;
}




//廢資源回收數 /提貨券金額/提貨券張數
void __fastcall BASIC::Sum040RecycleDataCnt(int StartLine, int TotalLine)
{

    int iTCnt, iTAmt, iT4, iPayType,iOtCnt,iOtAmt, iRecyClePackCnt,iRecycleFg;

    string str_sale_type,stmp,s, sPayType ;       //結帳型態

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

    giE = 0;    //提貨券金額
    giF = 0;    //提貨券張數

	// 2018/12/07 Lu Update
	int iPaCpnAmt = 0;         //紙本折價券金額
	int iPaCpnCnt = 0;         //紙本折價券張數
	int iPaPresentAmt = 0;    //紙本禮券張數
	int iPaPresentCnt = 0;    //紙本禮券金額

	giM = iPaCpnAmt;         //紙本折價券金額
	giN = iPaCpnCnt;         //紙本折價券張數
	giO = iPaPresentAmt;         //紙本折價券金額
	giP = iPaPresentCnt;         //紙本折價券張數


   // writelog("刪除檔案 " + str_tmp_ini);
    if (gbl_bil_haveline)
    {
        //for (int i=0; i<gtsl_bil->Count; i++)
		for (list<string>::iterator ls = gtsl_bil->begin(); ls != gtsl_bil->end(); ls++)
        {
            iRecyClePackCnt=0;
            iRecycleFg=0;
			s = *ls; // gtsl_bil->Strings[i];;
            str_sale_type = _StringSegment_EX(s, "|", 8);
            sPayType=_StringSegment_EX(s, "|", 10);   //支付代號

            stmp=_StringSegment_EX(s, "|", 11);
            iPayType=_StrToInt(stmp,"支付類別");
            iTAmt = _StrToInt(_StringSegment_EX(s, "|", 13), "支付金額");

            iRecycleFg=_StrToInt(_StringSegment_EX(s, "|", 29), "廢資源回fg_29");
            iRecyClePackCnt=_StrToInt(_StringSegment_EX(s, "|", 30), "廢資源回收數_30");

           // if (iTAmt)
             iTCnt=1;
          //  writelog("SumBill78: Bil Data str_sale_type,PayType ,"+ str_sale_type + stmp);
            if (_StrFind(str_sale_type,"Z0"))
            {

                 if( sPayType  == "E0" ) //提貨券
                   {
                      giE += iTAmt;    //提貨券金額
                      giF ++;          //提貨券張數
                   }

				 // 2018/12/07 Lu Update
				 if ((sPayType == "C0" || sPayType == "C9") && iRecycleFg != 6) //  紙本折價券
				 {
					 iPaCpnAmt += iTAmt;    //紙本折價券金額
					 iPaCpnCnt++;          //紙本折價券張數
				 }

				 // 2018/12/07 Lu Update
				 if ((sPayType == "B1" || sPayType == "B2")) //  紙本禮券
				 {
					 iPaPresentAmt += iTAmt;    //紙本禮券金額
					 iPaPresentCnt++;          //紙本禮券張數
				 }


                 switch(iRecycleFg)
                       {
                         //case 71:
                         //      Cpn71_cnt+=iTCnt;
                         //      Cpn71_amt+=iTAmt;
                         //     break;
                         case 1:   //廢電池
                               //Cpn71_cnt+=iTCnt;
                               Cpn71_amt+=iRecyClePackCnt;
                              break;
                         case 2:   //廢收機
                               //Cpn72_cnt+=iTCnt;
                               Cpn72_amt+=iRecyClePackCnt;
                              break;
                         case 3:   //廢筆電
                               //Cpn73_cnt+=iTCnt;
                               Cpn73_amt+=iRecyClePackCnt;
                              break;
                         case 4:   //廢光碟
                               //Cpn74_cnt+=iTCnt;
                               Cpn74_amt+=iRecyClePackCnt;
                              break;
                         case 5:   //廢墨水匣
                               //Cpn75_cnt+=iTCnt;;
                               Cpn75_amt+=iRecyClePackCnt;
                              break;
                         case 8:  //廢行動電源  20200409 Lu
                             //Cpn75_cnt+=iTCnt;;
                             Cpn76_amt += iRecyClePackCnt;
                             break;
                         default:
                               //Cpn71_cnt+=iTCnt;
                               //Cpn71_amt+=iRecyClePackCnt;
                               break;

                       }     // END OF SWITCH
              } // end of  if (str_sale_type.Pos("Z0"))

			else if (_StrFind(str_sale_type, "R2") || _StrFind(str_sale_type,"R3"))
            {
                  if( sPayType  == "E0" ) //提貨券
                   {
                      giE -= iTAmt;    //提貨券金額
                      giF --;          //提貨券張數
                   }

				  // 2018/12/07 Lu Update
				  if ((sPayType == "C0" || sPayType == "C9") && iRecycleFg != 6) //  紙本折價券
				  {
					  iPaCpnAmt -= iTAmt;    //紙本折價券金額
					  iPaCpnCnt--;          //紙本折價券張數
				  }

				  // 2018/12/07 Lu Update
				  if ((sPayType == "B1" || sPayType == "B2")) //  紙本禮券
				  {
					  iPaPresentAmt -= iTAmt;    //紙本禮券金額
					  iPaPresentCnt--;          //紙本禮券張數
				  }


                 switch(iPayType)
                       {
                         //case 71:
                         //      Cpn71_cnt-=iTCnt;                  //折價券71次數
                         //      Cpn71_amt-=iTAmt;                  //折價券71金額
                         //     break;
                         case 1:
                               //Cpn71_cnt-=iTCnt;                  //折價券71次數
                               Cpn71_amt-=iRecyClePackCnt;                  //折價券71金額
                              break;
                         case 2:
                               //Cpn72_cnt-=iTCnt;                  //折價券71次數
                               Cpn72_amt-=iRecyClePackCnt;                  //折價券71金額
                              break;
                         case 3:
                               //Cpn73_cnt-=iTCnt;                  //折價券71次數
                               Cpn73_amt-=iRecyClePackCnt;                  //折價券71金額
                              break;
                         case 4:
                               //Cpn74_cnt-=iTCnt;                  //折價券71次數
                               Cpn74_amt-=iRecyClePackCnt;                  //折價券71金額
                              break;
                         case 5:
                               //Cpn75_cnt-=iTCnt;;                  //折價券71次數
                               Cpn75_amt-=iRecyClePackCnt;                  //折價券71金額
                              break;
                         case 8:  //廢行動電源  20200409 Lu
                               //Cpn75_cnt+=iTCnt;;
                             Cpn76_amt -= iRecyClePackCnt;
                             break;
                         default:
                              //Cpn71_cnt-=iTCnt;                  //折價券71次數
                              //Cpn71_amt-=iRecyClePackCnt;                  //折價券71金額
                              break;

                       }     // END OF SWITCH
            }   // end of  else if ( str_sale_type.Pos("R2") || str_sale_type.Pos("R3") )
        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)


	giM = iPaCpnAmt;         //紙本折價券金額
	giN = iPaCpnCnt;         //紙本折價券張數
	giO = iPaPresentAmt;         //紙本折價券金額
	giP = iPaPresentCnt;         //紙本折價券張數

	char  buffer[10240];
	sprintf_s(buffer, sizeof(buffer),"%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|",
                       Cpn71_cnt,Cpn71_amt,Cpn72_cnt,Cpn72_amt,Cpn73_cnt,Cpn73_amt,
                       Cpn74_cnt,Cpn74_amt,Cpn75_cnt,Cpn75_amt,Cpn76_cnt,Cpn76_amt );
	StrBillPay = string(buffer);

    return ;
}



////計算電子禮券禮物卡轉餘額券  3240
int __fastcall BASIC::SumECP2Orher2Amt(int StartLine, int TotalLine)
{
    int iPayType, iPayID, iTCnt, iTAmt, iOt2Amt;
    string str_sale_type, stmp, s, sPayType, PrcCode;       //結帳型態

    iPayType = iPayID = iTAmt = iTCnt = 0;
    giA=giB=iOt2Amt=0;

    logsprintf("SumECP2Orher2Amt:計算禮物卡轉餘額券 (StartLine:%d,TotalLine:%d)",StartLine, TotalLine);

    if (gbl_bil_haveline)
    {
       // for (int i=0; i<gtsl_bil->Count; i++)
		for (list<string>::iterator ls = gtsl_bil->begin(); ls != gtsl_bil->end(); ls++)
        {
			s = *ls; // gtsl_bil->Strings[i];
            str_sale_type = _StringSegment_EX(s, SGM, 9);     //結帳型態
            sPayType=_StringSegment_EX(s, "|", 11);    //B3
            PrcCode=_StringSegment_EX(s, "|", 16);     //MSG_TYPE 1Q04 

            stmp=_StringSegment_EX(s, "|", 11);        //PAY_TYPE_ID
            iPayType=_StrToInt(stmp,"支付類別");

            iTAmt = _StrToInt( _StringSegment_EX(s, "|", 13), "支付金額" );
            iOt2Amt=_StrToInt( _StringSegment_EX(s, "|", 32), "轉餘額券" );


            stmp=_StringSegment_EX(s, "|", 12);  //支付類別序號
            iPayID=_StrToInt(stmp,"支付類別序號");

            //if( str_sale_type=="R2" || str_sale_type=="R3" )
            //   continue;

            if( sPayType=="B3" && ( PrcCode=="1X00" || PrcCode=="1Q04" ) )   //"1Q04"
               {
                 giA+=iOt2Amt;
                 logsprintf("SumECP2Orher2Amt:1040->(禮物卡轉餘額券金額:%s)",s.c_str() );
               }

        }  // end of  for (int i=StartLine; i<=TotalLine; i++)
    }      // end of if (gbl_bil_haveline)

    //giA=iOt2Amt;
    logsprintf("SumECP2Orher2Amt:1040->(禮物卡轉餘額券金額:%d)",giA);
    return giA;
}

/*********************
// Sub_C0InvoAmt4VDC NextPOS 合併 Sub_C0InvoAmt   A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
//// 外加手續費,即時購代售點卡集計 如要扣攤提參部門帳  3110
void __fastcall BASIC::Sub_C0InvoAmt(int StartLine, int TotalLine)
{
    //外加手續費,即時購代售點卡金額次數統計

    string str_is_rtype, str_trans_flag,Stmp, s;        //銷售形態，交易形態
    int i_trans_num = 0;                            //銷售數量

    //0:一般商品    1:代收   2:代售   3:代付   4:加值機(開,售)卡   5:加值機(加值)

    int i_trans_flag = 0;                          //交易型態
    int iRelType, i_type;

    //外加手續費, 即時購代售
    giA=giB=giC=giD=giE=giF=giG=0;

    string str_good_no, str_menu_no, str_bar_name, str_item_amt;
	string str_type, str_item_qty, strRelType, strCD_FMCODE;
    int seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;

    if (gbl_rec_haveline)
    {
       // for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; 

            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1))%1000;

            switch (i_type)
            {  
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
                // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
                // seg11.交易狀態區分:43 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34, seg9 = 37, seg10 = 38, seg11 = 43;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;
               
            }

            str_type = _StringSegment_EX(s, SGM, seg1);                  //銷貨型態 
            str_trans_flag = _StringSegment_EX(s, SGM, seg2);           //代售付區分   SPC_FLG  交易類別
            i_trans_flag = _StrToInt(str_trans_flag);                   //SPC_FLG  交易類別
            
            str_menu_no = "0000";
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //代收售,付類別
            //str_bar_name = Trim(_StringSegment_EX(s, SGM, 19));

            str_good_no = Trim(_StringSegment_EX(s, SGM, seg3));                 //CD_FMCODE
            i_trans_num = _StrToInt(_StringSegment_EX(s, SGM, seg4));  //銷售數量
            //str_item_qty = int2str(i_trans_num);
            str_item_amt = Trim(_StringSegment_EX(s, SGM, seg5));;     //AM_ITEM

             
            strRelType = _StringSegment_EX(s, SGM, seg10).substr(0, 2);   // 連線商品狀態  農會商品=17
            iRelType = _StrToInt(strRelType);

            str_is_rtype = _StringSegment_EX(s, SGM, seg11).substr(0,2);   // 交易狀態區分
            

#pragma region   農會商品 CD_FMCODE => M開頭7碼 
			if (str_good_no.substr(0, 1) == "M")  //農會商品 CD_FMCODE => M開頭7碼
			{
				if (_StrFind(str_type, "A0"))
				{
					str_type = "C0";
					i_trans_flag = 2;    //SPC_FLG  交易類別
				}
				else if (_StrFind(str_type, "A1"))
				{
					str_type = "C1";
					i_trans_flag = 2;    //SPC_FLG  交易類別
				}
			}
#pragma endregion

            // TaxFlg=_StringSegment_EX(s, "|", 35);           // 稅別  '0'(免稅) '1'(應稅)

			if (_StrFind(str_is_rtype, "FF") || _StrFind(str_is_rtype, "Z0") || _StrFind(str_is_rtype,"Z1") || Trim(str_is_rtype) == "")
            {
				if (_StrFind(str_type,"C0"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //外加手續費
                             giA+=_StrToInt(str_item_amt);   //金額
                             giB+=i_trans_num;               //次數
                             break;
                        case 2:     //即時購代售點卡
						case 14:    //2019/05/21 Add
                             giC+=_StrToInt(str_item_amt);   //金額
                             giD+=i_trans_num;               //次數
                             break;

                    }
                }
				else if (_StrFind(str_type,"C1"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //外加手續費
                             giA-=_StrToInt(str_item_amt);   //金額
                             giB-=i_trans_num;               //次數
                             break;
                        case 2:     //即時購代售點卡
						case 14:    //2019/05/21 Add
                             giC-=_StrToInt(str_item_amt);   //金額
                             giD-=i_trans_num;               //次數
                             break;
                    }
                }
            }
			else if (_StrFind(str_is_rtype, "R2") || _StrFind(str_is_rtype,"R3"))
            {
				if (_StrFind(str_type,"C0"))
                {
                    switch (i_trans_flag)
                    {
                        case 1:     //外加手續費
                             giA-=_StrToInt(str_item_amt);   //金額
                             giB-=i_trans_num;               //次數
                             break;
                        case 2:     //即時購代售點卡
						case 14:    //2019/05/21 Add
                             giC-=_StrToInt(str_item_amt);   //金額
                             giD-=i_trans_num;               //次數
                             break;
                    }
                }
				else if (_StrFind(str_type,"C1"))
                {
                    switch (i_trans_flag)
                    {
                       case 1:     //外加手續費
                             giA+=_StrToInt(str_item_amt);   //金額
                             giB+=i_trans_num;               //次數
                             break;
                        case 2:     //即時購代售點卡
						case 14:    //2019/05/21 Add
                             giC+=_StrToInt(str_item_amt);   //金額
                             giD+=i_trans_num;               //次數
                             break;
                    }
                }
            }
        }
    }
    
    logsprintf("Sub_C0InvoAmt C0C1: 外加手續費金次數(%d)(%d), 即時購代售點卡(%d)(%d)", giA, giB, giC, giD );
  
}
**************************/


//// 點卡手續費應免稅
void __fastcall BASIC::Sub_C0C1TaxInvoAmt(int StartLine, int TotalLine)
{
    //外加手續費,即時購代售點卡金額次數統計

    string str_is_rtype, str_trans_flag,Stmp, TaxFlg, s;        //銷售形態，交易形態

    //0:一般商品    1:代收   2:代售   3:代付   4:加值機(開,售)卡   5:加值機(加值)

    int i_trans_flag = 0;                          //交易型態
    int iRelType, i_type;
    int iItemAmt,iDisSubAmt,iSubSubAmt,ifoodSubSubAmt,iItemQty,iNetItemAmt;
    int seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
    //外加手續費
    giA = giB = giC = giD = giE = giF = 0;
    giG=giH=giI=0;

    string str_good_no, str_menu_no, str_bar_name;
    string str_type, strRelType;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; //gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;

            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
                // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
                // seg11.交易狀態區分:43 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34, 
                seg9 = 37, seg10=38, seg11 = 43;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35; 
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            str_type = _StringSegment_EX(s, SGM, seg1);                  //銷貨型態 
            str_trans_flag = _StringSegment_EX(s, SGM, seg2);            //代售付區分   SPC_FLG  交易類別
            i_trans_flag = _StrToInt(str_trans_flag);                    //SPC_FLG  交易類別

            str_menu_no = "0000";
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //代收售,付類別
            //str_bar_name = Trim(_StringSegment_EX(s, SGM, 19));

            str_good_no = Trim(_StringSegment_EX(s, SGM, seg3));                 //CD_FMCODE
            iItemQty = _StrToInt(_StringSegment_EX(s, SGM, seg4));      //銷售數量
            iItemAmt = _StrToInt(_StringSegment_EX(s, SGM, seg5));;     //AM_ITEM
            iDisSubAmt = _StrToInt(_StringSegment_EX(s, SGM, seg6));      //AM_DIS_SUB MM  折扣 攤題
            iSubSubAmt = _StrToInt(_StringSegment_EX(s, SGM, seg7));      //小計折扣 攤題
            ifoodSubSubAmt = _StrToInt(_StringSegment_EX(s, SGM, seg8));  //食物券折扣 攤題
            iNetItemAmt = iItemAmt - iDisSubAmt - iSubSubAmt - ifoodSubSubAmt;

            TaxFlg = _StringSegment_EX(s, SGM, seg9);           // 稅別  '0'(免稅) '1'(應稅)

            strRelType = _StringSegment_EX(s, SGM, seg10).substr(0, 2);   // 連線商品狀態  農會商品=17
            iRelType = _StrToInt(strRelType);

            str_is_rtype = _StringSegment_EX(s, SGM, seg11).substr(0, 2);   // 交易狀態區分
          

#pragma region   農會商品 CD_FMCODE => M開頭7碼 
			if (str_good_no.substr(0, 1) == "M")  //農會商品 CD_FMCODE => M開頭7碼
			{
				if (_StrFind(str_type, "A0"))
				{
					str_type = "C0";
					i_trans_flag = 2;    //SPC_FLG  交易類別
				}
				else if (_StrFind(str_type, "A1"))
				{
					str_type = "C1";
					i_trans_flag = 2;    //SPC_FLG  交易類別
				}
			}
#pragma endregion

             //不含 R4
			if (_StrFind(str_is_rtype, "FF") || _StrFind(str_is_rtype,"Z0") || _StrFind(str_is_rtype,"Z1") || Trim(str_is_rtype)=="" )
            {
				if (_StrFind(str_type,"C0"))
                {
                    giG += iItemQty;               //次數

                   if( Trim(TaxFlg)=="0" )
                    {
                        giA+=iNetItemAmt;   //金額   (免稅)
                        //giB+=i_trans_num;               //銷售數量
                    }
                   else
                    {
                        giB+=iNetItemAmt;   //金額 (應稅)
                        //giD+=i_trans_num;               //銷售數量
                    }
                }
				else if (_StrFind(str_type,"C1"))
                {
                    giG -= iItemQty;               //次數
                    if( Trim(TaxFlg)=="0" )
                    {
                        giA-=iNetItemAmt;   //金額   (免稅)
                        //giB+=i_trans_num;               //次數
                    }
                   else
                    {
                        giB-=iNetItemAmt;   //金額 (應稅)
                        //giD+=i_trans_num;               //次數
                    }
                }
            }
			else if (_StrFind(str_is_rtype, "R2") || _StrFind(str_is_rtype,"R3"))
            {
                giH += iItemQty;
				if (_StrFind(str_type,"C0"))
                {
                   if( Trim(TaxFlg)=="0" )
                    {
                        giC+=iNetItemAmt;   //金額 (免稅)
                        //giB+=i_trans_num;               //次數
                    }
                   else
                    {
                        giD+=iNetItemAmt;   //金額 (應稅)
                        //giD+=i_trans_num;               //次數
                    }
                }
				else if (_StrFind(str_type,"C1"))
                {
                    giH -= iItemQty;
                    if( Trim(TaxFlg)=="0" )
                    {
                        giC-=iNetItemAmt;   //金額   (免稅)
                        //giB+=i_trans_num;               //次數
                    }
                   else
                    {
                        giD-=iNetItemAmt;   //金額 (應稅)
                        //giD+=i_trans_num;               //次數
                    }
                }

            }
			else if (_StrFind(str_is_rtype,"R4"))
            {
				if (_StrFind(str_type,"C0"))
                {
                   if( Trim(TaxFlg)=="0" )
                    {
                        giE+=iNetItemAmt;   //金額 (免稅)

                    }
                   else
                    {
                        giF+=iNetItemAmt;   //金額 (應稅)

                    }
                }
				else if (_StrFind(str_type,"C1"))
                {
                    if( Trim(TaxFlg)=="0" )
                    {
                        giE-=iNetItemAmt;   //金額   (免稅)

                    }
                   else
                    {
                        giF-=iNetItemAmt;   //金額 (應稅)

                    }
                }
            }

        }
    }

    giI = giG - giH;
    logsprintf("Sub_C0C1TaxInvoAmt C0C1: 點卡手續費(免稅:%d)(應稅%d), 誤退(免稅:%d)(應稅:%d), 重印(免稅:%d),(應稅:%d), 點卡手續費數量(%d), 誤退(%d) Tot=%d" , 
                giA, giB, giC, giD, giE, giF, giG, giH, giI);

}


/******
// 點卡手續費數量  Sub_C0C1TaxInvoQty(0, gi_rec_line) ==> 併入 Sub_C0C1TaxInvoAmt
void __fastcall BASIC::Sub_C0C1TaxInvoQty(int StartLine, int TotalLine)
{
    //外加手續費,即時購代售點卡金額次數統計

    string str_is_rtype, str_trans_flag,Stmp, TaxFlg, s;        //銷售形態，交易形態
    int i_trans_num = 0;                            //銷售數量

    //0:一般商品    1:代收   2:代售   3:代付   4:加值機(開,售)卡   5:加值機(加值)

    int i_trans_flag = 0;                          //交易型態
    int iRelType, i_type;
    int seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;

    //外加手續費
    giA=giB=giC=giD=giE=giF=giG=0;

    string str_good_no, str_menu_no, str_bar_name, str_item_amt;
    string str_type, str_item_qty, strRelType;

    if (gbl_rec_haveline)
    {
        //for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];
            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
  
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, seg6.FG_TAX:37,  seg7..連線商品狀態:38, seg8.交易狀態區分:43 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 37, seg7 = 38, seg8 = 43; // , seg9, seg10;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.FG_TAX:38,  seg7..連線商品狀態:19, seg8.交易狀態區分:56 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 38, seg7 = 19, seg8 = 56; // , seg9, seg10;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            str_type = _StringSegment_EX(s, SGM, seg1);                  //銷貨型態

            str_trans_flag = _StringSegment_EX(s, SGM, seg2);           //代售付區分 SPC_FLG  交易類別
            i_trans_flag = _StrToInt(str_trans_flag);                   //SPC_FLG  交易類別

#pragma region   農會商品 CD_FMCODE => M開頭7碼 
            str_good_no = Trim(_StringSegment_EX(s, SGM, seg3));
            if (str_good_no.substr(0, 1) == "M")  //農會商品 CD_FMCODE => M開頭7碼
            {
                if (_StrFind(str_type, "A0"))
                {
                    str_type = "C0";
                    i_trans_flag = 2;    //SPC_FLG  交易類別
                }
                else if (_StrFind(str_type, "A1"))
                {
                    str_type = "C1";
                    i_trans_flag = 2;    //SPC_FLG  交易類別
                }
            }

            str_menu_no = "0000";
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //代收售,付類別
            //str_bar_name = Trim(_StringSegment_EX(s, "|", 19));  //NM_PLU

#pragma endregion

            i_trans_num = _StrToInt(_StringSegment_EX(s, SGM, seg4));  //銷售數量
            str_item_qty = int2str(i_trans_num);

            str_item_amt = Trim(_StringSegment_EX(s, SGM, seg5));  //AM_ITEM

            TaxFlg = _StringSegment_EX(s, SGM, seg6);           // 稅別  '0'(免稅) '1'(應稅)

            //連線商品區分
            strRelType = _StringSegment_EX(s, SGM, seg7);      // 連線商品狀態
            iRelType = _StrToInt(strRelType);

            str_is_rtype = _StringSegment_EX(s, SGM, seg8).substr(0, 2);   // 交易狀態區分
      

            //不含 R4
			if (_StrFind(str_is_rtype, "FF") || _StrFind(str_is_rtype, "Z0") || _StrFind(str_is_rtype,"Z1") || Trim(str_is_rtype) == "")
            {
				if (_StrFind(str_type,"C0"))
                {
                   //if( Trim(TaxFlg)=="0" )
                   // {
                        //giA+=_StrToInt(str_item_amt);   //金額   (免稅)
                        giA+=i_trans_num;               //次數
                   // }

                }
				else if (_StrFind(str_type,"C1"))
                {
                   // if( Trim(TaxFlg)=="0" )
                   // {
                        //giA-=_StrToInt(str_item_amt);   //金額   (免稅)
                        giA-=i_trans_num;               //次數
                   // }

                }
            }
			else if (_StrFind(str_is_rtype, "R2") || _StrFind(str_is_rtype,"R3"))
            {

				if (_StrFind(str_type,"C0"))
                {
                   //if( Trim(TaxFlg)=="0" )
                   // {
                        //giB+=_StrToInt(str_item_amt);   //金額 (免稅)
                        giB+=i_trans_num;               //次數
                   // }

                }
				else if (_StrFind(str_type,"C1"))
                {
                   // if( Trim(TaxFlg)=="0" )
                   // {
                        //giB-=_StrToInt(str_item_amt);   //金額   (免稅)
                        giB-=i_trans_num;               //次數
                   // }

                }

            }
        }
    }

    giC=giA-giB;
    logsprintf("Sub_C0C1TaxInvoQty C0C1: 點卡手續費數量(%d), 誤退(%d) Tot=%d", giA, giB, giC);

}
*************/



/// <summary>
///  外加手續費,即時購代售點卡集計 以連線區分分類  3110, 3113
///  
/// </summary>
/// <param name1="StartLine",  name2="TotalLine" ></param>
/// <returns></returns>
void __fastcall BASIC::Sub_C0InvoAmt4VDC(int StartLine, int TotalLine)
{
    //外加手續費,即時購代售點卡金額次數統計

    string str_is_rtype, str_trans_flag,Stmp, s;        //銷售形態，交易形態
    int i_trans_num = 0;                            //銷售數量

    //0:一般商品    1:代收   2:代售   3:代付   4:加值機(開,售)卡   5:加值機(加值)

    int i_trans_flag = 0;                          //交易型態
    int iRelType;

    //外加手續費, 即時購代售
    giA = giB = giC = giD = giE = giF = giG = 0;
    giH = giI = giJ = giK = giL = giM = giN = 0;

    string str_good_no, str_menu_no, str_bar_name, str_item_amt;
    string str_type, str_item_qty, strRelType;
    int i_type, seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11, segDONGLE, segNam;

    if (gbl_rec_haveline)
    {
       // for (int i=0; i<gtsl_rec->Count; i++)
		for (list<string>::iterator ls = gtsl_rec->begin(); ls != gtsl_rec->end(); ls++)
        {
			s = *ls; // gtsl_rec->Strings[i];

            i_type = _StrToInt(_StringSegment_EX(s, SGM, 1)) % 1000;
            switch (i_type)
            {
            case 110:
                // seg1.銷貨型態:9, seg2.FG_SPC:10, seg3.CD_FMCODE:17, seg4.銷售數量:28, seg5.AM_ITEM:31, 
                // seg6.AM_DIS_SUB:32, seg7.AM_SUB_SUB:33, seg8.AM_PAY_SUB:34, seg9.FG_TAX:37  seg10.連線商品狀態:38, 
                // seg11.交易狀態區分:43 ,  segDONGLE.DONGLE_TYPE:11, segNam.NM_PLU:19
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 28, seg5 = 31, seg6 = 32, seg7 = 33, seg8 = 34,
                    seg9 = 37, seg10 = 38, seg11 = 43;
                segDONGLE = 11, segNam = 19;
                break;
            case 113:
                // seg1.銷貨型態:9, seg2.NO_REPCNT:10, seg3.CD_FMCODE:17, seg4.銷售數量:29, seg5.AM_ITEM:32, seg6.AM_DIS_SUB:33,  
                // seg7.AM_SUB_SUB:34,  seg8.AM_PAY_SUB:35, seg9.FG_TAX:38, seg10.連線商品狀態:19 , seg11.交易狀態區分:82 
                seg1 = 9, seg2 = 10, seg3 = 17, seg4 = 29, seg5 = 32, seg6 = 33, seg7 = 34, seg8 = 35;
                seg9 = 38, seg10 = 19, seg11 = 82;
                break;
            case 120:
                //seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8, seg9, seg10, seg11;
                continue;
            default:
                continue;

            }

            str_type = _StringSegment_EX(s, SGM, seg1);                  //銷貨型態
            str_trans_flag = _StringSegment_EX(s, SGM, seg2); //代售付區分       //SPC_FLG  交易類別
            i_trans_flag = _StrToInt(str_trans_flag);       //SPC_FLG  交易類別

            str_good_no = Trim(_StringSegment_EX(s, SGM, seg3));
            //str_menu_no = gtsl_rec->Strings[i].SubString(128,4).Trim();      //代收售,付類別
            str_menu_no = "0000";

            str_bar_name = Trim(_StringSegment_EX(s, SGM, segNam));
            i_trans_num = _StrToInt(_StringSegment_EX(s, SGM, seg4));  //銷售數量
            str_item_qty = int2str(i_trans_num);

            str_item_amt = Trim(_StringSegment_EX(s, SGM, seg5));

             //連線商品區分
            strRelType = _StringSegment_EX(s, SGM, seg10).substr(0, 2);   // 連線商品狀態
            iRelType = _StrToInt(strRelType);

            str_is_rtype = _StringSegment_EX(s, SGM, seg11).substr(0,2);   // 交易狀態區分

           

            // TaxFlg=_StringSegment_EX(s, "|", 35);           // 稅別  '0'(免稅) '1'(應稅)

//#pragma region   農會商品 CD_FMCODE => M開頭7碼    
//			if (str_good_no.substr(0, 1) == "M")  //農會商品 CD_FMCODE => M開頭7碼
//			{
//				if (_StrFind(str_type, "A0"))
//				{
//					str_type = "C0";
//					i_trans_flag = 2;    //SPC_FLG  交易類別
//				}
//				else if (_StrFind(str_type, "A1"))
//				{
//					str_type = "C1";
//					i_trans_flag = 2;    //SPC_FLG  交易類別
//				}
//			}
//#pragma endregion

			if (_StrFind(str_is_rtype, "FF") || _StrFind(str_is_rtype, "Z0") || _StrFind(str_is_rtype,"Z1") || Trim(str_is_rtype) == "")
            {
				if (_StrFind(str_type,"C0"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA+=_StrToInt(str_item_amt);   //金額
                             giB+=i_trans_num;               //次數
                             break;
                        default:
                             giC+=_StrToInt(str_item_amt);   //金額
                             giD+=i_trans_num;               //次數
                             break;

                    }

                    //外加手續費,即時購代售點卡集計 如要扣攤提參部門帳
                    // A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
                    switch (i_trans_flag)
                    {
                    case 1:     //外加手續費
                        giH += _StrToInt(str_item_amt);   //金額
                        giI += i_trans_num;               //次數
                        break;
                    case 2:     //即時購代售點卡
                    case 14:    //2019/05/21 Add
                        giJ += _StrToInt(str_item_amt);   //金額
                        giK += i_trans_num;               //次數
                        break;

                    }
                }
				else if (_StrFind(str_type,"C1"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA-=_StrToInt(str_item_amt);   //金額
                             giB-=i_trans_num;               //次數
                             break;
                        default:
                             giC-=_StrToInt(str_item_amt);   //金額
                             giD-=i_trans_num;               //次數
                             break;
                    }

                    //外加手續費,即時購代售點卡集計 如要扣攤提參部門帳
                    // A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
                    switch (i_trans_flag)
                    {
                    case 1:     //外加手續費
                        giH -= _StrToInt(str_item_amt);   //金額
                        giI -= i_trans_num;               //次數
                        break;
                    case 2:     //即時購代售點卡
                    case 14:    //2019/05/21 Add
                        giJ -= _StrToInt(str_item_amt);   //金額
                        giK -= i_trans_num;               //次數
                        break;

                    }


                }
            }
			else if (_StrFind(str_is_rtype, "R2") || _StrFind(str_is_rtype, "R3"))
            {
				if (_StrFind(str_type,"C0"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA-=_StrToInt(str_item_amt);   //金額
                             giB-=i_trans_num;               //次數
                             break;
                        default:
                             giC-=_StrToInt(str_item_amt);   //金額
                             giD-=i_trans_num;               //次數
                             break;
                    }

                    //外加手續費,即時購代售點卡集計 如要扣攤提參部門帳
                    // A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
                    switch (i_trans_flag)
                    {
                    case 1:     //外加手續費
                        giH -= _StrToInt(str_item_amt);   //金額
                        giI -= i_trans_num;               //次數
                        break;
                    case 2:     //即時購代售點卡
                    case 14:    //2019/05/21 Add
                        giJ -= _StrToInt(str_item_amt);   //金額
                        giK -= i_trans_num;               //次數
                        break;

                    }


                }
				else if (_StrFind(str_type,"C1"))
                {
                    switch (iRelType)
                    {
                        case 0:     // C0VDC
                             giA+=_StrToInt(str_item_amt);   //金額
                             giB+=i_trans_num;               //次數
                             break;
                        default:
                             giC+=_StrToInt(str_item_amt);   //金額
                             giD+=i_trans_num;               //次數
                             break;
                    }

                    //外加手續費,即時購代售點卡集計 如要扣攤提參部門帳
                    // A-giH , B-giI , C-giJ , D-giK , E-giL , F-giM , G-giN 
                    switch (i_trans_flag)
                    {
                    case 1:     //外加手續費
                        giH += _StrToInt(str_item_amt);   //金額
                        giI += i_trans_num;               //次數
                        break;
                    case 2:     //即時購代售點卡
                    case 14:    //2019/05/21 Add
                        giJ += _StrToInt(str_item_amt);   //金額
                        giK += i_trans_num;               //次數
                        break;

                    }



                }
            }
        }
    }

    logsprintf("Sub_C0InvoAmt4VDC C0C1: C0_NVDC金額次數(%d)(%d), C0_VDC金額次數(%d)(%d)",  giA, giB, giC, giD );
    logsprintf("Sub_C0InvoAmt4VDC C0C1: 外加手續費金次數(%d)(%d), 即時購代售點卡(%d)(%d)", giH, giI, giJ, giK);
}




//收銀員交班明細表 for 切帳用
//iSumCash41:1050現金,  iSumPresent41:1050禮券+商品禮券, iSumCpn41:1050折價券+Other1+有價票券 CPN
//iSumPsRtn41:1050餘額券 ,  iSumVisualCpnAmt:1040無紙本折價金額, iWinnInvoAmt:1050發票兌獎金額

int __fastcall BASIC::SumCasherRptXDT(int iAutoZ, int StartLine, int iSumCash41,
                                   int iSumPresent41, int iSumCpn41, int iSumPsRtn41, int iSumVisualCpnAmt, int iWinnInvoAmt,
                                   string sTranCnt, string sXdtNo,
                                   string SCasherRpt,
                                   string sStr1001,
                                   string sStr1097,
                                   string sStr1095,
                                   string &RtnSumCasherData,
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

    int  pay_other1_amt, pay_other2_amt;

    //PAYCASHREC pay_cash_rec[100];
	PAYCASHREC pay_cash_rec[101];


    iacc_invo_amt=iacc_amt01=iacc_amt02=iacc_amt03=iacc_amt04=iacc_amt05=
        iacc_amt06=iacc_amt07=iacc_amt08=iacc_amt09=iacc_amt10=iacc_amt11=
        iacc_amt12=iacc_amt13=iacc_amt14=0;

    iacc_invo_cnt=iacc_cnt01=iacc_cnt02=iacc_cnt03=iacc_cnt04=iacc_cnt05=
        iacc_cnt06=iacc_cnt07=iacc_cnt08=iacc_cnt09=iacc_cnt10=iacc_cnt11=
        iacc_cnt12=iacc_cnt13=iacc_cnt14=0;

    icash_short_amt=ipresent_short_amt=icpn_short_amt=itot_short_amt=
        ircv_cnt=iinvo_rtn_cnt=iinvo_void_cnt=0;

    ipay_cash_amt=ipay_present_amt=ipay_cpn_amt=0;

	pay_other1_amt=pay_other2_amt=0;

    string str_sale_type, strtmp, tmpSer, tmp0041Date1, tmp0041ChrName,s;       //結帳型態


	logsprintf("SumCasherRptXDT: 收銀員交班明細表 StartLine=%d, iSumCash41=%d, iSumPsRtn41=%d, iSumVisualCpnAmt=%d, ifoodamt=%d, ifoodcnt=%d",
                StartLine, iSumCash41, iSumPsRtn41, iSumVisualCpnAmt, ifoodamt, ifoodcnt);

    for( cashi=0; cashi<100; cashi++)
        {
               pay_cash_rec[cashi].IndexNo=-1;
               pay_cash_rec[cashi].PayCashAmt=-9999;
        }

    int int_drop_line = (gtsl_drop->size() - 1);   //投庫次數
    //for (int i=StartLine, cashi=0; i<=int_drop_line; i++, cashi++)
	for (list<string>::iterator ls = gtsl_drop->begin(); ls != gtsl_drop->end(); ls++)
        {
			s = *ls; // gtsl_drop->Strings[i];
           _StringSegment_EX(s, SGM, 1);

           iTmp=_StrToInt(_StringSegment_EX(s, SGM, 9));   //本日頭庫累積次數 交班時產生
           if( iTmp == 0)
             {
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=0;
               continue;
              }

           //tmp0041Date1=gtsl_drop->Strings[i].SubString(1,60); // Rec_Type~sale_no
           tmp0041ChrName=_StringSegment_EX(s, SGM, 58);  //  gtsl_drop->Strings[i].SubString(450,10);

           tmpSer=_StringSegment_EX(s, SGM, 7);    //交易序號
            //現金
               pay_cash_rec[cashi].IndexNo=cashi;
               pay_cash_rec[cashi].PayCashAmt=_StrToInt(_StringSegment_EX(s, SGM, 11));
               if( pay_cash_rec[cashi].PayCashAmt >= 0)
                   ipay_cash_amt +=pay_cash_rec[cashi].PayCashAmt;
            //禮券
               ipay_present_amt += _StrToInt(_StringSegment_EX(s, SGM, 14));

            //折價券
               ipay_cpn_amt += _StrToInt(_StringSegment_EX(s, SGM, 15));

			//中獎發票金額(500)
			   pay_other1_amt += _StrToInt(_StringSegment_EX(s, SGM, 19));

            //中獎發票金額(1000)
               pay_other2_amt += _StrToInt(_StringSegment_EX(s, SGM, 20));

            //中獎發票金額(200)
               iacc_invo_amt += _StrToInt(_StringSegment_EX(s, SGM, 22));

            //費用單據金額 iacc_amt01..iacc_cnt14
               iacc_amt01+=_StrToInt(_StringSegment_EX(s, SGM, 23));
               iacc_cnt01+=_StrToInt(_StringSegment_EX(s, SGM, 24));

               iacc_amt02+=_StrToInt(_StringSegment_EX(s, SGM, 25));
               iacc_cnt02+=_StrToInt(_StringSegment_EX(s, SGM, 26));

               iacc_amt03+=_StrToInt(_StringSegment_EX(s, SGM, 27));
               iacc_cnt03+=_StrToInt(_StringSegment_EX(s, SGM, 28));

               iacc_amt04+=_StrToInt(_StringSegment_EX(s, SGM, 29));
               iacc_cnt04+=_StrToInt(_StringSegment_EX(s, SGM, 30));

               iacc_amt05+=_StrToInt(_StringSegment_EX(s, SGM, 31));
               iacc_cnt05+=_StrToInt(_StringSegment_EX(s, SGM, 32));

               iacc_amt06+=_StrToInt(_StringSegment_EX(s, SGM, 33));
               iacc_cnt06+=_StrToInt(_StringSegment_EX(s, SGM, 34));

               iacc_amt07+=_StrToInt(_StringSegment_EX(s, SGM, 35));
               iacc_cnt07+=_StrToInt(_StringSegment_EX(s, SGM, 36));

               iacc_amt08+=_StrToInt(_StringSegment_EX(s, SGM, 37));
               iacc_cnt08+=_StrToInt(_StringSegment_EX(s, SGM, 38));

               iacc_amt09+=_StrToInt(_StringSegment_EX(s, SGM, 39));
               iacc_cnt09+=_StrToInt(_StringSegment_EX(s, SGM, 40));

               iacc_amt10+=_StrToInt(_StringSegment_EX(s, SGM, 41));
               iacc_cnt10+=_StrToInt(_StringSegment_EX(s, SGM, 42));

               iacc_amt11+=_StrToInt(_StringSegment_EX(s, SGM, 43));
               iacc_cnt11+=_StrToInt(_StringSegment_EX(s, SGM, 44));

               iacc_amt12+=_StrToInt(_StringSegment_EX(s, SGM, 45));
               iacc_cnt12+=_StrToInt(_StringSegment_EX(s, SGM, 46));

               iacc_amt13+=_StrToInt(_StringSegment_EX(s, SGM, 47));
               iacc_cnt13+=_StrToInt(_StringSegment_EX(s, SGM, 48));

               iacc_amt14+=_StrToInt(_StringSegment_EX(s, SGM, 49));
               iacc_cnt14+=_StrToInt(_StringSegment_EX(s, SGM, 50));

        }     //for (int i=StartLine; i<=TotalLine; i++)

     int itot_acc_amt;   //費用單據金額
     itot_acc_amt=iacc_amt01+iacc_amt02+iacc_amt03+iacc_amt04+iacc_amt05+
                iacc_amt06+iacc_amt07+iacc_amt08+iacc_amt09+iacc_amt10+
                iacc_amt11+iacc_amt12; //+iacc_amt13+iacc_amt14;  20151123 改為食安與提貨券  Charles.Wang

     iacc_amt13=ifoodamt; //食安退款金額 ;
     iacc_cnt13=ifoodcnt; //食安退款次數 ;

     //現金短溢收=(現金+中獎發票金額(200+1000)+費用單據金額+找餘額券)-XDT現金 ,當acc_type > 01時才有值
     //現金短溢收=(現金+費用單據金額+找餘額券)-XDT現金 ,當acc_type > 01時才有值  // 2017/0815 Update
    //int itotSumCash41=(ipay_cash_amt+iacc_invo_amt+pay_other2_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);
    int itotSumCash41=(ipay_cash_amt+itot_acc_amt)-(iSumCash41-iSumPsRtn41);
    int itotSumPresent41=ipay_present_amt-iSumPresent41; //禮券短溢收,當acc_type > 01時才有值
    int itotSumCpn41=ipay_cpn_amt-iSumCpn41;
    int itotWinInvoAmt41=(iacc_invo_amt+ pay_other1_amt+pay_other2_amt)-iWinnInvoAmt;    // 2017/08/15 Update 中獎發票金額
    int iqt_cetel, iqt_R23Cnt, iqt_VCnt;
    int iqt_food,iamt_food;   //giH giI 食安退貨金額、次數

    int iRentAmt, iRemtCnt;
    sub_times(0,gi_rec_line);   //giH giI 食安退貨金額、次數      //代收次數,  租賃 退押金次數(giB), 退押金(giA) , 代付(giG)
    iRentAmt=giA; iRemtCnt=giB;
    iqt_cetel=giD;
    iqt_food=giI; iamt_food=giH;

    TotMissCnt(0,gi_tot_line);

    Sum040RecycleDataCnt(0, gi_bil_line);      //廢資源回收數 /提貨券金額/提貨券張數
    iacc_amt14=giE; //提貨券金額
    iacc_cnt14=giF; //提貨券張數

	// 2018/12/07 Lu Update
	int iPaCpnAmt = giM;         //紙本折價券金額
	int iPaCpnCnt = giN;         //紙本折價券張數
	int iPaPresentAmt = giO;    //紙本禮券金額
	int iPaPresentCnt = giP;    //紙本禮券張數

    iqt_R23Cnt=giA;
    iqt_VCnt=giB;

    logsprintf("SumCasherRptXDT 收銀員交接班明細表:收銀員交班(%s) 投庫筆數(%d), 代收張數(%d), 作廢發票張數(%d), 交易取消作廢發票張數(%d), 找餘額券(%d),提貨券金額(%d),提貨券張數(%d)",
                   sTranCnt.substr(52,8).c_str(), int_drop_line+1, iqt_cetel, iqt_R23Cnt, iqt_VCnt, iSumPsRtn41, iacc_amt14, iacc_cnt14);

     // Head1041.sprintf("1041|0500|%-6s|%2s|%14s|%05d|%-10s|%8s",
     //                   str_tencode.c_str(),
     //                   str_ecr_no.c_str(),
     //                  XDTsysDttm.c_str(),                   //x->dt_begin+x->tm_begin,
     //                   str_nz_cnt.ToInt(),
     //                   (str_trans1_5 + str_sum_tran).c_str(),
     //                   str_cashier_no.c_str()
     //                   );

	string nXDTdttb = _StringSegment_EX(sStr1095, SGM, 10) + _StringSegment_EX(sStr1095, SGM, 11);  //前次交班時間
    string nXDTdttn = _StringSegment_EX(sStr1095, SGM, 12) + _StringSegment_EX(sStr1095, SGM, 13);  //本次交班時間
	char buffer[4096];
    try
       {
        //3041|電文
		   sprintf_s(buffer, sizeof(buffer),"%s%14s||%5s||%-10s||%-20s||%04d||%04d||%010ld||%010ld||%010ld||%2s||%010ld||%010ld||%010ld||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%04d||%010ld||%010ld||%010ld||%010ld||%04d||%04d||%04d||%-10.10s||%s||\n",
                                         sTranCnt.substr(0,24).c_str(),
                                         nXDTdttn.c_str(),
                                         _StringSegment_EX(sTranCnt, SGM, 6).c_str(), //NO_TMCLOSE   //TranCnt.substr(34,26).c_str(),
                                         _StringSegment_EX(sTranCnt, SGM, 7).c_str(), //NO_DEAL
                                         _StringSegment_EX(sTranCnt, SGM, 8).c_str(), //NO_CASHER 
                                         0,       //Z_RTN_CNT 9
                                         0,       //X_RTN_CNT 10
                                         ipay_cash_amt,
                                         ipay_present_amt,
                                         ipay_cpn_amt,
                                         sXdtNo.c_str(),              // ACC_TYPE
                                         iacc_invo_amt,    //中獎發票金額(200)
                                         pay_other1_amt,   //中獎發票金額(500)
                                         pay_other2_amt,   //中獎發票金額(1000)
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
                                         iacc_amt13,    //食安退款金額
                                         iacc_cnt13,    //食安退款筆數
                                         iacc_amt14,    //提貨券金額
                                         iacc_cnt14,    //提貨券張數
                                         itotSumCash41, //現金短溢收,當acc_type > 01時才有值
                                         itotSumPresent41, //禮券短溢收,當acc_type > 01時才有值
                                         itotSumCpn41, //折價券短溢收,當acc_type > 01時才有值
                                         itotSumCash41+itotSumPresent41+itotSumCpn41+itotWinInvoAmt41, //短溢收合計,當acc_type > 01時才有值
                                         iqt_cetel, //代收張數,當acc_type > 01時才有值
                                         iqt_R23Cnt, //作廢發票張數,當acc_type > 01時才有值
                                         iqt_VCnt, //交易取消作廢發票張數,當acc_type > 01時才有值
                                         tmp0041ChrName.c_str(),  //10,收銀員
                                         "1||2||3||4||5||6||7||8||9||1");  //保留,ver1 10

	      RtnSumCasherData = string(buffer);

          //RtnSumCasherData+=sStr1097;
          logsprintf("SumCasherRptXDT::收銀員交班DATA->0041:%s.",RtnSumCasherData.c_str() );
        }
      catch(...)
       {
          logsprintf("SumCasherRptXDT::收銀員交班DATA->0041: Catch Error");
       }

     // if( iAutoZ )
     //   DeleteFile(sXdtFile.c_str());

   TStringList *tslCrp;
   tslCrp = new TStringList;
   string bt, BefTime, nt, NowTime, sStoreNo, sStoreName, sLog;

   //TIniFile *AutoCrpIni, *xzdata_ecr;
   //AutoCrpIni = new TIniFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");
   int i1,i2, iFTLSysPrinterInitial;
 
  //  xzdata_ecr = new TIniFile(XZDATA_INI);
  //  iFTLSysPrinterInitial = xzdata_ecr->ReadInteger("ECR","FTLSysPrinterInitial",0);    
  //  delete xzdata_ecr;
   m_IniReader.setINIFileName(XZDATA_INI);
   iFTLSysPrinterInitial = stoi(m_IniReader.ReadString("ECR", "FTLSysPrinterInitial", "0"));
 
   string CmdStr="";
   if (iFTLSysPrinterInitial == 1)
   {
	   sprintf_s(buffer, sizeof(buffer),"%c%cM080801", 27, 27);    //ESC, ESC, M , 字型[2]:08, SIZE[2]:08, 樣式[2]:00
	   CmdStr = string(CmdStr);
   }

   logsprintf("SumCasherRptXDT: 收銀員交接班明細表:XZDATA_INI->ECR->FTLSysPrinterInitial=%d, CmdStr(%s)", iFTLSysPrinterInitial, CmdStr.c_str());

   //sTranCnt +="|";
   bt = nXDTdttb;
   BefTime = bt.substr(0, 4) + "-" + bt.substr(4, 2) + "-" + bt.substr(6, 2) + "  " + bt.substr(8, 2) + ":" + bt.substr(10, 2);
   nt = nXDTdttn;  //_StringSegment_EX(sTranCnt, "|", 5); // sTranCnt.SubString(21,14);  1041
   NowTime=nt.substr(0,4)+"-"+nt.substr(4,2)+"-"+nt.substr(6,2)+"  "+nt.substr(8,2)+":"+nt.substr(10,2);
   read_ten_name(sStoreNo, sStoreName);

   sprintf_s(buffer, sizeof(buffer), "%s        收銀員交接班明細表", CmdStr.c_str()); tslCrp->push_back(string(buffer));
   //sprintf_s(buffer, sizeof(buffer), "%s%s     機號:%s", CmdStr.c_str(), NowTime.c_str(), _StringSegment_EX(sTranCnt, "|", 4).c_str());   tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "%s%s 開始", CmdStr.c_str(), BefTime.c_str());   tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "%s%s 結束  機號:%s", CmdStr.c_str(), NowTime.c_str(), _StringSegment_EX(sTranCnt, SGM, 4).c_str());   tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "%s店號:%s    店名:%s", CmdStr.c_str(), _StringSegment_EX(sTranCnt, SGM, 3).c_str(), sStoreName.c_str());  tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "%s收銀員編號:%s", CmdStr.c_str(), _StringSegment_EX(sTranCnt, SGM, 8).c_str());   tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "%s投庫明細:", CmdStr.c_str());   tslCrp->push_back(string(buffer));

   string ReadCashStr,CashStr;
   int  ReadCashCnt=0;
   int  ReadCashtot=0;
   int  ReadCashCntIdx=0;
   CashStr="現金||";
   //strtmp.sprintf("      ＋現           金:%8ld",ipay_cash_amt);   tslCrp->Add(strtmp);

   m_IniReader.setINIFileName("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");
   ReadCashStr = m_IniReader.ReadString("AutoX", "Cashdrp", "0");  // AutoCrpIni->ReadString("AutoX", "Cashdrp", "0");
   ReadCashCnt = stoi(m_IniReader.ReadString("AutoX", "Cashdrpcnt", "0")); //AutoCrpIni->ReadInteger("AutoX", "Cashdrpcnt", 0);
   ReadCashtot = stoi(m_IniReader.ReadString("AutoX", "Cashdrptot", "0")); //AutoCrpIni->ReadInteger("AutoX", "Cashdrptot", 0);

   if(ReadCashCnt > 0)
     {
		 sprintf_s(buffer, sizeof(buffer), "%s  ＋現           金:自.%8ld", CmdStr.c_str(), ReadCashtot);
		 tslCrp->push_back(string(buffer));
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

			  sprintf_s(buffer, sizeof(buffer), "%s  ＋現           金:%2d.%8ld", CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt );
			  tslCrp->push_back(string(buffer));
              ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
              ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||", 0, 0);

			   CashStr += string(buffer);  
              }

          }
       else
          {

			sprintf_s(buffer, sizeof(buffer), "%s                   :%2d.%8ld", CmdStr.c_str(),
                           pay_cash_rec[cashi].IndexNo+1+ReadCashCntIdx, pay_cash_rec[cashi].PayCashAmt);
			tslCrp->push_back(string(buffer));
            ReadCashtot+=pay_cash_rec[cashi].PayCashAmt;
            ReadCashCnt++;
            if( Update )
              {
               if( iAutoZ )
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||",
                           pay_cash_rec[cashi].IndexNo+1, pay_cash_rec[cashi].PayCashAmt);
               else
				   sprintf_s(buffer, sizeof(buffer), "%02d||%8ld||", 0, 0);

			   CashStr += string(buffer);
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
			   m_IniReader.WriteString("AutoX", "Cashdrp", CashStr);   //    AutoCrpIni->WriteString("AutoX", "Cashdrp", CashStr);
			   m_IniReader.WriteString("AutoX", "Cashdrpcnt", int2str(ReadCashCnt));  //AutoCrpIni->WriteInteger("AutoX", "Cashdrpcnt", ReadCashCnt);
			   m_IniReader.WriteString("AutoX", "Cashdrptot", int2str(ReadCashtot));  //AutoCrpIni->WriteInteger("AutoX", "Cashdrptot", ReadCashtot);
         }


   ////////
   strtmp = m_IniReader.ReadString("AutoX", "pres", "0");     //strtmp = AutoCrpIni->ReadString("AutoX", "pres", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s　＋禮券 / 商品禮券     :%7ld", CmdStr.c_str(), ipay_present_amt + i2);   tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "禮券||%02d||%8ld||", 0, ipay_present_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "禮券||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "pres", string(buffer));   //AutoCrpIni->WriteString("AutoX", "pres", strtmp);
     }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "cpn", "0");     //strtmp=AutoCrpIni->ReadString("AutoX","cpn","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋折價券/紅利點數     :%7ld", CmdStr.c_str(), ipay_cpn_amt + i2);   tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "折價||%02d||%8ld||", 0, ipay_cpn_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "折價||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "cpn", string(buffer));  //AutoCrpIni->WriteString("AutoX", "cpn", strtmp);
     }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "invo", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "invo", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋中獎發票金額(200元) :%7ld", CmdStr.c_str(), iacc_invo_amt + i2);  tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "中獎||%02d||%8ld||", 0, iacc_invo_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "中獎||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "invo", string(buffer));   //AutoCrpIni->WriteString("AutoX", "invo", strtmp);
    }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "invo500", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","invo500","0");
   i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋中獎發票金額(500元) :%7ld", CmdStr.c_str(), pay_other1_amt + i2);  tslCrp->push_back(string(buffer));

   if (Update)
   {
	   if (iAutoZ)
		   sprintf_s(buffer, sizeof(buffer), "中獎500||%02d||%8ld||", 0, pay_other1_amt + i2);
	   else
		   sprintf_s(buffer, sizeof(buffer), "中獎500||%02d||%8ld||", 0, 0);
	   m_IniReader.WriteString("AutoX", "invo500", string(buffer));   //AutoCrpIni->WriteString("AutoX","invo500",strtmp);
   }

    ////////
   strtmp = m_IniReader.ReadString("AutoX", "invo1000", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","invo1000","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋中獎發票金額(1000元):%7ld", CmdStr.c_str(), pay_other2_amt + i2);  tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "中獎1000||%02d||%8ld||", 0, pay_other2_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "中獎1000||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "invo1000", string(buffer));   //AutoCrpIni->WriteString("AutoX","invo1000",strtmp);
    }


   ///////
   strtmp = m_IniReader.ReadString("AutoX", "acc", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","acc","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "%s  ＋費用單據金額        :%7ld", CmdStr.c_str(), itot_acc_amt + i2);   tslCrp->push_back(string(buffer));
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "費用||%02d||%8ld||", 0, itot_acc_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "費用||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "acc", string(buffer));   //AutoCrpIni->WriteString("AutoX","acc",strtmp);
     }

   ///////

   ///////  食安退款    20151202 全家取消此行計算，移至退款筆數前
   /*strtmp=AutoCrpIni->ReadString("AutoX","food","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, "|", 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, "|", 3));

   strtmp.sprintf("      －食安退款       :   %8ld",iacc_amt13+i2);   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
       strtmp.sprintf("食安|%02d|%8ld|", 0, iacc_amt13+i2);
      else
       strtmp.sprintf("食安|%02d|%8ld|", 0, 0);
      AutoCrpIni->WriteString("AutoX","food",strtmp);
     }
   */
   ///////


   strtmp = m_IniReader.ReadString("AutoX", "drpacc", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","drpacc","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   sprintf_s(buffer, sizeof(buffer), "------------------------------------"); tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "%s  投庫小計         :   %8ld\n", CmdStr.c_str(), ipay_cash_amt + ipay_present_amt + ipay_cpn_amt +
	   iacc_invo_amt + pay_other1_amt + pay_other2_amt + itot_acc_amt + i2);                                                           tslCrp->push_back(string(buffer));
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "投庫||%02d||%8ld||", 0, ipay_cash_amt + ipay_present_amt + ipay_cpn_amt + iacc_invo_amt + pay_other2_amt + itot_acc_amt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "投庫||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "drpacc", string(buffer));   //AutoCrpIni->WriteString("AutoX","drpacc",strtmp);
     }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "Cashtot", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","Cashtot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> Cashtot = %s ,現金短溢收  :%8ld", strtmp.c_str(), itotSumCash41 + i2);

 //strtmp.sprintf("現金短溢收  :01234567  收銀員簽章",itotSumCash41);   tslCrp->Add(strtmp);
   sprintf_s(buffer, sizeof(buffer), "%s現金短溢收  :%7ld 收銀員簽章", CmdStr.c_str(), itotSumCash41 + i2);   tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "現短||%02d||%8ld||", 0, itotSumCash41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "現短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "Cashtot", string(buffer));  //AutoCrpIni->WriteString("AutoX", "Cashtot", strtmp);
    }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "prestot", "0");   //strtmp = AutoCrpIni->ReadString("AutoX", "prestot", "0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> prestot = %s ,禮券短溢收  :%8ld", strtmp.c_str(), itotSumPresent41 + i2);

   sprintf_s(buffer, sizeof(buffer), "%s禮券短溢收  :%7ld ┌──── ┐", CmdStr.c_str(), itotSumPresent41 + i2);   tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "禮短||%02d||%8ld||", 0, itotSumPresent41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "禮短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "prestot", string(buffer));  // AutoCrpIni->WriteString("AutoX","prestot",strtmp);
    }

   ////////
   strtmp = m_IniReader.ReadString("AutoX", "cpntot", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","cpntot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> cpntot = %s ,折價券短溢收  :%8ld", strtmp.c_str(), itotSumCpn41 + i2);

   sprintf_s(buffer, sizeof(buffer), "%s折價券短溢收:%7ld │         │", CmdStr.c_str(), itotSumCpn41 + i2);    tslCrp->push_back(string(buffer));
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "折短||%02d||%8ld||", 0, itotSumCpn41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "折短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "cpntot", string(buffer));  //AutoCrpIni->WriteString("AutoX","cpntot",strtmp);
     }

    ////////   中獎發票金額  ////////////
   strtmp = m_IniReader.ReadString("AutoX", "wininvotot", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","wininvotot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> wininvotot = %s , 中獎發票短溢收:%8ld", strtmp.c_str(), itotWinInvoAmt41 + i2);

   sprintf_s(buffer, sizeof(buffer), "%s中獎發票短溢收:%5ld │         │", CmdStr.c_str(), itotWinInvoAmt41 + i2);   tslCrp->push_back(string(buffer));
   int iTmptotWinInvoAmt41 = itotWinInvoAmt41+i2;
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "獎短||%02d||%8ld||", 0, itotWinInvoAmt41 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "獎短||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "wininvotot", string(buffer));  //AutoCrpIni->WriteString("AutoX","wininvotot",strtmp);
     }

  ////////

   strtmp = m_IniReader.ReadString("AutoX", "subtot", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","subtot","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> subtot = %s ,短溢收合計  :%8ld", strtmp.c_str(),
                                                itotSumCash41+itotSumPresent41+itotSumCpn41+i2+itotWinInvoAmt41);

   sprintf_s(buffer, sizeof(buffer), "%s短溢收合計  :%7ld │         │", CmdStr.c_str(), itotSumCash41 + itotSumPresent41 + itotSumCpn41 + i2 + itotWinInvoAmt41);   tslCrp->push_back(string(buffer));
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "短合||%02d||%8ld||", 0, itotSumCash41 + itotSumPresent41 + itotSumCpn41 + i2 + itotWinInvoAmt41);
      else
		  sprintf_s(buffer, sizeof(buffer), "短合||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "subtot", string(buffer));  //AutoCrpIni->WriteString("AutoX","subtot",strtmp);
     }

   ///////
   strtmp = m_IniReader.ReadString("AutoX", "agncnt", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","agncnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> agncnt = %s ,代收張數  :%8ld", strtmp.c_str(),
                                                     iqt_cetel+i2);

   sprintf_s(buffer, sizeof(buffer), "%s代收張數    :%7ld │         │", CmdStr.c_str(), iqt_cetel + i2);   tslCrp->push_back(string(buffer));
   if( Update )
      {
       if( iAutoZ )
		   sprintf_s(buffer, sizeof(buffer), "代張||%02d||%8ld||", 0, iqt_cetel + i2);  // 2013/10/17 Update Lu CodeView
       else
		   sprintf_s(buffer, sizeof(buffer), "代張||%02d||%8ld||", 0, 0);
	   m_IniReader.WriteString("AutoX", "agncnt", string(buffer));  //AutoCrpIni->WriteString("AutoX","agncnt",strtmp);
      }

    //////////////////////// 2014/12/09  ////////////////////////////////////////
   strtmp = m_IniReader.ReadString("AutoX", "s1051cnt", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","s1051cnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   string sMissRec, AutoXsMissRec, PrintAutoXsMissRec;
   AutoXsMissRec = m_IniReader.ReadString("AutoX", "s1051cntMissRec", "");
   S1051MissCnt(0, gi_1051_line, sMissRec);
   int  sMissRecCnt = giC;
   AutoXsMissRec += sMissRec;
   PrintAutoXsMissRec = AutoXsMissRec;

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> s1051cnt = %s ,紙本作廢發票張數  :%8ld, AutoXsMissRec(%s)", strtmp.c_str(), giA + i2, AutoXsMissRec.c_str());

   sprintf_s(buffer, sizeof(buffer), "%s紙本作廢發票:%7ld │         │", CmdStr.c_str(), giA + i2);  tslCrp->push_back(string(buffer));

     if( Update )
     {
		 if (iAutoZ)
		 {
			 sprintf_s(buffer, sizeof(buffer), "紙廢||%02d||%8ld||", 0, giA + i2);
		 }
		 else
		 {
			 sprintf_s(buffer, sizeof(buffer), "紙廢||%02d||%8ld||", 0, 0);
			 AutoXsMissRec = "";
		 }

	  m_IniReader.WriteString("AutoX", "s1051cnt", string(buffer));  //AutoCrpIni->WriteString("AutoX","s1051cnt",strtmp);
	  m_IniReader.WriteString("AutoX", "s1051cntMissRec", AutoXsMissRec);
     }
   ///////////////////////////////////////////////////////////////////////////
	 // 2018/12/07 Lu Update
	 strtmp = m_IniReader.ReadString("AutoX", "iPaPresentCnt", "0");  //紙本折禮券張數
	 i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
	 i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));

	 logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> iPaPresentCnt = %s ,紙本禮券張數  :%8ld", strtmp.c_str(), iPaPresentCnt + i2);

	 sprintf_s(buffer, sizeof(buffer), "%s紙本禮券張數:%7ld │         │", CmdStr.c_str(), iPaPresentCnt + i2);   tslCrp->push_back(buffer);

	 if (Update)
	 {
		 if (iAutoZ)
			 sprintf_s(buffer, sizeof(buffer), "紙禮||%02d||%8ld||", 0, iPaPresentCnt + i2);
		 else
			 sprintf_s(buffer, sizeof(buffer), "紙禮||%02d||%8ld||", 0, 0);

		 m_IniReader.WriteString("AutoX", "iPaPresentCnt", string(buffer));
	 }

	 /////////////////////////////////////////////////
	 strtmp = m_IniReader.ReadString("AutoX", "iPaCpnCnt", "0");  //紙本折折價券張數
	 i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
	 i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));

	 logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> iPaCpnCnt = %s ,紙本折價券張數  :%8ld", strtmp.c_str(), iPaCpnCnt + i2);

	 sprintf_s(buffer, sizeof(buffer), "%s紙本折價券張數:%5ld │         │", CmdStr.c_str(), iPaCpnCnt + i2);   tslCrp->push_back(buffer);

	 if (Update)
	 {
		 if (iAutoZ)
			 sprintf_s(buffer, sizeof(buffer), "紙折||%02d||%8ld||", 0, iPaCpnCnt + i2);
		 else
			 sprintf_s(buffer, sizeof(buffer), "紙折||%02d||%8ld||", 0, 0);

		 m_IniReader.WriteString("AutoX", "iPaCpnCnt", string(buffer));
	 }

	 ///////////////////////////////////////////////////////////////////////////


   strtmp = m_IniReader.ReadString("AutoX", "r23cnt", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","r23cnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> r23cnt = %s ,作廢發票張數  :%8ld", strtmp.c_str(), iqt_R23Cnt + iqt_VCnt + i2);
 
   sprintf_s(buffer, sizeof(buffer), "%s作廢發票總數:%7ld │         │", CmdStr.c_str(), iqt_R23Cnt + iqt_VCnt + i2);   tslCrp->push_back(string(buffer));

   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "廢張||%02d||%8ld||", 0, iqt_R23Cnt + iqt_VCnt + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "廢張||%02d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "r23cnt", string(buffer));  //AutoCrpIni->WriteString("AutoX","r23cnt",strtmp);
     }

 ////////////////////食安退款筆數2015/12/02///////////////////////////////////////////////////////////////////////////////////


   strtmp = m_IniReader.ReadString("AutoX", "Foodcnt", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","Foodcnt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   //iacc_amt13,     //食安退款金額
   //iacc_cnt13,     //食安退款筆數
   logsprintf("SumCasherRpt 收銀員交接班明細表: AutoCrp.ini:: AutoX -> Foodcnt = %s ,食安退款筆數 :%4ld, 食安退款金額 :%8ld,", strtmp.c_str(),
                                                      iacc_cnt13+i1, iacc_amt13+i2);
   sprintf_s(buffer, sizeof(buffer), "%s食安退款金額:%7ld │         │", CmdStr.c_str(), iacc_amt13 + i2);   tslCrp->push_back(string(buffer));
   //strtmp.sprintf("食安退款總數:%8ld  ", iacc_cnt13+i1 );   tslCrp->Add(strtmp);
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "食退||%04d||%8ld||", iacc_cnt13 + i1, iacc_amt13 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "食退||%04d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "Foodcnt", string(buffer));  //AutoCrpIni->WriteString("AutoX","Foodcnt",strtmp);
     }


   /////////////////////////// 2015/10/22 //////////////////////////////////////////////////////////////

   strtmp = m_IniReader.ReadString("AutoX", "BillAmt", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","BillAmt","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   //iacc_amt14,     //提貨券金額
   //iacc_cnt14,     //提貨券張數

  logsprintf("SumCasherRptXDT 收銀員交接班明細表: AutoCrp.ini:: AutoX -> BillAmt = %s ,提貨券張數 :%4ld, 提貨券金額 :%8ld,", strtmp.c_str(), iacc_cnt14 + i1, iacc_amt14 + i2);
   
  sprintf_s(buffer, sizeof(buffer), "%s提貨券金額  :%7ld │         │", CmdStr.c_str(), iacc_amt14 + i2);   tslCrp->push_back(string(buffer));
  sprintf_s(buffer, sizeof(buffer), "%s提貨券張數  :%7ld └──── ┘", CmdStr.c_str(), iacc_cnt14 + i1);  tslCrp->push_back(string(buffer));
   if( Update )
     {
      if( iAutoZ )
		  sprintf_s(buffer, sizeof(buffer), "提貨||%04d||%8ld||", iacc_cnt14 + i1, iacc_amt14 + i2);
      else
		  sprintf_s(buffer, sizeof(buffer), "提貨||%04d||%8ld||", 0, 0);
	  m_IniReader.WriteString("AutoX", "BillAmt", string(buffer));  //AutoCrpIni->WriteString("AutoX","BillAmt",strtmp);
     }

 /////////////////////////////////////////////////////////////////////////////////////////////////

   strtmp = m_IniReader.ReadString("AutoX", "xBtrPaks", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","xBtrPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   if( Cpn71_amt+i2 )
      {
		  sprintf_s(buffer, sizeof(buffer), "%s廢電池袋數  :%7ld  ", CmdStr.c_str(), Cpn71_amt + i2);   tslCrp->push_back(string(buffer));

         if( Update )
          {
             if( iAutoZ )
				 sprintf_s(buffer, sizeof(buffer), "廢池||%02d||%8ld||", 0, Cpn71_amt + i2);
             else
				 sprintf_s(buffer, sizeof(buffer), "廢池||%02d||%8ld||", 0, 0);
			 m_IniReader.WriteString("AutoX", "xBtrPaks", string(buffer));  // AutoCrpIni->WriteString("AutoX","xBtrPaks",strtmp);
          }
      }

   strtmp = m_IniReader.ReadString("AutoX", "xPhoPaks", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","xPhoPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));

   if( Cpn72_amt+i2 )
      {
		  sprintf_s(buffer, sizeof(buffer), "%s廢手機袋數  :%7ld  ", CmdStr.c_str(), Cpn72_amt + i2);   tslCrp->push_back(string(buffer));
         if( Update )
          {
             if( iAutoZ )
				 sprintf_s(buffer, sizeof(buffer), "廢機||%02d||%8ld||", 0, Cpn72_amt + i2);
             else
				 sprintf_s(buffer, sizeof(buffer), "廢機||%02d||%8ld||", 0, 0);
			 m_IniReader.WriteString("AutoX", "xPhoPaks", string(buffer));  //AutoCrpIni->WriteString("AutoX","xPhoPaks",strtmp);
          }
      }

   strtmp = m_IniReader.ReadString("AutoX", "xNotPaks", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","xNotPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   if( Cpn73_amt+i2 )
      {
		  sprintf_s(buffer, sizeof(buffer), "%s廢筆電袋數  :%7ld  ", CmdStr.c_str(), Cpn73_amt + i2);   tslCrp->push_back(string(buffer));
         if( Update )
          {
             if( iAutoZ )
				 sprintf_s(buffer, sizeof(buffer), "廢電||%02d||%8ld||", 0, Cpn73_amt + i2);
             else
				 sprintf_s(buffer, sizeof(buffer), "廢電||%02d||%8ld||", 0, 0);
			 m_IniReader.WriteString("AutoX", "xNotPaks", string(buffer));  //AutoCrpIni->WriteString("AutoX","xNotPaks",strtmp);
          }
      }

   strtmp = m_IniReader.ReadString("AutoX", "xDskPaks", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","xDskPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   if( Cpn74_amt+i2 )
      {
		  sprintf_s(buffer, sizeof(buffer), "%s廢光碟袋數  :%7ld  ", CmdStr.c_str(), Cpn74_amt + i2);   tslCrp->push_back(string(buffer));
        if( Update )
          {
             if( iAutoZ )
				 sprintf_s(buffer, sizeof(buffer), "廢碟||%02d||%8ld||", 0, Cpn74_amt + i2);
             else
				 sprintf_s(buffer, sizeof(buffer), "廢碟||%02d||%8ld||", 0, 0);
			 m_IniReader.WriteString("AutoX", "xDskPaks", string(buffer));  //AutoCrpIni->WriteString("AutoX","xDskPaks",strtmp);
          }
      }

   strtmp = m_IniReader.ReadString("AutoX", "xInkPaks", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","xInkPaks","0");
   i1=_StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2=_StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   if( Cpn75_amt+i2 )
      {
	     // 2019/11/27 Update   20200409 袋數
		  //sprintf_s(buffer, sizeof(buffer), "%s廢墨水匣袋數:%7ld  ", CmdStr.c_str(), Cpn75_amt + i2);   tslCrp->push_back(string(buffer));
		  sprintf_s(buffer, sizeof(buffer), "%s廢平板袋數  :%7ld  ", CmdStr.c_str(), Cpn75_amt + i2);   tslCrp->push_back(string(buffer));

        if( Update )
          {
             if( iAutoZ )
				 sprintf_s(buffer, sizeof(buffer), "廢水||%02d||%8ld||", 0, Cpn75_amt + i2);
             else
				 sprintf_s(buffer, sizeof(buffer), "廢水||%02d||%8ld||", 0, 0);
			 m_IniReader.WriteString("AutoX", "xInkPaks", string(buffer));  // AutoCrpIni->WriteString("AutoX","xInkPaks",strtmp);
          }

      }

   strtmp = m_IniReader.ReadString("AutoX", "xPwrPaks", "0");   //strtmp=AutoCrpIni->ReadString("AutoX","xInkPaks","0");
   i1 = _StrToInt(_StringSegment_EX(strtmp, SGM, 2));
   i2 = _StrToInt(_StringSegment_EX(strtmp, SGM, 3));
   if (Cpn76_amt + i2)
   {
       // 2019/11/27 Update   20200409  
       sprintf_s(buffer, sizeof(buffer), "%s廢行動電源袋數:%5ld  ", CmdStr.c_str(), Cpn76_amt + i2);   tslCrp->push_back(string(buffer));

       if (Update)
       {
           if (iAutoZ)
               sprintf_s(buffer, sizeof(buffer), "廢行||%02d||%8ld||", 0, Cpn76_amt + i2);
           else
               sprintf_s(buffer, sizeof(buffer), "廢行||%02d||%8ld||", 0, 0);
           m_IniReader.WriteString("AutoX", "xPwrPaks", string(buffer));  // AutoCrpIni->WriteString("AutoX","xInkPaks",strtmp);
       }

   }


   /***
   strtmp.sprintf("                             收銀員簽章");  tslCrp->Add(strtmp);
   strtmp.sprintf("                            ┌────-┐");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            │         │");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            │         │");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            │         │");   tslCrp->Add(strtmp);
   strtmp.sprintf("                            └────-┘");   tslCrp->Add(strtmp);
   ****/
   //sprintf(outfile,"  【此單據請放入投庫袋】");  msg_to_tkt(outfile);
   if ( Trim(PrintAutoXsMissRec) != "")
   {
	   //紙本作廢發 明細  2019/04/26
	   sprintf_s(buffer, sizeof(buffer), "--------紙本作廢發票明細----------");  tslCrp->push_back(string(buffer));
	   sprintf_s(buffer, sizeof(buffer), "序  交易序號     發票號碼");  tslCrp->push_back(string(buffer));
	   i2 = 0;
	   while (1)
	   {
		   i2++;
		   tmpSer = _StringSegment_EX(PrintAutoXsMissRec, SGM, i2-1);
		   if (Trim(tmpSer) == "" || i2>500) break;
		   sprintf_s(buffer, sizeof(buffer), "%d.%s", i2, tmpSer.c_str() );  tslCrp->push_back(string(buffer));

	   }  // end of while
	   sprintf_s(buffer, sizeof(buffer), "(作廢發票明細不包含卡紙重印)");  tslCrp->push_back(string(buffer));
	  
   }

   sprintf_s(buffer, sizeof(buffer), "\n\n\n\n");  tslCrp->push_back(string(buffer));
   sprintf_s(buffer, sizeof(buffer), "\x1b\x69");  tslCrp->push_back(string(buffer)); //切紙

    //輸出檔案
   //if (FileExists(SCasherRpt.c_str() ))
	 if (PathFileExists(StringToWString(SCasherRpt).c_str()))
		   DeleteFile(StringToWString(SCasherRpt).c_str());
	    
   //_ListToFile(tslCrp, SCasherRpt.c_str());
   _ListToWFile(tslCrp, SCasherRpt); //Unicode

   /**
    RtnSumCasherCrp="";
    for (int i=0;  i<tslCrp->Count; i++)
          {
           strtmp=tslCrp->Strings[i]+"\n";
           RtnSumCasherCrp+=strtmp;
           }
    **/

   delete tslCrp;
   //delete AutoCrpIni;

   //if( iAutoZ==0 ) //非自動日結
   //     DeleteFile("C:\\FTLPOS\\XZDATA\\AutoCrp.ini");

   return 0;
}



// 1051電文
// 紙本作廢發票集計
void __fastcall BASIC::S1051MissCnt(int StartLine, int TotalLine, string &sMissRec)
{
     giA = giB = giC = giD = 0;

     string str_miss_type, tmpSer, strtmp, tmpVsNo, s ;
	 int iTmpB, iTmpC; // , iTmpD;

     if (gbl_1051_haveline)
     {
        //for(int i=0; i<gtsl_1051_sal->Count; i++)
		 for (list<string>::iterator ls = gtsl_1051_sal->begin(); ls != gtsl_1051_sal->end(); ls++)
        {
            iTmpB = iTmpC = 0;
			s = *ls; // gtsl_1051_sal->Strings[i];
            str_miss_type = _StringSegment_EX(s, "|",8);  //結帳型態(V1,Z0,R2,R3...)

            tmpSer=_StringSegment_EX(s, "|",11);     //作廢交易序號

            strtmp=_StringSegment_EX(s, "|", 12);   //作廢發票號碼起
            tmpVsNo=strtmp;  //.SubString(3,8);

            // 2020/05/25 Lu Update
            strtmp = _StringSegment_EX(s, "|", 18);   //保留 20 Byte
            if (strtmp.substr(0, 1) == "1")
                continue;

            if( Trim(tmpVsNo)=="" || tmpVsNo.substr(2,8)=="00000000" )
              {
				//  logsprintf("不開發票作廢交易:(%s)..", s.c_str());
               //writeloglog(strtmp);
               continue;
              }


			if (_StrFind(str_miss_type, "R") )
            {
                //if( Trim(tmpVsNo)!="" && tmpVsNo!="00000000" )
                //{
                        strtmp=_StringSegment_EX(s, "|",15);
                        iTmpB=_StrToInt(Trim(strtmp) );     //使用載具區分  '0'未使用載具 '1'使用載具

                        if( iTmpB ==0)
                        {
                        giA++;
						if (_StrFind(str_miss_type, "R2") || _StrFind(str_miss_type, "R3"))
						{
							giC++;
							_Strsprintf(strtmp, "%10s   %-2s-%-8s|", tmpSer.substr(2, 8).c_str(), tmpVsNo.substr(0, 2).c_str(), tmpVsNo.substr(2, 8).c_str());
						    sMissRec += strtmp;
						}
						
						}
                        else
                        {
                        giB++;
                        }
                //}
            }

             
        }
		 logsprintf("S1051MissCnt:紙本作廢發票張數(%d), 載具作廢發票張數(%d), (%s) ", giA, giB, sMissRec.c_str());
    }
 }

