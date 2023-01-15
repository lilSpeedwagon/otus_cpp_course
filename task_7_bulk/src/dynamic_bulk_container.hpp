#pragma once

#include <list>

#include <src/bulk_container.hpp>


namespace bulk {

template <typename T>
class DynamicBulkContainer : public bulk::BulkContainer<T> {
public:
    DynamicBulkContainer() {}
    ~DynamicBulkContainer() {}

    bool Add(const T& item) override {
        container_.push_back(item);
        return !IsFull();
    }

    void Clear() override {
        container_.clear();
    }

    std::vector<T> GetItems() const override {
        std::vector<T> result;
        std::copy(container_.begin(), container_.end(), std::back_inserter(result));
        return container_;
    }

    size_t Size() const override {
        return container_.size();
    }

    bool IsFull() const  override {
        return false;
    }

private:
    std::list<T> container_;
};

} // namespace bulk
