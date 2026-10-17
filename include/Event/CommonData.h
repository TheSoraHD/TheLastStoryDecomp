#ifndef EVENT_COMMONDATA_H
#define EVENT_COMMONDATA_H


namespace Event {

// inferred: base class of about 12 Event::*Data classes, which override these 3 virtuals
// (named by vtable offset). Its typeinfo is only emitted in the Event::Actor TU, so it must
// have a key function defined there; its own vtable is unused and dead-stripped.
class CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10();
};

} // namespace Event

#endif
