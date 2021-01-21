
#pragma once
#include "device/keyboard.h"

class Guarded_Keyboard
        : public Keyboard {
    Guarded_Keyboard(const Guarded_Keyboard &) = delete;
    Guarded_Keyboard &operator=(const Guarded_Keyboard &) = delete;
public:
    Guarded_Keyboard() {}

    Key getkey();
};

