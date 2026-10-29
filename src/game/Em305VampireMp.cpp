#include "game/Em305VampireMp.h"
#include "atn/Csv.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" double strtod(const char* str);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field (lbl_8087F430); m10D8: route tables
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" int fn_800CB58C(atn::SoundRef* sound);                       // inferred: nonzero while playing
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
// inferred: arena of the multiplayer mode (lbl_8087F490); fn_803E6850 shows the arrows left
struct MpArena_unk;
extern "C" MpArena_unk* lbl_8087F490;
extern "C" void fn_803E6850(MpArena_unk* arena, CharaTask* chara, int count, int max, int arg);
// inferred: multiplayer session (lbl_8087F610)
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);
extern "C" void fn_804DA4A0(MpSession_unk* session, int arg);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);
// inferred: collision world and its ray test (as in game/EnemySeaLizerd.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

// inferred: float color converted to GXColor (as in game/GimmickCommon.cpp)
struct ColorF_unk {
    ColorF_unk() {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

// inferred: names invented
inline float HpRatio_unk(CharaStatus* status) {
    return status->m04 / status->mParamB.m84;
}
inline bool HasTarget_unk(CharaTask* chara) {
    bool result = true;
    if (chara->mC58.mC0 == 0 || chara->mC58.mC4 == 0) {
        result = false;
    }
    return result;
}
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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

// NONMATCHING: 96.7% - load order of the flag words and the vector sizes, and the registers of the
// second std::string constructor (as in game/Em201ZanGurg.cpp)
Em305VampireMp::Em305VampireMp(atn::Task* parent, EnemyInfo_unk* info) : Em305Vampire(parent, info), m162C(0) {
    mFlag22 = 1;
    mFlag21 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m14DC.clear();
    m14E8.clear();
    std::string dir("database/chara/special/");
    std::string name("db_em305_00_00.csv");
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
    m14D4.vf0C(path.c_str());
    m1564.fn_8023780C("/effect/ef_uc031.efp");
    m1570.fn_8023780C("/effect/ef_uc007.efp");
    m157C.fn_8023780C("/effect/ef_sk021a.efp");
    m1588.fn_8023780C("/effect/ef_sk025.efp");
}

Em305VampireMp::~Em305VampireMp() {}

// NONMATCHING: 94.0% - the inlined std::vector push_back of the two routes (known open case: the
// original fast path of the 16-byte elements has no placement-new null check) and registers
int Em305VampireMp::vf0C() {
    if (m1564.fn_80237874() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1570.fn_80237874() == 0 &&
        m157C.fn_80237874() == 0 && m1588.fn_80237874() == 0 && m14D4.fn_80473F50() == 0 &&
        fn_8013655C() == 0) {
        fn_802EC38C();
        if (m14D4.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m14D4.fn_80470580(), m14D4.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "weaken_route") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m14E8.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "door_route") == 0) {
                    int a = fn_800DC12C(csv.fn_8005B9CC());
                    int b = fn_800DC12C(csv.fn_8005B9CC());
                    int c = fn_800DC12C(csv.fn_8005B9CC());
                    int d = fn_800DC12C(csv.fn_8005B9CC());
                    m14DC.push_back(VampireDoor_unk(a, b, c, d));
                }
                if (strcmp(key, "weaken_time") == 0) {
                    m15E4[0] = atoi(csv.fn_8005B9CC());
                    m15E4[1] = atoi(csv.fn_8005B9CC());
                    m15E4[2] = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "weaken_speed") == 0) {
                    m15D8[0] = strtod(csv.fn_8005B9CC());
                    m15D8[1] = strtod(csv.fn_8005B9CC());
                    m15D8[2] = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "speed_up_value") == 0) {
                    m15F8 = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "speed_up_time") == 0) {
                    m15FC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "speed_up_wait") == 0) {
                    m1600 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "weaken_1to2") == 0) {
                    m15F0 = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "weaken_2to3") == 0) {
                    m15F4 = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "normal_wait_time") == 0) {
                    m15A8 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "through_wait_time") == 0) {
                    m15AC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "silver_arrow_count") == 0) {
                    m1604 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "stun_time") == 0) {
                    m15B8 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "tackle_skill") == 0) {
                    for (int i = 0; i < 3; i++) {
                        m1608[i] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    }
                } else if (strcmp(key, "rolling_skill") == 0) {
                    for (int i = 0; i < 3; i++) {
                        m1614[i] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    }
                } else if (strcmp(key, "shine_skill") == 0) {
                    for (int i = 0; i < 3; i++) {
                        m1620[i] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    }
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x1C011;
        mStatus.m18 |= 0x3000000;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        m15D4 = 2.0f;
        return 1;
    }
    return 0;
}

void Em305VampireMp::vf10() {
    m14F4 = mC58.mC4;
    if (!mFlag2 || !CanAct_unk()) {
        if (CanAct_unk() && m58C != 0 && vfF8() == 0) {
            vf100(0);
        }
        if (m162C > 0) {
            m162C--;
        }
        float ratio = HpRatio_unk(&mStatus);
        if (ratio < 0.5f) {
            m150C = m1608[2];
            m1514 = m1614[2];
            m1510 = m1620[2];
            m1538 = 0.7f;
        } else if (ratio < 0.8f) {
            m150C = m1608[1];
            m1514 = m1614[1];
            m1510 = m1620[1];
            m1538 = 0.8f;
        } else {
            m150C = m1608[0];
            m1514 = m1614[0];
            m1510 = m1620[0];
            m1538 = 1.0f;
        }
        if (m162C > 0) {
            m1538 *= 0.3f;
        }
        vf104();
    }
    if (m1528 == 0) {
        fn_803E6850(lbl_8087F490, this, m1604 - m1540, m1604, 0);
    }
    if (!CanAct_unk()) {
        fn_800CB5C8(&m15A0, 5, 0);
        fn_800CB5C8(&m15A4, 5, 0);
    } else {
        if (m58C == 7 && m14F8 == 1) {
            fn_800CB5C8(&m15A0, 5, 0);
            if (fn_800CB58C(&m15A4) == 0) {
                m15A4 = fn_800C344C("SE_EM305_002", &mPos, 0, -1, 1.0f);
            }
        } else {
            fn_800CB5C8(&m15A4, 5, 0);
            if (fn_800CB58C(&m15A0) == 0) {
                m15A0 = fn_800C344C("SE_EM305_001", &mPos, 0, -1, 1.0f);
            }
        }
        fn_800CB6E4(&m15A0, &mPos);
        fn_800CB6E4(&m15A4, &mPos);
    }
    fn_8014C540();
    fn_80145334();
}

void Em305VampireMp::vfFC() {
    fn_802EBABC();
}

void Em305VampireMp::vf104() {
    if (m1524 > 0 && m58C != 15) {
        m1524--;
    }
    if (m1528 != 0) {
        if (m1530 == 1) {
            m152C = 0.0f;
            if (++m1534 >= m1600 * 30) {
                m1530 = 2;
                m1534 = 0;
            }
        } else if (m1530 == 2) {
            m152C = m15F8;
            if (++m1534 >= m15FC * 30) {
                m1530 = 0;
                m1534 = 0;
            }
        } else {
            m152C = 0.0f;
        }
    }
    if (!HasTarget_unk(this)) {
        fn_8016E970(3);
    } else if (m14F4 != 0) {
        switch (m58C) {
        case 6:
            fn_802EAC70();
            break;
        case 7:
            fn_802EAF68();
            break;
        case 8:
            fn_802EB1F4();
            break;
        case 13:
            fn_802EB61C();
            break;
        case 14:
            fn_802EB814();
            break;
        case 15:
            fn_802EB97C();
            break;
        case 2:
            vfEC();
            break;
        default:
            fn_802EABE4();
            vf11C();
            break;
        }
    }
    m14FC++;
}

void Em305VampireMp::vf100(int state) {
    if (mC58.mC4 != 0) {
        atn::Vector3 pos = mC58.mC4->mPos;
        atn::Vector3 dir = pos - mPos;
        dir.Normalize_unk();
        switch (m58C) {
        case 6:
            fn_802EBB14();
            break;
        case 7:
            fn_802EBBA8();
            break;
        case 8:
            break;
        case 13:
            fn_802EBF40();
            break;
        case 14:
            fn_802EC098();
            break;
        case 2:
            fn_802EC280();
            break;
        default:
            fn_802EBABC();
            if (m55C != 6) {
                vf11C();
            }
            break;
        }
    }
}

void Em305VampireMp::vf14() {
    if (m1528 != 0) {
        mMotion.m40 = m15C4;
        mMotion.fn_8009373C("op_02_cloud", 0);
    } else {
        ColorF_unk red;
        atn::Model::Float4_unk white;
        white.m00 = 1.0f;
        white.m04 = 1.0f;
        white.m08 = 1.0f;
        white.m0C = 1.0f;
        mMotion.m40 = white;
        mMotion.fn_8009373C("op_02_cloud", 1);
        if (HpRatio_unk(&mStatus) < 0.5f) {
            red.r = 1.0f;
            red.g = 0.1f;
            red.b = 0.1f;
            red.a = 1.0f;
            if (mMotion.fn_80092954("em305_body") != 0) {
                mMotion.fn_80092954("em305_body")->mColor = red.ToGX();
            }
            if (mMotion.fn_80092954("em305_option") != 0) {
                mMotion.fn_80092954("em305_option")->mColor = red.ToGX();
            }
        }
    }
    fn_80149A30();
}

void Em305VampireMp::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m1524 <= 0 && msg->mData->m04 != 0x134) {
        msg->m68 = 0;
        msg->m88 = msg->mData->mC4;
        msg->m84 = 1;
        msg->m90 = 1;
        if (!mFlag2) {
            vf40(hit);
        } else {
            vf118(hit);
        }
        return;
    }
    msg->m10 *= 0.0f;
    if (msg->mData->m04 == 0x134) {
        msg->m0C |= 0x800;
    }
    fn_80151448(msg);
    if (!mFlag2) {
        vf40(hit);
    } else {
        vf118(hit);
    }
}

void Em305VampireMp::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802EC280();
        return;
    }
    if (msg->mData->m04 == 0x134) {
        if (m1524 > 0) {
            fn_802EC160();
        } else if (++m1540 >= m1604) {
            fn_802EBF40();
            m1540 = 0;
        }
    } else if (HpRatio_unk(&mStatus) < 0.85f && m1530 == 0) {
        m1530 = 1;
        m1534 = 0;
    }
}

void Em305VampireMp::vf110(SpecialState_unk* state) {
    state->m00 = m58C;
    state->m04 = m14BC;
    state->m08 = m1528;
    state->m0C = m1524;
    state->m0A = m1540;
    state->m0B = m14F8;
}

void Em305VampireMp::vf114(const SpecialState_unk* state) {
    if (state->m00 == 13 && m58C != 13) {
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1588, &mMotion);
        if (lbl_8087F610 != 0) {
            fn_804DA48C(lbl_8087F610, 4);
            fn_804DA4A0(lbl_8087F610, 0);
        }
    }
    if (state->m00 == 14 && m58C != 14) {
        if (lbl_8087F610 != 0) {
            fn_804DA48C(lbl_8087F610, 5);
            fn_804DA4A0(lbl_8087F610, 1);
        }
    }
    if (state->m00 == 15 && m58C != 15) {
        fn_80232B7C(this, 100);
        PlayEffect_unk(&m1594, &mMotion);
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 pos = fn_801781B0();
        fn_80108F38(mgr, &pos, 0x80);
    }
    if (state->m00 != 15 && m58C == 15) {
        fn_80239DAC(lbl_8087F3C0, this, 100, 0);
    }
    m58C = state->m00;
    m14BC = state->m04;
    m1528 = state->m08;
    m1524 = state->m0C;
    m1540 = state->m0A;
    m14F8 = state->m0B;
}

void Em305VampireMp::vf11C() {
    if (m1500 == 0 && m14F4 != 0) {
        if (m14FC < m15A8) {
            return;
        }
        atn::Vector3 pos = m14F4->mPos;
        atn::Vector3 d = pos - mPos;
        d.y = 0.0f;
        if (d.Length_unk() < 40.0f) {
            fn_802EBB14();
        } else {
            atn::Vector3 from = mPos;
            atn::Vector3 to = m14F4->mPos;
            from.y += 3.0f;
            to.y += 3.0f;
            CollResult_unk result;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, &mColli, 0) == 0) {
                atn::Vector3 dir = to - from;
                dir.Normalize_unk();
                mRot.y = dir.ToRotation_unk().y;
                fn_802EBBA8();
            }
        }
    }
    if (m1500 == 1) {
        if (m1524 <= 0) {
            fn_802EC098();
        } else {
            RoutePlace_unk* place = m14E8[m1560];
            atn::Vector3 d(mPos.x - place->mPos.x, 0.0f, mPos.z - place->mPos.z);
            if (fn_805F9920(&d) < 100.0f) {
                m1560++;
                if (m1560 >= (int)m14E8.size()) {
                    m1560 = 0;
                }
                fn_8017039C(m14E8[m1560]->mId, 0);
            }
        }
    }
}

void Em305VampireMp::vf108() {}
