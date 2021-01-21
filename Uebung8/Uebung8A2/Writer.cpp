#include "Writer.h"
#include <iostream>
#include <unistd.h>

Writer::Writer(int* database, std::shared_mutex& mutexShared)
        : database(database)
        , mutexShared(mutexShared)
    {
}

void Writer::run(){
    while (true) {
        sleep(2);
        std::unique_lock lock(mutexShared);
        (*database)++;
        std::cout << "increment counter to: " << *database << std::endl;
    }
}
