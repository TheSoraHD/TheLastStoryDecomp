#ifndef GAME_ENEMYQUARKSECOND_H
#define GAME_ENEMYQUARKSECOND_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/CharaStatus.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"
#include "game/IntVector_unk.h"

class PlayerTask;
namespace atn {
class Csv;
}

typedef IntVector_unk QuarkTable_unk; // inferred: a range table ("ShortRangeTableMode1"...)

// inferred: 0x30-byte member at 0x15E0 with an empty constructor (weak copy 0x803165E0 of
// game/Em316LaserSphere.cpp, which has three of them); a matrix filled with the paired-single copy.
// Not atn::Matrix3x4: the linker would have kept one copy of both constructors. Name invented.
struct MtxBlock_unk {
    MtxBlock_unk() {}
    float m[3][4];
};

// inferred: jump of the boss (0x30 bytes at 0x1568, empty constructor: weak copy 0x80338D04): target on
// the ground, rotation towards it, horizontal direction and start position. Names invented.
struct QuarkJump_unk {
    QuarkJump_unk() {}
    atn::Vector3 mTarget;
    atn::Vector3 mRot;
    atn::Vector3 mDir;
    atn::Vector3 mStart;
};

// inferred: hits received in the stick attack: a count and 32 words (constructor fn_80338D08 clears the
// count); names invented
struct QuarkHits_unk {
    QuarkHits_unk() : mCount(0) {}
    unsigned int mCount;
    int mData[32];
};

// inferred from __vt__16EnemyQuarkSecond (0x80789388): second fight of Quark. A direct subclass of
// EnemyTask (vtable of 0xF8 bytes, constructor 0x803388A4 calls __ct__9EnemyTask). Data file
// "database/chara/special/" + db_name (default "db_enemy_quark_second.csv"). The factory allocates
// 0x1788 bytes (SpecialEnemyBase.cpp, case 71). Names invented; members keep their offsets.
class EnemyQuarkSecond : public EnemyTask {
public:
    EnemyQuarkSecond(atn::Task* parent, EnemyInfo_unk* info); // 0x803388A4
    virtual ~EnemyQuarkSecond();                               // 0x8034515C
    virtual int vf0C();                                        // 0x80338FD0
    virtual void vf10();                                       // 0x80339760
    virtual void vf24();                                       // 0x8033C364
    virtual void vf3C(HitInfo_unk* hit);                       // 0x80339F8C
    virtual void vf40(HitInfo_unk* hit);                       // 0x8033A154
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir); // 0x80344BD8
    virtual void vf50();                                       // 0x8033ACB0
    virtual void vf54();                                       // 0x8033BD74
    virtual atn::Vector3 vf64(int center);                     // 0x8033BFE8
    virtual int vf68(CharaTask* attacker);                     // 0x80344BF4
    virtual void vf6C(int kind, CharaTask* other);             // 0x80344C1C
    virtual int vf78();                                        // 0x80344AB8
    virtual atn::Vector3 vfD8(CharaTask* target);              // 0x80344E88
    virtual float vfDC();                                      // 0x80345154

    inline void ResetState_unk();
    inline void PlayMotion_unk(int motion, int loop, float speed);
    inline int NextInTable_unk(int range, int& index);
    inline void fn_8033A7A0(); // inferred: picks the next action
    void fn_8033C490(); // inferred: state 6
    void fn_8033D040(); // inferred: state 7
    void fn_8033D750(); // inferred: state 9
    void fn_8033DB8C(); // inferred: state 14
    void fn_8033E058(); // inferred: state 10
    void fn_8033EF90(); // inferred: state 8
    void fn_8033F7FC(); // inferred: state 15
    void fn_803405C0(); // inferred: state 16
    void fn_80340E54(); // inferred: state 12
    void fn_8034100C(); // inferred: state 13
    void fn_803412AC(); // inferred: state 17
    void fn_80341BEC(); // inferred: state 11
    void fn_80342004(); // inferred: starts state 6
    void fn_8034228C();
    void fn_80342674();
    void fn_80342BC8();
    void fn_80342FB4();
    void fn_8034315C();
    void fn_803435B8();
    void fn_80343B0C();
    inline void fn_80344BB4(); // inferred: releases the field camera
    inline void StartDown_unk();
    inline void Fall_unk();
    inline bool IsMotionEnd_unk();
    inline void SetJump_unk(const atn::Vector3& target);
    void fn_8033AB88(); // inferred: moves the collision sphere in front
    void fn_8033C000(); // inferred: moves towards the target
    void fn_8033C3D8(); // inferred: state 0 (wait)
    void fn_80341E14(); // inferred: resets the state (counters, effects)
    void fn_80341EE4(); // inferred: back to state 0
    void fn_80344060(); // inferred: state 11
    void fn_803441AC(); // inferred: rain of bullets in a circle
    void fn_8034441C(int motion, int loop, float speed); // inferred: plays a motion (track 0)
    void fn_80344454(int msg, int notify, float radius, float angle); // inferred: gimmicks in a cone in front
    int fn_803446BC(const atn::Vector3& pos); // inferred: `pos` is far from the centre
    inline void fn_8034472C(int index, int value); // inferred: sets an event flag once
    void fn_80344780(); // inferred: debug menu
    void fn_803449AC(); // inferred: aggro rates of the party
    void fn_80344DF4(); // inferred: starts the stick attack

    CharaTask* m14B0; // inferred: target
    int m14B4;        // inferred: step of the state
    int m14B8;        // inferred: frame counter of the state
    int m14BC;        // inferred: mode (0 barrier, 1, 2)
    int m14C0;        // inferred: frames since the last action
    PlayerTask* m14C4; // inferred: the "Quark" player
    unsigned char m14C8; // inferred: set while the player holds the stick
    HitInfo_unk* m14CC; // inferred: "BurstSkill"
    HitInfo_unk* m14D0; // inferred: "LaserSkillID"
    HitInfo_unk* m14D4; // inferred: "PierceSkillID"
    HitInfo_unk* m14D8; // inferred: "SonicSkillID"
    HitInfo_unk* m14DC; // inferred: "SwordSkillID"
    HitInfo_unk* m14E0; // inferred: "AttackSkill"
    HitInfo_unk* m14E4[2]; // inferred: "RainSkill" (filled until a zero id)
    QuarkTable_unk m14EC[3][3]; // inferred: range tables (short, middle, long) of the three modes
    int m1558; // inferred: current mode (0-2, from the hit points)
    int m155C; // inferred: index in the short range table
    int m1560; // inferred: index in the middle range table
    int m1564; // inferred: index in the long range table
    QuarkJump_unk m1568; // inferred: current jump
    atn::Vector3 m1598; // inferred: centre of the arena
    atn::EfpHandle_unk m15A4; // inferred: "ef_bs072f.efp"
    atn::EfpHandle_unk m15B0; // inferred: "ef_bs054d.efp"
    atn::EfpHandle_unk m15BC; // inferred: "ef_bs054a.efp"
    ResLoader_unk m15C8;      // inferred: "ef_bs054e_01.eff"
    atn::Vector3 m15D4;
    MtxBlock_unk m15E0;
    atn::EfpHandle_unk m1610; // inferred: "ef_bu040a.efp"
    atn::EfpHandle_unk m161C; // inferred: "ef_bu040h.efp"
    unsigned char m1628;
    atn::EfpHandle_unk m162C; // inferred: "ef_bs058a.efp"
    atn::EfpHandle_unk m1638; // inferred: "ef_bs058c.efp"
    unsigned char m1644; // inferred: the rain of bullets is running (fn_803441AC)
    float m1648;         // inferred: radius of the rain
    atn::Vector3 m164C;  // inferred: centre of the rain
    int m1658;           // inferred: bullet id of the rain
    int m165C;
    int m1660;
    int m1664;
    ResLoader_unk m1668;      // inferred: "ef_bs051b.eff"
    atn::EfpHandle_unk m1674; // inferred: "ef_bd008d.efp"
    atn::EfpHandle_unk m1680; // inferred: "ef_bu047b.efp"
    atn::EfpHandle_unk m168C; // inferred: "ef_bu047c.efp"
    atn::EfpHandle_unk m1698; // inferred: "ef_bs079b.efp"
    int m16A4; // inferred: "DownCount"
    int m16A8; // inferred: "StundUpDamageCount"
    int m16AC; // inferred: hits counted in state 13
    unsigned char m16B0;
    unsigned char m16B1;
    unsigned char m16B2;
    unsigned char m16B3;
    unsigned char m16B4; // inferred: the stick attack is running
    atn::LH_Binary m16B8; // inferred: data file
    int m16C0; // inferred: hit points of the barrier
    int m16C4; // inferred: "BarrierHp"
    int m16C8;
    int m16CC; // inferred: "GatheringBurstCount"
    int m16D0;
    QuarkHits_unk m16D4; // inferred: hits received in the stick attack (at most 32)
    EnemyTask* m1758;    // inferred: "ThirdScriptId"
    ResHandle m175C;     // inferred: debug menu group
    int m1760;           // inferred: "DebugDisp"
    float m1764;         // inferred: "ShockExpandSnicWave"
    float m1768;
    int m176C; // inferred: "AttackWaitTime"
    int m1770; // inferred: "AttackedMaxInStick"
    float m1774; // inferred: "SwordDirectionLine"
    int m1778;   // inferred: "SwordThrowTime"
    float m177C; // inferred: "ShortRange"
    float m1780; // inferred: "MiddleRange"
    float m1784; // inferred: "LongRange"
};

#endif
