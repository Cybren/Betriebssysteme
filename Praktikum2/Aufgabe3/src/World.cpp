#include "World.h"

#include "Field.h"

#include <iostream>

const std::string World::add = "add";
const std::string World::cube = "cube";
const std::string World::pyramid = "pyramid";
const std::string World::move = "move";
const std::string World::exit = "exit";

World::World(int length, int width)
{
    this->length = length;
    this->width = width;
    this->field = new Field(this->length, this->width);
    this->running = true;

    this->inputThread = new std::thread(&World::receiveConsoleInput, this);
}

World::~World()
{
    // TODO implement me
}

int World::receiveConsoleInput()
{
    //TODO implement me
    return 0;
}

bool World::isRunning() const
{
    return running;
}

std::vector<std::string> World::split(std::string in) {
    char separator = ' ';
    int begin = 0;
    std::vector<std::string> result;

    while ((begin = in.find_first_not_of(separator, begin)) != std::string::npos) {
        int end = in.find_first_of(separator, begin);
        result.push_back(in.substr(begin, end - begin));
        begin = end;
    }
    return result;
}
