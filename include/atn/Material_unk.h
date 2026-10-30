#ifndef ATN_MATERIAL_UNK_H
#define ATN_MATERIAL_UNK_H

// Materials of the models (code in src/atn/MaterialParser_unk.cpp). A material (0x54 bytes) is
// read from the material files by LoadMaterial (atn/LH_Material.cpp) or built from a shader
// (atn/Shader_unk.cpp), and is drawn by atn/ModelDraw_unk.cpp. All names are invented.
#include "atn/GXDecl.h"
#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"
#include "std/string.h"
#include <boost/function.hpp>

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

struct ShaderTex_unk;
struct Material_unk;
namespace atn {
struct ShaderDesc_unk;
}

// inferred: string owned with new[] (heap 1); a null pointer reads as "" (lbl_8087EF00, also
// used by the inlines of Event/DebugInfo_unk and game/GameTask)
struct String_unk {
    String_unk() : mPtr(0) {}
    ~String_unk() {
        if (mPtr != 0) {
            delete[] mPtr;
            mPtr = 0;
        }
    }
    const char* c_str() const { return mPtr != 0 ? mPtr : &sEmpty_unk; }
    void assign(const char* s, unsigned long len) {
        if (s != 0 && len != 0) {
            char* p = new ((MemoryTag)1, "", "", 0) char[len + 1];
            memcpy(p, s, len);
            p[len] = 0;
            if (mPtr != 0) {
                delete[] mPtr;
                mPtr = 0;
            }
            mPtr = p;
        } else if (mPtr != 0) {
            delete[] mPtr;
            mPtr = 0;
        }
    }
    String_unk& operator=(const char* s) {
        assign(s, s != 0 ? strlen(s) : 0);
        return *this;
    }
    String_unk& operator=(const String_unk& o) { return *this = o.c_str(); }

    char* mPtr;
    static char sEmpty_unk; // lbl_8087EF00
};

// inferred: UV transform of a texture stage (0x14 bytes); the defaults are const objects of
// .sdata2 (0x80880B88-0x80880B98) that atn/ModelDraw_unk.cpp also reads
struct TexUv_unk {
    TexUv_unk() : mScaleU(sScaleU), mScaleV(sScaleV), mRot(sRot), mTransU(sTransU), mTransV(sTransV) {}
    ~TexUv_unk() {} // inferred: `delete` keeps its own null test

    float mScaleU;
    float mScaleV;
    float mRot;
    float mTransU;
    float mTransV;

    static const float sScaleU;
    static const float sScaleV;
    static const float sRot;
    static const float sTransU;
    static const float sTransV;
};

// inferred: texture stage of a material (0x18 bytes); the stages of a material are sorted by id
struct TexStage_unk {
    TexStage_unk()
        : mTexture(0), mUv(0), mIndUv(0), mId(-1), mOwnsTexture(0), mWrapS(0), mWrapT(0), mMinFilter(1),
          mMagFilter(1) {}
    TexStage_unk(int id);
    TexStage_unk(const TexStage_unk& o) : mTexture(0), mUv(0), mIndUv(0), mOwnsTexture(0), mWrapS(0), mWrapT(0) {
        *this = o;
    }
    ~TexStage_unk();
    TexStage_unk& operator=(const TexStage_unk& o);
    float GetScaleU() const { return mUv != 0 ? mUv->mScaleU : TexUv_unk::sScaleU; }
    float GetScaleV() const { return mUv != 0 ? mUv->mScaleV : TexUv_unk::sScaleV; }
    float GetRot() const { return mUv != 0 ? mUv->mRot : TexUv_unk::sRot; }
    float GetTransU() const { return mUv != 0 ? mUv->mTransU : TexUv_unk::sTransU; }
    float GetTransV() const { return mUv != 0 ? mUv->mTransV : TexUv_unk::sTransV; }
    float GetIndScaleU() const { return mIndUv != 0 ? mIndUv->mScaleU : TexUv_unk::sScaleU; }
    float GetIndScaleV() const { return mIndUv != 0 ? mIndUv->mScaleV : TexUv_unk::sScaleV; }
    float GetIndRot() const { return mIndUv != 0 ? mIndUv->mRot : TexUv_unk::sRot; }
    float GetIndTransU() const { return mIndUv != 0 ? mIndUv->mTransU : TexUv_unk::sTransU; }
    float GetIndTransV() const { return mIndUv != 0 ? mIndUv->mTransV : TexUv_unk::sTransV; }
    TexUv_unk* Uv() {
        if (mUv == 0) {
            mUv = new ((MemoryTag)6, "", "", 0) TexUv_unk;
        }
        return mUv;
    }
    TexUv_unk* IndUv() {
        if (mIndUv == 0) {
            mIndUv = new ((MemoryTag)6, "", "", 0) TexUv_unk;
        }
        return mIndUv;
    }
    void SetScaleU(float v) { Uv()->mScaleU = v; }
    void SetScaleV(float v) { Uv()->mScaleV = v; }
    void SetRot(float v) { Uv()->mRot = v; }
    void SetTransU(float v) { Uv()->mTransU = v; }
    void SetTransV(float v) { Uv()->mTransV = v; }
    void SetIndScaleU(float v) { IndUv()->mScaleU = v; }
    void SetIndScaleV(float v) { IndUv()->mScaleV = v; }
    void SetIndRot(float v) { IndUv()->mRot = v; }
    void SetIndTransU(float v) { IndUv()->mTransU = v; }
    void SetIndTransV(float v) { IndUv()->mTransV = v; }
    void fn_8007AB38(atn::ShaderDesc_unk* desc, int index); // inferred: setup from a shader description
    void fn_8007B100();                                   // inferred: applies wrap and filter modes
    const char* fn_8007B260(const char* path, Material_unk* mat, const char* p); // inferred: reads a stage block
    void fn_8007C018(const char* path, int mode);           // inferred: loads the texture

    String_unk mName;            // 0x00
    atn::Texture_unk* mTexture;  // 0x04
    TexUv_unk* mUv;              // 0x08
    TexUv_unk* mIndUv;           // 0x0C: inferred: indirect UV transform
    signed char mId;             // 0x10: inferred: texture id (index in the table of texture names)
    unsigned char mOwnsTexture;  // 0x11
    signed char mWrapS;          // 0x12
    signed char mWrapT;          // 0x13
    signed char mMinFilter;      // 0x14
    signed char mMagFilter;      // 0x15
};

// inferred: texture animation of a material (0x54 bytes)
struct TexAnim_unk {
    TexAnim_unk();

    int mStageId;          // 0x00
    TexStage_unk* mStage;  // 0x04
    float m08;
    float m0C;
    float m10;
    float mScrollU;        // 0x14: inferred: translation speed of the UV transform
    float mScrollV;        // 0x18
    float m1C;
    float m20;
    float m24;
    float mIndScrollU;     // 0x28: inferred: same for the indirect UV transform
    float mIndScrollV;     // 0x2C
    atn::Vector3 mWaveU;   // 0x30: inferred: {amplitude, frequency, phase} of a sine added to the U offset
    atn::Vector3 mWaveV;   // 0x3C
    float mWaveTimeU;      // 0x48
    float mWaveTimeV;      // 0x4C
    unsigned char mInd : 1;  // 0x50 (0x80): inferred: also scrolls the indirect UV transform
    unsigned char mWave : 1; // 0x50 (0x40)
    unsigned char m50_2 : 6;
};

// inferred: gives the texture of a stage name (fn_80080C20), 0 to load it from the files
typedef boost::function<atn::Texture_unk*(const std::string&)> TexGetter_unk;

// inferred: material (0x54 bytes; constructor 0x8007C144, destructor 0x8007C3F8)
struct Material_unk {
    Material_unk(ShaderTex_unk* shader);
    ~Material_unk();
    // inferred: reads one material from `data` and returns the end of what it read
    const char* fn_8007C46C(const char* path, const char* data, int size);
    void fn_8007FAF0(int mode);       // inferred: sets the draw mode
    int fn_800804D0(int id);           // inferred: index of the stage of id `id`, -1 if none
    TexStage_unk* fn_80080528(int id); // inferred: stage of id `id`
    int fn_800816A8();
    void fn_80081BD0();
    void fn_8007FE5C(atn::ShaderDesc_unk* desc); // inferred: blend modes of a shader description
    void fn_80080048(float time);                // inferred: texture animations
    void fn_80080584(const Material_unk& o);     // inferred: copy
    void fn_80080B34(atn::ShaderDesc_unk* desc); // inferred: stage names
    void fn_80080C20(const std::string& dir, TexGetter_unk& getter); // inferred: stage textures
    void fn_80081080(char* shader);              // inferred: shader name
    void fn_80081514(float u, float v);          // inferred: UV translation of all the stages
    void fn_800816B4(void* parent, const char* name); // inferred: debug menu
    void fn_80081D00(); // inferred: computes the pass mask m49
    TexStage_unk* FindStage_unk(int id);

    unsigned int m00; // inferred: name hash
    unsigned int m04;
    ShaderTex_unk* m08;
    TexStage_unk* m0C; // inferred: texture stages (m4A)
    TexAnim_unk* m10;  // inferred: texture animations (m4B)
    unsigned int m14_0 : 1;
    unsigned int m14_1 : 3;
    unsigned int m14_4 : 8;
    unsigned int m14_12 : 1;
    unsigned int m14_13 : 3;
    unsigned int m14_16 : 3;
    unsigned int m14_19 : 1;
    unsigned int m14_20 : 1;
    unsigned int m14_21 : 1;
    unsigned int m14_22 : 3;
    unsigned int m14_25 : 1;
    unsigned int m14_26 : 1;
    unsigned int m14_27 : 1;
    unsigned int m14_28 : 4;
    GXColor mColor; // 0x18: inferred: material colour
    GXColor m1C;    // inferred: ambient colour
    GXColor m20;
    GXColor mTevColor[3]; // 0x24: inferred: TEV colours 1-3 (RegColor1-3)
    float m30;
    int m34;     // inferred: TEV scale of the last stage
    GXColor m38; // inferred: swap table
    int m3C;
    int m40;
    int m44;           // inferred: sort mode (> 0: drawn later)
    unsigned char m48; // inferred: flags
    unsigned char m49; // inferred: pass mask
    unsigned char m4A; // inferred: number of texture stages
    unsigned char m4B; // inferred: number of texture animations
    signed char m4C;
    signed char m4D;
    signed char m4E;
    signed char m4F;
    signed char m50;
    signed char m51;
    unsigned char m52[2];
};

#endif
