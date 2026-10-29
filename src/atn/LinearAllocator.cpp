#include "atn/Task.h"
#include "atn/MemoryManager.h"
#include "atn/Texture_unk.h"
#include "atn/Memory.h"

// The real file defines atn::PostFxManager (RTTI name at 0x80732AC8), a task that owns the
// post effects and a linear allocator for their temporary textures.

struct Camera_unk;
// inferred: render parameters (fields named by offset)
struct PostFxParams_unk {
    unsigned char m000[0x54];
    int mBloom;      // 0x54
    unsigned char m058[0xD4 - 0x58];
    int mDof;        // 0xD4
    unsigned char m0D8[0x238 - 0xD8];
    float mBloomAlpha; // 0x238
    float mDofAlpha;   // 0x23C
    unsigned char m240[0x324 - 0x240];
    int mColorMatrix; // 0x324
    unsigned char m328[0x374 - 0x328];
    int mDamage;      // 0x374
    unsigned char m378[0x3E4 - 0x378];
    float mGammaAlpha; // 0x3E4
};
extern "C" PostFxParams_unk* lbl_8087EFA8;
extern "C" Camera_unk* lbl_8087EFB4;
// inferred: allocates from a memory manager
extern "C" void* fn_800839EC(atn::MemoryManager* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line, int arg);
extern "C" void fn_800BDB58(Camera_unk* camera, void* render, float alpha, int arg); // inferred: queues a render
extern "C" unsigned long GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                            unsigned char mipmap, unsigned char maxLod);
extern "C" void DCInvalidateRange(void* addr, unsigned long size);

namespace atn {

// inferred: allocator that hands out consecutive blocks of a fixed buffer
class LinearAllocator : public IAllocator {
public:
    LinearAllocator(void* base, unsigned long size) : mBase(base), mSize(size), mUsed(0) {}
    virtual ~LinearAllocator() {}
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name);
    virtual void vf10(void* p) {}
    virtual void* vf14() { return (unsigned char*)mBase + mSize; }
    virtual void* vf18() { return mBase; }
    virtual void vf1C() {}

    void* mBase;         // 0x04
    unsigned long mSize; // 0x08
    unsigned long mUsed; // 0x0C
};

// inferred: renderer interface (see atn/VideoTHP.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10(Camera_unk* camera) = 0;
    virtual int vf14();
    virtual void vf18();
    virtual void vf1C(); // 0x800461FC
};

// inferred: the post effects (constructors and vtables in their own files)
class PostFxBloom : public IRender {
public:
    PostFxBloom(); // 0x800A9E54
    virtual ~PostFxBloom() {}
    virtual void vf10(Camera_unk* camera);
    unsigned char m04[0x44 - 0x04];
};

class PostFxDof : public IRender {
public:
    PostFxDof(); // 0x800AC5F4
    virtual ~PostFxDof() {}
    virtual void vf10(Camera_unk* camera);
    unsigned char m04[0x7C - 0x04];
};

class PostFxMotionBlur : public IRender {
public:
    PostFxMotionBlur(); // 0x800AFE40
    virtual ~PostFxMotionBlur() {}
    virtual void vf10(Camera_unk* camera);
    void fn_800AAE20();
    unsigned char m04[0xB0 - 0x04];
};

class PostFxColorMatrix : public IRender {
public:
    PostFxColorMatrix(); // 0x800AA6C4
    virtual ~PostFxColorMatrix() {}
    virtual void vf10(Camera_unk* camera);
    void fn_800AA8DC();
    void fn_800AAE20();
    unsigned char m04[0x48 - 0x04];
    int m48;
    unsigned char m4C[0xFC - 0x4C];
};

class PostFxGodRay : public IRender {
public:
    PostFxGodRay(); // 0x800AEE9C
    virtual ~PostFxGodRay() {}
    virtual void vf10(Camera_unk* camera);
    int fn_800AEF4C();
    void fn_800AEF68();
    unsigned char m04[0x38 - 0x04];
    Texture_unk m38;
    unsigned char m68[0x78 - 0x68];
};

class PostFxDamage : public IRender {
public:
    PostFxDamage(); // 0x800ABD84
    virtual ~PostFxDamage() {}
    virtual void vf10(Camera_unk* camera);
};

// inferred: element of the rain drop array (constructed with new[])
struct PostFxRainDrop_unk {
    PostFxRainDrop_unk();
    unsigned char m00[0x10];
};

// inferred: array of rain drops (names invented)
struct PostFxRainDrops_unk {
    ~PostFxRainDrops_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mCount = 0;
    }
    int mCount;
    PostFxRainDrop_unk* mData;
};

class PostFxRain : public IRender {
public:
    PostFxRain(); // 0x800B089C
    virtual ~PostFxRain() {}
    virtual void vf10(Camera_unk* camera);
    int fn_800B0A48();
    void fn_800B1554();
    void fn_800B1598();
    unsigned char m04[0x80 - 0x04];
    PostFxRainDrops_unk m80;
    Texture_unk m88;
    Texture_unk mB8;
};

class PostFxGamma : public IRender {
public:
    PostFxGamma(); // 0x800AD0F0
    virtual ~PostFxGamma() {}
    virtual void vf10(Camera_unk* camera);
    void fn_800AD340();
    unsigned char m04[0x138 - 0x04];
    Texture_unk m138;
    unsigned char m168[0x170 - 0x168];
    Texture_unk m170[8];
    Texture_unk m2F0;
    Texture_unk m320;
};

// inferred: owner of the post effects (0x780 bytes)
// note: private base (the RTTI base list flags it with 0x80000000)
class PostFxManager : Task {
public:
    PostFxManager(Task* parent);
    virtual ~PostFxManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static PostFxManager* fn_800A8FF0(Task* parent);
    void fn_800A9558();
    void* fn_800A96AC(int width, int height, int format);
    void fn_800A9788();
    void fn_800A9794();
    void fn_800A97C4();

    LinearAllocator mAlloc;         // 0x048
    PostFxBloom mBloom;             // 0x058
    PostFxDof mDof;                 // 0x09C
    PostFxMotionBlur mMotionBlur;   // 0x118
    PostFxColorMatrix mColorMatrix; // 0x1C8
    PostFxGodRay mGodRay;           // 0x2C4
    PostFxDamage mDamage;           // 0x33C
    PostFxRain mRain;               // 0x340
    PostFxGamma mGamma;             // 0x428
    int m778;                       // 0x778
    unsigned char m77C[0x780 - 0x77C];
};

} // namespace atn

extern "C" atn::PostFxManager* lbl_8087EF8C; // inferred: instance
extern "C" unsigned long lbl_8087EF88;       // inferred: largest use of the temporary buffer

namespace atn {

PostFxManager* PostFxManager::fn_800A8FF0(Task* parent) {
    lbl_8087EF8C = new (6, "", "", 0) PostFxManager(parent);
    return lbl_8087EF8C;
}

int PostFxManager::vf0C() {
    return fn_800D3FA4();
}

void PostFxManager::vf10() {}

void PostFxManager::vf14() {
    mGodRay.fn_800AEF68();
    mColorMatrix.fn_800AA8DC();
    mGamma.fn_800AD340();
    mRain.fn_800B1598();
    fn_800A9558();
}

// note: not inline, its literals are in @stringBase0 (the out-of-line copy is removed by the linker)
PostFxManager::PostFxManager(Task* parent)
    : Task(parent), mAlloc(fn_800839EC(atn::MemoryManager::Instance(), 0x8CA00, 0x20, 6, "", "", 0, 0), 0x8CA00), m778(0xE) {}

PostFxManager::~PostFxManager() {}

void PostFxManager::fn_800A9558() {
    if (lbl_8087EFA8->mBloom != 0) {
        float alpha = lbl_8087EFA8->mBloomAlpha;
        fn_800BDB58(lbl_8087EFB4, &mBloom, alpha, m778);
    }
    if (mGodRay.fn_800AEF4C() != 0) {
        fn_800BDB58(lbl_8087EFB4, &mGodRay, 0.0f, 0xE);
    }
    if (lbl_8087EFA8->mDof != 0) {
        float alpha = lbl_8087EFA8->mDofAlpha;
        fn_800BDB58(lbl_8087EFB4, &mDof, alpha, 0xE);
    }
    fn_800BDB58(lbl_8087EFB4, &mMotionBlur, 0.0f, 0xE);
    if (lbl_8087EFA8->mColorMatrix != 0) {
        fn_800BDB58(lbl_8087EFB4, &mColorMatrix, (float)mColorMatrix.m48, m778);
    }
    fn_800BDB58(lbl_8087EFB4, &mGamma, lbl_8087EFA8->mGammaAlpha, 0xE);
    if (mRain.fn_800B0A48() != 0) {
        fn_800BDB58(lbl_8087EFB4, &mRain, 0.0f, 0xE);
    }
    if (lbl_8087EFA8->mDamage != 0) {
        fn_800BDB58(lbl_8087EFB4, &mDamage, 0.0f, 0xE);
    }
    mAlloc.mUsed = 0;
}

void* PostFxManager::fn_800A96AC(int width, int height, int format) {
    unsigned long size = GXGetTexBufferSize(width, height, format, 0, 0);
    void* p = mAlloc.vf0C(size, 0x20, 6, "TempPostFxTexture");
    DCInvalidateRange(p, size);
    return p;
}

void* LinearAllocator::vf0C(unsigned long size, unsigned long align, int heap, const char* name) {
    unsigned long pad = mUsed % align;
    if (pad != 0) {
        pad = align - pad;
    }
    if (size > mSize - (mUsed + pad)) {
        return 0;
    }
    mUsed += pad;
    void* p = (unsigned char*)mBase + mUsed;
    mUsed += size;
    return p;
}

void PostFxManager::fn_800A9788() {
    mAlloc.mUsed = 0;
}

void PostFxManager::fn_800A9794() {
    // note: with the casts MWCC does not fold size - (size - used)
    unsigned long used = mAlloc.mSize - (unsigned long)((int)mAlloc.mSize - (int)mAlloc.mUsed);
    if (lbl_8087EF88 > used) {
        used = lbl_8087EF88;
    }
    lbl_8087EF88 = used;
    mAlloc.mUsed = 0;
}

void PostFxManager::fn_800A97C4() {
    mColorMatrix.fn_800AAE20();
    mRain.fn_800B1554();
}

} // namespace atn
