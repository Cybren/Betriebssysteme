
#include "device/panic.h"
#include "debug/kernelpanic.h"
#include "machine/cpu.h"

Panic panic;

bool Panic::prologue() {
    kernelpanic("unexpected interrupt - processor halted.");
    return false;
}
