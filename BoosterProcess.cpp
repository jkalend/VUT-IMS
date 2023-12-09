#include "BoosterProcess.hpp"

void BoosterProcess::Behavior() {
    if (!OrbitingRockets.Empty()) {
        if (!OrbitingRockets.Empty()) {
            OrbitingRockets.GetFirst()->Activate();
        }
        Into(TankerQueue);
        tankers++;
        (new TankerProcess)->Activate();
        Passivate();
    } else if (!StarshipQueue.Empty() && tons_of_material != 0) {
        Entity *p = StarshipQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        Into(BoosterQueue);
        Passivate();
    }
}