
#include "meeting/bellringer.h"
#include "meeting/bell.h"
#include "device/watch.h"

extern Watch watch;

void Bellringer::check() {
    Bell *bell;
    bell = bells.first();
    if (bell) {
        bell->counter--;
        while ((bell = bells.first()) && bell->counter == 0) {
            bells.dequeue();
            bell->ring();
        }
    }
}

void Bellringer::job(Bell *bell, unsigned int ms) {
    unsigned int ticks = ms * 1000 / watch.interval();
    if (!bells.first()) {
        bells.enqueue(bell);
    } else {
        Bell *next = 0, *pred = 0;
        for (Bell *p : bells) {
            if (ticks < p->counter) {
                next = p;
                break;
            }
            ticks -= p->counter;
            pred = p;
        }
        if (pred) {
            bells.insert_after(pred, bell);
        } else {
            bells.insert_first(bell);
        }
        if (next) {
            next->counter -= ticks;
        }
    }
    bell->counter = ticks;
}

void Bellringer::cancel(Bell *bell) {
    for (Bell *curr : bells) {
        if (curr == bell) {
            Bell *followup = bells.next(bell);
            if (followup) {
                followup->counter += bell->counter;
            }
            bells.remove(bell);
            return;
        }
    }
}

bool Bellringer::bell_pending() {
    return bells.first() != nullptr;
}

