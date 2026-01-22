using System;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Text.RegularExpressions;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats.Png;
using SixLabors.ImageSharp.Formats.Webp;

namespace cmds
{
	public class Webp
	{
		public bool DeleteSourceFile { get; set; } = false;
		private string m_TargetDirectory = "";
		public string TargetDirectory
		{
			get { return m_TargetDirectory; }
			set {
				m_TargetDirectory = value; 
			}
		}
		public bool ExistsDIrectory
		{ 
			get
			{
				bool ret = false;
				if (m_TargetDirectory == "") return ret;
				if (System.IO.Directory.Exists(m_TargetDirectory))
				{
					ret = true;
				}
				return ret;
			}
		}
		public Webp()
		{
		}
		public Webp(String str)
		{
			m_TargetDirectory = str;
		}
		private string ConvertImgiToImgName(string filePath)
		{
			string fileName = System.IO.Path.GetFileNameWithoutExtension(filePath);
			string directory = System.IO.Path.GetDirectoryName(filePath) ?? "";

			// imgi_(\d+)_(\d+) のパターンにマッチするか確認
			var regex = new Regex(@"^imgi_(\d+)_(\d+)$", RegexOptions.IgnoreCase);
			var match = regex.Match(fileName);

			if (match.Success && match.Groups[1].Value == match.Groups[2].Value)
			{
				int number = int.Parse(match.Groups[1].Value);
				string newFileName = $"img{number:D3}.jpg";
				return System.IO.Path.Combine(directory, newFileName);
			}

			// マッチしない場合は通常の変換
			return System.IO.Path.ChangeExtension(filePath, ".jpg");
		}
		private string NameOverwriteCheck(string src)
		{
			if (!System.IO.File.Exists(src))
			{
				return src;
			}
			string directory = System.IO.Path.GetDirectoryName(src) ?? "";
			string fileNameWithoutExt = System.IO.Path.GetFileNameWithoutExtension(src);
			string extension = System.IO.Path.GetExtension(src);
			int count = 1;
			string newFile = $"{fileNameWithoutExt}{extension}";
			string newFilePath = Path.Combine(directory,newFile);
			while(System.IO.File.Exists(newFilePath))
			{
				newFile = $"{fileNameWithoutExt}_{count}{extension}";
				newFilePath = System.IO.Path.Combine(directory, newFile);
				count++;
			} ;
			return newFilePath;
		}
		public bool CovertToJpeg(string src,string dst)
		{
			try
			{
				dst = NameOverwriteCheck(dst);
				using (Image image = Image.Load(src))
				{
					image.SaveAsJpeg(dst);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Error: {ex.Message}");
				return false;
			}
			return true;
		}
		public bool ConvertAllToJpeg()
		{
			if (!ExistsDIrectory)
			{
				Console.WriteLine("Error: Target directory does not exist.");
				return false;
			}
			var files = cmds.FileUtl.ListFiles(m_TargetDirectory,"*",false);
			foreach (var file in files)
			{
				string ext = System.IO.Path.GetExtension(file).ToLower();
				if (ext == ".webp")
				{
					string dst = ConvertImgiToImgName(file);
					if (!CovertToJpeg(file, dst))
					{
						Console.WriteLine($"Failed to convert: {file}");
					}
					else
					{
						if (DeleteSourceFile)
						{
							try
							{
								System.IO.File.Delete(file);
							}
							catch (Exception ex)
							{
								Console.WriteLine($"Error deleting source file: {ex.Message}");
							}
						}
						Console.WriteLine($"Converting: {Path.GetFileName(file)} -> {Path.GetFileName(dst)}");
					}
				}
			}
			return true;
		}
	}
}
