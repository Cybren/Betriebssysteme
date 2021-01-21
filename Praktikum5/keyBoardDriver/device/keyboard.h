
#pragma once
#include "machine/keyctrl.h"
#include "guard/gate.h"
#include "machine/key.h"
#include "meeting/semaphore.h"

class Keyboard
        : public Gate, public Keyboard_Controller {
    Keyboard(const Keyboard &) = delete;
    Keyboard &operator=(const Keyboard &) = delete;
private:
    Key lastkey;
    Key currentkey;
    Semaphore sema;
public:
    Keyboard() : sema(0) {
        lastkey.scancode(0);
    }

    void plugin();
    bool prologue();
    void epilogue();
    Key getkey();
};
