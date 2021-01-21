
#pragma once
#include "meeting/bell.h"

class Guarded_Bell
        : public Bell {
    Guarded_Bell(const Guarded_Bell &) = delete;
    Guarded_Bell &operator=(const Guarded_Bell &) = delete;
public:
    Guarded_Bell() {}

    static void sleep(unsigned int ms);
};

