#ifndef GMK_MAPLIFT_H
#define GMK_MAPLIFT_H

#include "atn/Array_unk.h"
#include "atn/LH_Motion.h"
#include "game/GimmickCommon.h"
#include "std/string.h"
#include "std/vector.h"

// inferred: node pose filled by fn_800A04A0 from a motion (0x2C bytes); constructor 0x800A08E0
struct MotionPose_unk {
    MotionPose_unk(); // 0x800A08E0
    int m00;
    atn::Vector3 mPos; // 0x04
    atn::Quaternion_unk m10; // inferred: rotation
    atn::Vector3 mScale; // 0x20
};

namespace gmk {

// inferred: moves the map parts whose names contain one of the "LOCATOR" strings with the node
// "MOTION_NODE" of a motion ("MOTION": path, id, start, speed, loop, flag; one per state). The name
// is in the DOL (__vt__Q23gmk7MapLift).
class MapLift : public GimmickCommon {
public:
    // inferred: moved object (0x14 bytes, built with new[] in vf0C; same as gmk::ArenaFloor)
    struct Entry_unk {
        Entry_unk() : mType(0), mPos(0.0f, 0.0f, 0.0f) {}
        int mType;     // inferred: 0 model, 1 object of a part, 2 gimmick, 3 part
        void* mObject;
        atn::Vector3 mPos;
    };
    MapLift(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_80429668(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~MapLift();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);

    inline bool HasLocator_unk(const char* name); // inferred
    void fn_8042AB58(const atn::Vector3* pos, const atn::Vector3* prev); // inferred: moves the entries
    void fn_8042AE60(int state); // inferred: applies the first or last frame of a state

    GimmickDesc_unk* mDesc;   // 0xF4
    int mF8;                  // inferred: previous state
    int mFC;                  // inferred: "INIT_STATE"
    float mFrame;             // 0x100
    atn::LH_Motion mMotion[7]; // 0x104
    int m158[7];
    int mMotionId[7];         // 0x174
    float m190[7];
    float mSpeed[7];          // 0x1AC
    int mLoop[7];             // 0x1C8
    int m1E4[7];
    char mNode[0x40];         // 0x200: "MOTION_NODE"
    MotionPose_unk mPose;     // 0x240
    char m26C[0x12];
    atn::Buffer_unk<Entry_unk> mEntries; // 0x280
    std::vector<std::string> mLocators;  // 0x288
    atn::Vector3 mPos;        // 0x294
    atn::Vector3 mPrevPos;    // 0x2A0
    int m2AC;                 // inferred: unused (the object is 0x2B0 bytes)
};

} // namespace gmk

#endif
