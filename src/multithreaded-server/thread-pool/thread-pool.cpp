#include "./thread-pool.h"
#include <logger/logger.h>

ThreadPool::ThreadPool(size_t threadCount, size_t maxQueueSize):
isRunning(true), maxQueueSize(maxQueueSize){
    for (size_t i =0 ; i< threadCount; i++){
        workers.emplace_back([this]() {workerLoop();});
    }
}

ThreadPool::~ThreadPool(){
    this->shutdown();
}

bool ThreadPool::enqueue(function<void()> task, TaskPriority priority){
    unique_lock<mutex> lock(queueMutex);
    bool response = false;
    if(this->isRunning, this->taskQueue.size() < this->maxQueueSize){
        taskQueue.emplace(priority, task); 
        taskAvailable.notify_one();
        response = true;
    }

    return response;
}

void ThreadPool::shutdown(){
    unique_lock<mutex> lock(queueMutex);
    isRunning = false;

    taskAvailable.notify_all();

    for (auto& thread : workers) {
        if (thread.joinable()) {
            thread.join();
        }
    }

}

void ThreadPool::workerLoop() {
    while (true) {
        Task currentTask;

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            taskAvailable.wait(lock, [this]() {
                return !taskQueue.empty() || !isRunning;
            });

            if (!isRunning && taskQueue.empty()) {
                return; 
            }

            currentTask = std::move(taskQueue.top());
            taskQueue.pop();
        }

        try {
            if (currentTask.work) {
                currentTask.work();
            }
        } catch (const std::exception& e) {
            LOG_ERROR("[ThreadPool] Exception in task: ");
        }
    }
}


size_t ThreadPool::getQueueSize() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return taskQueue.size();
}

size_t ThreadPool::getMaxQueueSize() const {
    return maxQueueSize;
}