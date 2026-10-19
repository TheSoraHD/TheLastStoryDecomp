// Written from the target assembly (RevoEX NET library, Jun 9 2009 build).
#include <revolution/OS.h>

s32 NCDiGetEnabledConfigList(u32* wired, u32* wireless, u32* wirelessAoss);
s32 fn_806988A8(s32 result, int code);

// inferred: index of the lowest set bit, or -1
static int NETiGetLowestBit(u32 mask) {
    int i;
    u32 bit;

    for (i = 0, bit = 1; i < 32; i++, bit <<= 1) {
        if (mask & bit) {
            return i;
        }
    }
    return -1;
}

// inferred: detail code from the kind of enabled connection settings
int fn_806985B0(u32 wired, u32 wireless, u32 wirelessAoss) {
    int code = 99;
    int index;

    if (wired != 0) {
        if (wireless == 0 && wirelessAoss == 0) {
            index = NETiGetLowestBit(wired);
            code = index + 20;
        }
    } else if (wireless != 0) {
        if (wirelessAoss == 0) {
            index = NETiGetLowestBit(wireless);
            code = index + 30;
        }
    } else if (wirelessAoss != 0) {
        index = NETiGetLowestBit(wirelessAoss);
        code = index + 40;
    }
    return code;
}

s32 NETGetStartupErrorCode(s32 result) {
    u32 wirelessAoss;
    u32 wireless;
    u32 wired;
    int code = 99;

    if (NCDiGetEnabledConfigList(&wired, &wireless, &wirelessAoss) >= 0) {
        code = fn_806985B0(wired, wireless, wirelessAoss);
    }
    if (code < 0) {
        result = (s32)0x80000000;
        code = 99;
    }
    return fn_806988A8(result, code) - code;
}

// inferred: base error code for a SOStartup result
s32 fn_806988A8(s32 result, int code) {
    if (result >= 0) {
        return 0;
    }

    switch (result) {
    case -45:
        return -50200;
    case -28:
        return -50300;
    case -62:
        return -50400;
    case -111:
        return -52700;
    case -121:
        if (code >= 20 && code < 30) {
            return -51400;
        }
        return -51000;
    case -112:
    case -76:
    case -48:
    case -39:
        if (code >= 20 && code < 30) {
            return -51400;
        }
        return -51300;
    case -102:
    case -101:
    case -100:
        return -52000;
    case (s32)0x80000000:
        return -50100;
    default:
        OSReport("Unknown SOStartup Error: %d\n", result);
        return -50100;
    }
}
