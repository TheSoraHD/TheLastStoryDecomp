// game/GameTask.cpp (0x8036555C-0x80376B68): the field task (GameTask). Only accessors are
// reconstructed so far.
#include "game/GameTask.h"
#include "game/GameScene.h"
#include "game/LibraryTask.h"
#include "game/MapTask.h"
#include "game/SelectMember.h"
#include "atn/FadeCommon.h"

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" int fn_80570A4C(GameTaskData_unk* obj);

inline MapTask* GameTask::GetCurrentMap() {
    MapTask* map = 0;
    if (m10D8 != 0) {
        map = m10D8->m64;
    }
    return map;
}

// inferred: save menu (game/CampSaveLoad.cpp)
class MenuSaveTask {
public:
    static MenuSaveTask* fn_8011770C(atn::Task* parent);
    static MenuSaveTask* sInstance;
};

void GameTask::fn_8036E430(int a, int b) {
    m5624->m78 = 0x12;
    m5624->Capture();
    m5684 = 1;
    m5688 = a;
    m568C = b;
}

void GameTask::fn_8036E92C(int arg) {
    fn_8036EA6C(4);
    MenuSaveTask::fn_8011770C(this);
}

void GameTask::fn_8036E964() {
    fn_8036EE0C();
    if (MenuSaveTask::sInstance != 0) {
        fn_800D2338((atn::Task*)MenuSaveTask::sInstance);
    }
}

void GameTask::fn_8036E994(int type, int a, int b) {
    fn_8036EA6C(5);
    m5750 = SelectMember::fn_8057162C(this, type, 1);
    m5754 = a;
    m5758 = b;
}

void GameTask::fn_8036EA04() {
    fn_8036EA6C(3);
    LibraryTask::fn_80468F54(this);
}

void GameTask::fn_8036EA3C() {
    fn_8036EE0C();
    if (LibraryTask::sInstance != 0) {
        fn_800D2338(LibraryTask::sInstance);
    }
}

// NONMATCHING: 99% - r4/r5 swapped in the compare
int GameTask::fn_80370C2C() {
    atn::FadeCommon* fade = m5620;
    if (fade->mActive != 0) {
        return fade->mFrame < fade->mDuration + fade->mDelay;
    }
    return 1;
}

// NONMATCHING: 96% - the original branches bne/b around the type 4 test
void GameTask::fn_80370C6C() {
    int type = m10D8->m64->mType;
    if (type != 3) {
        if (type == 4) {
            return;
        }
        m5624->m78 = 0x12;
        m5624->Capture();
        m566C = 1;
    }
}

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

void GameTask::fn_80373F78() {
    if (m5590 != 0 && fn_80570A4C(m5590) != 0) {
        fn_803731E8();
    }
}

int GameTask::fn_80375184() {
    return m5638 == 0;
}

// NONMATCHING: 93% - the null scene path joins the null map return directly
int GameTask::fn_80376150() {
    MapTask* map = GetCurrentMap();
    if (map == 0) {
        return 0;
    }
    int ret = 0;
    if (m10F4 == 0 && map->mType != 1) {
        ret = 1;
    }
    return ret;
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
