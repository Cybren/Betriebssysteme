
#include "machine/keydecoder.h"
#include "machine/keyctrl.h"

//TODO
unsigned char Key_Decoder::normal_tab[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 225, 39, 8,
        0, 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 129, '+', 13,
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 148, 132, '^', 0, '#',
        'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
        '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
        0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, '<', 0, 0
};
//TODO
unsigned char Key_Decoder::shift_tab[] = {
        0, 0, '!', '"', 21, '$', '%', '&', '/', '(', ')', '=', '?', 96, 0,
        0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 154, '*', 0,
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 135, 142, 248, 0, 39,
        'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,
        0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '>', 0, 0
};
//TODO
unsigned char Key_Decoder::alt_tab[] = {
        0, 0, 0, 253, 0, 0, 0, 0, '{', '[', ']', '}', 92, 0, 0,
        0, '@', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '~', 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 230, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0
};
unsigned char Key_Decoder::asc_num_tab[] = {
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', ','
};
unsigned char Key_Decoder::scan_num_tab[] = {
        8, 9, 10, 53, 5, 6, 7, 27, 2, 3, 4, 11, 51
};

void Key_Decoder::get_ascii_code(unsigned char code) {
    if (code == 53 && prefix == prefix1) {
        gather.ascii('/');
        gather.scancode(Key::scan::div);
    } else if (gather.num_lock() && !prefix && code >= 71 && code <= 83) {
        gather.ascii(asc_num_tab[code - 71]);
        gather.scancode(scan_num_tab[code - 71]);
    } else if (gather.alt_right()) {
        gather.ascii(alt_tab[code]);
        gather.scancode(code);
    } else if (gather.SHIFT()) {
        gather.ascii(shift_tab[code]);
        gather.scancode(code);
    } else if (gather.caps_lock()) {
        if ((code >= 16 && code <= 26) || (code >= 30 && code <= 40)
            || (code >= 44 && code <= 50)) {
            gather.ascii(shift_tab[code]);
            gather.scancode(code);
        } else {
            gather.ascii(normal_tab[code]);
            gather.scancode(code);
        }
    } else {
        gather.ascii(normal_tab[code]);
        gather.scancode(code);
    }
}

Key Key_Decoder::decode(unsigned char code) {
    gather.invalidate();
    if (code == prefix1 || code == prefix2) {
        prefix = code;
    } else {
        if (code & break_bit) {
            code &= ~break_bit;
            switch (code) {
                case 42:
                case 54:
                    gather.SHIFT(false);
                    break;
                case 56:
                    if (prefix == prefix1)
                        gather.alt_right(false);
                    else
                        gather.alt_left(false);
                    break;
                case 29:
                    if (prefix == prefix1)
                        gather.ctrl_right(false);
                    else
                        gather.ctrl_left(false);
                    break;
            }

        } else {
            switch (code) {
                case 42:
                case 54:
                    gather.SHIFT(true);
                    break;
                case 56:
                    if (prefix == prefix1)
                        gather.alt_right(true);
                    else
                        gather.alt_left(true);
                    break;
                case 29:
                    if (prefix == prefix1)
                        gather.ctrl_right(true);
                    else
                        gather.ctrl_left(true);
                    break;
                case 58:
                    gather.caps_lock(!gather.caps_lock());
                    keyctrl->set_led(Keyboard_Controller::led_caps_lock, gather.caps_lock());
                    break;
                case 70:
                    gather.scroll_lock(!gather.scroll_lock());
                    keyctrl->set_led(Keyboard_Controller::led_scroll_lock, gather.scroll_lock());
                    break;
                case 69:
                    if (gather.ctrl_left()) {
                        get_ascii_code(code);
                    } else {
                        gather.num_lock(!gather.num_lock());
                        keyctrl->set_led(Keyboard_Controller::led_num_lock, gather.num_lock());
                    }
                    break;
                case 91:
                case 92:
                case 93:
                    break;
                default:
                    get_ascii_code(code);
            }
        }
        prefix = 0;
    }
    return gather;
}

