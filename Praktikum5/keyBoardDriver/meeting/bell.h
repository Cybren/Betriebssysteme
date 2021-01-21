
#pragma once
#include "meeting/waitingroom.h"
#include "object/queuelink.h"

class Bellringer;

class Bell
        : public Waitingroom {
    Bell(const Bell &) = delete;
    Bell &operator=(const Bell &) = delete;
private:
    friend class Bellringer;

    QueueLink<Bell> bellringer_link;
    unsigned int counter;
public:
    Bell()
            : counter(0) {}

    void ring();
    static void sleep(unsigned int ms);
};

