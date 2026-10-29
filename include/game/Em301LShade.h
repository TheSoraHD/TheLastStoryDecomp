#ifndef GAME_EM301LSHADE_H
#define GAME_EM301LSHADE_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"
#include "std/vector.h"

struct RoutePlace_unk;

// inferred: same layout as SpecialFlag_unk (global flag, index) with a constructor; the out-of-line
// check fn_802D0B6C follows vf0C (weak copy of an inline method). Names invented.
struct ShadeFlag_unk {
    ShadeFlag_unk() : mGlobal(-1), mIndex(0) {}
    bool IsSet_unk() { return mGlobal != -1; }
    int mGlobal;
    int mIndex;
};

// inferred: same layout as SpecialAim_unk of game/SpecialEnemyBase.cpp (node matrix, offset, radius,
// a flag and the enabled byte); names invented
struct ShadeAim_unk {
    ShadeAim_unk() : mNode(0), mOffset(0.0f, 0.0f, 0.0f), mRadius(0.0f), mEnabled(0) {}
    MtxPtr mNode;
    atn::Vector3 mOffset;
    float mRadius;
    ShadeFlag_unk mFlag; // inferred: "head_objectlock_notify_flag"
    unsigned char mEnabled;
};

// inferred: 0x1C-byte entry of m15AC (a character and two positions, fn_802D6004); names invented
struct ShadeTarget_unk {
    CharaTask* mChara;
    atn::Vector3 m04;
    atn::Vector3 m10;
};

// inferred: a class derived from std::vector (its destructor tests the address four times, as
// RoutePlaceList_unk of game/Em305Vampire.h); names invented
struct ShadePlaceList_unk : public std::vector<RoutePlace_unk*> {};
struct ShadeEnemyList_unk : public std::vector<EnemyTask*> {};

// inferred from __vt__11Em301LShade (0x80786C44): the "L shade" boss (absorbs the magic of the party,
// calls reinforcements). Data file "database/chara/special/" + db_name (default "db_em301_00_00.csv").
// The factory allocates 0x16B0 bytes (SpecialEnemyBase.cpp, case 29). The destructor is implicit (it
// is the last function of the file). Names invented; members keep their offsets.
class Em301LShade : public SpecialEnemyBase {
public:
    Em301LShade(atn::Task* parent, EnemyInfo_unk* info);
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf50();
    virtual void vf54();
    virtual atn::Vector3 vf64(int center);
    virtual void vfEC();
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();

    inline void SetState_unk(int state);
    inline void ShowMessage_unk(int id);
    inline bool IsMotionEnd_unk();
    inline void StartAbsorb_unk(int kind);
    void fn_802D3370();
    int fn_802D3C74();
    int fn_802D404C();
    void fn_802D4238();
    void fn_802D4378();
    void fn_802D4628();
    void fn_802D4E9C();
    void fn_802D5020();
    void fn_802D5674(int count);
    void fn_802D57B8(); // inferred: starts state 2 (dead)
    int fn_802D5D18(int kind);
    int fn_802D6004();
    void fn_802D67F8(int index);
    int fn_802D7058(); // inferred: nonzero when no party member is held

    int m14D4;                  // inferred: "big_shade"
    int m14D8;                  // inferred: copy of "first_appear_number"
    int m14DC;
    int m14E0;
    int m14E4;                  // inferred: step counter
    int m14E8;                  // inferred: frame counter of the state
    int m14EC;                  // inferred: hits received (vf40)
    int m14F0;                  // inferred: "damaged_count_max"
    int m14F4;
    int m14F8;
    int m14FC;
    int m1500;
    int m1504;
    int m1508;
    int m150C;
    int m1510;
    atn::EfpFile m1514; // inferred: "/effect/ef_sk006a.efp"
    atn::EfpFile m1520; // inferred: "/effect/ef_sk006c.efp"
    int m152C;
    int m1530;                  // inferred: "swing_skill_id"
    int m1534;                  // inferred: "shades_skill_id"
    int m1538;
    atn::Vector3 m153C;
    atn::EfpFile m1548; // inferred: "/effect/ef_sk012a.efp"
    atn::EfpFile m1554; // inferred: "/effect/ef_sk012c.efp"
    int m1560;
    float m1564;                // inferred: "absorb_rate"
    float m1568;                // inferred: "absorb_subtract_rate"
    float m156C;                // inferred: "absorb_min_rate"
    atn::Vector3 m1570;         // inferred: centre of the absorption (head node, fn_802D4628)
    ResLoader_unk m157C;        // inferred: "/effect/ef_mb001b.eff"
    atn::EfpFile m1588; // inferred: "/effect/ef_sk013a.efp"
    atn::EfpFile m1594; // inferred: "/effect/ef_sk013d.efp"
    atn::EfpFile m15A0; // inferred: "/effect/ef_sk013c.efp"
    std::vector<ShadeTarget_unk> m15AC;
    int m15B8;                  // inferred: "dead_type"
    float m15BC;                // inferred: "dead_camera" (three values)
    float m15C0;
    float m15C4;
    int m15C8;                  // inferred: nonzero hides the debug display (vf14)
    atn::EfpFile m15CC; // inferred: "/effect/ef_uc020d.efp"
    atn::EfpFile m15D8; // inferred: "/effect/ef_uc020a.efp"
    int m15E4;                  // inferred: index into m15F0
    int m15E8;
    int m15EC;
    atn::EfpFile m15F0[2]; // inferred: "ef_uc063.efp", "ef_uc064.efp"
    atn::EfpFile m1608; // inferred: "/effect/ef_uc015.efp"
    atn::EfpFile m1614; // inferred: "ef_bd013.efp"
    CharaTask* m1620;           // inferred: target (mC58.mC4)
    int m1624;
    unsigned int m1628;         // inferred: "first_appear_number"
    ShadePlaceList_unk m162C;   // inferred: "appear_point"
    ShadePlaceList_unk m1638;   // inferred: "reinforce_point"
    ShadeEnemyList_unk m1644;   // inferred: "reinforce_chara"
    atn::LH_Binary m1650;       // inferred: data file
    ShadeAim_unk m1658;         // inferred: the head the player can aim at
    ShadeFlag_unk m1678;        // inferred: "disappear_notify_flag"
    ShadeFlag_unk m1680;        // inferred: "hit_magic_notify_flag"
    ShadeFlag_unk m1688;        // inferred: "attack_miss_notify_flag"
    ShadeFlag_unk m1690;        // inferred: "friend_shade_notify_flag"
    ShadeFlag_unk m1698;        // inferred: "absorb_notify_flag"
    ShadeFlag_unk m16A0;        // inferred: "reinforce_notify_flag"
    ResHandle m16A8;            // inferred: debug menu group
    int m16AC;
};

#endif
