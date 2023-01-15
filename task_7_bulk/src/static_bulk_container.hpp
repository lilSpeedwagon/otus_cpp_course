#pragma once

#include <exception>
#include <vector>

#include <src/bulk_container.hpp>


namespace bulk {

template <typename T>
class StaticBulkContainer : public BulkContainer<T>{
public:
    StaticBulkContainer(size_t bulk_size) 
        : bulk_size_(bulk_size) {
        Init();
    }
    ~StaticBulkContainer() {}

    bool Add(const T& item) {
        if (item.size() >= bulk_size_) {
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
        return container_.size() == bulk_size_;
    }

private:
    void Init() {
        container_.reserve(bulk_size_);
    }

    size_t bulk_size_;
    std::vector<T> container_;
};

} // namespace bulk
