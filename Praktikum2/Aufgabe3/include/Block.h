#pragma once

#include <string>

class Block
{

public:
    friend class Field;
    virtual std::string toString() = 0;
    virtual ~Block() = default;

protected:
    Block(int id, int posX, int posY);

    int id;
    int posX;
    int posY;
};
