
#include "syscall/guarded_bell.h"
#include "guard/secure.h"

void Guarded_Bell::sleep(unsigned int ms) {
    Secure section;
    Bell::sleep(ms);
}

