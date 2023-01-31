#pragma once

#include <exception>
#include <string>
#include <vector>
#include <unordered_set>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <file_reader/file_reader.hpp>


namespace bayan::file {

class FileScanner {
public:
    FileScanner(
        const std::vector<boost::filesystem::path>& include_dirs,
        const std::vector<boost::filesystem::path>& exclude_dirs,
        size_t max_scan_depth, size_t min_file_size,
        const std::vector<std::string>& file_name_wildcards)
        : include_dirs_(include_dirs), max_scan_depth_(max_scan_depth),
          min_file_size_(static_cast<uintmax_t(min_file_size)) {
        exclude_dirs_.reserve(exclude_dirs.size());
        for (const auto& dir : exclude_dirs) {
            exclude_dirs_.insert(dir);
        }
        file_name_wildcards_.reserve(file_name_wildcards.size());
        for (const auto& wc : file_name_wildcards) {
            file_name_wildcards_.emplace_back(wc);
        }
    }
    
    ~FileScanner() {}

    void Scan() {
        for (const auto& dir : include_dirs_) {
            if (!boost::filesystem::is_directory(dir)) {
                throw std::runtime_error(dir.string() + " is not a directory");
            }
            ScanDirectory(dir, 0);
        }
    }
    
    void Reset() {
        scanned_files_.clear();
    }

    std::vector<FileHashReader> GetScannedFiles() const {
        return scanned_files_;
    }

private:
    void ScanDirectory(const boost::filesystem::path& path, size_t depth) {
        if (exclude_dirs_.count(path) == 0 || depth > max_scan_depth_) {
            return;
        }
        
        depth++;
        for (const auto& it : boost::filesystem::directory_iterator(path)) {
            const auto path = it.path();
            if (boost::filesystem::is_directory(path)) {
                ScanDirectory(path, depth);
            } else {
                ScanFile(path);
            }
        }
    }

    void ScanFile(const boost::filesystem::path& path) {
        auto size = boost::filesystem::file_size(path);
        const auto match_lambda = [path = path.string()](const auto& wc) {
            return boost::regex_match(path.string(), wc);
        }

        if (std::find_if(file_name_wildcards_.begin(), file_name_wildcards_.end(),
                         match_lambda) == file_name_wildcards_.end() ||
            size < min_file_size_) {
            return;
        }

        auto file_reader = factory.Create(path);
        scanned_files_.push_back(std::move(file_reader));
    }

    std::vector<boost::filesystem::path> include_dirs_;
    std::unordered_set<boost::filesystem::path> exclude_dirs_;
    size_t max_scan_depth_;
    uintmax_t min_file_size_;
    std::vector<boost::regex> file_name_wildcards_;

    FileHashReaderFactory factory_;
    std::vector<FileHashReader> scanned_files_;
};

} // bayan::file
