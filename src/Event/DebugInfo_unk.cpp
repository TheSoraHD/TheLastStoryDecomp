// Debug display of the events (Event::DebugInfo_unk, Event::Manager + 0x1DFC). Only part of the unit is
// written (the pages 0x80221FA4-0x80224A2C and their tables are missing), so it is not linked.
#include "Event/DebugInfo_unk.h"
#include "game/GameConfig_unk.h"
#include "Event/Manager.h"
#include "Event/Sequence.h"
#include "Event/Cut.h"
#include "Event/Section.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "atn/DrawHelper.h"
#include "atn/Model_unk.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" Event::Manager* lbl_8087F540;
extern "C" int sprintf(char* s, const char* format, ...);

// inferred: accessors (inline: weak copies, some of other files)
extern "C" inline Event::Manager* fn_8000DB1C() {
    return lbl_8087F540;
}
extern "C" inline atn::DrawHelper* fn_800F7250() {
    return lbl_8087EEB0;
}
// inferred: draws a text (weak copy 0x80221CAC; name invented)
extern "C" inline void fn_80221CAC(atn::DrawHelper* helper, float x, float y, float z, float w, float h, const char* text,
                                   unsigned int color, int arg) {
    helper->fn_800616C0(x, y, z, w, h, text, color, 0.0f, arg, 0, 0);
}
// inferred: weak copies of the event tree (C names: their class names are unknown)
extern "C" inline int fn_80221E00(Event::Cut* cut) {
    return cut->mIndex;
}
extern "C" inline const char* fn_80222A24(Event::Node* node) {
    return node->mName.c_str();
}
extern "C" inline int fn_802239F8(Event::Action* action) {
    return action->m10;
}
extern "C" inline int fn_80223A00(Event::Action* action) {
    return action->m14;
}
extern "C" inline int fn_80223EB0(Event::Sequence* seq) {
    return seq->m194;
}
extern "C" inline int fn_80223EB8(Event::Action* action) {
    return action->m18;
}
extern "C" inline int fn_80223EC0(Event::Action* action) {
    return action->m1C;
}
extern "C" inline int fn_80223EC8(Event::Channel* channel) {
    return channel->mActions.size();
}
extern "C" inline int fn_80223ED0(Event::Section* section) {
    return section->mChannels.size();
}
extern "C" inline int fn_80223ED8(Event::Cut* cut) {
    return cut->mSections.size();
}

namespace Event {

// inferred: draw functions of the pages (not written yet)
void fn_80221FA4(DebugInfo_unk* info);
void fn_80222AD0(DebugInfo_unk* info);
void fn_8022354C(DebugInfo_unk* info);
void fn_80223A10(DebugInfo_unk* info);
void fn_80224184(DebugInfo_unk* info);
void fn_80224638(DebugInfo_unk* info);

int fn_802238D0(DebugInfo_unk* info, Action* action, Action* other, int arg); // inferred: the action is shown

// inferred: columns of the three tables (6, 6 and 10 columns)
static DebugInfoColumnDef_unk sColumns0_unk[6] = {
    {0, 0, 0, 50.0f}, {0, 0, 0, 60.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 60.0f}, {0, 0, 0, 220.0f}, {0, 0, 0, 100.0f},
};
static DebugInfoColumnDef_unk sColumns1_unk[6] = {
    {0, 0, 0, 70.0f}, {0, 0, 0, 60.0f}, {0, 0, 0, 180.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 80.0f}, {0, 0, 0, 100.0f},
};
static DebugInfoColumnDef_unk sColumns2_unk[10] = {
    {0, 0, 0, 80.0f}, {0, 0, 0, 70.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 40.0f}, {0, 0, 0, 70.0f}, {0, 0, 0, 50.0f},
    {0, 0, 0, 40.0f}, {0, 0, 0, 70.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 40.0f},
};

// NONMATCHING: 90% - same instructions; the original keeps -1 in r31, 0 in r30 and the column table in r29
// (ours: table r31, -1 r30, 0 r29). Tried the table as a static/global array, a local pointer, decayed array.
DebugInfo_unk::DebugInfo_unk() : mPage(0), m04(30.0f), m08(30.0f), m0C(14.0f), m10(-1), m74(0) {
    m14.fn_80220168(sColumns0_unk, 6, 25);
    m2C.fn_80220168(sColumns1_unk, 6, 25);
    m44.fn_80220168(sColumns2_unk, 10, 25);
    m10 = -1;
    m74 = 0;
    mPages[0] = fn_80221FA4;
    mPages[1] = fn_80222AD0;
    mPages[2] = fn_8022354C;
    mPages[3] = fn_80223A10;
    mPages[4] = fn_80224184;
    mPages[5] = fn_80224638;
}

void DebugInfo_unk::fn_80221F54(int page) {
    if (page >= 6) {
        return;
    }
    if (mPages[page] == 0) {
        return;
    }
    if (mPage != page) {
        lbl_8087F0A8->mDrawSystemInfo = 0;
    }
    mPage = page;
    mPages[mPage](this);
}

// NONMATCHING: draft - page 3: actions of the current cut (and the name of the next cut); the earlier pages
// are not written, so the offsets of the string pool differ
void fn_80223A10(DebugInfo_unk* info) {
    Manager* mgr = fn_8000DB1C();
    Sequence* seq = mgr->fn_80112F54();
    if (seq == 0) {
        return;
    }
    fn_80221CAC(fn_800F7250(), info->m04, info->m08 + info->m0C, -1000000.0f, info->m0C, info->m0C,
                "Current Actions List:", 0xFFFFFFFF, 1);
    Cut* cut = seq->fn_805411DC();
    if (cut == 0) {
        return;
    }
    int index = fn_80221E00(cut);
    DebugInfoColumn_unk column;
    std::string text;
    if (info->m10 != index) {
        info->m10 = index;
        char buf[0x100];
        sprintf(buf, "Next Cut [%s]\n", fn_80222A24(cut));
        text = buf;
        column.mTexts.push_back(text);
        text = "";
        for (int i = 0; i < 5; i++) {
            column.mTexts.push_back(text);
        }
        column.m00 = 0;
        column.m10 = 0xFFC0C0C0;
        info->m14.fn_8022051C(column);
    }
    for (int i = 0; i < fn_80223ED8(cut); i++) {
        Section* section = cut->GetSection_unk(i);
        for (int j = 0; j < fn_80223ED0(section); j++) {
            Channel* channel = section->GetChannel_unk(j);
            for (int k = 0; k < fn_80223EC8(channel); k++) {
                Action* action = channel->GetAction_unk(k);
                if (fn_802238D0(info, action, action, 0) == 0 || fn_80223EB0(seq) != 10) {
                    continue;
                }
                column.mTexts.clear();
                char buf[0x100];
                sprintf(buf, "%d", fn_802239F8(action));
                column.mTexts.push_back(std::string(buf));
                sprintf(buf, "%d", fn_80223A00(action) - fn_802239F8(action));
                column.mTexts.push_back(std::string(buf));
                sprintf(buf, "%d", fn_80223EB8(action));
                column.mTexts.push_back(std::string(buf));
                sprintf(buf, "%d", fn_80223EC0(action));
                column.mTexts.push_back(std::string(buf));
                sprintf(buf, "%s.%s", fn_80222A24(section), fn_80222A24(channel));
                column.mTexts.push_back(std::string(buf));
                sprintf(buf, "%s", fn_80222A24(action));
                column.mTexts.push_back(std::string(buf));
                column.m00 = (int)action;
                info->m14.fn_8022051C(column);
            }
        }
    }
    info->m14.fn_802219CC(info->m04, info->m08, info->m0C, 3);
}


// ---- fn_80224638: page 5 (motions of the characters; over the inlining budget: weak copies) ----
// inferred: accessors of the event tree (weak copies; C names: their class names are unknown)
extern "C" inline void fn_8022492C(DebugInfoTable_unk* table) {
    table->mColumns.clear();
}
extern "C" inline int fn_802249E4(Section* section) {
    return section->m0C;
}
extern "C" inline int fn_80224618(Section* section) {
    return section->m10;
}
extern "C" inline DataList_unk<Actor>* fn_80224620(Sequence* seq) {
    return &seq->m164;
}
// inferred: actor of the given id (weak copy 0x802249EC)
extern "C" inline Actor* fn_802249EC(DataList_unk<Actor>* actors, int id) {
    for (int i = 0; i < (int)actors->mSize; i++) {
        Actor* actor = &actors->mData[(unsigned int)i];
        if (id == actor->m14) {
            return actor;
        }
    }
    return 0;
}
extern "C" const char* fn_80218218(int motion);           // inferred: motion name
extern "C" std::string fn_800DC980(const char* format, ...); // inferred: formatted string

// inferred: motion tracks shown for each character
static const int sTracks_unk[3] = {0, 3, 4};

// NONMATCHING: offsets of the strings in @stringBase0 (pages 1, 2 and 4 are missing)
void fn_80224638(DebugInfo_unk* info) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    if (seq == 0) {
        return;
    }
    fn_80221CAC(fn_800F7250(), info->m04, info->m08 + info->m0C, -1000000.0f, info->m0C, info->m0C,
                "Character motions List:", 0xFFFFFFFF, 1);
    Cut* cut = seq->fn_805411DC();
    if (cut == 0) {
        return;
    }
    fn_8022492C(&info->m44);
    for (int i = 0; i < fn_80223ED8(cut); i++) {
        Section* section = cut->GetSection_unk(i);
        if (fn_802249E4(section) != 3) {
            continue;
        }
        int id = fn_80224618(section);
        Actor* actor = fn_802249EC(fn_80224620(seq), id);
        if (actor == 0) {
            continue;
        }
        atn::Model* model = actor->fn_80012A1C();
        if (model == 0) {
            continue;
        }
        DebugInfoColumn_unk column;
        column.mTexts.push_back(std::string(fn_80222A24(section)));
        for (int k = 0; k < 3; k++) {
            int motion = model->fn_80139F2C(sTracks_unk[k]);
            float speed = model->GetMotionSpeed(sTracks_unk[k]);
            float frame = model->GetMotionFrame(sTracks_unk[k]);
            column.mTexts.push_back(std::string(motion < 0 ? "" : fn_80218218(motion)));
            column.mTexts.push_back(motion < 0 ? std::string("") : fn_800DC980("%.2f", speed));
            column.mTexts.push_back(motion < 0 ? std::string("") : fn_800DC980("%.0f", frame));
        }
        info->m44.fn_8022051C(column);
    }
    info->m44.fn_802219CC(info->m04, info->m08, info->m0C, 3);
}

} // namespace Event
