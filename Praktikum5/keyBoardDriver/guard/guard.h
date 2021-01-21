
#pragma once
#include "guard/gate.h"
#include "object/queue.h"

class Guard {
    Guard(const Guard &) = delete;
    Guard &operator=(const Guard &) = delete;
public:
    Guard() {}

private:
    Queue<Gate> epilogue_queue;
    int locked = 0;
    bool is_locked() const;
    void set_locked();
    void set_unlocked();
public:
    void enter();
    void leave();
    void relay(Gate *item);
};

extern Guard guard;
