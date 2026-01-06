using System;
using System.IO;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats.Png;
using SixLabors.ImageSharp.Formats.Webp;

public class ff
{
	public string Name { get; set; } = "";
	public int Number { get; set; } = -1;
	public string Node { get; set; } = "";
	public string Ext { get; set; } = "";
	public string  NumberStr { get; set; }= "";
	private string GetNumber(string s)
    {
        string ret = "";
        if (s== "") return ret;
 
		for (int i = s.Length - 1; i >= 0; i--)
        {
            if ((s[i] < '0') || (s[i] > '9'))
            {
                ret = s.Substring(i + 1);
                break;
			}
		}
        return ret;
	}
    public string AddNumber(int n)
    {
        if (n <= 0) return "";
        int len = NumberStr.Length;
        int num = Number + n;
        string numstr = num.ToString();
        while (numstr.Length < len)
        {
            numstr = "0" + numstr;
        }
        return Node + numstr + Ext;
    }
	public ff(string p)
    {
        Name = Path.GetFileName(p);
        string ss = Path.GetFileNameWithoutExtension(p);
        NumberStr = GetNumber(ss);
        Ext = Path.GetExtension(p);
		if (NumberStr != "")
        {
            int _num=-1;
			if (int.TryParse(NumberStr, out _num))
            {
                Number = _num;
            }
            Node = ss.Substring(0, ss.Length - NumberStr.Length);
		}
        else
        {
            Number = -1;
		}

	}
}
class Program
{
    public enum SaveFormat
    {
		Webp,
        Jpeg,
	   Png
 	}
    public enum ExeMode
    {
        None,
        ConvertWebp,
        RenameImgiFiles,
        NumberCheck,
		ToImgi
	}
	public static void ConvertWebpFile(SaveFormat op, bool delf = false, string webpFilePath = "", string pngFilePath = "")
    {
		if (File.Exists(webpFilePath)==false)
		{
            return;
		}
		string ext = ".jpeg";
        if (op == SaveFormat.Png)
        {
            ext = ".png";
        }
        if (pngFilePath == "")
        {
            pngFilePath = Path.Combine(
                Path.GetDirectoryName(webpFilePath)!,
                Path.GetFileNameWithoutExtension(webpFilePath) + ext
            );
        }
        pngFilePath = RenameImgiSub(pngFilePath);
		string d = Path.GetDirectoryName(pngFilePath)!;
        string f = Path.GetFileNameWithoutExtension(pngFilePath);
        while (File.Exists(pngFilePath))
        {
            f += "_";
            pngFilePath = Path.Combine(
                Path.GetDirectoryName(d)!,
                Path.GetFileNameWithoutExtension(f) + ext
            );
        }
        try
        {
            using (Image image = Image.Load(webpFilePath))
            {
                switch (op)
                {
                    case SaveFormat.Jpeg:
                        image.Save(pngFilePath, new SixLabors.ImageSharp.Formats.Jpeg.JpegEncoder());
                        break;
                    case SaveFormat.Png:
                        image.Save(pngFilePath, new PngEncoder());
                        break;
                }
                Console.WriteLine($"変換完了: {webpFilePath} -> {pngFilePath}");
				if (File.Exists(pngFilePath))
                {
                    if (delf)
                    {
                        File.Delete(webpFilePath);
						Console.WriteLine($"削除: {webpFilePath}");
					}
				}
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"変換失敗: {webpFilePath} -> {pngFilePath}");
            Console.WriteLine(ex.Message);
            return;
        }
    }
    public static void ConvertAllWebpInDirectory(SaveFormat op,bool delf=false, string path="")
    {
        string dir = "";
        string pat = "*.webp";
        if (path == "") 
        {
            dir = Directory.GetCurrentDirectory();
		}
		else if (Directory.Exists(path))
        {
            dir = path;
		}
		else if (File.Exists(path))
        {
            ConvertWebpFile(op,delf,path);
            return;
		}
		else if (path.IndexOf("*")>=0)
        {
			dir = Path.GetDirectoryName(path)!;
			pat = Path.GetFileNameWithoutExtension(path);
        }
        else
        {
                       Console.WriteLine("指定されたパスが存在しません。");
            return;
        }
		var webpFiles = Directory.GetFiles(dir, pat, SearchOption.TopDirectoryOnly);
        foreach (var webpFile in webpFiles)
        {
            ConvertWebpFile(op,delf,webpFile);
        }
    }
	public static string RenameImgiSub(string path)
    {
		string d = Path.GetDirectoryName(path)!;
		string f = Path.GetFileNameWithoutExtension(path);
		string e = Path.GetExtension(path);

		string[] fa = f.Split('_');
		if (fa.Length < 3) return path;
		if ((fa[0] != "imgi") || (fa[1] != fa[2])) return path;
		int n = 0;
		if (!int.TryParse(fa[1], out n)) return path;
		string newf = $"img{n:D3}";
		string newpath = Path.Combine(d, newf + e);
        return newpath;
	}
	public static void RenameImgi(string path)
    {
        if (!File.Exists(path)) return;
        string d = Path.GetDirectoryName(path)!;
        string f = Path.GetFileNameWithoutExtension(path);
        string e = Path.GetExtension(path);

        string [] fa = f.Split('_');
        if (fa.Length<3) return;
        if ((fa[0] !="imgi")||(fa[1]!= fa[2])) return;
        int n = 0;
        if (!int.TryParse(fa[1], out n)) return;
        string newf = $"img{n:D3}";
        string newpath = Path.Combine(d, newf + e);
        while (File.Exists(newpath))
        {
            newf +="_";
            newpath = Path.Combine(d, newf + e);
		}
        File.Move(path, newpath);
		Console.WriteLine($"リネーム完了: {Path.GetFileName(path)} >> {Path.GetFileName(newpath)}");

	}
	public static void RenameAllImgiInDirectory(string path="")
    {
        string dir = "";
        if (path == "")
        {
            dir = Directory.GetCurrentDirectory();
        }
        else if (Directory.Exists(path))
        {
            dir = path;
        }
        else if (File.Exists(path))
        {
            RenameImgi(path);
            return;
		}
		else
        {
            Console.WriteLine("指定されたパスが存在しません。");
            return;
        }
        Console.WriteLine($"ディレクトリ: {dir}");
		var files = Directory.GetFiles(dir, "*.*", SearchOption.TopDirectoryOnly);
        foreach (var file in files)
        {
            RenameImgi(file);
        }
	}
    public static void NumberCheckInDirectory(string path = "")
    {
        string dir = "";
        if (path == "")
        {
            dir = Directory.GetCurrentDirectory();
        }
        else if (Directory.Exists(path))
        {
            dir = path;
        }
        else
        {
            Console.WriteLine("指定されたパスが存在しません。");
            return;
        }
        var files = Directory.GetFiles(dir, "*.*", SearchOption.TopDirectoryOnly);
        var ffList = new System.Collections.Generic.List<ff>();
        foreach (var file in files)
        {
            ff f = new ff(file);
            if (f.Number >= 0)
            {
                ffList.Add(f);
            }
        }
        if (ffList.Count == 0)
        {
            Console.WriteLine("番号付きファイルが見つかりません。");
            return;
        }
        ffList.Sort((a, b) => a.Number.CompareTo(b.Number));
        for (int i = 1; i < ffList.Count; i++)
        {
            if (ffList[i - 1].Number == ffList[i].Number)
            {
                Console.WriteLine($"番号重複:　{ffList[i].Name})");
            }
            else if (ffList[i - 1].Number + 1 != ffList[i].Number)
            {
                int cnt = ffList[i].Number - ffList[i - 1].Number - 1;
                ff nf = new ff(ffList[i - 1].Name);
                for (int j = 1; j <= cnt; j++)
                {
                    Console.WriteLine($"番号欠番: {nf.AddNumber(j)}");
                }
            }
        }
    }
    public static void ToImgi(string p)
    {
        if (p == "") return;
        int i = 0;
        string d = Path.GetDirectoryName(p)!;
        string f = Path.GetFileNameWithoutExtension(p);
        string e = Path.GetExtension(p);

        if (int.TryParse(f, out i) == false) return;
        string newf = Path.Combine(d, $"imgi_{f}_{f}{e}");

        if (File.Exists(newf))
        {
            Console.WriteLine($"すでに存在: {Path.GetFileName(newf)}");
            return;
        }
		File.Move(p, newf);
        Console.WriteLine($"リネーム完了: {Path.GetFileName(p)} >> {Path.GetFileName(newf)}");
	}
	
	static void ShowUsage()
    {
        Console.WriteLine("[webp.exe] 使い方: webp  <command> [option]");
        Console.WriteLine("コマンド");
		Console.WriteLine("\tpng\twebpをpngへ変換");
	}
	static void Main(string[] args)
    {
        if (args.Length ==0)
        {
            ShowUsage();
            return;
		}   

        ExeMode em = ExeMode.None;
		bool delflag= false;

        string cmd = args[0].ToLower();
        SaveFormat sf = SaveFormat.Webp;
		if ((cmd =="jpeg")||(cmd =="jpg"))
        {
            delflag = false;
			em = ExeMode.ConvertWebp;
			sf = SaveFormat.Jpeg;
        }
        else if ((cmd == "jpegd") || (cmd == "jpgd"))
        {
            delflag = true;
			em = ExeMode.ConvertWebp;
			sf = SaveFormat.Jpeg;
		}
		else if (cmd == "png")
		{
			delflag = false;
			em = ExeMode.ConvertWebp;
			sf = SaveFormat.Png;
		}
		else if (cmd == "pngd")
		{
			delflag = true;
			em = ExeMode.ConvertWebp;
			sf = SaveFormat.Png;
		}
		else if ((cmd == "rimgi")|| (cmd == "imgi")|| (cmd == "img"))
		{
			em = ExeMode.RenameImgiFiles;
		}
		else if ((cmd == "num") || (cmd == "number") || (cmd == "numcheck") || (cmd == "numbercheck"))
		{
			em = ExeMode.NumberCheck;
		}
		else if ((cmd == "toimgi") || (cmd == "toimg") || (cmd == "toi"))
		{
			em = ExeMode.ToImgi;
		}
		else
        {
            ShowUsage();
            return;
		}
        switch (em)
        {
            case ExeMode.ConvertWebp:
                string pp = ".\\";
                if (args.Length >= 2)
                {
                    pp = args[1];
                }
                ConvertAllWebpInDirectory(sf, delflag, pp);
                break;
            case ExeMode.RenameImgiFiles:
                if (args.Length == 1)
                {
                    RenameAllImgiInDirectory();
                }
                else
                {
                    for (int i = 1; i < args.Length; i++)
                    {
                        RenameAllImgiInDirectory(args[i]);
                    }
                }
                break;
            case ExeMode.NumberCheck:
                if (args.Length == 1)
                {
                    NumberCheckInDirectory();
                }
                else
                {
                    NumberCheckInDirectory(args[1]);
                }
				Console.ReadLine();
				break;
            case ExeMode.ToImgi:
                if (args.Length == 1)
                {
                    Console.WriteLine("ファイル名を指定してください。");
                }
                else
                {
                    for (int i = 1; i < args.Length; i++)
                    {
                        ToImgi(args[i]);
                    }
                }
                Console.ReadLine();
                break;
            case ExeMode.None:
            default:
                ShowUsage();
                break;
        
		}
    }
}