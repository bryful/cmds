using System;
using System.IO;
using cmds;

class Program
{
	public enum ExecMode
	{
		ERR,
		IMAGE2JPEG,
		HEADERRENAME,
		FSORT,
		REFSORT
	}
	static ExecMode mode = ExecMode.ERR;
	static void ShowHelp()
	{
		string str = "";
		switch (mode)
		{
			case ExecMode.IMAGE2JPEG:
				str += "Image2Jpeg - Jpegへ画像変換\r\n";
				str += "\t使い方: Image2Jpeg <DirectoryPath> ...\r\n";
				break;
			case ExecMode.HEADERRENAME:
				str += "HeaderRename - ファイル名の修正\r\n";
				str += "\t使い方: HeaderRename <DirectoryPath> ...\r\n";
				break;
			case ExecMode.FSORT:
				str += "FSort - ファイルをフォルダ分け\r\n";
				str += "\t使い方: FSort <DirectoryPath> ...\r\n";
				break;
			case ExecMode.REFSORT:
				str += "ReFSort - フォルダ分けをもとに戻す\r\n";
				str += "\t使い方: ReFSort <DirectoryPath> ...\r\n";
				break;
			case ExecMode.ERR:
			default:
				str += "cmds - Cmd\r\n";
				str += "\t使い方: cmds <functionKey> [params]...\r\n";
				break;
		}
		Console.WriteLine(str);
	}
	[STAThread]
	static void Main(string[] args)
	{
		string[] prms = new string[0];
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
				case "headerrename":
				case "hren":
					mode = ExecMode.HEADERRENAME;
					prms = args;
					break;
				case "fsort":
				case "fs":
					mode = ExecMode.FSORT;
					prms = args;
					break;
				case "refsort":
				case "rfs":
					mode = ExecMode.REFSORT;
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
				case "headerrename":
				case "hren":
					mode = ExecMode.HEADERRENAME;
					prms = args.Skip(1).ToArray();
					break;
				case "fsort":
					mode = ExecMode.FSORT;
					prms = args.Skip(1).ToArray();
					break;
				case "refsort":
				case "rfs":
					mode = ExecMode.REFSORT;
					prms = args.Skip(1).ToArray();
					break;
				default:
					mode = ExecMode.ERR;
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
					Console.WriteLine($"{webp.TargetDirectory} : [Image2jpeg] 指定されたディレクトリが存在しません。");
					return;
				}
				webp.DeleteSourceFile = true;
				bool result = webp.ConvertAllToJpeg();
				break;
			case ExecMode.HEADERRENAME:
				if (prms.Length <= 0)
				{
					ShowHelp();
					return;
				}
				HR hR = new HR();
				bool hrResult = hR.HeaderRename(prms[0]);
				break;
			case ExecMode.FSORT:
				if (prms.Length <= 0)
				{
					ShowHelp();
					return;
				}
				FSort fsort = new FSort(prms[0]);
				fsort.SortFiles();
				break;
			case ExecMode.REFSORT:
				if (prms.Length <= 0)
				{
					ShowHelp();
					return;
				}
				FSort fsort2 = new FSort(prms[0]);
				fsort2.ReSortFiles();
				break;
			case ExecMode.ERR:
			default:
				ShowHelp();
				break;
		}
	}

}