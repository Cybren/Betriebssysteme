
#pragma once
#define DESTINATION_MODE_PHYSICAL    0x0
#define DESTINATION_MODE_LOGICAL    0x1
#define TRIGGER_MODE_EDGE            0x0
#define TRIGGER_MODE_LEVEL            0x1
#define POLARITY_HIGH                0x0
#define POLARITY_LOW                0x1
#define MASK_ENABLED                0x0
#define MASK_DISABLED                0x1
#define DELIVERY_MODE_FIXED            0x0
#define DELIVERY_MODE_LOWESTPRI        0x1
#define DELIVERY_MODE_SMI            0x2
#define DELIVERY_MODE_NMI            0x4
#define DELIVERY_MODE_INIT            0x5
#define DELIVERY_MODE_EXTINT        0x7
struct IOAPICID {
    unsigned int reserved_2:24,
            ID:4,
            reserved_1:4;
} __attribute__((packed));
struct IOREDTBL_L {
    unsigned int vector:8,
            delivery_mode:3,
            destination_mode:1,
            delivery_status:1,
            polarity:1,
            remote_irr:1,
            trigger_mode:1,
            mask:1,
            reserved:15;
} __attribute__((packed));
struct IOREDTBL_H {
    unsigned int reserved:24,
            logical_destination:8;
} __attribute__((packed));
union IOAPICRegister {
    struct IOAPICID IOAPICID;
    struct IOREDTBL_L IOREDTBL_L;
    struct IOREDTBL_H IOREDTBL_H;
    unsigned int value;
} __attribute__((packed));
typedef union IOAPICRegister IOAPICRegister_t;

