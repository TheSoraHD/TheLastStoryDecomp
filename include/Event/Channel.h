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

    Section* mSection; // 0x08
    int m0C;
    int m10;           // inferred: 1 after construction
    std::vector<boost::shared_ptr<Action> > mActions; // 0x14
};

} // namespace Event

#endif
