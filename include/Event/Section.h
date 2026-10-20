#ifndef EVENT_SECTION_H
#define EVENT_SECTION_H

#include "Event/Channel.h"
#include "std/string.h"

namespace Event {

class Cut;

// inferred: section of a cut (0x24 bytes, vtable 0x80794008, created by Event::Cut)
class Section : public Node {
public:
    Section(Cut* cut, int index); // 0x8053C750 (the index is not stored)
    Channel* fn_8053C788();       // inferred: appends a new channel and returns it
    void fn_Stripped_unk(std::string& path); // inferred: stripped by the linker (see Section.cpp)

    Cut* mCut; // 0x08
    int m0C;
    int m10;
    int m14;   // inferred: 1 after construction
    std::vector<boost::shared_ptr<Channel> > mChannels; // 0x18
};

} // namespace Event

#endif
