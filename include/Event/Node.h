#ifndef EVENT_NODE_H
#define EVENT_NODE_H

// inferred: hash of an attribute name (CRC-32 style, 0x800DC880); the second argument is not read
extern "C" unsigned int fn_800DC880(const char* str, int arg = 0);

namespace Event {

// inferred: default value of an attribute (int or string); it has a constructor because the
// __sinit of every table stores it, even when it is 0
struct ParamValue_unk {
    ParamValue_unk(int value) : mValue(value) {}
    ParamValue_unk(const char* str) : mValue((int)str) {}
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

// inferred: name buffer of a node (freed with delete[] by ~Node, 0x80537780)
struct NodeName_unk {
    NodeName_unk() : mBuf(0) {}
    ~NodeName_unk() {
        if (mBuf != 0) {
            delete[] mBuf;
            mBuf = 0;
        }
    }
    char* mBuf;
};

// inferred: base of the event tree nodes (vtable 0x80793AD8)
class Node {
public:
    Node() {}
    virtual ~Node(); // 0x80537780

    NodeName_unk mName; // 0x04
};

// inferred: declared in the class and defined inline afterwards: Label's destructor (0x8053B7B4,
// a weak copy in Event/Cut) inlines it, but the RTTI of Node is only emitted by Event/Node
inline Node::~Node() {}

} // namespace Event

#endif
