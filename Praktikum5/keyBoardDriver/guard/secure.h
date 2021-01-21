
#pragma once
#include "guard/guard.h"

class Secure {
    Secure(const Secure &) = delete;
    Secure &operator=(const Secure &) = delete;
public:
    Secure() {
        guard.enter();
    }

    ~Secure() {
        guard.leave();
    }

};
