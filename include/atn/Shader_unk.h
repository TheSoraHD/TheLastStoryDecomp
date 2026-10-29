#ifndef ATN_SHADER_UNK_H
#define ATN_SHADER_UNK_H

// Shaders of the models (src/atn/Shader_unk.cpp) and their manager (src/atn/ShaderManager_unk.cpp).
// All names are invented.
#include "atn/Array_unk.h"
#include "atn/GXDecl.h"
#include "std/vector.h"

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

namespace atn {
class LH_Model;
}

namespace atn {

// inferred: texture coordinate generation of a material (0x1C bytes; GXSetTexCoordGen2 arguments)
struct ShaderTexGen_unk {
    int m00[4];
    int mFunc; // 0x10
    int mSrc;  // 0x14
    int mMtx;  // 0x18
};

// inferred: texture matrix of a texture coordinate (0x30 bytes); mType: 0 plain, 1-4 matrix types
struct ShaderTexMtx_unk {
    int m00[4];
    int mType; // 0x10
    int m14[7];
};

// inferred: indirect stage (GXSetIndTexOrder / GXSetIndTexCoordScale)
struct ShaderIndStage_unk {
    int mTexMap;   // 0x00
    int mTexCoord; // 0x04: index of the material texture coordinate
    int mScaleS;   // 0x08
    int mScaleT;   // 0x0C
};

// inferred: TEV stage of a material (0x88 bytes; arguments of the GXSetTev* calls)
struct ShaderTevStage_unk {
    int mChannel;      // 0x00: 0 none, 1-3 color channel
    int mTexMap;       // 0x04
    int mTexCoord;     // 0x08
    int mRasSwap;      // 0x0C
    int mTexSwap;      // 0x10
    int mColorIn[4];   // 0x14
    int mKColorSel;    // 0x24
    int mColorOp;      // 0x28
    int mColorBias;    // 0x2C
    int mColorScale;   // 0x30
    int mColorClamp;   // 0x34
    int mColorReg;     // 0x38
    int mAlphaIn[4];   // 0x3C
    int mKAlphaSel;    // 0x4C
    int mAlphaOp;      // 0x50
    int mAlphaBias;    // 0x54
    int mAlphaScale;   // 0x58
    int mAlphaClamp;   // 0x5C
    int mAlphaReg;     // 0x60
    int mIndStage;     // 0x64
    int mIndFormat;    // 0x68
    int mIndBias;      // 0x6C
    int mIndMtx;       // 0x70
    int mIndWrapS;     // 0x74
    int mIndWrapT;     // 0x78
    int mIndAddPrev;   // 0x7C
    int mIndUtcLod;    // 0x80
    int mIndAlphaSel;  // 0x84
};

// inferred: material description of a model file (layout from the readers below, names invented)
struct ShaderDesc_unk {
    unsigned char m000[0x10];
    const char* m010; // inferred: shader name
    GXColor m014;
    GXColor m018;
    GXColor m01C;
    unsigned char m020[4];
    int m024[12];
    unsigned char m054;
    unsigned char m055[0x60 - 0x55];
    GXColor mKColor3; // 0x060: GXSetTevKColor(GX_KCOLOR3)
    int m064;
    unsigned char m068[0x7C - 0x68];
    int m07C;
    int mNumChans; // 0x080: GXSetNumChans(mNumChans / 2)
    unsigned char m084[0x8C - 0x84];
    int m08C;
    int m090;
    unsigned char m094[0xA4 - 0x94];
    int m0A4;
    int m0A8;
    unsigned char m0AC[0xE4 - 0xAC];
    int m0E4; // inferred: number of texture stages
    unsigned char m0E8[0xEC - 0xE8];
    int mTexGenCount;                // 0x0EC
    ShaderTexGen_unk** mTexGens;     // 0x0F0
    unsigned char m0F4[0xF8 - 0xF4];
    ShaderTexMtx_unk** mTexMtxs;     // 0x0F8
    unsigned char m0FC[0x13C - 0xFC];
    int mIndStageCount;              // 0x13C
    ShaderIndStage_unk mIndStages[4]; // 0x140
    unsigned char mSwapTables[3][4]; // 0x180: GXSetTevSwapModeTable 0-2
    unsigned char m18C[4];
    int mTevStageCount;              // 0x190
    ShaderTevStage_unk mTevStages[16]; // 0x194
    int mA14;
    int mA18;
    unsigned char mA1C[0xA28 - 0xA1C];
    int mA28;
    int mA2C;
    int mA30;
    unsigned char mA34[4];
    int mA38;
    unsigned char mA3C[0xA48 - 0xA3C];
    unsigned int mA48;    // inferred: number of textures
    const char** mA4C;    // inferred: texture names
};

} // namespace atn

// inferred: one texture of a shader (12 bytes); the shader sorts them by id. The fields are in a
// POD base: copies of the entries go through the stack, as in the original
struct ShaderTexEntryData_unk {
    int mId;
    int mIndex;
    struct TexStage_unk* mBind;
};

struct ShaderTexEntry_unk : ShaderTexEntryData_unk {
    ShaderTexEntry_unk() {
        mId = -1;
        mIndex = -1;
        mBind = 0;
    }
    ShaderTexEntry_unk(int id, int index) {
        mId = id;
        mIndex = index;
        mBind = 0;
    }
    bool operator<(const ShaderTexEntry_unk& o) const { return mId < o.mId; }
};

// inferred: a shader (0x14 bytes)
struct ShaderTex_unk {
    ShaderTex_unk(void* model, atn::ShaderDesc_unk* desc);
    ~ShaderTex_unk();

    atn::SimpleArray<ShaderTexEntry_unk> mTextures; // inferred: sorted by id
    atn::SimpleArray<int> mOrder;                   // inferred: index of each texture in mTextures
    atn::ShaderDesc_unk* mDesc;
};

// inferred: one level of detail of a material: a shader and the distance where it starts
struct ShaderLODEntry_unk {
    ShaderTex_unk* mShader;
    float mDistance;
};

// inferred: shaders of a material sorted by distance (0x10 bytes)
struct ShaderLOD_unk {
    ShaderLOD_unk();
    ~ShaderLOD_unk();
    void fn_800C4ECC(ShaderTex_unk* shader, float distance);
    void fn_800C4F38(struct ShaderMaterial_unk** out) const;
    void fn_800C4F70(void* parent);

    int m00; // inferred: reference count (debug menu "RefCount")
    std::vector<ShaderLODEntry_unk> mLODs;
};

// inferred: owner of all the shaders (0x24 bytes, lbl_8087EFE0)
struct ShaderManager_unk {
    ShaderManager_unk();
    void LoadShaders_unk();
    void CreateLODs_unk();
    ShaderLOD_unk* fn_800C5F5C(const char* name); // inferred: LOD list of a shader by name
    ShaderLOD_unk* fn_800C5FF0(unsigned int hash) const;
    struct ShaderMaterial_unk* fn_800C607C(int code);
    // inferred: shader by name (name invented)
    ShaderTex_unk* FindShader_unk(const char* name) {
        unsigned int hash = fn_800DC6B4(name);
        for (ShaderTex_unk** it = mShaders.begin(); it != mShaders.end(); it++) {
            if (hash == fn_800DC6B4((*it)->mDesc->m010)) {
                return *it;
            }
        }
        return 0;
    }

    std::vector<ShaderTex_unk*> mShaders;
    std::vector<atn::LH_Model*> mModels; // inferred: the shader files
    std::vector<ShaderLOD_unk*> mLODs;
};

#endif
