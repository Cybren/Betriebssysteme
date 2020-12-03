#pragma once

#include "user/app2/Block.h"

class CubeBlock : public Block
{

public:
    virtual ~CubeBlock() = default;

private:
    friend class Field;
    CubeBlock(int id, int posX, int posY);
};