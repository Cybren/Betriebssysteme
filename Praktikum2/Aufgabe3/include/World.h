#pragma once

#include <thread>
#include <vector>

class Field;
class World {

public:
    World(int length, int width);
    ~World();

    bool isRunning() const;

protected:
    int receiveConsoleInput();
    std::vector<std::string> split(std::string in);

    std::thread* inputThread;
    Field* field;
    int width;
    int length;
    bool running;
    static const std::string add;
    static const std::string cube;
    static const std::string pyramid;
    static const std::string move;
    static const std::string exit;
};