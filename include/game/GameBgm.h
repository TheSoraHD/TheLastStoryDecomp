#ifndef GAME_GAMEBGM_H
#define GAME_GAMEBGM_H

#include "atn/Task.h"
#include "atn/Bgm.h"

// inferred: BGM request (0x1C bytes); names invented
struct GameBgmReq_unk {
    GameBgmReq_unk() : mId(0), mName(0), mVolume(1.0f), mFade(0), mStart(0), mCross(0), mLoop(0) {}
    int mId;
    const char* mName; // 0x04
    float mVolume;     // 0x08
    int mFade;         // 0x0C: inferred: fade frames
    int mStart;        // 0x10: inferred: start position
    int mCross;        // 0x14: inferred: cross-fade with the previous BGM
    int mLoop;         // 0x18: inferred: copied to bit 0 of atn::Bgm::m10
};

// inferred from __vt__7GameBgm (0x8078A4F0): singleton task (lbl_8087F448) that plays the field
// BGM with two atn::Bgm players for the cross-fades
class GameBgm : public atn::Task {
public:
    GameBgm(atn::Task* parent); // 0x8037C934
    virtual ~GameBgm();         // 0x8037CA24
    virtual int vf0C();         // 0x8037CAF0
    virtual void vf10();        // 0x8037CB38

    static GameBgm* fn_8037C8C4(atn::Task* parent); // inferred: creates the singleton
    void fn_8037D274(atn::Bgm* bgm, const char* name); // inferred: starts `name` on `bgm`
    int fn_8037D34C();                                 // inferred: a BGM is playing
    void fn_8037D3E8(int a, int b, int c);
    void fn_8037D49C(float volume, int frames);        // inferred: changes the volume
    int fn_8037D4C0(int id, int fade, int b, int c, float volume); // inferred: requests a BGM
    void fn_8037D684(int a, int b);
    void fn_8037D758();                                // inferred: restores the BGM saved in the field
    void fn_8037D808();                                // inferred: saves the BGM in the field

    // inferred: atn::Bgm[2]. atn::Bgm has a pure virtual function (null vtable slot, matched in
    // atn/Bgm.cpp), so this compiler does not accept an array member of it while the original
    // constructs one in place: the array is built and destroyed here with the same runtime calls.
    struct BgmArray_unk {
        BgmArray_unk(); // destroyed in ~GameBgm (no destructor here: it would add a null check)
        atn::Bgm* Get(int i) { return (atn::Bgm*)m[i]; }
        unsigned char m[2][0x14];
    };

    int mState;               // 0x48
    BgmArray_unk mBgm;        // 0x4C
    atn::Bgm* mCur;           // 0x74
    atn::Bgm* mPrev;          // 0x78
    int mIndex;               // 0x7C
    int m80;
    int m84;                  // inferred: a request is pending (mNext)
    int m88;                  // inferred: swap the players
    GameBgmReq_unk mReq;      // 0x8C
    GameBgmReq_unk mPrevReq;  // 0xA8
    GameBgmReq_unk mNext;     // 0xC4
    float mE0;                // inferred: previous volume

    static GameBgm* sInstance; // lbl_8087F448
};

#endif
