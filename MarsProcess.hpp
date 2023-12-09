#ifndef MARS
#define MARS

#include <iostream>
#include <simlib.h>
#include <unistd.h>

extern int mars_count;
extern Queue MarsQueue, ReadyToDepart;

class MarsProcess: public Process {
    void Behavior ();
};

#endif