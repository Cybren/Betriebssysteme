#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

class Verbraucher
{
public:
    Verbraucher(std::queue<std::string>* queue, std::mutex* mutex, std::condition_variable* conditionVariableLeer,
            std::condition_variable* conditionVariableBelegt, int time);
    void run();

private:
    std::string entnehmen();
    void konsumieren(std::string kuchen);
    int time;
    int id;
    std::queue<std::string>* queue;
    std::mutex* mutex;
    std::condition_variable* conditionVariableLeer;
    std::condition_variable* conditionVariableBelegt;
};