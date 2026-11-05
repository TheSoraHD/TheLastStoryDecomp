#include "atn/AABBTree.h"
#include "atn/Memory.h"
#include "atn/RenderManager.h"
#include "atn/RenderSetting_unk.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int fn_80070A70(const atn::AABB_unk* box, const atn::AABB_unk* other); // inferred: a corner of other is in box
extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings

namespace atn {

AABBTree::AABBTree() {}

AABBTree::~AABBTree() {}

AABBTree::Handle AABBTree::fn_80045A24(const AABB_unk* box) {
    return Handle(this, mRoot->fn_80045D7C(box, 0));
}

// NONMATCHING: 97.4% - the original sets up the frustum argument (mr r3) after &root->mBounds
void AABBTree::vf18(RenderManager* view) {
    int* visible = mVisible.mData;
    if (lbl_8087EFA8->m1D4 != 0) {
        memset(visible, 0, mVisible.mSize);
        void* frustum = &atn::RenderManager::instance_->mCamera.mFrustum;
        mRoot->fn_80045C88(frustum, visible);
    } else {
        memset(visible, 1, mVisible.mSize);
    }
}

AABBTree::Node::~Node() {
    for (int i = 0; i < 4; i++) {
        delete mChildren[i];
    }
}

void AABBTree::Node::fn_80045C88(void* frustum, int* visible) {
    ((void)0); // inferred: keeps the recursive inlining to one level, as in the original
    int vis = fn_800502A8((const Frustum_unk*)frustum, &mBounds.mMin);
    visible[mIndex] = vis;
    if (vis != 0) {
        vis = atn::RenderManager::instance_->fn_800C0A50(&mBounds);
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
AABBTree::Node* AABBTree::Node::fn_80045D7C(const AABB_unk* box, int depth) {
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
    Node** child = &mChildren[index];
    if (*child == 0) {
        AABB_unk& area = areas[index];
        Vector3 boxCenter = box->GetCenter_unk();
        Vector3 areaCenter = area.GetCenter_unk();
        *child = new ((MemoryTag)6, "", "", 0) Node(area);
    }
    return (*child)->fn_80045D7C(box, depth + 1);
}

} // namespace atn
