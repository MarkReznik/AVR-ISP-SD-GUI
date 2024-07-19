using MainProgram;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace AvrStandalonIspProg
{
    public class FuseStruct
    {
        public string[] Text = new string[8];
        public string[] Name = new string[8];
        public bool[] IsChecked = new bool[8];
        public bool[] IsEnabled = new bool[8];
    }
    public class PartStruct
    {
        public string Part_Name;
        public string Signature;
        public string Prog_Flash;
        public string Prog_Eeprom;
        public string FlashPageSize;
        public string EepromPageSize;
        public string LockBits;
        public string LockBitsMask;
        public string HighFuseDef;
        public string LowFuseDef;
        public string ExtFuseDef;        
        public string HighFuseMask;
        public string LowFuseMask;
        public string ExtFuseMask;
        public FuseStruct cb_fuse_low = new FuseStruct();
        public FuseStruct cb_fuse_high = new FuseStruct();
        public FuseStruct cb_fuse_ext = new FuseStruct();
        public FuseStruct cb_lock_bits = new FuseStruct();
        public PartStruct()
        {
            Part_Name = "";
            Signature = "";
            Prog_Flash = "";
            Prog_Eeprom = "";
            FlashPageSize = "";
            EepromPageSize = "";
            LockBits = "";
            LockBitsMask = "";
            HighFuseDef="";
            LowFuseDef="";
            ExtFuseDef="";
            HighFuseMask = "";
            LowFuseMask = "";
            ExtFuseMask="";
            //cb_fuse_low = new CheckBox[8];
            //cb_fuse_high = new CheckBox[8];
            //cb_fuse_ext = new CheckBox[8];
            //cb_lock_bits = new CheckBox[8];
        }
    }
    public class XmlFinder
    {
        /*
        <PART_NAME>ATmega8</PART_NAME>
        <SIGNATURE>
            <ADDR000>$1E</ADDR000>
            <ADDR001>$93</ADDR001>
            <ADDR002>$07</ADDR002>
        </SIGNATURE>
        <PROG_FLASH>8192</PROG_FLASH>       In datasheet Flash  size 4K words or 8K bytes -> 8192 bytes
        <EEPROM>512</EEPROM>                In datasheet EEPROM size 512 bytes
        <FlashPageSize>64</FlashPageSize>   In datasheet Page   size 32 words -> 64 bytes
        <EepromPageSize>4</EepromPageSize>  In datasheet Page   size 4 bytes
        <LOCKBIT>
             <NMB_LOCK_BITS>6</NMB_LOCK_BITS>
         */
        public List<PartStruct> ReadDeviceInfoFromXmls(BackgroundWorker worker, DoWorkEventArgs e, string pathOfXmls)
        {
            String str1 = String.Empty;
            String str2 = String.Empty;
            String str_txt = String.Empty;
            String strval1 = String.Empty;
            String strval2 = String.Empty;
            string[] fileEntries=null;
            List<PartStruct> partlist = new List<PartStruct>();
            MainProgram.windowForm1.Texts texts = new MainProgram.windowForm1.Texts();
            texts.lblText = "Reading all devices from xml files.";
            worker.ReportProgress(1, texts);
            // Get the current directory.
            //string pathOfXmls = @"C:\Program Files\Atmel\AVR Tools\Partdescriptionfiles";//Directory.GetCurrentDirectory() + "\\Partdescriptionfiles";
            //MessageBox.Show("You selected: " + pathOfXmls);
            //return "";
            
            if (Directory.Exists(pathOfXmls))
            {
                // This path is a directory
                // Process the list of files found in the directory.
                fileEntries = Directory.GetFiles(pathOfXmls,"*.xml");
            }
            else
            {
                
                Console.WriteLine("{0} is not a valid file or directory.", pathOfXmls);
                return null;
            }
            int j=0;
            foreach (var item in fileEntries)
            {
                //MessageBox.Show("You selected: " + item);
                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }
                texts.lblText = "Reading from "+ item.Substring(item.LastIndexOf("\\")+1) + " " + ((100 * j) / fileEntries.Length) + "%";
                worker.ReportProgress((100 * j++) / fileEntries.Length, texts);
                XmlDocument xmldoc = new XmlDocument();
                string xmlFile = File.ReadAllText(item);
                PartStruct newpart = new PartStruct();
                xmldoc.LoadXml(xmlFile);                
                newpart.Part_Name = xmldoc.SelectSingleNode("/AVRPART/ADMIN/PART_NAME").InnerText;
                if (newpart.Part_Name.Contains("ATm") == false &&
                    newpart.Part_Name.Contains("ATt") == false)
                        continue;
                newpart.Prog_Flash = xmldoc.SelectSingleNode("/AVRPART/MEMORY/PROG_FLASH").InnerText;                
                newpart.Signature = xmldoc.SelectSingleNode("/AVRPART/ADMIN/SIGNATURE/ADDR000").InnerText.Remove(0,1)+
                    xmldoc.SelectSingleNode("/AVRPART/ADMIN/SIGNATURE/ADDR001").InnerText.Remove(0,1)+
                    xmldoc.SelectSingleNode("/AVRPART/ADMIN/SIGNATURE/ADDR002").InnerText.Remove(0,1);
                newpart.Prog_Eeprom = xmldoc.SelectSingleNode("/AVRPART/MEMORY/EEPROM").InnerText;
                try
                {
                    newpart.FlashPageSize = xmldoc.SelectSingleNode("/AVRPART/PROGRAMMING/FlashPageSize").InnerText;
                }
                catch (Exception)
                {
                    
                    //throw;
                }
                
                try
                {
                    newpart.EepromPageSize = xmldoc.SelectSingleNode("/AVRPART/PROGRAMMING/EepromPageSize").InnerText;
                }
                catch (Exception)
                {
                    
                    //throw;
                }
                ushort mask = 0;
                ushort temp_mask = 0;
                ushort deffuse = 0;
                string fusestr="";
                try
                {
                    newpart.LockBits = xmldoc.SelectSingleNode("/AVRPART/LOCKBIT/NMB_LOCK_BITS").InnerText;
                    deffuse = UInt16.Parse(newpart.LockBits);
                    deffuse = (ushort)(Math.Pow(2, deffuse));// exm: <NMB_LOCK_BITS>6</NMB_LOCK_BITS> 2^6=0b 0100 0000
                    for (int i = 0; i < 8; i++)
                    {
                        try
                        {
                            newpart.cb_lock_bits.IsEnabled[i] = false;
                            newpart.cb_lock_bits.IsChecked[i] = false;
                            newpart.cb_lock_bits.Text[i] = xmldoc.SelectSingleNode("/AVRPART/LOCKBIT/LOCKBIT" + i.ToString() + "/NAME").InnerText;
                            newpart.cb_lock_bits.Name[i] = xmldoc.SelectSingleNode("/AVRPART/LOCKBIT/LOCKBIT" + i.ToString() + "/TEXT").InnerText;
                            //newpart.cb_lock_bits.IsChecked[i] = false;
                            newpart.cb_lock_bits.IsEnabled[i] = true;
                        }
                        catch (Exception)
                        {
                            //throw;
                        }
                    }
                    deffuse-=1;// 2^6 - 1 = 0b 0100 0000 - 1=0b 0011 1111
                    newpart.LockBits = "0x" + deffuse.ToString("X2");
                    newpart.LockBitsMask = newpart.LockBits;
                }
                catch (Exception)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        newpart.cb_lock_bits.IsEnabled[i] = false;
                        newpart.cb_lock_bits.IsChecked[i] = false;
                    }
                    newpart.LockBits = "0x00";
                    newpart.LockBitsMask = "0x00";
                    //throw;
                }
                
                try
                {
                    newpart.LowFuseDef = xmldoc.SelectSingleNode("/AVRPART/FUSE/LOW/NMB_FUSE_BITS").InnerText;
                    fusestr = "LOW";
                }
                catch (Exception)
                {
                    try
                    {
                        newpart.LowFuseDef = xmldoc.SelectSingleNode("/AVRPART/FUSE/BYTE0/NMB_FUSE_BITS").InnerText;
                        fusestr = "BYTE0";
                    }
                    catch (Exception)
                    {
                        newpart.LowFuseDef = "00";
                        newpart.LowFuseMask = "00";
                        continue;
                        //throw;
                    }
                    
                    //throw;
                }
                temp_mask = UInt16.Parse(newpart.LowFuseDef);
                temp_mask = (ushort)(Math.Pow(2, temp_mask));
                temp_mask -= 1;
                //newpart.LowFuseDef = "8";
                mask=0;
                deffuse=0;
                /* Try to find default fuses from FUSE0 FUSE1 ... sections */
                for (int i = 0; i < 8; i++)
			    {                   
                    try
                    {
                        newpart.cb_fuse_low.IsEnabled[i] = false;
                        newpart.cb_fuse_low.IsChecked[i] = false;
                        deffuse |= (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/"+fusestr+"/FUSE" + i.ToString() + "/DEFAULT").InnerText))<<i);
                        newpart.cb_fuse_low.Text[i]= xmldoc.SelectSingleNode("/AVRPART/FUSE/"+fusestr+"/FUSE" + i.ToString() + "/NAME").InnerText;
                        newpart.cb_fuse_low.Name[i] = xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/FUSE" + i.ToString() + "/TEXT").InnerText;
                        newpart.cb_fuse_low.IsEnabled[i] = true;
                        newpart.cb_fuse_low.IsChecked[i] = (deffuse & (1 << i)) == 0 ? true : false;
                        mask |= (ushort)(1 << i);
                    }
                    catch (Exception)
                    {
                        
                        //throw;
                    }                                        
			    }
                /* Try to find default fuses from TEXT1 TEXT2 ... sections */
                
                try
                {
                    ushort temp_deffuse=0;
                    ushort temp_deffuse_mask=0;
                    ushort save_temp_deffuse_mask = 0;
                    bool skip_bit = false;
                    int skip_bit_to_save = -1;
                    for (int i = 1; i <= (UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/"+fusestr+"/NMB_TEXT").InnerText)); i++)
			        {
                        temp_deffuse_mask = (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/MASK").InnerText.Replace("0x", ""), NumberStyles.AllowHexSpecifier)));
                        bool isdeffuse = xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/TEXT").InnerText.Contains("default value");
                        /* continue to check default value in case the bit is disabled */
                        skip_bit = true;
                        for (int i1 = 0; i1 < 8; i1++)
			            {
                            if (((temp_deffuse_mask & (1 << i1)) != 0) && (newpart.cb_fuse_low.IsEnabled[i1] == false || i1 == skip_bit_to_save))
                            {
                                newpart.cb_fuse_low.IsEnabled[i1] = true;
                                skip_bit_to_save = i1;
                                skip_bit = false;
                                //break;
                            }
			            }
                        if (skip_bit==true && isdeffuse==false)
                        {
                            continue;
                        }
                        /* Get the default fuse from 'default value' TEXT?? */
                        
                        if ((temp_deffuse_mask != save_temp_deffuse_mask) || isdeffuse)
                        {
                            save_temp_deffuse_mask = temp_deffuse_mask;
                            mask |= temp_deffuse_mask;
                            temp_deffuse = (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/VALUE").InnerText.Replace("0x", ""), NumberStyles.AllowHexSpecifier)));
                            
                            str_txt=xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/TEXT").InnerText;
                            
                            str_txt=str_txt.Substring(str_txt.IndexOf("[")+1,str_txt.IndexOf("]")-(str_txt.IndexOf("[")+1));
                            
                            str1=str_txt.Substring(0,str_txt.IndexOf("="));
                            if (str_txt.IndexOf(" ")>=0)
	                        {
                                strval1 = str_txt.Substring(str_txt.IndexOf("=") + 1, str_txt.IndexOf(" ") - (str_txt.IndexOf("=") + 1));
                                str2=str_txt.Substring(str_txt.IndexOf(" ")+1,str_txt.LastIndexOf("=")-(str_txt.IndexOf(" ")+1));
                                strval2=str_txt.Substring(str_txt.LastIndexOf("=")+1,str_txt.Length-(str_txt.LastIndexOf("=")+1));
	                        }
                            else
	                        {
                                strval1=str_txt.Substring(str_txt.IndexOf("=")+1,str_txt.Length-(str_txt.IndexOf("=")+1));
	                        }
                            int cntbit = 0;
                            bool strval1singlebit = strval1.Length <= 1 ? true : false;
                            bool strval2singlebit = strval2.Length <= 1 ? true : false;
                            for (int ii = 0; ii < 8; ii++)
                            {
                                if ((temp_deffuse_mask & (1 << ii)) != 0)
                                {                                   
                                    if (strval1.Length>0)
                                    {
                                        newpart.cb_fuse_low.Text[ii] = str1 + ((strval1singlebit==false)?cntbit.ToString():"");
                                        cntbit++;
                                        strval1 = strval1.Remove(0, 1);
                                        if (strval1.Length == 0)
                                        {
                                            cntbit = 0;
                                        }
                                    }
                                    else if (strval2.Length > 0)
                                    {
                                        newpart.cb_fuse_low.Text[ii] = str2 + ((strval2singlebit == false) ? cntbit.ToString() : "");
                                        cntbit++;
                                        strval2 = strval2.Remove(0, 1);
                                        if (strval2.Length == 0)
                                        {
                                            cntbit = 0;
                                        }
                                    }
                                    if ((temp_deffuse & (1 << ii)) == 0 && isdeffuse)
                                    {
                                        newpart.cb_fuse_low.IsChecked[ii] = true;
                                        deffuse &= (ushort)~((ushort)1 << ii);
                                    }
                                    else
                                    {
                                        newpart.cb_fuse_low.IsChecked[ii] = false;
                                        deffuse |= (ushort)((ushort)1 << ii);
                                    }
                                }
                            }
                            
                        }
                    }
                    
                }
                catch (Exception)
                {
                    
                    //throw;
                }
                newpart.LowFuseDef = "0x" + deffuse.ToString("X2");
                mask |= temp_mask;
                for (int i = 0; i < 8; i++)
                {
                    if((mask&(1<<i))!=0)
                        newpart.cb_fuse_low.IsEnabled[i] = true;
                }
                newpart.LowFuseMask = "0x" + mask.ToString("X2");
                
                /* Try parse high fuses from xml file */
                try
                {
                    newpart.HighFuseDef = xmldoc.SelectSingleNode("/AVRPART/FUSE/HIGH/NMB_FUSE_BITS").InnerText;
                    //newpart.HighFuseDef = "8";
                    temp_mask = UInt16.Parse(newpart.HighFuseDef);
                    temp_mask = (ushort)(Math.Pow(2, temp_mask));
                    temp_mask -= 1;
                    deffuse = 0;
                    mask = 0;
                    for (int i = 0; i < 8 ; i++)
                    {
                        try
                        {
                            newpart.cb_fuse_high.IsEnabled[i] = false;
                            newpart.cb_fuse_high.IsChecked[i] = false;
                            deffuse |= (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/HIGH/FUSE" + i.ToString() + "/DEFAULT").InnerText))<<i);
                            newpart.cb_fuse_high.Text[i] = xmldoc.SelectSingleNode("/AVRPART/FUSE/HIGH/FUSE" + i.ToString() + "/NAME").InnerText;
                            newpart.cb_fuse_high.Name[i] = xmldoc.SelectSingleNode("/AVRPART/FUSE/HIGH/FUSE" + i.ToString() + "/TEXT").InnerText;
                            newpart.cb_fuse_high.IsEnabled[i] = true;
                            newpart.cb_fuse_high.IsChecked[i] = (deffuse & (1 << i)) == 0 ? true : false;
                            mask |= (ushort)(1 << i);
                        }
                        catch (Exception)
                        {
                            //deffuse = 0;
                            //break;
                            //throw;
                        }
                    }
                    fusestr = "HIGH";
                    try
                    {
                        
                        ushort temp_deffuse = 0;
                        ushort temp_deffuse_mask = 0;
                        ushort save_temp_deffuse_mask = 0;
                        bool skip_bit = false;
                        int skip_bit_to_save = -1;
                        for (int i = 1; i <= (UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/NMB_TEXT").InnerText)); i++)
                        {
                            temp_deffuse_mask = (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/MASK").InnerText.Replace("0x", ""), NumberStyles.AllowHexSpecifier)));
                            bool isdeffuse = xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/TEXT").InnerText.Contains("default value");
                            
                            /* continue to check default value in case the bit is disabled */
                            skip_bit = true;
                            for (int i1 = 0; i1 < 8; i1++)
                            {
                                if (((temp_deffuse_mask & (1 << i1)) != 0) && (newpart.cb_fuse_high.IsEnabled[i1] == false || i1 == skip_bit_to_save))
                                {
                                    newpart.cb_fuse_high.IsEnabled[i1] = true;
                                    skip_bit_to_save = i1;
                                    skip_bit = false;
                                    //break;
                                }
                            }
                            if (skip_bit == true && isdeffuse==false)
                            {
                                continue;
                            }
                            if ((temp_deffuse_mask != save_temp_deffuse_mask) || isdeffuse)
                            {
                                save_temp_deffuse_mask = temp_deffuse_mask;
                                mask |= temp_deffuse_mask;
                                temp_deffuse = (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/VALUE").InnerText.Replace("0x", ""), NumberStyles.AllowHexSpecifier)));
                                str_txt = xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/TEXT").InnerText;

                                str_txt = str_txt.Substring(str_txt.IndexOf("[") + 1, str_txt.IndexOf("]") - (str_txt.IndexOf("[") + 1));

                                str1 = str_txt.Substring(0, str_txt.IndexOf("="));
                                if (str_txt.IndexOf(" ") >= 0)
                                {
                                    strval1 = str_txt.Substring(str_txt.IndexOf("=") + 1, str_txt.IndexOf(" ") - (str_txt.IndexOf("=") + 1));
                                    str2 = str_txt.Substring(str_txt.IndexOf(" ") + 1, str_txt.LastIndexOf("=") - (str_txt.IndexOf(" ") + 1));
                                    strval2 = str_txt.Substring(str_txt.LastIndexOf("=") + 1, str_txt.Length - (str_txt.LastIndexOf("=") + 1));
                                }
                                else
                                {
                                    strval1 = str_txt.Substring(str_txt.IndexOf("=") + 1, str_txt.Length - (str_txt.IndexOf("=") + 1));
                                }
                                int cntbit = 0;
                                bool strval1singlebit = strval1.Length <= 1 ? true : false;
                                bool strval2singlebit = strval2.Length <= 1 ? true : false;
                                for (int ii = 0; ii < 8; ii++)
                                {
                                    if ((temp_deffuse_mask & (1 << ii)) != 0)
                                    {


                                        if (strval1.Length > 0)
                                        {
                                            newpart.cb_fuse_high.Text[ii] = str1 + ((strval1singlebit == false) ? cntbit.ToString() : "");
                                            cntbit++;
                                            strval1 = strval1.Remove(0, 1);
                                            if (strval1.Length == 0)
                                            {
                                                cntbit = 0;
                                            }
                                        }
                                        else if (strval2.Length > 0)
                                        {
                                            newpart.cb_fuse_high.Text[ii] = str2 + ((strval2singlebit == false) ? cntbit.ToString() : "");
                                            cntbit++;
                                            strval2 = strval2.Remove(0, 1);
                                            if (strval2.Length == 0)
                                            {
                                                cntbit = 0;
                                            }
                                        }
                                        if((temp_deffuse & (1 << ii)) == 0 && isdeffuse)
                                        {
                                            newpart.cb_fuse_high.IsChecked[ii] = true;
                                            deffuse&=(ushort)~((ushort)1 << ii);
                                        }
                                        else
	                                    {
                                            newpart.cb_fuse_high.IsChecked[ii] = false;
                                            deffuse |= (ushort)((ushort)1 << ii);    
	                                    }
                                    }
                                }

                            }
                        }

                    }
                    catch (Exception)
                    {

                        //throw;
                    }
                    newpart.HighFuseDef = "0x" + deffuse.ToString("X2");
                    mask |= temp_mask;
                    for (int i = 0; i < 8; i++)
                    {
                        if ((mask & (1 << i)) != 0)
                            newpart.cb_fuse_high.IsEnabled[i] = true;
                    }
                    newpart.HighFuseMask = "0x" + mask.ToString("X2");
                }
                catch (Exception)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        newpart.cb_fuse_high.IsEnabled[i] = false;
                        newpart.cb_fuse_high.IsChecked[i] = false;
                    }
                    newpart.HighFuseDef = "0x" + "00";
                    newpart.HighFuseMask = "0x" + "00";
                    //throw;
                }
                try
                {
                    newpart.ExtFuseDef = xmldoc.SelectSingleNode("/AVRPART/FUSE/EXTENDED/NMB_FUSE_BITS").InnerText;
                    temp_mask = UInt16.Parse(newpart.ExtFuseDef);
                    temp_mask = (ushort)(Math.Pow(2, temp_mask));
                    temp_mask -= 1;
                    deffuse = 0;
                    mask = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        try
                        {
                            newpart.cb_fuse_ext.IsEnabled[i] = false;
                            newpart.cb_fuse_ext.IsChecked[i] = false;
                            deffuse |= (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/EXTENDED/FUSE" + i.ToString() + "/DEFAULT").InnerText))<<i);
                            newpart.cb_fuse_ext.Text[i] = xmldoc.SelectSingleNode("/AVRPART/FUSE/EXTENDED/FUSE" + i.ToString() + "/NAME").InnerText;
                            newpart.cb_fuse_ext.Name[i] = xmldoc.SelectSingleNode("/AVRPART/FUSE/EXTENDED/FUSE" + i.ToString() + "/TEXT").InnerText;
                            newpart.cb_fuse_ext.IsEnabled[i] = true;
                            newpart.cb_fuse_ext.IsChecked[i] = (deffuse & (1 << i)) == 0 ? true : false;
                            mask |= (ushort)(1<<i);
                        }
                        catch (Exception)
                        {
                            //deffuse = 0;
                            //break;
                            //throw;
                        }
                    }
                    fusestr = "EXTENDED";
                    try
                    {
                        ushort temp_deffuse = 0;
                        ushort temp_deffuse_mask = 0;
                        ushort save_temp_deffuse_mask = 0;
                        bool skip_bit = false;
                        int skip_bit_to_save = -1;
                        for (int i = 1; i <= (UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/NMB_TEXT").InnerText)); i++)
                        {
                            temp_deffuse_mask = (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/MASK").InnerText.Replace("0x", ""), NumberStyles.AllowHexSpecifier)));
                            bool isdeffuse = xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/TEXT").InnerText.Contains("default value");
                            
                            /* continue to check default value in case the bit is disabled */
                            skip_bit = true;
                            for (int i1 = 0; i1 < 8; i1++)
                            {
                                if (((temp_deffuse_mask & (1 << i1)) != 0) && (newpart.cb_fuse_ext.IsEnabled[i1] == false || i1 == skip_bit_to_save))
                                {
                                    newpart.cb_fuse_ext.IsEnabled[i1] = true;
                                    skip_bit_to_save = i1;
                                    skip_bit = false;
                                    //break;
                                }
                            }
                            if (skip_bit == true && isdeffuse==false)
                            {
                                continue;
                            }
                            if ((temp_deffuse_mask != save_temp_deffuse_mask) || isdeffuse)
                            {
                                save_temp_deffuse_mask = temp_deffuse_mask;
                                mask |= temp_deffuse_mask;                                
                                temp_deffuse = (ushort)((UInt32.Parse(xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/VALUE").InnerText.Replace("0x", ""), NumberStyles.AllowHexSpecifier)));
                                str_txt = xmldoc.SelectSingleNode("/AVRPART/FUSE/" + fusestr + "/TEXT" + i.ToString() + "/TEXT").InnerText;

                                str_txt = str_txt.Substring(str_txt.IndexOf("[") + 1, str_txt.IndexOf("]") - (str_txt.IndexOf("[") + 1));

                                str1 = str_txt.Substring(0, str_txt.IndexOf("="));
                                if (str_txt.IndexOf(" ") >= 0)
                                {
                                    strval1 = str_txt.Substring(str_txt.IndexOf("=") + 1, str_txt.IndexOf(" ") - (str_txt.IndexOf("=") + 1));
                                    str2 = str_txt.Substring(str_txt.IndexOf(" ") + 1, str_txt.LastIndexOf("=") - (str_txt.IndexOf(" ") + 1));
                                    strval2 = str_txt.Substring(str_txt.LastIndexOf("=") + 1, str_txt.Length - (str_txt.LastIndexOf("=") + 1));
                                }
                                else
                                {
                                    strval1 = str_txt.Substring(str_txt.IndexOf("=") + 1, str_txt.Length - (str_txt.IndexOf("=") + 1));
                                }
                                int cntbit = 0;
                                bool strval1singlebit = strval1.Length <= 1 ? true : false;
                                bool strval2singlebit = strval2.Length <= 1 ? true : false;
                                for (int ii = 0; ii < 8; ii++)
                                {
                                    if ((temp_deffuse_mask & (1 << ii)) != 0)
                                    {


                                        if (strval1.Length > 0)
                                        {
                                            newpart.cb_fuse_ext.Text[ii] = str1 + ((strval1singlebit == false) ? cntbit.ToString() : "");
                                            cntbit++;
                                            strval1 = strval1.Remove(0, 1);
                                            if (strval1.Length == 0)
                                            {
                                                cntbit = 0;
                                            }
                                        }
                                        else if (strval2.Length > 0)
                                        {
                                            newpart.cb_fuse_ext.Text[ii] = str2 + ((strval2singlebit == false) ? cntbit.ToString() : "");
                                            cntbit++;
                                            strval2 = strval2.Remove(0, 1);
                                            if (strval2.Length == 0)
                                            {
                                                cntbit = 0;
                                            }
                                        }
                                        if ((temp_deffuse & (1 << ii)) == 0 && isdeffuse)
                                        {
                                            newpart.cb_fuse_ext.IsChecked[ii] = true;
                                            deffuse &= (ushort)~((ushort)1 << ii);
                                        }
                                        else
                                        {
                                            newpart.cb_fuse_ext.IsChecked[ii] = false;
                                            deffuse |= (ushort)((ushort)1 << ii);
                                        }
                                    }
                                }

                            }
                        }

                    }
                    catch (Exception)
                    {

                        //throw;
                    }
                    newpart.ExtFuseDef = "0x" + deffuse.ToString("X2");
                    mask |= temp_mask;
                    for (int i = 0; i < 8; i++)
                    {
                        if ((mask & (1 << i)) != 0)
                            newpart.cb_fuse_ext.IsEnabled[i] = true;
                    }
                    newpart.ExtFuseMask = "0x" + mask.ToString("X2");
                }
                catch (Exception)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        newpart.cb_fuse_ext.IsEnabled[i] = false;
                        newpart.cb_fuse_ext.IsChecked[i] = false;
                    }
                    newpart.ExtFuseDef = "0x" + "00";
                    newpart.ExtFuseMask = "0x" + "00";
                    //throw;
                }
                
                partlist.Add(newpart);
                //MessageBox.Show("You selected: " + newpart.Part_Name + "," + newpart.Signature + "," +
                //    newpart.Prog_Flash + "," + newpart.Prog_Eeprom + "," + newpart.FlashPageSize + ","
                //    + newpart.EepromPageSize);              
                
                //break;
            }
            return partlist;
        }
    }
}
