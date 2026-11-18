#ifndef ATN_EFFECTTEXTUREPAIR_UNK_H
#define ATN_EFFECTTEXTUREPAIR_UNK_H

#include "atn/Texture_unk.h"

// inferred: two textures (type 2 entries of the effect data, atn/LH_Effect.cpp), destroyed by the implicit
// destructor; atn/EffectSystem_unk.cpp uses the second one (0x30)
struct EffectTexturePair_unk {
    // inferred: one of the two textures is still loading (defined in atn/LH_Effect.cpp: an inline body here
    // would add @N labels to every file that includes this one)
    bool IsLoading_unk();
    atn::Texture_unk m00;
    atn::Texture_unk m30;
};

#endif
