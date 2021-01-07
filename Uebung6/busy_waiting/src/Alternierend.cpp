#include "Alternierend.h"
#include <iostream>
#include <unistd.h>

const int Alternierend::gesamtZahl = 4;
Konto Alternierend::konto;
int Alternierend::selectedTask = 0;

Alternierend::Alternierend(int id)
{
    this->id = id;
}

void Alternierend::run()
{
    {
        while (true) {
            //TODO implement me
            while(selectedTask != id);
            konto.setKontoStand(konto.getKontoStand()+(rand()%21-10));
            std::cout << "[Task " << id << "]" << "Kontostand: " << konto.getKontoStand() << std::endl;
            selectedTask = (selectedTask+1)%gesamtZahl;
        }
    }
}