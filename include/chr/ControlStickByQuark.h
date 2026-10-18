#ifndef CHR_CONTROLSTICKBYQUARK_H
#define CHR_CONTROLSTICKBYQUARK_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlStickByQuark.cpp created by CharaTask (0x8015BAF0-0x8015D5B0).
// Only the constructors are declared; sizes from the operator new calls, argument types from the
// constructors (ints stored as words, vectors read through pointers).

class ControlThrownAway : public ControlBase {
public:
    ControlThrownAway(Chara_unk* chara, int a, int b); // 0x80199348
    virtual int vf0C();
    unsigned char m08[0x10 - 0x08];
};

class ControlThrownAvoid : public ControlBase {
public:
    ControlThrownAvoid(Chara_unk* chara, int a, const atn::Vector3& dir); // 0x80199788
    virtual int vf0C();
    unsigned char m08[0x18 - 0x08];
};

class ControlThreadCatch : public ControlBase {
public:
    ControlThreadCatch(Chara_unk* chara, int a, int b); // 0x8019B29C
    virtual int vf0C();
    unsigned char m08[0x28 - 0x08];
};

class ControlThreadDead : public ControlBase {
public:
    ControlThreadDead(Chara_unk* chara, int a); // 0x8019BD48
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlThreadRelease : public ControlBase {
public:
    ControlThreadRelease(Chara_unk* chara); // 0x8019BE9C
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlAbsorbedHP : public ControlBase {
public:
    ControlAbsorbedHP(Chara_unk* chara, int a, int b); // 0x8019DA50
    virtual int vf0C();
    unsigned char m08[0x24 - 0x08];
};

class ControlCliffhangZanGurg : public ControlBase {
public:
    ControlCliffhangZanGurg(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b, atn::Vector3 c); // 0x8019E4F4
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlEndCliffhangZanGurg : public ControlBase {
public:
    ControlEndCliffhangZanGurg(Chara_unk* chara, atn::Vector3 a); // 0x8019EB7C
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

class ControlStickZanGurgStand : public ControlBase {
public:
    ControlStickZanGurgStand(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b); // 0x8019F0A8
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

class ControlStickZanGurgCliff : public ControlBase {
public:
    ControlStickZanGurgCliff(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b); // 0x8019F8E8
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

} // namespace chr

#endif
