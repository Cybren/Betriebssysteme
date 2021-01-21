
#pragma once
#include "object/queuelink.h"

class Gate {
    Gate(const Gate &) = delete;
    Gate &operator=(const Gate &) = delete;
    int epilogue_queued;
public:
    Gate() : epilogue_queued(0) {}

    QueueLink<Gate> queue_link;

    virtual ~Gate() {}

    virtual bool prologue() = 0;

    virtual void epilogue() {}

    bool set_queued() {
        int old = __sync_lock_test_and_set(&epilogue_queued, 1);
        return old == 0;
    };

    void set_dequeued() {
        __sync_lock_release(&epilogue_queued);
    };
};
