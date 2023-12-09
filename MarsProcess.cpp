#include "MarsProcess.hpp"

void MarsProcess::Behavior() {
    // std::cout << "========================" << std::endl;
    // std::cout << "MARS READY" << std::endl;
    // std::cout << "========================" << std::endl;
    mars_count++;
    Into(MarsQueue);
    while (!ReadyToDepart.Empty()) { // wait for all rockets to launch
        ReadyToDepart.GetFirst()->Activate();
    }
    Wait(1440); // mars is available for 2 months
    MarsQueue.Clear();
    // std::cout << "========================" << std::endl;
     std::cout << "MARS LEFT " << BoosterQueue.Length() << std::endl;
     std::cout << "MARS LEFT2 " << OrbitingRockets.Length() << std::endl;
    // std::cout << "========================" << std::endl;
}