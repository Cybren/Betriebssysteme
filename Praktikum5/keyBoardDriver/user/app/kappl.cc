
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
    Key key;
    kout << "strg + alt + 'e' for reboot" << endl << flush;

    kout << endl << "Press 's'" << endl << flush;
    key = keyboard.getkey();
    testC(key, 's');

    kout << endl <<  "Press 'T'" << endl << flush;
    key = keyboard.getkey();
    testC(key, 'T');

    kout << endl << "Press ')'" << endl << flush;
    key = keyboard.getkey();
    testC(key, ')');

    kout << endl << "Press '{'" << endl << flush;
    key = keyboard.getkey();
    testC(key, '{');

    kout << endl << "Press ENTER" << endl << flush;
    key = keyboard.getkey();
    testC(key, 13);
    //kout << key.ascii() << " " << (int)key.scancode() << flush;

    kout << endl << "Press BACKSPACE" << endl << flush;
    key = keyboard.getkey();
    testC(key, 32);
    //kout << key.ascii() << " " << (int)key.scancode() << flush;

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

