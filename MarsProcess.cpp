/*
file: MarsProcess.cpp
authors: xkubin27, xkalen07
description: definition of MarsProcess behavior
*/

#include "MarsProcess.hpp"

void MarsProcess::Behavior() {
    Into(MarsQueue);
    while (!ReadyToDepart.Empty()) { // wait for all rockets to launch
        ReadyToDepart.GetFirst()->Activate();
    }
    Wait(1440); // mars is available for 2 months
    MarsQueue.Clear();
}
