#ifndef GMK_ARENAFLOOR_H
#define GMK_ARENAFLOOR_H

#include "game/GimmickCommon.h"

struct ArenaGroup_unk;

namespace gmk {

// inferred: moves the map parts of a group ("MAP") by an offset ("OFFSET": x, y, z, time) and
// back. The name is in the DOL (__vt__Q23gmk10ArenaFloor).
class ArenaFloor : public GimmickCommon {
public:
    // inferred: moved object (0x14 bytes, built with new[] in vf0C)
    struct Entry_unk {
        Entry_unk() : mType(0), mPos(0.0f, 0.0f, 0.0f) {}
        int mType;
        void* mObject;
        atn::Vector3 mPos;
    };
    // inferred: array of entries with its count (destructor inlined in ~ArenaFloor)
    struct Entries_unk {
        ~Entries_unk() {
            if (mData != 0) {
                delete[] mData;
            }
            mData = 0;
            mCount = 0;
        }
        int mCount;
        Entry_unk* mData;
    };

    ArenaFloor(atn::Task* parent, int id, int arg) : GimmickCommon(parent, id, arg), mGroup(0) {
        mOffset.x = 0.0f;
        mOffset.y = 0.0f;
        mOffset.z = 0.0f;
        mTime = 0.0f;
        mValue = 0.0f;
        mEntries.mCount = 0;
        mEntries.mData = 0;
        m54 = 0;
    }
    static GimmickCommon* fn_8041DDE4(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~ArenaFloor() {}
    virtual int vf0C(); // 0x8041DF38: not decompiled yet
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);

    void fn_8041E480(float t); // inferred: places the entries; not decompiled yet
    void fn_8041E7E4(int on);  // inferred: switches the parts of type 5

    ArenaGroup_unk* mGroup; // 0xF4
    atn::Vector3 mOffset;   // 0xF8
    float mTime;            // 0x104
    float mValue;           // 0x108
    Entries_unk mEntries;   // 0x10C
    int m114;               // inferred: unused (the object is 0x118 bytes)
};

} // namespace gmk

#endif
