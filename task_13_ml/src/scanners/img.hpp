#pragma once

#include <filesystem>

#include <models/img.hpp>
#include <utils/csv.hpp>


namespace ml::scanners {

template<typename PointT, size_t N>
std::vector<models::ImageSample<PointT, N>> ScanImages(const std::filesystem::path& path) {
    auto csv_rows = utils::ReadCsv<PointT>(path);

    std::vector<models::ImageSample<PointT, N>> result;
    result.reserve(csv_rows.size());

    const size_t expected_row_length = N * N + 1; // NxN + class type 
    for (auto& row : csv_rows) {
        if (row.size() != expected_row_length) {
            throw std::runtime_error("invalid CSV row length");
        }

        models::ImageSample<PointT, N> sample{
            {},           // image
            row.front(),  // img_class
        };
        for (size_t i = 0; i < N; i++) {  
            for (size_t j = 0; j < N; j++) {
                sample.image.points[i][j] = row[i * N + j + 1];
            }
        }

        result.emplace_back(std::move(sample));
    }

    return result;
}

} // namespace ml::scanners
