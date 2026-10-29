#include "atn/LH_Effect.h"
#include "atn/LoadManager.h"
#include "atn/Texture_unk.h"
#include "atn/Model_unk.h"
#include "std/string.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_80239030(void* data, void* effect);       // inferred: parses the effect data
extern "C" void fn_8022D754(void* entry);                   // inferred: prepares an entry
extern "C" std::string fn_8006B0C8(const std::string& path); // inferred: directory of a path
extern "C" std::string fn_8006D008(const std::string& a, const char* b); // inferred: a + b
extern "C" void fn_8006AD24(std::string& path, const std::string& dir); // inferred
extern "C" void fn_8008AD4C(void* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path);        // inferred: loads a texture

// inferred: fn_800D59B8 is a member of the texture class (it follows its constructor), declared for
// now in include/game/GimmickCommon.h under this name: nonzero while the texture is loading
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

// inferred: object deleted through its virtual destructor (type 1 entries; probably the model
// class of include/atn/Model_unk.h, which is declared there without its vtable)
struct EffectModel_unk {
    EffectModel_unk(unsigned int flags); // 0x8008A4E0
    virtual ~EffectModel_unk();
    unsigned char m04[0x214 - 0x4];
};

// inferred: two textures (type 2 entries), destroyed by the implicit destructor
struct EffectTexturePair_unk {
    // inferred: one of the two textures is still loading
    bool IsLoading_unk() { return ((GimmickUvOwner_unk*)&m00)->fn_800D59B8() || ((GimmickUvOwner_unk*)&m30)->fn_800D59B8(); }
    atn::Texture_unk m00;
    atn::Texture_unk m30;
};

// inferred: entry of the effect data (0x138 bytes)
struct EffectEntry_unk {
    char mName[0x80];    // 0x00
    char mTexture[0x84]; // 0x80
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
    virtual void vf0C();

    EffectData_unk mData; // 0x5C
    int mParsed;          // 0xA0
};

// inferred: the object pointer is declared before the loop (it then takes r28 and the counter r27)
// with the type of the texture pair, so that `delete object` needs no temporary and the pointer
// stays in its saved register through the two texture destructors.
LoadEffect::~LoadEffect() {
    EffectTexturePair_unk* object;
    for (unsigned int i = 0; i < mData.mNumEntries; i++) {
        EffectEntry_unk* entry = &mData.mEntries[i];
        object = (EffectTexturePair_unk*)mData.mObjects[i];
        if (object != 0) {
            if (entry->mType == 1) {
                delete (EffectModel_unk*)object;
            } else if (entry->mType == 2) {
                delete object;
            } else {
                delete (Texture_unk*)object;
            }
            mData.mObjects[i] = 0;
        }
    }
    mData.fn_802396A0();
}

// NONMATCHING: 99.9% - only two saved registers swap: the original keeps the path (this + 6) in
// r19 and its length in r20, we get the reverse. Tried the path as a local, as an accessor, a cast
// or a reload of mData.mPath, its position, and other forms of the string constructor (an __init
// helper, traits::length, the length declared first); with the reload the path is a temporary of
// its own but lands below the string address instead.
void LoadEffect::vf0C() {
    if (mState != 2) {
        return;
    }
    if (mParsed != 0) {
        if (mRefCount == 0) {
            fn_80473104(5);
            return;
        }
        bool loading = false;
        for (unsigned int i = 0; i < mData.mNumEntries; i++) {
            EffectEntry_unk* entry = &mData.mEntries[i];
            if (mData.mObjects[i] != 0) {
                if (entry->mType == 1) {
                    if (((atn::Model*)mData.mObjects[i])->fn_8008B140() != 0) {
                        loading = true;
                    }
                } else if (entry->mType == 2) {
                    if (((EffectTexturePair_unk*)mData.mObjects[i])->IsLoading_unk()) {
                        loading = true;
                    }
                } else {
                    if (((GimmickUvOwner_unk*)mData.mObjects[i])->fn_800D59B8() != 0) {
                        loading = true;
                    }
                }
            }
        }
        if (!loading) {
            fn_80473104(3);
        }
        return;
    }
    if (fn_80239030(m54, &mData) == 0) {
        fn_80473130();
        fn_80473104(4);
    }
    const char* path = mPath;
    mData.mPath = path;
    std::string dir = fn_8006B0C8(path);
    for (unsigned int i = 0; i < mData.mNumEntries; i++) {
        EffectEntry_unk* entry = &mData.mEntries[i];
        fn_8022D754(entry);
        std::string name = fn_8006D008(fn_8006D008(dir, "/"), entry->mName);
        if (entry->mType == 1) {
            EffectModel_unk* model = new (7, "", "", 0) EffectModel_unk(0x2000001);
            if (strcmp(entry->mTexture, "") != 0) {
                std::string texture = fn_8006D008(fn_8006D008(dir, "/"), entry->mTexture);
                fn_8008AD4C(model, name.c_str(), texture.c_str());
            } else {
                fn_8008AD4C(model, name.c_str(), 0);
            }
            mData.mObjects[i] = model;
        } else if (entry->mType == 2) {
            EffectTexturePair_unk* pair = new (7, "", "", 0) EffectTexturePair_unk;
            if (strlen(entry->mName) != 0) {
                fn_800D5908(&pair->m00, name.c_str());
            }
            pair->m00.m2D = 1;
            pair->m00.m2E = 1;
            name = fn_8006D008(fn_8006D008(dir, "/"), entry->mTexture);
            fn_800D5908(&pair->m30, name.c_str());
            pair->m30.m2D = 1;
            pair->m30.m2E = 1;
            mData.mObjects[i] = pair;
        } else if (strlen(entry->mName) != 0) {
            fn_8006AD24(name, "texture");
            Texture_unk* texture = new (7, "", "", 0) Texture_unk;
            fn_800D5908(texture, name.c_str());
            texture->m2D = 1;
            texture->m2E = 1;
            mData.mObjects[i] = texture;
        } else {
            mData.mObjects[i] = 0;
        }
    }
    mParsed = 1;
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
