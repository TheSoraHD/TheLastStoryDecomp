#include "atn/GXDecl.h"
#include "atn/Memory.h"

struct MemoryManager_unk;

extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                             unsigned char mipmap, unsigned char maxLod);
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, int align, int heap, const char* name,
                              const char* file, int line, int arg7);
extern "C" void DCFlushRange(void* address, unsigned long size);
extern "C" int lbl_8087D8F8;
extern "C" int lbl_8087D8FC;
extern "C" float lbl_80881088;
extern "C" unsigned char lbl_807336B0[];
extern "C" int lbl_8087EFB8;

extern "C" void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short width,
                                  unsigned short height);
extern "C" void GXSetTexCopyDst(unsigned short width, unsigned short height, int format, unsigned char mipmap);
extern "C" void GXCopyTex(void* dest, unsigned char clear);
extern "C" void GXPixModeSync();
extern "C" void GXSetFog(int type, void* color, float start, float end, float nearZ, float farZ);
extern "C" void* lbl_8087EEE0;

extern "C" void fn_800C08B0(void* render) {
    // NONMATCHING: 73.90% - MWCC keeps the render pointer live across the GXSetFog argument setup.
    int* enabled = (int*)((unsigned char*)render + 0x890);
    if (*enabled != 0) {
        *enabled = 0;
        unsigned int color = 0;
        GXSetFog(0, &color, lbl_80881088, lbl_80881088,
                 *(float*)((unsigned char*)render + 0x1CC), *(float*)((unsigned char*)render + 0x1D0));
    }
}

extern "C" void fn_800C0508(void* render, int clear) {
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    GXSetTexCopySrc(0, 0, (unsigned short)width, (unsigned short)height);
    GXSetTexCopyDst((unsigned short)width, (unsigned short)height, 6, 0);
    GXCopyTex(*(void**)((unsigned char*)render + 0x774), (unsigned char)clear);
    GXPixModeSync();
}

extern "C" void fn_800C0590(void* render, int clear, int format) {
    // NONMATCHING: 57.35% - texture initialization differs in register allocation and branch layout.
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    GXSetTexCopySrc(0, 0, (unsigned short)width, (unsigned short)height);
    GXSetTexCopyDst((unsigned short)width, (unsigned short)height, format, 0);
    GXCopyTex(*(void**)((unsigned char*)render + 0x79C), (unsigned char)clear);
    GXPixModeSync();

    GXTexObj* tex = (GXTexObj*)((unsigned char*)render + 0x77C);
    GXInitTexObj(tex, *(void**)((unsigned char*)render + 0x79C), (unsigned short)width, (unsigned short)height,
                 format == 0x16 ? 0x16 : 3, 0, 0, 0);
    GXInitTexObjLOD(tex, 0, 0, lbl_80881088, lbl_80881088, lbl_80881088, 0, 0, 0);
}

extern "C" void fn_800BDB58(void* renderSystem, void* render, float order, int pass) {
    // NONMATCHING: 61.75% - queue bounds and node updates still differ from the reference.
    unsigned int passOffset = (unsigned int)pass * 4;
    unsigned char* system = (unsigned char*)renderSystem;
    if (*(void**)(system + passOffset) == 0) {
        return;
    }

    int tail = *(int*)(system + 0xF8);
    int next = tail + 0x10;
    if (next > *(int*)(system + 0xF4)) {
        return;
    }
    *(int*)(system + 0xF8) = next;
    unsigned char* entry = system + *(int*)(system + 0xF0) + tail;
    *(void**)entry = render;
    *(float*)(entry + 4) = order;
    *(void**)(entry + 8) = *(void**)(system + passOffset + 0x50);
    *(void**)(system + passOffset + 0x50) = entry;
    ++*(int*)(system + passOffset + 0xA0);
}
extern "C" int fn_800B5994(void* value);
extern "C" float fn_800B6218(void* value);

extern "C" int fn_800C0A50(void** value) {
    // NONMATCHING: 72.67% - MWCC lowers this nullable handle check through a different boolean path.
    if (*value == 0) {
        return 1;
    }
    return !fn_800B5994(*value);
}

extern "C" void* fn_800C0A8C(void* value) {
    return (unsigned char*)*(void**)((unsigned char*)value + 0x10) + 0x5C;
}

extern "C" void* fn_800C0A98(void* value) {
    return (unsigned char*)*(void**)((unsigned char*)value + 0x14) + 0x5C;
}

extern "C" void fn_800BDB40(void* render, int value) {
    int* field = (int*)((unsigned char*)render + 0x2FC);
    *field = value;
    if (value != 0) {
        return;
    }
    *field = lbl_8087EFB8;
}

extern "C" void fn_800BFAC4() {}

extern "C" int fn_800C16B4() {
    return lbl_8087EFB8;
}

extern "C" void fn_800C16BC() {}

extern "C" float fn_800C169C(void** value) {
    if (*value != 0) {
        return fn_800B6218(*value);
    }
    return lbl_80881088;
}

extern "C" void fn_800C1660(void* render, int first, int second) {
    // NONMATCHING: 73.33% - the reference stores its by-value pair through stack temporaries.
    int* count = (int*)((unsigned char*)render + 0x910);
    unsigned char* entry = (unsigned char*)render + 0x914 + *count * 8;
    if (entry != 0) {
        *(int*)entry = first;
        *(int*)(entry + 4) = second;
    }
    ++*count;
}

extern "C" void fn_800BC468(void* render, GXTexObj* tex) {
    // NONMATCHING: 80.87% - the reference unrolls the tiled gradient loop two pixels at a time.
    unsigned int size = GXGetTexBufferSize(256, 1, 0, 0, 0);
    unsigned char* image;
    if (*(void**)((unsigned char*)render + 0x550) == 0) {
        image = (unsigned char*)fn_800839EC(fn_800827E0(), size, 0x20, 6, (const char*)lbl_807336B0,
                                             (const char*)lbl_807336B0, 0, 0);
    } else {
        image = (unsigned char*)*(void**)((unsigned char*)render + 0x550);
    }

    for (int i = 0; i < 256; ++i) {
        int value = (i - 128) * 4;
        if (value < lbl_8087D8F8) {
            value = lbl_8087D8F8;
        } else if (value > lbl_8087D8FC) {
            value = lbl_8087D8FC;
        }
        int offset = (i & 7) + ((i & 0x7C) << 2);
        image[offset] = (unsigned char)value;
        image[offset + 8] = (unsigned char)value;
        image[offset + 16] = (unsigned char)value;
        image[offset + 24] = (unsigned char)value;

    }

    GXInitTexObj(tex, image, 256, 1, 1, 0, 1, 0);
    GXInitTexObjLOD(tex, 1, 1, lbl_80881088, lbl_80881088, lbl_80881088, 0, 0, 0);
    DCFlushRange(image, size);
}
