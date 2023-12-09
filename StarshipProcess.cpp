#include "StarshipProcess.hpp"

void StarshipProcess::Behavior() { // starship ready
    if (!BoosterQueue.Empty() && tons_of_material != 0) {
        Entity *p = BoosterQueue.GetFirst();
        p->Activate();
        std::cout << "GOT HERE STARSHIP" << std::endl;
        (new RocketProcess)->Activate();
    } else {
        Into(StarshipQueue);
        Passivate();
    }
    
}