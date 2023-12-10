/*
file: BoosterProcess.hpp
authors: xkubin27, xkalen07
description: definition of BoosterProcess class
*/

#ifndef BOOSTER
#define BOOSTER

#include <iostream>
#include <simlib.h>
#include <unistd.h>
#include "RocketProcess.hpp"
#include "TankerProcess.hpp"

extern int tankers;
extern Store tanker;
extern Queue OrbitingRockets, TankerQueue, StarshipQueue, BoosterQueue;

class BoosterProcess : public Process {
    void Behavior();
};

#endif