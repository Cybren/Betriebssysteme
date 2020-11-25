#include "user/app2/kappl.h"

#include "device/cga_stream.h"
extern CGA_Stream kout;
#include "syscall/guarded_keyboard.h"
extern Guarded_Keyboard keyboard;
#include "syscall/guarded_semaphore.h"
extern Guarded_Semaphore koutsem;

#include "syscall/guarded_bell.h"

#include "user/app2/World.h"

void KeyboardApplication::action()
{

    kout << "###############################################################" << endl;
    kout << "################## Create Block: 1 Type X Y ###################" << endl;
    kout << "################# Type: 1 = Cube 2 = Pyramid ##################" << endl;
    kout << "##################### Move Block: 2 ID X Y ####################" << endl;
    kout << "###############################################################" << endl;

    //TODO Fragen Sie die Größe des Felds vom Nutzer ab!
    /**
     * Die KeyboardApplication erlaubt Zugriff auf den aktuelle gedrückten Key
     * Zunächst ist zu überprüfen ob es sich um einen validen Key handelt.
     * Ist dies der Fall, soll der ASCII Wert (https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html)
     * des Keys augelesen werden in die richtige Zahl überführt werden, beispielsweise Drücken der 1 Taste liefert den Wert 49.
     * OOStubs unterstützt keinen Nummernblock. Daher sollten die normalen Nummerntasten verwednet werden!
     * Bei fehlerhaften Eingaben soll der Wert 3 für length und width angenommen werden.
     */

    kout << "Insert World length (1 digit): " << flush;
    int length;
    int width;

    char input = '0';
    Key key = keyboard.getkey();


    //TODO implement

    length = key - input;
    if(length > 9 || length < 1){
        kout << "invalid length, setting length to 3" << flush;
    }

    kout << length << endl;

    kout << "Insert World width (1 digit): " << flush;

    key = keyboard.getkey();
    //TODO implement
    width = key - input;

    if(width > 9 || width < 1){
        kout << "invalid width, setting width to 3" << flush;
    }

    kout << width << endl;

    World* world = new World(length, width);

    while (world->isRunning()) {
        Guarded_Bell bell;
        bell.sleep(1000);
        handleInput(world);
    }

    delete world;

    while (true) {
    }
}

void KeyboardApplication::handleInput(World* world) const
{

    //TODO Implementieren Sie die Interaktion mit der Welt!
    /**
     * Um mit der World zu Interagieren, sollen einige Eingaben vom Nutzer abgefragt werden, welche durch die Befehle in
     * den Zeilen 17 - 21 in dieser Klasse definiert sind. Dazu sollen vom Nutzer 4 verschiedene Eingaben abgefagt werden:
     * 1.: Die Action die durchgefürt werden soll
     * 2.: Falls die Action Create Block ist (1) soll der Type abgefragt werden. Falls die Action Move Block (2) ist die ID
     * 3.: Die X- und Y-Position des Blocks
     */
    int action = 0;
    int posX = 0;
    int posY = 0;
    int id = 0;

    // TODO implement
    char input = '0';
    kout << "Insert action (1 digit): " << flush;

    Key key = keyboard.getkey();

    action = key - input;

    if(action == 1){
        kout << "Insert type (1 digit): " << flush;
        key = keyboard.getkey();
        id = key - input;

        kout << "Insert x-pos (1 digit): " << flush;
        key = keyboard.getkey();
        posX = key - input;

        kout << "Insert y-pos (1 digit): " << flush;
        key = keyboard.getkey();
        posY = key - input;

        world->handleKeyboardInput(action, id, posX, posY);
    }else if (action == 2){
        kout << "Insert ID (1 digit): " << flush;
        key = keyboard.getkey();
        id = key - input;

        kout << "Insert x-pos (1 digit): " << flush;
        key = keyboard.getkey();
        posX = key - input;

        kout << "Insert y-pos (1 digit): " << flush;
        key = keyboard.getkey();
        posY = key - input;

        world->handleKeyboardInput(action, id, posX, posY);
    }else{
        kout << "invalid action." << flush;
    }
}
