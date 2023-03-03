#pragma once

#include <stdexcept>
#include <vector>

#include <containers/bulk_container.hpp>


namespace bulk_server::containers {

template <typename T>
class StaticBulkContainer : public BulkContainer<T>{
public:
    StaticBulkContainer(size_t bulk_size) : bulk_size_(bulk_size) {}
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
    }

    std::list<T> GetItems() const {
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
    size_t bulk_size_;
    std::list<T> container_;
};

} // namespace bulk_server::containers
