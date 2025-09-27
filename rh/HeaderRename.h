#pragma once
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <algorithm>
#include <cwctype>
#include <codecvt>
#include <Windows.h>
#include <filesystem>

// UTF-8��wstring�ϊ��֐��iWindows��p�j
static std::wstring Utf8ToWString(const std::string& str) {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}
static std::wstring ReplaceAll(const std::wstring& src, const std::wstring& target, const std::wstring& replacement) {
    if (target.empty()) return src;
    std::wstring result = src;
    size_t pos = 0;
    while ((pos = result.find(target, pos)) != std::wstring::npos) {
        result.replace(pos, target.length(), replacement);
        pos += replacement.length(); // �u����̈ʒu����Č���
    }
    return result;
}
class HeaderRename {
private:
    std::vector<std::wstring> delWords;

public:
    HeaderRename() {
        Init();
    }

    void Init() {
        delWords.clear();
        delWords.push_back(L"(���N�R�~�b�N)");
        delWords.push_back(L"���N�R�~�b�N");
        delWords.push_back(L"(��ʃR�~�b�N)");
        delWords.push_back(L"��ʃR�~�b�N");
        delWords.push_back(L"(��ʏ���)");
        delWords.push_back(L"(��ʏ���SF)");
        delWords.push_back(L"(��ʏ��ोߑ�SF)");
        delWords.push_back(L"(��ʏ��ौÓTSF)");
        delWords.push_back(L"(���Ǝ�)");
        delWords.push_back(L"���Ǝ�");
        delWords.push_back(L"(���l��)");
        delWords.push_back(L"���l��");
        delWords.push_back(L"[�G��]");
        delWords.push_back(L"�G��");
        delWords.push_back(L"(C94)");
        delWords.push_back(L"(C95)");
        delWords.push_back(L"(C96)");
        delWords.push_back(L"(C97)");
        delWords.push_back(L"(C98)");
        delWords.push_back(L"(C99)");
        delWords.push_back(L"(C100)");
        delWords.push_back(L"(C101)");
        delWords.push_back(L"(C102)");
        delWords.push_back(L"(C103)");
        delWords.push_back(L"(C104)");
        delWords.push_back(L"(C105)");
        delWords.push_back(L"(C106)");
        delWords.push_back(L"()");
        delWords.push_back(L"( )");
        delWords.push_back(L"[]");
        delWords.push_back(L"[ ]");
        delWords.push_back(L"�y �z");
        delWords.push_back(L"�y�z");
    }

    void AddDelWord(const std::wstring& word) {
        std::wstring w = Trim(word);
        if (w.empty()) return;
        if (std::find(delWords.begin(), delWords.end(), w) == delWords.end()) {
            delWords.push_back(w);
        }
    }

    bool LoadWords(const std::wstring& path) {
        delWords.clear();
        std::ifstream ifs(path, std::ios::binary);
        if (!ifs) {
            Init();
            return false;
        }
        std::string line;
        bool hasLine = false;
        while (std::getline(ifs, line)) {
            // �u������: std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::wstring wline = Utf8ToWString(line);
            AddDelWord(wline);
            hasLine = true;
        }
        if (!hasLine) {
            Init();
            return false;
        }
        return true;
    }

    // �C��: std::wstring��std::string�ɕϊ����Ă���o�͂���
    bool SaveWords(const std::wstring& path) {
        std::ofstream ofs(path);
        if (!ofs) {
            return false;
        }
        // std::wstring��UTF-8�ɕϊ����ďo��
        for (const auto& word : delWords) {
            // Windows��p: WideCharToMultiByte��UTF-8�ϊ�
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, word.c_str(), (int)word.size(), NULL, 0, NULL, NULL);
            std::string utf8word(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, word.c_str(), (int)word.size(), &utf8word[0], size_needed, NULL, NULL);
            ofs << utf8word << std::endl;
        }
        return true;
    }

    std::wstring DelSP(const std::wstring& src) {
        std::wstring s = Trim(src);
        if (s.length() <= 1) return s;
        std::wstring res(1, s[0]);
        for (size_t i = 1; i < s.length(); ++i) {
            if (s[i] != ' ') {
                res += s[i];
            } else if (s[i - 1] != ' ') {
                res += s[i];
            }
        }
        return res;
    }

    
    
    std::wstring ZenHan(const std::wstring& src) {
        std::wstring res;
        for (wchar_t c : src) {
            // �p���S�p�����p
            if (c >= L'�`' && c <= L'�y') res += L'A' + (c - L'�`');
            else if (c >= L'��' && c <= L'��') res += L'a' + (c - L'��');
            // �����S�p�����p
            else if (c >= L'�O' && c <= L'�X') res += L'0' + (c - L'�O');
            // ���ʗ�
            else if (c == L'�@') res += L' ';
            else if (c == L'�i') res += L'(';
            else if (c == L'�j') res += L')';
            else if (c == L'�o') res += L'{';
            else if (c == L'�p') res += L'}';
            else if (c == L'�m' || c == L'�y') res += L'[';
            else if (c == L'�n' || c == L'�z') res += L']';
            else res += c;
        }
        return res;
    }
    std::wstring TrimU(const std::wstring& src) {
        if (src.empty()) return src;
        size_t start = 0;
        size_t end = src.size();

        // �擪��
        while (start < end && (src[start] == L'_' || src[start] == L'-')) {
            ++start;
        }
        // ������
        while (end > start && (src[end - 1] == L'_' || src[end - 1] == L'-')) {
            --end;
        }
        return src.substr(start, end - start);

    }
    std::wstring RemoveDateStrings(const std::wstring& src) {
        // �N��2���܂���4���A���E����1���܂���2���A��؂��-, /, . �ɑΉ�
        std::wregex re(LR"((\b\d{4}|\b\d{2})[-/.]\d{1,2}[-/.]\d{1,2}\b)");
        return std::regex_replace(src, re, L"");
    }
    bool Rename(const std::filesystem::path& srcP) {
        // src: �t���p�X
		std::wstring dir = srcP.parent_path().wstring();
        std::wstring filename = srcP.stem().wstring();
        std::wstring ext = srcP.extension().wstring();

        filename = Trim(filename);
        if (filename.empty()) return false;

        std::wstring fn2 = ZenHan(filename);
        fn2 = RemoveDateStrings(fn2);
        fn2 = DelSP(fn2);
        for (const auto& word : delWords) {
			fn2 = ReplaceAll(fn2, word, L"");
        }
        fn2 = DelSP(fn2);
        fn2 = Trim(fn2);
		fn2 = TrimU(fn2);

        std::wstring newFilename = dir.empty() ? fn2 + ext : dir + L"\\" + fn2 + ext;
        if (newFilename == std::filesystem::absolute(srcP).wstring()) {
            std::cout << "���l�[���s�K�v " << srcP.filename().string() << std::endl;
            return L"";
        }

        while (std::filesystem::exists(newFilename)) {
            fn2 += L"_";
            newFilename = dir.empty() ? fn2 + ext : dir + L"\\" + fn2 + ext;
        }
        try {
			std::filesystem::path newP = std::filesystem::path(newFilename);
            std::filesystem::rename(srcP, newP);
            std::cout << "���l�[������: " << srcP.filename().string() << " �� " << newP.filename().string() << std::endl;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "���l�[�����s: " << e.what() << std::endl;
        }

        return true;
    }

    // ���[�e�B���e�B: ������̑O��󔒏���
    static std::string Trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
    static std::wstring Trim(const std::wstring& s) {
        size_t start = s.find_first_not_of(L" \t\r\n");
        if (start == std::wstring::npos) return L"";
        size_t end = s.find_last_not_of(L" \t\r\n");
        return s.substr(start, end - start + 1);
    }
};