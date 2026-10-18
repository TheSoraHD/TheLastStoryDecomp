#ifndef GAME_CHARATASK_H
#define GAME_CHARATASK_H

#include "atn/Task.h"

namespace chr {
class ControlBase;
}

// inferred: hit/damage description passed to CharaTask::vf3C and vf40 (0x801513D0);
// 0x10 holds a vector (knockback?) and 0x68/0x90 values that FriendArganan clears.
struct HitInfo_unk;

// inferred from __vt__9CharaTask (0x8077C460). Base class of all characters
// (NpcTask, BattleCharaTask, ...). Virtual methods are named by vtable offset; most of their
// parameters are unknown.
class CharaTask : public atn::Task {
public:
    CharaTask(atn::Task* parent, int type, void* data); // 0x801354B4
    virtual ~CharaTask();                               // 0x80136138
    virtual int vf0C();                                 // 0x8017CB74
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
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
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual void vfAC();
    virtual void vfB0();
    virtual void vfB4();
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();

    int fn_8013655C();
    void fn_80178208(chr::ControlBase* control); // inferred: changes the current control
    void fn_80139560();
    void fn_80145334();
    void fn_80149A30();

    int mType;  // 0x48
    int m4C;    // 0x4C: same value as mType
    void* mData; // 0x50
    unsigned char m54[0x80 - 0x54];
    int mId;    // 0x80: inferred: unique id given by the character manager
    unsigned char m84[0x1424 - 0x84];
};

#endif
