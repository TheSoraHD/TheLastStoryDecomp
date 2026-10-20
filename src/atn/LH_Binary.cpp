#include "atn/LH_Binary.h"
#include "atn/LoadManager.h"
#include "atn/Loader_unk.h"

// inferred: common header of the chunks of a binary resource file (16 bytes). The data of every
// chunk follows its header and holds offsets from the start of the file, turned into pointers by
// fn_804716B8 the first time the file is read (the file tag then becomes 'fixd').
struct BinChunk_unk {
    unsigned int mTag;     // 0x00: four-character code
    unsigned int mVersion; // 0x04
    unsigned int m08;
    unsigned int mSize;    // 0x0C: size of the chunk, header included
};

// inferred: animation track (12 bytes, SceneTrack_unk of include/atn/Light_unk.h)
struct BinTrack_unk {
    unsigned char m00[8];
    void* mKeys; // 0x08
};

// inferred: chunk 'anmn': animation node, up to three groups of three tracks
struct BinAnmNode_unk : BinChunk_unk {
    void* m10;
    unsigned int m14;
    unsigned int mFlags;     // 0x18: one bit per group of tracks
    BinTrack_unk* mTracks;   // 0x1C: points to m20
    BinTrack_unk m20[9];
};

// inferred: chunk 'scen'
struct BinScene_unk : BinChunk_unk {
    int m10;
    void** m14;
    int m18;
    void** m1C;
    int m20;
    void** m24;
};

// inferred: entry of a 'locs' chunk (0x3C bytes); 'loct' entries are 0x30 bytes
struct BinLocEntry_unk {
    void* m00;
    unsigned char m04[0x28 - 0x04];
    int m28;
    void** m2C; // 0x2C: one pointer every 0x10 bytes
    unsigned char m30[0x3C - 0x30];
};

struct BinLocItem_unk {
    void* m00;
    unsigned char m04[0xC];
};

// inferred: chunks 'locs'/'loct'
struct BinLocs_unk : BinChunk_unk {
    // inferred: entry of either kind
    BinLocEntry_unk* GetEntry_unk(int i) {
        if (mTag == 'locs') {
            return &((BinLocEntry_unk*)mEntries)[i];
        }
        return (BinLocEntry_unk*)((char*)mEntries + i * 0x30);
    }

    void* m10;
    int mNumEntries;  // 0x14
    void* mEntries;   // 0x18: points to m1C
    unsigned char m1C[4];
};

// inferred: chunk 'occt'
struct BinOcct_unk : BinChunk_unk {
    unsigned char m10[4];
    void* m14;
    unsigned char m18[4];
    void* m1C;
    unsigned char m20[8];
    void* m28;
};

// inferred: chunk 'modl'
struct BinModel_unk : BinChunk_unk {
    unsigned char m10[0x28 - 0x10];
    int m28;
    void** m2C;
    int m30;
    void** m34;
    void* m38;
    int m3C;
    void** m40;
    int m44;
    void** m48;
    void* m4C;
    void* m50[1];
};

// inferred: chunk 'mtrx'
struct BinMtrx_unk : BinChunk_unk {
    unsigned char m10[8];
    void* m18;
    unsigned char m1C[4];
    void* m20;
    unsigned char m24[4];
    void* m28;
    unsigned char m2C[4];
    void* m30;
};

// inferred: chunk 'mesh'
struct BinMesh_unk : BinChunk_unk {
    void* m10;
    unsigned char m14[4];
    void* m18;
    unsigned char m1C[0x4C - 0x1C];
    int m4C;
    void** m50;
    void* m54; // 0x54: from version 2
};

// inferred: entry of a 'data' chunk (0x14 bytes)
struct BinDataEntry_unk {
    unsigned char m00[4];
    void* m04;
    unsigned char m08[8];
    void* m10;
};

// inferred: chunk 'data'
struct BinData_unk : BinChunk_unk {
    int mNumEntries; // 0x10
    BinDataEntry_unk mEntries[1];
};

// inferred: chunk 'matr'
struct BinMaterial_unk : BinChunk_unk {
    void* m10;
    unsigned char m14[0xE4 - 0x14];
    int mE4;
    void** mE8;
    int mEC;
    void** mF0;
    int mF4;
    void** mF8;
    unsigned char mFC[0xA48 - 0xFC];
    int mA48;
    void** mA4C;
};

// inferred: the other chunks, fixed in place by fn_804716B8
struct BinGeneric_unk : BinChunk_unk {
    void* m10;
    void* m14;
    void* m18;
    void* m1C;
    void* m20;
    void* m24;
    unsigned char m28[4];
    void* m2C;
    unsigned char m30[8];
    void* m38;
    unsigned char m3C[8];
    void* m44;
    unsigned char m48[0x74 - 0x48];
    void* m74;
    void* m78;
};

// inferred: chunk 'camr' (camera of a scene, 15 tracks, see atn/LH_Scene.cpp)
struct BinCamera_unk : BinChunk_unk {
    void* m10;
    unsigned char m14[0xC];
    BinTrack_unk mTracks[15]; // 0x20
};

// inferred: chunk 'lite' (light of a scene, 17 tracks, LightData_unk of include/atn/Light_unk.h)
struct BinLight_unk : BinChunk_unk {
    void* m10;
    unsigned char m14[0x14];
    BinTrack_unk mTracks[17]; // 0x28
};

// inferred: chunk 'lita' (4 tracks)
struct BinLightAnim_unk : BinChunk_unk {
    void* m10;
    unsigned char m14[4];
    BinTrack_unk mTracks[4]; // 0x18
};

// inferred: chunks 'anim'/'omdl': an array of offsets
struct BinArray_unk : BinChunk_unk {
    int mCount;
    void** mItems;
};

struct BinArray2_unk : BinChunk_unk {
    unsigned char m10[4];
    int mCount;
    void** mItems;
};

// inferred: offset from the start of the file to pointer, rounded up so that the pointer is
// 4-byte aligned
template <class T>
inline T* __Fix_unk(const Loader_unk* loader, T* p) {
    if (p == 0) {
        return 0;
    }
    unsigned int offset = (unsigned int)p;
    unsigned int pad = (loader->m00 + offset) & 3;
    if (pad != 0) {
        offset += 4 - pad;
    }
    return (T*)(loader->m00 + offset);
}

// inferred: offset from the start of the file to pointer, unaligned
template <class T>
inline T* __FixRaw_unk(const Loader_unk* loader, T* p) {
    if (p == 0) {
        return 0;
    }
    return (T*)(loader->m00 + (unsigned int)p);
}

namespace atn {

void* LH_Binary::fn_80470580() {
    if (mCore != 0) {
        return ((LoadCore*)mCore)->m54;
    }
    return 0;
}

int LH_Binary::fn_8047059C() {
    if (mCore != 0) {
        return (int)((LoadCore*)mCore)->m50;
    }
    return 0;
}

void LH_Binary::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadCore>(path, 1);
}

} // namespace atn

// NONMATCHING: 99.07% - only registers in the track loop: the original keeps the byte offset in r4
// and the offset read from the track in r6 (here the other way round). Tried a track pointer or
// reference local, a keys local, the counter declared once for both loops.
// inferred: chunk 'anmn'
void fn_804707D4(Loader_unk* loader, const BinChunk_unk* header, BinAnmNode_unk* data) {
    int i;
    data->m10 = __Fix_unk(loader, data->m10);
    unsigned int flags = data->mFlags;
    data->mTracks = data->m20;
    int count = 0;
    for (i = 0; i < 3; i++) {
        if (flags & (1 << i)) {
            count += 3;
        }
    }
    for (i = 0; i < count; i++) {
        data->mTracks[i].mKeys = __Fix_unk(loader, data->mTracks[i].mKeys);
    }
}

// inferred: chunk 'scen'
void fn_804708AC(Loader_unk* loader, const BinChunk_unk* header, BinScene_unk* data) {
    data->m14 = __Fix_unk(loader, data->m14);
    for (int i = 0; i < data->m10; i++) {
        data->m14[i] = __Fix_unk(loader, data->m14[i]);
    }
    data->m1C = __Fix_unk(loader, data->m1C);
    for (int i = 0; i < data->m18; i++) {
        data->m1C[i] = __Fix_unk(loader, data->m1C[i]);
    }
    data->m24 = __Fix_unk(loader, data->m24);
    for (int i = 0; i < data->m20; i++) {
        data->m24[i] = __Fix_unk(loader, data->m24[i]);
    }
}

// inferred: fixes one entry of a 'locs'/'loct' chunk
static inline void __FixLocEntry_unk(const Loader_unk* loader, BinLocEntry_unk* entry) {
    entry->m00 = __FixRaw_unk(loader, entry->m00);
    entry->m2C = __FixRaw_unk(loader, entry->m2C);
    for (int j = 0; j < entry->m28; j++) {
        ((BinLocItem_unk*)entry->m2C)[j].m00 = __FixRaw_unk(loader, ((BinLocItem_unk*)entry->m2C)[j].m00);
    }
}

// NONMATCHING: 98.78% - only registers: the original gives the entry pointer r8, the inner counter r7
// and the item value r6 (here r6/r8/r7). Tried the entry as a local, the helper written out in both
// loops, a template helper, the counters declared at the top.
// inferred: chunks 'locs'/'loct'
void fn_80470A54(Loader_unk* loader, const BinChunk_unk* header, BinLocs_unk* data) {
    bool isLocs = header->mTag == 'locs';
    data->mEntries = data->m1C;
    data->m10 = __FixRaw_unk(loader, data->m10);
    if (isLocs) {
        for (int i = 0; i < data->mNumEntries; i++) {
            __FixLocEntry_unk(loader, &((BinLocEntry_unk*)data->mEntries)[i]);
        }
    } else {
        for (int i = 0; i < data->mNumEntries; i++) {
            __FixLocEntry_unk(loader, data->GetEntry_unk(i));
        }
    }
}

// The ((void)0) lines below stand for release asserts: without them these short functions are
// autoinlined into fn_804716B8, which the original calls out of line (counts found by testing).
// inferred: chunk 'occt'
void fn_80470C20(Loader_unk* loader, const BinChunk_unk* header, BinOcct_unk* data) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    data->m14 = __Fix_unk(loader, data->m14);
    data->m1C = __Fix_unk(loader, data->m1C);
    data->m28 = __Fix_unk(loader, data->m28);
}

// inferred: chunk 'modl'
void fn_80470CC0(Loader_unk* loader, const BinChunk_unk* header, BinModel_unk* data) {
    data->m2C = __Fix_unk(loader, data->m2C);
    for (int i = 0; i < data->m28; i++) {
        data->m2C[i] = __Fix_unk(loader, data->m2C[i]);
    }
    data->m34 = __Fix_unk(loader, data->m34);
    if (data->m34 != 0) {
        for (int i = 0; i < data->m30; i++) {
            data->m34[i] = __Fix_unk(loader, data->m34[i]);
        }
    }
    for (int i = 0; i < data->m30; i++) {
        data->m50[i] = __FixRaw_unk(loader, data->m50[i]);
    }
    data->m38 = __Fix_unk(loader, data->m38);
    data->m40 = __Fix_unk(loader, data->m40);
    for (int i = 0; i < data->m3C; i++) {
        data->m40[i] = __Fix_unk(loader, data->m40[i]);
    }
    data->m48 = __Fix_unk(loader, data->m48);
    data->m4C = __Fix_unk(loader, data->m4C);
    for (int i = 0; i < data->m44; i++) {
        data->m48[i] = __Fix_unk(loader, data->m48[i]);
    }
}

// inferred: chunk 'mtrx'
void fn_80470FA4(Loader_unk* loader, const BinChunk_unk* header, BinMtrx_unk* data) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    data->m18 = __Fix_unk(loader, data->m18);
    data->m20 = __Fix_unk(loader, data->m20);
    data->m28 = __Fix_unk(loader, data->m28);
    data->m30 = __Fix_unk(loader, data->m30);
}

// inferred: chunk 'mesh'
void fn_80471078(Loader_unk* loader, const BinChunk_unk* header, BinMesh_unk* data) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    data->m10 = __Fix_unk(loader, data->m10);
    data->m18 = __Fix_unk(loader, data->m18);
    data->m50 = __Fix_unk(loader, data->m50);
    for (int i = 0; i < data->m4C; i++) {
        data->m50[i] = __Fix_unk(loader, data->m50[i]);
    }
    if (data->mVersion >= 2) {
        data->m54 = __Fix_unk(loader, data->m54);
    }
}

// inferred: chunk 'data'
void fn_804711B0(Loader_unk* loader, const BinChunk_unk* header, BinData_unk* data) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    BinDataEntry_unk* entries = data->mEntries;
    for (int i = 0; i < data->mNumEntries; i++) {
        entries[i].m04 = __Fix_unk(loader, entries[i].m04);
        entries[i].m10 = __Fix_unk(loader, entries[i].m10);
    }
}

// inferred: chunk 'matr'
void fn_8047123C(Loader_unk* loader, const BinChunk_unk* header, BinMaterial_unk* data) {
    data->m10 = __Fix_unk(loader, data->m10);
    data->mE8 = __Fix_unk(loader, data->mE8);
    for (int i = 0; i < data->mE4; i++) {
        data->mE8[i] = __Fix_unk(loader, data->mE8[i]);
    }
    data->mF0 = __Fix_unk(loader, data->mF0);
    for (int i = 0; i < data->mEC; i++) {
        data->mF0[i] = __Fix_unk(loader, data->mF0[i]);
    }
    data->mF8 = __Fix_unk(loader, data->mF8);
    for (int i = 0; i < data->mF4; i++) {
        data->mF8[i] = __Fix_unk(loader, data->mF8[i]);
    }
    data->mA4C = __Fix_unk(loader, data->mA4C);
    for (int i = 0; i < data->mA48; i++) {
        data->mA4C[i] = __Fix_unk(loader, data->mA4C[i]);
    }
}

void* fn_804716B8(Loader_unk* loader, const BinChunk_unk* header, void* data);

// inferred: fixes an array of offsets and its items
template <class T>
inline void __FixArray_unk(const Loader_unk* loader, T* a) {
    a->mItems = __Fix_unk(loader, a->mItems);
    for (int i = 0; i < a->mCount; i++) {
        a->mItems[i] = __Fix_unk(loader, a->mItems[i]);
    }
}

extern "C" void fn_804714A4(Loader_unk* self) {
    self->m00 = 0;
}

extern "C" void fn_804714B0(Loader_unk* self, void* data, void* size, LoaderCallback_unk callback,
                            void* user) {
    self->m00 = (int)data;
    if (*(unsigned int*)data == 'fixd') {
        return;
    }
    char* end = (char*)data + (int)size;
    char* p = (char*)data + 0x10;
    while (p < end) {
        if ((unsigned int)p & 0xF) {
            p += (0x10 - (unsigned int)p) & 0xF;
        }
        char* next = (char*)fn_804716B8(self, (BinChunk_unk*)p, p);
        if (callback) {
            callback(user, (const atn::ChunkHeader*)p, p);
        }
        p = next;
    }
    *(unsigned int*)data = 'fixd';
}

// NONMATCHING: 99.88% - only the 'camr' loop: the original counts in r5 and reads the loader base
// into r7 (here r7 and r5). Tried indexing from the chunk or from a tracks local, a reference, an
// element local, a pointer walk, a down counter, groups of three.
// inferred: fixes the offsets of one chunk and returns the next one
void* fn_804716B8(Loader_unk* loader, const BinChunk_unk* header, void* data) {
    BinGeneric_unk* d = (BinGeneric_unk*)data;
    switch (header->mTag) {
    case 'modl':
        fn_80470CC0(loader, header, (BinModel_unk*)data);
        break;
    case 'strm':
        d->m10 = __Fix_unk(loader, d->m10);
        d->m14 = __Fix_unk(loader, d->m14);
        d->m1C = __Fix_unk(loader, d->m1C);
        break;
    case 'sdsc':
        d->m1C = __Fix_unk(loader, d->m1C);
        break;
    case 'mtrx':
        fn_80470FA4(loader, header, (BinMtrx_unk*)data);
        break;
    case 'mesh':
        fn_80471078(loader, header, (BinMesh_unk*)data);
        break;
    case 'subm':
        d->m38 = __Fix_unk(loader, d->m38);
        break;
    case 'prim':
        d->m18 = __Fix_unk(loader, d->m18);
        break;
    case 'node':
        d->m10 = __Fix_unk(loader, d->m10);
        d->m74 = __Fix_unk(loader, d->m74);
        d->m78 = __Fix_unk(loader, d->m78);
        break;
    case 'data':
        fn_804711B0(loader, header, (BinData_unk*)data);
        break;
    case 'matr':
        fn_8047123C(loader, header, (BinMaterial_unk*)data);
        break;
    case 'tmap':
        d->m14 = __Fix_unk(loader, d->m14);
        break;
    case 'dlst':
        d->m14 = __Fix_unk(loader, d->m14);
        break;
    case 'anim': {
        __FixArray_unk(loader, (BinArray2_unk*)data);
        break;
    }
    case 'anmn':
        fn_804707D4(loader, header, (BinAnmNode_unk*)data);
        break;
    case 'camr': {
        BinCamera_unk* c = (BinCamera_unk*)data;
        c->m10 = __Fix_unk(loader, c->m10);
        BinTrack_unk* t = c->mTracks;
        for (int i = 0; i < 15; i++, t++) {
            t->mKeys = __Fix_unk(loader, t->mKeys);
        }
        break;
    }
    case 'lita':
    {
        BinLightAnim_unk* l = (BinLightAnim_unk*)data;
        l->m10 = __Fix_unk(loader, l->m10);
        for (int i = 0; i < 4; i++) {
            l->mTracks[i].mKeys = __Fix_unk(loader, l->mTracks[i].mKeys);
        }
        break;
    }
    case 'lite': {
        BinLight_unk* l = (BinLight_unk*)data;
        l->m10 = __Fix_unk(loader, l->m10);
        BinTrack_unk* tracks = l->mTracks;
        for (int i = 0; i < 17; i++) {
            tracks[i].mKeys = __Fix_unk(loader, tracks[i].mKeys);
        }
        break;
    }
    case 'scen':
        fn_804708AC(loader, header, (BinScene_unk*)data);
        break;
    case 'text':
        d->m20 = __Fix_unk(loader, d->m20);
        d->m24 = __Fix_unk(loader, d->m24);
        break;
    case 'locs':
    case 'loct':
        fn_80470A54(loader, header, (BinLocs_unk*)data);
        break;
    case 'omdl': {
        __FixArray_unk(loader, (BinArray_unk*)data);
        break;
    }
    case 'occl':
        d->m10 = __Fix_unk(loader, d->m10);
        d->m20 = __Fix_unk(loader, d->m20);
        break;
    case 'occt':
        fn_80470C20(loader, header, (BinOcct_unk*)data);
        break;
    }
    return (char*)data + header->mSize;
}

// NONMATCHING: not decompiled - handwritten paired-single assembly (fres, ps_madds0/ps_sum0 with
// the matrix lbl_8078FBF0 and loads through qr6): inferred hermite interpolation of a track
extern "C" float fn_80471FD0(float t, float dt, const void* p0, const void* p1, const float* matrix);

// NONMATCHING: not decompiled - SceneTrack_unk::fn_8047202C (0x8047202C) sets GQR6 (mtspr) and
// reads the keys with quantized psq_l loads, not expressible without asm.

// inferred: empty function without references, kept by the linker
extern "C" void fn_80472340() {}
