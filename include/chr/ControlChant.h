#ifndef CHR_CONTROLCHANT_H
#define CHR_CONTROLCHANT_H

#include "chr/ControlMoveCursor.h"
#include "chr/ControlDropDown.h"
#include "atn/SoundRef.h"

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

namespace chr {

// Controls of chr/ControlChant.cpp (0x801BC5B8-0x801C041C), created by CharaTask::fn_8015F568 and
// fn_801603CC. The classes are declared in the order of their implicit destructors.

// inferred: chant of a magic (control id 0x1D): the cursor is moved to the target while the
// chant time runs; vf10 casts the magic (ControlMagic) or ends (ControlMagicEnd)
class ControlChant : public ControlMoveCursor {
public:
    ControlChant(Chara_unk* chara, CharaTask* target, const atn::Vector3& pos, int time, float range); // 0x801BC5B8
    virtual ~ControlChant();
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf5C() { fn_800CB5C8(&m30, 5, 0); }

    float m28;          // inferred: range of the magic (HitInfo_unk::m58)
    float m2C;          // inferred: chant time elapsed
    atn::SoundRef m30;  // inferred: chant sound
    CharaTask* m34;     // inferred: target (CharaTask::fn_80160170)
    int m38;            // inferred: chant time
    HitInfo_unk* m3C;   // inferred: magic being chanted
    float m40;          // inferred: speed rate passed to fn_8010CB2C
    bool m44;           // inferred: the magic is cast at the end
    bool m45;           // inferred: the chant time is over (enemies)
    bool m46;           // inferred: the second chant effect was started
    bool m47;           // inferred: passed to CharaTask::fn_801603CC
    float m48;          // inferred: power of the magic (arena mode)
};

// inferred: casting of a magic (control id 0x1E)
class ControlMagic : public ControlBase {
public:
    ControlMagic(Chara_unk* chara, CharaTask* target, float f, const atn::Vector3& pos, int b); // 0x801BE10C
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual atn::Vector3 vf18();
    virtual int vf28();
    virtual int vf44() {
        int result = 0;
        if (m1D && m18 < 2.0f) {
            result = 1;
        }
        return result;
    }
    virtual int vf48() { return 0x41; }
    // NONMATCHING: 60% - the original stores x/y (psq_st) before z (as ControlChargeAttackBase::vf18).
    virtual atn::Vector3 vf4C() { return m08; }
    virtual int vf50();

    atn::Vector3 m08; // inferred: target position
    CharaTask* m14;   // inferred: target
    float m18;        // inferred: time since the magic was cast
    bool m1C;         // inferred: vf10 ends with ControlMagicEnd
    bool m1D;         // inferred: the magic was cast
    bool m1E;         // inferred: aimed at m08 (else at the target)
    bool m1F;         // inferred: special cast motion (0x17A/0x17B)
    float m20;        // inferred: power of the magic
};

// inferred: end of a magic (control id 0x1F); falls like ControlDropDown
class ControlMagicEnd : public ControlDropDown {
public:
    ControlMagicEnd(Chara_unk* chara); // 0x801C02F4
};

} // namespace chr

#endif
