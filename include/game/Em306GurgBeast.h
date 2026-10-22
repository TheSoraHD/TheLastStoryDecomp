#ifndef GAME_EM306GURGBEAST_H
#define GAME_EM306GURGBEAST_H

#include "atn/FixedArray.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

struct RoutePlace_unk;

// inferred from __vt__14Em306GurgBeast (0x8078778C): beast ridden by another enemy ("ride=" id;
// "rideplayer", "reride"). Data file "/database/chara/special/" + db_name (default "db_em306_00_00.csv").
class Em306GurgBeast : public SpecialEnemyBase {
public:
    Em306GurgBeast(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em306GurgBeast();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf30(void* menu);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual int vf10C(HitMsg_unk* msg);

    void fn_802EE9A8(const char* text, int length); // inferred: reads the data file
    void fn_802F0F60(); // inferred: walk motion
    inline void CopyTrack_unk(Model_unk* dst, int i);
    void fn_802F1120(); // inferred: keeps the rider on the seat
    void fn_802F1240(CharaTask* rider); // inferred: drops the rider
    void fn_802F1320(int restart); // inferred: throws the rider off

    int m14D4;
    atn::Vector3 m14D8;
    int m14E4;                 // inferred: id of the rider ("ride=")
    EnemyTask* m14E8;          // inferred: rider
    int m14EC;                 // inferred: "SkillId"
    int m14F0;                 // inferred: "WaitFrame"
    atn::FixedArray_unk<RoutePlace_unk*, 8> m14F4; // inferred: "HomePoint" list
    int m1518;
    ResHandle m151C;           // inferred: debug menu group
    int m1520;
    int m1524;                 // inferred: "rideplayer"
    int m1528;                 // inferred: "reride"
    EnemyTask* m152C;          // inferred: rider from the player side ("rideplayer")
    int m1530;                 // inferred: seat node matrix (MtxPtr stored as int, as CharaTask::mF1C)
    int m1534;
    int m1538;
    int m153C;
    int m1540;
    int m1544;
    int m1548;
    int m154C;
    int m1550;
    int m1554;
    int m1558;
    int m155C;
    float m1560;
};

#endif
