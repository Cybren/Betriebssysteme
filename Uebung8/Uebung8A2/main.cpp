#include <iostream>

#include <queue>
#include <thread>
#include <unistd.h>
#include <semaphore.h>
#include <shared_mutex>
#include "Reader.h"
#include "Writer.h"

int main()
{

    int queue = 0;
    std::shared_mutex* mutex = new std::shared_mutex;

    std::thread* reader1 = new std::thread(&Reader::run, Reader(queue, mutex,2, 1));
    std::thread* reader2 = new std::thread(&Reader::run, Reader(queue, mutex,3, 2 ));
    std::thread* writer = new std::thread(&Writer::run, Writer(queue, mutex,1));;

    while (true) {
        sleep(1);
    }

    delete mutex;
    reader1->join();
    reader2->join();
    writer->join();
    delete reader1;
    delete reader2;
    delete writer;

    return EXIT_SUCCESS;
}