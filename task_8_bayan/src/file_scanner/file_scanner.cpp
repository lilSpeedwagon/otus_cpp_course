#include "file_scanner.hpp"

#include <algorithm>
#include <exception>


namespace bayan::file {

FileScanner::FileScanner(
    const std::vector<std::string>& include_dirs, const std::vector<std::string>& exclude_dirs,
    const std::vector<std::string>& file_name_wildcards, size_t max_scan_depth, size_t min_file_size)
    : include_dirs_(), exclude_dirs_(), max_scan_depth_(max_scan_depth),
      min_file_size_(static_cast<uintmax_t>(min_file_size)) {
    // include
    include_dirs_.reserve(include_dirs.size());
    std::transform(include_dirs.begin(), include_dirs.end(), std::back_inserter(include_dirs_),
                    [](const auto& str) { return boost::filesystem::path(str); });
    // exclude
    exclude_dirs_.reserve(exclude_dirs.size());
    for (const auto& dir : exclude_dirs) {
        exclude_dirs_.insert(dir);
    }
    // wildcards
    file_name_wildcards_.reserve(file_name_wildcards.size());
    for (const auto& wc : file_name_wildcards) {
        file_name_wildcards_.emplace_back(wc);
    }
}

void FileScanner::Scan() {
    for (const auto& dir : include_dirs_) {
        if (!boost::filesystem::is_directory(dir)) {
            throw std::runtime_error(dir.string() + " is not a directory");
        }
        ScanDirectory(dir, 0);
    }
}

void FileScanner::Reset() {
    scanned_files_.clear();
}

std::vector<boost::filesystem::path> FileScanner::GetScannedFiles() const {
    return scanned_files_;
}

void FileScanner::ScanDirectory(const boost::filesystem::path& dir_path, size_t depth) {
    if (exclude_dirs_.count(dir_path) != 0 || depth > max_scan_depth_) {
        return;
    }
    
    depth++;
    for (const auto& it : boost::filesystem::directory_iterator(dir_path)) {
        const auto path = it.path();
        if (boost::filesystem::is_directory(path)) {
            ScanDirectory(path, depth);
        } else {
            ScanFile(path);
        }
    }
}

void FileScanner::ScanFile(const boost::filesystem::path& path) {
    auto size = boost::filesystem::file_size(path);
    const auto match_lambda = [path = path.string()](const auto& wc) {
        return boost::regex_match(path, wc);
    };

    if (std::find_if(file_name_wildcards_.begin(), file_name_wildcards_.end(),
                        match_lambda) == file_name_wildcards_.end() ||
        size < min_file_size_) {
        return;
    }

    scanned_files_.push_back(path);
}

} // bayan::file
