
#pragma once
#include "machine/mp_registers.h"

#define CPU_MAX 8

struct system_configuration {
    unsigned int cpu_count;
    unsigned int bsp_id;
    int ioapic_id;
    bool disablePICs;
    char ioapic_irqs[24];
    char slot_map[16];
    unsigned int lapic_id[CPU_MAX];

    system_configuration() : cpu_count(0), bsp_id(0), ioapic_id(-1), disablePICs(false) {
        for (int i = 0; i < 24; ++i) {
            ioapic_irqs[i] = -1;
            if (i < 16) {
                slot_map[i] = -1;
            }
        }
    }

    void pretty_print();
};

typedef struct system_configuration SystemConfig;

class APICSystem {
public:
    enum SystemType {
        MP_APIC,
        UP_APIC,
        UNDETECTED
    };
    enum Device {
        timer = 0,
        keyboard = 1,
        com1 = 4,
        com2 = 3,
        com3 = 4,
        com4 = 3,
        floppy = 6,
        lpt1 = 7,
        rtc = 8,
        ps2mouse = 12,
        ide1 = 14,
        ide2 = 15
    };
private:
    SystemType system_type;
    unsigned int onlineCPUs;
    int compatibilityMode;
    int cpuFamily[CPU_MAX];
    int cpuModel[CPU_MAX];
    unsigned char cpuID[255];
    unsigned int lapicVersion[CPU_MAX];
    unsigned char logicalLAPICID[CPU_MAX];
    volatile char callout_cpu_number;
    void *relocatedSetupAP;
    SystemConfig mps_conf;
    SystemConfig acpi_conf;
    SystemConfig *sys_conf;
public:
    APICSystem() : system_type(UNDETECTED), onlineCPUs(1),
                   callout_cpu_number(0),
                   relocatedSetupAP((void *) 42) {
        copySetupAPtoLowMem();
    }

    void detectSystemType();
    bool bootCPU(unsigned int cpu_id, void *top_of_stack);
    int getCPUID();
    void sendCustomIPI(unsigned char logicalDestination, unsigned char vector);
    unsigned char getIOAPICSlot(APICSystem::Device device);
    const char *getCPUModel(unsigned int cpuID);

    SystemType getSystemType() {
        return system_type;
    }

    unsigned int getNumberOfCPUs() {
        return sys_conf->cpu_count;
    }

    unsigned int getNumberOfOnlineCPUs() {
        return onlineCPUs;
    }

    unsigned char getIOAPICID() {
        return sys_conf->ioapic_id;
    }

    unsigned char getBSPID() {
        return sys_conf->bsp_id;
    }

    void setupThisProcessor();

    unsigned char getLogicalLAPICID(unsigned char cpu) {
        return logicalLAPICID[cpu];
    }

private:
    void queryACPI(SystemConfig &conf);
    bool mpbChecksumOK(unsigned char *start_address, int len);
    void readMPCT(struct mpcth *mpcth, SystemConfig &conf);
    bool searchConfig(unsigned long base, unsigned long length, SystemConfig &conf);
    void delay(unsigned int wraparounds);
    void copySetupAPtoLowMem();
    void disablePICs();
    void generateIRQMap(char ioapic_irqs[], char slot_map[]);

};

extern APICSystem system;

