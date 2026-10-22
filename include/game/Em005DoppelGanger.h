#ifndef GAME_EM005DOPPELGANGER_H
#define GAME_EM005DOPPELGANGER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
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

    int m14B0;
    int m14B4;
    int m14B8;
    int m14BC; // inferred: state (0 while hidden)
    int m14C0;
    int m14C4;
    atn::EfpHandle_unk m14C8;
    atn::EfpHandle_unk m14D4;
    atn::EfpHandle_unk m14E0;
    atn::EfpHandle_unk m14EC;
    int m14F8;
    int m14FC;
    int m1500;
    int m1504;
    atn::LH_Binary m1508;
    int m1510;
    int m1514;
    int m1518;
    int m151C;
    int m1520;
    int m1524;
    int m1528;
    int m152C;
    unsigned char m1530[0x1540 - 0x1530];
    int m1540;
    int m1544;
    int m1548;
    int m154C;
    ResHandle m1550; // inferred: debug menu group
    int m1554;       // inferred: "DebugDisp"
};

#endif
