/*
file: FAAProcess.hpp
authors: xkubin27, xkalen07
description: definition of FAAProcess class
*/

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