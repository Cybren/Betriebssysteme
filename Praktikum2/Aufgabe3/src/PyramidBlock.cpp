#include "PyramidBlock.h"

PyramidBlock::PyramidBlock(int id, int posX, int posY)
        : Block(id, posX, posY)
{
}

std::string PyramidBlock::toString() {
    return "\033[1;31m" + std::to_string(this->id) + " \033[0m";
}