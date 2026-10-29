#include "gmk/FlamingArrow.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/CharaStatus.h"
#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data

// inferred: frame time (class unknown)
extern "C" void* fn_800F52F8();
extern "C" float fn_802F0998(void* settings); // inferred: gravity per frame
// inferred: collision world and its ray test (see CharaTask.cpp)
struct CollResult_unk {
    CollResult_unk();
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x28 - 0x10];
    atn::Vector3 m28; // inferred: normal
    unsigned char m34[0x38 - 0x34];
    void* m38;        // inferred: hit object
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
void* fn_801404F8();
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_801A03F4(void* obj); // inferred: the hit object is a character
extern "C" float fn_801A03E8(const atn::Vector3* a, const atn::Vector3* b); // inferred: dot product
// inferred: effect manager (class unknown)
struct ArrowEffectMgr_unk;
extern "C" ArrowEffectMgr_unk* fn_800F7FA0();
extern "C" void fn_80239DAC(ArrowEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800F7FA8(ArrowEffectMgr_unk* mgr, void* owner, int id);
extern "C" void fn_8026607C(ArrowEffectMgr_unk* mgr, atn::EfpFile* efp, void* owner, int arg);
extern "C" void fn_80276AE4(ArrowEffectMgr_unk* mgr, atn::EfpFile* efp, int arg2, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg5, int arg6, int arg7, float scale);
// inferred: sounds
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
struct SoundCfg_unk {
    // inferred: random pitch (weak copy 0x80417E88); name invented
    float fn_80417E88() { return 1.0f + (m34CC * (rand() % 1001 / 1000.0f) - 0.5f * m34CC); }
    unsigned char m00[0x34CC];
    float m34CC; // inferred: pitch variation
};
SoundCfg_unk* GetSoundMgr_unk(); // inferred: sound manager (Event/Path.cpp)
// inferred: model matrix helpers (class unknown, 0x30 bytes)
struct ArrowMtx_unk {
    float m[3][4];
};
extern "C" void fn_800F80A8(ArrowMtx_unk* mtx, const atn::Vector3* pos);
extern "C" void fn_800F80B8(ArrowMtx_unk* mtx, const atn::Vector3* rot);
extern "C" void fn_80316E38(atn::Model* model, ArrowMtx_unk* mtx);
// inferred: callback object passed to atn::Model::CalcMatrixStack (0x14 bytes, constructor and destructor out of line)
struct ArrowCallback_unk {
    unsigned char m00[0x14];
};
extern "C" void fn_80317034(ArrowCallback_unk* callback, int arg);
extern "C" void fn_8000D3A8(ArrowCallback_unk* callback, int arg);
// inferred: color (class unknown)
struct ArrowColor_unk {
    float r, g, b, a;
};
extern "C" void fn_800D8BB4(ArrowColor_unk* color, const ArrowColor_unk* src);
namespace atn {
// inferred: RGBA color of the engine (constructor: weak copy 0x800D8814 of atn/CompositeIntensityTintMulti.cpp)
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_);
    float r, g, b, a;
};
} // namespace atn

// inferred: atn::Model accessors of the color at 0x40 (weak copies; names keep the addresses)
inline ArrowColor_unk* fn_80417F14(atn::Model* model) {
    return (ArrowColor_unk*)&model->m40;
}
inline void fn_80417F1C(atn::Model* model, const ArrowColor_unk* color) {
    *(ArrowColor_unk*)&model->m40 = *color;
}
// inferred: next character of the player/enemy lists (0x14AC)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
// inferred: names invented
inline bool IsDisabled_unk(CharaTask* chara) {
    bool ret = false;
    if (chara->IsFlag_unk(4) || chara->IsFlag_unk(1)) {
        ret = true;
    }
    return ret;
}
inline bool IsHeld_unk(CharaTask* chara) {
    bool ret = false;
    if (chara->m55C == 6 && chara->m560 == 28) {
        ret = true;
    }
    return ret;
}
inline bool CanTarget_unk(CharaTask* chara) {
    bool ok = false;
    bool disabled = false;
    if (chara->IsFlag_unk(4) || chara->IsFlag_unk(1)) {
        disabled = true;
    }
    if (!disabled && (chara->mStatus.m0C & 0x20) != 0x20) {
        bool held = false;
        if (chara->m55C == 6 && chara->m560 == 28) {
            held = true;
        }
        if (!held) {
            ok = true;
        }
    }
    return ok;
}
// inferred: list of characters (16 entries; names invented)
struct ArrowTargets_unk {
    ArrowTargets_unk(); // 0x804180B0: weak copy
    CharaTask** At_unk(unsigned int i); // 0x804180BC: weak copy
    // NONMATCHING: 96.1% - register allocation (the loop pointer reuses r4 in the original)
    template <class T>
    void Collect_unk(CharaManager<T>* manager) {
        for (T* chara = manager->mHead; chara != 0; chara = (T*)((CharaListNode_unk*)chara)->mNext) {
            if (CanTarget_unk(chara)) {
                CharaTask** slot = &mData[mCount];
                if (slot != 0) {
                    *slot = chara;
                }
                if (++mCount >= 16) {
                    return;
                }
            }
        }
    }
    unsigned int mCount;
    CharaTask* mData[16];
};
inline ArrowTargets_unk::ArrowTargets_unk() {
    mCount = 0;
}
inline CharaTask** ArrowTargets_unk::At_unk(unsigned int i) {
    return &mData[i];
}
extern "C" int fn_803D6EDC(ArrowTargets_unk* list);           // inferred: empty
extern "C" unsigned int fn_80372574(ArrowTargets_unk* list);  // inferred: size
extern "C" CharaManager<PlayerTask>* fn_8000D9E8(); // inferred: player manager
extern "C" CharaManager<EnemyTask>* fn_801A03E0();  // inferred: enemy manager
// inferred: field object (see CharaTask.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
};
extern "C" Field_unk* fn_80121F00();

// inferred: sounds of the hit
static const char* const sHitSounds[] = {"SE_DG013_001_1", "SE_DG013_001_2", "SE_DG013_001_3"};

#define __MAX_FA(a, b) ((a) > (b) ? (a) : (b))

namespace gmk {

GimmickCommon* FlamingArrow::fn_80416F74(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) FlamingArrow(parent, desc);
    }
    return 0;
}

FlamingArrowShot_unk::FlamingArrowShot_unk() : mModel(0, 1), mState(0) {}

FlamingArrowShot_unk::~FlamingArrowShot_unk() {}

FlamingArrow::~FlamingArrow() {}

int FlamingArrow::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

void FlamingArrow::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL_BASE") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MODEL_ARROW") == 0) {
            const char* path = csv.fn_8005B9CC();
            for (unsigned int i = 0; i < 8; i++) {
                fn_8008AD4C(&mArrows[i].mModel, path, 0);
            }
        } else if (strcmp(field, "MOTION_BASE") == 0) {
            fn_80097A88(&mModel, 0, csv.fn_8005B9CC());
        } else if (strcmp(field, "MOTION_ARROW") == 0) {
            const char* path = csv.fn_8005B9CC();
            for (unsigned int i = 0; i < 8; i++) {
                fn_80097A88(&mArrows[i].mModel, 0, path);
            }
        } else if (strcmp(field, "EFP_SHOOT") == 0) {
            mEfpShoot.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_ARROW") == 0) {
            mEfpArrow.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_BURNOUT") == 0) {
            for (unsigned int i = 0; i < 3; i++) {
                mEfpBurnout[i].LoadRequest(csv.fn_8005B9CC());
            }
        } else if (strcmp(field, "EFF_HIT") == 0) {
            for (unsigned int i = 0; i < 3; i++) {
                mEfpHit[i].LoadRequest(csv.fn_8005B9CC());
            }
        } else if (strcmp(field, "PARAM") == 0) {
        }
    } while (csv.fn_8005BBF8() != 0);
}

int FlamingArrow::vf1C() {
    int ret = 0;
    if (mModel.fn_8008B140() != 0) {
        ret = 1;
    }
    for (unsigned int i = 0; i < 8; i++) {
        if (mArrows[i].mModel.fn_8008B140() != 0) {
            ret = 1;
        }
    }
    if (mEfpShoot.IsLoading() != 0) {
        ret = 1;
    }
    if (mEfpArrow.IsLoading() != 0) {
        ret = 1;
    }
    for (unsigned int i = 0; i < 3; i++) {
        if (mEfpBurnout[i].IsLoading() != 0) {
            ret = 1;
        }
        if (mEfpHit[i].IsLoading() != 0) {
            ret = 1;
        }
    }
    return ret;
}

void FlamingArrow::vf24() {}

FlamingArrowShot_unk* FlamingArrow::fn_8041762C() {
    for (int i = 0; i < 8; i++) {
        if (mArrows[i].mState == 0) {
            return &mArrows[i];
        }
    }
    return 0;
}

// NONMATCHING: 97.4% - stack layout of the temporaries (the original frame is 0x10 bytes larger) and
// a few saved registers. As in the original, nothing is inlined in this function (the inline
// budget is exhausted), so the Vector3 operators and the small helpers are called out of line.
void FlamingArrow::vf10() {
    if (vf60() == 0) {
        return;
    }
    for (unsigned int i = 0; i < 8; i++) {
        if (mArrows[i].mState == 1) {
            mArrows[i].mVel.y += fn_802F0998(fn_800F52F8());
            atn::Vector3 from(mArrows[i].mPos);
            atn::Vector3 to = mArrows[i].mPos + mArrows[i].mVel;
            CollResult_unk result;
            if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000004, 0, 0) != 0) {
                mArrows[i].mPos = result.m04;
                mArrows[i].mState = 2;
                mArrows[i].mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
                mArrows[i].mModel.SetMotionSpeed(0, 1.0f);
                atn::Vector3 pos(result.m04);
                atn::Vector3 normal = result.m28.ToRotation_unk();
                atn::Vector3 rot(0.0f, normal.y, 0.0f);
                int kind = 1;
                if (result.m38 != 0 && fn_801A03F4(result.m38) != 0) {
                    kind = 2;
                } else {
                    atn::Vector3 up(0.0f, 1.0f, 0.0f);
                    if (fn_801A03E8(&result.m28, &up) > 0.8660254f) {
                        kind = 0;
                    }
                }
                fn_80239DAC(fn_800F7FA0(), &mArrows[i].mModel, 0, 1);
                fn_800F7FA8(fn_800F7FA0(), &mArrows[i].mModel, 1);
                fn_8026607C(fn_800F7FA0(), &mEfpBurnout[kind], &mArrows[i].mModel, 1);
                fn_800F7FA8(fn_800F7FA0(), &mArrows[i].mModel, 2);
                fn_80276AE4(fn_800F7FA0(), &mEfpHit[kind], 0, &pos, &rot, -1, -1, 1, 1.0f);
                atn::SoundRef sound = fn_800C344C(sHitSounds[rand() % 3], &mArrows[i].mPos, 0, -1, 1.0f);
                if (GetSoundMgr_unk() != 0) {
                    fn_800CB688(&sound, 0, GetSoundMgr_unk()->fn_80417E88());
                }
            } else {
                mArrows[i].mPos = to;
            }
            atn::Vector3 dir = mArrows[i].mVel.ToRotation_unk();
            ArrowMtx_unk mtx;
            fn_800F80A8(&mtx, &mArrows[i].mPos);
            fn_800F80B8(&mtx, &dir);
            fn_80316E38(&mArrows[i].mModel, &mtx);
            mArrows[i].mModel.CalcMatrixStack(atn::ModelNodeCallback_unk(0));
        } else if (mArrows[i].mState == 2) {
            mArrows[i].mModel.fn_80097E80(1);
            mArrows[i].mModel.CalcMatrixStack(atn::ModelNodeCallback_unk(0));
            float length = mArrows[i].mModel.fn_80097D7C(0);
            if (mArrows[i].mModel.GetMotionFrame(0) >= length) {
                fn_80239DAC(fn_800F7FA0(), &mArrows[i].mModel, 1, 1);
                mArrows[i].mState = 3;
            }
        } else if (mArrows[i].mState == 3) {
            ArrowColor_unk color;
            fn_800D8BB4(&color, fn_80417F14(&mArrows[i].mModel));
            color.r = __MAX_FA(0.0f, color.r - 0.02f);
            color.g = __MAX_FA(0.0f, color.g - 0.02f);
            color.b = __MAX_FA(0.0f, color.b - 0.02f);
            fn_80417F1C(&mArrows[i].mModel, &color);
            if (color.r <= 0.001f) {
                mArrows[i].mState = 0;
            }
        }
    }
    if (++mTimer <= mDesc->m28) {
        return;
    }
    mTimer = 0;
    FlamingArrowShot_unk* arrow = fn_8041762C();
    if (arrow == 0) {
        return;
    }
    HitInfo_unk* skill = fn_80219E6C(mDesc->m20);
    RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(mDesc->m24);
    atn::Vector3 target;
    if (place != 0) {
        target = place->mPos;
    } else {
        ArrowTargets_unk list;
        if (mDesc->m2C == 2) {
            list.Collect_unk(fn_801A03E0());
        } else {
            list.Collect_unk(fn_8000D9E8());
        }
        if (fn_803D6EDC(&list) != 0) {
            return;
        }
        unsigned int count = fn_80372574(&list);
        target = *(*list.At_unk(rand() % count))->fn_8013C38C();
    }
    atn::Vector3 offset(0.0f, 0.0f, mDesc->m40 * (rand() % 1001 / 1000.0f));
    atn::Matrix3x4 rot;
    rot.fn_8013A13C(6.2831855f * (rand() % 1001 / 1000.0f));
    offset.fn_80011410(rot);
    target += offset;
    atn::Vector3 dir;
    float speed = skill->m4C;
    if (skill->m02 == 1) {
        dir = target - mDesc->mPos;
        dir.y = 0.0f;
        float time = dir.Length_unk() / speed;
        float height = mDesc->mPos.y - target.y;
        float vy = time * (-0.5f * fn_802F0998(fn_800F52F8())) - height / time;
        dir.Normalize_unk();
        dir *= speed;
        dir.y = vy;
        speed = dir.Length_unk();
        dir.Normalize_unk();
    } else {
        dir = target - mDesc->mPos;
        dir.Normalize_unk();
    }
    arrow->mState = 1;
    arrow->mPos = mDesc->mPos;
    arrow->mVel = dir * speed;
    arrow->mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
    arrow->mModel.SetMotionSpeed(0, 0.0f);
    arrow->mModel.fn_80097E80(1);
    atn::Vector3 angle = arrow->mVel.ToRotation_unk();
    ArrowMtx_unk mtx;
    fn_800F80A8(&mtx, &arrow->mPos);
    fn_800F80B8(&mtx, &angle);
    fn_80316E38(&arrow->mModel, &mtx);
    arrow->mModel.CalcMatrixStack(atn::ModelNodeCallback_unk(0));
    fn_80417F1C(&arrow->mModel, (const ArrowColor_unk*)&atn::Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f));
    fn_800F7FA8(fn_800F7FA0(), arrow, 0);
    fn_8026607C(fn_800F7FA0(), &mEfpArrow, arrow, 1);
}

void FlamingArrow::vf14() {
    if (vf60() != 0) {
        for (unsigned int i = 0; i < 8; i++) {
            if (mArrows[i].mState != 0) {
                mArrows[i].mModel.fn_8008CD60();
            }
        }
    }
}

int FlamingArrow::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

int FlamingArrow::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

} // namespace gmk
