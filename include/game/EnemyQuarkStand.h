#ifndef GAME_ENEMYQUARKSTAND_H
#define GAME_ENEMYQUARKSTAND_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/LH_Xsca.h"
#include "atn/ResLoader_unk.h"
#include "game/Bullet.h"
#include "game/CharaStatus.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

namespace atn {
class D2A_Task;
}
class PlayerTask;
namespace atn {
}

// Pending to unify with game/EnemyQuarkSecond (queue q8): this header only declares what
// game/EnemyQuarkStand.cpp needs; the helper classes are local to it (names invented).

// inferred: block of 0x60 bytes at 0x14C8 cleared by the inline constructor 0x8034585C (names unknown)
struct QuarkStandState_unk {
    QuarkStandState_unk();
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38;
    float m3C;
    int m40;
    float m44;
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
};

// inferred: list of skill ids read from the data file (ShortRange, MiddleRangeTableMode1, ...): a class
// derived from std::vector<int> (its destructor 0x8000D760 tests the address four times, as
// KrakenIntList_unk). Its out-of-line constructor and destructor are weak copies of other files
// (0x80288390 in game/Em203GreaterDaemon, 0x8000D760 in Event/Puppeteer_CharaTask; Event/Sequence also
// uses them), so the real class is shared; name invented.
struct IntVector_unk : public std::vector<int> {};

// inferred: effect slot of 0x18 bytes (only the byte at 0x14 is cleared by its inline ctor 0x803458C0)
struct QuarkSlot_unk {
    QuarkSlot_unk();
    unsigned char m00[0x14];
    unsigned char m14;
    unsigned char m15[0x18 - 0x15];
};

// inferred: delayed explosion (0x10 bytes: timer and position); entries of the fixed array at 0x181C
// (FixedArray_unk<QuarkShot_unk, 32>: full 0x80349450, push 0x80349470). The empty constructor is a weak
// copy (0x803458DC). Names invented.
struct QuarkShot_unk {
    QuarkShot_unk();
    float m00;          // inferred: timer
    atn::Vector3 m04;   // inferred: position
};

// inferred: shot parameters with the constructor of the weak copy 0x80349424 (m00 0.1, m18 1.0, the rest
// zero; the inline fields of gmk/BulletPoint are set in another order, so it is kept apart). Name invented.
struct QuarkBulletShot_unk : public BulletShot_unk {
    QuarkBulletShot_unk();
};

// inferred: delayed wave (0x14 bytes: position, timer, started flag and effect id); entries of the fixed
// array at 0x1A28. Names invented.
struct QuarkWave_unk {
    atn::Vector3 m00;
    float m0C;          // inferred: timer
    unsigned char m10;  // inferred: the effect is started
    unsigned char m11;  // inferred: effect id
};

// inferred: aim of the current attack (0x30 bytes at 0x15DC, set when an attack starts; its empty
// constructor is the weak copy 0x803458CC). Names invented.
struct QuarkDir_unk {
    QuarkDir_unk();
    // inferred: takes the position by value (the original copies it to a temporary)
    void SetTarget_unk(atn::Vector3 v) {
        mTarget = v;
        mTarget.y = 0.0f;
    }
    atn::Vector3 mTarget; // inferred: target position on the ground (y = 0)
    atn::Vector3 mRot;    // inferred: rotation toward the target
    atn::Vector3 mDir;    // inferred: horizontal direction toward the target
    atn::Vector3 mStart;  // inferred: position when the attack started
};

// inferred: object of 0x30 bytes with an empty out-of-line constructor (weak copy 0x803165E0 in
// game/Em316LaserSphere, which builds three of them); probably a matrix, but not atn::Matrix3x4 (its
// constructor is inline and never called). Name invented.
struct Mtx34_unk {
    Mtx34_unk();
    float m[3][4];
};

// inferred from __vt__15EnemyQuarkStand (0x80789550): Quark standing (third fight). Data file
// "database/chara/special/" + db_name (default "db_enemy_quark_stand.csv"). The factory allocates
// 0x1CF0 bytes (SpecialEnemyBase.cpp, case 62). Members keep their offsets (names unknown).
class EnemyQuarkStand : public EnemyTask {
public:
    EnemyQuarkStand(atn::Task* parent, EnemyInfo_unk* info); // 0x80345384
    virtual ~EnemyQuarkStand();                              // 0x80354398
    virtual int vf0C();                                      // 0x80345BA8
    virtual void vf10();                                     // 0x80346384
    virtual void vf3C(HitInfo_unk* hit);                     // 0x80346990
    virtual void vf40(HitInfo_unk* hit);                     // 0x80346A34
    virtual void vf50();                                     // 0x80347218
    virtual void vf54();                                     // 0x803482DC
    virtual atn::Vector3 vf64(int center);                   // 0x80348550
    virtual int vf78();                                      // 0x80353DE8
    virtual void vf98();                                     // 0x80353F50
    virtual void vf9C(int arg);                              // 0x803540BC
    virtual int vfB0(atn::Vector3* out);                     // 0x80352EC8
    virtual boost::function<void()> vfB4(CharaTask* chara);  // 0x80352F88
    virtual atn::Vector3 vfD0(float angle);                  // 0x80354194
    virtual float vfD4();                                    // 0x80354390
    virtual atn::Vector3 vfD8(CharaTask* target);            // 0x80354270
    virtual float vfDC();                                    // 0x80354388
    virtual float vfE0();                                    // 0x80354378
    inline void ReleaseCamera_unk();
    void fn_8034F384(); // inferred: leaves the current action
    void fn_8034F41C(); // inferred: state 0
    void fn_803519C4(); // inferred: state 20
    void fn_803524AC(int motion, int arg3, float speed);
    void fn_80352EA4();
    // inferred: states and helpers still in asm (names keep the address)
    void fn_80346D8C(); // inferred: picks the next attack
    void fn_8034701C(int kind); // inferred: starts attack `kind`
    inline void AimAt_unk();
    inline void TurnTo_unk(float rate);
    inline IntVector_unk& Table_unk(int range);
    inline int NextShort_unk();
    inline int NextMiddle_unk();
    inline int NextLong_unk();
    void fn_8034FB50();
    void fn_80350070();
    void fn_80350464();
    void fn_8035083C();
    void fn_80351480(int arg);
    void fn_80351AB8(CharaTask* chara, int kind);
    void fn_803470F0();
    void fn_8034C060(int arg); // inferred: rises toward the aim (motions 0x13F-0x141)
    void fn_80352938(float rate); // inferred: turns toward the target
    void fn_803489F4(const atn::Vector3& target); // inferred: matrix of the laser
    void fn_80348568();
    void fn_80348FF4();
    void fn_803494AC();
    void fn_80349BA0();
    void fn_8034A194();
    void fn_8034A644();
    void fn_8034AE48();
    void fn_8034B8CC();
    void fn_8034C714();
    void fn_8034CF48();
    void fn_8034DA0C();
    void fn_8034DE90();
    void fn_8034E898();
    void fn_8034EA50();
    void fn_8034ECE8();
    void fn_8034F53C();
    void fn_8034F788();
    void fn_80350F64();
    void fn_80350BF0(); // inferred: state 12
    void fn_803520D8();
    void fn_8035223C();
    void fn_803524E4();
    void fn_803526A8();
    void fn_80352C3C(int arg, int camera, float a, float b);
    void fn_8035359C();
    void fn_80353370(); // inferred: debug menu entries

    int m14B0;                     // inferred: target (CharaTask*)
    int m14B4;                     // inferred: step of the state
    int m14B8;                     // inferred: frame counter of the state
    int m14BC;                     // inferred: 1 once the hit points reach zero
    int m14C0;                     // inferred: frame counter
    PlayerTask* m14C4;             // inferred: "Manamia" (player id)
    QuarkStandState_unk m14C8;
    HitInfo_unk* m1528;            // inferred: "BurstSkill"
    HitInfo_unk* m152C;            // inferred: "LaserSkillID"
    HitInfo_unk* m1530;            // inferred: "LaserExplodeSkillID"
    HitInfo_unk* m1534;            // inferred: "TackleSkillID" (two ids)
    HitInfo_unk* m1538;
    HitInfo_unk* m153C;            // inferred: "SonicSkillID"
    HitInfo_unk* m1540;            // inferred: "HomingSkillID"
    HitInfo_unk* m1544;            // inferred: "AttackSkill" (two ids)
    HitInfo_unk* m1548;
    HitInfo_unk* m154C;            // inferred: "StickSkill"
    HitInfo_unk* m1550;            // inferred: "HoneSlashSkill"
    HitInfo_unk* m1554[2];         // inferred: "RainSkill" (zero terminated)
    HitInfo_unk* m155C;            // inferred: "ThunderSkill"
    // inferred: attack tables per mode, short/middle/long range (ShortRangeTableMode1 ... LongRangeTableMode3);
    // a flat array (the constructor builds the nine lists with one __construct_array)
    IntVector_unk m1560[9];
    int m15CC;                     // inferred: mode (0-2, from the hit point ratio)
    int m15D0;                     // inferred: index into the short range table
    int m15D4;                     // inferred: index into the middle range table
    int m15D8;                     // inferred: index into the long range table
    QuarkDir_unk m15DC;
    atn::Vector3 m160C;            // inferred: centre of the arena
    atn::EfpHandle_unk m1618;      // inferred: "ef_bd008b.efp"
    atn::EfpHandle_unk m1624;      // inferred: "ef_bs072f.efp"
    atn::EfpHandle_unk m1630;      // inferred: "ef_bs070d.efp"
    atn::EfpHandle_unk m163C;      // inferred: "ef_bs070a.efp"
    ResLoader_unk m1648;           // inferred: "ef_bs070e_01.eff"
    atn::Vector3 m1654;
    Mtx34_unk m1660;
    atn::EfpHandle_unk m1690;      // inferred: "ef_bd009a.efp"
    atn::EfpHandle_unk m169C;      // inferred: "ef_bd009b.efp"
    atn::EfpHandle_unk m16A8;      // inferred: "ef_bs078c.efp"
    atn::EfpHandle_unk m16B4;      // inferred: "ef_bs080a.efp"
    atn::EfpHandle_unk m16C0;      // inferred: "ef_bs058a.efp"
    atn::EfpHandle_unk m16CC;      // inferred: "ef_bs058c.efp"
    unsigned char m16D8;
    float m16DC;
    atn::Vector3 m16E0;
    int m16EC;
    int m16F0;
    int m16F4;
    atn::Vector3 m16F8;
    atn::LH_Xsca m1704;            // inferred: "/camera/battle/cm000_em211_00.xsca"
    atn::LH_Xsca m170C;            // inferred: "/camera/battle/cm000_em211_01.xsca"
    unsigned char m1714;
    unsigned char m1715[0x1718 - 0x1715];
    QuarkSlot_unk m1718[8];
    ResLoader_unk m17D8;           // inferred: "ef_bs051b.eff"
    CharaTask* m17E4;              // inferred: character stuck by the sword
    int m17E8;                     // inferred: hits received in state 12
    unsigned char m17EC;
    unsigned char m17ED[0x17F0 - 0x17ED];
    int m17F0;
    atn::D2A_Task* m17F4;          // inferred: "/data/d2anime/ui_direction_l.d2a"
    atn::D2A_Task* m17F8;          // inferred: "/data/d2anime/ui_direction_r.d2a"
    int m17FC;                     // inferred: "DownCount"
    int m1800;                     // inferred: "StundUpDamageCount"
    int m1804;                     // inferred: hits received in state 15
    unsigned char m1808;
    unsigned char m1809;           // inferred: rising (state 1)
    bool m180A;                    // inferred: the action is state 19
    unsigned char m180B;           // inferred: the camera was taken
    atn::LH_Binary m180C;          // inferred: data file
    int m1814;                     // inferred: hits received
    int m1818;                     // inferred: "GatheringBurstCount"
    FixedArray_unk<QuarkShot_unk, 32> m181C; // inferred: delayed explosions
    int m1A20;                     // inferred: bullet id of the explosions
    int m1A24;                     // inferred: explosions fired
    FixedArray_unk<QuarkWave_unk, 32> m1A28; // inferred: delayed waves
    atn::EfpHandle_unk m1CAC;      // inferred: "ef_bs049e.efp"
    int m1CB8;
    int m1CBC;
    ResHandle m1CC0;               // inferred: debug menu group "Quark"
    int m1CC4;                     // inferred: "DebugDisp"
    float m1CC8;                   // inferred: "ShockExpandSnicWave"
    float m1CCC;
    int m1CD0;                     // inferred: "AttackWaitTime"
    int m1CD4;                     // inferred: "AttackedMaxInStick"
    float m1CD8;                   // inferred: "SwordDirectionLine"
    int m1CDC;                     // inferred: "SwordThrowTime"
    float m1CE0;                   // inferred: "ShortRange"
    float m1CE4;                   // inferred: "MiddleRange"
    float m1CE8;                   // inferred: "LongRange"
    unsigned char m1CEC[0x1CF0 - 0x1CEC];
};

#endif
