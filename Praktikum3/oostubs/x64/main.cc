/*! \file
 *  \brief Enthält mit den main() und main_ap() Funktionen den Startpunkt für
 *  das System
 */

#include "debug/gdb/stub.h"
#include "debug/output.h"
#include "device/cga_stream.h"
#include "device/watch.h"
#include "guard/guard.h"
#include "machine/cpu.h"
#include "machine/ioapic.h"
#include "meeting/bellringer.h"
#include "syscall/guarded_keyboard.h"
#include "syscall/guarded_semaphore.h"
#include "thread/idlethread.h"
#include "thread/scheduler.h"
#include "user/app2/kappl.h"

CGA_Stream kout(0, 79, 0, 21, true);
CGA_Stream dout(0, 79, 23, 24);

GDB_Stub gdb;
Guarded_Keyboard keyboard;
Watch watch;
Bellringer bellringer;

const unsigned long IDLE_STACK_SIZE = 4096;
static unsigned char idle_stack[IDLE_STACK_SIZE];
IdleThread idle_thread(idle_stack + IDLE_STACK_SIZE);

void setupIdleThread()
{
    DBG << "Init Idlethread for CPU " << 0 << endl;
    IdleThread* thread = &idle_thread;
    scheduler.set_idle_thread(0, thread);
}

Guarded_Semaphore koutsem(1);

static const uint32_t NUM_APPS = 6;
static KeyboardApplication kapp;

/*! \brief Einsprungpunkt ins System
 *
 *  Dieser Code wird nur auf der Boot-CPU (diejenige mit der ID 0) ausgeführt.
 */

extern "C" int main()
{
    // Startmeldung ausgeben
    kout.setpos(37, 0);
    kout << "OOStuBs" << endl;

    // IOAPIC initialisieren
    IOAPIC::init();
    // Uhr aufziehen
    watch.windup(1000);
    watch.activate();
    // Keyboard aktivieren
    keyboard.Keyboard::plugin();
    // GDB Interrupt aktivieren
    gdb.receiveInterrupt(true);
    setupIdleThread();

    kapp.setID(NUM_APPS);
    scheduler.ready(&kapp);

    setupIdleThread();

    Guard::enter();
    CPU::Interrupt::enable();
    scheduler.schedule();

    return 0;
}
