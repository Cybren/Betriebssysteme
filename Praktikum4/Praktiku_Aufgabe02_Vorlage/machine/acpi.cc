#include "machine/acpi.h"
#include "debug/output.h"

namespace ACPI {

static RSDP* rsdp = 0;
static RSDT* rsdt = 0;
static XSDT* xsdt = 0;

static const uint64_t rsdp_signature =
	((uint64_t)'R') |
	(((uint64_t)'S') << 8) |
	(((uint64_t)'D') << 16) |
	(((uint64_t)' ') << 24) |
	(((uint64_t)'P') << 32) |
	(((uint64_t)'T') << 40) |
	(((uint64_t)'R') << 48) |
	(((uint64_t)' ') << 56);

static int checksum(const void *pos, unsigned len){
	uint8_t *mem = (uint8_t*)pos;
	uint8_t sum = 0;
	for(unsigned i=0; i<len; i++){
		sum += mem[i];
	}
	return sum;
}

static RSDP* find_rsdp(const void *pos, unsigned len){

	for(uint64_t *mem = (uint64_t*)pos; mem<((uint64_t*)pos)+len/8; mem+=2)	{
		if( *mem == rsdp_signature ){
			RSDP *rsdp = (RSDP*)mem;
			if( rsdp->revision==0 && checksum(mem,20)==0 )
				return rsdp;
			else if( rsdp->length > 20 && checksum(mem,rsdp->length)==0 )
				return rsdp;
		}
	}
	return 0;
}

bool init(){

	RSDP *rsdp = find_rsdp( (void*)((uintptr_t)(*(uint32_t*)(0x40e))), 1024 );
	if (rsdp == 0)
		rsdp = find_rsdp( (void*)0xe0000, 0xfffff-0xe0000);

	if (rsdp == 0){
		DBG << "No ACPI!" << endl;
		return false;
	}
	rsdt = (RSDT*)((uintptr_t)(rsdp->rsdtaddress));


	if( rsdp->revision != 0 && rsdp->length >= 36 ){
		xsdt = (XSDT*)rsdp->xsdtaddress;
	}
	DBG_VERBOSE << "ACPI revision " <<  rsdp->revision << endl;
	for(unsigned i = 0; i != count(); ++i){
		SDTH *sdt = get(i);
		if (sdt){
			char *c = (char*)&sdt->signature;
			DBG_VERBOSE << i << ". " << c[0] << c[1] << c[2] << c[3] << " @ " << (void*) sdt << endl;
		}
	}
	return true;
}

unsigned count(){
	if (xsdt)
		return (xsdt->length-36)/8;
	else if (rsdt)
		return (rsdt->length-36)/4;
	else
	 	return 0;
}

SDTH *get(unsigned num){
	if (xsdt){
		SDTH *entry = (SDTH*)xsdt->entries[num];
		if (checksum(entry, entry->length)==0 )
			return entry;
	} else if (rsdt){
		SDTH *entry = (SDTH*)((uintptr_t)(rsdt->entries[num]));
		if (checksum(entry, entry->length)==0 )
			return entry;
	}
	return 0;
}

SDTH* get(char a, char b, char c, char d){
	uint32_t signature = (uint32_t)a | ((uint32_t)b << 8) | ((uint32_t)c << 16) | ((uint32_t)d << 24);

	if (xsdt){
		for(unsigned i=0; i<count(); i++){
			SDTH *entry = (SDTH*)xsdt->entries[i];
			if(	entry->signature == signature && checksum(entry, entry->length)==0)
				return entry;
		}
	} else if (rsdt){
		for(unsigned i=0; i<count(); i++){
			SDTH *entry = (SDTH*)((uintptr_t)(rsdt->entries[i]));
			if( entry->signature == signature && checksum(entry, entry->length)==0)
				return entry;
		}
	}
	return 0;
}

int revision(){
	return rsdp ? rsdp->revision : -1;
}
}
