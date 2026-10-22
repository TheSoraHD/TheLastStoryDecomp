#ifndef GAME_EM101ELDERZESYASECOND_H
#define GAME_EM101ELDERZESYASECOND_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: warp points of the enemy ("warp_point"; a class derived from std::vector: its destructor
// tests `this` four times); name invented
struct RoutePlace_unk;
struct ElderList_unk : public std::vector<RoutePlace_unk*> {};

// inferred from __vt__21Em101ElderZesyaSecond (0x807847E0): second fight of the elder Zesya.
// Only the small functions are written (game/Em101ElderZesyaSecond.cpp); the factory allocates
// 0x15E8 bytes. Names invented.
class Em101ElderZesyaSecond : public EnemyTask {
public:
    Em101ElderZesyaSecond(atn::Task* parent, EnemyInfo_unk* info); // 0x8025A060
    virtual ~Em101ElderZesyaSecond();                               // 0x8025A4A4
    virtual int vf0C();
    virtual void vf14();                                            // 0x8025B5D4
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    // inferred: returns 4 (hit refused) unless the hit has the range of the current mode
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir); // 0x8025C92C

    void fn_8025C624(); // inferred: the barrier breaks
    void fn_8025C460(); // inferred: warps (state 11)
    // inferred: one of the three warp points nearest to the enemy (position and rotation)
    int fn_8025C7A8(atn::Vector3* pos, float* rot);
    void fn_8025C29C(int kind); // inferred: casts magic `kind` (state 7)
    void fn_8025BD1C();         // inferred: state 7 (chant, rising)
    void fn_8025B9EC();         // inferred: walks to the target (route or straight)
    void fn_8025BF58();         // inferred: state 8 (casts the magic at frame 62)

    atn::LH_Binary m14B0;
    int m14B8;          // inferred: "warp_count"
    float m14BC;        // inferred: "warp_count_dist"
    ElderList_unk m14C0;
    RoutePlace_unk* m14CC; // inferred: "center_warp_point"
    CharaTask* m14D0;      // inferred: target
    int m14D4;
    int m14D8;
    int m14DC;
    int m14E0;
    atn::Vector3 m14E4; // inferred: position of the magic
    atn::Vector3 m14F0; // inferred: position where the magic started
    int m14FC[2];       // inferred: magic ids (0x5E6, 0x5EA), indexed by m14E0
    int m1504;
    atn::Vector3 m1508; // inferred: warp destination
    float m1514;        // inferred: warp rotation
    atn::EfpHandle_unk m1518;
    atn::EfpHandle_unk m1524;
    atn::EfpHandle_unk m1530;
    atn::EfpHandle_unk m153C;
    atn::EfpHandle_unk m1548;
    int m1554; // inferred: nonzero while hits are filtered by vf4C
    unsigned char m1558[0x155C - 0x1558];
    atn::EfpHandle_unk m155C;
    atn::EfpHandle_unk m1568;
    atn::EfpHandle_unk m1574;
    atn::ColliSphere m1580; // inferred: barrier sphere (owner m158C, radius m15C8)
    int m15CC;
    int m15D0;
    int m15D4; // inferred: mode (0 and 1 accept different hit ranges)
    int m15D8; // inferred: hit counter
    int m15DC; // inferred: barrier break counter
    ResHandle m15E0; // inferred: debug menu group
    int m15E4;
};

#endif
