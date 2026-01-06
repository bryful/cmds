#pragma once
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <algorithm>
#include <filesystem>

std::string toLowwer(std::string s);
std::string JoinPath(const std::string& dir, const std::string& filename);
void SplitFullPath(const std::string& fullPath, std::string& parentDir, std::string& fileName, std::string& ext);

// フォルダやファイルの情報を管理するユーティリティクラス
// - パスの存在判定、ファイル名・拡張子・親ディレクトリの取得
// - ディレクトリ内のファイル・サブディレクトリ一覧取得
class FolderItems
{
private:
    std::filesystem::path _targetPath; // 対象パス
    std::string _parentPath;           // 親ディレクトリ
    std::string _fileName;             // ファイル名（拡張子なし）
    std::string _ext;                  // 拡張子
    bool _exists = false;              // 存在するか
    bool _isDir = false;               // ディレクトリか
    // パスの状態を更新
    void chkStatus()
    {
        _exists = false;
        _isDir = false;
        _parentPath = "";
        _fileName = "";
        _ext = "";
        if (std::filesystem::is_regular_file(_targetPath)) {
            _exists = true;
            _isDir = false;
        }
        if (std::filesystem::is_directory(_targetPath)) {
            _exists = true;
            _isDir = true;
        }
        if (_exists) {
            _fileName = _targetPath.stem().string();
            _ext = _targetPath.extension().string();
            _parentPath = _targetPath.parent_path().string();
        }
    }
public:
    // パス取得
    std::filesystem::path path() const {
        return _targetPath;
    }
    // 絶対パス取得
    std::string absolutePath() const {
        return std::filesystem::absolute(_targetPath).string();
    }
    // 親ディレクトリ取得
    std::filesystem::path parent() const {
        return _targetPath.parent_path();
    }
    std::string parentPath() const {
        return _parentPath;
    }
    // ファイル名（拡張子なし）取得
    std::string fileName() const {
        return _fileName;
    }
    // 拡張子取得
    std::string ext() const {
        return _ext;
    }
    // 存在チェック
    bool exists() const { return _exists; }
    // ディレクトリか
    bool isDir() const { return _isDir; }
    // ファイルか
    bool isFile() const { return !_isDir; }
    // パス設定（文字列）
    void setTargetPath(const std::string& path)
    {
        _targetPath = std::filesystem::path(path);
        chkStatus();
    }
    // パス設定（path型）
    void setTargetPath(const std::filesystem::path& path)
    {
        _targetPath = path;
        chkStatus();
    }
    // デフォルトコンストラクタ（カレントディレクトリ）
    FolderItems() 
    {
        setTargetPath(std::filesystem::absolute(std::filesystem::current_path()).string());
    }
    // 文字列パス指定コンストラクタ
    FolderItems(const std::string& path)
    {
        setTargetPath(path);
    }
    // path型パス指定コンストラクタ
    FolderItems(const std::filesystem::path& path)
    {
        setTargetPath(path);
    }
    // 指定拡張子のファイル一覧取得
    std::vector<std::filesystem::path> getFilesWithExtensions(const std::vector<std::string>& exts) const
    {
        std::vector<std::filesystem::path> files;
        if (!exists() || !isDir()) return files;
        for (const auto& entry : std::filesystem::directory_iterator(_targetPath)) {
            if (!entry.is_regular_file()) continue;
            std::string ext = toLowwer(entry.path().extension().string());
            if (std::find(exts.begin(), exts.end(), ext) != exts.end()) {
                files.push_back(entry.path());
            }
        }
        return files;
    }
    // ディレクトリ内の全ファイル一覧取得
    std::vector<std::filesystem::path> getFiles() const
    {
        std::vector<std::filesystem::path> files;
        if (!exists() || !isDir()) return files;
        for (const auto& entry : std::filesystem::directory_iterator(_targetPath)) {
            if (!entry.is_regular_file()) continue;
            files.push_back(entry.path());
        }
        return files;
    }
    // ディレクトリ内のサブディレクトリ一覧取得
    std::vector<std::filesystem::path> getDirectories() const
    {
        std::vector<std::filesystem::path> files;
        if (!exists() || !isDir()) return files;
        for (const auto& entry : std::filesystem::directory_iterator(_targetPath)) {
            if (entry.is_regular_file()) continue;
            files.push_back(entry.path());
        }
        return files;
    }
};