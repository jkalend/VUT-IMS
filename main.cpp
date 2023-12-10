/*
file: main.cpp
authors: xkubin27, xkalen07
description: main function, definition of generators and output functions
*/

#include <iostream>
#include <limits>
#include <simlib.h>
#include <unistd.h>
#include "StarshipProcess.hpp"
#include "BoosterProcess.hpp"
#include "MarsProcess.hpp"

Queue StarshipQueue("Starship Queue");            // starship ready
Queue BoosterQueue("Booster Queue");              // booster ready
Queue PermissionQueue("Permission Queue");        // waiting for permission
Queue MarsQueue("Mars Queue");                    // mars ready
Queue OrbitingRockets("Orbiting Rockets Queue");  // launched rockets waiting for tanking
Queue TankerQueue("Tanker Queue");                // Boosters with tankers
Queue LaunchingRocketsQueue("Launching rockets"); // Queue of rockets waiting on orbit
Queue ReadyToDepart("Ready to depart");           // Queue of rockets ready to depart to Mars

Store launchPad("launchPad", 1);
Store tanker("Tanker", 1);

int reachedMars = 0, returnedFromMars = 0;
int num_boosters = 10, num_starships = 80, num_starships_initial = 80;
int payload_size = 10000;
int tons_of_material = payload_size;
int tons_reached_mars = 0;

//////debug
int boosters = 0;
int nick0 = 0;
int rockets = 0;
int tankers = 0;
int launchRockets = 0;
int booster_returned_from_tanker = 0;
int fills = 0;
int enters = 0;

class StarshipGenerator : public Event { // GENERATOR Starship
    void Behavior() {
        if (num_starships > 0 || !tons_of_material) {
            (new StarshipProcess)->Activate();
            num_starships--;

            Activate(Time + Exponential(168)); // 1 weeks exponentionally
        }
    }
};

class BoosterGenerator : public Event { // GENERATOR Booster
    void Behavior() {
        if (num_boosters > 0 || !tons_of_material) {
            (new BoosterProcess)->Activate();
            num_boosters--;

            Activate(Time + Exponential(504)); // 3 weeks exponentially
        }
    }
};

class MarsGenerator : public Event {
    void Behavior() {
        (new MarsProcess)->Activate();
        Activate(Time + 17520); // 2 years for mars to be aligned
    }
};

int main(int argc, char *argv[]) {
    int opt, initial_boosters = 0, initial_starships = 0;
    while ((opt = getopt(argc, argv, ":hs:b:l:t:p:S:B:")) != -1) {
        switch (opt) {
        case 'h': {
            std::cout << "./ims [-s <starships>] [-b <boosters>] [-l <launchpads>] [-t <tankers>] [-p <payload>] [-B <initial boosters] [-S <initial starships>]" << std::endl;
            exit(0);
        }
        case 'B': {
            initial_boosters = std::stoi(optarg);
            break;
        }
        case 'S': {
            initial_starships = std::stoi(optarg);
            break;
        }
        case 'p': {
            payload_size = std::stoi(optarg);
            payload_size = (payload_size % 100 == 0) ? payload_size : payload_size + (100 - payload_size % 100);
            tons_of_material = payload_size;
            break;
        }
        case 's': {
            num_starships = std::stoi(optarg);
            break;
        }
        case 'b': {
            num_boosters = std::stoi(optarg);
            break;
        }
        case 'l': {
            launchPad.SetCapacity(std::stoi(optarg));
            break;
        }
        case 't': {
            tanker.SetCapacity(std::stoi(optarg));
            break;
        }
        case '?': {
            break;
        }
        default:
            break;
        }
    }
    for (; optind < argc; optind++) {
        printf("extra arguments : %s\n", argv[optind]);
    }

    std::cout << "====================================================" << std::endl;
    std::cout << "Payload size: " << payload_size << std::endl;
    std::cout << "Number of starships: " << num_starships << std::endl;
    std::cout << "Number of boosters: " << num_boosters << std::endl;
    std::cout << "Number of launch pads: " << launchPad.Capacity() << std::endl;
    std::cout << "Number of tankers: " << tanker.Capacity() << std::endl;
    std::cout << "===================== STARTING =====================" << std::endl;
    
    SetOutput("model2.out");
    Init(0, 1000000); // experiment initialization for time 0..1000
    for (int j = 0; j < initial_boosters; j++)
        (new BoosterProcess)->Activate();
    
    for (int j = 0; j < initial_starships; j++)
        (new StarshipProcess)->Activate();
    BoosterGenerator *BGen = new BoosterGenerator;
    BGen->Activate();
    StarshipGenerator *SGen = new StarshipGenerator;
    SGen->Activate();
    (new MarsGenerator)->Activate();

    Run();
    std::cout << "===================== STATS =====================" << std::endl;
    std::cout << "Tons to be delivered " << payload_size << std::endl;
    std::cout << "Tons delivered " << tons_reached_mars << std::endl;
    std::cout << "Rockets that reached Mars " << reachedMars << std::endl;
    std::cout << "Rockets that returned from Mars " << returnedFromMars << std::endl;
    std::cout << "Rockets in orbit " << OrbitingRockets.Length() << std::endl;
    std::cout << "Total starship rockets " << rockets << std::endl;
    std::cout << "Total starships launched " << launchRockets << std::endl;
    std::cout << "Total tanker launches " << tankers << std::endl;
    std::cout << "Total refuels " << fills << std::endl;
    std::cout << "Total launches " << enters << std::endl;
    SIMLIB_statistics.Output();
    launchPad.Output();
    LaunchingRocketsQueue.Output();
    BoosterQueue.Output();
    OrbitingRockets.Output();
}
