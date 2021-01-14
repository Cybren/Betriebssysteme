#pragma once

#include <queue>
#include <mutex>
#include <semaphore.h>

class Verbraucher {
public:
    Verbraucher(std::queue<std::string>* queue, std::mutex* mutex, sem_t* semaphoreLeer, sem_t* semaphoreBelegt, int time);
    void run();
private:
    std::string entnehmen();
    void konsumieren(std::string kuchen);
    int time;
    int id;
    std::queue<std::string>* queue;
    std::mutex* mutex;
    sem_t* semaphoreLeer;
    sem_t* semaphoreBelegt;

};