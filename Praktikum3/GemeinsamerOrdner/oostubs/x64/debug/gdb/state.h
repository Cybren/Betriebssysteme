/*! \file
 *  \brief Zustand der CPU
 */
#pragma once

#include "types.h"
#include "handler.h"

#include "machine/fpu.h"

#include "debug/assert.h"

/*! \brief Struktur mit dem Systemzustands nach Traps
 * GDB erlaubt sowohl das Lesen als auch Ändern von Registern.
 * Diese werden zum Beginn (vor der Ausführung des eigentlichen GDB-Stub-Codes)
 * gesichert und anschließend in die vorliegende Struktur geschrieben -- und
 * üblicherweise auch direkt übertragen.
 * Änderungen werden zuerst in dieser Struktur gespeichert und kurz vor dem
 * fortsetzen des eigentlichen Betriebsysstemcodes zurückgeschrieben.
 */
class State {
public:
	// Die von GDB bestimmte Registerreihenfolge auf x64
	enum RegisterNumber {
		// 64 bit General Register
		REG_RAX,
		REG_RBX,
		REG_RCX,
		REG_RDX,
		REG_RSI,
		REG_RDI,
		REG_RBP,
		REG_RSP,
		REG_8,
		REG_9,
		REG_10,
		REG_11,
		REG_12,
		REG_13,
		REG_14,
		REG_15,
		REG_RIP,

		// 32 bit (for GDB) Flag and Segment Register
		REG_EFLAGS,
		REG_CS,
		REG_SS,
		REG_DS, // nicht verwendet
		REG_ES, // nicht verwendet
		REG_FS,
		REG_GS,

		// 80 bit FPU Data
		REG_ST0,
		REG_ST1,
		REG_ST2,
		REG_ST3,
		REG_ST4,
		REG_ST5,
		REG_ST6,
		REG_ST7,

		// 32 bit FPU Status
		REG_FCTRL, // FPU Control Word (fcw)
		REG_FSTAT, // FPU Status Word (fsw)
		REG_FTAG , // FPU Tag Word (ftw)
		REG_FISEG, // FPU IP Selector (fcs)
		REG_FIOFF, // FPU IP Offset (fip)
		REG_FOSEG, // FPU Operand Pointer Selector (fos)
		REG_FOOFF, // FPU Operand Pointer Offset (foo)
		REG_FOP,   // Last Instruction Opcode (fop)

		// 128 bit XMM Register
		REG_XMM0,
		REG_XMM1,
		REG_XMM2,
		REG_XMM3,
		REG_XMM4,
		REG_XMM5,
		REG_XMM6,
		REG_XMM7,
		REG_XMM8,
		REG_XMM9,
		REG_XMM10,
		REG_XMM11,
		REG_XMM12,
		REG_XMM13,
		REG_XMM14,
		REG_XMM15,

		// 32bit XMM Status Register
		REG_MXCSR,

		// Gesamtzahl aller Register
		REGISTERS
	};

private:
	enum Offset {
		OFFSET_GENERAL,
		OFFSET_SEGMENT = REG_EFLAGS,
		OFFSET_FPU_DATA = REG_ST0,
		OFFSET_FPU_STATUS = REG_FCTRL,
		OFFSET_XMM_DATA = REG_XMM0,
		OFFSET_XMM_STATUS = REG_MXCSR,
	};

	enum Size {
		SIZE_GENERAL = OFFSET_SEGMENT - OFFSET_GENERAL,
		SIZE_SEGMENT = OFFSET_FPU_DATA - OFFSET_SEGMENT,
		SIZE_FPU_DATA = OFFSET_FPU_STATUS - OFFSET_FPU_DATA,
		SIZE_FPU_STATUS = OFFSET_XMM_DATA - OFFSET_FPU_STATUS,
		SIZE_XMM_DATA = OFFSET_XMM_STATUS - OFFSET_XMM_DATA,
		SIZE_XMM_STATUS = REGISTERS - OFFSET_XMM_STATUS
	};

	struct Registers {
		uintptr_t general[SIZE_GENERAL];
		uint32_t segment[SIZE_SEGMENT];
		struct FPU::State::ST fpu_data[SIZE_FPU_DATA];
		uint32_t fpu_status[SIZE_FPU_STATUS];
		struct FPU::State::XMM xmm_data[SIZE_XMM_DATA];
		uint32_t xmm_status[SIZE_XMM_STATUS];
	} __attribute__((packed));

	static_assert(REGISTERS == 57, "Wrong x64 GDB Register Size");
	assert_size(Registers, 536);

	Registers registers;

public:
	int signum;

	/*! \brief Zugriffsdaten für ein gesichertes Register
	 */
	struct Register {
		void* addr;
		size_t size;
		Register() : addr(nullptr), size(0) {}
		template <typename T>
		Register(T &value) : addr(&value), size(sizeof(T)) {}
	};

	/*! \brief Zugriffsdaten für ein bestimmtes gesichertes Register holen
	 * \param reg Registernummer. Entspricht diese keinem gültigen Register
	 * (also NUM_REGISTERS), so werden alle Register zurückgegeben
	 * \return Struktur mit Speicheraddresse und Größe
	 */
	static Register get(enum RegisterNumber reg = REGISTERS);
	static Register get(uintptr_t reg);

	/*! \brief Sichere den Kontext aus der Struktur (und der FPU)
	 * \param context Zeiger auf den Kontext vor der Unterbrechung
	 */
	static void save(const debug_context * const context);

	/*! \brief Lade den (geänderten) Kontext in die Register zurück
	 * \param context Zeiger auf den Kontext, welcher am Ende der
	 *  Unterbrechungsbehandlung wiederhergestellt wird.
	 */
	static void restore(debug_context * const context);
};
