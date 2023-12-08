#include <iostream>
#include <limits>
#include <simlib.h>
#include <unistd.h>

Queue StarshipQueue("Starship Queue");           // starship ready
Queue BoosterQueue("Booster Queue");             // booster ready
Queue PermissionQueue("Permission Queue");       // waiting for permission
Queue MarsQueue("Mars Queue");                   // mars ready
Queue FAAQueue("FAA Queue");                     // queue of assigned FAA permission
Queue OrbitingRockets("Orbiting Rockets Queue"); // launched rockets waiting for tanking
Queue TankerQueue("Tanker Queue");               // Boosters with tankers
Queue OnOrbitQueue("On Orbit Queue");            // Queue of rockets waiting on orbit

Store launchPad("launchPad", 1);
Store tanker("Tanker", 1);

int reachedMars = 0, returnedFromMars = 0;
int num_boosters = 10, num_starships = 80, num_starships_initial = 80;
int payload_size = 10000;
int tons_of_material = payload_size;
int tons_reached_mars = 0;
class BoosterProcess : public Process {
    void Behavior();
};

class StarshipProcess : public Process {
    void Behavior();
};
int mars_count = 0;
class MarsProcess : public Process {
    void Behavior() {
        mars_count++;
        Into(MarsQueue);
        while (!FAAQueue.Empty()) { // wait for all permissions to clear
            Entity *p = FAAQueue.GetFirst();
            p->Activate();
        }
        Wait(1440); // mars is available for 2 months
        MarsQueue.Clear();
    }
};

class FAAProcess : public Process {
    void Behavior() {
        Wait(Exponential(21)); // 3 weeks for FAA permission grant
        if (MarsQueue.Empty()) {
            Into(FAAQueue);
            Passivate();
        }
        Entity *p = PermissionQueue.GetFirst();
        p->Activate();
    }
};

class LaunchPadProcess : public Process {
public:
    int priority;
    LaunchPadProcess(int prio = 0) : Process() {
        priority = prio;
    }
    void Behavior() {
        Enter(launchPad);
        Wait(Exponential(240)); // the launch pad repair process takes 10 days exponentially
        Leave(launchPad);
    }
};

class RocketProcess : public Process { // starting from rocket available
    void Behavior() {
        if (!tons_of_material)
            Cancel();
        tons_of_material -= 100;
        Wait(Exponential(1440)); // 2 months for preparing rocket
        (new FAAProcess)->Activate();
        Into(PermissionQueue);
        Passivate();
        Wait(48); // 2 days to move rocket to launch pad

        (new LaunchPadProcess)->Activate(); // activate the launch pad repair process

        Wait(0.167);                      // 1h time to reach orbit
        (new BoosterProcess)->Activate(); // return back used booster

        for (int i = 0; i < 6; i++) {
            Into(OrbitingRockets);
            Passivate(); // waiting for refuelling
            TankerQueue.GetFirst()->Activate();
            Wait(4); // refuelling takes 4h
        }

        Wait(Uniform(1920, 3600)); // time to get to mars
        reachedMars++;
        tons_reached_mars += 100;
        if (tons_reached_mars == payload_size) {
            Stop();
        }
        Wait(Exponential(120));    // time to refuel on mars
        Wait(Uniform(1920, 3600)); // time to return back to earth
        (new StarshipProcess)->Activate();
        returnedFromMars++;
    }
};

void StarshipProcess::Behavior() { // starship ready

    if (StarshipQueue.Length() == num_starships_initial && tons_reached_mars == 1000) {
        Stop();
    }
    if (!BoosterQueue.Empty()) {
        Entity *p = BoosterQueue.GetFirst();
        p->Activate();
        // std::cout << "Creating Rocket Process" << std::endl;
        (new RocketProcess)->Activate();
    } else {
        Into(StarshipQueue);
        Passivate();
        // std::cout << "Entered StarshipQueue" << std::endl;
    }
}

// booster ready
void BoosterProcess::Behavior() {
    if (!OrbitingRockets.Empty() && !tanker.Full()) {
        Enter(tanker);
        Wait(48);                              // assemble tanker with booster
        (new LaunchPadProcess(1))->Activate(); // activate the launch pad repair process
        Wait(0.167);                           // time to reach orbit
        (new BoosterProcess)->Activate();      // return back used booster
        OrbitingRockets.GetFirst()->Activate();
        Into(TankerQueue);
        Passivate();
        Leave(tanker);

    } else if (!StarshipQueue.Empty()) {
        Entity *p = StarshipQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        Into(BoosterQueue);
        Passivate();
    }
}

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
    int opt, payload_size = 10000;
    while ((opt = getopt(argc, argv, ":hs:b:l:t:T:")) != -1) {
        switch (opt) {
        case 'h': {
            std::cout << "./ims [-s <starships>] [-b <boosters>] [-l <launchpads>] [-t <tankers>] [-p <payload>]" << std::endl;
            exit(0);
        }
        case 'p': {
            payload_size = std::stoi(optarg);
            payload_size = (payload_size % 100 == 0) ? payload_size : payload_size + (100 - payload_size % 100);
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
    SetOutput("model2.out");
    Init(0, std::numeric_limits<double>::max()); // experiment initialization for time 0..1000
    BoosterGenerator *BGen = new BoosterGenerator;
    BGen->Activate();
    StarshipGenerator *SGen = new StarshipGenerator;
    SGen->Activate();
    (new MarsGenerator)->Activate();
    Run();
    SIMLIB_statistics.Output();
    launchPad.Output();
    StarshipQueue.Output();
    BoosterQueue.Output();
    FAAQueue.Output();
    PermissionQueue.Output();
}