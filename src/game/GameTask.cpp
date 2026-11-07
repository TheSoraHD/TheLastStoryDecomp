// game/GameTask.cpp (0x8036555C-0x80376B68): the field task (GameTask). Only accessors are
// reconstructed so far.
#include "game/GameTask.h"
#include "game/GameScene.h"
#include "game/LibraryTask.h"
#include "game/MapTask.h"
#include "game/SelectMember.h"
#include "game/ShopBuySell.h"
#include "game/ShopColor.h"
#include "game/ShopIdentify.h"
#include "atn/FadeCommon.h"

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" int fn_80570A4C(GameTaskData_unk* obj);
extern "C" GameTask* lbl_8087F430;
extern "C" atn::Task* lbl_8087F9E0; // inferred: upgrade shop (ShopUpGrade)

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

int GameTask::fn_8036DAA8(CharaTask* chara) {
    if (m56D8 != 0 || m56D4 != 0 || mD18 != 0) {
        return 0;
    }
    if (m54E4 != 0) {
        return 0;
    }
    m56D8 = chara;
    return 1;
}

void GameTask::fn_8036E430(int a, int b) {
    m5624->m78 = 0x12;
    m5624->Capture();
    m5684 = 1;
    m5688 = a;
    m568C = b;
}

void GameTask::fn_8036E89C() {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->fn_80370320(0x394, 0, 0);
    }
    fn_8036EE0C();
    if (ShopBuySell::sInstance != 0) {
        fn_800D2338(ShopBuySell::sInstance);
    }
    if (lbl_8087F9E0 != 0) {
        fn_800D2338(lbl_8087F9E0);
    }
    if (ShopColor::sInstance != 0) {
        fn_800D2338(ShopColor::sInstance);
    }
    if (ShopIdentify::sInstance != 0) {
        fn_800D2338(ShopIdentify::sInstance);
    }
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

void GameTask::fn_80370B78(unsigned int color, int frames, int arg, float speed) {
    m54F0 = 1;
    m5620->mFrame = 1;
    m5620->mColor0 = color & 0xFFFFFF;
    m5620->mColor1 = color;
    m5620->mDelay = arg;
    m5620->mWait = 999999999;
    m5620->mDuration = frames;
    m5620->mZ = speed;
    m5620->mActive = 1;
}

void GameTask::fn_80370BD0(int frames, int arg, float speed) {
    unsigned int color = m5620->mColor1;
    m54F0 = 2;
    m5620->mFrame = 1;
    m5620->mColor0 = color;
    m5620->mColor1 = color & 0xFFFFFF;
    m5620->mDelay = arg;
    m5620->mWait = 0;
    m5620->mDuration = frames;
    m5620->mZ = speed;
    m5620->mActive = 1;
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

void GameTask::fn_80373164(CharaTask* owner, const atn::Vector3* pos, int id) {
    if (m56D4 != 0 && m56D4 == owner) {
        if (id != 0) {
            m1070 = (int)m10C8 - 30;
        }
    } else if (m5594 != 0 && mD1C == 0 && mF28 == owner) {
        m1014 = *pos;
        m5614 = m5598;
    }
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

void GameTask::fn_803766E4() {
    if (m56F0 != 0) {
        fn_800D2338(m56F0);
        m56F0 = 0;
    }
    m571C = 0;
}

// inferred: 0xC-byte entry of sGameTaskStatic_unk (constructor inlined in __sinit)
struct GameTaskStaticEntry_unk {
    int m00;
    int m04;
    int m08;
};

// inferred: static object of this file (.bss 0x807C8458, 0x98 bytes). The constructor is __sinit_\GameTask_cpp
// (NONMATCHING: 93% - the original computes the end of m44 before its start, r6/r7 swapped)
struct GameTaskStatic_unk {
    GameTaskStatic_unk()
        : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1), m18(0), m1C(0), m20(0), m24(0), m28(0), m2C(0),
          m30(0), m34(0), m38(-1), m3C(0), m40(0) {
        for (GameTaskStaticEntry_unk* p = m44; p < m44 + 7; p++) {
            p->m00 = -1;
            p->m04 = 0;
            p->m08 = 0;
        }
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38;
    int m3C;
    int m40;
    GameTaskStaticEntry_unk m44[7];
};

GameTaskStatic_unk sGameTaskStatic_unk;
