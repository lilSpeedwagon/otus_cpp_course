#include <iostream>

#include <hash/factory.hpp>
#include <file_comparator/file_comparator.hpp>
#include <file_reader/factory.hpp>
#include <file_scanner/file_scanner.hpp>


int main() {

    size_t block_size = 1024;

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

    bayan::hash::HashProcessorFactory hash_factory("default");
    bayan::file::FileHashReaderFactory hash_reader_factory(std::move(hash_factory), block_size);
    bayan::file::FileComparator comparator(std::move(hash_reader_factory));
    auto result = comparator.Compare(files);

    for (const auto& [hash, file_group] : result) {
        std::cout << hash << '\n';
        for (const auto& path : file_group) {
            std::cout << path << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
