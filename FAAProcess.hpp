#ifndef FAA
#define FAA

#include <iostream>
#include <simlib.h>
#include <unistd.h>

extern Queue PermissionQueue;

class FAAProcess: public Process {
    void Behavior ();
};

#endif