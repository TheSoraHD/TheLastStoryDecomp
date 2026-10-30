#include "game/NewCampPartsHint.h"
#include "game/TutorialTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const void* pos);
extern "C" void fn_801F6C80(atn::Task* task, const char* name, float value); // inferred: sets a value
extern "C" void fn_801F837C(atn::Task* task, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801F7590(atn::Task* task, const char* name, const NewCampColor_unk& value);
extern "C" void fn_801F7DF0(atn::Task* task, const char* name, float r, float g, float b); // inferred: sets a colour
extern "C" void fn_801F791C(atn::Task* task, const char* name, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" atn::SoundRef fn_80117228(int id);        // inferred: plays a system sound effect
extern "C" void fn_800D2338(atn::Task* task);        // inferred: deletes the task
extern "C" void* fn_8008937C(void* menu, const char* name); // inferred: debug menu group
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088FAC(void* group, const char* name, NewCampColor_unk* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_804A436C(int* cursor, int count, int wrap, int step, int arg); // inferred: moves a cursor
extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int a, int b); // inferred: moves a list cursor
extern "C" void fn_804A4930(atn::Task* task, int top, int count, int page); // inferred: scroll bar
extern "C" void* lbl_8087EF70;
extern "C" void fn_8052DEEC(atn::Task* camp, int id); // inferred: changes the parts of the camp
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered

extern "C" NewCampPartsPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);

// inferred: d2anime task of the D3Anime objects; only the members used here
struct NewCampHintD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x100 - 0x58]; // 0x58: inferred: layout
    float mFrame;                        // 0x100
    float mSpeed;                        // 0x104
};
inline NewCampHintD2A_unk* GetD2A_unk(atn::D3Anime* anime) {
    return (NewCampHintD2A_unk*)anime->GetTask();
}


// inferred: functions of NewCampPartsBase (0x8050F93C-0x80512024) still called by their automatic names
extern "C" void fn_805113E8(atn::D3Anime* anime, int a, int b, float c, float d);
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int a, int b, float c, float d);

// inferred: camp task (the parent m48); only the members used here
struct NewCampHintCamp_unk {
    unsigned char m0000[0x6C];
    unsigned char m6C[0x11F8 - 0x6C];
    int m11F8;
};

// inferred: field (lbl_8087F430)
struct NewCampHintField_unk;
extern "C" NewCampHintField_unk* lbl_8087F430;
extern "C" int fn_80370174(NewCampHintField_unk* field, int id); // inferred: event flag
extern "C" void fn_803766E4(NewCampHintField_unk* field);
extern "C" void fn_80376324(NewCampHintField_unk* field, int id);
extern "C" int fn_8021F094();                      // inferred: number of tutorials
extern "C" NewCampTutorial_unk* fn_8021F0D4(int index); // inferred: tutorial entry

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4), entries of 8 bytes
struct NewCampHintMsg_unk {
    struct Entry_unk {
        int m00;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) {
        return mEntries[id].mText != 0 ? mEntries[id].mText : lbl_808813D0;
    }
    Entry_unk mEntries[1];
};
extern "C" NewCampHintMsg_unk* lbl_8087F1E4;

// inferred: category of the list (kind of tutorial and message id)
struct NewCampHintCategory_unk {
    int mKind;
    int mMsg;
};

// inferred: the empty name of the table is the "" at 0x80887838 (.sdata2, inside the split of
// game/NewCampPartsGmk.cpp)
extern "C" const char lbl_80887838[];

static const NewCampPartsFile_unk sFiles[12] = {
    {"shop_board01_0001", "hint_logo.d2a", 0},
    {"item_board01_0001", lbl_80887838, 0},
    {"shop_board03_1001", lbl_80887838, 0},
    {"protector_in_board18_1001", lbl_80887838, 0},
    {"protector_in_board18_1002", lbl_80887838, 0},
    {"shop_board04_1001", "item_menu.d2a", 0},
    {"shop_board04_1002", "item_menu.d2a", 0},
    {"shop_board04_1003", "item_menu.d2a", 0},
    {"shop_board04_1004", "item_menu.d2a", 0},
    {"shop_board04_1005", "item_menu.d2a", 0},
    {"shop_board04_1006", "item_menu.d2a", 0},
    {"shop_board04_1007", "item_menu_all.d2a", 0},
};
static const char* const sBoard = "hint_board";
static const NewCampHintCategory_unk sCategories[7] = {
    {1, 0x16F}, {2, 0x170}, {3, 0x171}, {4, 0x172}, {5, 0x174}, {6, 0x173}, {-1, 0x16E},
};
// inferred: target values of mOpen (0.0f and 1.0f). They are in .sdata, where MWCC does not put a
// variable initialized to zero (it goes to .sbss), so they stay in the automatic split.
extern "C" float lbl_8087E480;
extern "C" float lbl_8087E484;

inline NewCampPartsPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
// inferred: set a text/value of the pane `name` (names invented)
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

NewCampPartsHint* NewCampPartsHint::fn_80514720(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsHint(parent);
    }
    return 0;
}

NewCampPartsHint::NewCampPartsHint(atn::Task* parent)
    : NewCampPartsBase(parent), mMode(0), mD8(0), mE0(0), mCategory(0), mCursor(0), mTop(0), mBase(0), mNoItem(0),
      mScroll(0), m128(0), mOpen(1.0f), mMove(292.0f, 0.0f, 0.0f), mOpenOffset(-150.0f, 0.0f, -100.0f) {
    m98 = "hint_board";
    m4C = 12;
    if (m4C > 0) {
        m50.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[m4C]);
        fn_8050FB38(m4C, &m50[0], sFiles, 0, 0);
        fn_8050FC88(m4C, &m50[0], 1);
    }
    (mBase = atn::D3Anime::Create(this, "/data/d2anime/item_list_base.d2a", 1))->fn_800D246C(1);
    (mNoItem = atn::D3Anime::Create(this, "/data/d2anime/shop_text_noitem.d2a", 1))->fn_800D246C(1);
    for (int i = 0; i < 10; i++) {
        mItems[i] = atn::D3AnimeInstance::Create(this, "/data/d2anime/hint_tutorial_list.d2a", 1);
        mItems[i]->fn_800D246C(1);
    }
    (mScroll = atn::D3AnimeInstance::Create(this, "scroll_bar.d2a", 1))->fn_800D246C(1);
    m5C.vf0C("camera/ui/shop_camera.xsca");
    mTutorialCol = NewCampColor_unk(0.21568628f, 0.53333336f, 1.0f, 1.0f);
    mTutorialShadow = NewCampColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
    mTipsCol = NewCampColor_unk(0.9372549f, 0.8f, 0.27058825f, 1.0f);
    mTipsShadow = NewCampColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
    mA4 = atn::Vector3(20.0f, -10.0f, -75.0f);
    mTutorial = 0;
    mWait = 0;
}

NewCampPartsHint::~NewCampPartsHint() {}

int NewCampPartsHint::vf4C() {
    return 0;
}

void NewCampPartsHint::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(scene, m50.mData, sFiles, m4C, sBoard);
    m58 = -1.0f;
    mBase->fn_800D246C(0);
    mNoItem->fn_800D246C(0);
    for (int i = 0; i < 10; i++) {
        mItems[i]->fn_800D246C(0);
    }
    mScroll->fn_800D246C(0);
    m80 = 0;
    m84 = 0;
    m88 = 0;
    m8C = 0;
    mCategory = 0;
    vf3C(0);
}

void NewCampPartsHint::vf10() {
    vf20();
    if (fn_8050FD20()) {
        mOpen = 0.2f * (lbl_8087E480 - mOpen) + mOpen;
    } else {
        mOpen = 0.2f * (lbl_8087E484 - mOpen) + mOpen;
    }
}

// NONMATCHING: 99.46% - register allocation only (matrix address/pool, the category flag and the item
// pointer get other saved registers)
void NewCampPartsHint::vf14() {
    int show = 0;
    if (fn_8050FD20() && ((NewCampHintCamp_unk*)m48)->m11F8 != 1) {
        show = 1;
    }
    vf3C(show);
    atn::Vector3 offset = mMove * 0.0f;
    offset += mOpenOffset * mOpen;
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        vf60(entry, 0, 7, 0.0f);
        atn::Matrix3x4 mtx = GetModel_unk(entry->mModel)->mMtx;
        atn::Vector3 pos = mtx.GetTrans_unk() + offset;
        mtx.m[0][3] = pos.x;
        mtx.m[1][3] = pos.y;
        mtx.m[2][3] = pos.z;
        GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
        vf58(entry, "reference");
        vf54(entry, "reference");
        vf5C(entry);
    }
    char name[0x40];
    NewCampPartsPoint_unk pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.m08 = 0.0f;
    pos.m0C = 0.0f;
    pos.m10 = 0.0f;
    NewCampPartsEntry_unk* cat = &m50[mCategory + 5];
    for (int i = 0; i < 7; i++) {
        int index = i + 5;
        NewCampPartsEntry_unk* entries = m50.mData;
        if (entries[index].mAnime->GetTask() != 0) {
            if (mCategory == i) {
                GetD2A_unk(entries[index].mAnime)->mFrame = 11.0f;
                if (show) {
                    int mode = mMode != 0;
                    sprintf(name, "cursor_point");
                    pos = GetPanePos_unk(cat->mAnime->GetTask(), name);
                    fn_805118B4(cat->mAnime->GetTask(), 0, cat, pos, mode);
                }
            } else {
                GetD2A_unk(entries[index].mAnime)->mFrame = 10.0f;
            }
        }
    }
    NewCampPartsEntry_unk* base = &m50[1];
    fn_80511258(base, mBase, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
    fn_80511258(base, mNoItem, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
    fn_805112A8(base, mScroll, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
    if (mBase->GetTask() != 0) {
        for (int i = 0; i < 10; i++) {
            if (mItems[i]->GetTask() != 0) {
                sprintf(name, "item_list%d", i + 1);
                pos = GetPanePos_unk(mBase->GetTask(), name);
                fn_801F6E78(mItems[i]->GetTask(), "base_point", &pos);
                fn_805112A8(base, mItems[i], atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
            }
        }
        int mode = mMode;
        if (mode == 1 || mode == 2) {
            int index = mCursor - mTop;
            sprintf(name, "item_cursor%d", index + 1);
            pos = GetPanePos_unk(mBase->GetTask(), name);
            fn_805119C8(mItems[index]->GetTask(), 0, base, pos, mode != 1);
        }
        if (mScroll->GetTask() != 0) {
            sprintf(name, "scroll_point");
            pos = GetPanePos_unk(mBase->GetTask(), name);
            fn_801F6E78(mScroll->GetTask(), "basePos", &pos);
            fn_801F6C80(mScroll->GetTask(), "s_hightMax", 245.0f);
            fn_804A4930(mScroll->GetTask(), mTop, mList.size(), 10);
        }
    }
}

// inferred: rebuilds the list of the tutorials of the current category
inline void NewCampPartsHint::ReadList_unk() {
    int kind = sCategories[mCategory].mKind;
    mList.clear();
    int count = fn_8021F094();
    for (int i = 0; i < count; i++) {
        NewCampTutorial_unk* tutorial = fn_8021F0D4(i);
        if (tutorial != 0 && fn_80370174(lbl_8087F430, tutorial->mFlag) > 0 && tutorial->mCategory < 7 &&
            (kind < 0 || tutorial->mCategory == kind)) {
            mList.push_back(tutorial);
        }
    }
    fn_80516544();
}

// NONMATCHING: 99.52% - register allocation of the category loop, and the inlined push_back (fast path
// loads the capacity before the size, see std/vector.h; the allocation message is a weak @STRING@ here)
void NewCampPartsHint::vf20() {
    if (!m96) {
        return;
    }
    int ready = 1;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].mAnime->mState == 1) {
            ready = 0;
            break;
        }
    }
    if (mBase->mState == 1) {
        ready = 0;
    }
    if (mNoItem->mState == 1) {
        ready = 0;
    }
    for (int i = 0; i < 10; i++) {
        if (mItems[i]->mState == 1) {
            ready = 0;
        }
    }
    if (!ready) {
        return;
    }
    for (int i = 0; i < m4C; i++) {
        fn_805113E8(m50[i].mAnime, 1, 0, 0.0f, 0.0f);
    }
    fn_805113E8(mBase, 1, 0, 0.0f, 1.0f);
    fn_805113E8(mNoItem, 0, 0, 0.0f, 1.0f);
    for (int i = 0; i < 10; i++) {
        fn_805114D4(mItems[i], 0, 0, 0.0f, 1.0f);
    }
    fn_805114D4(mScroll, 1, 0, 0.0f, 1.0f);
    for (int i = 0; i < 7; i++) {
        int index = i + 5;
        NewCampPartsEntry_unk* entries = m50.mData;
        if (entries[index].mAnime->GetTask() != 0) {
            const wchar_t* text = lbl_8087F1E4->Get_unk(sCategories[i].mMsg);
            SetPaneText_unk(GetD2A_unk(entries[index].mAnime)->mLayout, "name", text);
            GetD2A_unk(entries[index].mAnime)->mSpeed = 0.0f;
            GetD2A_unk(entries[index].mAnime)->mFrame = 10.0f;
        }
    }
    if (mNoItem->GetTask() != 0) {
        const wchar_t* text = fn_80116FC0(0, 0x2742);
        SetPaneText_unk(GetD2A_unk(mNoItem)->mLayout, "message", text);
    }
    if (fn_8050FD20()) {
        vf3C(0);
    }
    ReadList_unk();
    vf48();
    m96 = 0;
}

// NONMATCHING: 96.90% - register allocation only (entry address and loop offset swapped); tried
// !show/show == 0/a local, a reference and pointer forms
void NewCampPartsHint::vf3C(int show) {
    for (int i = 0; i < m4C; i++) {
        if (m50[i].m3C) {
            if (m50[i].mAnime != 0) {
                m50[i].mAnime->mVisible = show;
            }
            if (m50[i].mModel != 0) {
                m50[i].mModel->mHidden = show == 0;
            }
        }
    }
}

void NewCampPartsHint::vf24() {
    mCursor = 0;
    mTop = 0;
    mCategory = 0;
    fn_80515F28(0);
    if (mTutorial != 0) {
        fn_800D2338(mTutorial);
        mTutorial = 0;
    }
    mWait = 0;
    m128 = 0;
    m96 = 1;
}

void NewCampPartsHint::vf28() {
    m80 = 0;
    mD8 = 0;
    m58 = -1.0f;
    m95 = 0;
    mOpen = 1.0f;
    if (mTutorial != 0) {
        fn_800D2338(mTutorial);
        mTutorial = 0;
    }
    mWait = 0;
    if (m128 != 0) {
        fn_803766E4(lbl_8087F430);
    }
    m128 = 0;
    m96 = 0;
}

// NONMATCHING: 99.74% - the inlined push_back (see vf20) and the registers of the size/category loads
void NewCampPartsHint::vf30() {
    mD8 = 0;
    mE0 = 0;
    m80 = 0;
    m84 = 0;
    mMode = 0;
    mOpen = 1.0f;
    m95 = 1;
    mCursor = 0;
    mTop = 0;
    mCategory = 0;
    if (mTutorial != 0) {
        fn_800D2338(mTutorial);
        mTutorial = 0;
    }
    mWait = 0;
    ReadList_unk();
}

void NewCampPartsHint::vf2C() {
    m58 = 1.0f;
    m95 = 0;
    switch (mMode) {
    case 0:
        fn_80516004();
        break;
    case 1:
        fn_8051640C();
        break;
    case 2:
        if (mTutorial == 0) {
            if (--mWait <= 0) {
                fn_80515F28(1);
            }
        } else if (mTutorial->IsFlag_unk(2) && mTutorial->mState == 0) {
            mTutorial->fn_805BA354((const Camera_unk*)((NewCampHintCamp_unk*)m48)->m6C);
        } else if (mTutorial->mState == 4) {
            fn_800D2338(mTutorial);
            mTutorial = 0;
            mWait = 15;
        }
        break;
    }
}

void NewCampPartsHint::fn_80515F28(int mode) {
    mMode = mode;
    switch (mode) {
    case 0:
        fn_80516544();
        break;
    case 1:
        fn_80516544();
        break;
    case 2:
        fn_80516544();
        if (mCursor < (int)mList.size()) {
            NewCampTutorial_unk* tutorial = mList[mCursor];
            if (tutorial->mSeen != 0) {
                m128 = 1;
                if (lbl_8087F430 != 0) {
                    fn_803766E4(lbl_8087F430);
                    fn_80376324(lbl_8087F430, mList[mCursor]->mFlag);
                }
                fn_80515F28(1);
            } else {
                mTutorial = TutorialTask::fn_805B9F50(this, tutorial->mFlag, 0);
            }
        }
        break;
    }
}

// NONMATCHING: 96.94% - the inlined push_back (see vf20); the saved registers of the whole function are
// assigned in another order
void NewCampPartsHint::fn_80516004() {
    int category = mCategory;
    fn_804A436C(&mCategory, 7, 7, 1, 4);
    if (category != mCategory) {
        mTop = 0;
        mCursor = 0;
        ReadList_unk();
    }
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(8);
        fn_8052DEEC(m48, 0);
        m58 = -1.0f;
    } else if (fn_800A555C(pad, 0, 4)) {
        if (mList.size() == 0) {
            fn_80117228(0xB);
        } else {
            fn_80117228(1);
            fn_80515F28(1);
        }
    }
}

void NewCampPartsHint::fn_8051640C() {
    void* pad = lbl_8087EF70;
    int cursor = mCursor;
    fn_804A4494(&mCursor, &mTop, mList.size(), 10, 1, 3);
    if (cursor != mCursor) {
        fn_80516544();
    }
    if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        fn_80515F28(0);
    } else if (fn_800A555C(pad, 0, 4) && mCursor < (int)mList.size()) {
        if (fn_80370174(lbl_8087F430, mList[mCursor]->mFlag) >= 1) {
            fn_80117228(1);
            fn_80515F28(2);
        } else {
            fn_80117228(0xB);
        }
    }
}

void NewCampPartsHint::fn_80516544() {
    for (int i = 0; i < 10; i++) {
        atn::Task* task = mItems[i]->GetTask();
        if (task != 0) {
            int visible = 0;
            int index = mTop + i;
            NewCampTutorial_unk* tutorial;
            if (index < (int)mList.size() && (tutorial = mList[index]) != 0 &&
                fn_80370174(lbl_8087F430, tutorial->mFlag) >= 1) {
                fn_801F837C(task, "item", tutorial->mName);
                if (tutorial->mSeen != 0) {
                    fn_801F7590(task, "iconUvMask", NewCampColor_unk(0.5f, 0.0f, 1.0f, 1.0f));
                    fn_801F7DF0(task, "iconCol", 200.0f, 230.0f, 255.0f);
                } else {
                    fn_801F7590(task, "iconUvMask", NewCampColor_unk(0.0f, 0.0f, 0.5f, 1.0f));
                    fn_801F7DF0(task, "iconCol", 244.0f, 233.0f, 192.0f);
                }
                if (mMode == 1 && index == mCursor) {
                    fn_801F791C(task, "pushCol", 0, 255.0f);
                } else {
                    fn_801F791C(task, "pushCol", 0, 0.0f);
                }
                visible = 1;
            }
            mItems[i]->mVisible = visible;
        }
    }
    if (mList.size() != 0) {
        mNoItem->mVisible = 0;
    } else {
        mNoItem->mVisible = 1;
    }
}

void NewCampPartsHint::vf40(void* group) {
    group = fn_8008937C(group, m98.data());
    fn_80087E9C(group, "MoveOffset", &mMove, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80088FAC(group, "Tutorial", &mTutorialCol, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_80088FAC(group, "TutorialShadow", &mTutorialShadow, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_80088FAC(group, "Tips", &mTipsCol, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_80088FAC(group, "TipsShadow", &mTipsShadow, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_80087E9C(group, "OpenOffset", &mOpenOffset, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetPos", &mA4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetScl", &mB0, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "LocatorScl", &mBC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BasePos", &mC8, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
}

void NewCampPartsHint::vf48() {
    float frame = 0.0f;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
            SetPaneFloat_unk(GetD2A_unk(m50[i].mAnime)->mLayout, "frameOn", frame);
        }
    }
    if (mBase->GetTask() != 0) {
        SetPaneFloat_unk(GetD2A_unk(mBase)->mLayout, "frameOn", frame);
    }
}
