
#pragma once
#include "machine/key.h"

class Keyboard_Controller;

class Key_Decoder {
    Key_Decoder(const Key_Decoder &) = delete;
    Key_Decoder &operator=(const Key_Decoder &) = delete;
public:
    Key_Decoder(Keyboard_Controller *keyctrl) : keyctrl(keyctrl) {};
    Key decode(unsigned char code);
private:
    Keyboard_Controller *const keyctrl;
    unsigned char prefix;
    Key gather;
    static unsigned char normal_tab[];
    static unsigned char shift_tab[];
    static unsigned char alt_tab[];
    static unsigned char asc_num_tab[];
    static unsigned char scan_num_tab[];
    enum {
        break_bit = 0x80, prefix1 = 0xe0, prefix2 = 0xe1
    };
    void get_ascii_code(unsigned char code);
};

