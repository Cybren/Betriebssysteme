
#pragma once
#include "types.h"
#include "guard/gate.h"

class Watch
        : public Gate {
    Watch(const Watch &) = delete;
    Watch &operator=(const Watch &) = delete;
private:
    uint32_t interv;
    uint32_t ticks;
    uint32_t div;
public:
    Watch() {}

    bool windup(uint32_t us);
    bool prologue();
    void epilogue();
    uint32_t interval();
    void activate();
    void block();
    void unblock();
};
