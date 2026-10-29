#include "atn/LH_Efp.h"
#include "atn/Array_unk.h"
#include "atn/LoadManager.h"

// inferred: element of the effect data array (destructor 0x80243758, used by delete[])
struct EfpElem_unk {
    ~EfpElem_unk();
    unsigned char m00[0x48];
    unsigned char m48[4]; // inferred: member destroyed by fn_802375C4
};

// inferred: effect data built from the loaded file (0x14 bytes, functions at 0x8024373C-0x80243814)
struct EfpData_unk {
    EfpData_unk();                          // 0x8024373C
    ~EfpData_unk() { fn_802437C0(); }
    void fn_802437B4(const char* path);     // inferred: marks the data as pending
    void fn_802437C0();                     // inferred: releases the elements
    int fn_80243814(void* data, void* size); // inferred: 0 when the data is ready

    atn::SimpleArray<EfpElem_unk> mElems; // 0x00
    int m08;                             // inferred: state
    int m0C;
    int m10;
};

namespace atn {

// inferred from __vt__Q23atn7LoadEfp (0x8078FD20)
class LoadEfp : public LoadCore {
public:
    LoadEfp(const char* path) : LoadCore(path) { mData.fn_802437B4(mPath); }
    virtual ~LoadEfp();
    virtual void vf0C();

    EfpData_unk mData; // 0x5C
};

LoadEfp::~LoadEfp() {}

void LoadEfp::vf0C() {
    if (mState == 2) {
        if (mData.fn_80243814(m54, m50) == 0) {
            fn_80473104(3);
            fn_80473130();
        }
    }
}

void* LH_Efp::fn_80473C20() {
    if (mCore != 0) {
        return &((LoadEfp*)mCore)->mData;
    }
    return 0;
}

void LH_Efp::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadEfp>(path, 1);
}

} // namespace atn
