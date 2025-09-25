#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <cstdio>
#include <webp/decode.h>
#include <fstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
bool ConvertWebpToJpeg_Stb(const std::string& webpPath, const std::string& jpegPath, int quality = 75) {
    // WebPファイルをバイナリで読み込む
    std::ifstream file(webpPath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "WebPファイルが開けません: " << webpPath << std::endl;
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "WebPファイルの読み込み失敗: " << webpPath << std::endl;
        return false;
    }

    // WebPデコード（RGBで取得）
    int width = 0, height = 0;
    uint8_t* rgb = WebPDecodeRGB(buffer.data(), buffer.size(), &width, &height);
    if (!rgb) {
        std::cerr << "WebPデコード失敗: " << webpPath << std::endl;
        return false;
    }

    // stb_image_writeでJPEG保存
    int ok = stbi_write_jpg(jpegPath.c_str(), width, height, 3, rgb, quality);
    WebPFree(rgb);
    if (!ok) {
        std::cerr << "JPEG書き込み失敗: " << jpegPath << std::endl;
        return false;
    }
    std::cout << "変換完了: " << webpPath << " -> " << jpegPath << std::endl;
    return true;
}

void Usage() {
    std::cout << "Usage: wp <input.webp> " << std::endl;
    std::cout << "Example: wp image.webp" << std::endl;
}
std::string ConvertImgiToImgJpeg(const std::string& webpName) {
    std::regex re(R"(imgi_(\d+)_(\d+)\.webp)", std::regex::icase);
    std::smatch m;
    if (std::regex_match(webpName, m, re) && m[1] == m[2]) {
        int n = std::stoi(m[1]);
        char buf[16];
        std::snprintf(buf, sizeof(buf), "img%03d.jpeg", n);
        return std::string(buf);
    }
    // マッチしない場合は拡張子だけpngに
    return std::filesystem::path(webpName).replace_extension(".jpeg").string();
}
int main(int argc, char* argv[]) {

    if (argc < 2) {
        Usage();
        return 1;
	}
	std::string webpPath = "imgi_1_1.webp";
    std::string pngPath = ConvertImgiToImgJpeg(webpPath);

    ConvertWebpToJpeg_Stb(webpPath, pngPath);
    return 0;
}
