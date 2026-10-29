#ifndef GAME_NEWCAMPSCENE2_UNK_H
#define GAME_NEWCAMPSCENE2_UNK_H

#include "atn/Light_unk.h"
#include "std/vector.h"

// inferred: light setup of a scene (0x2B4 bytes; constructor 0x800C1D98 and destructor 0x800C1FB4 in
// atn/RenderSystem_unk.cpp, which declares its own view). Name invented.
struct NewCampScene2_unk {
    NewCampScene2_unk();
    ~NewCampScene2_unk();
    // inferred: weak copies 0x80185188, 0x801856A4 and 0x801856AC (game/GameConfig_unk.cpp)
    // NONMATCHING: AddLight_unk 95.3% - the growth path of std::vector::push_back (as in Event/Path.cpp)
    void AddLight_unk(Light_unk light) { mLights.push_back(light); }
    atn::D2A_DrawColor& GetAmbient_unk() { return mAmbient; }
    atn::D2A_DrawColor& GetLS_unk() { return mLS; }

    unsigned char m000[0x00C];
    atn::D2A_DrawColor mAmbient;    // 0x00C: debug menu "Ambient"
    atn::D2A_DrawColor mLS;         // 0x01C: debug menu "LS"
    unsigned char m02C[0x03C - 0x02C];
    int mFogMode;                   // 0x03C
    float mFogStart;                // 0x040
    float mFogEnd;                  // 0x044
    float mFogR;                    // 0x048
    float mFogG;                    // 0x04C
    float mFogB;                    // 0x050
    unsigned char m054[0x058 - 0x054];
    std::vector<Light_unk> mLights; // 0x058
    unsigned char m064[0x2B4 - 0x064];
};

// inferred: light `index` of the scene (atn/RenderSystem_unk.cpp)
extern "C" Light_unk* fn_800C2448(NewCampScene2_unk* scene, int index);

#endif
