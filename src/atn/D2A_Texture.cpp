#include "atn/D2A_Texture.h"
#include "atn/D2A_Data.h"
#include "atn/D2A_Message.h"
#include "std/string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s);
extern "C" float fn_801F0544(atn::D2A_ParamTable* table, int key);
extern "C" const char lbl_8073E57C[];
extern "C" void fn_800D58A4(atn::Texture_unk* texture);
extern "C" void fn_801FE99C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* anim);
extern "C" float fn_800A6BD4(const unsigned char* code, float value);
extern "C" void fn_801EDCB8(atn::D2A_Common* element);
extern "C" unsigned int fn_800DC6B4(const char* text);
extern "C" int fn_801F03AC(void* table, int key);
extern "C" std::basic_string<wchar_t> fn_801F0758(void* table, int key);
extern "C" void fn_801F0364(void* table, int key, int value, int arg);

struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

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

void D2A_Texture::vf10() {}
void D2A_Texture::vf14() {}
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
void D2A_Texture::vf20() { fn_801EDCB8(this); }

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

// NONMATCHING: 1.54% - texture layer rendering and UV tile scheduling remain incomplete.
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

// inferred: packed pointer-array view used by the D2A_Layout_unk helpers.
struct D2A_PointerArray_unk {
    unsigned int mSize;
    unsigned int mCapacity;
    void** mData;
};

extern "C" void fn_801FE89C(atn::D2A_Layout_unk* layout, const void* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x00);
    void** data = array->mData;
    void** end = data + array->mSize;
    void** it = data;
    while (it != end) {
        if (*it == value) {
            if (it == end) {
                return;
            }
            unsigned int index = it - data;
            unsigned int offset = index * 4;
            while (index < array->mSize - 1) {
                data = (void**)array->mData;
                void** entry = (void**)((char*)data + offset);
                index++;
                offset += 4;
                entry[0] = entry[1];
            }
            array->mSize = array->mSize - 1;
            return;
        }
        it++;
    }
}
extern "C" void fn_801FE91C(atn::D2A_Layout_unk* layout, const void* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x0C);
    void** data = array->mData;
    void** end = data + array->mSize;
    void** it = data;
    while (it != end) {
        if (*it == value) {
            if (it == end) return;
            unsigned int index = it - data;
            unsigned int offset = index * 4;
            while (index < array->mSize - 1) {
                data = (void**)array->mData;
                void** entry = (void**)((char*)data + offset);
                index++;
                offset += 4;
                entry[0] = entry[1];
            }
            array->mSize = array->mSize - 1;
            return;
        }
        it++;
    }
}
extern "C" void fn_801FE99C(atn::D2A_Layout_unk* layout, const void* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x18);
    void** data = array->mData;
    void** end = data + array->mSize;
    void** it = data;
    while (it != end) {
        if (*it == value) {
            if (it == end) return;
            unsigned int index = it - data;
            unsigned int offset = index * 4;
            while (index < array->mSize - 1) {
                data = (void**)array->mData;
                void** entry = (void**)((char*)data + offset);
                index++;
                offset += 4;
                entry[0] = entry[1];
            }
            array->mSize = array->mSize - 1;
            return;
        }
        it++;
    }
}
extern "C" void fn_801FEA1C(atn::D2A_Layout_unk* layout, const void* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x24);
    void** data = array->mData;
    void** end = data + array->mSize;
    void** it = data;
    while (it != end) {
        if (*it == value) {
            if (it == end) return;
            unsigned int index = it - data;
            unsigned int offset = index * 4;
            while (index < array->mSize - 1) {
                data = (void**)array->mData;
                void** entry = (void**)((char*)data + offset);
                index++;
                offset += 4;
                entry[0] = entry[1];
            }
            array->mSize = array->mSize - 1;
            return;
        }
        it++;
    }
}
extern "C" void fn_801FEA9C(atn::D2A_Layout_unk* layout, const void* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x30);
    void** data = array->mData;
    void** end = data + array->mSize;
    void** it = data;
    while (it != end) {
        if (*it == value) {
            if (it == end) return;
            unsigned int index = it - data;
            unsigned int offset = index * 4;
            while (index < array->mSize - 1) {
                data = (void**)array->mData;
                void** entry = (void**)((char*)data + offset);
                index++;
                offset += 4;
                entry[0] = entry[1];
            }
            array->mSize = array->mSize - 1;
            return;
        }
        it++;
    }
}
extern "C" void fn_801FEB1C(atn::D2A_Layout_unk* layout, const void* value) {
    D2A_PointerArray_unk* array = (D2A_PointerArray_unk*)((char*)layout + 0x3C);
    void** data = array->mData;
    void** end = data + array->mSize;
    void** it = data;
    while (it != end) {
        if (*it == value) {
            if (it == end) return;
            unsigned int index = it - data;
            unsigned int offset = index * 4;
            while (index < array->mSize - 1) {
                data = (void**)array->mData;
                void** entry = (void**)((char*)data + offset);
                index++;
                offset += 4;
                entry[0] = entry[1];
            }
            array->mSize = array->mSize - 1;
            return;
        }
        it++;
    }
}

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
