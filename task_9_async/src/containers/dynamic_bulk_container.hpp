#pragma once

#include <list>

#include <containers/bulk_container.hpp>


namespace async::containers {

template <typename T>
class DynamicBulkContainer : public BulkContainer<T> {
public:
    DynamicBulkContainer() = default;
    virtual ~DynamicBulkContainer() = default;

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
        return result;
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

} // namespace async::containers
