#ifndef GMK_OCEAN_H
#define GMK_OCEAN_H

#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: sea surface: two joints ("joint1", "joint2") of the model move up and down with two
// sine waves per state ("WAVE state index amplitude period phase"); the characters follow its
// height (fn_804188A0). The name is in the DOL (__vt__Q23gmk5Ocean, 0x500 bytes).
class Ocean : public GimmickCommon {
public:
    // inferred: one sine wave (names invented)
    struct Wave_unk {
        Wave_unk() : mAmp(0.0f), mSpeed(1.0f), mPhase(0.0f) {}
        float mAmp;
        float mSpeed;
        float mPhase;
    };

    Ocean(atn::Task* parent, int id, int arg) : GimmickCommon(parent, id, arg), mModel(8, 32), mTime(0.0f) {
        m54 = 0;
    }
    static GimmickCommon* fn_80418198(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~Ocean();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    void fn_804186D8(); // inferred: moves the joints

    Model_unk mModel;       // 0xF4
    Wave_unk mWaves[2][2];  // 0x4C4: [state][joint]
    unsigned char m4F4[0x4FC - 0x4F4];
    float mTime;            // 0x4FC
};

} // namespace gmk

#endif
