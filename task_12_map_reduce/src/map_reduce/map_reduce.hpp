#pragma once

#include <functional>
#include <vector>

#include <utils/concurrency.hpp>


namespace map_reduce {

// [input] -- split --> [blocks] -- map --> [mapped blocks] -- sort --> [sorted blocks] -- reduce --> [result]

template<typename InputT, typename KeyT, typename MapT, typename ReduceT>
class MapReduce {
public:
    using splitted_t = std::vector<InputT>;
    using mapped_t = std::unordered_map<KeyT, MapT>;
    using reduced_t = std::vector<ReduceT>;

    using map_function_t = std::function<mapped_t(splitted_t)>;
    using reduce_function_t = std::function<reduced_t(mapped_t)>;

    MapReduce(size_t map_concurrency_size, size_t reduce_concurrency_size, 
              map_function_t mapper, reduce_function_t reducer)
        : map_concurrency_size_(map_concurrency_size), reduce_concurrency_size_(reduce_concurrency_size),
          mapper_(mapper), reducer_(reducer) {}
    ~MapReduce() = default;

    reduced_t Handle(std::vector<T>&& input, ) {
        auto splitted_data_blocks = Split(std::move(input));
        auto mapped_data_blocks = Map(splitted_data_blocks);
        auto shuffled_data_blocks = Shuffle(mapped_data_blocks);
        auto reduced_data = Reduce(shuffled_data_blocks);
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
        auto mapped_blocks = utils::HandleInParallel<splitted_t, mapped_t>(input, mapper_);
        for (auto& block : mapped_blocks) {
            std::sort(block.begin(), block.end());
        }
        return mapped_blocks;
    }

    std::vector<mapped_t> Shuffle(std::vector<mapped_t>&& input) {
        // TODO shuffle items with the same key to the same buckets
    }

    reduced_t Reduce(std::vector<mapped_t>&& input) {
        // TODO apply reducer_() for every bucket and collect results
    }

    size_t map_concurrency_size_;
    size_t reduce_concurrency_size_;
    map_function_t mapper_;
    reduce_function_t reducer_;
};

} // namespace map_reduce
