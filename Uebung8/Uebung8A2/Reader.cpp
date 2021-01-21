#include "Reader.h"
#include <iostream>
#include <unistd.h>
#include <shared_mutex>


Reader::Reader(int queue, std::shared_mutex* mutex, int time, int id)
        : queue(queue)
        , mutex(mutex)
        , time(time)
        , id(id)
    {
}

std::string Reader::entnehmen(){
    std::shared_lock lock(mutex);



}

void Reader::konsumieren(std::string kuchen)
{
    std::cout << "[Verbraucher]Verbrauche " + kuchen << std::endl;
    sleep(this->time);
}

void Reader::run()
{
    while (true) {
        konsumieren(this->entnehmen());
    }
}
