
#pragma once
#include "types.h"

template<typename T, unsigned CAP>
class BBuffer {
    BBuffer(const BBuffer &) = delete;
    BBuffer &operator=(const BBuffer &) = delete;
private:
    T data[CAP];
    volatile unsigned in;
    volatile unsigned out;
public:
    BBuffer() : in(0), out(0) {}

    bool produce(T val) {
        unsigned nextin = (in + 1) % CAP;
        if (nextin != out) {
            data[in] = val;
            in = nextin;
            return true;
        }
        return false;
    }

    bool consume(T &val) {
        if (in != out) {
            val = data[out];
            out = (out + 1) % CAP;
            return true;
        }
        return false;
    }
};

