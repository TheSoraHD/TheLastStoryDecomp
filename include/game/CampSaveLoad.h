#ifndef GAME_CAMPSAVELOAD_H
#define GAME_CAMPSAVELOAD_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include "std/string.h"

// inferred: reference to a shared resource of the load manager (functions 0x80470364/0x80470528);
// same layout as in atn/LH_MotionSet.cpp
struct ResRef_unk {
    ResRef_unk() : mRes(0), m04(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_80470364(const char* name, int a, int b, int c); // inferred: starts loading `name`
    void fn_80470528();                                      // inferred: drops the reference

    void* mRes;
    void* m04;
};

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

// inferred: save data object made by fn_803B7944 / fn_803B27F8 (see src/auto_03_803B27AC_text.cpp)
struct CampSaveData_unk {
    unsigned char m000[0x4C];
    SaveSlot_unk mSlots[17]; // 0x4C
    unsigned char mC84[0xD60 - 0xC84];
    int mD60;
    int mD64;
};

// inferred from __vt__12CampSaveLoad (0x8077A030) and its constructor (0x80117B54); base of the
// save and load menus. The destructor is inlined in those of MenuSaveTask and MenuLoadTask.
class CampSaveLoad : public atn::Task {
public:
    CampSaveLoad(atn::Task* parent); // 0x80117B54
    virtual ~CampSaveLoad(); // 0x80117EF4 (inlined in the destructors of the menus)
    virtual int vf0C();  // 0x80117FE4
    virtual void vf10(); // 0x801185E0
    virtual void vf14(); // 0x801189B4
    virtual void vf1C(); // 0x801191A0
    virtual void vf20(); // 0x801191A4
    CampSaveData_unk* GetData_unk() { return mMode == 0 ? m4C : m50; }
    void fn_80119030(int state, int arg); // inferred: changes the state
    void fn_8011968C();                   // inferred: handles the cancel/back input
    void fn_8011B8D4(const wchar_t* text, int length); // inferred: reads the list of entries
    int fn_8011BBD0(int slot);            // inferred: nonzero if the slot is newer than the current data

    int mMode;                  // 0x48: inferred: 0 save, 1 load
    CampSaveData_unk* m4C;      // 0x4C: inferred: save data of the save menu
    CampSaveData_unk* m50;      // 0x50: inferred: save data of the load menu
    int m54;
    atn::Task* m58;             // 0x58: inferred: new map icon animation
    atn::Task* m5C;             // 0x5C: inferred: info bar
    atn::Task* m60;             // 0x60: inferred: camp info panels
    atn::Task* m64;
    atn::Task* m68;
    atn::Task* m6C;             // 0x6C: inferred: cursor
    atn::Task* m70;             // 0x70: inferred: menu animation task (fn_801F3FF8)
    int m74[8];
    atn::Task* m94[8];          // 0x94: inferred: slot icons
    atn::Task* mB4[8];          // 0xB4: inferred: "no data" panels
    int mD4[8];
    atn::Task* mF4[8];          // 0xF4: inferred: "clear" marks
    atn::Task* m114;
    atn::Task* m118;            // 0x118: inferred: left arrow
    int m11C;
    atn::Task* m120;            // 0x120: inferred: right arrow
    int m124;
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
    atn::Task* m178;            // 0x178: inferred: message window
    atn::Texture_unk mTextures[17]; // 0x17C
    ResRef_unk m4AC;
    int m4B4;
    atn::LH_Binary m4B8;
    atn::Buffer_unk<CampSaveLoadEntry_unk> mEntries; // 0x4C0
    float m4C8;
    unsigned char m4CC[0x4D0 - 0x4CC];
};

#endif
