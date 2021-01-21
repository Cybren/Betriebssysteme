
#pragma once
#define MPFPS_ID_SIGNATURE      (('_'<<24)|('P'<<16)|('M'<<8)|'_')
#define COMPATIBILITY_PIC       (1 << 7)
#define COMPATIBILITY_VIRTWIRE      0x0
#define MPCT_PROCESSOR          0x0
#define MPCT_BUS                0x1
#define MPCT_IOAPIC             0x2
#define MPCT_IOINT              0x3
#define MPCT_LINT               0x4
#define CPU_ENABLED             0x1
#define CPU_BOOTPROCESSOR       0x2
struct mpfps {
    char signature[4];
    unsigned int physptr;
    unsigned char length;
    unsigned char spec_rev;
    unsigned char checksum;
    unsigned char feature1;
    unsigned char feature2;
    unsigned char feature3;
    unsigned char feature4;
    unsigned char feature5;
};
static_assert(sizeof(struct mpfps) == 4 * 4, "wrong size for mpfps");


struct mpcth {
    char signature[4];
    unsigned short length;
    char spec_rev;
    char checksum;
    char oemid[8];
    char productid[12];
    unsigned int oemptr;
    unsigned short oemsize;
    unsigned short count;
    unsigned int lapic;
    unsigned short exttbllen;
    char exttblchksum;
    char reserved;
};
static_assert(sizeof(struct mpcth) == 4 * 11, "wrong size for mpcth");


struct mpct_processor {
    unsigned char type;
    unsigned char lapicid;
    unsigned char lapicver;
    unsigned char cpuflag;
    unsigned int cpusignature;
    unsigned int featureflags;
    unsigned int reserved[2];
};
static_assert(sizeof(struct mpct_processor) == 4 * 5, "wrong size for mpct_processor");


struct mpct_bus {
    unsigned char type;
    unsigned char busid;
    unsigned char bustype[6];
};
static_assert(sizeof(struct mpct_bus) == 4 * 2, "wrong size for mpct_bus");


struct mpct_ioapic {
    unsigned char type;
    unsigned char apicid;
    unsigned char apicver;
    unsigned char flags;
    unsigned int apicaddr;
};
static_assert(sizeof(struct mpct_ioapic) == 4 * 2, "wrong size for mpct_ioapic");


struct mpct_int {
    unsigned char type;
    unsigned char irqtype;
    unsigned short irqflag;
    unsigned char srcbus;
    unsigned char srcbusirq;
    unsigned char dstapic;
    unsigned char dstirq;
};
static_assert(sizeof(struct mpct_int) == 4 * 2, "wrong size for mpct_int");

