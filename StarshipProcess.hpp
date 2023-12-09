#ifndef STARSHIP
#define STARSHIP

#include <iostream>
#include <simlib.h>
#include <unistd.h>
#include "BoosterProcess.hpp"

extern Queue BoosterQueue;
extern Queue StarshipQueue;

class StarshipProcess : public Process {
    void Behavior();
};
#endif