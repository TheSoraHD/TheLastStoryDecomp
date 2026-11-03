#ifndef EVENT_NODE_H
#define EVENT_NODE_H

#include "atn/String_unk.h"

// inferred: hash of an attribute name (CRC-32 style, 0x800DC880); the second argument is not read
extern "C" unsigned int fn_800DC880(const char* str, int arg = 0);

namespace Event {

// inferred: default value of an attribute (int or string); it has a constructor because the
// __sinit of every table stores it, even when it is 0
struct ParamValue_unk {
    // inferred: attributes without a default leave the value alone (no store in the __sinit of
    // Event/EventCommandDef_unk)
    ParamValue_unk() {}
    ParamValue_unk(int value) : mValue(value) {}
    ParamValue_unk(const char* str) : mValue((int)str) {}
    // inferred: float defaults keep their bits (game/PathData_unk)
    ParamValue_unk(float value) {
        union {
            float f;
            int i;
        } u;
        u.f = value;
        mValue = u.i;
    }
    int mValue;
};

// inferred: name of an enum value of an attribute (xml name, label; both the same string)
struct ParamEnum_unk {
    const char* mName;
    const char* mLabel;
};

// inferred: attribute of an event node read from the event file (tables of 0x18 bytes in
// Event/Channel, Cut, Label and Section; only their __sinit is left, the readers were stripped)
struct ParamDef_unk {
    const char* mName;       // 0x00
    unsigned int mHash;      // 0x04: fn_800DC880(mName)
    int mType;               // 0x08: inferred: 0 int, 0x1C string, 0x1F enum...
    ParamValue_unk mDefault; // 0x0C
    int mEnumCount;          // 0x10
    const ParamEnum_unk* mEnumNames; // 0x14
};

// inferred: definition of an event command (0x28 bytes, table 0x80775208 in Event/EventCommandDef_unk)
struct CommandDef_unk {
    int mId;                       // 0x00
    const char* mLabel;            // 0x04: inferred: name shown by the tools
    const char* mName;             // 0x08: xml name
    unsigned int mHash;            // 0x0C: fn_800DC880(mName, 789456)
    int m10;                       // 0x10: inferred: category (1 effect/audio, 2 camera...)
    int mParamCount;               // 0x14
    ParamDef_unk* mParams;         // 0x18
    int mVariantParam;             // 0x1C: inferred: attribute that selects the variant, -1 none
    int mVariantCount;             // 0x20
    ParamDef_unk** mVariants;      // 0x24: inferred: attributes of each variant
};


// inferred: base of the event tree nodes (vtable 0x80793AD8)
class Node {
public:
    Node() {}
    virtual ~Node(); // 0x80537780

    // inferred: inlined by the readers of Event/EventParser_unk.cpp; name invented
    void SetName_unk(const char* name) { mName = name; }

    String_unk mName; // 0x04: inferred: freed with delete[] by ~Node (0x80537780)
};

// inferred: declared in the class and defined inline afterwards: Label's destructor (0x8053B7B4,
// a weak copy in Event/Cut) inlines it, but the RTTI of Node is only emitted by Event/Node
inline Node::~Node() {}

} // namespace Event

// inferred: definition of the command `id` (Event/EventCommandDef_unk)
extern "C" Event::CommandDef_unk* fn_800081C0(int id);

#endif
