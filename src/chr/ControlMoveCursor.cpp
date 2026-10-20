#include "chr/ControlMoveCursor.h"

extern "C" double sqrt(double x);
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" void* lbl_8087EFB4;                                        // inferred: camera

// inferred: weak copies of small inlines (getters of the pad, the camera and its rotation, cosine,
// dot product, horizontal length, rotation setter); the original calls them out of line in
// fn_801C346C (inline budget exceeded)
extern "C" inline void* fn_801156BC() {
    return lbl_8087EF70;
}
extern "C" inline void* fn_8008B964() {
    return lbl_8087EFB4;
}
extern "C" inline void* fn_800F7258(void* camera) {
    return (char*)camera + 0x104;
}
extern "C" inline atn::Vector3* fn_80116E6C(void* obj) {
    return (atn::Vector3*)((char*)obj + 0x2C);
}
extern "C" inline float fn_801C1060(float x) {
    return cos(x);
}
extern "C" inline float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}
extern "C" inline void fn_801C0738(CharaTask* chara, const atn::Vector3& rot) {
    chara->mRot = rot;
}

// inferred: a camera position and the point it looks at (two of them in CursorField_unk)
struct CursorView_unk {
    // inferred: viewing direction (inline)
    atn::Vector3 GetDir_unk() const { return (mTarget - mPos).GetNormalized_unk(); }
    atn::Vector3 mPos;
    atn::Vector3 mTarget;
};

// inferred: field state (lbl_8087F430), only the members read here
struct CursorField_unk {
    unsigned char m000[0x74];
    CursorView_unk m74;
    unsigned char m8C[0x268 - 0x8C];
    CursorView_unk m268;
    unsigned char m280[0x868 - 0x280];
    int m868;
    int m86C;
};
extern "C" CursorField_unk* lbl_8087F430;

// inferred: collision world and ray test (see gmk/MotionObject.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    void* m34;
    void* m38;
    unsigned int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3& from, const atn::Vector3& to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
atn::Vector3 fn_8017C9AC(CharaTask* chara); // inferred: position of the character's eyes

namespace chr {

ControlMoveCursor::ControlMoveCursor(Chara_unk* chara) : ControlBase(chara), m08(chara->mPos), m14(chara->mPos), m20(0.0f), m24(false) {}

void ControlMoveCursor::fn_801C3458(const CursorTarget_unk* target, float a, float b, float c) {
    if (m24) {
        fn_801C3944(target, a, b, c);
    } else {
        fn_801C346C(target, a, b, c);
    }
}

// NONMATCHING: 97.6% - the original does not call a constructor for the two Matrix3x4 locals (there is
// no weak __ct__Q23atn9Matrix3x4Fv in the game: its default constructor must be trivial), while ours
// calls the empty inline one (this function is over the inline budget: every inline is called)
void ControlMoveCursor::fn_801C346C(const CursorTarget_unk* target, float maxDist, float maxRadius, float turn) {
    atn::Vector3 stick;
    stick.x = -fn_800A56A8(fn_801156BC(), 0, 0, 0);
    stick.y = 0.0f;
    stick.z = -fn_800A56A8(fn_801156BC(), 0, 1, 0);
    float len = stick.Length_unk();
    atn::Vector3 rot(*mChara->GetRot_unk());
    if (len >= 0.4f) {
        float yaw = fn_80116E6C(fn_800F7258(fn_8008B964()))->y;
        rot = stick.ToRotation_unk();
        rot.y = yaw - rot.y;
        float t = (len - 0.4f) / 0.6f;
        t *= t;
        float speed = 1.0f < t ? 1.0f : t;
        if (m20 < 4.0f) {
            m20 += 0.4f;
            if (m20 > 4.0f) {
                m20 = 4.0f;
            }
        }
        atn::Vector3 move(0.0f, 0.0f, speed * m20);
        atn::Matrix3x4 mtx;
        mtx.fn_8013A13C(rot.y);
        move.fn_80011410(mtx);
        atn::Vector3 next = m08 + move;
        next -= m14;
        if (next.Length_unk() > maxDist) {
            m08 += move;
        } else {
            move = m08 + move - m14;
            move.y = 0.0f;
            move.Normalize_unk();
            m08 = move * (maxDist + 0.00001f) + m14;
        }
        m08.y = m14.y;
        if (target != 0) {
            if (fn_801C3910(target->mPos - m08) < target->mRadius) {
                m08.y += target->mHeight;
            }
        }
        move = m08 - m14;
        if (maxRadius < move.Length_unk()) {
            move.Normalize_unk();
            m08 = m14 + move * maxRadius;
        }
        move = m08 - m14;
        float dist = move.Length_unk();
        if (dist > 0.0f) {
            move.Normalize_unk();
            atn::Vector3 dir = mChara->fn_8014052C();
            if (turn < 0.0f) {
                dir = -dir;
            }
            float dot = fn_801A03E8(dir, move);
            if (dot < fn_801C1060(atn::Abs_unk(0.5f * turn))) {
                float a = move.ToRotation_unk().y;
                float b = dir.ToRotation_unk().y;
                float diff = atn::WrapAngle_unk(a - b);
                float rest;
                if (diff > 0.0f) {
                    b += atn::Abs_unk(0.5f * turn);
                    rest = diff - atn::Abs_unk(0.5f * turn);
                } else {
                    b -= atn::Abs_unk(0.5f * turn);
                    rest = diff + atn::Abs_unk(0.5f * turn);
                }
                move.Set_unk(0.0f, 0.0f, dist * fn_801C1060(rest));
                atn::Matrix3x4 mtx2;
                mtx2.fn_8013A13C(b);
                move.fn_80011410(mtx2);
                m08 = m14 + move;
            }
        }
        atn::Vector3 newRot(*mChara->GetRot_unk());
        newRot.y = move.ToRotation_unk().y;
        fn_801C0738(mChara, newRot);
    } else {
        m20 = 0.0f;
    }
}

// NONMATCHING: 96.6% - scheduling of the stores of the ray end point (`from + dir * dist`) around the
// CollResult_unk constructor. `to` is named so that this function stays out of the autoinline
// threshold of fn_801C3458 (with a temporary, fn_801C3458 inlines it); as a temporary the stack matches
// better.
void ControlMoveCursor::fn_801C3944(const CursorTarget_unk* target, float a, float dist, float c) {
    CursorField_unk* field = lbl_8087F430;
    if (field->m868 != 4 || field->m868 != field->m86C) {
        return;
    }
    atn::Vector3 dir = field->m268.GetDir_unk();
    atn::Vector3 from = fn_8017C9AC(mChara);
    CollResult_unk res;
    atn::Vector3 to = from + dir * dist;
    if (fn_8004ED34(lbl_8087EE98, &res, from, to, 4, &mChara->mColli, 0) != 0) {
        m08 = res.m04;
    } else {
        m08 = from + dir * dist;
    }
    atn::Vector3 look = field->m74.GetDir_unk();
    look.y = 0.0f;
    fn_801C0738(mChara, look.ToRotation_unk());
}

// NONMATCHING: 89.3% - scheduling: the original loads mChara before the two float constants and the
// collision world after the first position copy
int ControlMoveCursor::fn_801C3D1C() {
    atn::Vector3 from(mChara->mPos);
    void* colli = lbl_8087EE98;
    atn::Vector3 to(mChara->mPos);
    from.y += 5.0f;
    to.y += 23.0f;
    return fn_8004ED34(colli, 0, from, to, mChara->fn_80179D44() | 0x80000000, 0, 0) == 0;
}

} // namespace chr
