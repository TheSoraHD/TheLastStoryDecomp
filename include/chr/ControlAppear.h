#ifndef CHR_CONTROLAPPEAR_H
#define CHR_CONTROLAPPEAR_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor (implicit destructors at 0x80189534-0x801895F4)

// inferred: character appearing (motion 0x2B), moved from mStart by mOffset over the motion
class ControlAppear : public ControlBase {
public:
    ControlAppear(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& offset); // 0x801880D0
    virtual int vf0C();                     // 0x801881C4
    virtual boost::function<void()> vf10(); // 0x801883E8
    virtual void vf1C();                    // 0x80188BC4
    virtual void vf40();                    // 0x801882EC

    atn::Vector3 m08;     // 0x08: inferred: copy of Chara_unk::m600
    atn::Vector3 mStart;  // 0x14
    atn::Vector3 mOffset; // 0x20
};

class ControlAppearEnd : public ControlBase {
public:
    ControlAppearEnd(Chara_unk* chara, int motion, float blend); // 0x80188BD8
    virtual int vf0C();                     // 0x80188C70
    virtual boost::function<void()> vf10(); // 0x80188D3C
};

class ControlAppearSkelton : public ControlBase {
public:
    ControlAppearSkelton(Chara_unk* chara, const atn::Vector3& pos); // 0x8018906C
    virtual int vf0C();                     // 0x8018910C
    virtual boost::function<void()> vf10(); // 0x801891FC
    virtual void vf1C();                    // 0x801893F4

    int mState; // 0x08
    int mWait;  // 0x0C
};

class ControlAppearCoffin : public ControlBase {
public:
    ControlAppearCoffin(Chara_unk* chara, const atn::Vector3& pos); // 0x80189444
    virtual int vf0C();                                    // 0x801894DC

    int mWait; // 0x08
};

} // namespace chr

#endif
