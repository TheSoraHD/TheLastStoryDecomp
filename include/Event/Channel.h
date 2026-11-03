#ifndef EVENT_CHANNEL_H
#define EVENT_CHANNEL_H

#include "Event/Node.h"
#include <boost/shared_ptr.hpp>
#include "std/vector.h"
#include "std/string.h"

namespace Event {

class Section;
class Action;

// inferred: channel of a section (0x20 bytes, vtable 0x80793BA0)
class Channel : public Node {
public:
    Channel(Section* section); // 0x8053A4DC
    virtual ~Channel();        // 0x8053AD50
    Action* fn_8053A510(int index); // inferred: appends a new action and returns it
    void fn_Stripped_unk(std::string& path); // inferred: stripped by the linker (see Channel.cpp)
    // inferred: index check of the actions (inlined in Event/Sequence.cpp; name invented)
    bool IsActionIndex_unk(int i) const { return i >= 0 && i < (int)mActions.size(); }
    // inferred: checked accessor (inlined in Event/EventLoader_unk.cpp; name invented)
    Action* GetAction_unk(unsigned int i) {
        if (IsActionIndex_unk(i)) {
            return mActions[i].get();
        }
        return 0;
    }

    Section* mSection; // 0x08
    int m0C;
    int m10;           // inferred: 1 after construction
    std::vector<boost::shared_ptr<Action> > mActions; // 0x14
};

} // namespace Event

#endif
