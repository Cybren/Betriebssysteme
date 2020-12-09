

#pragma once

#include "types.h"


namespace LAPIC {

	void init(uint8_t logicalID);


	void endOfInterrupt();


	uint8_t getID();

	uint8_t getLogicalID();
	void setLogicalID(uint8_t id);

	uint8_t getVersion();

namespace IPI {


	bool isDelivered();

	void send(uint8_t destination, uint8_t vector);

	void sendGroup(uint8_t logical_destination, uint8_t vector);

	void sendAll(uint8_t vector);

	void sendOthers(uint8_t vector);

	void sendInit(bool assert = true);

	void sendStartup(uint8_t vector);

}

namespace Timer {


	uint32_t ticks();


	void set(uint32_t counter, uint8_t divide, uint8_t vector, bool periodic, bool masked = false);

}
}
