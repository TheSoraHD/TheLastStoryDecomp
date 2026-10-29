#ifndef EVENT_COMMONDATA_H
#define EVENT_COMMONDATA_H

#include "std/string.h"

namespace Event {

struct DataOwner_unk;

// inferred: base class of about 12 Event::*Data classes, which override these 3 virtuals
// (named by vtable offset). Its typeinfo is only emitted in the Event::Actor TU, so it must
// have a key function defined there. Its vtable (0x80775A48, four empty slots) is stored by the
// inline constructor (Event::Path, Event/Path.cpp), before the owner, the name and m14: they are members
// of this class, not of the derived ones.
class CommonData {
public:
    CommonData();
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    DataOwner_unk* m04; // inferred: owner (event scene task)
    std::string m08;    // inferred: file name
    int m14;
};


} // namespace Event

#endif
