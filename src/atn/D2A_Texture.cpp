#include "atn/D2A_Texture.h"
#include "atn/D2A_Data.h"
#include "atn/D2A_Message.h"
#include "math/Vec.h"
#include "std/string.h"
#include "atn/DrawHelper.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" std::string fn_8006B174(const std::string& path);
extern "C" std::string fn_8006B2D8(const std::string& path);
extern "C" unsigned int fn_800DC6B4(const char* s);
extern "C" float fn_801F0544(atn::D2A_ParamTable* table, int key);
extern "C" const char lbl_8073E57C[];
extern "C" void fn_800D58A4(atn::Texture_unk* texture);
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path);
extern "C" void fn_801FE99C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* value);
extern "C" float fn_800A6BD4(const unsigned char* code, float value);
extern "C" void fn_801EDCB8(atn::D2A_Common* element);
extern "C" unsigned int fn_800DC6B4(const char* text);
extern "C" int fn_801F03AC(void* table, int key);
extern "C" std::basic_string<wchar_t> fn_801F0758(void* table, int key);
extern "C" void fn_801F0364(void* table, int key, int value, int arg);
extern "C" const float lbl_80882CB4;

struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

inline float D2A_DegToRad_unk(float deg) {
    return lbl_80882CB4 * deg;
}

inline unsigned int fn_ToU8(const float& f) {
    return (f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f);
}

#define D2A_OUTLINE_SINGLE(FN)                                             \
    if (m0E == 1) {                                                                                \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,                    \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
    } else if (m0E == 2) {                                                                         \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
    } else if (m0E == 3) {                                                                         \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h,                              \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,                    \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x + outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h,                    \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x - outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h,                    \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
        lbl_8087EEB0->FN(outlineColor, whiteTexture,                                 \
           rect.x - outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,                    \
           uv[0], uv[1], uv[2], uv[3]);                                                  \
    }

#define D2A_DRAW_DUAL(FN, COLOR, TEX0, TEX1, BLEND) \
    if (m0E == 1) { \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
    } else if (m0E == 2) { \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
    } else if (m0E == 3) { \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x + outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x - outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
        lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, BLEND, rect.x - outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]); \
    }
#define D2A_XFORM_SINGLE_AT(FN, X, Y) \
    lbl_8087EEB0->FN(outlineColor, whiteTexture, extra.m04, 1, (X), (Y), 0.001f + rect.z, rect.w, rect.h, mB0, mB4, D2A_DegToRad_unk(extra.m00), uv[0], uv[1], uv[2], uv[3]);
#define D2A_XFORM_SINGLE_OUTLINES(FN) \
    if (m0E == 1) { D2A_XFORM_SINGLE_AT(FN, rect.x + outline.x, rect.y + outline.y); } \
    else if (m0E == 2) { \
        D2A_XFORM_SINGLE_AT(FN, rect.x + outline.x, rect.y); D2A_XFORM_SINGLE_AT(FN, rect.x, rect.y + outline.y); \
        D2A_XFORM_SINGLE_AT(FN, rect.x - outline.x, rect.y); D2A_XFORM_SINGLE_AT(FN, rect.x, rect.y - outline.y); \
    } else if (m0E == 3) { \
        D2A_XFORM_SINGLE_AT(FN, rect.x + outline.x, rect.y); D2A_XFORM_SINGLE_AT(FN, rect.x, rect.y + outline.y); \
        D2A_XFORM_SINGLE_AT(FN, rect.x - outline.x, rect.y); D2A_XFORM_SINGLE_AT(FN, rect.x, rect.y - outline.y); \
        D2A_XFORM_SINGLE_AT(FN, rect.x + outline.x, rect.y + outline.y); D2A_XFORM_SINGLE_AT(FN, rect.x + outline.x, rect.y - outline.y); \
        D2A_XFORM_SINGLE_AT(FN, rect.x - outline.x, rect.y - outline.y); D2A_XFORM_SINGLE_AT(FN, rect.x - outline.x, rect.y + outline.y); \
    }
#define D2A_XFORM_DUAL_AT(FN, X, Y) \
    lbl_8087EEB0->FN(outlineColor, whiteTexture, &mTexture1, extra.m04, (X), (Y), 0.001f + rect.z, rect.w, rect.h, mB0, mB4, D2A_DegToRad_unk(extra.m00), uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]);
#define D2A_XFORM_DUAL_OUTLINES(FN) \
    if (m0E == 1) { D2A_XFORM_DUAL_AT(FN, rect.x + outline.x, rect.y + outline.y); } \
    else if (m0E == 2) { \
        D2A_XFORM_DUAL_AT(FN, rect.x + outline.x, rect.y); D2A_XFORM_DUAL_AT(FN, rect.x, rect.y + outline.y); \
        D2A_XFORM_DUAL_AT(FN, rect.x - outline.x, rect.y); D2A_XFORM_DUAL_AT(FN, rect.x, rect.y - outline.y); \
    } else if (m0E == 3) { \
        D2A_XFORM_DUAL_AT(FN, rect.x + outline.x, rect.y); D2A_XFORM_DUAL_AT(FN, rect.x, rect.y + outline.y); \
        D2A_XFORM_DUAL_AT(FN, rect.x - outline.x, rect.y); D2A_XFORM_DUAL_AT(FN, rect.x, rect.y - outline.y); \
        D2A_XFORM_DUAL_AT(FN, rect.x + outline.x, rect.y + outline.y); D2A_XFORM_DUAL_AT(FN, rect.x + outline.x, rect.y - outline.y); \
        D2A_XFORM_DUAL_AT(FN, rect.x - outline.x, rect.y - outline.y); D2A_XFORM_DUAL_AT(FN, rect.x - outline.x, rect.y + outline.y); \
    }

namespace atn {

// NONMATCHING: 99.83% - two saved-register moves are emitted in the opposite order.
D2A_Texture::D2A_Texture(D2A_Data* owner, unsigned short id) : D2A_Common(owner, id) {
    mType = 0;

    char name[0x20];
    sprintf(name, lbl_8073E57C, id);
    mTexRect.mName = fn_800DC6B4(name);
    mTexRect.mTables[0].fn_801F04FC(0, 1, 0.0f);
    mTexRect.mTables[1].fn_801F04FC(0, 1, 0.0f);
    mTexRect.mTables[2].fn_801F04FC(0, 1, 1.0f);
    mTexRect.mTables[3].fn_801F04FC(0, 1, 1.0f);
    int tableOffset = 0;
    int i = 0;
    int valueOffset = 0;
    while (i < 4) {
        *(float*)((char*)this + 0x150 + valueOffset) =
            fn_801F0544((D2A_ParamTable*)((char*)this + 0x130 + tableOffset), 0);
        i++;
        tableOffset += 8;
        valueOffset += 4;
    }

    sprintf(name, lbl_8073E57C + 8, id);
    mTexScale.mName = fn_800DC6B4(name);
    mTexScale.mTables[0].fn_801F04FC(0, 1, 0.0f);
    mTexScale.mTables[1].fn_801F04FC(0, 1, 0.0f);
    mTexScale.mTables[2].fn_801F04FC(0, 1, 1.0f);
    mTexScale.mTables[3].fn_801F04FC(0, 1, 1.0f);
    i = 0;
    tableOffset = 0;
    valueOffset = 0;
    while (i < 4) {
        *(float*)((char*)this + 0x184 + valueOffset) =
            fn_801F0544((D2A_ParamTable*)((char*)this + 0x164 + tableOffset), 0);
        i++;
        tableOffset += 8;
        valueOffset += 4;
    }
}

D2A_Texture::~D2A_Texture() {
    fn_800D58A4(&mTexture0);
    fn_800D58A4(&mTexture1);
    if (mFC != 0) {
        fn_801FE99C(&((D2A_Data*)mFC)->m10, &mTexRect);
        fn_801FE99C(&((D2A_Data*)mFC)->m10, &mTexScale);
    }
}

// NONMATCHING: 97.96% - pointer-array growth uses a different register schedule.
void D2A_Texture::vf0C() {
    fn_801EC784();
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner != 0) {
        owner->m10.mTextureAnims.PushBackRef_unk(mTexRect);
        owner->m10.mTextureAnims.PushBackRef_unk(mTexScale);
    }
}

void D2A_Texture::vf14() {}

struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(const Color4f_unk& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    unsigned int ToU32() const {
        unsigned int ur = fn_ToU8(r);
        unsigned int ug = fn_ToU8(g);
        unsigned int ub = fn_ToU8(b);
        unsigned int ua = fn_ToU8(a);
        return (ua << 24) | (ur << 16) | (ug << 8) | ub;
    }
    float r, g, b, a;
};
struct D2A_Outline_unk {
    float x;
    float y;
    Color4f_unk mColor;
};

// inferred: copies the animated UV rectangle and scale, applying the element expressions.
extern "C" void fn_801F9E70(atn::D2A_Texture* texture, float* rect, float* scale) {
    rect[0] = texture->mTexRect.mValues[0];
    rect[1] = texture->mTexRect.mValues[1];
    rect[2] = texture->mTexRect.mValues[2];
    rect[3] = texture->mTexRect.mValues[3];
    scale[0] = texture->mTexScale.mValues[0];
    scale[1] = texture->mTexScale.mValues[1];
    scale[2] = texture->mTexScale.mValues[2];
    scale[3] = texture->mTexScale.mValues[3];
    if (texture->mRectCode[0].mCode != 0)
        rect[0] = fn_800A6BD4(texture->mRectCode[0].mCode, rect[0]);
    if (texture->mRectCode[1].mCode != 0)
        rect[1] = fn_800A6BD4(texture->mRectCode[1].mCode, rect[1]);
    if (texture->mRectCode[2].mCode != 0)
        rect[2] = fn_800A6BD4(texture->mRectCode[2].mCode, rect[2]);
    if (texture->mRectCode[3].mCode != 0)
        rect[3] = fn_800A6BD4(texture->mRectCode[3].mCode, rect[3]);
    if (texture->mScaleCode[0].mCode != 0)
        scale[0] = fn_800A6BD4(texture->mScaleCode[0].mCode, scale[0]);
    if (texture->mScaleCode[1].mCode != 0)
        scale[1] = fn_800A6BD4(texture->mScaleCode[1].mCode, scale[1]);
    if (texture->mScaleCode[2].mCode != 0)
        scale[2] = fn_800A6BD4(texture->mScaleCode[2].mCode, scale[2]);
    if (texture->mScaleCode[3].mCode != 0)
        scale[3] = fn_800A6BD4(texture->mScaleCode[3].mCode, scale[3]);
}

// NONMATCHING: 95.72% - register allocation and call scheduling remain.
void D2A_Texture::vf18() {
    if (!fn_801EDC78()) {
        return;
    }
    if (((GimmickUvOwner_unk*)&mTexture0)->fn_800D59B8() != 0) {
        return;
    }
    if ((m10 & 8) == 8) {
        if (((GimmickUvOwner_unk*)&mTexture1)->fn_800D59B8() != 0) {
            return;
        }
    }
    D2A_DrawRect rect;
    D2A_DrawColor color;
    D2A_DrawExtra extra;
    D2A_Outline_unk outline;
    fn_801ED928(&rect, &color, &extra, &outline);
    float uv[4];
    float scale[4];
    fn_801F9E70(this, uv, scale);
    Color4f_unk callbackColor(outline.mColor);
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner->mA4 != 0) {
        typedef void (*DrawCallback_unk)(D2A_Texture*, D2A_DrawColor*, Color4f_unk*);
        if (m0E == 0) {
            ((DrawCallback_unk)owner->mA4)(this, &color, 0);
        } else {
            ((DrawCallback_unk)owner->mA4)(this, &color, &callbackColor);
        }
    }

    float alpha = color.a;
    if (alpha <= 0.0f)
        return;

    // The engine packs color in A-R-G-B byte order and rounds intermediate values.
    callbackColor.a *= alpha;
    unsigned int r = fn_ToU8(color.r);
    unsigned int g = fn_ToU8(color.g);
    unsigned int b = fn_ToU8(color.b);
    unsigned int a = fn_ToU8(color.a);
    unsigned int packedColor = (a << 24) | (r << 16) | (g << 8) | b;
    unsigned int outlineColor = callbackColor.ToU32();
    void* whiteTexture = &lbl_8087EEB0->m10;
    // Dispatch between the eight combinations of UV mapping, texture layers,
    // and the renderer's standard or custom blend modes.
    if ((m10 & 1) != 1) {
        if ((m10 & 8) != 8) {
            if (extra.m04 >= 0) {
                lbl_8087EEB0->fn_8005E3E4(packedColor, &mTexture0, rect.x, rect.y, rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                if (m0E == 1) {
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                } else if (m0E == 2) {
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                } else if (m0E == 3) {
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x + outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x - outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                    lbl_8087EEB0->fn_8005E3E4(outlineColor, whiteTexture, rect.x - outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], extra.m04, 1);
                }
            } else {
                lbl_8087EEB0->fn_8005F90C(packedColor, &mTexture0, rect.x, rect.y, rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3]);
                D2A_OUTLINE_SINGLE(fn_8005F90C);
            }
        } else if (extra.m04 >= 0) {
            lbl_8087EEB0->fn_8005E838(packedColor, &mTexture0, &mTexture1, extra.m04, rect.x, rect.y, rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]);
            D2A_DRAW_DUAL(fn_8005E838, packedColor, &mTexture0, &mTexture1, extra.m04);
        } else {
            lbl_8087EEB0->fn_8005FD80(packedColor, &mTexture0, &mTexture1, extra.m04, rect.x, rect.y, rect.z, rect.w, rect.h, uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]);
            D2A_DRAW_DUAL(fn_8005FD80, packedColor, &mTexture0, &mTexture1, extra.m04);
        }
    } else if ((m10 & 8) != 8) {
        if (extra.m04 >= 0) {
            lbl_8087EEB0->fn_8005EEA0(packedColor, &mTexture0, extra.m04, 1, rect.x, rect.y, rect.z, rect.w, rect.h, mB0, mB4, D2A_DegToRad_unk(extra.m00), uv[0], uv[1], uv[2], uv[3]);
            D2A_XFORM_SINGLE_OUTLINES(fn_8005EEA0);
        } else {
            lbl_8087EEB0->fn_800602E8(packedColor, &mTexture0, extra.m04, 1, rect.x, rect.y, rect.z, rect.w, rect.h, mB0, mB4, D2A_DegToRad_unk(extra.m00), uv[0], uv[1], uv[2], uv[3]);
            D2A_XFORM_SINGLE_OUTLINES(fn_800602E8);
        }
    } else if (extra.m04 >= 0) {
        lbl_8087EEB0->fn_8005F36C(packedColor, &mTexture0, &mTexture1, extra.m04, rect.x, rect.y, rect.z, rect.w, rect.h, mB0, mB4, D2A_DegToRad_unk(extra.m00), uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]);
            D2A_XFORM_DUAL_OUTLINES(fn_8005F36C);
    } else {
        lbl_8087EEB0->fn_800607C0(packedColor, &mTexture0, &mTexture1, extra.m04, rect.x, rect.y, rect.z, rect.w, rect.h, mB0, mB4, D2A_DegToRad_unk(extra.m00), uv[0], uv[1], uv[2], uv[3], scale[0], scale[1], scale[2], scale[3]);
            D2A_XFORM_DUAL_OUTLINES(fn_800607C0);
    }
}

void D2A_Texture::vf10() {}

int D2A_Texture::vf1C() {
    if (((GimmickUvOwner_unk*)&mTexture0)->fn_800D59B8() != 0) {
        return 1;
    }
    if ((m10 & 8) == 8) {
        if (((GimmickUvOwner_unk*)&mTexture1)->fn_800D59B8() != 0) {
            return 1;
        }
    }
    return 0;
}

// inferred: packed pointer-array view used by the D2A_Layout_unk helpers.
struct D2A_PointerArray_unk {
    unsigned int mSize;
    unsigned int mCapacity;
    void** mData;
};
extern "C" void fn_801FE89C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<1>* value);
extern "C" void fn_801FE91C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<5>* value);
extern "C" void fn_801FEA1C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* value);
extern "C" void fn_801FEA9C(atn::D2A_Layout_unk* layout, atn::D2A_TextParam_unk* value);
extern "C" void fn_801FEB1C(atn::D2A_Layout_unk* layout, atn::D2A_IntAnim_unk* value);
extern "C" void* fn_801FEB9C(atn::D2A_Layout_unk* layout, const char* name);
extern "C" void* fn_801FEC08(atn::D2A_Layout_unk* layout, const char* name);
extern "C" void* fn_801FEC74(atn::D2A_Layout_unk* layout, const char* name);
static void SetLayoutFloat_unk(D2A_PointerArray_unk* array, unsigned int hash, int index, float value);
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FED70(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEDBC(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEEFC(atn::D2A_Layout_unk* layout, unsigned int hash, int value);
extern "C" void fn_801FEE08(atn::D2A_Layout_unk* layout, unsigned int hash, const wchar_t* value);
extern "C" void fn_801FEF40(atn::D2A_Layout_unk* layout, int frame);
extern "C" void fn_801FE090(D2A_Layout_unk* layout, const unsigned int* counts);

// NONMATCHING: the path normalization and string temporaries still differ from MWCC.
extern "C" int fn_801FD5A0(atn::D2A_Texture* texture, const char* path, int arg) {
    char fullPath[0x100];
    strcpy(fullPath, path);
    std::string name = fn_8006B174(std::string(fullPath));
    std::string base = fn_8006B2D8(name);
    if (base.size() >= 3 && base.substr(base.size() - 3) == "_jp") {
        base.replace(base.size() - 3, 3, "_xx");
    }
    base += ".texture";
    strcpy(fullPath, base.c_str());
    if (arg == 0) {
        return 1;
    }
    fn_800D58A4(&texture->mTexture0);
    if (fullPath[0] == 0) {
        fn_800D5908(&texture->mTexture0, lbl_8073E57C + 0x26);
    } else {
        fn_800D5908(&texture->mTexture0, fullPath);
    }
    texture->mTexture0.m2D = 1;
    texture->mTexture0.m2E = 1;
    return 1;
}

// NONMATCHING: mirrors fn_801FD5A0; this overload rejects an empty source path.
extern "C" int fn_801FDA20(atn::D2A_Texture* texture, const char* path, int arg) {
    if (*path == 0) {
        return 0;
    }
    char fullPath[0x100];
    strcpy(fullPath, path);
    std::string name = fn_8006B174(std::string(fullPath));
    std::string base = fn_8006B2D8(name);
    if (base.size() >= 3 && base.substr(base.size() - 3) == "_jp") {
        base.replace(base.size() - 3, 3, "_xx");
    }
    base += ".texture";
    strcpy(fullPath, base.c_str());
    if (arg == 0) {
        return 1;
    }
    fn_800D58A4(&texture->mTexture1);
    fn_800D5908(&texture->mTexture1, fullPath);
    texture->mTexture1.m2D = 1;
    texture->mTexture1.m2E = 1;
    return 1;
}

void D2A_Texture::vf20() { fn_801EDCB8(this); }

D2A_Layout_unk::D2A_Layout_unk() {}

D2A_Layout_unk::~D2A_Layout_unk() {
        D2A_Anim_unk<1>** data0 = mAnims1.mData;
        mAnims1.mSize = 0;
        mAnims1.mCapacity = 0;
        if (data0 != 0) {
            delete[] data0;
            mAnims1.mData = 0;
        }
        D2A_Anim_unk<5>** data1 = mAnims5.mData;
        mAnims5.mSize = 0;
        mAnims5.mCapacity = 0;
        if (data1 != 0) {
            delete[] data1;
            mAnims5.mData = 0;
        }
        D2A_Anim_unk<4>** data2 = mTextureAnims.mData;
        mTextureAnims.mSize = 0;
        mTextureAnims.mCapacity = 0;
        if (data2 != 0) {
            delete[] data2;
            mTextureAnims.mData = 0;
        }
        D2A_Anim_unk<4>** data3 = mAnims4.mData;
        mAnims4.mSize = 0;
        mAnims4.mCapacity = 0;
        if (data3 != 0) {
            delete[] data3;
            mAnims4.mData = 0;
        }
        D2A_TextParam_unk** data4 = mTexts.mData;
        mTexts.mSize = 0;
        mTexts.mCapacity = 0;
        if (data4 != 0) {
            delete[] data4;
            mTexts.mData = 0;
        }
        D2A_IntAnim_unk** data5 = mIntAnims.mData;
        mIntAnims.mSize = 0;
        mIntAnims.mCapacity = 0;
        if (data5 != 0) {
            delete[] data5;
            mIntAnims.mData = 0;
        }
}

// NONMATCHING: 99.59% - one inlined reserve/copy sequence differs from MWCC's loop schedule.
extern "C" void fn_801FE090(D2A_Layout_unk* layout, const unsigned int* counts) {
    layout->mAnims1.Reserve(counts[0], 8);
    layout->mAnims5.Reserve(counts[1], 8);
    layout->mTextureAnims.Reserve(counts[2], 8);
    layout->mAnims4.Reserve(counts[3], 8);
    layout->mTexts.Reserve(counts[4], 8);
    layout->mIntAnims.Reserve(counts[5], 8);
}

// inferred: unregisters an element value from the layout
extern "C" void fn_801FE89C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<1>* value) { layout->mAnims1.Remove(value); }

// inferred: unregisters an element value from the layout
extern "C" void fn_801FE91C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<5>* value) { layout->mAnims5.Remove(value); }

// inferred: unregisters an element value from the layout
extern "C" void fn_801FE99C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* value) { layout->mTextureAnims.Remove(value); }

// inferred: unregisters an element value from the layout
extern "C" void fn_801FEA1C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* value) { layout->mAnims4.Remove(value); }

// inferred: unregisters an element value from the layout
extern "C" void fn_801FEA9C(atn::D2A_Layout_unk* layout, atn::D2A_TextParam_unk* value) { layout->mTexts.Remove(value); }

// inferred: unregisters an element value from the layout
extern "C" void fn_801FEB1C(atn::D2A_Layout_unk* layout, atn::D2A_IntAnim_unk* value) { layout->mIntAnims.Remove(value); }

// NONMATCHING: 97.96% - hash lookup differs only in pointer-register assignment.
extern "C" void* fn_801FEB9C(atn::D2A_Layout_unk* layout, const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x00);
    void** end = array->mData + array->mSize;
    void** it = array->mData;
    while (it != end) {
        void* value = *it;
        if (*(unsigned int*)value == hash) {
            return value;
        }
        it++;
    }
    return 0;
}

// NONMATCHING: 97.96% - hash lookup differs only in pointer-register assignment.
extern "C" void* fn_801FEC08(atn::D2A_Layout_unk* layout, const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x0C);
    void** end = array->mData + array->mSize;
    void** it = array->mData;
    while (it != end) {
        void* value = *it;
        if (*(unsigned int*)value == hash) {
            return value;
        }
        it++;
    }
    return 0;
}

// NONMATCHING: 97.96% - hash lookup differs only in pointer-register assignment.
extern "C" void* fn_801FEC74(atn::D2A_Layout_unk* layout, const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x30);
    void** end = array->mData + array->mSize;
    void** it = array->mData;
    while (it != end) {
        void* value = *it;
        if (*(unsigned int*)value == hash) {
            return value;
        }
        it++;
    }
    return 0;
}

static void SetLayoutFloat_unk(D2A_PointerArray_unk* array, unsigned int hash, int index, float value) {
    void** it = array->mData;
    void** end = it + array->mSize;
    while (it != end) {
        atn::D2A_Anim_unk<1>* anim = (atn::D2A_Anim_unk<1>*)*it;
        if (anim->mName == hash) {
            anim->mTables[index].fn_801F04FC(0, 1, value);
            return;
        }
        it++;
    }
}

// NONMATCHING: 97.94% - table lookup uses a different pointer-register assignment.
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x00), hash, 0, value);
}

extern "C" void fn_801FED24(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x0C), hash, index, value);
}

// NONMATCHING: 98.16% - table lookup uses a different pointer-register assignment.
extern "C" void fn_801FED70(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x18), hash, index, value);
}

// NONMATCHING: 98.16% - table lookup uses a different pointer-register assignment.
extern "C" void fn_801FEDBC(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x24), hash, index, value);
}

// NONMATCHING: 95.57% - wide-string update loop has different pointer-register assignments.
extern "C" void fn_801FEE08(atn::D2A_Layout_unk* layout, unsigned int hash, const wchar_t* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x30);
    atn::D2A_TextParam_unk** end = (atn::D2A_TextParam_unk**)array->mData + array->mSize;
    atn::D2A_TextParam_unk** it = (atn::D2A_TextParam_unk**)array->mData;
    while (it != end) {
        atn::D2A_TextParam_unk* text = *it;
        if (text->mId == hash) {
            std::basic_string<wchar_t> copy(value);
            text->fn_801F25E0(0, copy, 0, 1);
            return;
        }
        it++;
    }
}

// NONMATCHING: 96.76% - integer table lookup has different pointer-register assignments.
extern "C" void fn_801FEEFC(atn::D2A_Layout_unk* layout, unsigned int hash, int value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x3C);
    atn::D2A_IntAnim_unk** end = (atn::D2A_IntAnim_unk**)array->mData + array->mSize;
    atn::D2A_IntAnim_unk** it = (atn::D2A_IntAnim_unk**)array->mData;
    while (it != end) {
        atn::D2A_IntAnim_unk* anim = *it;
        if (anim->mName == hash) {
            fn_801F0364(&anim->mTables[0], 0, value, 1);
            return;
        }
        it++;
    }
}

// NONMATCHING: 95.37% - animated-value loops have different register assignments.
extern "C" void fn_801FEF40(atn::D2A_Layout_unk* layout, int frame) {
    for (unsigned int i = 0; i < layout->mAnims1.mSize; i++) {
        atn::D2A_Anim_unk<1>* anim = layout->mAnims1.mData[i];
        anim->mValues[0] = fn_801F0544(&anim->mTables[0], frame);
    }
    for (unsigned int i = 0; i < layout->mAnims5.mSize; i++) {
        atn::D2A_Anim_unk<5>* anim = layout->mAnims5.mData[i];
        for (int j = 0; j < 5; j++) anim->mValues[j] = fn_801F0544(&anim->mTables[j], frame);
    }
    for (unsigned int i = 0; i < layout->mTextureAnims.mSize; i++) {
        atn::D2A_Anim_unk<4>* anim = layout->mTextureAnims.mData[i];
        for (int j = 0; j < 4; j++) anim->mValues[j] = fn_801F0544(&anim->mTables[j], frame);
    }
    for (unsigned int i = 0; i < layout->mAnims4.mSize; i++) {
        atn::D2A_Anim_unk<4>* anim = layout->mAnims4.mData[i];
        for (int j = 0; j < 4; j++) anim->mValues[j] = fn_801F0544(&anim->mTables[j], frame);
    }
    for (unsigned int i = 0; i < layout->mTexts.mSize; i++) {
        atn::D2A_TextParam_unk* text = layout->mTexts.mData[i];
        text->mValue[0] = fn_801F0758(&text->mTables[0], frame);
    }
    for (unsigned int i = 0; i < layout->mIntAnims.mSize; i++) {
        atn::D2A_IntAnim_unk* anim = layout->mIntAnims.mData[i];
        anim->mValues[0] = fn_801F03AC(&anim->mTables[0], frame);
    }
}
} // namespace atn
