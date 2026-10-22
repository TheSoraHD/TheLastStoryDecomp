#include "atn/SwapDesc_unk.h"
#include "atn/Memory.h"

// Loader of the effect resources (.eff files, big endian with offsets relative to the file):
// byte swap, relocation, unit conversion and the default flags of old versions. Name of the file
// invented (its pool has "old eff version!").

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" unsigned int fn_802386FC(struct EffectResData_unk* res, int frames); // inferred: (atn/EffectRes_unk.cpp)
extern "C" void fn_80234A8C();

// inferred: debug message window (see gmk/RandomMerchant.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

// inferred: byte reverse of a 32-bit value (the stores of it are compiled as stwbrx)
#define SWAP32_UNK(v) (((v) << 24) | (((v) >> 8) & 0xFF00) | ((v) >> 24) | (((v) << 8) & 0xFF0000))

// inferred: swap descriptions of the parts of the file
static SwapDesc_unk sHeaderDesc[] = {{0, 4, 0}, {2, 8, 0}, {0, 4, 0}, {2, 2, 0}, {2, 2, 0}, {2, 2, 0}, {-1, 0, 0}};
static SwapDesc_unk sEmitterSubDesc[] = {{2, 4, 0}, {2, 4, 0}, {2, 4, 0}, {2, 4, 0}, {2, 4, 0}, {-1, 0, 0}};
static SwapDesc_unk sEmitterDesc[] = {{0, 0x40, 0}, {2, 1, 0},  {2, 10, 0}, {2, 14, 0}, {2, 15, 0},
                                      {2, 15, 0},   {2, 15, 0}, {2, 15, 0}, {4, 1, sEmitterSubDesc},
                                      {2, 5, 0},    {2, 15, 0}, {2, 3, 0},  {2, 7, 0},  {2, 4, 0},
                                      {-1, 0, 0}};
static SwapDesc_unk sTextureSubDesc[] = {{0, 0x100, 0}, {2, 1, 0}, {2, 1, 0}, {-1, 0, 0}};
static SwapDesc_unk sTextureDesc[] = {{4, 1, sTextureSubDesc}, {2, 12, 0}, {-1, 0, 0}};
static SwapDesc_unk sDefSubDesc[] = {{2, 1, 0}, {2, 3, 0}, {2, 2, 0}, {2, 4, 0}, {2, 4, 0}, {-1, 0, 0}};
static SwapDesc_unk sDefDesc[] = {{2, 1, 0}, {0, 0x20, 0}, {2, 1, 0}, {2, 1, 0}, {4, 4, sDefSubDesc}, {-1, 0, 0}};
static SwapDesc_unk sWordDesc[] = {{2, 1, 0}, {-1, 0, 0}};
static SwapDesc_unk sKeyDesc[] = {{2, 2, 0}, {-1, 0, 0}};

// inferred: key of a curve
struct EffectKeyData_unk {
    float mTime;
    float mValue;
};
// inferred: curve
struct EffectCurveData_unk {
    unsigned int mCount;
    EffectKeyData_unk* mKeys;
};

// inferred: emitter (0x26C bytes, fields named by offset)
struct EffectEmitterData_unk {
    unsigned char m00[0xBC];
    float mBC[3];  // 0xBC: inferred: 0 when unused
    float mC8[3];  // 0xC8: inferred: 1 when unused (scale)
    unsigned char mD4[0xE0 - 0xD4];
    float mE0[3];  // 0xE0: degrees
    unsigned char mEC[0xF8 - 0xEC];
    float mF8[3];  // 0xF8: degrees
    float m104[3];
    float m110[3]; // 0x110: degrees
    unsigned char m11C[0x134 - 0x11C];
    float m134[3];
    float m140[3];
    unsigned char m14C[0x170 - 0x14C];
    float m170[3];
    float m17C[3];
    unsigned char m188[0x194 - 0x188];
    float m194[3]; // 0x194: color 0..255
    float m1A0;
    unsigned char m1A4[0x1B4 - 0x1A4];
    float m1B4[3]; // 0x1B4: color 0..255
    float m1C0;
    float m1C4[4];
    float m1D4[3]; // 0x1D4: color 0..255
    float m1E0;
    unsigned char m1E4[0x1EC - 0x1E4];
    float m1EC;
    float m1F0;
    unsigned char m1F4[4];
    float m1F8[3]; // 0x1F8: degrees
    unsigned char m204[0x210 - 0x204];
    float m210[3]; // 0x210: degrees
    float m21C[3];
    float m228[3]; // 0x228: degrees
    unsigned char m234[0x240 - 0x234];
    int m240;
    unsigned char m244[0x26C - 0x244];
};

// inferred: effect resource (see atn/EffectRes_unk.cpp)
struct EffectResData_unk {
    unsigned int mCount;                // 0x00: emitters
    EffectEmitterData_unk* mEmitters;   // 0x04
    unsigned int mTextureCount;         // 0x08
    unsigned char* mTextures;           // 0x0C: 0x138 bytes each
    unsigned int mDefCount;             // 0x10
    unsigned char* mDefs;               // 0x14: 0x110 bytes each
    unsigned int* mEmitterFlags;        // 0x18
    EffectCurveData_unk (*mEmitterCurves)[22]; // 0x1C
    unsigned int* mDefFlags;            // 0x20
    EffectCurveData_unk (*mDefCurves)[13]; // 0x24
    int* mEmitterEnabled;               // 0x28
    int* mDefEnabled;                   // 0x2C
    unsigned int* m30;                  // 0x30: inferred: unused channels of the emitters
    unsigned int* m34;
    unsigned int mLength;               // 0x38
    unsigned int mVersion;              // 0x3C
};

// inferred: header of the file (offsets relative to the start)
struct EffectFileHeader_unk {
    unsigned char m00[4];
    unsigned int mVersion;   // 0x04
    unsigned char m08[4];
    unsigned int mCount;     // 0x0C: emitters
    unsigned int mTextureCount; // 0x10
    unsigned int mTextures;  // 0x14
    unsigned int mEmitters;  // 0x18
    unsigned int mDefCount;  // 0x1C
    unsigned int mDefs;      // 0x20
    unsigned char m24[4];
    unsigned int mEmitterFlags;  // 0x28
    unsigned int mEmitterCurves; // 0x2C
    unsigned int mEmitterEnabled; // 0x30
    unsigned int mDefEnabled;    // 0x34
    unsigned int mDefFlags;      // 0x38
    unsigned int mDefCurves;     // 0x3C
    unsigned int m40;            // 0x40: version > 0x23
    unsigned int m44;
};

// inferred: degrees to radians and colors to 0..1 (curves included)
extern "C" void fn_80238808(EffectResData_unk* res) {
    for (unsigned int i = 0; i < res->mCount; i++) {
        EffectEmitterData_unk* em = &res->mEmitters[i];
        em->mE0[0] = 0.017453292f * em->mE0[0];
        em->mE0[1] = 0.017453292f * em->mE0[1];
        em->mE0[2] = 0.017453292f * em->mE0[2];
        em->m110[0] = 0.017453292f * em->m110[0];
        em->m110[1] = 0.017453292f * em->m110[1];
        em->m110[2] = 0.017453292f * em->m110[2];
        em->mF8[0] = 0.017453292f * em->mF8[0];
        em->mF8[1] = 0.017453292f * em->mF8[1];
        em->mF8[2] = 0.017453292f * em->mF8[2];
        for (unsigned int c = 15; c <= 17; c++) {
            EffectKeyData_unk* keys = res->mEmitterCurves[i][c].mKeys;
            unsigned int n = res->mEmitterCurves[i][c].mCount;
            for (unsigned int k = 0; k < n; k++) {
                keys[k].mValue = 0.017453292f * keys[k].mValue;
            }
        }
        em->m1F8[0] = 0.017453292f * em->m1F8[0];
        em->m1F8[1] = 0.017453292f * em->m1F8[1];
        em->m1F8[2] = 0.017453292f * em->m1F8[2];
        em->m228[0] = 0.017453292f * em->m228[0];
        em->m228[1] = 0.017453292f * em->m228[1];
        em->m228[2] = 0.017453292f * em->m228[2];
        em->m210[0] = 0.017453292f * em->m210[0];
        em->m210[1] = 0.017453292f * em->m210[1];
        em->m210[2] = 0.017453292f * em->m210[2];
        for (unsigned int c = 19; c <= 21; c++) {
            EffectKeyData_unk* keys = res->mEmitterCurves[i][c].mKeys;
            unsigned int n = res->mEmitterCurves[i][c].mCount;
            for (unsigned int k = 0; k < n; k++) {
                keys[k].mValue = 0.017453292f * keys[k].mValue;
            }
        }
        em->m1A0 /= 255.0f;
        em->m194[0] /= 255.0f;
        em->m194[1] /= 255.0f;
        em->m194[2] /= 255.0f;
        em->m1E0 /= 255.0f;
        em->m1D4[0] /= 255.0f;
        em->m1D4[1] /= 255.0f;
        em->m1D4[2] /= 255.0f;
        em->m1C0 /= 255.0f;
        em->m1B4[0] /= 255.0f;
        em->m1B4[1] /= 255.0f;
        em->m1B4[2] /= 255.0f;
        for (unsigned int c = 3; c <= 6; c++) {
            EffectKeyData_unk* keys = res->mEmitterCurves[i][c].mKeys;
            unsigned int n = res->mEmitterCurves[i][c].mCount;
            for (unsigned int k = 0; k < n; k++) {
                keys[k].mValue /= 255.0f;
            }
        }
    }
}

// inferred: the channels of the emitters left at their default values (old versions compute them)
extern "C" void fn_80238CA0(EffectFileHeader_unk* header, EffectResData_unk* res) {
    if (header->mVersion <= 0x23) {
        res->m30 = new (7, "", "", 0) unsigned int[res->mCount];
        for (unsigned int i = 0; i < res->mCount; i++) {
            EffectEmitterData_unk* em = &res->mEmitters[i];
            res->m30[i] = 0;
            if ((res->mEmitterFlags[i] & 1) != 1 && 0.0f == em->mBC[0] && 0.0f == em->mBC[1] && 0.0f == em->mBC[2] &&
                1.0f == em->mC8[0] && 1.0f == em->mC8[1] && 1.0f == em->mC8[2]) {
                res->m30[i] |= 1;
            }
            if ((res->mEmitterFlags[i] & 2) != 2 && 0.0f == em->mF8[0] && 0.0f == em->mF8[1] && 0.0f == em->mF8[2] &&
                1.0f == em->m104[0] && 1.0f == em->m104[1] && 1.0f == em->m104[2]) {
                res->m30[i] |= 2;
            }
            if ((res->mEmitterFlags[i] & 4) != 4 && 0.0f == em->m134[0] && 0.0f == em->m134[1] &&
                0.0f == em->m134[2] && 1.0f == em->m140[0] && 1.0f == em->m140[1] && 1.0f == em->m140[2]) {
                res->m30[i] |= 4;
            }
            if ((res->mEmitterFlags[i] & 8) != 8 && em->m240 == 0 && 0.0f == em->m170[0] && 0.0f == em->m170[1] &&
                0.0f == em->m170[2] && 1.0f == em->m17C[0] && 1.0f == em->m17C[1] && 1.0f == em->m17C[2]) {
                res->m30[i] |= 8;
            }
            if ((res->mEmitterFlags[i] & 0x10) != 0x10 && 0.0f == em->m1B4[0] && 0.0f == em->m1B4[1] &&
                0.0f == em->m1B4[2] && 0.0f == em->m1C0 && 1.0f == em->m1C4[0] && 1.0f == em->m1C4[1] &&
                1.0f == em->m1C4[2] && 1.0f == em->m1C4[3]) {
                res->m30[i] |= 0x10;
            }
            if ((res->mEmitterFlags[i] & 0x40) != 0x40 && 0.0f == em->m1EC && 1.0f == em->m1F0) {
                res->m30[i] |= 0x20;
            }
            if ((res->mEmitterFlags[i] & 0x80) != 0x80 && 0.0f == em->m210[0] && 0.0f == em->m210[1] &&
                0.0f == em->m210[2] && 1.0f == em->m21C[0] && 1.0f == em->m21C[1] && 1.0f == em->m21C[2]) {
                res->m30[i] |= 0x40;
            }
        }
    } else {
        res->m30 = (unsigned int*)header->m40;
    }
}

// inferred: builds the resource from the loaded file; 0 for unsupported versions
// NONMATCHING: 93.20% - register allocation (the original keeps the header in r26 and the resource
// in r27, here r27/r28) and the schedule of the relocations that follows from it; the old-version
// curve tables are allocated as new T[n][22] (one mulli by 0xB0 as in the original)
extern "C" int fn_80239030(EffectFileHeader_unk* header, EffectResData_unk* res) {
    fn_802371BC((unsigned char*)header, sHeaderDesc);
    if (header->mVersion < 0x22) {
        return 0;
    }
    if (header->mVersion < 0x24 && lbl_8087EEB8 != 0) {
        lbl_8087EEB8->fn_800697D8("old eff version!");
    }
    res->mVersion = header->mVersion;
    header->mEmitterFlags += (unsigned int)header;
    header->mEmitterCurves += (unsigned int)header;
    header->mEmitterEnabled += (unsigned int)header;
    header->mDefEnabled += (unsigned int)header;
    header->mDefFlags += (unsigned int)header;
    header->mDefCurves += (unsigned int)header;
    unsigned char* textures = (unsigned char*)(header->mTextures + (unsigned int)header);
    EffectEmitterData_unk* emitters = (EffectEmitterData_unk*)(header->mEmitters + (unsigned int)header);
    unsigned char* defs = (unsigned char*)(header->mDefs + (unsigned int)header);
    if (res->mVersion > 0x23) {
        unsigned int v = header->m40;
        unsigned int w = header->m44;
        header->m40 = SWAP32_UNK(v) + (unsigned int)header;
        header->m44 = SWAP32_UNK(w) + (unsigned int)header;
    }
    res->mEmitters = emitters;
    for (unsigned int i = 0; i < header->mCount; i++) {
        fn_802371BC((unsigned char*)&res->mEmitters[i], sEmitterDesc);
    }
    res->mCount = header->mCount;
    res->mTextures = textures;
    for (unsigned int i = 0; i < header->mTextureCount; i++) {
        fn_802371BC(res->mTextures + i * 0x138, sTextureDesc);
        if (header->mVersion <= 0x22) {
            res->mTextures[i * 0x138 + 0x80] = 0;
        }
    }
    res->mTextureCount = header->mTextureCount;
    res->mDefs = defs;
    for (unsigned int i = 0; i < header->mDefCount; i++) {
        fn_802371BC(res->mDefs + i * 0x110, sDefDesc);
    }
    res->mDefCount = header->mDefCount;
    res->mEmitterFlags = (unsigned int*)header->mEmitterFlags;
    for (unsigned int i = 0; i < res->mCount; i++) {
        fn_802371BC((unsigned char*)&res->mEmitterFlags[i], sWordDesc);
    }
    if (res->mVersion <= 0x23) {
        unsigned int* src = (unsigned int*)header->mEmitterCurves;
        res->mEmitterCurves = new (7, "", "", 0) EffectCurveData_unk[res->mCount][22];
        for (unsigned int i = 0; i < res->mCount; i++) {
            for (int c = 0; c < 22; c++) {
                unsigned int n = *src;
                EffectCurveData_unk* curve = &res->mEmitterCurves[i][c];
                curve->mCount = SWAP32_UNK(n);
                src++;
                curve->mKeys = new (7, "", "", 0) EffectKeyData_unk[SWAP32_UNK(n)];
                memcpy(curve->mKeys, src, curve->mCount * sizeof(EffectKeyData_unk));
                for (unsigned int k = 0; k < curve->mCount; k++) {
                    fn_802371BC((unsigned char*)&curve->mKeys[k], sKeyDesc);
                }
                src += curve->mCount * 2;
            }
        }
    } else {
        res->mEmitterCurves = (EffectCurveData_unk(*)[22])header->mEmitterCurves;
        for (unsigned int i = 0; i < res->mCount; i++) {
            for (int c = 0; c < 22; c++) {
                EffectCurveData_unk* curve = &res->mEmitterCurves[i][c];
                unsigned int n = curve->mCount;
                curve->mCount = SWAP32_UNK(n);
                unsigned int keys = (unsigned int)curve->mKeys;
                curve->mKeys = (EffectKeyData_unk*)(SWAP32_UNK(keys) + (unsigned int)header);
                for (unsigned int k = 0; k < curve->mCount; k++) {
                    fn_802371BC((unsigned char*)&curve->mKeys[k], sKeyDesc);
                }
            }
        }
    }
    res->mDefFlags = (unsigned int*)header->mDefFlags;
    for (unsigned int i = 0; i < res->mDefCount; i++) {
        fn_802371BC((unsigned char*)&res->mDefFlags[i], sWordDesc);
    }
    if (res->mVersion <= 0x23) {
        unsigned int* src = (unsigned int*)header->mDefCurves;
        res->mDefCurves = new (7, "", "", 0) EffectCurveData_unk[res->mDefCount][13];
        for (unsigned int i = 0; i < res->mDefCount; i++) {
            for (int c = 0; c < 13; c++) {
                unsigned int n = *src;
                EffectCurveData_unk* curve = &res->mDefCurves[i][c];
                curve->mCount = SWAP32_UNK(n);
                src++;
                curve->mKeys = new (7, "", "", 0) EffectKeyData_unk[SWAP32_UNK(n)];
                memcpy(curve->mKeys, src, curve->mCount * sizeof(EffectKeyData_unk));
                for (unsigned int k = 0; k < curve->mCount; k++) {
                    fn_802371BC((unsigned char*)&curve->mKeys[k], sKeyDesc);
                }
                src += curve->mCount * 2;
            }
        }
    } else {
        res->mDefCurves = (EffectCurveData_unk(*)[13])header->mDefCurves;
        for (unsigned int i = 0; i < res->mDefCount; i++) {
            for (int c = 0; c < 13; c++) {
                EffectCurveData_unk* curve = &res->mDefCurves[i][c];
                unsigned int n = curve->mCount;
                curve->mCount = SWAP32_UNK(n);
                unsigned int keys = (unsigned int)curve->mKeys;
                curve->mKeys = (EffectKeyData_unk*)(SWAP32_UNK(keys) + (unsigned int)header);
                for (unsigned int k = 0; k < curve->mCount; k++) {
                    fn_802371BC((unsigned char*)&curve->mKeys[k], sKeyDesc);
                }
            }
        }
    }
    res->mEmitterEnabled = (int*)header->mEmitterEnabled;
    res->mDefEnabled = (int*)header->mDefEnabled;
    for (unsigned int i = 0; i < res->mCount; i++) {
        res->mEmitterEnabled[i] = ((int*)header->mEmitterEnabled)[i] != 0;
    }
    for (unsigned int i = 0; i < res->mDefCount; i++) {
        res->mDefEnabled[i] = ((int*)header->mDefEnabled)[i] != 0;
    }
    if (res->mVersion <= 0x23) {
        res->m34 = new (7, "", "", 0) unsigned int[res->mTextureCount];
    } else {
        res->m34 = (unsigned int*)header->m44;
    }
    res->mLength = fn_802386FC(res, 0);
    fn_80238808(res);
    fn_80238CA0(header, res);
    return 1;
}

// inferred: releases what fn_80239030 allocated for old versions
extern "C" void fn_802396A0(EffectResData_unk* res) {
    if (res->mVersion <= 0x23) {
        for (unsigned int i = 0; i < res->mCount; i++) {
            for (unsigned int c = 0; c < 22; c++) {
                delete[] res->mEmitterCurves[i][c].mKeys;
            }
        }
        delete[] res->mEmitterCurves;
        for (unsigned int i = 0; i < res->mDefCount; i++) {
            for (unsigned int c = 0; c < 13; c++) {
                delete[] res->mDefCurves[i][c].mKeys;
            }
        }
        delete[] res->mDefCurves;
        if (res->m30 != 0) {
            delete[] res->m30;
        }
        delete[] res->m34;
    }
}

extern "C" void fn_80239794() {
    fn_80234A8C();
}
