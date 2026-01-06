using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Data;
using System.Text.RegularExpressions;

namespace renh
{
	public class HeaderRename
	{
		private List<string> delWords = new List<string>();
		public HeaderRename()
		{
			Init();
		}
		public void Init()
		{
			delWords.Clear();
			delWords.Add("(成年コミック)");
			delWords.Add("成年コミック");
			delWords.Add("(一般コミック)");
			delWords.Add("(一般小説)");
			delWords.Add("(一般小説･SF)");
			delWords.Add("(一般小説･近代SF)");
			delWords.Add("(一般小説･古典SF)");
			delWords.Add("(商業誌)");
			delWords.Add("(同人誌)");
			delWords.Add("[雑誌]");
			delWords.Add("()");
			delWords.Add("( )");
			delWords.Add("[]");
			delWords.Add("[ ]");
		}
		public void AddDelWord(string word)
		{
			word = word.Trim();
			if (word.Length == 0)
			{
				return;
			}	
			if (!delWords.Contains(word))
			{
				delWords.Add(word);
			}
		}
		public bool LoadWords(string path)
		{
			delWords.Clear();
			if (!File.Exists(path))
			{
				Init();
				return false;
			}
			var lines = File.ReadAllLines(path);
			if (lines.Length == 0)
			{
				Init();
				return false;
			}
			foreach (var line in lines)
			{
				AddDelWord(line);
			}
			return true;
		}
		public bool SaveWords(string path)
		{
			try
			{
				File.WriteAllLines(path, delWords);
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Error: {ex.Message}");
				return false;
			}
			return true;
		}

		public string DelSP(string src)
		{
			src = src.Trim();
			if ( src.Length<=1)
			{
				return src;
			}

			string res =src.Substring(0,1);
			for (int i = 1; i < src.Length; i++)
			{
				if (src[i] != ' ')
				{
					res += src.Substring(i,1);
				}
				else {
					if (src[i - 1] != ' ')
					{
						res += src.Substring(i, 1);
					}
				}
			}
			return res;
		}

		public string ZenHan(string src)
		{
			string res = "";
			if (src == null || src.Length == 0)
			{
				return res;
			}
			for (int i = 0; i < src.Length; i++)
			{
				char c = src[i];
				if (c == 'Ａ')
				{
					res += 'A';
				}
				else if (c == 'Ｂ')
				{
					res += 'B';
				}
				else if (c == 'Ｃ')
				{
					res += 'C';
				}
				else if (c == 'Ｄ')
				{
					res += 'D';
				}
				else if (c == 'Ｅ')
				{
					res += 'E';
				}
				else if (c == 'Ｆ')
				{
					res += 'F';
				}
				else if (c == 'Ｇ')
				{
					res += 'G';
				}
				else if (c == 'Ｈ')
				{
					res += 'H';
				}
				else if (c == 'Ｉ')
				{
					res += 'I';
				}
				else if (c == 'Ｊ')
				{
					res += 'J';
				}
				else if (c == 'Ｋ')
				{
					res += 'K';
				}
				else if (c == 'Ｌ')
				{
					res += 'L';
				}
				else if (c == 'Ｍ')
				{
					res += 'M';
				}
				else if (c == 'Ｎ')
				{
					res += 'N';
				}
				else if (c == 'Ｏ')
				{
					res += 'O';
				}
				else if (c == 'Ｐ')
				{
					res += 'P';
				}
				else if (c == 'Ｑ')
				{
					res += 'Q';
				}
				else if (c == 'Ｒ')
				{
					res += 'R';
				}
				else if (c == 'Ｓ')
				{
					res += 'S';
				}
				else if (c == 'Ｔ')
				{
					res += 'T';
				}
				else if (c == 'Ｕ')
				{
					res += 'U';
				}
				else if (c == 'Ｖ')
				{
					res += 'V';
				}
				else if (c == 'Ｗ')
				{
					res += 'W';
				}
				else if (c == 'Ｘ')
				{
					res += 'X';
				}
				else if (c == 'Ｙ')
				{
					res += 'Y';
				}
				else if (c == 'Ｙ')
				{
					res += 'Y';
				}
				else if (c == 'Ｚ')
				{
					res += 'Z';
				}
				else if (c == 'ａ')
				{
					res += 'a';
				}
				else if (c == 'ｂ')
				{
					res += 'b';
				}
				else if (c == 'ｃ')
				{
					res += 'c';
				}
				else if (c == 'ｄ')
				{
					res += 'd';
				}
				else if (c == 'ｅ')
				{
					res += 'e';
				}
				else if (c == 'ｆ')
				{
					res += 'f';
				}
				else if (c == 'ｇ')
				{
					res += 'g';
				}
				else if (c == 'ｈ')
				{
					res += 'h';
				}
				else if (c == 'ｉ')
				{
					res += 'i';
				}
				else if (c == 'ｊ')
				{
					res += 'j';
				}
				else if (c == 'ｋ')
				{
					res += 'k';
				}
				else if (c == 'ｌ')
				{
					res += 'l';
				}
				else if (c == 'ｍ')
				{
					res += 'm';
				}
				else if (c == 'ｎ')
				{
					res += 'n';
				}
				else if (c == 'ｏ')
				{
					res += 'o';
				}
				else if (c == 'ｐ')
				{
					res += 'p';
				}
				else if (c == 'ｑ')
				{
					res += 'q';
				}
				else if (c == 'ｒ')
				{
					res += 'r';
				}
				else if (c == 'ｓ')
				{
					res += 's';
				}
				else if (c == 'ｔ')
				{
					res += 't';
				}
				else if (c == 'ｕ')
				{
					res += 'u';
				}
				else if (c == 'ｖ')
				{
					res += 'v';
				}
				else if (c == 'ｗ')
				{
					res += 'w';
				}
				else if (c == 'ｘ')
				{
					res += 'x';
				}
				else if (c == 'ｙ')
				{
					res += 'y';
				}
				else if (c == 'ｚ')
				{
					res += 'z';
				}
				else if (c == '０')
				{
					res += '0';
				}
				else if (c == '１')
				{
					res += '1';
				}
				else if (c == '２')
				{
					res += '2';
				}
				else if (c == '３')
				{
					res += '3';
				}
				else if (c == '４')
				{
					res += '4';
				}
				else if (c == '５')
				{
					res += '5';
				}
				else if (c == '６')
				{
					res += '6';
				}
				else if (c == '７')
				{
					res += '7';
				}
				else if (c == '８')
				{
					res += '8';
				}
				else if (c == '９')
				{
					res += '9';
				}
				else if (c == '（')
				{
					res += '(';
				}
				else if (c == '）')
				{
					res += ')';
				}
				else if (c == '｛')
				{
					res += '{';
				}
				else if (c == '｝')
				{
					res += '}';
				}
				else if (c == '［')
				{
					res += '[';
				}
				else if (c == '］')
				{
					res += ']';
				}
				else if (c == '【')
				{
					res += '[';
				}
				else if (c == '】')
				{
					res += ']';
				}
				else
				{
					res += c;
				}
			}

			return res;
		}
		public string RemoveDateStrings(string src)
		{
			if (string.IsNullOrEmpty(src)) return src;
			// 年は2桁または4桁、月・日は1桁または2桁、区切りは-, /, . に対応
			string pattern = @"\b(\d{4}|\d{2})[-/.](\d{1,2})[-/.](\d{1,2})\b";
			return Regex.Replace(src, pattern, "").Trim();
		}
		public string Rename(string src)
		{
			string d = Path.GetDirectoryName(src);
			string fn = Path.GetFileNameWithoutExtension(src);
			string ext = Path.GetExtension(src);
			fn = fn.Trim();
			if (fn.Length == 0)
			{
				return src;
			}
			string fn2 = fn;
			fn2 = ZenHan(fn2);
			fn2 = RemoveDateStrings(fn2);
			foreach (var word in delWords)
			{
				fn2 = fn2.Replace(word, "");
			}
			fn2 = DelSP(fn2.Trim());
			fn2 = DelSP(fn2.Trim());

			string newFilename = "";
			if (d!=null && d.Length>0)
			{
				newFilename = d + "\\" + fn2 + ext;
			}
			else
			{
				newFilename = fn2 + ext;
			}
			if(newFilename == src)
			{
				return "";
			}	
			while (File.Exists(newFilename))
			{
				fn2 = fn2+"_";
				if (d != null && d.Length > 0)
				{
					newFilename = d + "\\" + fn2 + ext;
				}
				else
				{
					newFilename = fn2 + ext;
				}
			}
			string result = $"\"{fn}{ext}\" \r\n\t=> \"{fn2}{ext}\"";
			if (newFilename != src)
			{
				try
				{
					File.Move(src, newFilename);
				}
				catch (Exception ex)
				{
					result += $"\r\nError: {ex.Message}";
				}
			}
			else
			{
				result =  "";
			}
			return result;
		}
	}
}
