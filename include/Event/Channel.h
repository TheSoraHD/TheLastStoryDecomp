#ifndef EVENT_CHANNEL_H
#define EVENT_CHANNEL_H

#include "Event/Node.h"
#include "Event/SharedPtr_unk.h"
#include "std/vector.h"

namespace Event {

class Section;
class Action;

// inferred: channel of a section (0x20 bytes, vtable 0x80793BA0)
class Channel : public Node {
public:
    Channel(Section* section); // 0x8053A4DC
    virtual ~Channel();

    Section* mSection; // 0x08
    int m0C;
    int m10;           // inferred: 1 after construction
    std::vector<boost::shared_ptr<Action> > mActions; // 0x14
};

} // namespace Event

#endif
