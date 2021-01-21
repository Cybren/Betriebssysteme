
#pragma once
#include "thread/dispatcher.h"
#include "thread/thread.h"
#include "object/queue.h"
#include "machine/apicsystem.h"
#include "meeting/waitingroom.h"

class Scheduler
        : public Dispatcher {
    Scheduler(const Scheduler &) = delete;
    Scheduler &operator=(const Scheduler &) = delete;
    void dispatchNext();
private:
    Queue<Thread, &Thread::queue_link> readylist;
public:
    Scheduler() {}

    void schedule();
    void ready(Thread *that);
    void exit();
    void kill(Thread *that);
    void resume();
    Thread *idlethread[CPU_MAX];
public:
    bool isEmpty() {
        return readylist.first() == 0;
    }

    void set_idle_thread(int cpuid, Thread *thread) {
        idlethread[cpuid] = thread;
    }

public:
    void block(Waitingroom *waitingroom);
    void wakeup(Thread *customer);

};

extern Scheduler scheduler;
