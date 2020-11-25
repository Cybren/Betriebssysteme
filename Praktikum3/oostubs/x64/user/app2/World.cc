#include "user/app2/World.h"

#include "device/cga_stream.h"
#include "object/o_stream.h"
#include "user/app2/Field.h"
extern CGA_Stream kout;

World::World(int length, int width)
{
    this->length = length;
    this->width = width;
    this->running = true;

    kout << "World created: " << this->length << " " << this->width << endl;

    this->field = new Field(this->length, this->width);
}

void World::printField() {
    this->field->printField();
}

int World::handleKeyboardInput(int action, int id, int posX, int posY)
{
    /**
     * Innerhalb dieser Methode sollen die Befehle, die von der KeybordApllication entgegen genommen urden,
     * umgesetzt werden.
     * 1.: Soll ein Block hinzugefügt werden, muss anhand des ID Parameters entschieden werden um welchen Typ es
     * sich handelt und der neue Block an die entsprechende Position gesetzt werden
     * 2.: Soll ein Block bewegt werden, muss der entsprechende Block an die Position gesetzt werden.
     * 3.: Ausgabe des Resultierenden Feldes
     * 4.: Geben Sie die resultierenden IDs zurück
     */
    //TODO Implement me

}

World::~World()
{
    delete this->field;
}

bool World::isRunning() const
{
    return running;
}
