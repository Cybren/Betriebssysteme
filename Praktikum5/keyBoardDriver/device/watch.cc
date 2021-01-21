
#include "device/watch.h"
#include "debug/output.h"
#include "device/cgastr.h"
#include "machine/apicsystem.h"
#include "machine/ioapic.h"
#include "machine/plugbox.h"
#include "thread/scheduler.h"
#include "machine/lapic.h"
#include "utils/math.h"
#include "meeting/bellringer.h"

extern Bellringer bellringer;
extern CGA_Stream kout;
extern Plugbox plugbox;
extern IOAPIC ioapic;
extern APICSystem system;

uint32_t Watch::interval() {
    return interv;
}

bool Watch::windup(uint32_t us) {
    if (us == 0) {
        return false;
    }
    uint64_t ticks_ms = lapic.timer_ticks();
    uint64_t tmp_ticks = Math::div64(ticks_ms * us, 1000);
    uint32_t tmp32_ticks = (uint32_t) tmp_ticks;
    for (int i = 1; i <= 128; i *= 2) {
        if (tmp_ticks == ((uint64_t) tmp32_ticks)) {
            ticks = tmp32_ticks;
            div = i;
            interv = us;
            plugbox.assign(Plugbox::timer, this);
            return true;
        }
        tmp_ticks = tmp_ticks >> 1;
        tmp32_ticks = (uint32_t) tmp_ticks;
    }
    return false;
}

void Watch::activate() {
    lapic.setTimer(ticks, div, Plugbox::timer, true);
}

bool Watch::prologue() {
    static int direction = 0;
    static char handle[4] = {'/', '-', '\\', '|'};
    static int tick = 0;
    tick = (tick + 1) % 50;
    if (tick == 0) {
        direction = (direction + 1) % 4;
        CGA_Stream alive(50, 78, 0, 0);
        alive.show(-1, 0, handle[direction], CGA_Screen::Attribute(CGA_Screen::RED));
    }
    return true;
}

void Watch::block() {
    if (system.getCPUID() == 0) {
        if (bellringer.bell_pending()) {
            return;
        }
    }
    lapic.setTimerMasked(true);
}

void Watch::unblock() {
    lapic.setTimerMasked(false);
}

void Watch::epilogue() {
    if (system.getCPUID() == 0) {
        bellringer.check();
    }
    scheduler.resume();
}
