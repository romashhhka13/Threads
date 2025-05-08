using System.Diagnostics;
using System.Runtime.InteropServices;
using static IlichevNamaspace.Form1;
using System.Windows.Forms;

namespace IlichevNamaspace
{
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>

        static void Main()
        {
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.Run(new Form1());
        }
    }
}