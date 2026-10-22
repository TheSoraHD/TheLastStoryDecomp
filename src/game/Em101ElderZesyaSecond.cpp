#include "game/Em101ElderZesyaSecond.h"
#include "game/HitInfo_unk.h"

Em101ElderZesyaSecond::~Em101ElderZesyaSecond() {}

void Em101ElderZesyaSecond::vf14() {
    fn_80149A30();
}

int Em101ElderZesyaSecond::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (m1554 != 0 && hit != 0) {
        int ok = 0;
        if (m15D4 == 0 && hit->m74.m0C > 0.0f) {
            ok = 1;
        }
        if (m15D4 == 1 && hit->m74.m00 > 0.0f) {
            ok = 1;
        }
        if (ok == 0) {
            return 4;
        }
    }
    return 0;
}
