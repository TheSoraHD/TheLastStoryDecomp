#include <revolution/vf.h>

struct PF_VOLUME_SET VFipf_vol_set;

static u32 VFiPFVOL_CheckContextRegistered(s32 context_id) {
    u32 i;
    for (i = 1; i < 1; i++) {
        VFipf_vol_set.context[i].stat &= 1;
        if (VFipf_vol_set.context[i].stat != 0 && VFipf_vol_set.context[i].context_id == context_id) {
            return 1;
        }
    }
    return 0;
}

static s32 VFiPFVOL_SetUpVolumeForMount(struct PF_VOLUME* p_vol) {
    return 0;
}

static s32 VFiPFVOL_InitCurrentDir(struct PF_VOLUME* p_vol) {
    s32 err;
    s32 context_id;
    u32 i;

    if ((p_vol->flags & 2) == 0) {
        return 9;
    }

    VFiPFSYS_GetCurrentContextID(&context_id);
    p_vol->current_dir[0].stat |= 1;

    if (VFiPFVOL_CheckContextRegistered(context_id)) {
        p_vol->current_dir[1].stat |= 1;
        p_vol->current_dir[1].context_id = context_id;
    }

    for (i = 0; i < 1; i++) {
        err = VFiPFENT_GetRootDir(p_vol, &p_vol->current_dir[i].directory);
        if (err) {
            return err;
        }
    }

    return 0;
}

static s32 VFiPFVOL_FinalizeCurrentDir(struct PF_VOLUME* p_vol) {
    u32 i;

    if ((p_vol->flags & 2) == 0) {
        return 9;
    }
    for (i = 0; i < 1; i++) {
        p_vol->current_dir[i].stat = 0;
    }

    return 0;
}

static s32 VFiPFVOL_DoUnmountVolume(struct PF_VOLUME* p_vol, u32 mode) {
    s32 err;

    err = VFiPFDRV_unmount(p_vol, mode);
    if (err) {
        return err;
    }

    VFiPFVOL_FinalizeCurrentDir(p_vol);
    p_vol->flags &= ~2u;
    return 0;
}

static void VFiPFVOL_UnmountVolumeByEject(struct PF_VOLUME* p_vol) {
    VFiPFFILE_FinalizeAllFiles(p_vol);
    VFiPFDIR_FinalizeAllDirs(p_vol);
    VFiPFCACHE_FreeAllCaches(p_vol);
    VFiPFVOL_DoUnmountVolume(p_vol, 1);
    VFipf_vol_set.num_mounted_volumes--;
}

static s32 VFiPFVOL_p_attach(struct PF_VOLUME* p_vol, struct PF_DRV_TBL* p_drv, s16 vol_idx) {
    s32 err;

    VFipf_memset(p_vol, 0, sizeof(struct PF_VOLUME));
    p_vol->p_part = p_drv->p_part;
    p_vol->drv_char = vol_idx + 'A';
    p_vol->tail_entry.tracker_size = 1;
    p_vol->tail_entry.tracker_bits = p_vol->tail_entry.tracker_buff;

    err = VFiPFDRV_init(p_vol);
    if (err != 0) {
        return err;
    }

    VFiPFCACHE_SetCache(p_vol, p_drv->cache->pages, p_drv->cache->buffers, p_drv->cache->num_fat_pages, p_drv->cache->num_data_pages);
    VFiPFCACHE_SetFATBufferSize(p_vol, p_drv->cache->num_fat_buf_size);
    VFiPFCACHE_SetDataBufferSize(p_vol, p_drv->cache->num_data_buf_size);
    return 0;
}

static s32 VFiPFVOL_p_detach(struct PF_VOLUME* p_vol) {
    s32 err;
    err = VFiPFDRV_finalize(p_vol);
    if (err) {
        return err;
    }
    return 0;
}

static s32 VFiPFVOL_p_mount(struct PF_VOLUME* p_vol) {
    s32 err;

    if ((p_vol->flags & 2) == 0) {
        err = VFiPFVOL_DoMountVolume(p_vol);
        if (err != 0) {
            return err;
        }

        p_vol->fsi_flag &= ~7;
        VFipf_vol_set.num_mounted_volumes++;
    }
    return 0;
}

static s32 VFiPFVOL_CheckMediaInsert(struct PF_VOLUME* p_vol) {
    s32 err;

    if (VFiPFDRV_IsInserted(p_vol) != 0) {
        if (VFiPFDRV_IsDetected(p_vol) != 0) {
            if ((p_vol->flags & 2) != 0) {
                VFiPFVOL_UnmountVolumeByEject(p_vol);
            }
            err = VFiPFVOL_p_mount(p_vol);
            if (err != 0) {
                return err;
            }
        }
    } else {
        if ((p_vol->flags & 2) != 0) {
            if (VFiPFDRV_IsDetected(p_vol) != 0) {
                VFiPFVOL_UnmountVolumeByEject(p_vol);
            }
            p_vol->flags &= ~2;
        }
    }
    return 0;
}

s32 VFiPFVOL_InitModule(u32 config, void* param) {
    s32 vol_idx;
    struct PF_CHARCODE codeset;  // Present in DWARF but unused here.
    u32 i;

    if ((config & 0xFFFCFFFF) != 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }
    if ((config & 0x30000) == 0x30000) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if ((config & 0x10000) != 0) {
        VFipf_vol_set.config |= 0x10000;
    } else {
        VFipf_vol_set.config &= ~0x10000;
    }

    VFipf_vol_set.current_vol[0].p_vol = &VFipf_vol_set.volumes[0];
    VFipf_vol_set.current_vol[0].stat |= 1;
    VFipf_vol_set.num_attached_drives = 0;
    VFipf_vol_set.num_mounted_volumes = 0;

    if ((config & 0x10000) != 0) {
        VFipf_vol_set.config |= 0x10000;
    } else {
        VFipf_vol_set.config &= ~0x10000;
    }

    VFipf_vol_set.param = param;
    VFipf_vol_set.last_error = 0;
    VFipf_vol_set.last_driver_error = 0;
    VFipf_vol_set.setting = 1;

    VFipf_vol_set.codeset.oem2unicode = &VFiPFCODE_CP932_OEM2Unicode;
    VFipf_vol_set.codeset.unicode2oem = &VFiPFCODE_CP932_Unicode2OEM;
    VFipf_vol_set.codeset.oem_char_width = &VFiPFCODE_CP932_OEMCharWidth;
    VFipf_vol_set.codeset.is_oem_mb_char = &VFiPFCODE_CP932_isOEMMBchar;
    VFipf_vol_set.codeset.unicode_char_width = &VFiPFCODE_CP932_UnicodeCharWidth;
    VFipf_vol_set.codeset.is_unicode_mb_char = &VFiPFCODE_CP932_isUnicodeMBchar;

    for (vol_idx = 0; vol_idx < 26; vol_idx++) {
        VFipf_memset(&VFipf_vol_set.volumes[vol_idx], 0, sizeof(struct PF_VOLUME));
    }

    VFiPF_InitLockFile();

    return 0;
}
