#pragma once

#include <vector>
#include <map>
#include <stack>

class Block;
class Field {

public:
    Field(int length, int width);
    ~Field();

    int addCubeBlock(int posX, int posY);
    int addPyramidBlock(int posX, int posY);
    bool moveBlock(int id, int newXPos, int newYPos);

    std::string toString();
private:

    int addBlock(Block* block);
    int length;
    int width;
    std::vector<std::vector<std::stack<Block*>>> field;
    std::map<int, Block*> blocks;
    int blockCounter;
};