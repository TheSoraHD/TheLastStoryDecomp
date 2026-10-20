// inferred: render target / loaded texture wrapper (atn::Texture_unk, 0x30 bytes). File name invented.
#include "atn/Texture_unk.h"
#include "atn/PostFx.h"
#include <string.h>

// inferred: engine memory manager (class of atn/SmallAllocator_8.cpp; opaque here)
struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, unsigned long align, int heap,
                             const char* file, const char* func, int line, int arg);
extern "C" void fn_80083AD4(MemoryManager_unk* mgr, void* p);
extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                           unsigned char mipmap, unsigned char maxLod);

// inferred: other units declare these as free functions taking the texture (Event/Dialog, game/StrapTask...)
extern "C" void fn_800D58A4(atn::Texture_unk* tex);
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* path);
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path);
extern "C" int fn_800D5A3C(atn::Texture_unk* tex);

// inferred: name used by the callers for the "still loading" check (see include/game/GimmickCommon.h)
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

namespace atn {

Texture_unk::Texture_unk() : mImage(0), m2C(0), m2D(0), m2E(0), m2F(0) {
    memset(&mObj, 0xBAD0F00D, sizeof(mObj));
}

Texture_unk::~Texture_unk() {
    fn_800D58A4(this);
}

} // namespace atn

// inferred: releases the resource and the image buffer
extern "C" void fn_800D58A4(atn::Texture_unk* tex) {
    tex->m20.fn_80473F88();
    void* image = tex->mImage;
    tex->m2C = 0;
    if (image != 0) {
        fn_80083AD4(fn_800827E0(), image);
        tex->mImage = 0;
    }
}

// inferred: starts loading `path`
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* path) {
    tex->m20.vf0C(path);
    tex->m2C = 0;
}

// inferred: loads `path` and waits for it
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path) {
    tex->m20.fn_80473FCC(path);
    tex->m2C = 0;
    if (tex->m20.fn_80473EFC() == 3) {
        tex->m2C = 1;
        tex->m20.fn_80478158(&tex->mObj, tex->m2D, tex->m2E);
    }
}

// inferred: nonzero while loading; initialises the GX texture once loaded
int GimmickUvOwner_unk::fn_800D59B8() {
    atn::Texture_unk* tex = (atn::Texture_unk*)this;
    if (tex->m20.fn_80473F50() != 0) {
        return 1;
    }
    if (tex->m2C == 0 && tex->m20.fn_80473EFC() == 3) {
        tex->m2C = 1;
        tex->m20.fn_80478158(&tex->mObj, tex->m2D, tex->m2E);
    }
    return 0;
}

// inferred: allocates a screen sized RGBA8 texture
// NONMATCHING: 98% - width/height loaded in the opposite order (r29/r30) and the buffer size in r28
// instead of r31; tried locals, a manager local, an array and the non-inline body (~8 variants)
extern "C" int fn_800D5A3C(atn::Texture_unk* tex) {
    return tex->fn_800D5B58(lbl_8087EEE0->mWidth, lbl_8087EEE0->mHeight, 4, 0, 0);
}

namespace atn {

// inferred: allocates an empty texture (`name` is not used)
int Texture_unk::fn_800D5B58(int width, int height, int format, int lods, const char* name) {
    if (mImage != 0) {
        fn_800D58A4(this);
    }
    m2F = lods;
    unsigned int size = GXGetTexBufferSize(width, height, format, 1 < lods, lods);
    mImage = fn_800839EC(fn_800827E0(), size, 0x20, 6, "", "", 0, 0);
    GXInitTexObj(&mObj, mImage, width, height, format, 0, 0, 0);
    GXInitTexObjLOD(&mObj, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    m2C = 1;
    return 1;
}

} // namespace atn
