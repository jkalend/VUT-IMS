#include <iostream>
#include <simlib.h>

Queue StarshipQueue;   // starship ready
Queue BoosterQueue;    // booster ready
Queue PermissionQueue; // waiting for permission
Queue MarsQueue;       // mars ready
Queue FAAQueue;        // queue of assigned FAA permission
Queue OrbitingRockets; // launched rockets waiting for tanking
Queue TankerQueue;     // Boosters with tankers

Facility launchPad("launchPad");
Facility tanker("Tanker");

bool OnOrbitFlag = false;

class BoosterProcess : public Process {
    void Behavior();
};

class StarshipProcess : public Process {
    void Behavior();
};

class MarsProcess : public Process {
    void Behavior() {
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
        // std::cout << "Permission granted" << std::endl;
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
        Seize(launchPad, priority);
        Wait(Exponential(240)); // the launch pad repair process takes 10 days exponentially
        Release(launchPad);
        // std::cout << "Leave facility" << std::endl;
    }
};

class RocketProcess : public Process { // starting from rocket available
    void Behavior() {
        Wait(Exponential(1440)); // 2 months for preparing rocket
        (new FAAProcess)->Activate();
        Into(PermissionQueue);
        Passivate();
        Wait(48); // 2 days to move rocket to launch pad

        // std::cout << "Enter facility" << std::endl;
        (new LaunchPadProcess)->Activate(); // activate the launch pad repair process
        // std::cout << "Flying" << std::endl;

        Wait(1);                          // 1h time to reach orbit
        (new BoosterProcess)->Activate(); // return back used booster
        OnOrbitFlag = true;
        Into(OrbitingRockets);
        std::cout << "Waiting for refuelling" << std::endl;
        Passivate(); // waiting for refuelling
        std::cout << "Refuelling" << std::endl;
        Wait(4); // refuelling takes 4h
        TankerQueue.GetFirst()->Activate();
        (new BoosterProcess)->Activate(); // return back used booster
        Wait(Uniform(1920, 3600));        // time to get to mars
        Wait(Exponential(120));           // time to refuel on mars
        Wait(Uniform(1920, 3600));        // time to return back to earth
        (new StarshipProcess)->Activate();
        std::cout << "Got here" << std::endl;
    }
};

void StarshipProcess::Behavior() { // starship ready
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
    if (OnOrbitFlag && !tanker.Busy()) {
        Seize(tanker);
        Wait(48);                              // assemble tanker with booster
        (new LaunchPadProcess(1))->Activate(); // activate the launch pad repair process
        Wait(1);                               // time to reach orbit
        OrbitingRockets.GetFirst()->Activate();
        OnOrbitFlag = false;
        Into(TankerQueue);
        Passivate();
        Release(tanker);

    } else if (!StarshipQueue.Empty()) {
        // std::cout << "Entered BoosterQueue" << std::endl;
        Entity *p = StarshipQueue.GetFirst();
        p->Activate();
        (new RocketProcess)->Activate();
    } else {
        // std::cout << "Entered BoosterQueue" << std::endl;
        Into(BoosterQueue);
        Passivate();
    }
}

class StarshipGenerator : public Event { // GENERATOR Starship
    void Behavior() {
        (new StarshipProcess)->Activate();
        Activate(Time + Exponential(7)); // 1 weeks exponentionally
    }
};

class BoosterGenerator : public Event { // GENERATOR Booster
    void Behavior() {
        (new BoosterProcess)->Activate();
        Activate(Time + Exponential(56)); // 8 weeks exponentially
    }
};

class MarsGenerator : public Event {
    void Behavior() {
        (new MarsProcess)->Activate();
        Activate(Time + 17520); // 2 years for mars to be aligned
    }
};

int main() {
    SetOutput("model2.out");
    Init(0, 10000); // experiment initialization for time 0..1000
    (new BoosterGenerator)->Activate();
    (new StarshipGenerator)->Activate();
    (new MarsGenerator)->Activate();
    Run();
}