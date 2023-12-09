#include "TankerProcess.hpp"

void TankerProcess::Behavior() {
    Enter(tanker);
    Wait(48); // assemble tanker with booster
    Into(LaunchingRocketsQueue);
    (new LaunchPadProcess)->Activate(); // activate the launch pad repair process
    Passivate();
    Wait(0.167); // time to reach orbit
    (new BoosterProcess)->Activate();
    booster_returned_from_tanker++;
    Wait(4); // time to refuel
    Leave(tanker);
    (TankerQueue.GetFirst())->Activate();
}