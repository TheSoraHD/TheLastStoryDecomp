#include "math/Vec.h"
#include "atn/Memory.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

struct Camera_unk;
// inferred: frustum of the camera (camera + 0x204)
struct AABBFrustum_unk;
// inferred: camera of the scene (fields named by offset)
struct AABBCamera_unk {
    unsigned char m000[0x204];
    unsigned char mFrustum[1]; // 0x204
};
// inferred: render parameters (fields named by offset)
struct AABBParams_unk {
    unsigned char m000[0x1D4];
    int mCulling; // 0x1D4
};
extern "C" AABBParams_unk* lbl_8087EFA8;
extern "C" AABBCamera_unk* lbl_8087EFB4;

namespace atn {

// inferred: axis aligned box (min, max)
struct AABB_unk {
    AABB_unk Merge_unk(const AABB_unk& o) const; // 0x80070C98: union of both boxes
    // inferred: name invented
    Vector3 GetCenter_unk() const { return mMin + (mMax - mMin) * 0.5f; }
    Vector3 mMin;
    Vector3 mMax;
};

} // namespace atn

extern "C" int fn_800502A8(void* frustum, const atn::AABB_unk* box);             // inferred: frustum test
extern "C" int fn_800C0A50(AABBCamera_unk* camera, const atn::AABB_unk* box);   // inferred: occlusion test
extern "C" int fn_80070A70(const atn::AABB_unk* box, const atn::AABB_unk* other); // inferred: a corner of other is in box

namespace atn {

// inferred: renderer interface (see atn/VideoTHP.cpp); its inline virtuals are first emitted here
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C() { return 2; }
    virtual void vf10(Camera_unk* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* frustum) { return 1; }
    virtual void vf18() {}
    virtual void vf1C() {}
};

// inferred: node of the quadtree (0x44 bytes; names invented)
class AABBTreeNode_unk {
public:
    AABBTreeNode_unk(const AABB_unk& area) : mIndex(-1), mArea(area) {
        Vector3 center = mArea.GetCenter_unk();
        mBounds.mMin = center;
        mBounds.mMax = center;
        mChildren[0] = 0;
        mChildren[1] = 0;
        mChildren[2] = 0;
        mChildren[3] = 0;
    }
    ~AABBTreeNode_unk();
    void fn_80045C88(void* frustum, int* visible);
    AABBTreeNode_unk* fn_80045D7C(const AABB_unk* box, int depth);

    int mIndex;                       // 0x00
    AABB_unk mArea;                   // 0x04
    AABB_unk mBounds;                 // 0x1C: bounds of the inserted boxes
    AABBTreeNode_unk* mChildren[4];   // 0x34
};

// inferred: owner of the root node
struct AABBTreeRoot_unk {
    AABBTreeRoot_unk() : mNode(0) {}
    // inferred: the extra inline level (like boost::checked_delete) gives the register order of
    // the original in ~AABBTree; a plain `delete mNode` does not
    ~AABBTreeRoot_unk() { Delete_unk(mNode); }
    template <class T>
    static void Delete_unk(T* p) { delete p; }
    AABBTreeNode_unk* mNode;
};

// inferred: array of visibility results (size in bytes)
struct AABBTreeVisible_unk {
    AABBTreeVisible_unk() : mSize(0), mData(0) {}
    ~AABBTreeVisible_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mSize = 0;
    }
    unsigned long mSize;
    int* mData;
};

// inferred: result of an insertion (returned in r3/r4)
struct AABBTreeHandle_unk {
    class AABBTree* mTree;
    AABBTreeNode_unk* mNode;
};

// note: private base (the RTTI of AABBTree has no base list)
class AABBTree : IRender {
public:
    AABBTree();
    virtual ~AABBTree();
    virtual int vf0C() { return 2; }
    virtual void vf10(Camera_unk* camera) {}
    virtual void vf18();
    AABBTreeHandle_unk fn_80045A24(const AABB_unk* box);

    AABBTreeRoot_unk mRoot;       // 0x04
    AABBTreeVisible_unk mVisible; // 0x08
};

AABBTree::AABBTree() {}

AABBTree::~AABBTree() {}

AABBTreeHandle_unk AABBTree::fn_80045A24(const AABB_unk* box) {
    AABBTreeHandle_unk h;
    h.mNode = mRoot.mNode->fn_80045D7C(box, 0);
    h.mTree = this;
    return h;
}

// NONMATCHING: 97.4% - the original sets up the frustum argument (mr r3) after &root->mBounds
void AABBTree::vf18() {
    int* visible = mVisible.mData;
    if (lbl_8087EFA8->mCulling != 0) {
        memset(visible, 0, mVisible.mSize);
        void* frustum = lbl_8087EFB4->mFrustum;
        mRoot.mNode->fn_80045C88(frustum, visible);
    } else {
        memset(visible, 1, mVisible.mSize);
    }
}

AABBTreeNode_unk::~AABBTreeNode_unk() {
    for (int i = 0; i < 4; i++) {
        delete mChildren[i];
    }
}

void AABBTreeNode_unk::fn_80045C88(void* frustum, int* visible) {
    ((void)0); // inferred: keeps the recursive inlining to one level, as in the original
    int vis = fn_800502A8(frustum, &mBounds);
    visible[mIndex] = vis;
    if (vis != 0) {
        vis = fn_800C0A50(lbl_8087EFB4, &mBounds);
    }
    for (int i = 0; i < 4; i++) {
        if (mChildren[i] != 0 && vis != 0) {
            mChildren[i]->fn_80045C88(frustum, visible);
        }
    }
}

// NONMATCHING: 85% - the parameters get r24-r26 instead of r29-r31: the constructor loop of
// `areas` (AABB_unk has Vector3 members with a user constructor) is created first; with a POD
// array the order is the original one. Float registers of the dead center computations also differ.
AABBTreeNode_unk* AABBTreeNode_unk::fn_80045D7C(const AABB_unk* box, int depth) {
    mBounds = mBounds.Merge_unk(*box);
    if (depth == 10) {
        return this;
    }
    Vector3 mid = mArea.GetCenter_unk();
    Vector3 offset = box->mMax - mid;
    AABB_unk areas[4];
    int count = 0;
    int index = -1;
    for (int i = 0; i < 4; i++) {
        AABB_unk& area = areas[i];
        switch (i) {
        case 0:
            area.mMin = mid;
            area.mMin.y = mArea.mMin.y;
            area.mMax = mArea.mMax;
            break;
        case 1:
            area.mMin.x = mArea.mMin.x;
            area.mMin.z = mid.z;
            area.mMin.y = mArea.mMin.y;
            area.mMax.x = mid.x;
            area.mMax.z = mArea.mMax.z;
            area.mMax.y = mArea.mMax.y;
            break;
        case 2:
            area.mMin = mArea.mMin;
            area.mMax = mid;
            area.mMax.y = mArea.mMax.y;
            break;
        case 3:
            area.mMin.x = mid.x;
            area.mMin.z = mArea.mMin.z;
            area.mMin.y = mArea.mMin.y;
            area.mMax.x = mArea.mMax.x;
            area.mMax.z = mid.z;
            area.mMax.y = mArea.mMax.y;
            break;
        }
        if (fn_80070A70(&area, box)) {
            index = i;
            count++;
        }
    }
    if (count > 1) {
        return this;
    }
    AABBTreeNode_unk** child = &mChildren[index];
    if (*child == 0) {
        AABB_unk& area = areas[index];
        Vector3 boxCenter = box->GetCenter_unk();
        Vector3 areaCenter = area.GetCenter_unk();
        *child = new (6, "", "", 0) AABBTreeNode_unk(area);
    }
    return (*child)->fn_80045D7C(box, depth + 1);
}

} // namespace atn
