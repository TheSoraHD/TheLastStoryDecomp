#include "Event/EventSpVector_unk.h"
#include "Event/Section.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace std {
// inferred: push_back of the channels (see Event/EventSpVector_unk.h)
template <>
inline void vector<boost::shared_ptr<Event::Channel> >::__push_unk(size_type n, const boost::shared_ptr<Event::Channel>& x) {
    size_type c = this->mCap;
    if (n < c) {
        this->__push_fast_unk(n, x);
        return;
    }
    __vec_sp_unk<boost::shared_ptr<Event::Channel> >::__push_slow_unk(*this, x);
}
// inferred: the elements are destroyed one inline level below (register order of the destroy loop)
template <>
inline void __vec_storage_unk<boost::shared_ptr<Event::Channel> >::__shrink_unk(unsigned int newSize) {
    __vec_sp_unk<boost::shared_ptr<Event::Channel> >::__shrink_loop_unk(*this, newSize);
}
} // namespace std

namespace Event {

Section::Section(Cut* cut, int index) : mCut(cut), m0C(0), m10(0), m14(1) {}

// NONMATCHING: 99.99% - push_back through Event/EventSpVector_unk.h (same source as Sequence::fn_80540A08,
// at 100%); only the free of the reallocation buffer differs: the original reads its storage through the
// `this` of the buffer (lwz r3, 0x0(r30)), ours from the stack (lwz r3, 0x30(r1)).
Channel* Section::fn_8053C788() {
    mChannels.push_back(boost::shared_ptr<Channel>(new ((MemoryTag)4, "", "", 0) Channel(this)));
    return mChannels.back().get();
}

// inferred: the reader of the section attributes was stripped by the linker; only its literals are
// left in @stringBase0 ("." and the basic_string errors of the out-of-line replace). Reconstructed
// minimally so that the pool keeps its layout.
void Section::fn_Stripped_unk(std::string& path) {
    path += ".";
}

// inferred: values of the "type" attribute
static const ParamEnum_unk sTypeNames_unk[4] = {
    {"director", "director"},
    {"camera", "camera"},
    {"character", "character"},
    {"dialog", "dialog"},
};

// inferred: values of the "id" attribute of camera sections
static const ParamEnum_unk sCameraIds_unk[4] = {
    {"main", "main"},
    {"sub 1", "sub 1"},
    {"sub 2", "sub 2"},
    {"sub 3", "sub 3"},
};

// inferred: values of the "id" attribute of dialog sections
static const ParamEnum_unk sDialogIds_unk[4] = {
    {"left", "left"},
    {"right", "right"},
    {"sub", "sub"},
    {"narration", "narration"},
};

// inferred: attributes of the four kinds of section (one table per "type" value)
static ParamDef_unk sDirectorParams_unk[4] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"type", fn_800DC880("type"), 0x21, 1, 4, sTypeNames_unk},
    {"id", fn_800DC880("id"), 0, 0},
    {"channel_num", fn_800DC880("channel_num"), 0, 0},
};

static ParamDef_unk sCameraParams_unk[4] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"type", fn_800DC880("type"), 0x21, 2, 4, sTypeNames_unk},
    {"id", fn_800DC880("id"), 0x1F, 0, 4, sCameraIds_unk},
    {"channel_num", fn_800DC880("channel_num"), 0, 0},
};

static ParamDef_unk sCharacterParams_unk[4] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"type", fn_800DC880("type"), 0x21, 3, 4, sTypeNames_unk},
    {"id", fn_800DC880("id"), 7, 0},
    {"channel_num", fn_800DC880("channel_num"), 0, 0},
};

static ParamDef_unk sDialogParams_unk[4] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"type", fn_800DC880("type"), 0x21, 4, 4, sTypeNames_unk},
    {"id", fn_800DC880("id"), 0x1F, 0, 4, sDialogIds_unk},
    {"channel_num", fn_800DC880("channel_num"), 0, 0},
};

} // namespace Event
