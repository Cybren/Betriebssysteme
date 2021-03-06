#include "Verbraucher.h"
#include <iostream>
#include <unistd.h>

Verbraucher::Verbraucher(std::queue<std::string>* queue, std::mutex* mutex, std::condition_variable* conditionVariableLeer,
        std::condition_variable* conditionVariableBelegt, int time)
        : queue(queue)
        , mutex(mutex)
        , conditionVariableLeer(conditionVariableLeer)
        , conditionVariableBelegt(conditionVariableBelegt)
        , time(time)
{
}

std::string Verbraucher::entnehmen()
{
    std::unique_lock<std::mutex> lk(*mutex);
    while(queue->size() <= 0) {
        conditionVariableLeer->wait(lk);
    }
    std::string kuchen = queue->front();
    queue->pop();
    conditionVariableBelegt->notify_one();
    return kuchen;
    //TODO implement me
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
