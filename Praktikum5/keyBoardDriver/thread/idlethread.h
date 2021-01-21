
#pragma once
#include "thread/thread.h"

class IdleThread
        : public Thread {
public:
    IdleThread(void *tos) : Thread(tos) {}

    void action();
};
