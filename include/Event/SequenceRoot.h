#ifndef EVENT_SEQUENCEROOT_H
#define EVENT_SEQUENCEROOT_H

#include "atn/Task.h"
#include "atn/Memory.h"

namespace Event {

// inferred: singleton task; the destructor clears the static instance pointer.
class SequenceRoot : public atn::Task {
public:
    SequenceRoot(atn::Task* parent) : atn::Task(parent) {}
    // inferred: creates the single instance (inlined in the constructor of Event::Manager; name invented)
    static void Create_unk(atn::Task* parent) {
        if (sInstance == 0) {
            sInstance = new ((MemoryTag)3, "", "", 0) SequenceRoot(parent);
        }
    }
    virtual ~SequenceRoot();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static SequenceRoot* sInstance; // inferred name (0x8087F544)
};

} // namespace Event

#endif
