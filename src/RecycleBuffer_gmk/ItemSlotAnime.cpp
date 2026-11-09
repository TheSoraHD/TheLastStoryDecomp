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

// inferred: entry of RecycleBuffer (0x84 bytes; empty destructor 0x8044E158, cleared by 0x8044E198); names
// invented
struct RecycleBufferEntry_unk {
    ~RecycleBufferEntry_unk() {}
    void Clear_unk() {
        m00 = 0;
        m04 = 0;
        m14 = 0;
        m24 = 0;
        m30 = 0;
        m3C = 0;
        m40 = 0;
    }
    int m00;
    int m04;
    unsigned char m08[0x14 - 0x08];
    int m14;
    unsigned char m18[0x24 - 0x18];
    int m24;
    unsigned char m28[0x30 - 0x28];
    int m30;
    unsigned char m34[0x3C - 0x34];
    int m3C;
    int m40;
    unsigned char m44[0x84 - 0x44];
};

// inferred: task keeping a pool of entries (RecycleBuffer<gmk::ItemSlotAnime>, vtable 0x8078F3C8); members
// invented
template <class T>
class RecycleBuffer : public atn::Task {
public:
    virtual ~RecycleBuffer() {
        mNum = 0;
        if (m4C != 0) {
            delete[] m4C;
            m4C = 0;
        }
        if (mEntries != 0) {
            delete[] mEntries;
            mEntries = 0;
        }
    }

    // inferred: clears every entry (inlined in the destructor of ItemSlotAnimeBuffer); name invented
    void ClearAll_unk() {
        for (unsigned int i = 0; i < mNum; i++) {
            mEntries[i].Clear_unk();
        }
    }

    unsigned int mNum;              // 0x48
    unsigned char* m4C;
    RecycleBufferEntry_unk* mEntries; // 0x50
};

namespace gmk {

// inferred: the pool of the item animations (vtable 0x8078F388, instance lbl_8087F4F8)
class ItemSlotAnimeBuffer : public RecycleBuffer<ItemSlotAnime> {
public:
    virtual ~ItemSlotAnimeBuffer();
    virtual int vf0C(); // inferred: 0x804523D0
};

// inferred: clears the entries and the instance pointer
ItemSlotAnimeBuffer::~ItemSlotAnimeBuffer() {
    ClearAll_unk();
    lbl_8087F4F8 = 0;
}

} // namespace gmk
