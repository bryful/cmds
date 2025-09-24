#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <cstdio>

// stb_image, stb_image_write を使う場合
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace fs = std::filesystem;

// WebP→PNG変換
bool ConvertWebpToPng(const std::string& webpPath, const std::string& pngPath) {
    int w, h, comp;
    unsigned char* img = stbi_load(webpPath.c_str(), &w, &h, &comp, 4);
    if (!img) {
        std::cerr << "読み込み失敗: " << webpPath << std::endl;
        return false;
    }
    int ok = stbi_write_png(pngPath.c_str(), w, h, 4, img, w * 4);
    stbi_image_free(img);
    if (!ok) {
        std::cerr << "書き込み失敗: " << pngPath << std::endl;
        return false;
    }
    std::cout << "変換完了: " << webpPath << " -> " << pngPath << std::endl;
    return true;
}

// imgi_x_x.png → imgNNN.png 形式へリネーム
void RenameImgi(const fs::path& file) {
    std::regex re(R"(imgi_(\d+)_(\d+))");
    std::smatch m;
    std::string stem = file.stem().string();
    if (std::regex_match(stem, m, re) && m[1] == m[2]) {
        int n = std::stoi(m[1]);
        char buf[32];
        std::snprintf(buf, sizeof(buf), "img%03d%s", n, file.extension().string().c_str());
        fs::path newpath = file.parent_path() / buf;
        if (!fs::exists(newpath)) {
            fs::rename(file, newpath);
            std::cout << "リネーム: " << file.filename() << " -> " << newpath.filename() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "使い方: webp <png|rimgi> [path]" << std::endl;
        return 0;
    }
    std::string cmd = argv[1];
    std::string path = (argc >= 3) ? argv[2] : ".";

    if (cmd == "png") {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.path().extension() == ".webp") {
                fs::path pngPath = entry.path();
                pngPath.replace_extension(".png");
                ConvertWebpToPng(entry.path().string(), pngPath.string());
            }
        }
    } else if (cmd == "rimgi") {
        for (const auto& entry : fs::directory_iterator(path)) {
            RenameImgi(entry.path());
        }
    } else {
        std::cout << "未対応コマンド: " << cmd << std::endl;
    }
    return 0;
}
