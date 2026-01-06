using System;
using System.Collections.Generic;
using System.Text;

namespace cmds
{
	public class HR
	{
		private List<string> m_DelWords = new List<string>()
		{
			"オリジナル",
			"(成年コミック)",
			"(一般コミック)",
			"(一般小説)",
			"(一般小説･SF)",
			"(一般小説･近代SF)",
			"(一般小説･古典SF)",
			"(商業誌)",
			"(同人誌)",
			"[雑誌]",
			"｢映画｣",
			"｢邦画｣",
			"｢アニメ映画｣",
			"成年コミック",
			"一般コミック",
			"アニメ",
			"商業誌",
			"同人誌",
			"アニメ映画",
			"雑誌",
			"映画",
			"邦画",
			"(C94)",
			"(C95)",
			"(C96)",
			"(C97)",
			"(C98)",
			"(C99)",
			"(C100)",
			"(C101)",
			"(C102)",
			"(C103)",
			"(C104)",
			"(C105)",
			"(C106)",
			"(C107)",
			"(C108)",
			"()",
			"【】",
			"「」",
			"[]",
			"「 」",
			"( )",
			"[ ]",
			"【 】"

		};
		public void LoadDelWords(string path)
		{
			if (File.Exists(path))
			{
				var lines = File.ReadAllLines(path);
				m_DelWords.Clear();
				foreach (var line in lines)
				{
					var trimmedLine = line.Trim();
					if (!string.IsNullOrEmpty(trimmedLine))
					{
						m_DelWords.Add(trimmedLine);
					}
				}
			}
		}
		public void SaveDelWords(string path)
		{
			File.WriteAllLines(path, m_DelWords);
		}
		public HR()
		{
		}
		private string HeaderCheck(string n)
		{
			int len = n.Length;
			n = n.Replace("〜", "～");
			n = n.Replace("（", "(");
			n = n.Replace("）", ")");
			n = n.Replace("｛", "{");
			n = n.Replace("｝", "}");
			n = n.Replace("!", "！");
			n = n.Replace("?", "？");
			n = n.Replace("*", "＊");
			n = n.Replace("$", "＄");
			n = n.Replace("%", "％");
			n = n.Replace("#", "＃");
			n = n.Replace("　", " ");

			do
			{
				foreach (var delword in m_DelWords)
				{
					n = n.Replace(delword, "");
				}
				n = n.Trim();
				if (len == n.Length)
				{
					break;
				}
				else
				{
					len = n.Length;
				}
			} while (len!=n.Length);
			return n;
		}
		private bool HeaderRenameSub(string filepath)
		{
			string filename = Path.GetFileNameWithoutExtension(filepath);
			string directory = Path.GetDirectoryName(filepath) ?? "";
			string ext = Path.GetExtension(filepath);

			string newfilepath = Path.Combine(directory, HeaderCheck(filename) + ext);

			if (newfilepath != filepath)
			{
				if (File.Exists(filepath))
				{
					if (File.Exists(newfilepath))
					{
						return false;
					}
					try
					{
						File.Move(filepath, newfilepath);
					}
					catch (Exception ex)
					{
						Console.WriteLine($"{filepath} Error: {ex.Message}");
						return false;
					}
				}
				else if (Directory.Exists(filepath))
				{
					if (Directory.Exists(newfilepath))
					{
						return false;
					}
					try
					{
						Directory.Move(filepath, newfilepath);
					}
					catch (Exception ex)
					{
						Console.WriteLine($"{filepath} Error: {ex.Message}");
						return false;
					}
				}
				return true;
			}
			else
			{
				Console.WriteLine($"{filepath} Error! no target!");
				return false;
			}
		}

		public bool HeaderRename(string targetDirectory)
		{
			if (!Directory.Exists(targetDirectory))
			{
				Console.WriteLine($"{targetDirectory} : 指定されたディレクトリが存在しません。");
				return false;
			}
			var entries = cmds.FileUtl.getImageFiles(targetDirectory);
			var entries2 = cmds.FileUtl.getDirs(targetDirectory);

			foreach (var entry in entries)
			{
				HeaderRenameSub(entry);
			}
			foreach (var entry in entries2)
			{
				HeaderRenameSub(entry);
			}

			return true;
		}
	}
}
