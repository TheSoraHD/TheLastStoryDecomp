#include "atn/LH_MotionCommandData.h"
#include "atn/Array_unk.h"
#include "atn/LoadManager.h"
#include "atn/EfpHandle_unk.h"

// inferred: element types of the motion command data arrays (names unknown). The first one has a
// constructor and no destructor: delete[] removes the array cookie but destroys nothing.
struct MotionCmdElemA_unk {
    MotionCmdElemA_unk();
    int m00;
};
// inferred: {resource, loader at 0x04} (destructor 0x802375C4, also a member of EfpElem_unk in
// LH_Efp.cpp); name invented
struct ResLoader_unk {
    ~ResLoader_unk();
    int m00;
};
struct MotionCmdElemD_unk {
    ~MotionCmdElemD_unk(); // 0x80041C0C
    int m00;
};

// inferred: motion command data built from the loaded file (0x30 bytes)
struct MotionCmdData_unk {
    int fn_800A091C(); // inferred: 0 when the data is ready

    atn::Vector_unk<MotionCmdElemA_unk> m00;
    atn::Vector_unk<ResLoader_unk> m0C;
    atn::Vector_unk<atn::EfpHandle_unk> m18; // destructor 0x800EF73C
    atn::Vector_unk<MotionCmdElemD_unk> m24;
};

typedef void (*MotionCmdParser_unk)(MotionCmdData_unk* data, void* buffer, void* size);

namespace atn {

// inferred from __vt__Q23atn21LoadMotionCommandData (0x8078FE78)
class LoadMotionCommandData : public LoadCore {
public:
    LoadMotionCommandData(const char* path) : LoadCore(path), m5C(1), mParser(0), m64(0) {}
    virtual ~LoadMotionCommandData();
    virtual void vf0C();

    int m5C;                     // inferred: the data still has to be parsed
    MotionCmdParser_unk mParser; // 0x60: set by LH_MotionCommandData::vf0C
    int m64;
    MotionCmdData_unk mData;     // 0x68
};

LoadMotionCommandData::~LoadMotionCommandData() {}

void LoadMotionCommandData::vf0C() {
    if (mState == 2) {
        if (m5C != 0) {
            if (mParser != 0) {
                mParser(&mData, m54, m50);
            }
            if (m64 == 0) {
                fn_80473130();
            }
            m5C = 0;
        } else if (mData.fn_800A091C() == 0) {
            fn_80473104(3);
        }
    }
}

void LH_MotionCommandData::vf0C(const char* path) {
    fn_80473F88();
    LoadMotionCommandData* core = lbl_8087F518->Get_unk<LoadMotionCommandData>(path, 1);
    mCore = core;
    core->mParser = (MotionCmdParser_unk)mCallback;
}

void* LH_MotionCommandData::fn_80476CE4() {
    if (mCore != 0) {
        return &((LoadMotionCommandData*)mCore)->mData;
    }
    return 0;
}

} // namespace atn
