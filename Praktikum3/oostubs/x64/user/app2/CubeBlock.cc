#include "user/app2/CubeBlock.h"

CubeBlock::CubeBlock(int id, int posX, int posY)
        : Block(id, posX, posY)
{
    this->type = Type::CUBE;
}
