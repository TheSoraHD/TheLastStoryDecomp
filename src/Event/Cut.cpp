#include "Event/Cut.h"
#include "Event/Sequence.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace Event {

Cut::Cut(Sequence* sequence, int index)
    : mSequence(sequence), mSkip(0), mCinematic(1), m18(0), m1C(0), m20(0), m24(0), mNextCut(-1),
      m2C(0), m48(0), m49(0), m4A(0), m4B(0) {
    mIndex = index;
}

int Cut::fn_8053B028() {
    return mNextCut;
}

// NONMATCHING: 41.5% - the boost::shared_ptr counts (lwarx/stwcx. of sp_counted_base_cw_ppc.hpp)
// now match; what differs is the inlined std::vector::push_back growth path of std/vector.h
// (the original keeps max_size() in a register and divides it at run time, and keeps an
// unreachable copy of the fast path after the length check).
Label* Cut::fn_8053B030(int index) {
    if (index == -1) {
        index = ++mSequence->mLabelCount;
    } else if (index > mSequence->mLabelCount) {
        mSequence->mLabelCount = index;
    }
    boost::shared_ptr<Label> label(new ((MemoryTag)4, "", "", 0) Label(this, index));
    mLabels.push_back(label);
    return mLabels[mLabels.size() - 1].get();
}

// NONMATCHING: 40.5% - the boost::shared_ptr counts (lwarx/stwcx. of sp_counted_base_cw_ppc.hpp)
// now match; what differs is the inlined std::vector::push_back growth path of std/vector.h
// (the original keeps max_size() in a register and divides it at run time, and keeps an
// unreachable copy of the fast path after the length check).
Section* Cut::fn_8053B828(int index) {
    boost::shared_ptr<Section> section(new ((MemoryTag)4, "", "", 0) Section(this, index));
    mSections.push_back(section);
    return mSections[mSections.size() - 1].get();
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

// NONMATCHING: 98.7% - only register numbers: the original gives the vector destroy loop
// (count, element, &pn, pi) r25-r28 in the opposite order (tried std/vector.h variants: loop
// variable order, clear() instead of __shrink_unk, extra inline levels for the element destructor).
// The weak ~Section emitted here (0x8053BF80, 98.9%) has the same difference.
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
