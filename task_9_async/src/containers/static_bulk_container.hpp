#pragma once

#include <stdexcept>
#include <vector>

#include <containers/bulk_container.hpp>


namespace async::containers {

template <typename T>
class StaticBulkContainer : public BulkContainer<T>{
public:
    StaticBulkContainer(size_t bulk_size) 
        : bulk_size_(bulk_size) {
        Init();
    }
    virtual ~StaticBulkContainer() = default;

    bool Add(const T& item) {
        if (IsFull()) {
            throw std::runtime_error("bulk size exceeded");
        }
        container_.push_back(item);
        return !IsFull();
    }

    void Clear() {
        container_.clear();
        Init();
    }

    std::vector<T> GetItems() const {
        return container_;
    }

    size_t Size() const {
        return container_.size();
    }

    size_t BulkSize() const {
        return bulk_size_;
    }

    bool IsFull() const {
        return container_.size() >= bulk_size_;
    }

private:
    void Init() {
        container_.reserve(bulk_size_);
    }

    size_t bulk_size_;
    std::vector<T> container_;
};

} // namespace async::containers
