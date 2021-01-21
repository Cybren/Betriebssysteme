
#pragma once
#include "object/queue.h"

class Thread;

class Waitingroom
        : public Queue<Thread> {
    Waitingroom(const Waitingroom &) = delete;
    Waitingroom &operator=(const Waitingroom &) = delete;
public:
    Waitingroom() {}

    virtual ~Waitingroom();
    virtual void remove(Thread *customer);
};

