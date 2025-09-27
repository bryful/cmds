// rh.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "HeaderRename.h"
#include <cwctype>
#include <locale>
#include <codecvt>
#include <Windows.h>
namespace fs = std::filesystem;

std::string toLowwer(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}
std::wstring toLowwer(std::wstring s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](wchar_t c) { return std::towlower(c); });
    return s;
}

// wstring → UTF-8 string 変換関数（Windows専用）
std::string WStringToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

void Usage() {
    std::cout << "Usage: rh <path>" << std::endl;
}

int main(int argc, char* argv[])
{
    HeaderRename headerRename;

    // exeパス取得
    std::wstring exePath = std::filesystem::path(argv[0]).wstring();
   // std::cout << "exePath=" << exePath << std::endl;

    // .lstファイル名生成
    std::wstring lstf = exePath;
    size_t dot = lstf.find_last_of('.');
    if (dot != std::string::npos) {
        lstf = lstf.substr(0, dot) + L".lst";
    } else {
        lstf += L".lst";
    }
    //std::cout << "lstf=" << lstf << std::endl;

    if (!headerRename.LoadWords(lstf)) {
        headerRename.SaveWords(lstf);
    }

    // 対象パス取得
    std::string targetPath;
    if (argc < 2) {
        targetPath = fs::current_path().string(); // 修正: fs::current_path() を使用
    } else {
        targetPath = argv[1];
    }
    if (!fs::exists(targetPath) || !fs::is_directory(targetPath)) {
        std::cout << "Error: Path not found." << std::endl;
        Usage();
        return 1;
    }

    // ディレクトリ内のファイル列挙
    std::filesystem::path target = std::filesystem::path(targetPath);
    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(target)) {
        if (!entry.is_regular_file()) continue;
        std::wstring ex = toLowwer(entry.path().extension().wstring());
        if (ex == L".zip" || ex == L".rar") {
            files.push_back(entry.path());
        }
    }
    if (files.empty()) {
        std::cout << "No files found." << std::endl;
        return 0;
    }

    for (const auto& file : files) {
        try {
            if (!headerRename.Rename(file)) {
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "ファイルパス変換エラー: " << e.what() << std::endl;
        }
        
    }
    return 0;
}

