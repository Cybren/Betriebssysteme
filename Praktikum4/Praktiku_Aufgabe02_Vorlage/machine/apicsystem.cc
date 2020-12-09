#include "machine/apicsystem.h"
#include "machine/io_port.h"
#include "machine/lapic.h"
#include "machine/lapic_registers.h"
#include "utils/memutil.h"
#include "debug/output.h"
#include "machine/acpi.h"


extern char ___SETUP_AP_START__, ___SETUP_AP_END__;
extern "C" void setup_ap(void);
namespace LAPIC {
	extern volatile uintptr_t base;
}
namespace System {

const unsigned LAPIC_MAX = CPU_MAX;
static struct {
	uint32_t id;
	uint32_t cpu;
} lapic[LAPIC_MAX];
static unsigned lapics = 0;

static struct {
	uint32_t id;
	void* address;
	uint32_t interrupt_base;
} ioapic;

static uint8_t slot_map[16];

static uint8_t cpu_id[255];
static unsigned cpus = 0;
static unsigned online_cpus = 0;
static bool online_cpu[CPU_MAX];


const uintptr_t relocatedSetupAP = 0x40000;
static_assert((relocatedSetupAP & ~0x000ff000) == 0, "Not a valid 1 MB address for relocatedSetupAP!");

static void disablePICs() {
	DBG_VERBOSE << "PIC comp mode, disabling PICs." << endl;


	IO_Port port1(0x22);
	port1.outb(0x70);

	IO_Port port2(0x23);
	port2.outb(0x01);
}

bool init() {
	if (!ACPI::init()) {
		DBG << "no ACPI!" << endl;
	}

	ACPI::MADT *madt = (ACPI::MADT*)ACPI::get('A', 'P', 'I', 'C');
	if(madt == 0) {
		DBG << "ERROR: no MADT found in ACPI" << endl;
		return false;
	}


	LAPIC::base = (uintptr_t)(madt->local_apic_address);
	DBG_VERBOSE << "LAPIC Address " << (void*)((uintptr_t)madt->local_apic_address) << endl;


	if (madt->flags_pcat_compat)
		disablePICs();


	for (unsigned i = 0; i < sizeof(slot_map)/sizeof(slot_map[0]); i++) {
		slot_map[i] = i;
	}


	lapics = 0;
	for (ACPI::SubHeader *mads = madt->first(); mads < madt->end(); mads = mads->next())
		switch(mads->type) {
			case ACPI::MADS::Type_LAPIC:
				{
					ACPI::MADS::LAPIC* mads_lapic = (ACPI::MADS::LAPIC*)mads;
					if (!mads_lapic->flags_enabled) {
						DBG_VERBOSE << "Detected disabled LAPIC with ID " << (unsigned)mads_lapic->apic_id << endl;
					} else if (cpus >= CPU_MAX) {
						DBG << "Got more CPUs than CPU_MAX" << endl;
					} else {
						DBG_VERBOSE << "Detected LAPIC with ID " << (unsigned)mads_lapic->apic_id << endl;
						cpu_id[mads_lapic->apic_id] = cpus;
						lapic[lapics].id = mads_lapic->apic_id;
						lapic[lapics].cpu = cpus++;
						lapics++;
						assert(lapics <= LAPIC_MAX);
					}
					break;
				}
			case ACPI::MADS::Type_IOAPIC:
				{
					ACPI::MADS::IOAPIC* mads_ioapic = (ACPI::MADS::IOAPIC*)mads;
					DBG_VERBOSE << "Detected IO APIC with ID " << (unsigned)mads_ioapic->ioapic_id << " / Base " << (void*)((uintptr_t)mads_ioapic->global_system_interrupt_base) << endl;
					if (mads_ioapic->global_system_interrupt_base > 23) {
						DBG << "Ignoring IOAPIC since we currently only support one." << endl;
					} else {
						ioapic.id = mads_ioapic->ioapic_id;
						ioapic.address = (void*)((uintptr_t)mads_ioapic->ioapic_address);
						ioapic.interrupt_base = mads_ioapic->global_system_interrupt_base;
					}
					break;
				}
			case ACPI::MADS::Type_Interrupt_Source_Override:
				{
					ACPI::MADS::Interrupt_Source_Override* mads_iso = (ACPI::MADS::Interrupt_Source_Override*)mads;
					if (mads_iso->bus == 0) {
						DBG_VERBOSE << "Overriding Interrupt Source " << (unsigned)mads_iso->source << " with " <<  mads_iso->global_system_interrupt << endl;
						if (mads_iso->source < sizeof(slot_map)/sizeof(slot_map[0]))
							slot_map[mads_iso->source] = mads_iso->global_system_interrupt;
					} else {
						DBG_VERBOSE << "Override for bus " << mads_iso->bus << " != ISA. Does not conform to ACPI." << endl;
					}
					break;
				}
			case ACPI::MADS::Type_LAPIC_Address_Override:
				{
					ACPI::MADS::LAPIC_Address_Override* mads_lao = (ACPI::MADS::LAPIC_Address_Override*)mads;
					LAPIC::base = (uintptr_t)(mads_lao->lapic_address_low);
					DBG_VERBOSE << "Overriding LAPIC address with " << (void*)((uintptr_t)mads_lao->lapic_address) << endl;
					break;
				}
		}

	return true;
}

uint8_t getIOAPICSlot(System::Device device) {
	return slot_map[device];
}

int getCPUID() {
	return cpu_id[LAPIC::getID()];
}

void* getIOAPICAddress() {
	return ioapic.address;
}

uint8_t getIOAPICID() {
	return ioapic.id;
}

static void delay(unsigned int wraparounds) {
	for (unsigned int i = 0; i < wraparounds; i++) {
		unsigned int curr_count, prev_count = ~0;
		int delta;

		IO_Port port1(0x43);
		IO_Port port2(0x40);
		port1.outb(0x00);
		curr_count = port2.inb();
		curr_count |= port2.inb() << 8;

		do {
			prev_count = curr_count;
			port1.outb(0x00);
			curr_count = port2.inb();
			curr_count |= port2.inb() << 8;
			delta = curr_count - prev_count;






		} while (delta < 300);
	}
}

void bootAPs() {


	uint8_t vector = relocatedSetupAP >> 12;





	memcpy(reinterpret_cast<void*>(relocatedSetupAP), &___SETUP_AP_START__, &___SETUP_AP_END__ - &___SETUP_AP_START__);
	DBG_VERBOSE << "relocatedSetupAP = " << relocatedSetupAP << endl;


	LAPIC::IPI::sendInit();


	delay(1);





	DBG_VERBOSE << "Sending STARTUP IPI #1" << endl;
	LAPIC::IPI::sendStartup(vector);

	delay(1);

	DBG_VERBOSE << "Sending STARTUP IPI #2" << endl;
	LAPIC::IPI::sendStartup(vector);
}

void setupThisProcessor() {


	uint8_t cpu = cpu_id[LAPIC::getID()];


	LAPIC::init(getLogicalLAPICID(cpu));


	online_cpu[cpu] = true;
	__sync_fetch_and_add(&online_cpus, 1);
}

unsigned getNumberOfCPUs() {
	return cpus;
}

unsigned getNumberOfOnlineCPUs() {
	return online_cpus;
}

bool isOnlineCPU(uint8_t id) {
	return id > CPU_MAX ? false : online_cpu[id];
}

uint8_t getLogicalLAPICID(uint8_t cpu) {
	return cpu < CPU_MAX ? (1 << cpu) : 0;
}

}
