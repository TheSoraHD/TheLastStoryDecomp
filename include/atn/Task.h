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
    virtual ~Task();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
};

} // namespace atn

#endif
