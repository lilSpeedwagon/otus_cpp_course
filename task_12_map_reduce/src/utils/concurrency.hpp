#pragma once

#include <functional>
#include <optional>
#include <thread>
#include <vector>


namespace map_reduce::utils {

/// @brief Runs handler_func in several parallel to process each element of `data` is a separate thread.
/// @tparam T type of input elements
/// @tparam U type of output elements, must be default constructible
/// @param data vector of items to process
/// @param handler_func functor to apply for every element
/// @return vector of processed elements
template <typename T, typename U>
std::vector<U> HandleInParallel(std::vector<T>&& items, std::function<U(T)> handler_func) {
    std::vector<std::thread> threads;
    std::vector<U> result;
    threads.reserve(items.size());
    result.reserve(items.size());
    
    // run one handler for every item
    for (const auto& item : items) {
        result.emplace_back(U());
        threads.emplace_back(
            [&result = result.back(), handler_func] (T&& item) {
                result = handler_func(std::move(item));
            }, 
            std::move(item)
        );
    }

    // await for all threads
    for (auto& t : threads) {
        t.join();
    }

    return result;
}

} // namespace map_reduce::utils
