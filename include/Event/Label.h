#ifndef EVENT_LABEL_H
#define EVENT_LABEL_H

#include "Event/Node.h"
#include "std/string.h"

namespace Event {

class Cut;

// inferred: label of a cut (0x14 bytes, created by Event::Cut 0x8053B030)
class Label : public Node {
public:
    Label(Cut* cut, int index); // 0x8053C6A4
    void fn_Stripped_unk(std::string& path); // inferred: stripped by the linker (see Label.cpp)

    Cut* mCut;   // 0x08
    int mIndex;  // 0x0C
    int mFrame;  // 0x10: inferred from the "frame" attribute
};

} // namespace Event

#endif
