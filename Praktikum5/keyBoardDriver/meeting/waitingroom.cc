
#include "meeting/waitingroom.h"
#include "thread/scheduler.h"

Waitingroom::~Waitingroom() {
    Thread *customer;
    while ((customer = dequeue())) {
        scheduler.wakeup(customer);
    }
}

void Waitingroom::remove(Thread *customer) {
    Queue<Thread>::remove(customer);
}
