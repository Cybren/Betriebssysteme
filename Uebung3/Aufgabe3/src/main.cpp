#include "Scheduler.h"
#include "Task.h"
#include <queue>
#include <thread>
#include <unistd.h>

void enqueueTasks(std::queue<Task>* taskQueue)
{
    //TODO implement me
}

int main()
{
    std::queue<Task>* taskQueue = new std::queue<Task>();

    Scheduler* scheduler = new Scheduler(taskQueue);

    std::thread thread(enqueueTasks, taskQueue);

    scheduler->schedule();

    thread.join();

    delete taskQueue;
    delete scheduler;
    return 0;
}