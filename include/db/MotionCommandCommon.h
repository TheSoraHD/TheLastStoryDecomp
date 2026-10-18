#ifndef DB_MOTIONCOMMANDCOMMON_H
#define DB_MOTIONCOMMANDCOMMON_H

#include "atn/Task.h"
#include "atn/LH_MotionCommandData.h"

namespace db {

class MotionCommandCommon : public atn::Task {
public:
    MotionCommandCommon(atn::Task* parent);
    virtual ~MotionCommandCommon();
    virtual int vf0C();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static MotionCommandCommon* fn_80218720(atn::Task* parent);

    atn::LH_MotionCommandData mLoader; // 0x48
    void* mData;                        // 0x54

    static MotionCommandCommon* sInstance; // inferred name (0x8087F2A8)
};

} // namespace db

#endif
