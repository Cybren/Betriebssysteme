
#include "meeting/bell.h"
#include "meeting/bellringer.h"
#include "thread/scheduler.h"

extern Bellringer bellringer;

void Bell::ring() {
    Thread *customer;
    while ((customer = dequeue())) {
        scheduler.wakeup(customer);
    }
}

void Bell::sleep(unsigned int ms) {
    Bell tmp;
    bellringer.job(&tmp, ms);
    scheduler.block(&tmp);
}

