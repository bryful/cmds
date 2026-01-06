using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace cmds
{
	public class WinNaturalComparer : IComparer<string>
	{
		[DllImport("shlwapi.dll", CharSet = CharSet.Unicode)]
		static extern int StrCmpLogicalW(string x, string y);

		public int Compare(string? x, string? y)
		{
			if (x is null && y is null) return 0;
			if (x is null) return -1;
			if (y is null) return 1;
			return StrCmpLogicalW(x, y);
		}
	}
	public class FileLister
	{
		static public string [] ListFiles(string dirPath, string searchPattern="*", bool searchAllSubDirs=false)
		{
			string[] result = new String[0];
			if (!Directory.Exists(dirPath))
			{
				return result;
			}

			var options = new EnumerationOptions
			{
				IgnoreInaccessible = true,       // アクセス権限のないフォルダを無視
				RecurseSubdirectories = searchAllSubDirs,     
				ReturnSpecialDirectories = false  // "." や ".." を含めない
			};

			result = Directory.EnumerateFiles(dirPath, searchPattern, options).ToArray();
			return result;
		}
		static public string[] ListSort(string[] files)
		{
			Array.Sort(files, new WinNaturalComparer());
			return files;
		}
		static public string[] getImageFiles(string dirPath)
		{
			var files = ListFiles(dirPath, "*", false);
			List<string> flist = new List<string>();
			
			foreach(var f in files)
			{
				string ext = Path.GetExtension(f).ToLower();
				if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".webp" || ext == ".bmp" || ext == ".tiff" || ext == ".tif" || ext == ".gif")
				{
					flist.Add(f);
				}
			}
			if (flist.Count == 0)
			{
				return new string[0];
			}
			return ListSort(flist.ToArray());
		}
	}
}
