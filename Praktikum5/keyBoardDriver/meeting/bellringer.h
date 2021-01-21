
#pragma once
#include "meeting/bell.h"
#include "object/queue.h"

class Bellringer {
    Bellringer(const Bellringer &) = delete;
    Bellringer &operator=(const Bellringer &) = delete;
public:
    Bellringer() {}

public:
    void check();
    void job(Bell *bell, unsigned int ms);
    void cancel(Bell *bell);
    bool bell_pending();
private:
    Queue<Bell, &Bell::bellringer_link> bells;

};

