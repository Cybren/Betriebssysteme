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
    if(!currentkey.valid()){
        currentkey = key;
        //extend to (strg + alt + entf)
        if(currentkey.alt() && lastkey.ctrl()){
            reboot();
        }
        return true;
    }
    return false;
}

void Keyboard::epilogue() {
    //TODO
    Key key = getkey();
    if(key.scancode() != 0){
        kout << key;
        kout.flush();
    }
}

Key Keyboard::getkey() {
    Key key = currentkey;
    currentkey.invalidate();
    lastkey = key;
    return key;
}

