#ifndef CHR_CONTROLPUSHEDDOWN_H
#define CHR_CONTROLPUSHEDDOWN_H

#include "chr/ControlBase.h"

namespace chr {

// Controls of chr/ControlPushedDown.cpp created by CharaTask::fn_8013CB68 when two characters bump
// into each other (class names from the vtables). The classes declare no destructor (implicit
// destructors 0x801C6ED0, 0x801C6F10 and 0x801C6F50).

// inferred: knocked back by another character (control id 0x44)
class ControlPushedDown : public ControlBase {
public:
    ControlPushedDown(Chara_unk* chara, const atn::Vector3& dir, int a, int b, int c); // 0x801C48B4
    virtual int vf0C(); // 0x801C4F54
    // inferred: nonzero once the character can be pushed again (m2C clear and frame past 20)
    int fn_801C59D4();

    atn::Vector3 m08; // inferred: rotation kept while lying down
    atn::Vector3 m14; // inferred: position before the push
    atn::Vector3 m20; // inferred: rotation before the push
    int m2C;          // inferred: walk back to m14 when finished
    int m30;          // inferred: may end early (frame 30)
    int m34;          // inferred: plays the voices
    int m38;          // inferred: state (0 fall, 1 turn to the player, 2 get up, 3 walk back)
};

// inferred: steps aside to let another character pass (control id 0x45)
class ControlPushedAvoid : public ControlBase {
public:
    ControlPushedAvoid(Chara_unk* chara, const atn::Vector3& dir, Chara_unk* other); // 0x801C61EC
    virtual int vf0C();                     // 0x801C6464
    virtual boost::function<void()> vf10(); // 0x801C6CDC
    virtual void vf1C();                    // 0x801C6CC8

    atn::Vector3 m08; // inferred: direction of the side step
    atn::Vector3 m14; // inferred: position to come back to
    atn::Vector3 m20; // inferred: rotation before the step
    int m2C;          // inferred: state (0 step, 1 wait for the other one, 2 come back)
    float m30;        // inferred: time since the start
    Chara_unk* m34;   // inferred: the other character
    int m38;          // inferred: was following (control id 0x68): chr::ControlChaseFollow afterwards
};

// inferred: created by CharaTask::fn_8016BBCC (motion 0x52) (control id 0x61)
class ControlOttotto : public ControlBase {
public:
    ControlOttotto(Chara_unk* chara, const atn::Vector3* dir, int time, int motion); // 0x801C5A04
    virtual int vf0C(); // 0x801C5C50

    atn::Vector3 m08; // inferred: direction of the stagger
    int m14;          // inferred: frames left
};

} // namespace chr

#endif
