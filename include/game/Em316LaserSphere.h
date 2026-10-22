#ifndef GAME_EM316LASERSPHERE_H
#define GAME_EM316LASERSPHERE_H

#include "atn/EffectModel_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__16Em316LaserSphere (0x80788760): laser sphere enemy (factory types 45 and 49,
// 0x1CA0 bytes). Data file "database/chara/special/" + db_name (default "db_em601_00_00.csv"); it
// orbits a target ("target=" id) and fires lasers (LaserID) or tackles (TackleID). Names invented.
class Em316LaserSphere : public EnemyTask {
public:
    Em316LaserSphere(atn::Task* parent, EnemyInfo_unk* info); // 0x803161F8
    virtual int vf0C();                                        // 0x803165EC
    virtual void vf10();                                       // 0x80316A68
    virtual void vf14();                                       // 0x80317040
    virtual void vf3C(HitInfo_unk* hit);                       // 0x80317110
    virtual void vf40(HitInfo_unk* hit);                       // 0x80317188
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset); // 0x80317360
    virtual int vfE4();                                        // 0x8031ACAC
    virtual void vfEC();                                       // 0x80319090

    // inferred: sets the state (m58C) and restarts its step and frame counters (inlined everywhere)
    // (the destructor is implicit: 0x8031AF88, emitted at the end of the file)
    void SetState_unk(int state);
    void SetAppear_unk(const atn::Vector3& pos, const atn::Vector3& offset); // inferred: inline (name invented)
    void fn_80318F98(); // inferred: empty (called by vf10 before fn_80317EF8)
    // inferred: the move of fn_80319384 has finished (name invented)
    bool IsMoveDone_unk() const { return m151C >= 1.0f; }
    void fn_803176D0(); // inferred: shares the targets between all the laser spheres
    void fn_80317EA0(); // inferred: picks the target (the leader or the default target)
    void fn_80317EF8(); // inferred: state 1: waits, then moves around the target
    void fn_80318594(); // inferred: updates the collision capsule from the position
    atn::Vector3 fn_80318648(CharaTask* chara); // inferred: "spine1" of `chara` (or its position + 100)
    void fn_80318720(); // inferred: turns towards the target
    CharaTask* fn_80318A30(); // inferred: a player the sphere can attack (state 5)
    int fn_80318AA4(CharaTask* chara); // inferred: nonzero when `chara` is in reach
    void fn_80318F9C(); // inferred: state 11: waits 90 frames
    void fn_803191FC(); // inferred: state 6: appears
    void fn_80319384(); // inferred: states 0 and 12: moves to m1510
    void fn_80319578(); // inferred: state 8
    void fn_8031958C(); // inferred: state 9 (tackle)
    void fn_803199A4(); // inferred: state 10: orbits the target
    void fn_80319C10(); // inferred: back to state 0
    void fn_80319C90(); // inferred: fires the laser
    void fn_8031A484(); // inferred: dies
    void fn_8031A670(CharaTask* chara); // inferred: attacks `chara`

    atn::LH_Binary m14B0;
    CharaTask* m14B8; // inferred: target
    int m14BC;        // inferred: step of the state
    int m14C0;        // inferred: frame counter
    int m14C4;        // inferred: nonzero when the target is shared with other spheres
    int m14C8;        // inferred: index among the spheres with the same target
    int m14CC;        // inferred: number of spheres with the same target
    int m14D0;        // inferred: "target=" id
    CharaTask* m14D4; // inferred: default target
    atn::Vector3 m14D8;
    atn::Vector3 m14E4; // inferred: appearance position
    float m14F0;        // inferred: "AppearMoveRate"
    atn::Vector3 m14F4;
    float m1500;
    atn::Vector3 m1504; // inferred: start of the move
    atn::Vector3 m1510; // inferred: end of the move
    float m151C;        // inferred: progress of the move
    int m1520;          // inferred: frame counter of the wait
    int m1524;
    atn::Vector3 m1528; // inferred: offset of the orbit
    float m1534;        // inferred: angle of the orbit
    float m1538;        // inferred: target angle of the orbit
    int m153C;
    atn::Vector3 m1540; // inferred: velocity of the fall
    float m154C;        // inferred: gravity
    atn::EfpHandle_unk m1550; // inferred: "ef_uc066.efp"
    atn::Vector3 m155C;
    atn::Vector3 m1568;
    int m1574;          // inferred: frames before the next attack
    int m1578;
    HitInfo_unk* m157C; // inferred: "LaserID"
    HitInfo_unk* m1580; // inferred: "TackleID"
    ResLoader_unk m1584;      // inferred: "ef_sk043c_01.eff"
    atn::EfpHandle_unk m1590; // inferred: "ef_sk043a.efp"
    ResLoader_unk m159C;      // inferred: "ef_sk043g_01.eff"
    ResLoader_unk m15A8;      // inferred: "ef_sk043g_02.eff"
    CharaTask* m15B4;         // inferred: character caught by the tackle
    atn::Matrix3x4 m15B8; // inferred: the empty constructor 0x803165E0 is the weak copy of Matrix3x4()
    atn::Matrix3x4 m15E8;
    atn::Matrix3x4 m1618;
    EffectModel_unk m1648[3]; // inferred: "em316_00_bs00.model" (trail of the sphere)
    int m1C84;                // inferred: "LaserWait"
    int m1C88;                // inferred: "MoveWait"
    ResHandle m1C8C;          // inferred: debug menu group
    int m1C90;                // inferred: "DebugDisp"
    int m1C94;                // inferred: "DebugLog"
    int m1C98;
    int m1C9C;
};

#endif
