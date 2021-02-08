
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
    int i = 1; //test is active
    //int i = 0; //test is not active

    if(i){
        Key key;

        key.ascii(115);
        key.scancode(31);
        testC(key, 's');

        key.ascii(84);
        key.scancode(20);
        testC(key, 'T');

        key.ascii(41);
        key.scancode(10);
        testC(key, ')');

        key.ascii(123);
        key.scancode(9);
        testC(key, '{');

        key.ascii('\n');
        key.scancode(28);
        testC(key, '\n');

        key.ascii(8);
        key.scancode(57);
        testC(key, '\b');

    }
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

