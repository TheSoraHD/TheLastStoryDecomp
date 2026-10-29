#include "game/Em101LittleZesyaSecond.h"
#include "game/CharaManager.h"
#include "game/HomingBullet.h"
#include "game/PlayerTask.h"
#include "chr/ControlBase.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: debug menu entry (as in game/Em101ElderZesya.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as ElderLook_unk in game/Em101ElderZesya.cpp)
struct ZesyaLook_unk {
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        if (flag) {
            m04 = 1;
        } else {
            m04 = 0;
        }
    }
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
};
// inferred: camera parameters of the field (lbl_8087F430 + 0x588, as ElderCamParam_unk in
// game/Em101ElderZesya.cpp)
struct ZesyaCamParam_unk {
    ZesyaCamParam_unk(const ZesyaCamParam_unk& o)
        : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C),
          m20(o.m20), m24(o.m24), m28(o.m28), m2C(o.m2C), m30(o.m30), m34(o.m34), m38(o.m38) {}
    ZesyaCamParam_unk& operator=(const ZesyaCamParam_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        m14 = o.m14;
        m18 = o.m18;
        m1C = o.m1C;
        m20 = o.m20;
        m24 = o.m24;
        m28 = o.m28;
        m2C = o.m2C;
        m30 = o.m30;
        m34 = o.m34;
        m38 = o.m38;
        return *this;
    }
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    int m38;
};
// inferred: camera mode of the field (lbl_8087F430 + 0x4D8, as ElderCamera_unk in game/Em101ElderZesya.cpp)
struct ZesyaCamera_unk {
    void Set_unk(int mode, const ZesyaCamParam_unk& param) {
        if (mMode != mode) {
            mMode = mode;
            mParam = param;
            m08 = 0.05f;
            m04 = 1.0f;
        }
    }
    int mMode;
    float m04;
    float m08;
    ZesyaCamParam_unk mParam;
};
// inferred: field (lbl_8087F430): camera at 0x4D8, look point at 0x97C, route tables at 0x10D8 (as in
// game/Em101ElderZesya.cpp)
struct Field_unk {
    unsigned char m0000[0x4D8];
    ZesyaCamera_unk m4D8;
    unsigned char m520[0x588 - 0x520];
    ZesyaCamParam_unk m588;
    unsigned char m5C4[0x8A0 - 0x5C4];
    CharaTask* m8A0; // inferred: character the camera follows
    unsigned char m8A4[0x97C - 0x8A4];
    ZesyaLook_unk m97C;
    unsigned char m9A4[0x10D8 - 0x9A4];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: word at 0x1C of the running control (class unknown)
struct Control1C_unk {
    unsigned char m00[0x1C];
    int m1C;
};
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in game/Em101ElderZesyaSecond.cpp)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a, float b);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" int rand();
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6); // inferred: nearest route point (as in gmk/MeetingSpot.cpp)
// inferred: collision world and its ray test (as in game/Em101LittleZesya.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10;
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// NONMATCHING: 99.81% - r26/r27 of the second std::string and r25/r26 of the strlen of the parameters
// (same as Em101LittleZesya / Em101ElderZesya / Em201ZanGurg)
Em101LittleZesyaSecond::Em101LittleZesyaSecond(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14CC(0), m14D0(2), m14D4(20),
      m14D8(90), m14E8(0), m14EC(0.0f), m14F0(0.0f), m14F4(0.0f), m14F8(0.0f), m14FC(0.0f, 0.0f, 0.0f),
      m1508(0.0f, 0.0f, 0.0f), m1514(0.0f, 0.0f, 0.0f), m1548(0), m154C(0),
      m1554(0) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em101_02_00.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    void* group = m1550.Load("LittleZesyaSecond");
    fn_80087994(group, "DebugDisp", &m1554, 0, 0);
    m1520.LoadRequest("ef_bs007a.efp");
    m152C.LoadRequest("ef_sc019a.efp");
    m1538.LoadRequest("ef_sk030b2.efp");
    mFlagB5 = 1;
    mFlagB11 = 1;
}

Em101LittleZesyaSecond::~Em101LittleZesyaSecond() {}

int Em101LittleZesyaSecond::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1520.IsLoading() == 0 &&
        m152C.IsLoading() == 0 && m1538.IsLoading() == 0 && m14B0.fn_80473F50() == 0) {
        m58C = 0;
        fn_8016E970(2);
        fn_80260324();
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x1C21D;
        mStatus.m18 |= 0x3800000;
        mStatus.fn_80134674(2);
        mStatus.fn_80134674(3);
        return 1;
    }
    return 0;
}

void Em101LittleZesyaSecond::fn_80260324() {
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "NormalAttackID") == 0) {
            m14C8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WarpCenterPoint") == 0 && lbl_8087F430 != 0) {
            RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(atoi(csv.fn_8005B9CC()));
            if (place != 0) {
                m14FC = place->mPos;
            }
        }
    } while (csv.fn_8005BBF8());
}

// inferred: back to the idle action (name invented; the same block is repeated in the states)
inline void Em101LittleZesyaSecond::ResetAction_unk() {
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14C4 = 0;
    mFlag5 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 102, 1);
}

// inferred: gives the camera back (name invented)
inline void Em101LittleZesyaSecond::ReleaseCamera_unk() {
    if (m154C != 0) {
        Field_unk* field = lbl_8087F430;
        field->m8A0 = 0;
        field->m4D8.mMode = 0;
        field->m97C.m00 = 0;
        m154C = 0;
    }
}

// inferred: plays an effect at a position (locals in the order of game/Em305Vampire.cpp; name invented)
inline void PlayEffectAt_unk(atn::EfpFile* efp, const atn::Vector3& pos, const atn::Vector3& rot) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: state 1, casts m638 while the control allows it (as Em101LittleZesya::fn_8025DE7C; name invented)
inline void Em101LittleZesyaSecond::Cast_unk() {
    if (fn_80158BB4()) {
        ResetAction_unk();
        m58C = 0;
        fn_8016E970(3);
    } else {
        fn_80139560();
        if (lbl_8087F048 != 0 && fn_80158CA4() != 0) {
            if (mControl && mControl->vf30() > m598) {
                m598 = mControl->vf30();
                m594 = 1;
                m590 = fn_800F8548(lbl_8087F048);
            }
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, m638, m590, 30, -1, 0.0f);
        }
    }
}

// inferred: state 12, warp slash along the front between frames 17 and 25 (name invented)
inline void Em101LittleZesyaSecond::Slash_unk() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= 17.0f && frame < 25.0f) {
        HitInfo_unk* hit = fn_80219E6C(0x5E3);
        float t = (mMotion.mTracks[0].m08 - 17.0f) * 0.125f;
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        atn::Vector3 d = dir * 120.0f;
        atn::Vector3 pos = mPos + d * t;
        fn_800FAB80(lbl_8087F048, this, hit, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        if (m14C4 == 0) {
            fn_80232B7C(0, 0);
            PlayEffectAt_unk(&m1538, pos, mRot);
            m14C4 = 1;
        }
    } else {
        frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            ResetAction_unk();
            m58C = 0;
            fn_8016E970(3);
        }
    }
}

// NONMATCHING: 99.96% - only the stack slots of the warp slash locals (state 12: front vector, positions and
// color; 12 variants: helper inlines, declaration orders, temporaries)
void Em101LittleZesyaSecond::vf10() {
    m14B8 = mC58.mC4;
    m14BC++;
    if (fn_80370A78(lbl_8087F430, 0xFA) == 1) {
        ReleaseCamera_unk();
        ResetAction_unk();
        m58C = 0;
        fn_8016E970(3);
        fn_80370AE4(lbl_8087F430, 0xFA, 0);
    }
    if (mC58.mC0 == 0 || m14B8 == 0) {
        if (m58C == 2) {
            mPos = m1514;
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag25 = 1;
                mMotion.mTracks[0].m0C = 0.0f;
                fn_800D246C(1);
                fn_800EE360(this);
            }
        } else {
            fn_8016E970(3);
            m58C = 0;
        }
    } else {
        switch (m58C) {
        case 1:
            Cast_unk();
            break;
        case 6:
            fn_80261A50();
            break;
        case 7: {
            float frame = mMotion.mTracks[0].m08;
            if ((float)__fabs(frame - 24.0f) < 1.1920929e-7f) {
                fn_800FAB80(lbl_8087F048, this, fn_80219E6C(0x5E9), m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f,
                            1.0f);
            } else if (frame >= mMotion.fn_80097D7C(0)) {
                ResetAction_unk();
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        }
        case 11:
            fn_80261DC0();
            break;
        case 12:
            Slash_unk();
            break;
        case 8:
            if (m14BC > 10) {
                if (m14E8 == 1) {
                    ResetAction_unk();
                    m58C = 10;
                    fn_8016E970(3);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 1.0f, 0.1f);
                    mMotion.mTracks[0].m08 = 0.0f;
                    fn_802621D4((atn::Vector3*)&m14EC, &m14F8);
                    mPos = *(atn::Vector3*)&m14EC;
                    mRot.y = m14F8;
                    m1544 = 0.0f;
                } else {
                    ResetAction_unk();
                    m58C = 9;
                    fn_8016E970(3);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
                    fn_802621D4((atn::Vector3*)&m14EC, &m14F8);
                    mPos = *(atn::Vector3*)&m14EC;
                    mRot.y = m14F8;
                }
            }
            break;
        case 9:
            if (m14BC > 10) {
                ResetAction_unk();
                m58C = 0;
                fn_8016E970(3);
                m14BC = m14D4;
                Field_unk* field = lbl_8087F430;
                fn_80370AE4(field, 0xF1, fn_80370A78(field, 0xF1) + 1);
            }
            break;
        case 10: {
            float frame = mMotion.mTracks[0].m08;
            if (17.0f < frame && frame <= 19.0f) {
                fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x5E8), m590, 30, -1, 0.0f);
            } else {
                frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    ResetAction_unk();
                    m58C = 0;
                    fn_8016E970(3);
                    Field_unk* field = lbl_8087F430;
                    fn_80370AE4(field, 0xF1, fn_80370A78(field, 0xF1) + 1);
                }
            }
            m1544 = mMotion.mTracks[0].m08;
            break;
        }
        case 2: {
            mPos = m1514;
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag25 = 1;
                mMotion.mTracks[0].m0C = 0.0f;
                fn_800D246C(1);
                fn_800EE360(this);
            }
            break;
        }
        default:
            fn_80261948();
            if (m55C != 6) {
                fn_80261078();
            }
            break;
        }
        bool down = mStatus.IsState_unk(0x80000) || mStatus.IsState_unk(0x1000000);
        if (down) {
            fn_8014C0B4(25, m484[0]);
        } else {
            fn_8014C0B4(25, 0x1C4);
        }
        if (m58C != 11) {
            int camera = m154C;
            Field_unk* field = lbl_8087F430;
            if (camera != 0) {
                field->m8A0 = 0;
                field->m4D8.mMode = 0;
                field->m97C.m00 = 0;
                m154C = 0;
            }
        }
    }
    fn_8014C540();
    fn_80145334();
}

void Em101LittleZesyaSecond::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    switch (m58C) {
    case 6:
    case 7:
    case 1:
    case 11:
        msg->m10 *= 0.0f;
        break;
    case 8:
        return;
    }
    mStatus.fn_80134674(2);
    mStatus.fn_80134674(3);
    fn_80151448(msg);
    vf40(hit);
}

void Em101LittleZesyaSecond::vf40(HitInfo_unk* hit) {
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8 * 0.5f, -1);
        case 20:
        reset:
            m58C = 0;
            m14BC = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m14C4 = 0;
            mFlag5 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 102, 1);
            break;
        }
    }
    m1548++;
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802620A8();
    }
}

// inferred: plays an effect on the model (locals in the order of Em003LesserDaemon's PlayEffectM_unk; name
// invented)
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
    Color4f_unk color;
    atn::Vector3 rot;
    atn::Vector3 pos;
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

// NONMATCHING: 98.51% - registers of the two status tests (r0/r3/r4 swapped) and stack slots of the effect
// locals and of the rotation of the normal attack (8 variants: bool locals, inline predicate, if/else,
// declaration orders, ToRotationZ_unk)
// inferred: chooses the next action: warp after 5 hits, the warp attacks every m14D0 actions, the normal
// attack in range, otherwise the warp slash or a warp
void Em101LittleZesyaSecond::fn_80261078() {
    atn::Vector3 target = m14B8->mPos;
    atn::Vector3 d = target - mPos;
    float dist = PSVECMag(&d);
    float reach = m14C8->m40 + mStatus.mParamB.m28;
    m14D4 = mStatus.IsState_unk(8) || mStatus.IsState_unk(0x200) ? 40 : 20;
    float range = reach - 8.0f;
    m14D8 = 90;
    float rate = mStatus.mParamB.m84 > 0 ? mStatus.m04 / (float)mStatus.mParamB.m84 : 0.0f;
    if (rate < 0.5f) {
        m14D4 /= 2;
        m14D8 /= 4;
    }
    if (m1548 > 5) {
        ResetAction_unk();
        m58C = 8;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 101);
        PlayEffect_unk(&m1520, &mMotion);
        m14E8 = 0;
        m14CC = m14D0 + 1;
        m1548 = 0;
    } else if (m14CC > m14D0) {
        if (m14BC > m14D4) {
            if (rand() % 2 == 1) {
                ResetAction_unk();
                m58C = 11;
                fn_8016E970(3);
                m590 = fn_800F8548(lbl_8087F048);
                m594 = 1;
                m598 = 0;
                m1508 = mPos;
                mFlag5 = 0;
                fn_8016D74C(fn_80219E6C(0x5E2), 0, 0);
            } else {
                ResetAction_unk();
                m58C = 6;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
                mMotion.mTracks[0].m0C = 2.0f;
                fn_80232B7C(this, 102);
                PlayEffect_unk(&m152C, &mMotion);
            }
            m14CC = 0;
        }
    } else if (dist <= range) {
        if (m14BC > m14D4) {
            m14CC++;
            ResetAction_unk();
            m58C = 1;
            fn_801561E4(fn_80219E6C(200));
            bool fast = mStatus.IsState_unk(8) || mStatus.IsState_unk(0x200);
            if (!fast) {
                mMotion.mTracks[0].m0C = 1.5f;
            } else {
                mMotion.mTracks[0].m0C = 0.9f;
            }
            m598 = 0;
            atn::Vector3 dir = m14B8->mPos - mPos;
            PSVECNormalize(&dir, &dir);
            atn::Vector3 rot = dir.ToRotation_unk();
            mRot.y = rot.y;
        }
    } else if (m14BC > m14D8) {
        if (rand() % 2 == 1 && dist < 120.0f) {
            ResetAction_unk();
            m58C = 12;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 0.6f;
            mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
        } else {
            ResetAction_unk();
            m58C = 8;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
            fn_80232B7C(this, 101);
            PlayEffect_unk(&m1520, &mMotion);
            m14E8 = 1;
        }
    }
}

void Em101LittleZesyaSecond::fn_80261948() {
    switch (m55C) {
    case 6:
        if (m560 == 10) {
            m598 = 0;
            m59D = 0;
            m59C = 0;
            m59F = 1;
            m590 = fn_800F8548(lbl_8087F048);
            SetHit_unk(fn_80219E6C(0xE5));
            ((Control1C_unk*)mControl.get())->m1C = 1;
            m58C = 1;
            Field_unk* field = lbl_8087F430;
            fn_80370AE4(field, 0xF1, fn_80370A78(field, 0xF0) + 1);
        }
        break;
    case 7:
        mFC0 = 0;
        mFlag5 = 0;
        break;
    default:
        fn_8016E970(3);
        if (m58C == 0) {
            mFlag5 = 1;
        }
        mFC0 = m14B8;
        break;
    }
    fn_80139560();
}

inline atn::Vector3 GetTrans_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 98.11% - stores of the homing parameters scheduled with the bullet data load (r24/r3) and r3/r4
// of the node pointer (6 variants: store orders, aggregate init, locals)
void Em101LittleZesyaSecond::fn_80261A50() {
    if (m14C4 == 0) {
        if (mMotion.mTracks[0].m08 > 60.0f) {
            m12CC = 1.0f;
            atn::Vector3 hips = GetTrans_unk(mMotion.GetNodeMtx_unk("hips"));
            for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                if (p->CanAct_unk()) {
                    atn::Model* model = &p->mMotion;
                    atn::Vector3 target = GetTrans_unk(model->GetNodeMtx_unk("hips"));
                    atn::Vector3 dir = (target - hips).GetNormalize();
                    atn::Vector3 pos = hips + dir * 15.0f;
                    HomingParam_unk param;
                    param.mAccel = 0.0f;
                    param.m08 = -1.0f;
                    param.mMaxSpeed = 10.0f;
                    param.mTime = 300.0f;
                    param.mNode = 0;
                    param.mCount = -1;
                    param.mDelay = 3.0f;
                    param.mTarget = p;
                    param.mTurn = 1.2566371f;
                    fn_800F8574(lbl_8087F048, this, fn_80219E6C(0x5E7), &pos, &dir, &param, 0x24, 0, 0.0f, 1.0f);
                }
            }
            m14C4 = 1;
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 100.0f) {
            m14BC = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m14C4 = 0;
            mFlag5 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 102, 1);
            m58C = 0;
            fn_8016E970(3);
        }
    }
}

// NONMATCHING: 98.17% - the original stores the camera parameters through a base register (&field->m4D8) and
// keeps the look point address in r5 (known blocker of EnemyJill / Em101ElderZesya::fn_8025922C; 10 variants:
// Set_unk forms, pointer orders, volatile, wrapper inline); the if/else in ZesyaLook_unk::Set_unk gives the
// dead branch of the original
// inferred: state 11, the camera follows the enemy while its control runs (as Em101ElderZesya::fn_8025922C)
void Em101LittleZesyaSecond::fn_80261DC0() {
    Field_unk* field = lbl_8087F430;
    if (m55C == 6) {
        switch (m560) {
        case 13:
            fn_80139560();
            break;
        case 14: {
            fn_80139560();
            atn::Vector3 pos = mPos;
            pos.y += 15.0f;
            if (m154C == 0) {
                ZesyaCamera_unk* cam = &field->m4D8;
                ZesyaCamParam_unk param = field->m588;
                param.m08 = -0.43633232f;
                param.m04 = 0.7853982f;
                param.m00 = 15.0f;
                param.m10 = 8.0f;
                field->m8A0 = this;
                cam->Set_unk(4, param);
                atn::Vector3 look = mPos;
                look.y += 25.0f;
                field->m97C.Set_unk(look, false);
                m154C = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
            break;
        }
        default:
            ReleaseCamera_unk();
            ResetAction_unk();
            m58C = 0;
            fn_8016E970(3);
            break;
        }
    } else {
        ResetAction_unk();
        m58C = 0;
        fn_8016E970(3);
    }
}

void Em101LittleZesyaSecond::fn_802620A8() {
    if (m58C == 11) {
        m1514 = m1508;
    } else {
        m1514 = mPos;
    }
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14C4 = 0;
    mFlag5 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 102, 1);
    m58C = 2;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: negated vector (name invented)
inline atn::Vector3 Neg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: warp destination: behind the target at attack range (m14E8 == 1) or 60 from the warp center on
// the side of the enemy, dropped to the ground and moved to the nearest route point, facing the target
void Em101LittleZesyaSecond::fn_802621D4(atn::Vector3* pos, float* rot) {
    if (m14B8 == 0) {
        return;
    }
    atn::Vector3 dir = m14B8->mPos - m14FC;
    PSVECNormalize(&dir, &dir);
    if (m14E8 == 1) {
        float range = m14C8->m40 + mStatus.mParamB.m28;
        *pos = m14B8->mPos + Neg_unk(dir) * range * 0.75f;
        pos->y += 100.0f;
    } else {
        dir = mPos - m14FC;
        PSVECNormalize(&dir, &dir);
        *pos = m14FC + Neg_unk(dir) * 60.0f;
        pos->y += 100.0f;
    }
    CollResult_unk result;
    atn::Vector3 to = atn::Vector3(0.0f, -200.0f, 0.0f) + *pos;
    if (fn_8004ED34(lbl_8087EE98, &result, pos, &to, 0x80000000, &mColli, 0) != 0) {
        *pos = result.m10;
    }
    int point = fn_803C1560(lbl_8087F430->m10D8, pos, 0, 0, 0, 1, -1.0f);
    *pos = lbl_8087F430->m10D8->mPoints[point - 1].mPos;
    *rot = (m14B8->mPos - *pos).ToRotationZ_unk().y;
}

// NONMATCHING: 99.83% - slots of x/y of the negated front (stores z, x, y in the original; 10 variants:
// member order, constructor, locals, inline, IsDown_unk, Neg_unk)
int Em101LittleZesyaSecond::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (mFlag5) {
        if (!IsDown_unk()) {
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx m;
            float ry = mRot.y;
            PSMTXRotRad(m, 'y', ry);
            PSMTXMultVec(m, &front, &front);
            atn::Vector3 back(-front.x, -front.y, -front.z);
            if (PSVECDotProduct(dir, &back) > -0.5f) {
                return (m55C != 6) + 1;
            }
        }
    }
    return 0;
}

int Em101LittleZesyaSecond::vfA8(int kind) {
    if (kind == 1 && mFlag5) {
        bool down = mStatus.IsState_unk(0x80000) || mStatus.IsState_unk(0x1000000);
        if (!down) {
            return 3;
        }
    }
    return -1;
}
