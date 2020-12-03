/*! \file
 *  \brief Enthält Klasse Key
 */

#pragma once

#include "types.h"

/*! \brief Abstraktion für eine Taste bestehend aus Scancode und Modifierbits.
 */
struct Key
{
	/*! \brief Scancodes der Tasten (Code 1)
	 */
	enum Scancode {
		// Ungültiger Scancode
		KEY_INVALID = 0,

		// Die "echten" Scancodes
		KEY_ESCAPE,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_0,
		KEY_DASH,
		KEY_EQUAL,
		KEY_BACKSPACE,
		KEY_TAB,
		KEY_Q,
		KEY_W,
		KEY_E,
		KEY_R,
		KEY_T,
		KEY_Y,
		KEY_U,
		KEY_I,
		KEY_O,
		KEY_P,
		KEY_OPEN_BRACKET,
		KEY_CLOSE_BRACKET,
		KEY_ENTER,
		KEY_LEFT_CTRL,
		KEY_A,
		KEY_S,
		KEY_D,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_SEMICOLON,
		KEY_APOSTROPH,
		KEY_GRAVE_ACCENT,
		KEY_LEFT_SHIFT,
		KEY_BACKSLASH,
		KEY_Z,
		KEY_X,
		KEY_C,
		KEY_V,
		KEY_B,
		KEY_N,
		KEY_M,
		KEY_COMMA,
		KEY_PERIOD,
		KEY_SLASH,
		KEY_RIGHT_SHIFT,
		KEY_KP_STAR,
		KEY_LEFT_ALT,
		KEY_SPACEBAR,
		KEY_CAPS_LOCK,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_NUM_LOCK,
		KEY_SCROLL_LOCK,
		KEY_KP_7,
		KEY_KP_8,
		KEY_KP_9,
		KEY_KP_DASH,
		KEY_KP_4,
		KEY_KP_5,
		KEY_KP_6,
		KEY_KP_PLUS,
		KEY_KP_1,
		KEY_KP_2,
		KEY_KP_3,
		KEY_KP_0,
		KEY_KP_PERIOD,
		KEY_SYSREQ,
		KEY_EUROPE_2,
		KEY_F11,
		KEY_F12,
		KEY_KP_EQUAL,

		// Anzahl der Tasten
		KEYS,

		// Aliase
		KEY_DIV   = 8,  // KEY_7
		KEY_DEL   = 83, // KEY_KP_PERIOD
		KEY_UP    = 72, // KEY_KP_8
		KEY_DOWN  = 80, // KEY_KP_2
		KEY_LEFT  = 75, // KEY_KP_4
		KEY_RIGHT = 77, // KEY_KP_6

	};

	Scancode scancode;

	// Bit-Masken fuer die Modifier-Tasten
	bool shift       : 1,
	     alt_left    : 1,
	     alt_right   : 1,
	     ctrl_left   : 1,
	     ctrl_right  : 1,
	     caps_lock   : 1,
	     num_lock    : 1,
	     scroll_lock : 1;

	/*! \brief Default-Konstruktor: setzt ASCII, Scancode und Modifier auf 0
	 *  und bezeichnet so einen ungültigen Tastencode
	 *
	 */
	Key() : scancode(KEY_INVALID), shift(false), alt_left(false), alt_right(false),
	        ctrl_left(false), ctrl_right(false),
	        caps_lock(false), num_lock(false), scroll_lock(false) {}

	/*! \brief Mit Scancode = 0 werden ungültige Tasten gekennzeichnet.
	 *  \return Gibt an, ob der Tastencode gültig ist.
	 */
	bool valid ()      {
		return scancode != KEY_INVALID && scancode < KEYS;
	}

	/*! \brief Setzt den Scancode auf 0 und sorgt somit für einen
	 *  ungültigen Tastencode.
	 *
	 */
	void invalidate () {
		scancode = KEY_INVALID;
	}

	/*! \brief Abfragen des ASCII-Codes
	 *  \return ASCI-Code
	 */
	unsigned char ascii();

	/*! \brief Zeigt an, ob Modifier alt vorhanden ist
	 *
	 */
	bool alt ()         {
		return alt_left | alt_right;
	}

	/*! \brief Zeigt an, ob Modifier ctrl vorhanden ist
	 *
	 */
	bool ctrl ()        {
		return ctrl_left | ctrl_right;
	}

	/*! \brief Liefert ASCII-Wert
	 *
	 */
	operator char ()    {
		return ascii();
	}

};
