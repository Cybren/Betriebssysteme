
#pragma once
#include "guard/gate.h"

class Plugbox {
private:
    Plugbox(const Plugbox &) = delete;
    Plugbox &operator=(const Plugbox &) = delete;
    enum {
        size = 256
    };
    Gate *map[size];
public:
    enum Vector {
        timer = 32,
        keyboard = 33,
        serial = 36,
        assassin = 100,
        wakeup = 101
    };
    Plugbox();
    void assign(unsigned int vector, Gate *gate);
    Gate *report(unsigned int vector);
};
