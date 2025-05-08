namespace IlichevNamaspace
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
            listBox = new ListBox();
            inputBox = new TextBox();
            buttonSend = new Button();
            listBox1 = new ListBox();
            SuspendLayout();
            // 
            // listBox
            // 
            listBox.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 204);
            listBox.FormattingEnabled = true;
            listBox.ItemHeight = 21;
            listBox.Location = new Point(373, 10);
            listBox.Name = "listBox";
            listBox.Size = new Size(290, 319);
            listBox.TabIndex = 2;
            // 
            // inputBox
            // 
            inputBox.Font = new Font("Segoe UI", 14.25F);
            inputBox.Location = new Point(123, 337);
            inputBox.Name = "inputBox";
            inputBox.Size = new Size(540, 33);
            inputBox.TabIndex = 3;
            // 
            // buttonSend
            // 
            buttonSend.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point, 204);
            buttonSend.Location = new Point(6, 337);
            buttonSend.Name = "buttonSend";
            buttonSend.Size = new Size(111, 34);
            buttonSend.TabIndex = 0;
            buttonSend.Text = "Send";
            buttonSend.UseVisualStyleBackColor = true;
            buttonSend.Click += buttonSend_Click;
            // 
            // listBox1
            // 
            listBox1.Font = new Font("Segoe UI", 12F);
            listBox1.FormattingEnabled = true;
            listBox1.ItemHeight = 21;
            listBox1.Location = new Point(6, 10);
            listBox1.Name = "listBox1";
            listBox1.Size = new Size(361, 319);
            listBox1.TabIndex = 4;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(666, 379);
            Controls.Add(listBox1);
            Controls.Add(inputBox);
            Controls.Add(listBox);
            Controls.Add(buttonSend);
            Name = "Form1";
            Text = "Form1";
            FormClosing += Form1_FormClosing;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private ListBox listBox;
        private TextBox inputBox;
        private Button buttonSend;
        private ListBox listBox1;
    }
}
