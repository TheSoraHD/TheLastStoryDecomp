// atn/MotionBank_unk.cpp (file name invented): the motion banks shared by the models, kept in two static
// maps (0x800A0A68-0x800A2664), and the random generator of MP::CryptMP (0x800A2664).
// Split out of atn/Motion_unk at 0x800A0A68: own string pool at 0x80732768, .data, the second half of
// .bss/.sbss and the __sinit (the one of <iostream>).
#include "atn/Model_unk.h"
#include "atn/MotionCmd_unk.h"
#include "atn/MotionModelRes_unk.h"
#include "atn/LH_Motion.h"
#include "atn/Memory.h"
#include "std/ios_init.h"
#include "std/tree.h"
#include "std/tr1_random.h"

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void fn_800A09D0(short* out, atn::Model* model, atn::LH_Motion* motion); // atn/Motion_unk

// inferred: the motion banks shared by the models: the node indices of a (model, motion) pair (map A, by
// the hash of the indices) and the uses of each pair (map B, by model hash and motion id). MSL
// std::map<unsigned long long, ...> laid out by hand as in game/GameEventScript.h; names invented.
typedef unsigned long long MotionKey_unk;

// inferred: node indices of a bank and their reference count
struct MotionBank_unk {
    MotionBank_unk() : mData(0), mRef(0) {}
    MotionBank_unk(short* const& data, const int& ref) : mData(data), mRef(ref) {}
    short* mData;
    int mRef;
};
struct MotionBankPair_unk {
    MotionBankPair_unk(const MotionKey_unk& key, const MotionBank_unk& bank) : first(key), second(bank) {}
    const MotionKey_unk first;
    MotionBank_unk second;
};
struct MotionBankNode_unk {
    typedef MotionBankPair_unk Value;
    typedef MotionBank_unk Mapped;
    MotionBankNode_unk* mLeft;
    MotionBankNode_unk* mRight;
    unsigned int mParent;
    Value mValue; // 0x10
};

// inferred: std::make_pair
static inline MotionBank_unk MakeBank_unk(short* data, int ref) {
    return MotionBank_unk(data, ref);
}

// inferred: use of a bank by a (model, motion) pair and its reference count
struct MotionUse_unk {
    MotionUse_unk() : mBank(0), mRef(0) {}
    MotionUse_unk(const MotionKey_unk& bank, int ref) : mBank(bank), mRef(ref) {}
    MotionKey_unk mBank;
    int mRef;
};
struct MotionUsePair_unk {
    MotionUsePair_unk(const MotionKey_unk& key, const MotionUse_unk& use) : first(key), second(use) {}
    const MotionKey_unk first;
    MotionUse_unk second;
};
struct MotionUseNode_unk {
    typedef MotionUsePair_unk Value;
    typedef MotionUse_unk Mapped;
    MotionUseNode_unk* mLeft;
    MotionUseNode_unk* mRight;
    unsigned int mParent;
    Value mValue; // 0x10
};

// inferred: iterator of the trees (in place increment)
template <class Node>
struct MotionTreeIter_unk {
    MotionTreeIter_unk(Node* node) : mNode(node) {}
    static Node* Parent_unk(Node* node) { return (Node*)(node->mParent & ~1); }
    MotionTreeIter_unk& operator++() {
        if (mNode->mRight != 0) {
            Node* n = mNode->mRight;
            while (n->mLeft != 0) {
                n = n->mLeft;
            }
            mNode = n;
        } else {
            while (mNode != Parent_unk(mNode)->mLeft) {
                mNode = Parent_unk(mNode);
            }
            mNode = Parent_unk(mNode);
        }
        return *this;
    }
    Node* mNode;
};

// inferred: MSL __red_black_tree::balance_erase (unlinks z and rebalances, returns the node after z);
// same algorithm as libc++ __tree_remove (both by H. Hinnant), with the parent and the side of x kept
// in locals. Not declared in std/tree.h: a new member there renumbers the labels of the units that
// include it. Name invented.
template <class NodePtr>
NodePtr RbBalanceErase_unk(NodePtr z, NodePtr root) {
    typedef std::__red_black_tree<NodePtr> Tree;
    NodePtr next = z;
    if (next->right_ != 0) {
        NodePtr n = next->right_;
        while (n->left_ != 0) {
            n = n->left_;
        }
        next = n;
    } else {
        while (next != next->parent()->left_) {
            next = next->parent();
        }
        next = next->parent();
    }
    NodePtr y = z;
    if (z->left_ != 0 && z->right_ != 0) {
        y = next;
    }
    NodePtr x = y->left_ != 0 ? y->left_ : y->right_;
    NodePtr xp = y->parent();
    if (x != 0) {
        x->set_parent(xp);
    }
    bool left;
    if (y == y->parent()->left_) {
        y->parent()->left_ = x;
        left = true;
    } else {
        y->parent()->right_ = x;
        left = false;
    }
    bool removedBlack = !y->is_red();
    if (y != z) {
        y->set_parent(z->parent());
        if (z == y->parent()->left_) {
            y->parent()->left_ = y;
        } else {
            y->parent()->right_ = y;
        }
        y->left_ = z->left_;
        if (y->left_ != 0) {
            y->left_->set_parent(y);
        } else {
            xp = y;
        }
        y->right_ = z->right_;
        if (y->right_ != 0) {
            y->right_->set_parent(y);
        } else {
            xp = y;
        }
        if (z->parent_ & 1) {
            y->set_red();
        } else {
            y->set_black();
        }
        if (root == z) {
            root = y;
        }
    } else if (x == 0 && z == root) {
        removedBlack = false;
    }
    if (removedBlack) {
        while (x != root && (x == 0 || (x->parent_ & 1) == 0)) {
            if (left) {
                NodePtr w = xp->right_;
                if ((w->parent_ & 1) == 1) {
                    w->parent_ &= ~1;
                    xp->set_red();
                    Tree::rotate_left(xp, root);
                    w = xp->right_;
                }
                if ((w->left_ == 0 || (w->left_->parent_ & 1) == 0) && (w->right_ == 0 || (w->right_->parent_ & 1) == 0)) {
                    w->set_red();
                    x = xp;
                    xp = x->parent();
                    left = x == xp->left_;
                } else {
                    if (w->right_ == 0 || (w->right_->parent_ & 1) == 0) {
                        w->left_->set_black();
                        w->set_red();
                        Tree::rotate_right(w, root);
                        w = xp->right_;
                    }
                    if (xp->parent_ & 1) {
                        w->set_red();
                    } else {
                        w->set_black();
                    }
                    xp->set_black();
                    w->right_->set_black();
                    Tree::rotate_left(xp, root);
                    x = root;
                }
            } else {
                NodePtr w = xp->left_;
                if ((w->parent_ & 1) == 1) {
                    w->parent_ &= ~1;
                    xp->set_red();
                    Tree::rotate_right(xp, root);
                    w = xp->left_;
                }
                if ((w->left_ == 0 || (w->left_->parent_ & 1) == 0) && (w->right_ == 0 || (w->right_->parent_ & 1) == 0)) {
                    w->set_red();
                    x = xp;
                    xp = x->parent();
                    left = x == xp->left_;
                } else {
                    if (w->left_ == 0 || (w->left_->parent_ & 1) == 0) {
                        w->right_->set_black();
                        w->set_red();
                        Tree::rotate_left(w, root);
                        w = xp->left_;
                    }
                    if (xp->parent_ & 1) {
                        w->set_red();
                    } else {
                        w->set_black();
                    }
                    xp->set_black();
                    w->left_->set_black();
                    Tree::rotate_right(xp, root);
                    x = root;
                }
            }
        }
        if (x != 0) {
            x->set_black();
        }
    }
    return next;
}


// inferred: the MSL red-black tree: size, root (the end node is at &mRoot), the compare object, first node
template <class Node>
struct MotionTree_unk {
    typedef MotionTreeIter_unk<Node> iterator;
    // inferred: std::less<unsigned long long>
    struct Less_unk {
        bool operator()(const MotionKey_unk& a, const MotionKey_unk& b) const { return a < b; }
    };

    MotionTree_unk() : mSize(0), mRoot(0), mFirst(End_unk()) {}
    ~MotionTree_unk() {
        if (mRoot != 0) {
            Destroy_unk(mRoot);
        }
    }
    void Destroy_unk(Node* node) {
        if (node->mLeft != 0) {
            (void)((Node*)node->mLeft->mParent == node);
            Destroy_unk(node->mLeft);
        }
        if (node->mRight != 0) {
            (void)((Node*)node->mRight->mParent == node);
            Destroy_unk(node->mRight);
        }
        delete node;
    }
    Node* End_unk() { return (Node*)&mRoot; }
    iterator Find_unk(MotionKey_unk key) {
        Node* result = End_unk();
        Node* node = mRoot;
        while (node != 0) {
            if (!mComp(node->mValue.first, key)) {
                result = node;
                node = node->mLeft;
            } else {
                node = node->mRight;
            }
        }
        if (result == End_unk() || mComp(key, result->mValue.first)) {
            return iterator(End_unk());
        }
        return iterator(result);
    }
    void Erase_unk(iterator pos) {
        Node* n = pos.mNode;
        if (pos.mNode == mFirst) {
            ++pos;
            mFirst = pos.mNode;
        }
        RbBalanceErase_unk((std::__tree_node_base_void*)n, (std::__tree_node_base_void*)mRoot);
        delete n;
        mSize--;
    }
    // inferred: std::map::operator[]
    typename Node::Value& At_unk(const MotionKey_unk& key) {
        Node* pred;
        bool left;
        bool leftmost;
        Node* parent = FindInsertPos_unk(key, pred, left, leftmost);
        if (pred == 0 || mComp(pred->mValue.first, key)) {
            return InsertNode_unk(parent, left, leftmost, typename Node::Value(key, typename Node::Mapped()))->mValue;
        }
        return pred->mValue;
    }
    Node* FindInsertPos_unk(const MotionKey_unk& key, Node*& pred, bool& left, bool& leftmost);
    Node* InsertNode_unk(Node* parent, bool left, bool leftmost, const typename Node::Value& value);
    static Node* Allocate_unk() {
        Node* p = (Node*)::operator new(sizeof(Node));
        if (p == 0) {
            __msl_error("Memory allocation failure");
        }
        return p;
    }
    // inferred: frees the node if the construction does not finish
    struct NodeHolder_unk {
        NodeHolder_unk(void* alloc, Node* node) : mAlloc(alloc), mNode(node) {}
        ~NodeHolder_unk() {
            if (mNode != 0) {
                delete mNode;
            }
        }
        Node* release() {
            Node* node = mNode;
            mNode = 0;
            return node;
        }
        void* mAlloc;
        Node* mNode;
    };

    unsigned int mSize;
    Node* mRoot;
    Less_unk mComp; // 0x08
    Node* mFirst;   // 0x0C
};

template <class Node>
struct MotionTreeL4_unk : public MotionTree_unk<Node> {
    ~MotionTreeL4_unk() {}
};
template <class Node>
struct MotionTreeL3_unk : public MotionTreeL4_unk<Node> {
    ~MotionTreeL3_unk() {}
};
template <class Node>
struct MotionTreeL2_unk : public MotionTreeL3_unk<Node> {
    ~MotionTreeL2_unk() {}
};
template <class Node>
struct MotionMap_unk : public MotionTreeL2_unk<Node> {
    ~MotionMap_unk() {}
};

// inferred: the two maps (a function static, destroyed by fn_800A0D90)
struct MotionBankCache_unk {
    MotionMap_unk<MotionBankNode_unk> mBanks; // 0x00
    MotionMap_unk<MotionUseNode_unk> mUses;   // 0x10

    void Release_unk(MotionKey_unk key);
};

static inline MotionBankCache_unk& GetMotionBanks_unk() {
    static MotionBankCache_unk sBanks;
    return sBanks;
}

// inferred: drops the use of a (model, motion) pair, and its bank with the last use
inline void MotionBankCache_unk::Release_unk(MotionKey_unk key) {
    MotionTreeIter_unk<MotionUseNode_unk> use = mUses.Find_unk(key);
    if (--use.mNode->mValue.second.mRef <= 0) {
        MotionTreeIter_unk<MotionBankNode_unk> bank = mBanks.Find_unk(use.mNode->mValue.second.mBank);
        if (--bank.mNode->mValue.second.mRef <= 0) {
            delete[] bank.mNode->mValue.second.mData;
            mBanks.Erase_unk(bank);
        }
        mUses.Erase_unk(use);
    }
}

struct MotionBankRef_unk {
    MotionBankRef_unk(short* data, const MotionKey_unk& key) : mKey(key) { mData = data; }
    short* mData;
    MotionKey_unk mKey;
};
extern "C" MotionBankRef_unk fn_800A130C(MotionBankCache_unk* cache, atn::Model* model, atn::LH_Motion* motion);
extern "C" unsigned int fn_800DC500(const void* data, unsigned long len); // inferred: checksum

// NONMATCHING: 94.9% - registers: the use and bank iterators swap r29/r30 and the erase keeps the
// incremented iterator in a saved register
extern "C" void fn_800A0A68(atn::MotionCmd_unk* cmd, atn::Model* model, const atn::LH_Motion* motion) {
    if (cmd->mKey != 0) {
        GetMotionBanks_unk().Release_unk(cmd->mKey);
    }
    MotionBankRef_unk result = fn_800A130C(&GetMotionBanks_unk(), model, (atn::LH_Motion*)motion);
    cmd->mData = result.mData;
    cmd->mKey = result.mKey;
}

// NONMATCHING: 94.0% - registers (as fn_800A0A68)
extern "C" void fn_800A1060(atn::MotionCmd_unk* cmd) {
    if (cmd->mKey != 0) {
        GetMotionBanks_unk().Release_unk(cmd->mKey);
    }
    cmd->mKey = 0;
    cmd->mData = 0;
}

template <class Node>
Node* MotionTree_unk<Node>::FindInsertPos_unk(const MotionKey_unk& key, Node*& pred, bool& left, bool& leftmost) {
    pred = 0;
    Node* parent = End_unk();
    Node* node = mRoot;
    left = true;
    leftmost = true;
    while (node != 0) {
        parent = node;
        if (mComp(key, node->mValue.first)) {
            node = node->mLeft;
            left = true;
        } else {
            pred = node;
            node = node->mRight;
            left = false;
            leftmost = false;
        }
    }
    return parent;
}

template <class Node>
Node* MotionTree_unk<Node>::InsertNode_unk(Node* parent, bool left, bool leftmost, const typename Node::Value& value) {
    if (mSize == 0xFFFFFFFF) {
        __msl_error("tree::insert length error");
    }
    NodeHolder_unk holder(&mRoot, Allocate_unk());
    ::new (&holder.mNode->mValue) typename Node::Value(value);
    Node* node = holder.release();
    node->mRight = 0;
    node->mLeft = 0;
    ::new (&node->mParent) unsigned int((unsigned int)parent);
    if (left) {
        parent->mLeft = node;
    } else {
        parent->mRight = node;
    }
    mSize++;
    std::__red_black_tree_void::balance_insert((std::__tree_node_base_void*)node, (std::__tree_node_base_void*)mRoot);
    if (leftmost) {
        mFirst = node;
    }
    return node;
}

// inferred: key of the bank: string hash (djb2, signed chars) and checksum of the node indices
static inline unsigned int HashBytes_unk(const char* data, int size) {
    unsigned int h = 5381;
    for (int i = 0; i < size; i++) {
        h = (h << 5) + (h + data[i]);
    }
    return h & 0x7FFFFFFF;
}

// NONMATCHING: 99.7% - the order of the result stores of one branch (the string pool matches since the split)
extern "C" MotionBankRef_unk fn_800A130C(MotionBankCache_unk* cache, atn::Model* model, atn::LH_Motion* motion) {
    unsigned int modelHash = model->fn_8008B138();
    unsigned int motionId = motion->fn_804768B4();
    MotionKey_unk useKey = ((MotionKey_unk)motionId << 32) | modelHash;
    MotionUseNode_unk* use = cache->mUses.Find_unk(useKey).mNode;
    if (use == cache->mUses.End_unk()) {
        short keys[0x100];
        int count = ((MotionModelRes_unk*)model->GetRes_unk())->m44;
        fn_800A09D0(keys, model, motion);
        int size = count * 2;
        unsigned int sum = fn_800DC500(keys, size);
        MotionKey_unk bankKey = ((MotionKey_unk)HashBytes_unk((const char*)keys, size) << 32) | sum;
        if (cache->mBanks.Find_unk(bankKey).mNode == cache->mBanks.End_unk()) {
            short* data = (short*)new ((MemoryTag)6, "", "", 0) char[size];
            memcpy(data, keys, size);
            cache->mBanks.At_unk(bankKey).second = MakeBank_unk(data, 1);
            cache->mUses.At_unk(useKey).second = MotionUse_unk(bankKey, 1);
            return MotionBankRef_unk(data, useKey);
        } else {
            cache->mUses.At_unk(useKey).second = MotionUse_unk(bankKey, 1);
            MotionBank_unk& bank = cache->mBanks.At_unk(bankKey).second;
            bank.mRef++;
            return MotionBankRef_unk(bank.mData, useKey);
        }
    } else {
        const MotionKey_unk& bankKey = use->mValue.second.mBank;
        use->mValue.second.mRef++;
        return MotionBankRef_unk(cache->mBanks.At_unk(bankKey).second.mData, useKey);
    }
}

// inferred: random generator of MP::CryptMP (called from MP/CryptMP vf1C), created on first use with the tick
// as seed
typedef std::tr1::variate_generator<std::tr1::mt19937, std::tr1::uniform_real<double> > Random_unk;
extern "C" unsigned long OSGetTick();
static double sRandMin_unk = 0.0;
static double sRandMax_unk = 1.0;
static Random_unk* sRandom_unk;

// NONMATCHING: 83.4% - the original builds the engine before the distribution and keeps the temporaries in
// ascending order (same as MP/CryptMP vf0C); MWCC evaluates the distribution first here. The range doubles are in
// .data in the original (0x807789C8 = 0.0, 0x807789D0 = 1.0); here the zero one goes to .bss.
extern "C" Random_unk* fn_800A2664() {
    if (sRandom_unk == 0) {
        sRandom_unk = new ((MemoryTag)1, "", "", 0)
            Random_unk(std::tr1::mt19937(OSGetTick()), std::tr1::uniform_real<double>(sRandMin_unk, sRandMax_unk));
    }
    return sRandom_unk;
}
