using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Reflection;
using System.Diagnostics;
namespace renh
{
	internal class Program
	{
		static void Usage()
		{
			Console.WriteLine("Usage: renh <path>");
		}

		static void Main(string[] args)
		{
			HeaderRename headerRename = new HeaderRename();

			string exePath = System.Reflection.Assembly.GetEntryAssembly().Location;
			if (exePath == null || exePath == "")
			{
				exePath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
			}
			Console.WriteLine("exePath=" + exePath);
			string lstf = Path.ChangeExtension(exePath, ".lst");
			Console.WriteLine("lstf=" + lstf);

			if (headerRename.LoadWords(lstf)==false)
			{
				headerRename.SaveWords(lstf);	
			}


			string targetPath = "";
			if (args.Length < 1)
			{
				targetPath = Directory.GetCurrentDirectory();
			}
			else
			{
				targetPath = args[0];
			}
			if (!Directory.Exists(targetPath))
			{
				Console.WriteLine("Error: Path not found.");
				Usage();
				return;
			}
			var files = Directory.GetFiles(targetPath, "*");
			if (files.Length == 0)
			{
				Console.WriteLine("No files found.");
				return;
			}
			foreach (var file in files)
			{
				string e = Path.GetExtension(file).ToLower();
				if( e==".zip" || e==".rar" )
				{
					string ret = headerRename.Rename(file);
					if (ret != "")
					{
						Console.WriteLine(ret);
					}
				}
			}
		}
	}
}
