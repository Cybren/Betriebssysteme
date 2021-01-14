#include "Konditor.h"
#include <iostream>
#include <unistd.h>

Konditor::Konditor(std::queue<std::string>* queue, std::mutex* mutex, std::condition_variable* conditionVariableLeer,
        std::condition_variable* conditionVariableBelegt, int time, int id)
        : queue(queue)
        , mutex(mutex)
        , conditionVariableLeer(conditionVariableLeer)
        , conditionVariableBelegt(conditionVariableBelegt)
        , time(time)
        , id(id)
{
}

std::string Konditor::backen()
{
    std::cout << "[Konditor " << this->id << "]Backe Kuchen" << std::endl;
    return "Kuchen";
}

void Konditor::einfuegen(std::string kuchen)
{
    std::unique_lock<std::mutex> lk(*mutex);
    while(queue->size() >= 5) {
        conditionVariableBelegt->wait(lk);
    }    
    queue->push(kuchen);
    conditionVariableLeer->notify_all();
    //TODO implement me
}

void Konditor::run()
{
    while (true) {
        sleep(this->time);
        einfuegen(this->backen());
    }
}