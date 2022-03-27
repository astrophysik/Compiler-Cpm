#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "token.h"

template<typename T>
struct pipe {
    std::queue<T> queue;
    std::mutex m;
    std::condition_variable cv;

    void push(T tokens) {
        std::unique_lock lock(m);
        queue.push(std::move(tokens));
        lock.unlock();
        cv.notify_one();
    }

    T pop() {
        std::unique_lock lock(m);
        while (queue.empty()) {
            cv.wait(lock);
        }
        auto res = queue.front();
        queue.pop();
        lock.unlock();
        return res;
    }
};