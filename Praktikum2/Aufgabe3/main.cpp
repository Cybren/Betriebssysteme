#include <iostream>
#include <World.h>
#include <unistd.h>

int main() {
    std::cout << "\033[1;31m######################################################################\033[0m" << std::endl;
    std::cout << "\033[1;31m#################### Create Block: add TYPE X Y ######################\033[0m" << std::endl;
    std::cout << "\033[1;31m##################### Move Block: move ID X Y ########################\033[0m" << std::endl;
    std::cout << "\033[1;31m############################ Exit: exit ##############################\033[0m" << std::endl;
    std::cout << "\033[1;31m######################################################################\033[0m" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Insert World length: " << std::endl;
    std::string length;
    getline(std::cin, length);
    std::cout << "Insert World width: " << std::endl;
    std::string width;
    getline(std::cin, width);

    World* world = new World(std::stoi(length), std::stoi(width));

    while (world->isRunning()) {
        sleep(1);
    }

    delete world;

    return 0;
}