// rh.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <vector>
#include <string>
#include "HeaderRename.h"

// 追加: C++17以降でfilesystemを有効にする
#if __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "no filesystem support"
#endif

void Usage() {
    std::cout << "Usage: rh <path>" << std::endl;
}

int main(int argc, char* argv[])
{
    HeaderRename headerRename;

    // exeパス取得
    std::string exePath = argv[0];
   // std::cout << "exePath=" << exePath << std::endl;

    // .lstファイル名生成
    std::string lstf = exePath;
    size_t dot = lstf.find_last_of('.');
    if (dot != std::string::npos) {
        lstf = lstf.substr(0, dot) + ".lst";
    } else {
        lstf += ".lst";
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
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(targetPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".zip" || ext == ".rar") {
                files.push_back(entry.path().string());
            }
        }
    }
    if (files.empty()) {
        std::cout << "No files found." << std::endl;
        return 0;
    }

    for (const auto& file : files) {
        std::string ret = headerRename.Rename(file);
        if (!ret.empty()) {
            std::cout << ret << std::endl;
        }
    }
    return 0;
}

