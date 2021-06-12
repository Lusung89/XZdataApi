using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

//D:\2020NextPOS\SRC\XZDATA\XZdataApi\x64\Release\XZdataApi.dll   ==> x64
//D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll  ==>x32


namespace XZDataCSTest
{    public static class XZdataApi
    {
        //static String dllFile = @"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll";
        public const string DLLFILENAME = @"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll";

        [DllImport(DLLFILENAME,  EntryPoint = "XZdataApi_INIT") ]
        public static extern int XZdataApi_INIT();

        //收銀員帳表 , 交班
        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "WX")]
        public static extern int WX(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName, 
                                    byte[] Version);

        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "WX_OK")]
        public static extern int WX_OK(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                    byte[] Version);

        //日結
        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "WZ")]
        public static extern int WZ(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                    byte[] Version, int AutoZ);

        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\x64\Release\XZdataApi.dll", EntryPoint = "WZ_OK")]
        public static extern int WZ_OK(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                           byte[] Version, bool delFolder);

        //讀帳
        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "WXReport")]
        public static extern int WXReport(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                    byte[] Version);

        //擔當同交班
        /// <summary> CHECKIN::WriteData
        /// 擔當同交班  報表Class
        /// </summary>
        /// <param name=""></param>
        /// <returns>int</returns>
        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "Checkin")]
        public static extern int Checkin(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                    byte[] Version);
 
        //重點商品列管帳表
        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "WSPC")]
        public static extern int WSPC(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                           byte[] Version, int iQryType);

        //盤點人員讀帳
        [DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll", EntryPoint = "WVXZ")]
        public static extern int WVXZ(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName,
                                    byte[] Version);


    }
}
//D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll
//D:\2020NextPOS\SRC\XZDATA\XZdataApi\x64\Debug\XZdataApi.dll
//[DllImport(@"D:\2020NextPOS\SRC\XZDATA\XZdataApi\Release\XZdataApi.dll",
//                   EntryPoint = "XZdataApi_INIT", CallingConvention = CallingConvention.Cdecl)] 

//  [DllImport(@"C:\FTLPOS\XZDATA\XZData.dll", EntryPoint = "WVXZ", SetLastError = true, CharSet = CharSet.Auto)]
//public static extern int WVXZ(byte[] StoreNO, byte[] EcrNO, byte[] SalesNo, byte[] SalFileName, byte[] Version);
//public string ExecuteWVXZ(String StoreNO, String EcrNO, String SalesNo, String SalFileName, String Version)

