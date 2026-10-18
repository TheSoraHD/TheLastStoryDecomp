#ifndef ATN_RAIN_H
#define ATN_RAIN_H

namespace atn {

// inferred: rain renderer (derives from atn::IRender, vtable 0x80778EF8); size 0x14 as a member
class Rain {
public:
    Rain();          // 0x800B1B28
    virtual ~Rain(); // 0x800B1BAC
    void fn_800B1C1C();
    void fn_800B23C8();
    void fn_800B28F8(Rain* a, Rain* b);
    void fn_800B2D2C();

    unsigned char m04[0x14 - 0x04];
};

} // namespace atn

#endif
