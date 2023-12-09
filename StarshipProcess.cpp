#include "StarshipProcess.hpp"

void StarshipProcess::Behavior() { // starship ready
    if (!BoosterQueue.Empty()) {
        Entity *p = BoosterQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        Into(StarshipQueue);
        Passivate();
    }
}