// Hit reactions: damage, guard, dodge, knockdown and getting up, revival, falling to death, guard
// broken and the reaction of the enemies (ControlDamageLastOne).
#include "chr/ControlDamage.h"
#include "chr/FuncPair_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/Lemon.h"
#include "atn/LoadHandle.h"
#include "atn/SoundRef.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/bind.hpp>

// inferred: arena mode object (lbl_8087F610, as in chr/ControlCharge.cpp)
struct Arena_unk {
    unsigned char m000[0x540];
    int m540;
};
extern "C" Arena_unk* lbl_8087F610;
extern "C" void fn_804DA690(Arena_unk* arena); // inferred: the player got up
// inferred: game state (lbl_8087EFA8, as in game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;
// inferred: field object (class unknown, as in gmk/LottaSummon.cpp)
struct Field_unk {
    void fn_803750E4(int id);
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id);                     // inferred: field flag
extern "C" void fn_80370320(Field_unk* field, int id, int arg2, int arg3); // inferred: starts an event
// inferred: effect manager of the characters (lbl_8087F048)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_800FDE60(Effect_unk* mgr, int arg, CharaTask* chara, const atn::Vector3& dir);
extern "C" void fn_8010A308(Effect_unk* mgr, const atn::Vector3* from, const atn::Vector3* to, float radius);
// inferred: RGBA colour (names invented)
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_801070C8(Effect_unk* effect, int id, int a, int b, chr::Motion_unk* motion,
                            const atn::Vector3* pos, const atn::Vector3* rot, const Color4f_unk& color, float scale);
extern "C" void fn_80107208(Effect_unk* effect, int id, const atn::Vector3* pos, float scale);
extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" void fn_803EA77C(void* voice, CharaTask* chara, int id, int arg, float volume, float arg2);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void* lbl_8087EE98; // inferred: collision world
// inferred: collision world (out-of-line copy 0x801404F8 in game/CharaTask.cpp)
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* axb);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length (PSVECSquareMag)
extern "C" double cos(double x);
extern "C" int rand();
// inferred: lens of a camera (as in game/CharaTask.cpp); names invented
struct Lens_unk {
    unsigned char m000[0x158];
    float m158;
    float m15C;
    float m160;
};
struct CameraLens_unk {
    unsigned char m000[0x2FC];
    Lens_unk* m2FC;
};
extern "C" void* lbl_8087EFB4;

inline Lens_unk* CharaTask::GetLens_unk() {
    return (mMotion.m1DC == 0) ? ((CameraLens_unk*)lbl_8087EFB4)->m2FC : mMotion.m1DC;
}

// inferred: inline in the original (weak copy 0x8013C3B4, defined out of line in game/CharaTask.cpp)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}

// inferred: facing direction of the rotation `rot` (name invented, as in chr/ControlCdnStone.cpp)
inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', rot.y);
    PSMTXMultVec(mtx, &v, &v);
    return v;
}

// inferred: a x b by value (name invented, as in chr/ControlChaseCaught.cpp)
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: sounds of the knockdown and of the broken guard
static const char* const sSe[4] = {"SE_BTCOM_016", "SE_BTCOM_017", "SE_BTCOM_018"};

namespace chr {

ControlDamage::ControlDamage(Chara_unk* chara, const atn::Vector3& dir, int motion) : ControlBase(chara) {
    chara->m560 = 0x16;
    Motion_unk* model = &mChara->mMotion;
    atn::Vector3 n = dir;
    PSVECNormalize(&n, &n);
    bool front = false;
    atn::Vector3 push = dir;
    push.y *= 0.3f;
    CharaTask* c = mChara;
    c->m6B8 = push;
    atn::Vector3 facing = FrontDir_unk(mChara->mRot);
    float dot = PSVECDotProduct(&n, &facing);
    if (dot > (float)cos(0.5235988f)) {
        m08 = n.ToRotation_unk();
        front = true;
    } else {
        m08 = (-n).ToRotation_unk();
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    if (front) {
        motion = 0x37;
    } else if (motion == -1) {
        if (fn_805F9920(&dir) > 25.0f) {
            motion = 0x36;
        } else {
            int r = rand() % 2;
            motion = 0x34;
            if (r == 0) {
                motion = 0x33;
            }
        }
    } else if (fn_805F9920(&dir) > 25.0f) {
        if (motion == 0x33) {
            motion = 0x36;
        } else if (motion == 0x34) {
            motion = 0x36;
        }
    }
    if (!model->fn_80097D40(motion)) {
        motion = 0x33;
    }
    model->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
    model->mTracks[0].m0C = 1.0f;
    if (lbl_8087F610) {
        model->mTracks[0].m0C = 1.2f;
    }
    m14 = 2.0f * model->fn_80097D7C(0);
    if (lbl_8087F498 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 6, 0, 1.0f, 10.0f);
    }
}

int ControlDamage::vf0C() {
    int done = 0;
    float dt = lbl_8087EFA8->m3A4;
    m14 -= dt;
    Motion_unk* model = &mChara->mMotion;
    float frame = model->mTracks[0].m08;
    if ((frame >= model->fn_80097D7C(0) && mChara->m574.y > -0.01f) || m14 <= 0.0f) {
        done = 1;
    }
    mChara->fn_8013CB68(m08, 0, 0.0f, mChara->m568);
    return done;
}

ControlGuard::ControlGuard(Chara_unk* chara, float f, const atn::Vector3& dir, int a) : ControlBase(chara), m14(a) {
    chara->m560 = 0x14;
    atn::Vector3 push = dir * f;
    push.y *= 0.0f;
    CharaTask* c = mChara;
    c->m6B8 = push;
    Motion_unk* model = &c->mMotion;
    mChara->fn_801446F0();
    if (fn_805F9920(&dir) > 0.0001f) {
        m08 = (-dir).GetNormalize().ToRotation_unk();
    } else {
        m08 = mChara->mRot;
    }
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->fn_80097C08(0, mChara->m484[26], 0, 0, 1, 0.0f, 0.1f);
    model->mTracks[0].m0C = 1.0f;
    m18 = 2.0f * model->fn_80097D7C(0);
}

int ControlGuard::vf0C() {
    int done = 0;
    float dt = lbl_8087EFA8->m3A4;
    m18 -= dt;
    Motion_unk* model = &mChara->mMotion;
    float frame = model->mTracks[0].m08;
    if ((frame >= model->fn_80097D7C(0) && mChara->m574.y > -0.01f) || m18 <= 0.0f) {
        done = 1;
    }
    mChara->fn_8013CB68(m08, 0, 0.0f, mChara->m568);
    return done;
}

ControlAvoid::ControlAvoid(Chara_unk* chara, const atn::Vector3& dir) : ControlBase(chara) {
    chara->m560 = 0x40;
    Motion_unk* model = &mChara->mMotion;
    mChara->fn_801446F0();
    if (fn_805F9920(&dir) > 0.0001f) {
        m08 = (-dir).GetNormalize().ToRotation_unk();
    } else {
        m08 = FrontDir_unk(mChara->mRot);
    }
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
    model->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

int ControlAvoid::vf0C() {
    int done = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0) && mChara->m574.y > -0.01f) {
        done = 1;
    }
    mChara->fn_8013CB68(m08, 0, 0.0f, mChara->m568);
    return done;
}

// NONMATCHING: 99.95% - r25/r26 swapped for the path of the motion and the temporary string.
ControlKnockdown::ControlKnockdown(Chara_unk* chara, const atn::Vector3& dir, int motion, int arg4)
    : ControlBase(chara), m14(0), m18(0), m1C(999999.0f), m20(arg4), m38(1), m3C(1) {
    bool held = false;
    if (chara->m560 == 0x72) {
        held = true;
    }
    chara->m560 = 0x17;
    atn::Vector3 push = dir;
    push.y *= 0.3f;
    CharaTask* c = mChara;
    c->m6B8 = push;
    Motion_unk* model = &c->mMotion;
    m08 = (-push).GetNormalize().ToRotation_unk();
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    if (motion >= 0 && !model->fn_80097D40(motion)) {
        motion = -1;
    }
    if (motion < 0) {
        int r = rand() % 3;
        if (held) {
            model->fn_80097C08(0, 0x30, 0, 0, 1, 0.0f, 0.1f);
        } else if (mChara->mStatus.IsState_unk(0x20)) {
            if (model->fn_80097D40(r + 0x2E)) {
                model->fn_80097C08(0, r + 0x2E, 0, 0, 1, 0.0f, 0.1f);
            } else {
                model->fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
            }
        } else {
            if (model->fn_80097D40(r + 0x39)) {
                model->fn_80097C08(0, r + 0x38, 0, 0, 1, 0.0f, 0.1f);
            } else {
                model->fn_80097C08(0, 0x38, 0, 0, 1, 0.0f, 0.1f);
            }
        }
    } else {
        model->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
        m38 = 0;
    }
    if (mChara->m50 == 0x30E0D) {
        model->mTracks[0].m0C = 3.0f;
    } else {
        model->mTracks[0].m0C = 1.0f;
    }
    if (model->mTracks[0].m00 == 0x3A || model->mTracks[0].m00 == 0x30) {
        mChara->fn_80176DFC(1, 0, 0);
    }
    atn::LoadHandle* handle = model->fn_80097D40(model->mTracks[0].m00);
    if (handle) {
        const char* path = handle->fn_80473F18();
        std::string name = fn_8006B174(std::string(path));
        if (name == "na000_dma03a_00.motion") {
            m1C = 20.0f;
        } else if (name == "na000_dma03a_01.motion") {
            m1C = 40.0f;
        } else if (name == "na000_dma03a_03.motion") {
            m1C = 35.0f;
        }
    }
    if (lbl_8087F498 && !mChara->mFlag2) {
        if (mChara->mStatus.IsState_unk(0x20)) {
            fn_803EA77C(lbl_8087F498, chara, 0xA, 0, 1.0f, 10.0f);
            if (mChara->mType == 0 && lbl_8087F430 && !fn_80370174(lbl_8087F430, 0xB6)) {
                fn_80370320(lbl_8087F430, 0xB6, 1, 0);
            }
        } else if (rand() % 2 == 0) {
            fn_803EA77C(lbl_8087F498, chara, 8, 0, 1.0f, 10.0f);
        }
    }
}

// NONMATCHING: 98.1% - the inlined fn_801AE41C loads mChara again (the original keeps a second
// model pointer), the lens accessor reloads m1DC three times, and r3/r4 are swapped in the
// arguments of the two sounds.
int ControlKnockdown::vf0C() {
    int result = 0;
    CharaTask* chara = mChara;
    Motion_unk* model = &chara->mMotion;
    if (fn_801AE41C()) {
        mChara->fn_80176DFC(0, 0, 0);
        result = 1;
    }
    switch (m18) {
    case 0:
        if (model->mTracks[0].m08 > m1C) {
            CharaTask* c = mChara;
            c->m6B8 = atn::Vector3(0.0f, 0.0f, 0.0f);
        }
        if (m38 && model->fn_80097D40(0x3D)) {
            atn::Vector3 from = mChara->mPos;
            from.y += mChara->m620;
            atn::Vector3 to(0.0f, 0.0f, 2.0f + mChara->m620);
            Mtx mtx;
            PSMTXRotRad(mtx, 'y', 3.1415927f + m08.y);
            PSMTXMultVec(mtx, &to, &to);
            to += from;
            CollResult_unk res;
            if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
                atn::ColliBase* colli = (atn::ColliBase*)res.m38;
                if ((colli->mFlags & 8) == 8 && m20) {
                    GimmickCommon* gimmick = (GimmickCommon*)colli->m0C;
                    if (gimmick->m50 == 1 && gimmick->vf5C()) {
                        GimmickMsg_unk msg;
                        msg.mType = 4;
                        gimmick->vf40(&msg);
                    }
                }
                if ((res.m3C & 4) != 4) {
                    model->fn_80097C08(0, 0x3F, 0, 0, 1, 0.0f, 0.1f);
                    m14 = 0;
                    m18 = 1;
                    if (mChara->m5B0 < 10.0f) {
                        fn_800C344C(sSe[1], &mChara->mPos, 0, -1, 1.0f);
                    } else {
                        fn_800C344C(sSe[2], &mChara->mPos, 0, -1, 1.0f);
                    }
                }
            }
        }
        m14++;
        break;
    case 1: {
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            model->fn_80097C08(0, 0x40, 1, 0, 1, 0.0f, 0.1f);
            m18 = 2;
        } else {
            atn::Vector3 v = mChara->m574;
            v.y = 0.0f;
            mChara->m574 = v;
        }
        break;
    }
    case 2:
        if (mChara->m574.y > -0.1f) {
            atn::Vector3 from = mChara->mPos;
            atn::Vector3 to = mChara->mPos;
            from.y += 3.0f;
            to.y -= 3.0f;
            if (fn_8004ED34(fn_801404F8(), 0, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
                model->fn_80097C08(0, 0x3D, 1, 0, 1, 0.0f, 0.1f);
                m14 = 0;
                m18 = 3;
            }
        }
        m14++;
        break;
    case 3:
        m14++;
        break;
    }
    if (m3C) {
        int id = model->mTracks[0].m00;
        float frame = model->mTracks[0].m08;
        bool dust = false;
        if (id == 0x2E || id == 0x38) {
            if (frame >= 12.0f) {
                dust = true;
            }
        } else if (id == 0x2F || id == 0x39) {
            if (frame >= 26.0f) {
                dust = true;
            }
        } else if (id == 0x30 || id == 0x3A) {
            if (frame >= 26.0f) {
                dust = true;
            }
        }
        if (dust) {
            m3C = 0;
            atn::Vector3 from = mChara->mPos;
            atn::Vector3 to = from;
            MtxPtr node = mChara->mMotion.GetNodeMtx_unk("spine1");
            if (!node) {
                node = mChara->mMotion.GetNodeMtx_unk("hips");
            }
            if (node) {
                to = from = atn::Vector3(node[0][3], node[1][3], node[2][3]);
                to.y -= 5.0f;
            } else {
                from.y += 3.0f;
                to.y -= 3.0f;
            }
            CollResult_unk res;
            if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0) && res.m34) {
                int attr = *(int*)res.m34;
                if ((attr >= 0x14 && attr <= 0x15) || (attr >= 0x1C && attr <= 0x1D)) {
                    atn::Vector3 pos = res.m10;
                    if (mChara->GetLens_unk()) {
                        pos.y = -mChara->GetLens_unk()->m160 * mChara->GetLens_unk()->m158 + 0.01f;
                    }
                    fn_801070C8(lbl_8087F048, 0x7D7, 0, 0, 0, &res.m10, &mChara->mRot,
                                Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
                    fn_80107208(lbl_8087F048, 0x7D7, &res.m10, 1.0f);
                }
            }
        }
    }
    mChara->fn_8013CB68(m08, 0, 0.0f, mChara->m568);
    return result;
}

bool ControlKnockdown::fn_801AE41C() {
    if (m14 > 90) {
        return true;
    }
    Motion_unk* model = &mChara->mMotion;
    switch (m18) {
    case 0: {
        bool done = false;
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0) && mChara->m574.y > -0.01f) {
            done = true;
        }
        return done;
    }
    case 3:
        return m14 > 15;
    }
    return false;
}

boost::function<void()> ControlKnockdown::vf10() {
    if (m24) {
        return m24;
    }
    return boost::bind(&CharaTask::fn_8015ECC4, (CharaTask*)mChara);
}

void ControlKnockdown::vf1C() {
    if (mChara->CanAct_unk()) {
        mChara->mColli.mFlags |= 1;
    }
    mChara->fn_80176DFC(0, 0, 0);
}

ControlGetUp::ControlGetUp(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0x1B;
    Motion_unk* model = &mChara->mMotion;
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    int id = model->mTracks[0].m00;
    if (id == 0x3D || id == 0x1E0) {
        model->fn_80097C08(0, 0x3E, 0, 0, 1, 0.0f, 0.1f);
    } else if (id == 0x3A || id == 0x30 || id == 0x171) {
        model->fn_80097C08(0, 0x3C, 0, 0, 1, 1.0f, 0.1f);
    } else {
        model->fn_80097C08(0, 0x3B, 0, 0, 1, 0.0f, 0.1f);
    }
    model->mTracks[0].m0C = mChara->IsPlayer() ? 1.3f : 1.0f;
    mChara->m6B8 = atn::Vector3(0.0f, 0.0f, 0.0f);
}

int ControlGetUp::vf0C() {
    int done = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        done = 1;
        if (mChara->mType == 0 && lbl_8087F610 && lbl_8087F610->m540 != 2) {
            fn_804DA690(lbl_8087F610);
        }
    }
    mChara->fn_8013CB68(mChara->mRot, 0, 0.0f, mChara->m568);
    return done;
}

ControlRevival::ControlRevival(Chara_unk* chara, int time) : ControlBase(chara), m08(time) {
    chara->m560 = 0x1C;
    mChara->mMotion.mTracks[0].m0C = 0.0f;
}

int ControlRevival::vf0C() {
    int done = 0;
    if (--m08 < 0) {
        mChara->mColli.mFlags |= 1;
        mChara->fn_8015495C(0, 1, 1, 1);
        done = 1;
    }
    return done;
}

// NONMATCHING: 98.4% - the original loads mChara before saving r31 (as in ControlCdnPiyo::vf10)
// and mChara of the second bind after its ptmf.
boost::function<void()> ControlRevival::vf10() {
    CharaTask* chara = mChara;
    if (chara->mType != 0 && chara->m564 != 7) {
        return FuncPair_unk(boost::bind(&CharaTask::fn_8016EB48, chara, false, false, false),
                            boost::bind(&CharaTask::fn_8015ECC4, (CharaTask*)mChara));
    }
    return boost::bind(&CharaTask::fn_8015ECC4, chara);
}

void ControlRevival::vf1C() {
    mChara->mColli.mFlags |= 1;
}

void ControlRevival::vf40() {
    atn::Vector3& v = mChara->mMotion.m220[mChara->m524].m04;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
}

ControlRevivalSkelton::ControlRevivalSkelton(Chara_unk* chara, int a) : ControlBase(chara), m08(a) {
    chara->m560 = 0x1C;
    Motion_unk* model = &mChara->mMotion;
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    model->mTracks[0].m08 = model->fn_80097D7C(0);
    model->mTracks[0].m0C = -1.0f;
}

int ControlRevivalSkelton::vf0C() {
    int done = 0;
    CharaTask* chara = mChara;
    if (chara->mMotion.mTracks[0].m08 <= 0.0f) {
        chara->mColli.mFlags |= 1;
        mChara->fn_8015495C(0, 1, 1, 1);
        done = 1;
    }
    return done;
}

// NONMATCHING: 94.1% - the original loads mChara before saving r31 (as in ControlCdnPiyo::vf10).
boost::function<void()> ControlRevivalSkelton::vf10() {
    return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
}

void ControlRevivalSkelton::vf1C() {
    mChara->mColli.mFlags |= 1;
}

ControlFallDead::ControlFallDead(Chara_unk* chara, const atn::Vector3& rot) : ControlBase(chara) {
    chara->m560 = 0x3B;
    Motion_unk* model = &mChara->mMotion;
    m0C = 45;
    m10.Set_unk(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    float yaw = rot.y;
    PSMTXRotRad(mtx, 'y', yaw);
    PSMTXMultVec(mtx, &m10, &m10);
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    if (atn::Abs_unk(atn::WrapAngle_unk(rot.y - mChara->mRot.y)) < 1.5707964f) {
        model->fn_80097C08(0, 0x1D3, 0, 0, 1, 0.0f, 0.1f);
        mChara->mRot = rot;
    } else {
        model->fn_80097C08(0, 0x1D4, 0, 0, 1, 0.0f, 0.1f);
        atn::Vector3 r = rot;
        r.y = atn::WrapAngle_unk(3.1415927f + r.y);
        *mChara->GetRot_unk() = r;
    }
    model->mTracks[0].m0C = 1.0f;
    m08 = 0;
    m1C = mChara->mColli.m48;
    if (lbl_8087F498) {
        fn_803EA77C(lbl_8087F498, mChara, 0xA, 0xF, 1.0f, 10.0f);
    }
}

int ControlFallDead::vf0C() {
    int landed = 0;
    Motion_unk* model = &mChara->mMotion;
    if (m08 != 2) {
        atn::Vector3 from = mChara->mColli.m48;
        atn::Vector3 to = from;
        to.y -= mChara->mColli.m54;
        to.y += mChara->m574.y;
        CollResult_unk res;
        if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0) &&
            (mChara->m54C & 0x400) == 0x400 && res.m34 && *(unsigned int*)res.m34 == 0x1A) {
            mChara->fn_8015495C(0, 1, 1, 1);
            return 1;
        }
    }
    if (m08 == 0) {
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0) - 1.0f) {
            model->mTracks[0].m0C = 0.0f;
            atn::Vector3 v = mChara->m574;
            v.y = mChara->mColli.m48.y - m1C.y;
            mChara->m574 = v;
            m08 = 1;
        }
        atn::Vector3 pos = mChara->mPos;
        pos += m10 * 0.2f;
        pos.y += -34.0f / model->fn_80097D7C(0);
        mChara->mPos = pos;
    } else if (m08 == 1) {
        m0C--;
        if (landed || m0C <= 0) {
            if (lbl_8087F048) {
                fn_800FDE60(lbl_8087F048, 0, mChara, atn::Vector3(0.0f, -1.0f, 0.0f));
            }
            model->fn_80097C08(0, 0x1D6, 0, 0, 1, 0.0f, 0.1f);
            model->mTracks[0].m0C = 1.0f;
            m08 = 2;
        } else {
            atn::Vector3 pos = mChara->mPos;
            pos.y += mChara->m574.y;
            mChara->mPos = pos;
        }
    } else {
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            return 1;
        }
        mChara->vf34(&mChara->mRot, 1, 0.0f, 1.0f);
    }
    if (m08 != 2) {
        atn::Vector3 from = m1C;
        atn::Vector3 to = mChara->mColli.m48;
        if (lbl_8087F048) {
            fn_8010A308(lbl_8087F048, &from, &to, mChara->m5B0);
        }
    }
    m1C = mChara->mColli.m48;
    return 0;
}

// NONMATCHING: 68.1% - the original schedules the loads of m08 and mChara into the prologue.
bool ControlFallDead::fn_801AF81C() {
    int state = m08;
    Motion_unk* model = &mChara->mMotion;
    bool result = false;
    if (state == 2) {
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            result = true;
        }
    }
    return result;
}

// NONMATCHING: 90.2% - the original sets r3/r4 of the motion call before choosing the motion (with
// a branch; a ?: gives neg/or here).
ControlGuarded::ControlGuarded(Chara_unk* chara, int a) : ControlBase(chara), m08(a) {
    int id = 0x15;
    if (a) {
        id = 0x16;
    }
    chara->m560 = id;
    Motion_unk* model = &mChara->mMotion;
    mChara->fn_801446F0();
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    int motion = 0x1DE;
    if (a) {
        motion = 0x1DD;
    }
    model->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
    model->mTracks[0].m0C = 1.0f;
    if (mChara->mType != 2) {
        fn_800C344C(sSe[0], &mChara->mPos, 0, -1, 1.0f);
    }
}

// NONMATCHING: 99.8% - stack slots of the three temporaries of the push (front, negated, scaled).
int ControlGuarded::vf0C() {
    Motion_unk* model = &mChara->mMotion;
    int done = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= model->fn_80097D7C(0)) {
        done = 1;
    }
    if (m08 == 0 && model->mTracks[0].m08 < 25.0f) {
        mChara->m6B8 = -FrontDir_unk(mChara->mRot) * 0.33f;
    }
    mChara->fn_8013CB68(mChara->mRot, 0, 0.0f, mChara->m568);
    return done;
}

// NONMATCHING: 99.3% - the original stores m08 after loading m14.y for the rotation.
ControlDamageLastOne::ControlDamageLastOne(Chara_unk* chara, const atn::Vector3* dir, int kind)
    : ControlBase(chara), m20(kind) {
    chara->m560 = 0x8E;
    Motion_unk* model = &mChara->mMotion;
    atn::Vector3 n = *dir;
    PSVECNormalize(&n, &n);
    atn::Vector3 push = *dir;
    push.y *= 0.3f;
    CharaTask* c = mChara;
    c->m6B8 = push;
    m14 = (-n).ToRotation_unk();
    mChara->fn_8016DA4C();
    if (mChara->mFlag28 && lbl_8087F430) {
        lbl_8087F430->fn_803750E4(0xB5);
    }
    mChara->fn_80164DCC(1);
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->IsFlag5_unk()) {
        mChara->mFlag5 = 0;
    }
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    switch (m20) {
    case 9:
    case 10:
    case 12:
        model->fn_80097C08(0, 0x171, 0, 1, 1, 0.0f, 0.1f);
        break;
    case 11: {
        model->fn_80097C08(0, 0x172, 0, 1, 1, 0.0f, 0.1f);
        m08.Set_unk(-10.0f, 0.0f, -25.0f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', m14.y);
        PSMTXMultVec(mtx, &m08, &m08);
        float scale = 0.04f;
        m08 *= scale;
        break;
    }
    }
    model->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 6, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 92.4% - the inlined fn_801B0260 loads mChara again (the original keeps a second
// model pointer, so r30/r31 are swapped), and side * 2 is computed before the second front direction.
int ControlDamageLastOne::vf0C() {
    Motion_unk* model = &mChara->mMotion;
    if (fn_801B0260()) {
        return 1;
    }
    switch (m20) {
    case 10:
    case 12:
        if (model->mTracks[0].m00 == 0x171) {
            model->mTracks[0].m0C = 0.8f;
            if (model->mTracks[0].m08 >= 12.0f) {
                model->fn_80097C08(0, 0x1DD, 0, 1, 1, 0.0f, 0.1f);
                model->mTracks[0].m0C = 1.0f;
            }
        } else if (model->mTracks[0].m00 == 0x1DD) {
            if (model->mTracks[0].m08 >= 23.0f) {
                model->fn_80097C08(0, 0x30, 0, 1, 1, 0.0f, 0.2f);
                atn::Vector3 side = Cross_unk(FrontDir_unk(mChara->mRot), atn::Vector3(0.0f, 1.0f, 0.0f));
                side = -FrontDir_unk(mChara->mRot) * 6.0f - (side * 2.0f);
                mChara->m6B8 = side;
            }
        }
        break;
    case 11:
        if (model->mTracks[0].m08 > 30.0f && model->mTracks[0].m08 < 54.0f) {
            mChara->m6B8 = m08;
            mChara->m574 = atn::Vector3(0.0f, 0.0f, 0.0f);
        }
        break;
    }
    mChara->fn_8013CB68(m14, 0, 0.0f, mChara->m568);
    return 0;
}

// NONMATCHING: 80.0% - the original schedules the loads of m20 and mChara into the prologue.
bool ControlDamageLastOne::fn_801B0260() {
    Motion_unk* model = &mChara->mMotion;
    if (m20 == 10 || m20 == 12) {
        bool result = false;
        if (model->mTracks[0].m00 == 0x30) {
            float frame = model->mTracks[0].m08;
            if (frame >= model->fn_80097D7C(0)) {
                result = true;
            }
        }
        return result;
    }
    float frame = model->mTracks[0].m08;
    return frame >= model->fn_80097D7C(0);
}

// NONMATCHING: 94.0% - the original loads mChara before saving r31 (as in ControlCdnPiyo::vf10).
boost::function<void()> ControlDamageLastOne::vf10() {
    return boost::bind(&CharaTask::fn_8015ECC4, (CharaTask*)mChara);
}

} // namespace chr
