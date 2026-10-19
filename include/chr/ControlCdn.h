#ifndef CHR_CONTROLCDN_H
#define CHR_CONTROLCDN_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: status-condition controls of the unit that starts at 0x8018EEC4 (ControlCdnStone is
// declared in chr/ControlStickByQuark.h), created by CharaTask (0x8016676C-0x80168B1C).
// Only the constructors are declared; sizes from the operator new calls, argument types from the
// constructors (ints stored as words, vectors read through pointers).

class ControlCdnParalyzed : public ControlBase {
public:
    ControlCdnParalyzed(Chara_unk* chara); // 0x8018F970
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
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
    int m08;
    int m0C; // inferred: nonzero selects control 0x35 instead of 0x34
};

class ControlCdnSleep : public ControlBase {
public:
    ControlCdnSleep(Chara_unk* chara); // 0x801908F0
    virtual int vf0C();
    unsigned char m08[0x10 - 0x08];
};

class ControlCdnRestoreSleep : public ControlBase {
public:
    ControlCdnRestoreSleep(Chara_unk* chara); // 0x80190E80
    virtual int vf0C();
};

class ControlCdnPiyo : public ControlBase {
public:
    ControlCdnPiyo(Chara_unk* chara, const atn::Vector3& dir, int time, int flag); // 0x80191170
    virtual int vf0C();
    unsigned char m08[0x2C - 0x08];
};

class ControlCdnRestorePiyo : public ControlBase {
public:
    ControlCdnRestorePiyo(Chara_unk* chara); // 0x80191A44
    virtual int vf0C();
};

class ControlCdnPsycho : public ControlBase {
public:
    ControlCdnPsycho(Chara_unk* chara, int flag); // 0x80191D30
    virtual int vf0C();
    unsigned char m08[0x14 - 0x08];
};

class ControlCdnRestorePsycho : public ControlBase {
public:
    ControlCdnRestorePsycho(Chara_unk* chara); // 0x80192130
    virtual int vf0C();
};

} // namespace chr

#endif
