#include "Event/EventSpVector_unk.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace std {
// inferred: push_back of the actions (see Event/EventSpVector_unk.h)
template <>
inline void vector<boost::shared_ptr<Event::Action> >::__push_unk(size_type n, const boost::shared_ptr<Event::Action>& x) {
    size_type c = this->mCap;
    if (n < c) {
        this->__push_fast_unk(n, x);
        return;
    }
    __vec_sp_unk<boost::shared_ptr<Event::Action> >::__push_slow_unk(*this, x);
}
// inferred: the elements are destroyed one inline level below (register order of the destroy loop)
template <>
inline void __vec_storage_unk<boost::shared_ptr<Event::Action> >::__shrink_unk(unsigned int newSize) {
    __vec_sp_unk<boost::shared_ptr<Event::Action> >::__shrink_loop_unk(*this, newSize);
}
} // namespace std

namespace Event {

Channel::Channel(Section* section) : mSection(section), m0C(0), m10(1) {}

// NONMATCHING: 99.99% - push_back through Event/EventSpVector_unk.h (same source as Sequence::fn_80540A08,
// at 100%); only the free of the reallocation buffer differs: the original reads its storage through the
// `this` of the buffer (lwz r3, 0x0(r30)), ours from the stack (lwz r3, 0x30(r1)).
Action* Channel::fn_8053A510(int index) {
    mActions.push_back(boost::shared_ptr<Action>(new ((MemoryTag)4, "", "", 0) Action(this, index)));
    return mActions.back().get();
}

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
