// Debug display of the events (Event::DebugInfo_unk, Event::Manager + 0x1DFC). Every function is written; the
// pool @stringBase0 lacks the "" that follows "vector length error" in the original (its first user is unknown)
// and the three basic_string messages before the column names, so the pages are one byte off (not linked).
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
#include "game/MapTask.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" Event::Manager* lbl_8087F540;
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" std::string fn_800DC980(const char* format, ...); // inferred: formatted string

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

namespace std {

// inferred: growth of vector<string>::operator= (defined out of the class: its message goes to the file
// pool; no check of the result here, it is checked after the old storage is freed)
template <class T>
unsigned int __vec_grow_assign_unk(vector<T>& v, unsigned int n) {
    if (n > v.max_size() - v.capacity()) {
        __msl_error("vector length error");
    }
    unsigned int cap = v.capacity();
    if (cap < v.max_size() / 3) {
        cap += __vec_max_unk((unsigned int)((cap + 1) * 3 / 5), n);
    } else if (cap < v.max_size() / 3 * 2) {
        cap += __vec_max_unk((unsigned int)((cap + 1) / 2), n);
    } else {
        cap = v.max_size();
    }
    return cap;
}

template <class T>
inline const T& __vec_min_unk(const T& a, const T& b) {
    return (b < a) ? b : a;
}

// inferred: assignment (inlined in the weak copy of Event::DebugInfoColumn_unk::operator=, 0x80220660):
// the old elements are assigned first, the rest destroyed or copy-constructed; when the new size is over
// the capacity, the storage is cleared and freed before the new one is allocated. Defined here, not in
// std/vector_string.h: a new body in that header renumbers the @N labels of the files that include it.
// NONMATCHING: 53% (weak copy 0x80220660) - block order (the original lays out the assignment path between
// the growth and the reallocation)
template <>
inline vector<string>& vector<string>::operator=(const vector<string>& x) {
    if (this != &x) {
        const string* first = x.mData;
        const string* last = x.mData + x.mSize;
        size_type n = last - first;
        if (n > this->mCap) {
            size_type cap = __vec_grow_assign_unk(*this, n - this->mCap);
            this->clear();
            if (this->mData != 0) {
                ::operator delete(this->mData);
                this->mData = 0;
                this->mCap = 0;
            }
            __check_length_unk(cap);
            this->mData = __vec_alloc_sel_unk<string>::alloc(cap);
            this->mCap = cap;
            string* p = this->mData + this->mSize;
            for (; first != last; ++first, ++p) {
                new (p) string(*first);
                ++this->mSize;
            }
        } else {
            const string* mid = first + __vec_min_unk(this->mSize, n);
            string* p = this->mData;
            for (; first < mid; ++first, ++p) {
                *p = *first;
            }
            if (n < this->mSize) {
                this->__shrink_unk(n);
            } else if (this->mSize < n) {
                p = this->mData + this->mSize;
                for (; mid != last; ++mid, ++p) {
                    new (p) string(*mid);
                    ++this->mSize;
                }
            }
        }
    }
    return *this;
}

// inferred: reserve of the rows of a debug table (inlined in 0x80220168): the rows are copy-constructed
// into the new storage and the old ones destroyed with it, not moved as raw bytes
template <>
inline void vector<Event::DebugInfoColumn_unk>::reserve(size_type n) {
    if (n > this->mCap) {
        __vec_storage_unk<Event::DebugInfoColumn_unk> buf;
        __check_length_unk(n);
        buf.mData = __vec_alloc_sel_unk<Event::DebugInfoColumn_unk>::alloc(n);
        buf.mCap = n;
        Event::DebugInfoColumn_unk* last = this->mData + this->mSize;
        Event::DebugInfoColumn_unk* dst = buf.mData + buf.mSize;
        for (Event::DebugInfoColumn_unk* p = this->mData; p < last; ++p, ++dst) {
            new (dst) Event::DebugInfoColumn_unk(*p);
            ++buf.mSize;
        }
        __vec_swap_unk(this->mCap, buf.mCap);
        __vec_swap_unk(this->mData, buf.mData);
        __vec_swap_unk(this->mSize, buf.mSize);
    }
}

} // namespace std

namespace Event {

// inferred: as in Event/Sequence.cpp (inline, defined there)
inline Cut* Sequence::FindCutByIndex_unk(int index) {
    for (int i = 0; i < (int)mB8.size(); i++) {
        if (index == mB8[i]->mIndex) {
            return mB8[i].get();
        }
    }
    return 0;
}

// inferred: sets the columns of the table and drops its rows
// inferred: noinline: the constructor calls it with bl (MWCC would inline it there)
// NONMATCHING: 98.2% - registers of the clear() loop (count and pointer)
__declspec(noinline) void DebugInfoTable_unk::fn_80220168(const DebugInfoColumnDef_unk* defs, int count, int rows) {
    m00 = defs;
    m04 = count;
    mColumns.clear();
    mColumns.reserve(count);
    m14 = rows;
}

// inferred: weak copy (inline)
inline int DebugInfoTable_unk::fn_80220620() {
    return (int)mColumns.size() >= m14 - 1;
}

// inferred: adds a row at the top of the table (the last one is dropped when the table is full)
int DebugInfoTable_unk::fn_8022051C(const DebugInfoColumn_unk& column) {
    if (m04 != column.mTexts.size()) {
        return 0;
    }
    if (fn_80220620()) {
        mColumns.back() = column;
    } else {
        mColumns.push_back(column);
    }
    for (int i = mColumns.size() - 1; i > 0; i--) {
        DebugInfoColumn_unk tmp(mColumns[i - 1]);
        mColumns[i - 1] = mColumns[i];
        mColumns[i] = tmp;
    }
    return 1;
}

// inferred: replaces the row with the key of `column` (0: wrong width or no such row)
// NONMATCHING: 55% - block order of the inlined vector<string>::operator= (the original lays out the
// assignment path between the growth and the reallocation; same in the weak copy 0x80220660, 53%)
int DebugInfoTable_unk::fn_802214F8(const DebugInfoColumn_unk& column) {
    if (m04 != column.mTexts.size()) {
        return 0;
    }
    for (int i = 0; i < (int)mColumns.size(); i++) {
        if (column.m00 == mColumns[i].m00) {
            mColumns[i] = column;
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 88.9% - scheduling of the first draw call (the original takes c_str() before the row)
// inferred: draws the table: the names of the columns in the row `row`, then the rows below them
void DebugInfoTable_unk::fn_802219CC(float x, float y, float h, int row) {
    float left = 0.0f;
    for (int i = 0; i < m04; i++) {
        std::string name = m00[i].mName;
        fn_80221CAC(fn_800F7250(), left + (x + h), y + h * row, -1000000.0f, h, h, name.c_str(), 0xFF00EAAF, 1);
        int r = row + 2;
        for (int j = 0; j < (int)mColumns.size(); j++) {
            std::string text = mColumns[j].mTexts[i];
            fn_80221CAC(fn_800F7250(), left + (x + h), y + h * r++, -1000000.0f, h, h, text.c_str(),
                        mColumns[j].m10, 1);
        }
        left += m00[i].mWidth;
    }
}

// inferred: draw functions of the pages (not written yet)
void fn_80221FA4(DebugInfo_unk* info);
void fn_80222AD0(DebugInfo_unk* info);
void fn_8022354C(DebugInfo_unk* info);
void fn_80223A10(DebugInfo_unk* info);
void fn_80224184(DebugInfo_unk* info);
void fn_80224638(DebugInfo_unk* info);

int fn_802238D0(DebugInfo_unk* info, Action* action, Action* other, int arg); // inferred: the action is shown

// inferred: columns of the three tables (6, 6 and 10 columns)
// inferred: the names are built by the __sinit of the unit (NONMATCHING: 96.42% - offsets of the names in
// @stringBase0, the strings of the page functions before them are not written yet, and registers)
static DebugInfoColumnDef_unk sColumns0_unk[6] = {
    {"Frame", 50.0f}, {"Length", 60.0f}, {"FadeIn", 50.0f}, {"FadeOut", 60.0f}, {"Path", 220.0f}, {"Name", 100.0f},
};
static DebugInfoColumnDef_unk sColumns1_unk[6] = {
    {"Frame", 70.0f}, {"Actor", 60.0f}, {"Motion", 180.0f}, {"Speed", 50.0f}, {"St.Frame", 80.0f}, {"Loop", 100.0f},
};
static DebugInfoColumnDef_unk sColumns2_unk[10] = {
    {"Actor", 80.0f}, {"Low", 70.0f}, {"Speed", 50.0f}, {"Frame", 40.0f}, {"Upper", 70.0f},
    {"Speed", 50.0f}, {"Frame", 40.0f}, {"Face", 70.0f}, {"Speed", 50.0f}, {"Frame", 40.0f},
};

// inferred: a column with this key exists
int DebugInfoTable_unk::fn_80221CBC(const void* key) {
    for (int i = 0; i < (int)mColumns.size(); i++) {
        if (key == mColumns[i].m00) {
            return 1;
        }
    }
    return 0;
}

// inferred: number of cuts from the current one to the cut `index` (0: current, -1: not reached)
// inferred: noinline: page 0 calls it with bl (MWCC would inline it there)
__declspec(noinline) int fn_80221CF8(int index) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    int count = 0;
    if (index == seq->m94) {
        return 0;
    }
    Cut* cut = seq->FindCutByIndex_unk(seq->m94);
    if (cut == 0) {
        return -1;
    }
    for (int next = cut->fn_8053B028(); next != -1; next = seq->FindCutByIndex_unk(next)->fn_8053B028()) {
        count++;
        if (index == next) {
            return count;
        }
    }
    return -1;
}

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

// inferred: name of the MA of the event (weak copy 0x80222A38; C name: the method name is unknown)
extern "C" inline std::string fn_80222A38(const Event::Manager* mgr) {
    return mgr->m1E7C;
}

// inferred: delay of the MA (end - start)
static inline float GetMADelay_unk(Manager* mgr) {
    float end;
    float start = mgr->m1E88;
    end = mgr->m1E8C;
    return end - start;
}

// inferred: frames of the cuts before the cut `index`, plus `frame`
static inline float GetElapsed_unk(Sequence* seq, int index, float frame) {
    float elapsed = 0.0f;
    for (int i = 0; i < (int)seq->mB8.size(); i++) {
        Cut* c = seq->IsCutIndex_unk(i) ? seq->mB8[i].get() : 0;
        if (index == c->mIndex) {
            elapsed += frame;
            break;
        }
        elapsed += c->m2C;
    }
    return elapsed;
}

// inferred: frames of all the cuts
static inline int GetTotal_unk(Sequence* seq) {
    int total = 0;
    for (int i = 0; i < (int)seq->mB8.size(); i++) {
        Cut* c = seq->IsCutIndex_unk(i) ? seq->mB8[i].get() : 0;
        total += c->m2C;
    }
    return total;
}

// inferred: frame rate of the event
static inline float GetFps_unk(Manager* mgr) {
    return mgr->m1EB4;
}

// inferred: draws a line of the event info (x 40, size 12)
static inline void DrawInfoLine_unk(float x, float y, const std::string& text) {
    fn_80221CAC(fn_800F7250(), x, y, -1000000.0f, 12.0f, 12.0f, text.c_str(), 0xFFFFFFFF, 0);
}

// inferred: page 0 of the debug info: the event, the current cut, the frames, the MA and the frame rate
// NONMATCHING: 99.83% - offsets of the strings in @stringBase0 (the "" after "vector length error") and the
// stack slots of the iterator tags of the two appends
void fn_80221FA4(DebugInfo_unk* info) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    if (seq == 0) {
        return;
    }
    Cut* cut = seq->fn_805411DC();
    if (cut == 0) {
        return;
    }
    float frame = seq->m198;
    float x = 40.0f;
    float y = 246.0f;
    std::string text = fn_800DC980("Event       : %s\n", seq->mName.c_str());
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    text = fn_800DC980("Cut Number  : %d\n", fn_80221CF8(cut->mIndex));
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    text = fn_800DC980("Cut Name    : %s\n", cut->mName.c_str());
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    text = fn_800DC980("Cut Frame   : %.02f / %.02f\n", frame, (float)cut->m2C);
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    float elapsed = GetElapsed_unk(fn_8000DB1C()->fn_80112F54(), cut->mIndex, frame);
    int total = GetTotal_unk(fn_8000DB1C()->fn_80112F54());
    text = fn_800DC980("Total Frame : %.02f / %.02f\n", elapsed, (float)total);
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    int ma = fn_8000DB1C()->m1E78;
    text = "MA          : ";
    if (ma != 0) {
        text += fn_80222A38(fn_8000DB1C());
    } else {
        text += "\n";
    }
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    text = "MA Delay    : ";
    if (ma != 0) {
        text += fn_800DC980("%.02f\n", GetMADelay_unk(fn_8000DB1C()));
    }
    DrawInfoLine_unk(x, y, text);
    y += 13.0f;
    text = fn_800DC980("Fps         : %.02f\n", GetFps_unk(fn_8000DB1C()));
    DrawInfoLine_unk(x, y, text);
    lbl_8087F0A8->mDrawSystemInfo = 2;
}

// inferred: name of the state of a sequence (inline with external linkage: its literals go to .data, in
// reverse order)
inline const char* GetSequenceStateName_unk(int state) {
    switch (state) {
    case 0:
        return "error";
    case 1:
        return "created";
    case 2:
        return "loading xml";
    case 3:
        return "loading bin";
    case 4:
        return "loaded";
    case 5:
        return "unprepared";
    case 6:
        return "preparing";
    case 7:
        return "stop";
    case 8:
        return "ready";
    case 9:
        return "play";
    case 10:
        return "playing";
    case 11:
        return "pause";
    case 12:
        return "paused";
    case 13:
        return "finish";
    case 14:
        return "finished";
    }
    return 0;
}

// inferred: name of the kind of a sequence (m50)
inline const char* GetSequenceTypeName_unk(int type) {
    switch (type) {
    case 1:
        return "main";
    case 2:
        return "background";
    case 3:
        return "in_game";
    }
    return 0;
}

// inferred: origin and position of the event (copied by value)
static inline atn::Vector3 GetOrigin_unk(Sequence* seq) {
    return seq->m9C;
}
static inline atn::Vector3 GetPos_unk(Sequence* seq) {
    return seq->mA8;
}

// inferred: cameras and frame time of the manager
static inline int GetCurCam_unk(Manager* mgr) {
    return mgr->m1A38;
}
static inline int GetPreCam_unk(Manager* mgr) {
    return mgr->m1A3C;
}
static inline float GetSpeed_unk(Manager* mgr) {
    return mgr->mB4;
}

// inferred: first cut of the sequence
static inline int GetStartCut_unk(Sequence* seq) {
    return seq->m94;
}

// inferred: wait mode of the sequence
static inline int GetWaitMode_unk(Sequence* seq) {
    return seq->m54;
}

// inferred: radians to degrees
static inline float RadToDeg_unk(float rad) {
    return 57.295776f * rad;
}

// inferred: a sound plays on the channel `i` of the sequence
static inline bool HasSound_unk(Sequence* seq, int i) {
    return seq->m174[i].mPtr != 0;
}

// inferred: draws a row of a debug page
static inline void DrawRow_unk(DebugInfo_unk* info, int row, const char* text) {
    fn_80221CAC(fn_800F7250(), info->m04, info->m08 + info->m0C * row, -1000000.0f, info->m0C, info->m0C, text,
                0xFFFFFFFF, 1);
}

// inferred: page 1 of the debug info: state of the manager and of the sequence
// NONMATCHING: 99.31% - offsets of the strings in @stringBase0 (the "" after "vector length error") and the
// registers of the sound channel flags
void fn_80222AD0(DebugInfo_unk* info) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    if (seq == 0) {
        return;
    }
    int row = 1;
    const char* cam = "error";
    switch (fn_8000DB1C()->m1A38) {
    case 0:
        cam = "main";
        break;
    case 1:
        cam = "sub1";
        break;
    case 2:
        cam = "sub2";
        break;
    case 3:
        cam = "sub3";
        break;
    }
    int mem = seq->m288 / 1024;
    char buf[0x100];
    DrawRow_unk(info, row++, "Event Info:");
    row++;
    DrawRow_unk(info, row++, "Manager:");
    sprintf(buf, " state=%s, cam=%s, mem=%d KB", fn_8000DB1C()->fn_804803A4() ? "Ready" : "Initializing", cam, mem);
    DrawRow_unk(info, row++, buf);
    {
        Manager* m = fn_8000DB1C();
        sprintf(buf, " speed=%.2f, skipping=%s ", GetSpeed_unk(m), m->fn_801130F4() ? "1" : "0");
    }
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " curCamIndex=%d, preCamIndex=%d ", GetCurCam_unk(fn_8000DB1C()), GetPreCam_unk(fn_8000DB1C()));
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " charaNum=%d ", fn_8000DB1C()->m1A70.size());
    DrawRow_unk(info, row++, buf);
    row++;
    DrawRow_unk(info, row++, "Seqence:");
    sprintf(buf, " state=%s, id=%d ", GetSequenceStateName_unk(seq->m194), seq->m190);
    DrawRow_unk(info, row++, buf);
    std::string type = GetSequenceTypeName_unk(seq->m50);
    char map[0x100];
    MapTask::GetMapNameFromId(map, seq->GetState_unk().m02, seq->GetState_unk().m00, seq->GetState_unk().m03);
    sprintf(buf, " name=%s, type=%s, waitMode=%d, map=%s ", seq->mName.c_str(), type.c_str(), GetWaitMode_unk(seq), map);
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " messageFile=%s ", seq->m68.c_str());
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " startCut=%d, emptyHands=%s, customFov=%s, ", GetStartCut_unk(seq), seq->m98_3 ? "1" : "0",
            seq->m98_4 ? "1" : "0");
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " orientation=%f, cutNumber=%d ", RadToDeg_unk(seq->mB4), seq->mB8.size());
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " Origin: x=%f, y=%f, z=%f ", GetOrigin_unk(seq).x, GetOrigin_unk(seq).y, GetOrigin_unk(seq).z);
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " Pos   : x=%f, y=%f, z=%f ", GetPos_unk(seq).x, GetPos_unk(seq).y, GetPos_unk(seq).z);
    DrawRow_unk(info, row++, buf);
    sprintf(buf, " Frame: cur=%.2f ", seq->m198);
    // inferred: the original takes the x position for y here (as the mark of fn_8022354C)
    fn_80221CAC(fn_800F7250(), info->m04, info->m04 + info->m0C * row++, -1000000.0f, info->m0C, info->m0C, buf,
                0xFFFFFFFF, 1);
    bool snd0 = HasSound_unk(seq, 0);
    bool snd1 = HasSound_unk(seq, 1);
    bool snd2 = HasSound_unk(seq, 2);
    bool snd3 = HasSound_unk(seq, 3);
    sprintf(buf, " sndChannel=[%s][%s][%s][%s]", snd0 ? " " : "X", snd1 ? " " : "X", snd2 ? " " : "X",
            snd3 ? " " : "X");
    DrawRow_unk(info, row++, buf);
}

// inferred: cut `index` of the sequence (0 when out of range)
inline Cut* GetCutAt_unk(Sequence* seq, int index) {
    return (index >= 0 && index < (int)seq->mB8.size()) ? seq->mB8[index].get() : 0;
}

// inferred: page 2 of the debug info: list of the cuts (ten around the current one)
// NONMATCHING: 96.3% - offsets of the strings in @stringBase0 (the "" after "vector length error") and the
// register of the length of the cut
void fn_8022354C(DebugInfo_unk* info) {
    Sequence* seq = fn_8000DB1C()->fn_80112F54();
    if (seq == 0) {
        return;
    }
    char buf[0x100];
    int row = 1;
    fn_80221CAC(fn_800F7250(), info->m04, info->m08 + info->m0C * row, -1000000.0f, info->m0C, info->m0C,
                "Cut List:", 0xFFFFFFFF, 1);
    row += 2;
    sprintf(buf, "Cut: num=%d", seq->mB8.size());
    fn_80221CAC(fn_800F7250(), info->m04, info->m08 + info->m0C * row++, -1000000.0f, info->m0C, info->m0C, buf,
                0xFFFFFFFF, 1);
    int count = seq->mB8.size();
    int start = 0;
    for (int page = 1; page < count / 10 + 1; page++) {
        if (page * 10 <= seq->m1A0) {
            start = page * 10;
        }
    }
    int end = start + 10;
    if (end >= count) {
        end = count;
    }
    for (int i = start; i < end; i++) {
        if (i == seq->m1A0) {
            sprintf(buf, ">");
            // inferred: the original takes the x position for y here (the mark is drawn off the list)
            fn_80221CAC(fn_800F7250(), info->m04, info->m04 + info->m0C * row, -1000000.0f, info->m0C,
                        info->m0C, buf, 0xFFFFFFFF, 1);
        }
        Cut* cut = GetCutAt_unk(seq, i);
        if (i < 9) {
            const char* mark = cut->mIndex == seq->m94 ? "StartCut" : "";
            const char* skip = cut->fn_8053C124(cut->mSkip);
            const char* name = fn_80222A24(cut);
            int len = cut->m2C;
            int id = cut->mIndex;
            sprintf(buf, "[%02d]: id=%d, name=%s,  next=%d, len=%d, skip=%s, %s", i, id, name, cut->fn_8053B028(),
                    len, skip, mark);
        } else {
            const char* mark = cut->mIndex == seq->m94 ? "StartCut" : "";
            const char* skip = cut->fn_8053C124(cut->mSkip);
            const char* name = fn_80222A24(cut);
            int len = cut->m2C;
            int id = cut->mIndex;
            sprintf(buf, "[%02d]: id=%d, name=%s, next=%d, len=%d, skip=%s, %s", i, id, name, cut->fn_8053B028(),
                    len, skip, mark);
        }
        fn_80221CAC(fn_800F7250(), info->m04 + info->m0C, info->m08 + info->m0C * row++, -1000000.0f,
                    info->m0C, info->m0C, buf, 0xFFFFFFFF, 1);
    }
}

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
                column.m00 = action;
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

// inferred: channel of the action (defined as extern "C" in Event/Node.cpp)
extern "C" void* fn_805381AC(void* self);

// NONMATCHING: 99.96% - offsets of the strings in @stringBase0 (the "" after "vector length error")
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
    column.m00 = key;
    column.m10 = color;
    if (info->m2C.fn_80221CBC(key)) {
        info->m2C.fn_802214F8(column);
    } else {
        info->m2C.fn_8022051C(column);
    }
}

// ---- fn_80224184: page 4 (motion actions; over the inlining budget: weak copies) ----
// inferred: attribute of the action by name (weak copy 0x802245C0; C name: the method name is unknown). The
// empty assertions put fn_80224184 over the inlining budget (R3: the original calls every inline there)
extern "C" inline ActionParam_unk* fn_802245C0(Action* action, const char* name) {
    SEQ_ASSERT10_UNK;
    SEQ_ASSERT10_UNK;
    SEQ_ASSERT10_UNK;
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

// inferred: page 4 of the debug info: motion actions of the current cut
// NONMATCHING: 99.92% - offsets of the strings in @stringBase0 (the "" after "vector length error")
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

// NONMATCHING: 99.88% - offsets of the strings in @stringBase0 (the "" after "vector length error")
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
