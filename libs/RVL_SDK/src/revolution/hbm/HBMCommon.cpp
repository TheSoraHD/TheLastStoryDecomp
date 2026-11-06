#include <homebuttonLib.h>
#include <revolution/hbm/HBMApi.h>
#include <HBMRes.h>

#include <nw4hbm/lyt.h>
#include <nw4hbm/ut.h>

#include <revolution/MEM.h>
#include <revolution/OS.h>
#include <revolution/version.h>

// inferred: the C API of the Home Button menu is its own translation unit in the version of 2010
// (0x805C3C50-0x805C3FE0): HBMBase.cpp does not inline HBMAllocMem and the API does not inline
// HomeButton::getInstance

namespace homebutton {
void InitAxSound(const void* soundData, void* mem, u32 memSize); // inferred: HBMAxSound (asm)
void ShutdownAxSound();                                           // inferred: HBMAxSound (asm)
} // namespace homebutton

/******************************************************************************
 *
 * API
 *
 ******************************************************************************/

RVL_LIB_VERSION(HBM, "Jul 30 2010", "15:28:44", "0x4302_145");

enum HBMAllocatorType {
    HBM_ALLOCATOR_APPLI,
    HBM_ALLOCATOR_LOCAL,
    HBM_ALLOCATOR_NW4R,
};

static MEMAllocator sAllocator;
MEMAllocator* spAllocator = &sAllocator; // inferred: global in the version of 2010 (HBMBase.cpp uses it)

void* HBMAllocMem(u32 size) {
    return MEMAllocFromAllocator(spAllocator, size);
}

void HBMFreeMem(void* pBlock) {
    MEMFreeToAllocator(spAllocator, pBlock);
}

static HBMAllocatorType getAllocatorType(const HBMDataInfo* pDataInfo) {
    if (pDataInfo->pAllocator != NULL) {
        return HBM_ALLOCATOR_APPLI;
    }

    if (pDataInfo->mem != NULL) {
        return HBM_ALLOCATOR_LOCAL;
    }

    return HBM_ALLOCATOR_NW4R;
}

void HBMCreate(const HBMDataInfo* pDataInfo) {
    MEMiHeapHead* hExpHeap;

    if (getAllocatorType(pDataInfo) == HBM_ALLOCATOR_LOCAL) {
        hExpHeap = MEMCreateExpHeap(pDataInfo->mem, pDataInfo->memSize);
        MEMInitAllocatorForExpHeap(&sAllocator, hExpHeap, 32);
        spAllocator = &sAllocator;
    }

    switch (getAllocatorType(pDataInfo)) {
    case HBM_ALLOCATOR_APPLI: {
        nw4hbm::lyt::Layout::SetAllocator(pDataInfo->pAllocator);
        spAllocator = pDataInfo->pAllocator;
        break;
    }

    case HBM_ALLOCATOR_LOCAL: {
        nw4hbm::lyt::Layout::SetAllocator(spAllocator);
        break;
    }

    case HBM_ALLOCATOR_NW4R: {
        spAllocator = nw4hbm::lyt::Layout::GetAllocator();
        break;
    }
    }

    homebutton::HomeButton::createInstance(pDataInfo);
    homebutton::HomeButton::getInstance()->create();
}

void HBMDelete() {
    const HBMDataInfo* pDataInfo = homebutton::HomeButton::getInstance()->getHBMDataInfo();

    homebutton::HomeButton::deleteInstance();

    if (getAllocatorType(pDataInfo) == HBM_ALLOCATOR_LOCAL) {
        MEMDestroyExpHeap(spAllocator->heap);
    }
}

void HBMInit() {
    OSRegisterVersion(__HBMVersion);
    homebutton::HomeButton::getInstance()->init();
}

HBMSelectBtnNum HBMCalc(const HBMControllerData* pController) {
    homebutton::HomeButton::getInstance()->calc(pController);
    return homebutton::HomeButton::getInstance()->getSelectBtnNum();
}

void HBMDraw() {
    homebutton::HomeButton::getInstance()->draw();
}

HBMSelectBtnNum HBMGetSelectBtnNum() {
    return homebutton::HomeButton::getInstance()->getSelectBtnNum();
}

void HBMSetAdjustFlag(BOOL flag) {
    homebutton::HomeButton::getInstance()->setAdjustFlag(flag);
}

void HBMStartBlackOut() {
    homebutton::HomeButton::getInstance()->startBlackOut();
}

void HBMSetBlackOutColor(u8 r, u8 g, u8 b) {
    homebutton::HomeButton::getInstance()->setBlackOutColor(r, g, b);
}

BOOL HBMIsReassignedControllers() {
    return homebutton::HomeButton::getInstance()->getReassignedFlag();
}

void HBMCreateSound(void* soundData, void* memBuf, u32 memSize) {
    homebutton::HomeButton::getInstance();
    homebutton::InitAxSound(soundData, memBuf, memSize);
}

void HBMDeleteSound() {
    homebutton::ShutdownAxSound();
}

void HBMUpdateSound() {
    homebutton::HomeButton::getInstance()->update_sound();
}
