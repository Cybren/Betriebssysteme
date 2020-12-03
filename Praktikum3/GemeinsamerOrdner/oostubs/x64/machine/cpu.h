/*! \file
 *  \brief Enthält die Klasse CPU
 */

/*! \defgroup sync CPU Synchronisation
 *
 * Im Synchronisationsmodul sind die Funktionalitäten implementiert,
 * die benötigt werden um mehrere Prozessoren und Aktivitäten
 * miteinander in Einklang zu bringen. Dies bezieht sich insbesondere
 * darauf, wenn mehrere Teilnehmer auf eine Ressource zugreifen
 * wollen. Dabei können die Teilnehmer auf der gleichen CPU oder
 * unterschiedlichen Prozessoren laufen.
 */

#pragma once

#include "types.h"

/*! \brief Implementierung einer Abstraktion fuer den Prozessor.
 *
 *  Derzeit wird angeboten: Interrupts zuzulassen, zu verbieten, den
 *  Prozessor in den Haltmodus zu schicken oder ganz anzuhalten.
 */
namespace CPU
{

	namespace Interrupt {
		/*! \brief Erlauben von (Hardware-)Interrupts
		 *  \ingroup sync
		 *
		 *
		 *  Lässt die Unterbrechungsbehandlung zu, indem die Assembleranweisung
		 *  \b sti ausgeführt wird.
		 */
		inline void enable() {
			asm volatile("sti\n\t nop\n\t" : : : "memory");
		}

		/*! \brief Interrupts werden ignoriert/verboten
		 *  \ingroup sync
		 *
		 *  Verhindert eine Reaktion auf Unterbrechungen, indem die
		 *  Assembleranweisung \b cli ausgeführt wird. Der Rückgabewert gibt an ob
		 *  Unterbrechungen schon gesperrt waren.
		 *  \return true wenn die Unterbrechungen zum Zeitpunkt des Aufrufs frei waren, false wenn sie schon gesperrt waren.
		 */
		inline bool disable() {
			uintptr_t out;
			asm volatile (
				"pushf\n\t"
				"pop %0\n\t"
				"cli\n\t"
				: "=r"(out)
				:
				: "memory"
			);

			bool enabled = (out & 0x200) != 0;
			return enabled;
		}

		/*! \brief Unterbrechungszustand wiederherstellen
		 *
		 *  Zusammen mit dem Rückgabewert von CPU::disable_int() kann
		 *  diese Funktion verwendet werden, um das Sperren und freigeben
		 *  von Unterbrechungen zu schachteln.
		 *  \param val Gibt an ob Unterbrechungen wieder freigegeben werden sollen oder nicht.
		 */
		inline void restore(bool val) {
			if(val)
				enable();
		}
	}

	/*! \brief Prozessor einen Hinweis auf einen Spinloop geben und "kurz" anhalten
	 *
	 * Fördert die CPU Performance während eines Spinloops, indem ein kleiner
	 * Moment gewartet wird und nicht auf den Speicher zugegriffen wird.
	 */
	inline void pause() {
		asm volatile("pause\n\t" : : : "memory");
	}

	/*! \brief Prozessor bis zum nächsten Interrupt anhalten
	 *
	 *  Versetzt den Prozessor in den Haltezustand, aus dem er nur durch einen
	 *  Interrupt wieder erwacht. Intern werden dazu die Interrupts mit \b sti
	 *  freigegeben und der Prozessor mit \b hlt angehalten. Intel garantiert,
	 *  dass die Befehlsfolge \b sti \b hlt atomar ausgeführt wird.
	 */
	inline void idle() {
		asm volatile("sti\n\t hlt\n\t" : : : "memory");
	}
	/*! \brief Prozessor dauerhaft anhalten
	 *
	 *  Hält den Prozessor an. Intern werden dazu die Interrupts mit \b cli
	 *  gesperrt und anschließend der Prozessor mit \b hlt angehalten. Da der
	 *  Haltezustand nur durch einen Interrupt verlassen werden könnte, ist
	 *  somit garantiert, dass die CPU bis zum nächsten Kaltstart "steht".
	 *  Das Programm kehrt aus die() nie zurück. In einer
	 *  Multiprozessorumgebung hat die Ausführung der Funktion nur
	 *  Auswirkungen auf die CPU, die ihn ausführt. Die anderen CPUs laufen
	 *  weiter.
	 */
	inline void die() {
		asm volatile("cli\n\t hlt\n\t" : : : "memory");
	}

	/*! \brief Prozessor anhalten
	 *
	 *  Hält den Prozessor an. Intern wird dazu die \b hlt Instruktion ausgeführt.
	 *  In diesem Zustand verbraucht der Prozessor sehr wenig Strom.
	 *  Nur ein Interrupt kann den Prozessor aufwecken.
	 */
	inline void halt() {
		asm volatile("hlt\n\t" : : : "memory");
	}

	/*! \brief Zugriff auf die Model Specific Register
	 */
	template<uint32_t id>
	class MSR {
	public:
		static uint64_t read(void) {
			uint64_t retval;
			asm volatile("rdmsr" : "=A"(retval) : "c"(id) : "memory");
			return retval;
		}

		static void write(uint64_t val) {
			asm volatile("wrmsr" : : "A"(val), "c"(id) : "memory");
		}
	};

	/*! \brief Zugriff auf die Control Register
	 */
	template<uint8_t id>
	class CR {
	public:
		static uintptr_t read(void) {
			uintptr_t val;
			asm volatile("mov %%cr%c1, %0" : "=r"(val) : "n"(id));
			return val;
		}

		static void write(uintptr_t value) {
			asm volatile("mov %0, %%cr%c1" : : "r"(value), "n"(id));
		}
	};

	/*! \brief Zugriff auf den Timestamp Counter
	 */
	namespace TSC {
		inline uint64_t read(void) {
			uint64_t val;
			asm volatile("rdtsc" : "=A"(val));
			return val;
		}
	}

	/*! \brief Zugriff auf ddie Performance Monitor Counter
	 */
	template<uint8_t id>
	class PMC {
	public:
		static uint64_t read(void) {
			uint64_t val;
			asm volatile("rdpmc" : "=A"(val) : "c"(id) : "memory" );
			return val;
		}
	};
};
