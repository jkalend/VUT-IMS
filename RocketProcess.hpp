#ifndef ROCKET
#define ROCKET

#include <iostream>
#include <simlib.h>
#include <unistd.h>
#include "StarshipProcess.hpp"
#include "LaunchPadProcess.hpp"
#include "BoosterProcess.hpp"
#include "TankerProcess.hpp"
#include "FAAProcess.hpp"

extern int payload_size, reachedMars, returnedFromMars, tons_of_material, rockets, launchRockets,tons_reached_mars, tankers, fills, nick0;
extern Queue BoosterQueue, LaunchingRocketsQueue, MarsQueue, ReadyToDepart, PermissionQueue, TankerQueue, OrbitingRockets;

class RocketProcess : public Process { // starting from rocket available
    void Behavior();
};
#endif