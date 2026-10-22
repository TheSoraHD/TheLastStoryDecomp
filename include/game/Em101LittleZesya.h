#ifndef GAME_EM101LITTLEZESYA_H
#define GAME_EM101LITTLEZESYA_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred: position and rotation saved by fn_8025D6D0 / sent by Em101ElderZesya (fn_8025FE30); names
// invented
struct ZesyaWarp_unk {
    int m00;
    int m04;
    atn::Vector3 m08;
};

// inferred from __vt__16Em101LittleZesya (0x80784958): small Zesya (summoned by Em101ElderZesya, which
// calls fn_8025D6D0, fn_8025D704 and fn_8025FE30).
class Em101LittleZesya : public EnemyTask {
public:
    Em101LittleZesya(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em101LittleZesya();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);

    void fn_8025D7B0(); // inferred: runs the state machine
    void fn_8025DE54(); // inferred: state 0
    void fn_8025DE7C(); // inferred: state 0 of the spell (casts m638 while the control allows it)
    void fn_8025E748(); // inferred: state of the attack
    void fn_8025E930();
    void fn_8025F754();
    inline void Idle_unk();
    inline float HpRate_unk();

    atn::LH_Binary m14B0;   // inferred: data file
    CharaTask* m14B8;       // inferred: target
    int m14BC;
    int m14C0;
    atn::Vector_unk<atn::Vector3> m14C4; // inferred: points (as Em101ElderZesya::m14BC)
    int m14D0;              // inferred: frame counter
    unsigned char m14D4[0x14EC - 0x14D4];
    int m14EC;
    int m14F0;
    int m14F4;
    int m14F8;              // inferred: the position below was saved
    atn::Vector3 m14FC;     // inferred: saved position
    atn::Vector3 m1508;     // inferred: saved rotation
    int m1514;
    int m1518;
    int m151C;
    int m1520;
    int m1524;
    int m1528;              // inferred: attack skill
    int m152C;
    int m1530;
    int m1534;
    int m1538;
    int m153C;
    float m1540;            // inferred: last frame
    unsigned char m1544[0x1568 - 0x1544];
    float m1568;            // inferred: added to the regeneration (CharaStatus +0x174)
    atn::EfpHandle_unk m156C;
    atn::EfpHandle_unk m1578;
    atn::EfpHandle_unk m1584;
    unsigned char m1590[0x15A8 - 0x1590];
    ZesyaWarp_unk m15A8;
    int m15BC;
    int m15C0;
    float m15C4;
    ResHandle m15C8;        // inferred: debug menu group
    int m15CC;
};

#endif
