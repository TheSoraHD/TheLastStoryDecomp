#include "game/EnemyFieldMagician.h"
#include "atn/Csv.h"
#include "atn/SoundManager.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
// inferred: std::string operator+(const char*, const std::string&) (0x8006AF38)
extern "C" std::string fn_8006AF38(const char* a, const std::string& b);
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" void fn_800E2FE0(BattleCharaTask* chara); // inferred: BattleCharaTask per-frame update
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
// inferred: field (lbl_8087F430); m10D8 holds the route tables, the event flags are read with
// fn_80370A78 and written with fn_80370AE4
struct Field_unk {
    void fn_803750E4(int id);
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: effects (as in game/Em002Skelton.cpp)
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, const void* mtx,
                            const atn::Vector3* pos, const atn::Vector3* rot, const Color4f_unk* color, int arg8,
                            int arg9, int arg10, float scale); // inferred: plays an effect
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: result of a segment test (hit position at +4)
struct CollSegResult_unk {
    int m00;
    atn::Vector3 m04;
};
// inferred: collision world (m4008: result of the segment tests)
struct CollWorld_unk {
    unsigned char m0000[0x4008];
    CollSegResult_unk m4008;
};
extern "C" CollWorld_unk* lbl_8087EE98;

// inferred: game timer (m3A4: frame time)
struct Timer_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" Timer_unk* lbl_8087EFA8;
inline float GetFrameTime_unk() {
    return lbl_8087EFA8->m3A4;
}

// inferred: segment (as in game/CharaTask.cpp)
struct Segment_unk {
    Segment_unk() {}
    Segment_unk(const atn::Vector3& start, const atn::Vector3& end) : mStart(start), mEnd(end) {}
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};

// inferred: sounds of the field (raised, weakened, broken)
static const char* const sFieldSounds[] = {"SE_SP007_001", "SE_SP007_002", "SE_SP007_003"};

// inferred: name invented (as in CharaTask.cpp)
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

// inferred: negated vector (name invented)
inline atn::Vector3 Neg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: forward direction of a character (name invented)
inline atn::Vector3 GetFront_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// inferred: translation matrix and matrix product (names invented)
inline atn::Matrix3x4 TransMtx_unk(const atn::Vector3& v) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}
inline atn::Matrix3x4 MulMtx_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 m;
    PSMTXConcat(a.m, b.m, m.m);
    return m;
}

// inferred: plays an effect on the model (as in game/Em002Skelton.cpp; name invented)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
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

inline void EnemyFieldMagician::PlayFieldEffect_unk(int index) {
    atn::Vector3 pos(mPos);
    pos += GetFront_unk(mRot) * 10.0f;
    pos.y += 15.0f;
    fn_80232B7C(&m15C0[0], 0);
    fn_8023A8B4(lbl_8087F3C0, &m15C0[index], 0, 0, pos, mRot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                1.0f);
}

EnemyFieldMagician::EnemyFieldMagician(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m1608(0), m160C(0), m1610(0), m1614(0), m1618(0), m1624(info), m1628(90),
      m1650(0), m1654(-1), m1658(-1), m165C(-1), m1660(-1) {
    m54C |= 0x02000000;
    m15C0[0].fn_8023780C("ef_sk051a.efp");
    m15C0[1].fn_8023780C("ef_sk051b.efp");
    m15C0[2].fn_8023780C("ef_sk051c.efp");
    m15C0[3].fn_8023780C("ef_sk051d.efp");
    m15F0.fn_8023780C("ef_bs007a.efp");
    m14B0[0].fn_80058078("ef_col00n.hcb");
    m14B0[0].mFlags &= ~1;
    m14B0[1].fn_80058078("ef_col01n.hcb");
    m14B0[1].mFlags &= ~1;
    fn_80097A88(&mMotion, 0x13F, "na000_skn00a_04.motion");
    fn_80097A88(&mMotion, 0x140, "na000_skn00b_04.motion");
    m15FC.fn_80237654("ef_bu040g_01.eff");
    std::string path;
    const char* name = strstr(info->m2C, "db_name=");
    if (name != 0) {
        path = name + 8;
        path = fn_8006AF38("/database/chara/special/", path);
    }
    if (path.size() != 0) {
        m161C.vf0C(path.c_str());
    }
}

int EnemyFieldMagician::vf0C() {
    for (int i = 0; i < 4; i++) {
        if (m15C0[i].fn_80237874() != 0) {
            return 0;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (m14B0[i].fn_800580BC() != 0) {
            return 0;
        }
    }
    if (m161C.fn_80473F50() != 0) {
        return 0;
    }
    if (m15F0.fn_80237874() != 0 || m15FC.fn_802376D0() != 0) {
        return 0;
    }
    if (BattleCharaTask::vf0C() != 0) {
        fn_8032C964();
        mStatus.m18 |= 0x40;
        mStatus.m18 |= 0x100;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4;
        mStatus.m18 |= 0x1;
        mStatus.m18 |= 0x400000;
        return 1;
    }
    return 0;
}

void EnemyFieldMagician::fn_8032C964() {
    atn::Csv csv((const char*)m161C.fn_80470580(), m161C.fn_8047059C());
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] != 0 && field[0] != ';') {
            if (strcmp(field, "FieldWeakTime") == 0) {
                m1628 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "WarpPoint") == 0) {
                for (unsigned int i = 0; i < 8; i++) {
                    int id = atoi(csv.fn_8005B9CC());
                    if (id > 0) {
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m162C.push_back(place);
                        }
                    }
                }
            } else if (strcmp(field, "WarpPointLoop") == 0) {
                m1650 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "FieldExecuteLocalFlag") == 0) {
                m1654 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "FieldWeakLocalFlag") == 0) {
                m1658 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "FieldBreakLocalFlag") == 0) {
                m165C = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "FieldUnuseLocalFlag") == 0) {
                m1660 = atoi(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

void EnemyFieldMagician::vf10() {
    bool unused = false;
    if (m1660 >= 0 && lbl_8087F430 != 0 && fn_80370A78(lbl_8087F430, m1660) == 1) {
        unused = true;
    }
    if (m55C == 7 || unused) {
        if (m58C >= 6) {
            m58C = 0;
        }
        if (m1608 != 0) {
            fn_80239DAC(lbl_8087F3C0, &m15C0[0], 0, 0);
            if (lbl_8087EFE8 != 0) {
                lbl_8087EFE8->fn_800CFD18(this, 15);
            }
            m14B0[0].mFlags &= ~1;
            m14B0[1].mFlags &= ~1;
            m1608 = 0;
        }
        m1618 = 0;
    }
    switch (m58C) {
    case 6:
        fn_8032D1F4();
        fn_80145334();
        fn_8014C540();
        break;
    case 7:
        if (++m1610 >= 600) {
            lbl_8087F430->fn_803750E4(0xF7);
        }
        fn_80145334();
        fn_8014C540();
        break;
    case 8:
    case 9: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            if (m58C == 8) {
                fn_8032D48C();
            } else {
                m58C = 0;
            }
        }
        fn_80145334();
        fn_8014C540();
        break;
    }
    default:
        if (m1608 == 0 && m55C == 2 && mC58.mC0 == 1 && !unused) {
            if (m1618 != 0) {
                m58C = 8;
                mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.mTracks[0].m20 = 1.0f;
                fn_80232B7C(&m15F0, 0);
                atn::Vector3 pos;
                atn::Vector3 rot;
                Color4f_unk color;
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
                fn_8023A8B4(lbl_8087F3C0, &m15F0, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
                m1618 = 0;
            } else {
                m58C = 6;
                mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.2f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                m1608 = 0;
            }
            fn_80145334();
            fn_8014C540();
        } else {
            fn_800E2FE0(this);
        }
        break;
    }
    fn_8032CF10();
}

// NONMATCHING: 96.3% - stack slots: the original keeps the offset of the collision matrices below the
// locals of PlayFieldEffect_unk
void EnemyFieldMagician::fn_8032CF10() {
    if (m1608 != 0 && m160C > 0 && --m160C == 0) {
        m14B0[0].mFlags |= 1;
        m14B0[1].mFlags &= ~1;
        fn_80239DAC(lbl_8087F3C0, &m15C0[0], 0, 1);
        PlayFieldEffect_unk(3);
        fn_800C344C(sFieldSounds[0], &mPos, 0, -1, 1.0f);
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->fn_800CFD18(this, 15);
        }
    }
    for (int i = 0; i < 2; i++) {
        if ((m14B0[i].mFlags & 1) == 1) {
            atn::Matrix3x4 mtx(*(atn::Matrix3x4*)mMotion.mMtx);
            mtx = MulMtx_unk(mtx, TransMtx_unk(atn::Vector3(0.0f, 15.0f, 10.0f)));
            m14B0[i].SetMtx_unk(mtx);
        }
    }
}

// NONMATCHING: 98.8% - mRot.y is read through r3 before `this` moves to r31 in the original
void EnemyFieldMagician::fn_8032D1F4() {
    m1610 = 0;
    float frame = mMotion.mTracks[0].m08;
    if (frame > 5.0f && frame < 20.0f) {
        mPos.y += 1.5f * GetFrameTime_unk();
    }
    if (m1608 == 0 && mMotion.mTracks[0].m08 >= 20.0f) {
        m14B0[0].mFlags |= 1;
        m14B0[1].mFlags &= ~1;
        PlayFieldEffect_unk(0);
        fn_800C344C(sFieldSounds[0], &mPos, 0, -1, 1.0f);
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->fn_800CFD18(this, 15);
        }
        m1608 = 1;
        m160C = 0;
        int flag = m1654;
        if (flag >= 0 && fn_80370A78(lbl_8087F430, flag) == 0) {
            fn_80370AE4(lbl_8087F430, flag, 1);
        }
    }
    float frame2 = mMotion.mTracks[0].m08;
    if (frame2 >= mMotion.fn_80097D7C(0)) {
        m58C = 7;
        mMotion.fn_80097C08(0, 0x140, 1, 0, 1, 1.0f, 0.1f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.mTracks[0].m20 = 1.0f;
    }
}

void EnemyFieldMagician::fn_8032D48C() {
    m58C = 9;
    mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.mTracks[0].m20 = 1.0f;
    if (m162C.size() == 0) {
        mPos = m1624->m04;
        mRot.y = m1624->m14;
    } else {
        RoutePlace_unk* place = m162C[m1614];
        mPos = place->mPos;
        mRot.y = place->m14;
        if (m1650 != 0) {
            m1614 = (m1614 + 1) % m162C.size();
        } else if (m1614 + 1 < (int)m162C.size()) {
            m1614++;
        }
    }
    fn_80232B7C(&m15F0, 0);
    PlayEffect_unk(&m15F0, &mMotion);
}

// NONMATCHING: 98.9% - volatile registers of the segment copies; `this`/`msg` moves swapped
void EnemyFieldMagician::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((unsigned int)(m58C - 8) <= 1) {
        msg->m88 = -1;
        msg->m64 = 3;
        msg->m90 = 0;
        msg->m84 = 2;
        return;
    }
    if ((unsigned int)(msg->mData->m04 - 0x1FA) <= 2) {
        return;
    }
    if (m1608 != 0 && msg->mAttacker != 0) {
        if (msg->m44 == 0) {
            atn::Vector3 from(msg->mAttacker->mPos);
            atn::Vector3 to(mPos);
            from.y += 10.0f;
            to.y += 10.0f;
            CollSegResult_unk* result = &lbl_8087EE98->m4008;
            Segment_unk seg(from, to);
            if (m14B0[0].vf10((atn::ColliResult_unk*)result, 0x100, (const atn::ColliCapsuleShape_unk*)&seg, 0, 0) != 0) {
                CharaTask* attacker = msg->mAttacker;
                if (attacker->m55C == 6 && ((unsigned int)(attacker->m560 - 4) <= 1 || attacker->m560 == 9)) {
                    attacker->vf7C(0);
                }
                msg->m88 = -1;
                msg->m64 = 3;
                msg->m90 = 0;
                msg->m84 = 2;
                fn_80232B7C(0, 0);
                fn_8023A680(lbl_8087F3C0, &m15FC, -1, 0, 0, &result->m04, &mRot, 0, 0, -1, 1, 1.0f);
                return;
            }
        } else if (msg->m44 == 2) {
            atn::Vector3 dir(msg->m28);
            dir.y = 0.0f;
            PSVECNormalize(&dir, &dir);
            atn::Vector3 back = Neg_unk(GetFront_unk(mRot));
            if (PSVECDotProduct(&dir, &back) >= 0.0f) {
                bool weak = m160C > 0;
                bool through = false;
                if (msg->mData->m01 == 1 && msg->mData->m74.m08 < 0.1f) {
                    m14B0[0].mFlags &= ~1;
                    m14B0[1].mFlags |= 1;
                    m160C = m1628;
                    msg->m90 = 0;
                    fn_80239DAC(lbl_8087F3C0, &m15C0[0], 0, 1);
                    PlayFieldEffect_unk(1);
                    if (lbl_8087EFE8 != 0) {
                        lbl_8087EFE8->m34D8 = this;
                        fn_800C344C(sFieldSounds[1], &mPos, 0, -1, 1.0f);
                        lbl_8087EFE8->m34D8 = 0;
                    }
                    int flag = m1658;
                    if (flag >= 0 && fn_80370A78(lbl_8087F430, flag) == 0) {
                        fn_80370AE4(lbl_8087F430, flag, 1);
                    }
                }
                if (weak && msg->mData->m01 == 0) {
                    through = true;
                }
                if (!through) {
                    return;
                }
            }
        }
    }
    fn_80151448(msg);
    vf40(hit);
}

void EnemyFieldMagician::vf40(HitInfo_unk* hit) {
    BattleCharaTask::vf40(hit);
    if (m1608 != 0 || m58C == 6) {
        if (m58C >= 6) {
            if (m58C == 7 && m55C == 6 && m560 == 0x16) {
                HitMsg_unk* msg = (HitMsg_unk*)hit;
                if (LengthSq_unk(msg->m10) < 16.0f) {
                    if (LengthSq_unk(msg->m10) < 1e-5f) {
                        msg->m10 = Neg_unk(GetFront_unk(mRot));
                    } else {
                        msg->m10.Normalize_unk();
                    }
                    msg->m10 *= 4.0f;
                }
                fn_8015E7A0(msg->m10, -1, 0);
            }
            m58C = 0;
        }
        if (m1608 != 0) {
            fn_80239DAC(lbl_8087F3C0, &m15C0[0], 0, 0);
            PlayFieldEffect_unk(2);
            fn_800C344C(sFieldSounds[2], &mPos, 0, -1, 1.0f);
            if (lbl_8087EFE8 != 0) {
                lbl_8087EFE8->fn_800CFD18(this, 15);
            }
            int flag = m165C;
            m14B0[0].mFlags &= ~1;
            m14B0[1].mFlags &= ~1;
            m1608 = 0;
            if (flag >= 0 && fn_80370A78(lbl_8087F430, flag) == 0) {
                fn_80370AE4(lbl_8087F430, flag, 1);
            }
        }
    }
    m1618 = 1;
}

bool EnemyFieldMagician::vfC4() {
    return false;
}
