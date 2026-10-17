#ifndef EVENT_SEQUENCEROOT_H
#define EVENT_SEQUENCEROOT_H

#include "atn/Task.h"

namespace Event {

// inferred: singleton task; the destructor clears the static instance pointer.
class SequenceRoot : public atn::Task {
public:
    virtual ~SequenceRoot();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static SequenceRoot* sInstance; // inferred name (0x8087F544)
};

} // namespace Event

#endif
