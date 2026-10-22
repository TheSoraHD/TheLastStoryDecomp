#include "atn/Task.h"

// atn::TaskBase: base of atn::Task with a tag and a unique id, so that a stored pointer can be checked
// before use (Task::m28 keeps the id it had when it was set). Separate file from atn/Task.cpp: Task.cpp
// keeps every call to IsValid whose result is ignored (asserts), which MWCC removes when it can see the
// definition; the __sinit at the end belongs to this file.

namespace atn {

static unsigned long long sTaskTag = *(const unsigned long long*)"XTASKTAG";

} // namespace atn

// inferred: counters of the task system (one object: the constructor reaches mNextId from the address
// of mCount). Name unknown; Task.cpp uses mCount.
extern "C" {
atn::TaskStats_unk lbl_807C75D8;
}

namespace atn {

// NONMATCHING: 58.86% - orden de las cargas del contador de ids y de la etiqueta (planificador); probado:
// orden inverso, locales y post-incremento.
TaskBase::TaskBase(int arg) {
    TASK_U64(mId) = ++lbl_807C75D8.mNextId;
    TASK_U64(mTag) = sTaskTag;
}

TaskBase::~TaskBase() {
    TASK_U64(mId) = 0;
    TASK_U64(mTag) = 0;
}

bool TaskBase::IsValid() const {
    if (this != 0 && TASK_U64(mTag) == sTaskTag && TASK_U64(mId) != 0) {
        return true;
    }
    return false;
}

} // namespace atn
