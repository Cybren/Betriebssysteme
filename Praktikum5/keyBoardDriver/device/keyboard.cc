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
    Key key = key_hit();
    //rebooten mit (strg + alt + e)
    if(currentkey.alt() && currentkey.ctrl() && currentkey.ascii() == 'e'){
        reboot();
    }
    if(key.valid()){
        if(!currentkey.valid()){
            currentkey = key;
            return true;
        }
    }
    return false;
}

void Keyboard::epilogue() {
    //TODO
    sema.v();
}

Key Keyboard::getkey() {
    //TODO
    sema.p();
    Key key = currentkey;
    currentkey.invalidate();
    return key;
}

