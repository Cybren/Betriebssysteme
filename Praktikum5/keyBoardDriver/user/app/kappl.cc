
#include "user/app/kappl.h"
#include "device/cgastr.h"

extern CGA_Stream kout;
#include "syscall/guarded_keyboard.h"

extern Guarded_Keyboard keyboard;
#include "syscall/guarded_semaphore.h"

extern Guarded_Semaphore koutsem;
#include "syscall/guarded_bell.h"

void testC(Key, char);

void KeyboardApplication::action() {
    //TODO
    while (true) {
        
    }
}

void testC(Key key, char correctC) {
    if (key.valid()) {
        kout << key.ascii() << flush;
        if (key.ascii() == correctC) {
            kout << "   -Correct Input";
        } else {
            kout << "   -False Input";
        }
        kout << endl << flush;
    }
}

