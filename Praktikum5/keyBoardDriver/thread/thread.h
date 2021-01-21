
#pragma once
#include "machine/toc.h"

class Waitingroom;

#include "object/queuelink.h"

class Thread {
public:
    Thread(void *tos);
    QueueLink<Thread> queue_link;
private:
    struct toc regs;
public:
    void go();
    void resume(Thread *next);
    virtual void action() = 0;
private:
    volatile bool kill_flag;
public:
    void set_kill_flag() {
        kill_flag = true;
    }

    void reset_kill_flag() {
        kill_flag = false;
    }

    bool dying() {
        return kill_flag;
    }

public:
    Waitingroom *waitingroom;
    Waitingroom *waiting_in();
    void waiting_in(Waitingroom *w);
};
