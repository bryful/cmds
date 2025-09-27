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

// �t�H���_��t�@�C���̏����Ǘ����郆�[�e�B���e�B�N���X
// - �p�X�̑��ݔ���A�t�@�C�����E�g���q�E�e�f�B���N�g���̎擾
// - �f�B���N�g�����̃t�@�C���E�T�u�f�B���N�g���ꗗ�擾
class FolderItems
{
private:
    std::filesystem::path _targetPath; // �Ώۃp�X
    std::string _parentPath;           // �e�f�B���N�g��
    std::string _fileName;             // �t�@�C�����i�g���q�Ȃ��j
    std::string _ext;                  // �g���q
    bool _exists = false;              // ���݂��邩
    bool _isDir = false;               // �f�B���N�g����
    // �p�X�̏�Ԃ��X�V
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
    // �p�X�擾
    std::filesystem::path path() const {
        return _targetPath;
    }
    // ��΃p�X�擾
    std::string absolutePath() const {
        return std::filesystem::absolute(_targetPath).string();
    }
    // �e�f�B���N�g���擾
    std::filesystem::path parent() const {
        return _targetPath.parent_path();
    }
    std::string parentPath() const {
        return _parentPath;
    }
    // �t�@�C�����i�g���q�Ȃ��j�擾
    std::string fileName() const {
        return _fileName;
    }
    // �g���q�擾
    std::string ext() const {
        return _ext;
    }
    // ���݃`�F�b�N
    bool exists() const { return _exists; }
    // �f�B���N�g����
    bool isDir() const { return _isDir; }
    // �t�@�C����
    bool isFile() const { return !_isDir; }
    // �p�X�ݒ�i������j
    void setTargetPath(const std::string& path)
    {
        _targetPath = std::filesystem::path(path);
        chkStatus();
    }
    // �p�X�ݒ�ipath�^�j
    void setTargetPath(const std::filesystem::path& path)
    {
        _targetPath = path;
        chkStatus();
    }
    // �f�t�H���g�R���X�g���N�^�i�J�����g�f�B���N�g���j
    FolderItems() 
    {
        setTargetPath(std::filesystem::absolute(std::filesystem::current_path()).string());
    }
    // ������p�X�w��R���X�g���N�^
    FolderItems(const std::string& path)
    {
        setTargetPath(path);
    }
    // path�^�p�X�w��R���X�g���N�^
    FolderItems(const std::filesystem::path& path)
    {
        setTargetPath(path);
    }
    // �w��g���q�̃t�@�C���ꗗ�擾
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
    // �f�B���N�g�����̑S�t�@�C���ꗗ�擾
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
    // �f�B���N�g�����̃T�u�f�B���N�g���ꗗ�擾
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