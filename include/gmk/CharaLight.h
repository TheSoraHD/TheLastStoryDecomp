#ifndef GMK_CHARALIGHT_H
#define GMK_CHARALIGHT_H

#include "atn/CharaLightData_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: light that follows a character and changes size with the state. The name is in the
// DOL (__vt__Q23gmk10CharaLight).
class CharaLight : public GimmickCommon {
public:
    // inferred: parameters of states 2-6 ("PARAM")
    struct Param_unk {
        Param_unk() : m00(0.0f), mSize(0.0f, 0.0f) {}
        float m00;
        atn::Vector2_unk mSize;
    };

    CharaLight(atn::Task* parent, GimmickDesc_unk* desc); // 0x80422684
    static GimmickCommon* fn_80422610(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~CharaLight();
    virtual int vf0C();
    virtual void vf10(); // 0x80422BF4: not decompiled yet
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);


    GimmickDesc_unk* mDesc;    // 0xF4
    CharaLightData_unk mLight; // 0xF8
    void* mTarget;             // 0x148: inferred: character followed by the light
    int m14C;
    float m150;
    Param_unk mParams[5];      // 0x154
    atn::Vector2_unk mStartSize; // 0x190
    atn::Vector2_unk mCurSize;   // 0x198
};

} // namespace gmk

#endif
