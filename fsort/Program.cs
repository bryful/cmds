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
namespace fsort

{
	internal class Program
	{
		static void Usage()
		{
			Console.WriteLine("Usage: fsort <path>");
		}

		static void Main(string[] args)
		{

			string targetPath = "";
			if (args.Length < 1)
			{
				targetPath = Directory.GetCurrentDirectory();
			}
			else if (args.Length == 1)
			{
				targetPath = args[0];
			} else if (args.Length > 1)
			{
				string op = args[0].ToLower();
				if (op == "-r" || op == "-rev" || op == "/r" || op=="/rev")
				{
					targetPath = args[1];
					string res = FileSort.FsortDir(targetPath);
					Console.WriteLine(res);
				} else
				{
					Usage();
				}
				return;

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
				string res = FileSort.FSort(file);
				Console.WriteLine(res);
			}
		}
	}
}
