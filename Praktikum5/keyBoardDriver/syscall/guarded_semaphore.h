
#pragma once
#include "meeting/semaphore.h"
#include "guard/secure.h"

class Guarded_Semaphore
        : public Semaphore {
    Guarded_Semaphore(const Guarded_Semaphore &) = delete;
    Guarded_Semaphore &operator=(const Guarded_Semaphore &) = delete;
public:
    Guarded_Semaphore(int c)
            : Semaphore(c) {}

    void p() {
        Secure section;
        Semaphore::p();
    }

    void v() {
        Secure section;
        Semaphore::v();
    }
};

