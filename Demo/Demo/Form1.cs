using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Demo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            WpfApplication2.MainWindow wpfwindow = new WpfApplication2.MainWindow();
            Console.WriteLine("I am here!");
            wpfwindow.ShowDialog();
        }
    }
}
