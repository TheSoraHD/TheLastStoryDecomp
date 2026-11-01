#include "revolution/nand.h"
#include "revolution/fs.h"

static const char *USER_DIR_LIST[] =
{
    "/meta",
    "/ticket",
    "/title/00010000",
    "/title/00010001",
    "/title/00010003",
    "/title/00010004",
    "/title/00010005",
    "/title/00010006",
    "/title/00010007",
    "/shared2/title",
    NULL,
};

s32 nandCalcUsage(u32* fsBlock, u32* inode, const char* dir_list[]) {
    ISFSError ret = ISFS_ERROR_UNKNOWN;
    *fsBlock = 0;
    *inode = 0;

    while (*dir_list) {
        u32 blk = 0;
        u32 node = 0;
        ret = ISFS_GetUsage((const u8*)*dir_list, &blk, &node);

        if (ret == ISFS_ERROR_OK) {
            *fsBlock += blk;
            *inode += node;
        }
        else if (ret == ISFS_ERROR_NOEXISTS) {
            ret = ISFS_ERROR_OK;
        }
        else {
            break;
        }

        ++dir_list;
    }

    return ret;
}

ISFSError nandCalcUserUsage(u32* fsBlock, u32* inode) {
    return nandCalcUsage(fsBlock, inode, USER_DIR_LIST);
}

static u32 nandCheck(const u32 reqBlock, const u32 reqInode, const u32 homeBlock, const u32 homeInode, const u32 userBlock, const u32 userInode) {
    u32 answer = 0;

    if (homeBlock + reqBlock > 0x400) {
        answer |= 1;
    }
    
    if (homeInode + reqInode > 0x21) {
        answer |= 2;
    }

    if (userBlock + reqBlock > 0x4400) {
        answer |= 4;
    }

    if (userInode + reqInode > 0xFA0) {
        answer |= 8;
    }

    return answer;
}

s32 NANDCheck(const u32 fsBlock, const u32 inode, u32* answer) {
    ISFSError ret = ISFS_ERROR_UNKNOWN;
    u32 homeBlocks = 0xFFFFFFFF;
    u32 homeInodes = 0xFFFFFFFF;
    u32 userBlocks = 0xFFFFFFFF;
    u32 userInodes = 0xFFFFFFFF;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    ret = ISFS_GetUsage((const u8*)nandGetHomeDir(), &homeBlocks, &homeInodes);

    if (ret != ISFS_ERROR_OK) {
        return nandConvertErrorCode(ret);
    }

    ret = nandCalcUserUsage(&userBlocks, &userInodes);

    if (ret != ISFS_ERROR_OK) {
        return nandConvertErrorCode(ret);
    }

    *answer = nandCheck(fsBlock, inode, homeBlocks, homeInodes, userBlocks, userInodes);
    return NAND_RESULT_OK;
}
// inferred: the async check (0x80622260) and its callback are in the 2010 SDK, not in the 2007 source;
// the callback also serves a second request (state 0x15, free area) that the game does not use
static void nandUserAreaCallback(ISFSError result, void* ctxt);

s32 NANDCheckAsync(u32 fsBlock, u32 inode, u32* answer, NANDCallback cb, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = cb;
    block->reqBlocks = fsBlock;
    block->reqInodes = inode;
    block->answer = answer;
    block->userBlocks = 0;
    block->userInodes = 0;
    block->workBlocks = 0;
    block->workInodes = 0;
    block->dir = USER_DIR_LIST;
    block->state = 0x14;
    return nandConvertErrorCode(ISFS_GetUsageAsync((const u8*)nandGetHomeDir(), &block->homeBlocks, &block->homeInodes, nandUserAreaCallback, block));
}

static void nandUserAreaCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if (result == ISFS_ERROR_OK || result == ISFS_ERROR_NOEXISTS) {
        const char* dir = *(b->dir);

        if (result == ISFS_ERROR_OK) {
            b->userBlocks += b->workBlocks;
            b->userInodes += b->workInodes;
        }

        if (dir) {
            ISFSError ret;
            ++b->dir;
            ret = ISFS_GetUsageAsync((const u8*)dir, &(b->workBlocks), &(b->workInodes), nandUserAreaCallback, ctxt);

            if (ret != ISFS_ERROR_OK) {
                ((NANDCallback)(b->callback))(nandConvertErrorCode(ret), b);
            }
        }
        else if (b->state == 0x14) {
            *(b->answer) = nandCheck(b->reqBlocks, b->reqInodes, b->homeBlocks, b->homeInodes, b->userBlocks, b->userInodes);
            ((NANDCallback)(b->callback))(nandConvertErrorCode(ISFS_ERROR_OK), b);
        }
        else if (b->state == 0x15) {
            u32 userBlocks = (b->userBlocks > 0x4400) ? 0 : 0x4400 - b->userBlocks;
            u32 userInodes = (b->userInodes > 0xFA0) ? 0 : 0xFA0 - b->userInodes;
            u32 homeBlocks = (b->homeBlocks > 0x400) ? 0 : 0x400 - b->homeBlocks;
            u32 homeInodes = (b->homeInodes > 0x21) ? 0 : 0x21 - b->homeInodes;

            *(u32*)(b->bytes) = (userBlocks < homeBlocks) ? userBlocks : homeBlocks;
            *(u32*)(b->inodes) = (userInodes < homeInodes) ? userInodes : homeInodes;
            ((NANDCallback)(b->callback))(nandConvertErrorCode(ISFS_ERROR_OK), b);
        }
        else {
            OSReport("Illegal status is detected at %s()", __FUNCTION__);
            ((NANDCallback)(b->callback))(nandConvertErrorCode(ISFS_ERROR_UNKNOWN), b);
        }
    }
    else {
        ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
    }
}
