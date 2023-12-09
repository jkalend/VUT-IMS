#include "FAAProcess.hpp"

void FAAProcess::Behavior() {
        Wait(Exponential(21)); // 3 weeks for FAA permission grant
        Entity *p = PermissionQueue.GetFirst();
        p->Activate();
        // std::cout << "GIVEN PERMISSION " << perms << std::endl;
    }