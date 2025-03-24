using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net.Sockets;

namespace LR1_Ilichev
{
    public partial class Form1 : Form
    {

        int threads_num;
        Process childProcess = null;
        System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StopEvent");
        System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        System.Threading.EventWaitHandle exitEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ExitEvent");
        System.Threading.Mutex mutex = new Mutex(false, "hMutex");


        public Form1()
        {
            InitializeComponent();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                childProcess = Process.Start("C++Ilichev.exe");
                childProcess.EnableRaisingEvents = true;

                childProcess.Exited += (sender, e) =>
                {
                    this.Invoke((Action)(() => listBox.Items.Clear()));

                };
                confirmEvent.WaitOne();
                listBox.Items.Add("Главный поток - main()");
                listBox.Items.Add("Все потоки");
                listBox.SetSelected(1, true);
                threads_num = 0;
            }
            else
            {
                int value = (int)numericUpDown.Value;
                for (int i = 0; i < value; i++)
                {
                    startEvent.Set();
                    confirmEvent.WaitOne();
                    listBox.Items.Add($"Рабочий поток #{threads_num++}");

                }
            }
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            if (!(childProcess == null || childProcess.HasExited))
            {
                stopEvent.Set();
                confirmEvent.WaitOne();
                listBox.Items.RemoveAt(1 + threads_num--);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (childProcess != null && !childProcess.HasExited)
            {
                exitEvent.Set();
                confirmEvent.WaitOne();
            }
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            int addr = listBox.SelectedIndex;
            if (addr == -1)
                return;
            string mess = inputBox.Text;
            mutex.WaitOne();
            Program.mapsend(addr - 2, mess);
            mutex.ReleaseMutex();
            confirmEvent.WaitOne();
        }
    }
}
