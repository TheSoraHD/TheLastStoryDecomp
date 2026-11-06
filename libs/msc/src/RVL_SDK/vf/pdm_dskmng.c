#include <revolution/vf.h>

PDM_DISK_SET VFipdm_disk_set;

s32 VFipdm_init_diskmanager(u32 config, void* param) {
    u32 i;

    VFipf_memset(&VFipdm_disk_set, 0, sizeof(VFipdm_disk_set));
    for (i = 0; i < 0x1A; i++) {
        VFipdm_disk_set.disk[i].disk_lock_handle = 0;
    }

    for (i = 0; i < 0x1A; i++) {
        VFipdm_disk_set.partition[i].part_lock_handle = 0;
    }

    return 0;
}
