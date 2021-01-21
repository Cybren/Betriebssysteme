#include "serial.h"
#include "types.h"
#include "debug/output.h"
#include "machine/cpu.h"
#include "machine/io_port.h"
#include "machine/ioapic.h"
#include "machine/plugbox.h"
#include "machine/apicsystem.h"

extern IOAPIC ioapic;

Serial::Serial(Serial::comPort port, Serial::baudRate baudrate, Serial::dataBits databits, Serial::stopBits stopbits,
               Serial::parity parity) : port(port) {
    uint16_t divisor = baudrate;
    writeReg(lcr, 0x80);
    writeReg(dll, divisor & 0xff);
    writeReg(dlh, (divisor >> 8) & 0xff);
    writeReg(lcr, databits | stopbits | parity);
    writeReg(fcr, 0x07);
    writeReg(mcr, 0x08);
}

void Serial::writeReg(Serial::regIndex reg, char out) {
    IO_Port p((uint16_t) port + (uint16_t) reg);
    p.outb(out);
}

char Serial::readReg(Serial::regIndex reg) {
    IO_Port p((uint16_t) port + (uint16_t) reg);
    return p.inb();
}

int Serial::write(char out, bool blocking) {
    while ((readReg(lsr) & 0x20) == 0)
        if (!blocking)
            return -1;
    writeReg(tbr, out);
    return out;
}

int Serial::read(bool blocking) {
    while ((readReg(lsr) & 0x1) == 0)
        if (!blocking)
            return -1;
    return readReg(rbr);
}

bool Serial::receiveInterrupt(bool enable) {
    unsigned char irq = 0xff;
    switch (port) {
        case COM1:
            irq = system.getIOAPICSlot(APICSystem::com1);
            break;
        case COM2:
            irq = system.getIOAPICSlot(APICSystem::com2);
            break;
        case COM3:
            irq = system.getIOAPICSlot(APICSystem::com3);
            break;
        case COM4:
            irq = system.getIOAPICSlot(APICSystem::com4);
            break;
        default:
            break;
    }
    if (irq != 0xff) {
        DBG_VERBOSE << "Serial IRQ" << irq << endl;
        ioapic.config(irq, Plugbox::serial, TRIGGER_MODE_LEVEL);
        if (enable)
            ioapic.allow(irq);
        else
            ioapic.forbid(irq);
    } else {
        DBG << "No IOAPIC Slot for Serial found!" << endl;
    }
    char data = readReg(ier);
    const char mask = 0x01;
    writeReg(ier, enable ? (data | mask) : (data & ~mask));
    for (int i = 0; i < 500; ++i) {
        asm volatile ("cpuid\n\t":: : "eax", "ebx", "ecx", "edx", "memory" );
    }
    while ((readReg(lsr) & 0x1)) {
        readReg(rbr);
    }
    return (data & mask) != 0;
}
