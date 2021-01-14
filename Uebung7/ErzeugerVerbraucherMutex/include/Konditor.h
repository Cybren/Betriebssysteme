#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>

class Konditor {
public:
    Konditor(std::queue<std::string>* queue, std::mutex* mutex, sem_t* semaphoreLeer, sem_t* semaphoreBelegt, int time, int id);
    void run();
private:
    std::string backen();
    void einfuegen(std::string kuchen);
    int time;
    int id;
    std::queue<std::string>* queue;
    std::mutex* mutex;
    sem_t* semaphoreLeer;
    sem_t* semaphoreBelegt;
};