#ifndef GAME_EM101LITTLEZESYA_H
#define GAME_EM101LITTLEZESYA_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/CharaStatus.h"
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

    void fn_8025CFF4(); // inferred: reads the data file
    void fn_8025D7B0(); // inferred: runs the state machine
    void fn_8025D90C(); // inferred: chooses the next action
    void fn_8025E100();
    void fn_8025F07C(); // inferred: warp action 0
    void fn_8025F45C(); // inferred: warp action 1
    void fn_8025DE54(); // inferred: state 0
    void fn_8025F35C(); // inferred: back to state 0 (half of m15C0 as frame counter)
    void fn_8025E9E4(); // inferred: state 12 (warp at frame 60, back to state 0 at frame 100)
    void fn_8025EAE4(); // inferred: warp
    void fn_8025E008(); // inferred: starts state 11 (effect m156C)
    void fn_8025E834(); // inferred: starts state 13 after 10 frames (effect m1578)
    void fn_8025DE7C(); // inferred: state 0 of the spell (casts m638 while the control allows it)
    void fn_8025E748(); // inferred: state of the attack
    void fn_8025E930();
    void fn_8025F754();
    inline void Idle_unk();
    inline int Nearest_unk(const atn::Vector3& pos);
    inline float HpRate_unk();
    inline void WarpNear_unk(CharaTask* target, atn::Vector3* dest, float* rot);

    atn::LH_Binary m14B0;   // inferred: data file
    CharaTask* m14B8;       // inferred: target
    RoutePlace_unk* m14BC;  // inferred: "WaitPointID"
    RoutePlace_unk* m14C0;  // inferred: "ShootPointID"
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
    float m1518;            // inferred: copied from m568 on load
    HitInfo_unk* m151C;     // inferred: "NormalAttackID"
    HitInfo_unk* m1520;     // inferred: "HighJumpAttackID"
    HitInfo_unk* m1524;     // inferred: "LightSwordID"
    HitInfo_unk* m1528;     // inferred: "WarpSlashID" (attack skill)
    int m152C;
    int m1530;
    int m1534;
    int m1538;
    int m153C;
    float m1540;            // inferred: last frame
    unsigned char m1544[0x1548 - 0x1544];
    int m1548;
    float m154C;
    float m1550;
    float m1554;
    float m1558;
    float m155C;
    float m1560;
    float m1564;
    float m1568;            // inferred: added to the regeneration (CharaStatus +0x174)
    atn::EfpFile m156C;
    atn::EfpFile m1578;
    atn::EfpFile m1584;
    atn::Vector3 m1590;     // inferred: warp destination
    unsigned char m159C[0x15A8 - 0x159C];
    ZesyaWarp_unk m15A8;
    int m15BC;
    int m15C0;
    float m15C4;
    ResHandle m15C8;        // inferred: debug menu group
    int m15CC;
};

#endif
