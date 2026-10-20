// inferred: Event::StringData and Event::DummyData share one TU (DummyData's RTTI name follows
// StringData's with 4-byte alignment inside the same .rodata section). The TU continues after
// this code (fn_80543108, fn_805437E4... use the strings at 0x8075E148).
// inferred: DummyData is declared before StringData (vtables are emitted in reverse declaration order)
#include "Event/DummyData.h"
#include "Event/StringData.h"

namespace Event {

// inferred: returns 1 (li r3, 0x1)
int StringData::vf10() const { return 1; }

void StringData::vf0C() {}

void StringData::vf08() {}

// inferred: returns 1 (li r3, 0x1)
int DummyData::vf10() const { return 1; }

void DummyData::vf0C() {}

void DummyData::vf08() {}

} // namespace Event
