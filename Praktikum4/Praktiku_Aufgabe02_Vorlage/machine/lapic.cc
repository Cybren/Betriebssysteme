#include "machine/lapic.h"
#include "machine/lapic_registers.h"
#include "machine/io_port.h"

namespace LAPIC {

volatile uintptr_t base = 0xfee00000;

Register read(Index idx) {
	return *((volatile Register *)(base + idx));
}
void write(Index idx, Register value) {
	*((volatile Register *)(base + idx)) = value;
}


union IdentificationRegister {
	struct {
		uint32_t reserved : 24,
		         apic_id  :  8;
	};
	Register value;

	IdentificationRegister() : value(read(Index::Identification)) {}
} __attribute__((packed));


union VersionRegister {
	struct {
		uint32_t version                : 8,
		         reserved_1             : 8,
		         max_lvt_entry          : 8,
		         suppress_eoi_broadcast : 1,
		         reserved_2             : 7;
	};
	Register value;

	VersionRegister() : value(read(Index::Version)) {}
} __attribute__((packed));


union LogicalDestinationRegister {
	struct {
		uint32_t reserved : 24;
		uint32_t lapic_id :  8;
	};
	Register value;

	LogicalDestinationRegister() : value(read(Index::LogicalDestination)) {}
	~LogicalDestinationRegister() {
		write(Index::LogicalDestination, value);
	}
} __attribute__((packed));

enum Model {
	Cluster = 0x0,
	Flat = 0xf
};

union DestinationFormatRegister {
	struct {
		uint32_t reserved : 28;
		Model    model    :  4;
	};
	Register value;
	DestinationFormatRegister() : value(read(Index::DestinationFormat)) {}
	~DestinationFormatRegister() {
		write(Index::DestinationFormat, value);
	}
} __attribute__((packed));


union TaskPriorityRegister {
	struct {
		uint32_t task_prio_sub :  4,
		         task_prio     :  4,
		         reserved      : 24;
	};
	Register value;
	TaskPriorityRegister() : value(read(Index::TaskPriority)) {}
	~TaskPriorityRegister() {
		write(Index::TaskPriority, value);
	}
} __attribute__((packed));


enum APICSoftware {
	APICDisabled = 0,
	APICEnabled = 1,
};
enum FocusProcessorChecking {
	CheckingEnabled = 0,
	CheckingDisabled = 1,
};
enum SuppressEOIBroadcast {
	Broadcast = 0,
	SuppressBroadcast = 1,
};
union SpuriousInterruptVectorRegister  {
	struct {
		uint32_t spurious_vector :  8;
		APICSoftware apic_software :  1;
		FocusProcessorChecking focus_processor_checking : 1;
		uint32_t reserved_1 : 2;
		SuppressEOIBroadcast eoi_broadcast_suppression : 1;
		uint32_t	reserved:19;
	};
	Register value;

	SpuriousInterruptVectorRegister() : value(read(Index::SpuriousInterruptVector)) {}
	~SpuriousInterruptVectorRegister() {
		write(Index::SpuriousInterruptVector, value);
	}
} __attribute__((packed));
static_assert(sizeof(SpuriousInterruptVectorRegister) == 4, "LAPIC Spurious Interrupt Vector has wrong size");

uint8_t getID() {
	IdentificationRegister ir;
	return ir.apic_id;
}

uint8_t getLogicalID() {
	LogicalDestinationRegister ldr;
	return ldr.lapic_id;
}

uint8_t getVersion() {
	VersionRegister vr;
	return vr.version;
}

void init(uint8_t logicalID) {


	LogicalDestinationRegister ldr;
	ldr.lapic_id = logicalID;


	TaskPriorityRegister tpr;
	tpr.task_prio = 0;
	tpr.task_prio_sub = 0;


	DestinationFormatRegister dfr;
	dfr.model = Model::Flat;


	SpuriousInterruptVectorRegister sivr;
	sivr.spurious_vector = 0xff;
	sivr.apic_software = APICSoftware::APICEnabled;
	sivr.focus_processor_checking = FocusProcessorChecking::CheckingDisabled;
}

void endOfInterrupt() {

	read(SpuriousInterruptVector);


	write(EOI, 0);
}

}
