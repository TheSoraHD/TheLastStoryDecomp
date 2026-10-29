#include "Event/Channel.h"
#include "Event/Action.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace Event {

Channel::Channel(Section* section) : mSection(section), m0C(0), m10(1) {}

// NONMATCHING: 40.5% - the boost::shared_ptr counts (lwarx/stwcx. of sp_counted_base_cw_ppc.hpp)
// now match; what differs is the inlined std::vector::push_back growth path of std/vector.h
// (the original keeps max_size() in a register and divides it at run time, and keeps an
// unreachable copy of the fast path after the length check).
Action* Channel::fn_8053A510(int index) {
    boost::shared_ptr<Action> action(new ((MemoryTag)4, "", "", 0) Action(this, index));
    mActions.push_back(action);
    return mActions[mActions.size() - 1].get();
}

// NONMATCHING: 98.9% - only register numbers: the original gives the vector destroy loop
// (count, element, &pn, pi) r25-r28 in the opposite order (tried std/vector.h variants: loop
// variable order, clear() instead of __shrink_unk, extra inline levels for the element destructor).
Channel::~Channel() {}

// inferred: the reader of the channel attributes was stripped by the linker; only its literals are
// left in @stringBase0 ("." and the basic_string errors of the out-of-line replace). Reconstructed
// minimally so that the pool keeps its layout.
void Channel::fn_Stripped_unk(std::string& path) {
    path += ".";
}

// inferred: attributes of a channel
// NONMATCHING (__sinit_\Channel_cpp): 93.6% - same as Event/Label: after the last hash call the
// original stores the default (0x3C) before the hash (0x34).
static ParamDef_unk sParamDefs_unk[] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"type", fn_800DC880("type"), 0x20, 0},
    {"action_num", fn_800DC880("action_num"), 0, 0},
};

} // namespace Event
