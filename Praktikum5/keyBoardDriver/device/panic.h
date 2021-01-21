
#pragma once
#include "guard/gate.h"

class Panic
        : public Gate {
    Panic(const Panic &) = delete;
    Panic &operator=(const Panic &) = delete;
public:
    Panic() {}

    bool prologue();
};

