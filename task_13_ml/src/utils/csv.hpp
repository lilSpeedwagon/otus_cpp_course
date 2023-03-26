#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>


namespace ml::utils {

template<typename T>
using CsvRow = std::vector<T>;

template<typename T>
std::vector<CsvRow<T>> ReadCsv(const std::filesystem::path& path, char delimiter = ' ') {
    std::ifstream file(path);
    if (!file.is_open() || !file.good()) {
        throw std::runtime_error("cannot open file");
    }
    
    std::vector<CsvRow<T>> result;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row_items;
        boost::split(row_items, std::move(line), [delimiter](auto c) { return c == delimiter; });

        CsvRow<T> row;
        row.reserve(row_items.size());
        for (auto& item : row_items) {
            T value;
            if constexpr (std::is_integral_v<T>) {
                value = std::stoi(item);
            } else if (std::is_same_v<std::string, T>) {
                value = std::move(item);
            } else {
                value = static_cast<T>(item);
            }
            row.emplace_back(std::move(value));
        }

        result.emplace_back(std::move(row));
    }

    return result;
}

} // namespace ml::utils
