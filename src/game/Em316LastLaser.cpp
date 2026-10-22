#include "game/Em316LastLaser.h"

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
// inferred: effect/bullet manager of the characters (class unknown)
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" int fn_800F8548(BulletMgr_unk* mgr); // inferred: new bullet id

inline void LaserList_unk::Erase_unk(LaserPoint_unk** it) {
    memmove(it, it + 1, (&this->mData[this->mSize] - it - 1) * sizeof(LaserPoint_unk*));
    this->mSize--;
}

// inferred: removes every point of the trail and stops its effects and its sound
inline void LaserTrace::Reset_unk() {
    LaserPoint_unk** it = m28.begin();
    m04 = 0;
    m34 = 0;
    while (it != m28.end()) {
        fn_80239DAC(lbl_8087F3C0, *it, 0, 0);
        delete *it;
        m28.Erase_unk(it);
    }
    if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    fn_800CB5C8(&m38, 0, 0);
}

// NONMATCHING: 97.2% - same register swap as ~LaserTrace in the inlined Reset_unk
Em316LastLaser::~Em316LastLaser() {
    m1538.Reset_unk();
}

void Em316LastLaser::vf14() {
    fn_80149A30();
}

void Em316LastLaser::vf3C(HitInfo_unk* hit) {
    fn_80151448((HitMsg_unk*)hit);
    vf40(hit);
}

void Em316LastLaser::fn_8031D27C() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
}

void Em316LastLaser::fn_8031D2BC() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
}

LaserTrace::LaserTrace()
    : m04(0), m08(0), m10(0.0f), m14(0.0f), m18(0.0f), m1C(0.0f), m20(1.0f), m24(0.0f), m34(0) {
    m28.__shrink_unk(0);
}

// NONMATCHING: 91.7% - registers: the original keeps the iterator in r29 and `this` in r30 (here r31/r29);
// tried the statement order, a for loop and a local for the deleted point
LaserTrace::~LaserTrace() {
    Reset_unk();
}

LaserPoint_unk** LaserList_unk::Back_unk() {
    return &(*this)[size() - 1];
}
