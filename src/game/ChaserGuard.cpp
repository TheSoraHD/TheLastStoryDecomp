#include "game/ChaserGuard.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: field (class unknown); m10D8 holds the route tables
struct Field_unk {
    unsigned char m00[0x10D8];
    void* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" ChaserRoute_unk* fn_803C17FC(void* routes, int id); // inferred: route point by id
// inferred: owner of the list the guard registers in (class unknown)
struct ChaserOwner_unk {
    unsigned char m00[0x258];
    void* m258;
};
extern "C" ChaserOwner_unk* lbl_8087FA20;
extern "C" void fn_804AAFCC(void* list, ChaserGuard* guard); // inferred: removes the guard
// inferred: chase manager (class unknown)
struct ChaseState_unk {
    unsigned char m00[0x48];
    int m48; // inferred: state (0: idle, 3: the guards stop)
};
extern "C" ChaseState_unk* lbl_8087F098;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_8017D13C(ChaserGuard* guard); // inferred: ChaserGuard (outside the split)

// NONMATCHING: 0x414 of 0x464 bytes - the inlined vector::push_back of the target also evaluates a
// dead growth computation (max((cap + 1) * 3 / 5, 1) and max((cap + 1) / 2, 1), 0x8017CD44) after
// the length check; also symbol names: the EnemyTask constructor is fn_8035B694, the floats and
// strings are lbl_80881B78/lbl_80881B7C/lbl_807380F8 (outside the split ranges of this file).
ChaserGuard::ChaserGuard(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0.0f, 0.0f, 0.0f), m14D0(0), m14D4(0), m14D8(0),
      m14F8(0), m14FC(0), m1500(1.0f), m1504(0), m1508(0), m1510(0.0f, 0.0f, 0.0f) {
    atn::Csv csv(info->m2C, strlen(info->m2C));
    for (const char* tok = csv.fn_8005B9CC(); tok != 0 && strcmp("", tok) != 0; tok = csv.fn_8005B9CC()) {
        m14EC.push_back(fn_803C17FC(lbl_8087F430->m10D8, fn_800DC12C(tok)));
    }
    m14DC = info->m04;
    m14E8 = info->m14;
    mFlag24 = 0;
    m1454 = 0;
}

// NONMATCHING: 99.9% - only the symbol name differs: the EnemyTask destructor is fn_8035B78C.
ChaserGuard::~ChaserGuard() {
    if (lbl_8087FA20 != 0) {
        fn_804AAFCC(lbl_8087FA20->m258, this);
    }
}

void ChaserGuard::vf10() {
    int state = lbl_8087F098->m48;
    if (state != 0) {
        PlayerTask* player = lbl_8087F8A0->mHead;
        if (player != 0 && (player->mStatus.m0C & 0x20) != 0x20) {
            if (m55C == 6) {
                fn_80139560();
                fn_80145334();
            } else if (state != 3) {
                fn_8017D13C(this);
                fn_80145334();
            }
        }
    }
}

void ChaserGuard::vf14() {
    fn_80149A30();
}
