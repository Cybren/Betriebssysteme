
#include "thread/dispatcher.h"
#include "thread/thread.h"
#include "guard/guard.h"
#include "debug/output.h"
#include "debug/kernelpanic.h"

extern Guard guard;

void Dispatcher::go(Thread *first) {
    if (!active()) {
        setActive(first);
        first->go();
    }
}

void Dispatcher::dispatch(Thread *next) {
    Thread *current = active();
    if (current == next) return;
    setActive(next);
    current->resume(next);
}

void Dispatcher::kickoff(Thread *object
) {
    guard.leave();
    object->action();
    kernelpanic("Application should not return in kickoff");
}

