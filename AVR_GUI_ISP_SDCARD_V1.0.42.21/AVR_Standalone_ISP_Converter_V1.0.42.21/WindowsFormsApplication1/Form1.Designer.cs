namespace WindowsFormsApplication1
{
    partial class Form1
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
            this.button_Encript = new System.Windows.Forms.Button();
            this.button_Decript = new System.Windows.Forms.Button();
            this.textBox_InputHex = new System.Windows.Forms.TextBox();
            this.textBox_OutputHex = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBox_Input
            // 
            this.textBox_Input.Location = new System.Drawing.Point(123, 86);
            this.textBox_Input.Name = "textBox_Input";
            this.textBox_Input.Size = new System.Drawing.Size(100, 20);
            this.textBox_Input.TabIndex = 0;
            // 
            // textBox_Key
            // 
            this.textBox_Key.Location = new System.Drawing.Point(123, 148);
            this.textBox_Key.Name = "textBox_Key";
            this.textBox_Key.Size = new System.Drawing.Size(100, 20);
            this.textBox_Key.TabIndex = 1;
            // 
            // textBox_Output
            // 
            this.textBox_Output.Location = new System.Drawing.Point(123, 214);
            this.textBox_Output.Name = "textBox_Output";
            this.textBox_Output.Size = new System.Drawing.Size(100, 20);
            this.textBox_Output.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(120, 70);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Hex Input";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(120, 132);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(25, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Key";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(120, 198);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Hex Output";
            // 
            // button_Encript
            // 
            this.button_Encript.Location = new System.Drawing.Point(250, 86);
            this.button_Encript.Name = "button_Encript";
            this.button_Encript.Size = new System.Drawing.Size(75, 23);
            this.button_Encript.TabIndex = 6;
            this.button_Encript.Text = "Encript";
            this.button_Encript.UseVisualStyleBackColor = true;
            this.button_Encript.Click += new System.EventHandler(this.button_Encript_Click);
            // 
            // button_Decript
            // 
            this.button_Decript.Location = new System.Drawing.Point(250, 214);
            this.button_Decript.Name = "button_Decript";
            this.button_Decript.Size = new System.Drawing.Size(75, 23);
            this.button_Decript.TabIndex = 7;
            this.button_Decript.Text = "Decript";
            this.button_Decript.UseVisualStyleBackColor = true;
            this.button_Decript.Click += new System.EventHandler(this.button_Decript_Click);
            // 
            // textBox_InputHex
            // 
            this.textBox_InputHex.Location = new System.Drawing.Point(123, 109);
            this.textBox_InputHex.Name = "textBox_InputHex";
            this.textBox_InputHex.Size = new System.Drawing.Size(100, 20);
            this.textBox_InputHex.TabIndex = 0;
            // 
            // textBox_OutputHex
            // 
            this.textBox_OutputHex.Location = new System.Drawing.Point(123, 240);
            this.textBox_OutputHex.Name = "textBox_OutputHex";
            this.textBox_OutputHex.Size = new System.Drawing.Size(100, 20);
            this.textBox_OutputHex.TabIndex = 2;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(638, 356);
            this.Controls.Add(this.button_Decript);
            this.Controls.Add(this.button_Encript);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_OutputHex);
            this.Controls.Add(this.textBox_Output);
            this.Controls.Add(this.textBox_Key);
            this.Controls.Add(this.textBox_InputHex);
            this.Controls.Add(this.textBox_Input);
            this.Name = "Form1";
            this.Text = "Form1";
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
        private System.Windows.Forms.Button button_Encript;
        private System.Windows.Forms.Button button_Decript;
        private System.Windows.Forms.TextBox textBox_InputHex;
        private System.Windows.Forms.TextBox textBox_OutputHex;
    }
}

