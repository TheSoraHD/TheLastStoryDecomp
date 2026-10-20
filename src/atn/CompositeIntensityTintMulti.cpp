#include "atn/Texture_unk.h"
#include "atn/Memory.h"
#include "std/vector.h"

// The cache of composed textures (0x800D81CC-0x800D87A4) comes from another file (own "" pool at
// 0x80734758, own destructor chain entry 0x807C77E8); its object is a function-level static of an inline
// function, so this file constructs it too (chain entry 0x807C7800). The original emits the weak copies
// of its inline helpers (0x800D87A4-0x800D8C14: handle ctor/AddRef/Release/texture, RGBA ctor, texture
// accessors, push_back of the command list...) before the constructor.

struct Camera_unk;
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture
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
inline int fn_800D8870(atn::Texture_unk* tex) {
    if (tex->mImage != 0) {
        return (signed char)tex->m2F;
    }
    return tex->m20.fn_80478198();
}

namespace atn {

// inferred: RGBA color with an out-of-line copy (weak copy 0x800D8BB4)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    Color4f_unk(const Color4f_unk& c) {
        float r_ = c.r;
        float g_ = c.g;
        float b_ = c.b;
        float a_ = c.a;
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};


// inferred: RGBA color of the commands (see atn/TextureComposite.cpp)
struct CompositeColor_unk {
    CompositeColor_unk() {}
    float r, g, b, a;
};

// inferred: description of one layer (0x40 bytes; names invented)
struct CompositeLayerDesc_unk {
    const char* mPath;     // 0x00
    Color4f_unk mColor[3]; // 0x04
    int mUse[3];           // 0x34
};

// inferred: cached composed texture (refcounted; 0x1BC bytes)
struct CompositeCacheEntry_unk {
    void fn_800D8448(); // inferred: add a reference
    void fn_800D8458(); // inferred: release
};

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

// inferred: reference to a cache entry
struct CompositeCacheHandle_unk {
    CompositeCacheHandle_unk() : mEntry(0) {}
    void Release_unk() {
        if (mEntry != 0) {
            mEntry->fn_800D8458();
            mEntry = 0;
        }
    }
    Texture_unk* fn_800D8808() { return (Texture_unk*)((char*)mEntry + 0x18C); } // weak copy 0x800D8808
    CompositeCacheEntry_unk* mEntry;
};

// inferred: renderer interface (see atn/AABBTree.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C() { return 2; }
    virtual void vf10(Camera_unk* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* frustum) { return 1; }
    virtual void vf18() {}
    virtual void vf1C() {}
};

// inferred: see atn/TextureComposite.cpp
class TextureCompositeCommand {
public:
    virtual ~TextureCompositeCommand() {}
    virtual void vf0C() = 0;
};

class TextureCompositeMaskIntensitySingle : public TextureCompositeCommand {
public:
    TextureCompositeMaskIntensitySingle(Texture_unk* a, Texture_unk* b, Texture_unk* c, Color4f_unk color0,
                                        Color4f_unk color1)
        : m04(a), m08(c), m0C(b) {
        m10.r = color0.r;
        m10.g = color0.g;
        m10.b = color0.b;
        m10.a = color0.a;
        m20.r = color1.r;
        m20.g = color1.g;
        m20.b = color1.b;
        m20.a = color1.a;
    }
    virtual ~TextureCompositeMaskIntensitySingle() {}
    virtual void vf0C();

    Texture_unk* m04;
    Texture_unk* m08;
    Texture_unk* m0C;
    CompositeColor_unk m10;
    CompositeColor_unk m20;
};

class TextureCompositeCopy : public TextureCompositeCommand {
public:
    TextureCompositeCopy(Texture_unk* tex, const Color4f_unk& color) : m04(tex) {
        m08.r = color.r;
        m08.g = color.g;
        m08.b = color.b;
        m08.a = color.a;
    }
    virtual ~TextureCompositeCopy() {}
    virtual void vf0C();

    Texture_unk* m04;
    CompositeColor_unk m08;
};

class TextureCompositeCopyAlpha : public TextureCompositeCommand {
public:
    TextureCompositeCopyAlpha(Texture_unk* tex, const Color4f_unk& color) : m04(tex) {
        m08.r = color.r;
        m08.g = color.g;
        m08.b = color.b;
        m08.a = color.a;
    }
    virtual ~TextureCompositeCopyAlpha() {}
    virtual void vf0C();

    Texture_unk* m04;
    CompositeColor_unk m08;
};

struct TextureCompositeCommandList_unk : public std::vector<TextureCompositeCommand*> {};

class TextureComposite : public IRender {
public:
    TextureComposite();
    virtual ~TextureComposite();
    virtual void vf10(Camera_unk* camera);
    virtual int vf14() { return 1; }
    virtual void vf18() {}

    void Request_unk(Texture_unk* texture, int format);
    void fn_800D8C0C(Texture_unk* alpha) { m10 = alpha; }
    void fn_800D8894(TextureCompositeCommand* command) { mCommands.push_back(command); } // weak copy 0x800D8894

    int m04;
    int m08;
    Texture_unk* m0C;
    Texture_unk* m10;
    TextureCompositeCommandList_unk mCommands; // 0x14
};

// inferred: interface of the texture compositors
class ITextureCompositor {
public:
    ITextureCompositor() : m08(0) {}
    virtual ~ITextureCompositor() {}
    virtual int vf0C() = 0;
    virtual int vf10() = 0;
    virtual void vf14() = 0;
    virtual Color4f_unk* vf18(int i) = 0;

    CompositeCacheHandle_unk mHandle; // 0x04
    int m08;
};

// inferred: tints the intensity of several layers and composes them on a base texture
class CompositeIntensityTintMulti : public ITextureCompositor {
public:
    CompositeIntensityTintMulti(const char* base, const CompositeLayerDesc_unk* layers, int count);
    virtual ~CompositeIntensityTintMulti();
    virtual int vf0C();
    virtual int vf10();
    virtual void vf14();
    virtual Color4f_unk* vf18(int i);

    int mDone;                        // 0x0C
    TextureComposite mComposite;      // 0x10
    Color4f_unk mColors[6][3];        // 0x30
    int mUse[6][3];                   // 0x150
    Texture_unk mBase;                // 0x198
    Texture_unk mLayers[6];           // 0x1C8
    Texture_unk mIntensity;           // 0x2E8
    TextureComposite mIntensityComp;  // 0x318
    Texture_unk mAlpha;               // 0x338
    TextureComposite mAlphaComp;      // 0x368
    int mCount;                       // 0x388
};

CompositeIntensityTintMulti::CompositeIntensityTintMulti(const char* base, const CompositeLayerDesc_unk* layers,
                                                         int count)
    : mDone(0), mCount(count) {
    CompositeCacheEntry_unk* entry = CompositeCache_unk::Get_unk().fn_800D8624(base, layers, count);
    if (entry == 0) {
        mHandle.mEntry = CompositeCache_unk::Get_unk().fn_800D8698(base, layers, count);
        mHandle.mEntry->fn_800D8448();
        fn_800D5908(&mBase, base);
        for (int i = 0; i < mCount; i++) {
            mColors[i][0] = layers->mColor[0];
            mUse[i][0] = layers->mUse[0];
            mColors[i][1] = layers->mColor[1];
            mUse[i][1] = layers->mUse[1];
            mColors[i][2] = layers->mColor[2];
            mUse[i][2] = layers->mUse[2];
            if (mUse[i][0] != 0 || mUse[i][1] != 0 || mUse[i][2] != 0) {
                fn_800D5908(&mLayers[i], layers->mPath);
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

// NONMATCHING: 0% - the original is over the inlining budget and calls every inline helper out of line
// (fn_800D8808/14/28/4C/70, fn_800D8894 = TextureComposite::fn_800D8894 with push_back inlined, the
// command constructors, the Color4f_unk copy and fn_800D8C0C); ours inlines them all. Adding the cost of
// one more fn_800D8894 call (or ~10 more fn_800D8870 calls) tips MWCC over the budget, and then the code
// and the weak copy fn_800D8894 (0x2EC) come out as in the original: some inline cost of the original
// source is missing here (texture getters, color constructor and the command constructors are as small
// as their weak copies allow).
void CompositeIntensityTintMulti::vf14() {
    m08 = 1;
    if (mDone != 0) {
        return;
    }
    Texture_unk* target = mHandle.fn_800D8808();
    target->fn_800D5B58(fn_800D8828(&mBase), fn_800D884C(&mBase), 0xE, fn_800D8870(&mBase), "CompositeTexture");
    mIntensity.fn_800D5B58(fn_800D8828(&mBase), fn_800D884C(&mBase), 1, fn_800D8870(&mBase), "BaseTextureIntensity");
    mIntensityComp.Request_unk(&mIntensity, 1);
    mIntensityComp.fn_800D8894(new (6, "", "", 0)
                                           TextureCompositeCopy(&mBase, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    mAlpha.fn_800D5B58(fn_800D8828(&mBase), fn_800D884C(&mBase), 0, fn_800D8870(&mBase), "BaseTextureIntensity");
    mAlphaComp.Request_unk(&mAlpha, 0x20);
    mAlphaComp.fn_800D8894(new (6, "", "", 0)
                                       TextureCompositeCopyAlpha(&mBase, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    mComposite.Request_unk(target, -1);
    mComposite.fn_800D8C0C(&mAlpha);
    mComposite.fn_800D8894(new (6, "", "", 0)
                                       TextureCompositeCopy(&mBase, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    Color4f_unk channels[3] = {
        Color4f_unk(1.0f, 0.0f, 0.0f, 0.0f),
        Color4f_unk(0.0f, 1.0f, 0.0f, 0.0f),
        Color4f_unk(0.0f, 0.0f, 1.0f, 0.0f),
    };
    for (int i = 0; i < mCount; i++) {
        for (int j = 0; j < 3; j++) {
            if (mUse[i][j] != 0) {
                mComposite.fn_800D8894(new (6, "", "", 0) TextureCompositeMaskIntensitySingle(
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
