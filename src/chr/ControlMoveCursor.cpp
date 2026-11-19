#include "chr/ControlMoveCursor.h"
#include "atn/RenderManager.h"
#include "atn/Camera_unk.h"
#include "atn/ColliBase.h"
#include "game/GameTask.h"

extern "C" double sqrt(double x);
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis

// inferred: weak copies of small inlines (getters of the pad, the camera and its rotation, cosine,
// dot product, horizontal length, rotation setter); the original calls them out of line in
// fn_801C346C (inline budget exceeded)
inline void* GetPad_unk() {
    return lbl_8087EF70;
}
// inferred: weak copies of another file (the original calls them out of line)
inline atn::Camera* atn::RenderManager::fn_800F7258() {
    return &mCamera;
}
// inferred: weak copy 0x801C1060 (name from symbols.txt)
inline float Cos_unk(float x) {
    return cos(x);
}
extern "C" inline float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}

// inferred: viewing direction of a camera (inline; name invented)
static inline atn::Vector3 GetDir_unk(const atn::Camera& camera) {
    return (camera.mTarget - camera.mPos).GetNormalize();
}

// inferred: the field task (lbl_8087F430)
extern "C" GameTask* lbl_8087F430;

struct ColliManager_unk;
extern "C" ColliManager_unk* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, atn::ColliResult_unk* result, const atn::Vector3& from, const atn::Vector3& to,
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

// inferred: the rotation matrices are raw Mtx viewed as atn::Matrix3x4 (as in ControlJumpAt): the
// original calls no constructor for them (no weak __ct__Q23atn9Matrix3x4Fv in the game), and this
// function is over the inline budget (every inline is called)
void ControlMoveCursor::fn_801C346C(const CursorTarget_unk* target, float maxDist, float maxRadius, float turn) {
    atn::Vector3 stick;
    stick.x = -fn_800A56A8(GetPad_unk(), 0, 0, 0);
    stick.y = 0.0f;
    stick.z = -fn_800A56A8(GetPad_unk(), 0, 1, 0);
    float len = stick.Length_unk();
    atn::Vector3 rot(*mChara->GetRot_unk());
    if (len >= 0.4f) {
        float yaw = fn_8008B964()->fn_800F7258()->GetRot_unk()->y;
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
        float z = speed * m20;
        atn::Vector3 move(0.0f, 0.0f, z);
        Mtx mtx;
        ((atn::Matrix3x4*)mtx)->fn_8013A13C(rot.y);
        move.fn_80011410(*(atn::Matrix3x4*)mtx);
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
            if (dot < Cos_unk(atn::Abs_unk(0.5f * turn))) {
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
                move.Set_unk(0.0f, 0.0f, dist * Cos_unk(rest));
                Mtx mtx2;
                ((atn::Matrix3x4*)mtx2)->fn_8013A13C(b);
                move.fn_80011410(*(atn::Matrix3x4*)mtx2);
                m08 = m14 + move;
            }
        }
        atn::Vector3 newRot(*mChara->GetRot_unk());
        newRot.y = move.ToRotation_unk().y;
        mChara->SetRot_unk(newRot);
    } else {
        m20 = 0.0f;
    }
}

// The original calls this function out of line from fn_801C3458 (with the ray end point as a temporary,
// MWCC would inline it there): __declspec(noinline).
__declspec(noinline) void ControlMoveCursor::fn_801C3944(const CursorTarget_unk* target, float a, float dist, float c) {
    GameTask* field = lbl_8087F430;
    if (field->mCamera.m7FC != 4 || field->mCamera.m7FC != field->mCamera.m800) {
        return;
    }
    atn::Vector3 dir = GetDir_unk(field->mCamera.m1F4);
    atn::Vector3 from = fn_8017C9AC(mChara);
    atn::ColliResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, from, from + dir * dist, 4, mChara->GetColli_unk(), 0) != 0) {
        m08 = res.m04;
    } else {
        m08 = from + dir * dist;
    }
    atn::Vector3 look = GetDir_unk(field->mCamera.mCamera);
    look.y = 0.0f;
    mChara->SetRot_unk(look.ToRotation_unk());
}

int ControlMoveCursor::fn_801C3D1C() const {
    atn::Vector3 from(mChara->mPos);
    void* colli = lbl_8087EE98;
    atn::Vector3 to(mChara->mPos);
    from.y += 5.0f;
    to.y += 23.0f;
    return fn_8004ED34(colli, 0, from, to, mChara->fn_80179D44() | 0x80000000, 0, 0) == 0;
}

} // namespace chr
