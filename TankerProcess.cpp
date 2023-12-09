#include "TankerProcess.hpp"

void TankerProcess::Behavior() {
    // tankers++;
    int nick = tankers; // DEBUG
    Enter(tanker);
    Wait(48); // assemble tanker with booster
    Into(LaunchingRocketsQueue);
    (new LaunchPadProcess)->Activate(); // activate the launch pad repair process
    Passivate();
    // std::cout << "LAUNCHING TANKER " << LT << std::endl;
    // std::cout << "TIME " << Time << std::endl;
    Wait(0.167); // time to reach orbit
    (new BoosterProcess)->Activate();
    Wait(4); // time to refuel
    Leave(tanker);
    // std::cout << "LEAVING TANKER " << nick << std::endl;
    (TankerQueue.GetFirst())->Activate();
}