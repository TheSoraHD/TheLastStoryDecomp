#ifndef GAME_EM101LITTLEZESYASECOND_H
#define GAME_EM101LITTLEZESYASECOND_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/CharaStatus.h"
#include "game/ResHandle.h"

// inferred from __vt__22Em101LittleZesyaSecond (0x80784AC8): second fight of the little Zesya.
// Only the small functions are written (game/Em101LittleZesyaSecond.cpp); the factory allocates
// 0x1558 bytes. Names invented.
class Em101LittleZesyaSecond : public EnemyTask {
public:
    Em101LittleZesyaSecond(atn::Task* parent, EnemyInfo_unk* info); // 0x8025FE60
    virtual ~Em101LittleZesyaSecond();                               // 0x80262840
    virtual int vf0C();                                              // 0x80260218
    virtual void vf10();                                             // 0x802604E0
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir);
    virtual int vfA8(int kind);                                      // 0x802627E8

    void fn_80260324(); // inferred: reads the data file
    void fn_802620A8(); // inferred: death
    void fn_80261948(); // inferred: state 0 (control updates)
    void fn_80261A50(); // inferred: fires a homing bullet at every player from the hips (frame 60)
    void fn_80261078(); // inferred: chooses the next action
    void fn_80261DC0(); // inferred: state 11 (camera)
    void fn_802621D4(atn::Vector3* pos, float* rot); // inferred: warp destination
    inline void ResetAction_unk();
    inline void ReleaseCamera_unk();
    inline void Cast_unk();
    inline void Slash_unk();

    atn::LH_Binary m14B0;
    CharaTask* m14B8;   // inferred: target
    int m14BC;
    int m14C0;
    int m14C4;          // inferred: frame counter
    HitInfo_unk* m14C8; // inferred: "NormalAttackID"
    int m14CC;
    int m14D0;
    int m14D4;
    int m14D8;
    unsigned char m14DC[0x14E8 - 0x14DC];
    int m14E8;
    float m14EC;
    float m14F0;
    float m14F4;
    float m14F8;
    atn::Vector3 m14FC; // inferred: "WarpCenterPoint"
    atn::Vector3 m1508;
    atn::Vector3 m1514; // inferred: position of the death
    atn::EfpHandle_unk m1520;
    atn::EfpHandle_unk m152C;
    atn::EfpHandle_unk m1538;
    float m1544; // inferred: last frame
    int m1548;
    int m154C;
    ResHandle m1550; // inferred: debug menu group
    int m1554;
};

#endif
