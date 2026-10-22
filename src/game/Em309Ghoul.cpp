#include "game/Em309Ghoul.h"
#include "math/Vec.h"
#include <string.h>

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, see MP/D2AMpMenuSetRule.cpp)
struct GhoulMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" GhoulMsgTable_unk* lbl_8087F1E4;

// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets a counter

Em309Ghoul::~Em309Ghoul() {}

atn::Vector3 Em309Ghoul::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (fn_80370A78(lbl_8087F430, 0xA1) == 0) {
        result = mPos;
    } else {
        result = m624.mData[1].m04.mCenter;
    }
    return result;
}

void Em309Ghoul::fn_80300654() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 1;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x141), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

void Em309Ghoul::fn_803009F4() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 9;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x143, 0, 0, 1, 0.0f, 0.1f);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x145), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

void Em309Ghoul::fn_80301200() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 13;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
    fn_80370AE4(lbl_8087F430, 0xA3, 1);
}
