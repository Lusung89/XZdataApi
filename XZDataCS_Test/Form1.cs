using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using XZDataCSTest;

namespace XZDataCS_Test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// test file path
        /// </summary>
        private string XZDataTestPath
        {
            get { return "C:\\Ftlpos\\XZData\\XZDataTest.txt"; }
        }

        private string SalFileName
        {
            get { return @"c:\FTLPOS\XZDATA\30UTF8.sal"; }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            int temp;

            if (File.Exists(this.XZDataTestPath))
            {
                File.Delete(this.XZDataTestPath);
            }

            temp = XZdataApi.XZdataApi_INIT();
            File.WriteAllText(XZDataTestPath, temp.ToString());


            ////////////////////////////////////////////////////////////
            string StoreNO = "019989";
            string EcrNO = "03";
            string SalesNo = "99999";
            //string SalFileName = @"c:\FTLPOS\XZDATA\30UTF8.sal";
            string Version = "V99.9999";

            byte[] byStoreNo = Encoding.ASCII.GetBytes(StoreNO);
            byte[] byEcrNO = Encoding.ASCII.GetBytes(EcrNO);
            byte[] bySalesNO = Encoding.ASCII.GetBytes(SalesNo);
            byte[] bySalFileName = Encoding.ASCII.GetBytes(SalFileName);
            byte[] byVersion = Encoding.ASCII.GetBytes(Version);

            ////////////////////////////////////////////////////////////

            textBox1.Clear();
            textBox1.AppendText("讀帳 SaleFile:" + SalFileName);

            temp = XZdataApi.WXReport(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion); //讀帳
            //File.WriteAllText(XZDataTestPath, "XZdataApi.WXReport:" + temp.ToString());
            textBox1.AppendText("\nXZdataApi.WXReport:Return = " + temp.ToString() );

            // temp = XZdataApi.WX(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion); //收銀員帳表, 交班
            // File.WriteAllText(XZDataTestPath, "XZdataApi.WX:" + temp.ToString());

            //日結 AutoZ=0, 自動日結 AutoZ=1;
            //temp = XZdataApi.WZ(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion, AutoZ); 
            //File.WriteAllText(XZDataTestPath, "XZdataApi.WZ:" + temp.ToString());


        }

        private void button2_Click(object sender, EventArgs e)
        {
            int temp;

            if (File.Exists(this.XZDataTestPath))
            {
                File.Delete(this.XZDataTestPath);
            }

            temp = XZdataApi.XZdataApi_INIT();
            File.WriteAllText(XZDataTestPath, temp.ToString());


            ////////////////////////////////////////////////////////////
            string StoreNO = "019989";
            string EcrNO = "03";
            string SalesNo = "99999";
            //string SalFileName = @"c:\FTLPOS\XZDATA\30UTF8.sal";
            string Version = "V99.9999";

            byte[] byStoreNo = Encoding.ASCII.GetBytes(StoreNO);
            byte[] byEcrNO = Encoding.ASCII.GetBytes(EcrNO);
            byte[] bySalesNO = Encoding.ASCII.GetBytes(SalesNo);
            byte[] bySalFileName = Encoding.ASCII.GetBytes(SalFileName);
            byte[] byVersion = Encoding.ASCII.GetBytes(Version);

            ////////////////////////////////////////////////////////////
            textBox1.Clear();
            textBox1.AppendText("交班 SaleFile:" + SalFileName);

            temp = XZdataApi.WX(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion); //收銀員帳表, 交班
            File.WriteAllText(XZDataTestPath, "\nXZdataApi.WX:" + temp.ToString());

            //日結 AutoZ=0, 自動日結 AutoZ=1;
            //temp = XZdataApi.WZ(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion, AutoZ); 
            //File.WriteAllText(XZDataTestPath, "XZdataApi.WZ:" + temp.ToString());
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int temp, AutoZ;

            if (File.Exists(this.XZDataTestPath))
            {
                File.Delete(this.XZDataTestPath);
            }

            temp = XZdataApi.XZdataApi_INIT();
            File.WriteAllText(XZDataTestPath, temp.ToString());


            ////////////////////////////////////////////////////////////
            string StoreNO = "019989";
            string EcrNO = "03";
            string SalesNo = "99999";
            //string SalFileName = @"c:\FTLPOS\XZDATA\30UTF8.sal";
            string Version = "V99.9999";

            byte[] byStoreNo = Encoding.ASCII.GetBytes(StoreNO);
            byte[] byEcrNO = Encoding.ASCII.GetBytes(EcrNO);
            byte[] bySalesNO = Encoding.ASCII.GetBytes(SalesNo);
            byte[] bySalFileName = Encoding.ASCII.GetBytes(SalFileName);
            byte[] byVersion = Encoding.ASCII.GetBytes(Version);

            ////////////////////////////////////////////////////////////
            textBox1.Clear();
            textBox1.AppendText("日結 SaleFile:" + SalFileName);

            //日結 AutoZ=0, 自動日結 AutoZ=1;
            AutoZ = 0;
            temp = XZdataApi.WZ(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion, AutoZ); 
            File.WriteAllText(XZDataTestPath, "\nXZdataApi.WZ:" + temp.ToString());

        }
    }
}



/*****
byte[] byStoreNo = Encoding.ASCII.GetBytes(StoreNO);
byte[] byEcrNO = Encoding.ASCII.GetBytes(EcrNO);
byte[] bySalesNO = Encoding.ASCII.GetBytes(SalesNo);
byte[] bySalFileName = Encoding.ASCII.GetBytes(SalFileName);
byte[] byVersion = Encoding.ASCII.GetBytes(Version);

int nFlag = WVXZ(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion);



******/