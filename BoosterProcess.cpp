#include "BoosterProcess.hpp"

void BoosterProcess::Behavior() {
    if (!OrbitingRockets.Empty() && !tanker.Full()) {
        if (!OrbitingRockets.Empty()) {
            OrbitingRockets.GetFirst()->Activate();
        }
        // std::cout << "LAUNCHING TANKER SIDE " << LT << std::endl;
        // std::cout << "TIME " << Time << std::endl;
        Into(TankerQueue);
        tankers++;
        // std::cout << "LAUNCHING TANKER SIDE " << tankers << std::endl;
        (new TankerProcess)->Activate();
        Passivate();
    } else if (!StarshipQueue.Empty()) {
        Entity *p = StarshipQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        Into(BoosterQueue);
        Passivate();
    }
}