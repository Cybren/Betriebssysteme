#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

class Konditor
{
public:
    Konditor(std::queue<std::string>* queue, std::mutex* mutex, std::condition_variable* conditionVariableLeer,
            std::condition_variable* conditionVariableBelegt, int time, int id);
    void run();

private:
    std::string backen();
    void einfuegen(std::string kuchen);
    int time;
    int id;
    std::queue<std::string>* queue;
    std::mutex* mutex;
    std::condition_variable* conditionVariableLeer;
    std::condition_variable* conditionVariableBelegt;
};