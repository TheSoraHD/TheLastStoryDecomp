#ifndef GAME_EM005DOPPELGANGER_H
#define GAME_EM005DOPPELGANGER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/CharaStatus.h"
#include "game/ResHandle.h"

// inferred from __vt__17Em005DoppelGanger (0x80784210): doppelganger enemy (copies a character,
// "SrcCopyCharaId"/"DstCopyRobotId"). Data file "database/chara/special/" + db_name (default
// "db_em005_00_00.csv"). Only the small functions are written (game/Em005DoppelGanger.cpp).
// Names invented; the factory allocates 0x1558 bytes.
class Em005DoppelGanger : public EnemyTask {
public:
    Em005DoppelGanger(atn::Task* parent, EnemyInfo_unk* info); // 0x80250A24
    virtual ~Em005DoppelGanger();                               // 0x80250D90
    virtual int vf0C();                                         // 0x80250ECC
    virtual void vf10();                                        // 0x80251344
    virtual void vf14();                                        // 0x80252A84
    virtual void vf3C(HitInfo_unk* hit);                        // 0x80252A90
    virtual void vf40(HitInfo_unk* hit);                        // 0x80252C40
    virtual void vf44();                                        // 0x80252E48
    virtual void vf54();                                        // 0x802529CC
    virtual void vfF0();                                        // 0x80253684

    void fn_80253C9C(); // inferred: swaps places with a random party member (end of state)
    void fn_80252E6C(); // inferred: state 0 (chooses the next action)
    inline void ResetStep_unk();
    void fn_80253884(); // inferred: state 1 (spell / move along the route)
    inline void Move_unk();
    inline int IsHiddenPhase_unk();
    void fn_802531F4(); // inferred: state of the appearance (copies the character and shows itself)

    int m14B0;
    int m14B4; // inferred: step of the state
    int m14B8;
    int m14BC; // inferred: phase (0 while hidden, 2 copying a character)
    int m14C0; // inferred: wait counter (240)
    int m14C4;
    atn::EfpHandle_unk m14C8;
    atn::EfpHandle_unk m14D4;
    atn::EfpHandle_unk m14E0;
    atn::EfpHandle_unk m14EC;
    int m14F8;        // inferred: "DstCopyRobotId"
    CharaTask* m14FC; // inferred: the copy robot (its model plays the effect m14E0)
    int m1500;        // inferred: "SrcCopyCharaId" (negative: the first player)
    CharaTask* m1504; // inferred: copied character (the only attacker that can hurt it while hidden)
    atn::LH_Binary m1508;
    int m1510;
    int m1514;
    int m1518;
    int m151C;
    int m1520; // inferred: hit counter of phase 2
    int m1524; // inferred: hits before the stagger (4)
    RoutePlace_unk* m1528; // inferred: "JumpPoint"
    RoutePlace_unk* m152C; // inferred: "WarpPoint"
    unsigned char m1530[0x1540 - 0x1530];
    int m1540; // inferred: hit counter (capped at 60)
    int m1544; // inferred: frame counter
    int m1548;
    int m154C;
    ResHandle m1550; // inferred: debug menu group
    int m1554;       // inferred: "DebugDisp"
};

#endif
