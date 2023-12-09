#include "LaunchPadProcess.hpp"

void LaunchPadProcess::Behavior() {
    enters++;
    Enter(launchPad);
   // std::cout << "ENTERED LAUNCHPAD" << std::endl;
    Entity *p = LaunchingRocketsQueue.GetFirst();
    p->Activate();
    Wait(Exponential(240)); // the launch pad repair process takes 10 days exponentially
    Leave(launchPad);
    // std::cout << "CLEAR LAUNCHPAD" << std::endl;
}