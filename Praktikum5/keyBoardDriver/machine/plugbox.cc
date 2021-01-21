
#include "machine/plugbox.h"
#include "device/panic.h"

extern Panic panic;

Plugbox::Plugbox() {
    for (unsigned int slot = 0; slot < size; slot++)
        map[slot] = &panic;
}

void Plugbox::assign(unsigned int slot, Gate *gate) {
    if (slot < size)
        map[slot] = gate;
}

Gate *Plugbox::report(unsigned int slot) {
    if (slot < size)
        return map[slot];
    else
        return &panic;
}
