using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace cmds
{
	public class FSort
	{
		private string m_TargetDirectory = "";
		public string TargetDirectory
		{
			get { return m_TargetDirectory; }
			set
			{
				value = value.Trim();
				if (Directory.Exists(value))
				{
					m_TargetDirectory = value;
				}
				else
				{
					m_TargetDirectory = "";
				}
			}
		}
		public bool ExistsDIrectory
		{
			get
			{
				bool ret = false;
				if (m_TargetDirectory == "") return ret;
				if (Directory.Exists(m_TargetDirectory))
				{
					ret = true;
				}
				return ret;
			}
		}
		public FSort() { }
		public FSort(string path)
		{
			TargetDirectory = path;
		}
		/// <summary>
		/// 入力文字列から最初の [ ] で囲まれた文字列を取得します。
		/// 見つからない場合は空文字列を返します。
		/// </summary>
		public string GetBracketContent(string src)
		{
			if (string.IsNullOrEmpty(src)) return "";
			var m = Regex.Match(src, @"\[(.*?)\]");
			return m.Success ? m.Groups[1].Value : "";
		}
		public bool SortFiles()
		{
			if (!ExistsDIrectory)
			{
				return false;
			}
			var files = FileUtl.getImageFiles(m_TargetDirectory);
			foreach (var file in files)
			{
				string fileName = Path.GetFileName(file);
				string bracketContent = GetBracketContent(fileName);
				if (string.IsNullOrEmpty(bracketContent))
				{
					continue; // [ ] が見つからない場合はスキップ
				}
				string destDir = Path.Combine(m_TargetDirectory, bracketContent);
				if (!Directory.Exists(destDir))
				{
					Directory.CreateDirectory(destDir);
				}
				string destPath = Path.Combine(destDir, fileName);
				if (!File.Exists(destPath))
				{
					File.Move(file, destPath);
				}
			}
			return true;
		}
		public bool ReSortFiles()
		{
			if (!ExistsDIrectory)
			{
				return false;
			}
			var dirs = FileUtl.ListDirs(m_TargetDirectory, "*", false);

			foreach (var dir in dirs)
			{
				var files = FileUtl.ListFiles(dir, "*", false);
				foreach (var file in files)
				{
					string fileName = Path.GetFileName(file);
					string destPath = Path.Combine(m_TargetDirectory, fileName);
					if (!File.Exists(destPath))
					{
						File.Move(file, destPath);
					}
				}
				// ディレクトリが空なら削除
				if (Directory.GetFiles(dir).Length == 0 && Directory.GetDirectories(dir).Length == 0)
				{
					Directory.Delete(dir);
				}
			}
			return true;
		}
	}
}
