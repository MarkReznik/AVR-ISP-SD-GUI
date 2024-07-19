namespace MainProgram
{
    partial class windowForm1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox_Input = new System.Windows.Forms.TextBox();
            this.textBox_Key = new System.Windows.Forms.TextBox();
            this.textBox_Output = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_InputHex = new System.Windows.Forms.TextBox();
            this.textBox_OutputHex = new System.Windows.Forms.TextBox();
            this.textBoxRollingCounter = new System.Windows.Forms.TextBox();
            this.labelRollingCounter = new System.Windows.Forms.Label();
            this.textBoxProgFuseHigh = new System.Windows.Forms.TextBox();
            this.labelFusesDefaults = new System.Windows.Forms.Label();
            this.textBoxMaskFuseHigh = new System.Windows.Forms.TextBox();
            this.labelFusesMask = new System.Windows.Forms.Label();
            this.textBoxChipsizeBytes = new System.Windows.Forms.TextBox();
            this.textBoxPagesizeWords = new System.Windows.Forms.TextBox();
            this.textBoxLicense = new System.Windows.Forms.TextBox();
            this.labelFlashSize = new System.Windows.Forms.Label();
            this.labelFlashPageSize = new System.Windows.Forms.Label();
            this.labelLicenseTotal = new System.Windows.Forms.Label();
            this.labelChipSelector = new System.Windows.Forms.Label();
            this.textBoxFilename = new System.Windows.Forms.TextBox();
            this.textBoxPath = new System.Windows.Forms.TextBox();
            this.buttonOpenConfig = new System.Windows.Forms.Button();
            this.buttonCreateOpenIni = new System.Windows.Forms.Button();
            this.buttonWriteToIni = new System.Windows.Forms.Button();
            this.textBoxSignature = new System.Windows.Forms.TextBox();
            this.labelSignature = new System.Windows.Forms.Label();
            this.textBoxProgname = new System.Windows.Forms.TextBox();
            this.labelProgName = new System.Windows.Forms.Label();
            this.buttonWriteConfig = new System.Windows.Forms.Button();
            this.buttonSelectFolder = new System.Windows.Forms.Button();
            this.progressBarCrypt = new System.Windows.Forms.ProgressBar();
            this.cancelAsyncButton = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.labelStatusConst = new System.Windows.Forms.Label();
            this.labelStatus = new System.Windows.Forms.Label();
            this.buttonSelectHEX = new System.Windows.Forms.Button();
            this.textBoxFileInfo = new System.Windows.Forms.TextBox();
            this.labelFileInfo = new System.Windows.Forms.Label();
            this.comboBoxChipSelect = new System.Windows.Forms.ComboBox();
            this.textBoxProgFuseLow = new System.Windows.Forms.TextBox();
            this.textBoxProgFuseExt = new System.Windows.Forms.TextBox();
            this.textBoxProgFuseLock = new System.Windows.Forms.TextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.textBoxFinalFuseHigh = new System.Windows.Forms.TextBox();
            this.textBoxFinalFuseLow = new System.Windows.Forms.TextBox();
            this.textBoxFinalFuseExt = new System.Windows.Forms.TextBox();
            this.textBoxFinalFuseLock = new System.Windows.Forms.TextBox();
            this.labelFusesFinal = new System.Windows.Forms.Label();
            this.textBoxEepromSizeBytes = new System.Windows.Forms.TextBox();
            this.textBoxEepromPageSizeBytes = new System.Windows.Forms.TextBox();
            this.labelEepromSize = new System.Windows.Forms.Label();
            this.labelEepromPageSize = new System.Windows.Forms.Label();
            this.textBoxMaskFuseLow = new System.Windows.Forms.TextBox();
            this.textBoxMaskFuseExt = new System.Windows.Forms.TextBox();
            this.textBoxMaskFuseLock = new System.Windows.Forms.TextBox();
            this.textBoxSPIDIV = new System.Windows.Forms.TextBox();
            this.labelSpiDiv = new System.Windows.Forms.Label();
            this.labelFuses = new System.Windows.Forms.Label();
            this.checkBoxResetLicenses = new System.Windows.Forms.CheckBox();
            this.checkBoxUseSerialCounter = new System.Windows.Forms.CheckBox();
            this.textBoxEepromStartAddress = new System.Windows.Forms.TextBox();
            this.labelEepromStartAddress = new System.Windows.Forms.Label();
            this.cb_lock_bit0 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit1 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit2 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit3 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit4 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit5 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit6 = new System.Windows.Forms.CheckBox();
            this.cb_lock_bit7 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low0 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low1 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low2 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low3 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low4 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low5 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low6 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_low7 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high0 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high1 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high2 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high3 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high4 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high5 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high6 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_high7 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext0 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext1 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext2 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext3 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext4 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext5 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext6 = new System.Windows.Forms.CheckBox();
            this.cb_fuse_ext7 = new System.Windows.Forms.CheckBox();
            this.buttonRefresh = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.label31 = new System.Windows.Forms.Label();
            this.label32 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.label34 = new System.Windows.Forms.Label();
            this.label35 = new System.Windows.Forms.Label();
            this.label36 = new System.Windows.Forms.Label();
            this.label37 = new System.Windows.Forms.Label();
            this.label38 = new System.Windows.Forms.Label();
            this.label39 = new System.Windows.Forms.Label();
            this.label40 = new System.Windows.Forms.Label();
            this.label41 = new System.Windows.Forms.Label();
            this.label42 = new System.Windows.Forms.Label();
            this.label43 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.label45 = new System.Windows.Forms.Label();
            this.label46 = new System.Windows.Forms.Label();
            this.textBoxFlashHexFilePath = new System.Windows.Forms.TextBox();
            this.label48 = new System.Windows.Forms.Label();
            this.buttonSelectEEPROM = new System.Windows.Forms.Button();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.textBoxEepromHexFilePath = new System.Windows.Forms.TextBox();
            this.textBoxUpStep = new System.Windows.Forms.TextBox();
            this.textBoxMaxFolders = new System.Windows.Forms.TextBox();
            this.label51 = new System.Windows.Forms.Label();
            this.label52 = new System.Windows.Forms.Label();
            this.textBoxDownStep = new System.Windows.Forms.TextBox();
            this.label50 = new System.Windows.Forms.Label();
            this.comboBoxFolderSelector = new System.Windows.Forms.ComboBox();
            this.label53 = new System.Windows.Forms.Label();
            this.buttonMakeList = new System.Windows.Forms.Button();
            this.labelProgNum = new System.Windows.Forms.Label();
            this.textBoxFileInfo2 = new System.Windows.Forms.TextBox();
            this.label54 = new System.Windows.Forms.Label();
            this.textBoxEepromStartAddressCalibBytes = new System.Windows.Forms.TextBox();
            this.checkBoxUseCalibBytes = new System.Windows.Forms.CheckBox();
            this.labelEepromStartAddressCalibBytes = new System.Windows.Forms.Label();
            this.textBoxBootHexFile = new System.Windows.Forms.TextBox();
            this.buttonSelectBoot = new System.Windows.Forms.Button();
            this.textBoxBootHexFilePath = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label47 = new System.Windows.Forms.Label();
            this.textBoxEepromHexFile = new System.Windows.Forms.TextBox();
            this.label49 = new System.Windows.Forms.Label();
            this.textBoxEncryptionCounter = new System.Windows.Forms.TextBox();
            this.label55 = new System.Windows.Forms.Label();
            this.textBoxFileUniqueVale = new System.Windows.Forms.TextBox();
            this.label56 = new System.Windows.Forms.Label();
            this.checkBoxStopUsingUniqueFile = new System.Windows.Forms.CheckBox();
            this.label57 = new System.Windows.Forms.Label();
            this.label58 = new System.Windows.Forms.Label();
            this.comboBoxIspClock = new System.Windows.Forms.ComboBox();
            this.cbFwUpdate = new System.Windows.Forms.CheckBox();
            this.buttonOpenList = new System.Windows.Forms.Button();
            this.buttonCopyFuses = new System.Windows.Forms.Button();
            this.buttonWriteChipConfig = new System.Windows.Forms.Button();
            this.buttonReadChipConfig = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox_Input
            // 
            this.textBox_Input.Location = new System.Drawing.Point(843, 241);
            this.textBox_Input.Name = "textBox_Input";
            this.textBox_Input.Size = new System.Drawing.Size(100, 20);
            this.textBox_Input.TabIndex = 0;
            // 
            // textBox_Key
            // 
            this.textBox_Key.Location = new System.Drawing.Point(843, 205);
            this.textBox_Key.Name = "textBox_Key";
            this.textBox_Key.Size = new System.Drawing.Size(54, 20);
            this.textBox_Key.TabIndex = 1;
            this.textBox_Key.Visible = false;
            // 
            // textBox_Output
            // 
            this.textBox_Output.Location = new System.Drawing.Point(843, 372);
            this.textBox_Output.Name = "textBox_Output";
            this.textBox_Output.Size = new System.Drawing.Size(100, 20);
            this.textBox_Output.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(865, 228);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Hex Input";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(901, 208);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(48, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "PassKey";
            this.label2.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(863, 359);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Hex Output";
            // 
            // textBox_InputHex
            // 
            this.textBox_InputHex.Location = new System.Drawing.Point(843, 263);
            this.textBox_InputHex.Multiline = true;
            this.textBox_InputHex.Name = "textBox_InputHex";
            this.textBox_InputHex.Size = new System.Drawing.Size(100, 86);
            this.textBox_InputHex.TabIndex = 0;
            // 
            // textBox_OutputHex
            // 
            this.textBox_OutputHex.Location = new System.Drawing.Point(843, 394);
            this.textBox_OutputHex.Multiline = true;
            this.textBox_OutputHex.Name = "textBox_OutputHex";
            this.textBox_OutputHex.Size = new System.Drawing.Size(100, 86);
            this.textBox_OutputHex.TabIndex = 2;
            // 
            // textBoxRollingCounter
            // 
            this.textBoxRollingCounter.Enabled = false;
            this.textBoxRollingCounter.Location = new System.Drawing.Point(579, 137);
            this.textBoxRollingCounter.Name = "textBoxRollingCounter";
            this.textBoxRollingCounter.Size = new System.Drawing.Size(54, 20);
            this.textBoxRollingCounter.TabIndex = 1;
            this.textBoxRollingCounter.Visible = false;
            // 
            // labelRollingCounter
            // 
            this.labelRollingCounter.AutoSize = true;
            this.labelRollingCounter.Enabled = false;
            this.labelRollingCounter.Location = new System.Drawing.Point(639, 145);
            this.labelRollingCounter.Name = "labelRollingCounter";
            this.labelRollingCounter.Size = new System.Drawing.Size(102, 13);
            this.labelRollingCounter.TabIndex = 4;
            this.labelRollingCounter.Text = "Encrypt value(1..64)";
            this.labelRollingCounter.Visible = false;
            // 
            // textBoxProgFuseHigh
            // 
            this.textBoxProgFuseHigh.Location = new System.Drawing.Point(96, 314);
            this.textBoxProgFuseHigh.Name = "textBoxProgFuseHigh";
            this.textBoxProgFuseHigh.Size = new System.Drawing.Size(40, 20);
            this.textBoxProgFuseHigh.TabIndex = 1;
            this.textBoxProgFuseHigh.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // labelFusesDefaults
            // 
            this.labelFusesDefaults.AutoSize = true;
            this.labelFusesDefaults.Location = new System.Drawing.Point(75, 242);
            this.labelFusesDefaults.Name = "labelFusesDefaults";
            this.labelFusesDefaults.Size = new System.Drawing.Size(85, 13);
            this.labelFusesDefaults.TabIndex = 4;
            this.labelFusesDefaults.Text = "Default/Program";
            // 
            // textBoxMaskFuseHigh
            // 
            this.textBoxMaskFuseHigh.Location = new System.Drawing.Point(163, 314);
            this.textBoxMaskFuseHigh.Name = "textBoxMaskFuseHigh";
            this.textBoxMaskFuseHigh.Size = new System.Drawing.Size(39, 20);
            this.textBoxMaskFuseHigh.TabIndex = 1;
            this.textBoxMaskFuseHigh.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // labelFusesMask
            // 
            this.labelFusesMask.AutoSize = true;
            this.labelFusesMask.Location = new System.Drawing.Point(168, 242);
            this.labelFusesMask.Name = "labelFusesMask";
            this.labelFusesMask.Size = new System.Drawing.Size(33, 13);
            this.labelFusesMask.TabIndex = 4;
            this.labelFusesMask.Text = "Mask";
            // 
            // textBoxChipsizeBytes
            // 
            this.textBoxChipsizeBytes.Location = new System.Drawing.Point(12, 391);
            this.textBoxChipsizeBytes.Name = "textBoxChipsizeBytes";
            this.textBoxChipsizeBytes.Size = new System.Drawing.Size(120, 20);
            this.textBoxChipsizeBytes.TabIndex = 1;
            // 
            // textBoxPagesizeWords
            // 
            this.textBoxPagesizeWords.Location = new System.Drawing.Point(12, 427);
            this.textBoxPagesizeWords.Name = "textBoxPagesizeWords";
            this.textBoxPagesizeWords.Size = new System.Drawing.Size(120, 20);
            this.textBoxPagesizeWords.TabIndex = 1;
            // 
            // textBoxLicense
            // 
            this.textBoxLicense.Location = new System.Drawing.Point(580, 379);
            this.textBoxLicense.Name = "textBoxLicense";
            this.textBoxLicense.Size = new System.Drawing.Size(54, 20);
            this.textBoxLicense.TabIndex = 1;
            this.textBoxLicense.TextChanged += new System.EventHandler(this.textBoxLicense_MouseLeave);
            // 
            // labelFlashSize
            // 
            this.labelFlashSize.AutoSize = true;
            this.labelFlashSize.Location = new System.Drawing.Point(12, 375);
            this.labelFlashSize.Name = "labelFlashSize";
            this.labelFlashSize.Size = new System.Drawing.Size(88, 13);
            this.labelFlashSize.TabIndex = 4;
            this.labelFlashSize.Text = "Flash size [Bytes]";
            // 
            // labelFlashPageSize
            // 
            this.labelFlashPageSize.AutoSize = true;
            this.labelFlashPageSize.Location = new System.Drawing.Point(12, 412);
            this.labelFlashPageSize.Name = "labelFlashPageSize";
            this.labelFlashPageSize.Size = new System.Drawing.Size(120, 13);
            this.labelFlashPageSize.TabIndex = 4;
            this.labelFlashPageSize.Text = "Flash page size [Words]";
            // 
            // labelLicenseTotal
            // 
            this.labelLicenseTotal.AutoSize = true;
            this.labelLicenseTotal.Location = new System.Drawing.Point(639, 390);
            this.labelLicenseTotal.Name = "labelLicenseTotal";
            this.labelLicenseTotal.Size = new System.Drawing.Size(127, 13);
            this.labelLicenseTotal.TabIndex = 4;
            this.labelLicenseTotal.Text = "Total Licenses(1...65535)";
            // 
            // labelChipSelector
            // 
            this.labelChipSelector.AutoSize = true;
            this.labelChipSelector.Location = new System.Drawing.Point(107, 176);
            this.labelChipSelector.Name = "labelChipSelector";
            this.labelChipSelector.Size = new System.Drawing.Size(70, 13);
            this.labelChipSelector.TabIndex = 4;
            this.labelChipSelector.Text = "Chip Selector";
            // 
            // textBoxFilename
            // 
            this.textBoxFilename.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxFilename.Location = new System.Drawing.Point(12, 10);
            this.textBoxFilename.Name = "textBoxFilename";
            this.textBoxFilename.ReadOnly = true;
            this.textBoxFilename.Size = new System.Drawing.Size(142, 20);
            this.textBoxFilename.TabIndex = 1;
            // 
            // textBoxPath
            // 
            this.textBoxPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPath.Location = new System.Drawing.Point(161, 551);
            this.textBoxPath.Multiline = true;
            this.textBoxPath.Name = "textBoxPath";
            this.textBoxPath.ReadOnly = true;
            this.textBoxPath.Size = new System.Drawing.Size(651, 23);
            this.textBoxPath.TabIndex = 1;
            // 
            // buttonOpenConfig
            // 
            this.buttonOpenConfig.Location = new System.Drawing.Point(306, 52);
            this.buttonOpenConfig.Name = "buttonOpenConfig";
            this.buttonOpenConfig.Size = new System.Drawing.Size(102, 37);
            this.buttonOpenConfig.TabIndex = 6;
            this.buttonOpenConfig.Text = "Encrypted file Read settings";
            this.buttonOpenConfig.UseVisualStyleBackColor = true;
            this.buttonOpenConfig.Click += new System.EventHandler(this.button_Decrypt_Click);
            // 
            // buttonCreateOpenIni
            // 
            this.buttonCreateOpenIni.Location = new System.Drawing.Point(881, 8);
            this.buttonCreateOpenIni.Name = "buttonCreateOpenIni";
            this.buttonCreateOpenIni.Size = new System.Drawing.Size(88, 37);
            this.buttonCreateOpenIni.TabIndex = 6;
            this.buttonCreateOpenIni.Text = "Normal file Read settings";
            this.buttonCreateOpenIni.UseVisualStyleBackColor = true;
            this.buttonCreateOpenIni.Click += new System.EventHandler(this.buttonCreateOpenIni_Click);
            // 
            // buttonWriteToIni
            // 
            this.buttonWriteToIni.Location = new System.Drawing.Point(881, 51);
            this.buttonWriteToIni.Name = "buttonWriteToIni";
            this.buttonWriteToIni.Size = new System.Drawing.Size(88, 38);
            this.buttonWriteToIni.TabIndex = 6;
            this.buttonWriteToIni.Text = "Normail file Write settings";
            this.buttonWriteToIni.UseVisualStyleBackColor = true;
            this.buttonWriteToIni.Click += new System.EventHandler(this.buttonWriteToIni_Click);
            // 
            // textBoxSignature
            // 
            this.textBoxSignature.Location = new System.Drawing.Point(213, 193);
            this.textBoxSignature.Name = "textBoxSignature";
            this.textBoxSignature.Size = new System.Drawing.Size(57, 20);
            this.textBoxSignature.TabIndex = 1;
            // 
            // labelSignature
            // 
            this.labelSignature.AutoSize = true;
            this.labelSignature.Location = new System.Drawing.Point(216, 177);
            this.labelSignature.Name = "labelSignature";
            this.labelSignature.Size = new System.Drawing.Size(52, 13);
            this.labelSignature.TabIndex = 4;
            this.labelSignature.Text = "Signature";
            // 
            // textBoxProgname
            // 
            this.textBoxProgname.Location = new System.Drawing.Point(37, 95);
            this.textBoxProgname.MaxLength = 12;
            this.textBoxProgname.Name = "textBoxProgname";
            this.textBoxProgname.Size = new System.Drawing.Size(83, 20);
            this.textBoxProgname.TabIndex = 1;
            // 
            // labelProgName
            // 
            this.labelProgName.AutoSize = true;
            this.labelProgName.Location = new System.Drawing.Point(126, 99);
            this.labelProgName.Name = "labelProgName";
            this.labelProgName.Size = new System.Drawing.Size(144, 13);
            this.labelProgName.TabIndex = 4;
            this.labelProgName.Text = "Program info line 1 (12 chars)";
            // 
            // buttonWriteConfig
            // 
            this.buttonWriteConfig.Location = new System.Drawing.Point(433, 51);
            this.buttonWriteConfig.Name = "buttonWriteConfig";
            this.buttonWriteConfig.Size = new System.Drawing.Size(102, 38);
            this.buttonWriteConfig.TabIndex = 6;
            this.buttonWriteConfig.Text = "Encrypted file Write settings";
            this.buttonWriteConfig.UseVisualStyleBackColor = true;
            this.buttonWriteConfig.Click += new System.EventHandler(this.button_Encript_Click);
            // 
            // buttonSelectFolder
            // 
            this.buttonSelectFolder.Location = new System.Drawing.Point(306, 7);
            this.buttonSelectFolder.Name = "buttonSelectFolder";
            this.buttonSelectFolder.Size = new System.Drawing.Size(229, 36);
            this.buttonSelectFolder.TabIndex = 6;
            this.buttonSelectFolder.Text = "Select Programmer Working Space";
            this.buttonSelectFolder.UseVisualStyleBackColor = true;
            this.buttonSelectFolder.Click += new System.EventHandler(this.buttonSelectFolder_Click);
            // 
            // progressBarCrypt
            // 
            this.progressBarCrypt.Location = new System.Drawing.Point(307, 97);
            this.progressBarCrypt.Name = "progressBarCrypt";
            this.progressBarCrypt.Size = new System.Drawing.Size(228, 27);
            this.progressBarCrypt.TabIndex = 8;
            // 
            // cancelAsyncButton
            // 
            this.cancelAsyncButton.Location = new System.Drawing.Point(548, 97);
            this.cancelAsyncButton.Name = "cancelAsyncButton";
            this.cancelAsyncButton.Size = new System.Drawing.Size(71, 27);
            this.cancelAsyncButton.TabIndex = 6;
            this.cancelAsyncButton.Text = "Cancel";
            this.cancelAsyncButton.UseVisualStyleBackColor = true;
            this.cancelAsyncButton.Click += new System.EventHandler(this.cancelAsyncButton_Click);
            // 
            // labelStatusConst
            // 
            this.labelStatusConst.AutoSize = true;
            this.labelStatusConst.Location = new System.Drawing.Point(305, 132);
            this.labelStatusConst.Name = "labelStatusConst";
            this.labelStatusConst.Size = new System.Drawing.Size(75, 13);
            this.labelStatusConst.TabIndex = 3;
            this.labelStatusConst.Text = "Current status:";
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Location = new System.Drawing.Point(380, 132);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(0, 13);
            this.labelStatus.TabIndex = 3;
            // 
            // buttonSelectHEX
            // 
            this.buttonSelectHEX.Location = new System.Drawing.Point(158, 7);
            this.buttonSelectHEX.Name = "buttonSelectHEX";
            this.buttonSelectHEX.Size = new System.Drawing.Size(118, 23);
            this.buttonSelectHEX.TabIndex = 6;
            this.buttonSelectHEX.Text = "Select Flash *.hex file";
            this.buttonSelectHEX.UseVisualStyleBackColor = true;
            this.buttonSelectHEX.Click += new System.EventHandler(this.buttonSelectHEX_Click);
            // 
            // textBoxFileInfo
            // 
            this.textBoxFileInfo.Location = new System.Drawing.Point(21, 122);
            this.textBoxFileInfo.MaxLength = 15;
            this.textBoxFileInfo.Name = "textBoxFileInfo";
            this.textBoxFileInfo.Size = new System.Drawing.Size(99, 20);
            this.textBoxFileInfo.TabIndex = 1;
            // 
            // labelFileInfo
            // 
            this.labelFileInfo.AutoSize = true;
            this.labelFileInfo.Location = new System.Drawing.Point(127, 125);
            this.labelFileInfo.Name = "labelFileInfo";
            this.labelFileInfo.Size = new System.Drawing.Size(144, 13);
            this.labelFileInfo.TabIndex = 4;
            this.labelFileInfo.Text = "Program info line 2 (15 chars)";
            // 
            // comboBoxChipSelect
            // 
            this.comboBoxChipSelect.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.comboBoxChipSelect.FormattingEnabled = true;
            this.comboBoxChipSelect.Location = new System.Drawing.Point(81, 192);
            this.comboBoxChipSelect.Name = "comboBoxChipSelect";
            this.comboBoxChipSelect.Size = new System.Drawing.Size(123, 21);
            this.comboBoxChipSelect.TabIndex = 10;
            this.comboBoxChipSelect.SelectedIndexChanged += new System.EventHandler(this.comboBoxChipSelect_SelectedIndexChanged);
            // 
            // textBoxProgFuseLow
            // 
            this.textBoxProgFuseLow.Location = new System.Drawing.Point(96, 340);
            this.textBoxProgFuseLow.Name = "textBoxProgFuseLow";
            this.textBoxProgFuseLow.Size = new System.Drawing.Size(40, 20);
            this.textBoxProgFuseLow.TabIndex = 1;
            this.textBoxProgFuseLow.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxProgFuseExt
            // 
            this.textBoxProgFuseExt.Location = new System.Drawing.Point(96, 287);
            this.textBoxProgFuseExt.Name = "textBoxProgFuseExt";
            this.textBoxProgFuseExt.Size = new System.Drawing.Size(40, 20);
            this.textBoxProgFuseExt.TabIndex = 1;
            this.textBoxProgFuseExt.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxProgFuseLock
            // 
            this.textBoxProgFuseLock.Location = new System.Drawing.Point(96, 260);
            this.textBoxProgFuseLock.Name = "textBoxProgFuseLock";
            this.textBoxProgFuseLock.ReadOnly = true;
            this.textBoxProgFuseLock.Size = new System.Drawing.Size(40, 20);
            this.textBoxProgFuseLock.TabIndex = 1;
            this.textBoxProgFuseLock.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(44, 318);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(34, 13);
            this.label20.TabIndex = 4;
            this.label20.Text = "HIGH";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(46, 344);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(32, 13);
            this.label21.TabIndex = 4;
            this.label21.Text = "LOW";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(12, 290);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(66, 13);
            this.label22.TabIndex = 4;
            this.label22.Text = "EXTENDED";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(43, 263);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(35, 13);
            this.label23.TabIndex = 4;
            this.label23.Text = "LOCK";
            // 
            // textBoxFinalFuseHigh
            // 
            this.textBoxFinalFuseHigh.Location = new System.Drawing.Point(227, 314);
            this.textBoxFinalFuseHigh.Name = "textBoxFinalFuseHigh";
            this.textBoxFinalFuseHigh.Size = new System.Drawing.Size(40, 20);
            this.textBoxFinalFuseHigh.TabIndex = 1;
            this.textBoxFinalFuseHigh.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxFinalFuseLow
            // 
            this.textBoxFinalFuseLow.Location = new System.Drawing.Point(227, 339);
            this.textBoxFinalFuseLow.Name = "textBoxFinalFuseLow";
            this.textBoxFinalFuseLow.Size = new System.Drawing.Size(40, 20);
            this.textBoxFinalFuseLow.TabIndex = 1;
            this.textBoxFinalFuseLow.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxFinalFuseExt
            // 
            this.textBoxFinalFuseExt.Location = new System.Drawing.Point(227, 286);
            this.textBoxFinalFuseExt.Name = "textBoxFinalFuseExt";
            this.textBoxFinalFuseExt.Size = new System.Drawing.Size(40, 20);
            this.textBoxFinalFuseExt.TabIndex = 1;
            this.textBoxFinalFuseExt.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxFinalFuseLock
            // 
            this.textBoxFinalFuseLock.Location = new System.Drawing.Point(227, 259);
            this.textBoxFinalFuseLock.Name = "textBoxFinalFuseLock";
            this.textBoxFinalFuseLock.Size = new System.Drawing.Size(40, 20);
            this.textBoxFinalFuseLock.TabIndex = 1;
            this.textBoxFinalFuseLock.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // labelFusesFinal
            // 
            this.labelFusesFinal.AutoSize = true;
            this.labelFusesFinal.Location = new System.Drawing.Point(233, 242);
            this.labelFusesFinal.Name = "labelFusesFinal";
            this.labelFusesFinal.Size = new System.Drawing.Size(29, 13);
            this.labelFusesFinal.TabIndex = 4;
            this.labelFusesFinal.Text = "Final";
            // 
            // textBoxEepromSizeBytes
            // 
            this.textBoxEepromSizeBytes.Location = new System.Drawing.Point(147, 391);
            this.textBoxEepromSizeBytes.Name = "textBoxEepromSizeBytes";
            this.textBoxEepromSizeBytes.Size = new System.Drawing.Size(120, 20);
            this.textBoxEepromSizeBytes.TabIndex = 1;
            // 
            // textBoxEepromPageSizeBytes
            // 
            this.textBoxEepromPageSizeBytes.Location = new System.Drawing.Point(147, 427);
            this.textBoxEepromPageSizeBytes.Name = "textBoxEepromPageSizeBytes";
            this.textBoxEepromPageSizeBytes.Size = new System.Drawing.Size(120, 20);
            this.textBoxEepromPageSizeBytes.TabIndex = 1;
            // 
            // labelEepromSize
            // 
            this.labelEepromSize.AutoSize = true;
            this.labelEepromSize.Location = new System.Drawing.Point(143, 375);
            this.labelEepromSize.Name = "labelEepromSize";
            this.labelEepromSize.Size = new System.Drawing.Size(109, 13);
            this.labelEepromSize.TabIndex = 4;
            this.labelEepromSize.Text = "EEPROM size [Bytes]";
            // 
            // labelEepromPageSize
            // 
            this.labelEepromPageSize.AutoSize = true;
            this.labelEepromPageSize.Location = new System.Drawing.Point(144, 412);
            this.labelEepromPageSize.Name = "labelEepromPageSize";
            this.labelEepromPageSize.Size = new System.Drawing.Size(136, 13);
            this.labelEepromPageSize.TabIndex = 4;
            this.labelEepromPageSize.Text = "EEPROM page size [Bytes]";
            // 
            // textBoxMaskFuseLow
            // 
            this.textBoxMaskFuseLow.Location = new System.Drawing.Point(163, 340);
            this.textBoxMaskFuseLow.Name = "textBoxMaskFuseLow";
            this.textBoxMaskFuseLow.Size = new System.Drawing.Size(39, 20);
            this.textBoxMaskFuseLow.TabIndex = 1;
            this.textBoxMaskFuseLow.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxMaskFuseExt
            // 
            this.textBoxMaskFuseExt.Location = new System.Drawing.Point(163, 287);
            this.textBoxMaskFuseExt.Name = "textBoxMaskFuseExt";
            this.textBoxMaskFuseExt.Size = new System.Drawing.Size(39, 20);
            this.textBoxMaskFuseExt.TabIndex = 1;
            this.textBoxMaskFuseExt.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxMaskFuseLock
            // 
            this.textBoxMaskFuseLock.Location = new System.Drawing.Point(163, 260);
            this.textBoxMaskFuseLock.Name = "textBoxMaskFuseLock";
            this.textBoxMaskFuseLock.Size = new System.Drawing.Size(39, 20);
            this.textBoxMaskFuseLock.TabIndex = 1;
            this.textBoxMaskFuseLock.Leave += new System.EventHandler(this.textBoxProgFuseHigh_Leave);
            // 
            // textBoxSPIDIV
            // 
            this.textBoxSPIDIV.Location = new System.Drawing.Point(750, 138);
            this.textBoxSPIDIV.Name = "textBoxSPIDIV";
            this.textBoxSPIDIV.Size = new System.Drawing.Size(54, 20);
            this.textBoxSPIDIV.TabIndex = 1;
            this.textBoxSPIDIV.Visible = false;
            // 
            // labelSpiDiv
            // 
            this.labelSpiDiv.AutoSize = true;
            this.labelSpiDiv.Location = new System.Drawing.Point(810, 135);
            this.labelSpiDiv.Name = "labelSpiDiv";
            this.labelSpiDiv.Size = new System.Drawing.Size(114, 13);
            this.labelSpiDiv.TabIndex = 4;
            this.labelSpiDiv.Text = "ISP Clock DIV 2^(0..7)";
            this.labelSpiDiv.Visible = false;
            // 
            // labelFuses
            // 
            this.labelFuses.AutoSize = true;
            this.labelFuses.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.labelFuses.Location = new System.Drawing.Point(11, 222);
            this.labelFuses.Name = "labelFuses";
            this.labelFuses.Size = new System.Drawing.Size(262, 13);
            this.labelFuses.TabIndex = 4;
            this.labelFuses.Text = "___________Fuses and Lock Bits_________";
            // 
            // checkBoxResetLicenses
            // 
            this.checkBoxResetLicenses.AutoSize = true;
            this.checkBoxResetLicenses.Location = new System.Drawing.Point(602, 360);
            this.checkBoxResetLicenses.Name = "checkBoxResetLicenses";
            this.checkBoxResetLicenses.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBoxResetLicenses.Size = new System.Drawing.Size(219, 17);
            this.checkBoxResetLicenses.TabIndex = 12;
            this.checkBoxResetLicenses.Text = "Reset license counter on next encryption";
            this.checkBoxResetLicenses.UseVisualStyleBackColor = true;
            this.checkBoxResetLicenses.CheckedChanged += new System.EventHandler(this.checkBoxResetLicenses_CheckedChanged);
            // 
            // checkBoxUseSerialCounter
            // 
            this.checkBoxUseSerialCounter.AutoSize = true;
            this.checkBoxUseSerialCounter.Location = new System.Drawing.Point(603, 412);
            this.checkBoxUseSerialCounter.Name = "checkBoxUseSerialCounter";
            this.checkBoxUseSerialCounter.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBoxUseSerialCounter.Size = new System.Drawing.Size(140, 17);
            this.checkBoxUseSerialCounter.TabIndex = 12;
            this.checkBoxUseSerialCounter.Text = "Use 32 bit serial counter";
            this.checkBoxUseSerialCounter.UseVisualStyleBackColor = true;
            this.checkBoxUseSerialCounter.CheckedChanged += new System.EventHandler(this.checkBoxResetLicenses_CheckedChanged);
            // 
            // textBoxEepromStartAddress
            // 
            this.textBoxEepromStartAddress.Location = new System.Drawing.Point(580, 429);
            this.textBoxEepromStartAddress.Name = "textBoxEepromStartAddress";
            this.textBoxEepromStartAddress.Size = new System.Drawing.Size(54, 20);
            this.textBoxEepromStartAddress.TabIndex = 1;
            this.textBoxEepromStartAddress.Leave += new System.EventHandler(this.textBoxEepromStartAddress_Leave);
            // 
            // labelEepromStartAddress
            // 
            this.labelEepromStartAddress.AutoSize = true;
            this.labelEepromStartAddress.Location = new System.Drawing.Point(640, 433);
            this.labelEepromStartAddress.Name = "labelEepromStartAddress";
            this.labelEepromStartAddress.Size = new System.Drawing.Size(116, 13);
            this.labelEepromStartAddress.TabIndex = 4;
            this.labelEepromStartAddress.Text = "EEPROM start address";
            // 
            // cb_lock_bit0
            // 
            this.cb_lock_bit0.AutoSize = true;
            this.cb_lock_bit0.Location = new System.Drawing.Point(490, 276);
            this.cb_lock_bit0.Name = "cb_lock_bit0";
            this.cb_lock_bit0.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit0.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit0.TabIndex = 12;
            this.cb_lock_bit0.UseVisualStyleBackColor = true;
            this.cb_lock_bit0.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit1
            // 
            this.cb_lock_bit1.AutoSize = true;
            this.cb_lock_bit1.Location = new System.Drawing.Point(490, 261);
            this.cb_lock_bit1.Name = "cb_lock_bit1";
            this.cb_lock_bit1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit1.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit1.TabIndex = 12;
            this.cb_lock_bit1.UseVisualStyleBackColor = true;
            this.cb_lock_bit1.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit2
            // 
            this.cb_lock_bit2.AutoSize = true;
            this.cb_lock_bit2.Location = new System.Drawing.Point(490, 246);
            this.cb_lock_bit2.Name = "cb_lock_bit2";
            this.cb_lock_bit2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit2.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit2.TabIndex = 12;
            this.cb_lock_bit2.UseVisualStyleBackColor = true;
            this.cb_lock_bit2.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit3
            // 
            this.cb_lock_bit3.AutoSize = true;
            this.cb_lock_bit3.Location = new System.Drawing.Point(490, 231);
            this.cb_lock_bit3.Name = "cb_lock_bit3";
            this.cb_lock_bit3.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit3.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit3.TabIndex = 12;
            this.cb_lock_bit3.UseVisualStyleBackColor = true;
            this.cb_lock_bit3.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit4
            // 
            this.cb_lock_bit4.AutoSize = true;
            this.cb_lock_bit4.Location = new System.Drawing.Point(490, 216);
            this.cb_lock_bit4.Name = "cb_lock_bit4";
            this.cb_lock_bit4.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit4.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit4.TabIndex = 12;
            this.cb_lock_bit4.UseVisualStyleBackColor = true;
            this.cb_lock_bit4.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit5
            // 
            this.cb_lock_bit5.AutoSize = true;
            this.cb_lock_bit5.Location = new System.Drawing.Point(490, 201);
            this.cb_lock_bit5.Name = "cb_lock_bit5";
            this.cb_lock_bit5.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit5.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit5.TabIndex = 12;
            this.cb_lock_bit5.UseVisualStyleBackColor = true;
            this.cb_lock_bit5.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit6
            // 
            this.cb_lock_bit6.AutoSize = true;
            this.cb_lock_bit6.Location = new System.Drawing.Point(490, 186);
            this.cb_lock_bit6.Name = "cb_lock_bit6";
            this.cb_lock_bit6.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit6.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit6.TabIndex = 12;
            this.cb_lock_bit6.UseVisualStyleBackColor = true;
            this.cb_lock_bit6.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_lock_bit7
            // 
            this.cb_lock_bit7.AutoSize = true;
            this.cb_lock_bit7.Location = new System.Drawing.Point(490, 170);
            this.cb_lock_bit7.Name = "cb_lock_bit7";
            this.cb_lock_bit7.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_lock_bit7.Size = new System.Drawing.Size(15, 14);
            this.cb_lock_bit7.TabIndex = 12;
            this.cb_lock_bit7.UseVisualStyleBackColor = true;
            this.cb_lock_bit7.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low0
            // 
            this.cb_fuse_low0.AutoSize = true;
            this.cb_fuse_low0.Location = new System.Drawing.Point(338, 433);
            this.cb_fuse_low0.Name = "cb_fuse_low0";
            this.cb_fuse_low0.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low0.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low0.TabIndex = 12;
            this.cb_fuse_low0.UseVisualStyleBackColor = true;
            this.cb_fuse_low0.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low1
            // 
            this.cb_fuse_low1.AutoSize = true;
            this.cb_fuse_low1.Location = new System.Drawing.Point(338, 418);
            this.cb_fuse_low1.Name = "cb_fuse_low1";
            this.cb_fuse_low1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low1.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low1.TabIndex = 12;
            this.cb_fuse_low1.UseVisualStyleBackColor = true;
            this.cb_fuse_low1.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low2
            // 
            this.cb_fuse_low2.AutoSize = true;
            this.cb_fuse_low2.Location = new System.Drawing.Point(338, 403);
            this.cb_fuse_low2.Name = "cb_fuse_low2";
            this.cb_fuse_low2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low2.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low2.TabIndex = 12;
            this.cb_fuse_low2.UseVisualStyleBackColor = true;
            this.cb_fuse_low2.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low3
            // 
            this.cb_fuse_low3.AutoSize = true;
            this.cb_fuse_low3.Location = new System.Drawing.Point(338, 388);
            this.cb_fuse_low3.Name = "cb_fuse_low3";
            this.cb_fuse_low3.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low3.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low3.TabIndex = 12;
            this.cb_fuse_low3.UseVisualStyleBackColor = true;
            this.cb_fuse_low3.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low4
            // 
            this.cb_fuse_low4.AutoSize = true;
            this.cb_fuse_low4.Location = new System.Drawing.Point(338, 373);
            this.cb_fuse_low4.Name = "cb_fuse_low4";
            this.cb_fuse_low4.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low4.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low4.TabIndex = 12;
            this.cb_fuse_low4.UseVisualStyleBackColor = true;
            this.cb_fuse_low4.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low5
            // 
            this.cb_fuse_low5.AutoSize = true;
            this.cb_fuse_low5.Location = new System.Drawing.Point(338, 357);
            this.cb_fuse_low5.Name = "cb_fuse_low5";
            this.cb_fuse_low5.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low5.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low5.TabIndex = 12;
            this.cb_fuse_low5.UseVisualStyleBackColor = true;
            this.cb_fuse_low5.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low6
            // 
            this.cb_fuse_low6.AutoSize = true;
            this.cb_fuse_low6.Location = new System.Drawing.Point(338, 342);
            this.cb_fuse_low6.Name = "cb_fuse_low6";
            this.cb_fuse_low6.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low6.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low6.TabIndex = 12;
            this.cb_fuse_low6.UseVisualStyleBackColor = true;
            this.cb_fuse_low6.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_low7
            // 
            this.cb_fuse_low7.AutoSize = true;
            this.cb_fuse_low7.Location = new System.Drawing.Point(338, 327);
            this.cb_fuse_low7.Name = "cb_fuse_low7";
            this.cb_fuse_low7.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_low7.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_low7.TabIndex = 12;
            this.cb_fuse_low7.UseVisualStyleBackColor = true;
            this.cb_fuse_low7.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high0
            // 
            this.cb_fuse_high0.AutoSize = true;
            this.cb_fuse_high0.Location = new System.Drawing.Point(338, 278);
            this.cb_fuse_high0.Name = "cb_fuse_high0";
            this.cb_fuse_high0.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high0.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high0.TabIndex = 12;
            this.cb_fuse_high0.UseVisualStyleBackColor = true;
            this.cb_fuse_high0.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high1
            // 
            this.cb_fuse_high1.AutoSize = true;
            this.cb_fuse_high1.Location = new System.Drawing.Point(338, 263);
            this.cb_fuse_high1.Name = "cb_fuse_high1";
            this.cb_fuse_high1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high1.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high1.TabIndex = 12;
            this.cb_fuse_high1.UseVisualStyleBackColor = true;
            this.cb_fuse_high1.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high2
            // 
            this.cb_fuse_high2.AutoSize = true;
            this.cb_fuse_high2.Location = new System.Drawing.Point(338, 248);
            this.cb_fuse_high2.Name = "cb_fuse_high2";
            this.cb_fuse_high2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high2.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high2.TabIndex = 12;
            this.cb_fuse_high2.UseVisualStyleBackColor = true;
            this.cb_fuse_high2.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high3
            // 
            this.cb_fuse_high3.AutoSize = true;
            this.cb_fuse_high3.Location = new System.Drawing.Point(338, 233);
            this.cb_fuse_high3.Name = "cb_fuse_high3";
            this.cb_fuse_high3.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high3.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high3.TabIndex = 12;
            this.cb_fuse_high3.UseVisualStyleBackColor = true;
            this.cb_fuse_high3.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high4
            // 
            this.cb_fuse_high4.AutoSize = true;
            this.cb_fuse_high4.Location = new System.Drawing.Point(338, 218);
            this.cb_fuse_high4.Name = "cb_fuse_high4";
            this.cb_fuse_high4.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high4.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high4.TabIndex = 12;
            this.cb_fuse_high4.UseVisualStyleBackColor = true;
            this.cb_fuse_high4.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high5
            // 
            this.cb_fuse_high5.AutoSize = true;
            this.cb_fuse_high5.Location = new System.Drawing.Point(338, 203);
            this.cb_fuse_high5.Name = "cb_fuse_high5";
            this.cb_fuse_high5.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high5.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high5.TabIndex = 12;
            this.cb_fuse_high5.UseVisualStyleBackColor = true;
            this.cb_fuse_high5.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high6
            // 
            this.cb_fuse_high6.AutoSize = true;
            this.cb_fuse_high6.Location = new System.Drawing.Point(338, 187);
            this.cb_fuse_high6.Name = "cb_fuse_high6";
            this.cb_fuse_high6.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high6.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high6.TabIndex = 12;
            this.cb_fuse_high6.UseVisualStyleBackColor = true;
            this.cb_fuse_high6.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_high7
            // 
            this.cb_fuse_high7.AutoSize = true;
            this.cb_fuse_high7.Location = new System.Drawing.Point(338, 172);
            this.cb_fuse_high7.Name = "cb_fuse_high7";
            this.cb_fuse_high7.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_high7.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_high7.TabIndex = 12;
            this.cb_fuse_high7.UseVisualStyleBackColor = true;
            this.cb_fuse_high7.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext0
            // 
            this.cb_fuse_ext0.AutoSize = true;
            this.cb_fuse_ext0.Location = new System.Drawing.Point(490, 433);
            this.cb_fuse_ext0.Name = "cb_fuse_ext0";
            this.cb_fuse_ext0.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext0.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext0.TabIndex = 12;
            this.cb_fuse_ext0.UseVisualStyleBackColor = true;
            this.cb_fuse_ext0.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext1
            // 
            this.cb_fuse_ext1.AutoSize = true;
            this.cb_fuse_ext1.Location = new System.Drawing.Point(490, 418);
            this.cb_fuse_ext1.Name = "cb_fuse_ext1";
            this.cb_fuse_ext1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext1.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext1.TabIndex = 12;
            this.cb_fuse_ext1.UseVisualStyleBackColor = true;
            this.cb_fuse_ext1.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext2
            // 
            this.cb_fuse_ext2.AutoSize = true;
            this.cb_fuse_ext2.Location = new System.Drawing.Point(490, 403);
            this.cb_fuse_ext2.Name = "cb_fuse_ext2";
            this.cb_fuse_ext2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext2.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext2.TabIndex = 12;
            this.cb_fuse_ext2.UseVisualStyleBackColor = true;
            this.cb_fuse_ext2.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext3
            // 
            this.cb_fuse_ext3.AutoSize = true;
            this.cb_fuse_ext3.Location = new System.Drawing.Point(490, 388);
            this.cb_fuse_ext3.Name = "cb_fuse_ext3";
            this.cb_fuse_ext3.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext3.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext3.TabIndex = 12;
            this.cb_fuse_ext3.UseVisualStyleBackColor = true;
            this.cb_fuse_ext3.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext4
            // 
            this.cb_fuse_ext4.AutoSize = true;
            this.cb_fuse_ext4.Location = new System.Drawing.Point(490, 373);
            this.cb_fuse_ext4.Name = "cb_fuse_ext4";
            this.cb_fuse_ext4.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext4.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext4.TabIndex = 12;
            this.cb_fuse_ext4.UseVisualStyleBackColor = true;
            this.cb_fuse_ext4.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext5
            // 
            this.cb_fuse_ext5.AutoSize = true;
            this.cb_fuse_ext5.Location = new System.Drawing.Point(490, 358);
            this.cb_fuse_ext5.Name = "cb_fuse_ext5";
            this.cb_fuse_ext5.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext5.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext5.TabIndex = 12;
            this.cb_fuse_ext5.UseVisualStyleBackColor = true;
            this.cb_fuse_ext5.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext6
            // 
            this.cb_fuse_ext6.AutoSize = true;
            this.cb_fuse_ext6.Location = new System.Drawing.Point(490, 343);
            this.cb_fuse_ext6.Name = "cb_fuse_ext6";
            this.cb_fuse_ext6.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext6.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext6.TabIndex = 12;
            this.cb_fuse_ext6.UseVisualStyleBackColor = true;
            this.cb_fuse_ext6.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // cb_fuse_ext7
            // 
            this.cb_fuse_ext7.AutoSize = true;
            this.cb_fuse_ext7.Location = new System.Drawing.Point(490, 328);
            this.cb_fuse_ext7.Name = "cb_fuse_ext7";
            this.cb_fuse_ext7.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_fuse_ext7.Size = new System.Drawing.Size(15, 14);
            this.cb_fuse_ext7.TabIndex = 12;
            this.cb_fuse_ext7.UseVisualStyleBackColor = true;
            this.cb_fuse_ext7.CheckedChanged += new System.EventHandler(this.cb_fuse_high7_CheckedChanged);
            // 
            // buttonRefresh
            // 
            this.buttonRefresh.Location = new System.Drawing.Point(6, 232);
            this.buttonRefresh.Name = "buttonRefresh";
            this.buttonRefresh.Size = new System.Drawing.Size(66, 21);
            this.buttonRefresh.TabIndex = 6;
            this.buttonRefresh.Text = "Refresh";
            this.buttonRefresh.UseVisualStyleBackColor = true;
            this.buttonRefresh.Visible = false;
            this.buttonRefresh.Click += new System.EventHandler(this.comboBoxChipSelect_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.label4.Location = new System.Drawing.Point(304, 311);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(70, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "LOW FUSES";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(304, 277);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(28, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Bit 0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(304, 263);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(28, 13);
            this.label6.TabIndex = 4;
            this.label6.Text = "Bit 1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(304, 248);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(28, 13);
            this.label7.TabIndex = 4;
            this.label7.Text = "Bit 2";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(304, 233);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(28, 13);
            this.label8.TabIndex = 4;
            this.label8.Text = "Bit 3";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(304, 218);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(28, 13);
            this.label9.TabIndex = 4;
            this.label9.Text = "Bit 4";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(304, 204);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(28, 13);
            this.label10.TabIndex = 4;
            this.label10.Text = "Bit 5";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(304, 188);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(28, 13);
            this.label11.TabIndex = 4;
            this.label11.Text = "Bit 6";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(304, 172);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(28, 13);
            this.label12.TabIndex = 4;
            this.label12.Text = "Bit 7";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(304, 328);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(28, 13);
            this.label14.TabIndex = 13;
            this.label14.Text = "Bit 7";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(304, 344);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(28, 13);
            this.label15.TabIndex = 14;
            this.label15.Text = "Bit 6";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(304, 359);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(28, 13);
            this.label16.TabIndex = 15;
            this.label16.Text = "Bit 5";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(304, 373);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(28, 13);
            this.label17.TabIndex = 16;
            this.label17.Text = "Bit 4";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(304, 388);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(28, 13);
            this.label18.TabIndex = 17;
            this.label18.Text = "Bit 3";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(304, 403);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(28, 13);
            this.label19.TabIndex = 18;
            this.label19.Text = "Bit 2";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(304, 418);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(28, 13);
            this.label24.TabIndex = 19;
            this.label24.Text = "Bit 1";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(304, 432);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(28, 13);
            this.label25.TabIndex = 20;
            this.label25.Text = "Bit 0";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.label26.Location = new System.Drawing.Point(304, 154);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(72, 13);
            this.label26.TabIndex = 21;
            this.label26.Text = "HIGH FUSES";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(450, 328);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(28, 13);
            this.label27.TabIndex = 22;
            this.label27.Text = "Bit 7";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(450, 344);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(28, 13);
            this.label28.TabIndex = 23;
            this.label28.Text = "Bit 6";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(450, 359);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(28, 13);
            this.label29.TabIndex = 24;
            this.label29.Text = "Bit 5";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(450, 373);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(28, 13);
            this.label30.TabIndex = 25;
            this.label30.Text = "Bit 4";
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(450, 388);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(28, 13);
            this.label31.TabIndex = 26;
            this.label31.Text = "Bit 3";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(450, 403);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(28, 13);
            this.label32.TabIndex = 27;
            this.label32.Text = "Bit 2";
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(450, 418);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(28, 13);
            this.label33.TabIndex = 28;
            this.label33.Text = "Bit 1";
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(450, 432);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(28, 13);
            this.label34.TabIndex = 29;
            this.label34.Text = "Bit 0";
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.label35.Location = new System.Drawing.Point(450, 311);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(66, 13);
            this.label35.TabIndex = 30;
            this.label35.Text = "EXT FUSES";
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Location = new System.Drawing.Point(450, 171);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(28, 13);
            this.label36.TabIndex = 31;
            this.label36.Text = "Bit 7";
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Location = new System.Drawing.Point(450, 187);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(28, 13);
            this.label37.TabIndex = 32;
            this.label37.Text = "Bit 6";
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Location = new System.Drawing.Point(450, 202);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(28, 13);
            this.label38.TabIndex = 33;
            this.label38.Text = "Bit 5";
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Location = new System.Drawing.Point(450, 216);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(28, 13);
            this.label39.TabIndex = 34;
            this.label39.Text = "Bit 4";
            // 
            // label40
            // 
            this.label40.AutoSize = true;
            this.label40.Location = new System.Drawing.Point(450, 231);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(28, 13);
            this.label40.TabIndex = 35;
            this.label40.Text = "Bit 3";
            // 
            // label41
            // 
            this.label41.AutoSize = true;
            this.label41.Location = new System.Drawing.Point(450, 246);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(28, 13);
            this.label41.TabIndex = 36;
            this.label41.Text = "Bit 2";
            // 
            // label42
            // 
            this.label42.AutoSize = true;
            this.label42.Location = new System.Drawing.Point(450, 261);
            this.label42.Name = "label42";
            this.label42.Size = new System.Drawing.Size(28, 13);
            this.label42.TabIndex = 37;
            this.label42.Text = "Bit 1";
            // 
            // label43
            // 
            this.label43.AutoSize = true;
            this.label43.Location = new System.Drawing.Point(450, 275);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(28, 13);
            this.label43.TabIndex = 38;
            this.label43.Text = "Bit 0";
            // 
            // label44
            // 
            this.label44.AutoSize = true;
            this.label44.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.label44.Location = new System.Drawing.Point(450, 154);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(62, 13);
            this.label44.TabIndex = 39;
            this.label44.Text = "LOCK BITS";
            // 
            // label45
            // 
            this.label45.AutoSize = true;
            this.label45.Location = new System.Drawing.Point(640, 378);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(82, 13);
            this.label45.TabIndex = 4;
            this.label45.Text = "0 = Free license";
            // 
            // label46
            // 
            this.label46.AutoSize = true;
            this.label46.Location = new System.Drawing.Point(810, 148);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(133, 13);
            this.label46.TabIndex = 4;
            this.label46.Text = "0=4MHz,1=2MHz,2=1MHz";
            this.label46.Visible = false;
            // 
            // textBoxFlashHexFilePath
            // 
            this.textBoxFlashHexFilePath.Location = new System.Drawing.Point(161, 464);
            this.textBoxFlashHexFilePath.Multiline = true;
            this.textBoxFlashHexFilePath.Name = "textBoxFlashHexFilePath";
            this.textBoxFlashHexFilePath.ReadOnly = true;
            this.textBoxFlashHexFilePath.Size = new System.Drawing.Size(651, 23);
            this.textBoxFlashHexFilePath.TabIndex = 1;
            // 
            // label48
            // 
            this.label48.AutoSize = true;
            this.label48.Location = new System.Drawing.Point(50, 466);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(111, 13);
            this.label48.TabIndex = 4;
            this.label48.Text = "AppFlash hex file path";
            // 
            // buttonSelectEEPROM
            // 
            this.buttonSelectEEPROM.Location = new System.Drawing.Point(158, 65);
            this.buttonSelectEEPROM.Name = "buttonSelectEEPROM";
            this.buttonSelectEEPROM.Size = new System.Drawing.Size(118, 24);
            this.buttonSelectEEPROM.TabIndex = 6;
            this.buttonSelectEEPROM.Text = "Select Eeprom *.hex file";
            this.buttonSelectEEPROM.UseVisualStyleBackColor = true;
            this.buttonSelectEEPROM.Click += new System.EventHandler(this.buttonSelectEEPROM_Click);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(1002, 742);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBox1.Size = new System.Drawing.Size(111, 17);
            this.checkBox1.TabIndex = 12;
            this.checkBox1.Text = "Use serial counter";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBoxResetLicenses_CheckedChanged);
            // 
            // textBoxEepromHexFilePath
            // 
            this.textBoxEepromHexFilePath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxEepromHexFilePath.Location = new System.Drawing.Point(161, 522);
            this.textBoxEepromHexFilePath.Multiline = true;
            this.textBoxEepromHexFilePath.Name = "textBoxEepromHexFilePath";
            this.textBoxEepromHexFilePath.ReadOnly = true;
            this.textBoxEepromHexFilePath.Size = new System.Drawing.Size(651, 23);
            this.textBoxEepromHexFilePath.TabIndex = 1;
            // 
            // textBoxUpStep
            // 
            this.textBoxUpStep.Location = new System.Drawing.Point(640, 52);
            this.textBoxUpStep.Name = "textBoxUpStep";
            this.textBoxUpStep.Size = new System.Drawing.Size(54, 20);
            this.textBoxUpStep.TabIndex = 1;
            this.textBoxUpStep.MouseLeave += new System.EventHandler(this.textBoxUpStep_MouseLeave);
            // 
            // textBoxMaxFolders
            // 
            this.textBoxMaxFolders.Location = new System.Drawing.Point(640, 31);
            this.textBoxMaxFolders.Name = "textBoxMaxFolders";
            this.textBoxMaxFolders.Size = new System.Drawing.Size(54, 20);
            this.textBoxMaxFolders.TabIndex = 1;
            this.textBoxMaxFolders.TextChanged += new System.EventHandler(this.textBoxMaxFolders_TextChanged);
            // 
            // label51
            // 
            this.label51.AutoSize = true;
            this.label51.Location = new System.Drawing.Point(698, 56);
            this.label51.Name = "label51";
            this.label51.Size = new System.Drawing.Size(46, 13);
            this.label51.TabIndex = 4;
            this.label51.Text = "Step Up";
            // 
            // label52
            // 
            this.label52.AutoSize = true;
            this.label52.Location = new System.Drawing.Point(697, 34);
            this.label52.MaximumSize = new System.Drawing.Size(150, 0);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(126, 13);
            this.label52.TabIndex = 4;
            this.label52.Text = "Number of Folders (1..99)";
            // 
            // textBoxDownStep
            // 
            this.textBoxDownStep.Location = new System.Drawing.Point(640, 73);
            this.textBoxDownStep.Name = "textBoxDownStep";
            this.textBoxDownStep.Size = new System.Drawing.Size(54, 20);
            this.textBoxDownStep.TabIndex = 1;
            this.textBoxDownStep.TextChanged += new System.EventHandler(this.textBoxDownStep_TextChanged);
            this.textBoxDownStep.MouseLeave += new System.EventHandler(this.textBoxDownStep_MouseLeave);
            // 
            // label50
            // 
            this.label50.AutoSize = true;
            this.label50.Location = new System.Drawing.Point(698, 77);
            this.label50.Name = "label50";
            this.label50.Size = new System.Drawing.Size(60, 13);
            this.label50.TabIndex = 4;
            this.label50.Text = "Step Down";
            // 
            // comboBoxFolderSelector
            // 
            this.comboBoxFolderSelector.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.comboBoxFolderSelector.FormattingEnabled = true;
            this.comboBoxFolderSelector.Location = new System.Drawing.Point(640, 6);
            this.comboBoxFolderSelector.Name = "comboBoxFolderSelector";
            this.comboBoxFolderSelector.Size = new System.Drawing.Size(54, 21);
            this.comboBoxFolderSelector.TabIndex = 10;
            this.comboBoxFolderSelector.SelectedIndexChanged += new System.EventHandler(this.comboBoxFolderSelector_SelectedIndexChanged);
            // 
            // label53
            // 
            this.label53.AutoSize = true;
            this.label53.Location = new System.Drawing.Point(697, 10);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(78, 13);
            this.label53.TabIndex = 4;
            this.label53.Text = "Folder Selector";
            // 
            // buttonMakeList
            // 
            this.buttonMakeList.Location = new System.Drawing.Point(640, 96);
            this.buttonMakeList.Name = "buttonMakeList";
            this.buttonMakeList.Size = new System.Drawing.Size(73, 27);
            this.buttonMakeList.TabIndex = 6;
            this.buttonMakeList.Text = "Make a list in folder \'0\'";
            this.buttonMakeList.UseVisualStyleBackColor = true;
            this.buttonMakeList.Click += new System.EventHandler(this.buttonMakeList_Click);
            // 
            // labelProgNum
            // 
            this.labelProgNum.AutoSize = true;
            this.labelProgNum.Location = new System.Drawing.Point(18, 98);
            this.labelProgNum.Name = "labelProgNum";
            this.labelProgNum.Size = new System.Drawing.Size(0, 13);
            this.labelProgNum.TabIndex = 4;
            // 
            // textBoxFileInfo2
            // 
            this.textBoxFileInfo2.Location = new System.Drawing.Point(21, 151);
            this.textBoxFileInfo2.MaxLength = 15;
            this.textBoxFileInfo2.Name = "textBoxFileInfo2";
            this.textBoxFileInfo2.Size = new System.Drawing.Size(99, 20);
            this.textBoxFileInfo2.TabIndex = 1;
            // 
            // label54
            // 
            this.label54.AutoSize = true;
            this.label54.Location = new System.Drawing.Point(127, 154);
            this.label54.Name = "label54";
            this.label54.Size = new System.Drawing.Size(144, 13);
            this.label54.TabIndex = 4;
            this.label54.Text = "Program info line 3 (15 chars)";
            // 
            // textBoxEepromStartAddressCalibBytes
            // 
            this.textBoxEepromStartAddressCalibBytes.Location = new System.Drawing.Point(580, 328);
            this.textBoxEepromStartAddressCalibBytes.Name = "textBoxEepromStartAddressCalibBytes";
            this.textBoxEepromStartAddressCalibBytes.Size = new System.Drawing.Size(54, 20);
            this.textBoxEepromStartAddressCalibBytes.TabIndex = 1;
            this.textBoxEepromStartAddressCalibBytes.Leave += new System.EventHandler(this.textBoxEepromStartAddressCalibBytes_Leave);
            // 
            // checkBoxUseCalibBytes
            // 
            this.checkBoxUseCalibBytes.AutoSize = true;
            this.checkBoxUseCalibBytes.Location = new System.Drawing.Point(601, 311);
            this.checkBoxUseCalibBytes.Name = "checkBoxUseCalibBytes";
            this.checkBoxUseCalibBytes.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBoxUseCalibBytes.Size = new System.Drawing.Size(201, 17);
            this.checkBoxUseCalibBytes.TabIndex = 12;
            this.checkBoxUseCalibBytes.Text = "Write 4 Calibration bytes to EEPROM";
            this.checkBoxUseCalibBytes.UseVisualStyleBackColor = true;
            this.checkBoxUseCalibBytes.CheckedChanged += new System.EventHandler(this.checkBoxResetLicenses_CheckedChanged);
            // 
            // labelEepromStartAddressCalibBytes
            // 
            this.labelEepromStartAddressCalibBytes.AutoSize = true;
            this.labelEepromStartAddressCalibBytes.Location = new System.Drawing.Point(638, 332);
            this.labelEepromStartAddressCalibBytes.Name = "labelEepromStartAddressCalibBytes";
            this.labelEepromStartAddressCalibBytes.Size = new System.Drawing.Size(116, 13);
            this.labelEepromStartAddressCalibBytes.TabIndex = 4;
            this.labelEepromStartAddressCalibBytes.Text = "EEPROM start address";
            // 
            // textBoxBootHexFile
            // 
            this.textBoxBootHexFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxBootHexFile.Location = new System.Drawing.Point(12, 39);
            this.textBoxBootHexFile.Name = "textBoxBootHexFile";
            this.textBoxBootHexFile.ReadOnly = true;
            this.textBoxBootHexFile.Size = new System.Drawing.Size(142, 20);
            this.textBoxBootHexFile.TabIndex = 1;
            // 
            // buttonSelectBoot
            // 
            this.buttonSelectBoot.Location = new System.Drawing.Point(158, 36);
            this.buttonSelectBoot.Name = "buttonSelectBoot";
            this.buttonSelectBoot.Size = new System.Drawing.Size(118, 23);
            this.buttonSelectBoot.TabIndex = 6;
            this.buttonSelectBoot.Text = "Select Boot *.hex file";
            this.buttonSelectBoot.UseVisualStyleBackColor = true;
            this.buttonSelectBoot.Click += new System.EventHandler(this.buttonSelectBoot_Click);
            // 
            // textBoxBootHexFilePath
            // 
            this.textBoxBootHexFilePath.Location = new System.Drawing.Point(161, 493);
            this.textBoxBootHexFilePath.Multiline = true;
            this.textBoxBootHexFilePath.Name = "textBoxBootHexFilePath";
            this.textBoxBootHexFilePath.ReadOnly = true;
            this.textBoxBootHexFilePath.Size = new System.Drawing.Size(651, 23);
            this.textBoxBootHexFilePath.TabIndex = 1;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(47, 495);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(114, 13);
            this.label13.TabIndex = 4;
            this.label13.Text = "BootFlash hex file path";
            // 
            // label47
            // 
            this.label47.AutoSize = true;
            this.label47.Location = new System.Drawing.Point(46, 525);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(113, 13);
            this.label47.TabIndex = 4;
            this.label47.Text = "EEPROM hex file path";
            // 
            // textBoxEepromHexFile
            // 
            this.textBoxEepromHexFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxEepromHexFile.Location = new System.Drawing.Point(12, 67);
            this.textBoxEepromHexFile.Name = "textBoxEepromHexFile";
            this.textBoxEepromHexFile.ReadOnly = true;
            this.textBoxEepromHexFile.Size = new System.Drawing.Size(142, 20);
            this.textBoxEepromHexFile.TabIndex = 1;
            // 
            // label49
            // 
            this.label49.AutoSize = true;
            this.label49.Enabled = false;
            this.label49.Location = new System.Drawing.Point(639, 132);
            this.label49.Name = "label49";
            this.label49.Size = new System.Drawing.Size(74, 13);
            this.label49.TabIndex = 4;
            this.label49.Text = "0 = Decrypted";
            this.label49.Visible = false;
            // 
            // textBoxEncryptionCounter
            // 
            this.textBoxEncryptionCounter.Location = new System.Drawing.Point(580, 232);
            this.textBoxEncryptionCounter.Name = "textBoxEncryptionCounter";
            this.textBoxEncryptionCounter.ReadOnly = true;
            this.textBoxEncryptionCounter.Size = new System.Drawing.Size(54, 20);
            this.textBoxEncryptionCounter.TabIndex = 1;
            // 
            // label55
            // 
            this.label55.AutoSize = true;
            this.label55.Location = new System.Drawing.Point(640, 235);
            this.label55.Name = "label55";
            this.label55.Size = new System.Drawing.Size(96, 13);
            this.label55.TabIndex = 4;
            this.label55.Text = "Encryption counter";
            // 
            // textBoxFileUniqueVale
            // 
            this.textBoxFileUniqueVale.Location = new System.Drawing.Point(580, 174);
            this.textBoxFileUniqueVale.Name = "textBoxFileUniqueVale";
            this.textBoxFileUniqueVale.ReadOnly = true;
            this.textBoxFileUniqueVale.Size = new System.Drawing.Size(54, 20);
            this.textBoxFileUniqueVale.TabIndex = 1;
            // 
            // label56
            // 
            this.label56.AutoSize = true;
            this.label56.Location = new System.Drawing.Point(640, 177);
            this.label56.Name = "label56";
            this.label56.Size = new System.Drawing.Size(87, 13);
            this.label56.TabIndex = 4;
            this.label56.Text = "File unique value";
            // 
            // checkBoxStopUsingUniqueFile
            // 
            this.checkBoxStopUsingUniqueFile.AutoSize = true;
            this.checkBoxStopUsingUniqueFile.Location = new System.Drawing.Point(600, 198);
            this.checkBoxStopUsingUniqueFile.Name = "checkBoxStopUsingUniqueFile";
            this.checkBoxStopUsingUniqueFile.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBoxStopUsingUniqueFile.Size = new System.Drawing.Size(180, 30);
            this.checkBoxStopUsingUniqueFile.TabIndex = 12;
            this.checkBoxStopUsingUniqueFile.Text = "Force using this file unique value\r\nwith current encription counter";
            this.checkBoxStopUsingUniqueFile.UseVisualStyleBackColor = true;
            this.checkBoxStopUsingUniqueFile.CheckedChanged += new System.EventHandler(this.checkBoxResetLicenses_CheckedChanged);
            // 
            // label57
            // 
            this.label57.AutoSize = true;
            this.label57.Location = new System.Drawing.Point(43, 554);
            this.label57.Name = "label57";
            this.label57.Size = new System.Drawing.Size(116, 13);
            this.label57.TabIndex = 4;
            this.label57.Text = "Programmer folder path";
            // 
            // label58
            // 
            this.label58.AutoSize = true;
            this.label58.Location = new System.Drawing.Point(668, 272);
            this.label58.Name = "label58";
            this.label58.Size = new System.Drawing.Size(54, 13);
            this.label58.TabIndex = 4;
            this.label58.Text = "ISP Clock";
            // 
            // comboBoxIspClock
            // 
            this.comboBoxIspClock.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.comboBoxIspClock.FormattingEnabled = true;
            this.comboBoxIspClock.Location = new System.Drawing.Point(580, 268);
            this.comboBoxIspClock.Name = "comboBoxIspClock";
            this.comboBoxIspClock.Size = new System.Drawing.Size(84, 21);
            this.comboBoxIspClock.TabIndex = 10;
            // 
            // cbFwUpdate
            // 
            this.cbFwUpdate.AutoSize = true;
            this.cbFwUpdate.Location = new System.Drawing.Point(781, 8);
            this.cbFwUpdate.Name = "cbFwUpdate";
            this.cbFwUpdate.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cbFwUpdate.Size = new System.Drawing.Size(75, 17);
            this.cbFwUpdate.TabIndex = 40;
            this.cbFwUpdate.Text = "FwUpdate";
            this.cbFwUpdate.UseVisualStyleBackColor = true;
            this.cbFwUpdate.Visible = false;
            // 
            // buttonOpenList
            // 
            this.buttonOpenList.Location = new System.Drawing.Point(719, 96);
            this.buttonOpenList.Name = "buttonOpenList";
            this.buttonOpenList.Size = new System.Drawing.Size(73, 27);
            this.buttonOpenList.TabIndex = 6;
            this.buttonOpenList.Text = "Open a list in folder \'0\'";
            this.buttonOpenList.UseVisualStyleBackColor = true;
            this.buttonOpenList.Click += new System.EventHandler(this.buttonOpenList_Click);
            // 
            // buttonCopyFuses
            // 
            this.buttonCopyFuses.Location = new System.Drawing.Point(270, 286);
            this.buttonCopyFuses.Name = "buttonCopyFuses";
            this.buttonCopyFuses.Size = new System.Drawing.Size(25, 73);
            this.buttonCopyFuses.TabIndex = 6;
            this.buttonCopyFuses.Text = "Copy";
            this.buttonCopyFuses.UseVisualStyleBackColor = true;
            this.buttonCopyFuses.Click += new System.EventHandler(this.buttonCopyFuses_Click);
            // 
            // buttonWriteChipConfig
            // 
            this.buttonWriteChipConfig.Location = new System.Drawing.Point(3, 202);
            this.buttonWriteChipConfig.Name = "buttonWriteChipConfig";
            this.buttonWriteChipConfig.Size = new System.Drawing.Size(74, 21);
            this.buttonWriteChipConfig.TabIndex = 6;
            this.buttonWriteChipConfig.Text = "Write config";
            this.buttonWriteChipConfig.UseVisualStyleBackColor = true;
            this.buttonWriteChipConfig.Click += new System.EventHandler(this.buttonWriteChipConfig_Click);
            // 
            // buttonReadChipConfig
            // 
            this.buttonReadChipConfig.Location = new System.Drawing.Point(3, 181);
            this.buttonReadChipConfig.Name = "buttonReadChipConfig";
            this.buttonReadChipConfig.Size = new System.Drawing.Size(74, 21);
            this.buttonReadChipConfig.TabIndex = 6;
            this.buttonReadChipConfig.Text = "Read config";
            this.buttonReadChipConfig.UseVisualStyleBackColor = true;
            this.buttonReadChipConfig.Click += new System.EventHandler(this.buttonReadChipConfig_Click);
            // 
            // windowForm1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(829, 602);
            this.Controls.Add(this.cbFwUpdate);
            this.Controls.Add(this.label36);
            this.Controls.Add(this.label37);
            this.Controls.Add(this.label38);
            this.Controls.Add(this.label39);
            this.Controls.Add(this.label40);
            this.Controls.Add(this.label41);
            this.Controls.Add(this.label42);
            this.Controls.Add(this.label43);
            this.Controls.Add(this.label44);
            this.Controls.Add(this.label27);
            this.Controls.Add(this.label28);
            this.Controls.Add(this.label29);
            this.Controls.Add(this.label30);
            this.Controls.Add(this.label31);
            this.Controls.Add(this.label32);
            this.Controls.Add(this.label33);
            this.Controls.Add(this.label34);
            this.Controls.Add(this.label35);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.label18);
            this.Controls.Add(this.label19);
            this.Controls.Add(this.label24);
            this.Controls.Add(this.label25);
            this.Controls.Add(this.label26);
            this.Controls.Add(this.buttonReadChipConfig);
            this.Controls.Add(this.buttonWriteChipConfig);
            this.Controls.Add(this.buttonRefresh);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.checkBoxUseCalibBytes);
            this.Controls.Add(this.checkBoxUseSerialCounter);
            this.Controls.Add(this.cb_fuse_ext7);
            this.Controls.Add(this.cb_fuse_high7);
            this.Controls.Add(this.cb_fuse_low7);
            this.Controls.Add(this.cb_fuse_ext6);
            this.Controls.Add(this.cb_lock_bit7);
            this.Controls.Add(this.cb_fuse_high6);
            this.Controls.Add(this.cb_fuse_low6);
            this.Controls.Add(this.cb_fuse_ext5);
            this.Controls.Add(this.cb_lock_bit6);
            this.Controls.Add(this.cb_fuse_high5);
            this.Controls.Add(this.cb_fuse_low5);
            this.Controls.Add(this.cb_fuse_ext4);
            this.Controls.Add(this.cb_lock_bit5);
            this.Controls.Add(this.cb_fuse_high4);
            this.Controls.Add(this.cb_fuse_low4);
            this.Controls.Add(this.cb_fuse_ext3);
            this.Controls.Add(this.cb_lock_bit4);
            this.Controls.Add(this.cb_fuse_ext2);
            this.Controls.Add(this.cb_fuse_high3);
            this.Controls.Add(this.cb_fuse_high2);
            this.Controls.Add(this.cb_fuse_low3);
            this.Controls.Add(this.cb_fuse_ext1);
            this.Controls.Add(this.cb_fuse_low2);
            this.Controls.Add(this.cb_fuse_high1);
            this.Controls.Add(this.cb_lock_bit3);
            this.Controls.Add(this.cb_fuse_ext0);
            this.Controls.Add(this.cb_fuse_low1);
            this.Controls.Add(this.cb_fuse_high0);
            this.Controls.Add(this.cb_lock_bit2);
            this.Controls.Add(this.cb_fuse_low0);
            this.Controls.Add(this.cb_lock_bit1);
            this.Controls.Add(this.cb_lock_bit0);
            this.Controls.Add(this.checkBoxStopUsingUniqueFile);
            this.Controls.Add(this.checkBoxResetLicenses);
            this.Controls.Add(this.comboBoxIspClock);
            this.Controls.Add(this.comboBoxFolderSelector);
            this.Controls.Add(this.comboBoxChipSelect);
            this.Controls.Add(this.progressBarCrypt);
            this.Controls.Add(this.buttonWriteToIni);
            this.Controls.Add(this.buttonCreateOpenIni);
            this.Controls.Add(this.buttonWriteConfig);
            this.Controls.Add(this.buttonSelectEEPROM);
            this.Controls.Add(this.buttonCopyFuses);
            this.Controls.Add(this.buttonOpenList);
            this.Controls.Add(this.buttonMakeList);
            this.Controls.Add(this.cancelAsyncButton);
            this.Controls.Add(this.buttonSelectBoot);
            this.Controls.Add(this.buttonSelectHEX);
            this.Controls.Add(this.buttonSelectFolder);
            this.Controls.Add(this.buttonOpenConfig);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label52);
            this.Controls.Add(this.label56);
            this.Controls.Add(this.label55);
            this.Controls.Add(this.label49);
            this.Controls.Add(this.labelRollingCounter);
            this.Controls.Add(this.label57);
            this.Controls.Add(this.label47);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label48);
            this.Controls.Add(this.labelEepromStartAddressCalibBytes);
            this.Controls.Add(this.labelEepromStartAddress);
            this.Controls.Add(this.label50);
            this.Controls.Add(this.label51);
            this.Controls.Add(this.labelSpiDiv);
            this.Controls.Add(this.label46);
            this.Controls.Add(this.label45);
            this.Controls.Add(this.labelLicenseTotal);
            this.Controls.Add(this.labelFusesMask);
            this.Controls.Add(this.labelEepromPageSize);
            this.Controls.Add(this.labelFlashPageSize);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.labelSignature);
            this.Controls.Add(this.labelEepromSize);
            this.Controls.Add(this.labelFlashSize);
            this.Controls.Add(this.label54);
            this.Controls.Add(this.labelFileInfo);
            this.Controls.Add(this.labelProgNum);
            this.Controls.Add(this.label58);
            this.Controls.Add(this.labelProgName);
            this.Controls.Add(this.label53);
            this.Controls.Add(this.labelChipSelector);
            this.Controls.Add(this.label23);
            this.Controls.Add(this.label22);
            this.Controls.Add(this.label21);
            this.Controls.Add(this.label20);
            this.Controls.Add(this.labelFusesFinal);
            this.Controls.Add(this.labelFuses);
            this.Controls.Add(this.labelFusesDefaults);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.labelStatus);
            this.Controls.Add(this.labelStatusConst);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_OutputHex);
            this.Controls.Add(this.textBox_Output);
            this.Controls.Add(this.textBoxFileUniqueVale);
            this.Controls.Add(this.textBoxMaxFolders);
            this.Controls.Add(this.textBoxEncryptionCounter);
            this.Controls.Add(this.textBoxRollingCounter);
            this.Controls.Add(this.textBoxEepromHexFilePath);
            this.Controls.Add(this.textBoxBootHexFilePath);
            this.Controls.Add(this.textBoxFlashHexFilePath);
            this.Controls.Add(this.textBoxDownStep);
            this.Controls.Add(this.textBoxPath);
            this.Controls.Add(this.textBoxUpStep);
            this.Controls.Add(this.textBoxEepromStartAddressCalibBytes);
            this.Controls.Add(this.textBoxEepromStartAddress);
            this.Controls.Add(this.textBoxSPIDIV);
            this.Controls.Add(this.textBoxLicense);
            this.Controls.Add(this.textBoxMaskFuseLock);
            this.Controls.Add(this.textBoxMaskFuseExt);
            this.Controls.Add(this.textBoxMaskFuseLow);
            this.Controls.Add(this.textBoxMaskFuseHigh);
            this.Controls.Add(this.textBoxEepromPageSizeBytes);
            this.Controls.Add(this.textBoxPagesizeWords);
            this.Controls.Add(this.textBoxSignature);
            this.Controls.Add(this.textBoxEepromSizeBytes);
            this.Controls.Add(this.textBoxFileInfo2);
            this.Controls.Add(this.textBoxChipsizeBytes);
            this.Controls.Add(this.textBoxFileInfo);
            this.Controls.Add(this.textBoxEepromHexFile);
            this.Controls.Add(this.textBoxBootHexFile);
            this.Controls.Add(this.textBoxProgname);
            this.Controls.Add(this.textBoxFilename);
            this.Controls.Add(this.textBoxFinalFuseLock);
            this.Controls.Add(this.textBoxProgFuseLock);
            this.Controls.Add(this.textBoxFinalFuseExt);
            this.Controls.Add(this.textBoxProgFuseExt);
            this.Controls.Add(this.textBoxFinalFuseLow);
            this.Controls.Add(this.textBoxProgFuseLow);
            this.Controls.Add(this.textBoxFinalFuseHigh);
            this.Controls.Add(this.textBoxProgFuseHigh);
            this.Controls.Add(this.textBox_Key);
            this.Controls.Add(this.textBox_InputHex);
            this.Controls.Add(this.textBox_Input);
            this.MaximumSize = new System.Drawing.Size(845, 640);
            this.MinimumSize = new System.Drawing.Size(845, 640);
            this.Name = "windowForm1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "AVR Standalone ISP Cryptor. SW Version 1.0.42.21";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.windowForm1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_Input;
        private System.Windows.Forms.TextBox textBox_Key;
        private System.Windows.Forms.TextBox textBox_Output;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_InputHex;
        private System.Windows.Forms.TextBox textBox_OutputHex;
        private System.Windows.Forms.TextBox textBoxRollingCounter;
        private System.Windows.Forms.Label labelRollingCounter;
        private System.Windows.Forms.TextBox textBoxProgFuseHigh;
        private System.Windows.Forms.Label labelFusesDefaults;
        private System.Windows.Forms.TextBox textBoxMaskFuseHigh;
        private System.Windows.Forms.Label labelFusesMask;
        private System.Windows.Forms.TextBox textBoxChipsizeBytes;
        private System.Windows.Forms.TextBox textBoxPagesizeWords;
        private System.Windows.Forms.TextBox textBoxLicense;
        private System.Windows.Forms.Label labelFlashSize;
        private System.Windows.Forms.Label labelFlashPageSize;
        private System.Windows.Forms.Label labelLicenseTotal;
        private System.Windows.Forms.Label labelChipSelector;
        private System.Windows.Forms.TextBox textBoxFilename;
        private System.Windows.Forms.TextBox textBoxPath;
        private System.Windows.Forms.Button buttonOpenConfig;
        private System.Windows.Forms.Button buttonCreateOpenIni;
        private System.Windows.Forms.Button buttonWriteToIni;
        private System.Windows.Forms.TextBox textBoxSignature;
        private System.Windows.Forms.Label labelSignature;
        private System.Windows.Forms.TextBox textBoxProgname;
        private System.Windows.Forms.Label labelProgName;
        private System.Windows.Forms.Button buttonWriteConfig;
        private System.Windows.Forms.Button buttonSelectFolder;
        private System.Windows.Forms.ProgressBar progressBarCrypt;
        private System.Windows.Forms.Button cancelAsyncButton;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.Label labelStatusConst;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Button buttonSelectHEX;
        private System.Windows.Forms.TextBox textBoxFileInfo;
        private System.Windows.Forms.Label labelFileInfo;
        private System.Windows.Forms.ComboBox comboBoxChipSelect;
        private System.Windows.Forms.TextBox textBoxProgFuseLow;
        private System.Windows.Forms.TextBox textBoxProgFuseExt;
        private System.Windows.Forms.TextBox textBoxProgFuseLock;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox textBoxFinalFuseHigh;
        private System.Windows.Forms.TextBox textBoxFinalFuseLow;
        private System.Windows.Forms.TextBox textBoxFinalFuseExt;
        private System.Windows.Forms.TextBox textBoxFinalFuseLock;
        private System.Windows.Forms.Label labelFusesFinal;
        private System.Windows.Forms.TextBox textBoxEepromSizeBytes;
        private System.Windows.Forms.TextBox textBoxEepromPageSizeBytes;
        private System.Windows.Forms.Label labelEepromSize;
        private System.Windows.Forms.Label labelEepromPageSize;
        private System.Windows.Forms.TextBox textBoxMaskFuseLow;
        private System.Windows.Forms.TextBox textBoxMaskFuseExt;
        private System.Windows.Forms.TextBox textBoxMaskFuseLock;
        private System.Windows.Forms.TextBox textBoxSPIDIV;
        private System.Windows.Forms.Label labelSpiDiv;
        private System.Windows.Forms.Label labelFuses;
        private System.Windows.Forms.CheckBox checkBoxResetLicenses;
        private System.Windows.Forms.CheckBox checkBoxUseSerialCounter;
        private System.Windows.Forms.TextBox textBoxEepromStartAddress;
        private System.Windows.Forms.Label labelEepromStartAddress;
        private System.Windows.Forms.CheckBox cb_lock_bit0;
        private System.Windows.Forms.CheckBox cb_lock_bit1;
        private System.Windows.Forms.CheckBox cb_lock_bit2;
        private System.Windows.Forms.CheckBox cb_lock_bit3;
        private System.Windows.Forms.CheckBox cb_lock_bit4;
        private System.Windows.Forms.CheckBox cb_lock_bit5;
        private System.Windows.Forms.CheckBox cb_lock_bit6;
        private System.Windows.Forms.CheckBox cb_lock_bit7;
        private System.Windows.Forms.CheckBox cb_fuse_low0;
        private System.Windows.Forms.CheckBox cb_fuse_low1;
        private System.Windows.Forms.CheckBox cb_fuse_low2;
        private System.Windows.Forms.CheckBox cb_fuse_low3;
        private System.Windows.Forms.CheckBox cb_fuse_low4;
        private System.Windows.Forms.CheckBox cb_fuse_low5;
        private System.Windows.Forms.CheckBox cb_fuse_low6;
        private System.Windows.Forms.CheckBox cb_fuse_low7;
        private System.Windows.Forms.CheckBox cb_fuse_high0;
        private System.Windows.Forms.CheckBox cb_fuse_high1;
        private System.Windows.Forms.CheckBox cb_fuse_high2;
        private System.Windows.Forms.CheckBox cb_fuse_high3;
        private System.Windows.Forms.CheckBox cb_fuse_high4;
        private System.Windows.Forms.CheckBox cb_fuse_high5;
        private System.Windows.Forms.CheckBox cb_fuse_high6;
        private System.Windows.Forms.CheckBox cb_fuse_high7;
        private System.Windows.Forms.CheckBox cb_fuse_ext0;
        private System.Windows.Forms.CheckBox cb_fuse_ext1;
        private System.Windows.Forms.CheckBox cb_fuse_ext2;
        private System.Windows.Forms.CheckBox cb_fuse_ext3;
        private System.Windows.Forms.CheckBox cb_fuse_ext4;
        private System.Windows.Forms.CheckBox cb_fuse_ext5;
        private System.Windows.Forms.CheckBox cb_fuse_ext6;
        private System.Windows.Forms.CheckBox cb_fuse_ext7;
        private System.Windows.Forms.Button buttonRefresh;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.Label label40;
        private System.Windows.Forms.Label label41;
        private System.Windows.Forms.Label label42;
        private System.Windows.Forms.Label label43;
        private System.Windows.Forms.Label label44;
        private System.Windows.Forms.Label label45;
        private System.Windows.Forms.Label label46;
        private System.Windows.Forms.TextBox textBoxFlashHexFilePath;
        private System.Windows.Forms.Label label48;
        private System.Windows.Forms.Button buttonSelectEEPROM;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.TextBox textBoxEepromHexFilePath;
        private System.Windows.Forms.TextBox textBoxUpStep;
        private System.Windows.Forms.TextBox textBoxMaxFolders;
        private System.Windows.Forms.Label label51;
        private System.Windows.Forms.Label label52;
        private System.Windows.Forms.TextBox textBoxDownStep;
        private System.Windows.Forms.Label label50;
        private System.Windows.Forms.ComboBox comboBoxFolderSelector;
        private System.Windows.Forms.Label label53;
        private System.Windows.Forms.Button buttonMakeList;
        private System.Windows.Forms.Label labelProgNum;
        private System.Windows.Forms.TextBox textBoxFileInfo2;
        private System.Windows.Forms.Label label54;
        private System.Windows.Forms.TextBox textBoxEepromStartAddressCalibBytes;
        private System.Windows.Forms.CheckBox checkBoxUseCalibBytes;
        private System.Windows.Forms.Label labelEepromStartAddressCalibBytes;
        private System.Windows.Forms.TextBox textBoxBootHexFile;
        private System.Windows.Forms.Button buttonSelectBoot;
        private System.Windows.Forms.TextBox textBoxBootHexFilePath;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label47;
        private System.Windows.Forms.TextBox textBoxEepromHexFile;
        private System.Windows.Forms.Label label49;
        private System.Windows.Forms.TextBox textBoxEncryptionCounter;
        private System.Windows.Forms.Label label55;
        private System.Windows.Forms.TextBox textBoxFileUniqueVale;
        private System.Windows.Forms.Label label56;
        private System.Windows.Forms.CheckBox checkBoxStopUsingUniqueFile;
        private System.Windows.Forms.Label label57;
        private System.Windows.Forms.Label label58;
        private System.Windows.Forms.ComboBox comboBoxIspClock;
        private System.Windows.Forms.CheckBox cbFwUpdate;
        private System.Windows.Forms.Button buttonOpenList;
        private System.Windows.Forms.Button buttonCopyFuses;
        private System.Windows.Forms.Button buttonWriteChipConfig;
        private System.Windows.Forms.Button buttonReadChipConfig;
    }
}

