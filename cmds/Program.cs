using System;
using System.IO;
using cmds;

class Program
{
	public enum ExecMode
	{
		ERR,
		IMAGE2JPEG,
	}
	static ExecMode mode = ExecMode.ERR;
	static void ShowHelp()
	{
		string str = "";
		switch (mode)
		{
			case ExecMode.IMAGE2JPEG:
				break;
			case ExecMode.ERR:
			default:
				str += "cmds - Cmd\r\n";
				str += "\t使い方: cmds <mode> ...\r\n";
				break;
		}
		Console.WriteLine(str);
	}
	[STAThread]
	static void Main(string[] args)
	{
		string[] prms = new string[0];
		foreach (string arg in args)
		{
			Console.WriteLine(arg);
		}
		if (args.Length < 1)
		{
			ShowHelp();
			return;
		}
		string? processPath = Environment.ProcessPath;
		string exename = processPath != null
			? Path.GetFileNameWithoutExtension(processPath).ToLower()
			: string.Empty;
		// モード判定
		if (exename != "cmds")
		{
			// 実行ファイル名からモードを判定
			switch (exename)
			{
				case "image2jpeg":
				case "tojpeg":
					mode = ExecMode.IMAGE2JPEG;
					prms = args;
					break;
				default:
					mode = ExecMode.ERR;
					break;
			}
		}
		else
		{
			// コマンドライン引数からモードを判定
			string modeStr = args[0].ToLower();
			switch (modeStr)
			{
				case "image2jpeg":
				case "tojpeg":
					mode = ExecMode.IMAGE2JPEG;
					prms = args.Skip(1).ToArray();
					break;
				default:
					break;
			}
		}

		// 実行
		switch (mode)
		{
			case ExecMode.IMAGE2JPEG:
				if (prms.Length<=0)
				{
					ShowHelp();
					return;
				}
				Webp webp = new Webp(prms[0]);
				if (!webp.ExistsDIrectory)
				{
					Console.WriteLine("[Image2jpeg] 指定されたディレクトリが存在しません。");
					return;
				}
				bool result = webp.ConvertAllToJpeg();
				break;
			case ExecMode.ERR:
			default:
				ShowHelp();
				break;
		}
	}

}