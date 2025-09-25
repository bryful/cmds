#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <cstdio>
#include <webp/decode.h>
#include <png.h>
#include <fstream>
#include <jpeglib.h>

bool ConvertWebpToPng(const std::string& webpPath, const std::string& pngPath) {
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

    // WebPデコード
    int width = 0, height = 0;
    uint8_t* rgba = WebPDecodeRGBA(buffer.data(), buffer.size(), &width, &height);
    if (!rgba) {
        std::cerr << "WebPデコード失敗: " << webpPath << std::endl;
        return false;
    }

    // PNG書き出し
    FILE* fp = fopen(pngPath.c_str(), "wb");
    if (!fp) {
        std::cerr << "PNGファイル作成失敗: " << pngPath << std::endl;
        WebPFree(rgba);
        return false;
    }
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        std::cerr << "libpng初期化失敗" << std::endl;
        fclose(fp);
        WebPFree(rgba);
        return false;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "PNG書き込み中にエラー" << std::endl;
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        WebPFree(rgba);
        return false;
    }
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    // 行ポインタ配列を作成
    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = rgba + y * width * 4;
    }
    png_write_image(png_ptr, row_pointers.data());
    png_write_end(png_ptr, nullptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    WebPFree(rgba);
    std::cout << "変換完了: " << webpPath << " -> " << pngPath << std::endl;
    return true;
}

void Usage() {
    std::cout << "Usage: wp <input.webp> " << std::endl;
    std::cout << "Example: wp image.webp" << std::endl;
}
std::string ConvertImgiToImgPng(const std::string& webpName) {
    std::regex re(R"(imgi_(\d+)_(\d+)\.webp)", std::regex::icase);
    std::smatch m;
    if (std::regex_match(webpName, m, re) && m[1] == m[2]) {
        int n = std::stoi(m[1]);
        char buf[16];
        std::snprintf(buf, sizeof(buf), "img%03d.png", n);
        return std::string(buf);
    }
    // マッチしない場合は拡張子だけpngに
    return std::filesystem::path(webpName).replace_extension(".png").string();
}
int main(int argc, char* argv[]) {

    if (argc < 2) {
        Usage();
        return 1;
	}
	std::string webpPath = "imgi_1_1.webp";
    std::string pngPath = ConvertImgiToImgPng(webpPath);

    ConvertWebpToPng(webpPath, pngPath);
    return 0;
}
