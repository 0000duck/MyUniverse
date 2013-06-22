﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MyUniverseControl;

namespace MyUniverseControlTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        public MyUniverseControl.MyUniverseControl ctrl = new MyUniverseControl.MyUniverseControl();

        private void Play_Click(object sender, EventArgs e)
        {
            
            //MessageBox.Show("Clicked Play");
            ctrl.Play();
        }

        private void Pause_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("Clicked Pause");
            ctrl.Pause();
        }

        private void Test_Click(object sender, EventArgs e)
        {
            ctrl.Test();
        }

        private void RotateClockwiseTrue_Click(object sender, EventArgs e)
        {
            ctrl.SetRotateClockwise(true);
        }

        private void RotateClockwiseFalse_Click(object sender, EventArgs e)
        {
            ctrl.SetRotateClockwise(false);
        }

        private void RotatingTrue_Click(object sender, EventArgs e)
        {
            ctrl.SetRotating(true);
        }

        private void RotatingFalse_Click(object sender, EventArgs e)
        {
            ctrl.SetRotating(false);
        }

        private void GetRotateRate_Click(object sender, EventArgs e)
        {
            int i = ctrl.GetRotateRate();
            MessageBox.Show(i.ToString());
        }

        private void SetRotateRate_Click(object sender, EventArgs e)
        {
            ctrl.SetRotateRate(int.Parse(this.textBox1.Text));
        }

    }
}
