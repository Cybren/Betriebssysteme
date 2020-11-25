#pragma once

#include "types.h"

namespace ACPI {

	/* ACPI Spec 4.0a: 5.2.5.3 RSDP Structure */
	struct RSDP {
		char signature[8]; /* must be 'RSD PTR ' */
		uint8_t checksum;
		char oemid[6];
		uint8_t revision;
		uint32_t rsdtaddress;
		uint32_t length;
		uint64_t xsdtaddress;
		uint8_t extended_checksum;
		uint8_t reserved[3];
	} __attribute__((packed));

	/* ACPI Spec 4.0a: 5.2.6 System Description Table Header */
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

		/* methode to get the first sub-header and the pointer right
		 * after the table
		 */
		void *end(){return (uint8_t*)(this)+length; }
	} __attribute__((packed));

	/* ACPI Spec 4.0a: 5.2.7 Root System Description Table (RSDT) */
	struct RSDT : SDTH {
		uint32_t entries[];
	} __attribute__((packed));

	/* ACPI Spec 4.0a: 5.2.8 Extended System Description Table (XSDT) */
	struct XSDT : SDTH {
		uint64_t entries[];
	} __attribute__((packed));

	/******************************************************
	 * Sub-header used for SRAT's and MADT's sub-structures
	 *
	 *******************************************************/
	struct SubHeader {
		uint8_t type;
		uint8_t length;

		/* methodes to travers multiple sub-structures */
		SubHeader *next(){return (SubHeader*)((uint8_t*)(this)+length); }
	} __attribute__((packed));

	/*********************************************
	 * Multiple APIC Definition Table
	 *
	 * used to get the APIC IDs
	 *********************************************/
	 /* ACPI Spec 4.0a: 5.2.12 Multiple APIC Description Table (MADT) */
	 struct MADT : SDTH
	 {
	 	uint32_t local_apic_address;
	 	uint32_t flags_pcat_compat:1,
	 		flags_reserved:31;

	 	/* methode to get first sub-header */
	 	SubHeader *first() {
	 		return (SubHeader*)((uint8_t*)this+sizeof(MADT));
	 	}
	 } __attribute__((packed));

	// Multiple APIC Definition Structure
	namespace MADS {
		enum Type {
			Type_LAPIC = 0,
			Type_IOAPIC = 1,
			Type_Interrupt_Source_Override = 2,
			Type_LAPIC_Address_Override = 5,
		};

		/* ACPI Spec 4.0a: 5.2.12.2 Processor Local APIC Structure */
		struct LAPIC : SubHeader
		{
			uint8_t acpi_processor_id;
			uint8_t apic_id;
			uint32_t flags_enabled:1,
				flags_reserved:31; /* must be 0 */
		} __attribute__((packed));

		/* ACPI Spec 4.0a: 5.2.12.3 I/O APIC Structure */
		struct IOAPIC : SubHeader
		{
			uint8_t ioapic_id;
			uint8_t reserved;
			uint32_t ioapic_address;
			uint32_t global_system_interrupt_base;
		} __attribute__((packed));

		/* ACPI Spec 4.0a: 5.2.12.5 Interrupt Source Override Structure */
		struct Interrupt_Source_Override : SubHeader
		{
			uint8_t bus;
			uint8_t source;
			uint32_t global_system_interrupt;
			uint16_t flags_polarity:2,
				flags_trigger_mode:2,
				flags_reserved:12; /* must be 0 */
		} __attribute__((packed));

		/* ACPI Spec 4.0a: 5.2.12.8 Local APIC Address Override Structure */
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
