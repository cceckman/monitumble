using System;
using System.ServiceProcess;

namespace com.cceckman.monitumble
{
    public static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        public static void Main()
        {
            Taskbar taskbar = new Taskbar();
            Console.WriteLine("Position: {0}, AlwaysOnTop: {1}; AutoHide: {2}; Bounds: {3}", taskbar.Position, taskbar.AlwaysOnTop, taskbar.AutoHide, taskbar.Bounds);

            taskbar.Position = TaskbarPosition.Right;

            Console.ReadLine();
            
            //ServiceBase[] ServicesToRun;
            //ServicesToRun = new ServiceBase[] 
            //{ 
            //    new HostService() 
            //};
            //ServiceBase.Run(ServicesToRun);
        }
    }
}
