#ifndef ATN_AABBTREE_H
#define ATN_AABBTREE_H

#include "math/Vec.h"
#include "atn/AABB_unk.h"
#include "atn/IRender.h"
#include <boost/scoped_ptr.hpp>

namespace atn {

// Quadtree of boxes (map parts and models are registered in it; MapTask + 0xD0). Class and
// nested names from the RSO DWARF (atn::AABBTree, AABBTree::Node, AABBTree::Handle,
// boost::scoped_ptr<AABBTree::Node>). Private base: the RTTI of AABBTree has no base list.
class AABBTree : IRender {
public:
    // inferred: node of the quadtree (0x44 bytes; member names invented)
    class Node {
    public:
        Node(const AABB_unk& area) : mIndex(-1), mArea(area) {
            Vector3 center = mArea.GetCenter_unk();
            mBounds.mMin = center;
            mBounds.mMax = center;
            mChildren[0] = 0;
            mChildren[1] = 0;
            mChildren[2] = 0;
            mChildren[3] = 0;
        }
        ~Node();
        void fn_80045C88(void* frustum, int* visible);
        Node* fn_80045D7C(const AABB_unk* box, int depth);

        int mIndex;            // 0x00
        AABB_unk mArea;        // 0x04
        AABB_unk mBounds;      // 0x1C: bounds of the inserted boxes
        Node* mChildren[4];    // 0x34
    };

    // inferred: result of an insertion (returned in r3/r4; names invented)
    struct Handle {
        Handle() : mTree(0), mNode(0) {}
        Handle(AABBTree* tree, Node* node) : mTree(tree), mNode(node) {}
        AABBTree* mTree;
        Node* mNode;
    };

    // inferred: array of visibility results (size in bytes; name invented)
    struct Visible_unk {
        Visible_unk() : mSize(0), mData(0) {}
        ~Visible_unk() {
            if (mData != 0) {
                delete[] mData;
            }
            mData = 0;
            mSize = 0;
        }
        unsigned long mSize;
        int* mData;
    };

    AABBTree();
    virtual ~AABBTree();
    virtual int GetRenderType_unk() { return 2; }
    virtual void vf10(RenderManager* view) {}
    virtual void vf18(RenderManager* view);
    Handle fn_80045A24(const AABB_unk* box);

    boost::scoped_ptr<Node> mRoot; // 0x04
    Visible_unk mVisible;          // 0x08
};

} // namespace atn

#endif
