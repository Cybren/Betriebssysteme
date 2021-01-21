
#pragma once
#include "thread/thread.h"
#include "guard/secure.h"
#include "thread/scheduler.h"

class Guarded_Scheduler {
public:
    static void ready(Thread *that) {
        Secure section;
        scheduler.ready(that);
    }

    static void exit() {
        Secure section;
        scheduler.exit();
    }

    static void kill(Thread *that) {
        Secure section;
        scheduler.kill(that);
    }

    static void resume() {
        Secure section;
        scheduler.resume();
    }
};
