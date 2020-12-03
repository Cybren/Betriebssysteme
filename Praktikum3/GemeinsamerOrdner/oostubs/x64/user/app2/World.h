#pragma once

#include "object/queue.h"
#include "thread/thread.h"
#include "types.h"
#include "user/app2/Field.h"

class Field;
class World
{

public:
    World(int length, int width);
    ~World();

    bool isRunning() const;

    int handleKeyboardInput(int action, int id, int posX, int posY);

    void printField();
protected:
    Field* field;
    int width;
    int length;
    bool running;
};