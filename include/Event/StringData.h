#ifndef EVENT_STRINGDATA_H
#define EVENT_STRINGDATA_H

#include "Event/CommonData.h"

namespace Event {

class StringData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
};

} // namespace Event

#endif
