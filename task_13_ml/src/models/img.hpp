#pragma once

#include <array>


namespace ml::models {

template<typename PointT, size_t N>
struct Image {
    using point_t = PointT;
    static const size_t side_size = N;

    std::array<std::array<PointT, N>, N> points;
};

template<typename PointT, size_t N>
struct ImageSample {
    Image<PointT, N> image;
    int img_class;
};

} // namespace ml::models
