
#pragma once
#include "object/o_stream.h"
#include "machine/serial.h"
#include "guard/gate.h"
#include "object/bbuffer.h"

class Console
        : public O_Stream, public Serial, public Gate {
private:
    void write_number(int x);
protected:
    BBuffer<char, 16> buffer_prologue;
public:
    typedef enum {
        RESET = 0,
        BRIGHT = 1,
        DIM = 2,
        UNDERSCORE = 4,
        BLINK = 5,
        REVERSE = 7,
        HIDDEN = 8,
    } attrib;
    typedef enum {
        BLACK = 0,
        RED = 1,
        GREEN = 2,
        YELLOW = 3,
        BLUE = 4,
        MAGENTA = 5,
        CYAN = 6,
        WHITE = 7
    } color;
    Console(Serial::comPort port = COM1, Serial::baudRate baudrate = BAUD_115200, Serial::dataBits databits = DATA_8BIT,
            Serial::stopBits stopbits = STOP_1BIT, Serial::parity parity = PARITY_NONE);

    ~Console() {}

    void flush();
    void setForeground(Console::color c);
    void setBackground(Console::color c);
    void setAttribute(Console::attrib a);
    void reset();
    void setpos(int x, int y);
    bool getpos(int &x, int &y);
    void print(char *string, int length);
    void listen();
    bool prologue();
    void epilogue();
    char getchar();
private:
    BBuffer<char, 16> buffer_epilogue;
    Semaphore sem;

};
