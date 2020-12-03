#pragma once

class Block
{

public:
    enum Type
    {
        BASIC,
        CUBE,
        PYRAMID
    };

    friend class Field;
    virtual ~Block() = default;

protected:
    Block(int id, int posX, int posY);

    int id;
    int posX;
    int posY;
    Type type;
};
