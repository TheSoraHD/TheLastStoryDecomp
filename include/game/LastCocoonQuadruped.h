#ifndef GAME_LASTCOCOONQUADRUPED_H
#define GAME_LASTCOCOONQUADRUPED_H

#include "game/EnemyTask.h"
#include "atn/EfpHandle_unk.h"

// inferred from __vt__19LastCocoonQuadruped (0x8078FBC0): quadruped form of the last cocoon; the enemy
// factory (fn_805A5374) allocates 0x18F8 bytes. Members keep their offsets (names unknown).
class LastCocoonQuadruped : public EnemyTask {
public:
    LastCocoonQuadruped(atn::Task* parent, EnemyInfo_unk* info); // 0x8045FAB0
    virtual void vf14();                                         // 0x80460A90
    virtual void vf3C(HitInfo_unk* hit);                         // 0x80460A94
    virtual atn::Vector3 vf64(int center);                       // 0x80460EF0
    virtual float vf90();                                        // 0x80468D40
    virtual int vfE4();                                          // 0x80468D18
    void fn_804654F0(); // inferred: back to mode 0 at the end of the motion
    void fn_804617A4(); // inferred: chooses the target of the mode
    void fn_80461838();
    void fn_804635B4();
    void fn_80463C58(); // inferred: mode 9 at frame 5: lands on the ground under m14D4
    void fn_80463DCC(); // inferred: end of the motion: back to mode 0 (states 1, 7) or mode 7
    void fn_80466868(); // inferred: end of the motion of mode 6 (shake at frame 3)
    void fn_80466974(); // inferred: back to mode 0
    void fn_80466AE4(); // inferred: mode 7 (motion 0x156)
    void fn_80466B8C(); // inferred: mode 8 (motion 0x13F and the effect m1790)
    // inferred: starts a mode (names invented)
    void SetMode_unk(int mode);

    unsigned char m14B0[0x14B8 - 0x14B0];
    CharaTask* m14B8; // inferred: target
    int m14BC;
    int m14C0; // inferred: frame counter of the mode
    int m14C4; // inferred: state (7: no hit reaction)
    unsigned char m14C8[0x14D4 - 0x14C8];
    atn::Vector3 m14D4; // inferred: position to land on
    atn::Vector3 m14E0; // inferred: rotation after landing
    unsigned char m14EC[0x1554 - 0x14EC];
    CharaTask* m1554; // inferred: target of modes 11 and 12
    unsigned char m1558[0x1574 - 0x1558];
    CharaTask* m1574; // inferred: target of mode 13
    unsigned char m1578[0x15C8 - 0x1578];
    CharaTask* m15C8; // inferred: target of mode 17
    unsigned char m15CC[0x1790 - 0x15CC];
    atn::EfpFile m1790;
    unsigned char m179C[0x1800 - 0x179C];
    int m1800; // inferred: an effect is attached to the motion
    unsigned char m1804[0x18F8 - 0x1804];
};

#endif
