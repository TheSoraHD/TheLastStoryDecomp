#include "game/CosEditProtector.h"
#include "game/GameTask.h"
#include "game/NewCampTask.h"
#include "game/CostumeEditChara.h"
#include "game/CharaTask.h"
#include "game/CharaStatus.h"
#include "game/Inventory.h"
#include "game/MenuCommonTask.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "atn/D2A_Task.h"
#include "atn/D2A_TaskInstance.h"
#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "db/EquipParam_unk.h"
#include "game/MslSort_unk.h"
#include "macros.h"

// Third file of CosEditProtector (inferred file name, game/CosEditProtector.cpp and
// game/CosEditPalette_unk.cpp have the rest): the equipment lists of the costume menu (states 0, 2,
// 3, 11-13), their item lists and the std::sort copies of the lists.

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s);          // inferred: string hash
extern "C" void* lbl_8087EF70;                                // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" atn::SoundRef fn_80117228(int id);                 // inferred: plays a system sound effect
extern "C" const wchar_t* fn_80116FC0(int a, int id);         // inferred: system text
extern "C" GameTask* lbl_8087F430;                            // inferred: field
// inferred: not used by this file (only its __sinit is left; as game/CosEditPalette_unk.cpp)
static atn::Vector3 sHidePos(0.0f, -600.0f, 0.0f);

// inferred: item data (only the help text is read)
struct CosEditListItem_unk {
    unsigned char m00[0x0C];
    const wchar_t* mHelp; // 0x0C
};
extern "C" CosEditListItem_unk* fn_80211480(int id);    // inferred: item data
extern "C" int fn_80206BE4(void* item);                  // inferred: item id of a protector
extern "C" int fn_8020EF80(void* entry);                 // inferred: item id of an equipment entry
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id); // inferred: help text
extern "C" void fn_801E7D78(atn::D3Anime* anime, void* item, int arg); // inferred: shows a protector
extern "C" void fn_801E7EA8(atn::D3Anime* anime, void* entry, int arg); // inferred: shows an equipment

// inferred: state 0, the top page: four slots (two protectors, two equipments) and the costume
// page; A opens the list of the slot, B closes the menu
// NONMATCHING: 94.4% - the original tests the cases 0 and 1 of the A button one by one (no range test)
void CosEditProtector::fn_801D80BC() {
    switch (m1B98) {
    case 0:
    case 1: {
        int slot = m1B98 != 0;
        void* item = fn_801CFA5C(fn_801CF334()->mFace, slot);
        fn_801E7D78(mSelect, item, 1);
        CosEditListItem_unk* data = fn_80211480(fn_80206BE4(item));
        if (data != 0) {
            fn_804A3C24(MenuCommonTask::sInstance, data->mHelp, 0);
        }
        break;
    }
    case 2:
    case 3: {
        int slot;
        if (m1B98 == 2) {
            slot = 0;
        } else {
            slot = 1;
        }
        void* entry = fn_801CFAA8(fn_801CF334()->mFace, slot);
        fn_801E7EA8(mSelect, entry, 1);
        CosEditListItem_unk* data = fn_80211480(fn_8020EF80(entry));
        if (data != 0) {
            fn_804A3C24(MenuCommonTask::sInstance, data->mHelp, 0);
        }
        break;
    }
    default:
        fn_801E7EA8(mSelect, 0, 1);
        break;
    }
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        switch (m1B98) {
        case 4:
            if (lbl_8087F430->fn_80370174(0xC6) == 0) {
                lbl_8087F430->fn_80370320(0xC6, 1, 0);
            }
            fn_80117228(1);
            vf34(11);
            break;
        case 0:
        case 1: {
            int slot;
            if (m1B98 == 1) {
                slot = 1;
            } else {
                slot = 0;
            }
            int member = fn_801CF334()->mFace;
            void* item = fn_801CFA5C(member, slot);
            fn_801E07A4(member, slot);
            if (mB4C.size() != 0) {
                if (item == 0) {
                    fn_80117228(0xB);
                    if (lbl_8087F430 != 0) {
                        lbl_8087F430->fn_803750E4(0x159);
                    }
                } else {
                    fn_80117228(1);
                    fn_80117228(0xA);
                    m1E4 = slot;
                    vf34(3);
                }
            } else if (lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x395) &&
                       lbl_8087F430->fn_80370174(0x396)) {
                fn_80117228(0xB);
                MenuCommonTask::sInstance->fn_804A4294(fn_80116FC0(1, 0x111));
            } else {
                fn_80117228(0xB);
                if (lbl_8087F430 != 0) {
                    lbl_8087F430->fn_803750E4(0x159);
                }
            }
            break;
        }
        default: {
            CostumeEditChara* sel = fn_801CF334();
            Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[sel->mFace];
            if (m1B98 == 3 && fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                fn_80117228(0xB);
                break;
            }
            m1E4 = m1B98 != 2;
            fn_80117228(1);
            fn_80117228(0xA);
            vf34(2);
            break;
        }
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf34(1);
        if (lbl_8087EFE8) {
            lbl_8087EFE8->fn_800CFBA0(1, 0, 0x20, 30);
            lbl_8087EFE8->fn_800CFBA0(2, 0, 0x20, 15);
        }
    } else {
        int cursor = m80;
        fn_80510D64(1, 3);
        if (cursor != m80) {
            m1B98 = m80;
        }
    }
}

extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int a, int b); // inferred: moves a list cursor
extern "C" void fn_804A4300(MenuCommonTask* menu, const wchar_t* text);              // inferred: shows a message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...);                  // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2];                                            // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/CosEditPalette_unk.cpp)
struct CosEditListMsg_unk {
    struct Pair_unk {
        int m00;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    // inferred: text of an (id, text) pair of the table
    const wchar_t* GetPair_unk(int id) {
        Pair_unk* pairs = (Pair_unk*)mText;
        return pairs[id].mText != 0 ? pairs[id].mText : lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" CosEditListMsg_unk* lbl_8087F1E4;
// inferred: item data with its name
struct CosEditListName_unk {
    unsigned char m00[0x08];
    const wchar_t* mName; // 0x08
};
// inferred: character data (only the name and the face icon are read)
struct CosEditCharaInfo_unk {
    unsigned char m00[0x04];
    const wchar_t* mName; // 0x04
    unsigned char m08[0x2C - 0x08];
    int mIcon;            // 0x2C: inferred: 1-based, 4 per row
};
extern "C" CosEditCharaInfo_unk* fn_8020924C(int id); // inferred: character data
extern "C" int fn_80219558(int id);            // inferred: party member of a character id
// inferred: protector data (only the kind is read)
struct CosEditProtData_unk {
    unsigned char m00[0x80];
    int m80; // 0x80: inferred: equipment index given to fn_80134800
    unsigned char m84[0xB8 - 0x84];
    int mKind; // 0xB8
};
extern "C" CosEditProtData_unk* fn_80206C50(int id); // inferred: protector data of an item id

// inferred: state 2, the list of the equipments of the slot m1E4: A equips the item (taking it
// from the member who has it), B goes back, the two shoulder buttons sort the list
// NONMATCHING: 96.9% - registers and scheduling
void CosEditProtector::fn_801D8560() {
    if (mB34[m1E4].size() != 0 && m80 < (int)mB34[m1E4].size()) {
        CosEditItem_unk* e = &mB34[m1E4][m80];
        if (e != 0) {
            fn_801E7EA8(mSelect, e->mItem, 1);
            CosEditListItem_unk* data = fn_80211480(fn_8020EF80(e->mItem));
            if (data != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, data->mHelp, 0);
            }
        }
    }
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        if (mB34[m1E4].size() != 0 && m80 < (int)mB34[m1E4].size()) {
            CharaTask* chara = fn_801CF334()->mChara;
            int member = fn_801CF334()->mFace;
            CosEditItem_unk* e = 0;
            if (mB34[m1E4].size() != 0) {
                e = &mB34[m1E4][m80];
            }
            if (e == 0) {
                fn_80117228(0xB);
                return;
            }
            Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
            if (m1E4 == 1 && fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                MenuCommonTask::sInstance->fn_804A4294(lbl_8087F1E4->Get_unk(0x375));
                fn_80117228(0xB);
                return;
            }
            int ok;
            if (e->mOwner >= 0) {
                void* cur = fn_801CFAA8(member, m1E4);
                if (e->mEnabled == 0) {
                    fn_80117228(0xB);
                    CosEditCharaInfo_unk* info = fn_8020924C(e->mOwner);
                    const wchar_t* format = lbl_8087F1E4->Get_unk(0x225);
                    wchar_t text[0x100];
                    fn_800DD3FC(text, format, info->mName,
                                ((CosEditListName_unk*)fn_80211480(fn_8020EF80(cur)))->mName);
                    fn_804A4300(MenuCommonTask::sInstance, text);
                    return;
                }
                int other = fn_80219558(e->mOwner);
                Inventory::Equip_unk* own = &Inventory::instance_->mEquip[member];
                CharaTask* otherChara = (CharaTask*)((NewCampTask*)m48)->fn_80530384(other).m04;
                CosEditItem_unk back;
                back.Clear_unk();
                back.mItem = cur;
                back.mId = m1E4;
                back.mIndex = own->mProtectors[m1E4].mId;
                back.mOwner = -1;
                back.mEnabled = 1;
                if (fn_8020EF4C((EquipParamEntry_unk*)cur)) {
                    fn_801DC42C(other, otherChara, &back, 0, 0);
                    ok = fn_801DC42C(member, chara, e, 0, 0);
                } else {
                    ok = fn_801DC42C(member, chara, e, 0, 0);
                    fn_801DC42C(other, otherChara, &back, 0, 0);
                }
            } else {
                if (e->mEnabled == 0) {
                    fn_80117228(0xB);
                    MenuCommonTask::sInstance->fn_804A4294(lbl_8087F1E4->Get_unk(0x227));
                    return;
                }
                ok = fn_801DC42C(member, chara, e, 1, 1);
            }
            if (ok) {
                fn_801DD26C(member);
                fn_801E6EF0(fn_801CF334()->mFace, m1E4, &m80, &m88, 0);
                fn_801E6714();
            }
            fn_80117228(0xD);
        } else {
            fn_80117228(0xB);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        fn_80117228(0xA);
        vf34(0);
    } else if (fn_800A555C(pad, 0, 13) && mB34[m1E4].size() != 0) {
        fn_80117228(0x12);
        mB7C = 1;
        int id = -1;
        int owner = -1;
        if (mB34[m1E4].size() != 0) {
            CosEditItem_unk* e = &mB34[m1E4][m80];
            id = e->mIndex;
            owner = e->mOwner;
        }
        fn_801DD26C(fn_801CF334()->mFace);
        int top = m80 - m88;
        std::vector<CosEditItem_unk>& list = mB34[m1E4];
        for (int i = 0; i < (int)list.size(); i++) {
            CosEditItem_unk* e = &list[i];
            if (e->mOwner == owner && e->mIndex == id) {
                m80 = i;
                m88 = MAX(i - top, 0);
                if (mB34[m1E4].size() < 10) {
                    m88 = 0;
                }
                break;
            }
        }
        fn_801E6714();
    } else if (fn_800A555C(pad, 0, 12) && mB34[m1E4].size() != 0) {
        fn_80117228(0x12);
        mB7C = 0;
        int id = -1;
        int owner = -1;
        if (mB34[m1E4].size() != 0) {
            CosEditItem_unk* e = &mB34[m1E4][m80];
            id = e->mIndex;
            owner = e->mOwner;
        }
        fn_801DD26C(fn_801CF334()->mFace);
        int top = m80 - m88;
        std::vector<CosEditItem_unk>& list = mB34[m1E4];
        for (int i = 0; i < (int)list.size(); i++) {
            CosEditItem_unk* e = &list[i];
            if (e->mOwner == owner && e->mIndex == id) {
                m80 = i;
                m88 = MAX(i - top, 0);
                if (mB34[m1E4].size() < 10) {
                    m88 = 0;
                }
                break;
            }
        }
        fn_801E6714();
    } else {
        int cursor = m80;
        fn_804A4494(&m80, &m88, m84, m8C, 1, 3);
        if (cursor != m80) {
            fn_801E6714();
        }
    }
}

// inferred: state 3, the list of the protectors of the slot m1E4 (as fn_801D8560)
// NONMATCHING: 98.7% - registers and scheduling
void CosEditProtector::fn_801D8CC4() {
    if (mB4C.size() != 0 && m80 < (int)mB4C.size()) {
        CosEditItem_unk* e = &mB4C[m80];
        void* item = fn_80206C50(e->mId);
        fn_801E7D78(mSelect, item, e->mIndex < 0);
        CosEditListItem_unk* data = fn_80211480(fn_80206BE4(item));
        if (data != 0) {
            fn_804A3C24(MenuCommonTask::sInstance, data->mHelp, 0);
        }
    }
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        if (fn_801D5420(fn_801CF334()->mFace, m1E4) == 0) {
            fn_80117228(0xB);
            MenuCommonTask::sInstance->fn_804A4294(fn_80116FC0(1, 0x111));
            return;
        }
        if (mB4C.size() != 0 && m80 < (int)mB4C.size()) {
            CharaTask* chara = fn_801CF334()->mChara;
            int member = fn_801CF334()->mFace;
            CosEditItem_unk* e = &mB4C[m80];
            int ok;
            if (e->mOwner >= 0) {
                void* cur = fn_801CFA5C(member, m1E4);
                if (e->mEnabled == 0) {
                    fn_80117228(0xB);
                    if (fn_801D5420(fn_80219558(e->mOwner), e->m14) == 0) {
                        MenuCommonTask::sInstance->fn_804A4294(lbl_8087F1E4->Get_unk(0x223));
                        return;
                    }
                    CosEditCharaInfo_unk* info = fn_8020924C(e->mOwner);
                    const wchar_t* format = lbl_8087F1E4->Get_unk(0x225);
                    wchar_t text[0x100];
                    fn_800DD3FC(text, format, info->mName,
                                ((CosEditListName_unk*)fn_80211480(fn_80206BE4(cur)))->mName);
                    fn_804A4300(MenuCommonTask::sInstance, text);
                    return;
                }
                int other = fn_80219558(e->mOwner);
                CharaTask* otherChara = (CharaTask*)((NewCampTask*)m48)->fn_80530384(other).m04;
                CosEditItem_unk back;
                back.mItem = cur;
                back.mId = fn_80206BE4(cur);
                back.mIndex = -1;
                back.mOwner = -1;
                back.mEnabled = 1;
                back.m14 = e->m14;
                fn_801CF3C0(other, otherChara, &back, e->m14, 0, 0);
                ok = fn_801CF3C0(member, chara, e, m1E4, 0, 0);
            } else {
                ok = fn_801CF3C0(member, chara, e, m1E4, 1, 1);
            }
            if (ok) {
                fn_801E07A4(member, m1E4);
                fn_801E6FC8(member, m1E4, &m80, &m88, 0);
                fn_801E6B10();
            }
            fn_80117228(0xD);
        } else {
            fn_80117228(0xB);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        fn_80117228(0xA);
        vf34(0);
    } else if (fn_800A555C(pad, 0, 13) && mB4C.size() != 0) {
        fn_80117228(0x12);
        mB80 = 1;
        CosEditItem_unk sel;
        if (mB4C.size() != 0) {
            sel = mB4C[m80];
        }
        fn_801E07A4(fn_801CF334()->mFace, m1E4);
        fn_801E7098(&sel, m1E4, &m80, &m88);
        fn_801E6B10();
    } else if (fn_800A555C(pad, 0, 12) && mB4C.size() != 0) {
        fn_80117228(0x12);
        mB80 = 0;
        CosEditItem_unk sel;
        if (mB4C.size() != 0) {
            sel = mB4C[m80];
        }
        fn_801E07A4(fn_801CF334()->mFace, m1E4);
        fn_801E7098(&sel, m1E4, &m80, &m88);
        fn_801E6B10();
    } else {
        int cursor = m80;
        fn_804A4494(&m80, &m88, m84, m8C, 1, 3);
        if (cursor != m80) {
            fn_801E6B10();
        }
    }
}

// inferred: state 11, the best equipment page: A applies it (state 12) or takes everything off
// (state 13), B goes back
void CosEditProtector::fn_801D924C() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        switch (m1BBC) {
        case 1:
            fn_80117228(0x12);
            vf34(12);
            break;
        case 0:
            fn_80117228(0x12);
            vf34(13);
            break;
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf34(0);
    } else {
        int cursor = m80;
        fn_80510D64(1, 3);
        if (cursor != m80) {
            m1BBC = m80;
        }
    }
}

// inferred: state 12, takes every protector and equipment off the member
void CosEditProtector::fn_801D9388() {
    int member = fn_801CF334()->mFace;
    CharaTask* chara = fn_801CF334()->mChara;
    fn_801DD26C(member);
    if (fn_801D5420(member, 0)) {
        fn_801DCF60(member, chara, 0, 1);
    }
    if (member == 3) {
        fn_801DCF60(member, chara, 1, 1);
    }
    fn_801DC42C(member, chara, fn_801DCE88(member, 0, 1), 1, 1);
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    if (!fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
        fn_801DC42C(member, chara, fn_801DCE88(member, 1, 1), 1, 1);
    }
    vf34(0);
}

// inferred: entry of the item table (db/ItemParam_unk; only the item code is read)
struct CosEditListParam_unk {
    unsigned char m00[0x04];
    int mId; // 0x04
};
extern "C" int fn_802114D8();                          // inferred: number of entries of the item table
extern "C" CosEditListParam_unk* fn_802114E0(int index); // inferred: entry by index
extern "C" bool fn_8020F0AC(EquipParamEntry_unk* entry, int member); // inferred: whether the member can wear it
extern "C" int fn_804444E8(Inventory* inv, int id);                   // inferred: number of items
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);

typedef MslIter_unk<CosEditItem_unk> CosEditItemIter_unk;
// inferred: begin() and end() of the lists as iterators (weak copies 0x801DC410 and 0x801DC418)
inline CosEditItemIter_unk CosEditBegin_unk(std::vector<CosEditItem_unk>& list) {
    return CosEditItemIter_unk(list.begin());
}
inline CosEditItemIter_unk CosEditEnd_unk(std::vector<CosEditItem_unk>& list) {
    return CosEditItemIter_unk(list.end());
}

// inferred: predicates of the std::sort of the lists: by kind (out of line, below), for the members who wear
// armour (physical defence first) and for the others (magic defence first); the members' own pieces first,
// then by value and name
struct CosEditEquipLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const;
};
struct CosEditArmourLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const {
        if (a.mIndex == b.mIndex) {
            if (a.mOwner >= 0 && b.mOwner >= 0) {
                return a.mOwner < b.mOwner;
            }
            if (a.mOwner >= 0) {
                return true;
            }
            return false;
        }
        float va = ((EquipParamEntry_unk*)a.mItem)->m08;
        float vb = ((EquipParamEntry_unk*)b.mItem)->m08;
        if ((float)__fabs(va - vb) < 1.1920929e-7f) {
            return wcscmp(((CosEditListName_unk*)fn_80211480(fn_8020EF80(a.mItem)))->mName,
                          ((CosEditListName_unk*)fn_80211480(fn_8020EF80(b.mItem)))->mName) < 0;
        }
        return va > vb;
    }
};
struct CosEditRobeLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const {
        if (a.mIndex == b.mIndex) {
            if (a.mOwner >= 0 && b.mOwner >= 0) {
                return a.mOwner < b.mOwner;
            }
            if (a.mOwner >= 0) {
                return true;
            }
            return false;
        }
        float va = ((EquipParamEntry_unk*)a.mItem)->m0C;
        float vb = ((EquipParamEntry_unk*)b.mItem)->m0C;
        if ((float)__fabs(va - vb) < 1.1920929e-7f) {
            return wcscmp(((CosEditListName_unk*)fn_80211480(fn_8020EF80(a.mItem)))->mName,
                          ((CosEditListName_unk*)fn_80211480(fn_8020EF80(b.mItem)))->mName) < 0;
        }
        return va > vb;
    }
};

// inferred: inserts at the front of a member list (name invented)
inline void PushFront_unk(atn::FixedArray_unk<NewCampPair_unk*, 8>& list, NewCampPair_unk* pair) {
    NewCampPair_unk** pos = list.begin();
    unsigned int index = pos - list.mData;
    for (unsigned int i = list.mSize; i > index; i--) {
        list.mData[i] = list.mData[i - 1];
    }
    list.mData[index] = pair;
    list.mSize++;
}

// inferred: state 13, takes everything off the whole party one member at a time: first every weapon and
// protector goes back to the inventory and the members are queued (the selected one first); then, while
// some member has no free armour of its kind, the members get the free robes (m1C90, kinds 1/3/5) or
// armours (m1C9C) of the inventory, and the others get the preferred free pieces
void CosEditProtector::fn_801D94D0() {
    if (m1BC8 < 0) {
        m1C0C.clear();
        m1C30.clear();
        m1C54.clear();
        int count = ((NewCampTask*)m48)->m1224.size();
        for (int i = 0; i < count; i++) {
            NewCampPair_unk* pair = &((NewCampTask*)m48)->m1224[i];
            Inventory::Equip_unk* equip = &Inventory::Order()->mEquip[pair->mId];
            for (int slot = 0; slot < 2; slot++) {
                if (slot == 1 && pair->mId != 3) {
                    continue;
                }
                if (fn_801D5420(pair->mId, slot)) {
                    int id = fn_80206BE4(fn_801CFA5C(pair->mId, slot));
                    if (id > 0) {
                        Inventory::Order()->AddItem(id, 1, 1, 0, 0, 0, 0);
                        equip->mWeapons[slot].mId = -1;
                    }
                }
            }
            for (int slot = 0; slot < 2; slot++) {
                int id = fn_8020EF80(fn_801CFAA8(pair->mId, slot));
                if (id > 0) {
                    Inventory::Order()->AddItem(id, 1, 1, 0, 0, 0, 0);
                    equip->mProtectors[slot].mId = -1;
                    if (slot == 0) {
                        equip->mProtectors[2].mId = -1;
                    } else if (slot == 1) {
                        equip->mProtectors[3].mId = -1;
                    }
                }
            }
        }
        m1BC8 = fn_801CF334()->mFace;
        for (int i = 0; i < count; i++) {
            NewCampPair_unk* pair = &((NewCampTask*)m48)->m1224[i];
            if (pair != 0) {
                if (pair->mId == m1BC8) {
                    PushFront_unk(m1C0C, pair);
                } else {
                    m1C0C.push_back(pair);
                }
            }
        }
        return;
    }
    if (m1C0C.size() == 0) {
        for (unsigned int i = 0; i < m1C30.size(); i++) {
            NewCampPair_unk* pair = m1C30[i];
            int member = pair->mId;
            CharaTask* chara = (CharaTask*)pair->m04;
            if (fn_801D5420(member, 0)) {
                fn_801DCF60(member, chara, 0, 0);
            }
            if (member == 3) {
                fn_801DCF60(member, chara, 1, 0);
            }
            if (pair->mId == m1BC8) {
                PushFront_unk(m1C54, pair);
            } else {
                m1C54.push_back(pair);
            }
        }
        m1C54.clear();
        m1C30.clear();
        vf34(0);
        return;
    }
    if (m1BCC == 0) {
        m1C78.clear();
        m1C84.clear();
        m1C90.clear();
        m1C9C.clear();
        for (int k = 0; k < fn_802114D8(); k++) {
            CosEditListParam_unk* param = fn_802114E0(k);
            if (param == 0 || !fn_8020EF04(param->mId)) {
                continue;
            }
            EquipParamEntry_unk* entry = fn_8020EFEC(param->mId);
            if (entry == 0) {
                continue;
            }
            int num = fn_804444E8(Inventory::Order(), param->mId);
            if (num <= 0) {
                continue;
            }
            std::vector<CosEditItem_unk>* list = 0;
            if (entry->mKind == 0) {
                if (fn_8020EF4C(entry)) {
                    if (entry->mA8 == 8) {
                        list = &m1C9C;
                    } else {
                        list = &m1C90;
                    }
                } else {
                    list = &m1C78;
                }
            } else if (entry->mKind == 1) {
                list = &m1C84;
            }
            if (list == 0) {
                continue;
            }
            for (int j = 0; j < num; j++) {
                CosEditItem_unk item;
                item.mItem = entry;
                item.mId = entry->mKind;
                item.mIndex = entry->mId;
                item.mOwner = -1;
                item.mEnabled = 1;
                item.m14 = param->mId;
                list->push_back(item);
            }
        }
        if (m1C78.size() >= m1C0C.size() && m1C84.size() >= m1C0C.size()) {
            m1BCC = 1;
        } else {
            int armours = 0;
            int robes = 0;
            for (int i = m1C0C.size() - 1; i >= 0; i--) {
                int id = m1C0C[i]->mId;
                if (id == 1 || id == 3 || id == 5) {
                    robes++;
                } else {
                    armours++;
                }
            }
            NewCampPair_unk* found = 0;
            int pick = -1;
            int selected = -1;
            for (unsigned int i = 0; i < m1C0C.size(); i++) {
                if (m1BC8 == m1C0C[i]->mId) {
                    selected = i;
                    break;
                }
            }
            CosEditItem_unk* best = 0;
            if (m1C90.size() != 0) {
                MslSort_unk(CosEditBegin_unk(m1C90), CosEditEnd_unk(m1C90), CosEditArmourLess_unk());
                best = m1C90.begin();
                for (int i = m1C0C.size() - 1; i >= 0; i--) {
                    int id = m1C0C[i]->mId;
                    if (id == 1 || id == 3 || id == 5) {
                        if (m1BC8 != id || robes < 2) {
                            pick = i;
                            break;
                        }
                    }
                }
                if (pick >= 0 && selected >= 0 && pick != selected &&
                    (m1BC8 == 1 || m1BC8 == 3 || m1BC8 == 5)) {
                    fn_801DD26C(m1BC8);
                    CosEditItem_unk* cur = fn_801DCE88(m1BC8, 0, 0);
                    if (cur != 0 && cur->mIndex == best->mIndex) {
                        pick = selected;
                    }
                }
                if (pick >= 0 && pick < (int)m1C0C.size()) {
                    NewCampPair_unk** it = m1C0C.begin() + pick;
                    found = *it;
                    m1C0C.erase(it);
                }
            }
            if (found == 0 && m1C9C.size() != 0) {
                MslSort_unk(CosEditBegin_unk(m1C9C), CosEditEnd_unk(m1C9C), CosEditArmourLess_unk());
                best = m1C9C.begin();
                for (int i = m1C0C.size() - 1; i >= 0; i--) {
                    int id = m1C0C[i]->mId;
                    if (id == 0 || id == 2 || id == 4 || id == 6) {
                        if (m1BC8 != id || armours < 2) {
                            pick = i;
                            break;
                        }
                    }
                }
                if (pick >= 0 && selected >= 0 && pick != selected &&
                    (m1BC8 == 0 || m1BC8 == 2 || m1BC8 == 4 || m1BC8 == 6)) {
                    fn_801DD26C(m1BC8);
                    CosEditItem_unk* cur = fn_801DCE88(m1BC8, 0, 0);
                    if (cur != 0 && cur->mIndex == best->mIndex) {
                        pick = selected;
                    }
                }
                if (pick >= 0 && pick < (int)m1C0C.size()) {
                    NewCampPair_unk** it = m1C0C.begin() + pick;
                    found = *it;
                    m1C0C.erase(it);
                }
            }
            if (found != 0) {
                int member = found->mId;
                if (member == m1BC8) {
                    CharaTask* chara = (CharaTask*)found->m04;
                    if (fn_801D5420(member, 0)) {
                        fn_801DCF60(member, chara, 0, 0);
                    }
                    if (member == 3) {
                        fn_801DCF60(member, chara, 1, 0);
                    }
                    m1C54.push_back(found);
                } else {
                    m1C30.push_back(found);
                }
                fn_801DC42C(found->mId, (CharaTask*)found->m04, best, 0, 1);
            }
            return;
        }
    }
    NewCampPair_unk* pair = m1C0C[0];
    fn_801DD26C(pair->mId);
    int member = pair->mId;
    CharaTask* chara = (CharaTask*)pair->m04;
    if (fn_801D5420(member, 0)) {
        fn_801DCF60(member, chara, 0, 0);
    }
    if (member == 3) {
        fn_801DCF60(member, chara, 1, 0);
    }
    fn_801DC42C(member, chara, fn_801DCE88(member, 0, 0), 0, 1);
    if (!fn_8020EF4C(fn_8020ED84(0, Inventory::Order()->mEquip[member].mProtectors[0].mId))) {
        fn_801DC42C(member, chara, fn_801DCE88(member, 1, 0), 0, 1);
    }
    m1C54.push_back(pair);
    m1C0C.erase(m1C0C.begin());
}

extern "C" int fn_80219544(int member); // inferred: character id of a party member

// inferred: equipment entry (only the kind is read here)
struct CosEditEquip2_unk {
    unsigned char m00[0xA8];
    int mKind; // 0xA8
};
// inferred: equipment data (only the kind and the name are read)
struct CosEditEquip_unk {
    unsigned char m00[0x78];
    int mKind;            // 0x78: inferred: 1 and 3 use the second slot
    unsigned char m7C[0x80 - 0x7C];
    const wchar_t* mName; // 0x80
};
extern "C" void fn_8044441C(Inventory* inv, int id, int num); // inferred: removes items

// inferred: equips the equipment entry on the member (a: puts the old one back in the inventory, b:
// takes the new one out); a full-body piece also fills the second slot, and taking one off puts the
// preferred free piece in it. 0 when nothing changes
// NONMATCHING: 41% - the original calls fn_801DC8C8 out of line: over the inlining budget (R3); writing
// fn_801DD26C did not change it
int CosEditProtector::fn_801DC42C(int member, CharaTask* chara, CosEditItem_unk* e, int a, int b) {
    if (e == 0 || e->mIndex < 0) {
        return 0;
    }
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    int slot = 0;
    int kind = ((CosEditEquip_unk*)e->mItem)->mKind;
    if (kind == 1 || kind == 3) {
        slot = 1;
    }
    if (m1E8 == 2 && e->mIndex == equip->mProtectors[slot].mId) {
        return 0;
    }
    int wasFull = 0;
    if (slot == 0 && fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
        wasFull = 1;
    }
    int isFull = 0;
    if (slot == 0 && fn_8020EF4C(fn_8020ED84(0, e->mIndex))) {
        isFull = 1;
    }
    fn_801DC8C8(member, slot, e->mIndex);
    if (slot == 0) {
        fn_801DC8C8(member, 2, e->mIndex);
    } else if (slot == 1) {
        fn_801DC8C8(member, 3, e->mIndex);
    }
    if (isFull) {
        fn_801DC8C8(member, 1, e->mIndex);
        fn_801DC8C8(member, 3, e->mIndex);
    }
    if (a) {
        Inventory::instance_->AddItem(fn_8020EF80(fn_801CFAA8(member, slot)), 1, 1, 0, 0, 0, 0);
    }
    if (!wasFull && isFull) {
        Inventory::instance_->AddItem(fn_8020EF80(fn_801CFAA8(member, 1)), 1, 1, 0, 0, 0, 0);
    }
    int base = member * 4;
    Inventory::instance_->fn_8044D0DC(slot, e->mIndex, slot + base, member);
    if (slot == 0) {
        Inventory::instance_->fn_8044D0DC(2, e->mIndex, base + 2, member);
    } else if (slot == 1) {
        Inventory::instance_->fn_8044D0DC(3, e->mIndex, base + 3, member);
    }
    if (isFull) {
        Inventory::instance_->fn_8044D0DC(1, e->mIndex, base + 1, member);
        Inventory::instance_->fn_8044D0DC(3, e->mIndex, base + 3, member);
    }
    if (b) {
        fn_8044441C(Inventory::instance_, fn_8020EF80(fn_8020ED84(slot, e->mIndex)), 1);
    }
    CharaStatus* status = Inventory::instance_->GetPlayerStatusInfo(member);
    fn_801CEA98(member, chara, slot, e->mIndex, -1, 0, isFull ? 0 : status->fn_801347C8(slot), 0xE);
    if (slot == 0) {
        fn_801CEA98(member, chara, 2, e->mIndex, -1, 0, isFull ? 0 : status->fn_801347C8(2), 0xE);
    } else if (slot == 1) {
        fn_801CEA98(member, chara, 3, e->mIndex, -1, 0, isFull ? 0 : status->fn_801347C8(3), 0xE);
    }
    if (isFull) {
        fn_801CEA98(member, chara, 1, e->mIndex, -1, 0, 0, 0xE);
        fn_801CEA98(member, chara, 3, e->mIndex, -1, 0, isFull ? 0 : status->fn_801347C8(3), 0xE);
    }
    if (wasFull && !isFull) {
        fn_801DD26C(member);
        fn_801DC42C(member, chara, fn_801DCE88(member, 1, 0), 0, 1);
    }
    return 1;
}

extern "C" int* fn_8044D034(Inventory* inv, int index); // inferred: equipment record

// inferred: forgets the record of the equipment of the slot when the new item `id` is of another
// kind (or the slot was empty)
void CosEditProtector::fn_801DC8C8(int member, int slot, int id) {
    EquipParamEntry_unk* cur = fn_8020ED84(slot, Inventory::instance_->mEquip[member].mProtectors[slot].mId);
    EquipParamEntry_unk* next = fn_8020ED84(slot, id);
    int index = slot + member * 4;
    if (cur == 0) {
        int* record = fn_8044D034(Inventory::instance_, index);
        if (record != 0) {
            *record = -1;
        }
    } else if (cur != 0 && next != 0 && ((CosEditEquip2_unk*)cur)->mKind != ((CosEditEquip2_unk*)next)->mKind) {
        int* record = fn_8044D034(Inventory::instance_, index);
        if (record != 0) {
            *record = -1;
        }
    }
}

// inferred: equipment priorities of a member (fn_80218BB4/fn_80218BC4) and the data they compare
// (weapon or protector parameters); names invented
struct CosEditStat_unk {
    int mKind;
    int mValue;
};
struct CosEditPriority_unk {
    int m00;
    CosEditStat_unk mList[30];
};
struct CosEditGear_unk {
    float mStat0;
    float mStat1;
    float mStat2;
    float mStat3;
    unsigned char m10[0xE4 - 0x10];
    CosEditStat_unk mElem0; // 0xE4: inferred: element and its strength
    unsigned char mEC[0xF8 - 0xEC];
    CosEditStat_unk mElem1; // 0xF8
};
// inferred: protector parameters (same stats, elements at 0xC4 and 0xD8)
struct CosEditArmor_unk {
    float mStat0;
    float mStat1;
    float mStat2;
    float mStat3;
    unsigned char m10[0xC4 - 0x10];
    CosEditStat_unk mElem0; // 0xC4
    unsigned char mCC[0xD8 - 0xCC];
    CosEditStat_unk mElem1; // 0xD8
};

// inferred: the preferred of two weapons: the first criterion of the member priorities that differs decides (kinds 1-4:
// a stat, 6: the strength of an element); a when the priorities end
void* CosEditProtector::fn_801DC9A4(void* priorities, void* a, void* b) {
    if (priorities == 0) {
        return a;
    }
    if (a != 0 && b == 0) {
        return a;
    }
    if (a == 0 && b != 0) {
        return b;
    }
    CosEditGear_unk* ga = (CosEditGear_unk*)a;
    CosEditGear_unk* gb = (CosEditGear_unk*)b;
    CosEditPriority_unk* p = (CosEditPriority_unk*)priorities;
    for (int i = 0; i < 30; i++) {
        CosEditStat_unk* c = &p->mList[i];
        if (c->mKind == 0) {
            break;
        }
        switch (c->mKind) {
        case 1:
            if (ga->mStat0 > gb->mStat0) {
                return a;
            }
            if (ga->mStat0 < gb->mStat0) {
                return b;
            }
            break;
        case 2:
            if (ga->mStat2 > gb->mStat2) {
                return a;
            }
            if (ga->mStat2 < gb->mStat2) {
                return b;
            }
            break;
        case 3:
            if (ga->mStat1 > gb->mStat1) {
                return a;
            }
            if (ga->mStat1 < gb->mStat1) {
                return b;
            }
            break;
        case 4:
            if (ga->mStat3 > gb->mStat3) {
                return a;
            }
            if (ga->mStat3 < gb->mStat3) {
                return b;
            }
            break;
        case 6: {
            int kind = c->mValue;
            CosEditStat_unk* ea = 0;
            if (ga->mElem0.mKind == kind && ga->mElem0.mValue > 0) {
                ea = &ga->mElem0;
            }
            if (ga->mElem1.mKind == kind && ga->mElem1.mValue > 0) {
                ea = &ga->mElem1;
            }
            CosEditStat_unk* eb = 0;
            if (gb->mElem0.mKind == kind && gb->mElem0.mValue > 0) {
                eb = &gb->mElem0;
            }
            if (gb->mElem1.mKind == kind && gb->mElem1.mValue > 0) {
                eb = &gb->mElem1;
            }
            if (ea != 0 && eb != 0) {
                if (ea->mValue > eb->mValue) {
                    return a;
                }
                if (ea->mValue < eb->mValue) {
                    return b;
                }
            } else if (ea != 0 && eb == 0) {
                return a;
            } else if (ea == 0 && eb != 0) {
                return b;
            }
            break;
        }
        }
    }
    return a;
}

// inferred: the preferred of two protectors (as fn_801DC9A4): the first criterion of the member priorities that differs decides (kinds 1-4:
// a stat, 6: the strength of an element); a when the priorities end
void* CosEditProtector::fn_801DCBA8(void* priorities, void* a, void* b) {
    if (priorities == 0) {
        return a;
    }
    if (a != 0 && b == 0) {
        return a;
    }
    if (a == 0 && b != 0) {
        return b;
    }
    CosEditArmor_unk* ga = (CosEditArmor_unk*)a;
    CosEditArmor_unk* gb = (CosEditArmor_unk*)b;
    CosEditPriority_unk* p = (CosEditPriority_unk*)priorities;
    for (int i = 0; i < 30; i++) {
        CosEditStat_unk* c = &p->mList[i];
        if (c->mKind == 0) {
            break;
        }
        switch (c->mKind) {
        case 1:
            if (ga->mStat0 > gb->mStat0) {
                return a;
            }
            if (ga->mStat0 < gb->mStat0) {
                return b;
            }
            break;
        case 2:
            if (ga->mStat2 > gb->mStat2) {
                return a;
            }
            if (ga->mStat2 < gb->mStat2) {
                return b;
            }
            break;
        case 3:
            if (ga->mStat1 > gb->mStat1) {
                return a;
            }
            if (ga->mStat1 < gb->mStat1) {
                return b;
            }
            break;
        case 4:
            if (ga->mStat3 > gb->mStat3) {
                return a;
            }
            if (ga->mStat3 < gb->mStat3) {
                return b;
            }
            break;
        case 6: {
            int kind = c->mValue;
            CosEditStat_unk* ea = 0;
            if (ga->mElem0.mKind == kind && ga->mElem0.mValue > 0) {
                ea = &ga->mElem0;
            }
            if (ga->mElem1.mKind == kind && ga->mElem1.mValue > 0) {
                ea = &ga->mElem1;
            }
            CosEditStat_unk* eb = 0;
            if (gb->mElem0.mKind == kind && gb->mElem0.mValue > 0) {
                eb = &gb->mElem0;
            }
            if (gb->mElem1.mKind == kind && gb->mElem1.mValue > 0) {
                eb = &gb->mElem1;
            }
            if (ea != 0 && eb != 0) {
                if (ea->mValue > eb->mValue) {
                    return a;
                }
                if (ea->mValue < eb->mValue) {
                    return b;
                }
            } else if (ea != 0 && eb == 0) {
                return a;
            } else if (ea == 0 && eb != 0) {
                return b;
            }
            break;
        }
        }
    }
    return a;
}

extern "C" void* fn_80218BC4(int member); // inferred: protector priorities of a member

// inferred: free entry of the protector list that the priorities of the member prefer for the slot
// (a: from the protector it wears)
CosEditItem_unk* CosEditProtector::fn_801DCDAC(int member, int slot, int a) {
    void* priorities = fn_80218BC4(member);
    CosEditItem_unk* result = 0;
    void* best = fn_801CFA5C(member, slot);
    if (a == 0) {
        best = 0;
    }
    fn_801E07A4(member, slot);
    unsigned int count = mB4C.size();
    for (unsigned int i = 0; i < count; i++) {
        CosEditItem_unk* e = &mB4C[i];
        if (e->mOwner < 0) {
            void* prot = fn_80206C50(e->mId);
            if (fn_801DCBA8(priorities, best, prot) == prot) {
                best = prot;
                result = e;
            }
        }
    }
    return result;
}

extern "C" void* fn_80218BB4(int member); // inferred: equipment priorities of a member

// inferred: free entry of the equipment list `slot` that the priorities of the member prefer (a: from
// the item it wears)
// NONMATCHING: 98.2% - registers and scheduling
CosEditItem_unk* CosEditProtector::fn_801DCE88(int member, int slot, int a) {
    void* priorities = fn_80218BB4(member);
    CosEditItem_unk* result = 0;
    void* best = fn_801CFAA8(member, slot);
    if (a == 0) {
        best = 0;
    }
    std::vector<CosEditItem_unk>& list = mB34[slot];
    unsigned int count = list.size();
    for (unsigned int i = 0; i < count; i++) {
        CosEditItem_unk* e = &list[i];
        if (e->mOwner < 0 && e->mEnabled != 0) {
            void* item = e->mItem;
            if (fn_801DC9A4(priorities, best, item) == item) {
                best = item;
                result = e;
            }
        }
    }
    return result;
}

extern "C" bool fn_80206D18(void* weapon, int member, int hand); // inferred: the member can equip the weapon

// inferred: takes the weapon of the hand `slot` off the member; when the hand stays empty, takes one
// that another member of the party can give
// NONMATCHING: 97.5% - registers and scheduling
void CosEditProtector::fn_801DCF60(int member, CharaTask* chara, int slot, int a) {
    CosEditItem_unk* e = fn_801DCDAC(member, slot, a);
    if (e != 0) {
        fn_801CF3C0(member, chara, e, slot, a, 1);
    }
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    if (equip->mWeapons[slot].mId < 0 && m1C54.size() != 0) {
        for (int i = m1C54.size() - 1; i >= 0; i--) {
            NewCampPair_unk* other = m1C54[i];
            for (int j = 0; j < 2; j++) {
                if (j == 1 && other->mId != 3) {
                    continue;
                }
                void* weapon = fn_801CFA5C(other->mId, j);
                if (fn_80206D18(weapon, member, slot)) {
                    CosEditItem_unk* give = fn_801DCDAC(other->mId, j, 0);
                    if (give == 0) {
                        give = fn_801DD12C(other, j);
                    }
                    if (give != 0 && fn_80206D18(weapon, member, slot)) {
                        CosEditItem_unk back;
                        back.mItem = weapon;
                        back.mId = fn_80206BE4(weapon);
                        back.mIndex = -1;
                        back.mOwner = -1;
                        back.mEnabled = 1;
                        back.m14 = -1;
                        fn_801CF3C0(member, chara, &back, slot, 0, 0);
                        fn_801CF3C0(other->mId, (CharaTask*)other->m04, give, j, 0, 1);
                        return;
                    }
                }
            }
        }
    }
}

// inferred: takes from another member of the party (latest first) a weapon that `member` can hold
// in the hand `slot`; the entry given back is mB64
CosEditItem_unk* CosEditProtector::fn_801DD12C(NewCampPair_unk* member, int slot) {
    for (int i = m1C54.size() - 1; i >= 0; i--) {
        NewCampPair_unk* other = m1C54[i];
        if (member == other) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            if (j == 1 && other->mId != 3) {
                continue;
            }
            void* weapon = fn_801CFA5C(other->mId, j);
            if (fn_80206D18(weapon, member->mId, slot)) {
                CosEditItem_unk* give = fn_801DCDAC(other->mId, j, 0);
                if (give != 0) {
                    mB64.mItem = weapon;
                    mB64.mId = fn_80206BE4(weapon);
                    mB64.mIndex = -1;
                    mB64.mOwner = -1;
                    mB64.mEnabled = 1;
                    mB64.m14 = -1;
                    fn_801CF3C0(member->mId, (CharaTask*)member->m04, &mB64, slot, 0, 0);
                    fn_801CF3C0(other->mId, (CharaTask*)other->m04, give, j, 0, 1);
                    return &mB64;
                }
            }
        }
    }
    return 0;
}

// inferred: builds the lists of the two protector slots for the member: the pieces worn by the party
// (enabled when both can swap them), then the free pieces of the inventory that the member can wear
// (one entry per item, after the worn copies of the same piece); finally sorts them
// NONMATCHING: 94.7% - the original is over the inlining budget (R3: vector/FixedArray accessors and the
// CosEditItem_unk constructor 0x801D8C90 called out of line)
void CosEditProtector::fn_801DD26C(int member) {
    std::vector<CosEditItem_unk> worn[2];
    for (int i = 0; i < 2; i++) {
        mB34[i].clear();
    }
    Inventory::Equip_unk* equip = &Inventory::Order()->mEquip[member];
    if (Inventory::Order() != 0) {
        for (int slot = 0; slot < 2; slot++) {
            void* cur = fn_801CFAA8(member, slot);
            for (int j = 0; j < (int)((NewCampTask*)m48)->m1224.size(); j++) {
                NewCampPair_unk* pair = &((NewCampTask*)m48)->m1224[j];
                Inventory::Equip_unk* other = &Inventory::Order()->mEquip[pair->mId];
                int chara = fn_80219544(pair->mId);
                EquipParamEntry_unk* entry = fn_8020ED84(slot, other->mProtectors[slot].mId);
                CosEditItem_unk item;
                item.mId = slot;
                item.mIndex = other->mProtectors[slot].mId;
                item.mOwner = chara;
                item.mItem = entry;
                item.mEnabled = 0;
                if (fn_8020F0AC(entry, member) && fn_8020F0AC((EquipParamEntry_unk*)cur, pair->mId)) {
                    item.mEnabled = 1;
                }
                worn[slot].push_back(item);
            }
            for (int k = 0; k < fn_802114D8(); k++) {
                if (Inventory::Order() == 0) {
                    continue;
                }
                CosEditListParam_unk* param = fn_802114E0(k);
                if (param == 0 || !fn_8020EF04(param->mId)) {
                    continue;
                }
                EquipParamEntry_unk* entry = fn_8020EFEC(param->mId);
                if (entry == 0 || entry->mKind != slot) {
                    continue;
                }
                if (entry->mKind == 1 && fn_8020EF4C(entry)) {
                    continue;
                }
                if (!fn_8020F0AC(entry, member)) {
                    continue;
                }
                bool found = false;
                for (unsigned int j = 0; j < mB34[slot].size(); j++) {
                    if (param->mId == mB34[slot][j].m14) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    continue;
                }
                for (unsigned int j = 0; j < worn[slot].size(); j++) {
                    if (entry->mId == worn[slot][j].mIndex) {
                        mB34[slot].push_back(worn[slot][j]);
                    }
                }
                int count = fn_804444E8(Inventory::Order(), param->mId);
                for (int j = 0; j < count; j++) {
                    CosEditItem_unk item;
                    item.mId = slot;
                    item.mIndex = entry->mId;
                    item.mOwner = -1;
                    item.mItem = entry;
                    item.mEnabled = 1;
                    item.m14 = param->mId;
                    mB34[slot].push_back(item);
                }
            }
        }
        if (fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
            bool found = false;
            for (unsigned int j = 0; j < mB34[1].size(); j++) {
                if (mB34[1][j].mOwner < 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                for (unsigned int j = 0; j < mB34[0].size(); j++) {
                    if (mB34[0][j].mOwner < 0 && !fn_8020EF4C(fn_8020ED84(0, mB34[0][j].mIndex))) {
                        mB34[0][j].mEnabled = 0;
                    }
                }
            }
        }
        switch (mB7C) {
        case 1:
            MslSort_unk(CosEditBegin_unk(mB34[0]), CosEditEnd_unk(mB34[0]), CosEditEquipLess_unk());
            MslSort_unk(CosEditBegin_unk(mB34[1]), CosEditEnd_unk(mB34[1]), CosEditEquipLess_unk());
            break;
        case 0:
            if (member == 0 || member == 2 || member == 3 || member == 6) {
                MslSort_unk(CosEditBegin_unk(mB34[0]), CosEditEnd_unk(mB34[0]), CosEditArmourLess_unk());
                MslSort_unk(CosEditBegin_unk(mB34[1]), CosEditEnd_unk(mB34[1]), CosEditArmourLess_unk());
            } else if (member == 1 || member == 4 || member == 5) {
                MslSort_unk(CosEditBegin_unk(mB34[0]), CosEditEnd_unk(mB34[0]), CosEditRobeLess_unk());
                MslSort_unk(CosEditBegin_unk(mB34[1]), CosEditEnd_unk(mB34[1]), CosEditRobeLess_unk());
            }
            break;
        }
    }
}

extern "C" wchar_t* wcschr(const wchar_t* s, wchar_t c);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" unsigned long wcslen(const wchar_t* s);

// inferred: order of the equipment list (std::sort predicate): items worn first (by member), then
// by name without the "+n" suffix, the shorter name first; names invented
bool CosEditEquipLess_unk::operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const {
    if (a.mIndex == b.mIndex) {
        if (a.mOwner >= 0 && b.mOwner >= 0) {
            return a.mOwner < b.mOwner;
        }
        if (a.mOwner >= 0) {
            return true;
        }
        return false;
    }
    CosEditListName_unk* na = (CosEditListName_unk*)fn_80211480(fn_8020EF80(a.mItem));
    CosEditListName_unk* nb = (CosEditListName_unk*)fn_80211480(fn_8020EF80(b.mItem));
    wchar_t sa[64];
    fn_800DD3FC(sa, L"%s", na->mName);
    wchar_t* plus = wcschr(sa, L'+');
    if (plus != 0) {
        *plus = 0;
    }
    wchar_t sb[64];
    fn_800DD3FC(sb, L"%s", nb->mName);
    plus = wcschr(sb, L'+');
    if (plus != 0) {
        *plus = 0;
    }
    if (wcscmp(sa, sb) == 0) {
        int la = wcslen(na->mName);
        int lb = wcslen(nb->mName);
        if (la != lb) {
            return la < lb;
        }
    }
    return wcscmp(na->mName, nb->mName) < 0;
}

extern "C" bool fn_80206B9C(int code);                 // inferred: whether an item code is a weapon
extern "C" int fn_8044453C(Inventory* inv, int index); // inferred: number of items of an entry of the item table
// inferred: weapon data (only the attack values are read)
struct CosEditWeaponStat_unk {
    float mAttack; // 0x00
    float mMagic;  // 0x04
};
// inferred: predicates of the std::sort of the weapon list: by kind (out of line, below), for the members who
// wear armour (attack first) and for the others (magic first); the members' own weapons first
struct CosEditArmorLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const;
};
struct CosEditWeaponArmourLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const {
        if (a.mId == b.mId) {
            if (a.mOwner >= 0 && b.mOwner >= 0) {
                if (a.mOwner == b.mOwner) {
                    return a.m14 < b.m14;
                }
                return a.mOwner < b.mOwner;
            }
            if (a.mOwner >= 0) {
                return true;
            }
            return false;
        }
        float va = ((CosEditWeaponStat_unk*)a.mItem)->mAttack;
        float vb = ((CosEditWeaponStat_unk*)b.mItem)->mAttack;
        if ((float)__fabs(va - vb) < 1.1920929e-7f) {
            return wcscmp(((CosEditListName_unk*)fn_80211480(fn_80206BE4(a.mItem)))->mName,
                          ((CosEditListName_unk*)fn_80211480(fn_80206BE4(b.mItem)))->mName) < 0;
        }
        return va > vb;
    }
};
struct CosEditWeaponRobeLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const {
        if (a.mId == b.mId) {
            if (a.mOwner >= 0 && b.mOwner >= 0) {
                if (a.mOwner == b.mOwner) {
                    return a.m14 < b.m14;
                }
                return a.mOwner < b.mOwner;
            }
            if (a.mOwner >= 0) {
                return true;
            }
            return false;
        }
        float va = ((CosEditWeaponStat_unk*)a.mItem)->mMagic;
        float vb = ((CosEditWeaponStat_unk*)b.mItem)->mMagic;
        if ((float)__fabs(va - vb) < 1.1920929e-7f) {
            return wcscmp(((CosEditListName_unk*)fn_80211480(fn_80206BE4(a.mItem)))->mName,
                          ((CosEditListName_unk*)fn_80211480(fn_80206BE4(b.mItem)))->mName) < 0;
        }
        return va > vb;
    }
};

// inferred: builds the weapon list of the hand for the member: the weapons worn by the party (mB58, enabled
// when both can swap them), then the free weapons of the inventory that the member can hold (one entry per
// item, after the worn copies of the same weapon); finally sorts it
// NONMATCHING: 0% - the original is over the inlining budget (R3: every vector accessor, clear, push_back and
// begin/end called out of line, other weak copies than those of mB34: the weapon entries are probably
// another type, without constructor); here everything is inlined
void CosEditProtector::fn_801E07A4(int member, int slot) {
    mB4C.clear();
    mB58.clear();
    void* cur = fn_801CFA5C(member, slot);
    for (int j = 0; j < (int)((NewCampTask*)m48)->m1224.size(); j++) {
        NewCampPair_unk* pair = &((NewCampTask*)m48)->m1224[j];
        Inventory::Order()->mEquip[pair->mId];
        for (int k = 0; k < 4; k++) {
            void* weapon = fn_801CFA5C(pair->mId, k);
            CosEditItem_unk item;
            item.mItem = weapon;
            item.mId = fn_80206BE4(weapon);
            item.mIndex = -1;
            item.m14 = k;
            item.mOwner = fn_80219544(pair->mId);
            item.mEnabled = 0;
            if (fn_80206D18(weapon, member, slot) && fn_80206D18(cur, pair->mId, k)) {
                item.mEnabled = 1;
                item.m14 = k;
            }
            mB58.push_back(item);
        }
    }
    for (int k = 0; k < fn_802114D8(); k++) {
        if (Inventory::Order() == 0) {
            continue;
        }
        CosEditListParam_unk* param = fn_802114E0(k);
        if (param == 0 || !fn_80206B9C(param->mId)) {
            continue;
        }
        CosEditProtData_unk* weapon = fn_80206C50(param->mId);
        if (weapon == 0 || !fn_80206D18(weapon, member, slot)) {
            continue;
        }
        bool found = false;
        for (unsigned int j = 0; j < mB4C.size(); j++) {
            if (param->mId == mB4C[j].mId) {
                found = true;
                break;
            }
        }
        if (found) {
            continue;
        }
        for (unsigned int j = 0; j < mB58.size(); j++) {
            if (param->mId == mB58[j].mId) {
                mB4C.push_back(mB58[j]);
            }
        }
        int count = fn_8044453C(Inventory::Order(), k);
        for (int j = 0; j < count; j++) {
            CosEditItem_unk item;
            item.mItem = weapon;
            item.mId = param->mId;
            item.mIndex = -1;
            item.mOwner = -1;
            item.mEnabled = 1;
            item.m14 = -1;
            mB4C.push_back(item);
        }
    }
    switch (mB80) {
    case 1:
        MslSort_unk(CosEditBegin_unk(mB4C), CosEditEnd_unk(mB4C), CosEditArmorLess_unk());
        break;
    case 0:
        if (member == 0 || member == 2 || member == 3 || member == 6) {
            MslSort_unk(CosEditBegin_unk(mB4C), CosEditEnd_unk(mB4C), CosEditWeaponArmourLess_unk());
        } else if (member == 1 || member == 4 || member == 5) {
            MslSort_unk(CosEditBegin_unk(mB4C), CosEditEnd_unk(mB4C), CosEditWeaponRobeLess_unk());
        }
        break;
    }
}

// inferred: order of the protector list: same item, worn first (by member, then by slot), then by name
// as CosEditEquipLess_unk
bool CosEditArmorLess_unk::operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const {
    if (a.mId == b.mId) {
        if (a.mOwner >= 0 && b.mOwner >= 0) {
            if (a.mOwner == b.mOwner) {
                return a.m14 < b.m14;
            }
            return a.mOwner < b.mOwner;
        }
        if (a.mOwner >= 0) {
            return true;
        }
        return false;
    }
    CosEditListName_unk* na = (CosEditListName_unk*)fn_80211480(fn_80206BE4(a.mItem));
    CosEditListName_unk* nb = (CosEditListName_unk*)fn_80211480(fn_80206BE4(b.mItem));
    wchar_t sa[64];
    fn_800DD3FC(sa, L"%s", na->mName);
    wchar_t* plus = wcschr(sa, L'+');
    if (plus != 0) {
        *plus = 0;
    }
    wchar_t sb[64];
    fn_800DD3FC(sb, L"%s", nb->mName);
    plus = wcschr(sb, L'+');
    if (plus != 0) {
        *plus = 0;
    }
    if (wcscmp(sa, sb) == 0) {
        int la = wcslen(na->mName);
        int lb = wcslen(nb->mName);
        if (la != lb) {
            return la < lb;
        }
    }
    return wcscmp(na->mName, nb->mName) < 0;
}

// Members of atn::D2A_TaskInstanceBase<32, 8> with automatic names (as in game/CosEditPalette_unk.cpp)
typedef atn::D2A_TaskInstanceBase<32, 8> CosEditListLayout_unk;
extern "C" void fn_801F7590(CosEditListLayout_unk* inst, const char* name, const float* uv);
extern "C" void fn_801F837C(CosEditListLayout_unk* inst, const char* name, const wchar_t* text);
// inferred: texture rectangle (u0, v0, u1, v1), passed by value (as in game/CosEditPalette_unk.cpp)
struct CosEditListUV_unk {
    CosEditListUV_unk() {}
    CosEditListUV_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    CosEditListUV_unk(const CosEditListUV_unk& o) { __Vec4Copy(&u0, &o.u0); }
    CosEditListUV_unk& operator=(const CosEditListUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
        return *this;
    }
    float u0, v0, u1, v1;
};
extern "C" int fn_80444B64(Inventory* inv, int id);                    // inferred: icon of an item
extern "C" CosEditListUV_unk fn_80444C50(Inventory* inv, int icon);    // inferred: texture rectangle of an icon
// inferred: sets a rectangle of the layout (the name comes first, as SetUV_unk of CosEditPalette_unk)
inline void SetListUV_unk(const char* name, CosEditListLayout_unk* inst, CosEditListUV_unk uv) {
    fn_801F7590(inst, name, &uv.u0);
}

// inferred: draws the page of the equipment list m1E4 (names, icons, face of the member who wears
// it and the state of each row)
// NONMATCHING: 88% - registers and scheduling
void CosEditProtector::fn_801E6714() {
    m84 = mB34[m1E4].size();
    m8C = 10;
    if (m80 >= m84) {
        m80 = MAX(m84 - 1, 0);
    }
    if (m84 >= m8C && m84 - 1 <= m8C + m88 - 1) {
        m88 = m84 - m8C;
    }
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[fn_801CF334()->mFace];
    if (mLists[0].mBase != 0) {
        fn_801D4E74(fn_801CF334()->mFace, mLists[0].mBase->GetTask(), m1B98);
    }
    for (int i = 0; i < m8C; i++) {
        CosEditListLayout_unk* task = (CosEditListLayout_unk*)mLists[0].mItems[i]->GetTask();
        int index = m88 + i;
        if (index >= 0 && index < (int)mB34[m1E4].size()) {
            CosEditItem_unk* e = &mB34[m1E4][index];
            int sub = 0;
            int kind = ((CosEditEquip_unk*)e->mItem)->mKind;
            if (kind == 1 || kind == 3) {
                sub = 1;
            }
            int id = fn_8020EF80(e->mItem);
            CosEditListName_unk* data = (CosEditListName_unk*)fn_80211480(id);
            CosEditListUV_unk face(1.0f, 1.0f, 1.0f, 1.0f);
            CosEditListUV_unk uv = fn_80444C50(Inventory::instance_, fn_80444B64(Inventory::instance_, id));
            if (data != 0) {
                fn_801F837C(task, "item01", data->mName);
            } else if (e->mItem != 0) {
                fn_801F837C(task, "item01", ((CosEditEquip_unk*)e->mItem)->mName);
            }
            SetListUV_unk("iconUv01", task, uv);
            if (e->mOwner >= 0) {
                CosEditCharaInfo_unk* info = fn_8020924C(e->mOwner);
                int icon;
                if (info != 0) {
                    icon = info->mIcon;
                } else {
                    icon = 0;
                }
                if (icon > 0) {
                    int k = icon - 1;
                    face = CosEditListUV_unk(0.25f * (k % 4), 0.5f * (k / 4), 0.25f * (k % 4 + 1), 0.5f * (k / 4 + 1));
                }
            }
            SetListUV_unk("uv0001", task, face);
            SetListUV_unk("mask_uv0001", task, face);
            if (e->mIndex == equip->mProtectors[sub].mId && equip->mProtectors[sub].m04 < 0) {
                task->mFrame = 2.0f;
            } else if (e->mEnabled == 0) {
                task->mFrame = 3.0f;
            } else {
                task->mFrame = 1.0f;
            }
        } else {
            task->mFrame = 0.0f;
        }
    }
}

extern "C" int fn_80444BE8(Inventory* inv, CosEditListName_unk* item); // inferred: icon of an item
extern "C" int fn_80444C48(Inventory* inv);                            // inferred: icon of the empty slot

// inferred: draws the page of the protector list (as fn_801E6714)
// NONMATCHING: 92.6% - registers and scheduling
void CosEditProtector::fn_801E6B10() {
    m84 = mB4C.size();
    m8C = 10;
    if (m80 >= m84) {
        m80 = MAX(m84 - 1, 0);
    }
    if (m84 >= m8C && m84 - 1 <= m8C + m88 - 1) {
        m88 = m84 - m8C;
    }
    if (mLists[0].mBase != 0) {
        fn_801D4E74(fn_801CF334()->mFace, mLists[0].mBase->GetTask(), m1B98);
    }
    int member = fn_801CF334()->mFace;
    for (int i = 0; i < m8C; i++) {
        CosEditListLayout_unk* task = (CosEditListLayout_unk*)mLists[0].mItems[i]->GetTask();
        int index = m88 + i;
        if (index >= 0 && index < (int)mB4C.size()) {
            CosEditItem_unk* e = &mB4C[index];
            CosEditListName_unk* data = (CosEditListName_unk*)fn_80211480(e->mId);
            CosEditProtData_unk* prot = fn_80206C50(e->mId);
            if (data != 0 && prot != 0) {
                int icon;
                if (e->mIndex < 0) {
                    icon = fn_80444BE8(Inventory::instance_, data);
                    fn_801F837C(task, "item01", data->mName);
                } else {
                    icon = fn_80444C48(Inventory::instance_);
                    fn_801F837C(task, "item01", lbl_8087F1E4->GetPair_unk(prot->mKind + 0xEB));
                }
                CosEditListUV_unk uv = fn_80444C50(Inventory::instance_, icon);
                SetListUV_unk("iconUv01", task, uv);
                CosEditListUV_unk face(1.0f, 1.0f, 1.0f, 1.0f);
                if (e->mOwner >= 0) {
                    CosEditCharaInfo_unk* info = fn_8020924C(e->mOwner);
                    int faceIcon;
                    if (info != 0) {
                        faceIcon = info->mIcon;
                    } else {
                        faceIcon = 0;
                    }
                    if (faceIcon > 0) {
                        int k = faceIcon - 1;
                        face = CosEditListUV_unk(0.25f * (k % 4), 0.5f * (k / 4), 0.25f * (k % 4 + 1),
                                                 0.5f * (k / 4 + 1));
                    }
                    if (e->mOwner == member) {
                        task->mFrame = 2.0f;
                    }
                    if (e->mEnabled != 0) {
                        task->mFrame = 1.0f;
                    } else {
                        task->mFrame = 3.0f;
                    }
                } else {
                    task->mFrame = 1.0f;
                }
                SetListUV_unk("uv0001", task, face);
                SetListUV_unk("mask_uv0001", task, face);
            }
        } else {
            task->mFrame = 0.0f;
        }
    }
}

// inferred: puts the cursor of the equipment list `list` on the entry that the member wears (a: at
// the top of the page)
void CosEditProtector::fn_801E6EF0(int member, int list, int* cursor, int* top, int a) {
    int chara = fn_80219544(member);
    std::vector<CosEditItem_unk>& items = mB34[list];
    for (int i = 0; i < (int)items.size(); i++) {
        if (chara == items[i].mOwner) {
            *cursor = i;
            if (a) {
                *top = i;
            } else {
                if (*top > i) {
                    *top = i;
                }
                if (*top < *cursor - 9) {
                    *top = *cursor - 9;
                }
            }
            if (mB34[list].size() < 10) {
                *top = 0;
            }
            break;
        }
    }
}

// inferred: puts the cursor of the protector list on the protector of the slot `slot` that the member
// wears (as fn_801E6EF0)
void CosEditProtector::fn_801E6FC8(int member, int slot, int* cursor, int* top, int a) {
    int chara = fn_80219544(member);
    for (int i = 0; i < (int)mB4C.size(); i++) {
        if (chara == mB4C[i].mOwner && slot == mB4C[i].m14) {
            *cursor = i;
            if (a) {
                *top = i;
            } else {
                if (*top > i) {
                    *top = i;
                }
                if (*top < *cursor - 9) {
                    *top = *cursor - 9;
                }
            }
            if (mB4C.size() < 10) {
                *top = 0;
            }
        }
    }
}

// inferred: puts the cursor of the protector list on the entry of `item` (same item, same owner and
// slot, or both free), keeping its row on the page
void CosEditProtector::fn_801E7098(CosEditItem_unk* item, int list, int* cursor, int* top) {
    int row = *cursor - *top;
    for (int i = 0; i < (int)mB4C.size(); i++) {
        CosEditItem_unk* e = &mB4C[i];
        if (e->mId == item->mId &&
            ((e->mOwner < 0 && item->mOwner < 0) || (e->mOwner == item->mOwner && e->m14 == item->m14))) {
            *cursor = i;
            *top = MAX(i - row, 0);
            if (mB4C.size() < 10) {
                *top = 0;
            }
            return;
        }
    }
}


extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
// inferred: d2anime task seen through its layout (as in game/CosEditProtector.cpp)
struct CosEditListTask_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[4]; // 0x58
};

extern "C" const CosEditListUV_unk lbl_807C7040; // inferred: empty texture rectangle
// inferred: sets a text of the layout (the layout address is taken before the hash)
inline void SetListText_unk(atn::D2A_Task* task, const char* name, const wchar_t* text) {
    void* layout = ((CosEditListTask_unk*)task)->mLayout;
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
// inferred: sets a texture rectangle of the task (by value, as SetUV_unk of game/CosEditProtector.cpp)
inline void SetTaskUV_unk(atn::D2A_Task* task, const char* name, CosEditListUV_unk uv) {
    task->SetParam2_unk(name, &uv.u0);
}

// inferred: shows the weapons and the protectors that the member wears (names and icons)
// NONMATCHING: 98.7% - registers of the item/data temporaries; pool offsets until the rest of the unit is written
void CosEditProtector::fn_801E714C(int member, atn::D2A_Task* task) {
    if (task == 0) {
        return;
    }
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    CosEditListUV_unk uv(0.0f, 0.0f, 0.0f, 0.0f);
    void* item = fn_801CFA5C(member, 0);
    CosEditListName_unk* data = (CosEditListName_unk*)fn_80211480(fn_80206BE4(item));
    if (item != 0 && data != 0) {
        int icon = fn_80444BE8(Inventory::instance_, data);
        SetListText_unk(task, "main", data->mName);
        uv = fn_80444C50(Inventory::instance_, icon);
        SetTaskUV_unk(task, "mainIconUv", uv);
    } else {
        SetListText_unk(task, "main", L"\x2015\x2015\x2015\x2015");
        uv = lbl_807C7040;
        SetTaskUV_unk(task, "mainIconUv", uv);
    }
    item = fn_801CFA5C(member, 1);
    data = (CosEditListName_unk*)fn_80211480(fn_80206BE4(item));
    if (item != 0 && data != 0) {
        int icon = fn_80444BE8(Inventory::instance_, data);
        SetListText_unk(task, "sub", data->mName);
        uv = fn_80444C50(Inventory::instance_, icon);
        SetTaskUV_unk(task, "subIconUv", uv);
    } else {
        SetListText_unk(task, "sub", L"\x2015\x2015\x2015\x2015");
        uv = lbl_807C7040;
        SetTaskUV_unk(task, "subIconUv", uv);
    }
    EquipParamEntry_unk* entry = fn_8020ED84(0, equip->mProtectors[0].mId);
    int id = fn_8020EF80(entry);
    data = (CosEditListName_unk*)fn_80211480(id);
    if (entry != 0 && data != 0) {
        SetListText_unk(task, "protector01", data->mName);
        uv = fn_80444C50(Inventory::instance_, fn_80444B64(Inventory::instance_, id));
        SetTaskUV_unk(task, "upIconUv", uv);
    } else {
        SetListText_unk(task, "protector01", L"\x2015\x2015\x2015\x2015");
        uv = lbl_807C7040;
        SetTaskUV_unk(task, "upIconUv", uv);
    }
    entry = fn_8020ED84(1, equip->mProtectors[1].mId);
    if (fn_8020EF4C(entry)) {
        entry = fn_8020ED84(0, equip->mProtectors[0].mId);
    }
    id = fn_8020EF80(entry);
    data = (CosEditListName_unk*)fn_80211480(id);
    if (entry != 0 && data != 0) {
        SetListText_unk(task, "protector02", data->mName);
        uv = fn_80444C50(Inventory::instance_, fn_80444B64(Inventory::instance_, id));
        SetTaskUV_unk(task, "lowIconUv", uv);
    } else {
        SetListText_unk(task, "protector02", L"\x2015\x2015\x2015\x2015");
        uv = lbl_807C7040;
        SetTaskUV_unk(task, "lowIconUv", uv);
    }
}

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_8021C6A4(wchar_t* out, const CosEditStat_unk* skill); // inferred: name of a skill

// inferred: stats of a member with an equipment changed (as in game/NewCampPartsBase.cpp)
CharaParam_unk fn_80134800(CharaStatus* status, int id, int kindA, int indexA, int itemA, int kindB, int indexB,
                           int itemB);
extern "C" const char* fn_80117200(int stat); // inferred: prefix of a stat name ("atk_", ...)
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);           // inferred: sets a float
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value); // inferred: sets a vector item
// inferred: stat colours of game/MenuCommon_unk.cpp (up, down, same)
struct CosEditListColor_unk {
    float r, g, b, a;
};
extern "C" CosEditListColor_unk lbl_807C7898;
extern "C" CosEditListColor_unk lbl_807C78A8;
extern "C" CosEditListColor_unk lbl_807C78B8;
// inferred: layout parameters of a d2anime task by name (as in game/GameUI.cpp)
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, float value) {
    fn_801FECE0(&mData.m10, fn_800DC6B4(name), value);
}
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, int index, float value) {
    fn_801FED24(&mData.m10, fn_800DC6B4(name), index, value);
}
// inferred: differences between the stats with the selected item and the current ones (the agility does not
// change with the equipment)
inline void StatDiff_unk(int* diff, int* values, const CharaParam_unk& cur, const CharaParam_unk& next) {
    diff[0] = (int)next.m00 - (int)cur.m00;
    diff[1] = (int)next.m08 - (int)cur.m08;
    diff[2] = 0;
    diff[3] = (int)next.m0C - (int)cur.m0C;
    diff[4] = (int)next.m04 - (int)cur.m04;
    for (int i = 0; i < 5; i++) {
        values[i] += diff[i];
    }
}

// inferred: shows the stats of the member with the selected equipment (values, up/down arrows and colours)
// NONMATCHING: 99.4% - registers of this/member/equip; pool offsets until the rest of the unit is written
void CosEditProtector::fn_801E75F8(int member, CharaStatus* status, atn::D3Anime* base, NewCampPartsEntry_unk* entry) {
    if (entry == 0 || entry->mAnime == 0 || entry->mAnime->GetTask() == 0) {
        return;
    }
    atn::D2A_Task* task = (atn::D2A_Task*)entry->mAnime->GetTask();
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    CharaParam_unk cur = fn_80134800(status, member, 0, 0, 0, 0, 0, 0);
    int values[5];
    int diff[5];
    values[0] = (int)cur.m00;
    values[1] = (int)cur.m08;
    values[4] = (int)cur.m04;
    values[3] = (int)cur.m0C;
    values[2] = cur.m94;
    memset(diff, 0, sizeof(diff));
    if (m7C == 2) {
        CosEditItem_unk* sel = 0;
        if (mB34[m1E4].size() != 0) {
            sel = &mB34[m1E4][m80];
        }
        if (sel != 0) {
            int indexB = 0;
            int kindB = 0;
            int itemB = 0;
            if (m1E4 == 0) {
                if (fn_8020EF4C((EquipParamEntry_unk*)sel->mItem)) {
                    itemB = sel->mIndex;
                    kindB = 2;
                    indexB = 1;
                } else if (fn_8020EF4C(fn_8020ED84(m1E4, equip->mProtectors[m1E4].mId))) {
                    CosEditItem_unk* other = fn_801DCE88(member, 1, 0);
                    if (other != 0) {
                        itemB = other->mIndex;
                        kindB = 2;
                        indexB = 1;
                    }
                }
            }
            CharaParam_unk next = fn_80134800(status, member, 2, m1E4, sel->mIndex, kindB, indexB, itemB);
            StatDiff_unk(diff, values, cur, next);
        }
    } else if (m7C == 3) {
        CosEditProtData_unk* prot = 0;
        if (mB4C.size() != 0 && m80 < (int)mB4C.size()) {
            prot = fn_80206C50(mB4C[m80].mId);
        }
        if (prot != 0) {
            CharaParam_unk next = fn_80134800(status, member, 1, m1E4, prot->m80, 0, 0, 0);
            StatDiff_unk(diff, values, cur, next);
        }
    }
    for (int i = 0; i < 5; i++) {
        char name[0x40];
        char disp[0x40];
        char sub[0x40];
        sprintf(name, "%sparam", fn_80117200(i));
        task->SetNumber_unk(name, values[i], 0);
        sprintf(disp, "%supdw_disp", fn_80117200(i));
        sprintf(name, "%scol", fn_80117200(i));
        sprintf(sub, "%scolSub", fn_80117200(i));
        if (diff[i] > 0) {
            task->SetLayoutValue_unk(disp, 1.0f);
            task->SetColor_unk(name, lbl_807C7898.r, lbl_807C7898.g, lbl_807C7898.b);
            task->SetColor_unk(sub, lbl_807C7898.r, lbl_807C7898.g, lbl_807C7898.b);
        } else if (diff[i] == 0) {
            task->SetLayoutValue_unk(disp, 0.0f);
            task->SetColor_unk(name, lbl_807C78B8.r, lbl_807C78B8.g, lbl_807C78B8.b);
            task->SetColor_unk(sub, lbl_807C78B8.r, lbl_807C78B8.g, lbl_807C78B8.b);
        } else {
            task->SetLayoutValue_unk(disp, 1.0f);
            task->SetColor_unk(name, lbl_807C78A8.r, lbl_807C78A8.g, lbl_807C78A8.b);
            task->SetColor_unk(sub, lbl_807C78A8.r, lbl_807C78A8.g, lbl_807C78A8.b);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (mStatusUp[i]->GetTask() == 0 || mStatusDown[i]->GetTask() == 0) {
            continue;
        }
        mStatusUp[i]->mVisible = 0;
        mStatusDown[i]->mVisible = 0;
        char pos[0x40];
        sprintf(pos, "%supdw_pos", fn_80117200(i));
        atn::D2A_DrawRect rect = fn_801F4E8C(task, fn_800DC6B4(pos));
        atn::D3Anime* arrow = 0;
        if (diff[i] > 0) {
            arrow = mStatusUp[i];
        } else if (diff[i] < 0) {
            arrow = mStatusDown[i];
        }
        if (arrow != 0 && (unsigned int)(m7C - 2) <= 1) {
            fn_80511258(entry, arrow, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
            arrow->mVisible = 1;
            float x = rect.x;
            ((atn::D2A_Task*)arrow->GetTask())->SetLayoutValue_unk("basePos", 0, x);
            float y = rect.y;
            ((atn::D2A_Task*)arrow->GetTask())->SetLayoutValue_unk("basePos", 1, y);
        }
    }
}

// inferred: writes the names of the two skills of the protector (dashes without one, question marks
// while it is not known)
extern "C" void fn_801E7D78(atn::D3Anime* anime, void* item, int known) {
    if (anime == 0 || anime->GetTask() == 0) {
        return;
    }
    wchar_t names[2][64];
    memset(names[0], 0, sizeof(names[0]));
    memset(names[1], 0, sizeof(names[1]));
    CosEditStat_unk* skill = &((CosEditArmor_unk*)item)->mElem0;
    for (int i = 0; i < 2; i++) {
        if (item == 0) {
            wcscpy(names[i], L"-----");
        } else if (known == 0) {
            wcscpy(names[i], L"?????");
        } else if (skill == 0 || !fn_8021C6A4(names[i], skill)) {
            wcscpy(names[i], L"-----");
        }
        char name[0x40];
        sprintf(name, "skill_name%02d", i);
        CosEditListTask_unk* task = (CosEditListTask_unk*)anime->GetTask();
        fn_801FEE08(task->mLayout, fn_800DC6B4(name), names[i]);
        skill = (CosEditStat_unk*)((char*)skill + 0x14);
    }
}

// inferred: same for the two skills of an equipment
extern "C" void fn_801E7EA8(atn::D3Anime* anime, void* entry, int known) {
    if (anime == 0 || anime->GetTask() == 0) {
        return;
    }
    wchar_t names[2][64];
    memset(names[0], 0, sizeof(names[0]));
    memset(names[1], 0, sizeof(names[1]));
    CosEditStat_unk* skill = &((CosEditGear_unk*)entry)->mElem0;
    for (int i = 0; i < 2; i++) {
        if (entry == 0) {
            wcscpy(names[i], L"-----");
        } else if (known == 0) {
            wcscpy(names[i], L"?????");
        } else if (skill == 0 || !fn_8021C6A4(names[i], skill)) {
            wcscpy(names[i], L"-----");
        }
        char name[0x40];
        sprintf(name, "skill_name%02d", i);
        CosEditListTask_unk* task = (CosEditListTask_unk*)anime->GetTask();
        fn_801FEE08(task->mLayout, fn_800DC6B4(name), names[i]);
        skill = (CosEditStat_unk*)((char*)skill + 0x14);
    }
}
