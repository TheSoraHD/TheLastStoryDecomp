#include "game/ChaserGuard.h"
#include "game/GameUI.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
#include "atn/DrawHelper.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" double acos(double x);
extern "C" double fabs(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
// inferred: field (class unknown); m10D8 holds the route tables
struct Field_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x5660 - 0x10DC];
    int m5660;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803C17FC(RouteTable_unk* routes, int id); // inferred: route point by id
extern "C" int fn_80370174(Field_unk* field, int id);         // inferred
extern "C" void* fn_803CC6B4(RouteTable_unk* routes, int id);  // inferred: sound data by id
extern "C" GameUI* lbl_8087F490; // inferred: the HUD (fn_803E3BE8 shows a message)
// inferred: owner of the list the guard registers in (class unknown)
struct ChaserOwner_unk {
    unsigned char m00[0x258];
    void* m258;
};
// inferred: color as four floats (as in gmk/PlayEffect.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" ChaserOwner_unk* lbl_8087FA20;
extern "C" void fn_804AAFCC(void* list, ChaserGuard* guard); // inferred: removes the guard
extern "C" void fn_804AAD68(void* list, ChaserGuard* guard, const Color4f_unk* color); // inferred: adds the guard
// inferred: chase manager (class unknown)
struct ChaseState_unk {
    unsigned char m00[0x48];
    int m48; // inferred: state (0: idle, 3: the guards stop)
    unsigned char m4C[0x58 - 0x4C];
    int m58; // inferred: frames since the chase started
    unsigned char m5C[0x70 - 0x5C];
    ChaserGuard* m70; // inferred: guard that caught the player
    int m74;          // inferred: number of guards
    unsigned char m78[0xF8 - 0x78];
    int mF8;
};
extern "C" ChaseState_unk* lbl_8087F098;
extern "C" CharaTask* fn_80183860(ChaseState_unk* mgr, const atn::Vector3* pos); // inferred: player to chase
extern "C" void fn_80182788(ChaseState_unk* mgr, ChaserGuard* guard, CharaTask* target); // inferred: catch
extern "C" void fn_80182874(ChaseState_unk* mgr); // inferred: release
// inferred: parameters of the chase (game settings + 0x4DC); names keep the settings offsets
struct ChaseParam_unk {
    int m4DC;   // inferred: debug display of the guards
    float m4E0; // inferred: sight range while chasing
    float m4E4; // inferred: sight range
    float m4E8; // inferred: sight angle
    float m4EC; // inferred: catch distance (slow player)
    float m4F0; // inferred: catch distance
    float m4F4; // inferred: hearing distance
    float m4F8; // inferred: head swing speed
    float m4FC; // inferred: speeds of the states
    float m500;
    float m504;
    float m508;
    int m50C;   // inferred: frames of the states
    int m510;
    int m514;
    int m518;
    int m51C;
};
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x4DC];
    ChaseParam_unk mChase; // 0x4DC
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: accessor (the original computes the address of the parameters once and reuses it)
inline ChaseParam_unk* GetChaseParam_unk() {
    return &lbl_8087F0A8->mChase;
}
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: collision world and its ray test (see CharaTask.cpp)
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
// inferred: debug drawing (lbl_8087EEB0), font (lbl_8087EEC8) and camera (lbl_8087EFB4)
extern "C" void* lbl_8087EEC8;
extern "C" void* lbl_8087EFB4;
extern "C" void fn_8006F2F0(void* font, wchar_t* dst, const char* src, int n); // inferred: to wide characters
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg, int arg2, float volume); // inferred: plays a sound
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: front direction for a rotation around y (as in CharaTask.cpp)
inline atn::Vector3 GetFrontOf_unk(Mtx m, CharaTask* chara) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}
inline atn::Vector3 GetFront_unk(Mtx m, float ry) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// inferred: negated vector (the out-of-line atn::Vector3::operator- is not used); name invented
inline atn::Vector3 Negate_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: direction from `pos` to the chased player (name invented)
inline atn::Vector3 DirToPlayer_unk(const atn::Vector3& pos) {
    return fn_80183860(lbl_8087F098, &pos)->mPos - pos;
}

// inferred: names of the states (debug display)
static const char* sStateNames_unk[] = {"START", "CHASE", "DETECT", "CAUGHT", "LOST", "SEARCH", "CLEAR", 0};

// inferred: converts a [0,1] float channel to 0..255 (as in atn/D2A_Line.cpp)
inline unsigned int ToU8_unk(float f) {
    return (f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f);
}
inline unsigned int ToArgb_unk(const Color4f_unk& c) {
    unsigned int r = ToU8_unk(c.r);
    unsigned int g = ToU8_unk(c.g);
    unsigned int b = ToU8_unk(c.b);
    unsigned int a = ToU8_unk(c.a);
    return (a << 24) | (r << 16) | (g << 8) | b;
}
inline void DrawLine_unk(const atn::Vector3& a, const atn::Vector3& b, Color4f_unk color) {
    lbl_8087EEB0->fn_80063764(a, b, 0.0f, ToArgb_unk(color));
}
inline void DrawCircle_unk(const atn::Vector3& pos, float radius, Color4f_unk color) {
    lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(color), pos.x, pos.y, pos.z, 0.0f, radius, 10.0f);
}

// NONMATCHING: 92% - the inlined vector::push_back of the target also evaluates a dead growth
// computation (max((cap + 1) * 3 / 5, 1) and max((cap + 1) / 2, 1), 0x8017CD44) after the length
// check.
ChaserGuard::ChaserGuard(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0.0f, 0.0f, 0.0f), m14D0(0), m14D4(0), m14D8(0),
      m14F8(0), m14FC(0), m1500(1.0f), m1504(0), m1508(0), m1510(0.0f, 0.0f, 0.0f) {
    atn::Csv csv(info->m2C, strlen(info->m2C));
    for (const char* tok = csv.fn_8005B9CC(); tok != 0 && strcmp("", tok) != 0; tok = csv.fn_8005B9CC()) {
        m14EC.push_back(fn_803C17FC(lbl_8087F430->m10D8, fn_800DC12C(tok)));
    }
    m14DC = info->m04;
    m14E8 = info->m14;
    mFlag24 = 0;
    m1454 = 0;
}

ChaserGuard::~ChaserGuard() {
    if (lbl_8087FA20 != 0) {
        fn_804AAFCC(lbl_8087FA20->m258, this);
    }
}

void ChaserGuard::vf10() {
    int state = lbl_8087F098->m48;
    if (state != 0) {
        PlayerTask* player = lbl_8087F8A0->mHead;
        if (player != 0 && (player->mStatus.m0C & 0x20) != 0x20) {
            if (m55C == 6) {
                fn_80139560();
                fn_80145334();
            } else if (state != 3) {
                fn_8017D13C();
                fn_80145334();
            }
        }
    }
}

void ChaserGuard::vf14() {
    fn_80149A30();
}

// inferred: swings the head left and right (names invented)
inline void ChaserGuard::SwingHead_unk(const ChaseParam_unk* param) {
    atn::Vector3 off(0.0f, m1500 * param->m4F8, 0.0f);
    atn::Vector3 rot = mLook.mRot + off;
    mLook.fn_80129954(rot, 0.3f);
    if ((float)__fabs(rot.y) > mLook.mLimit.y) {
        m1500 *= -1.0f;
    }
}

// inferred: the chased player is close enough to be caught (name invented)
inline bool ChaserGuard::IsPlayerNear_unk() {
    CharaTask* player = fn_80183860(lbl_8087F098, &mPos);
    if (player == 0) {
        return false;
    }
    atn::Vector3 d = player->mPos - mPos;
    float dist = PSVECSquareMag(&d);
    bool slow = player->m570 <= 0.6f;
    float range = lbl_8087F0A8->mChase.m4F0;
    if (slow) {
        range = lbl_8087F0A8->mChase.m4EC;
    }
    return dist < range * range;
}

// NONMATCHING: 99.2% - scheduling of the loads before the fan drawing (the rotation is computed
// after the first color load in the target).
void ChaserGuard::fn_8017D13C() {
    mC58.mC4 = lbl_8087F8A0->mHead;
    mC58.m11C.m01 = 0;
    if (lbl_8087F098->m48 == 0 && m14B0 != 3 && m14B0 != 5) {
        fn_8017F4A0(5);
        return;
    }
    if (lbl_8087F098->m48 == 0) {
        m14D0 = 0;
    }
    mColli.mFlags |= 1;
    switch (m14B0) {
    case 0:
        fn_8017F4A0(5);
        break;
    case 5:
        fn_8017F348();
        break;
    case 2:
        mC58.m11C.m01 = 1;
        fn_8017FBEC();
        m14D4 = 0;
        if (++m14D0 > lbl_8087F0A8->mChase.m514) {
            fn_8016EB48(0, 0, 0);
            fn_8017F4A0(1);
        }
        break;
    case 1:
        fn_8017E650();
        break;
    case 3:
        fn_8017E9D8();
        break;
    case 4:
        fn_8017F118();
        break;
    }
    if (m14B0 != 1) {
        m1504 = 0;
    }
    Settings_unk* settings = lbl_8087F0A8;
    float range = settings->mChase.m4E4;
    if (settings->mChase.m4DC != 0) {
        Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
        switch (m14B0) {
        case 1:
        case 2:
            color = Color4f_unk(1.0f, 1.0f, 0.0f, 1.0f);
            range = settings->mChase.m4E0;
            lbl_8087EEB0->fn_80063764((mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), (m14B8 + atn::Vector3(0.0f, 5.0f, 0.0f)), 0.0f, ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(color), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->mChase.m4F4, 10.0f);
            break;
        case 3:
            color = Color4f_unk(1.0f, 0.0f, 0.0f, 1.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(color), mPos.x, mPos.y, mPos.z, 0.0f, settings->mChase.m4F4, 10.0f);
            break;
        case 4:
            color = Color4f_unk(0.0f, 0.0f, 1.0f, 1.0f);
            lbl_8087EEB0->fn_80063764((mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), (m14B8 + atn::Vector3(0.0f, 5.0f, 0.0f)), 0.0f, ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
            lbl_8087EEB0->fn_80063764((mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), (lbl_8087F430->m10D8->mPoints[mC58.m0C.mPointB - 1].mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), 0.0f, ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(Color4f_unk(0.5f, 0.5f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->mChase.m4EC, 10.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(Color4f_unk(0.1f, 0.1f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->mChase.m4F0, 10.0f);
            break;
        case 5:
            color = Color4f_unk(0.0f, 1.0f, 0.0f, 1.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(IsPlayerNear_unk() ? Color4f_unk(1.0f, 0.5f, 0.0f, 1.0f)
                                                      : Color4f_unk(0.5f, 0.5f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->mChase.m4EC, 10.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(IsPlayerNear_unk() ? Color4f_unk(1.0f, 0.5f, 0.0f, 1.0f)
                                                      : Color4f_unk(0.1f, 0.1f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->mChase.m4F0, 10.0f);
            break;
        }
        float rot = mRot.y + mLook.mRot.y;
        lbl_8087EEB0->fn_80063484(12, ToArgb_unk(color), mPos.x, 15.0f + mPos.y, mPos.z, rot, 0.0f, range, lbl_8087F0A8->mChase.m4E8);
        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &(mPos + atn::Vector3(0.0f, 25.0f, 0.0f)));
        if (screen.z > 0.0f && screen.z < 1.0f) {
            wchar_t text[0x100];
            fn_8006F2F0(lbl_8087EEC8, text, sStateNames_unk[m14B0], 0x100);
            lbl_8087EEB0->fn_80061824(screen.x, screen.y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, ToArgb_unk(color), 1, 1, 0, 0, 0xFF000000);
        }
    }
}

// NONMATCHING: 97.1% - stack slots of the push direction temporaries and register allocation.
void ChaserGuard::fn_8017E650() {
    mC58.m11C.m01 = 1;
    m1508++;
    if (fn_8017FBEC()) {
        m14D4 = 0;
        m1504 = 1;
        CharaTask* player = fn_80183860(lbl_8087F098, &mPos);
        if (player == 0 || (player->m55C == 6 && player->m560 == 0x66)) {
            fn_8017F4A0(3);
            const atn::Vector3& dir = (player != 0) ? (mPos - player->mPos).GetNormalize()
                                                    : atn::Vector3(0.0f, 0.0f, 1.0f);
            fn_8015E7A0(dir * 3.0f, 0x38, 0);
            mMotion.mTracks[0].m0C = 0.5f;
            m14B4 = 0;
        } else {
            float range = lbl_8087F0A8->mChase.m4F4;
            if (lbl_8087F098->mF8 != 0) {
                range *= 1.8f;
            }
            range *= range;
            atn::Vector3 d = player->mPos - mPos;
            if (PSVECSquareMag(&d) < range) {
                fn_80182788(lbl_8087F098, this, player);
                fn_8017F4A0(3);
                m14B4 = player;
                return;
            }
        }
    } else {
        m1504 = 0;
        if (++m14D4 > lbl_8087F0A8->mChase.m50C) {
            fn_8017F4A0(4);
            return;
        }
        for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
            if (enemy != this && enemy->CanAct_unk() && enemy->m60->m24 == 0x35 &&
                ((ChaserGuard*)enemy)->m1504 != 0) {
                atn::Vector3 d = enemy->mPos - mPos;
                if (PSVECSquareMag(&d) < 1600.0f) {
                    m14D4 = 0;
                    m14B8 = ((ChaserGuard*)enemy)->m14B8;
                    break;
                }
            }
        }
    }
    mC58.m0C.fn_80128A30(&m14B8, &mPos, 8, 0.0f);
    mC58.m0C.fn_801255C8();
    fn_8017FED0();
}

// NONMATCHING: 98.2% - the target keeps &mPos in r29 for the direction to the player and its frame
// is 0x10 bytes smaller.
void ChaserGuard::fn_8017E9D8() {
    mColli.mFlags &= ~1;
    if (m14B4 != 0) {
        bool caught = m14B4->m560 == 0x64 || m14B4->m560 == 0x65;
        if (caught) {
            m14D4++;
            if (m150C != 0) {
                if (mMotion.mTracks[0].m00 != 0x8C) {
                    atn::Vector3 d = m1510 - mPos;
                    if (PSVECSquareMag(&d) <= 9.0f) {
                        mMotion.m34C = 1;
                        mMotion.mTracks[0].m20 = 1.0f;
                        mMotion.fn_80097C08(0, 0x8C, 1, 0, 1, 0.0f, 0.1f);
                        mMotion.mTracks[0].m0C = 1.5f;
                        m580 = 0.0f;
                        m584 = 0.0f;
                        if (fn_80183860(lbl_8087F098, &mPos) != 0) {
                            atn::Vector3 dir = DirToPlayer_unk(mPos);
                            dir.fn_80139F60();
                            mRot = dir;
                        }
                    } else {
                        atn::Vector3 n = d.GetNormalize();
                        mPos += n * 3.0f;
                        mRot.y = d.ToRotation_unk().y;
                    }
                }
            } else if (mMotion.mTracks[0].m00 == 0x87) {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.fn_80097C08(0, 0x88, 1, 0, 1, 0.0f, 0.1f);
                    mMotion.mTracks[0].m0C = 1.0f;
                }
            }
        } else {
            m14B4 = 0;
            if (lbl_8087F098->m70 == this) {
                fn_80182874(lbl_8087F098);
            }
            if (mMotion.mTracks[0].m00 == 0x88) {
                fn_80161570(0x89, 0, 0, -1.0f, -1.0f);
                mMotion.mTracks[0].m0C = 0.7f;
            } else if (fn_80183860(lbl_8087F098, &mPos) != 0) {
                atn::Vector3 d = mPos - fn_80183860(lbl_8087F098, &mPos)->mPos;
                if (PSVECSquareMag(&d) > 0.01f) {
                    PSVECNormalize(&d, &d);
                    d *= 3.0f;
                } else {
                    d = atn::Vector3(0.0f, 0.0f, 1.0f);
                }
                fn_8015E7A0(d, 0x38, 0);
                mMotion.mTracks[0].m0C = 0.5f;
            }
        }
    }
    if (m14B4 == 0) {
        if (m55C != 2) {
            fn_80139560();
        } else {
            lbl_8087F430->m5660 = 0;
            if (++m14D0 > lbl_8087F0A8->mChase.m518) {
                m14D8 = 0x2D;
                fn_8017F4A0(1);
            }
        }
    }
}

void ChaserGuard::fn_8017F118() {
    mC58.m11C.m01 = 5;
    ChaseParam_unk* param;
    if (fn_8017FBEC()) {
        m14D4 = 0;
        if (++m14D0 > (param = &lbl_8087F0A8->mChase)->m510) {
            fn_8016EB48(0, 0, 0);
            fn_8017F4A0(1);
            return;
        }
    } else {
        m14D0 = 0;
        if (++m14D4 > (param = &lbl_8087F0A8->mChase)->m51C) {
            fn_8016EB48(0, 0, 0);
            m14FC = 1;
            fn_8017F4A0(5);
            return;
        }
    }
    if (mC58.m0C.m2C != 0) {
        if (mMotion.mTracks[0].m00 != 0x45) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x45, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        }
        SwingHead_unk(&lbl_8087F0A8->mChase);
    } else {
        SwingHead_unk(param);
        fn_8017FED0();
    }
}

void ChaserGuard::fn_8017F348() {
    bool seen = fn_8017FBEC();
    if (lbl_8087F098->m48 != 0) {
        if (seen) {
            if (++m14D0 > lbl_8087F0A8->mChase.m510) {
                fn_8017F4A0(2);
                return;
            }
        } else {
            m14D0 = 0;
        }
    }
    if (mC58.m0C.m2C != 0 && m14EC.size() != 0) {
        m14FC = 0;
        m14F8 = (m14F8 + 1) % m14EC.size();
        mC58.m0C.fn_80128508(m14EC[m14F8], &mPos, 8);
        mC58.m0C.fn_801255C8();
    }
    SwingHead_unk(&lbl_8087F0A8->mChase);
    fn_8017FED0();
}

// NONMATCHING: 99.86% - stack slots of the loop temporaries (the front vector is above them in the
// original) and two swapped address registers.
void ChaserGuard::fn_8017F4A0(int state) {
    m14B4 = 0;
    m14D0 = 0;
    m14D4 = 0;
    mLook.fn_80129A48(0.3f);
    switch (state) {
    case 4: {
        mC58.m0C.fn_80128A30(&m14B8, &mPos, 8, 0.0f);
        mC58.m0C.fn_801255C8();
        mC58.mC0 = 0;
        if (fn_80183860(lbl_8087F098, &mPos) != 0) {
            fn_80183860(lbl_8087F098, &mPos)->mC58.mC0--;
        }
        GameUI* player = lbl_8087F490;
        if (player != 0) {
            player->fn_803E3BE8(fn_803CC6B4(lbl_8087F430->m10D8, 0x2714), 0, 10.0f);
        }
        break;
    }
    case 5: {
        if (m14EC.size() != 0) {
            mC58.m0C.fn_80128508(m14EC[m14F8], &mPos, 8);
        } else {
            mC58.m0C.fn_80128A30(&m14DC, &mPos, 8, m14E8);
        }
        mC58.m0C.fn_801255C8();
        GameUI* player = lbl_8087F490;
        if (player != 0 && lbl_8087F098->m48 != 0 && m14B0 == 4) {
            player->fn_803E3BE8(fn_803CC6B4(lbl_8087F430->m10D8, 0x2715), 0, 10.0f);
        }
        break;
    }
    case 3: {
        m150C = 0;
        if (lbl_8087F098->m70 == this) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x87, 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
            m580 = 0.0f;
            m584 = 0.0f;
            GameUI* player = lbl_8087F490;
            player->fn_803E3BE8(fn_803CC6B4(lbl_8087F430->m10D8, 0x2717), 0, 10.0f);
        } else {
            m150C = lbl_8087F098->m70;
            m1510 = mPos;
            if (m150C != 0) {
                float base = 6.2831855f * (float)(lbl_8087F098->m74 - 2) / 8.0f;
                for (int i = 0; i < 8; i++) {
                    Mtx m2;
                    Mtx m1;
                    atn::Vector3 dir = Negate_unk(GetFrontOf_unk(m1, m150C));
                    PSMTXRotRad(m2, 'y', base + 6.2831855f * (float)i / 9.0f);
                    PSMTXMultVec(m2, &dir, &dir);
                    atn::Vector3 to = m150C->mPos + dir * 20.0f;
                    atn::Vector3 from = m150C->mPos;
                    atn::Vector3 end = to;
                    from.y += 5.0f;
                    end.y += 5.0f;
                    CollResult_unk result;
                    if (fn_8004ED34(lbl_8087EE98, &result, &from, &end, 0x80000000, 0, 0) == 0) {
                        m1510 = to - dir * 5.0f;
                        break;
                    }
                }
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.fn_80097C08(0, 0x20, 1, 0, 1, 0.0f, 0.1f);
                mMotion.mTracks[0].m0C = 1.5f;
            }
        }
        CharaTask* target = fn_80183860(lbl_8087F098, &mPos);
        if (mC58.mC0 != 0) {
            mC58.mC0 = 0;
            if (target != 0) {
                target->mC58.mC0--;
            }
        }
        if (target != 0) {
            mLook.fn_80129978(&target->mMotion, "neck", lbl_807C7030, 0.3f);
        }
        break;
    }
    case 2: {
        m12C8 = 10;
        fn_800C31F4("SE_UI001_020", 0, -1, 1.0f);
        CharaTask* target = fn_80183860(lbl_8087F098, &mPos);
        if (target != 0) {
            mLook.fn_80129978(&target->mMotion, "neck", lbl_807C7030, 0.3f);
        }
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        m1508 = 0;
        Color4f_unk color(1.0f, 0.0f, 0.0f, 1.0f);
        fn_804AAD68(lbl_8087FA20->m258, this, &color);
        break;
    }
    case 1: {
        if (m14B0 != 2) {
            m12C8 = 10;
        }
        mC58.mC0 = 1;
        CharaTask* target = fn_80183860(lbl_8087F098, &mPos);
        if (target != 0) {
            target->mC58.mC0++;
            mLook.fn_80129978(&target->mMotion, "neck", lbl_807C7030, 0.3f);
        }
        mC58.m0C.fn_80128A30(&m14B8, &mPos, 8, 0.0f);
        mC58.m0C.fn_801255C8();
        GameUI* player = lbl_8087F490;
        if (player != 0) {
            if (m14B0 == 3) {
                player->fn_803E3BE8(fn_803CC6B4(lbl_8087F430->m10D8, 0x2712), 0, 10.0f);
            } else {
                player->fn_803E3BE8(fn_803CC6B4(lbl_8087F430->m10D8, 0x2711), 0, 10.0f);
            }
        }
        break;
    }
    }
    m14B0 = state;
}

// NONMATCHING: 99.9% - the address registers of the copy into m14C4 are swapped (r4/r5).
bool ChaserGuard::fn_8017FBEC() {
    CharaTask* player = fn_80183860(lbl_8087F098, &mPos);
    if (player == 0) {
        return false;
    }
    float dist = PSVECSquareMag(&(player->mPos - mPos));
    float range = lbl_8087F0A8->mChase.m4E4;
    if (m14B0 == 1) {
        range = lbl_8087F0A8->mChase.m4E0;
    }
    if (dist < range * range) {
        atn::Vector3 up(0.0f, 20.0f, 0.0f);
        bool seen = false;
        atn::Vector3 eye = mPos + up;
        atn::Vector3 target = player->mPos + up;
        atn::Vector3 dir = (target - eye).GetNormalize();
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y + mLook.mRot.y);
        PSMTXMultVec(m, &front, &front);
        float angle = acos(PSVECDotProduct(&front.GetNormalize(), &dir));
        if (angle < 0.5f * lbl_8087F0A8->mChase.m4E8) {
            CollResult_unk result;
            seen = fn_8004ED34(lbl_8087EE98, &result, &eye, &target, 0x80000000, 0, 0) == 0;
            if (lbl_8087F0A8->mChase.m4DC != 0) {
                if (!seen) {
                    lbl_8087EEB0->fn_80063764(eye, result.m04, 0.0f, 0xFFFFFF00);
                    lbl_8087EEB0->fn_80063764(result.m04, target, 0.0f, 0xFF4D4D00);
                } else {
                    lbl_8087EEB0->fn_80063764(eye, target, 0.0f, 0xFF4D4D00);
                }
            }
        }
        if (seen) {
            m14B8 = player->mPos;
            m14C4 = mPos - m14B8;
        }
        return seen;
    }
    return false;
}

// NONMATCHING: 99.46% - the target keeps &mPos in r30 for the direction to the player.
void ChaserGuard::fn_8017FED0() {
    RouteWalker_unk& route = mC58.m0C;
    route.fn_80126214();
    atn::Vector3 dir = route.m58;
    float look = 1.0f;
    atn::Vector3 rot = mRot;
    if (mC58.m0C.m2C == 0) {
        if (PSVECSquareMag(&dir) > 0.01f) {
            PSVECNormalize(&dir, &dir);
            rot = dir.ToRotation_unk();
        }
    } else {
        look = 0.0f;
    }
    if (m14B0 - 4 <= 1u && IsPlayerNear_unk() && fn_80183860(lbl_8087F098, &mPos) != 0) {
        rot = DirToPlayer_unk(mPos).ToRotationZ_unk();
    }
    float speed = lbl_8087F0A8->mChase.m500;
    if (m14B0 == 5) {
        if (m14FC != 0) {
            speed = lbl_8087F0A8->mChase.m508;
        } else {
            speed = lbl_8087F0A8->mChase.m4FC;
        }
    } else if (m14B0 == 4) {
        speed = lbl_8087F0A8->mChase.m504;
    } else if (m14B0 == 1) {
        if (m1508 < 450) {
            speed *= 1.15f;
        } else if (m1508 < 750) {
            speed *= 1.1f;
        } else if (m1508 >= 1800) {
            if (m1508 < 2700) {
                speed *= 0.9f;
            } else {
                speed *= 0.8f;
            }
        }
        if (m1504 == 0) {
            speed *= 0.9f;
        }
        if (m14D8 > 0) {
            m14D8--;
            speed *= 0.85f;
        }
        bool hard = fn_80370174(lbl_8087F430, 0xD4) == 1;
        if (hard) {
            speed *= 0.9f;
        }
        int time = lbl_8087F098->m58;
        if (time > 4500) {
            speed *= hard ? 0.5f : 0.7f;
        } else if (time > 3600) {
            speed *= hard ? 0.65f : 0.8f;
        } else if (time > 2700) {
            speed *= hard ? 0.8f : 0.9f;
        }
    }
    vf34(rot, 0, look, speed);
}
