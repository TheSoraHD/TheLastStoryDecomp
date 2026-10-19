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

// inferred: save data object made by fn_803B7944 / fn_803B27F8 (see src/auto_03_803B27AC_text.cpp)
struct CampSaveData_unk {
    unsigned char m000[0xD60];
    int mD60;
    int mD64;
};

// inferred from __vt__12CampSaveLoad (0x8077A030) and its constructor (0x80117B54); base of the
// save and load menus. The destructor is inlined in those of MenuSaveTask and MenuLoadTask.
class CampSaveLoad : public atn::Task {
public:
    CampSaveLoad(atn::Task* parent); // 0x80117B54
    virtual ~CampSaveLoad() { mEntries.Free_unk(); }
    virtual int vf0C();  // 0x80117FE4
    virtual void vf10(); // 0x801185E0
    virtual void vf14(); // 0x801189B4
    virtual void vf1C(); // 0x801191A0
    virtual void vf20(); // 0x801191A4
    void fn_8011B8D4(const wchar_t* text, int length); // inferred: reads the list of entries

    int mMode;                  // 0x48: inferred: 0 save, 1 load
    CampSaveData_unk* m4C;      // 0x4C: inferred: save data of the save menu
    CampSaveData_unk* m50;      // 0x50: inferred: save data of the load menu
    unsigned char m54[0x70 - 0x54];
    atn::Task* m70;             // 0x70: inferred: menu animation task (fn_801F3FF8)
    unsigned char m74[0x128 - 0x74];
    int m128;                   // 0x128: inferred: selected slot
    unsigned char m12C[0x150 - 0x12C];
    int m150;
    unsigned char m154[0x17C - 0x154];
    atn::Texture_unk mTextures[17]; // 0x17C
    ResRef_unk m4AC;
    int m4B4;
    atn::LH_Binary m4B8;
    atn::Buffer_unk<CampSaveLoadEntry_unk> mEntries; // 0x4C0
    float m4C8;
    unsigned char m4CC[0x4D0 - 0x4CC];
};

#endif
