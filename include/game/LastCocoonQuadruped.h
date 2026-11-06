#ifndef GAME_LASTCOCOONQUADRUPED_H
#define GAME_LASTCOCOONQUADRUPED_H

#include "game/EnemyTask.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ColliBase.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: a vector inside a struct (one more null test in the destructor; name invented)
struct QuadList_unk {
    std::vector<int> mItems;
};
// inferred: 0x50 bytes at 0x1504 with four vectors (element types unknown; names invented)
struct QuadRoutes_unk {
    unsigned char m00[0x20];
    std::vector<int> m20;
    std::vector<int> m2C;
    std::vector<int> m38;
    QuadList_unk m44;
};

// inferred from __vt__19LastCocoonQuadruped (0x8078FBC0): quadruped form of the last cocoon; the enemy
// factory (fn_805A5374) allocates 0x18F8 bytes. Members keep their offsets (names unknown).
class LastCocoonQuadruped : public EnemyTask {
public:
    LastCocoonQuadruped(atn::Task* parent, EnemyInfo_unk* info); // 0x8045FAB0
    virtual ~LastCocoonQuadruped();                              // 0x80468D48
    virtual void vf14();                                         // 0x80460A90
    virtual void vf20();                                         // 0x80461520
    virtual void vf24();                                         // 0x804628B4
    virtual void vf3C(HitInfo_unk* hit);                         // 0x80460A94
    virtual atn::Vector3 vf64(int center);                       // 0x80460EF0
    virtual float vf90();                                        // 0x80468D40
    virtual int vfE4();                                          // 0x80468D18
    void fn_80465294(); // inferred: mode 13: drains the hp of m1574 every 30 frames
    void fn_804654F0(); // inferred: back to mode 0 at the end of the motion
    void fn_80461398(); // inferred: places the hit spheres on the head, waist and spine
    void fn_804617A4(); // inferred: chooses the target of the mode
    void fn_80461838();
    void fn_804635B4();
    void fn_804638A4(); // inferred: turns to m14B8, then the bullet 0x5B4 at frames 19-26
    void fn_80463C58(); // inferred: mode 9 at frame 5: lands on the ground under m14D4
    void fn_80463DCC(); // inferred: end of the motion: back to mode 0 (states 1, 7) or mode 7
    void fn_80466370(); // inferred: mode 15: jump with the bullet 0x5B5 at frames 20-24
    void fn_804666D8(); // inferred: mode with the moving effect (frames 10-80)
    void fn_804678F4(); // inferred: mode 12: moves towards m1554 in 30 frames
    void fn_80467C18(CharaTask* target, const atn::Vector3& pos); // inferred: mode 15: jumps to pos
    void fn_80466868(); // inferred: end of the motion of mode 6 (shake at frame 3)
    void fn_80466974(); // inferred: back to mode 0
    // inferred: place of the given kind farthest from the players (names invented)
    atn::Vector3 fn_804687B0(int* id, int kind8, int kind4);
    atn::Vector3 fn_80468A08(int id, int* next); // inferred: position of the place after id
    void fn_80466A04(); // inferred: mode 6 (motion 0x14A and SE_EM206_001)
    void fn_80466AE4(); // inferred: mode 7 (motion 0x156)
    void fn_80466B8C(); // inferred: mode 8 (motion 0x13F and the effect m1790)
    // inferred: starts a mode (names invented)
    void SetMode_unk(int mode);

    atn::LH_Binary m14B0; // inferred: parameter file
    CharaTask* m14B8; // inferred: target
    int m14BC;
    int m14C0; // inferred: frame counter of the mode
    int m14C4; // inferred: state (7: no hit reaction)
    unsigned char m14C8[0x14D4 - 0x14C8];
    atn::Vector3 m14D4; // inferred: position to land on
    atn::Vector3 m14E0; // inferred: rotation after landing
    unsigned char m14EC[0x1504 - 0x14EC];
    QuadRoutes_unk m1504;
    CharaTask* m1554; // inferred: target of modes 11 and 12
    int m1558;
    atn::Vector3 m155C; // inferred: position of the target (mode 12)
    atn::Vector3 m1568; // inferred: step towards it (1/30 of the distance)
    CharaTask* m1574; // inferred: target of mode 13
    unsigned char m1578[0x15AC - 0x1578];
    CharaTask* m15AC;   // inferred: target of mode 15
    atn::Vector3 m15B0; // inferred: position to jump to (mode 15)
    atn::Vector3 m15BC; // inferred: step added to the position (frames 10-55 of fn_804666D8)
    CharaTask* m15C8; // inferred: target of mode 17
    unsigned char m15CC[0x15D8 - 0x15CC];
    atn::ColliCapsule mCapsules[5]; // 0x15D8
    atn::EfpFile m1790;
    atn::EfpFile m179C;
    atn::ColliCapsule m17A8; // inferred: enabled (bit 0 of mFlags) when mode 6 starts
    int m1800; // inferred: an effect is attached to the motion
    unsigned char m1804[0x1824 - 0x1804];
    atn::EffFile m1824;
    atn::EfpFile m1830;
    atn::EffFile m183C;
    atn::EffFile m1848;
    unsigned char m1854[0x18F0 - 0x1854];
    ResHandle m18F0; // inferred: debug menu group
    unsigned char m18F4[0x18F8 - 0x18F4];
};

#endif
