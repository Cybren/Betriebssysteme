#include "Konditor.h"
#include "Verbraucher.h"
#include <cstdlib>
#include <mutex>
#include <queue>
#include <thread>
#include <unistd.h>

int main()
{

    std::queue<std::string>* queue = new std::queue<std::string>();
    std::mutex* mutex = new std::mutex;
    sem_t* semaphoreLeer = new sem_t();
    sem_t* semaphoreBelegt = new sem_t();

    //TODO initialisieren Sie die Semaphore mittles der Funktion sem_init mit den richtigen Werten
    sem_init(reinterpret_cast<sem_t *>(mutex), 0, 1);
    sem_init(semaphoreBelegt, 0, 0);
    sem_init(semaphoreLeer, 0, 5);

    std::thread* konditor1 = new std::thread(&Konditor::run, Konditor(queue, mutex, semaphoreLeer, semaphoreBelegt,2, 1));
    std::thread* konditor2 = new std::thread(&Konditor::run, Konditor(queue, mutex, semaphoreLeer, semaphoreBelegt,3, 2 ));
    std::thread* verbraucher = new std::thread(&Verbraucher::run, Verbraucher(queue, mutex, semaphoreLeer, semaphoreBelegt,1));

    while (true) {
        sleep(1);
    }

    delete mutex;
    sem_destroy(semaphoreLeer);
    sem_destroy(semaphoreBelegt);
    delete queue;
    konditor1->join();
    konditor2->join();
    verbraucher->join();
    delete konditor1;
    delete konditor2;
    delete verbraucher;

    return EXIT_SUCCESS;
}