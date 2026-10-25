#include "atn/D2A_Texture.h"
#include "atn/D2A_Data.h"
#include "atn/D2A_Message.h"
#include "std/string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s);
extern "C" float fn_801F0544(atn::D2A_ParamTable* table, int key);
extern "C" const char lbl_8073E57C[];
extern "C" void fn_800D58A4(atn::Texture_unk* texture);
extern "C" int fn_800D59B8(atn::Texture_unk* texture);
extern "C" void fn_801FE99C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* anim);
extern "C" float fn_800A6BD4(const unsigned char* code, float value);
extern "C" void fn_801EDCB8(atn::D2A_Common* element);
extern "C" unsigned int fn_800DC6B4(const char* text);
extern "C" int fn_801F03AC(void* table, int key);
extern "C" std::basic_string<wchar_t> fn_801F0758(void* table, int key);
extern "C" void fn_801F0364(void* table, int key, int value, int arg);

namespace atn {

// NONMATCHING: 99.13% - equivalent loop and array setup retain different GPR assignments from MWCC.
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

// NONMATCHING: the pointer-array growth sequence has a different register schedule.
void D2A_Texture::vf0C() {
    fn_801EC784();
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner != 0) {
        owner->m10.mTextureAnims.PushBackRef_unk(mTexRect);
        owner->m10.mTextureAnims.PushBackRef_unk(mTexScale);
    }
}

void D2A_Texture::vf14() {}

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
    if (texture->mExpr0.mCode != 0)
        rect[0] = fn_800A6BD4((const unsigned char*)texture->mExpr0.mCode, rect[0]);
    if (texture->mExpr0.mTail[0] != 0)
        rect[1] = fn_800A6BD4((const unsigned char*)texture->mExpr0.mTail[0], rect[1]);
    if (texture->mExpr0.mTail[1] != 0)
        rect[2] = fn_800A6BD4((const unsigned char*)texture->mExpr0.mTail[1], rect[2]);
    if (texture->mExpr0.mTail[2] != 0)
        rect[3] = fn_800A6BD4((const unsigned char*)texture->mExpr0.mTail[2], rect[3]);
    if (texture->mExpr1.mCode != 0)
        scale[0] = fn_800A6BD4((const unsigned char*)texture->mExpr1.mCode, scale[0]);
    if (texture->mExpr1.mTail[0] != 0)
        scale[1] = fn_800A6BD4((const unsigned char*)texture->mExpr1.mTail[0], scale[1]);
    if (texture->mExpr1.mTail[1] != 0)
        scale[2] = fn_800A6BD4((const unsigned char*)texture->mExpr1.mTail[1], scale[2]);
    if (texture->mExpr1.mTail[2] != 0)
        scale[3] = fn_800A6BD4((const unsigned char*)texture->mExpr1.mTail[2], scale[3]);
}

// NONMATCHING: the texture layer rendering and UV tile scheduling remain to be reconstructed.
void D2A_Texture::vf18() {
    if (!fn_801EDC78()) {
        return;
    }
    if (fn_800D59B8(&mTexture0) != 0) {
        return;
    }
    if ((m10 & 8) != 0 && fn_800D59B8(&mTexture1) != 0) {
        return;
    }
    D2A_DrawRect rect;
    D2A_DrawColor color;
    D2A_DrawExtra extra;
    int work[6];
    fn_801ED928(&rect, &color, &extra, work);
    float uv[4];
    float scale[4];
    fn_801F9E70(this, uv, scale);
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner != 0 && owner->mA4 != 0) {
        typedef void (*DrawCallback_unk)(D2A_Texture*, D2A_DrawColor*, void*);
        ((DrawCallback_unk)owner->mA4)(this, &color, 0);
    }
}

// inferred: removes a pointer from one of the layout's packed pointer arrays.
struct D2A_PointerArray_unk {
    unsigned int mSize;
    unsigned int mCapacity;
    void** mData;
};

static void RemoveLayoutPointer_unk(D2A_PointerArray_unk* array, const void* value) {
    for (unsigned int i = 0; i < array->mSize; i++) {
        if (array->mData[i] == value) {
            array->mSize--;
            for (; i < array->mSize; i++) {
                array->mData[i] = array->mData[i + 1];
            }
            return;
        }
    }
}

// NONMATCHING: register allocation differs from the original packed-array erase loop.
extern "C" void fn_801FE89C(atn::D2A_Layout_unk* layout, const void* value) {
    RemoveLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x00), value);
}
// NONMATCHING: register allocation differs from the original packed-array erase loop.
extern "C" void fn_801FE91C(atn::D2A_Layout_unk* layout, const void* value) {
    RemoveLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x0C), value);
}
// NONMATCHING: register allocation differs from the original packed-array erase loop.
extern "C" void fn_801FE99C(atn::D2A_Layout_unk* layout, const void* value) {
    RemoveLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x18), value);
}
// NONMATCHING: register allocation differs from the original packed-array erase loop.
extern "C" void fn_801FEA1C(atn::D2A_Layout_unk* layout, const void* value) {
    RemoveLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x24), value);
}
// NONMATCHING: register allocation differs from the original packed-array erase loop.
extern "C" void fn_801FEA9C(atn::D2A_Layout_unk* layout, const void* value) {
    RemoveLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x30), value);
}
// NONMATCHING: register allocation differs from the original packed-array erase loop.
extern "C" void fn_801FEB1C(atn::D2A_Layout_unk* layout, const void* value) {
    RemoveLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x3C), value);
}

static void* FindLayoutPointer_unk(D2A_PointerArray_unk* array, unsigned int hash) {
    for (unsigned int i = 0; i < array->mSize; i++) {
        void* value = array->mData[i];
        if (*(unsigned int*)value == hash) {
            return value;
        }
    }
    return 0;
}

// NONMATCHING: string hash lookup differs in loop and register allocation.
extern "C" void* fn_801FEB9C(atn::D2A_Layout_unk* layout, const char* name) {
    return FindLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x00), fn_800DC6B4(name));
}
// NONMATCHING: string hash lookup differs in loop and register allocation.
extern "C" void* fn_801FEC08(atn::D2A_Layout_unk* layout, const char* name) {
    return FindLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x0C), fn_800DC6B4(name));
}
// NONMATCHING: string hash lookup differs in loop and register allocation.
extern "C" void* fn_801FEC74(atn::D2A_Layout_unk* layout, const char* name) {
    return FindLayoutPointer_unk((D2A_PointerArray_unk*)((char*)layout + 0x30), fn_800DC6B4(name));
}

static void SetLayoutFloat_unk(D2A_PointerArray_unk* array, unsigned int hash, int index, float value) {
    for (unsigned int i = 0; i < array->mSize; i++) {
        atn::D2A_Anim_unk<1>* anim = (atn::D2A_Anim_unk<1>*)array->mData[i];
        if (anim->mName == hash) {
            anim->mTables[index].fn_801F04FC(0, 1, value);
            return;
        }
    }
}

// NONMATCHING: table lookup and setter call shape differ from the original.
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x00), hash, 0, value);
}
// NONMATCHING: table lookup and setter call shape differ from the original.
extern "C" void fn_801FED24(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x0C), hash, index, value);
}
// NONMATCHING: table lookup and setter call shape differ from the original.
extern "C" void fn_801FED70(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x18), hash, index, value);
}
// NONMATCHING: table lookup and setter call shape differ from the original.
extern "C" void fn_801FEDBC(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value) {
    SetLayoutFloat_unk((D2A_PointerArray_unk*)((char*)layout + 0x24), hash, index, value);
}
// NONMATCHING: integer table lookup differs from the original call sequence.
extern "C" void fn_801FEEFC(atn::D2A_Layout_unk* layout, unsigned int hash, int value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x3C);
    for (unsigned int i = 0; i < array->mSize; i++) {
        atn::D2A_IntAnim_unk* anim = (atn::D2A_IntAnim_unk*)array->mData[i];
        if (anim->mName == hash) {
            fn_801F0364(&anim->mTables[0], 0, value, 1);
            return;
        }
    }
}

// NONMATCHING: string construction and parameter replacement differ from the original.
extern "C" void fn_801FEE08(atn::D2A_Layout_unk* layout, unsigned int hash, const wchar_t* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x30);
    for (unsigned int i = 0; i < array->mSize; i++) {
        atn::D2A_TextParam_unk* text = (atn::D2A_TextParam_unk*)array->mData[i];
        if (text->mId == hash) {
            std::basic_string<wchar_t> copy(value);
            text->fn_801F25E0(0, copy, 0, 1);
            return;
        }
    }
}

// NONMATCHING: the animated value loops do not yet reproduce the original register schedule.
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

extern "C" void fn_801FD530() {}
extern "C" void fn_801FDE90(atn::D2A_Common* element) { fn_801EDCB8(element); }

D2A_Layout_unk::D2A_Layout_unk() {}
// NONMATCHING: array cleanup differs from the original deleting destructor sequence.
D2A_Layout_unk::~D2A_Layout_unk() {}

// NONMATCHING: the inlined reserve and element-copy sequences differ from MWCC's loop schedule.
extern "C" void fn_801FE090(D2A_Layout_unk* layout, const unsigned int* counts) {
    layout->mAnims1.Reserve(counts[0], 8);
    layout->mAnims5.Reserve(counts[1], 8);
    layout->mTextureAnims.Reserve(counts[2], 8);
    layout->mAnims4.Reserve(counts[3], 8);
    layout->mTexts.Reserve(counts[4], 8);
    layout->mIntAnims.Reserve(counts[5], 8);
}

// NONMATCHING: loading-state branches have different control flow from the original.
extern "C" int fn_801FD534(atn::D2A_Texture* texture) {
    if (fn_800D59B8(&texture->mTexture0) != 0) {
        return 1;
    }
    if ((texture->m10 & 8) != 0 && fn_800D59B8(&texture->mTexture1) != 0) {
        return 1;
    }
    return 0;
}

// NONMATCHING: resource path scanning and texture creation remain to be reconstructed.
extern "C" int fn_801FD5A0(atn::D2A_Texture* texture, const char* path, int arg) {
    ((void)texture);
    ((void)path);
    ((void)arg);
    return 0;
}

// NONMATCHING: resource path scanning and texture creation remain to be reconstructed.
extern "C" int fn_801FDA20(atn::D2A_Texture* texture, const char* path, int arg) {
    ((void)texture);
    ((void)path);
    ((void)arg);
    return 0;
}

} // namespace atn
