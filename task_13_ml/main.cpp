#include <filesystem>
#include <iostream>

#include <models/img.hpp>
#include <scanners/img.hpp>


int main(/*int argc, char** argv*/) {

    // expected 28x28 images with 1 byte per pixel
    using img_t = ml::models::Image<uint8_t, 28>;

    std::filesystem::path path = "../../task_13_ml/data/test_data_catboost.txt";
    auto images = ml::scanners::ScanImages<img_t::point_t, img_t::side_size>(path);

    for (const auto& img : images) {
        std::cout << "img class " << img.img_class << '\n';
        for (const auto& line : img.image.points) {
            for (const auto& pxl : line) {
                std::cout << pxl << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
