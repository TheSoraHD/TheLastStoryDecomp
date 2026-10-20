#include "Event/Channel.h"
#include "Event/Action.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace Event {

Channel::Channel(Section* section) : mSection(section), m0C(0), m10(1) {}

// NONMATCHING: the boost::shared_ptr reference counts are lwarx/stwcx. loops in the original
// (asm functions of boost), plain increments here.
Action* Channel::fn_8053A510(int index) {
    boost::shared_ptr<Action> action(new (4, "", "", 0) Action(this, index));
    mActions.push_back(action);
    return mActions[mActions.size() - 1].get();
}

// NONMATCHING: same atomic decrements when the actions are released.
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
