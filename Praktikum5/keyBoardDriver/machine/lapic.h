
#pragma once
#include "types.h"
#include "machine/lapic_registers.h"

class APICSystem;

class LAPIC {
private:
    void write(uint16_t reg, LAPICRegister_t value);
    LAPICRegister_t read(uint16_t reg);
    enum {
        lapicid_reg = 0x020,
        lapicver_reg = 0x030,
        tpr_reg = 0x080,
        eoi_reg = 0x0b0,
        ldr_reg = 0x0d0,
        dfr_reg = 0x0e0,
        spiv_reg = 0x0f0,
        icrl_reg = 0x300,
        icrh_reg = 0x310,
        timerctrl_reg = 0x320,
        icr_reg = 0x380,
        ccr_reg = 0x390,
        dcr_reg = 0x3e0,
    };
    friend APICSystem;
    void init();
    void setLogicalLAPICID(uint8_t id);
public:
    static uint32_t LAPIC_BASE;

    LAPIC() {}

    void ackIRQ();
    uint8_t getLAPICID();
    uint8_t getVersion();
    void sendIPI(uint8_t destination, struct ICR_L data);
    bool isIPIDelivered();
    bool isExternalAPIC();
    bool isLocalAPIC();
    bool isPentium4orNewer();
    uint32_t timer_ticks();
    uint8_t timer_div(uint8_t div);
    void setTimer(uint32_t counter, uint8_t divide, uint8_t vector, bool periodic, bool masked = false);
    void setTimerMasked(bool masked = false);
};

extern LAPIC lapic;

