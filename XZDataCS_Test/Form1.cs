using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
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
            textBox1.Clear();
            textBox1.AppendText("日結開始..");
            if (File.Exists(this.XZDataTestPath))
            {
                File.Delete(this.XZDataTestPath);
                textBox1.Text += (Environment.NewLine + "Delete file:" + this.XZDataTestPath);
            }

            temp = XZdataApi.XZdataApi_INIT();
            //File.WriteAllText(XZDataTestPath, temp.ToString());

            ////////////////////////////////////////////////////////////
            string StoreNO = "";
            string EcrNO = "";

            if (GetSalInfo(lb_FilePath.Text, ref StoreNO, ref EcrNO) <= 0)
            {
                textBox1.Text += (Environment.NewLine + "Open file: Error " + lb_FilePath.Text);
                return;
            }
            else
            {
                textBox1.Text += (Environment.NewLine + "Open file:" + lb_FilePath.Text);
            }

            string SalFileName = lb_FilePath.Text;  //@"c:\FTLPOS\XZDATA\30UTF8.sal";
            string SalesNo = "99999";
            string Version = "V99.9999";

            byte[] byStoreNo = Encoding.ASCII.GetBytes(StoreNO);
            byte[] byEcrNO = Encoding.ASCII.GetBytes(EcrNO);
            byte[] bySalesNO = Encoding.ASCII.GetBytes(SalesNo);
            byte[] bySalFileName = Encoding.ASCII.GetBytes(SalFileName);
            byte[] byVersion = Encoding.ASCII.GetBytes(Version);

            ////////////////////////////////////////////////////////////
            
            textBox1.AppendText(Environment.NewLine + "日結 SaleFile:" + SalFileName);

            //日結 AutoZ=0, 自動日結 AutoZ=1;
            textBox1.AppendText(Environment.NewLine + "日結 XZdataApi.WZ: StoreNo=" + StoreNO + ", EcrNo = "+ EcrNO + ", SalFileName = " + SalFileName);
            AutoZ = 0;
                       
            temp = XZdataApi.WZ(byStoreNo, byEcrNO, bySalesNO, bySalFileName, byVersion, AutoZ); 
            File.WriteAllText(XZDataTestPath, "\nXZdataApi.WZ:" + temp.ToString());

            if (temp > 0)
            {
                XZdataApi.WZ_OK(byEcrNO, bySalesNO, bySalesNO, bySalFileName, byVersion, true);
                textBox1.AppendText(Environment.NewLine + "日結 XZdataApi.WZ_OK: StoreNo=" + StoreNO + ", EcrNo = " + EcrNO + ", SalFileName = " + SalFileName);
            }
            else
            {
                textBox1.AppendText(Environment.NewLine + "日結失敗!!");
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (lb_FilePath.Text.Trim() == "")
                lb_FilePath.Text = "c:\\";

            openFileDialog1.InitialDirectory = lb_FilePath.Text;
            openFileDialog1.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;
        }

        private void btn_LoadFile_Click(object sender, EventArgs e)
        {
            var fileContent = string.Empty;
            var filePath = string.Empty;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                //Get the path of specified file
                filePath = openFileDialog1.FileName;
                lb_FilePath.Text = filePath;

                //Read the contents of the file into a stream
                //var fileStream = openFileDialog1.OpenFile();

                //using (StreamReader reader = new StreamReader(fileStream))
                //{
                //    fileContent = reader.ReadToEnd();
                //}
            }
        }

        private int  GetSalInfo(string filePath,ref string store_no, ref string Ecr_no)
        {
            int irtn = 0;
            var fileContent = string.Empty;
            var LineContent = string.Empty;

            //Read the contents of the file into a stream
            var fileStream = filePath;

            try
            {
                using (StreamReader reader = new StreamReader(fileStream))
                {
                    //fileContent = reader.ReadToEnd();
                    if ((LineContent = reader.ReadLine()) != null)
                        irtn = 1;
                    reader.Close();
                }

                //string sp = "||";
                //char[] charArr = sp.ToCharArray();
                //string[] words = LineContent.Split('|');

                string[] separatingStrings = { "||" };
                string[] words = LineContent.Split(separatingStrings, System.StringSplitOptions.RemoveEmptyEntries);
                store_no = words[2];
                Ecr_no = words[3];
            }
            catch (FileNotFoundException e)
            {
                textBox1.AppendText(Environment.NewLine + "GetSalInfo: FileNotFoundExceptions");
                // FileNotFoundExceptions are handled here.
            }
            catch (IOException e)
            {
                // Extract some information from this exception, and then
                // throw it to the parent method.
                if (e.Source != null)
                    textBox1.AppendText(Environment.NewLine + "GetSalInfo: IOException source: " + e.Source);
                //throw;
            }
            catch  
            {
                textBox1.AppendText(Environment.NewLine + "GetSalInfo: Error..");
            }


            // while ((line = file.ReadLine()) != null)
            // {
            //     System.Console.WriteLine(line);
            //     counter++;
            // }

            return irtn;
        }

        private string SalSplit( string saldata, int idx)
        {
            string rtndata = "";
            string[] separatingStrings = { "||" };
            string[] words = saldata.Split(separatingStrings, System.StringSplitOptions.RemoveEmptyEntries);

            rtndata = words[idx];

            //string[] separatingStrings = { "<<", "..." };
            //string text = "one<<two......three<four";
            //string[] words = text.Split(separatingStrings, System.StringSplitOptions.RemoveEmptyEntries);
            return rtndata;
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