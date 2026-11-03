#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include "Event/Node.h"
#include "std/vector.h"
#include "math/Vec.h"

namespace Event {

class Channel;
struct ActionOwner_unk;
class Sequence;
typedef Sequence EventManager_unk; // inferred: the event manager of the actions is the sequence (Event/Sequence.h)

// inferred: action of a channel (0x40 bytes, vtable 0x80793AB0, defined in Event/Node)
// inferred: parameter of an action (8 bytes)
struct ActionParam_unk {
    // inferred: the value read for different uses (four weak copies with the same code in
    // Event/ActionActorPosition: 0x80551D28, 0x80551D6C, 0x80551D88, 0x80552024; not templates, which
    // would be emitted after the other weak copies); names invented
    // (defined in that file: a new inline body in a shared header renumbers the @N labels of every file
    // that includes it)
    inline int GetMode_unk() const;
    inline int GetActorId_unk() const;
    inline unsigned int GetHash_unk() const;
    inline int GetIndex_unk() const;

    int m00;
    union {
        int i;
        float f;
        short h[2]; // inferred: two values packed in one parameter (Event::Sequence::fn_8053F818)
    } m04;
};

// inferred: key of the curve of an action (time, value)
struct ActionKey_unk {
    float mTime;
    float mValue;
};

class Action : public Node {
public:
    Action(Channel* channel, int index); // 0x80537708
    virtual ~Action();                   // 0x8053A40C
    virtual void vf0C(int n, int value); // 0x8053A3CC: inferred: sets the value of parameter n

    void fn_805377F0(int arg, int index); // inferred: reads the action from the event data
    ActionOwner_unk* fn_805381A0();       // inferred: section of the channel
    EventManager_unk* fn_805381C8();      // inferred: sequence of the cut of the section
    atn::Vector3 fn_805382BC(int n);      // inferred: a position read from the parameters
    atn::Vector3 fn_805384CC(int n);      // inferred: a position read from the parameters (actor node)
    void fn_80538DC4(int n);
    void fn_80538FD4(int n);              // inferred: rotates the vector parameters n..n+2 by the event yaw
    atn::Vector3 fn_805381F0(int n);      // inferred: parameters n, n+1 and n+2 as a vector
    MtxPtr fn_805389B8(int n);            // inferred: node matrix of an actor (params n: actor id, n+1: node)
    atn::Vector3 fn_80538B18(int n);      // inferred: vector parameter rotated by the event yaw
    atn::Vector3 fn_80538C14(int n);      // inferred: same code as fn_805381F0
    void fn_80538CE0(int n, const atn::Vector3& v); // inferred: sets parameters n..n+2 to a vector
    float fn_8053A2E8(float t);           // inferred: value of the curve at t (linear)
    int fn_805391E4(unsigned int hash);   // inferred: index of the attribute `hash`, -1 if none
    ActionParam_unk* fn_805392BC(unsigned int hash); // inferred: attribute `hash`, 0 if none
    ActionParam_unk* fn_80539330(unsigned int hash); // inferred: same code as fn_805392BC
    float fn_805393A4(int frame);         // inferred: position of `frame` between m10 and m14
    void fn_805393F8(const ActionKey_unk& key); // inferred: adds a key to the curve (sorted by time)

    // inferred: parameter n, 0 when the action has fewer parameters (inlined by the behaviours,
    // which only pass constants)
    ActionParam_unk* GetParam_unk(int n) { return (int)mParams.size() > n ? &mParams[n] : 0; }
    // inferred: parameter n, 0 if out of range (inlined in vf0C and the Event/Node getters)
    ActionParam_unk* GetParamAt_unk(int n) {
        bool valid = false;
        if (n >= 0 && n < (int)mParams.size()) {
            valid = true;
        }
        return valid ? &mParams[n] : 0;
    }
    // inferred: sets parameter n to a float (type 3); inlined
    void SetFloat_unk(int n, float value) {
        ActionParam_unk* p = GetParamAt_unk(n);
        p->m00 = 3;
        p->m04.f = value;
    }
    // inferred: sets parameter n to a float of the given type; inlined
    void SetTyped_unk(int n, int type, float value) {
        ActionParam_unk* p = GetParamAt_unk(n);
        p->m00 = type;
        p->m04.f = value;
    }

    Channel* mChannel; // 0x08
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    std::vector<ActionKey_unk> mKeys;     // 0x20: inferred: curve (fn_8053A2E8)
    std::vector<ActionParam_unk> mParams; // 0x2C
    int m38; // inferred: cleared by ActionCameraChange when it starts
    int m3C;
};

} // namespace Event

#endif
