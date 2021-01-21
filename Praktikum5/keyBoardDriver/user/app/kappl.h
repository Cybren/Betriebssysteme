
#pragma once
#include "types.h"
#include "thread/thread.h"

class KeyboardApplication
        : public Thread {
    KeyboardApplication(const KeyboardApplication &) = delete;
    KeyboardApplication &operator=(const KeyboardApplication &) = delete;
private:
    int id;
    uint8_t appstack[4096];
public:
    KeyboardApplication(int i = 0) : Thread(appstack + 4096), id(i) {}

    void setID(int i) {
        id = i;
    }

    void action();
};
