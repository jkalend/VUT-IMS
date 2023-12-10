/*
file: LaunchPadProcess.cpp
authors: xkubin27, xkalen07
description: definition of LaunchPadProcess behavior
*/

#include "LaunchPadProcess.hpp"

void LaunchPadProcess::Behavior() {
    enters++;
    Enter(launchPad);
    Entity *p = LaunchingRocketsQueue.GetFirst();
    p->Activate();
    Wait(Exponential(240)); // the launch pad repair process takes 10 days exponentially
    Leave(launchPad);
}