
#pragma once

class Spinlock {
private:
    Spinlock(const Spinlock &copy);
public:
    Spinlock() {}

    void lock() {
    }

    void unlock() {
    }
};

