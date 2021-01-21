
#include "device/cgastr.h"
#include "debug/output.h"
#include "user/app/kappl.h"

CGA_Stream kout(0, 79, 0, 16, true);
CGA_Stream dout(0, 79, 17, 24);
#include "machine/ioapic.h"
#include "machine/cpu.h"
#include "machine/plugbox.h"
#include "debug/gdb/stub.h"

Plugbox plugbox;
IOAPIC ioapic;
GDB_Stub gdb;
#include "syscall/guarded_keyboard.h"

Guarded_Keyboard keyboard;
#include "guard/guard.h"
#include "thread/scheduler.h"
#include "thread/thread.h"
#include "device/watch.h"

Watch watch;
#include "meeting/bellringer.h"

Bellringer bellringer;
#include "thread/idlethread.h"

const unsigned long IDLE_STACK_SIZE = 4096;
static unsigned char idle_stack[IDLE_STACK_SIZE];
IdleThread idle_thread(idle_stack + IDLE_STACK_SIZE);

void setupIdleThread() {
    IdleThread *thread = &idle_thread;
    scheduler.set_idle_thread(0, thread);
}

#include "syscall/guarded_semaphore.h"

Guarded_Semaphore koutsem(1);
static const uint32_t NUM_APPS = 1;
static KeyboardApplication kapp;
extern "C" int main() {
    kout.setpos(37, 0);
    kout << "OOStuBs" << endl;
    ioapic.init();
    watch.windup(1000);
    watch.activate();
    keyboard.Keyboard::plugin();
    gdb.receiveInterrupt(true);
    setupIdleThread();
    kapp.setID(0);
    scheduler.ready(&kapp);
    guard.enter();
    CPU::enable_int();
    scheduler.schedule();
    return 0;
}

