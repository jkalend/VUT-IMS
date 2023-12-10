/*
file: StarshipProcess.cpp
authors: xkubin27, xkalen07
description: definition of StarshipProcess behavior
*/

#include "StarshipProcess.hpp"

void StarshipProcess::Behavior() { // starship ready
    if (!BoosterQueue.Empty() && tons_of_material != 0) {
        Entity *p = BoosterQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        Into(StarshipQueue);
        Passivate();
    }
    
}