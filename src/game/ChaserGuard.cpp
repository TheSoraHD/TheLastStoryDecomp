#include "game/ChaserGuard.h"
#include "atn/RenderManager.h"
#include "game/GameScene.h"
#include "game/GameUI.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
#include "atn/DrawHelper.h"
#include "game/TownTask.h"
#include "game/GameTask.h"
#include "math/VecAssign_unk.h"
#include "game/MiniMapTask.h"
#include "atn/ColliBase.h"
#include "game/GameSceneFind_unk.h"
#include "game/GameConfig_unk.h"
#include "game/ChaseManager.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" double acos(double x);
extern "C" double fabs(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" GameTask* lbl_8087F430;
extern "C" GameUI* lbl_8087F490; // inferred: the HUD (fn_803E3BE8 shows a message)
// inferred: color as four floats (as in gmk/PlayEffect.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" ChaseManager* lbl_8087F098;
extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: accessor (the original computes the address of the parameters once and reuses it)
inline GameConfig4DC_unk* GetChaseParam_unk() {
    return &lbl_8087F0A8->m4DC;
}
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;
struct ColliManager_unk;
extern "C" ColliManager_unk* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, atn::ColliResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: debug drawing (lbl_8087EEB0), font (lbl_8087EEC8) and camera (atn::RenderManager::instance_)
namespace atn {
class Font_unk;
}
extern "C" atn::Font_unk* lbl_8087EEC8;
extern "C" void fn_8006F2F0(void* font, wchar_t* dst, const char* src, int n); // inferred: to wide characters
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
    return lbl_8087F098->fn_80183860(&pos)->mPos - pos;
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
        m14EC.push_back(((GameScene*)lbl_8087F430->m10D8)->fn_803C17FC(fn_800DC12C(tok)));
    }
    m14DC = info->m04;
    m14E8 = info->m14;
    mFlag24 = 0;
    m1454 = 0;
}

ChaserGuard::~ChaserGuard() {
    if (lbl_8087FA20 != 0) {
        ((MiniMapTask*)lbl_8087FA20->m258)->fn_804AAFCC(this);
    }
}

void ChaserGuard::vf10() {
    int state = lbl_8087F098->mState;
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
inline void ChaserGuard::SwingHead_unk(const GameConfig4DC_unk* param) {
    atn::Vector3 off(0.0f, m1500 * param->mScanRate, 0.0f);
    atn::Vector3 rot = mLook.mRot + off;
    mLook.fn_80129954(rot, 0.3f);
    if ((float)__fabs(rot.y) > mLook.mLimit.y) {
        m1500 *= -1.0f;
    }
}

// inferred: the chased player is close enough to be caught (name invented)
inline bool ChaserGuard::IsPlayerNear_unk() {
    CharaTask* player = lbl_8087F098->fn_80183860(&mPos);
    if (player == 0) {
        return false;
    }
    atn::Vector3 d = player->mPos - mPos;
    float dist = PSVECSquareMag(&d);
    bool slow = player->m570 <= 0.6f;
    float range = lbl_8087F0A8->m4DC.mHearRun;
    if (slow) {
        range = lbl_8087F0A8->m4DC.mHearWalk;
    }
    return dist < range * range;
}

// NONMATCHING: 99.2% - scheduling of the loads before the fan drawing (the rotation is computed
// after the first color load in the target).
void ChaserGuard::fn_8017D13C() {
    mC58.mC4 = lbl_8087F8A0->mHead;
    mC58.m11C.m01 = 0;
    if (lbl_8087F098->mState == 0 && m14B0 != 3 && m14B0 != 5) {
        fn_8017F4A0(5);
        return;
    }
    if (lbl_8087F098->mState == 0) {
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
        if (++m14D0 > lbl_8087F0A8->m4DC.mChaseDelay) {
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
    GameConfig_unk* settings = lbl_8087F0A8;
    float range = settings->m4DC.mVisSearchDistance;
    if (settings->m4DC.m00 != 0) {
        Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
        switch (m14B0) {
        case 1:
        case 2:
            color = Color4f_unk(1.0f, 1.0f, 0.0f, 1.0f);
            range = settings->m4DC.mVisChaseDistance;
            lbl_8087EEB0->fn_80063764((mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), (m14B8 + atn::Vector3(0.0f, 5.0f, 0.0f)), 0.0f, ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(color), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->m4DC.mCatchDistance, 10.0f);
            break;
        case 3:
            color = Color4f_unk(1.0f, 0.0f, 0.0f, 1.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(color), mPos.x, mPos.y, mPos.z, 0.0f, settings->m4DC.mCatchDistance, 10.0f);
            break;
        case 4:
            color = Color4f_unk(0.0f, 0.0f, 1.0f, 1.0f);
            lbl_8087EEB0->fn_80063764((mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), (m14B8 + atn::Vector3(0.0f, 5.0f, 0.0f)), 0.0f, ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
            lbl_8087EEB0->fn_80063764((mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), (lbl_8087F430->m10D8->m98.mData[mC58.m0C.mPointB - 1].mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), 0.0f, ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(Color4f_unk(0.5f, 0.5f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->m4DC.mHearWalk, 10.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(Color4f_unk(0.1f, 0.1f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->m4DC.mHearRun, 10.0f);
            break;
        case 5:
            color = Color4f_unk(0.0f, 1.0f, 0.0f, 1.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(IsPlayerNear_unk() ? Color4f_unk(1.0f, 0.5f, 0.0f, 1.0f)
                                                      : Color4f_unk(0.5f, 0.5f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->m4DC.mHearWalk, 10.0f);
            lbl_8087EEB0->fn_80062C8C(12, ToArgb_unk(IsPlayerNear_unk() ? Color4f_unk(1.0f, 0.5f, 0.0f, 1.0f)
                                                      : Color4f_unk(0.1f, 0.1f, 0.0f, 1.0f)), mPos.x, mPos.y, mPos.z, 0.0f, lbl_8087F0A8->m4DC.mHearRun, 10.0f);
            break;
        }
        float rot = mRot.y + mLook.mRot.y;
        lbl_8087EEB0->fn_80063484(12, ToArgb_unk(color), mPos.x, 15.0f + mPos.y, mPos.z, rot, 0.0f, range, lbl_8087F0A8->m4DC.mVisAngle);
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(mPos + atn::Vector3(0.0f, 25.0f, 0.0f));
        if (screen.z > 0.0f && screen.z < 1.0f) {
            wchar_t text[0x100];
            fn_8006F2F0(lbl_8087EEC8, text, sStateNames_unk[m14B0], 0x100);
            lbl_8087EEB0->fn_80061824(screen.x, screen.y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, ToArgb_unk(color), 1, 1, 0, 0, 0xFF000000);
        }
    }
}

// NONMATCHING: 99.89% - r3/r4 of ++m14D4 (the original increments in place). 97.1 -> 99.89 with the push
// vector as a named copy scaled in place (`*=`) and the distances in SqDist_unk (its d is an inline local).
// inferred: squared distance of two points (name invented)
static inline float SqDist_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 d = a - b;
    return PSVECSquareMag(&d);
}
void ChaserGuard::fn_8017E650() {
    mC58.m11C.m01 = 1;
    m1508++;
    if (fn_8017FBEC()) {
        m14D4 = 0;
        m1504 = 1;
        CharaTask* player = lbl_8087F098->fn_80183860(&mPos);
        if (player == 0 || (player->m55C == 6 && player->m560 == 0x66)) {
            fn_8017F4A0(3);
            atn::Vector3 push = (player != 0) ? (mPos - player->mPos).GetNormalize() : atn::Vector3(0.0f, 0.0f, 1.0f);
            push *= 3.0f;
            fn_8015E7A0(push, 0x38, 0);
            mMotion.mTracks[0].m0C = 0.5f;
            m14B4 = 0;
        } else {
            float range = lbl_8087F0A8->m4DC.mCatchDistance;
            if (lbl_8087F098->mCaught != 0) {
                range *= 1.8f;
            }
            range *= range;
            if (SqDist_unk(player->mPos, mPos) < range) {
                lbl_8087F098->fn_80182788(this, player);
                fn_8017F4A0(3);
                m14B4 = player;
                return;
            }
        }
    } else {
        m1504 = 0;
        if (++m14D4 > lbl_8087F0A8->m4DC.mLoseTime) {
            fn_8017F4A0(4);
            return;
        }
        for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
            if (enemy != this && enemy->CanAct_unk() && enemy->m60->m24 == 0x35 &&
                ((ChaserGuard*)enemy)->m1504 != 0) {
                if (SqDist_unk(enemy->mPos, mPos) < 1600.0f) {
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
                        if (lbl_8087F098->fn_80183860(&mPos) != 0) {
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
            if (lbl_8087F098->mCatcher == this) {
                lbl_8087F098->fn_80182874();
            }
            if (mMotion.mTracks[0].m00 == 0x88) {
                fn_80161570(0x89, 0, 0, -1.0f, -1.0f);
                mMotion.mTracks[0].m0C = 0.7f;
            } else if (lbl_8087F098->fn_80183860(&mPos) != 0) {
                atn::Vector3 d = mPos - lbl_8087F098->fn_80183860(&mPos)->mPos;
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
            if (++m14D0 > lbl_8087F0A8->m4DC.mCatchDelay) {
                m14D8 = 0x2D;
                fn_8017F4A0(1);
            }
        }
    }
}

void ChaserGuard::fn_8017F118() {
    mC58.m11C.m01 = 5;
    GameConfig4DC_unk* param;
    if (fn_8017FBEC()) {
        m14D4 = 0;
        if (++m14D0 > (param = &lbl_8087F0A8->m4DC)->mFindTime) {
            fn_8016EB48(0, 0, 0);
            fn_8017F4A0(1);
            return;
        }
    } else {
        m14D0 = 0;
        if (++m14D4 > (param = &lbl_8087F0A8->m4DC)->mWaitTime) {
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
        SwingHead_unk(&lbl_8087F0A8->m4DC);
    } else {
        SwingHead_unk(param);
        fn_8017FED0();
    }
}

void ChaserGuard::fn_8017F348() {
    bool seen = fn_8017FBEC();
    if (lbl_8087F098->mState != 0) {
        if (seen) {
            if (++m14D0 > lbl_8087F0A8->m4DC.mFindTime) {
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
    SwingHead_unk(&lbl_8087F0A8->m4DC);
    fn_8017FED0();
}

// inferred: marks a chara on the minimap of the town (m258, name invented)
inline void MarkChara_unk(CharaTask* chara, const atn::Color& color) {
    ((MiniMapTask*)lbl_8087FA20->m258)->fn_804AAD68(chara, &color);
}

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
        if (lbl_8087F098->fn_80183860(&mPos) != 0) {
            lbl_8087F098->fn_80183860(&mPos)->mC58.mC0--;
        }
        GameUI* player = lbl_8087F490;
        if (player != 0) {
            player->fn_803E3BE8(((GameScene*)lbl_8087F430->m10D8)->fn_803CC6B4(0x2714), 0, 10.0f);
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
        if (player != 0 && lbl_8087F098->mState != 0 && m14B0 == 4) {
            player->fn_803E3BE8(((GameScene*)lbl_8087F430->m10D8)->fn_803CC6B4(0x2715), 0, 10.0f);
        }
        break;
    }
    case 3: {
        m150C = 0;
        if (lbl_8087F098->mCatcher == this) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x87, 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
            m580 = 0.0f;
            m584 = 0.0f;
            GameUI* player = lbl_8087F490;
            player->fn_803E3BE8(((GameScene*)lbl_8087F430->m10D8)->fn_803CC6B4(0x2717), 0, 10.0f);
        } else {
            m150C = lbl_8087F098->mCatcher;
            m1510 = mPos;
            if (m150C != 0) {
                float base = 6.2831855f * (float)((int)lbl_8087F098->mGuards.size() - 2) / 8.0f;
                for (int i = 0; i < 8; i++) {
                    Mtx m2;
                    Mtx m1;
                    atn::Vector3 dir = Negate_unk(GetFrontOf_unk(m1, m150C));
                    PSMTXRotRad(m2, 'y', base + 6.2831855f * (float)i / 9.0f);
                    PSMTXMultVec(m2, &dir, &dir);
                    atn::Vector3 to = m150C->mPos + dir * 20.0f;
                    atn::Vector3 from = m150C->mPos;
                    atn::Vector3 end;
                    atn::VecAssign_unk(end, to);
                    from.y += 5.0f;
                    end.y += 5.0f;
                    atn::ColliResult_unk result;
                    if (fn_8004ED34(lbl_8087EE98, &result, &from, &end, 0x80000000, 0, 0) == 0) {
                        atn::VecAssign_unk(m1510, to - dir * 5.0f);
                        break;
                    }
                }
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.fn_80097C08(0, 0x20, 1, 0, 1, 0.0f, 0.1f);
                mMotion.mTracks[0].m0C = 1.5f;
            }
        }
        CharaTask* target = lbl_8087F098->fn_80183860(&mPos);
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
        CharaTask* target = lbl_8087F098->fn_80183860(&mPos);
        if (target != 0) {
            mLook.fn_80129978(&target->mMotion, "neck", lbl_807C7030, 0.3f);
        }
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        m1508 = 0;
        MarkChara_unk(this, atn::Color(1.0f, 0.0f, 0.0f, 1.0f));
        break;
    }
    case 1: {
        if (m14B0 != 2) {
            m12C8 = 10;
        }
        mC58.mC0 = 1;
        CharaTask* target = lbl_8087F098->fn_80183860(&mPos);
        if (target != 0) {
            target->mC58.mC0++;
            mLook.fn_80129978(&target->mMotion, "neck", lbl_807C7030, 0.3f);
        }
        mC58.m0C.fn_80128A30(&m14B8, &mPos, 8, 0.0f);
        mC58.m0C.fn_801255C8();
        GameUI* player = lbl_8087F490;
        if (player != 0) {
            if (m14B0 == 3) {
                player->fn_803E3BE8(((GameScene*)lbl_8087F430->m10D8)->fn_803CC6B4(0x2712), 0, 10.0f);
            } else {
                player->fn_803E3BE8(((GameScene*)lbl_8087F430->m10D8)->fn_803CC6B4(0x2711), 0, 10.0f);
            }
        }
        break;
    }
    }
    m14B0 = state;
}


bool ChaserGuard::fn_8017FBEC() {
    CharaTask* player = lbl_8087F098->fn_80183860(&mPos);
    if (player == 0) {
        return false;
    }
    float dist = PSVECSquareMag(&(player->mPos - mPos));
    float range = lbl_8087F0A8->m4DC.mVisSearchDistance;
    if (m14B0 == 1) {
        range = lbl_8087F0A8->m4DC.mVisChaseDistance;
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
        if (angle < 0.5f * lbl_8087F0A8->m4DC.mVisAngle) {
            atn::ColliResult_unk result;
            seen = fn_8004ED34(lbl_8087EE98, &result, &eye, &target, 0x80000000, 0, 0) == 0;
            if (lbl_8087F0A8->m4DC.m00 != 0) {
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
            atn::VecAssign_unk(m14C4, mPos - m14B8);
        }
        return seen;
    }
    return false;
}

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
    if (m14B0 - 4 <= 1u && IsPlayerNear_unk() && lbl_8087F098->fn_80183860(&mPos) != 0) {
        const atn::Vector3& pos = mPos;
        atn::Vector3 to = DirToPlayer_unk(pos);
        rot = to.ToRotationZ_unk();
    }
    float speed = lbl_8087F0A8->m4DC.mRunSpeed;
    if (m14B0 == 5) {
        if (m14FC != 0) {
            speed = lbl_8087F0A8->m4DC.mReturnSpeed;
        } else {
            speed = lbl_8087F0A8->m4DC.mWalkSpeed;
        }
    } else if (m14B0 == 4) {
        speed = lbl_8087F0A8->m4DC.mLostSpeed;
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
        bool hard = lbl_8087F430->fn_80370174(0xD4) == 1;
        if (hard) {
            speed *= 0.9f;
        }
        if (lbl_8087F098->mFrames > 4500) {
            speed *= hard ? 0.5f : 0.7f;
        } else if (lbl_8087F098->mFrames > 3600) {
            speed *= hard ? 0.65f : 0.8f;
        } else if (lbl_8087F098->mFrames > 2700) {
            speed *= hard ? 0.8f : 0.9f;
        }
    }
    vf34(rot, 0, look, speed);
}
