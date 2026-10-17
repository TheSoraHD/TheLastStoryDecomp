#ifndef EVENT_POSTSEQUENCE_H
#define EVENT_POSTSEQUENCE_H

#include "atn/Task.h"

namespace Event {

// inferred: singleton task; the destructor clears the static instance pointer.
class PostSequence : public atn::Task {
public:
    virtual ~PostSequence();
    virtual int vf0C();
    virtual void vf10(); // defined in another TU (0x80487C3C)
    virtual void vf14();

    static PostSequence* sInstance; // inferred name (0x8087F548)
};

} // namespace Event

#endif
