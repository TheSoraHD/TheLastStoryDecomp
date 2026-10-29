#ifndef CHR_CONTROLCHASECAUGHT_H
#define CHR_CONTROLCHASECAUGHT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlChaseCaught.cpp (chase mode: being caught, held and released;
// catching, holding and releasing; following the leader). Created by CharaTask (0x801598B4-0x8015A184)
// and by each other's vf10. Class names from the vtables, sizes from the operator new calls.

// inferred: caught by m08 (control id 0x64)
class ControlChaseCaught : public ControlBase {
public:
    ControlChaseCaught(Chara_unk* chara, CharaTask* catcher, int presses, int time); // 0x801A77BC
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    CharaTask* m08; // inferred: the character that catches
    int m0C;        // inferred: presses of button 0x22 needed to escape (player)
    int m10;        // inferred: time held
};

// inferred: held by m08 (control id 0x65)
class ControlChaseHeld : public ControlBase {
public:
    ControlChaseHeld(Chara_unk* chara, CharaTask* catcher, int presses, int time); // 0x801A7DC8
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    CharaTask* m08;
    int m0C;
    int m10;
    int m14; // inferred: presses so far
    int m18;
    int m1C;
};

// inferred: released (control id 0x66)
class ControlChaseRelease : public ControlBase {
public:
    ControlChaseRelease(Chara_unk* chara);
    virtual int vf0C();
    virtual boost::function<void()> vf10();
};

// inferred: catches m08 (control id 0x69)
class ControlChaseCatch : public ControlBase {
public:
    ControlChaseCatch(Chara_unk* chara, CharaTask* target); // 0x801A9A94
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    CharaTask* m08;
};

// inferred: holds m08 while it is caught or held
class ControlChaseHold : public ControlBase {
public:
    ControlChaseHold(Chara_unk* chara, CharaTask* target);
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    CharaTask* m08;
};

// inferred: releases m08
class ControlChaseReleased : public ControlBase {
public:
    ControlChaseReleased(Chara_unk* chara, CharaTask* target);
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    CharaTask* m08;
};

// inferred: moves beside the leader (control id 0x67)
class ControlChaseAssist : public ControlBase {
public:
    ControlChaseAssist(Chara_unk* chara); // 0x801A880C
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    bool m08; // inferred: arrived
    int m0C;  // inferred: frames
};

// inferred: follows the leader (control id 0x68)
class ControlChaseFollow : public ControlBase {
public:
    ControlChaseFollow(Chara_unk* chara); // 0x801A90BC
    virtual int vf0C();
    virtual int vf34() { return 1; }
    int m08;
};

} // namespace chr

#endif
