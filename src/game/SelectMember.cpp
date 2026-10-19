#include "game/SelectMember.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "macros.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" SelectMemberAnim_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: d2anime task
extern "C" SelectMemberAnim_unk* fn_801F64D0(atn::Task* parent, const char* path); // inferred: d2anime list item
extern "C" void fn_801FEF40(void* layout, int arg);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
// inferred: plays a system sound effect
extern "C" atn::SoundRef fn_80117228(int id);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
// inferred: pad input (lbl_8087EF70)
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void fn_804A436C(int* cursor, int count, int wrap, int step, int arg); // inferred: moves a cursor
// inferred: 2D point of a d2anime pane (0x14 bytes here); names invented
struct SelectMemberPoint_unk {
    SelectMemberPoint_unk() {}
    float x;
    float y;
    float m08;
    float m0C;
    float m10;
};
extern "C" SelectMemberPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const SelectMemberPoint_unk* pos);
extern "C" void* lbl_8087F580; // inferred: menu cursor
extern "C" void fn_804A53D4(void* cursor, int a, int b, const SelectMemberPoint_unk* pos, int c);
extern "C" void fn_801F4CB4(atn::Task* task, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801F4AA0(atn::Task* task, const char* name, float r, float g, float b); // inferred: color
extern "C" void fn_801F837C(atn::Task* task, const char* name, int text); // inferred: sets a text id
extern "C" void fn_801F8598(atn::Task* task, const char* name, int value, int arg);
extern "C" void fn_801F6C80(atn::Task* task, const char* name, float value);
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4); name invented
struct SelectMemberMsg_unk {
    const wchar_t* Get_unk(int id) {
        if (mText[id] != 0) {
            return mText[id];
        }
        return lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" SelectMemberMsg_unk* lbl_8087F1E4;
// inferred: field (lbl_8087F430)
struct SelectMemberField_unk {
    unsigned char m0000[0x10D0];
    int m10D0; // inferred: story progress
};
extern "C" SelectMemberField_unk* lbl_8087F430;
extern "C" int fn_80370174(SelectMemberField_unk* field, int id); // inferred: reads a field value
// inferred: members available at a point of the story
struct SelectMemberAvail_unk {
    unsigned char m00[4];
    unsigned char m04[7];
};
extern "C" SelectMemberAvail_unk* fn_80217D9C(int progress);
extern "C" unsigned char* fn_80216544(int type, int progress, int value);
struct SelectMemberRule_unk {
    unsigned char m00[0x54];
    int m54; // inferred: number of members
};
extern "C" SelectMemberRule_unk* fn_8021771C(int a, int b, int c);
// inferred: party data (lbl_8087F4F0); one 0x43C-byte block per member
struct SelectMemberParty_unk {
    unsigned char m0000[0x64A0];
    int mOrder[7]; // 0x64A0: inferred: order of the members
};
extern "C" SelectMemberParty_unk* lbl_8087F4F0;
// inferred: character manager (lbl_8087F8A0)
extern "C" void* lbl_8087F8A0;
struct SelectMemberChara_unk {
    unsigned char m000[0x7D4];
    CharaStatus mStatus; // 0x7D4
};
extern "C" SelectMemberChara_unk* fn_8054A33C(void* manager, int id); // inferred: character of a member
extern "C" int fn_80219544(int name); // inferred: character id of a name id
struct SelectMemberInfo_unk {
    unsigned char m00[8];
    int m08; // inferred: name text id
};
extern "C" SelectMemberInfo_unk* fn_8020924C(int id);
CharaParam_unk fn_80134800(CharaStatus* status, int id, int kindA, int indexA, int itemA, int kindB, int indexB,
                           int itemB);

// inferred: save menu (game/CampSaveLoad.cpp)
class MenuSaveTask {
public:
    static MenuSaveTask* fn_8011770C(atn::Task* parent);
    static MenuSaveTask* sInstance;

    unsigned char m000[0x13C];
    int m13C; // inferred: state (7: finished)
    unsigned char m140[0x4B0 - 0x140];
    int m4B0;
};

// inferred: position of the pane `name` (name invented)
static inline SelectMemberPoint_unk __GetPos_unk(atn::Task* anim, const char* name) {
    return fn_801F4E8C(anim, fn_800DC6B4(name));
}

// inferred: sets the text of the pane `name` (name invented)
static inline void __SetText_unk(SelectMemberAnim_unk* anim, const char* name, const wchar_t* text) {
    fn_801FEE08(anim->GetLayout_unk(), fn_800DC6B4(name), text);
}

SelectMember* SelectMember::fn_8057162C(atn::Task* parent, int type, unsigned char save) {
    if (parent != 0) {
        return new (1, "", "", 0) SelectMember(parent, type, save);
    }
    return 0;
}

// NONMATCHING: 99.6% - the list path and the slot offset swap r30/r31.
SelectMember::SelectMember(atn::Task* parent, int type, unsigned char save)
    : atn::Task(parent), mCursor(0), mDecided(0), mSave(save), mType(type), mCursor2(0), mC8(0) {
    (mAnims[0] = fn_801F3FF8(this, "/data/d2anime/member_select_base.d2a", 0))->fn_800D246C(1);
    const char* list = "/data/d2anime/member_select_list.d2a";
    for (int i = 0; i < 7; i++) {
        (mAnims[1 + i] = fn_801F64D0(this, list))->fn_800D246C(1);
    }
    (mAnims[8] = fn_801F3FF8(this, "/data/d2anime/member_select_info.d2a", 0))->fn_800D246C(1);
    mMax = 0;
    mState = 0;
}

SelectMember::~SelectMember() {}

int SelectMember::vf0C() {
    if (fn_800D3FA4() != 0) {
        mAnims[0]->fn_800D246C(0);
        mAnims[0]->mVisible_unk = 0;
        mAnims[0]->mFlags |= 4;
        fn_801FEF40(mAnims[0]->m58, 0);
        for (int i = 0; i < 7; i++) {
            mAnims[1 + i]->fn_800D246C(0);
            mAnims[1 + i]->m4D = 0;
            mAnims[1 + i]->mFlags |= 4;
        }
        mAnims[8]->fn_800D246C(0);
        mAnims[8]->mVisible_unk = 0;
        mAnims[8]->mFlags |= 4;
        fn_80571C64(0);
        fn_80117228(30);
        return 1;
    }
    return 0;
}

void SelectMember::vf10() {
    switch (mState) {
    case 0:
        fn_80572290();
        fn_80571C64(2);
        break;
    case 2:
        fn_80571DB8();
        break;
    case 3:
        fn_80571FD8();
        break;
    case 4:
        fn_80572194();
        break;
    case 5:
        if (MenuSaveTask::sInstance != 0 && MenuSaveTask::sInstance->m13C == 7) {
            fn_800D2338((atn::Task*)MenuSaveTask::sInstance);
            fn_80571C64(1);
        }
        break;
    }
}

void SelectMember::vf14() {
    for (int i = 0; i < 9; i++) {
        mAnims[i]->mFlags |= 4;
    }
    switch (mState) {
    case 0:
        fn_80572450();
        break;
    case 2: {
        fn_80572450();
        char name[64];
        if (mCursor >= (int)mMembers.mCount) {
            sprintf(name, "end_cursor");
        } else {
            sprintf(name, "cursur_point%d", mCursor);
        }
        SelectMemberPoint_unk pos = __GetPos_unk(mAnims[0], name);
        fn_804A53D4(lbl_8087F580, 0, 0, &pos, 0);
        break;
    }
    case 3: {
        fn_80572450();
        mAnims[8]->mFlags &= ~4;
        char name[64];
        sprintf(name, "cursor_point%d", mCursor2 + 1);
        SelectMemberPoint_unk pos = __GetPos_unk(mAnims[8], name);
        fn_804A53D4(lbl_8087F580, 0, 0, &pos, 0);
        break;
    }
    case 4: {
        fn_80572450();
        mAnims[8]->mFlags &= ~4;
        char name[64];
        sprintf(name, "cursor_point%d", mCursor2 + 1);
        SelectMemberPoint_unk pos = __GetPos_unk(mAnims[8], name);
        fn_804A53D4(lbl_8087F580, 0, 0, &pos, 0);
        break;
    }
    }
}

// NONMATCHING: 96.7% - `this` takes r30 instead of r31 (and the texts of cases 3b/4 r31 instead of r30).
void SelectMember::fn_80571C64(int state) {
    mPrevState = mState;
    mState = state;
    switch (state) {
    case 3:
        mCursor2 = 0;
        if (mType == 0x13A) {
            __SetText_unk(mAnims[8], "check_msg", fn_80116FC0(0, 0x8954D8));
        } else {
            __SetText_unk(mAnims[8], "check_msg", lbl_8087F1E4->Get_unk(881));
        }
        break;
    case 4:
        mCursor2 = 0;
        __SetText_unk(mAnims[8], "check_msg", lbl_8087F1E4->Get_unk(883));
        break;
    case 5:
        MenuSaveTask::fn_8011770C(this);
        if (MenuSaveTask::sInstance != 0) {
            MenuSaveTask::sInstance->m4B0 = 1;
        }
        break;
    }
}

void SelectMember::fn_80571DB8() {
    void* pad = lbl_8087EF70;
    fn_804A436C(&mCursor, mMembers.mCount + 1, mMembers.mCount + 1, 1, 3);
    if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80571C64(1);
        fn_80117228(2);
        return;
    }
    if (fn_800A555C(pad, 0, 4) == 0) {
        return;
    }
    int n = CountSelected_unk();
    if (mCursor >= (int)mMembers.mCount) {
        if (mMax <= n || (int)mMembers.mCount <= n) {
            fn_80571C64(3);
            fn_80117228(1);
            mAnims[8]->m104 = 1.0f;
        } else {
            fn_80117228(11);
        }
        return;
    }
    Member_unk& member = mMembers.mData[mCursor];
    if (member.mFixed != 0) {
        fn_80117228(11);
    } else if (member.mSelected == 0) {
        if (mMax <= n || (int)mMembers.mCount <= n) {
            fn_80117228(11);
        } else {
            member.mSelected = 1;
            fn_80117228(1);
            if (mMax <= n + 1 || (int)mMembers.mCount <= n + 1) {
                mCursor = mMembers.mCount;
                mCursor2 = mMembers.mCount;
            }
        }
    } else {
        member.mSelected = 0;
        fn_80117228(1);
    }
}

// NONMATCHING: 99.3% - registers of the order loop (front/back/member pointer).
void SelectMember::fn_80571FD8() {
    void* pad = lbl_8087EF70;
    fn_804A436C(&mCursor2, 2, 2, 1, 3);
    if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80571C64(2);
        fn_80117228(2);
        mAnims[0]->m104 = 1.0f;
        return;
    }
    if (fn_800A555C(pad, 0, 4) == 0) {
        return;
    }
    if (mCursor2 == 0) {
        int front = 0;
        int back = 6;
        for (unsigned int i = 0; i < 7; i++) {
            bool selected = false;
            for (unsigned int j = 0; j < mMembers.mCount; j++) {
                if (mMembers.mData[j].mId == i && mMembers.mData[j].mSelected != 0) {
                    selected = true;
                    break;
                }
            }
            if (selected) {
                lbl_8087F4F0->mOrder[front++] = i;
            } else {
                lbl_8087F4F0->mOrder[back--] = i;
            }
        }
        mDecided = 1;
        if (mSave != 0) {
            fn_80571C64(4);
        } else {
            fn_80571C64(1);
        }
        fn_80117228(1);
    } else {
        fn_80571C64(2);
        fn_80117228(2);
        mAnims[0]->m104 = 1.0f;
    }
}

void SelectMember::fn_80572194() {
    void* pad = lbl_8087EF70;
    fn_804A436C(&mCursor2, 2, 2, 1, 3);
    if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80571C64(1);
        fn_80117228(2);
        return;
    }
    if (fn_800A555C(pad, 0, 4) == 0) {
        return;
    }
    if (mCursor2 == 0) {
        fn_80571C64(5);
        fn_80117228(1);
    } else {
        fn_80571C64(1);
        fn_80117228(2);
    }
}

void SelectMember::fn_80572290() {
    mMembers.mCount = 0;
    mMax = 1;
    int progress = 0;
    int value = 0;
    if (lbl_8087F430 != 0) {
        progress = lbl_8087F430->m10D0;
        value = fn_80370174(lbl_8087F430, 99);
    }
    SelectMemberAvail_unk* avail = fn_80217D9C(progress);
    if (avail != 0) {
        mMembers.PushBack_unk(Member_unk(0, 1, 1));
        for (int i = 1; i < 7; i++) {
            if (avail->m04[i] != 0) {
                mMembers.PushBack_unk(Member_unk(i, 0, 0));
            }
        }
    }
    unsigned char* key = fn_80216544(mType, progress, value);
    if (key != 0) {
        SelectMemberRule_unk* rule = fn_8021771C(key[0], key[1], key[2]);
        if (rule != 0) {
            mMax = rule->m54;
        }
    }
}

// NONMATCHING: 95.8% - the original indexes the members with this + 8 * i (no strength reduction to
// a pointer), so one more saved register is used and the others shift.
void SelectMember::fn_80572450() {
    mAnims[0]->mFlags &= ~4;
    int n = CountSelected_unk();
    fn_801F4CB4(mAnims[0], "memberNum(1)", n, 0);
    fn_801F4CB4(mAnims[0], "memberMax(1)", mMax, 0);
    if (n >= mMax) {
        fn_801F4AA0(mAnims[0], "memberNumCol", 20.0f, 100.0f, 200.0f);
        mAnims[0]->m100 = 10.0f;
    } else {
        fn_801F4AA0(mAnims[0], "memberNumCol", 255.0f, 255.0f, 255.0f);
        mAnims[0]->m100 = 9.0f;
    }
    SelectMemberPoint_unk pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.m08 = 0.0f;
    pos.m0C = 0.0f;
    pos.m10 = 0.0f;
    for (int i = 0; i < 7; i++) {
        if (i >= (int)mMembers.mCount) {
            continue;
        }
        char name[64];
        sprintf(name, "member%02d", i);
        pos = __GetPos_unk(mAnims[0], name);
        fn_801F6E78(mAnims[1 + i], "base_pos", &pos);
        Member_unk* member = &mMembers.mData[i];
        SelectMemberInfo_unk* info = fn_8020924C(fn_80219544(member->mId));
        mAnims[1 + i]->mFlags &= ~4;
        fn_801F837C(mAnims[1 + i], "name(1)", info->m08);
        CharaStatus* status;
        SelectMemberChara_unk* chara = fn_8054A33C(lbl_8087F8A0, member->mId);
        if (chara != 0) {
            status = &chara->mStatus;
        } else {
            status = (CharaStatus*)((char*)lbl_8087F4F0 + member->mId * 0x43C + 0x64EC);
        }
        CharaParam_unk param = fn_80134800(status, member->mId, 0, 0, 0, 0, 0, 0);
        fn_801F8598(mAnims[1 + i], "nowLv", param.m78, 0);
        float rate = (float)status->m1A8.mExp / MAX(1.0f, (float)(status->m1A8.mExp + status->fn_8012B028()));
        if (param.m78 >= 99) {
            rate = 0.0f;
        }
        fn_801F6C80(mAnims[1 + i], "expRate", rate);
        if (member->mSelected != 0) {
            mAnims[1 + i]->m54 = 2.0f;
        } else {
            mAnims[1 + i]->m54 = -2.0f;
        }
    }
}
