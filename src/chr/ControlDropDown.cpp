#include "chr/ControlDropDown.h"
#include "game/CharaManager.h"

extern "C" int rand();
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* axb);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;    // inferred: player manager
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* mgr, CharaTask* chara, int id, int arg, float a, float b); // inferred: voice
extern "C" void* lbl_8087F610;
extern "C" void* lbl_8087F048;
extern "C" void fn_8010A308(void* mgr, const atn::Vector3* from, const atn::Vector3* to, float radius);
// inferred: field (lbl_8087F430): a task (its flag 2 is tested) with the route tables at 0x10D8
struct DropField_unk {
    unsigned char m0000[0x38];
    unsigned int mFlags;
    unsigned char m003C[0x10D8 - 0x3C];
    void* m10D8;
};
extern "C" DropField_unk* lbl_8087F430;
extern "C" int fn_80370174(DropField_unk* field, int id); // inferred: field flag
extern "C" int fn_803C1560(void* routes, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float dist); // inferred: nearest route point
extern "C" atn::Vector3 fn_803C11A4(void* routes, int point); // inferred: position of a route point
// inferred: camera settings (lbl_8087EFA8)
struct DropCamera_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" DropCamera_unk* lbl_8087EFA8;
// inferred: result of the ray test fn_8004ED34
struct DropCollResult_unk {
    DropCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 mPos; // 0x10
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 mNormal; // 0x28
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, DropCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// inferred: random float in [0, 1] (name invented)
inline float Rand01_unk() {
    return (float)(rand() % 1001) / 1000.0f;
}
// inferred: normalizes unless the vector is (almost) zero; name invented
inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(PSVECSquareMag(v)) < 1.1920929e-7f) {
        return;
    }
    PSVECNormalize(v, v);
}

// inferred: vector rotated by a matrix (by value; name invented)
inline atn::Vector3 RotY_unk(Mtx m, const atn::Vector3& v) {
    atn::Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: front direction for a rotation around y (name invented; as in game/ChaserNpc.cpp)
inline atn::Vector3 Front_unk(float ry) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// inferred: frames of slow fall before the character is considered landed (name invented)
inline int SlowLimit_unk() {
    return lbl_8087F610 != 0 ? 25 : 45;
}

namespace chr {

// NONMATCHING: 98.0% - the original keeps a stored (and frsp-converted) z of the temporary of the last
// velocity update; mChara lands in r6 instead of r4
ControlDropDown::ControlDropDown(Chara_unk* chara, const atn::Vector3& vel)
    : ControlBase(chara), mVel(vel), mFrames(0), mStart(0.0f, 0.0f, 0.0f), mFallSpeed(5.0f), m2C(0), m2D(0),
      mSlowFrames(0) {
    chara->m560 = 0x5A;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x68, 0, 0, 1, 0.0f, 0.1f);
    m08 = mChara->m570;
    mVel.y = 0.0f;
    mStart = chara->mPos - vel * 5.0f - mChara->m574 * 3.0f;
    atn::Vector3 v = mChara->m574;
    v.y += -0.015f;
    mChara->m574 = v;
}

inline void ControlDropDown::Land_unk(Motion_unk* motion) {
    motion->fn_80097C08(0, 0x69, 0, 0, 1, 0.0f, 0.1f);
    if (!mChara->mFlag3 && lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0xE, 0, 1.0f, 10.0f);
    }
}

inline void ControlDropDown::End_unk() {
    if ((unsigned int)(mChara->mType - 2) <= 1) {
        mChara->SetHit_unk(0);
        mChara->fn_8015495C(0, 1, 1, 1);
    }
}

// NONMATCHING: 98.2% - same code; several stack slots of the ray probes (fwd/up/side are 12 bytes lower)
// and a few registers differ
int ControlDropDown::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float speed = 0.0f;
    float scale = 1.0f;
    if (motion->mTracks[0].m00 == 0x68) {
        mFrames++;
        if (mChara->m574.y > -0.01f) {
            Land_unk(motion);
        } else {
            atn::Vector3 v = mChara->m574;
            v += mVel;
            mChara->m574 = v;
        }
        if (mChara->mType != 2 && lbl_8087F430 != 0 && (lbl_8087F430->mFlags & 2) == 2) {
            atn::Vector3 d = mChara->mPos - mStart;
            int lost = d.y < -500.0f;
            if (mSlowFrames > (lbl_8087F610 != 0 ? 25 : 45)) {
                if (lbl_8087F610 != 0) {
                    Land_unk(motion);
                    m2D = 1;
                } else {
                    lost = 1;
                }
            }
            if (mChara->mType == 0 && !lost) {
                atn::Vector3 top;
                atn::Vector3 bottom;
                top = bottom = mChara->mPos;
                bottom.y -= 500.0f;
                void* colli = lbl_8087EE98;
                if (fn_8004ED34(colli, 0, &top, &bottom, mChara->fn_80179D44() | 0x80000000, 0, 0) == 0) {
                    lost = 1;
                    for (int i = 0; i < 4; i++) {
                        Mtx m;
                        PSMTXRotRad(m, 'y', 1.5707964f * i);
                        atn::Vector3 p = mChara->mPos + RotY_unk(m, atn::Vector3(0.0f, 0.0f, 2.0f));
                        bottom = top = p;
                        bottom.y -= 500.0f;
                        colli = lbl_8087EE98;
                        if (fn_8004ED34(colli, 0, &top, &bottom, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
                            lost = 0;
                            break;
                        }
                    }
                }
            }
            if (lost && fn_80370174(lbl_8087F430, 0x12B) == 0) {
                mChara->mPos = mStart;
                mChara->m574 = lbl_807C7030;
                mVel = lbl_807C7030;
                if (mChara->mType != 0) {
                    CharaTask* leader = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
                    if (leader != 0) {
                        Mtx r;
                        PSMTXRotRad(r, 'y', 6.2831855f * Rand01_unk());
                        mChara->mPos = leader->mPos + RotY_unk(r, Front_unk(leader->mRot.y)) * 5.0f;
                    }
                } else {
                    if (lbl_8087EE98 != 0) {
                        atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                        Mtx m;
                        PSMTXRotRad(m, 'y', mChara->mRot.y);
                        PSMTXMultVec(m, &fwd, &fwd);
                        atn::Vector3 up(0.0f, 1.0f, 0.0f);
                        atn::Vector3 side;
                        PSVECCrossProduct(&up, &fwd, &side);
                        SafeNormalize_unk(&side);
                        DropCollResult_unk result;
                        for (int i = 0; i < 4; i++) {
                            float t = 30.0f * (i + 1);
                            if (fn_8004ED34(lbl_8087EE98, &result, &(mStart + up * t),
                                            &(mStart + fwd * t - up * t * 2.0f), 0x80000000, 0, 0) &&
                                PSVECDotProduct(&result.mNormal, &up) >= 0.8660254f) {
                                mChara->mPos = result.mPos;
                                break;
                            }
                            if (fn_8004ED34(lbl_8087EE98, &result, &(mStart + up * t),
                                            &(mStart - fwd * t - up * t * 2.0f), 0x80000000, 0, 0) &&
                                PSVECDotProduct(&result.mNormal, &up) >= 0.8660254f) {
                                mChara->mPos = result.mPos;
                                break;
                            }
                            if (fn_8004ED34(lbl_8087EE98, &result, &(mStart + up * t),
                                            &(mStart + side * t - up * t * 2.0f), 0x80000000, 0, 0) &&
                                PSVECDotProduct(&result.mNormal, &up) >= 0.8660254f) {
                                mChara->mPos = result.mPos;
                                break;
                            }
                            if (fn_8004ED34(lbl_8087EE98, &result, &(mStart + up * t),
                                            &(mStart - side * t - up * t * 2.0f), 0x80000000, 0, 0) &&
                                PSVECDotProduct(&result.mNormal, &up) >= 0.8660254f) {
                                mChara->mPos = result.mPos;
                                break;
                            }
                        }
                    }
                    void* routes = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
                    if (routes != 0) {
                        int point = fn_803C1560(routes, &mChara->mPos, 0, 0, 0, 0, -1.0f);
                        if (point > 0) {
                            mChara->mPos = fn_803C11A4(routes, point);
                        }
                    }
                }
            }
        }
    } else {
        int end = mFrames < 8;
        if (motion->fn_80097D40(0x69) == 0 || motion->mTracks[0].m08 >= motion->fn_80097D7C(0) - 5.0f) {
            end = 1;
        }
        if (mChara->mType == 0 && mChara->fn_80178078() > 0.6f &&
            motion->mTracks[0].m08 >= motion->fn_80097D7C(0) - 5.0f) {
            end = 1;
        }
        if (end) {
            End_unk();
            return 1;
        }
        speed = 0.3f * mVel.Length_unk();
    }
    atn::Vector3 pos = mChara->mPos;
    mChara->vf34(mChara->mRot, m2C, speed, scale);
    mFallSpeed += 0.4f * ((pos.y - mChara->mPos.y) - mFallSpeed);
    if (mFallSpeed < 0.3f * lbl_8087EFA8->m3A4) {
        m2C = 1;
        mSlowFrames++;
    }
    if (motion->mTracks[0].m00 == 0x68) {
        atn::Vector3 from = pos;
        atn::Vector3 to = mChara->mPos;
        if (lbl_8087F048 != 0) {
            fn_8010A308(lbl_8087F048, &from, &to, mChara->m5B0);
        }
    }
    return 0;
}

ControlDropDownQuadruped::ControlDropDownQuadruped(Chara_unk* chara, const atn::Vector3& vel)
    : ControlDropDown(chara, vel) {}

int ControlDropDownQuadruped::vf0C() {
    float frame;
    float speed = 0.0f;
    float scale = 1.6f;
    Motion_unk* motion = &mChara->mMotion;
    frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        End_unk();
        return 1;
    }
    if (motion->mTracks[0].m08 <= 14.0f) {
        atn::Vector3 v = mChara->m574;
        v += mVel;
        mChara->m574 = v;
        speed = 1.0f;
    }
    atn::Vector3 pos = mChara->mPos;
    mChara->vf34(mChara->mRot, 0, speed, scale);
    return 0;
}

} // namespace chr
