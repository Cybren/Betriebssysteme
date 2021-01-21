#include "thread/idlethread.h"
#include "syscall/guarded_scheduler.h"
#include "machine/cpu.h"
#include "device/watch.h"
#include "debug/output.h"

extern Watch watch;

void IdleThread::action() {
    while (true) {
        CPU::disable_int();
        if (scheduler.isEmpty()) {
            watch.block();
            CPU::idle();
            watch.unblock();

        } else {
            CPU::enable_int();
            Guarded_Scheduler::resume();
        }
    }
}
