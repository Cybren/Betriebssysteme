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

    int database = 0;
    std::shared_mutex mutex;

    std::thread* reader1 = new std::thread(&Reader::run, Reader(&database, mutex, 1, 1));
    std::thread* reader2 = new std::thread(&Reader::run, Reader(&database, mutex, 2, 2 ));
    std::thread* writer = new std::thread(&Writer::run, Writer(&database, mutex));

    while (true) {
        sleep(1);
    }

    reader1->join();
    reader2->join();
    writer->join();
    delete reader1;
    delete reader2;
    delete writer;

    return EXIT_SUCCESS;
}