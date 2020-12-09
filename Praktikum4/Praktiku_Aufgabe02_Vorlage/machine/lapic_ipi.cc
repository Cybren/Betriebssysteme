#include "machine/lapic_registers.h"

namespace LAPIC {
namespace IPI {


enum DeliveryMode {
	Fixed             = 0,
	LowestPriority    = 1,
	SMI               = 2,

	NMI               = 4,
	Init              = 5,
	InitLevelDeAssert = 5,
	StartUp           = 6,

};


enum DestinationMode {
	Physical = 0,
	Logical  = 1
};

enum DeliveryStatus {
	Idle  = 0,
	SendPending = 1
};


enum Level {
	DeAssert = 0,
	Assert  = 1
};


enum TriggerMode {
	EdgeTriggered  = 0,
	LevelTriggered = 1
};


enum DestinationShorthand {
	NoShorthand      = 0,
	Self             = 1,
	AllIncludingSelf = 2,
	AllExcludingSelf = 3
};


enum InterruptMask {
	Unmasked = 0,
	Masked = 1
};


union InterruptCommand {
	struct {

		uint64_t vector : 8;


		enum DeliveryMode delivery_mode : 3;


		enum DestinationMode destination_mode : 1;


		enum DeliveryStatus delivery_status : 1;

		uint64_t : 1;


		enum Level level : 1;


		enum TriggerMode trigger_mode : 1;

		uint64_t : 2;

		enum DestinationShorthand destination_shorthand : 2;


		uint64_t : 36;


		uint64_t destination : 8;
	} __attribute__((packed));


	struct {
		Register value_low;
		Register value_high;
	} __attribute__((packed));

	InterruptCommand() = default;

	InterruptCommand(uint8_t destination, uint8_t vector = 0,
	                 enum DestinationMode destination_mode = DestinationMode::Physical,
	                 enum DeliveryMode delivery_mode = DeliveryMode::Fixed,
	                 enum TriggerMode trigger_mode = TriggerMode::EdgeTriggered,
	                 enum Level level = Level::Assert) {
		readRegister();
		this->vector = vector;
		this->delivery_mode = delivery_mode;
		this->destination_mode = destination_mode;
		this->level = level;
		this->trigger_mode = trigger_mode;
		this->destination_shorthand =  DestinationShorthand::NoShorthand;
		this->destination = destination;
	}

	InterruptCommand(DestinationShorthand destination_shorthand, uint8_t vector,
	                 enum DeliveryMode delivery_mode = DeliveryMode::Fixed,
	                 enum TriggerMode trigger_mode = TriggerMode::EdgeTriggered,
	                 enum Level level = Level::Assert) {
		readRegister();
		this->vector = vector;
		this->delivery_mode = delivery_mode;
		this->level = level;
		this->trigger_mode = trigger_mode;
		this->destination_shorthand = destination_shorthand;
		this->destination = destination;
	}

	void send() {
		write(InterruptCommandRegisterHigh, value_high);
		write(InterruptCommandRegisterLow, value_low);
	}

	bool isSendPending() {
		value_low = read(InterruptCommandRegisterLow);
		return delivery_status == DeliveryStatus::SendPending;
	}

private:
	void readRegister() {
		while (isSendPending()) {}
		value_high = read(InterruptCommandRegisterHigh);
	}
};
static_assert(sizeof(InterruptCommand) == 8, "LAPIC Interrupt Command has wrong size");

bool isDelivered() {
	InterruptCommand ic;
	return !ic.isSendPending();
}

void send(uint8_t destination, uint8_t vector) {
	InterruptCommand ic(destination, vector);
	ic.send();
}

void sendGroup(uint8_t logical_destination, uint8_t vector) {
	InterruptCommand ic(logical_destination, vector, DestinationMode::Logical);
	ic.send();
}

void sendAll(uint8_t vector) {
	InterruptCommand ic(DestinationShorthand::AllIncludingSelf, vector);
	ic.send();
}

void sendOthers(uint8_t vector) {
	InterruptCommand ic(DestinationShorthand::AllExcludingSelf, vector);
	ic.send();
}

void sendInit(bool assert) {
	LAPIC::IPI::InterruptCommand ic(DestinationShorthand::AllExcludingSelf, 0, DeliveryMode::Init,
	                                assert ? TriggerMode::EdgeTriggered : TriggerMode::LevelTriggered,
	                                assert ? Level::Assert : Level::DeAssert);
	ic.send();
}

void sendStartup(uint8_t vector) {
	InterruptCommand ic(DestinationShorthand::AllExcludingSelf, vector, DeliveryMode::StartUp);
	ic.send();
}

}
}
