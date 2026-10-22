#include "game/ChaserNpc.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
#include "macros.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int rand();
extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings (parameters of the chased character at 0x53C)
struct Settings_unk {
    unsigned char m00[0x53C];
    int m53C;   // inferred: debug display
    float m540; // inferred: walk speed
    float m544; // inferred: minimum run speed
    float m548; // inferred: maximum run speed
    float m54C; // inferred: speed change per frame
    int m550;   // inferred: frames out of sight before walking again
    float m554; // inferred: distance beyond which the player can be lost
    float m558; // inferred: distance at which the NPC starts running (player seen)
    float m55C; // inferred: same, player not seen
    float m560; // inferred: distance at which the NPC stops at a place
    float m564; // inferred: distance at which the player catches the NPC
    int m568;   // inferred: frames before the NPC can be caught again
    float m56C; // inferred: sight range
    float m570; // inferred: sight angle
    float m574; // inferred: slow down per period of m578 frames
    int m578;
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: field (class unknown); m10D8 holds the route tables
struct Field_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float radius); // inferred: nearest route point
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: next point towards `point`
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
// inferred: debug drawing (lbl_8087EEB0) and camera (lbl_8087EFB4)
extern "C" void* lbl_8087EEB0;
extern "C" void* lbl_8087EFB4;
extern "C" void fn_80063764(void* draw, const atn::Vector3* a, const atn::Vector3* b, unsigned int color,
                            float width); // inferred: line
extern "C" void fn_800616C0(void* draw, float x, float y, float z, float w, float h, float f6, const char* text,
                            unsigned int color, int a, int b, int c); // inferred: text
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg, int arg2, float volume); // inferred: plays a sound

// inferred: color as four floats (as in gmk/PlayEffect.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
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

ChaserNpc* ChaserNpc::sInstance;

ChaserNpc* ChaserNpc::fn_8018059C() {
    return sInstance;
}

// NONMATCHING: 99.9% - the message of vector::__allocate is lbl_80775A88 (a pool of another file)
// and the inlined vector::push_back loads the capacity before the size.
ChaserNpc::ChaserNpc(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14C0(-1), m14C4(1.0f), m14C8(0), m14CC(0), m14D0(0) {
    atn::Csv csv(info->m2C, strlen(info->m2C));
    for (const char* tok = csv.fn_8005B9CC(); tok != 0 && strcmp("", tok) != 0; tok = csv.fn_8005B9CC()) {
        m14B4.push_back(fn_800DC12C(tok));
    }
    mFlag24 = 0;
    sInstance = this;
}

ChaserNpc::~ChaserNpc() {
    sInstance = 0;
}

void ChaserNpc::vf10() {
    m568 = 1.0f;
    if (m55C == 6) {
        if (m14B0 != 0 && m14B0 != 4 && m14D0 == 0 && lbl_8087F8A0->mHead->m560 != 0x69) {
            atn::Vector3 d = lbl_8087F8A0->mHead->mPos - mPos;
            if (PSVECMag(&d) < lbl_8087F0A8->m564) {
                fn_8018161C(4);
                return;
            }
        }
        fn_80139560();
        fn_80145334();
    } else {
        fn_80180AE0();
        fn_80145334();
    }
}

void ChaserNpc::vf14() {
    fn_80149A30();
}

// inferred: names of the states (debug display)
static const char* sStateNames_unk[] = {"START", "STAY", "WALK", "RUN", "CAUGHT", "END"};

// NONMATCHING: 98.7% - register allocation of the color conversions of the debug text.
void ChaserNpc::fn_80180AE0() {
    mC58.mC4 = 0;
    mC58.m11C.m01 = 0;
    m14D0 = MAX(m14D0 - 1, 0);
    switch (m14B0) {
    case 0:
        fn_80139560();
        break;
    case 1: {
        CharaTask* player = lbl_8087F8A0->mHead;
        atn::Vector3 d = player->mPos - mPos;
        if (PSVECMag(&d) < lbl_8087F0A8->m564 && m14D0 == 0 && lbl_8087F8A0->mHead->m560 != 0x69) {
            fn_8018161C(4);
        } else {
            m14C4 = 1.0f;
            m14C8 = MAX(m14C8 - 1, 0);
        }
        break;
    }
    case 3:
        fn_8018128C();
        break;
    case 2:
        fn_801814F0();
        break;
    case 4:
        m14D0 = lbl_8087F0A8->m568;
        fn_8018161C(3);
        break;
    case 5:
        m14C8 = 0;
        break;
    }
    if (lbl_8087F0A8->m53C != 0) {
        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &(mPos + atn::Vector3(0.0f, 25.0f, 0.0f)));
        if (screen.z > 0.0f && screen.z < 1.0f) {
            char text[0x100];
            float y = screen.y - 12.0f;
            sprintf(text, "%s", sStateNames_unk[m14B0]);
            fn_800616C0(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, text,
                        ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)), 1, 1, 0);
            sprintf(text, "W %d, V %f", m14C0 >= 0 ? m14B4[m14C0] : -1, m14C4 / 1.32f);
            y += 12.0f;
            fn_800616C0(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, text,
                        ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)), 1, 1, 0);
            y += 12.0f;
            CharaTask* player = lbl_8087F8A0->mHead;
            atn::Vector3 d = player->mPos - mPos;
            sprintf(text, "S %d, R %d, D %f", m14C8, m14D0, PSVECMag(&d));
            fn_800616C0(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, text,
                        ToArgb_unk(Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)), 1, 1, 0);
        }
    }
}

void ChaserNpc::fn_8018120C() {
    fn_8018161C(1);
    lbl_8087F8A0->mHead->fn_8014C0B4(3, 0x1F);
    lbl_8087F8A0->mHead->fn_8014C0B4(2, 0x19);
    fn_8014C0B4(3, 0x1F);
    fn_8014C0B4(2, 0x19);
}

void ChaserNpc::fn_80181284() {
    fn_8018161C(3);
}

void ChaserNpc::fn_8018128C() {
    mC58.m11C.m01 = 1;
    atn::Vector3 d = lbl_8087F8A0->mHead->mPos - mPos;
    float dist = PSVECMag(&d);
    Settings_unk* settings = lbl_8087F0A8;
    if (dist < settings->m564 && m14D0 == 0 && lbl_8087F8A0->mHead->m560 != 0x69) {
        fn_8018161C(4);
        return;
    }
    m14C8++;
    if (dist > settings->m554 &&
        fn_8004ED34(lbl_8087EE98, 0, &mPos, &lbl_8087F8A0->mHead->mPos, 0x80000000, 0, 0) != 0) {
        if (++m14CC > settings->m550) {
            fn_8018161C(2);
            return;
        }
    } else {
        m14CC = 0;
    }
    m14C4 /= 1.32f;
    if (dist > 100.0f) {
        m14C4 = MAX(m14C4 - settings->m54C, settings->m544);
    } else if (dist < 60.0f) {
        m14C4 = MIN(m14C4 + settings->m54C, settings->m548);
    }
    float slow = settings->m574 * (float)(m14C8 / settings->m578);
    float limit = settings->m548 - slow;
    m14C4 = MIN(m14C4, limit);
    m14C4 = (float)MAX((double)m14C4, settings->m544);
    m14C4 *= 1.32f;
    if (mC58.m0C.m2C != 0) {
        if (dist > settings->m560) {
            fn_8018161C(1);
            return;
        }
        fn_80181A64();
    }
    fn_80181D94();
}

void ChaserNpc::fn_801814F0() {
    Settings_unk* settings = lbl_8087F0A8;
    m14C4 = 1.32f * settings->m540;
    atn::Vector3 d = lbl_8087F8A0->mHead->mPos - mPos;
    float dist = PSVECMag(&d);
    if (dist < settings->m564 && m14D0 == 0 && lbl_8087F8A0->mHead->m560 != 0x69) {
        fn_8018161C(4);
        return;
    }
    if (mC58.m0C.m2C != 0) {
        fn_8018161C(1);
        return;
    }
    if (dist < (fn_80182134() ? settings->m558 : settings->m55C)) {
        fn_8018161C(3);
        return;
    }
    fn_80181D94();
}

// inferred: identity rotated by z, y and x (name invented)
inline void ChaserRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

void ChaserNpc::fn_8018161C(int state) {
    switch (m14B0) {
    case 1:
        fn_80181A64();
        break;
    case 3:
        m14CC = 0;
        break;
    case 4:
        fn_80181A64();
        break;
    }
    switch (state) {
    case 3:
        fn_800C31F4("SE_UI001_020", 0, -1, 1.0f);
        lbl_8087F8A0->mHead->fn_8014C0B4(3, 0x1F);
        lbl_8087F8A0->mHead->fn_8014C0B4(2, 0x19);
        fn_8014C0B4(3, 0x1F);
        fn_8014C0B4(2, 0x19);
        if (m14B0 == 2 || m14B0 == 0) {
            fn_80181A64();
        }
        break;
    case 1:
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x8B, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        break;
    case 4: {
        lbl_8087F8A0->mHead->fn_8014C228();
        fn_8014C228();
        CharaTask* player = lbl_8087F8A0->mHead;
        Mtx m;
        atn::Vector3 front = GetFrontOf_unk(m, player);
        Mtx scale;
        PSMTXScale(scale, m540.x, m540.y, m540.z);
        m14D4 = *(atn::Matrix3x4*)scale;
        atn::Matrix3x4 rot;
        ChaserRotZYX_unk(rot, player->mRot);
        PSMTXConcat(rot.m, m14D4.m, m14D4.m);
        atn::Vector3 pos = player->mPos - front * 5.0f + atn::Vector3(0.0f, 0.5f, 0.0f);
        m14D4.m[0][3] = pos.x;
        m14D4.m[1][3] = pos.y;
        m14D4.m[2][3] = pos.z;
        fn_801598B4((int)player, -1, 0x96);
        player->fn_80159BA4((int)this);
        break;
    }
    }
    m14B0 = state;
}

// NONMATCHING: 97.6% - register allocation of the place search and two swapped stack slots.
void ChaserNpc::fn_80181A64() {
    CharaTask* player = lbl_8087F8A0->mHead;
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int found = -1;
    int playerPoint = fn_803C1560(table, &player->mPos, player->fn_80179D44(), 0, 0, 1, -1.0f);
    int point = fn_803C1560(table, &mPos, fn_80179D44(), 0, 0, 1, -1.0f);
    atn::Vector3 away = (player->mPos - mPos).GetNormalized_unk();
    int count = 0;
    int best = -1;
    unsigned int i = rand() % m14B4.size();
    float bestAngle = 0.0f;
    while (found == -1) {
        RoutePlace_unk* place = table->FindPlace_unk(m14B4[i]);
        int dest = fn_803C1560(table, &place->mPos, fn_80179D44(), 0, 0, 1, -1.0f);
        bool ok = true;
        count++;
        int next = fn_803CD958((RoutePath_unk*)((char*)table->mPaths + (point - 1) * 8), dest - 1);
        atn::Vector3 dir = (table->mPoints[next - 1].mPos - mPos).GetNormalized_unk();
        float angle = acos(PSVECDotProduct(&dir, &away));
        while (next != 0 && next != dest) {
            if (next == playerPoint) {
                ok = false;
                break;
            }
            next = fn_803CD958((RoutePath_unk*)((char*)table->mPaths + (next - 1) * 8), dest - 1);
        }
        if (ok) {
            if (best == -1 || bestAngle < angle) {
                best = i;
                bestAngle = angle;
            }
            if (angle < 1.5707964f) {
                ok = false;
            }
        }
        if (ok) {
            found = i;
        } else if (count >= (int)m14B4.size()) {
            found = best;
            if (best == -1) {
                found = i;
            }
        }
        i = (i + 1) % m14B4.size();
    }
    m14C0 = found;
    mC58.m0C.fn_80128818(m14B4[found], &mPos, 8);
    mC58.m0C.fn_801255C8();
}

// NONMATCHING: 99.7% - register allocation of the inlined RouteTable_unk::FindPlace_unk (index/offset and
// the place pointer swapped). The current index in a local (`idx`) fixes the load order of m14C0.
void ChaserNpc::fn_80181D94() {
    RouteWalker_unk& route = mC58.m0C;
    if (route.fn_80125474() == 0) {
        RouteTable_unk* table = lbl_8087F430->m10D8;
        int point = fn_803C1560(table, &mPos, 0, 0, 0, 1, -1.0f);
        if (point != 0) {
            mPos = table->mPoints[point - 1].mPos;
        } else {
            int idx = m14C0;
            mPos = table->FindPlace_unk(m14B4[idx])->mPos;
        }
        mC58.m0C.fn_80128818(m14B4[m14C0], &mPos, 8);
        mC58.m0C.fn_801255C8();
    }
    route.fn_80126214();
    atn::Vector3 dir = route.m58;
    atn::Vector3 rot = mRot;
    float speed = m14C4;
    if (mC58.m0C.m2C == 0) {
        if (fn_805F9920(&dir) > 0.01f) {
            PSVECNormalize(&dir, &dir);
            rot = dir.ToRotation_unk();
        }
    } else {
        speed = 0.0f;
    }
    vf34(&rot, 0, speed, m14C4);
}

bool ChaserNpc::fn_80182134() {
    CharaTask* player = lbl_8087F8A0->mHead;
    float dist = PSVECMag(&(player->mPos - mPos));
    float angleMax = lbl_8087F0A8->m570;
    if (dist < lbl_8087F0A8->m56C) {
        atn::Vector3 up(0.0f, 20.0f, 0.0f);
        bool seen = false;
        atn::Vector3 eye = mPos + up;
        atn::Vector3 target = player->mPos + up;
        atn::Vector3 dir = (target - eye).GetNormalized_unk();
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y + mLook.mRot.y);
        PSMTXMultVec(m, &front, &front);
        float angle = acos(PSVECDotProduct(&front.GetNormalized_unk(), &dir));
        if (angle < 0.5f * angleMax) {
            CollResult_unk result;
            seen = fn_8004ED34(lbl_8087EE98, &result, &eye, &target, 0x80000000, 0, 0) == 0;
            if (lbl_8087F0A8->m53C != 0) {
                if (!seen) {
                    fn_80063764(lbl_8087EEB0, &eye, &result.m04, 0xFFFFFF00, 0.0f);
                    fn_80063764(lbl_8087EEB0, &result.m04, &target, 0xFF4D4D00, 0.0f);
                } else {
                    fn_80063764(lbl_8087EEB0, &eye, &target, 0xFF4D4D00, 0.0f);
                }
            }
        }
        return seen;
    }
    return false;
}
