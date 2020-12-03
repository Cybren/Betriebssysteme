#include "user/app2/PyramidBlock.h"

PyramidBlock::PyramidBlock(int id, int posX, int posY)
        : Block(id, posX, posY)
{
    this->type = Type::PYRAMID;
}
