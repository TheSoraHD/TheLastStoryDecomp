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
    virtual void vf24();
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf88();
    virtual void vfEC();

    void fn_8024FCDC(); // inferred: starts the death sequence
    void fn_8024FE18(int arg); // inferred: death of a boss / of a linked daemon
    void fn_8024E1C0(); // inferred: starts the queued action m1654 (m1658 is the next one)
    int fn_8024F9B8();  // inferred: finds a free warp position behind the target (mWarpDest)
    void fn_802506AC(); // inferred: warps to the point farthest from the target
    int fn_80250884(float radius);
    inline int FarthestPoint_unk();
    inline void StartWarp_unk();
    inline int FarthestFromSelf_unk();
    const atn::Vector3& GetPoint_unk(unsigned int i) const { return mPoints[i]; }

    CharaTask* m14B0;         // inferred: target (its position is read by fn_802506AC)
    int m14B4;
    int m14B8;
    int m14BC;
    int m14C0;
    int m14C4;
    int m14C8;
    int m14CC;
    HitInfo_unk* m14D0;       // inferred: magic cast by fn_8024E1C0 (CharaTask::fn_8015F568)
    int m14D4;
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
    unsigned int mPointCount;     // 0x1590: inferred: warp points
    atn::Vector3 mPoints[8];      // 0x1594
    atn::Vector3 mWarpDest;       // 0x15F4: inferred: chosen warp point
    unsigned char m1600[0x1610 - 0x1600];
    atn::Vector3 m1610;           // inferred: another warp destination (action 11 of fn_8024E1C0)
    int m161C;
    int m1620;
    int m1624;
    int m1628[6];             // inferred: "AngryCount1".. per phase (names from the pool)
    int m1640;
    float m1644;
    float m1648;
    int m164C;
    int m1650;
    int m1654;
    int m1658;
    int m165C;
    float m1660;              // inferred: ranges (35, 50, 80, 60)
    float m1664;
    float m1668;
    float m166C;
    int m1670;
    int m1674[6];
    int m168C[6];
    int m16A4[6];
    int m16BC;
    float m16C0;
    int m16C4;
    float m16C8;
    int m16CC;
    int m16D0;
    int m16D4;
    unsigned int mLinkCount;          // inferred: linked daemons (0x16D8)
    Em003LesserDaemon* mLinks[2];     // 0x16DC
    int m16E4;                // inferred: "Boss"? (nonzero: the death is handled by fn_8024FE18)
    ResHandle m16E8;          // inferred: debug menu group
    int m16EC;
};

#endif
