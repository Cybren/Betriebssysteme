
#include "machine/keyctrl.h"
#include "debug/output.h"
#include "machine/apicsystem.h"
#include "machine/ioapic.h"

extern APICSystem system;
extern IOAPIC ioapic;
extern CGA_Stream kout;

Keyboard_Controller::Keyboard_Controller() :
        keydecoder(this), ctrl_port(0x64), data_port(0x60) {
    set_led(led_caps_lock, false);
    set_led(led_scroll_lock, false);
    set_led(led_num_lock, false);
    set_repeat_rate(0, 0);
}

Key Keyboard_Controller::key_hit() {
    Key pressed;
    int control;
    while (((control = ctrl_port.inb()) & outb) && !(control & auxb)) {
        //check with auxb
        unsigned char code = data_port.inb();
        //kout << (int)code << " ";
        //kout.flush();
        pressed = keydecoder.decode(code);
        if (pressed.valid()){
            break;
        }
    }
    return pressed;
}

void Keyboard_Controller::reboot() {
    const IO_Port CMOS_ctrl(0x70);
    const IO_Port CMOS_data(0x71);
    const IO_Port system_control_portA(0x92);
    DBG_VERBOSE << "rebooting smp" << endl;
    CMOS_ctrl.outb(0xe + 1);
    CMOS_data.outb(0);
    system_control_portA.outb(0x3);
}

void Keyboard_Controller::set_repeat_rate(int speed, int delay) {
    if (speed < 0 || speed > 31 || delay < 0 || delay > 3)
        return;
    speed = (0xf & speed) | ((0x3 & delay) << 5);
    send_command(kbd_cmd::set_speed, speed);
}

void Keyboard_Controller::set_led(led_t led, bool on) {
    if (on)
        leds |= led;
    else
        leds &= ~led;
    send_command(kbd_cmd::set_led, leds);
}

void Keyboard_Controller::drainKeyboardBuffer() {
    while (ctrl_port.inb() & outb) {
        data_port.inb();
    }
}

void Keyboard_Controller::send_command(unsigned char cmd, unsigned char data) {
    send_byte(cmd);
    send_byte(data);
}

void Keyboard_Controller::send_byte(unsigned char byte) {
    while (ctrl_port.inb() & inpb) {}
    data_port.outb(byte);
}
