
#pragma once

class Key {
private:
    unsigned char asc;
    unsigned char scan;
    unsigned char modi;
    struct mbit {
        enum {
            SHIFT = 1,
            alt_left = 2,
            alt_right = 4,
            ctrl_left = 8,
            ctrl_right = 16,
            caps_lock = 32,
            num_lock = 64,
            scroll_lock = 128
        };
    };
public:
    Key() : asc(0), scan(0), modi(0) {}

    bool valid();
    void invalidate();
    void ascii(unsigned char a);
    void scancode(unsigned char s);
    unsigned char ascii();
    unsigned char scancode();
    void SHIFT(bool pressed);
    void alt_left(bool pressed);
    void alt_right(bool pressed);
    void ctrl_left(bool pressed);
    void ctrl_right(bool pressed);
    void caps_lock(bool pressed);
    void num_lock(bool pressed);
    void scroll_lock(bool pressed);
    bool SHIFT();
    bool alt_left();
    bool alt_right();
    bool ctrl_left();
    bool ctrl_right();
    bool caps_lock();
    bool num_lock();
    bool scroll_lock();
    bool alt();
    bool ctrl();

    operator char() {
        return (char) asc;
    }

    struct scan {
        enum {
            f1 = 0x3b, del = 0x53, up = 72, down = 80, left = 75, right = 77,
            div = 8
        };
    };
};

