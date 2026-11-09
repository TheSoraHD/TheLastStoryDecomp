#include "gmk/ItemSlotAnime.h"
#include "atn/Task.h"
#include "game/GameTask.h"
#include "game/MapTask.h"

extern "C" bool fn_80213E60(gmk::ItemSlotItem_unk* item, int kind, int sub, int num); // inferred: db/ItemRare_unk.cpp
extern "C" GameTask* lbl_8087F430; // inferred: the field task

namespace gmk {

int ItemSlotAnime::fn_8044E610() {
    return mState == 2 && m20 != 0 && m24 > 0;
}

int ItemSlotAnime::fn_80450A5C() {
    return m20 != 0 ? m20->mId : 0;
}

// inferred: the item may be taken here (301 and flagged items always; the others by the item table of the
// current map)
int ItemSlotAnime::fn_80450A78() const {
    if (m20 == 0) {
        return 0;
    }
    if (m20->mId == 301) {
        return 1;
    }
    if (m20 == 0) {
        return 0;
    }
    if (m20->mC2 != 0) {
        return 1;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        MapTask* map = lbl_8087F430->GetCurrentMap();
        return fn_80213E60(m20, map->mType, map->mId, map->mSubId);
    }
    return 0;
}

} // namespace gmk

extern "C" atn::Task* lbl_8087F4F8;

extern "C" void fn_80450B28() {
    if (lbl_8087F4F8 != 0) {
        lbl_8087F4F8->mFlags |= 4;
    }
}

extern "C" void fn_80450B44() {
    if (lbl_8087F4F8 != 0) {
        lbl_8087F4F8->mFlags &= ~4;
    }
}

extern "C" int fn_80450B60(int value) {
    unsigned int index = value - 0x2711;
    if (index > 7) {
        if (value != 0x271A) {
            goto rejected;
        }
    }
    return 1;
rejected:
    return 0;
}
