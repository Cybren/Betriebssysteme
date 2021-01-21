
#pragma once
#include "thread/thread.h"

class Dispatcher {
    Dispatcher(const Dispatcher &) = delete;
    Dispatcher &operator=(const Dispatcher &) = delete;
protected:
    Thread *life;

    void setActive(Thread *c) {
        life = c;
    }

public:
    Dispatcher()
            : life(0) {
    }

    Thread *active() {
        return life;
    }

    void go(Thread *first);
    void dispatch(Thread *next);
    static void kickoff(Thread *object);
};

