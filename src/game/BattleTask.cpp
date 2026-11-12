// game/BattleTask.cpp (0x800F13BC-0x8010D98C): battle manager (BattleTask, HomingBullet). Only the
// effect start/stop functions are reconstructed so far.
#include "game/BattleTask.h"
#include "atn/Memory.h"
#include "atn/EffectManager.h"
#include "atn/EffectSystem_unk.h"
#include "math/Vec.h"
#include "atn/SoundRef.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "game/HitInfo_unk.h"
#include "game/LogMessageTask.h"
#include "macros.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "game/GameScene.h"
#include "game/BattleCharaTask.h"
#include "atn/Camera_unk.h"
#include "atn/RenderManager.h"
#include "atn/DrawHelper.h"
#include "atn/Color.h"
#include "atn/D2A_Task.h"
#include "atn/NodeChunk.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void* fn_80210220(int id); // inferred: record of an attract-hate parameter (as in game/CharaTask.cpp)
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" GameTask* lbl_8087F430;
extern "C" int rand();
extern "C" void* lbl_8087F610; // inferred: multiplayer manager (null in single player)
extern "C" atn::EfpFile* fn_800EFBC4(int id); // inferred: battle effect of row `id`
extern "C" atn::EfpFile* fn_800EFDA0(int id); // inferred: skill effect of row `id`
extern "C" const char* fn_800EFE3C(int id);   // inferred: sound of row `id`
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_80219558(int id); // inferred: party member kind of a character id
extern "C" int fn_80239D58(atn::EffectManager* mgr, void* owner, int id); // inferred: the effect is playing

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: node callback of a model (as game/CharaTask.cpp: the functor manager uses the default allocator of
// boost::function3, std::allocator<function_base>)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
// inferred: sets the node callback of a model (weak copy 0x800F29D0 of an inline of atn::Model that the
// other files call by this name; ModelBase::mB8 is declared with another signature)
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const EffectColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an EFP effect at the model, without offset (as gmk/Coffin.cpp)
inline void PlayEfp_unk(atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    EffectColor_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

BattleTask* BattleTask::fn_800F13BC(atn::Task* parent) {
    if (parent != 0 && lbl_8087F048 == 0) {
        lbl_8087F048 = new ((MemoryTag)1, "", "", 0) BattleTask(parent);
    }
    return lbl_8087F048;
}

// inferred: setup: waits for the effects and the d2anime tasks, then binds the node callback of the effect
// models (the nodes follow the character the model is drawn on)
int BattleTask::vf0C() {
    int ready = 1;
    if (fn_800D3FA4() == 0) {
        ready = 0;
    }
    if (fn_800F3490() != 0) {
        ready = 0;
    }
    if (fn_800F3B4C() != 0) {
        ready = 0;
    }
    if (ready != 0) {
        mCB30->fn_800D246C(0);
        mCB30->mRepeat = 0;
        mCB30->FreezeTask();
        if (mCB60 != 0) {
            mCB60->fn_800D246C(0);
            mCB60->mRepeat = 1;
            mCB60->FreezeTask();
        }
        m3E2F4->fn_800D246C(0);
        m3E2F4->mRepeat = 0;
        m3E2F4->FreezeTask();
        for (unsigned int i = 0; i < 8; i++) {
            atn::Model* model = mSlots[i].mModel.GetModel_unk();
            if (model->m20C_31_24 == 8) {
                fn_800F29D0(model, boost::bind(&BetoModel::fn_8010D854, &mSlots[i], _1, _2, _3));
                model->fn_80091CFC("head");
                model->fn_80091CFC("lefthand");
                model->fn_80091CFC("leftleg");
                model->fn_80091CFC("righthand");
                model->fn_80091CFC("rightleg");
                model->fn_80091CFC("spine1");
                model->fn_80091CFC("waist");
            }
        }
        fn_800D246C(1);
    }
    return ready;
}

// ---- fn_800F72F4: target marker of a character (over the inlining budget: every helper below is called
// out of line, and their weak copies follow it at 0x800F7F60-0x800F8548) ----

// inferred: marker of a character (0x78 bytes, 72 of them at BattleTask+0x367B8); names invented
struct BattleMarker_unk {
    atn::Matrix3x4 mMtx;  // 0x00: base of the line
    atn::Matrix3x4 mMtx2; // 0x30: head of the line
    atn::Vector3 m60;     // inferred: last end point of the line
    float m6C;            // inferred: last scale
    int m70;              // inferred: frames left of the appearance (12 when the target changes)
    unsigned short mMode; // 0x74: 1 own side or multiplayer, 2 `arg`, 3 the leader is flagged, 0 otherwise
    unsigned short mFlags; // 0x76: 0x01 used, 0x10 effect started
};

// inferred: effect resource of a marker (0xC bytes; passed to fn_8023A680)
struct MarkerEfp_unk {
    unsigned char m00[0xC];
};

// inferred: markers and resources of the target lines (BattleTask+0x367B8..0x389C8); names invented
struct BattleMarkers_unk {
    // inferred: weak copy 0x800F8538; name invented
    BattleMarker_unk* fn_800F8538(int i) { return &mItems[i]; }
    BattleMarker_unk mItems[72];
    // inferred: effects of the line (0xC bytes each): 0/1 base and head toward an enemy, 2/3 appearance,
    // 4/5 base and head toward the own side
    MarkerEfp_unk mEfp[6];     // 0x38978
    int mHideLines;            // 0x389C0: inferred: nonzero hides the lines (sets mode 1)
    unsigned char mTex[4];     // 0x389C4: inferred: texture of the line (DrawHelper::fn_80064F00)
};

// inferred: player character as seen from here (as game/CharaTask.cpp)
struct Player_unk : public BattleCharaTask {
    // inferred: weak copy 0x800F7FB4 (name invented)
    int GetM146C_unk() { return m146C; }
};

// inferred: the "Hate" block of the game config (from 0x350); names as in GameConfig_unk
struct HateConfig_unk {
    int mTargetedNumDefault;       // 0x00
    int mHideDown;                 // 0x04
    int mTargetSightDown;          // 0x08
    int mHateRatioDown;            // 0x0C
    int mHateLineFadeout;          // 0x10
    unsigned int mLineColorNormal; // 0x14
    unsigned int mLineColorSword;  // 0x18
    unsigned int mLineColorArcher; // 0x1C
    unsigned int mLineColorMagic;  // 0x20
};

// inferred: segment clipped by the frustum (empty constructor: weak copy 0x800F8544); names invented
struct LineSeg_unk {
    LineSeg_unk() {}
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};

// inferred: vertex of the texture commands (0x18, as atn/DrawHelper.cpp)
namespace atn {
struct DrawTexVertex_unk {
    DrawTexVertex_unk() {}
    Vector3 mPos;
    unsigned int mColor;
    Vec2_unk mUV;
};
} // namespace atn

extern "C" PlayerManager* fn_8000D9E8(); // inferred: player manager (weak copy of another file)
// inferred: first player of the list (weak copy of another file)
inline PlayerTask* PlayerManager::GetHead_unk() {
    return mHead;
}
extern "C" const char* const lbl_80735DD0[]; // inferred: sound names of the markers (.rodata of this file)
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

// inferred: weak copies of game/BattleTask.cpp (C names, called out of line by other files too)
extern "C" inline void* fn_800F7F90() {
    return lbl_8087F610;
}
extern "C" inline void fn_800F7F98(atn::EffectManager* mgr, int replaying) {
    mgr->mSystem.mReplaying = replaying;
}
extern "C" inline atn::EffectManager* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline void fn_800F7FA8(atn::EffectManager* mgr, void* owner, int group) {
    fn_80232B7C((unsigned int)owner, group);
}
// inferred: center of the collision (the player keeps its own copy)
extern "C" inline const atn::Vector3* fn_800F7FBC(BattleCharaTask* chara) {
    if (chara->mFlagC31) {
        return &chara->m148C;
    }
    return &chara->mColli.m48;
}
// inferred: radius of the collision
extern "C" inline float fn_800F7FF8(BattleCharaTask* chara) {
    if (chara->mFlagC31) {
        return chara->m1498;
    }
    return chara->m5B0;
}
// inferred: colour of a 0xAARRGGBB value
extern "C" inline atn::Color fn_800F8014(unsigned int rgba) {
    return atn::Color(0.003921569f * (float)((rgba >> 16) & 0xFF), 0.003921569f * (float)((rgba >> 8) & 0xFF),
                      0.003921569f * (float)(rgba & 0xFF), 0.003921569f * (float)(rgba >> 24));
}
extern "C" inline HateConfig_unk* fn_800F8098(GameConfig_unk* config) {
    return (HateConfig_unk*)&config->mTargetedNumDefault;
}
extern "C" inline const Frustum_unk* fn_800F80A0(atn::Camera* camera) {
    return &camera->mFrustum;
}
// inferred: translation matrix by value
extern "C" inline atn::Matrix3x4 fn_800F80A8(const atn::Vector3* pos) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, pos->x, pos->y, pos->z);
    return m;
}
// inferred: mtx = mtx * rot_z * rot_y * rot_x
extern "C" inline void fn_800F80B8(MtxPtr mtx, const atn::Vector3* rot) {
    ((atn::Matrix3x4*)mtx)->RotateZYX_unk(*rot);
}
// inferred: mtx = mtx * scale
extern "C" inline void fn_800F8290(MtxPtr mtx, float x, float y, float z) {
    Mtx s;
    PSMTXScale(s, x, y, z);
    Mtx t;
    PSMTXConcat(mtx, s, t);
    __MtxCopy(mtx, t);
}
extern "C" inline void fn_800F831C(atn::DrawTexVertex_unk* v, const atn::Vector3* pos, unsigned int color,
                                   const atn::Vec2_unk* uv) {
    v->mPos = *pos;
    v->mColor = color;
    v->mUV = *uv;
}
extern "C" inline atn::Model* fn_800F84C8(CharaTask* chara) {
    return (atn::Model*)&chara->mMotion;
}
extern "C" inline void fn_800F84D0(atn::EffectManager* mgr, int layer) {
    mgr->mSystem.mLayer = layer;
}
// inferred: operators of atn::Vec2_unk (inlined; names as the Vector3 ones)
inline atn::Vec2_unk operator-(const atn::Vec2_unk& a, const atn::Vec2_unk& b) {
    return atn::Vec2_unk(a.x - b.x, a.y - b.y);
}
inline atn::Vec2_unk operator*(const atn::Vec2_unk& a, float t) {
    return atn::Vec2_unk(a.x * t, a.y * t);
}
inline atn::Vec2_unk operator+(const atn::Vec2_unk& a, const atn::Vec2_unk& b) {
    return atn::Vec2_unk(a.x + b.x, a.y + b.y);
}
extern "C" inline atn::Vec2_unk fn_800F84D8(const atn::Vec2_unk& a, const atn::Vec2_unk& b, float t) {
    return (b - a) * t + a;
}
extern "C" inline float fn_800F8524(const float* a, const float* b, float t) {
    return t * (*b - *a) + *a;
}
extern "C" inline GameConfig_unk* fn_800F52F8() {
    return lbl_8087F0A8;
}
extern "C" inline int fn_800F5300(CharaTask* chara) {
    return chara->mFlag28;
}
extern "C" inline CharaC58_unk* fn_800F530C(CharaTask* chara) {
    return &chara->mC58;
}
extern "C" inline atn::DrawHelper* fn_800F7250() {
    return lbl_8087EEB0;
}
inline atn::Camera* atn::RenderManager::fn_800F7258() {
    return &mCamera;
}

// DRAFT: written for the weak copies; the drawing of the target line is incomplete
// NONMATCHING: draft - draws the target line of `chara` (base and head models, the line through the
// clipped segment and the appearance animation)
void BattleTask::fn_800F72F4(BattleCharaTask* chara, int arg) {
    if (chara->fn_800EE180() == 0) {
        return;
    }
    CharaC58_unk* info = fn_800F530C(chara);
    CharaTask* target = info->mC4;
    if (target == 0 || target->IsPartyMember_unk()) {
        return;
    }
    int index = fn_80102890(chara);
    fn_80102890(target);
    BattleMarkers_unk* markers = (BattleMarkers_unk*)&m367B4[4];
    BattleMarker_unk* marker = markers->fn_800F8538(index);
    marker->mFlags |= 1;
    int changed = 0;
    int mode = 0;
    if (markers->mHideLines != 0 || (target->IsPlayer() && fn_800F5300(target) != 0) || fn_800F7F90() != 0) {
        mode = 1;
    } else if (fn_800F5300(fn_8000D9E8()->GetHead_unk()) != 0) {
        mode = 3;
    } else if (arg != 0) {
        mode = 2;
    }
    if (marker->mMode != mode) {
        changed = 1;
    }
    CharaTask* last = (CharaTask*)info->mC8;
    if (last != target && last != 0) {
        changed = 1;
    }
    if (changed) {
        fn_800F7F98(fn_800F7FA0(), 1);
        fn_80239DAC(fn_800F7FA0(), (unsigned int)marker, 0, 0);
        fn_800F7F98(fn_800F7FA0(), 0);
        marker->m70 = 12;
        marker->mFlags &= ~0x10;
    } else if (chara->vfE4()) {
        if ((marker->mFlags & 0x10) != 0x10) {
            fn_800F7F98(fn_800F7FA0(), 1);
            fn_800F7FA8(fn_800F7FA0(), marker, 0);
            fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&markers->mEfp[2], -1, 5, (int)marker, 0, 0, 0, 0, -1, 1, 1.0f);
            fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&markers->mEfp[3], -1, 5, (int)&marker->mMtx2, 0, 0, 0, 0, -1, 1, 2.0f);
            if (((Player_unk*)chara)->GetM146C_unk() == 0x12 || ((Player_unk*)chara)->GetM146C_unk() == 0x11) {
                fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&markers->mEfp[2], -1, 5, (int)marker, 0, 0, 0, 0, -1, 1, 1.0f);
            }
            fn_800F7F98(fn_800F7FA0(), 0);
        }
        marker->mFlags |= 0x10;
    } else {
        marker->mFlags &= ~0x10;
    }
    atn::Vector3 from(*fn_800F7FBC(chara));
    atn::Vector3 to(*fn_800F7FBC((BattleCharaTask*)target));
    atn::Vector3 dir = (to - from).GetNormalize();
    from += dir * fn_800F7FF8(chara);
    to -= dir * fn_800F7FF8((BattleCharaTask*)target) * 0.3f;
    dir = to - from;
    float scale = (dir.Length_unk() - 4.0f) / 50.0f;
    dir.Normalize_unk();
    atn::Vector3 rot = dir.ToRotation_unk();
    atn::Color color;
    switch (mode) {
    case 1:
        if (chara->GetStatus_unk()->fn_80134270()) {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorMagic);
        } else if (chara->GetStatus_unk()->fn_80134250()) {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorArcher);
        } else {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorSword);
        }
        if ((unsigned int)(((Player_unk*)chara)->GetM146C_unk() - 0x11) <= 1) {
            color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorMagic);
        }
        break;
    case 2:
        color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorMagic);
        break;
    default:
        color = fn_800F8014(fn_800F8098(fn_800F52F8())->mLineColorNormal);
        break;
    }
    marker->mMode = mode;
    float size = 1.0f;
    switch (((Player_unk*)chara)->GetM146C_unk()) {
    case 0x11:
        size = 1.5f;
        break;
    case 0x12:
    case 0x3B:
        size = 2.0f;
        break;
    }
    marker->m70--;
    if (markers->mHideLines == 0 && marker->m70 >= 0) {
        float t = (float)(marker->m70 - 6) / 6.0f;
        size = 0.0f > t ? 0.0f : (float)(marker->m70 - 6) / 6.0f;
        to = atn::Lerp_unk(to, marker->m60, size);
        dir = to - from;
        scale = (dir.Length_unk() - 4.0f) / 50.0f;
        scale = fn_800F8524(&scale, &marker->m6C, size);
        dir.Normalize_unk();
        rot = dir.ToRotation_unk();
        LineSeg_unk seg;
        LineSeg_unk clipped;
        seg.mStart = from;
        seg.mEnd = to;
        float len;
        if (fn_80050420(fn_800F80A0(fn_8008B964()->fn_800F7258()), &seg.mStart, &clipped.mStart)) {
            len = ((clipped.mEnd - clipped.mStart).Length_unk() - 4.0f) / 50.0f;
        } else {
            len = 0.001f;
        }
        scale = len;
        marker->mMtx = fn_800F80A8(&clipped.mStart);
        fn_800F80B8(marker->mMtx.m, &rot);
        fn_800F8290(marker->mMtx.m, 4.0f, 4.0f, scale);
        marker->mMtx2 = fn_800F80A8(&(to - dir * 4.0f));
        fn_800F80B8(marker->mMtx2.m, &rot);
        atn::DrawTexVertex_unk verts[7];
        fn_800F831C(&verts[0], &from, color.ToU32(), &atn::Vec2_unk(0.0f, 1.0f));
        int count = marker->m70 < 6 ? marker->m70 : 6;
        for (int i = 0; i < count; i++) {
            atn::DrawTexVertex_unk* v = &verts[i + 1];
            float f = (float)(count - i - 1) / 6.0f;
            v->mPos = atn::Lerp_unk(to, marker->m60, f);
            v->mPos += (v->mPos - from).GetNormalize() * (4.0f * (1.0f - f * f));
            verts[i].mColor = color.ToU32();
            v->mUV = fn_800F84D8(atn::Vec2_unk(1.0f, 0.0f), atn::Vec2_unk(0.0f, 0.0f), (float)i / (float)count);
        }
        fn_800F7250()->fn_80064F00(verts, count + 1, 0xA0, 0, (int)markers->mTex, 0.0f);
        if (marker->m70 == 8) {
            if (info->mC4->IsPlayer()) {
                if (fn_800F5300(info->mC4)) {
                    fn_800C31F4(lbl_80735DD0[0], 0, -1, 2.0f);
                } else {
                    fn_800C31F4(lbl_80735DD0[0], 0, -1, 1.0f);
                }
            } else {
                fn_800C31F4(lbl_80735DD0[1], 0, -1, 1.0f);
            }
        }
        if (fn_800F7F90() != 0 && marker->m70 == 1) {
            fn_801065F4(target, fn_800F84C8(target), (const ColorF_unk*)&color);
        }
    } else {
        float last = scale;
        LineSeg_unk seg;
        LineSeg_unk clipped;
        seg.mStart = from;
        seg.mEnd = to;
        if (fn_80050420(fn_800F80A0(fn_8008B964()->fn_800F7258()), &seg.mStart, &clipped.mStart)) {
            scale = ((clipped.mEnd - clipped.mStart).Length_unk() - 4.0f) / 50.0f;
        } else {
            scale = 0.001f;
        }
        marker->mMtx = fn_800F80A8(&clipped.mStart);
        fn_800F80B8(marker->mMtx.m, &rot);
        if (markers->mHideLines != 0) {
            fn_800F8290(marker->mMtx.m, 3.0f, 3.0f, scale);
        } else {
            fn_800F8290(marker->mMtx.m, size, size, scale);
        }
        marker->mMtx2 = fn_800F80A8(&(to - dir * 4.0f));
        fn_800F80B8(marker->mMtx2.m, &rot);
        marker->m60 = to;
        marker->m6C = last;
    }
    if (fn_80239D58(fn_800F7FA0(), marker, 0) == 0) {
        MarkerEfp_unk* res = (mode == 1 || mode == 3) ? &markers->mEfp[4] : &markers->mEfp[0];
        fn_800F7F98(fn_800F7FA0(), 1);
        fn_800F84D0(fn_800F7FA0(), 5);
        fn_800F7FA8(fn_800F7FA0(), marker, 0);
        fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&res[0], -1, 5, (int)marker, 0, 0, (const EffectColor_unk*)&color, 0, -1, 1, 1.0f);
        fn_8023A680(fn_800F7FA0(), (EffectRes_unk*)&res[1], -1, 5, (int)&marker->mMtx2, 0, 0, (const EffectColor_unk*)&color, 0, -1, 1,
                    1.0f);
        fn_800F84D0(fn_800F7FA0(), 0);
        fn_800F7F98(fn_800F7FA0(), 0);
    }
}

extern "C" void* fn_800FAB80(BattleTask* mgr, CharaTask* chara, HitInfo_unk* data, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int time, int b, int c, float f1, float f2);

// inferred: fires a bullet of `chara` on behalf of `user` and returns it (SkillEffectManager)
extern "C" void* fn_800FBA30(BattleTask* mgr, CharaTask* chara, CharaTask* user, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, const atn::Vector3* rot) {
    mgr->m3E398 = user;
    void* bullet = fn_800FAB80(mgr, chara, data, id, pos, rot, 0, 30, -1, -1, 0.0f, 1.0f);
    mgr->m3E398 = 0;
    return bullet;
}

void BattleTask::fn_800FDE60(int effect, CharaTask* chara, const atn::Vector3& dir) {
    fn_800E2A24(&m38A5C);
    m38A5C.m00 = effect;
    m38A5C.m04 = chara;
    m38A5C.m08 = fn_80219E6C(10000);
    m38A5C.m44 = 3;
    m38A5C.m28 = dir;
    m38A5C.m48 = 0;
    m38A5C.m4C = 0;
    m38A5C.m0C |= 0x10000;
    fn_800FDF14(&m38A5C);
}

int BattleTask::fn_80100BB8(CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range, float theta) {
    return 0;
}

void BattleTask::fn_80100BC0() {
    fn_801055A8(0);
    lbl_8087F3C0->fn_8023A614(5);
}

Bullet* BattleTask::fn_801010A0() {
    for (int i = 0; i < 256; i++) {
        if (!mBullets[i].HasFlag_unk(0x10000)) {
            return &mBullets[i];
        }
    }
    return 0;
}

HomingBullet* BattleTask::fn_801010E8() {
    for (int i = 0; i < 32; i++) {
        if (!mHomings[i].HasFlag_unk(0x10000)) {
            return &mHomings[i];
        }
    }
    return 0;
}

void BattleTask::fn_80101138() {
    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf14(0);
        }
    }
}

void BattleTask::fn_801011B8() {
    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf0C(0, 1);
        }
    }
    for (unsigned int i = 0; i < 32; i++) {
        if (mHomings[i].HasFlag_unk(0x10000)) {
            mHomings[i].vf0C(0, 1);
        }
    }
}

void BattleTask::fn_8010129C() {
    for (unsigned int i = 0; i < 256; i++) {
        mBullets[i].vf18();
    }
    for (unsigned int i = 0; i < 32; i++) {
        mHomings[i].vf18();
    }
}

// inferred: defined in this file (follows fn_8010129C, which calls it on every bullet)
void Bullet::vf18() {
    m04 &= ~0x10000;
}

int BattleTask::fn_80101340(CharaTask* chara) {
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000) && mBullets[i].HasFlag_unk(0x20000) && mBullets[i].m9C == chara) {
            count++;
        }
    }
    return count;
}

void BattleTask::fn_801021B4(RayObj_unk* obj, int a, int b) {
    m3E2E8 = obj;
    m3E2E0 = 0;
    m3E2E4 = 0;
    m3E2EC = b;
    m3E2F0 = a;
    if (obj != 0) {
        if ((obj->m08 & 2) == 2) {
            m3E2E0 = (int)obj->m0C;
        } else if ((obj->m08 & 8) == 8) {
            m3E2E4 = obj->m0C;
        }
    }
}

void BattleTask::fn_801027A4(CharaTask* chara) {
    if (FindChara_unk(chara) < 0) {
        mCharas[mCharaCount].mChara = chara;
        mCharaCount++;
    }
}

void BattleTask::fn_80102824(CharaTask* chara, PlayerTask* leader) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        mCharas[i].mChara = (CharaTask*)leader;
    }
}

int BattleTask::fn_80102890(CharaTask* chara) {
    if (chara == 0) {
        return -1;
    }
    for (int i = 0; i < mCharaCount; i++) {
        if (mCharas[i].mChara == chara) {
            return i;
        }
    }
    return -1;
}

void BattleTask::fn_801028E4(CharaTask* chara, CharaTask* other, int value) {
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b < 0) {
        return;
    }
    if (a >= 0) {
        mCharas[a].m08[b] = value;
    } else {
        for (int i = 0; i < 0x48; i++) {
            mCharas[i].m08[b] = value;
        }
    }
}

void BattleTask::fn_801031D0(CharaTask* chara, CharaTask* other, int all) {
    fn_80210220(lbl_8087F0A8->mParamIndex);
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b >= 0 && a >= 0) {
        BattleChara_unk* entry = &mCharas[a];
        if (entry->mChara != 0 && entry->mChara->CanAct_unk()) {
            entry->fn_8010895C(b, all);
        }
    }
}

void BattleTask::fn_8010337C(CharaTask* chara, CharaTask* other) {
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b < 0) {
        return;
    }
    if (a >= 0) {
        mCharas[a].m6E4[b] = 0.0f;
    } else {
        for (int i = 0; i < mCharaCount; i++) {
            mCharas[i].m6E4[b] = 0.0f;
        }
    }
}

// NONMATCHING: 99.5% - registers of the index of `other` and of the count base swapped
int BattleTask::fn_80103484(CharaTask* chara, CharaTask* other) {
    int b;
    int a = FindChara_unk(chara);
    b = FindChara_unk(other);
    if (b < 0) {
        return -1;
    }
    if (a >= 0) {
        BattleChara_unk* entry = &mCharas[a];
        if ((entry->mChara->mFlags & 1) == 1) {
            return -1;
        }
        return entry->fn_801083D8(b);
    }
    BattleChara_unk* entry = mCharas;
    int sum = 0;
    int count = 0;
    for (int i = 0; i < mCharaCount; i++) {
        if ((entry->mChara->mFlags & 1) != 1) {
            sum += entry->fn_801083D8(b);
            count++;
        }
        entry++;
    }
    if (count > 0) {
        return sum / count;
    }
    return -1;
}

// NONMATCHING: 99.3% - registers of the entry pointer and of the flags of the inlined CanAct_unk swapped
void BattleTask::fn_80103E60(CharaTask* target) {
    if (target == 0) {
        return;
    }
    CharaTask* c;
    BattleChara_unk* p;
    int count = 0;
    p = mCharas;
    for (int i = 0; i < mCharaCount; i++, p++) {
        c = p->mChara;
        if (c != 0 && c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000 && (c->mType == 0 || c->mC58.m00 != 0) &&
            c->mC58.mC4 == target) {
            count++;
        }
    }
    target->m102C = count;
}

BattleChara_unk* BattleTask::fn_80103F60(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        return &mCharas[i];
    }
    return 0;
}

void BattleTask::fn_80105510(int value) {
    if (value > 300) {
        if (m363BC < value) {
            m363BC = 9999;
        } else {
            m363BC = 300;
        }
    } else if (value < 20) {
        m363BC = 20;
    } else {
        m363BC = value;
    }
    if (lbl_8087F430 == 0) {
        return;
    }
    if (m363BC == 9999) {
        ((GameTask*)lbl_8087F430)->fn_80370320(10, 1, 0);
    } else {
        ((GameTask*)lbl_8087F430)->fn_80370320(10, 0, 0);
    }
}

void BattleTask::fn_801058F8() {
    fn_80103FD4();
    for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
        fn_80103600(player);
    }
}

void BattleTask::fn_80105950(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0[0], model);
}

void BattleTask::fn_801059F8(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)chara, 0, 1);
}

void BattleTask::fn_80105A08(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0[1], model);
}

void BattleTask::fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363C0[2], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105B3C(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)chara, 0, 0);
}

void BattleTask::fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363C0[3], model, 0, lbl_807C7030, lbl_807C7030, *color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105BD8(atn::Model* model) {
    lbl_8087F3C0->mSystem.m78 = 1;
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[25], model);
    lbl_8087F3C0->mSystem.m78 = 0;
}

void BattleTask::fn_80105C9C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[1], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[0], model);
    }
}

// NONMATCHING: 90% - the original loads the effect manager before the clamp of the level
void BattleTask::fn_80105DCC(CharaTask* chara, atn::Model* model, int level) {
    fn_80232B7C((unsigned int)chara, 4);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[5], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[2] + MIN((unsigned int)MAX(level, 0), 3u), model);
    }
}

void BattleTask::fn_80105F3C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 4, 1);
}

void BattleTask::fn_80105F4C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[7], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[6], model);
    }
}

void BattleTask::fn_8010607C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 3, 1);
}

void BattleTask::fn_8010608C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[9], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[8], model);
    }
}

void BattleTask::fn_8010652C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_8010653C(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 6);
    PlayEfp_unk(&mEfp38AF4[29], model);
}

void BattleTask::fn_801065E4(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 6, 1);
}

void BattleTask::fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color) {
    fn_80232B7C((unsigned int)chara, 7);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[30], model, 0, lbl_807C7030, lbl_807C7030, *(const EffectColor_unk*)color,
                -1, -1, 1, 1.0f);
}

void BattleTask::fn_80106680(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 8);
    PlayEfp_unk(&mEfp38AF4[31], model);
}

void BattleTask::fn_80106728(int arg, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[44], model);
}

void BattleTask::fn_801067D0(CharaTask* chara, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[45], model);
}

void BattleTask::fn_801070C8(int id, unsigned int owner, int group, atn::Model* model, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, float scale) {
    atn::EfpFile* efp = fn_800EFBC4(id);
    if (efp != 0) {
        fn_80232B7C(owner, group);
        fn_8023A8B4(lbl_8087F3C0, efp, model, 0, *pos, *rot, *color, -1, -1, 1, scale);
    }
}

void BattleTask::fn_80107168(int id, unsigned int owner, int group, atn::Model* model, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, float scale) {
    atn::EfpFile* efp = fn_800EFDA0(id);
    if (efp != 0) {
        fn_80232B7C(owner, group);
        fn_8023A8B4(lbl_8087F3C0, efp, model, 0, *pos, *rot, *color, -1, -1, 1, scale);
    }
}

void BattleTask::fn_80107208(int id, const atn::Vector3* pos, float scale) {
    const char* name = fn_800EFE3C(id);
    if (name != 0) {
        fn_800C344C(name, pos, 0, -1, scale);
    }
}

int BattleTask::fn_80107274(CharaTask* chara) {
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara == chara) {
            return 0;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed == 0) {
            mSlots[i].mUsed = 1;
            mSlots[i].mChara = chara;
            return 1;
        }
    }
    return 0;
}

void BattleTask::fn_80107380(CharaTask* chara) {
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara == chara) {
            mSlots[i].mUsed = 0;
            mSlots[i].mChara = 0;
        }
    }
}

void BattleTask::fn_8010743C(const atn::Vector3* pos, int changed, float f) {
    m3E300 = 1;
    lbl_8087F3C0->mSystem.mReplaying = 1;
    if (changed != 0) {
        fn_80239DAC(lbl_8087F3C0, (unsigned int)&m389F4, 0, 0);
    }
    if (changed != 0 || !fn_80239D58(lbl_8087F3C0, &m389F4, 0)) {
        EffectColor_unk color = m3E304;
        color.a = m3E2FC;
        fn_80232B7C((unsigned int)&m389F4, 0);
        fn_8023A680(lbl_8087F3C0, (EffectRes_unk*)&m389F4, -1, 6, (int)pos, 0, 0, &color, 0, -1, 1, f / 15.0f);
    }
    lbl_8087F3C0->mSystem.mReplaying = 0;
}

void BattleTask::fn_80107574() {
    m3E300 = 2;
}

void BattleTask::fn_80107584() {
    lbl_8087F3C0->mSystem.mReplaying = 1;
    switch (m3E300) {
    case 1: {
        m3E2FC = MIN(1.0f, m3E2FC + 0.1f);
        EffectColor_unk color;
        color.r = m3E304.r;
        color.g = m3E304.g;
        color.b = m3E304.b;
        color.a = m3E2FC;
        lbl_8087F3C0->fn_8023A184((unsigned int)&m389F4, 0, &color);
        break;
    }
    case 2: {
        m3E2FC = MAX(0.0f, m3E2FC - 0.1f);
        EffectColor_unk color;
        color.r = m3E304.r;
        color.g = m3E304.g;
        color.b = m3E304.b;
        color.a = m3E2FC;
        lbl_8087F3C0->fn_8023A184((unsigned int)&m389F4, 0, &color);
        if (m3E2FC < 0.001f) {
            fn_80239DAC(lbl_8087F3C0, (unsigned int)&m389F4, 0, 0);
            m3E300 = 0;
        }
        break;
    }
    }
    lbl_8087F3C0->mSystem.mReplaying = 0;
}

void BattleTask::fn_801076D0(void* owner, atn::Model* model) {
    if (fn_80239D58(lbl_8087F3C0, owner, 20) == 0) {
        fn_80232B7C((unsigned int)owner, 20);
        PlayEfp_unk(&mEfp38AF4[16], model);
    }
}

void BattleTask::fn_80107798(void* owner, int flag) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 20, flag);
}

void BattleTask::fn_801077A8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 21);
    PlayEfp_unk(&mEfp38AF4[17], model);
}

void BattleTask::fn_80107850(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 21, 1);
}

void BattleTask::fn_80107860(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 22);
    PlayEfp_unk(&mEfp38AF4[19], model);
}

void BattleTask::fn_80107908(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 23);
    PlayEfp_unk(&mEfp38AF4[20], model);
}

void BattleTask::fn_801079B0(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 23, 1);
}

void BattleTask::fn_801079C0(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 24);
    PlayEfp_unk(&mEfp38AF4[22], model);
}

void BattleTask::fn_80107A68(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 24, 1);
}

void BattleTask::fn_80107A78(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 25);
    PlayEfp_unk(&mEfp38AF4[18], model);
}

void BattleTask::fn_80107B20(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 25, 1);
}

void BattleTask::fn_80107B30(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 26);
    PlayEfp_unk(&mEfp38AF4[23], model);
}

void BattleTask::fn_80107BD8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 26, 1);
}

void BattleTask::fn_80107BE8(void* owner, atn::Model* model) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80232B7C((unsigned int)owner, 27);
    PlayEfp_unk(&mEfp38AF4[24], model);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CA8(void* owner, int flag) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 27, flag);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CEC(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 28);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[25], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107D94(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 28, 1);
}

void BattleTask::fn_80107DA4(void* owner, atn::Model* model, float scale, const atn::Vector3& pos) {
    fn_80232B7C((unsigned int)owner, 29);
    EffectColor_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[26], model, 0, pos, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107E58(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 29, 1);
}

void BattleTask::fn_80107E68(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 30);
    PlayEfp_unk(&mEfp38AF4[21], model);
}

void BattleTask::fn_80107F10(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_80107F20(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 31);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[27], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107FC8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 31, 1);
}

void BattleTask::fn_80107FD8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 32);
    PlayEfp_unk(&mEfp38AF4[28], model);
}

void BattleTask::fn_80108080(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 32, 1);
}

// NONMATCHING: 99% - the "" literals of the allocations are in the string pool of the file (not complete yet)
BattleChara_unk::BattleChara_unk() : mHateValues(0), mHateIds(0), mHateCount(0) {
    int count = 10;
    if (lbl_8087F610 == 0) {
        count = 5;
    }
    mHateCount = count;
    mHateValues = new ((MemoryTag)3, "", "", 0) short[mHateCount];
    mHateIds = new ((MemoryTag)3, "", "", 0) short[mHateCount];
    mChara = 0;
    m92D = 0;
    m248 = 1.0f;
    m92E = 0;
    m92F = 0;
    fn_801081D8();
}

BattleChara_unk::~BattleChara_unk() {
    if (mHateValues != 0) {
        delete[] mHateValues;
        mHateValues = 0;
    }
    if (mHateIds != 0) {
        delete[] mHateIds;
        mHateIds = 0;
    }
    mHateCount = 0;
}

void BattleChara_unk::fn_801081D8() {
    for (int i = 0; i < 72; i++) {
        m08[i] = 0;
        m128[i] = 1.0f;
        m48C[i] = 0;
        m5AC[i] = 0;
        m36C[i] = 0;
        m24C[i] = 0.0f;
        m80C[i] = 0;
        m6E4[i] = 0.0f;
    }
    for (int i = 0; i < mHateCount; i++) {
        mHateValues[i] = 0;
        mHateIds[i] = -1;
    }
    m6CC = (int)(70.0f * (rand() / 32767.0f));
    m6DC = 0;
    m6E0 = 0;
    m804 = 0;
    m808 = 0;
    m92F = 0;
    m930 = 0;
}

int BattleTask::fn_80108378(CharaTask* chara) {
    BattleTask* battle = lbl_8087F048;
    if (battle != 0) {
        if (chara == 0) {
            return -1;
        }
        for (int i = 0; i < battle->mCharaCount; i++) {
            if (battle->mCharas[i].mChara == chara) {
                return i;
            }
        }
        return -1;
    }
    return -1;
}

void BattleChara_unk::fn_801088B8(int id, int value) {
    int n = 0;
    for (int i = 0; i < mHateCount; i++) {
        if (id == mHateIds[i]) {
            n++;
        }
    }
    if (n >= 10) {
        return;
    }
    for (int i = 0; i < mHateCount - 1; i++) {
        mHateIds[i] = mHateIds[i + 1];
        mHateValues[i] = mHateValues[i + 1];
    }
    mHateIds[mHateCount - 1] = id;
    mHateValues[mHateCount - 1] = value;
}

void BattleChara_unk::fn_8010895C(int id, int all) {
    for (int i = 0; i < mHateCount; i++) {
        if (id == mHateIds[i]) {
            mHateIds[i] = -1;
            mHateValues[i] = 0;
            if (all == 0) {
                break;
            }
        }
    }
    int skip = 1;
    for (int i = 0; i < mHateCount; i++) {
        int hateId = mHateIds[i];
        if (hateId >= 0 && mHateValues[i] > 0) {
            skip = 0;
        }
        if (skip == 0 && (hateId < 0 || mHateValues[i] == 0)) {
            for (int j = i + 1; j < mHateCount; j++) {
                mHateIds[j - 1] = mHateIds[j];
                mHateValues[j - 1] = mHateValues[j];
            }
        }
    }
}

void BattleTask::fn_80108A54(CharaTask* chara) {
    fn_80108A5C(chara, &chara->mRot);
}

// NONMATCHING: 99.5% - the config pointer is allocated below the parameters in the original
void BattleTask::fn_80108A5C(CharaTask* chara, const atn::Vector3* dir) {
    CharaTask* target = chara->mFC0;
    GameConfig_unk* config = lbl_8087F0A8;
    if (target != 0) {
        if (!target->CanAct_unk()) {
            target = 0;
        } else {
            atn::Vector3 d = target->mPos - chara->mPos;
            if (d.LengthSq_unk() > config->mLockOnReleaseLen * config->mLockOnReleaseLen &&
                !(target->m55C == 6 && target->m560 == 30)) {
                target = 0;
            }
        }
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0 && lbl_8087F430->GetCurrentMap()->mId == 0x3F &&
        lbl_8087F430->GetCurrentMap()->mSubId == 2) {
        target = 0;
    }
    if (target == 0) {
        CharaTask* found = fn_800FFE68(chara, dir, 0, 1, config->mLockOnLen, config->mLockOnTheta);
        if (found != 0) {
            target = found;
        }
    }
    chara->mFC0 = target;
}

// inferred: the early return as !(a) || !(b) gives the bne over a blr of the original
void BattleTask::fn_80109828(const wchar_t* text) {
    if (!(lbl_8087F0A8->m06C > 0) || !(LogMessageTask::sInstance != 0)) {
        return;
    }
    if (text != 0) {
        LogMessageTask::sInstance->fn_8047961C(text, -0x3301, 0, 0);
    }
}

// inferred: camp message table (lbl_8087F1E4, as in game/CosEditItemList_unk.cpp)
struct BattleMsg_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" BattleMsg_unk* lbl_8087F1E4;
extern "C" bool fn_8021BE98(wchar_t* buf, SpEffect_unk* effect); // inferred: text of a status effect
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf

// inferred: log message "<character> <effect>" (format 0x8B of the message table)
void BattleTask::fn_80109864(SpEffect_unk* effect, CharaTask* chara) {
    if (!(lbl_8087F0A8->m06C > 0) || !(LogMessageTask::sInstance != 0)) {
        return;
    }
    wchar_t text[0x100];
    memset(text, 0, sizeof(text));
    const wchar_t* format = lbl_8087F1E4->Get_unk(0x8B);
    wchar_t name[0x100];
    memset(name, 0, sizeof(name));
    if (fn_8021BE98(name, effect)) {
        fn_800DD3FC(text, format, chara->m60->m04, name);
    }
    if (text[0] != 0) {
        LogMessageTask::sInstance->fn_8047961C(text, -0x333334, 2, 0);
    }
}

// inferred: the 0.0f bound goes through a variable (MAX evaluates it twice); with the literal f2/f3 come out swapped
void BattleTask::fn_8010AB34() {
    float add = m38A28;
    float zero;
    m38A04 = MAX(zero = 0.0f, MIN(m38A04 + add, m38A0C));
}

// inferred: as fn_8010AB34
void BattleTask::fn_8010AB94() {
    float add = m38A2C;
    float zero;
    m38A04 = MAX(zero = 0.0f, MIN(m38A04 + add, m38A0C));
}

void BattleTask::fn_8010ABF4() {
    m38A04 = 0.0f;
    fn_8010ACCC();
    m38CB8 = m38C90;
    fn_8010AEF4();
}

void BattleTask::fn_8010AC60() {
    m38CB8.m00 = 0;
    m38CB8.m04 = 0;
    m38CB8.m08 = 0;
    m38CB8.m0C = 0;
    m38CB8.m10 = 0;
    BattleChara_unk* p = mCharas;
    for (int i = 0; i < mCharaCount; i++) {
        CharaTask* c = p->mChara;
        if (c != 0 && c->mC58.mD0 != 0) {
            c->mC58.mD0 = 0;
        }
        p->m92E = 0;
        p++;
    }
    fn_80105510(40);
}

int BattleTask::fn_8010B180(CharaTask* a, CharaTask* b) {
    if (a == 0 || b == 0 || lbl_8087F430->fn_80376210() == 0) {
        return 1;
    }
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (scene == 0) {
        return 1;
    }
    int areaA = a->mC58.mB4;
    int areaB = b->mC58.mB4;
    if (areaA > 0 && areaB > 0 && areaA != areaB) {
        gs::AiArea* area = scene->fn_803C1884(areaA);
        if (area != 0 && area->m28 >= 0 && area->m28 != areaB) {
            return 0;
        }
    }
    return 1;
}

CharaTask* BattleTask::fn_8010C948(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i < 0) {
        return 0;
    }
    int best = 0;
    int found = -1;
    int* row = m391E0[i];
    for (int j = 0; j < mCharaCount; j++) {
        if (best < row[j]) {
            best = row[j];
            found = j;
        }
    }
    if (found < 0 || mCharaCount <= found) {
        return 0;
    }
    return mCharas[found].mChara;
}

float BattleTask::fn_8010CA24() {
    return 2.4f;
}

float BattleTask::fn_8010CA2C() {
    return 0.15f;
}

float BattleTask::fn_8010CA34() {
    return 1.5f;
}

float BattleTask::fn_8010CA3C(CharaTask* chara, float rate) {
    rate += chara->mStatus.fn_80134168(27, -1) / 100.0f;
    if (chara->mStatus.fn_80134134(46) != 0) {
        rate *= 0.7f;
    }
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= 1.5f;
        if (chara->mType == 2 && chara->mFlag5) {
            rate *= 1.5f;
        }
    }
    if (chara->mType == 0) {
        rate = MIN(2.2f, rate);
    }
    return rate;
}

float BattleTask::fn_8010CB2C(CharaTask* chara, float rate) {
    rate *= 1.0f + chara->mStatus.mParamB.m94 / 2000.0f;
    if (chara->mType == 0 || chara->mType == 3) {
        rate *= chara->fn_801789D8(0x4FBF, 0) != 0 ? 2.0f : 1.4f;
    }
    rate *= chara->mStatus.fn_8012DB04();
    rate += rate * chara->mStatus.fn_80134168(0x2B, -1) / 100.0f;
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= 2.4f;
    }
    if (chara->mType == 0 || chara->mType == 3) {
        PlayerTask* leader;
        if ((lbl_8087F430 != 0 && lbl_8087F430->fn_803761BC() == 0) ||
            (lbl_8087F8A0 != 0 && (leader = lbl_8087F8A0->mHead) != 0 &&
             (leader->mFlag28 || leader != lbl_8087F8A0->fn_8054A33C(0)))) {
            rate *= 2.0f;
        }
        if (chara->mType == 0 && lbl_8087F610 != 0) {
            rate *= 1.4f;
        }
    }
    return rate;
}

float BattleTask::fn_8010CCD0() {
    return 0.75f;
}

float BattleTask::fn_8010CCD8(CharaTask* chara) {
    if (chara == 0 || chara->mType == 2) {
        return 0.0f;
    }
    float gain = 2.0f * m38A48;
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0 && leader->mFlag28) {
        gain *= 1.5f;
    }
    return gain;
}

// inferred: random roll of the status effect `kind` (2/3, 7/8, 4 and 5); names invented
int BattleTask::fn_8010CD3C(int kind, CharaTask* chara, CharaTask* other) {
    switch (kind) {
    case 2:
    case 3:
        if (chara != 0 && chara->m55C != 6 && chara->mStatus.fn_801346C8(kind, 0) && rand() % 100 < 15) {
            return 1;
        }
        break;
    case 4:
        if (chara != 0 && other != 0 && chara->mStatus.fn_801346C8(4, 0) &&
            (other->m5C != 0 ? other->m5C->mD0 : 0) > 0 && rand() % 100 < 5) {
            return 1;
        }
        break;
    case 7:
    case 8:
        if (chara != 0 && (chara->m55C == 1 || chara->m55C == 2) && (unsigned int)chara->m5C->m11C <= 2 &&
            (chara->mFlagB16 || rand() % 100 < 15)) {
            return 1;
        }
        break;
    case 5:
        if (chara != 0 && chara->mStatus.fn_801346C8(5, 0) && rand() % 100 < 30) {
            return 1;
        }
        break;
    }
    return 0;
}

int BattleTask::fn_8010D770(HitInfo_unk* hit) {
    if (hit != 0) {
        int id = hit->m04;
        if (id == 0xCC || id == 0xD1 || id == 0xD5 || id == 0x5BC || id == 0x658) {
            return 1;
        }
    }
    return 0;
}

int BattleTask::fn_8010D7B4(HitInfo_unk* hit) {
    if (hit == 0) {
        return 0;
    }
    int id = hit->m04;
    if (id == 0x136 || id == 0x139 || id == 0x13C || id == 0x146 || id == 0x15A || id == 0x232C) {
        return 1;
    }
    return 0;
}

BattleTask::BetoModel::BetoModel() : mUsed(0), mModel(0x505), mChara(0) {
}

// inferred: the node takes the position of the node of the same name of the character, scaled by vf60
void BattleTask::BetoModel::fn_8010D854(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk,
                                        const atn::ModelNodeCallbackContext& context) {
    if (mChara != 0) {
        float scale = mChara->vf60();
        MtxPtr node = mChara->mMotion.GetNodeMtx_unk(chunk.mName);
        if (node != 0) {
            atn::Vector3 pos = ((atn::Matrix3x4*)node)->GetTrans_unk();
            mtx->fn_801479E4(pos);
            atn::Vector3 s(scale, scale, scale);
            atn::Matrix3x4 r;
            Mtx m;
            PSMTXScale(m, s.x, s.y, s.z);
            PSMTXConcat(mtx->m, m, r.m);
            *mtx = r;
        }
    }
}
