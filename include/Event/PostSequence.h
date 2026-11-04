#ifndef EVENT_POSTSEQUENCE_H
#define EVENT_POSTSEQUENCE_H

#include "atn/Task.h"
#include "atn/Memory.h"

namespace Event {

// inferred: singleton task; the destructor clears the static instance pointer.
class PostSequence : public atn::Task {
public:
    PostSequence(atn::Task* parent) : atn::Task(parent) {}
    // inferred: creates the single instance (inlined in the constructor of Event::Manager; name invented)
    static void Create_unk(atn::Task* parent) {
        if (sInstance == 0) {
            sInstance = new ((MemoryTag)3, "", "", 0) PostSequence(parent);
        }
    }
    virtual ~PostSequence();
    virtual int vf0C();
    virtual void vf10(); // defined in another TU (0x80487C3C)
    virtual void vf14();

    static PostSequence* sInstance; // inferred name (0x8087F548)
};

} // namespace Event

#endif
