#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <shared_mutex>


class Writer {
public:
    Writer(int queue, std::shared_mutex* mutex, int time);
    void run();
private:
    void einfuegen();
    int time;
    int id;
    int queue;
    std::shared_mutex* mutex;
    sem_t* semaphoreLeer;
    sem_t* semaphoreBelegt;

};





