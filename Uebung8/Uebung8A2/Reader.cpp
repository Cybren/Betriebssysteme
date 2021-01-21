#include "Reader.h"
#include <iostream>
#include <unistd.h>
#include <shared_mutex>


Reader::Reader(int* database, std::shared_mutex& mutexShared, int id, int time)
        : database(database)
        , mutexShared(mutexShared)
        , id(id)
        , time(time)
    {
}

void Reader::run(){
    while (true) {
        sleep(time);
        std::shared_lock lock(mutexShared);
        std::cout << "Reader(" << id <<  "): database value is: " << *database << std::endl;
    }
}
