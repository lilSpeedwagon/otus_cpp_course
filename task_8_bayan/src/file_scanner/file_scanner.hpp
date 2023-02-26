#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/regex.hpp>

#include <utils.hpp>


namespace bayan::file {

/// @brief Scanner for searching files with specified preferences.
class FileScanner : public boost::noncopyable {
public:
    /// @brief Constructor
    /// @param include_dirs directories to search into 
    /// @param exclude_dirs directories to exclude from search
    /// @param file_name_wildcards list of file masks to search 
    /// @param max_scan_depth max directory search depth
    /// @param min_file_size min size of file to search
    FileScanner(const std::vector<std::string>& include_dirs, const std::vector<std::string>& exclude_dirs,
                const std::vector<std::string>& file_name_wildcards, size_t max_scan_depth, size_t min_file_size);
    ~FileScanner() = default;

    /// @brief Perform file search according to specified params.
    void Scan();

    /// @brief Drop previous search results.
    void Reset();

    /// @brief Returns the results of search.
    std::vector<boost::filesystem::path> GetScannedFiles() const;

private:
    FileScanner(FileScanner&&);
    FileScanner& operator=(FileScanner&&);

    void ScanDirectory(const boost::filesystem::path& dir_path, size_t depth);
    void ScanFile(const boost::filesystem::path& path);

    std::vector<boost::filesystem::path> include_dirs_;
    std::unordered_set<boost::filesystem::path> exclude_dirs_;
    std::vector<boost::regex> file_name_wildcards_;
    size_t max_scan_depth_;
    uintmax_t min_file_size_;

    std::vector<boost::filesystem::path> scanned_files_;
};

} // bayan::file
