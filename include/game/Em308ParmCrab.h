#ifndef GAME_EM308PARMCRAB_H
#define GAME_EM308PARMCRAB_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"
#include "game/CharaStatus.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct CrabList_unk : public std::vector<int> {};
// inferred: "BasePoints" (route places); same wrapper as CrabList_unk; name invented
struct CrabPlaceList_unk : public std::vector<RoutePlace_unk*> {};

// inferred from __vt__13Em308ParmCrab (0x80787A00). The factory allocates 0x1620 bytes
// (SpecialEnemyBase.cpp). Members keep their offsets (names unknown).
class Em308ParmCrab : public EnemyTask {
public:
    Em308ParmCrab(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em308ParmCrab();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset);
    virtual void vfEC();
    virtual float vf90();

    void fn_802FC188(); // inferred: death
    void fn_802F8EBC(); // inferred: movement
    void fn_802F93F0(); // inferred: chooses the next action
    void fn_802F97B8(); // inferred: state 8 (needle)
    void fn_802F9F3C(); // inferred: state 9 (turn)
    void fn_802FA2F4(); // inferred: state 10 (tackle run)
    void fn_802FA764(); // inferred: state 11 (keeps the distance)
    void fn_802FB0C4(); // inferred: state 12 (escape)
    inline int OnWater_unk();
    inline void ResetAttack_unk();
    inline void StopAttack_unk();
    void fn_802FB6AC(CharaTask* target); // inferred: with the flag 2 of m15B8 moves towards target (state 7)
    void fn_802FBA30(CharaTask* target); // inferred: attacks target (state 6, or 8 with the flag 4 of m15B8)
    int fn_802FBE00(CharaTask* target, int next); // inferred: starts a turn towards target (0 if it already faces it)

    int m14B0;
    int m14B4;
    char m14B8[0x14D8 - 0x14B8]; // inferred: "MoveSE" (sound name)
    atn::SoundRef m14D8;
    atn::EfpHandle_unk m14DC;
    int m14E8;                // inferred: "NeedleChargeTime"
    int m14EC;                // inferred: "NeedleChargeTime2"
    float m14F0;
    HitInfo_unk* m14F4;       // inferred: skill 0x3F7 (its m68 gives the wait frames)
    float m14F8;
    char m14FC[0x151C - 0x14FC]; // inferred: "NeedleChargeSE" (sound name)
    atn::SoundRef m151C;
    atn::EfpHandle_unk m1520;
    float m152C;
    HitInfo_unk* m1530;       // inferred: skill 0x3F8
    float m1534;
    int m1538;
    int m153C;                // inferred: next state after the turn (fn_802FBE00)
    int m1540;
    float m1544;
    float m1548;
    atn::Vector3 m154C;       // inferred: jump velocity (vf80)
    CrabList_unk m1558;
    RoutePlace_unk* m1564;    // inferred: place of state 12 (vf10 moves there with fn_8017039C)
    unsigned char m1568[0x1574 - 0x1568];
    int m1574;
    HitInfo_unk* m1578;       // inferred: skill 0x400
    int m157C;
    int m1580;
    float m1584;
    float m1588;
    float m158C;
    int m1590;
    float m1594;              // inferred: "EscapeHpRate"
    float m1598;              // inferred: "EscapeSpeed"
    RoutePlace_unk* m159C;    // inferred: escape place (fn_802FB0C4)
    atn::Vector3 m15A0;       // inferred: escape point (fn_802FB0C4)
    CrabPlaceList_unk m15AC;  // inferred: "BasePoints"
    int m15B8;                // inferred: flags 1 Move, 2 NormalAttack, 4 Needle, 8 Recover, 0x10 RecoverMove
    CharaTask* m15BC;         // inferred: target
    unsigned char m15C0[0x15C4 - 0x15C0];
    CharaTask* m15C4;         // inferred: target of the turn (fn_802FBE00)
    int m15C8;
    int m15CC;
    int m15D0;
    int m15D4;
    int m15D8;
    int m15DC;
    float m15E0;
    float m15E4;
    float m15E8;
    float m15EC;
    float m15F0;
    atn::EfpHandle_unk m15F4;
    atn::EfpHandle_unk m1600;
    atn::LH_Binary m160C;     // inferred: data file
    ResHandle m1614;          // inferred: debug menu group
    int m1618;
    unsigned char m161C[0x1620 - 0x161C];
};

#endif
