#pragma once

#include <cmath>
#include <functional>
#include <list>
#include <vector>
#include <unordered_map>

#include <utils/concurrency.hpp>


namespace map_reduce {

// [input] -- split --> [blocks] -- map --> [mapped blocks] -- sort --> [sorted blocks] -- reduce --> [result]

template<typename InputT, typename KeyT, typename MapT, typename ReduceT>
class MapReduce {
public:
    using splitted_t = std::vector<InputT>;
    using mapped_t = std::unordered_map<KeyT, MapT>;
    using shuffled_t = std::unordered_map<KeyT, std::vector<MapT>>;
    using reduced_t = std::vector<ReduceT>;

    using map_function_t = std::function<mapped_t(splitted_t)>;
    using reduce_function_t = std::function<reduced_t(shuffled_t)>;

    MapReduce(size_t map_concurrency_size, size_t reduce_concurrency_size, 
              map_function_t mapper, reduce_function_t reducer)
        : map_concurrency_size_(map_concurrency_size), reduce_concurrency_size_(reduce_concurrency_size),
          mapper_(mapper), reducer_(reducer) {}
    ~MapReduce() = default;

    reduced_t Handle(std::vector<InputT>&& input) {
        auto splitted_data_blocks = Split(std::move(input));
        auto mapped_data_blocks = Map(std::move(splitted_data_blocks));
        auto shuffled_data_blocks = Shuffle(std::move(mapped_data_blocks));
        auto reduced_data = Reduce(std::move(shuffled_data_blocks));
        return reduced_data;
    }

private:
    std::vector<splitted_t> Split(std::vector<InputT>&& input) {
        std::vector<splitted_t> result;
        result.reserve(map_concurrency_size_);
        const size_t block_size = std::ceil(static_cast<float>(input.size()) / map_concurrency_size_);

        size_t i = 0;
        while (i < input.size()) {
            result.emplace_back();
            auto& block = result.back();
            block.reserve(block_size);
            for (; i < input.size() && block.size() < block_size; i++) {
                auto& item = input[i];
                block.emplace_back(std::move(item));
            }
        }

        return result;
    }

    std::vector<mapped_t> Map(std::vector<splitted_t>&& input) {
        auto mapped_blocks = utils::HandleInParallel<splitted_t, mapped_t>(std::move(input), mapper_);
        return mapped_blocks;
    }

    std::vector<shuffled_t> Shuffle(std::vector<mapped_t>&& input) {
        // distribute items over flat map
        shuffled_t items_map;
        for (auto& block : input) {
            for (auto& [key, value] : block) {
                auto bucket_it = items_map.find(key);
                if (bucket_it == items_map.end()) {
                    auto inserted = items_map.insert({std::move(key), {}});
                    bucket_it = inserted.first;
                }
                bucket_it->second.push_back(std::move(value));
            }
        }

        // prepare resulting vector with size equal to the number of reduce tasks
        // every item consists of several map keys with corresponding data blocks
        std::vector<shuffled_t> result;
        result.reserve(reduce_concurrency_size_);
        for (size_t i = 0; i < reduce_concurrency_size_; i++) {
            result.emplace_back();
        }

        // distribute key-values across reduce task data blocks
        size_t block_number = 0;
        for (auto& [key, bucket] : items_map) {
            std::sort(bucket.begin(), bucket.end(), std::less<MapT>{});
            result[block_number][std::move(key)] = std::move(bucket);
            block_number++;
            if (block_number >= reduce_concurrency_size_) {
                block_number = 0;
            }
        }

        return result;
    }

    reduced_t Reduce(std::vector<shuffled_t>&& input) {
        auto reduced_blocks = utils::HandleInParallel<shuffled_t, reduced_t>(std::move(input), reducer_);
        
        // merge results
        std::vector<ReduceT> result;
        for (auto& block : reduced_blocks) {
            for (auto& item : block) {
                result.push_back(std::move(item));
            }
        }
        return result;
    }

    size_t map_concurrency_size_;
    size_t reduce_concurrency_size_;
    map_function_t mapper_;
    reduce_function_t reducer_;
};

} // namespace map_reduce
