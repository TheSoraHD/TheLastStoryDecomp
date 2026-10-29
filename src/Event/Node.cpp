#include "Event/Behavior.h"

namespace Event {

ActionOwner_unk* ActionArg_unk::fn_805381A0() {
    void* context = reinterpret_cast<void**>(this)[2];
    return reinterpret_cast<ActionOwner_unk**>(context)[2];
}

} // namespace Event

extern "C" void* fn_805381AC(void* self) {
    void* context = reinterpret_cast<void**>(self)[2];
    return reinterpret_cast<void**>(context)[2];
}

extern "C" void* fn_805381B8(void* self) {
    void* context = reinterpret_cast<void**>(self)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<void**>(context)[2];
}

namespace Event {

EventManager_unk* ActionArg_unk::fn_805381C8() {
    void* context = reinterpret_cast<void**>(this)[2];
    context = reinterpret_cast<void**>(context)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<EventManager_unk**>(context)[2];
}

} // namespace Event

extern "C" void* fn_805381DC(void* self) {
    void* context = reinterpret_cast<void**>(self)[2];
    context = reinterpret_cast<void**>(context)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<void**>(context)[2];
}

// NONMATCHING: 90.49% - the second and third component checks allocate different registers.
extern "C" void fn_805381F0(atn::Vector3* output, Event::ActionArg_unk* args, int index) {
    Event::ActionParam_unk* param;
    int valid = 0;
    if (index >= 0 && index < args->m30) {
        valid = 1;
    }
    param = valid != 0 ? args->m2C + index : 0;
    output->x = param->m04.f;

    int next = index + 1;
    valid = 0;
    if (next >= 0 && next < args->m30) {
        valid = 1;
    }
    param = valid != 0 ? args->m2C + next : 0;
    output->y = param->m04.f;

    next = index + 2;
    valid = 0;
    if (next >= 0 && next < args->m30) {
        valid = 1;
    }
    param = valid != 0 ? args->m2C + next : 0;
    output->z = param->m04.f;
}
