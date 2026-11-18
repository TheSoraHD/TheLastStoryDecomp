#include "atn/Texture_unk.h"
#include "atn/Color.h"
#include "atn/CompositeCache_unk.h"
#include "atn/Memory.h"
#include "atn/IRender.h"
#include "std/vector.h"
#include "atn/TextureComposite.h"
#include "atn/CompositeIntensityTintMulti.h"

// The cache of composed textures (0x800D81CC-0x800D87A4) comes from another file (own "" pool at
// 0x80734758, own destructor chain entry 0x807C77E8); its object is a function-level static of an inline
// function, so this file constructs it too (chain entry 0x807C7800). The original emits the weak copies
// of its inline helpers (0x800D87A4-0x800D8C14: handle ctor/AddRef/Release/texture, RGBA ctor, texture
// accessors, push_back of the command list...) before the constructor.

// inferred: fn_800D59B8 is a member of the texture class (see game/StrapTask.cpp)
struct GimmickUvOwner_unk {
    int fn_800D59B8(); // inferred: still loading
};
// inferred: texture accessors (inline; weak copies 0x800D8828, 0x800D884C, 0x800D8870)
inline int fn_800D8828(atn::Texture_unk* tex) {
    return GXGetTexObjWidth(&tex->mObj);
}
inline int fn_800D884C(atn::Texture_unk* tex) {
    return GXGetTexObjHeight(&tex->mObj);
}
// inferred: asserts of the debug build, empty in release: they only count for the inline budget (with them
// vf14 goes over it and calls every inline out of line, as the original; about 80 are needed)
#define TEX_ASSERT_UNK(x) ((void)0)
inline int fn_800D8870(atn::Texture_unk* tex) {
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0); TEX_ASSERT_UNK(tex != 0);
    if (tex->mImage != 0) {
        return (signed char)tex->m2F;
    }
    return tex->m20.fn_80478198();
}

namespace atn {

// atn::Color4f_unk is atn::Color (atn/Color.h)


// inferred: cache of composed textures (an MSL std::list in a function-level static; names invented)
struct CompositeCacheNode_unk {
    CompositeCacheNode_unk* mPrev;
    CompositeCacheNode_unk* mNext;
};
struct CompositeCacheList_unk {
    CompositeCacheList_unk() {
        CompositeCacheList_unk* self = this;
        self->mSize = 0;
        CompositeCacheNode_unk* tail = &self->mTail;
        tail->mNext = tail;
        tail->mPrev = tail;
    }
    unsigned int mSize;
    CompositeCacheNode_unk mTail;
};
class CompositeCache_unk {
public:
    ~CompositeCache_unk(); // 0x800D8568
    CompositeCacheEntry_unk* fn_800D8624(const char* base, const CompositeLayerDesc_unk* layers, int count);
    CompositeCacheEntry_unk* fn_800D8698(const char* base, const CompositeLayerDesc_unk* layers, int count);
    static CompositeCache_unk& Get_unk() {
        static CompositeCache_unk sCache;
        return sCache;
    }

    CompositeCacheList_unk mList;
};

// Reference to a cache entry and compositor interface: atn/TextureCompositor_unk.h. The handle's
// constructor, AddRef_unk and Release_unk are plain functions of this file (Model.cpp calls them out of
// line); the texture accessor is inline (weak copy 0x800D8808).
inline Texture_unk* CompositeCacheHandle_unk::fn_800D8808() {
    return &mEntry->mTexture;
}

CompositeCacheHandle_unk::CompositeCacheHandle_unk() : mEntry(0) {}

void CompositeCacheHandle_unk::AddRef_unk() const {
    if (mEntry != 0) {
        mEntry->fn_800D8448();
    }
}

void CompositeCacheHandle_unk::Release_unk() {
    if (mEntry != 0) {
        mEntry->fn_800D8458();
        mEntry = 0;
    }
}

inline ITextureCompositor::ITextureCompositor() : m08(0) {}
inline ITextureCompositor::~ITextureCompositor() {}

CompositeIntensityTintMulti::CompositeIntensityTintMulti(const char* base, const CompositeLayerDesc_unk* layers,
                                                         int count)
    : mDone(0), mCount(count) {
    CompositeCacheEntry_unk* entry = CompositeCache_unk::Get_unk().fn_800D8624(base, layers, count);
    if (entry == 0) {
        mHandle.mEntry = CompositeCache_unk::Get_unk().fn_800D8698(base, layers, count);
        mHandle.mEntry->fn_800D8448();
        mBase.fn_800D5908(base);
        for (int i = 0; i < mCount; i++) {
            mColors[i][0] = layers->mColor[0];
            mUse[i][0] = layers->mUse[0];
            mColors[i][1] = layers->mColor[1];
            mUse[i][1] = layers->mUse[1];
            mColors[i][2] = layers->mColor[2];
            mUse[i][2] = layers->mUse[2];
            if (mUse[i][0] != 0 || mUse[i][1] != 0 || mUse[i][2] != 0) {
                mLayers[i].fn_800D5908(layers->mPath);
            }
            layers++;
        }
    } else {
        mHandle.mEntry = entry;
        entry->fn_800D8448();
        mDone = 1;
    }
}

CompositeIntensityTintMulti::~CompositeIntensityTintMulti() {
    mHandle.Release_unk();
}

int CompositeIntensityTintMulti::vf0C() {
    if (mDone == 0) {
        int loading = ((GimmickUvOwner_unk*)&mBase)->fn_800D59B8();
        for (int i = 0; i < mCount; i++) {
            loading |= ((GimmickUvOwner_unk*)&mLayers[i])->fn_800D59B8();
        }
        return loading;
    }
    return 0;
}

// over the inline budget as the original (empty asserts in fn_800D8870): every inline helper is called out
// of line (fn_800D8808/28/4C/70, fn_800D8894, the command constructors, the Color4f_unk copy, fn_800D8C0C)
void CompositeIntensityTintMulti::vf14() {
    m08 = 1;
    if (mDone != 0) {
        return;
    }
    Texture_unk* target = mHandle.fn_800D8808();
    target->fn_800D5B58(fn_800D8828(&mBase), fn_800D884C(&mBase), 0xE, fn_800D8870(&mBase), "CompositeTexture");
    mIntensity.fn_800D5B58(fn_800D8828(&mBase), fn_800D884C(&mBase), 1, fn_800D8870(&mBase), "BaseTextureIntensity");
    mIntensityComp.Request_unk(&mIntensity, 1);
    mIntensityComp.fn_800D8894(new ((MemoryTag)6, "", "", 0)
                                           TextureCompositeCopy(&mBase, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    mAlpha.fn_800D5B58(fn_800D8828(&mBase), fn_800D884C(&mBase), 0, fn_800D8870(&mBase), "BaseTextureIntensity");
    mAlphaComp.Request_unk(&mAlpha, 0x20);
    mAlphaComp.fn_800D8894(new ((MemoryTag)6, "", "", 0)
                                       TextureCompositeCopyAlpha(&mBase, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    mComposite.Request_unk(target, -1);
    mComposite.fn_800D8C0C(&mAlpha);
    mComposite.fn_800D8894(new ((MemoryTag)6, "", "", 0)
                                       TextureCompositeCopy(&mBase, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    Color4f_unk channels[3] = {
        Color4f_unk(1.0f, 0.0f, 0.0f, 0.0f),
        Color4f_unk(0.0f, 1.0f, 0.0f, 0.0f),
        Color4f_unk(0.0f, 0.0f, 1.0f, 0.0f),
    };
    for (int i = 0; i < mCount; i++) {
        for (int j = 0; j < 3; j++) {
            if (mUse[i][j] != 0) {
                mComposite.fn_800D8894(new ((MemoryTag)6, "", "", 0) TextureCompositeMaskIntensitySingle(
                    &mBase, &mIntensity, &mLayers[i], mColors[i][j], channels[j]));
            }
        }
    }
}

int CompositeIntensityTintMulti::vf10() {
    mDone = (mDone != 0 || mComposite.m04 != 0) ? 1 : 0;
    return mDone;
}

Color4f_unk* CompositeIntensityTintMulti::vf18(int i) {
    return &mColors[0][i];
}

} // namespace atn
