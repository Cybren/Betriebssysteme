/*! \file
 *  \brief Enthält Klasse Key_Decoder
 */

#pragma once

#include "machine/key.h"

/*! \brief Dekodierer für die empfangenen Tastaturcontrollercodes
 *  \ingroup io
 *
 *  Die Klasse Key_Decoder dient dazu, aus den Make- und Break-Codes
 *  Modifier und Scancode der gedrückten Taste zu bestimmen.
 */
class Key_Decoder
{
	/*! \brief Prefixbytes für Tasten
	 */
	unsigned char prefix;

	/*! \brief Aktivierte Modifiertasten (z.B. Caps Lock)
	 */
	Key modifier;

protected:
	/*! \brief Aktueller Status (gedrückt oder losgelassen) der Tasten
	 */
	bool status[Key::Scancode::KEYS];

	/*! \brief Konstruktor für den Tastendekoder
	 */
	Key_Decoder(){};

	/*! \brief Interpretiert die Make und Break-Codes der Tastatur und
	 *  liefert den Scancode und Informationen darüber,
	 *  welche zusätzlichen Tasten wie Shift und Ctrl gedrückt wurden.
	 *
	 *  \return gelesene Taste (invalid, falls das das Zeichen noch nicht
	 *  komplett ist und noch Make- oder Breakcodes fehlen)
	 */
	Key decode(unsigned char code);

public:
	/// Namen der LEDs
	enum LED {
		LED_SCROLL_LOCK = 1 << 0,     ///< Scroll Lock
		LED_NUM_LOCK    = 1 << 1,     ///< Num Lock
		LED_CAPS_LOCK   = 1 << 2,     ///< Caps Lock

	};

	/*! \brief Setzt oder löscht die angegebene Leuchtdiode.
	 *  \param led Gibt an, welche LED geschaltet werden soll.
	 *  \param on LED an- oder ausschalten.
	 */
	virtual void set_led (LED led, bool on) = 0;

};
