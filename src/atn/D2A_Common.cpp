#include "atn/D2A_Data.h"
#include "math/Vec.h"

// Base class of the elements of a d2anime layout: animated position, color, rotation, blend and
// shadow, registered in the layout of the owner, and the draw parameters combined with the parent.

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: key tables (atn/D2A_ParamTable_unk.cpp)
extern "C" float fn_801F0544(atn::D2A_ParamTable* table, int key);
extern "C" void fn_801F0364(atn::D2A_IntParamTable* table, int key, int value, int arg);
extern "C" int fn_801F03AC(atn::D2A_IntParamTable* table, int key);
// inferred: unregister the animated values from the layout (automatic names)
extern "C" void fn_801FE89C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<1>* anim);
extern "C" void fn_801FE91C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<5>* anim);
extern "C" void fn_801FEA1C(atn::D2A_Layout_unk* layout, atn::D2A_Anim_unk<4>* anim);
extern "C" void fn_801FEB1C(atn::D2A_Layout_unk* layout, atn::D2A_IntAnim_unk* anim);
// inferred: evaluates an expression of the element with `x` as argument (atn/ParseTree.cpp)
extern "C" float fn_800A6BD4(const unsigned char* code, float x);

namespace atn {

// inferred: shadow parameters (offset and color) of the draw parameters (names invented)
struct D2A_Shadow_unk {
    float x;
    float y;
    float r, g, b, a;
};

// inferred: register an animated value in the layout (names invented; with this extra inline
// level the registers of the original come out, as in D2A_Message)
inline void Add_unk(D2A_Layout_unk* layout, D2A_IntAnim_unk& anim) {
    layout->mIntAnims.PushBackRef_unk(anim);
}
inline void Add_unk(D2A_Layout_unk* layout, D2A_Anim_unk<4>& anim) {
    layout->mAnims4.PushBackRef_unk(anim);
}
inline void Add_unk(D2A_Layout_unk* layout, D2A_Anim_unk<5>& anim) {
    layout->mAnims5.PushBackRef_unk(anim);
}
inline void Add_unk(D2A_Layout_unk* layout, D2A_Anim_unk<1>& anim) {
    layout->mAnims1.PushBackRef_unk(anim);
}

// inferred: current values at frame `frame` (name invented)
template <int N>
inline void UpdateAnim_unk(D2A_Anim_unk<N>& anim, int frame) {
    for (int i = 0; i < N; i++) {
        anim.mValues[i] = fn_801F0544(&anim.mTables[i], frame);
    }
}

D2A_Common::D2A_Common(D2A_Data* owner, unsigned short id)
    : m04(0), mId(id), m0C(0), m0E(0), m10(0), mB0(0.0f), mB4(0.0f), mFC((D2A_Owner_unk*)owner), mParent(0) {
    m14 = 0.0f;
    m18 = 60.0f;
    char name[32];

    sprintf(name, "pos%04d\0", id);
    mRect.mName = fn_800DC6B4(name);
    mRect.mTables[0].fn_801F04FC(0, 1, 10.0f);
    mRect.mTables[1].fn_801F04FC(0, 1, 10.0f);
    mRect.mTables[2].fn_801F04FC(0, 1, 30.0f);
    mRect.mTables[3].fn_801F04FC(0, 1, 30.0f);
    mRect.mTables[4].fn_801F04FC(0, 1, 0.0f);
    UpdateAnim_unk(mRect, 0);

    sprintf(name, "col%04d\0", id);
    mColor.mName = fn_800DC6B4(name);
    mColor.mTables[0].fn_801F04FC(0, 1, 255.0f);
    mColor.mTables[1].fn_801F04FC(0, 1, 255.0f);
    mColor.mTables[2].fn_801F04FC(0, 1, 255.0f);
    mColor.mTables[3].fn_801F04FC(0, 1, 255.0f);
    UpdateAnim_unk(mColor, 0);

    sprintf(name, "rot%04d\0", id);
    mRot.mName = fn_800DC6B4(name);
    mRot.mTables[0].fn_801F04FC(0, 1, 0.0f);
    UpdateAnim_unk(mRot, 0);

    sprintf(name, "blend%04d\0", id);
    mBlend.mName = fn_800DC6B4(name);
    fn_801F0364(&mBlend.mTables[0], 0, 0, 1);
    mBlend.mValues[0] = fn_801F03AC(&mBlend.mTables[0], 0);

    sprintf(name, "sdw_col%04d\0", id);
    mShadowColor.mName = fn_800DC6B4(name);
    mShadowColor.mTables[0].fn_801F04FC(0, 1, 255.0f);
    mShadowColor.mTables[1].fn_801F04FC(0, 1, 255.0f);
    mShadowColor.mTables[2].fn_801F04FC(0, 1, 255.0f);
    mShadowColor.mTables[3].fn_801F04FC(0, 1, 255.0f);
    UpdateAnim_unk(mShadowColor, 0);

    sprintf(name, "sdw_ofs%04d\0", id);
    mShadowOffset.mName = fn_800DC6B4(name);
    mShadowOffset.mTables[0].fn_801F04FC(0, 1, 1.0f);
    UpdateAnim_unk(mShadowOffset, 0);
}

D2A_Common::~D2A_Common() {
    if (mFC != 0) {
        fn_801FE91C(&((D2A_Data*)mFC)->m10, &mRect);
        fn_801FEA1C(&((D2A_Data*)mFC)->m10, &mColor);
        fn_801FE89C(&((D2A_Data*)mFC)->m10, &mRot);
        fn_801FEB1C(&((D2A_Data*)mFC)->m10, &mBlend);
        fn_801FEA1C(&((D2A_Data*)mFC)->m10, &mShadowColor);
        fn_801FE89C(&((D2A_Data*)mFC)->m10, &mShadowOffset);
    }
}

void D2A_Common::fn_801EC784() {
    if (mFC != 0) {
        Add_unk(&((D2A_Data*)mFC)->m10, mRect);
        Add_unk(&((D2A_Data*)mFC)->m10, mColor);
        Add_unk(&((D2A_Data*)mFC)->m10, mRot);
        Add_unk(&((D2A_Data*)mFC)->m10, mBlend);
        Add_unk(&((D2A_Data*)mFC)->m10, mShadowColor);
        Add_unk(&((D2A_Data*)mFC)->m10, mShadowOffset);
    }
}

void D2A_Common::vf14() {}

} // namespace atn

// inferred: sets the parent element (name kept: called by the loader as extern "C")
extern "C" void fn_801ED910(atn::D2A_Common* child, atn::D2A_Common* parent) {
    child->mParent = parent;
    if (parent != 0) {
        child->m0C = parent->mId;
    }
}

namespace atn {

void D2A_Common::fn_801ED928(D2A_DrawRect* rect, D2A_DrawColor* color, D2A_DrawExtra* extra, void* work) {
    D2A_DrawRect parentRect;
    D2A_DrawColor parentColor;
    D2A_DrawExtra parentExtra;
    D2A_Shadow_unk parentShadow;
    D2A_Shadow_unk* shadow = (D2A_Shadow_unk*)work;

    rect->x = mRect.mValues[0];
    rect->y = mRect.mValues[1];
    rect->w = mRect.mValues[2];
    rect->h = mRect.mValues[3];
    rect->z = mRect.mValues[4];
    color->a = mColor.mValues[0];
    color->r = mColor.mValues[1];
    color->g = mColor.mValues[2];
    color->b = mColor.mValues[3];
    extra->m00 = mRot.mValues[0];
    extra->m04 = mBlend.mValues[0];
    shadow->a = mShadowColor.mValues[0];
    shadow->r = mShadowColor.mValues[1];
    shadow->g = mShadowColor.mValues[2];
    shadow->b = mShadowColor.mValues[3];
    shadow->x = mShadowOffset.mValues[0];
    shadow->y = mShadowOffset.mValues[0];

    if (mParent != 0) {
        mParent->fn_801ED928(&parentRect, &parentColor, &parentExtra, &parentShadow);
        rect->x *= parentRect.w;
        rect->y *= parentRect.h;
        rect->w *= parentRect.w;
        rect->h *= parentRect.h;
        color->a *= parentColor.a;
        Vector3 v(rect->x, rect->y, 0.0f);
        Mtx m;
        PSMTXRotRad(m, 'z', DegToRad_unk(parentExtra.m00));
        PSMTXMultVec(m, &v, &v);
        rect->x = v.x;
        rect->y = v.y;
        rect->x += parentRect.x;
        rect->y += parentRect.y;
    }

    if (mRectCode[0].mCode != 0) {
        rect->x = fn_800A6BD4(mRectCode[0].mCode, rect->x);
    }
    if (mRectCode[1].mCode != 0) {
        rect->y = fn_800A6BD4(mRectCode[1].mCode, rect->y);
    }
    if (mRectCode[2].mCode != 0) {
        rect->w = fn_800A6BD4(mRectCode[2].mCode, rect->w);
    }
    if (mRectCode[3].mCode != 0) {
        rect->h = fn_800A6BD4(mRectCode[3].mCode, rect->h);
    }
    if (mRectCode[4].mCode != 0) {
        rect->z = fn_800A6BD4(mRectCode[4].mCode, rect->z);
    }
    if (mColorCode[0].mCode != 0) {
        color->a = fn_800A6BD4(mColorCode[0].mCode, color->a) / 255.0f;
    } else {
        color->a = color->a / 255.0f;
    }
    if (mColorCode[1].mCode != 0) {
        color->r = fn_800A6BD4(mColorCode[1].mCode, color->r) / 255.0f;
    } else {
        color->r = color->r / 255.0f;
    }
    if (mColorCode[2].mCode != 0) {
        color->g = fn_800A6BD4(mColorCode[2].mCode, color->g) / 255.0f;
    } else {
        color->g = color->g / 255.0f;
    }
    if (mColorCode[3].mCode != 0) {
        color->b = fn_800A6BD4(mColorCode[3].mCode, color->b) / 255.0f;
    } else {
        color->b = color->b / 255.0f;
    }
    if (mRotCode.mCode != 0) {
        extra->m00 = fn_800A6BD4(mRotCode.mCode, extra->m00);
    }

    if (mParent != 0) {
        rect->z += parentRect.z;
    }
}

void D2A_Common::vf18() {}

int D2A_Common::fn_801EDC78() {
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner != 0) {
        float frame = owner->mFrame;
        if (m14 <= frame && frame <= m18) {
            return 1;
        }
    }
    return 0;
}

} // namespace atn

extern "C" void fn_801EDCB8(atn::D2A_Common* element) {}

namespace atn {

void D2A_Common::vf20() {}

int D2A_Common::vf1C() {
    return 0;
}

void D2A_Common::vf10() {}

void D2A_Common::vf0C() {
    fn_801EC784();
}

} // namespace atn
