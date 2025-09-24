#pragma once
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <algorithm>

class HeaderRename {
private:
    std::vector<std::string> delWords;

public:
    HeaderRename() {
        Init();
    }

    void Init() {
        delWords.clear();
        delWords.push_back("(���N�R�~�b�N)");
        delWords.push_back("���N�R�~�b�N");
        delWords.push_back("(��ʃR�~�b�N)");
        delWords.push_back("��ʃR�~�b�N");
        delWords.push_back("(��ʏ���)");
        delWords.push_back("(��ʏ���SF)");
        delWords.push_back("(��ʏ��ोߑ�SF)");
        delWords.push_back("(��ʏ��ौÓTSF)");
        delWords.push_back("(���Ǝ�)");
        delWords.push_back("���Ǝ�");
        delWords.push_back("(���l��)");
        delWords.push_back("���l��");
        delWords.push_back("[�G��]");
        delWords.push_back("�G��");
        delWords.push_back("(C94)");
        delWords.push_back("(C95)");
        delWords.push_back("(C96)");
        delWords.push_back("(C97)");
        delWords.push_back("(C98)");
        delWords.push_back("(C99)");
        delWords.push_back("(C100)");
        delWords.push_back("(C101)");
        delWords.push_back("(C102)");
        delWords.push_back("(C103)");
        delWords.push_back("(C104)");
        delWords.push_back("(C105)");
        delWords.push_back("(C106)");
        delWords.push_back("()");
        delWords.push_back("( )");
        delWords.push_back("[]");
        delWords.push_back("[ ]");
    }

    void AddDelWord(const std::string& word) {
        std::string w = Trim(word);
        if (w.empty()) return;
        if (std::find(delWords.begin(), delWords.end(), w) == delWords.end()) {
            delWords.push_back(w);
        }
    }

    bool LoadWords(const std::string& path) {
        delWords.clear();
        std::ifstream ifs(path);
        if (!ifs) {
            Init();
            return false;
        }
        std::string line;
        bool hasLine = false;
        while (std::getline(ifs, line)) {
            AddDelWord(line);
            hasLine = true;
        }
        if (!hasLine) {
            Init();
            return false;
        }
        return true;
    }

    bool SaveWords(const std::string& path) {
        std::ofstream ofs(path);
        if (!ofs) {
            return false;
        }
        for (const auto& word : delWords) {
            ofs << word << std::endl;
        }
        return true;
    }

    std::string DelSP(const std::string& src) {
        std::string s = Trim(src);
        if (s.length() <= 1) return s;
        std::string res(1, s[0]);
        for (size_t i = 1; i < s.length(); ++i) {
            if (s[i] != ' ') {
                res += s[i];
            } else if (s[i - 1] != ' ') {
                res += s[i];
            }
        }
        return res;
    }

    std::string ZenHan(const std::string& src) {
        std::string res;
        for (char c : src) {
            // �p�����S�p�����p
            if (c >= '�`' && c <= '�y') res += 'A' + (c - '�`');
            else if (c >= '��' && c <= '��') res += 'a' + (c - '��');
            else if (c >= '�O' && c <= '�X') res += '0' + (c - '�O');
            // ���ʗ�
            else if (c == '�@') res += ' ';
            else if (c == '�i') res += '(';
            else if (c == '�j') res += ')';
            else if (c == '�o') res += '{';
            else if (c == '�p') res += '}';
            else if (c == '�m' || c == '�y') res += '[';
            else if (c == '�n' || c == '�z') res += ']';
            else res += c;
        }
        return res;
    }

    std::string RemoveDateStrings(const std::string& src) {
        // �N��2���܂���4���A���E����1���܂���2���A��؂��-, /, . �ɑΉ�
        std::regex re(R"((\b\d{4}|\b\d{2})[-/.]\d{1,2}[-/.]\d{1,2}\b)");
        return std::regex_replace(src, re, "");
    }

    std::string Rename(const std::string& src) {
        // src: �t���p�X
        size_t lastSlash = src.find_last_of("/\\");
        std::string dir = (lastSlash != std::string::npos) ? src.substr(0, lastSlash) : "";
        std::string filename = (lastSlash != std::string::npos) ? src.substr(lastSlash + 1) : src;

        size_t lastDot = filename.find_last_of('.');
        std::string fn = (lastDot != std::string::npos) ? filename.substr(0, lastDot) : filename;
        std::string ext = (lastDot != std::string::npos) ? filename.substr(lastDot) : "";

        fn = Trim(fn);
        if (fn.empty()) return src;

        std::string fn2 = ZenHan(fn);
        fn2 = RemoveDateStrings(fn2);
        for (const auto& word : delWords) {
            size_t pos;
            while ((pos = fn2.find(word)) != std::string::npos) {
                fn2.erase(pos, word.length());
            }
        }
        fn2 = DelSP(Trim(fn2));
        fn2 = DelSP(Trim(fn2));

        std::string newFilename = dir.empty() ? fn2 + ext : dir + "\\" + fn2 + ext;
        if (newFilename == src) return "";

        // �t�@�C�����d�����̏����͏ȗ��i�K�v�Ȃ�ǉ��j

        return newFilename;
    }

    // ���[�e�B���e�B: ������̑O��󔒏���
    static std::string Trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
};