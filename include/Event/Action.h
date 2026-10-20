#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include "Event/Node.h"

namespace Event {

class Channel;

// inferred: action of a channel (0x40 bytes, vtable 0x80793AB0, defined in Event/Node)
class Action : public Node {
public:
    Action(Channel* channel, int index); // 0x80537708
    virtual ~Action();                   // 0x8053A40C
    virtual void vf0C();                 // 0x8053A3CC

    Channel* mChannel; // 0x08
    unsigned char m0C[0x40 - 0x0C];
};

} // namespace Event

#endif
