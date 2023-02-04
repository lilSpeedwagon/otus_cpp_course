#include <iostream>

#include <file_scanner/file_scanner.hpp>


int main() {

    std::vector<boost::filesystem::path> include = {
        "./",
    };
    std::vector<boost::filesystem::path> exclude = {};
    std::vector<std::string> wildcards = {
        ".*"
    };

    bayan::file::FileScanner scanner(include, exclude, 10, 0, wildcards);
    scanner.Scan();
    auto files = scanner.GetScannedFiles();
    std::cout << "files:\n";
    for (const auto& f : files) {
        std::cout << f << '\n';
    }

    return 0;
}
