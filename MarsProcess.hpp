/*
file: MarsProcess.hpp
authors: xkubin27, xkalen07
description: definition of MarsProcess class
*/

#ifndef MARS
#define MARS

#include <iostream>
#include <simlib.h>
#include <unistd.h>

extern Queue MarsQueue, ReadyToDepart, BoosterQueue, OrbitingRockets;

class MarsProcess: public Process {
    void Behavior ();
};

#endif
