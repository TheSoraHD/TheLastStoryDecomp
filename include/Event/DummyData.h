#ifndef EVENT_DUMMYDATA_H
#define EVENT_DUMMYDATA_H

#include "Event/CommonData.h"
#include "math/Vec.h"

namespace Event {

// inferred: data with a matrix (0x48 bytes, the list at 0x104 of the sequence, "DUID")
class DummyData : public CommonData {
public:
    DummyData(Sequence* owner, const char* name, int id); // inferred: inlined by "DUID" (Event/EventParser_unk.cpp)
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    atn::Matrix3x4 m18; // inferred: identity when created (pose of a dummy, read by Event/Puppeteer_CharaTask.cpp)
};

} // namespace Event

#endif
