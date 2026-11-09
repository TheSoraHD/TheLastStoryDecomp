#include "gmk/ItemSlotAnime.h"
#include "atn/Task.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "atn/FixedArray.h"
#include "game/NewCampPartsOption.h"

extern "C" bool fn_80213E60(gmk::ItemSlotItem_unk* item, int kind, int sub, int num); // inferred: db/ItemRare_unk.cpp
extern "C" GameTask* lbl_8087F430; // inferred: the field task

// inferred: task of an item animation entry (layout task or effect; bytes at 0x4C/0x4D set by
// ItemSlotAnimeBuffer::vf0C); names invented
class ItemSlotTask_unk : public atn::Task {
public:
    unsigned char m48[0x4C - 0x48];
    unsigned char m4C;
    unsigned char m4D;
};

// inferred: entry of RecycleBuffer (0x84 bytes; empty destructor 0x8044E158, cleared by 0x8044E198); names
// invented
struct RecycleBufferEntry_unk {
    ~RecycleBufferEntry_unk() {}
    void Clear_unk() {
        m00 = 0;
        m04.mSize = 0;
        m14.mSize = 0;
        m24.mSize = 0;
        m30.mSize = 0;
        m3C = 0;
        mTasks.mSize = 0;
    }
    // inferred: every task of the entry finished its vf0C
    bool IsReady_unk() {
        for (ItemSlotTask_unk** it = mTasks.begin(); it != mTasks.end(); it++) {
            if (*it != 0 && !(*it)->IsFlag_unk(2)) {
                return false;
            }
        }
        return true;
    }
    int m00;
    atn::FixedArray_unk<ItemSlotTask_unk*, 3> m04;
    atn::FixedArray_unk<ItemSlotTask_unk*, 3> m14;
    atn::FixedArray_unk<ItemSlotTask_unk*, 2> m24;
    atn::FixedArray_unk<ItemSlotTask_unk*, 2> m30;
    ItemSlotTask_unk* m3C;
    atn::FixedArray_unk<ItemSlotTask_unk*, 16> mTasks; // 0x40
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

    inline void Free_unk(RecycleBufferEntry_unk* entry);

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
    inline bool Start_unk(); // inferred: true while the tasks are loading; then shows them (name invented)
};

} // namespace gmk


extern "C" gmk::ItemSlotAnimeBuffer* lbl_8087F4F8; // inferred: instance of the buffer

// inferred: clears the bit of the entry in the use mask (name invented)
template <class T>
inline void RecycleBuffer<T>::Free_unk(RecycleBufferEntry_unk* entry) {
    for (unsigned int i = 0; i < mNum; i++) {
        if (&mEntries[i] == entry) {
            m4C[i >> 3] &= ~(1 << (i & 7));
            return;
        }
    }
}

namespace gmk {

inline void ItemSlotAnime::Release_unk() {
    if (m00 != 0) {
        if (lbl_8087F4F8 != 0) {
            atn::FixedArray_unk<ItemSlotTask_unk*, 16>& tasks = m00->mTasks;
            for (ItemSlotTask_unk** it = tasks.begin(); it != tasks.end(); it++) {
                if (*it != 0) {
                    (*it)->FreezeTask();
                }
            }
            lbl_8087F4F8->Free_unk(m00);
        }
        m00 = 0;
    }
}

ItemSlotAnime::ItemSlotAnime() : m00(0), mState(0), mPos(0.0f, 0.0f, 0.0f), mScale(1.0f), m18(0), m1C(0), m20(0), m24(0) {
    mFlag31 = 1;
    mFlag30 = 1;
    mBits29 = 3;
    mFlag27 = 1;
    mFlag26 = 0;
    mFlag25 = 0;
    mFlag24 = 0;
    mF0 = 0;
    mState = 0;
    Release_unk();
}

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

extern "C" void* lbl_8087F4FC; // inferred: instance of gmk::ItemSlotTextCache

namespace gmk {

// inferred: cache of the texts of the item animations (vtable 0x8078F348, instance lbl_8087F4FC)
class ItemSlotTextCache : public atn::Task {
public:
    virtual ~ItemSlotTextCache();
};

ItemSlotTextCache::~ItemSlotTextCache() {
    lbl_8087F4FC = 0;
}

} // namespace gmk

namespace gmk {

// inferred: once the tasks of every entry are ready they are shown (and frozen until used)
inline bool ItemSlotAnimeBuffer::Start_unk() {
    for (unsigned int i = 0; i < mNum; i++) {
        if (!mEntries[i].IsReady_unk()) {
            return true;
        }
    }
    for (unsigned int i = 0; i < mNum; i++) {
        RecycleBufferEntry_unk* e = &mEntries[i];
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(0);
            }
        }
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->FreezeTask();
            }
        }
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->m4D = 0;
            }
        }
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->m4C = 1;
            }
        }
        for (ItemSlotTask_unk** it = e->m04.begin(); it != e->m04.end(); it++) {
            if (*it != 0) {
                (*it)->m4D = 1;
            }
        }
        for (ItemSlotTask_unk** it = e->m14.begin(); it != e->m14.end(); it++) {
            if (*it != 0) {
                (*it)->m4D = 1;
            }
        }
        e->m3C->m4D = 1;
    }
    return false;
}

int ItemSlotAnimeBuffer::vf0C() {
    return !Start_unk();
}

// inferred: clears the entries and the instance pointer
ItemSlotAnimeBuffer::~ItemSlotAnimeBuffer() {
    ClearAll_unk();
    lbl_8087F4F8 = 0;
}


} // namespace gmk
