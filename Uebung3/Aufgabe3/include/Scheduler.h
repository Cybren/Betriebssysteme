#pragma once

#include "Task.h"
#include <queue>

class Scheduler
{
public:
    Scheduler(std::queue<Task>* taskQueue);
    void schedule();

private:
    std::queue<Task>* taskQueue;
};
