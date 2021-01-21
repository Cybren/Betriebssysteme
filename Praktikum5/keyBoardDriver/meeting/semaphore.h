
#pragma once
#include "meeting/waitingroom.h"

class Semaphore
        : public Waitingroom {
    Semaphore(const Semaphore &) = delete;
    Semaphore &operator=(const Semaphore &) = delete;
    int counter;
public:
    Semaphore(int c = 0)
            : counter(c) {}

    //Warten auf das Freiwerden eines kritischen Abschnitts.
    void p();
    //Freigeben des kritischen Abschnitts.
    void v();
};

