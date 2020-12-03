/*! \file
 *  \brief Enthält die Klasse KeyboardApplication
 */

#pragma once

#include "World.h"
#include "thread/thread.h"
#include "types.h"

/*! \brief Die Klasse KeyboardApplication definiert eine Anwendung für OO-Stubs.
 */
class KeyboardApplicationTest : public Thread
{
    // Verhindere Kopien und Zuweisungen
    KeyboardApplicationTest(const KeyboardApplicationTest&) = delete;
    KeyboardApplicationTest& operator=(const KeyboardApplicationTest&) = delete;

private:
    int id;
    uint8_t appstack[4096];

public:
    /*! \brief Konstruktor
     *
     * \param i Instanz-ID
     */
    KeyboardApplicationTest(int i = 0)
            : Thread(appstack + 4096)
            , id(i)
    {
    }

    /*! \brief Setzt eine Instanz-ID
     *
     */
    void setID(int i) { id = i; }

    /*! \brief Enthält den Code der Anwendung
     *
     */
    void action();

    void handleInput(World* world) const;
};
