#pragma once

#include "object/queue.h"
#include "types.h"
#include "user/app2/Array.h"
#include "user/app2/Block.h"

class Field
{

public:
    Field(int length, int width);
    ~Field();

    int addCubeBlock(int posX, int posY);
    int addPyramidBlock(int posX, int posY);
    bool moveBlock(int id, int newXPos, int newYPos);

    void printField();

private:
    int addBlock(Block* block);
    bool prepareMove(Block* block, int newXPos, int newYPos);
    int length;
    int width;
    Array<Array<Array<Block*>>> field;
    Array<Block*> blocks;
    int highestBlockId;

    static int height;
};