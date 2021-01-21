#include "Writer.h"
#include <unistd.h>

Writer::Writer(int queue, std::shared_mutex* mutex, int time)
        : queue(queue)
        , mutex(mutex)
        , time(time)
    {
}


void Writer::einfuegen() {
    std::srand(1);
    int rand = std::rand();
    std::unique_lock lock(mutex);



}

void Writer::run()
{
    while (true) {
        sleep(this->time);
        einfuegen();
    }
}
