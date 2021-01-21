#pragma once

#include <queue>
#include <mutex>
#include <semaphore.h>
#include <shared_mutex>

class Reader {

public:

    Reader(int* database, std::shared_mutex& mutexShared, int id, int time);
    void run();
private:
    int id;
    int time;
    int* database;
    std::shared_mutex& mutexShared;
};





