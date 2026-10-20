#include "Event/Section.h"
#include "atn/Memory.h"
#include "std/string_members.h"

namespace Event {

Section::Section(Cut* cut, int index) : mCut(cut), m0C(0), m10(0), m14(1) {}

// NONMATCHING: the boost::shared_ptr reference counts are lwarx/stwcx. loops in the original
// (asm functions of boost), plain increments here.
Channel* Section::fn_8053C788() {
    boost::shared_ptr<Channel> channel(new (4, "", "", 0) Channel(this));
    mChannels.push_back(channel);
    return mChannels[mChannels.size() - 1].get();
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
