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
#include "macros.h"

// Third file of CosEditProtector (inferred file name, game/CosEditProtector.cpp and
// game/CosEditPalette_unk.cpp have the rest): the equipment lists of the costume menu (states 0, 2,
// 3, 11-13) and their item lists. The std::sort copies and the list helpers of the file are not
// written yet.

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s);          // inferred: string hash
extern "C" void* lbl_8087EF70;                                // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" atn::SoundRef fn_80117228(int id);                 // inferred: plays a system sound effect
extern "C" const wchar_t* fn_80116FC0(int a, int id);         // inferred: system text
extern "C" GameTask* lbl_8087F430;                            // inferred: field
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
    unsigned char m00[0xB8];
    int mKind; // 0xB8
};
extern "C" CosEditProtData_unk* fn_80206C50(int id); // inferred: protector data of an item id

// inferred: state 2, the list of the equipments of the slot m1E4: A equips the item (taking it
// from the member who has it), B goes back, the two shoulder buttons sort the list
// NONMATCHING: 0% - written, not compared yet
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
// NONMATCHING: 0% - written, not compared yet
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
// NONMATCHING: 41% - the original calls fn_801DC8C8 out of line: over the inlining budget (R3) once
// fn_801DD26C (not written yet) is a candidate of the file
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
// NONMATCHING: 0% - written, not compared yet
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
// NONMATCHING: 0% - written, not compared yet
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

extern "C" wchar_t* wcschr(const wchar_t* s, wchar_t c);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" unsigned long wcslen(const wchar_t* s);

// inferred: order of the equipment list (std::sort predicate): items worn first (by member), then
// by name without the "+n" suffix, the shorter name first; names invented
struct CosEditEquipLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const;
};

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

// inferred: order of the protector list: same item, worn first (by member, then by slot), then by name
// as CosEditEquipLess_unk
struct CosEditArmorLess_unk {
    bool operator()(const CosEditItem_unk& a, const CosEditItem_unk& b) const;
};

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
// NONMATCHING: 0% - written, not compared yet
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
// NONMATCHING: 0% - written, not compared yet
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

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_8021C6A4(wchar_t* out, const CosEditStat_unk* skill); // inferred: name of a skill
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
// inferred: d2anime task seen through its layout (as in game/CosEditProtector.cpp)
struct CosEditListTask_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[4]; // 0x58
};

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
