#ifndef CHR_CONTROLCLIFFHANG_H
#define CHR_CONTROLCLIFFHANG_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: hangs from a ledge and moves along it (route links of type 5); created by
// CharaTask::fn_8016939C (control 0x3A). The class declares no destructor (implicit destructor at
// 0x801AC3F4).
class ControlCliffhang : public ControlBase {
public:
    ControlCliffhang(Chara_unk* chara, int a, int flags, float f); // 0x801AAB64
    virtual int vf0C();                                            // 0x801AAC48
    virtual boost::function<void()> vf10();                        // 0x801AB294
    virtual atn::Vector3 vf14();                                   // 0x801AC2FC
    // inferred: starts moving along route link `route` (`first`: use `angle` as the start
    // direction); 0x801AB6B8
    void fn_801AB6B8(int route, int first, float angle);
    int fn_801AB9E4(int route); // inferred: reads the ledge of route link `route` (0 if missing)
    int fn_801AC14C();          // inferred: index of the move asked by the pad (-1: none)

    int m08;            // 0x08: inferred: moving
    int m0C;            // 0x0C: inferred: finished
    int m10;            // 0x10: inferred: direction index of the ledge (1-4)
    atn::Vector3 m14;   // 0x14: inferred: ledge start
    atn::Vector3 m20;   // 0x20: inferred: ledge end
    int m2C;            // 0x2C: inferred: route link
    int m30;            // 0x30: inferred: route link at the end of the ledge
    atn::Vector3 m34;   // 0x34: inferred: rotation facing the wall
    int m40;            // 0x40: inferred: the ledge is horizontal
    int m44[5];         // 0x44: inferred: motions
    float m58[5];       // 0x58: inferred: speeds of the motions
    float m6C;          // 0x6C: inferred: current speed
    int m70;            // 0x70: inferred: moving forward along the ledge
    float m74;          // 0x74: inferred: position on the ledge (0-1)
    int m78;            // 0x78
    int m7C;            // 0x7C: inferred: state (0 grab, 1 hang, 2 climb)
    unsigned int m80;   // 0x80: inferred: flags
    atn::Vector3 m84;   // 0x84: inferred: position when the state started
    float m90;          // 0x90: inferred: turn per frame
    int m94;            // 0x94: inferred: drop
    int m98;            // 0x98: inferred: other motion set
};

} // namespace chr

#endif
