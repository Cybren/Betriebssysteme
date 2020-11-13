#include "Field.h"

#include "CubeBlock.h"
#include "PyramidBlock.h"

#include <sstream>

Field::Field(int length, int width)
{
    this->length = length;
    this->width = width;

    this->field = std::vector<std::vector<std::stack<Block*>>>(this->length);

    for (int i = 0; i < this->length; i++) {
        this->field.at(i) = std::vector<std::stack<Block*>>(this->width);
    }
}

Field::~Field()
{
    for (std::pair<int, Block*> pair : this->blocks) {
        delete pair.second;
    }
}

int Field::addBlock(Block* block)
{
    this->blocks.emplace(block->id, block);
    std::stack<Block*>& tmp = this->field.at(block->posX).at(block->posY);
    if (!tmp.empty()) {
        if (dynamic_cast<PyramidBlock*>(tmp.top()) != nullptr) {
            return -1;
        } else {
            tmp.push(block);
        }
    } else {
        this->field.at(block->posX).at(block->posY).push(block);
    }
    return block->id;
}

int Field::addCubeBlock(int posX, int posY)
{
    int id = this->blockCounter++;
    return addBlock(new CubeBlock(id, posX, posY));
}

int Field::addPyramidBlock(int posX, int posY)
{
    int id = this->blockCounter++;
    return addBlock(new PyramidBlock(id, posX, posY));
}

bool Field::moveBlock(int id, int newXPos, int newYPos)
{
    if (this->blocks.find(id) == this->blocks.end()) {
        return false;
    }
    std::stack<Block*> stack = this->field.at(newXPos).at(newYPos);
    if (!stack.empty() && dynamic_cast<PyramidBlock*>(stack.top()) != nullptr) {
        return false;
    }
    Block* block = this->blocks.at(id);
    stack = this->field.at(block->posX).at(block->posY);
    if (!stack.empty() && stack.top()->id != id) {
        return false;
    }
    if(!this->field.at(block->posX).at(block->posY).empty()) {
        this->field.at(block->posX).at(block->posY).pop();
    }
    block->posX = newXPos;
    block->posY = newYPos;
    this->field.at(newXPos).at(newYPos).push(block);
    return true;
}

std::string Field::toString()
{
    std::stringstream stream;
    for (int i = 0; i < this->length; i++) {
        for (int j = 0; j < this->length; j++) {
            if (this->field.at(i).at(j).empty()) {
                stream << "X ";
            } else {
                stream << this->field.at(i).at(j).top()->toString();
            }
        }
        stream << std::endl;
    }
    return stream.str();
}
