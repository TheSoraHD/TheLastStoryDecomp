#include <revolution/OS.h>

/* inferred: file name unknown (0x80698A28, between NETVersion.c and
   netmemcpy.c); the function forwards to NCD. */

s32 NCDGetWirelessMacAddress(u8* mac);

s32 NETGetWirelessMacAddress(u8* mac) {
    return NCDGetWirelessMacAddress(mac);
}
