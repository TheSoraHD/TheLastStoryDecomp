// Copyright (c) TheSoraHD. All rights reserved.

namespace atn {

extern "C" {
void GXSetProjection(const float projection[4][4], int type);
void GXSetScissor(unsigned int left, unsigned int top, unsigned int width, unsigned int height);
void GXSetCopyFilter(unsigned char antialias, const unsigned char samples[12][2], unsigned char vertical,
                     const unsigned char filter[7]);
void GXLoadTexObj(const void* obj, unsigned int map);
int GXGetTexObjFmt(const void* obj);
void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short width, unsigned short height);
void GXSetTexCopyDst(unsigned short width, unsigned short height, int format, unsigned char mipmap);
void GXCopyTex(void* destination, unsigned char clear);
void GXPixModeSync();
void __dl__FPv(void* ptr);
}

// inferred: GraphicsDevice field accessed by fn_80071D60.
struct GraphicsDeviceField_unk {
    unsigned char m00[0xCC];
    int mCC;
};

extern "C" void fn_80071D60(GraphicsDeviceField_unk* self, int value) {
    self->mCC = value;
}

extern "C" void fn_80071D68(void* self, const float projection[4][4]) {
    GXSetProjection(projection, 0);
}

extern "C" void fn_80071D74(void* self, const float projection[4][4]) {
    GXSetProjection(projection, 1);
}

extern "C" void fn_800760BC(void* self, unsigned int left, unsigned int top, unsigned int width,
                            unsigned int height) {
    GXSetScissor(left, top, width, height);
}

// inferred: arguments consumed by GXSetScissor at offsets 0x60-0x6C.
struct GraphicsDeviceScissor_unk {
    unsigned char m00[0x60];
    unsigned int m60;
    unsigned int m64;
    unsigned int m68;
    unsigned int m6C;
};

extern "C" void fn_800760D0(GraphicsDeviceScissor_unk* self) {
    GXSetScissor(self->m60, self->m64, self->m68, self->m6C);
}

// inferred: copy-filter settings stored in GraphicsDevice.
struct GraphicsDeviceCopyFilter_unk {
    unsigned char m00[0x19];
    unsigned char m19;
    unsigned char m1A[24];
    unsigned char m32[7];
};

extern "C" void fn_80076174(GraphicsDeviceCopyFilter_unk* self, int enabled) {
    if (enabled) {
        GXSetCopyFilter(self->m19, reinterpret_cast<const unsigned char (*)[2]>(self->m1A), 1, self->m32);
    } else {
        GXSetCopyFilter(0, 0, 0, 0);
    }
}

// inferred: copies a texture object into a GX texture using this object's dimensions.
struct GraphicsDeviceTextureCopy_unk {
    unsigned char m00[0x28];
    void* m28;
    unsigned char m2C[0x10];
    unsigned int m3C;
    unsigned int m40;
};

extern "C" void fn_80071D80(GraphicsDeviceTextureCopy_unk* self, const GraphicsDeviceTextureCopy_unk* texture) {
    GXSetTexCopySrc(0, 0, (unsigned short)self->m3C, (unsigned short)self->m40);
    int format = GXGetTexObjFmt(texture);
    GXSetTexCopyDst((unsigned short)self->m3C, (unsigned short)self->m40, format, 0);
    GXCopyTex(texture->m28, 0);
    GXPixModeSync();
}

// inferred: texture binding cache, with entries beginning at offset 0x80.
struct GraphicsDeviceTextureCache_unk {
    unsigned char m00[0x80];
    const void* m80[1];
};

// NONMATCHING: 72.00% - MWCC keeps the nullable texture in r5 instead of normalizing it through r6.
extern "C" void fn_800763C0(GraphicsDeviceTextureCache_unk* self, unsigned int index, const void* texture) {
    const void** slot = self->m80 + index;
    if (*slot != texture) {
        *slot = texture;
        if (texture != 0) {
            GXLoadTexObj(texture, index);
        }
    }
}

extern "C" void fn_800763FC(GraphicsDeviceTextureCache_unk* self, const void* texture, unsigned int index) {
    const void** slot = self->m80 + index;
    if (*slot != texture) {
        *slot = texture;
        if (texture != 0) {
            GXLoadTexObj(texture, index);
        }
    }
}

// inferred: decrements a reference-counted display-list allocation.
struct GraphicsDeviceDisplayList_unk {
    unsigned char m00[0xE0];
};

extern "C" void fn_80076F88(GraphicsDeviceDisplayList_unk* self, unsigned int index) {
    char* item = reinterpret_cast<char*>(self) + index * 8;
    int* refs = reinterpret_cast<int*>(item + 0xE4);
    void** ptr = reinterpret_cast<void**>(item + 0xE0);
    if (--*refs <= 0) {
        __dl__FPv(*ptr);
        *ptr = 0;
    }
}

// NONMATCHING: 65.57% - loop/control-flow and register allocation differ from the original.
// inferred: compares two cached GX state records (0x30 bytes each) followed by
// the corresponding six-word records at offset 0x1B0.
struct GraphicsDeviceState_unk {
    int m00[3];
    unsigned char m0C;
    int m10[3];
    unsigned char m1C;
    int m20[3];
    unsigned char m2C;
};

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

} // namespace atn
