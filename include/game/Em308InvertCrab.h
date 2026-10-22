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
    void fn_802F566C();
    void fn_802F58A0();
    void fn_802F66CC(); // inferred: death

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
    unsigned char m1530[0x1534 - 0x1530];
    int m1534;
    float m1538;
    float m153C;
    atn::Vector3 m1540;       // inferred: jump velocity (vf80)
    unsigned char m154C[0x1550 - 0x154C];
    HitInfo_unk* m1550;       // inferred: skill 0x400
    int m1554;
    int m1558;                // inferred: "EscapeCount"
    float m155C;              // inferred: "EscapeSpeed"
    unsigned char m1560[0x1570 - 0x1560];
    InvertList_unk m1570;     // inferred: "BasePoints" (route places)
    int m157C;                // inferred: flags 2 "NormalAttack", 4 "Needle", 8 "Recover"
    int m1580;
    int m1584;
    unsigned char m1588[0x158C - 0x1588];
    int m158C;
    int m1590;
    int m1594;
    int m1598;
    int m159C;
    int m15A0;
    unsigned char m15A4[0x15A8 - 0x15A4];
    int m15A8;
    atn::EfpHandle_unk m15AC;
    ResLoader_unk m15B8;
    atn::LH_Binary m15C4;     // inferred: data file
    ResHandle m15CC;          // inferred: debug menu group
    int m15D0;
    unsigned char m15D4[0x15D8 - 0x15D4];
};

#endif
