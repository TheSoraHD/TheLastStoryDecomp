#include "game/EnemyQuarkFirst.h"

EnemyQuarkFirst::~EnemyQuarkFirst() {}

void EnemyQuarkFirst::vfFC() {
    fn_8016E970(2);
    vf100(0);
}

int EnemyQuarkFirst::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (m14D8 == 1 && !IsDown_unk()) {
        if (m55C != 6) {
            return 2;
        }
        if (m560 == 10) {
            return 3;
        }
        return 1;
    }
    return 0;
}

int EnemyQuarkFirst::vfA8(int kind) {
    if (kind == 1 && m14D8 == 1 && m58C != 9 && m58C != 1 && !IsDown_unk()) {
        return 3;
    }
    return -1;
}
