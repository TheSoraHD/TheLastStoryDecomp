#ifndef GAME_EM205OGRE_H
#define GAME_EM205OGRE_H

#include "atn/ResLoader_unk.h"
#include "game/SpecialEnemyBase.h"
#include <boost/scoped_ptr.hpp>

struct Bullet_unk;

// inferred from __vt__9Em205Ogre (0x807863B4): ogre driven by the SpecialEnemyBase state machine. Data
// file "/database/chara/special/" + db_name (default "db_em205_00_00.csv"). The factory allocates
// 0x1518 bytes (SpecialEnemyBase.cpp, case 23).
class Em205Ogre : public SpecialEnemyBase {
public:
    Em205Ogre(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em205Ogre();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf30(void* menu);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf50();
    virtual void vf54();
    virtual void vf7C(int arg);
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual int vfB0(atn::Vector3* out);
    virtual boost::function<void()> vfB4(CharaTask* chara);
    virtual int vf10C(HitMsg_unk* msg);

    void fn_802B7AF8(const char* text, int length); // inferred: reads the data file
    void fn_802B98B0(); // inferred: updates the collision capsule from the nodes
    void fn_802B9C5C(); // inferred: finds the nearest heal circle (m14F8)
    inline int HpPercent_unk();
    bool IsHealing_unk() const { return m14FC != 0; } // inferred: name invented
    inline bool WantsHeal_unk();

    CharaTask* m14D4;      // inferred: target (mC58.mC4 when the state starts)
    atn::Vector3 m14D8;    // inferred: direction of the attack
    float m14E4;           // inferred: "WaitFrame"? (float read with fn_800DC288)
    int m14E8;             // inferred: "SkillId"
    int m14EC;             // inferred: "SwingSkillId"
    int m14F0;             // inferred: "SpitSkillId"
    int m14F4;             // inferred: "WaitFrame"
    Bullet_unk* m14F8;     // inferred: nearest heal circle
    int m14FC;
    int m1500;             // inferred: "HealCircleMoveHpMin"
    int m1504;             // inferred: "HealCircleMoveHpMax"
    ResHandle m1508;       // inferred: debug menu group
    int m150C;
    int m1510;             // inferred: set by vf10C when a hit is reduced (state 9)
    // inferred: "/data/eff/gm001_000a02.eff" (only in the map of area 2, 10, 2); the safe-bool member
    // pointer of the scoped_ptr is lbl_80786360 (get, 0x802B98A8)
    boost::scoped_ptr<ResLoader_unk> m1514;
};

#endif
