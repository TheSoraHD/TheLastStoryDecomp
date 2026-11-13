#ifndef GAME_CAMPSAVELOAD_H
#define GAME_CAMPSAVELOAD_H

#include "atn/Task.h"
#include "atn/D2A_Task.h"
#include "atn/Texture_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include "std/string.h"
#include "game/GameSaveTask.h"
#include "atn/ResRef_unk.h"


// inferred: entry of CampSaveLoad::mEntries, one row of the list read by fn_8011B8D4 (0x18 bytes;
// weak constructor 0x8011BBBC and destructor 0x80117E88); names invented
struct CampSaveLoadEntry_unk {
    int m00;
    int m04;
    int m08;
    std::basic_string<wchar_t> m0C;
};

// inferred: date and play time of a save slot (names invented)
struct SaveDate_unk {
    int mSec;  // 0x00
    int mMin;  // 0x04
    int mHour; // 0x08
    int m0C;
    int m10;
    int m14;   // 0x14: inferred: compared first (chapter or year)
    int m18;
    int mDay;  // 0x1C
};

// inferred: one save slot (0xB8 bytes; checked by fn_803BEAB4 / fn_803BEB54)
struct SaveSlot_unk {
    int m00;
    int m04;
    SaveDate_unk m08;
    unsigned char m28[0x30 - 0x28];
    unsigned char m30;
    unsigned char m31;
    unsigned short m32;
    unsigned char m34[0xB8 - 0x34];
};

class GameLoadTask;
class TutorialPopup;

// inferred: view of GameSaveLoadCommon (save task fn_803B7944 / load task fn_803B27F8) (see src/auto_03_803B27AC_text.cpp)
struct CampSaveData_unk {
    unsigned char m000[0x4C];
    SaveSlot_unk mSlots[17]; // 0x4C
    unsigned char mC84[0xD60 - 0xC84];
    int mD60;
    int mD64;
};


// inferred: arrow of the page (left/right) and its "pressed" flag (names invented)
struct CampArrow_unk {
    atn::D2A_Task* mTask;
    int mPressed;
};

// inferred from __vt__12CampSaveLoad (0x8077A030) and its constructor (0x80117B54); base of the
// save and load menus. The destructor is inlined in those of MenuSaveTask and MenuLoadTask.
class CampSaveLoad : public atn::Task {
public:
    CampSaveLoad(atn::Task* parent); // 0x80117B54
    virtual ~CampSaveLoad(); // 0x80117EF4 (inlined in the destructors of the menus)
    virtual int vf0C();  // 0x80117FE4
    virtual void vf10(); // 0x801185E0
    void fn_801191A8(); // inferred: state 1 (slot selection)
    void fn_8011999C(); // inferred: state 5
    void fn_80119F0C(); // inferred: info bar
    void fn_8011A4D0(); // inferred: slot frames and cursor
    virtual void vf14(); // 0x801189B4
    virtual void vf1C(); // 0x801191A0
    virtual void vf20(); // 0x801191A4
    CampSaveData_unk* GetData_unk() { return mMode == 0 ? (CampSaveData_unk*)m4C : (CampSaveData_unk*)m50; }
    SaveSlot_unk* GetSlot_unk(int slot) { return &GetData_unk()->mSlots[slot]; } // inferred: name invented
    void fn_80119030(int state, int arg); // inferred: changes the state
    void fn_8011968C();                   // inferred: handles the cancel/back input
    void fn_8011979C();                   // inferred: handles the confirmation window input
    void fn_8011B3D8(int wrap);           // inferred: moves the cursor m154 within [0, m158)
    void fn_8011B570();                   // inferred: moves the cursor on the slot grid
    void fn_8011B8D4(const wchar_t* text, int length); // inferred: reads the list of entries
    void fn_8011AD38();
    void fn_8011B018(int, SaveSlot_unk*); // inferred: shows a save slot                   // inferred: animates the slot frames
    int fn_8011BBD0(int slot);            // inferred: nonzero if the slot is newer than the current data

    int mMode;                  // 0x48: inferred: 0 save, 1 load
    GameSaveTask* m4C;          // 0x4C: save menu task (fn_803B7944); seen as CampSaveData_unk by GetData_unk
    GameLoadTask* m50;          // 0x50: inferred: load task of the load menu (seen as CampSaveData_unk by GetData_unk)
    int m54;
    atn::D2A_Task* m58;             // 0x58: inferred: new map icon animation
    atn::D2A_Task* m5C;             // 0x5C: inferred: info bar
    atn::D2A_Task* m60;             // 0x60: inferred: camp info panels
    atn::D2A_Task* m64;
    atn::D2A_Task* m68;
    atn::D2A_Task* m6C;             // 0x6C: inferred: cursor
    atn::D2A_Task* m70;             // 0x70: inferred: menu animation task (fn_801F3FF8)
    atn::D2A_Task* m74[8];        // 0x74: inferred: slot frames
    atn::D2A_Task* m94[8];          // 0x94: inferred: slot icons
    atn::D2A_Task* mB4[8];          // 0xB4: inferred: "no data" panels
    int mD4[8];
    atn::Task* mF4[8];          // 0xF4: inferred: "clear" marks
    atn::D2A_Task* m114;
    CampArrow_unk mArrows[2];   // 0x118: inferred: left and right arrows
    int m128;                   // 0x128: inferred: selected slot
    int m12C;
    int m130;
    int m134;
    int m138;
    int m13C;                   // 0x13C: inferred: state (fn_80119030)
    int m140;                   // 0x140: inferred: previous state
    int m144;                   // 0x144: inferred: state argument
    int m148;
    int m14C;
    int m150;
    int m154;
    int m158;
    int m15C;
    int m160;
    int m164;
    int m168;
    int m16C;
    int m170;
    int m174;
    TutorialPopup* m178;        // 0x178: inferred: message window
    atn::Texture_unk mTextures[17]; // 0x17C
    ResRef_unk m4AC;            // 0x4AC: inferred: "/preload/saveload.pk"
    int m4B0;                   // inferred: set once the archive is in use (was ResRef_unk::m04)
    int m4B4;
    atn::LH_Binary m4B8;
    atn::SimpleArray<CampSaveLoadEntry_unk> mEntries; // 0x4C0
    float m4C8;
    unsigned char m4CC[0x4D0 - 0x4CC];
};

#endif
