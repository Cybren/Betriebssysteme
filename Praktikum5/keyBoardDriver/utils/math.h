
#pragma once
#include "types.h"

class Math {
public:
    static uint64_t div64(uint64_t num, uint64_t den, uint64_t *rem_p = 0) {
        uint64_t quot = 0, qbit = 1;
        while ((int64_t) den >= 0) {
            den <<= 1;
            qbit <<= 1;
        }
        while (qbit) {
            if (den <= num) {
                num -= den;
                quot += qbit;
            }
            den >>= 1;
            qbit >>= 1;
        }
        if (rem_p) {
            *rem_p = num;
        }
        return quot;
    }

    template<typename T>
    static T min(T a, T b) {
        return a > b ? b : a;
    }

    template<typename T>
    static T max(T a, T b) {
        return a > b ? a : b;
    }

};

