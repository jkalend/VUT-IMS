#ifndef LAUNCH_PAD
#define LAUNCH_PAD

#include <iostream>
#include <simlib.h>
#include <unistd.h>

class LaunchPadProcess : public Process {
    void Behavior() override;
};

extern int enters;
extern Store launchPad;
extern Queue LaunchingRocketsQueue;



#endif