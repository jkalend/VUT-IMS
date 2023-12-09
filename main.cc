#include <iostream>
#include <limits>
#include <simlib.h>
#include <unistd.h>

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
int fills = 0;
int enters = 0;

TStat *st;


class BoosterProcess : public Process {
    void Behavior();
};

class StarshipProcess : public Process {
    void Behavior();
};


int mars_count = 0;
class MarsProcess : public Process {
    void Behavior() {
        std::cout << "========================" << std::endl;
        std::cout << "MARS READY" << std::endl;
        std::cout << "========================" << std::endl;
        mars_count++;
        Into(MarsQueue);
        while (!ReadyToDepart.Empty()) { // wait for all rockets to launch
            ReadyToDepart.GetFirst()->Activate();
        }
        Wait(1440); // mars is available for 2 months
        MarsQueue.Clear();
        std::cout << "========================" << std::endl;
        std::cout << "MARS LEFT" << std::endl;
        std::cout << "========================" << std::endl;
    }
};

class FAAProcess : public Process {
    void Behavior() {
        Wait(Exponential(21)); // 3 weeks for FAA permission grant
        Entity *p = PermissionQueue.GetFirst();
        p->Activate();
        //std::cout << "GIVEN PERMISSION " << perms << std::endl;
    }
};

class LaunchPadProcess : public Process {
public:
    int priority;
    LaunchPadProcess(int prio = 0) : Process() {
        priority = prio;
    }
    void Behavior() {
        enters++;
        Enter(launchPad);
        std::cout << "ENTERED LAUNCHPAD" << std::endl;
        Entity *p = LaunchingRocketsQueue.GetFirst();
        p->Activate();
        Wait(Exponential(240)); // the launch pad repair process takes 10 days exponentially
        Leave(launchPad);
        // std::cout << "CLEAR LAUNCHPAD" << std::endl;
    }
};

class TankerProcess : public Process {
    void Behavior() {
        // tankers++;
        int nick = tankers; // DEBUG
        Enter(tanker);
        Wait(48); // assemble tanker with booster
        Into(LaunchingRocketsQueue);
        (new LaunchPadProcess(1))->Activate(); // activate the launch pad repair process
        Passivate();
        // std::cout << "LAUNCHING TANKER " << LT << std::endl;
        // std::cout << "TIME " << Time << std::endl;
        Wait(0.167); // time to reach orbit
        (new BoosterProcess)->Activate();
        Wait(4); // time to refuel
        Leave(tanker);
        // std::cout << "LEAVING TANKER " << nick << std::endl;
        (TankerQueue.GetFirst())->Activate();
    }
};

class RocketProcess : public Process { // starting from rocket available
    void Behavior() {
        int nick = nick0++; // DEBUG

        if (!tons_of_material)
            Cancel();
        tons_of_material -= 100;

        rockets++;
        std::cout << "ROCKET " << nick << " STARTED" << std::endl;
        //std::cout << "MATERIAL LEFT " << tons_of_material << std::endl;
        Wait(Exponential(1440)); // 2 months for preparing rocket

        // std::cout << "PERMISSION " << nick << std::endl;
        (new FAAProcess)->Activate();
        Into(PermissionQueue);
        Passivate();

        Wait(48); // 2 days to move rocket to launch pad
        //std::cout << "GOING TO PAD " << nick << std::endl;
        Into(LaunchingRocketsQueue);
        launchRockets++;
        (new LaunchPadProcess)->Activate(); // activate the launch pad repair process
        Passivate();

        std::cout << "LAUNCHING " << nick << std::endl;
        Wait(0.167);                      // 1h time to reach orbit
        (new BoosterProcess)->Activate(); // return back used booster

        for (int i = 0; i < 6; i++) {
            if (!BoosterQueue.Empty()) {
                (BoosterQueue.GetFirst())->Activate();
                Into(TankerQueue);
                tankers++;
                (new TankerProcess)->Activate();
                //std::cout << "LAUNCHING TANKER MAIN " << LT << " i: " << i << " NICK: " << nick << std::endl;
                //std::cout << "TIME " << Time << std::endl;
                // if (!OrbitingRockets.Empty()) {
                //     OrbitingRockets.GetFirst()->Activate();
                // }
                Passivate();
            } else {
                //std::cout << "WAITING FOR TANKER " << nick << " " << i << std::endl;
                // std::cout << "BOOSTERS " << BoosterQueue.Length() << std::endl;
                Into(OrbitingRockets);
                Passivate();
                // std::cout << "FILL AFTER WAIT " << nick << " " << i << std::endl;
            }
            fills++;
            std::cout << "FILLING " << nick << " " << i << std::endl;
            //std::cout << "FILLS " << fills << std::endl;
        }
        std::cout << "READY TO LEAVE ORBIT " << nick << std::endl;
        if (MarsQueue.Empty()) {
            Into(ReadyToDepart);
            Passivate();
        }
        std::cout << "GOING OFF " << nick << std::endl << std::endl;
        // std::cout << "FILLS " << fills << std::endl;
        Wait(Uniform(1920, 3600)); // time to get to mars
        reachedMars++;
        tons_reached_mars += 100;
        std::cout << "REACHED MARS " << nick << std::endl;
        if (tons_reached_mars == payload_size) {
            std::cout << "Simulation stopped" << std::endl;
            Stop();
        }
        Wait(Exponential(120));    // time to refuel on mars
        Wait(Uniform(1920, 3600)); // time to return back to earth
        (new StarshipProcess)->Activate();
        returnedFromMars++;
        std::cout << "RETURNED FROM MARS " << nick << std::endl;
    }
};

void StarshipProcess::Behavior() { // starship ready
    if (!BoosterQueue.Empty()) {
        Entity *p = BoosterQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        Into(StarshipQueue);
        Passivate();
    }
}

// booster ready
void BoosterProcess::Behavior() {
    if (!OrbitingRockets.Empty() && !tanker.Full()) {
        if (!OrbitingRockets.Empty()) {
            OrbitingRockets.GetFirst()->Activate();
        }
        //std::cout << "LAUNCHING TANKER SIDE " << LT << std::endl;
        //std::cout << "TIME " << Time << std::endl;
        Into(TankerQueue);
        tankers++;
        // std::cout << "LAUNCHING TANKER SIDE " << tankers << std::endl;
        (new TankerProcess)->Activate();
        Passivate();
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
    int opt;
    while ((opt = getopt(argc, argv, ":hs:b:l:t:p:")) != -1) {
        switch (opt) {
        case 'h': {
            std::cout << "./ims [-s <starships>] [-b <boosters>] [-l <launchpads>] [-t <tankers>] [-p <payload>]" << std::endl;
            exit(0);
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

    std::cout << "========================================" << std::endl;
    std::cout << "Payload size: " << payload_size << std::endl;
    std::cout << "Number of starships: " << num_starships << std::endl;
    std::cout << "Number of boosters: " << num_boosters << std::endl;
    std::cout << "Number of launch pads: " << launchPad.Capacity() << std::endl;
    std::cout << "Number of tankers: " << tanker.Capacity() << std::endl;
    std::cout << "========================================" << std::endl;

    SetOutput("model2.out");
    Init(0, 50000); // experiment initialization for time 0..1000
    BoosterGenerator *BGen = new BoosterGenerator;
    BGen->Activate();
    StarshipGenerator *SGen = new StarshipGenerator;
    SGen->Activate();
    (new MarsGenerator)->Activate();

    st = new TStat("TStat");

    Run();
    std::cout << "========================================" << std::endl;
    std::cout << payload_size << std::endl;
    std::cout << tons_reached_mars << std::endl;
    std::cout << "rockets that reached Mars " << reachedMars << std::endl;
    std::cout << "rockets that returned from Mars " << returnedFromMars << std::endl;
    std::cout << "rockets in orbit " << OrbitingRockets.Length() << std::endl;
    std::cout << "total starship rockets " << rockets << std::endl;
    std::cout << "total starships launched " << launchRockets << std::endl;
    // std::cout << "total boosters " << boosters << std::endl;
    std::cout << "total tanker launches " << tankers << std::endl;
    std::cout << "total fills " << fills << std::endl;
    std::cout << "total enters " << enters << std::endl;
    SIMLIB_statistics.Output();
    launchPad.Output();
    LaunchingRocketsQueue.Output();
    // StarshipQueue.Output();
    BoosterQueue.Output();
    OrbitingRockets.Output();
    // TankerQueue.Output();
    // PermissionQueue.Output();
}
