#pragma once

#include "Block.h"

class CubeBlock : public Block {

public:
    virtual std::string toString();
    virtual ~CubeBlock() = default;

private:
    friend class Field;
    CubeBlock(int id, int posX, int posY);

};