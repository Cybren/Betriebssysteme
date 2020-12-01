#include "Task.h"

#include <unistd.h>
#include <iostream>

Task::Task(std::string name, int runtime) {
    this->name = name;
    this->runtime = runtime;
}

void Task::execute() {
    std::cout << "Task " << this->name << " started." << std::endl;
    sleep(this->runtime);
    std::cout << "Task " << this->name << " finished." << std::endl;
}