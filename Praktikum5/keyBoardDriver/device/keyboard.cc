
#include "device/keyboard.h"
#include "machine/apicsystem.h"
#include "machine/ioapic.h"
#include "machine/plugbox.h"
#include "machine/key.h"

extern IOAPIC ioapic;
extern Plugbox plugbox;
extern APICSystem system;

void Keyboard::plugin() {
    plugbox.assign(Plugbox::keyboard, this);
    ioapic.config(system.getIOAPICSlot(APICSystem::keyboard), Plugbox::keyboard);
    ioapic.allow(system.getIOAPICSlot(APICSystem::keyboard));
    drainKeyboardBuffer();
}

#include "device/cgastr.h"

extern CGA_Stream kout;

bool Keyboard::prologue() {
   //TODO
}

void Keyboard::epilogue() {
    //TODO
}

Key Keyboard::getkey() {
    //TODO
}

