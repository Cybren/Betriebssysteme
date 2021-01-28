#pragma once
#include "machine/io_port.h"
#include "machine/keydecoder.h"
#include "machine/key.h"

class Keyboard_Controller {
    Keyboard_Controller(const Keyboard_Controller &) = delete;
    Keyboard_Controller &operator=(const Keyboard_Controller &) = delete;
protected:
    void send_command(unsigned char cmd, unsigned char data);
    void send_byte(unsigned char byte);
    void drainKeyboardBuffer();
private:
    Key_Decoder keydecoder;
    char leds;
    const IO_Port ctrl_port;
    const IO_Port data_port;
    enum {
        outb = 0x01, inpb = 0x02, auxb = 0x20
    };
    struct kbd_cmd {
        enum {
            set_led = 0xed, set_speed = 0xf3
        };
    };
    enum {
        cpu_reset = 0xfe
    };
    struct kbd_reply {
        enum {
            ack = 0xfa
        };
    };
public:
    Keyboard_Controller();
    Key key_hit();
    void reboot();
    void set_repeat_rate(int speed, int delay);
    enum led_t {
        led_caps_lock = 4,
        led_num_lock = 2,
        led_scroll_lock = 1
    };
    void set_led(led_t led, bool on);
};

