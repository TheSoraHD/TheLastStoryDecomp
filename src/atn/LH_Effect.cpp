#include "atn/LH_Effect.h"
#include "atn/LoadManager.h"
#include "atn/Texture_unk.h"

// inferred: object deleted through its virtual destructor (type 1 entries; probably the model
// class of include/atn/Model_unk.h, which is declared there without its vtable)
struct EffectModel_unk {
    virtual ~EffectModel_unk();
};

// inferred: two textures (type 2 entries), destroyed by the implicit destructor
struct EffectTexturePair_unk {
    atn::Texture_unk m00;
    atn::Texture_unk m30;
};

// inferred: entry of the effect data (0x138 bytes)
struct EffectEntry_unk {
    unsigned char m00[0x104];
    unsigned int mType; // 0x104: 1 = model, 2 = texture pair, otherwise a texture
    unsigned char m108[0x138 - 0x108];
};

// inferred: effect data (0x44 bytes, cleared with memset; functions at 0x80239030/0x802396A0)
struct EffectData_unk {
    EffectData_unk() { memset(this, 0, sizeof(EffectData_unk)); }
    void fn_802396A0(); // inferred: releases the data

    unsigned char m00[8];
    unsigned int mNumEntries;  // 0x08
    EffectEntry_unk* mEntries; // 0x0C
    unsigned char m10[0x34 - 0x10];
    void** mObjects;           // 0x34: one object per entry
    unsigned char m38[0x40 - 0x38];
    const char* mPath;         // 0x40
};

namespace atn {

// inferred from __vt__Q23atn10LoadEffect (0x8078FCD0)
class LoadEffect : public LoadCore {
public:
    LoadEffect(const char* path) : LoadCore(path), mParsed(0) {}
    virtual ~LoadEffect();
    // NONMATCHING: not decompiled yet (0x804732DC: builds the models and textures of the entries)
    virtual void vf0C();

    EffectData_unk mData; // 0x5C
    int mParsed;          // 0xA0
};

// NONMATCHING: the original keeps the object in a saved register (r28) from its load, and the
// offsets of the two arrays swap registers; tried the order of the loads, a switch, int/unsigned
// index, a reference to the entry and an inline delete helper.
LoadEffect::~LoadEffect() {
    for (unsigned int i = 0; i < mData.mNumEntries; i++) {
        void* object = mData.mObjects[i];
        EffectEntry_unk* entry = &mData.mEntries[i];
        if (object != 0) {
            if (entry->mType == 1) {
                delete (EffectModel_unk*)object;
            } else if (entry->mType == 2) {
                delete (EffectTexturePair_unk*)object;
            } else {
                delete (Texture_unk*)object;
            }
            mData.mObjects[i] = 0;
        }
    }
    mData.fn_802396A0();
}

void* LH_Effect::fn_804738C4() {
    if (mCore != 0) {
        return &((LoadEffect*)mCore)->mData;
    }
    return 0;
}

void LH_Effect::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadEffect>(path, 1);
}

} // namespace atn
