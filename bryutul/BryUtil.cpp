#include "BryUtil.h"
// 文字列を小文字に変換するユーティリティ関数
std::string toLowwer(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}

// ディレクトリとファイル名を安全に結合する関数
std::string JoinPath(const std::string& dir, const std::string& filename) {
    std::filesystem::path p = std::filesystem::path(dir) / filename;
    return p.string();
}

// フルパスを親ディレクトリ・ファイル名・拡張子に分割する関数
void SplitFullPath(const std::string& fullPath, std::string& parentDir, std::string& fileName, std::string& ext) {
    std::filesystem::path p(fullPath);
    parentDir = p.parent_path().string();
    fileName = p.stem().string();
    ext = p.extension().string();
}