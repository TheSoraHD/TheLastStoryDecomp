#ifndef ATN_D2A_COMMON_H
#define ATN_D2A_COMMON_H

namespace atn {

struct D2A_Owner_unk;

// inferred: table of {key, value} float parameters; fn_801F04FC sets the value of `key`.
struct D2A_ParamTable {
    struct Entry {
        int mKey;
        float mValue;
    };
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

// inferred from __vt__Q23atn10D2A_Common; field layout only as far as derived classes use it.
class D2A_Common {
public:
    D2A_Common();          // 0x801EC158
    virtual ~D2A_Common(); // 0x801EC6C0
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();

    int fn_801EDC78(); // inferred: nonzero when the element is drawn
    void fn_801ED928(D2A_DrawRect* rect, D2A_DrawColor* color, D2A_DrawExtra* extra, void* work); // inferred

    int m04;
    unsigned short mType; // 0x08
    unsigned char m0A[0x0E - 0x0A];
    unsigned short m0E; // inferred: outline mode of D2A_Window
    unsigned char m10[0x30 - 0x10];
    D2A_ParamTable m30;
    D2A_ParamTable m38;
    unsigned char m40[0x58 - 0x40];
    float m58;
    unsigned char m5C[0xFC - 0x5C];
    D2A_Owner_unk* mFC; // inferred: owner (layout or data), see D2A_Window::vf18
    unsigned char m100[0x12C - 0x100];
};

} // namespace atn

#endif
