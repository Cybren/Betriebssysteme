
#pragma once
#include "types.h"
#include "machine/ioapic_registers.h"
#include "machine/plugbox.h"

class IOAPIC {
    IOAPIC(const IOAPIC &) = delete;
    IOAPIC &operator=(const IOAPIC &) = delete;
private:
    enum {
        ioapicid_reg = 0x00,
        ioredtbl_reg = 0x10
    };
    void write(unsigned char reg, IOAPICRegister_t value);
    IOAPICRegister_t read(unsigned char reg);
    void getRedirectionTableEntry(unsigned char slot, struct IOREDTBL_L &low, struct IOREDTBL_H &high);
    void setRedirectionTableEntry(unsigned char slot, struct IOREDTBL_L low, struct IOREDTBL_H high);
    void setID(unsigned char id);
public:
    static volatile uint32_t *IOREGSEL_REG;
    static volatile uint32_t *IOWIN_REG;

    IOAPIC() {}

    void init();
    void config(unsigned char slot, Plugbox::Vector vector, int triggermode = TRIGGER_MODE_EDGE,
                int polarity = POLARITY_HIGH);
    void allow(unsigned char slot);
    void forbid(unsigned char slot);
    bool status(unsigned char slot);
};

