#include "atn/LH_ColliTree.h"
#include "atn/LoadManager.h"

// inferred: object built from the loaded data (constructor 0x8043FA08, destructor 0x8043FE90)
struct ColliTreeData_unk {
    ColliTreeData_unk(void* data);
    ~ColliTreeData_unk();
    unsigned char m00[0x20];
    void* m20; // the collision tree
    unsigned char m24[0x40 - 0x24];
};

namespace atn {

// inferred from __vt__Q23atn13LoadColliTree (0x8078FC68)
class LoadColliTree : public LoadCore {
public:
    LoadColliTree(const char* path) : LoadCore(path), m5C(0) {}
    virtual ~LoadColliTree();
    virtual void vf0C();

    void* m5C;
};

LoadColliTree::~LoadColliTree() {}

void LoadColliTree::vf0C() {
    if (mState == 2) {
        ColliTreeData_unk data(m54);
        m5C = data.m20;
        fn_80473104(3);
    }
}

void* LH_ColliTree::fn_80472D64() {
    if (mCore != 0) {
        return ((LoadColliTree*)mCore)->m5C;
    }
    return 0;
}

void LH_ColliTree::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadColliTree>(path, 1);
}

} // namespace atn
