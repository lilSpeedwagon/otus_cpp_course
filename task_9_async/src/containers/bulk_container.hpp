#pragma once

#include <vector>


namespace bulk {

/// @brief Bulk container interface. Provides base functions to store
/// bulk items for the further handling.
/// @tparam T type of bulk items to store
template<typename T>
class BulkContainer {
public:
    BulkContainer() = default;
    virtual ~BulkContainer() = default;

    /// @brief Add new item to container.
    /// @return whether it is possible to add more items to container
    virtual bool Add(const T& item) = 0;

    /// @brief Clear the container
    virtual void Clear() = 0;

    /// @brief Get a vector of stored items
    virtual std::vector<T> GetItems() const = 0;

    /// @brief Get number of the stored items
    virtual size_t Size() const = 0;

    /// @brief Returns true if container us full.
    virtual bool IsFull() const = 0;
};

} // namespace bulk
