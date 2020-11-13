#include "CubeBlock.h"

CubeBlock::CubeBlock(int id, int posX, int posY)
        : Block(id, posX, posY)
{
}

std::string CubeBlock::toString() {
    return "\033[1;36m" + std::to_string(this->id) + " \033[0m";
}