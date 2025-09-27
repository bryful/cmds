#include "BryUtil.h"
// ��������������ɕϊ����郆�[�e�B���e�B�֐�
std::string toLowwer(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}

// �f�B���N�g���ƃt�@�C���������S�Ɍ�������֐�
std::string JoinPath(const std::string& dir, const std::string& filename) {
    std::filesystem::path p = std::filesystem::path(dir) / filename;
    return p.string();
}

// �t���p�X��e�f�B���N�g���E�t�@�C�����E�g���q�ɕ�������֐�
void SplitFullPath(const std::string& fullPath, std::string& parentDir, std::string& fileName, std::string& ext) {
    std::filesystem::path p(fullPath);
    parentDir = p.parent_path().string();
    fileName = p.stem().string();
    ext = p.extension().string();
}