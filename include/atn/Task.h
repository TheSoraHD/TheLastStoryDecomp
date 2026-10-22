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

    // 64-bit values kept as two words: a long long member would align the class to 8 and move members
    // of the classes that embed a Task (CharaTask). Access them with TASK_U64.
    unsigned long m04;     // padding before the 64-bit values
    unsigned long mTag[2]; // 0x08: "XTASKTAG" (inferred: checked by IsValid)
    unsigned long mId[2];  // 0x10: unique id (0 once destroyed)
};

#define TASK_U64(x) (*(unsigned long long*)(x))

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
    unsigned long m30[2]; // id of m28 when it was set (TASK_U64)
    unsigned int mFlags; // 0x38: 1 children only, 2 ready (vf0C done), 4 stopped, 0x10, 0x20 vf10 done,
                         // 0x40 destroyed, 0xDEAD0000 killed
    int m3C; // inferred: number of tasks in the tree (kept in the root)
    int m40; // frames to wait before deleting a killed task
    void (*m44)(Task* task); // inferred: deleter (null: delete)
};

} // namespace atn

#endif
