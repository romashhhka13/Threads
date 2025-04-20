using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net.Sockets;

namespace IlichevNamaspace
{
    public partial class Form1 : Form
    {
        int threads_num = 0;
        public enum MessageType : int
        {
            MT_INIT,
            MT_EXIT,
            MT_SEND,
            MT_CONFIRM,
            MT_STOP,
            MT_START
        };


        [StructLayout(LayoutKind.Sequential)]
        public struct MessageHeader
        {
            [MarshalAs(UnmanagedType.I4)]
            public MessageType type; 
            [MarshalAs(UnmanagedType.I4)]
            public int addr;
            [MarshalAs(UnmanagedType.I4)]
            public int size;
        };

        [DllImport("DLLIlichev.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern MessageHeader send_client(MessageType type, int addr, string data="");

        [DllImport("DLLIlichev.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern int receive_threads_num();

        public Form1()
        {
            InitializeComponent();
            var m = send_client(MessageType.MT_INIT, 0);
            threads_num = receive_threads_num();
            if (m.type == MessageType.MT_CONFIRM)
            {
                listBox.Items.Add("Главный поток - main()");
                listBox.Items.Add("Все потоки");
                listBox.SetSelected(0, true);

                for (int i = 0; i < threads_num; i++)
                {
                    listBox.Items.Add($"Рабочий поток #{i}");
                }

            }

        }

        private void update_threads_num(int n)
        {
            if (n > threads_num)
            {
                for (int i = threads_num; i < n; i++)
                {
                    listBox.Items.Add($"Рабочий поток #{i}");
                }
            }
            else if (n < threads_num)
            {
                for (int i = n; i < threads_num; i++)
                {
                    listBox.Items.RemoveAt(listBox.Items.Count - 1);
                }
            }
            threads_num = n;
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            int value = (int)numericUpDown.Value;
            int prev_num = threads_num;
            for (int i = prev_num; i < value + prev_num; i++)
            {
                var m = send_client(MessageType.MT_START, 0);
                int num = receive_threads_num();
                if (m.type == MessageType.MT_CONFIRM)
                    update_threads_num(num);

            }
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            var m = send_client(MessageType.MT_STOP, 0);
            int num = receive_threads_num();
            if (m.type == MessageType.MT_CONFIRM)
                update_threads_num(num);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            var m = send_client(MessageType.MT_EXIT, 0);
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            int addr = listBox.SelectedIndex - 2;
            string mess = inputBox.Text;
            if (addr == -3)
                return;

            var m = send_client(MessageType.MT_SEND, addr, mess);
            int num = receive_threads_num();
            if (m.type == MessageType.MT_CONFIRM)
                inputBox.Text = string.Empty;

        }
    }
}
