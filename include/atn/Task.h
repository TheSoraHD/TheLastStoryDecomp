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
    virtual void vf18();

    // inferred: sizeof(atn::Task) == 0x48 (FriendManager adds no members and is allocated with 0x48)
    unsigned char m04[0x48 - 0x04];
};

} // namespace atn

#endif
