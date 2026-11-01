#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include "Event/Node.h"
#include "std/vector.h"
#include "math/Vec.h"

namespace Event {

class Channel;

// inferred: action of a channel (0x40 bytes, vtable 0x80793AB0, defined in Event/Node)
// inferred: parameter of an action (8 bytes; also seen as ActionParam_unk in Event/Behavior.h)
struct ActionValue_unk {
    int m00;
    union {
        int i;
        float f;
    } m04;
};

class Action : public Node {
public:
    Action(Channel* channel, int index); // 0x80537708
    virtual ~Action();                   // 0x8053A40C
    virtual void vf0C(int n, int value); // 0x8053A3CC: inferred: sets the value of parameter n

    void fn_805377F0(int arg, int index); // inferred: reads the action from the event data
    atn::Vector3 fn_805381F0(int n);      // inferred: parameters n, n+1 and n+2 as a vector
    int fn_805391E4(unsigned int hash);   // inferred: index of the attribute `hash`, -1 if none
    ActionValue_unk* fn_805392BC(unsigned int hash); // inferred: attribute `hash`, 0 if none
    ActionValue_unk* fn_80539330(unsigned int hash); // inferred: same code as fn_805392BC
    float fn_805393A4(int frame);         // inferred: position of `frame` between m10 and m14

    // inferred: parameter n, 0 if out of range (inlined in vf0C)
    ActionValue_unk* GetParam_unk(int n) {
        bool valid = false;
        if (n >= 0 && n < (int)mParams.size()) {
            valid = true;
        }
        return valid ? &mParams[n] : 0;
    }

    Channel* mChannel; // 0x08
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    std::vector<int> m20;                 // inferred: element type unknown (POD)
    std::vector<ActionValue_unk> mParams; // 0x2C (m2C/m30 of ActionArg_unk)
    unsigned char m38[0x40 - 0x38];
};

} // namespace Event

#endif
