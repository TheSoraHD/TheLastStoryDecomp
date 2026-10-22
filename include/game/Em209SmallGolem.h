#ifndef GAME_EM209SMALLGOLEM_H
#define GAME_EM209SMALLGOLEM_H

#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__15Em209SmallGolem (0x80786B18); layout from the constructor (0x802CCFD8)
class Em209SmallGolem : public EnemyTask {
public:
    Em209SmallGolem(atn::Task* parent, EnemyInfo_unk* info); // 0x802CCFD8
    virtual ~Em209SmallGolem();                              // 0x802CD2E4
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf50();
    virtual void vf54();
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset);
    virtual void vf84(int motion, float blend);
    virtual void vf98();
    virtual void vf9C(int arg);
    virtual void vfEC();

    atn::LH_Binary m14B0; // inferred: data file
    int m14B8;
    int m14BC;
    int m14C0;
    int m14C4;
    int m14C8;
    atn::Vector3 m14CC;
    atn::Vector3 m14D8;
    atn::Vector3 m14E4;
    atn::Vector3 m14F0;
    unsigned char m14FC[0x150C - 0x14FC];
    int m150C;
    int m1510; // inferred: motion id (0x407, Em209BigGolem 0x409)
    int m1514; // inferred: motion id (0x406, Em209BigGolem 0x408)
    float m1518;
    float m151C;
    int m1520;
    int m1524;
    atn::EfpHandle_unk m1528;
    atn::EfpHandle_unk m1534;
    int m1540;
    int m1544;
    unsigned char m1548;
    unsigned char m1549;
    int m154C;
    ResHandle m1550; // inferred: debug menu group
    int m1554;
};

#endif
