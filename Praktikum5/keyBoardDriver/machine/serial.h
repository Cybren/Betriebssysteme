
#pragma once
#include "types.h"
#include "meeting/semaphore.h"

class Serial {
public:
    typedef enum {
        COM1 = 0x3f8,
        COM2 = 0x2f8,
        COM3 = 0x3e8,
        COM4 = 0x238,
    } comPort;
    typedef enum {
        BAUD_300 = 384,
        BAUD_600 = 192,
        BAUD_1200 = 96,
        BAUD_2400 = 48,
        BAUD_4800 = 24,
        BAUD_9600 = 12,
        BAUD_19200 = 6,
        BAUD_38400 = 3,
        BAUD_57600 = 2,
        BAUD_115200 = 1,
    } baudRate;
    typedef enum {
        DATA_5BIT = 0,
        DATA_6BIT = 1,
        DATA_7BIT = 2,
        DATA_8BIT = 3,
    } dataBits;
    typedef enum {
        STOP_1BIT = 0,
        STOP_1_5BIT = 4,
        STOP_2BIT = 4,
    } stopBits;
    typedef enum {
        PARITY_NONE = 0,
        PARITY_ODD = 8,
        PARITY_EVEN = 24,
        PARITY_MARK = 40,
        PARITY_SPACE = 56,
    } parity;
private:
    enum regIndex {
        rbr = 0,
        tbr = 0,
        dll = 0,
        ier = 1,
        dlh = 1,
        irr = 2,
        fcr = 2,
        lcr = 3,
        mcr = 4,
        lsr = 5,
        msr = 6
    };
    const Serial::comPort port;
    char readReg(Serial::regIndex reg);
    void writeReg(Serial::regIndex reg, char out);
public:
    Serial(Serial::comPort port = COM1, Serial::baudRate baudrate = BAUD_115200, Serial::dataBits databits = DATA_8BIT,
           Serial::stopBits stopbits = STOP_1BIT, Serial::parity parity = PARITY_NONE);
    int read(bool blocking = true);
    int write(char out, bool blocking = true);
    bool receiveInterrupt(bool enable);
};
