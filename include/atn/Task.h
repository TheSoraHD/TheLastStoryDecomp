#ifndef ATN_TASK_H
#define ATN_TASK_H

namespace atn {

// inferred from the vtables at 0x807796F0 (TaskBase) and 0x807796D0 (Task).
// Virtual methods are named by vtable offset until their real names are known.
class TaskBase {
public:
    virtual ~TaskBase();
    virtual int vf0C() = 0;
};

class Task : public TaskBase {
public:
    Task(Task* parent); // 0x800D1D3C
    virtual ~Task();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(Task* child); // inferred: notification that `child` was removed (see SequenceControl)

    void fn_800D3F28(Task* child); // inferred: attaches `child` (null detaches)
    int fn_800D3FA4(); // inferred: non-virtual, returns nonzero on success (called by many vf0C)
    void fn_800D246C(int arg); // inferred
    // inferred: flag test through an accessor: GimmickCommon::vf30 (0x803EDF40) loads the word
    // twice (here and for the |=), which a direct read would not do. Name invented.
    bool IsFlag_unk(unsigned int flag) const { return (mFlags & flag) == flag; }

    // inferred: sizeof(atn::Task) == 0x48 (FriendManager adds no members and is allocated with 0x48)
    unsigned char m04[0x20 - 0x04];
    void* m20; // inferred: compared with the argument of GimmickManager::fn_803EE374
    Task* m24; // inferred: parent of the shared d2anime tasks (D2A_TaskInstanceBase)
    unsigned char m28[0x38 - 0x28];
    unsigned int mFlags; // 0x38
    unsigned char m3C[0x48 - 0x3C];
};

} // namespace atn

#endif
