#include "FAAProcess.hpp"

void FAAProcess::Behavior() {
        Wait(Exponential(504)); // 3 weeks for FAA permission grant
        Entity *p = PermissionQueue.GetFirst();
        p->Activate();
    }