#include "Konditor.h"
#include "Verbraucher.h"
#include <cstdlib>
#include <mutex>
#include <queue>
#include <thread>
#include <unistd.h>
#include <condition_variable>

int main()
{

    std::queue<std::string>* queue = new std::queue<std::string>();
    std::mutex* mutex = new std::mutex;
    std::condition_variable* conditionVariableLeer = new std::condition_variable();
    std::condition_variable* conditionVariableBelegt = new std::condition_variable();
    std::thread* konditor1 = new std::thread(&Konditor::run, Konditor(queue, mutex, conditionVariableLeer, conditionVariableBelegt,2, 1));
    std::thread* konditor2 = new std::thread(&Konditor::run, Konditor(queue, mutex, conditionVariableLeer, conditionVariableBelegt,3, 2 ));
    std::thread* verbraucher = new std::thread(&Verbraucher::run, Verbraucher(queue, mutex, conditionVariableLeer, conditionVariableBelegt,1));;

    while (true) {
        sleep(1);
    }

    delete mutex;
    delete conditionVariableBelegt;
    delete conditionVariableLeer;
    delete queue;
    konditor1->join();
    konditor2->join();
    verbraucher->join();
    delete konditor1;
    delete konditor2;
    delete verbraucher;

    return EXIT_SUCCESS;
}