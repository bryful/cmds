#include <iostream>
#include <string>
#include <filesystem>
#include "FSort.h"

namespace fs = std::filesystem;

void Usage() {
    std::cout << "Usage: fst <path>" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string targetPath;
    if (argc < 2) {
        targetPath = fs::current_path().string();
    } else if (argc == 2) {
        targetPath = argv[1];
    } else if (argc > 2) {
        std::string op = argv[1];
        std::transform(op.begin(), op.end(), op.begin(), ::tolower);
        if (op == "-r" || op == "-rev" || op == "/r" || op == "/rev") {
            targetPath = argv[2];
            std::string res = FsortDir(targetPath);
            std::cout << res << std::endl;
            return 0;
        } else {
            Usage();
            return 1;
        }
    }

    if (!fs::exists(targetPath) || !fs::is_directory(targetPath)) {
        std::cout << "Error: Path not found." << std::endl;
        Usage();
        return 1;
    }

    bool found = false;
    for (const auto& entry : fs::directory_iterator(targetPath)) {
        if (entry.is_regular_file()) {
            found = true;
            std::string res = FSort(entry.path().string());
            std::cout << res << std::endl;
        }
    }
    if (!found) {
        std::cout << "No files found." << std::endl;
    }
    return 0;
}
