/*
file: RocketProcess.cpp
authors: xkubin27, xkalen07
description: definition of RocketProcess behavior
*/

#include "RocketProcess.hpp"

void RocketProcess::Behavior() {
        
        tons_of_material -= 100;

        rockets++;
        Wait(Exponential(1440)); // 2 months for preparing rocket

        (new FAAProcess)->Activate();
        Into(PermissionQueue);
        Passivate();

        Wait(48); // 2 days to move rocket to launch pad
        Into(LaunchingRocketsQueue);
        launchRockets++;
        (new LaunchPadProcess)->Activate(); // activate the launch pad repair process
        Passivate();

        Wait(0.167);                      // 1h time to reach orbit
        (new BoosterProcess)->Activate(); // return back used booster

        for (int i = 0; i < 6; i++) {
            if (!BoosterQueue.Empty()) {
                (BoosterQueue.GetFirst())->Activate();
                Into(TankerQueue);
                tankers++;
                (new TankerProcess)->Activate();
                Passivate();
            } else {
                Into(OrbitingRockets);
                Passivate();
            }
            fills++;
        }
        if (MarsQueue.Empty()) {
            Into(ReadyToDepart);
            Passivate();
        }
        Wait(Uniform(1920, 3600)); // time to get to mars
        reachedMars++;
        tons_reached_mars += 100;
        if (tons_reached_mars == payload_size) {
            std::cout << "Simulation stopped" << std::endl;
            Stop();
        }
        Wait(Exponential(120));    // time to refuel on mars
        Wait(Uniform(1920, 3600)); // time to return back to earth
        (new StarshipProcess)->Activate();
        returnedFromMars++;
    }