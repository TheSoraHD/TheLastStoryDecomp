#ifndef CHR_CONTROLGMKCOFFINOPENING_H
#define CHR_CONTROLGMKCOFFINOPENING_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor (implicit destructors at 0x801B515C-0x801B525C)

// inferred: character opening a coffin gimmick (motions 0x231, 0x232)
class ControlGmkCoffinOpening : public ControlBase {
public:
    ControlGmkCoffinOpening(Chara_unk* chara, const atn::Vector3& pos, float rotY); // 0x801B409C
    virtual int vf0C();                                                    // 0x801B417C

    int mState; // 0x08
};

class ControlGmkCoffinOpen : public ControlBase {
public:
    ControlGmkCoffinOpen(Chara_unk* chara, const atn::Vector3& pos, int alt, float rotY); // 0x801B4238
    virtual int vf0C();                                                          // 0x801B4344
};

// inferred: character getting up from a coffin (motion 0x22F or 0x230)
class ControlGmkCoffinGetUp : public ControlBase {
public:
    ControlGmkCoffinGetUp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& rot); // 0x801B4394
    virtual int vf0C();                                                      // 0x801B44E8

    int mWait;  // 0x08
    int mState; // 0x0C
};

// inferred: caught by a bat trap (motion 0x236); mDamage is subtracted every frame
class ControlGmkTrapBat : public ControlBase {
public:
    ControlGmkTrapBat(Chara_unk* chara, float damage); // 0x801B46E8
    virtual int vf0C();                                // 0x801B4788
    virtual boost::function<void()> vf10();            // 0x801B47EC

    float mDamage; // 0x08
};

// inferred: gimmick object used by ControlGmkBreakArmor (class unknown)
struct GmkTarget_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70(int arg);

    unsigned char m04[0x6C - 0x04];
    atn::Vector3 m6C; // 0x6C: inferred: position
};

// inferred: breaking a gimmick's armor (motion 0xB)
class ControlGmkBreakArmor : public ControlBase {
public:
    ControlGmkBreakArmor(Chara_unk* chara, GmkTarget_unk* target, void* arg); // 0x801B4D34
    virtual int vf0C();                                                      // 0x801B507C

    GmkTarget_unk* mTarget; // 0x08
    void* m0C;              // 0x0C: passed to fn_80547708
};

} // namespace chr

#endif
