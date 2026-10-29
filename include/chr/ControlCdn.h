#ifndef CHR_CONTROLCDN_H
#define CHR_CONTROLCDN_H

#include "chr/ControlBase.h"

namespace chr {

// Status-condition controls of chr/ControlCdnStone.cpp (0x8018EEC4-0x80192908), created by
// CharaTask (0x8016676C-0x80168B1C). Each one switches to its "Restore" control when it ends
// (vf10). Sizes from the operator new calls, argument types from the constructors.

class ControlCdnStone : public ControlBase {
public:
    ControlCdnStone(Chara_unk* chara, int a); // 0x8018EEC4 (first function of this unit)
    virtual int vf0C();
    virtual boost::function<void()> vf10();

    atn::Vector3 m08; // inferred: shake offset applied to the character (mF88)
    atn::Vector3 m14; // inferred: shake direction; its y (always 0 in the direction) is the shake timer
    int m20;          // inferred: frames left (450)
    int m24;          // inferred: nonzero: the time does not run out
};

// inferred: from RTTI "chr::ControlCdnRestoreStone" (vtable 0x8077D... in this unit); created inline
// by ControlCdnStone::vf10
class ControlCdnRestoreStone : public ControlBase {
public:
    ControlCdnRestoreStone(Chara_unk* chara);
    virtual int vf0C();
};

class ControlCdnParalyzed : public ControlBase {
public:
    ControlCdnParalyzed(Chara_unk* chara); // 0x8018F970
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    int m08;
};

class ControlCdnRestoreParalyzed : public ControlBase {
public:
    ControlCdnRestoreParalyzed(Chara_unk* chara); // 0x8018FEBC
    virtual int vf0C();
};

class ControlCdnBetray : public ControlBase {
public:
    ControlCdnBetray(Chara_unk* chara, int a, int b); // 0x8018FF6C
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    int m08; // inferred: side the character switches to (CharaTask::mF14)
    int m0C; // inferred: nonzero selects control 0x35 instead of 0x34
};

class ControlCdnSleep : public ControlBase {
public:
    ControlCdnSleep(Chara_unk* chara); // 0x801908F0
    virtual ~ControlCdnSleep();
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    int m08;
    int m0C;
};

class ControlCdnRestoreSleep : public ControlBase {
public:
    ControlCdnRestoreSleep(Chara_unk* chara); // 0x80190E80
    virtual int vf0C();
    virtual boost::function<void()> vf10();
};

class ControlCdnPiyo : public ControlBase {
public:
    ControlCdnPiyo(Chara_unk* chara, const atn::Vector3& dir, int time, int flag); // 0x80191170
    virtual ~ControlCdnPiyo();
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    int m08;          // inferred: phase (0 = falling, 1 = dizzy)
    float m0C;        // inferred: time left
    int m10;          // inferred: nonzero for the long version (other motion, slower push)
    atn::Vector3 m14; // inferred: push direction
    atn::Vector3 m20; // inferred: rotation the character looks at
};

class ControlCdnRestorePiyo : public ControlBase {
public:
    ControlCdnRestorePiyo(Chara_unk* chara); // 0x80191A44
    virtual int vf0C();
    virtual boost::function<void()> vf10();
};

class ControlCdnPsycho : public ControlBase {
public:
    ControlCdnPsycho(Chara_unk* chara, int flag); // 0x80191D30
    virtual ~ControlCdnPsycho();
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    int m08;   // inferred: phase (the constructor starts it at 1)
    float m0C; // inferred: time left
    int m10;
};

class ControlCdnRestorePsycho : public ControlBase {
public:
    ControlCdnRestorePsycho(Chara_unk* chara); // 0x80192130
    virtual int vf0C();
    virtual boost::function<void()> vf10();
};

} // namespace chr

#endif
