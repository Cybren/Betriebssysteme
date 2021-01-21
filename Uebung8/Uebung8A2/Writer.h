#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <shared_mutex>


class Writer {
public:
    Writer(int* database, std::shared_mutex& mutexShared);
    void run();
private:
    int* database;
    std::shared_mutex& mutexShared;
};





