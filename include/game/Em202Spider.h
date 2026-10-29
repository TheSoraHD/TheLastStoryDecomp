#ifndef GAME_EM202SPIDER_H
#define GAME_EM202SPIDER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "std/vector.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct RoutePlace_unk;
// inferred: route places read from "needle_point"/"outer_point" (vf0C)
struct SpiderList_unk : public std::vector<RoutePlace_unk*> {};

// inferred: color of the materials of the spider (r, g, b, a; starts white); name invented
struct SpiderColor_unk {
    SpiderColor_unk() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    SpiderColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    SpiderColor_unk operator-(const SpiderColor_unk& o) const { return SpiderColor_unk(r - o.r, g - o.g, b - o.b, a - o.a); }
    SpiderColor_unk operator+(const SpiderColor_unk& o) const { SpiderColor_unk c; c.r = r + o.r; c.g = g + o.g; c.b = b + o.b; c.a = a + o.a; return c; }
    SpiderColor_unk operator*(float f) const { SpiderColor_unk c; c.r = r * f; c.g = g * f; c.b = b * f; c.a = a * f; return c; }
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

// inferred from __vt__11Em202Spider (0x807854C8). Data file "database/chara/special/" + db_name (default
// "db_em202_00_00.csv"). The factory allocates 0x15B0 bytes. Members keep their offsets (names invented).
class Em202Spider : public EnemyTask {
public:
    Em202Spider(atn::Task* parent, EnemyInfo_unk* info); // 0x8027C990
    virtual ~Em202Spider();                              // 0x8027CD9C
    virtual int vf0C();                                  // 0x8027CF28
    virtual void vf10();                                 // 0x8027D838
    virtual void vf14();                                 // 0x8027E19C
    virtual void vf3C(HitInfo_unk* hit);                 // 0x8027E410
    virtual void vf40(HitInfo_unk* hit);                 // 0x8027E678
    virtual void vf54();                                 // 0x8027E8EC
    virtual void vf7C(int arg);                          // 0x8027EA84
    virtual void vfEC();                                 // 0x80280EF8

    void fn_802829DC(); // inferred: death
    inline void ResetState_unk();
    inline void Dash_unk(const atn::Vector3& rot);
    inline void Attack_unk(HitInfo_unk* const& skill);
    void fn_8027EB14(); // inferred: chooses the next action
    void fn_8027F504();
    void fn_8027FA7C(); // inferred: state 5
    void fn_80280184(); // inferred: state 6
    void fn_802805C8(); // inferred: state 9 (turn)
    void fn_80281F88();
    void fn_8027F69C(); // inferred: effect and sound where the arms touch the ground
    void fn_8027F8A8(); // inferred: wait of m1520 frames, then the action m1528
    void fn_80281924(CharaTask* target);
    void fn_80281054(CharaTask* target); // inferred: starts state 1 (faces the target)
    void fn_80281344(CharaTask* target); // inferred: starts state 3 (faces the target)
    void fn_80281634(CharaTask* target, int next); // inferred: starts state 4 (then the action `next`)
    int fn_802826A0(CharaTask* target, int arg); // inferred: starts state 9 (turn) when the target is not in front
    void fn_80281ED8(); // inferred: starts state 7 (moves to the place m14B8)

    atn::LH_Binary m14B0;     // inferred: data file
    int m14B8;                // inferred: place id
    int m14BC;
    SpiderList_unk m14C0;
    SpiderList_unk m14CC;
    int m14D8;
    float m14DC;              // inferred: 55.0f
    float m14E0;              // inferred: collision radius of the data (m5B0 before vf0C sets it to 25)
    int m14E4;                // inferred: phase ("start_state": "1st" 0, "2nd" 1)
    int m14E8;
    CharaTask* m14EC;         // inferred: target (passed to fn_80281924/fn_80281054)
    int m14F0;
    int m14F4[2];             // inferred: places of state 8 (index m14F0)
    int m14FC;                // inferred: damage taken (vf40)
    int m1500;
    int m1504;
    HitInfo_unk* m1508;       // inferred: skill "CrowSkillID"
    HitInfo_unk* m150C;       // inferred: skill "JumpAttackSkillID"
    HitInfo_unk* m1510;       // inferred: skill "TackleSkillID"
    HitInfo_unk* m1514;       // inferred: skill "DashSkillID"
    HitInfo_unk* m1518;       // inferred: skill "NeedleSkillID"
    int m151C;
    int m1520;
    int m1524;
    int m1528;
    int m152C;
    int m1530;
    int m1534;                // inferred: 60 ("NeedleChargeTime")
    int m1538;                // inferred: argument of the turn (state 9, fn_802826A0)
    float m153C;              // inferred: angle left to turn (state 9)
    CharaTask* m1540;         // inferred: target of the turn (state 9)
    int m1544;
    int m1548;
    int m154C;
    atn::EfpFile m1550; // inferred: "/effect/ef_uc019.efp"
    atn::EffFile m155C;      // inferred: "/effect/ef_sk002b.eff"
    atn::EfpFile m1568; // inferred: "/effect/ef_uc001.efp"
    atn::EfpFile m1574; // inferred: "/effect/ef_sk002d.efp"
    atn::EfpFile m1580; // inferred: "/effect/ef_uc007.efp"
    atn::EfpFile m158C; // inferred: "/effect/ef_sc003_01b.efp"
    SpiderColor_unk m1598;    // inferred: color of the materials "em202_body"/"em202_tail" (vf14)
    atn::SoundRef m15A8;
};

#endif
