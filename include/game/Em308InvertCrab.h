#ifndef GAME_EM308INVERTCRAB_H
#define GAME_EM308INVERTCRAB_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"
#include "game/CharaStatus.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as CrabList_unk in
// game/Em308ParmCrab.h); name invented
struct InvertList_unk : public std::vector<RoutePlace_unk*> {};

// inferred from __vt__15Em308InvertCrab (0x80787938): crab (sister of Em308ParmCrab). The factory allocates
// 0x15D8 bytes (SpecialEnemyBase.cpp). Data file "database/chara/special/" + db_name (default
// "db_em308_00_00.csv"). Members keep their offsets (names unknown).
class Em308InvertCrab : public EnemyTask {
public:
    Em308InvertCrab(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em308InvertCrab();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset);
    virtual void vfEC();
    virtual int vf68(CharaTask* attacker) { return 0; }
    virtual void vf6C(int kind, CharaTask* other) {}
    // inferred: inline (weak copy 0x802F6E54 emitted at the end of the file); the slot 0x60
    // (0x802F6E5C, returns 8.0f) is not written: CharaTask::vf60 is declared void in the shared header
    virtual float vf90() { return 3.5f; }

    int fn_802F2C50(); // inferred: ground below is water/lava (ray test attributes 0x14, 0x15, 0x1A-0x1D)
    void fn_802F33A0(); // inferred: movement
    void fn_802F389C(); // inferred: chooses the next action
    void fn_802F3BE8(); // inferred: state 8 (needle)
    void fn_802F42CC(); // inferred: state 9 (turn)
    void fn_802F469C(); // inferred: state 10 (tackle run)
    void fn_802F4A98(); // inferred: state 11 (keeps the distance)
    void fn_802F5370(); // inferred: state 12 (escape)
    void fn_802F566C();
    inline void ResetAttack_unk();
    void fn_802F58A0();
    void fn_802F5BCC(CharaTask* target);
    void fn_802F5F20(CharaTask* target);
    int fn_802F62A0(CharaTask* target, int next); // inferred: starts a turn towards target
    void fn_802F66CC(); // inferred: death
    void fn_802F693C(CharaTask* target); // inferred: starts the escape (state 12)

    int m14B0;
    char m14B4[0x14D4 - 0x14B4]; // inferred: "MoveSE" (sound name)
    atn::SoundRef m14D4;
    atn::EfpHandle_unk m14D8;
    int m14E4;
    float m14E8;
    HitInfo_unk* m14EC;       // inferred: skill 0x3F7
    float m14F0;
    char m14F4[0x1514 - 0x14F4]; // inferred: "NeedleChargeSE" (sound name)
    atn::SoundRef m1514;
    atn::EfpHandle_unk m1518;
    float m1524;
    HitInfo_unk* m1528;       // inferred: skill 0x3F8
    float m152C;
    int m1530;                // inferred: next state after the turn (fn_802F62A0)
    int m1534;
    float m1538;
    float m153C;
    atn::Vector3 m1540;       // inferred: jump velocity (vf80)
    unsigned char m154C[0x1550 - 0x154C];
    HitInfo_unk* m1550;       // inferred: skill 0x400
    int m1554;
    int m1558;                // inferred: "EscapeCount"
    float m155C;              // inferred: "EscapeSpeed"
    RoutePlace_unk* m1560;    // inferred: escape place (fn_802F5370)
    atn::Vector3 m1564;       // inferred: escape point (fn_802F5370)
    InvertList_unk m1570;     // inferred: "BasePoints" (route places)
    int m157C;                // inferred: flags 2 "NormalAttack", 4 "Needle", 8 "Recover"
    int m1580;
    int m1584;
    CharaTask* m1588;         // inferred: target of the turn (fn_802F62A0)
    int m158C;
    int m1590;
    int m1594;
    int m1598;
    int m159C;
    int m15A0;
    int m15A4;                // inferred: charge counter of the needle (fn_802F3BE8)
    int m15A8;
    atn::EfpHandle_unk m15AC;
    ResLoader_unk m15B8;
    atn::LH_Binary m15C4;     // inferred: data file
    ResHandle m15CC;          // inferred: debug menu group
    int m15D0;
    unsigned char m15D4[0x15D8 - 0x15D4];
};

#endif
