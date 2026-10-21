#ifndef ATN_D2A_COMMON_H
#define ATN_D2A_COMMON_H

namespace atn {

struct D2A_Owner_unk;
class D2A_Data;

// inferred: table of {key, value} float parameters; fn_801F04FC sets the value of `key`.
struct D2A_ParamTable {
    struct Entry {
        int mKey;
        float mValue;
    };
    D2A_ParamTable() : mEntries(0), mCount(0) {}
    void fn_801F04FC(int key, int arg, float value); // arg unused by the callee
    Entry* mEntries; // 0x00
    int mCount;      // 0x04
};

// inferred: draw parameters computed by D2A_Common::fn_801ED928
struct D2A_DrawRect {
    D2A_DrawRect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), z(0.0f) {}
    float x, y, w, h, z;
};
struct D2A_DrawColor {
    D2A_DrawColor() {} // 0x80041B8C (weak)
    float r, g, b, a;
};
struct D2A_DrawExtra {
    D2A_DrawExtra() : m00(0.0f), m04(0) {}
    float m00;
    int m04;
};

// inferred: int keyed table (same layout as D2A_ParamTable; functions 0x801F0364/0x801F03AC)
struct D2A_IntParamTable {
    struct Entry {
        int mKey;
        int mValue;
    };
    D2A_IntParamTable() : mEntries(0), mCount(0) {}
    Entry* mEntries; // 0x00
    int mCount;      // 0x04
};

// inferred: animated values of an element: name hash, N key tables and their current values
// (names invented; the loader reads them as D2A_TrackGroup_unk)
template <int N>
struct D2A_Anim_unk {
    D2A_Anim_unk() : mName(0) {}
    unsigned int mName;          // 0x00: hash of "pos%04d", "col%04d"...
    D2A_ParamTable mTables[N];   // 0x04
    float mValues[N];
};

// inferred: animated int value (blend mode)
struct D2A_IntAnim_unk {
    D2A_IntAnim_unk() : mName(0) {}
    unsigned int mName;
    D2A_IntParamTable mTables[1]; // 0x04
    int mValues[1];               // 0x0C
};

// inferred: byte code of an expression of the element (see fn_800A6BD4); name invented
struct D2A_Expr_unk {
    D2A_Expr_unk() : mCode(0) {}
    const unsigned char* mCode;
};

// inferred from __vt__Q23atn10D2A_Common; field layout only as far as derived classes use it.
class D2A_Common {
public:
    D2A_Common(D2A_Data* owner, unsigned short id); // 0x801EC158
    virtual ~D2A_Common(); // 0x801EC6C0
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual int vf1C(); // inferred: nonzero while loading
    virtual void vf20();

    void fn_801EC784(); // inferred: registers the animated values in the layout of the owner
    int fn_801EDC78(); // inferred: nonzero when the element is drawn
    void fn_801ED928(D2A_DrawRect* rect, D2A_DrawColor* color, D2A_DrawExtra* extra, void* work); // inferred

    int m04;
    unsigned short mType; // 0x08
    unsigned short mId; // 0x0A
    unsigned short m0C; // inferred: id of the parent
    unsigned short m0E; // inferred: outline mode of D2A_Window
    union {
        unsigned int m10; // inferred: flags (cleared as a word by the constructor)
        struct {
            unsigned int m10_0 : 29;
            unsigned int mOutline : 1; // inferred: draws the outline of the text (D2A_Message)
            unsigned int m10_30 : 2;
        };
    };
    float m14; // inferred: start frame of the element
    float m18; // inferred: end frame of the element
    D2A_Anim_unk<5> mRect;      // 0x1C: "pos": x, y, w, h, z
    D2A_Anim_unk<4> mColor;     // 0x5C: "col": a, r, g, b
    D2A_Anim_unk<1> mRot;       // 0x90: "rot": degrees
    D2A_IntAnim_unk mBlend;     // 0xA0: "blend"
    float mB0;
    float mB4;
    D2A_Anim_unk<4> mShadowColor;  // 0xB8: "sdw_col": a, r, g, b
    D2A_Anim_unk<1> mShadowOffset; // 0xEC: "sdw_ofs"
    D2A_Owner_unk* mFC;         // inferred: owner (a D2A_Data; D2A_Window reads it as D2A_Owner_unk)
    D2A_Common* mParent;        // 0x100
    D2A_Expr_unk mRectCode[5];  // 0x104: inferred: expressions of the values
    D2A_Expr_unk mColorCode[4]; // 0x118
    D2A_Expr_unk mRotCode;      // 0x128
};

} // namespace atn

#endif
