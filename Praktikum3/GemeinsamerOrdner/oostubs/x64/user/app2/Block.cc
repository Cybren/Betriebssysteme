#include "user/app2/Block.h"

Block::Block(int id, int posX, int posY)
{
    this->id = id;
    this->posX = posX;
    this->posY = posY;
    this->type = Type::BASIC;
}