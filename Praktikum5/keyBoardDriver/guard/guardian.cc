
#include "types.h"
#include "guardian.h"
#include "machine/plugbox.h"
#include "machine/lapic.h"
#include "debug/output.h"

extern Plugbox plugbox;
extern LAPIC lapic;
#include "guard/guard.h"

extern Guard guard;
extern "C" void guardian(uint32_t vector, irq_context *context) {
    (void) vector;
    (void) context;
    Gate *gate = plugbox.report(vector);
    bool wantsEpilogue = gate->prologue();
    lapic.ackIRQ();
    if (wantsEpilogue) {
        guard.relay(gate);
    }
}
