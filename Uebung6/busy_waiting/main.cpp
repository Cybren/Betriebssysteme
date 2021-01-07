#include "Alternierend.h"
#include "Konto.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

int main()
{
    std::vector<std::thread*> threads;

    Alternierend::konto.setKontoStand(100.0);

    for (int i = 0; i < Alternierend::gesamtZahl; i++) {
        threads.push_back(new std::thread(&Alternierend::run, Alternierend(i)));
    }

    while (true) {
        sleep(1);
    }

    for (int i = 0; i < Alternierend::gesamtZahl; i++) {
        threads.at(i)->join();
        delete threads.at(i);
    }

    return EXIT_SUCCESS;
}