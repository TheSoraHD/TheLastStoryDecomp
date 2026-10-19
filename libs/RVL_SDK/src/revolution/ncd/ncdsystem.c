// Written from the target assembly (RevoEX NCD library, Jun 9 2009 build).
#include <revolution/OS.h>
#include <revolution/IPC.h>
#include <revolution/version.h>
#include <string.h>

// The SDK headers of this build define NULL as a pointer constant (pointer checks use cmplw).
#undef NULL
#define NULL ((void*)0)

// inferred: layout of one network connection setting (0x91C bytes)
typedef struct NCDiConfigEntry {
    u8 flags;             // 0x000 inferred: 0x80 enabled, 0x01 wired, 0x06 ip set, 0x10 proxy
    u8 _01[3];            // 0x001
    u8 ip[0x14];          // 0x004
    u8 dns[0xC];          // 0x018
    u8 proxy[0x79C];      // 0x024
    u8 wireless[0x15C];   // 0x7C0
} NCDiConfigEntry;

// inferred: settings file image handled by /dev/net/ncd/manage (0x1B5C bytes)
typedef struct NCDiConfig {
    u8 _00[6];                  // 0x000
    u8 nwc24Permission;         // 0x006 inferred
    u8 _07;                     // 0x007
    NCDiConfigEntry entries[3]; // 0x008
} NCDiConfig;

// inferred: interface settings returned by NCDGetCurrentIfConfig
typedef struct NCDIfConfig {
    u8 type;            // 0x00 inferred: 1 wireless, 2 wired
    u8 nwc24Permission; // 0x01
    u8 data[0x15C];     // 0x02
} NCDIfConfig;

// inferred: IP settings returned by NCDGetCurrentIpConfig
typedef struct NCDIpConfig {
    BOOL useIp;         // 0x00
    BOOL useProxy;      // 0x04
    u8 ip[0x14];        // 0x08
    u8 dns[0xC];        // 0x1C
    u8 proxy[0x79C];    // 0x28
} NCDIpConfig;

// inferred: reply buffer filled by the NCD ioctls
typedef struct NCDiResult {
    s32 result; // 0x00
    s32 value;  // 0x04
    u8 _08[0x18];
} NCDiResult;

#define NCD_HEAP_SIZE 0x1B60
#define NCD_CONFIG_SIZE sizeof(NCDiConfig)

RVL_LIB_VERSION(NCD, "Jun  9 2009", "11:59:48", "0x4199_60831");

// inferred: names of the static work data. The explicit initializer places the mutex first in
// .bss, as in the target.
static OSMutex NcdMutex __attribute__((aligned(32))) = {0};
static NCDiResult NcdResult __attribute__((aligned(32)));
static IPCIOVector NcdVectors[4] __attribute__((aligned(32)));
static u32 NcdInitialized = 0;   // inferred
static NCDiConfig* NcdConfig;    // inferred: settings image in the IPC arena

static void fn_806984C0(void);
s32 fn_8069832C(const char* funcName, NCDiConfig* config, s32 command);

s32 NCDGetCurrentIfConfig(NCDIfConfig* ifConfig) {
    s32 result = 0;
    NCDiConfig* config;
    s32 index;
    NCDiConfigEntry* entry;

    if (ifConfig == NULL) {
        return -3;
    }

    fn_806984C0();
    result = fn_8069832C("NCDGetCurrentIfConfig", NULL, 3);
    if (result == 0) {
        config = NcdConfig;
        index = NcdResult.value;
        if (index < 0 || index >= 3) {
            result = -7;
        } else {
            entry = &config->entries[index];
            if (entry->flags & 1) {
                ifConfig->type = 2;
                memcpy(ifConfig->data, entry->wireless, 4);
            } else {
                ifConfig->type = 1;
                memcpy(ifConfig->data, entry->wireless, 0x15C);
            }
            ifConfig->nwc24Permission = config->nwc24Permission;
        }
    }
    OSUnlockMutex(&NcdMutex);
    return result;
}

s32 NCDGetCurrentIpConfig(NCDIpConfig* ipConfig) {
    s32 result = 0;
    NCDiConfig* config;
    s32 index;

    if (ipConfig == NULL) {
        return -3;
    }

    fn_806984C0();
    result = fn_8069832C("NCDGetCurrentIpConfig", NULL, 3);
    if (result == 0) {
        config = NcdConfig;
        index = NcdResult.value;
        if (index < 0 || index >= 3) {
            result = -7;
        } else {
            memcpy(ipConfig->dns, config->entries[index].dns, 0xC);
            if (config->entries[index].flags & 6) {
                ipConfig->useIp = TRUE;
                memcpy(ipConfig->ip, config->entries[index].ip, 0x14);
            } else {
                ipConfig->useIp = FALSE;
                memcpy(ipConfig->ip, config->entries[index].ip, 0x14);
            }
            if (config->entries[index].flags & 0x10) {
                ipConfig->useProxy = TRUE;
                memcpy(ipConfig->proxy, config->entries[index].proxy, 0x79C);
            } else {
                ipConfig->useProxy = FALSE;
                memset(ipConfig->proxy, 0, 0x79C);
            }
        }
    }
    OSUnlockMutex(&NcdMutex);
    return result;
}

s32 NCDGetLinkStatus(void) {
    s32 result;
    s32 fd;

    if (!OSGetCurrentThread()) {
        return -5;
    }

    fn_806984C0();
    fd = IOS_Open("/dev/net/ncd/manage", 0);
    if (fd < 0) {
        if (fd == -6) {
            result = -8;
        } else {
            result = -2;
        }
    } else {
        NcdVectors[0].base = &NcdResult;
        NcdVectors[0].length = sizeof(NCDiResult);
        if (IOS_Ioctlv(fd, 7, 0, 1, NcdVectors) < 0) {
            result = -2;
        } else {
            result = NcdResult.result;
            if (result == 0) {
                result = NcdResult.value;
                result = (result >= 0) ? result : -1;
            }
        }
        if (IOS_Close(fd) < 0) {
            result = -1;
        }
    }
    OSUnlockMutex(&NcdMutex);
    return result;
}

s32 NCDGetWirelessMacAddress(u8* mac) {
    s32 result = 0;
    s32 fd;

    if (mac == NULL) {
        return -3;
    }
    if (!OSGetCurrentThread()) {
        return -5;
    }

    fn_806984C0();
    fd = IOS_Open("/dev/net/ncd/manage", 0);
    if (fd < 0) {
        if (fd == -6) {
            result = -8;
        } else {
            result = -2;
        }
    } else {
        NcdVectors[0].base = &NcdResult;
        NcdVectors[0].length = sizeof(NCDiResult);
        NcdVectors[1].base = NcdConfig;
        NcdVectors[1].length = 6;
        if (IOS_Ioctlv(fd, 8, 0, 2, NcdVectors) < 0) {
            result = -2;
        } else {
            result = NcdResult.result;
            if (result == 0) {
                memcpy(mac, NcdConfig, 6);
            }
        }
        if (IOS_Close(fd) < 0) {
            result = -1;
        }
    }
    OSUnlockMutex(&NcdMutex);
    return result;
}

// NONMATCHING: 99.0% - r4/r5 swapped inside the unrolled loop (config base vs flags byte); tried
// entry pointer/flags locals, declaration orders, bitfields, while/continue forms, NcdConfig direct.
s32 NCDiGetEnabledConfigList(u32* wired, u32* wireless, u32* wirelessAoss) {
    s32 result;
    u32 wiredList = 0;
    u32 wirelessList = 0;
    u32 aossList = 0;
    NCDiConfig* config;
    int i;
    NCDiConfigEntry* entry;

    fn_806984C0();
    result = fn_8069832C("NCDiGetEnabledConfigList", NULL, 3);
    if (result == 0) {
        config = NcdConfig;
        for (i = 0; i < 3; i++) {
            entry = &config->entries[i];
            if (entry->flags & 0x80) {
                if (entry->flags & 1) {
                    wiredList |= 1 << i;
                } else {
                    if (entry->wireless[2] != 1) {
                        wirelessList |= 1 << i;
                    }
                    if (entry->wireless[2] == 1) {
                        aossList |= 1 << i;
                    }
                }
            }
        }
    }
    OSUnlockMutex(&NcdMutex);

    if (wired != 0) {
        *wired = wiredList;
    }
    if (wireless != 0) {
        *wireless = wirelessList;
    }
    if (wirelessAoss != 0) {
        *wirelessAoss = aossList;
    }
    return result;
}

s32 fn_8069832C(const char* funcName, NCDiConfig* config, s32 command) {
    s32 result = 0;
    s32 fd;

    if (!OSGetCurrentThread()) {
        return -5;
    }

    fn_806984C0();
    fd = IOS_Open("/dev/net/ncd/manage", 0);
    if (fd < 0) {
        if (fd == -6) {
            result = -8;
        } else {
            result = -2;
        }
    } else {
        NcdVectors[0].base = NcdConfig;
        NcdVectors[0].length = NCD_CONFIG_SIZE;
        NcdVectors[1].base = &NcdResult;
        NcdVectors[1].length = sizeof(NCDiResult);
        switch (command) {
        case 3:
        case 5:
            if (IOS_Ioctlv(fd, command, 0, 2, NcdVectors) < 0) {
                result = -2;
            } else {
                result = NcdResult.result;
                if (result == 0 && config != NULL) {
                    memcpy(config, NcdConfig, NCD_CONFIG_SIZE);
                }
            }
            break;
        case 4:
        case 6:
            if (config != NULL) {
                memcpy(NcdConfig, config, NCD_CONFIG_SIZE);
            }
            if (IOS_Ioctlv(fd, command, 1, 1, NcdVectors) < 0) {
                result = -2;
            } else {
                result = NcdResult.result;
            }
            break;
        }
        if (IOS_Close(fd) < 0) {
            result = -1;
        }
    }
    OSUnlockMutex(&NcdMutex);
    return result;
}

static void fn_806984C0(void) {
    BOOL enabled = OSDisableInterrupts();
    u8* heap;

    if (!(NcdInitialized & 1)) {
        OSRegisterVersion(__NCDVersion);
        OSInitMutex(&NcdMutex);
        heap = (u8*)(((u32)IPCGetBufferLo() + 31) & ~31);
        if ((u32)IPCGetBufferHi() - (u32)heap < NCD_HEAP_SIZE) {
            OSPanic("ncdsystem.c", 1500, "Could not reserve heap for NCD library from IPC arena");
        }
        IPCSetBufferLo(heap + NCD_HEAP_SIZE);
        NcdConfig = (NCDiConfig*)heap;
        memset(heap, 0, NCD_HEAP_SIZE);
        memset(&NcdResult, 0, sizeof(NCDiResult));
        memset(NcdVectors, 0, sizeof(NcdVectors));
        NcdInitialized |= 1;
    }
    OSRestoreInterrupts(enabled);
    OSLockMutex(&NcdMutex);
}
