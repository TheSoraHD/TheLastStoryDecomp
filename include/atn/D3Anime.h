#ifndef ATN_D3ANIME_H
#define ATN_D3ANIME_H

#include "atn/Task.h"
#include "math/Vec.h"

namespace atn {

// inferred: d2anime task (fn_801F3FF8); only the members used here
struct D2ATask_unk : public Task {
    unsigned char m48[0xFC - 0x48];
    unsigned int mFC;
};

// inferred: d2anime layout drawn as a 3D billboard (names of the members invented)
class D3Anime : public Task {
public:
    D3Anime(Task* parent, const char* name, int load); // 0x80201EFC
    static D3Anime* Create(Task* parent, const char* name, int load); // 0x80201E78
    virtual ~D3Anime();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    int Load(int arg);       // 0x802020B4
    D2ATask_unk* GetTask() const; // 0x80202118
    void Draw();             // 0x80202460
    Matrix3x4 GetDrawMtx_unk(float w, float h, float z);

    int UpdateLoad_unk() {
        int busy;
        if (mState == 1 && mTask != 0) {
            if (mTask->IsFlag_unk(2)) {
                mTask->fn_800D246C(0);
                mTask->mFlags |= 4;
                mState = 2;
                busy = 0;
            } else {
                busy = 1;
            }
        } else {
            busy = 0;
        }
        return busy;
    }

    struct D2ATask_unk* mTask; // 0x48
    char mName[0x40];    // 0x4C
    Matrix3x4 mMtx;      // 0x8C
    Vector3 mPos;        // 0xBC
    Matrix3x4 mRot;      // 0xC8
    Vector3 mScale;      // 0xF8
    int mState : 8;      // 0x104
    int mVisible : 1;
    int mFlag9 : 1;      // inferred: centered
    int mFlag10 : 1;
    int mEnabled : 1;
};

} // namespace atn

#endif
