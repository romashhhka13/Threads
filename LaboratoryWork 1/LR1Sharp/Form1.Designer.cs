namespace LR1_Ilichev
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            buttonStart = new Button();
            buttonStop = new Button();
            numericUpDown = new NumericUpDown();
            listBox = new ListBox();
            ((System.ComponentModel.ISupportInitialize)numericUpDown).BeginInit();
            SuspendLayout();
            // 
            // buttonStart
            // 
            buttonStart.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point, 204);
            buttonStart.Location = new Point(28, 157);
            buttonStart.Name = "buttonStart";
            buttonStart.Size = new Size(111, 34);
            buttonStart.TabIndex = 0;
            buttonStart.Text = "Start";
            buttonStart.UseVisualStyleBackColor = true;
            buttonStart.Click += buttonStart_Click;
            // 
            // buttonStop
            // 
            buttonStop.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point, 204);
            buttonStop.Location = new Point(28, 197);
            buttonStop.Name = "buttonStop";
            buttonStop.Size = new Size(111, 34);
            buttonStop.TabIndex = 0;
            buttonStop.Text = "Stop";
            buttonStop.UseVisualStyleBackColor = true;
            buttonStop.Click += buttonStop_Click;
            // 
            // numericUpDown
            // 
            numericUpDown.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point, 204);
            numericUpDown.Location = new Point(28, 118);
            numericUpDown.Name = "numericUpDown";
            numericUpDown.RightToLeft = RightToLeft.No;
            numericUpDown.Size = new Size(111, 33);
            numericUpDown.TabIndex = 1;
            numericUpDown.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // listBox
            // 
            listBox.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 204);
            listBox.FormattingEnabled = true;
            listBox.ItemHeight = 21;
            listBox.Location = new Point(163, 12);
            listBox.Name = "listBox";
            listBox.Size = new Size(498, 361);
            listBox.TabIndex = 2;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(666, 379);
            Controls.Add(listBox);
            Controls.Add(numericUpDown);
            Controls.Add(buttonStop);
            Controls.Add(buttonStart);
            Name = "Form1";
            Text = "Form1";
            FormClosing += Form1_FormClosing;
            ((System.ComponentModel.ISupportInitialize)numericUpDown).EndInit();
            ResumeLayout(false);
        }

        #endregion

        private Button buttonStart;
        private Button buttonStop;
        private NumericUpDown numericUpDown;
        private ListBox listBox;
    }
}
