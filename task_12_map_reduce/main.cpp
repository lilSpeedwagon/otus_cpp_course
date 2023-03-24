#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <map_reduce/map_reduce.hpp>
#include <utils/file.hpp>
#include <utils/string.hpp>


std::vector<std::string> ReadInputFile(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::string> result;
    std::string line;
    while (std::getline(file, line)) {    
        result.emplace_back(std::move(line));
    }
    return result;
}


int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "invalid number of arguments\n"
                  << "usage: map_reduce <src> <map_size> <reduce_size>";
        return 1;  
    }

    const std::string input_path(argv[1]);
    const auto map_size = static_cast<size_t>(std::atoi(argv[2]));
    const auto reduce_size = static_cast<size_t>(std::atoi(argv[3]));

    auto input = ReadInputFile(input_path);

    using map_reduce_t = map_reduce::MapReduce<
        std::string,  // InputT
        std::string,  // KeyT
        std::string,  // MapT
        std::string   // ReduceT
    >;

    auto mapper = [](map_reduce_t::splitted_t&& items) {
        map_reduce_t::mapped_t map;
        
        // find min prefix required to identify string among mapped block items
        for (size_t i = 0; i < items.size(); i++) {
            const auto& str = items[i];
            size_t prefix_size = 1;
            
            for (size_t j = 0; j < items.size(); j++) {
                if (i == j) {
                    continue;
                }

                const auto common_prefix_size = map_reduce::utils::MaxPrefixSize(str, items[j]);
                if (common_prefix_size > prefix_size) {
                    prefix_size = common_prefix_size + 1;
                }
            }

            auto prefix = str.substr(0, prefix_size);
            map[std::move(prefix)] = str;
        }

        return map;
    };

    auto reducer = [](map_reduce_t::shuffled_t&& map) {
        map_reduce_t::reduced_t result;
        
        // write result to file
        auto file_name = map_reduce::utils::GenerateFileName();
        std::ofstream file(file_name);
        
        // find min prefix required to identify string among items with common prefixes
        for (auto& [prefix, items] : map) {
            
            for (size_t i = 0; i < items.size(); i++) {
                size_t prefix_size = prefix.size();
                const auto& str = items[i];

                for (size_t j = 0; j < items.size(); j++) {
                    if (i == j) {
                        continue;
                    }

                    const auto common_prefix_size = map_reduce::utils::MaxPrefixSize(str, items[j], prefix.size());
                    if (common_prefix_size > prefix_size) {
                        prefix_size = common_prefix_size + 1;
                    }
                }

                auto min_prefix = str.substr(0, prefix_size);
                file << min_prefix << '\n';
                result.emplace_back(std::move(min_prefix));
            }
        }

        return result;
    };

    map_reduce_t map_reduce_instance(map_size, reduce_size, mapper, reducer);
    auto result = map_reduce_instance.Handle(std::move(input));

    for (const auto& item : result) {
        std::cout << item << '\n';
    }

    return 0;
}
