#include "Konditor.h"
#include <iostream>
#include <unistd.h>

Konditor::Konditor(std::queue<std::string>* queue, std::mutex* mutex, sem_t* semaphoreLeer, sem_t* semaphoreBelegt, int time, int id)
        : queue(queue)
        , mutex(mutex)
        , semaphoreLeer(semaphoreLeer)
        , semaphoreBelegt(semaphoreBelegt)
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
    sem_wait(semaphoreBelegt);
    queue->push(kuchen);
    sem_post(semaphoreLeer);
}

void Konditor::run()
{
    while (true) {
        sleep(this->time);
        einfuegen(this->backen());
    }
}