#ifndef GAME_EM206EGGMONSTER_H
#define GAME_EM206EGGMONSTER_H

#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "atn/ColliBase.h"

// inferred from __vt__15Em206EggMonster (0x807866F0); the enemy factory (fn_805A5374) allocates 0x1718 bytes.
// Boss of the egg (model "em_dg004Boss_B"). Only part of the members are known; the gaps keep the offsets.
class Em206EggMonster : public EnemyTask {
public:
    Em206EggMonster(atn::Task* parent, void* data); // 0x802C0C38
    virtual ~Em206EggMonster();                     // 0x802C7AA8
    virtual int vf0C();                             // 0x802C0F28
    virtual void vf10();                            // 0x802C1208
    virtual void vf24();                            // 0x802C2B68
    virtual void vf3C(HitInfo_unk* hit);            // 0x802C25C4
    virtual void vf40(HitInfo_unk* hit);            // 0x802C2944
    virtual void vf50();                            // 0x802C5068
    virtual void vf54();                            // 0x802C5D4C
    virtual atn::Vector3 vf64(int center);          // 0x802C3DF8
    virtual float vf90();                           // 0x802C7AA0
    virtual int vfB8(wchar_t* name);                // 0x802C2AE8
    virtual int vfE4();                             // 0x802C7A60
    virtual void vfEC();                            // 0x802C46BC
    virtual int vfF4();                             // 0x802C64DC

    void fn_802C2BCC(); // inferred: movement
    int fn_802C38E4();  // inferred: chooses the next action
    void fn_802C3EDC(); // inferred: state 1 (follows the target)
    void fn_802C42D4(); // inferred: state 2
    void fn_802C4480(); // inferred: state 4
    void fn_802C4858(); // inferred: starts state 3 (attack)
    void fn_802C4B70(); // inferred: death
    void fn_802C4E80(int id, float radius, float angle);
    void fn_802C6878(CharaTask* chara, const atn::Vector3& pos);
    // inferred: ends the current action (name invented; inlined in every state)
    inline void EndAction_unk();

    unsigned char m14B0[0x14BC - 0x14B0];
    float m14BC;          // inferred: 1772.0f
    float m14C0;          // inferred: 1835.0f
    unsigned char m14C4[0x14D0 - 0x14C4];
    int m14D0;
    int m14D4;
    int m14D8;
    unsigned char m14DC[0x1524 - 0x14DC];
    int m1524;
    CharaTask* m1528;     // inferred: target of the attack
    unsigned char m152C[0x156C - 0x152C];
    atn::EffFile m156C;   // inferred: "/effect/ef_bs046b_01.eff"
    atn::EfpFile m1578;   // inferred: "/effect/ef_sk023a.efp"
    CharaTask* m1584;     // inferred: target
    CharaTask* m1588;     // inferred: previous target
    int m158C;
    int m1590;            // inferred: frames of the state
    int m1594;
    unsigned char m1598[0x15B0 - 0x1598];
    int m15B0;
    int m15B4;
    int m15B8;
    int m15BC;
    int m15C0;
    int m15C4;
    int m15C8;
    int m15CC;
    float m15D0[8];       // inferred: limits of the arena (1330.76f...)
    int m15F0;
    int m15F4;
    int m15F8;
    int m15FC;
    atn::ColliSphere m1600;  // inferred: "neck"
    atn::ColliCapsule m164C; // inferred: "tail1" to "tail3"
    atn::EfpFile m16A4;   // inferred: "/effect/ef_uc015.efp"
    atn::EfpFile m16B0;   // inferred: "/effect/ef_sk016a.efp"
    int m16BC;
    float m16C0;          // inferred: -1.0f
    atn::Matrix3x4 m16C4; // inferred: matrix of the effect m16F4
    atn::EffFile m16F4;   // inferred: "/effect/ef_ui010b.eff"
    int m1700;
    chr::CharaData_unk* m1704; // inferred: copy of CharaTask::m5C
    chr::CharaData_unk* m1708; // inferred: fn_8020A81C("em305")
    ResHandle m170C;      // inferred: debug menu group
    int m1710;            // inferred: "DebugDisp"
    int m1714;
};

#endif
