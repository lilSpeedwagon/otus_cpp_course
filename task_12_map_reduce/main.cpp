#include <iostream>
#include <string>
#include <vector>

#include <map_reduce/map_reduce.hpp>


int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "invalid number of arguments\n"
                  << "usage: map_reduce <src> <map_size> <reduce_size>";
        return 1;  
    }

    const std::string input_path(argv[1]);
    const auto map_size = static_cast<size_t>(std::atoi(argv[2]));
    const auto reduce_size = static_cast<size_t>(std::atoi(argv[3]));

    // std::string input = ... TODO
    std::vector<std::string> input = {{""}};

    using map_reduce_t = map_reduce::MapReduce<std::string, std::string, std::string>;

    auto mapper = [](const std::string& item) { return item; };
    auto reducer = [](const std::string& item) { return item; };

    map_reduce_t map_reduce_instance(map_size, reduce_size);
    map_reduce_instance.Handle(input, mapper, reducer);

    return 0;
}
