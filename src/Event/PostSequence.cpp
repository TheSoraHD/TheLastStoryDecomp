// inferred: Event::PostSequence and Event::SequenceRoot share one TU (their RTTI names are
// packed together in .rodata). SequenceRoot is declared first: vtables are emitted in reverse
// declaration order.
#include "Event/SequenceRoot.h"
#include "Event/PostSequence.h"

namespace Event {

int SequenceRoot::vf0C() { return 1; }

void SequenceRoot::vf10() {}

void SequenceRoot::vf14() {}

SequenceRoot::~SequenceRoot() {
    sInstance = 0;
}

int PostSequence::vf0C() { return 1; }

void PostSequence::vf14() {}

PostSequence::~PostSequence() {
    sInstance = 0;
}

} // namespace Event
