#pragma once

#include "user/app2/Block.h"

class PyramidBlock : public Block
{

public:
    virtual ~PyramidBlock() = default;

private:
    friend class Field;
    PyramidBlock(int id, int posX, int posY);
};