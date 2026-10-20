#ifndef EVENT_DUMMYDATA_H
#define EVENT_DUMMYDATA_H

#include "Event/CommonData.h"

namespace Event {

class DummyData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
};

} // namespace Event

#endif
