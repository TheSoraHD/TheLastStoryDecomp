// game/GameTask.cpp (0x8036555C-0x80376B68): the field task (GameTask). Only accessors are
// reconstructed so far.
#include "game/GameTask.h"
#include "game/GameScene.h"

void GameTask::fn_80371320(PlayerTask* player) {
    if (m10E0 != 0) {
        m10E0->mLeader = player;
    }
}

void GameTask::fn_80371664() {
    mCBAC = 0;
}

void GameTask::fn_8037177C() {
    m5768 = 0;
    m5774 = 0;
    m5770 = 0;
}

int GameTask::fn_80375184() {
    return m5638 == 0;
}

void GameTask::fn_80376194(int on) {
    m10F4 = on == 0;
}

int GameTask::fn_803761A4() {
    return 1;
}

int GameTask::fn_803761AC() {
    return m10FC == 0;
}

int GameTask::fn_803761CC() {
    return m1110 == 0;
}

void GameTask::fn_803761E4(int visible) {
    m1118 = visible;
}

int GameTask::fn_803761EC() {
    return m111C;
}

void GameTask::fn_803761F4(int value) {
    m111C = value;
}

int GameTask::fn_803761FC() {
    return m1164 != 0;
}

int GameTask::fn_80376210() {
    return m117C != 0;
}

int GameTask::fn_80376224() {
    return m1180 != 0;
}

int GameTask::fn_80376238() {
    return m1194 != 0;
}

void GameTask::fn_8037624C(int value) {
    m1194 = value;
}
