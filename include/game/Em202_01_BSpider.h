#ifndef GAME_EM202_01_BSPIDER_H
#define GAME_EM202_01_BSPIDER_H

#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/Em202SpiderBase.h"
#include "game/EnemyTask.h"

// inferred from __vt__16Em202_01_BSpider (0x80784F48). The factory allocates 0x1540 bytes and passes the
// character data (info->m20) to the constructor. Members keep their offsets (names invented).
class Em202_01_BSpider : public EnemyTask {
public:
    Em202_01_BSpider(atn::Task* parent, void* data); // 0x8026FC0C
    virtual ~Em202_01_BSpider();                     // 0x8026FDD0
    virtual int vf0C();                              // 0x8026FECC
    virtual void vf10();                             // 0x8026FFE4
    virtual void vf14();                             // 0x80270500
    virtual void vf3C(HitInfo_unk* hit);             // 0x80270774
    virtual void vf40(HitInfo_unk* hit);             // 0x802709E0
    virtual void vf7C(int arg);                      // 0x80271074
    virtual void vfEC();                             // 0x80271654

    void fn_80270B8C(); // inferred: movement of the frame (route in state 7, facing m14B0)
    void fn_80271104(); // inferred: state 10 (needle attack)
    void fn_802714B0(); // inferred: state 11 (turn), then the action m14D8
    void fn_8027179C(CharaTask* target); // inferred: starts state 6 (attack)
    void fn_80271AA4(CharaTask* target); // inferred: starts state 8 (attack)
    void fn_80271DAC(CharaTask* target); // inferred: starts state 10 (needle attack at the party)
    void fn_802721A0(); // inferred: starts state 12 (runs to the farthest free spot)
    int fn_8027232C(CharaTask* target, int arg); // inferred: starts state 11 (turn) when the target is not in front
    void fn_80272660(); // inferred: death

    CharaTask* m14B0;         // inferred: target
    int m14B4;                // inferred: frames of the state
    int m14B8;
    int m14BC;                // inferred: spot taken (index of lbl_807C8338)
    int m14C0;
    int m14C4;                // inferred: counter of the needle attack
    int m14C8;                // inferred: attacks before the escape
    int m14CC;
    int m14D0;                // inferred: step of the needle attack (state 10)
    int m14D4;
    int m14D8;                // inferred: argument of the turn (state 11)
    int m14DC;                // inferred: frames of the turn
    float m14E0;              // inferred: start angle of the turn
    float m14E4;              // inferred: target angle of the turn
    CharaTask* m14E8;         // inferred: target of the turn
    int m14EC;
    SpiderBaseColor_unk m14F0; // inferred: color of the materials (vf14)
    ResLoader_unk m1500;      // inferred: "/effect/ef_sk002b.eff"
    atn::EfpHandle_unk m150C; // inferred: "/effect/ef_uc001.efp"
    atn::EfpHandle_unk m1518; // inferred: "/effect/ef_sk002d.efp"
    atn::EfpHandle_unk m1524; // inferred: "/effect/ef_uc007.efp"
    atn::EfpHandle_unk m1530; // inferred: "/effect/ef_sc003_01b.efp"
    atn::SoundRef m153C;
};

#endif
