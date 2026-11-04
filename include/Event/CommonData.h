#ifndef EVENT_COMMONDATA_H
#define EVENT_COMMONDATA_H

#include "std/string.h"

namespace Event {

class Sequence;

// inferred: base class of about 12 Event::*Data classes, which override these 3 virtuals
// (named by vtable offset). Its typeinfo is only emitted in the Event::Actor TU, so it must
// have a key function defined there. Its vtable (0x80775A48, four empty slots) is stored by the
// inline constructor (Event::Path, Event/Path.cpp), before the owner, the name and m14: they are members
// of this class, not of the derived ones.
class CommonData {
public:
    CommonData();
    // inferred: inlined in the constructor of Event::Actor (0x8000D528)
    CommonData(Sequence* owner, const char* name, int id) : m04(owner), m08(name), m14(id) {}
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
    // inferred: inlined by the readers of Event/EventParser_unk.cpp ("..NA" chunks); name invented
    void SetName_unk(const std::string& name) { m08 = name; }

    Sequence* m04; // inferred: owner (the sequence of the data)
    std::string m08;    // inferred: file name
    int m14;
};


} // namespace Event

#endif
