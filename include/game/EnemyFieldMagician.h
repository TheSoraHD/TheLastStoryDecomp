#ifndef GAME_ENEMYFIELDMAGICIAN_H
#define GAME_ENEMYFIELDMAGICIAN_H

#include "atn/FixedArray.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"

struct RoutePlace_unk;

// inferred from __vt__18EnemyFieldMagician (0x80788D20): a magician that raises a magic field
// (two collision objects) and warps between route places; no destructor declared (the implicit one
// is emitted last, 0x8032DDD0)
class EnemyFieldMagician : public EnemyTask {
public:
    EnemyFieldMagician(atn::Task* parent, EnemyInfo_unk* info);
    virtual int vf0C();
    virtual void vf10();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual bool vfC4();

    void fn_8032C964(); // inferred: reads the parameters from the csv file
    void fn_8032CF10(); // inferred: updates the field (timer and collision matrices)
    void fn_8032D1F4(); // inferred: raises the field (state 6)
    void fn_8032D48C(); // inferred: warps to the next place (state 9)
    // inferred: effect `index` of m15C0 in front of the magician (inlined; name invented)
    void PlayFieldEffect_unk(int index);

    atn::ColliObject m14B0[2];       // inferred: "ef_col00n.hcb", "ef_col01n.hcb" (field)
    atn::EfpFile m15C0[4];     // inferred: "ef_sk051a.efp" - "ef_sk051d.efp"
    atn::EfpFile m15F0;        // inferred: "ef_bs007a.efp" (warp)
    ResLoader_unk m15FC;             // inferred: "ef_bu040g_01.eff"
    int m1608;                       // inferred: the field is up
    int m160C;                       // inferred: frames before the weakened field comes back
    int m1610;                       // inferred: frames in state 7
    int m1614;                       // inferred: index of the next warp place
    int m1618;                       // inferred: warp requested
    atn::LH_Binary m161C;            // inferred: csv file ("db_name=")
    EnemyInfo_unk* m1624;            // inferred: spawn description
    int m1628;                       // inferred: "FieldWeakTime"
    atn::FixedArray_unk<RoutePlace_unk*, 8> m162C; // inferred: "WarpPoint"
    int m1650;                       // inferred: "WarpPointLoop"
    int m1654;                       // inferred: "FieldExecuteLocalFlag"
    int m1658;                       // inferred: "FieldWeakLocalFlag"
    int m165C;                       // inferred: "FieldBreakLocalFlag"
    int m1660;                       // inferred: "FieldUnuseLocalFlag"
};

#endif
