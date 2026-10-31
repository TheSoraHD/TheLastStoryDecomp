#ifndef ATN_TASK_H
#define ATN_TASK_H

namespace atn {

// inferred: counters of the task system (atn/TaskBase.cpp)
struct TaskStats_unk {
    unsigned long long mCount;  // live tasks
    unsigned long long mNextId; // last id given to a TaskBase
};

// inferred from the vtables at 0x807796F0 (TaskBase) and 0x807796D0 (Task).
// Virtual methods are named by vtable offset until their real names are known.
class TaskBase {
public:
    TaskBase(int arg); // 0x800D5618 (the argument is not used; Task passes 0)
    virtual ~TaskBase();
    virtual int vf0C() = 0;

    bool IsValid() const; // 0x800D56C4: tag and id set (also false for a null this)

    // The 64-bit members align every task class to 8: the enemy factory (fn_805A5374) allocates
    // sizes rounded up to 8 (0x1568 for Em306GurgBeast, whose last member ends at 0x1564).
    unsigned long m04;          // padding before the 64-bit values
    unsigned long long mTag;    // 0x08: "XTASKTAG" (inferred: checked by IsValid)
    unsigned long long mId;     // 0x10: unique id (0 once destroyed)
};

#define TASK_U64(x) (x)

class Task : public TaskBase {
public:
    Task(Task* parent); // 0x800D1D3C
    virtual ~Task();
    virtual int vf0C();
    // inferred: inline and empty (the only copies are weak, first emitted in game/AreaEffConfig.cpp at
    // 0x8004312C and 0x80043128)
    virtual void vf10() {}
    virtual void vf14() {}
    virtual void vf18(Task* child); // inferred: notification that `child` was removed (see SequenceControl)

    void fn_800D3F28(Task* child); // inferred: attaches `child` (null detaches)
    int fn_800D3FA4(); // inferred: non-virtual, returns nonzero on success (called by many vf0C)
    void fn_800D246C(int arg); // inferred
    // inferred: flag test through an accessor: GimmickCommon::vf30 (0x803EDF40) loads the word
    // twice (here and for the |=), which a direct read would not do. Name invented.
    bool IsFlag_unk(unsigned int flag) const { return (mFlags & flag) == flag; }
    // name from the RSO (weak copy 0x80244CAC, first emitted by game/Em001Necromancer.cpp)
    void FreezeTask() { mFlags |= 4; }
    // name from the RSO (weak copy 0x803D6E2C, emitted by game/GameUI.cpp)
    void MeltTask() { mFlags &= ~4; }
    // inferred: weak copies in game/GameUI.cpp (0x803CFC58, 0x803D6E70); names invented
    bool IsFrozen_unk() const { return (mFlags & 4) != 0; }
    unsigned int GetFlag1_unk() const { return mFlags & 1; }

    void fn_800D1D94(Task* parent); // inferred: links the task into the tree of `parent` (ctor body)
    void fn_800D4558(); // inferred: sets flag 4 (hidden/stopped) in all the descendants
    void fn_800D4DB8(); // inferred: clears flag 4 in all the descendants

    // inferred: tree links (atn/Task.cpp)
    Task** GetNextLink_unk() { return &m1C; }
    void AddChild_unk(Task* child) {
        if (child != 0) {
            child->m20 = this;
            Task** link = &m18;
            while (*link != 0) {
                link = (*link)->GetNextLink_unk();
            }
            *link = child;
        }
    }
    void RemoveChild_unk(Task* child) {
        Task** link = &m18;
        if (child != 0) {
            for (Task* t = *link; t != 0; t = *link) {
                if (child == t) {
                    *link = t->m1C;
                    child->m20 = 0;
                    return;
                }
                link = &t->m1C;
            }
        } else {
            *link = 0;
        }
    }

    // inferred: sizeof(atn::Task) == 0x48 (FriendManager adds no members and is allocated with 0x48)
    Task* m18; // first child
    Task* m1C; // next sibling
    void* m20; // parent Task (inferred: compared with the argument of GimmickManager::fn_803EE374)
    Task* m24; // root of the tree
    Task* m28; // inferred: task notified through vf18 when this one is destroyed (set by fn_800D3F28)
    unsigned long m2C; // padding before m30
    unsigned long long m30; // id of m28 when it was set
    unsigned int mFlags; // 0x38: 1 children only, 2 ready (vf0C done), 4 stopped, 0x10, 0x20 vf10 done,
                         // 0x40 destroyed, 0xDEAD0000 killed
    int m3C; // inferred: number of tasks in the tree (kept in the root)
    int m40; // frames to wait before deleting a killed task
    void (*m44)(Task* task); // inferred: deleter (null: delete)
};

} // namespace atn

#endif
