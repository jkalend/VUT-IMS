#ifndef TANKER
#define TANKER

#include <iostream>
#include <simlib.h>
#include <unistd.h>
#include "BoosterProcess.hpp"

extern int tankers;

class TankerProcess : public Process {
    void Behavior();
};

#endif