#include "Scheduler.h"

Scheduler::Scheduler(std::queue<Task> *taskQueue) {
    this->taskQueue = taskQueue;
}

void Scheduler::schedule() {
    /**
     * Normalerweise ist die Anzahl der Tasks in einem System nicht bekannt und kann zur Laufzeit nicht vorhergesagt werde.
     * Damit das von Ihnen entwickelte Scheduling terminiert, wird anstatt der üblichen while(true) eine for-Schleife
     * verwendet, die die maximale Anzahl der Tasks enthält.
     */
    for(int i = 0; i < 6; i++) {
        //TODO implement me
    }
}