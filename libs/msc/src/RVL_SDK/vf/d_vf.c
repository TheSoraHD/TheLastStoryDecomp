#include <revolution/os/OSMutex.h>
#include <revolution/vf.h>

// inferred: the original .bss keeps the 32-byte alignment of the deadstripped l_vf_drive_work
__declspec(align(32)) static OSMutex l_Mutex;
#if defined(VERSION_R4QK01)
static s32 l_vf_init = 0;
static s32 l_InitedMutex = 0;
#else
static s32 l_InitedMutex;
static s32 l_vf_init;
#endif
// TLS: l_vf_drive_work (used only by VFInit) is deadstripped

#define VF_ERROR_B001 0xB001
#define VF_ERR_GENERIC 0xB002
#define VF_ERR_DRIVE_NOT_FOUND 0xB003
#define VF_ERR_0xB004 0xB004
#define VF_ERR_0xB005 0xB005

static void _VFInitMutex() {
    if (l_InitedMutex == 0) {
        OSInitMutex(&l_Mutex);
        l_InitedMutex = 1;
    }
}

static void _VFLockMutex() {
    if (l_InitedMutex != 0) {
        OSLockMutex(&l_Mutex);
    }
}

static void _VFUnlockMutex() {
    if (l_InitedMutex != 0) {
        OSUnlockMutex(&l_Mutex);
    }
}

#if !defined(VERSION_R4QK01)
s32 VFIsAvailable() {
    return l_vf_init != 0;
}
#endif

void VFInitEx(void* i_heap_start_address_p, u32 i_size) {
    _VFInitMutex();
    _VFLockMutex();

    if (l_vf_init == 0) {
#if !defined(VERSION_R4QK01)
        l_vf_init = 1;
#endif
        VFSysInit(i_heap_start_address_p, i_size);
        VFipdm_init_diskmanager(0, 0);
        VFipf2_init_prfile2(0, 0);
        dHash_InitHashTable();
#if defined(VERSION_R4QK01)
        VFSysSetTimeStampCallback(NULL);
        l_vf_init = 1;
#endif
    }

    _VFUnlockMutex();
}

void VFFinalize() {
    _VFLockMutex();

    if (l_vf_init != 0) {
        VFSysFinalize();
        l_vf_init = 0;
    }

    _VFUnlockMutex();
}

#if defined(VERSION_R4QK01)
s32 VFIsAvailable() {
    return l_vf_init != 0;
}
#endif

static s32 VFAttachDriveNANDFlash(const s8* i_drive) {
    s32 err = VF_ERR_0xB004;
    s32 idx = -1;

    _VFLockMutex();
    if (dHash_GetArg(i_drive) == -1) {
        err = VFSysSetDeviceNANDFlash(&idx, 0, 0);
        if (err == 0) {
            dHash_SetArg(i_drive, idx & 0xFF);
        }
    }
    VFSysSetLastError(err);
    _VFUnlockMutex();
    return err;
}

static s32 VFDetachDrive_common(const s8* i_drive, u8 i_set_err) {
    s32 err;
    s32 idx;

    _VFLockMutex();
    idx = dHash_GetArg(i_drive);
    err = VFSysUnsetDevice(idx);
    if (err == 0) {
        dHash_DeleteData(i_drive);
    }
    if (i_set_err != 0) {
        VFSysSetLastError(err);
    }
    _VFUnlockMutex();
    return err;
}

static s32 VF_activate_drive_common(s32 i_handle_idx, const char* i_sys_file_name_p, void* i_memory_p) {
    s32 err;
    s32 errChk;  // Present in DWARF but unused here.
    struct VF_HANDLE_DRIVE* drive_p;
    struct PF_DRV_TBL* drv_tbl[2];

    err = VFSysCheckExistPrfFile(i_handle_idx, (const s8*)i_sys_file_name_p, i_memory_p);
    if (err != 0) {
        VFSysSetLastError(err);
        return err;
    }

    err = VFSysMountDrv(i_handle_idx, (const s8*)i_sys_file_name_p, i_memory_p);
    if (err != 0) {
        VFSysSetLastError(err);
        return err;
    }

    drive_p = (void*)VFSysGetDriveP(i_handle_idx);

    if (drive_p == 0) {
        VFSysSetLastError(0xb002);
        return 0xb002;
    }

    drv_tbl[0] = &drive_p->pf_drv;
    drv_tbl[1] = 0;

    err = VFipf2_attach(drv_tbl);

    if (err != 0) {
        err = VFipf2_errnum();
        VFSysSetLastError(err);
        VFipdm_close_partition(drive_p->pf_part_p);
        return err;
    }

    return 0;
}

static s32 VF_activate_driveEx(const s8* i_drive, const s8* i_sys_file_name_p, void* i_memory_p) {
    s32 handle_idx;

    handle_idx = dHash_GetArg(i_drive);
    VFSysSetNandFuncEx(handle_idx);
    return VF_activate_drive_common(handle_idx, (const char*)i_sys_file_name_p, i_memory_p);
}

static s32 VFActivateDriveNANDFlashEx(const s8* i_drive, const s8* i_sys_file_name_p) {
    s32 Err;

    _VFLockMutex();
    Err = VF_activate_driveEx(i_drive, i_sys_file_name_p, NULL);
    _VFUnlockMutex();
    return Err;
}

static s32 VFInactivateDrive_common(s32 i_handle_idx, u32 i_mode) {
    s32 Err;

    Err = VFSysUnmountDrv(i_handle_idx, i_mode);
    VFSysSetLastError(Err);
    return Err;
}

static s32 VFInactivateDrive(const s8* i_drive) {
    s32 err;
    s32 handle_idx;

    _VFLockMutex();
    handle_idx = dHash_GetArg(i_drive);
    err = VFInactivateDrive_common(handle_idx, 0);
    _VFUnlockMutex();
    return err;
}

static s8* VF_path2handleidx(long* o_handle_idx_p, const char* i_path_p) {
    s8 drive[8];
    const s8* str_p;
    s32 idx;
    const s8* ret_p;

    *o_handle_idx_p = -1;
    str_p = (const s8*)i_path_p;
    idx = 0;
    ret_p = (const s8*)i_path_p;

    VFipf_memset(drive, 0, 8);

    while (*str_p != '\0') {
        if (*str_p == '\\' || *str_p == '/') {
            break;
        }

        if (*str_p == ':') {
            *o_handle_idx_p = dHash_GetArg(drive);
            if (*o_handle_idx_p == -1 || idx > 7) {
                ret_p = 0;
            } else {
                ret_p = str_p + 1;
            }
            break;
        }

        if (idx < 7) {
            drive[idx] = *str_p;
        }

        str_p++;
        idx++;
    }

    return (s8*)ret_p;
}
