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
     * Innerhalb dieser Methode sollen die Befehle, die von der KeybordApplication entgegen genommen wurden,
     * umgesetzt werden.
     * 1.: Soll ein Block hinzugefügt werden, muss anhand des ID Parameters entschieden werden um welchen Typ es
     * sich handelt und der neue Block an die entsprechende Position gesetzt werden
     * 2.: Soll ein Block bewegt werden, muss der entsprechende Block an die Position gesetzt werden.
     * 3.: Ausgabe des Resultierenden Feldes
     * 4.: Geben Sie die resultierenden IDs zurück
     */
    //TODO Implement me
    int newId = 0;
    if(action == 1){
        if(id == 1){
            newId = field->addCubeBlock(posX, posY);
        }else if(id == 2){
            newId = field->addPyramidBlock(posX, posY);
        }else{
            kout << "invalid type" << flush;
            newId = -1;
        }
    }else{
        field->moveBlock(id, posX, posY);
    }
    field->printField();
    return newId;
}

World::~World()
{
    delete this->field;
}

bool World::isRunning() const
{
    return running;
}
