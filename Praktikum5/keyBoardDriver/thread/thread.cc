
#include "thread/thread.h"
#include "thread/dispatcher.h"

Thread::Thread(void *tos) {
    toc_settle(&regs, tos, Dispatcher::kickoff, this);
    kill_flag = false;
    waitingroom = 0;

}

void Thread::resume(Thread *next) {
    toc_switch(&regs, &(next->regs));
}

void Thread::go() {
    toc_go(&regs);
}

Waitingroom *Thread::waiting_in() {
    return waitingroom;
}

void Thread::waiting_in(Waitingroom *w) {
    waitingroom = w;
}

