#pragma once

#include <list>

#include <containers/bulk_container.hpp>


namespace bulk_server::containers {

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

    std::list<T> GetItems() const override {
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

} // namespace bulk_server::containers
