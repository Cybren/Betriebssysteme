
#include "machine/lapic.h"
#include "machine/io_port.h"
#include "utils/math.h"

LAPIC lapic;
uint32_t LAPIC::LAPIC_BASE = 0xfee00000;

void LAPIC::write(uint16_t reg, LAPICRegister_t val) {
    *((volatile uint32_t *) (LAPIC_BASE + reg)) = val.value;
}

LAPICRegister_t LAPIC::read(uint16_t reg) {
    LAPICRegister_t value;
    value.value = *((volatile uint32_t *) (LAPIC_BASE + reg));
    return value;
}

uint8_t LAPIC::getLAPICID() {
    LAPICRegister_t value = read(lapicid_reg);
    if (isPentium4orNewer()) {
        return value.lapicid_p4.apic_id;
    } else {
        return value.lapicid_p.apic_id;
    }
}

uint8_t LAPIC::getVersion() {
    LAPICRegister_t value = read(lapicver_reg);
    return value.lapicver.version;
}

void LAPIC::init() {
    LAPICRegister_t value;
    value = read(ldr_reg);
    value.ldr.lapic_id = 0;
    write(ldr_reg, value);
    value = read(tpr_reg);
    value.tpr.task_prio = 0;
    value.tpr.task_prio_sub = 0;
    write(tpr_reg, value);
    value = read(dfr_reg);
    value.dfr.model = DESTINATION_MODEL_FLAT;
    write(dfr_reg, value);
    value = read(spiv_reg);
    value.svr.spurious_vector = 0xff;
    value.svr.apic_enable = LAPIC_ENABLED;
    value.svr.focus_processor_checking = FOCUS_CPU_DISABLED;
    write(spiv_reg, value);
}

void LAPIC::ackIRQ() {
    read(spiv_reg);
    LAPICRegister_t eoi;
    eoi.value = 0;
    write(eoi_reg, eoi);
}

void LAPIC::setLogicalLAPICID(uint8_t id) {
    LAPICRegister_t value = read(ldr_reg);
    value.ldr.lapic_id = id;
    write(ldr_reg, value);
}

void LAPIC::sendIPI(uint8_t destination, struct ICR_L data) {
    while (!isIPIDelivered()) {

    }
    LAPICRegister_t value = read(icrh_reg);
    value.icr_h.destination_field = destination;
    write(icrh_reg, value);
    value.icr_l = data;
    write(icrl_reg, value);
}

bool LAPIC::isIPIDelivered() {
    LAPICRegister_t value = read(icrl_reg);
    return value.icr_l.delivery_status == DELIVERY_STATUS_IDLE;
}

bool LAPIC::isExternalAPIC() {
    uint8_t version = getVersion();
    if ((version & 0xf0) == 0x00) {
        return true;
    } else {
        return false;
    }
}

bool LAPIC::isLocalAPIC() {
    uint8_t version = getVersion();
    if ((version & 0xf0) == 0x10) {
        return true;
    } else {
        return false;
    }
}

bool LAPIC::isPentium4orNewer() {
    uint8_t version = getVersion();
    if (version >= 0x14) {
        return true;
    } else {
        return false;
    }
}

uint32_t LAPIC::timer_ticks() {
    IO_Port ctrl(0x43);
    IO_Port data(0x42);
    IO_Port help(0x61);
    setTimer(0xffffffff, 1, 42, false, true);
    help.outb(0x01);
    ctrl.outb(0xB0);
    data.outb(0xFF);
    data.outb(0xFF);
    uint32_t ticks = read(ccr_reg).value;
    while (!(help.inb() & 0x20));
    ticks = ticks - read(ccr_reg).value;
    setTimer(0, 1, 42, false, true);
    uint32_t freq = (uint32_t) Math::div64(((uint64_t) ticks) * 1000 * 1000, 838 * 65535);
    return freq;
}

uint8_t LAPIC::timer_div(uint8_t div) {
    const uint8_t masks[] = {0xb, 0x0, 0x1, 0x2, 0x3, 0x8, 0x9, 0xa};
    if (!((div != 0) && !(div & (div - 1)))) {
        return 0xff;
    }
    int trail = __builtin_ctz(div);
    if (trail > 7) {
        return 0xff;
    }
    return masks[trail];
}

void LAPIC::setTimer(uint32_t counter, uint8_t divide, uint8_t vector, bool periodic, bool masked) {
    (void) counter;
    (void) divide;
    (void) vector;
    (void) periodic;
    (void) masked;
    LAPICRegister_t val;
    val.value = 0;
    write(icr_reg, val);
    val.value = 0;
    val.timer_ctrl.vector = vector;
    if (periodic) {
        val.timer_ctrl.timer_mode = 1;
    }
    val.timer_ctrl.masked = masked;
    write(timerctrl_reg, val);
    val.value = timer_div(divide);
    write(dcr_reg, val);
    val.value = counter;
    write(icr_reg, val);
}

void LAPIC::setTimerMasked(bool masked) {
    LAPICRegister_t control = read(timerctrl_reg);
    control.timer_ctrl.masked = masked;
    write(timerctrl_reg, control);
}
