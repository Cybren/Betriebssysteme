#pragma once

#include "Konto.h"

class Alternierend
{
public:
    Alternierend(int id);
    void run();
    const static int gesamtZahl;
    static Konto konto;
    static int selectedTask;

private:
    int id;
};
