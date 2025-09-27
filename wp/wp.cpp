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

namespace fs = std::filesystem;

bool ConvertWebpToJpeg_Stb(const fs::path & webpPath, const fs::path& jpegPath, int quality = 75) {
    // WebPファイルをバイナリで読み込む
    std::ifstream file(webpPath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "WebPファイルが開けません: " << webpPath.filename().string() << std::endl;
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "WebPファイルの読み込み失敗: " << webpPath.filename().string() << std::endl;
        return false;
    }
    file.close();

    // WebPデコード（RGBで取得）
    int width = 0, height = 0;
    uint8_t* rgb = WebPDecodeRGB(buffer.data(), buffer.size(), &width, &height);
    if (!rgb) {
        std::cerr << "WebPデコード失敗: " << webpPath.filename().string() << std::endl;
        return false;
    }

    // stb_image_writeでJPEG保存
    int ok = stbi_write_jpg(jpegPath.string().c_str(), width, height, 3, rgb, quality);
    WebPFree(rgb);
    if (!ok) {
        std::cerr << "JPEG書き込み失敗: " << jpegPath << std::endl;
        return false;
    }
    else {
        try {
            if (std::filesystem::remove(webpPath)) {
            }
            else {
                return false;
            }
            std::cout << "変換完了: " << webpPath.filename().string() << " -> " << jpegPath.filename().string() << std::endl;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "削除エラー: " << webpPath.filename().string() <<" - " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}
void Usage() {
    std::cout << "Usage: wp <input.webp> " << std::endl;
    std::cout << "Example: wp image.webp" << std::endl;
}
fs::path ConvertImgiToImgJpeg(const fs::path& webpName) {
    std::regex re(R"(imgi_(\d+)_(\d+)\.webp)", std::regex::icase);
    std::smatch m;
	std::string fn = webpName.filename().string();
    if (std::regex_match(fn, m, re) && m[1] == m[2]) {
        int n = std::stoi(m[1]);
        char buf[16];
        std::snprintf(buf, sizeof(buf), "img%03d.jpeg", n);
        fs::path ret = fs::path(webpName);
        
        return ret.replace_filename(buf);
    }
    // マッチしない場合は拡張子だけpngに
    fs::path aa = fs::path(webpName);
    return aa.replace_extension(".jpeg").string();
}
bool ConvertWebpToJpeg_Stb(const fs::path& path) {

    fs::path pngPath = ConvertImgiToImgJpeg(path);
	return ConvertWebpToJpeg_Stb(path, pngPath);

}
std::vector<fs::path> GetFilesWithExtensions(const fs::path& dir, const std::vector<std::string>& exts) {
    std::vector<fs::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        for (const auto& e : exts) {
            if (ext == e) {
                files.push_back(entry);
                break;
            }
        }
    }
    return files;
}
int main(int argc, char* argv[]) {

    if (argc < 2) {
        Usage();
        return 1;
	}
	fs::path webpPath = fs::path(argv[1]);
    if (fs::is_regular_file(webpPath)) {
        ConvertWebpToJpeg_Stb(webpPath);
    }
    else if (fs::is_directory(webpPath)) {
		std::vector<std::string> exts = { ".webp"};
		std::vector<fs::path> lst = GetFilesWithExtensions(webpPath, exts);
        for (const auto& f : lst) {
			ConvertWebpToJpeg_Stb(f);
        }

    }
    else {
        Usage();
        return 1;
    }
    return 0;
}
