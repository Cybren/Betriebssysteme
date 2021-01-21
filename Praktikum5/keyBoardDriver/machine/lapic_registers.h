
#pragma once
#include "types.h"

#define DESTINATION_SHORTHAND_NO                0x0
#define DESTINATION_SHORTHAND_SELF                0x1
#define DESTINATION_SHORTHAND_ALLINCSELF            0x2
#define DESTINATION_SHORTHAND_ALLEXCSELF            0x3
#define TRIGGER_MODE_EDGE                    0x0
#define TRIGGER_MODE_LEVEL                    0x1
#define LEVEL_DEASSERT                        0x0
#define LEVEL_ASSERT                        0x1
#define DELIVERY_STATUS_IDLE                    0x0
#define DELIVERY_STATUS_SEND_PENDING                0x1
#define DESTINATION_MODE_PHYSICAL                0x0
#define DESTINATION_MODE_LOGICAL                0x1
#define DELIVERY_MODE_FIXED                    0x0
#define DELIVERY_MODE_LOWESTPRI                    0x1
#define DELIVERY_MODE_SMI                    0x2
#define DELIVERY_MODE_NMI                    0x4
#define DELIVERY_MODE_INIT                    0x5
#define DELIVERY_MODE_STARTUP                    0x6
#define DESTINATION_MODEL_CLUSTER                0x0
#define DESTINATION_MODEL_FLAT                    0xf
#define FOCUS_CPU_ENABLED                    0x0
#define FOCUS_CPU_DISABLED                    0x1
#define LAPIC_DISABLED                        0x0
#define LAPIC_ENABLED                        0x1
struct LAPICID_P {
    uint32_t reserved_1:24,
            apic_id:4,
            reserved_2:4;
} __attribute__((packed));
struct LAPICID_P4 {
    uint32_t reserved_1:24,
            apic_id:8;
} __attribute__((packed));
struct LAPICVER {
    uint32_t version:8,
            reserved_1:8,
            max_lvt_entry:8,
            reserved_2:8;
} __attribute__((packed));
struct ICR_L {
    uint32_t vector:8,
            delivery_mode:3,
            destination_mode:1,
            delivery_status:1,
            reserved_1:1,
            level:1,
            trigger_mode:1,
            reserved_2:2,
            destination_shorthand:2,
            reserved_3:12;
} __attribute__((packed));
struct LAPICTimer_Control {
    uint32_t vector:8,
            reserved_1:4,
            delivery_status:1,
            reserved_2:3,
            masked:1,
            timer_mode: 2,
            reserved_3:13;
} __attribute__((packed));
struct ICR_H {
    uint32_t reserved:24,
            destination_field:8;
} __attribute__((packed));
struct LDR {
    uint32_t reserved:24,
            lapic_id:8;
} __attribute__((packed));
struct DFR {
    uint32_t reserved:28,
            model:4;
} __attribute__((packed));
struct TPR {
    uint32_t task_prio_sub:4,
            task_prio:4,
            reserved:24;
} __attribute__((packed));
struct SVR {
    uint32_t spurious_vector:8,
            apic_enable:1,
            focus_processor_checking:1,
            reserved:22;
} __attribute__((packed));
union LAPICRegister {
    struct LAPICID_P lapicid_p;
    struct LAPICID_P4 lapicid_p4;
    struct LAPICVER lapicver;
    struct ICR_L icr_l;
    struct ICR_H icr_h;
    struct LDR ldr;
    struct DFR dfr;
    struct TPR tpr;
    struct SVR svr;
    struct LAPICTimer_Control timer_ctrl;
    uint32_t value;
} __attribute__((packed));
typedef union LAPICRegister LAPICRegister_t;

