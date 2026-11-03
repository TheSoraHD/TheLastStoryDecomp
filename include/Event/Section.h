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
    // inferred: index check of the channels (inlined in Event/Sequence.cpp; name invented)
    bool IsChannelIndex_unk(int i) const { return i >= 0 && i < (int)mChannels.size(); }
    // inferred: checked accessor (inlined in Event/EventLoader_unk.cpp; name invented)
    Channel* GetChannel_unk(unsigned int i) {
        if (IsChannelIndex_unk(i)) {
            return mChannels[i].get();
        }
        return 0;
    }

    Cut* mCut; // 0x08
    int m0C;
    int m10;
    int m14;   // inferred: 1 after construction
    std::vector<boost::shared_ptr<Channel> > mChannels; // 0x18
};

} // namespace Event

#endif
