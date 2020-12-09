#pragma once

#include "types.h"

namespace ACPI {


	struct RSDP {
		char signature[8];
		uint8_t checksum;
		char oemid[6];
		uint8_t revision;
		uint32_t rsdtaddress;
		uint32_t length;
		uint64_t xsdtaddress;
		uint8_t extended_checksum;
		uint8_t reserved[3];
	} __attribute__((packed));


	struct SDTH {
		uint32_t signature;
		uint32_t length;
		uint8_t revision;
		uint8_t checksum;
		char oemid[6];
		char oem_table_id[8];
		uint32_t oem_revision;
		uint32_t creator_id;
		uint32_t creator_revision;


		void *end(){return (uint8_t*)(this)+length; }
	} __attribute__((packed));


	struct RSDT : SDTH {
		uint32_t entries[];
	} __attribute__((packed));


	struct XSDT : SDTH {
		uint64_t entries[];
	} __attribute__((packed));


	struct SubHeader {
		uint8_t type;
		uint8_t length;


		SubHeader *next(){return (SubHeader*)((uint8_t*)(this)+length); }
	} __attribute__((packed));



	 struct MADT : SDTH
	 {
	 	uint32_t local_apic_address;
	 	uint32_t flags_pcat_compat:1,
	 		flags_reserved:31;


	 	SubHeader *first() {
	 		return (SubHeader*)((uint8_t*)this+sizeof(MADT));
	 	}
	 } __attribute__((packed));


	namespace MADS {
		enum Type {
			Type_LAPIC = 0,
			Type_IOAPIC = 1,
			Type_Interrupt_Source_Override = 2,
			Type_LAPIC_Address_Override = 5,
		};


		struct LAPIC : SubHeader
		{
			uint8_t acpi_processor_id;
			uint8_t apic_id;
			uint32_t flags_enabled:1,
				flags_reserved:31;
		} __attribute__((packed));


		struct IOAPIC : SubHeader
		{
			uint8_t ioapic_id;
			uint8_t reserved;
			uint32_t ioapic_address;
			uint32_t global_system_interrupt_base;
		} __attribute__((packed));


		struct Interrupt_Source_Override : SubHeader
		{
			uint8_t bus;
			uint8_t source;
			uint32_t global_system_interrupt;
			uint16_t flags_polarity:2,
				flags_trigger_mode:2,
				flags_reserved:12;
		} __attribute__((packed));


		struct LAPIC_Address_Override : SubHeader
		{
			uint16_t reserved;
			union {
				uint64_t lapic_address;
				struct {
					uint32_t lapic_address_low;
					uint32_t lapic_address_high;
				} __attribute__((packed));
			};
		} __attribute__((packed));

	}

	bool init();
	unsigned count();
	SDTH *get(unsigned num);
	SDTH *get(char a, char b, char c, char d);
	int revision();
}
