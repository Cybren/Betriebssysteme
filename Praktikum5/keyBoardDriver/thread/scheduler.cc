
#include "thread/scheduler.h"
#include "guard/guard.h"
#include "machine/plugbox.h"
#include "machine/cpu.h"
#include "debug/output.h"

extern Guard guard;

void Scheduler::schedule() {
    Thread *first = readylist.dequeue();
    if (!first) {
        first = idlethread[system.getCPUID()];
        DBG << "Idling in Scheduler::schedule" << endl;
    }
    go(first);
}

void Scheduler::ready(Thread *that) {
    readylist.enqueue(that);
}

void Scheduler::dispatchNext() {
    Thread *next = readylist.dequeue();
    if (!next) {
        next = idlethread[system.getCPUID()];
    }
    dispatch(next);
}

void Scheduler::resume() {
    Thread *me = active();
    if (me->dying()) {
        me->reset_kill_flag();
    } else {
        if (!readylist.first()) {
            return;
        } else if (me != idlethread[system.getCPUID()]) {
            readylist.enqueue(me);
        }
    }
    dispatchNext();
}

void Scheduler::exit() {
    dispatchNext();
}

void Scheduler::block(Waitingroom *waitingroom) {
    Thread *me = active();
    waitingroom->enqueue(me);
    me->waiting_in(waitingroom);
    dispatchNext();
}

void Scheduler::wakeup(Thread *customer) {
    customer->waiting_in(0);
    ready(customer);
}

void Scheduler::kill(Thread *that) {
    that->set_kill_flag();
    Waitingroom *waitingroom = that->waiting_in();
    if (waitingroom) {
        waitingroom->remove(that);
        that->waiting_in(0);
        return;
    }
    readylist.remove(that);
}

Scheduler scheduler;
