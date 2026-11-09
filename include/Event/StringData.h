#ifndef EVENT_STRINGDATA_H
#define EVENT_STRINGDATA_H

#include "Event/CommonData.h"

namespace Event {

class StringData : public CommonData {
public:
    StringData(Sequence* owner, const char* name, int id); // inferred: inlined by "STID" (Event/EventParser_unk.cpp)
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
};

} // namespace Event

#endif
