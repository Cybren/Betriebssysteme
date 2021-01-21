
#include "ioapic.h"

volatile uint32_t *IOAPIC::IOREGSEL_REG = (volatile uint32_t *) 0xfec00000;
volatile uint32_t *IOAPIC::IOWIN_REG = (volatile uint32_t *) 0xfec00010;
#include "apicsystem.h"

void IOAPIC::write(unsigned char reg, IOAPICRegister_t value) {
    *IOREGSEL_REG = reg;
    *IOWIN_REG = value.value;
}

IOAPICRegister_t IOAPIC::read(unsigned char reg) {
    *IOREGSEL_REG = reg;
    IOAPICRegister_t value;
    value.value = *IOWIN_REG;
    return value;
}

void IOAPIC::getRedirectionTableEntry(unsigned char slot, struct IOREDTBL_L &low, struct IOREDTBL_H &high) {
    low = read(ioredtbl_reg + 2 * slot + 0).IOREDTBL_L;
    high = read(ioredtbl_reg + 2 * slot + 1).IOREDTBL_H;
}

void IOAPIC::setRedirectionTableEntry(unsigned char slot, struct IOREDTBL_L low, struct IOREDTBL_H high) {
    IOAPICRegister_t reg;
    reg.IOREDTBL_L = low;
    write(ioredtbl_reg + 2 * slot + 0, reg);
    reg.IOREDTBL_H = high;
    write(ioredtbl_reg + 2 * slot + 1, reg);
}

void IOAPIC::init() {
    unsigned char ioapicID = system.getIOAPICID();
    setID(ioapicID);
    struct IOREDTBL_L entry_low;
    struct IOREDTBL_H entry_high;
    for (unsigned char i = 0; i < 24; ++i) {
        getRedirectionTableEntry(i, entry_low, entry_high);
        entry_high.logical_destination = 1;
        entry_low.vector = 0x22;
        entry_low.delivery_mode = DELIVERY_MODE_LOWESTPRI;
        entry_low.destination_mode = DESTINATION_MODE_LOGICAL;
        entry_low.polarity = POLARITY_HIGH;
        entry_low.trigger_mode = TRIGGER_MODE_EDGE;
        entry_low.mask = MASK_DISABLED;
        setRedirectionTableEntry(i, entry_low, entry_high);
    }
}

void IOAPIC::setID(unsigned char id) {
    IOAPICRegister_t reg = read(ioapicid_reg);
    reg.IOAPICID.ID = id;
    write(ioapicid_reg, reg);
}

void IOAPIC::config(unsigned char slot, Plugbox::Vector vector, int trigger_mode, int polarity
) {
    struct IOREDTBL_L low;
    struct IOREDTBL_H high;
    getRedirectionTableEntry(slot, low, high);
    low.vector = vector;
    low.polarity = polarity;
    low.trigger_mode = trigger_mode;
    setRedirectionTableEntry(slot, low, high);
}

void IOAPIC::allow(unsigned char slot) {
    struct IOREDTBL_L low;
    struct IOREDTBL_H high;
    getRedirectionTableEntry(slot, low, high);
    low.mask = MASK_ENABLED;
    setRedirectionTableEntry(slot, low, high);
}

void IOAPIC::forbid(unsigned char slot) {
    struct IOREDTBL_L low;
    struct IOREDTBL_H high;
    getRedirectionTableEntry(slot, low, high);
    low.mask = MASK_DISABLED;
    setRedirectionTableEntry(slot, low, high);
}

bool IOAPIC::status(unsigned char slot) {
    struct IOREDTBL_L low;
    struct IOREDTBL_H high;
    getRedirectionTableEntry(slot, low, high);
    return low.mask == MASK_ENABLED;
}

