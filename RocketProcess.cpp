#include "RocketProcess.hpp"

void RocketProcess::Behavior() {
        int nick = nick0++; // DEBUG

        
        tons_of_material -= 100;

        rockets++;
        std::cout << "ROCKET " << nick << " STARTED" << std::endl;
        // std::cout << "MATERIAL LEFT " << tons_of_material << std::endl;
        Wait(Exponential(1440)); // 2 months for preparing rocket

        // std::cout << "PERMISSION " << nick << std::endl;
        (new FAAProcess)->Activate();
        Into(PermissionQueue);
        Passivate();

        Wait(48); // 2 days to move rocket to launch pad
        // std::cout << "GOING TO PAD " << nick << std::endl;
        Into(LaunchingRocketsQueue);
        launchRockets++;
        (new LaunchPadProcess)->Activate(); // activate the launch pad repair process
        Passivate();

        //std::cout << "LAUNCHING " << nick << std::endl;
        Wait(0.167);                      // 1h time to reach orbit
        (new BoosterProcess)->Activate(); // return back used booster

        for (int i = 0; i < 6; i++) {
            if (!BoosterQueue.Empty()) {
                (BoosterQueue.GetFirst())->Activate();
                Into(TankerQueue);
                tankers++;
                (new TankerProcess)->Activate();
                 std::cout << "LAUNCHING TANKER MAIN " << " i: " << i << " NICK: " << nick << std::endl;
                // std::cout << "TIME " << Time << std::endl;
                //  if (!OrbitingRockets.Empty()) {
                //      OrbitingRockets.GetFirst()->Activate();
                //  }
                Passivate();
            } else {
                 std::cout << "WAITING FOR TANKER " << nick << " " << i << std::endl;
                //  std::cout << "BOOSTERS " << BoosterQueue.Length() << std::endl;
                Into(OrbitingRockets);
                Passivate();
                // std::cout << "FILL AFTER WAIT " << nick << " " << i << std::endl;
            }
            fills++;
            std::cout << "FILLING " << nick << " " << i << std::endl;
            // std::cout << "FILLS " << fills << std::endl;
        }
        //std::cout << "READY TO LEAVE ORBIT " << nick << std::endl;
        if (MarsQueue.Empty()) {
            Into(ReadyToDepart);
            Passivate();
        }
        std::cout << "GOING OFF " << nick << std::endl;
       //           << std::endl;
        // std::cout << "FILLS " << fills << std::endl;
        Wait(Uniform(1920, 3600)); // time to get to mars
        reachedMars++;
        tons_reached_mars += 100;
        //std::cout << "REACHED MARS " << nick << std::endl;
        if (tons_reached_mars == payload_size) {
            std::cout << "Simulation stopped" << std::endl;
            Stop();
        }
        Wait(Exponential(120));    // time to refuel on mars
        Wait(Uniform(1920, 3600)); // time to return back to earth
        (new StarshipProcess)->Activate();
        returnedFromMars++;
        std::cout << "RETURNED FROM MARS " << nick << std::endl;
        std::cout << "Material left " << tons_of_material << std::endl;
    }