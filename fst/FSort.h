#pragma once
// C# FileSort�N���X��C++17�ڐA��
#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

// [xxxx]��xxxx�������擾
std::string GetBracketContent(const std::string& src) {
    std::smatch m;
    std::regex re(R"(\[(.*?)\])");
    if (std::regex_search(src, m, re)) {
        return m[1].str();
    }
    return "";
}

// FSort: �t�@�C����[xxxx]�f�B���N�g���Ɉړ�
std::string FSort(const std::string& p) {
    fs::path path(p);
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        return "Error: File not found. " + p;
    }
    std::string fn = path.filename().string();
    std::string cnt = GetBracketContent(fn);
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (ext != ".zip" && ext != ".rar") {
        return "Skipped (not zip/rar). " + fn;
    }
    if (cnt.empty()) {
        return "No brackets found. " + fn;
    }
    fs::path newDir = path.parent_path() / cnt;
    if (!fs::exists(newDir)) {
        fs::create_directory(newDir);
    }
    fs::path newPath = newDir / fn;
    if (fs::exists(newPath)) {
        return "File already exists in target directory. " + newPath.string();
    }
    try {
        fs::rename(path, newPath);
        return "Moved: " + fn + " -> " + newDir.string();
    }
    catch (const std::exception& ex) {
        return std::string("Error moving file: ") + ex.what();
    }
}

// �T�u�f�B���N�g�����̃t�@�C�������̊K�w�Ɉړ����A��Ȃ�폜
std::string FsortDirSub(const fs::path& targetPath) {
    std::string ret;
    if (!fs::exists(targetPath) || !fs::is_directory(targetPath)) return ret;

    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(targetPath)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }
    if (files.empty()) return ret;

    for (const auto& file : files) {
        fs::path d = file.parent_path();
        fs::path d2 = d.parent_path();
        fs::path fn = file.filename();
        fs::path dest = d2 / fn;
        try {
            fs::rename(file, dest);
            ret += "moved file:" + fn.string() + "\n";
        }
        catch (const std::exception& ex) {
            ret += "Error moving file: " + std::string(ex.what()) + "\n" + fn.string() + "\n";
        }
    }
    // �c�t�@�C�����Ȃ���΃f�B���N�g���폜
    if (fs::is_empty(targetPath)) {
        fs::remove(targetPath);
    }
    return ret;
}

// �w��f�B���N�g�����̃T�u�f�B���N�g��������
std::string FsortDir(const std::string& targetPath) {
    std::string ret;
    fs::path path(targetPath);
    if (fs::exists(path) && fs::is_regular_file(path)) {
        path = path.parent_path();
    }
    if (!fs::exists(path) || !fs::is_directory(path)) return ret;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            std::string s = FsortDirSub(entry.path());
            if (!s.empty()) ret += s;
        }
    }
    return ret;
}

