#pragma once

#include <string>

class Task {
public:
    Task(std::string name, int runtime);
    void execute();

private:
    std::string name;
    int runtime;
};

