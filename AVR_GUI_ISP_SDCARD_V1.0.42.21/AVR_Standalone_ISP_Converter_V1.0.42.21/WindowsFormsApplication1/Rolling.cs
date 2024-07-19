using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
//using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using Ini;
using System.Threading;
using System.Diagnostics;
using AvrStandalonIspProg;
using System.Xml.Serialization;

namespace MainProgram
{
    
    public partial class windowForm1 : Form
    {
        enum Filetypes{ConfigFile,TempConfigFile,DatFile,HexFile,SerialFile} ;
        byte[] roll =   { 0x00, 0xE9, 0x79, 0x25, 0x54, 0x88, 0xDF, 0xA5, 0xB9, 0xF6, 0xC2, 
                                0xAB, 0xFC, 0xB3, 0xB1, 0x37, 0xE1, 0x83, 0xD5, 0x97, 0x04, 
                                0x38, 0xFE, 0x99, 0x26, 0xE4, 0x8E, 0x4B, 0xF7, 0x04, 0xC5, 
                                0xA5, 0x1F, 0x88, 0x68, 0xEF, 0x1D, 0x39, 0xBC, 0x62, 0xD8, 
                                0x63, 0x0F, 0x6A, 0xA5, 0x12, 0xAA, 0xE5, 0x8C, 0x75, 0x36, 
                                0xBA, 0x81, 0x8F, 0x30, 0x3D, 0xEB, 0xE0, 0xFE, 0x70, 0xFA, 
                                0x94, 0xAE, 0xB5, 0x31 };
        IniFile ini;
        string configfilepath;
        string tempconfigfilepath;
        string datfilepath;
        string folderpath;
        string rootfolderpath;
        string hexfilepath;
        string serialfilepath;
        string xmlspath;
        byte RollCounterValue = 0;
        ushort MaxLicenses = 0;
        ushort EncryptCounter = 0;
        ushort FileUniqueNumber = 0;
        List<PartStruct> partlist;
        bool DoEventChangeFuses = true;
        bool DoEventUseEeprom = true;
        bool DoEventUseCalib = true;
        bool DoEventMaxFolders = true;
        int LastWorkingFolder = 0;
        int[] uniqieIds=new int[100];
        public windowForm1()
        {
            //path = @"C:\temp\config.ini";
            InitializeComponent();
            comboBoxIspClock.Items.Add("4000 Khz");
            comboBoxIspClock.Items.Add("2000 Khz");
            comboBoxIspClock.Items.Add("1000 Khz");
            comboBoxIspClock.Items.Add("500 Khz");
            comboBoxIspClock.Items.Add("250 Khz");
            comboBoxIspClock.Items.Add("125 Khz");
            comboBoxIspClock.Items.Add("62.5 Khz");
            comboBoxIspClock.Items.Add("31.25 Khz");
            comboBoxIspClock.SelectedIndex = 0;
            InitializeBackgroundWorker();
            partlist = new List<PartStruct>();
            ReadXMLSettings();
            if (partlist == null || partlist.Count==0)
            {
                ReadDevices();
            }
            else
            {
                if (partlist != null)
                {
                    foreach (var item in partlist)
                    {
                        comboBoxChipSelect.Items.Add(item.Part_Name);
                    }
                }
                
                comboBoxChipSelect.SelectedIndex = 0;
                labelStatus.Text = "Reading devices done.";
                labelStatus.ForeColor = Color.Green;
            }
            ResetForm();
            if (ReadRootConfig() == true)
            {
                comboBoxFolderSelector_SelectedIndexChanged(comboBoxFolderSelector,null);
            }
        }
        void ResetForm()
        {
            cancelAsyncButton.Enabled = false;
            buttonCreateOpenIni.Enabled = false;
            buttonOpenConfig.Enabled = false;
            buttonSelectFolder.Enabled = true;
            buttonWriteToIni.Enabled = false;
            buttonWriteConfig.Enabled = false;
            buttonSelectHEX.Enabled = false;
            buttonSelectBoot.Enabled = false;
            buttonSelectEEPROM.Enabled = false;
            buttonRefresh.Enabled = false;
            buttonMakeList.Enabled = false;
            buttonReadChipConfig.Enabled = false;
            buttonWriteChipConfig.Enabled = false;
            comboBoxChipSelect.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBoxChipSelect.SelectedIndex = 0;
            comboBoxFolderSelector.DropDownStyle = ComboBoxStyle.DropDownList;
            textBoxFilename.Text = "";
            textBoxFlashHexFilePath.Text = "";
            textBoxBootHexFile.Text = "";
            textBoxBootHexFilePath.Text = "";
            textBoxEepromHexFile.Text = "";
            textBoxEepromHexFilePath.Text = "";
            textBoxProgname.Text = "";
            textBoxFileInfo.Text = "";
            textBoxFileInfo2.Text = "";
            textBoxChipsizeBytes.ReadOnly = true;
            textBoxEepromPageSizeBytes.ReadOnly = true;
            textBoxEepromSizeBytes.ReadOnly = true;
            textBoxMaskFuseExt.ReadOnly = true;
            textBoxMaskFuseHigh.ReadOnly = true;
            textBoxMaskFuseLock.ReadOnly = true;
            textBoxMaskFuseLow.ReadOnly = true;
            textBoxPagesizeWords.ReadOnly = true;
            textBoxPath.ReadOnly = true;
            //textBoxProgFuseExt.ReadOnly = true;
            //textBoxProgFuseHigh.ReadOnly = true;
            textBoxProgFuseLock.ReadOnly = true;
            //textBoxProgFuseLow.ReadOnly = true;
            textBoxSignature.ReadOnly = true;
            textBoxFileUniqueVale.Text = "";
            textBoxEncryptionCounter.Text = "";
            textBoxRollingCounter.Text = "0";
            textBoxSPIDIV.Text = "0";
            textBoxLicense.Text = "0";
            checkBoxUseSerialCounter.Checked = false;
            textBoxEepromStartAddress.Enabled = false;
            textBoxEepromStartAddress.Text = "0x0000";
            checkBoxUseCalibBytes.Checked = false;
            textBoxEepromStartAddressCalibBytes.Enabled = false;
            textBoxEepromStartAddressCalibBytes.Text = "0x0000";
        }
        bool ReadRootConfig()
        {
            if (rootfolderpath != "")
            {
                if (File.Exists(rootfolderpath + "\\0\\config.ini"))
                {
                    ini = new IniFile(rootfolderpath + "\\0\\config.ini");
                    ini.path = rootfolderpath + "\\0\\config.ini";
                    uint maxfolders = TryParseNumOrDef(ini.IniReadValue("settings", "maxfolders"), 0);
                    if (maxfolders == 0)
                    {
                        ini.IniWriteValue("settings", "maxfolders", "10");
                        maxfolders = TryParseNumOrDef(ini.IniReadValue("settings","maxfolders"), 0);
                    }
                    uint upstep = TryParseNumOrDef(ini.IniReadValue("settings","up"), 0);
                    if (upstep == 0)
                    {
                        ini.IniWriteValue("settings", "up", "1");
                        upstep = TryParseNumOrDef(ini.IniReadValue("settings","up"), 0);
                    }
                    uint downstep = TryParseNumOrDef(ini.IniReadValue("settings","down"), 0);
                    if (downstep == 0)
                    {
                        ini.IniWriteValue("settings", "down", "1");
                        downstep = TryParseNumOrDef(ini.IniReadValue("settings","down"), 0);
                    }
                    
                    
                    textBoxMaxFolders.Text = maxfolders.ToString();
                    textBoxUpStep.Text = upstep.ToString();
                    textBoxDownStep.Text = downstep.ToString();
                    
                    comboBoxFolderSelector.DataSource = FillMaxFolders(maxfolders);
                    if (LastWorkingFolder<maxfolders)
                    {
                        comboBoxFolderSelector.SelectedIndex = LastWorkingFolder;
                    }
                    else if (maxfolders > 0)
                    {
                        comboBoxFolderSelector.SelectedIndex = 0;
                    }
                    //MakeUniqueFileIdsArray();
                    DoEventMaxFolders = true;
                    return true;
                }
                else
                    return false;
            }
            else
                 return false;
        }
        string[] FillMaxFolders(uint maxnum)
        {
            string[] cbfolders=new string[maxnum];
            for (int i = 0; i < maxnum; i++)
            {
                //if(i != 0 )
                    cbfolders[i] = (i+1).ToString();
            }
            return cbfolders;
        }
        void textBoxFinalFuseExt_Leave(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }
        public void ReadDevices()
        {
            progressBarCrypt.Maximum = 100;
            progressBarCrypt.Minimum = 0;
            progressBarCrypt.Value = progressBarCrypt.Minimum;
            cancelAsyncButton.Enabled = true;
            buttonCreateOpenIni.Enabled = false;
            buttonOpenConfig.Enabled = false;
            buttonSelectFolder.Enabled = false;
            buttonWriteToIni.Enabled = false;
            buttonWriteConfig.Enabled = false;
            buttonSelectHEX.Enabled = false;
            buttonSelectBoot.Enabled = false;
            buttonSelectEEPROM.Enabled = false;
            buttonRefresh.Enabled = false;
            buttonMakeList.Enabled = false;
            buttonReadChipConfig.Enabled = false;
            buttonWriteChipConfig.Enabled = false;
            // Start the asynchronous operation.
            InitializeBackgroundWorker();
            backgroundWorker1.RunWorkerAsync("dev");
            
            
        }
        // Set up the BackgroundWorker object by 
        // attaching event handlers. 
        private void InitializeBackgroundWorker()
        {
            
            backgroundWorker1 = new BackgroundWorker();
            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.DoWork +=
                new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.RunWorkerCompleted +=
                new RunWorkerCompletedEventHandler(
            backgroundWorker1_RunWorkerCompleted);
            backgroundWorker1.ProgressChanged +=
                new ProgressChangedEventHandler(
            backgroundWorker1_ProgressChanged);
            cancelAsyncButton.Enabled = false;
        }


        private void cancelAsyncButton_Click(System.Object sender,
            System.EventArgs e)
        {
            if (backgroundWorker1.WorkerSupportsCancellation == true)
            {
                // Cancel the asynchronous operation.
                this.backgroundWorker1.CancelAsync();

                // Disable the Cancel button.
                cancelAsyncButton.Enabled = false;
            }
        }
        // This event handler is where the actual,
        // potentially time-consuming work is done.
        private void backgroundWorker1_DoWork(object sender,
            DoWorkEventArgs e)
        {
            // Get the BackgroundWorker that raised this event.
            BackgroundWorker worker = sender as BackgroundWorker;
            
            // Assign the result of the computation
            // to the Result property of the DoWorkEventArgs
            // object. This is will be available to the 
            // RunWorkerCompleted eventhandler.
            if ((string)(e.Argument)=="enc")
            {
                e.Result = EncryptBytes(worker, e);
            }
            if ((string)(e.Argument) == "dev")
            {
                e.Result = "dev";
                partlist = new XmlFinder().ReadDeviceInfoFromXmls(backgroundWorker1,e,xmlspath);
            }
        }

        // This event handler deals with the results of the
        // background operation.
        private void backgroundWorker1_RunWorkerCompleted(
            object sender, RunWorkerCompletedEventArgs e)
        {
            //byte[] bytes=(byte[])e.Result;
            // First, handle the case where an exception was thrown.
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
                labelStatus.Text = "Error during encryption.";
                labelStatus.ForeColor = Color.Red;
            }
            else if (e.Cancelled || e.Result==null)
            {
                // Next, handle the case where the user canceled 
                // the operation.
                // Note that due to a race condition in 
                // the DoWork event handler, the Cancelled
                // flag may not have been set, even though
                // CancelAsync was called.
                //MessageBox.Show("Action Canceled");
                labelStatus.Text = "Encryption proccess canceled.";
                labelStatus.ForeColor = Color.Red;
                //resultLabel.Text = "Canceled";
            }
            else
            {
                // Finally, handle the case where the operation 
                // succeeded.
                if ( e.Result.GetType().ToString() != "System.String")
                {

                    labelStatus.ForeColor = Color.Green;
                    labelStatus.Text = "Encryption done.";
                    //MessageBox.Show("Action "+((byte[])e.Result).ToString());
                    //textBox_OutputHex.Text = BitConverter.ToString(bytes).Replace("-", ""); //
                    labelStatus.ForeColor = Color.Black;
                    labelStatus.Text = "Writing dat file.";
                    using (File.CreateText(folderpath + "\\" + "flash.dat"))//+ folderpath.Remove(0, folderpath.LastIndexOf("\\"))
                    {
                    }
                    if (SelectFile(Filetypes.DatFile, "project.dat") == true)
                    {
                        try
                        {
                            //File.WriteAllBytes(datfilepath, (byte[])e.Result);
                            File.WriteAllBytes(folderpath + "\\" + "flash.dat", (byte[])e.Result);
                            labelStatus.Text = "Encription and Writing dat file done.";
                            labelStatus.ForeColor = Color.Green;
                            //if (checkBoxStopUsingUniqueFile.Checked == false && textBoxEncryptionCounter.Text!="0x0FFF")
                            //{
                            //    EncryptCounter++;
                            //    textBoxEncryptionCounter.Text = "0x" + EncryptCounter.ToString("X4");
                            //}
                            //textBoxFileUniqueVale.Text = "0x" + FileUniqueNumber.ToString("X4");
                            progressBarCrypt.Value = progressBarCrypt.Maximum;
                            button_Decrypt_Click(null,null);
                            if (labelStatus.Text.Contains("Reading settings from dat file done"))
                                labelStatus.Text = "Verifying write encrypted file done";
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("Dat file exeption: " + ex.Message);
                            labelStatus.Text = "Writing dat file failed.";
                            labelStatus.ForeColor = Color.Red;
                        }
                    }
                    else
                    {
                        labelStatus.Text = "Writing dat file failed.";
                        labelStatus.ForeColor = Color.Red;
                    }
                }
                else
                {
                    if (partlist!=null)
                    {
                        foreach (var item in partlist)
                        {
                            comboBoxChipSelect.Items.Add(item.Part_Name);
                        }
                    }
                    
                    comboBoxChipSelect.SelectedIndex = 0;
                    labelStatus.Text = "Reading devices done.";
                    labelStatus.ForeColor = Color.Green;
                    //XmlSerializer xs = new XmlSerializer(typeof(List<PartStruct>));
                    //TextWriter tw = new StreamWriter("Parts.xml");
                    //xs.Serialize(tw, partlist);
                }
                //resultLabel.Text = e.Result.ToString();
            }
            backgroundWorker1.Dispose();
            // Enable the Encrypt button.
            //button_Encript.Enabled = true;
            // Enable the Decrypt button.
            //button_Decript.Enabled = true;
            
            buttonCreateOpenIni.Enabled = true;
            buttonOpenConfig.Enabled = true;
            buttonSelectFolder.Enabled = true;
            buttonWriteToIni.Enabled = true;
            buttonWriteConfig.Enabled = true;
            buttonSelectHEX.Enabled = true;
            buttonSelectBoot.Enabled = true;
            buttonSelectEEPROM.Enabled = true;
            buttonRefresh.Enabled = true;
            buttonMakeList.Enabled = true;
            buttonReadChipConfig.Enabled = true;
            buttonWriteChipConfig.Enabled = true;
            // Disable the Cancel button.
            cancelAsyncButton.Enabled = false;
        }

        // This event handler updates the progress bar.
        private void backgroundWorker1_ProgressChanged(object sender,
            ProgressChangedEventArgs e)
        {
            if (((Texts)(e.UserState)).outText.Length > 0)
                this.textBox_OutputHex.Text = (String)((Texts)(e.UserState)).outText;
            if (((Texts)(e.UserState)).inText.Length > 0)
                this.textBox_InputHex.Text = (String)((Texts)e.UserState).inText;
            this.labelStatus.Text = (String)((Texts)e.UserState).lblText;
            this.progressBarCrypt.Value = e.ProgressPercentage;            
        }

        /*Encrypt settings into project.dat file*/
        private void button_Encript_Click(object sender, EventArgs e)
        {
            
            progressBarCrypt.Maximum = 100;
            progressBarCrypt.Minimum = 0;
            progressBarCrypt.Value = progressBarCrypt.Minimum;
            labelStatus.ForeColor = Color.Black;
            labelStatus.Text = "Writing settings to config.ini file.";
            /* Write config.ini first */
            if (folderpath == null)
            {
                MessageBox.Show("Select folder first.");
                return;
            }
            configfilepath = folderpath + "\\config.ini";
            if(WriteConfigIni()==false)
                return;
            
            /*check if rolling counter exists and valid 0..127 */
            labelStatus.Text = "Checking the rolling counter is a valid number.";
            if (Byte.TryParse(textBoxRollingCounter.Text, out RollCounterValue) == false)
            {
                MessageBox.Show(labelRollingCounter.Text + " not a digits");
                return ;
            }
            else if (RollCounterValue > 64)
            {
                MessageBox.Show(labelRollingCounter.Text + " should be 0..64");
                return ;
            }           
            
            //buttonWriteToIni_Click(null, null);
            
            cancelAsyncButton.Enabled = true;
            buttonCreateOpenIni.Enabled = false;
            buttonOpenConfig.Enabled = false;
            buttonSelectFolder.Enabled = false;
            buttonWriteToIni.Enabled = false;
            buttonWriteConfig.Enabled = false;
            buttonSelectHEX.Enabled = false;
            buttonSelectBoot.Enabled = false;
            buttonSelectEEPROM.Enabled = false;
            buttonRefresh.Enabled = false;
            buttonMakeList.Enabled = false;
            buttonReadChipConfig.Enabled = false;
            buttonWriteChipConfig.Enabled = false;
            // Start the asynchronous operation.
            while (backgroundWorker1.IsBusy == true) ;
            labelStatus.Text = "Reading all unique id from files.";
            MakeUniqueFileIdsArray();
            InitializeBackgroundWorker();
            backgroundWorker1.RunWorkerAsync("enc");
            
        }
        public class Texts
        {
            public string lblText;
            public string inText;
            public string outText;
            public Texts()
            {
                lblText = "";
                inText = "";
                outText = "";
            }
        }
        public class hexVars
        {
            public int startAddress=0;
            public int pagesCounter=0;
            public int binBytesCheckSum=0;
            public int binBytesCounter=0;
            public bool result=false;
            public byte[] binBytes=Enumerable.Repeat((byte)0xFF, 0xFFFFF).ToArray();
        };
        public hexVars ConvertIntelHexToBin(string filePath, int pageSizeWords)
        {
            hexVars vars=new hexVars();
            vars.result = false;
            var hfb = File.ReadAllLines(filePath);//textBoxFlashHexFilePath.Text
            vars.binBytesCheckSum = 0;
            vars.binBytesCounter = 0;
            vars.pagesCounter = 0;
            int startAddress = 0xFFFFF, nextAddress, offset = 0;
            byte checkSum, byteValue, cntBytes;
            foreach (var item in hfb)
            {//:0E0000000C94CE040C9400000C9400000C94A0
                if (item.IndexOf(':') < 0)
                    continue;
                checkSum = 0;
                string str = item.Substring(item.IndexOf(':') + 1);//0E0000000C94CE040C9400000C9400000C94A0
                if (byte.TryParse(str.Substring(0, 2), NumberStyles.AllowHexSpecifier, null, out cntBytes) == false)
                {
                    MessageBox.Show("error parse count value in hex file.");
                    return vars;
                }
                checkSum += cntBytes;
                if (int.TryParse(str.Substring(2, 4), NumberStyles.AllowHexSpecifier, null, out nextAddress) == false)
                {
                    MessageBox.Show("error parse start address in hex file.");
                    return vars;
                }
                checkSum += (byte)(nextAddress >> 8);
                checkSum += (byte)(nextAddress & 0xFF);
                nextAddress += offset;
                if (byte.TryParse(str.Substring(6, 2), NumberStyles.AllowHexSpecifier, null, out byteValue) == false)
                {
                    MessageBox.Show("error parse type value in hex file.");
                    return vars;
                }
                checkSum += byteValue;
                if (byteValue == 0x01)
                {
                    break;//eof
                }
                else if (byteValue == 0x02)
                {
                    if (int.TryParse(str.Substring(2, 4), NumberStyles.AllowHexSpecifier, null, out nextAddress) == false)
                    {
                        MessageBox.Show("error parse offset address in hex file.");
                        return vars;
                    }
                    if (int.TryParse(str.Substring(8, 4), NumberStyles.AllowHexSpecifier, null, out offset) == false)
                    {
                        MessageBox.Show("error parse extend segment address in hex file.");
                        return vars;
                    }
                    offset <<= 4;
                    offset += nextAddress;
                    continue;
                }
                else if (byteValue == 0x03)
                {
                    continue;
                }
                if (nextAddress < startAddress)
                    startAddress = nextAddress;
                for (int i = 0; i <= cntBytes; i++)
                {
                    if (byte.TryParse(str.Substring(i * 2 + 8, 2), NumberStyles.AllowHexSpecifier, null, out byteValue) == false)
                    {
                        MessageBox.Show("error parse data value in hex file.");
                        return vars;
                    }
                    if (i == cntBytes)
                    {
                        if ((byte)(0x100 - checkSum) != byteValue)
                        {
                            MessageBox.Show("error checksum value in hex file.");
                            return vars;
                        }
                        else
                            break;
                    }
                    checkSum += byteValue;
                    //vars.binBytes[vars.binBytesCounter] = byteValue;
                    vars.binBytes[nextAddress+i] = byteValue;
                    vars.binBytesCheckSum += byteValue;
                    vars.binBytesCounter++;
                }
            }
            //binWordsCounter >>= 1;
            for (; (startAddress % (pageSizeWords << 1)) != 0;  )
            {
                vars.binBytes[--startAddress] = 0xFF;
                vars.binBytes[--startAddress] = 0xFF;
                vars.binBytesCheckSum += 0xFF;
                vars.binBytesCheckSum += 0xFF;
            }
            for (; (vars.binBytesCounter % (pageSizeWords << 1)) != 0; )
            {
                vars.binBytes[vars.binBytesCounter++] = 0xFF;
                vars.binBytes[vars.binBytesCounter++] = 0xFF;
                vars.binBytesCheckSum += 0xFF;
                vars.binBytesCheckSum += 0xFF;
            }
            vars.pagesCounter = ((vars.binBytesCounter >> 1) / pageSizeWords);
            vars.startAddress = (startAddress >> 1);
            vars.result = true;               
            return vars;
        }
        byte[] EncryptBytes( BackgroundWorker worker, DoWorkEventArgs e)
        {
            
            
            Texts texts=new Texts();
            texts.lblText="Reading all bytes from config.ini file.";
            worker.ReportProgress( 3,texts);
            var cfb = File.ReadAllBytes(configfilepath);
            
            /* get max licenses value  */
            int num;
            if (int.TryParse(textBoxLicense.Text, NumberStyles.Integer, null, out num))
            {
                if ((num < 0) || (num > 0xffff))
                {
                    MessageBox.Show("Value out of range. Should be 0..65535");
                    //textBoxLicense.Text = "0";
                    return null;
                }
                else
                {
                    //textBoxLicense.Text = num.ToString();
                    MaxLicenses = (ushort)num;
                }
            }
            else
            {
                MessageBox.Show("Value out of range. Should be 0..65535");
                //textBoxLicense.Text = "0";
                return null;
            }
            /* end get max licenses value  */

            texts.lblText ="Reading all bytes from .hex file.";
            worker.ReportProgress( 4,texts);
            //var hfb = File.ReadAllBytes(textBoxFlashHexFilePath.Text);
            ushort pageSizeWords;
            if (ushort.TryParse(textBoxPagesizeWords.Text, NumberStyles.Integer, null, out pageSizeWords) == false)
            {
                MessageBox.Show("error parse page size in words.");
                return null;
            }
            hexVars appVars = new hexVars();
            hexVars bootVars = new hexVars();
            if (textBoxFlashHexFilePath.Text!="")
            {
                if (File.Exists(textBoxFlashHexFilePath.Text) == false)
                {
                    MessageBox.Show(textBoxFlashHexFilePath.Text + " file not found");
                    return null;
                }
                appVars = ConvertIntelHexToBin(textBoxFlashHexFilePath.Text, pageSizeWords);
                if (appVars.result==false)
                {
                    return null;
                }
            }
            if (textBoxBootHexFilePath.Text != "")
            {
                if (File.Exists(textBoxBootHexFilePath.Text) == false)
                {
                    MessageBox.Show(textBoxBootHexFilePath.Text + " file not found");
                    return null;
                }
                bootVars = ConvertIntelHexToBin(textBoxBootHexFilePath.Text, pageSizeWords);
                if (bootVars.result == false)
                {
                    return null;
                }
            }
            byte[] efb=null;
            if (textBoxEepromHexFilePath.Text!="")
            {
                if (File.Exists(textBoxEepromHexFilePath.Text)==true)
                {
                    texts.lblText = "Reading all bytes from eeprom file.";
                    efb = File.ReadAllBytes(textBoxEepromHexFilePath.Text);
                }
            }
            texts.lblText = "Fill spaces in common bytes array from 2048.";
            worker.ReportProgress( 5,texts);
            //byte[] bytes = Enumerable.Repeat((byte)0x20, 1024 + hfb.Length).ToArray();
            if (efb==null)
            {
                efb = new byte[0];
            }
            byte[] bytes = Enumerable.Repeat((byte)0xFF, 2048 + 0x40000 + efb.Length).ToArray();
            texts.lblText = "copy config bytes to common bytes.";
            worker.ReportProgress( 6,texts);
            Buffer.BlockCopy(cfb, 0, bytes, 0, cfb.Length);
            texts.lblText = "copy hexfile bytes to common bytes.";
            worker.ReportProgress( 7,texts);
            //Buffer.BlockCopy(hfb, 0, bytes, 1024, hfb.Length);
            int adr = 2000;
            bytes[adr++] = (byte)(appVars.startAddress >> 16);
            bytes[adr++] = (byte)(appVars.startAddress >> 8);//1FFFE byte address = FFFF word address
            bytes[adr++] = (byte)(appVars.startAddress & 0xFF);
            bytes[adr++] = (byte)(appVars.pagesCounter >> 8);
            bytes[adr++] = (byte)(appVars.pagesCounter & 0xFF);
            bytes[adr++] = (byte)(appVars.binBytesCheckSum >> 8);
            bytes[adr++] = (byte)(appVars.binBytesCheckSum & 0xFF);
            bytes[adr++] = (byte)(bootVars.startAddress >> 16);
            bytes[adr++] = (byte)(bootVars.startAddress >> 8);//1FFFE byte address = FFFF word address
            bytes[adr++] = (byte)(bootVars.startAddress & 0xFF);
            bytes[adr++] = (byte)(bootVars.pagesCounter >> 8);
            bytes[adr++] = (byte)(bootVars.pagesCounter & 0xFF);
            bytes[adr++] = (byte)(bootVars.binBytesCheckSum >> 8);
            bytes[adr++] = (byte)(bootVars.binBytesCheckSum & 0xFF);
            bytes[adr++] = (byte)(pageSizeWords >> 8);
            bytes[adr++] = (byte)(pageSizeWords);
            adr = 512;
            if (checkBoxStopUsingUniqueFile.Checked == true)
            {
                bytes[adr++] = (byte)(0x77 ^ 1);//1=force change encrypcounter to next one :)
            }
            else
            {
                bytes[adr++] = (byte)(0x77 ^ 0);
            }

            if (
                (datfilepath.Contains("flash.dat") == false) 
                ||(!File.Exists(datfilepath)) 
                //||(textBoxEncryptionCounter.Text == "0x0FFE")
                //||(textBoxEncryptionCounter.Text == "0x0FFF")
                )
            {
                EncryptCounter = 0;                
            }
            //0x0123 = 0x12, 0x30
            /*
            if (checkBoxStopUsingUniqueFile.Checked == true || textBoxEncryptionCounter.Text=="0x0FFF")
            {
                bytes[adr++] = (byte)(0x77 ^ (byte)((0x0FFF) >> 4));//max encryrt count 0xFFF
                bytes[adr++] = (byte)(0x77 ^ (byte)(((0x0FFF) << 4) & 0xF0));
            }
            else
            */
            {
                bytes[adr++] = (byte)(0x77 ^ (byte)((EncryptCounter + 1) >> 4));//max encryrt count 0xFFF
                bytes[adr++] = (byte)(0x77 ^ (byte)(((EncryptCounter + 1) << 4) & 0xF0));    
            } 
            //
            bytes[adr++] = (byte)(0x77 ^ (byte)((checkBoxResetLicenses.Checked==true)?1:0));
            bytes[adr++] = (byte)(0x77 ^ (byte)((MaxLicenses) >> 8));
            bytes[adr++] = (byte)(0x77 ^ (byte)(MaxLicenses));
            bytes[adr++] = (byte)(0x77 ^ (byte)((efb.Length == 0) ? 0 : 1));
            bytes[adr++] = (byte)(0x77 ^ (byte)((checkBoxUseSerialCounter.Checked == true) ? 1 : 0));
            bytes[adr++] = (byte)(0x77 ^ (byte)((checkBoxUseCalibBytes.Checked == true) ? 1 : 0));
            if (datfilepath.Contains("flash.dat") == false || !File.Exists(datfilepath))
	        {
                FileUniqueNumber=(ushort)(new Random(DateTime.Now.Second).Next(1,0xFFE));                
	        }
            ChangeFileUniqueIdIfExists(LastWorkingFolder+1);
            bytes[adr++] = (byte)(0x77 ^ (byte)((FileUniqueNumber) >> 8) & 0x0F);
            bytes[adr++] = (byte)(0x77 ^ (byte)((FileUniqueNumber)));
            //textBoxLicense.Text
            //Buffer.BlockCopy(hfb, 0, bytes, 1024, binBytesCounter);
            Buffer.BlockCopy(appVars.binBytes, appVars.startAddress<<1, bytes, 2048, appVars.binBytesCounter);
            Buffer.BlockCopy(bootVars.binBytes, bootVars.startAddress<<1, bytes, 2048 + appVars.binBytesCounter, bootVars.binBytesCounter);
            Buffer.BlockCopy(efb, 0, bytes, 2048 + 0x40000, efb.Length);
            texts.lblText = "Replacing '-' in Input TextBox.";
            //texts.inText = BitConverter.ToString(bytes).Replace("-", "");
            worker.ReportProgress(8, texts);
            //NOP(10000);
            texts.inText = "";
            texts.lblText="Encrypting, please Wait...";
            worker.ReportProgress( 9,texts);
            //NOP(1000000);
            int b1, b2;
            RollCounterValue = (byte)(new Random(DateTime.Now.Second).Next(1, roll.Length-1));
            //RollCounterValue = 0;//debug
            if (cbFwUpdate.Checked)
                RollCounterValue = 5;// 0x88
            for (int i = 0; i < bytes.Length && (RollCounterValue != 0); ++i)
            {
                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }
                bytes[i] ^= roll[((RollCounterValue) % roll.Length)];//168; // pseudo encrypt
                //foreach (var item in textBox_Key.Text)
                //{
                //    bytes[i] ^= (byte)item;
                //}
                /*ROL*/
                b1 = ((int)bytes[i] << 7);
                b2 = ((int)bytes[i] >> 1);
                bytes[i] = (byte)(b1 | b2);
                texts.lblText = "Encrypted " + bytes[i].ToString("X2") + " " + ((100 * i) / bytes.Length) + "%";
                worker.ReportProgress((100 * i) / bytes.Length, texts);                
            }
            texts.lblText = "Encrypted " + (100) + "%";
            worker.ReportProgress(100, texts);
            return bytes;
        }
        /*reading encrypted config.ini from project.dat file*/
        private void button_Decrypt_Click(object sender, EventArgs e)
        {
            int b1, b2;
            byte RollCounterValue;
            EncryptCounter = 0;
            FileUniqueNumber = 0;
            datfilepath = folderpath + "\\" + "flash.dat";        
            // if flash.dat file not exists case -> try parse old 1.dat file
            if (File.Exists(datfilepath) == false)
            {
                datfilepath = folderpath + "\\" + folderpath.Remove(0, folderpath.LastIndexOf("\\")) + ".dat";   // 
                if (File.Exists(datfilepath) == false)
                {
                    labelStatus.Text = "Encrypted file not exists";
                    datfilepath = "";
                    return;
                }
            }
            labelStatus.Text = "Checking the rolling counter is a valid number.";
            RollCounterValue = 0;
            labelStatus.Text = "Reading settings section from dat file to temp var.";
            byte[] bytes = new byte[1000];// File.ReadAllBytes(datfilepath);
            byte[] tempbytes=new byte[bytes.Length];
            byte[] readbytes=File.ReadAllBytes(datfilepath);
            try
            {
                Buffer.BlockCopy(readbytes, 0, tempbytes, 0, bytes.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message+". Not valid file '" + datfilepath + "'");
                return;
            }
            /*decrypt only config data from fle*/
            for (int j = 0; j < roll.Length ; j++)//window=5
            {
                labelStatus.Text = "Try decrypt...";// with rolling counter value " + RollCounterValue.ToString();
                Buffer.BlockCopy(tempbytes, 0, bytes, 0, 1000);//bytes.Length
                textBoxRollingCounter.Text = j.ToString();
                progressBarCrypt.Maximum = bytes.Length;
                progressBarCrypt.Minimum = 0;
                progressBarCrypt.Value = progressBarCrypt.Minimum;
                for (int i = 0; i < ("[settings]".Length) && j != 0 ; ++i)
                {                
                    /*ROR*/
                    b1 = ((int)bytes[i] << 1);
                    b2 = ((int)bytes[i] >> 7);
                    bytes[i] = (byte)(b1 | b2);
                    bytes[i] ^= roll[(( j) % roll.Length)];//168; // pseudo encrypt
                    progressBarCrypt.Value = i;
                }
                textBox_InputHex.Text = BitConverter.ToString(bytes).Replace("-", "");
                textBox_Input.Text = FromHexString(textBox_InputHex.Text);
                if (String.Compare(textBox_Input.Text, 0, "[settings]", 0, "[settings]".Length, true) == 0)//
                {
                    RollCounterValue = (byte)j;
                    
                    labelStatus.Text = "Successed to decrypt";// with rolling counter value " + j.ToString();
                    labelStatus.ForeColor = Color.Green;
                    if (SelectFile(Filetypes.TempConfigFile, "config.ini") == false)
                        return;
                    for (int i = ("[settings]".Length); i < bytes.Length && j != 0; ++i)
                    {
                        /*ROR*/
                        b1 = ((int)bytes[i] << 1);
                        b2 = ((int)bytes[i] >> 7);
                        bytes[i] = (byte)(b1 | b2);
                        bytes[i] ^= roll[((j) % roll.Length)];//168; // pseudo encrypt
                        progressBarCrypt.Value = i;
                    }
                    labelStatus.Text = "Successed to decrypt settings";// with rolling counter value " + j.ToString();
                    labelStatus.ForeColor = Color.Black;
                    textBox_InputHex.Text = BitConverter.ToString(bytes).Replace("-", "");
                    textBox_Input.Text = FromHexString(textBox_InputHex.Text);
                    try
                    {
                        File.WriteAllText(tempconfigfilepath, textBox_Input.Text);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                    configfilepath = tempconfigfilepath;
                    /*read config.ini config*/
                    ReadConfigIni(false);
                    textBoxRollingCounter.Text = RollCounterValue.ToString();
                    labelStatus.Text = "Reading settings from dat file done";// with rolling counter value " + j.ToString();
                    if ((bytes[513]!=0x20&&bytes[514]!=0x20) && (bytes[513]!=0xFF&&bytes[514]!=0xFF))
                    {
                        //0x12, 0x30 = 0x0123
                        EncryptCounter = (ushort)((ushort)((byte)(0x77 ^ bytes[513]) << 4) & 0x0FF0) ;
                        EncryptCounter |= (ushort)((byte)(0x77 ^ bytes[514]) >> 4);
                    }
                    if ((bytes[521] != 0x20 && bytes[522] != 0x20 && bytes[521] != 0xFF && bytes[522] != 0xFF))
                    {
                        FileUniqueNumber = (ushort)(((byte)(0x77 ^ bytes[521]) & 0x0F)<< 8 );
                        FileUniqueNumber |= (ushort)(byte)(0x77 ^ bytes[522]);
                    }
                    if (datfilepath.Contains("flash.dat")==false)
                    {
                        EncryptCounter = 0;
                        //FileUniqueNumber = (ushort)(new Random(DateTime.Now.Second).Next(1, 0xFFF));
                    }
                    ChangeFileUniqueIdIfExists(LastWorkingFolder+1);
                    checkBoxStopUsingUniqueFile.Checked = (bool)(((byte)(0x77 ^ bytes[512]) == 0) ? false : true);//EncryptCounter == 0x0FFF ? true : false;
                    checkBoxResetLicenses.Checked = (bool)(((byte)(0x77 ^ bytes[515]) == 0) ? false : true);
                    MaxLicenses = (ushort)((byte)(0x77 ^ bytes[516])<<8);
                    MaxLicenses |= (ushort)(byte)(0x77 ^ bytes[517]);
                    textBoxLicense.Text = MaxLicenses.ToString();
                    checkBoxUseSerialCounter.Checked = (bool)(((byte)(0x77 ^ bytes[519]) == 0) ? false : true);
                    checkBoxUseCalibBytes.Checked = (bool)(((byte)(0x77 ^bytes[520]) == 0) ? false : true);
                    textBoxEncryptionCounter.Text = "0x" + EncryptCounter.ToString("X4");
                    textBoxFileUniqueVale.Text = "0x" + FileUniqueNumber.ToString("X4");
                    break;
                }
                else
                {
                    labelStatus.Text = "Failed to decrypt settings with rolling counter value " + j.ToString();
                    labelStatus.ForeColor = Color.Red;
                }
            }
        }
        private void ChangeFileUniqueIdIfExists(int folderNum)
        {
            if (FileUniqueNumber==0)
            {
                FileUniqueNumber = (ushort)(new Random(DateTime.Now.Second).Next(1, 0xFFE));
            }
            for (int i = 0; i < uniqieIds.Length; i++)
            {
                if ((i != folderNum) && (uniqieIds[i] == FileUniqueNumber) && (FileUniqueNumber != 0))
                {
                    FileUniqueNumber = (ushort)(new Random(DateTime.Now.Second).Next(1, 0xFFE));
                    i = 0;
                }
            }
        }
        /*
        private bool CheckFileUniqueIdIfNotExists(int folderNum)
        {
            for (int i = 0; i < uniqieIds.Length; i++)
            {
                if ((i != folderNum) && (uniqieIds[i] == FileUniqueNumber) && (FileUniqueNumber != 0))
                {
                    MessageBox.Show("This file ID is already used in folder '"+i.ToString()+"'.\r\nPlease delete file 'flash.dat' from folder that not in use.");
                    return false;
                }
            }
            return true;
        }
        */
        private void MakeUniqueFileIdsArray()
        {
            ushort readFolderUniqueFileId;
            for (int i = 0; i < uniqieIds.Length; i++)
            {
                readFolderUniqueFileId=DecryptFolderUniqueFileId(i);
                /*
                for (int j = 0; j < i; j++)
                {
                    if ((uniqieIds[j] == readFolderUniqueFileId) && (readFolderUniqueFileId != 0))
                    {
                        //MessageBox.Show("Note: Folder " + i + " contains the same file id as folder " + j);
                    }
                }
                */
                uniqieIds[i] = readFolderUniqueFileId;
            }
        }
        private ushort DecryptFolderUniqueFileId(int folderNum)
        {
            int b1, b2;
            byte RollCounterValue;
            string nextDatFilePathToCheck;
            ushort tempEncryptCounter = 0;
            ushort tempFileUniqueNumber = 0;// (ushort)(new Random(DateTime.Now.Second).Next(1, 0xFFE));            
            if (Directory.Exists(rootfolderpath) == false)
            {
                return 0;
            }
            nextDatFilePathToCheck = Path.Combine(rootfolderpath, folderNum.ToString()) + "\\" + "flash.dat"; ;
            //datfilepath = folderpath + "\\" + "flash.dat";   //+ folderpath.Remove(0, folderpath.LastIndexOf("\\"))         
            if (File.Exists(nextDatFilePathToCheck) == false)
            {
                return 0;
            }
            labelStatus.Text = "Checking the rolling counter is a valid number.";
            RollCounterValue = 0;
            labelStatus.Text = "Reading settings section from dat file to temp var.";
            byte[] bytes = new byte[1000];// File.ReadAllBytes(datfilepath);
            byte[] tempbytes = new byte[bytes.Length];
            byte[] readbytes = File.ReadAllBytes(nextDatFilePathToCheck);
            try
            {
                Buffer.BlockCopy(readbytes, 0, tempbytes, 0, bytes.Length);
            }
            catch (Exception ex)
            {
                //MessageBox.Show(ex.Message + ". Not valid file '" + datfilepath + "'");
                return 0;
            }
            /*decrypt only config data from fle*/
            for (int j = 0; j < roll.Length; j++)//window=5
            {
                labelStatus.Text = "Try decrypt with rolling counter value " + RollCounterValue.ToString();
                Buffer.BlockCopy(tempbytes, 0, bytes, 0, 1000);//bytes.Length
                textBoxRollingCounter.Text = j.ToString();
                progressBarCrypt.Maximum = bytes.Length;
                progressBarCrypt.Minimum = 0;
                progressBarCrypt.Value = progressBarCrypt.Minimum;
                for (int i = 0; i < ("[settings]".Length) && j != 0; ++i)
                {
                    /*ROR*/
                    b1 = ((int)bytes[i] << 1);
                    b2 = ((int)bytes[i] >> 7);
                    bytes[i] = (byte)(b1 | b2);
                    bytes[i] ^= roll[((j) % roll.Length)];//168; // pseudo encrypt
                    progressBarCrypt.Value = i;
                }
                textBox_InputHex.Text = BitConverter.ToString(bytes).Replace("-", "");
                textBox_Input.Text = FromHexString(textBox_InputHex.Text);
                if (String.Compare(textBox_Input.Text, 0, "[settings]", 0, "[settings]".Length, true) == 0)//
                {
                    RollCounterValue = (byte)j;

                    labelStatus.Text = "Successed to decrypt with rolling counter value " + j.ToString();
                    labelStatus.ForeColor = Color.Green;
                    //if (SelectFile(Filetypes.TempConfigFile, "config.ini") == false)
                    //    return;
                    for (int i = 512; i < bytes.Length && j != 0; ++i)
                    {
                        /*ROR*/
                        b1 = ((int)bytes[i] << 1);
                        b2 = ((int)bytes[i] >> 7);
                        bytes[i] = (byte)(b1 | b2);
                        bytes[i] ^= roll[((j) % roll.Length)];//168; // pseudo encrypt
                        progressBarCrypt.Value = i;
                    }
                    labelStatus.Text = "Successed to decrypt settings with rolling counter value " + j.ToString();
                    labelStatus.ForeColor = Color.Black;
                    textBoxRollingCounter.Text = RollCounterValue.ToString();
                    labelStatus.Text = "Reading settings from dat file done with rolling counter value " + j.ToString();
                    if ((bytes[513] != 0x20 && bytes[514] != 0x20) && (bytes[513] != 0xFF && bytes[514] != 0xFF))
                    {
                        //0x12, 0x30 = 0x0123
                        tempEncryptCounter = (ushort)((ushort)((byte)(0x77 ^ bytes[513]) << 4) & 0x0FF0);
                        tempEncryptCounter |= (ushort)((byte)(0x77 ^ bytes[514]) >> 4);
                    }
                    if ((bytes[521] != 0x20 && bytes[522] != 0x20 && bytes[521] != 0xFF && bytes[522] != 0xFF))
                    {
                        tempFileUniqueNumber = (ushort)(((byte)(0x77 ^ bytes[521]) & 0x0F) << 8);
                        tempFileUniqueNumber |= (ushort)(byte)(0x77 ^ bytes[522]);
                    }
                    if (nextDatFilePathToCheck.Contains("flash.dat") == false)
                    {
                        tempEncryptCounter = 0;
                        tempFileUniqueNumber = 0;
                    }
                    return tempFileUniqueNumber;
                }
                else
                {
                    labelStatus.Text = "Failed to decrypt settings with rolling counter value " + j.ToString();
                    labelStatus.ForeColor = Color.Red;
                }
            }
            return 0;
        }
        // Convert string of chars to string of hexcode representation 
        public static string ToHexString(string str)
        {
            var sb = new StringBuilder();

            var bytes = Encoding.Unicode.GetBytes(str);
            foreach (var t in bytes)
            {
                sb.Append(t.ToString("X2"));
            }

            return sb.ToString(); // returns: "48656C6C6F20776F726C64" for "Hello world"
        }

        // Convert string of hexcode to string of chars representation
        public static string FromHexString(string hexString)
        {
            string str;
            var bytes = new byte[hexString.Length / 2];
            for (var i = 0; i < bytes.Length; i++)
            {
                bytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            }
            str = Encoding.UTF8.GetString(bytes); // returns: "Hello world" for "48656C6C6F20776F726C64"
            return str;
        }

        //Convert string of hexcode to byte array
        public byte[] FromHexStringToBytes(string hexString)
        {
            var bytes = new byte[hexString.Length / 2];
            for (var i = 0; i < bytes.Length; i++)
            {
                bytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            }
            return bytes;
        }

        void SaveXMLsettings()
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;

            XmlWriter writer = XmlWriter.Create("Settings.xml", settings);
            writer.WriteStartDocument();
            writer.WriteComment("This file is generated by the encrypt program.");
            writer.WriteStartElement("Project");
            writer.WriteAttributeString("Folder", rootfolderpath);
            writer.WriteAttributeString("Key", textBox_Key.Text);
            writer.WriteAttributeString("XmlsPath", xmlspath);
            writer.WriteEndElement();
            writer.WriteEndDocument();
            writer.Flush();
            writer.Close();
        }
        void ReadXMLSettings()
        {
            if (!File.Exists("Settings.xml"))
            {
                using(File.Create(Directory.GetCurrentDirectory()+"\\Settings.xml"))
                {
                }
                SaveXMLsettings();
            }
            XmlReader reader = XmlReader.Create("Settings.xml");

            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element && reader.Name == "Project")
                {
                    rootfolderpath = reader.GetAttribute(0);
                    if(rootfolderpath=="")
                        rootfolderpath=Directory.GetCurrentDirectory();
                    textBoxPath.Text = rootfolderpath;
                    textBox_Key.Text = reader.GetAttribute(1);
                    xmlspath = "";
                    try
                    {
                        xmlspath = reader.GetAttribute(2);
                    }
                    catch (Exception)
                    {
                        xmlspath = @"C:\Program Files\Atmel\AVR Tools\Partdescriptionfiles";
                    }
                    finally
                    {
                        
                    }
                    break;
                }
            }
            reader.Close();
            if (File.Exists("Parts.xml"))
            {
                using (var sr = new StreamReader("Parts.xml"))
                {
                    XmlSerializer xs = new XmlSerializer(typeof(List<PartStruct>));
                    partlist = (List<PartStruct>)xs.Deserialize(sr);
                }
            }
            else
            {
                if (xmlspath == "" || xmlspath == null)
                    xmlspath = @"C:\Program Files\Atmel\AVR Tools\Partdescriptionfiles";
                if (!Directory.Exists(xmlspath))
                {
                    xmlspath = @"C:\Program Files (x86)\Atmel\AVR Tools\Partdescriptionfiles";
                    if (!Directory.Exists(xmlspath))
                    {
                        FolderBrowserDialog dlg = new FolderBrowserDialog();
                        dlg.Description = "Select a Partdescriptionfiles folder with xml  files";
                        if (dlg.ShowDialog() == DialogResult.OK)
                        {
                            xmlspath = dlg.SelectedPath;
                        }
                    }
                }
            }
            if (!xmlspath.Contains("Partdescriptionfiles"))
            {
                //MessageBox.Show("Cannot find folder 'Partdescriptionfiles' at selected path " + xmlspath + ". Try to set it in settings.xml");
                xmlspath = "";
                //labelStatus.Text = "Reading devices failed.";
            }
            if (Directory.Exists(xmlspath))
            {
                SaveXMLsettings();
            }
            else
            {

                if (partlist.Count == 0)
                {
                    //MessageBox.Show("Parts.xml files does not exists.");
                    labelStatus.Text = "Reading devices failed.";
                    labelStatus.ForeColor = Color.Red;
                }
                else
                {
                    foreach (var item in partlist)
                    {
                        comboBoxChipSelect.Items.Add(item.Part_Name);
                    }
                    comboBoxChipSelect.SelectedIndex = 0;
                    labelStatus.Text = "Reading devices done.";
                    labelStatus.ForeColor = Color.Green;
                }
            }
        }
        void CheckConfig()
        {
            using (FolderBrowserDialog dlg = new FolderBrowserDialog())
            {
                dlg.Description = "Select a folder";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    configfilepath = dlg.SelectedPath;
                    configfilepath += "\\config.ini";
                    MessageBox.Show("You selected: " + configfilepath);
                }
                else
                    return;
            }
            
            if (File.Exists(configfilepath) == false)
            {
                MessageBox.Show(configfilepath+" path not exists");
                File.CreateText(configfilepath);
            }
            ini = new IniFile(configfilepath);
            ini.IniWriteValue("config", "test", "test");
        }

        private void buttonCreateOpenIni_Click(object sender, EventArgs e)
        {
            if (folderpath == null)
            {
                MessageBox.Show("Select folder first.");
                return;
            }
            if (File.Exists(folderpath + "\\config.ini") == false)
            {
                MessageBox.Show("Write settings first.");
                return;
            }
            configfilepath = folderpath + "\\config.ini";
            //if (SelectFile(Filetypes.ConfigFile,"config.ini") == false)
                //return;
            ReadConfigIni(false);

        }

        private void buttonWriteToIni_Click(object sender, EventArgs e)
        {
            if (folderpath == null)
            {
                MessageBox.Show("Select folder first.");
                return;
            }
            configfilepath = folderpath + "\\config.ini";            
            WriteConfigIni();

        }
        bool SelectFile(Filetypes filetype,string filename)
        {
            string filepath=null;
            if (folderpath==null)
            {
                MessageBox.Show("Working folder not selected!");
                return false;
            }
            
            if (filetype==Filetypes.ConfigFile)
            {
                configfilepath = folderpath + "\\" + filename;
                filepath = configfilepath;
                        
            }
            else if (filetype == Filetypes.TempConfigFile)
            {
                tempconfigfilepath = folderpath + "\\" + filename;
                filepath = tempconfigfilepath;
            }
            else if (filetype==Filetypes.DatFile)
            {
                datfilepath = folderpath + "\\" + "flash.dat";// folderpath.Remove(0, folderpath.LastIndexOf("\\") + 1) + ".dat";
                filepath = datfilepath;
                        
            }
            else if (filetype == Filetypes.HexFile)
            {
                if (filename.Length == 0)
                {
                    MessageBox.Show("Hex Filename not selected!");
                    return false;
                }
                hexfilepath = folderpath + "\\" + filename;
                filepath = hexfilepath;

            }
            else if (filetype == Filetypes.SerialFile)
            {
                if (filename.Length == 0)
                {
                    MessageBox.Show("Serial text filename not selected!");
                    return false;
                }
                serialfilepath = folderpath + "\\" + filename;
                filepath = serialfilepath;

            }
            if (File.Exists(filepath) == false)
            {
                try
                {
                    using (File.CreateText(filepath))
                    {

                    }
                }
                catch (Exception)
                {
                    MessageBox.Show(filepath + " path not exists");
                    folderpath = null;
                    configfilepath = null;
                    hexfilepath = null;
                    serialfilepath = null;
                    datfilepath = null;
                    tempconfigfilepath = null;
                    return false;
                }               
            }
            //textBoxPath.Text = filepath;
            return true;
        }

        void ReadConfigIni(bool skipFindFiles)
        {
            if (File.Exists(configfilepath) == false)
            {
                MessageBox.Show(configfilepath+" not found!");
                return;
            }            
            try
            {
                ini = new IniFile(configfilepath);
                textBoxFlashHexFilePath.Text = ini.IniReadValue("settings", "hexfile");
                textBoxBootHexFilePath.Text = ini.IniReadValue("settings", "bootfile");
                if ((textBoxFlashHexFilePath.Text == "")&&(textBoxBootHexFilePath.Text == ""))
                {
                    if (!skipFindFiles)
                    {
                        MessageBox.Show(configfilepath + " file is empty or corrupted!");
                    }
                }                  
                if (textBoxFlashHexFilePath.Text != "")
                {
                        
                    if (textBoxFlashHexFilePath.Text.Contains("\\") == false)
                    {
                        textBoxFlashHexFilePath.Text = folderpath + "\\" + textBoxFlashHexFilePath.Text;
                    }
                    if (File.Exists(textBoxFlashHexFilePath.Text) == false)
                    {
                        if (!skipFindFiles)
                        {
                            MessageBox.Show("File '" + textBoxFlashHexFilePath.Text + "' not exists!");
                        }
                        textBoxFlashHexFilePath.Text = "";
                        textBoxFilename.Text = "";
                        //return;
                    }
                    textBoxFilename.Text = textBoxFlashHexFilePath.Text.Substring(textBoxFlashHexFilePath.Text.LastIndexOf("\\") + 1);
                }
                //textBoxBootHexFilePath.Text = ini.IniReadValue("settings", "bootfile");
                if (textBoxBootHexFilePath.Text != "")
                {
                    if (textBoxBootHexFilePath.Text.Contains("\\") == false)
                    {
                        textBoxBootHexFilePath.Text = folderpath + "\\" + textBoxBootHexFilePath.Text;
                    }
                    if (File.Exists(textBoxBootHexFilePath.Text) == false)
                    {
                        if (!skipFindFiles) 
                            MessageBox.Show("File '" + textBoxBootHexFilePath.Text + "' not exists!");
                        textBoxBootHexFilePath.Text = "";
                        textBoxBootHexFile.Text = "";
                        //return;
                    }
                    textBoxBootHexFile.Text = textBoxBootHexFilePath.Text.Substring(textBoxBootHexFilePath.Text.LastIndexOf("\\") + 1);
                }
                else
                {
                    textBoxBootHexFilePath.Text = "";
                    textBoxBootHexFile.Text = "";
                }
                textBoxEepromHexFilePath.Text = ini.IniReadValue("settings", "eepromfile");
                if (textBoxEepromHexFilePath.Text != "")
                {
                    if (textBoxEepromHexFilePath.Text.Contains("\\") == false)
                    {
                        textBoxEepromHexFilePath.Text = folderpath + "\\" + textBoxEepromHexFilePath.Text;
                    }
                    if (File.Exists(textBoxEepromHexFilePath.Text) == false)
                    {
                        if (!skipFindFiles) 
                            MessageBox.Show("File '" + textBoxEepromHexFilePath.Text + "' not exists!");
                        textBoxEepromHexFilePath.Text = "";
                        textBoxEepromHexFile.Text = "";
                        //return;
                    }
                    textBoxEepromHexFile.Text = textBoxEepromHexFilePath.Text.Substring(textBoxEepromHexFilePath.Text.LastIndexOf("\\") + 1);
                }
                else
                {
                    textBoxEepromHexFilePath.Text = "";
                    textBoxEepromHexFile.Text = "";
                }
                String infostr = ini.IniReadValue("settings", "progname");
                if (infostr.Length > 12)
                    infostr = infostr.Remove(12);
                textBoxProgname.Text = infostr;
                infostr=ini.IniReadValue("settings", "proginfo");
                if (infostr.Length > 15)
                {
                    textBoxFileInfo.Text = infostr.Remove(15);
                    textBoxFileInfo2.Text = ini.IniReadValue("settings", "proginfo").Remove(0, 15);
                }
                else
                {
                    textBoxFileInfo.Text = infostr;
                    textBoxFileInfo2.Text = "";
                }
                if ((comboBoxChipSelect.Text = TryParseStringFromSettings("chipname", "")) == "")
                {
                    MessageBox.Show("Chipname is missing in config.ini file");
                    return;
                }
                textBoxSignature.Text = (textBoxSignature.Text = TryParseValueFromSettings("signature", 0).ToString("X6")) == "000000" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].Signature :
                    textBoxSignature.Text;
                if ((textBoxProgFuseHigh.Text = TryParseValueFromSettings("progfuses", 0).ToString("X8")) == "00000000")
                {
                    textBoxProgFuseHigh.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].HighFuseDef;
                    textBoxProgFuseLock.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LockBits;
                    textBoxProgFuseLow.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LowFuseDef;
                    textBoxProgFuseExt.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].ExtFuseDef;
                }
                else
                {
                    textBoxProgFuseLock.Text = "0x" + textBoxProgFuseHigh.Text.Substring(0, 2);
                    textBoxProgFuseLow.Text = "0x" + textBoxProgFuseHigh.Text.Substring(2, 2);
                    textBoxProgFuseExt.Text = "0x" + textBoxProgFuseHigh.Text.Substring(6, 2);
                    textBoxProgFuseHigh.Text = "0x" + textBoxProgFuseHigh.Text.Substring(4, 2);
                }
                if ((textBoxFinalFuseHigh.Text = TryParseValueFromSettings("normfuses", 0).ToString("X8")) == "00000000")
                {
                    textBoxFinalFuseHigh.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].HighFuseDef;
                    textBoxFinalFuseLock.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LockBits;
                    textBoxFinalFuseLow.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LowFuseDef;
                    textBoxFinalFuseExt.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].ExtFuseDef;
                }
                else
                {
                    textBoxFinalFuseLock.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(0, 2);
                    textBoxFinalFuseLow.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(2, 2);
                    textBoxFinalFuseExt.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(6, 2);
                    textBoxFinalFuseHigh.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(4, 2);
                }
                textBoxProgFuseHigh_Leave(textBoxFinalFuseLock, null);
                textBoxProgFuseHigh_Leave(textBoxFinalFuseLow, null);
                textBoxProgFuseHigh_Leave(textBoxFinalFuseExt, null);
                textBoxProgFuseHigh_Leave(textBoxFinalFuseHigh, null);
                if ((textBoxMaskFuseHigh.Text = TryParseValueFromSettings("maskfuses", 0).ToString("X8")) == "00000000")
                {
                    textBoxMaskFuseHigh.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].HighFuseDef;
                    textBoxMaskFuseLock.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LockBits;
                    textBoxMaskFuseLow.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LowFuseDef;
                    textBoxMaskFuseExt.Text = 
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].ExtFuseDef;
                }
                else
                {
                    textBoxMaskFuseLock.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(0, 2);
                    textBoxMaskFuseLow.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(2, 2);
                    textBoxMaskFuseExt.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(6, 2);
                    textBoxMaskFuseHigh.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(4, 2);
                }
                textBoxChipsizeBytes.Text = (textBoxChipsizeBytes.Text =TryParseValueFromSettings("chipsize",0).ToString())=="0"? 
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].Prog_Flash:
                    textBoxChipsizeBytes.Text;                
                textBoxPagesizeWords.Text = (textBoxPagesizeWords.Text=TryParseValueFromSettings( "pagesize",0).ToString())=="0"? 
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].FlashPageSize:
                    textBoxPagesizeWords.Text;
                textBoxEepromSizeBytes.Text = (textBoxEepromSizeBytes.Text=TryParseValueFromSettings( "eepromsize",0).ToString())=="0"?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].Prog_Eeprom:
                    textBoxEepromSizeBytes.Text;
                textBoxEepromPageSizeBytes.Text = (textBoxEepromPageSizeBytes.Text = TryParseValueFromSettings("eeprompagesize", 0).ToString()) == "0" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].EepromPageSize :
                    textBoxEepromPageSizeBytes.Text;
                //textBoxSPIDIV.Text = TryParseValueFromSettings("spidiv", 0).ToString();
                comboBoxIspClock.SelectedIndex = (int)TryParseValueFromSettings("spidiv", 0);
                labelStatus.Text = "Reading ini file done";
                labelStatus.ForeColor = Color.Green;
                SelectFile(Filetypes.SerialFile, "serial.txt");
                ini = new IniFile(serialfilepath);
                //checkBoxUseSerialCounter.Checked = TryParseValueFromSettings("nextserial", 0) == 0 ? false : true;
                textBoxEepromStartAddress.Text = "0x"+TryParseValueFromSettings("startaddress", 0).ToString("X4");
                textBoxEepromStartAddress_Leave(textBoxEepromStartAddress,null);
                SelectFile(Filetypes.SerialFile, "calbytes.txt");
                ini = new IniFile(serialfilepath);
                //checkBoxUseCalibBytes.Checked = TryParseValueFromSettings("enabled", 0) == 0 ? false : true;
                textBoxEepromStartAddressCalibBytes.Text = "0x" + TryParseValueFromSettings("startaddress", 0).ToString("X4");
                textBoxEepromStartAddressCalibBytes_Leave(textBoxEepromStartAddressCalibBytes, null);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
                labelStatus.Text = "Reading ini file failed!";
                labelStatus.ForeColor = Color.Red;
                //throw;
            }
            
        }
        string TryParseStringFromSettings(string Key, string defstring)
        {
            try
            {
                return ini.IniReadValue("settings", Key);
            }
            catch (Exception)
            {
                MessageBox.Show("Key '" + Key + "' missing in config.ini file.");
                return defstring;
                //throw;
            }

        }
        UInt32 TryParseValueFromSettings(string Key, UInt32 defnum)
        {
            try
            {
                return TryParseNumOrDef(ini.IniReadValue("settings", Key), defnum);
            }
            catch (Exception)
            {
                MessageBox.Show("Key '" + Key + "' missing in config.ini file.");
                return defnum;               
                //throw;
            }
            
        }
        void ConvFuseStructToCheckBox(ref CheckBox cb, FuseStruct fs,int bit_num)
        {
            //CheckBox cb = new CheckBox();
            cb.Checked = fs.IsChecked[bit_num];
            cb.Enabled = fs.IsEnabled[bit_num];
            cb.Text = fs.Text[bit_num];
            cb.Tag = fs.Name[bit_num];
            //return cb;
        }
        void UpdateFuseCheckBoxes(PartStruct ps)
        {
            //partlist[comboBoxChipSelect.SelectedIndex]
            ConvFuseStructToCheckBox(ref cb_fuse_ext0,ps.cb_fuse_ext, 0);
            ConvFuseStructToCheckBox(ref cb_fuse_ext1, ps.cb_fuse_ext, 1);
            ConvFuseStructToCheckBox(ref cb_fuse_ext2, ps.cb_fuse_ext, 2);
            ConvFuseStructToCheckBox(ref cb_fuse_ext3, ps.cb_fuse_ext, 3);
            ConvFuseStructToCheckBox(ref cb_fuse_ext4, ps.cb_fuse_ext, 4);
            ConvFuseStructToCheckBox(ref cb_fuse_ext5, ps.cb_fuse_ext, 5);
            ConvFuseStructToCheckBox(ref cb_fuse_ext6, ps.cb_fuse_ext, 6);
            ConvFuseStructToCheckBox(ref cb_fuse_ext7, ps.cb_fuse_ext, 7);
            ConvFuseStructToCheckBox(ref cb_fuse_high0, ps.cb_fuse_high, 0);
            ConvFuseStructToCheckBox(ref cb_fuse_high1, ps.cb_fuse_high, 1);
            ConvFuseStructToCheckBox(ref cb_fuse_high2, ps.cb_fuse_high, 2);
            ConvFuseStructToCheckBox(ref cb_fuse_high3, ps.cb_fuse_high, 3);
            ConvFuseStructToCheckBox(ref cb_fuse_high4, ps.cb_fuse_high, 4);
            ConvFuseStructToCheckBox(ref cb_fuse_high5, ps.cb_fuse_high, 5);
            ConvFuseStructToCheckBox(ref cb_fuse_high6, ps.cb_fuse_high, 6);
            ConvFuseStructToCheckBox(ref cb_fuse_high7, ps.cb_fuse_high, 7);
            ConvFuseStructToCheckBox(ref cb_fuse_low0, ps.cb_fuse_low, 0);
            ConvFuseStructToCheckBox(ref cb_fuse_low1, ps.cb_fuse_low, 1);
            ConvFuseStructToCheckBox(ref cb_fuse_low2, ps.cb_fuse_low, 2);
            ConvFuseStructToCheckBox(ref cb_fuse_low3, ps.cb_fuse_low, 3);
            ConvFuseStructToCheckBox(ref cb_fuse_low4, ps.cb_fuse_low, 4);
            ConvFuseStructToCheckBox(ref cb_fuse_low5, ps.cb_fuse_low, 5);
            ConvFuseStructToCheckBox(ref cb_fuse_low6, ps.cb_fuse_low, 6);
            ConvFuseStructToCheckBox(ref cb_fuse_low7, ps.cb_fuse_low, 7);
            ConvFuseStructToCheckBox(ref cb_lock_bit0, ps.cb_lock_bits, 0);
            ConvFuseStructToCheckBox(ref cb_lock_bit1, ps.cb_lock_bits, 1);
            ConvFuseStructToCheckBox(ref cb_lock_bit2, ps.cb_lock_bits, 2);
            ConvFuseStructToCheckBox(ref cb_lock_bit3, ps.cb_lock_bits, 3);
            ConvFuseStructToCheckBox(ref cb_lock_bit4, ps.cb_lock_bits, 4);
            ConvFuseStructToCheckBox(ref cb_lock_bit5, ps.cb_lock_bits, 5);
            ConvFuseStructToCheckBox(ref cb_lock_bit6, ps.cb_lock_bits, 6);
            ConvFuseStructToCheckBox(ref cb_lock_bit7, ps.cb_lock_bits, 7);
            cb_fuse_high7_CheckedChanged(null,null);
        }
        UInt32 TryParseNumOrDef(string str, UInt32 defnum)
        {
            UInt32 num;
            if (str.Contains('h'))//hex value
            {
                if(UInt32.TryParse(str.Replace("h",""),System.Globalization.NumberStyles.HexNumber,null,out num)==false)
                    return defnum;
                return num;
            }
            if (UInt32.TryParse(str, System.Globalization.NumberStyles.None, null, out num) == false)
                return defnum;
            return num;
        }
        bool WriteConfigIni()
        {
            try
            {
                //uint num;textBoxFileInfo.Text==""||
                if ((textBoxFilename.Text == "" && textBoxBootHexFile.Text == "")|| textBoxProgname.Text == ""  || comboBoxChipSelect.Text == "" ||  
                    textBoxProgFuseLock.Text=="" || textBoxProgFuseLow.Text=="" || textBoxProgFuseHigh.Text=="" ||  textBoxProgFuseExt.Text=="" || 
                    textBoxFinalFuseLock.Text=="" || textBoxFinalFuseLow.Text=="" || textBoxFinalFuseHigh.Text=="" || textBoxFinalFuseExt.Text=="" ||
                    textBoxMaskFuseLock.Text=="" || textBoxMaskFuseLow.Text=="" || textBoxMaskFuseHigh.Text=="" || textBoxMaskFuseExt.Text=="" ||
                    textBoxSignature.Text.Length!=6 )
                {
                    MessageBox.Show("Not all fields filled correctly");
                    return false;
                }
                if (textBoxFilename.Text != "")
                {
                    if (File.Exists(textBoxFlashHexFilePath.Text) == false)
                    {
                        MessageBox.Show("Flash hex file not exists!");
                        return false;
                    }
                }
                if (textBoxBootHexFilePath.Text != "")
                {
                    if (File.Exists(textBoxBootHexFilePath.Text) == false)
                    {
                        MessageBox.Show("Boot hex file not exists!");
                        return false;
                    }
                }
                
                ini = new IniFile(configfilepath);
                if (File.Exists(configfilepath) == true)
                {
                    File.Delete(configfilepath);
                }
                using (File.CreateText(configfilepath))
                {

                }
                
                if (textBoxProgname.Text.Length > 12)
                    textBoxProgname.Text=textBoxProgname.Text.Remove(12);
                ini.IniWriteValue("settings", "progname", textBoxProgname.Text);
                if (textBoxFileInfo.Text.Length > 15)
                    textBoxFileInfo.Text = textBoxFileInfo.Text.Remove(15);
                else if (textBoxFileInfo.Text.Length < 15)
                    while (textBoxFileInfo.Text.Length < 15)
			        {
			            textBoxFileInfo.Text=textBoxFileInfo.Text+" ";
			        }
                if (textBoxFileInfo2.Text.Length > 15)
                    textBoxFileInfo2.Text = textBoxFileInfo2.Text.Remove(15);
                else if (textBoxFileInfo2.Text.Length < 15)
                    while (textBoxFileInfo2.Text.Length < 15)
                    {
                        textBoxFileInfo2.Text = textBoxFileInfo2.Text + " ";
                    }
                ini.IniWriteValue("settings", "proginfo", textBoxFileInfo.Text + textBoxFileInfo2.Text);
                
                if (!TryWriteValueToIni("signature",labelSignature, textBoxSignature.Text, true, 6, 0, 0))
                    return false;
                if (!TryWriteValueToIni("progfuses",labelFusesDefaults, (textBoxProgFuseLock.Text + textBoxProgFuseLow.Text + textBoxProgFuseHigh.Text + textBoxProgFuseExt.Text).Replace("0x",""), true, 8, 0, 0))
                    return false;
                if (!TryWriteValueToIni("normfuses", labelFusesFinal, (textBoxFinalFuseLock.Text + textBoxFinalFuseLow.Text + textBoxFinalFuseHigh.Text + textBoxFinalFuseExt.Text).Replace("0x",""), true, 8, 0, 0))
                    return false;
                if (!TryWriteValueToIni("maskfuses", labelFusesMask, (textBoxMaskFuseLock.Text + textBoxMaskFuseLow.Text + textBoxMaskFuseHigh.Text + textBoxMaskFuseExt.Text).Replace("0x",""), true, 8, 0, 0))
                    return false;
                if (!TryWriteValueToIni("chipsize", labelFlashSize, textBoxChipsizeBytes.Text, false, 0, 1024, 262144))
                    return false;
                if (!TryWriteValueToIni("pagesize", labelFlashPageSize, textBoxPagesizeWords.Text, false, 0, 0, 256))
                    return false;
                if (!TryWriteValueToIni("eepromsize", labelEepromSize, textBoxEepromSizeBytes.Text, false, 0, 0, 8192))
                    return false;
                if (!TryWriteValueToIni("eeprompagesize", labelEepromPageSize, textBoxEepromPageSizeBytes.Text, false, 0, 0, 256))
                    return false;
                /*
                if (!TryWriteValueToIni("maxprogs", labelLicenseTotal, textBoxLicense.Text, false, 0, 0, 0xFFFF))
                    return false;
                if (!TryWriteValueToIni("rollcounter", labelRollingCounter, textBoxRollingCounter.Text, false, 0, 0, 127))
                    return false;
                */
                //if (!TryWriteValueToIni("spidiv", labelSpiDiv, textBoxSPIDIV.Text, false, 0, 0, 7))
                if (!TryWriteValueToIni("spidiv", labelSpiDiv, comboBoxIspClock.SelectedIndex.ToString(), false, 0, 0, 7))
                    return false;
                //ini.IniWriteValue("settings", "progcounterreset", checkBoxResetLicenses.Checked==true?"1":"0");
                ini.IniWriteValue("settings", "save_serial", (checkBoxUseSerialCounter.Checked == false) ? "0" : "1");
                ini.IniWriteValue("settings", "save_calib", (checkBoxUseCalibBytes.Checked == false) ? "0" : "1");
                if (File.Exists(textBoxEepromHexFilePath.Text))
                {
                    ini.IniWriteValue("settings", "eepromfile", textBoxEepromHexFilePath.Text);
                }
                ini.IniWriteValue("settings", "hexfile", (textBoxFlashHexFilePath.Text.Contains('\\') == true) ?
                    textBoxFlashHexFilePath.Text : textBoxFilename.Text);
                ini.IniWriteValue("settings", "bootfile", (textBoxBootHexFilePath.Text.Contains('\\') == true) ?
                    textBoxBootHexFilePath.Text : textBoxBootHexFile.Text);
                if (comboBoxChipSelect.Text.Length < 5)
                {
                    MessageBox.Show("Incorrect chip selector name");
                    return false;
                }
                
                ini.IniWriteValue("settings", "chipname", comboBoxChipSelect.Text);
                labelStatus.Text = "Writing ini file done.";
                labelStatus.ForeColor = Color.Green;
                SelectFile(Filetypes.SerialFile, "serial.txt");
                ini = new IniFile(serialfilepath);
                if (!TryWriteValueToIni("startaddress", labelEepromStartAddress, textBoxEepromStartAddress.Text.Replace("0x",""), true, 4, 0, 0xFFFF))
                    return false;
                if (checkBoxUseSerialCounter.Checked == false)
                {
                    ini.IniWriteValue("settings", "nextserial","00000000h");
                }
                else
                {
                    if(TryParseValueFromSettings("nextserial", 0) == 0)
                        ini.IniWriteValue("settings", "nextserial", "00000001h");
                }
                SelectFile(Filetypes.SerialFile, "calbytes.txt");
                ini = new IniFile(serialfilepath);
                if (!TryWriteValueToIni("startaddress", labelEepromStartAddressCalibBytes, textBoxEepromStartAddressCalibBytes.Text.Replace("0x", ""), true, 4, 0, 0xFFFF))
                    return false;
                ini.IniWriteValue("settings", "calibbytes", "00000000h");
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message+"\r\n"+ex.StackTrace);
                labelStatus.Text = "Writing ini file failed!";
                labelStatus.ForeColor = Color.Red;
                return false;
                //throw;
            }
            return true;
            //MessageBox.Show("Writing to config.ini file done.");
        }
        private bool TryWriteValueToIni(string Key, Label lbl ,string textString,bool isHex, UInt32 strLength, UInt32 minValue, UInt32 maxValue)
        {
            UInt32 num;
            if (UInt32.TryParse(textString,isHex?NumberStyles.HexNumber:NumberStyles.None,null, out num))
            {
                if (isHex && textString.Length == strLength)//hex case
                {
                    ini.IniWriteValue("settings", Key, num.ToString("X" + strLength.ToString()) + "h");
                    return true;
                }
                else if (!isHex && num >= minValue && num <= maxValue)//decimal case
                {
                    ini.IniWriteValue("settings", Key, num.ToString());
                    return true;
                }                   
            }
            MessageBox.Show("'"+lbl.Text + "' field is icorrect.");
            return false;
        }
        private void buttonSelectFolder_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog dlg = new FolderBrowserDialog())
            {
                try
                {
                    ReadXMLSettings();
                }
                catch (Exception ex)
                {                    
                    MessageBox.Show(ex.Message);
                }
                dlg.Description = "Select a folder";
                if (folderpath != null)
                {
                    dlg.SelectedPath = rootfolderpath;
                    
                }
                else
                    dlg.SelectedPath = Directory.GetCurrentDirectory();
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    /*
                    Int32 foldnum=0;
                    Int32.TryParse(dlg.SelectedPath.Substring(dlg.SelectedPath.LastIndexOf("\\") + 1,
                        dlg.SelectedPath.Length - (dlg.SelectedPath.LastIndexOf("\\") + 1)), NumberStyles.None, null, out foldnum);
                    if (foldnum < 1 || foldnum > 199)
                    {
                        MessageBox.Show("Folder name must be a number from 1 to 199");
                        return;
                    }
                     */

                    rootfolderpath = dlg.SelectedPath;//folderpath = dlg.SelectedPath;
                    if (ReadRootConfig() == false)
                    {
                        //MessageBox.Show("No config at root folder");
                        DialogResult dialogResult = MessageBox.Show("This root folder not contain configuration files.\r\nDo you want create it here?", 
                            "Create configuration files dialog box.", MessageBoxButtons.YesNo);
                        if (dialogResult == DialogResult.Yes)
                        {
                            //do something
                            try
                            {
                                Directory.CreateDirectory(rootfolderpath + "\\0");
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.Message);
                                labelStatus.ForeColor = Color.Red;
                                labelStatus.Text = ex.Message;
                                ResetForm();
                            }
                            ini = new IniFile(rootfolderpath + "\\0\\config.ini");
                            ini.path = rootfolderpath + "\\0\\config.ini";
                            ini.IniWriteValue("settings", "maxfolders", "10");
                            ini.IniWriteValue("settings", "up", "1");
                            ini.IniWriteValue("settings", "down", "1");
                            ReadRootConfig();
                        }
                        else if (dialogResult == DialogResult.No)
                        {
                            //do something else
                            rootfolderpath = "";
                        }
                    }
                    textBoxPath.Text = rootfolderpath;
                    if (rootfolderpath=="")
                    {
                        return;
                    }
                    SaveXMLsettings();
                    cancelAsyncButton.Enabled = false;
                    buttonCreateOpenIni.Enabled = true;
                    buttonOpenConfig.Enabled = true;
                    buttonSelectFolder.Enabled = true;
                    buttonWriteToIni.Enabled = true;
                    buttonWriteConfig.Enabled = true;
                    buttonSelectHEX.Enabled = true;
                    buttonSelectBoot.Enabled = true;
                    buttonSelectEEPROM.Enabled = true;
                    buttonRefresh.Enabled = true;
                    buttonMakeList.Enabled = true;
                    buttonReadChipConfig.Enabled = true;
                    buttonWriteChipConfig.Enabled = true;
                }
            }
        }
        private void buttonSelectHEX_Click(object sender, EventArgs e)
        {
            if (folderpath == null)
            {
                MessageBox.Show("Select folder first.");
                return;
            }
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                // Create an instance of the open file dialog box.
                //OpenFileDialog openFileDialog1 = new OpenFileDialog();

                // Set filter options and filter index.
                dlg.Filter = "Hex Files (.hex)|*.hex";
                dlg.FilterIndex = 1;
                dlg.Multiselect = false;
                if (textBoxFlashHexFilePath.Text != "")
                {
                    dlg.InitialDirectory = textBoxFlashHexFilePath.Text.Substring(0,textBoxFlashHexFilePath.Text.LastIndexOf('\\'));
                }
                else
                    dlg.InitialDirectory = folderpath;
                // Call the ShowDialog method to show the dialog box.
                //DialogResult userClickedOK = (dlg.ShowDialog());
                //dlg.Description = "Select a hex file";
                
                if (dlg.ShowDialog() == DialogResult.OK)
                {                    
                    textBoxFilename.Text = dlg.FileName.Substring(dlg.FileName.LastIndexOf("\\")+1);
                    textBoxFlashHexFilePath.Text = dlg.FileName;
                }
                else
                {
                    textBoxFilename.Text = "";
                    textBoxFlashHexFilePath.Text = "";
                }
            }
        }
        private void buttonSelectBoot_Click(object sender, EventArgs e)
        {
            if (folderpath == null)
            {
                MessageBox.Show("Select folder first.");
                return;
            }
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                // Create an instance of the open file dialog box.
                //OpenFileDialog openFileDialog1 = new OpenFileDialog();

                // Set filter options and filter index.
                dlg.Filter = "Hex Files (.hex)|*.hex";
                dlg.FilterIndex = 1;
                dlg.Multiselect = false;
                if (textBoxBootHexFilePath.Text != "")
                    dlg.InitialDirectory = textBoxBootHexFilePath.Text.Substring(0,textBoxBootHexFilePath.Text.LastIndexOf('\\'));
                else
                    dlg.InitialDirectory = folderpath;
                // Call the ShowDialog method to show the dialog box.
                //DialogResult userClickedOK = (dlg.ShowDialog());
                //dlg.Description = "Select a hex file";

                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    textBoxBootHexFile.Text = dlg.FileName.Substring(dlg.FileName.LastIndexOf("\\") + 1);
                    textBoxBootHexFilePath.Text = dlg.FileName;
                }
                else
                {
                    textBoxBootHexFile.Text = "";
                    textBoxBootHexFilePath.Text = "";
                }
            }
        }
        private void buttonSelectEEPROM_Click(object sender, EventArgs e)
        {
            if (folderpath == null)
            {
                MessageBox.Show("Select folder first.");
                return;
            }
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                // Create an instance of the open file dialog box.
                //OpenFileDialog openFileDialog1 = new OpenFileDialog();

                // Set filter options and filter index.
                dlg.Filter = "Hex Files (.hex)|*.hex";
                dlg.FilterIndex = 1;
                dlg.Multiselect = false;
                if (textBoxEepromHexFilePath.Text != "")
                    dlg.InitialDirectory = textBoxEepromHexFilePath.Text.Substring(0,textBoxEepromHexFilePath.Text.LastIndexOf('\\'));
                else
                    dlg.InitialDirectory = folderpath;
                // Call the ShowDialog method to show the dialog box.
                //DialogResult userClickedOK = (dlg.ShowDialog());
                //dlg.Description = "Select a hex file";

                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    textBoxEepromHexFile.Text = dlg.FileName.Substring(dlg.FileName.LastIndexOf("\\") + 1);
                    textBoxEepromHexFilePath.Text = dlg.FileName;
                }
                else
                {
                    textBoxEepromHexFilePath.Text = "";
                    textBoxEepromHexFile.Text = "";
                }
            }
        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveXMLsettings();
        }
        private void NOP(long durationTicks)
        {
            Stopwatch sw = new Stopwatch();

            sw.Start();

            while (sw.ElapsedTicks < durationTicks)
            {

            }
        }

        private void comboBoxChipSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            uint num;
            if ((comboBoxChipSelect.SelectedIndex < 0) || partlist==null)
                return;
            textBoxChipsizeBytes.Text = partlist[comboBoxChipSelect.SelectedIndex].Prog_Flash;
            num=(UInt32.TryParse(partlist[comboBoxChipSelect.SelectedIndex].FlashPageSize, out num) == true) ? num : 0;
            textBoxPagesizeWords.Text = (num/2).ToString();
            textBoxEepromSizeBytes.Text = partlist[comboBoxChipSelect.SelectedIndex].Prog_Eeprom;
            textBoxEepromPageSizeBytes.Text = partlist[comboBoxChipSelect.SelectedIndex].EepromPageSize;
            textBoxSignature.Text = partlist[comboBoxChipSelect.SelectedIndex].Signature;
            textBoxProgFuseHigh.Text = partlist[comboBoxChipSelect.SelectedIndex].HighFuseDef;
            textBoxMaskFuseHigh.Text = partlist[comboBoxChipSelect.SelectedIndex].HighFuseMask;
            textBoxProgFuseLow.Text = partlist[comboBoxChipSelect.SelectedIndex].LowFuseDef;
            textBoxMaskFuseLow.Text = partlist[comboBoxChipSelect.SelectedIndex].LowFuseMask;
            textBoxProgFuseLock.Text = partlist[comboBoxChipSelect.SelectedIndex].LockBits;
            textBoxMaskFuseLock.Text = partlist[comboBoxChipSelect.SelectedIndex].LockBitsMask;
            textBoxProgFuseExt.Text = partlist[comboBoxChipSelect.SelectedIndex].ExtFuseDef;
            textBoxMaskFuseExt.Text = partlist[comboBoxChipSelect.SelectedIndex].ExtFuseMask;
            UpdateFuseCheckBoxes(partlist[comboBoxChipSelect.SelectedIndex]);
        }

        private void checkBoxResetLicenses_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Name.Contains("Serial"))
            {
                textBoxEepromStartAddress.Enabled = ((CheckBox)sender).Checked;
            }
            else if (((CheckBox)sender).Name.Contains("Calib"))
            {
                textBoxEepromStartAddressCalibBytes.Enabled = ((CheckBox)sender).Checked;
            }
            else if (((CheckBox)sender).Name.Contains("StopUsing"))
            {
                //textBoxEncryptionCounter.Text = ((CheckBox)sender).Checked==true?"0x0FFF":"0x"+EncryptCounter.ToString("X4");
            }
        }
        private void checkBoxUseCalibBytes_CheckedChanged(object sender, EventArgs e)
        {
            textBoxEepromStartAddressCalibBytes.Enabled = ((CheckBox)sender).Checked;
        }
        private void checkBoxUseEepromHex_CheckedChanged(object sender, EventArgs e)
        {
            if (DoEventUseEeprom==false)
            {
                return;
            }
            CheckBox cb = (CheckBox)sender;
            if (cb.Checked==true)
            {               
                if (File.Exists(textBoxEepromHexFilePath.Text)==false)
                {                   
                    MessageBox.Show("EEPROM hex file not exists!");
                    DoEventUseEeprom = false;
                    cb.Checked = false;
                    DoEventUseEeprom = true;
                    return ;
                }
                else
                {
                    File.Copy(textBoxEepromHexFilePath.Text, folderpath + "\\eeprom.hex", true);                    
                }
            }
            else
            {
                File.Delete(folderpath + "\\eeprom.hex");
            }
        }

        private void textBoxProgFuseHigh_Leave(object sender, EventArgs e)
        {
            byte num,mask_num=0;
            TextBox tb=(TextBox) sender;
            if (Byte.TryParse(tb.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out num))
            {
                tb.Text = "0x" + num.ToString("X2");
                if (tb.Name.Contains("Final") == false)
                {
                    return;
                }
                DoEventChangeFuses = false;
                if (tb.Name.Contains("High") == true)
                    Byte.TryParse(textBoxMaskFuseHigh.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out mask_num);
                else if (tb.Name.Contains("Low") == true)
                    Byte.TryParse(textBoxMaskFuseLow.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out mask_num);
                else if (tb.Name.Contains("Ext") == true)
                    Byte.TryParse(textBoxMaskFuseExt.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out mask_num);
                else if (tb.Name.Contains("Lock") == true)
                    Byte.TryParse(textBoxMaskFuseLock.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out mask_num);
                //num &= mask_num;
                tb.Text = "0x" + num.ToString("X2");
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high0.Checked = (((num & (1 << 0)) == 0) ? cb_fuse_high0.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low0.Checked = (((num & (1 << 0)) == 0) ? cb_fuse_low0.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext0.Checked = (((num & (1 << 0)) == 0) ? cb_fuse_ext0.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit0.Checked = (((num & (1 << 0)) == 0) ? cb_lock_bit0.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high1.Checked = (((num & (1 << 1)) == 0) ? cb_fuse_high1.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low1.Checked = (((num & (1 << 1)) == 0) ? cb_fuse_low1.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext1.Checked = (((num & (1 << 1)) == 0) ? cb_fuse_ext1.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit1.Checked = (((num & (1 << 1)) == 0) ? cb_lock_bit1.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high2.Checked = (((num & (1 << 2)) == 0) ? cb_fuse_high2.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low2.Checked = (((num & (1 << 2)) == 0) ? cb_fuse_low2.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext2.Checked = (((num & (1 << 2)) == 0) ? cb_fuse_ext2.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit2.Checked = (((num & (1 << 2)) == 0) ? cb_lock_bit2.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high3.Checked = (((num & (1 << 3)) == 0) ? cb_fuse_high3.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low3.Checked = (((num & (1 << 3)) == 0) ? cb_fuse_low3.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext3.Checked = (((num & (1 << 3)) == 0) ? cb_fuse_ext3.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit3.Checked = (((num & (1 << 3)) == 0) ? cb_lock_bit3.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high4.Checked = (((num & (1 << 4)) == 0) ? cb_fuse_high4.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low4.Checked = (((num & (1 << 4)) == 0) ? cb_fuse_low4.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext4.Checked = (((num & (1 << 4)) == 0) ? cb_fuse_ext4.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit4.Checked = (((num & (1 << 4)) == 0) ? cb_lock_bit4.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high5.Checked = (((num & (1 << 5)) == 0) ? cb_fuse_high5.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low5.Checked = (((num & (1 << 5)) == 0) ? cb_fuse_low5.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext5.Checked = (((num & (1 << 5)) == 0) ? cb_fuse_ext5.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit5.Checked = (((num & (1 << 5)) == 0) ? cb_lock_bit5.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high6.Checked = (((num & (1 << 6)) == 0) ? cb_fuse_high6.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low6.Checked = (((num & (1 << 6)) == 0) ? cb_fuse_low6.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext6.Checked = (((num & (1 << 6)) == 0) ? cb_fuse_ext6.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit6.Checked = (((num & (1 << 6)) == 0) ? cb_lock_bit6.Enabled : false);
                if (tb.Name.Contains("High") == true)
                    cb_fuse_high7.Checked = (((num & (1 << 7)) == 0) ? cb_fuse_high7.Enabled : false);
                else if (tb.Name.Contains("Low") == true)
                    cb_fuse_low7.Checked = (((num & (1 << 7)) == 0) ? cb_fuse_low7.Enabled : false);
                else if (tb.Name.Contains("Ext") == true)
                    cb_fuse_ext7.Checked = (((num & (1 << 7)) == 0) ? cb_fuse_ext7.Enabled : false);
                else if (tb.Name.Contains("Lock") == true)
                    cb_lock_bit7.Checked = (((num & (1 << 7)) == 0) ? cb_lock_bit7.Enabled : false);
            }
            else
            {
                DoEventChangeFuses = true;
                tb.Text = "";
            }
            DoEventChangeFuses = true;
        }

        private void textBoxEepromStartAddress_Leave(object sender, EventArgs e)
        {
            UInt16 num;
            TextBox tb = (TextBox)sender;
            if (UInt16.TryParse(tb.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out num))
            {
                tb.Text = "0x" + num.ToString("X4");
            }
            else
                tb.Text = "0x0000";
        }
        private void textBoxLicense_MouseLeave(object sender, EventArgs e)
        {
            int num;
            TextBox tb = (TextBox)sender;
            if (int.TryParse(tb.Text, NumberStyles.Integer, null, out num))
            {
                if ((num < 0) || (num > 0xffff))
                {
                    MessageBox.Show("Value out of range. Should be 0..65535");
                    tb.Text = "0";
                }
                else
                {
                    tb.Text = num.ToString();
                    MaxLicenses = (ushort)num;
                }
            }
            else
                tb.Text = "0";
        }
        private void textBoxEepromStartAddressCalibBytes_Leave(object sender, EventArgs e)
        {
            UInt16 num;
            TextBox tb = (TextBox)sender;
            if (UInt16.TryParse(tb.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out num))
            {
                tb.Text = "0x" + num.ToString("X4");
            }
            else
                tb.Text = "0x0000";
        }
        private void cb_fuse_high7_CheckedChanged(object sender, EventArgs e)
        {
            int num = 0;
            if (DoEventChangeFuses == false)
            {
                return;
            }
            num =  (cb_lock_bit0.Checked && cb_lock_bit0.Enabled == true ? 0 : 1) 
                | ((cb_lock_bit1.Checked && cb_lock_bit1.Enabled == true ? 0 : 1) << 1)
                | ((cb_lock_bit2.Checked && cb_lock_bit2.Enabled == true ? 0 : 1) << 2) 
                | ((cb_lock_bit3.Checked && cb_lock_bit3.Enabled == true ? 0 : 1) << 3)
                | ((cb_lock_bit4.Checked && cb_lock_bit4.Enabled == true ? 0 : 1) << 4)
                | ((cb_lock_bit5.Checked && cb_lock_bit5.Enabled == true ? 0 : 1) << 5)
                | ((cb_lock_bit6.Checked && cb_lock_bit6.Enabled == true ? 0 : 1) << 6)
                | ((cb_lock_bit7.Checked && cb_lock_bit7.Enabled == true ? 0 : 1) << 7);
            textBoxFinalFuseLock.Text = "0x" + num.ToString("X2");
            num = 0;
            num =  (cb_fuse_ext0.Checked && cb_fuse_ext0.Enabled == true ? 0 : 1)
                | ((cb_fuse_ext1.Checked && cb_fuse_ext1.Enabled == true ? 0 : 1) << 1)
                | ((cb_fuse_ext2.Checked && cb_fuse_ext2.Enabled == true ? 0 : 1) << 2)
                | ((cb_fuse_ext3.Checked && cb_fuse_ext3.Enabled == true ? 0 : 1) << 3)
                | ((cb_fuse_ext4.Checked && cb_fuse_ext4.Enabled == true ? 0 : 1) << 4)
                | ((cb_fuse_ext5.Checked && cb_fuse_ext5.Enabled == true ? 0 : 1) << 5)
                | ((cb_fuse_ext6.Checked && cb_fuse_ext6.Enabled == true ? 0 : 1) << 6)
                | ((cb_fuse_ext7.Checked && cb_fuse_ext7.Enabled == true ? 0 : 1) << 7);
            textBoxFinalFuseExt.Text = "0x" + num.ToString("X2");
            num = 0;
            num =  (cb_fuse_low0.Checked && cb_fuse_low0.Enabled == true ? 0 : 1)
                | ((cb_fuse_low1.Checked && cb_fuse_low1.Enabled == true ? 0 : 1) << 1)
                | ((cb_fuse_low2.Checked && cb_fuse_low2.Enabled == true ? 0 : 1) << 2)
                | ((cb_fuse_low3.Checked && cb_fuse_low3.Enabled == true ? 0 : 1) << 3)
                | ((cb_fuse_low4.Checked && cb_fuse_low4.Enabled == true ? 0 : 1) << 4)
                | ((cb_fuse_low5.Checked && cb_fuse_low5.Enabled == true ? 0 : 1) << 5)
                | ((cb_fuse_low6.Checked && cb_fuse_low6.Enabled == true ? 0 : 1) << 6)
                | ((cb_fuse_low7.Checked && cb_fuse_low7.Enabled == true ? 0 : 1) << 7);
            textBoxFinalFuseLow.Text = "0x" + num.ToString("X2");
            num = 0;
            num =  (cb_fuse_high0.Checked && cb_fuse_high0.Enabled == true ? 0 : 1)
                | ((cb_fuse_high1.Checked && cb_fuse_high1.Enabled == true ? 0 : 1) << 1)
                | ((cb_fuse_high2.Checked && cb_fuse_high2.Enabled == true ? 0 : 1) << 2)
                | ((cb_fuse_high3.Checked && cb_fuse_high3.Enabled == true ? 0 : 1) << 3)
                | ((cb_fuse_high4.Checked && cb_fuse_high4.Enabled == true ? 0 : 1) << 4)
                | ((cb_fuse_high5.Checked && cb_fuse_high5.Enabled == true ? 0 : 1) << 5)
                | ((cb_fuse_high6.Checked && cb_fuse_high6.Enabled == true ? 0 : 1) << 6)
                | ((cb_fuse_high7.Checked && cb_fuse_high7.Enabled == true ? 0 : 1) << 7);
            textBoxFinalFuseHigh.Text = "0x" + num.ToString("X2");
        }

        private void windowForm1_Load(object sender, EventArgs e)
        {

        }

        private void comboBoxFolderSelector_SelectedIndexChanged(object sender, EventArgs e)
        {            
            if (DoEventMaxFolders==false)
            {
                return;
            }
            progressBarCrypt.Value = 0;
            labelStatus.ForeColor = Color.Black;
            labelStatus.Text = "";
            ResetForm();
            ComboBox cb=(ComboBox)sender;
            LastWorkingFolder = cb.SelectedIndex;
            labelProgNum.Text = (LastWorkingFolder + 1).ToString() + ".";
            folderpath = Path.Combine(rootfolderpath, (LastWorkingFolder + 1).ToString());
            datfilepath = "";
            //textBoxPath.Text = folderpath;

            if (Directory.Exists(folderpath))
            {

                //if (File.Exists(folderpath + "\\" + (LastWorkingFolder + 1).ToString() + ".dat") || File.Exists(folderpath + "\\" + "flash.dat"))
	            {
		            button_Decrypt_Click(null,null);
                    buttonsEnable();
	            }
                
            }
            else
            {
                try
                {
                    Directory.CreateDirectory(rootfolderpath + "\\" + (LastWorkingFolder + 1).ToString());
                    buttonsEnable();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    labelStatus.ForeColor = Color.Red;
                    labelStatus.Text = ex.Message;
                    ResetForm();
                }
                
            }
        }
        private void buttonsEnable()
        {
            cancelAsyncButton.Enabled = false;
            buttonCreateOpenIni.Enabled = true;
            buttonOpenConfig.Enabled = true;
            buttonSelectFolder.Enabled = true;
            buttonWriteToIni.Enabled = true;
            buttonWriteConfig.Enabled = true;
            buttonSelectHEX.Enabled = true;
            buttonSelectBoot.Enabled = true;
            buttonSelectEEPROM.Enabled = true;
            buttonRefresh.Enabled = true;
            buttonMakeList.Enabled = true;
            buttonReadChipConfig.Enabled = true;
            buttonWriteChipConfig.Enabled = true;
        }
        private void textBoxMaxFolders_TextChanged(object sender, EventArgs e)
        {
            TextBox tb = (TextBox)sender;
            uint num;
            ini = new IniFile(rootfolderpath + "\\0\\config.ini");
            ini.path = rootfolderpath + "\\0\\config.ini";
            if (uint.TryParse(tb.Text, out num))
            {
                if (num<100)
                {                    
                    ini.IniWriteValue("settings", "maxfolders", num.ToString());
                    //return;
                }
            }
            DoEventMaxFolders = false;
            ReadRootConfig();
        }

        private void textBoxUpStep_MouseLeave(object sender, EventArgs e)
        {
            TextBox tb = (TextBox)sender;
            uint num;
            ini = new IniFile(rootfolderpath + "\\0\\config.ini");
            ini.path = rootfolderpath + "\\0\\config.ini";
            if (uint.TryParse(tb.Text, out num))
            {
                if (num < 30)
                {
                    ini.IniWriteValue("settings", "up", num.ToString());
                    //return;
                }
            }
            //DoEventMaxFolders = false;
            //ReadRootConfig();
        }

        private void textBoxDownStep_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBoxDownStep_MouseLeave(object sender, EventArgs e)
        {
            TextBox tb = (TextBox)sender;
            uint num;
            ini = new IniFile(rootfolderpath + "\\0\\config.ini");
            ini.path = rootfolderpath + "\\0\\config.ini";
            if (uint.TryParse(tb.Text, out num))
            {
                if (num < 30)
                {
                    ini.IniWriteValue("settings", "down", num.ToString());
                    //return;
                }
            }
            //DoEventMaxFolders = false;
            //ReadRootConfig();
        }
        private void MakeList()
        {
            string text =   "# of folder" + (("# of folder".Length < 8) ? "\t\t" : "\t") +
                            "Line 1" + (("Line 1".Length < 8) ? "\t\t" : "\t") +
                            "Line 2" + (("Line 2".Length < 8) ? "\t\t" : "\t") +
                            "Line 3" + (("Line 3".Length < 8) ? "\t\t" : "\t") +
                            "\r\n";
            for (int i = 0; i < comboBoxFolderSelector.Items.Count; i++)
            {
                if (File.Exists(rootfolderpath + "\\" + (i + 1).ToString() + "\\flash.dat"))// + (i + 1).ToString() + ".dat"))
                {
                    configfilepath = rootfolderpath + "\\" + (i + 1).ToString() + "\\config.ini";
                    ReadConfigIni(true);
                    text += (i + 1).ToString() + (((i + 1).ToString().Length < 8) ? "\t\t" : "\t") +
                        textBoxProgname.Text + ((textBoxProgname.Text.Length < 8) ? "\t\t" : "\t") +
                        textBoxFileInfo.Text + ((textBoxFileInfo.Text.Length < 8) ? "\t\t" : "\t") + 
                        textBoxFileInfo2.Text + "\r\n";
                }
            }
            System.IO.File.WriteAllText(rootfolderpath + "\\0\\list.txt", text);
            configfilepath = rootfolderpath + "\\" + (LastWorkingFolder + 1).ToString() + "\\config.ini";
            ReadConfigIni(true);
        }

        private void buttonMakeList_Click(object sender, EventArgs e)
        {
            MakeList();
            labelStatus.Text = "Writing list, done.";
        }

        private void buttonOpenList_Click(object sender, EventArgs e)
        {
            try
            {
                Process.Start(rootfolderpath + "\\0\\list.txt");
            }
            catch (Exception)
            {
                MessageBox.Show("File " + rootfolderpath + "\\0\\list.txt is not exist.");
                //throw;
            } 
        }

        private void buttonCopyFuses_Click(object sender, EventArgs e)
        {
            uint numFinal,numMask,numDefault;
            if (uint.TryParse(textBoxFinalFuseExt.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out numFinal))
            {
                if (uint.TryParse(textBoxMaskFuseExt.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out numMask))
                {
                    numDefault = numFinal & numMask;
                    textBoxProgFuseExt.Text = "0x" + numDefault.ToString("X2");

                }
            }
            if (uint.TryParse(textBoxFinalFuseHigh.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out numFinal))
            {
                if (uint.TryParse(textBoxMaskFuseHigh.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out numMask))
                {
                    numDefault = numFinal & numMask;
                    textBoxProgFuseHigh.Text = "0x" + numDefault.ToString("X2");

                }
            }
            if (uint.TryParse(textBoxFinalFuseLow.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out numFinal))
            {
                if (uint.TryParse(textBoxMaskFuseLow.Text.Replace("0x", ""), NumberStyles.HexNumber, null, out numMask))
                {
                    numDefault = numFinal & numMask;
                    textBoxProgFuseLow.Text = "0x" + numDefault.ToString("X2");

                }
            }
        }
        void ReadChipConfigIni()
        {

            var filePath = string.Empty;

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                
                openFileDialog.Filter = "ini files (*.ini)|*.ini";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    //Get the path of specified file
                    filePath = openFileDialog.FileName;
                }
                else
                    return;
            }
            try
            {
                ini = new IniFile(filePath);
                
                if ((comboBoxChipSelect.Text = TryParseStringFromSettings("chipname", "")) == "")
                {
                    MessageBox.Show("Chipname is missing in config.ini file");
                    return;
                }
                textBoxSignature.Text = (textBoxSignature.Text = TryParseValueFromSettings("signature", 0).ToString("X6")) == "000000" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].Signature :
                    textBoxSignature.Text;
                if ((textBoxProgFuseHigh.Text = TryParseValueFromSettings("progfuses", 0).ToString("X8")) == "00000000")
                {
                    textBoxProgFuseHigh.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].HighFuseDef;
                    textBoxProgFuseLock.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LockBits;
                    textBoxProgFuseLow.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LowFuseDef;
                    textBoxProgFuseExt.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].ExtFuseDef;
                }
                else
                {
                    textBoxProgFuseLock.Text = "0x" + textBoxProgFuseHigh.Text.Substring(0, 2);
                    textBoxProgFuseLow.Text = "0x" + textBoxProgFuseHigh.Text.Substring(2, 2);
                    textBoxProgFuseExt.Text = "0x" + textBoxProgFuseHigh.Text.Substring(6, 2);
                    textBoxProgFuseHigh.Text = "0x" + textBoxProgFuseHigh.Text.Substring(4, 2);
                }
                if ((textBoxFinalFuseHigh.Text = TryParseValueFromSettings("normfuses", 0).ToString("X8")) == "00000000")
                {
                    textBoxFinalFuseHigh.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].HighFuseDef;
                    textBoxFinalFuseLock.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LockBits;
                    textBoxFinalFuseLow.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LowFuseDef;
                    textBoxFinalFuseExt.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].ExtFuseDef;
                }
                else
                {
                    textBoxFinalFuseLock.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(0, 2);
                    textBoxFinalFuseLow.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(2, 2);
                    textBoxFinalFuseExt.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(6, 2);
                    textBoxFinalFuseHigh.Text = "0x" + textBoxFinalFuseHigh.Text.Substring(4, 2);
                }
                textBoxProgFuseHigh_Leave(textBoxFinalFuseLock, null);
                textBoxProgFuseHigh_Leave(textBoxFinalFuseLow, null);
                textBoxProgFuseHigh_Leave(textBoxFinalFuseExt, null);
                textBoxProgFuseHigh_Leave(textBoxFinalFuseHigh, null);
                if ((textBoxMaskFuseHigh.Text = TryParseValueFromSettings("maskfuses", 0).ToString("X8")) == "00000000")
                {
                    textBoxMaskFuseHigh.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].HighFuseDef;
                    textBoxMaskFuseLock.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LockBits;
                    textBoxMaskFuseLow.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].LowFuseDef;
                    textBoxMaskFuseExt.Text =
                        partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].ExtFuseDef;
                }
                else
                {
                    textBoxMaskFuseLock.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(0, 2);
                    textBoxMaskFuseLow.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(2, 2);
                    textBoxMaskFuseExt.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(6, 2);
                    textBoxMaskFuseHigh.Text = "0x" + textBoxMaskFuseHigh.Text.Substring(4, 2);
                }
                textBoxChipsizeBytes.Text = (textBoxChipsizeBytes.Text = TryParseValueFromSettings("chipsize", 0).ToString()) == "0" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].Prog_Flash :
                    textBoxChipsizeBytes.Text;
                textBoxPagesizeWords.Text = (textBoxPagesizeWords.Text = TryParseValueFromSettings("pagesize", 0).ToString()) == "0" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].FlashPageSize :
                    textBoxPagesizeWords.Text;
                textBoxEepromSizeBytes.Text = (textBoxEepromSizeBytes.Text = TryParseValueFromSettings("eepromsize", 0).ToString()) == "0" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].Prog_Eeprom :
                    textBoxEepromSizeBytes.Text;
                textBoxEepromPageSizeBytes.Text = (textBoxEepromPageSizeBytes.Text = TryParseValueFromSettings("eeprompagesize", 0).ToString()) == "0" ?
                    partlist[partlist.IndexOf(partlist.Single(a => a.Part_Name == comboBoxChipSelect.Text))].EepromPageSize :
                    textBoxEepromPageSizeBytes.Text;
                comboBoxIspClock.SelectedIndex = (int)TryParseValueFromSettings("spidiv", 0);
                labelStatus.Text = "Reading ini file done";
                labelStatus.ForeColor = Color.Green;                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
                labelStatus.Text = "Reading ini file failed!";
                labelStatus.ForeColor = Color.Red;
                //throw;
            }

        }
        bool WriteChipConfigIni()
        {
            try
            {
                //uint num;textBoxFileInfo.Text==""||
                if ( comboBoxChipSelect.Text == "" ||
                    textBoxProgFuseLock.Text == "" || textBoxProgFuseLow.Text == "" || textBoxProgFuseHigh.Text == "" || textBoxProgFuseExt.Text == "" ||
                    textBoxFinalFuseLock.Text == "" || textBoxFinalFuseLow.Text == "" || textBoxFinalFuseHigh.Text == "" || textBoxFinalFuseExt.Text == "" ||
                    textBoxMaskFuseLock.Text == "" || textBoxMaskFuseLow.Text == "" || textBoxMaskFuseHigh.Text == "" || textBoxMaskFuseExt.Text == "" ||
                    textBoxSignature.Text.Length != 6)
                {
                    MessageBox.Show("Not all fields filled correctly");
                    return false;
                }

                SaveFileDialog saveFileDialog1 = new SaveFileDialog();
                string saveinifilepath="";
               
                saveFileDialog1.Filter = "ini files (*.ini)|*.ini";
                saveFileDialog1.FilterIndex = 1;
                saveFileDialog1.RestoreDirectory = true;

                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    saveinifilepath = saveFileDialog1.FileName;
                }
                else
                    return false;

                ini = new IniFile(saveinifilepath);
                if (File.Exists(saveinifilepath) == true)
                {
                    File.Delete(saveinifilepath);
                }
                using (File.CreateText(saveinifilepath))
                {

                }

                if (!TryWriteValueToIni("signature", labelSignature, textBoxSignature.Text, true, 6, 0, 0))
                    return false;
                if (!TryWriteValueToIni("progfuses", labelFusesDefaults, (textBoxProgFuseLock.Text + textBoxProgFuseLow.Text + textBoxProgFuseHigh.Text + textBoxProgFuseExt.Text).Replace("0x", ""), true, 8, 0, 0))
                    return false;
                if (!TryWriteValueToIni("normfuses", labelFusesFinal, (textBoxFinalFuseLock.Text + textBoxFinalFuseLow.Text + textBoxFinalFuseHigh.Text + textBoxFinalFuseExt.Text).Replace("0x", ""), true, 8, 0, 0))
                    return false;
                if (!TryWriteValueToIni("maskfuses", labelFusesMask, (textBoxMaskFuseLock.Text + textBoxMaskFuseLow.Text + textBoxMaskFuseHigh.Text + textBoxMaskFuseExt.Text).Replace("0x", ""), true, 8, 0, 0))
                    return false;
                if (!TryWriteValueToIni("chipsize", labelFlashSize, textBoxChipsizeBytes.Text, false, 0, 1024, 262144))
                    return false;
                if (!TryWriteValueToIni("pagesize", labelFlashPageSize, textBoxPagesizeWords.Text, false, 0, 0, 256))
                    return false;
                if (!TryWriteValueToIni("eepromsize", labelEepromSize, textBoxEepromSizeBytes.Text, false, 0, 0, 8192))
                    return false;
                if (!TryWriteValueToIni("eeprompagesize", labelEepromPageSize, textBoxEepromPageSizeBytes.Text, false, 0, 0, 256))
                    return false;
                
                if (!TryWriteValueToIni("spidiv", labelSpiDiv, comboBoxIspClock.SelectedIndex.ToString(), false, 0, 0, 7))
                    return false;
                
                if (comboBoxChipSelect.Text.Length < 5)
                {
                    MessageBox.Show("Incorrect chip selector name");
                    return false;
                }

                ini.IniWriteValue("settings", "chipname", comboBoxChipSelect.Text);
                labelStatus.Text = "Writing ini file done.";
                labelStatus.ForeColor = Color.Green;
                

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + "\r\n" + ex.StackTrace);
                labelStatus.Text = "Writing ini file failed!";
                labelStatus.ForeColor = Color.Red;
                return false;
                //throw;
            }
            return true;
            //MessageBox.Show("Writing to config.ini file done.");
        }

        private void buttonWriteChipConfig_Click(object sender, EventArgs e)
        {
            WriteChipConfigIni();
        }

        private void buttonReadChipConfig_Click(object sender, EventArgs e)
        {
            ReadChipConfigIni();
        }
    }
}
