
#include "stub.h"
#include "debug/assert.h"
#include "debug/output.h"

extern void const *const dbg_irq_entries[];
typedef enum {
    GATE_TASK_32 = 0x5,
    GATE_INT_16 = 0x6,
    GATE_TRAP_16 = 0x7,
    GATE_INT_32 = 0xe,
    GATE_TRAP_32 = 0xf,
} gate;
struct IDT_Entry {
    uint16_t address_low;
    uint16_t selector;
    union {
        struct {
            uint8_t zero;
            gate type : 4;
            uint8_t segment : 1;
            uint8_t dpl: 2;
            uint8_t present: 1;
        } __attribute__((packed));
        uint16_t flags;
    };
    uint16_t address_high;
} __attribute__((packed));

static_assert(sizeof(IDT_Entry) == 8, "Struct IDT_entry has wrong size");

struct IDT {
    uint16_t limit;
    struct IDT_Entry *base;
} __attribute__((packed));

static_assert(sizeof(IDT) == 6, "Struct IDT has wrong size");
extern struct IDT idt_desc_global;

static struct IDT *get_idt(void) {
    struct IDT *idt = &idt_desc_global;
    return idt;
}

static unsigned get_cs(void) {
    unsigned cs;

    asm volatile (
    "push    %%cs;\r\n"
    "pop     %%eax;\r\n"
    : "=a" (cs)::
    );

    return cs;
}

bool GDB_Stub::install_handler(int vector) {
    struct IDT *idt = get_idt();
    if (vector > (int) (idt->limit / sizeof(struct IDT_Entry)))
        return false;
    else {
        struct IDT_Entry &entry = idt->base[vector];
        entry.zero = 0;
        entry.segment = get_cs();
        entry.type = GATE_INT_32;
        entry.dpl = 0;
        entry.present = 1;
        assert(entry.flags == 0x8e00);
        unsigned address = (unsigned) (dbg_irq_entries[vector]);
        entry.address_low = address & 0xffff;
        entry.address_high = (address >> 16) & 0xffff;
        return true;
    }
}

