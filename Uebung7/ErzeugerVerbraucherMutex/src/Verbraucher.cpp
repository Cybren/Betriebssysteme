#include "Verbraucher.h"
#include <iostream>
#include <unistd.h>

Verbraucher::Verbraucher(std::queue<std::string>* queue, std::mutex* mutex, sem_t* semaphoreLeer, sem_t* semaphoreBelegt, int time)
        : queue(queue)
        , mutex(mutex)
        , semaphoreLeer(semaphoreLeer)
        , semaphoreBelegt(semaphoreBelegt)
        , time(time)
{
}

std::string Verbraucher::entnehmen(){
    //TODO implement me

    sem_wait(semaphoreBelegt);
    sem_wait(reinterpret_cast<sem_t *>(mutex));

    std::string backwahre = queue->front();
    queue->pop();

    sem_post(reinterpret_cast<sem_t *>(mutex));
    sem_post(semaphoreLeer);

    return backwahre;
}

void Verbraucher::konsumieren(std::string kuchen)
{
    std::cout << "[Verbraucher]Verbrauche " + kuchen << std::endl;
    sleep(this->time);
}

void Verbraucher::run()
{
    while (true) {
        konsumieren(this->entnehmen());
    }
}
