/*
file: TankerProcess.hpp
authors: xkubin27, xkalen07
description: definition of TankerProcess class
*/

#ifndef TANKER
#define TANKER

#include <iostream>
#include <simlib.h>
#include <unistd.h>
#include "BoosterProcess.hpp"

extern int tankers, booster_returned_from_tanker;

class TankerProcess : public Process {
    void Behavior();
};

#endif