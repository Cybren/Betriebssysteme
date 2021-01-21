#include "meeting/semaphore.h"
#include "thread/scheduler.h"

void Semaphore::p() {
    if (counter == 0) {
        scheduler.block(this);
    } else {
        counter--;
    }
}

void Semaphore::v() {
    Thread *customer = dequeue();
    if (customer) {
        scheduler.wakeup(customer);
    } else {
        counter++;
    }
}
