#include <revolution/vf.h>

struct VF_DRIVE_INFO l_driveInfo[26];

static struct VF_DRIVE_INFO* _getDriveInfoP(struct PDM_DISK* p_disk);
static u32 dCommon_getFatTypeFromDisk(struct PDM_DISK* p_disk);

static struct VF_DRIVE_INFO* _getDriveInfoP(struct PDM_DISK* p_disk) {
    u32 handleIdx;
    handleIdx = (u32)VFSysPDMDisk2HandleIdx(p_disk);
    if (handleIdx < 0x1A) {
        return &l_driveInfo[handleIdx];
    }
    return NULL;
}

void dCommon_initDriveInfo() {
    struct VF_DRIVE_INFO* driveInfo_p;
    int i;

    for (i = 0, driveInfo_p = l_driveInfo; i < 26; i++, driveInfo_p++) {
        driveInfo_p->prf_file_size = 0;
        driveInfo_p->last_device_error = 0;
        driveInfo_p->fat_type = 0;
        driveInfo_p->reserved_sec_num = 0;
        driveInfo_p->root_ent_num = 0;
    }
}

static u32 dCommon_getFatTypeFromDisk(struct PDM_DISK* p_disk) {
    struct VF_DRIVE_INFO* driveInfo_p = _getDriveInfoP(p_disk);
    return driveInfo_p ? driveInfo_p->fat_type : 1;
}

static s32 dCommon_flush_from_handle_p(struct VF_HANDLE_TYPE* i_handle_p, int i_setLastDeviceError) {
    s32 err = -1;

    if (i_handle_p != NULL && i_handle_p->device_p != NULL) {
        if (i_handle_p->device_p->type == 0) {
            int handle_idx = VFSysHandleP2Idx(i_handle_p);
            if (handle_idx != -1) {
                err = VFi_NandFlushNANDFromHandleIdx(handle_idx, i_setLastDeviceError);
            }
        } else {
            err = 0;
        }
    }
    return err;
}
