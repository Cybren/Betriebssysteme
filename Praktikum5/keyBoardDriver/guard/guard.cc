
#include "guard/guard.h"
#include "machine/cpu.h"
#include "debug/output.h"

bool Guard::is_locked() const {
    return (locked != 0);
}

void Guard::set_locked() {
    locked++;
}

void Guard::set_unlocked() {
    locked--;
}

void Guard::enter() {
    set_locked();
}

void Guard::leave() {
    bool status;
    for (;;) {
        status = CPU::disable_int();
        Gate *iter = epilogue_queue.dequeue();
        if (iter == 0) {
            break;
        }
        iter->set_dequeued();
        CPU::restore_int(status);
        iter->epilogue();
    }
    set_unlocked();
    CPU::restore_int(status);
}

void Guard::relay(Gate *item) {
    if (is_locked()) {
        if (item->set_queued()) {
            epilogue_queue.enqueue(item);
        }
    } else {
        set_locked();
        CPU::enable_int();
        item->epilogue();
        leave();
    }
}

Guard guard;
