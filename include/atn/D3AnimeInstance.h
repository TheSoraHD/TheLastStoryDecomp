#ifndef ATN_D3ANIMEINSTANCE_H
#define ATN_D3ANIMEINSTANCE_H

#include "atn/Task.h"
#include "math/Vec.h"
#include "atn/D3Anime.h"

namespace atn {

// inferred: d2anime instance task (fn_801F64D0); only the members used here
struct D2AInstance_unk : public Task {
    unsigned char m48[0x4E - 0x48];
    unsigned char m4E;
};

// inferred: instance of a d2anime layout drawn in 3D (same layout as D3Anime)
class D3AnimeInstance : public Task {
public:
    D3AnimeInstance(Task* parent, const char* name, int load); // 0x80202AF0
    static D3AnimeInstance* Create(Task* parent, const char* name, int load); // 0x80202A6C
    virtual ~D3AnimeInstance();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    int Load();              // 0x80202CA4
    D2AInstance_unk* GetTask() const; // 0x80202D00
    void Draw();             // 0x80203328
    Matrix3x4 GetDrawMtx_unk(float w, float h, float z);
    void SetMatrix(const Matrix3x4& m); // 0x8020303C
    void UpdateMtx();                   // 0x80203260

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

    D2AInstance_unk* mTask; // 0x48
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
