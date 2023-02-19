#pragma once

#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <queue>
#include <unordered_map>


namespace async::pubsub {

/// @brief Queue implementing publisher-subscriber patter.
/// Some threads may subscribe and wait for new items in the queue.
/// Other threads may push new items to the queue.
/// @tparam T item type
template<typename T>
class Queue {
public:
    Queue() = default;
    ~Queue() = default;

    /// @brief Push a new item to the queue. 
    /// May be blocked on mutex while somebody else read/write to the queue
    /// @param item new item
    void Push(const T& item) {
        std::scoped_lock lock(mutex_);
        queue_.push(item);
        cv_.notify_one();
    }

    /// @brief Read the next element in the queue
    /// This method blocks if queue is empty and waits until a new item appears.
    /// @return next item in queue
    T PopNext() {
        // check whether the queue already has items
        { 
            std::scoped_lock scope(mutex_);
            if (!queue_.empty()) {
                return Pop();
            }
        }

        // else wait for new items
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [&queue = this->queue_]() { return !queue.empty(); });
        return Pop();
    }

private:
    T Pop() {
        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
};

} // namespace async::pubsub
