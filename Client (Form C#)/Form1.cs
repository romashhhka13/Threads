using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net.Sockets;
using System.Text;


namespace IlichevNamaspace
{
    public partial class Form1 : Form
    {
        bool flag = true;

        public void ProcessMessages()
        {
            while (flag)
            {
                var m = Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_GETDATA);
                switch (m.header.type)
                {
                    case MessageTypes.MT_INIT:
                        listBox.Items.Add($"{m.header.from}");
                        break;

                    case MessageTypes.MT_EXIT:
                        listBox.Items.Remove($"{m.header.from}");
                        break;

                    case MessageTypes.MT_DATA:
                        listBox1.Items.Add($"{m.header.from}: {m.data}");
                        break;
                    default:
                        Thread.Sleep(100);
                        break;
                }
            }
        }


        public Form1()
        {

            InitializeComponent();

            var m = Message.send(MessageRecipients.MR_BROKER, MessageTypes.MT_INIT);
            if (m.header.type == MessageTypes.MT_CONFIRM)
            {
                listBox.Items.Add("Все клиенты");
                listBox1.Items.Add("Получаемые сообщения");
                this.Text = $"{m.header.to}";

                Thread t = new Thread(ProcessMessages);
                t.Start();
            }
        }


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            flag = false;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {

            string s = inputBox.Text;
            if (listBox.SelectedIndex == -1)
            {
                return;
            }
            else if (listBox.SelectedIndex == 0)
            {
                
                if (s is not null)
                {
                    var m = Message.send(MessageRecipients.MR_ALL, MessageTypes.MT_DATA, s);
                    if (m.header.type == MessageTypes.MT_CONFIRM)
                    {
                        inputBox.Clear();
                    }

                }
            }
            else
            {
                if (s is not null)
                {
                    if (int.TryParse(listBox.SelectedItem.ToString(), out int clientId))
                    {
                        var m = Message.send((MessageRecipients)clientId, MessageTypes.MT_DATA, s);
                        if (m.header.type == MessageTypes.MT_CONFIRM)
                        {
                            inputBox.Clear();
                        }
                    }
                }
            }
        }
    }
}
