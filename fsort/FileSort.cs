using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.RegularExpressions;
namespace fsort
{
	public class FileSort
	{
		/// <summary>
		/// 入力文字列から最初の [ ] で囲まれた文字列を取得します。
		/// 見つからない場合は空文字列を返します。
		/// </summary>
		static public string GetBracketContent(string src)
		{
			if (string.IsNullOrEmpty(src)) return "";
			var m = Regex.Match(src, @"\[(.*?)\]");
			return m.Success ? m.Groups[1].Value : "";
		}
		static public string FSort(string p)
		{
			string ret = "";
			if (File.Exists(p) == false)
			{
				ret = $"Error: File not found. {p}";
				return ret;
			}
			string dir = Path.GetDirectoryName(p);
			string fn = Path.GetFileName(p);
			string cnt = GetBracketContent(fn);
			string ext = Path.GetExtension(p).ToLower();
			if (ext != ".zip" && ext != ".rar")
			{
				ret = $"Skipped (not zip/rar). {fn}";
				return ret;
			}
			if (cnt == "")
			{
				ret = $"No brackets found. {fn}";
				return ret;
			}
			string newDir = Path.Combine(dir, cnt);
			if (Directory.Exists(newDir) == false)
			{
				Directory.CreateDirectory(newDir);
			}
			string newPath = Path.Combine(newDir, fn);
			if (File.Exists(newPath))
			{
				ret = $"File already exists in target directory. {newPath}";
				return ret;
			}
			try
			{
				File.Move(p, newPath);
				ret = $"Moved: {fn} -> {newDir}";
			}
			catch (Exception ex)
			{
				ret = $"Error moving file: {ex.Message}";
			}
			return ret;
		}

		static public string FsortDirSub(string targetPath)
		{
			string ret = "";
			if (Directory.Exists(targetPath) == false)
			{
				return ret;
			}
			var files = Directory.GetFiles(targetPath, "*");
			if (files.Length == 0)
			{
				return ret;
			}
			foreach (var file in files)
			{
				string d = Path.GetDirectoryName(file);
				string d2 = Path.GetDirectoryName(d);
				string fn = Path.GetFileName(file);
				try
				{
					File.Move(file, Path.Combine(d2, fn));
					ret += $"moved file:{fn}\n";
				}
				catch (Exception ex)
				{
					ret += $"Error moving file: {ex.Message}\n{fn}\n";
				}
			}
			files = Directory.GetFiles(targetPath, "*");
			if ( files.Length==0)
			{
				Directory.Delete(targetPath);
			}
			return ret;
		}
		static public string FsortDir(string targetPath)
		{
			string ret = "";
			if ( File.Exists(targetPath))
			{
				targetPath = Path.GetDirectoryName(targetPath);
			}
			if (Directory.Exists(targetPath) == false)
			{
				return ret;
			}
			var dirs = Directory.GetDirectories(targetPath, "*");
			if (dirs.Length == 0)
			{
				return ret;
			}
			foreach (var dir in dirs)
			{
				string s = FsortDirSub(dir);
				if (s!="")
				{
					ret += s;
				}
			}
			return ret;
		}
	}
}
