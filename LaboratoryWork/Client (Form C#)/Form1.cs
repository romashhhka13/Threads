using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net.Sockets;
using System.Text;


namespace IlichevNamaspace
{
    public partial class Form1 : Form
    {
        public enum MessageTypes : int
        {
            MT_INIT,
            MT_EXIT,
            MT_GETDATA,
            MT_DATA,
            MT_NODATA,
            MT_CONFIRM
        };

        public enum MessageRecipients : int
        {
            MR_BROKER = 10,
            MR_ALL = 50,
            MR_USER = 100
        };


        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public struct MessageHeader
        {
            [MarshalAs(UnmanagedType.I4)]
            public MessageRecipients to;
            [MarshalAs(UnmanagedType.I4)]
            public MessageRecipients from;
            [MarshalAs(UnmanagedType.I4)]
            public MessageTypes type;
            [MarshalAs(UnmanagedType.I4)]
            public int size;
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 4)]
        public struct MessageStruct
        {
            public MessageHeader header;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string data;
        }


        [DllImport("DLLIlichev.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern MessageStruct sendMessageC(int to, int type = (int)MessageTypes.MT_DATA, string data = "");

        bool flag = true;



        public void ProcessMessages()
        {
            while (flag)
            {
                var mess = sendMessageC((int)MessageRecipients.MR_BROKER, (int)MessageTypes.MT_GETDATA);
                switch (mess.header.type)
                {
                    case MessageTypes.MT_INIT:
                        listBox.Items.Add($"{mess.header.from}");
                        break;

                    case MessageTypes.MT_EXIT:
                        listBox.Items.Remove($"{mess.header.from}");
                        break;

                    case MessageTypes.MT_DATA:
                        listBox1.Items.Add($"{mess.header.from}: {mess.data}");
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
            var mess = sendMessageC((int)MessageRecipients.MR_BROKER, (int)MessageTypes.MT_INIT);
            if (mess.header.type == MessageTypes.MT_CONFIRM)
            {
                listBox.Items.Add("Все клиенты");
                listBox1.Items.Add("Получаемые сообщения");
                this.Text = $"{mess.header.to}";

                Thread t = new Thread(ProcessMessages);
                t.Start();
            }

        }


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            /*var mess = sendMessageC((int)MessageRecipients.MR_BROKER, (int)MessageTypes.MT_EXIT);
            if (mess.header.type == MessageTypes.MT_CONFIRM)
            {
                flag = false;
            }*/
            flag = false;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {

            if (listBox.SelectedIndex == -1)
            {
                return;
            }
            else if (listBox.SelectedIndex == 0)
            {
                var m = sendMessageC((int)MessageRecipients.MR_ALL, (int)MessageTypes.MT_DATA, inputBox.Text);
                if (m.header.type == MessageTypes.MT_CONFIRM)
                {
                    inputBox.Clear();
                }
            }
            else
            {
                if (int.TryParse(listBox.SelectedItem.ToString(), out int clientId))
                {
                    var m = sendMessageC(clientId, (int)MessageTypes.MT_DATA, inputBox.Text);
                    if (m.header.type == MessageTypes.MT_CONFIRM)
                    {
                        inputBox.Clear();
                    }
                }
            }
        }
    }
}
