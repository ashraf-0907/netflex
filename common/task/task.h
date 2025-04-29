#pragma once 

#include <functional>

namespace netflex {

    enum class TaskPriority {
        LOW = 1,
        MEDIUM = 5,
        HIGH = 10
    };

    struct Task {
        TaskPriority priority;
        std::function<void()> work;

        bool operator<(const Task& other) const {
            return static_cast<int>(priority) < static_cast<int>(other.priority);
        }

        Task(TaskPriority p = TaskPriority::MEDIUM, std::function<void()> fn = {}): priority(p), work(std::move(fn)) {}
    };
}  
