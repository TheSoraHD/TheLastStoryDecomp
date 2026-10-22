#include "atn/LH_Xsca.h"
#include "atn/LoadManager.h"

// inferred: object built from the loaded data (constructor 0x805BDD1C, destructor 0x805BDD74)
struct Xsca_unk {
    Xsca_unk(void* data);
    ~Xsca_unk();
    unsigned char m00[0x20];
};

namespace atn {

// inferred from __vt__Q23atn8LoadXsca (0x8078FFD8)
class LoadXsca : public LoadCore {
public:
    LoadXsca(const char* path) : LoadCore(path), m5C(0) {}
    virtual ~LoadXsca();
    virtual void vf0C();

    Xsca_unk* m5C;
};

LoadXsca::~LoadXsca() {
    if (m5C != 0) {
        delete m5C;
    }
}

void LoadXsca::vf0C() {
    if (mState == 2) {
        m5C = new (6, "", "", 0) Xsca_unk(m54);
        fn_80473104(3);
    }
}

void LH_Xsca::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadXsca>(path, 1);
}

void* LH_Xsca::fn_804786F8() {
    if (mCore != 0) {
        return ((LoadXsca*)mCore)->m5C;
    }
    return 0;
}

} // namespace atn
