#ifndef ATN_D2A_COMMON_H
#define ATN_D2A_COMMON_H

namespace atn {

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

    int m04;
    unsigned short mType; // 0x08
    unsigned char m0A[0x30 - 0x0A];
    D2A_ParamTable m30;
    D2A_ParamTable m38;
    unsigned char m40[0x58 - 0x40];
    float m58;
    unsigned char m5C[0x12C - 0x5C];
};

} // namespace atn

#endif
