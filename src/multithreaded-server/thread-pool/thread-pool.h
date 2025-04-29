#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include "task/task.h"

using namespace std;
using namespace netflex;

class ThreadPool {
public:
    ThreadPool(size_t threadCount, size_t maxQueueSize = 1000);
    ~ThreadPool();

    bool enqueue(function<void()> task, netflex::TaskPriority priority = netflex::TaskPriority::MEDIUM);

    void shutdown();

    size_t getQueueSize() const;
    size_t getMaxQueueSize() const;

private:
    std::vector<std::thread> workers;

    std::priority_queue<netflex::Task> taskQueue;

    mutable std::mutex queueMutex;
    std::condition_variable taskAvailable;

    std::atomic<bool> isRunning;
    size_t maxQueueSize;

    void workerLoop();
};
