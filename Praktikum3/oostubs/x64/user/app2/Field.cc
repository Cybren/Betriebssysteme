#include "user/app2/Field.h"

#include "user/app2/Block.h"
#include "user/app2/CubeBlock.h"
#include "user/app2/PyramidBlock.h"

#include "device/cga_stream.h"
#include "object/o_stream.h"
#include "user/app2/Array.h"

extern CGA_Stream kout;

int Field::height = 2;

Field::Field(int length, int width)
        : field(length)
        , blocks(1000)
{
    this->length = length;
    this->width = width;

    for (int i = 0; i < length; i++) {
        this->field[i] = *(new Array<Array<Block*>>(width));
        for (int j = 0; j < width; j++) {
            this->field[i][j] = *(new Array<Block*>(Field::height));
            for (int k = 0; k < Field::height; k++) {
                this->field[i][j][k] = (new Block(-1, i, j));
            }
        }
    }
    kout << "Field created: " << length << " " << width << endl;
    this->printField();
}

Field::~Field()
{
    for (int i = 0; i < length; i++) {
        this->field[i] = *(new Array<Array<Block*>>(width));
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < Field::height; k++) {
                delete this->field[i][j][k];
            }
        }
    }
}

int Field::addBlock(Block* block)
{
    bool added = false;
    for (int i = 0; i < Field::height; i++) {
        if (this->field[block->posX][block->posY].get(i)->id == -1) {
            Block* tmp = this->field[block->posX][block->posY].get(i);
            this->field[block->posX][block->posY].set(block, i);
            delete tmp;
            added = true;
            break;
        } else if (this->field[block->posX][block->posY].get(i)->type == Block::Type::PYRAMID) {
            kout << "Adding Block not possible, a PyramidBlock is at the top!" << endl;
            delete block;
            return -1;
        }
    }
    if(!added) {
        return -1;
    }
    this->blocks.add(block);
    return block->id;
}

int Field::addCubeBlock(int posX, int posY)
{
    int id = addBlock(new CubeBlock(this->highestBlockId, posX, posY));
    if (id != -1) {
        this->highestBlockId++;
    }
    return id;
}

int Field::addPyramidBlock(int posX, int posY)
{
    int id = addBlock(new PyramidBlock(this->highestBlockId, posX, posY));
    if (id != -1) {
        this->highestBlockId++;
    }
    return id;
}

bool Field::moveBlock(int id, int newXPos, int newYPos)
{
    if (id > this->highestBlockId || id < 0) {
        kout << "Moving Block with id: " << id << " not possible!" << endl;
        return false;
    }
    Block* block = this->blocks.get(id);
    bool success = this->prepareMove(block, newXPos, newYPos);
    if (!success) {
        kout << "Moving Block with id: " << id << " not possible!" << endl;
        return false;
    }
    block->posX = newXPos;
    block->posY = newYPos;
    this->addBlock(block);
    return true;
}

bool Field::prepareMove(Block* block, int newXPos, int newYPos)
{
    Array<Block*> stack = this->field[newXPos][newYPos];
    if (stack.get(0)->id != -1 && stack.get(stack.getObjectCount())->type == Block::Type::PYRAMID) {
        kout << "Goal position is a pyramid block!" << endl;
        return false;
    }
    if (stack.get(0)->id != -1 && stack.get(stack.getObjectCount())->id != block->id) {
        kout << "Requested block is not on top!" << endl;
        return false;
    }

    int count = this->field[block->posX][block->posY].getObjectCount();
    if (count == 0) {
        this->field[block->posX][block->posY].set(new Block(-1, block->posX, block->posY), count);
    } else {
        this->field[block->posX][block->posY].set(new Block(-1, block->posX, block->posY), count - 1);
    }
    return true;
}

void Field::printField()
{
    for (int i = 0; i < this->length; i++) {
        for (int j = 0; j < this->width; j++) {
            for (int k = 0; k < Field::height; k++) {
                kout << "id: " << this->field[i][j].get(k)->id << " X: " << this->field[i][j].get(k)->posX << " Y: " << this->field[i][j].get(k)->posY
                     << " Z: " << k << endl;
            }
        }
    }
}