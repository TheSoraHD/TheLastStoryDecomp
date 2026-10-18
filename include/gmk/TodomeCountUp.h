#ifndef GMK_TODOMECOUNTUP_H
#define GMK_TODOMECOUNTUP_H

#include "atn/LH_Binary.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: counts finishing blows ("todome") on a character and plays a sound per hit; the
// sound names come from a CSV picked by the kind of character. The name is in the DOL
// (__vt__Q23gmk13TodomeCountUp).
class TodomeCountUp : public GimmickCommon {
public:
    struct Name_unk {
        char mName[0x40];
    };

    TodomeCountUp(atn::Task* parent, GimmickDesc_unk* desc); // 0x8042F128
    static GimmickCommon* fn_8042F0B4(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~TodomeCountUp() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf24();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);

    GimmickDesc_unk* mDesc; // 0xF4
    void* mTarget;          // 0xF8: inferred: character whose finishing blows are counted
    int mCharaId;           // 0xFC
    int mCount;             // 0x100
    int m104;
    atn::LH_Binary mLoader; // 0x108
    int mNumNames;          // 0x110
    Name_unk mNames[128];   // 0x114
    int m2114;              // inferred: unused (the object is 0x2118 bytes)
};

} // namespace gmk

#endif
