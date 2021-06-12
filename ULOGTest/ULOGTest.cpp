// ULOGTest.cpp : Defines the entry point for the application.
//

// disable .sdf, ipch file: Tools > Options > Text Editor C/C++ > Advanced
// Disable Database ==> true
// Fallback Location -> "Always use Fallback Location" ==> true
//                      "Do Not Warn if Fallback Location Used" ==> true

#include "stdafx.h"
#include "ULOGTest.h"
//#include "..\\XZdataApi\\XZdataApi.h"
#include "..\\XZdataApi\\Lcommon.h"



//#pragma comment (lib, "..\\Debug\\TP720BatteryLib.lib")
//#pragma comment (lib, "..\\Debug\\XZdataApi.lib")
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Dialog1(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ULOGTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ULOGTEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ULOGTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ULOGTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog1);

			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_ULOG_INIT:
			ULogInit_DATA();
			break;
		case ID_ULOG_XREPORT:
			ULog_XREPORT();
			break;
		case ID_ULOG_CHECKIN:
			ULog_CHECKIN();
			break;
		case ID_ULOG_SPCDATA:
			ULog_SPCDATA();
			break;
		case ID_ULOG_VXZDATA:
			ULog_VXZDATA();
			break;
		case ID_REPORT_XDATA:
			ULog_XDATA();
			break;
		case ID_REPORT_ZDATA:
			ULog_ZDATA();
			break;
		case ID_REPORT_AUTOZDATA:
			ULog_AUTOZDATA();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



// Message handler for Dialog1 box.
INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//////////////////////////////////////////  My Test Function Section ///////////////////////////////////////////////////////////////

int ULogInit_DATA()
{
	string stest, str, ustr;   //非Unicode
	wstring wstr, wsustr;              //Unicode 通用字串

	/*stest = "0";*/
	//stest = "1010 | 0563 | 019989 | 01 | 20171228184408 | 01153 | 0028318719 | A0 | 000 | 00 | | XY99900065 | 0000021800000 | | | | M999999 | M121704 | 六龜 - 農會測試商品０元 | 02012000 | | | 0000000200 | 0000019200 | 0000019200 | 0001 | PS | 0000000200 | 0000000200 | 0000000000 | 0000000000 | 0000000000 | 000 | 000000 | 1 | 17110 | 11000000 | Z0 | 0000000000 | | | | | | 0 | |";
	stest = "xxx|||||1010 || M999999 || M121704 || 六龜 - 農會測試商品０元 ||";
	wstr = L"1010 || M999999 || M121704 ||堃 六龜 - 農會測試商品０元 ||";
	
	wsustr = _WStringSegmentW(wstr, L"||", 4, 0);
	stest= WStringToString(wstr);

	str = _WStringSegment(stest, "||", 1);
	str = _WStringSegment(stest, "||", 2);
	str = _WStringSegment(stest, "||", 3);
	str = _WStringSegment(stest, "||", 4);
	str = _WStringSegment(stest, "||", 5);
	str = _WStringSegment(stest, "||", 6);
	str = _WStringSegment(stest, "||", 7);
	str = _WStringSegment(stest, "||", 8);
	str = _WStringSegment(stest, "||", 9);
	str = _WStringSegment(stest, "|", 17);
	str = _WStringSegment(stest, "|", 18);
	str = _WStringSegment(stest, "|", 19);
	str = _WStringSegment(stest, "|", 20);
	str = _WStringSegment(stest, "|", 21);
    
	/*char ctmp[] = "12345678901234567890";
	char ctmp1[120];
	sprintf(ctmp1, "%5.5s", &ctmp[5]);*/


	/*wstring wstr;
	byte StoreNO[] = "000001";
	byte EcrNO[] = "000001";
	byte SalesNo[] = "000001";
	byte SalFileName[] = "C:\\aaaa\\bbbb\\sss.txt";
	byte Version[] = "V.000001";
	TestWVXZ(StoreNO,EcrNO,SalesNo, SalFileName, Version);
	return(0);
	*/

	//wstr = L"c:\\ftlpos\\xzdata\\021_UTF.sal";
	//_WOpenFile(wstr, L"r,ccs=UTF-8");

	//return 0;

	//str = "1234567890六龜 - 農會測試商品０元";
	str = "1堃0";  //UTF8堃:e5a083  
	//wstr = L"1堃0";
	//wstr = string_To_UTF8(str);
	//str = _UnicodeToUTF8(wstr);
	wstr = _UTF8ToWUnicode(str);

	ustr=string_To_UTF8(str);

	return 0;

	//CXZdataApi cxz = new CXZdataApi();
	//nXZdataApi=fnXZdataApi_INIT();
	//int i = nXZdataApi;

	//currentDateTime(2, -1);

	//stest = "1010 | 0563 | 019989 | 01 | 20171228184408 | 01153 | 0028318719 | A0 | 000 | 00 | | XY99900065 | 0000021800000 | | | | M999999 | M121704 | 六龜 - 農會測試商品０元 | 02012000 | | | 0000000200 | 0000019200 | 0000019200 | 0001 | PS | 0000000200 | 0000000200 | 0000000000 | 0000000000 | 0000000000 | 000 | 000000 | 1 | 17110 | 11000000 | Z0 | 0000000000 | | | | | | 0 | |";
	//stest = "xxx|||||1010 | M999999 | M121704 | 六龜 - 農會測試商品０元 |";
	//wstr = L"1010 | M999999 | M121704 | 六龜 - 農會測試商品０元 |";

	//size_t isize, iLen = 0;

	//wstr = StringToWString(stest);

	//str ="六龜 - 農會測試商品０元|";
	//isize = str.size();
	//isize = str.length();

	//size_t pos_start = 0;
	//	pos_start = strlen("六龜 - 農會測試商品０元|");

	//	pos_start = _mbslen((unsigned char *)str.c_str() );

	//	unsigned char uchr='|';

	//	//str = string((char *)_mbschr((unsigned char *)str.c_str(), &uchr));
	//	pos_start = _mbscspn((unsigned char *)str.c_str(), &uchr);
			
		//return(0);

		//str=_WStringSegment(stest, "|", 1, 0);

	//split(stest, '|');

	

	/*SPCDATA *AllSpcData = new SPCDATA(); OK
	int i = AllSpcData->WriteSpcInqData("c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999", 0);  */
	
	//VXZDATA *vxzdata = new VXZDATA;     //盤點人員讀帳
	//vxzdata->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999");

	/*CHECKIN *checkin = new CHECKIN; OK
	checkin->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999");*/

	/*XDATA *xdata = new XDATA(); 
	xdata->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999");*/

	//ZDATA *zdata = new ZDATA();         //日結
	//zdata->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999", ""); 

//#ifdef _DEBUG  //_NDEBUG
//	AUTOZDATA *autozdata = new AUTOZDATA();         //自動日結
//	autozdata->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999", "");
//	//stest=_ExtractFilePath("c:\\FTLPOS\\XZDATA\\02.sal");
//#else
//	AUTOZDATA *autozdata = new AUTOZDATA();         //自動日結
//	autozdata->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999", "");
//	//stest=_ExtractFilePath("c:\\FTLPOS\\XZDATA\\02.sal"); 
//#endif


//#if _DEBUG
//	AUTOZDATA *autozdata = new AUTOZDATA();         //自動日結
//#endif
	/*TStringList *lsFileList = new TStringList();
	wstring wstr = L"c:\\ftlpos\\XZDATA";
	_GetFileList(lsFileList, wstr,  L"*.*",  1,  0);*/

	//wstr = L"C:\\FTLPOS\\HIS\\16";
	//_DeleteDirectory(wstr);

	
	

	//pXdata->BackupFile("C:\\FTLPOS\\1102.sal", "", "141"); //檔案備份
	                          //取得POS基本資料

	//XREPORT *xreport = new XREPORT(); //讀帳
	//stest = "c:\\ftlpos\\XZDATA\\02.sal";
	//xreport->loadsalfile(stest);

	//pXdata->sub_times(0, pXdata->gi_rec_line);   //代收、售、付次數統計

	return 0;
}

int ULog_XREPORT()
{
	int irtn;
	XREPORT *xreport = new XREPORT(); //讀帳 XREPORT::WriteData
	//irtn = xreport->WriteData("012731", "02", "00320", "c:\\FTLPOS\\XZDATA\\02_UTF8.sal", "V99.9999");
	irtn = xreport->WriteData("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\011U.sal", "V99.9999");
	return irtn;
}

int ULog_CHECKIN()
{
	int irtn;
	CHECKIN *checkin = new CHECKIN;   //擔當
	irtn=checkin->WriteData("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\30UTF8.sal", "V99.9999");

	return irtn;
}

int ULog_SPCDATA()
{
	int irtn;
	SPCDATA *AllSpcData = new SPCDATA();  //重點商品
	irtn = AllSpcData->WriteSpcInqData("c:\\FTLPOS\\XZDATA\\30UTF8.sal", "V99.9999", 0);

	return irtn;
}

int ULog_VXZDATA()
{
	int irtn;

	VXZDATA *vxzdata = new VXZDATA;     //盤點人員讀帳
	irtn = vxzdata->WriteData("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\30UTF8.sal", "V99.9999");
	return irtn;
}

int ULog_XDATA()
{
	int irtn;

	XDATA *xdata = new XDATA();  //交班  OK
	irtn=xdata->WriteData("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\01.sal", "V99.9999");
	wx_ok("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\01.sal", "V99.9999");
	return irtn;
}

int ULog_ZDATA()
{
	int irtn;

	ZDATA *zdata = new ZDATA();         //日結  OK
	irtn = zdata->WriteData("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\011.sal", "V99.9999", "");
	wz_ok("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\011.sal", "V99.9999");

	return irtn;
}

int ULog_AUTOZDATA()
{
	int irtn;

		AUTOZDATA *autozdata = new AUTOZDATA();         //自動日結  OK
		irtn = autozdata->WriteData("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\30UTF8.sal", "V99.9999", "");
		//wz_ok("019989", "03", "99999", "c:\\FTLPOS\\XZDATA\\02.sal", "V99.9999");
		return irtn;
}


