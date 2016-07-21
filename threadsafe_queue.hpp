#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>


namespace http_server {


template <typename T>
class threadsafe_queue
{
    std::queue<T>           queue_;
    std::mutex              mutex_;
    std::condition_variable cond_;

public:

    threadsafe_queue() = default;
    threadsafe_queue(const threadsafe_queue&) = delete;
    void operator=(const threadsafe_queue&) = delete;

    T pop()
    {
        std::unique_lock lock(mutex_);
        while (queue_.empty()) {

            cond_.wait(lock);
        }

        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    void push(const T &item) 
    {
        std::unique_lock lock(mutex_);
        queue_.push(item);
        lock.unlock();
        cond_.notify_one();
    }
};

}
