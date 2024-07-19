using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        byte[] roll={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        int RollCounter = 0;
        public Form1()
        {
            InitializeComponent();
        }
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

        public static string FromHexString(string hexString)
        {
            string str;
            var bytes = new byte[hexString.Length / 2];
            for (var i = 0; i < bytes.Length; i++)
            {
                bytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            }
            str=Encoding.UTF8.GetString(bytes); // returns: "Hello world" for "48656C6C6F20776F726C64"
            return str;
        }
        public byte[] FromHexStringToBytes(string hexString)
        {
            var bytes = new byte[hexString.Length / 2];
            for (var i = 0; i < bytes.Length; i++)
            {
                bytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            }
            return bytes;
        }
        private void button_Encript_Click(object sender, EventArgs e)
        {
            RollCounter++;
            var bytes = System.Text.Encoding.UTF8.GetBytes(textBox_Input.Text);
            textBox_InputHex.Text = BitConverter.ToString(bytes).Replace("-","");
            for (int i = 0; i < bytes.Length; ++i)
            {
                int b1, b2;
                bytes[i] ^= roll[((i + RollCounter) % roll.Length)];//168; // pseudo encrypt
                foreach (var item in textBox_Key.Text)
                {
                    bytes[i] ^= (byte)item;
                }
                b1 = ((int)bytes[i] << 7);
                b2 = ((int)bytes[i] >> 1);
                bytes[i] = (byte)(b1 | b2);
            }
            //textBox_Output.Text = System.Text.Encoding.ASCII.GetString(bytes);
            textBox_OutputHex.Text = BitConverter.ToString(bytes).Replace("-", ""); // 
            
            //for (int i = 0; i < bytes.Length; ++i) bytes[i] ^= 168; // pseudo decrypt
        }

        private void button_Decript_Click(object sender, EventArgs e)
        {
            int b1, b2;
            var bytes = FromHexStringToBytes(textBox_OutputHex.Text);
            for (int i = 0; i < bytes.Length; ++i)
            {                
                b1 = ((int)bytes[i] << 1);
                b2 = ((int)bytes[i] >> 7);
                bytes[i] = (byte)(b1 | b2);
                bytes[i] ^= roll[((i + RollCounter) % roll.Length)];//168; // pseudo encrypt
                foreach (var item in textBox_Key.Text)
                {
                    bytes[i] ^= (byte)item;
                }
            }
            textBox_InputHex.Text = BitConverter.ToString(bytes).Replace("-", "");
            textBox_Input.Text = FromHexString(textBox_InputHex.Text);
        }
    }
}
