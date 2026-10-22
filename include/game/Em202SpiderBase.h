#ifndef GAME_EM202SPIDERBASE_H
#define GAME_EM202SPIDERBASE_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct SpiderBaseList_unk : public std::vector<int> {};

// inferred: color of the materials "em202_body"/"em202_tail" (r, g, b, a; starts white); name invented
struct SpiderBaseColor_unk {
    SpiderBaseColor_unk() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    SpiderBaseColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    SpiderBaseColor_unk operator-(const SpiderBaseColor_unk& o) const {
        return SpiderBaseColor_unk(r - o.r, g - o.g, b - o.b, a - o.a);
    }
    SpiderBaseColor_unk operator+(const SpiderBaseColor_unk& o) const {
        SpiderBaseColor_unk c;
        c.r = r + o.r;
        c.g = g + o.g;
        c.b = b + o.b;
        c.a = a + o.a;
        return c;
    }
    SpiderBaseColor_unk operator*(float f) const {
        SpiderBaseColor_unk c;
        c.r = r * f;
        c.g = g * f;
        c.b = b * f;
        c.a = a * f;
        return c;
    }
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

// inferred from __vt__15Em202SpiderBase. The factory allocates 0x15E0 bytes (SpecialEnemyBase.cpp).
// Members keep their offsets (names unknown).
class Em202SpiderBase : public EnemyTask {
public:
    Em202SpiderBase(atn::Task* parent, EnemyInfo_unk* info); // 0x80282AE8
    virtual ~Em202SpiderBase();
    virtual void vf14();                                       // 0x80283D60
    virtual void vf3C(HitInfo_unk* hit);                       // 0x80283FD4
    virtual void vf40(HitInfo_unk* hit);                       // 0x8028429C
    virtual void vf7C(int arg);
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset); // 0x802844E0
    virtual void vfEC();                                       // 0x80286B78

    void fn_80287788(); // inferred: death
    inline void ResetAttack_unk();
    void fn_802847CC(); // inferred: movement of the frame (route in state 7, facing m1580)
    void fn_802868A0(); // inferred: walks to the base point m1564, then attacks m1580
    void fn_80286CB0(CharaTask* target); // inferred: starts state 7 (move), or the attack on m1580
    void fn_8028702C(CharaTask* target); // inferred: starts the attack (state 6, or state 8 in needle mode)
    int fn_80287400(CharaTask* target, int arg); // inferred: starts state 9 (turn) when the target is not in front

    int m14B0;
    int m14B4;
    unsigned char m14B8[0x14D8 - 0x14B8];
    atn::SoundRef m14D8;
    atn::EfpHandle_unk m14DC; // inferred: "/effect/ef_uc007.efp"
    int m14E8;                // inferred: 60
    int m14EC;                // inferred: 15
    float m14F0;              // inferred: 70.0f
    HitInfo_unk* m14F4;       // inferred: skill 0x3F7 ("NeedleID")
    float m14F8;              // inferred: 180.0f
    unsigned char m14FC[0x151C - 0x14FC];
    atn::SoundRef m151C;
    atn::EfpHandle_unk m1520; // inferred: "/effect/ef_sk002d.efp"
    float m152C;              // inferred: 30.0f
    HitInfo_unk* m1530;       // inferred: skill 0x3F8 ("TackleID")
    float m1534;              // inferred: pi
    int m1538;                // inferred: argument of the turn (state 9, fn_80287400)
    int m153C;
    float m1540;              // inferred: start angle of the turn (state 9)
    float m1544;              // inferred: target angle of the turn (state 9)
    atn::Vector3 m1548;       // inferred: jump velocity (state 13, vf80)
    int m1554;
    unsigned char m1558[0x155C - 0x1558];
    float m155C;              // inferred: speed rate of state 12 (fn_802847CC)
    int* m1560;               // inferred: current base point (place id at 0, fn_802868A0)
    atn::Vector3 m1564;       // inferred: position of the base point
    SpiderBaseList_unk m1570;
    int m157C;                // inferred: flags (4: needle mode, fn_8028702C)
    CharaTask* m1580;         // inferred: target of the attack (fn_80286CB0)
    int m1584;
    CharaTask* m1588;         // inferred: target of the turn (state 9)
    int m158C;
    int m1590;                // inferred: 90
    int m1594;                // inferred: 90
    int m1598;
    int m159C;                // inferred: frames of the jump (vf80)
    float m15A0;              // inferred: 1.0f
    atn::EfpHandle_unk m15A4; // inferred: "/effect/ef_uc001.efp"
    atn::EfpHandle_unk m15B0; // inferred: "/effect/ef_sc003_01b.efp"
    atn::LH_Binary m15BC;     // inferred: data file
    SpiderBaseColor_unk m15C4; // inferred: color of the materials (vf14)
    ResHandle m15D4;          // inferred: debug menu group
    int m15D8;
    unsigned char m15DC[0x15E0 - 0x15DC];
};

#endif
