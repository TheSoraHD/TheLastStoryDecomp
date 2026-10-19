#include "game/ChaserNpc.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" ChaserNpc* lbl_8087F090; // inferred: the current ChaserNpc
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x564];
    float m564; // inferred: distance at which the NPC notices the player
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" void fn_80180AE0(ChaserNpc* npc); // inferred: ChaserNpc (outside the split)
extern "C" void fn_8018161C(ChaserNpc* npc, int state); // inferred: ChaserNpc, sets the state

// NONMATCHING: 99.7% - symbol names: the EnemyTask constructor is fn_8035B694 in symbols.txt, the
// 1.0f and the strings are lbl_80881BF8/lbl_80738154/lbl_80775A88 (outside the split ranges of
// this file); also the inlined vector::push_back loads the capacity before the size.
ChaserNpc::ChaserNpc(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14C0(-1), m14C4(1.0f), m14C8(0), m14CC(0), m14D0(0) {
    atn::Csv csv(info->m2C, strlen(info->m2C));
    for (const char* tok = csv.fn_8005B9CC(); tok != 0 && strcmp("", tok) != 0; tok = csv.fn_8005B9CC()) {
        m14B4.push_back(fn_800DC12C(tok));
    }
    mFlag24 = 0;
    lbl_8087F090 = this;
}

// NONMATCHING: 99.9% - only the symbol name differs: the EnemyTask destructor is fn_8035B78C.
ChaserNpc::~ChaserNpc() {
    lbl_8087F090 = 0;
}

// NONMATCHING: 99.9% - only the symbol name differs: the 1.0f is lbl_80881BF8, outside the
// split ranges of this file (no .sdata2 range).
void ChaserNpc::vf10() {
    m568 = 1.0f;
    if (m55C == 6) {
        if (m14B0 != 0 && m14B0 != 4 && m14D0 == 0 && lbl_8087F8A0->mHead->m560 != 0x69) {
            atn::Vector3 d = lbl_8087F8A0->mHead->mPos - mPos;
            if (PSVECMag(&d) < lbl_8087F0A8->m564) {
                fn_8018161C(this, 4);
                return;
            }
        }
        fn_80139560();
        fn_80145334();
    } else {
        fn_80180AE0(this);
        fn_80145334();
    }
}

void ChaserNpc::vf14() {
    fn_80149A30();
}
