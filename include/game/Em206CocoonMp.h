#ifndef GAME_EM206COCOONMP_H
#define GAME_EM206COCOONMP_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

// inferred from __vt__13Em206CocoonMp (0x80786568); the enemy factory (fn_805A5374) allocates 0x1938 bytes.
// Multiplayer version of the egg boss (game/Em206EggMonster.cpp). Layout from the constructor (0x802BA92C);
// the analysis of the states is in wip/enemies-third/Em206CocoonMp/cocoon_decode.txt.
class Em206CocoonMp : public SpecialEnemyBase {
public:
    Em206CocoonMp(atn::Task* parent, EnemyInfo_unk* info); // 0x802BA92C
    virtual ~Em206CocoonMp();                               // 0x802C0B68
    virtual int vf0C();                                     // 0x802BABCC
    virtual void vf10();                                    // 0x802BADB0
    virtual void vf20();                                    // 0x802BCE84
    virtual void vf24();                                    // 0x802BCD3C
    virtual void vf38(int arg);                             // 0x802BDF68
    virtual void vf3C(HitInfo_unk* hit);                    // 0x802BAFAC
    virtual void vf40(HitInfo_unk* hit);                    // 0x802BB104
    virtual void vf50();                                    // 0x802BBCDC
    virtual void vf54();                                    // 0x802BC240
    virtual atn::Vector3 vf64(int center);                  // 0x802BB368
    virtual float vf90();                                   // 0x802C0B60
    virtual int vfE4();                                     // 0x802C0B20
    virtual void vfEC();                                    // 0x802BDFF8
    virtual void vfFC();                                    // 0x802BB3B8
    virtual void vf100(int state);                          // 0x802BB410
    virtual void vf104();                                   // 0x802BB92C
    virtual void vf108();                                   // 0x802BBCD8
    virtual void vf110(SpecialState_unk* state);            // 0x802C07BC
    virtual void vf114(const SpecialState_unk* state);      // 0x802C07F8

    inline void SetState_unk(int state);
    inline void PlayMotion_unk(int motion, int loop, float speed);
    inline void Die_unk();
    // inferred: turns towards `rot` and moves forward at `speed` with gravity and collision (0x802BC4EC)
    void fn_802BC4EC(const atn::Vector3& rot, float speed, float max);
    void fn_802BD118(); // inferred: moves toward the target
    int fn_802BD480();  // inferred: chooses the next action
    void fn_802BDA04(); // inferred: colors of the materials
    void fn_802BDFA0(); // inferred: default state
    void fn_802BE0FC(); // inferred: state 6
    void fn_802BE180(); // inferred: state 8
    void fn_802BE2A0(); // inferred: state 9
    void fn_802BE7EC(); // inferred: state 10
    void fn_802BE9C8(); // inferred: state 11
    void fn_802C0458(); // inferred: state 16
    void fn_802C06B4(); // inferred: state 17
    void fn_802C0738(); // inferred: state 18

    atn::LH_Binary m14D4;   // inferred: "database/chara/special/" + db_name ("db_last_cocoon.csv")
    int m14DC;              // inferred: step of the state
    int m14E0;              // inferred: frames of the state
    int m14E4;
    int m14E8;
    int m14EC;              // inferred: counter of the colors
    int m14F0;              // inferred: ice mode
    int m14F4;              // inferred: timer
    atn::Vector3 m14F8;     // inferred: direction of state 13
    int m1504;
    atn::Vector3 m1508;     // inferred: start of the jump of state 16
    atn::Vector3 m1514;     // inferred: end of the jump of state 16 (route point 500)
    int m1520;
    int m1524;              // inferred: last m1520 sent as event flag 10
    int m1528;              // inferred: 5 (actions left)
    atn::Vector3 m152C;
    atn::ColliCapsule m1538[11]; // inferred: collision of the body (table lbl_80746A70)
    int m1900;
    int m1904;              // inferred: 900 (frames before the next blizzard)
    int m1908;
    float m190C;            // inferred: 1.0f
    int m1910;              // inferred: 0x4650
    int m1914;              // inferred: dead
    int m1918;
    atn::EfpFile m191C;     // inferred: "/effect/ef_uc015.efp"
    ResHandle m1928;        // inferred: debug menu group
    int m192C;              // inferred: "DebugDisp"
    float m1930;            // inferred: 40.0f
    int m1934;
};

#endif
