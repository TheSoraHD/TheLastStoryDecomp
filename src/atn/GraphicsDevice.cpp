// Copyright (c) TheSoraHD. All rights reserved.

#include "atn/GraphicsDevice.h"
#include "atn/Texture_unk.h"

extern "C" {
void GXSetProjection(const float projection[4][4], int type);
void GXSetScissor(unsigned int left, unsigned int top, unsigned int width, unsigned int height);
void GXSetCopyFilter(unsigned char antialias, const unsigned char samples[12][2], unsigned char vertical,
                     const unsigned char filter[7]);
void GXLoadTexObj(const void* obj, unsigned int map);
int GXGetTexObjFmt(const GXTexObj* obj);
void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short width, unsigned short height);
void GXSetTexCopyDst(unsigned short width, unsigned short height, int format, unsigned char mipmap);
void GXCopyTex(void* destination, unsigned char clear);
void GXPixModeSync();
}

void operator delete(void* ptr);

using atn::GraphicsDevice_unk;
using atn::GraphicsDeviceState_unk;

// NONMATCHING: 65.57% - loop/control-flow and register allocation differ from the original.
extern "C" int fn_80071088(const GraphicsDeviceState_unk* a, const GraphicsDeviceState_unk* b) {
    for (int i = 0; i < 9; i++) {
        const GraphicsDeviceState_unk* x = a + i;
        const GraphicsDeviceState_unk* y = b + i;
        if (x->m00[0] != y->m00[0] || x->m00[1] != y->m00[1] || x->m00[2] != y->m00[2] || x->m0C != y->m0C ||
            x->m10[0] != y->m10[0] || x->m10[1] != y->m10[1] || x->m10[2] != y->m10[2] || x->m1C != y->m1C ||
            x->m20[0] != y->m20[0] || x->m20[1] != y->m20[1] || x->m20[2] != y->m20[2] || x->m2C != y->m2C) {
            return 0;
        }
    }

    const int* x = reinterpret_cast<const int*>(a) + (0x1B0 / 4);
    const int* y = reinterpret_cast<const int*>(b) + (0x1B0 / 4);
    for (int i = 0; i < 9 * 6; i++) {
        if (x[i] != y[i]) {
            return 0;
        }
    }
    return 1;
}

extern "C" void fn_80071D60(GraphicsDevice_unk* device, int value) {
    device->mCC = value;
}

extern "C" void fn_80071D68(GraphicsDevice_unk* device, const float projection[4][4]) {
    GXSetProjection(projection, 0);
}

extern "C" void fn_80071D74(GraphicsDevice_unk* device, const float projection[4][4]) {
    GXSetProjection(projection, 1);
}

// inferred: copies the EFB into the texture
extern "C" void fn_80071D80(GraphicsDevice_unk* device, atn::Texture_unk* texture) {
    GXSetTexCopySrc(0, 0, (unsigned short)device->mWidth, (unsigned short)device->mHeight);
    int format = GXGetTexObjFmt(&texture->mObj);
    GXSetTexCopyDst((unsigned short)device->mWidth, (unsigned short)device->mHeight, format, 0);
    GXCopyTex(texture->mImage, 0);
    GXPixModeSync();
}

extern "C" void fn_800760BC(GraphicsDevice_unk* device, unsigned int left, unsigned int top, unsigned int width,
                            unsigned int height) {
    GXSetScissor(left, top, width, height);
}

extern "C" void fn_800760D0(GraphicsDevice_unk* device) {
    GXSetScissor(device->mScissor[0], device->mScissor[1], device->mScissor[2], device->mScissor[3]);
}

extern "C" void fn_80076174(GraphicsDevice_unk* device, int enabled) {
    if (enabled) {
        GXSetCopyFilter(device->mCopyAntialias, device->mCopySamples, 1, device->mCopyVertFilter);
    } else {
        GXSetCopyFilter(0, 0, 0, 0);
    }
}

// NONMATCHING: 72.00% - MWCC keeps the nullable texture in r5 instead of normalizing it through r6.
extern "C" void fn_800763C0(GraphicsDevice_unk* device, unsigned int map, const void* texObj) {
    const void** slot = device->mTexObjs + map;
    if (*slot != texObj) {
        *slot = texObj;
        if (texObj != 0) {
            GXLoadTexObj(texObj, map);
        }
    }
}

extern "C" void fn_800763FC(GraphicsDevice_unk* device, const void* texObj, unsigned int map) {
    const void** slot = device->mTexObjs + map;
    if (*slot != texObj) {
        *slot = texObj;
        if (texObj != 0) {
            GXLoadTexObj(texObj, map);
        }
    }
}

// inferred: releases one reference to a display-list allocation
extern "C" void fn_80076F88(GraphicsDevice_unk* device, unsigned int index) {
    GraphicsDevice_unk::DisplayListRef_unk* ref = &device->mDisplayLists[index];
    if (--ref->mRefs <= 0) {
        ::operator delete(ref->mData);
        ref->mData = 0;
    }
}
