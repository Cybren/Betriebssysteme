#pragma once

#include <queue>
#include <mutex>
#include <semaphore.h>
#include <shared_mutex>

class Reader {

public:

    Reader(int queue, std::shared_mutex* mutex, int time, int id);
    void run();
private:
    std::string entnehmen();
    void konsumieren(std::string kuchen);
    int time;
    int id;
    int queue;
    std::shared_mutex* mutex;
    sem_t* semaphoreLeer;
    sem_t* semaphoreBelegt;
};





