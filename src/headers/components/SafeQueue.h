//
// Created by Bulut Gözübüyük on 1.06.2021.
//

#ifndef SIYAH1_SAFEQUEUE_H
#define SIYAH1_SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

// A thread safe queue.
template<class T>
class SafeQueue {
public:
//    SafeQueue(void)
    SafeQueue() : q(), m(), c() {}
//
//    ~SafeQueue(void) {}

    // Add an element to the queue.
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    // T dequeue(void) {
    T dequeue() {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty()) {
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

    T get(int value) {
        std::unique_lock<std::mutex> lock(m);

        std::chrono::milliseconds timeout = std::chrono::milliseconds(value);

        if (!c.wait_for(lock, timeout, [this] { return !q.empty(); }))
            return std::string();

//      filename = std::move(q.front());
        T val = q.front();
        q.pop();
        return val;
    }

    T get2(const std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(m);

        while (q.empty()) {
            if (c.wait_for(lock, timeout) == std::cv_status::timeout) return std::string();
        }
//        std::string ret = q.front();
//        q.pop();
//        return ret;
        T val = q.front();
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};


#endif //SIYAH1_SAFEQUEUE_H
