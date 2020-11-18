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

World::~World(){
    this->inputThread->join();
    delete inputThread;
    delete field;
}

int World::receiveConsoleInput(){
    std::string command;

    while(this->isRunning()){
        std::cout << "Enter command: ";
        getline(std::cin, command);
        std::cout << std::endl;
        std::vector<std::string> pieces = this->split(command);

        if(pieces.empty()){
        continue;
        }

        if(pieces.at(0) == World::exit){
            this->running = false;
            break;
        }

        if(pieces.size() != 4){
            std::cerr << "Your command has " << pieces.size() << " parameter, you need to have four or exit" << std::endl;
            continue;
        }else{
            std::string& action = pieces.at(0);
            int xPos = std::stoi(pieces.at(2));
            int yPos = std::stoi(pieces.at(3));

            if(xPos >= this->length || yPos >= this->width){
                std::cerr << "The position x: " << xPos << " y: " << yPos << " is outside the World" << std::endl;
                continue;
            }

            if(action == World::add){
                if(pieces.at(1) == World::pyramid){
                    int blockId = this->field->addPyramidBlock(xPos, yPos);
                    if(blockId != -1){
                        std::cout << "Created pyramid id: " << blockId << std::endl;
                    }else{
                        std::cerr << "You can't place the pyramid at this position" << std::endl;
                        continue;
                    }
                }else if(pieces.at(1) == World::cube){
                    int blockId = this->field->addCubeBlock(xPos, yPos);
                    if(blockId != -1){
                        std::cout << "Created cube id: " << blockId << std::endl;
                    }else{
                        std::cerr << "You can't place the cube at this position" << std::endl;
                        continue;
                    }
                }else{
                    std::cerr << pieces.at(1) << " is not supported. Please use pyramid or cube" << std::endl;
                    continue;
                }
            }else if(action == World::move){
                bool successful = this->field->moveBlock(std::stoi(pieces.at(1)), xPos, yPos);
                if(!successful){
                    std::cerr << "This movement of block " << pieces.at(1) << " is not possible" << std::endl;
                }
            }else{
                std::cerr << "The command: " << command << " is not supported. Please use add, move or exit" << std::endl;
                continue;
            }
            std::cout << this->field->toString() << std::endl;
        }
    }
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
