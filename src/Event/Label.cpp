#include "Event/Label.h"
#include "std/string_members.h"

namespace Event {

Label::Label(Cut* cut, int index) {
    mCut = cut;
    mFrame = 0;
    mIndex = index;
}

// inferred: the reader of the label attributes was stripped by the linker; only its literals are
// left in @stringBase0 ("." and the basic_string errors of the out-of-line replace). Reconstructed
// minimally so that the pool keeps its layout.
void Label::fn_Stripped_unk(std::string& path) {
    path += ".";
}

// inferred: attributes of a label in the event file
// NONMATCHING (__sinit_\Label_cpp): 93.8% - after the last hash call the original stores the
// default of "frame" (0x3C) before its hash (0x34) and interleaves the epilogue; tried int/pointer/
// class/union defaults, a class for the hash, const/static/global table, omitted defaults.
static ParamDef_unk sParamDefs_unk[] = {
    {"name", fn_800DC880("name"), 0x1C, ""},
    {"id", fn_800DC880("id"), 0, 0},
    {"frame", fn_800DC880("frame"), 0, 0},
};

} // namespace Event
