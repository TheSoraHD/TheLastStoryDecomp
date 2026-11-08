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
extern "C" inline int fn_80223A08(Event::Action* action) {
    return action->m0C;
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
// inferred: the action is active at the current frame and is shown on the page (mode 0: not listed
// yet in the table of page 3; mode 1: a motion action)
int fn_802238D0(DebugInfo_unk* info, Action* action, Action* other, int mode) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    if (seq == 0) {
        return 0;
    }
    int start = action->m10;
    int end = action->m14;
    int length = end - start;
    if (start == 0 && length == 0) {
        end = 1;
    }
    float frame = seq->m198;
    if (start <= frame && end >= frame) {
        switch (mode) {
        case 0: {
            bool found;
            for (int i = 0; i < (int)info->m14.mColumns.size(); i++) {
                if (other == (Action*)info->m14.mColumns[i].m00) {
                    found = true;
                    goto done;
                }
            }
            found = false;
        done:
            if (found) {
                return 0;
            }
            return 1;
        }
        case 1: {
            int type = action->m0C;
            if (type == 0x21 || type == 0x25 || type == 0x20) {
                return 1;
            }
            return 0;
        }
        }
    }
    return 0;
}

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

// inferred: channel of the action (defined as extern "C" in Event/Node.cpp)
extern "C" void* fn_805381AC(void* self);

// NONMATCHING: draft (offsets of the strings in @stringBase0)
// inferred: adds (or replaces) the row of a motion; the key is the action, moved by `upper` for the
// upper body motion; the color marks a changed speed or start frame
void fn_80223F50(DebugInfo_unk* info, Action* action, int motion, int start, int loop, float speed, int upper) {
    const char* key = (const char*)action + upper;
    unsigned int color = 0xFFFFFFFF;
    if (speed != 1.0f) {
        if (speed >= 0.8f && speed <= 1.2f) {
            color = 0xFFFFFF00;
        } else {
            color = 0xFFFF0000;
        }
    } else if (0.0f != start) {
        color = 0xFFFF0000;
    }
    DebugInfoColumn_unk column;
    column.mTexts.push_back(fn_800DC980("%d", fn_802239F8(action)));
    column.mTexts.push_back(std::string(fn_80222A24((Node*)fn_805381AC(action))));
    column.mTexts.push_back(std::string(fn_80218218(motion)));
    column.mTexts.push_back(fn_800DC980("%.2f", speed));
    column.mTexts.push_back(fn_800DC980("%d", start));
    column.mTexts.push_back(std::string(loop ? "O" : "X"));
    column.m00 = (int)key;
    column.m10 = color;
    if (info->m2C.fn_80221CBC(key)) {
        info->m2C.fn_802214F8(column);
    } else {
        info->m2C.fn_8022051C(column);
    }
}

// ---- fn_80224184: page 4 (motion actions; over the inlining budget: weak copies) ----
// inferred: attribute of the action by name (weak copy 0x802245C0; C name: the method name is unknown)
extern "C" inline ActionParam_unk* fn_802245C0(Action* action, const char* name) {
    return action->fn_805392BC(fn_800DC880(name));
}
// inferred: value of an attribute read as int, float, motion and flag (four weak copies with the
// same code; names unknown)
extern "C" inline int fn_802245B8(ActionParam_unk* param) {
    return param->m04.i;
}
extern "C" inline float fn_80224600(ActionParam_unk* param) {
    return param->m04.f;
}
extern "C" inline int fn_80224608(ActionParam_unk* param) {
    return param->m04.i;
}
extern "C" inline int fn_80224610(ActionParam_unk* param) {
    return param->m04.i;
}

// NONMATCHING: draft - under the inlining budget here (the original calls push_back, the string
// constructor and the attribute accessors out of line)
void fn_80224184(DebugInfo_unk* info) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    if (seq == 0) {
        return;
    }
    fn_80221CAC(fn_800F7250(), info->m04, info->m08 + info->m0C, -1000000.0f, info->m0C, info->m0C,
                "Current Motions List:", 0xFFFFFFFF, 1);
    Cut* cut = seq->fn_805411DC();
    if (cut == 0) {
        return;
    }
    int index = fn_80221E00(cut);
    if (info->m10 != index) {
        info->m10 = index;
        DebugInfoColumn_unk column;
        column.mTexts.push_back(fn_800DC980("Next Cut [%s]\n", fn_80222A24(cut)));
        for (int i = 0; i < 5; i++) {
            column.mTexts.push_back(std::string(""));
        }
        column.m00 = 0;
        column.m10 = 0xFFC0C0C0;
        info->m2C.fn_8022051C(column);
    }
    for (int i = 0; i < fn_80223ED8(cut); i++) {
        Section* section = cut->GetSection_unk(i);
        for (int j = 0; j < fn_80223ED0(section); j++) {
            Channel* channel = section->GetChannel_unk(j);
            for (int k = 0; k < fn_80223EC8(channel); k++) {
                Action* action = channel->GetAction_unk(k);
                if (fn_802238D0(info, action, 0, 1) == 0) {
                    continue;
                }
                switch (fn_80223A08(action)) {
                case 0x21: {
                    int motion = fn_802245B8(fn_802245C0(action, "motion"));
                    float speed = fn_80224600(fn_802245C0(action, "speed"));
                    int start = fn_80224608(fn_802245C0(action, "start"));
                    fn_80223F50(info, action, motion, start, fn_80224610(fn_802245C0(action, "loop")), speed, 0);
                    if (fn_80224610(fn_802245C0(action, "part_mot")) != 0) {
                        int upMotion = fn_802245B8(fn_802245C0(action, "up_mot"));
                        float upSpeed = fn_80224600(fn_802245C0(action, "up_speed"));
                        int upStart = fn_80224608(fn_802245C0(action, "up_start"));
                        fn_80223F50(info, action, upMotion, upStart, fn_80224610(fn_802245C0(action, "up_loop")),
                                    upSpeed, 1);
                    }
                    break;
                }
                case 0x25: {
                    ActionParam_unk* use = fn_802245C0(action, "sync_use");
                    if (use == 0 || fn_80224610(use) == 0) {
                        break;
                    }
                    Actor* actor = fn_802249EC(fn_80224620(seq), fn_80224618(section));
                    atn::Model* model = actor != 0 ? actor->fn_80012A1C() : 0;
                    int motion = fn_802245B8(fn_802245C0(action, "sync_mot"));
                    fn_80223F50(info, action, motion, 0, 1, model != 0 ? model->GetMotionSpeed(0) : 1.0f, 0);
                    break;
                }
                case 0x20: {
                    int motion = fn_802245B8(fn_802245C0(action, "motion"));
                    float speed = fn_80224600(fn_802245C0(action, "speed"));
                    int start = fn_80224608(fn_802245C0(action, "start"));
                    fn_80223F50(info, action, motion, start, fn_80224610(fn_802245C0(action, "loop")), speed, 0);
                    break;
                }
                }
            }
        }
    }
    info->m2C.fn_802219CC(info->m04, info->m08, info->m0C, 3);
}

// ---- fn_80224638: page 5 (motions of the characters; over the inlining budget: weak copies) ----

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
