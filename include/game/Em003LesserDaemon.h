#ifndef GAME_EM003LESSERDAEMON_H
#define GAME_EM003LESSERDAEMON_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__17Em003LesserDaemon (0x80784210). Data file "database/chara/special/" + db_name
// (default "db_em003_00_00.csv"). Members keep their offsets (names unknown).
class Em003LesserDaemon : public EnemyTask {
public:
    Em003LesserDaemon(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em003LesserDaemon();
    virtual void vf14();
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf88();
    virtual void vfEC();

    void fn_8024FCDC(); // inferred: starts the death sequence
    void fn_8024FE18(int arg); // inferred: death of a boss / of a linked daemon

    unsigned char m14B0[0x14B8 - 0x14B0];
    int m14B8;
    int m14BC;
    unsigned char m14C0[0x14D8 - 0x14C0];
    HitInfo_unk* m14D8;       // inferred: explosion skill
    int m14DC;
    unsigned char m14E0[0x14E4 - 0x14E0];
    int m14E4;
    unsigned char m14E8[0x14EC - 0x14E8];
    atn::EfpHandle_unk m14EC; // inferred: "/effect/ef_cm007.efp"
    atn::EfpHandle_unk m14F8;
    atn::EfpHandle_unk m1504;
    atn::EfpHandle_unk m1510;
    atn::EfpHandle_unk m151C;
    ResLoader_unk m1528;
    ResLoader_unk m1534;
    atn::EfpHandle_unk m1540;
    atn::EfpHandle_unk m154C;
    atn::EfpHandle_unk m1558;
    atn::EfpHandle_unk m1564;
    atn::EfpHandle_unk m1570;
    atn::EfpHandle_unk m157C;
    atn::LH_Binary m1588;     // inferred: data file
    unsigned char m1590[0x16D8 - 0x1590];
    unsigned int mLinkCount;          // inferred: linked daemons (0x16D8)
    Em003LesserDaemon* mLinks[2];     // 0x16DC
    int m16E4;                // inferred: "Boss"? (nonzero: the death is handled by fn_8024FE18)
    ResHandle m16E8;          // inferred: debug menu group
};

#endif
