#include "Scheduler.h"
#include "Task.h"
#include <queue>
#include <thread>
#include <unistd.h>

void enqueueTasks(std::queue<Task>* taskQueue)
{
    Task B("B", 3);
    Task F("F", 2);
    taskQueue->push(B);
    taskQueue->push(F);
    sleep(1);
    Task E("E", 4);
    taskQueue->push(E);
    sleep(1);
    Task A("A", 1);
    Task D("D", 2);
    taskQueue->push(A);
    taskQueue->push(D);
    sleep(2);
    Task C("C", 4);
    taskQueue->push(C);
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