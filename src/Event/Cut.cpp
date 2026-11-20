#include "Event/EventSpVector_unk.h"
#include "Event/Cut.h"
#include "Event/Sequence.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace std {
// inferred: push_back of the labels (see Event/EventSpVector_unk.h)
template <>
inline void vector<boost::shared_ptr<Event::Label> >::__push_unk(size_type n, const boost::shared_ptr<Event::Label>& x) {
    size_type c = this->mCap;
    if (n < c) {
        this->__push_fast_unk(n, x);
        return;
    }
    __vec_sp_unk<boost::shared_ptr<Event::Label> >::__push_slow_unk(*this, x);
}
// inferred: push_back of the sections (see Event/EventSpVector_unk.h)
template <>
inline void vector<boost::shared_ptr<Event::Section> >::__push_unk(size_type n, const boost::shared_ptr<Event::Section>& x) {
    size_type c = this->mCap;
    if (n < c) {
        this->__push_fast_unk(n, x);
        return;
    }
    __vec_sp_unk<boost::shared_ptr<Event::Section> >::__push_slow_unk(*this, x);
}
// inferred: the elements are destroyed one inline level below (register order of the destroy loop)
template <>
inline void __vec_storage_unk<boost::shared_ptr<Event::Label> >::__shrink_unk(unsigned int newSize) {
    __vec_sp_unk<boost::shared_ptr<Event::Label> >::__shrink_loop_unk(*this, newSize);
}
// inferred: the elements are destroyed one inline level below (register order of the destroy loop)
template <>
inline void __vec_storage_unk<boost::shared_ptr<Event::Section> >::__shrink_unk(unsigned int newSize) {
    __vec_sp_unk<boost::shared_ptr<Event::Section> >::__shrink_loop_unk(*this, newSize);
}
// inferred: the elements are destroyed one inline level below (register order of the destroy loop)
template <>
inline void __vec_storage_unk<boost::shared_ptr<Event::Channel> >::__shrink_unk(unsigned int newSize) {
    __vec_sp_unk<boost::shared_ptr<Event::Channel> >::__shrink_loop_unk(*this, newSize);
}
} // namespace std

namespace Event {

Cut::Cut(Sequence* sequence, int index)
    : mSequence(sequence), mSkip(0), mCinematic(1), m18(0), m1C(0), m20(0), m24(0), mNextCut(-1),
      m2C(0), mLightFlags() {
    mIndex = index;
}

int Cut::fn_8053B028() {
    return mNextCut;
}

// NONMATCHING: 99.7% - push_back through Event/EventSpVector_unk.h; only register numbers left: the
// shared_count (&pn, pi) registers of the two destroy loops of the reallocation buffer are swapped and the
// original frees the buffer storage through its `this` (lwz r3, 0x0(r30)).
Label* Cut::fn_8053B030(int index) {
    Sequence* sequence = mSequence;
    if (index == -1) {
        index = ++sequence->mLabelCount;
    } else if (index > sequence->mLabelCount) {
        sequence->mLabelCount = index;
    }
    mLabels.push_back(boost::shared_ptr<Label>(new ((MemoryTag)4, "", "", 0) Label(this, index)));
    return mLabels.back().get();
}

// NONMATCHING: 99.7% - push_back through Event/EventSpVector_unk.h; only register numbers left: the
// shared_count (&pn, pi) registers of the two destroy loops of the reallocation buffer are swapped and the
// original frees the buffer storage through its `this` (lwz r3, 0x0(r30)).
Section* Cut::fn_8053B828(int index) {
    mSections.push_back(boost::shared_ptr<Section>(new ((MemoryTag)4, "", "", 0) Section(this, index)));
    return mSections.back().get();
}

const char* Cut::fn_8053C124(int skip) {
    switch (skip) {
    case 0:
        return "none";
    case 1:
        return "next";
    case 2:
        return "all";
    case 3:
        return "fast";
    case 4:
        return "unknow";
    default:
        return "<error>";
    }
}

Cut::~Cut() {}

// inferred: the reader of the cut attributes was stripped by the linker; only the basic_string
// errors of its string copy are left in @stringBase0. Reconstructed minimally so that the pool
// keeps its layout.
void Cut::fn_Stripped_unk(std::string& path) {
    std::string copy(path);
    path = copy;
}

// inferred: values of the "skip" attribute
static const ParamEnum_unk sSkipNames_unk[4] = {
    {"none", "none"},
    {"next", "next"},
    {"all", "all"},
    {"fast", "fast"},
};

// inferred: values of the "in_effect" and "out_effect" attributes
static const ParamEnum_unk sEffectNames_unk[4] = {
    {"none", "none"},
    {"fade to black", "fade to black"},
    {"fade to white", "fade to white"},
    {"capture fade", "capture fade"},
};

// inferred: attributes of a cut in the event file
// NONMATCHING (__sinit_\Cut_cpp): 97.4% - same as Event/Label: the original stores the default of
// the last attribute before its hash.
static ParamDef_unk sParamDefs_unk[] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"id", fn_800DC880("id"), 0, 0},
    {"skip", fn_800DC880("skip"), 0x1F, 3, 4, sSkipNames_unk},
    {"cinematic", fn_800DC880("cinematic"), 2, 1},
    {"next_cut", fn_800DC880("next_cut"), 0x1A, -1},
    {"in_effect", fn_800DC880("in_effect"), 0x1F, 0, 4, sEffectNames_unk},
    {"in_duration", fn_800DC880("in_duration"), 0, 0},
    {"out_effect", fn_800DC880("out_effect"), 0x1F, 0, 4, sEffectNames_unk},
    {"out_duration", fn_800DC880("out_duration"), 0, 0},
    {"label_num", fn_800DC880("label_num"), 0, 0},
    {"section_num", fn_800DC880("section_num"), 0, 0},
    {"length", fn_800DC880("length"), 0, 0},
};

} // namespace Event
