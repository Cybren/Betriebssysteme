
#include "console.h"
#include "debug/output.h"
#include "machine/cgascr.h"
#include "machine/ioapic.h"

extern Plugbox plugbox;

Console::Console(Serial::comPort port, Serial::baudRate baudrate, Serial::dataBits databits, Serial::stopBits stopbits,
                 Serial::parity parity) : Serial(port, baudrate, databits, stopbits, parity), sem(0) {}

void Console::flush() {
    print(buffer, pos);
    pos = 0;
}

void Console::write_number(int num) {
    if (num < 0) {
        write('-');
        num = -num;
    }
    int div;
    for (div = 1; num / div >= 10; div *= 10);
    while (div > 0) {
        write('0' + (num / div));
        num %= div;
        div /= 10;
    }
}

void Console::setForeground(color c) {
    put(0x1b);
    put(0x5b);
    put(0x33);
    put(0x30 + c);
    put(0x6d);
}

void Console::setBackground(color c) {
    put(0x1b);
    put(0x5b);
    put(0x34);
    put(0x30 + c);
    put(0x6d);
}

void Console::setAttribute(attrib a) {
    put(0x1b);
    put(0x5b);
    put(0x34);
    put(0x30 + a);
    put(0x6d);
}

void Console::reset() {
    flush();
    write(0x1b);
    write(0x63);
}

void Console::setpos(int x, int y) {
    flush();
    write(0x1b);
    write(0x5b);
    write_number(y + 1);
    write(0x3b);
    write_number(x + 1);
    write(0x48);
}

bool Console::getpos(int &x, int &y) {
    flush();
    write(0x1b);
    write(0x5b);
    write(0x36);
    write(0x6e);
    while (read() != 0x1b);
    if (read() == 0x5b) {
        y = 0;
        for (int r = read(); r != 0x3b; r = read())
            y = y * 10 + r - '0';
        y--;
        x = 0;
        for (int r = read(); r != 0x52; r = read())
            x = x * 10 + r - '0';
        x--;
        return true;
    } else
        return false;
}

void Console::print(char *string, int length) {
    for (int p = 0; p < length; p++) {
        if (string[p] == '\n' && p > 0 && string[p - 1] != '\r')
            write('\r');
        write(string[p]);
    }
}

void Console::listen() {
    receiveInterrupt(true);
    plugbox.assign(Plugbox::serial, this);
}

bool Console::prologue() {
    bool produced = false;
    int value;
    while ((value = Serial::read(false)) != -1 && buffer_prologue.produce(value))
        produced = true;
    return produced;
}

void Console::epilogue() {
    char val;
    while (buffer_prologue.consume(val)) {
        if (buffer_epilogue.produce(val))
            sem.v();
    }
}

#include "guard/guard.h"

char Console::getchar() {
    sem.p();
    char val;
    guard.enter();
    bool valid = buffer_epilogue.consume(val);
    guard.leave();
    if (valid)
        return val;
    else {
        DBG << "Console: Signal but buffer empty" << endl;
        return 0;
    }
}
