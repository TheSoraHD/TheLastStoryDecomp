#include "game/CosEditProtector.h"
#include "game/GameTask.h"
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
