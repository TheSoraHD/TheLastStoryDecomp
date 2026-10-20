#include "game/TownTask.h"
#include "atn/Memory.h"
#include "atn/Csv.h"
#include "atn/LH_Binary.h"

extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void* fn_804A7EA4(TownTask* task, const char* path); // inferred: loads a minimap csv
extern "C" void* lbl_8087EFB4;
extern "C" void* fn_800C16B4(void* obj);
extern "C" void fn_8049B780(MapObject_unk* obj, void* arg);

TownTask* lbl_8087FA20;

extern "C" void fn_8003E120(TownNode_unk* node, TownNode_unk* root); // inferred: rebalances after an insert
// inferred: allocates a tree node
inline TownNode_unk* TownAllocNode_unk() {
    TownNode_unk* node = (TownNode_unk*)::operator new(sizeof(TownNode_unk));
    if (node == 0) {
        __msl_error("Memory allocation failure");
    }
    return node;
}
// inferred: owner of a node being inserted (freed if the insertion fails); names invented
struct TownNodeHolder_unk {
    TownNodeHolder_unk(TownNode_unk** alloc, TownNode_unk* node) : mAlloc(alloc), mNode(node) {}
    TownNode_unk* Release_unk() {
        TownNode_unk* node = mNode;
        mNode = 0;
        return node;
    }
    ~TownNodeHolder_unk() {
        if (mNode != 0) {
            ::operator delete(mNode);
        }
    }
    TownNode_unk** mAlloc;
    TownNode_unk* mNode;
};

// inferred: creator of the task; not referenced (the task is created by another file), so the linker strips it,
// but its "" literal stays at the start of the string pool. Name invented.
TownTask* TownTask::Create_unk(atn::Task* parent) {
    return new (1, "", "", 0) TownTask(parent);
}

TownTask::TownTask(atn::Task* parent)
    : MapTask(parent), m120(0), m124(1), m128(0), m12C(2), m130(0), m134(50.0f), m138(0), m13C(0),
      m140(0.0f, 0.0f, 0.0f), m164(1.1f), m168(0.0f, 0.0f, 0.0f), m174(0), m178(0), mWidth(3), mHeight(3) {
    lbl_8087FA20 = this;
    mF8 = 1;
    m258 = fn_804A7EA4(this, "boot/minimap_town.csv");
}

// NONMATCHING: 26% - the tree erase is inlined one recursion level deeper here than in the original (the
// rest of the code is the same: vector, two trees, two strings, ResHandle)
TownTask::~TownTask() {
    lbl_8087FA20 = 0;
}

int TownTask::vf0C() {
    if (GetLoader_unk()->fn_80473F50() != 0) {
        return 0;
    }
    if (m120 == 0) {
        m120 = 1;
        fn_805B7B24();
        void* group = m11C.Load("Town");
        fn_800874C8(group, "Width", &mWidth, 1, 3, 1, 0, 0);
        fn_800874C8(group, "Height", &mHeight, 1, 3, 1, 0, 0);
    }
    if (m128 != 0) {
        return 0;
    }
    if (m124 != 0) {
        fn_805B8DB4(1);
        m124 = 0;
    }
    if (MapTask::vf0C() != 0) {
        for (MapObject_unk* obj = m70; obj != 0; obj = obj->mNext) {
            if (obj->mType == 4) {
                fn_8049B780(obj, fn_800C16B4(lbl_8087EFB4));
            }
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: vf10 (0x805B7398, 0x76C bytes) is not written yet

// NONMATCHING: 34% - the target stores z before the paired x/y store (psq_l, lfs, stfs, psq_st); no Vec.h helper
// has that order and new inline asm is not allowed
void TownTask::fn_805B7B04(const atn::Vector3& pos) {
    m128 = 0;
    m168 = pos;
}

void TownTask::vf14() {
    MapTask::vf14();
}

// inferred: list of the blocks shown with a block (block ids, -1 = none); names invented
struct TownBlockList_unk {
    TownBlockList_unk() {
        for (int i = 0; i < 9; i++) {
            m[i] = -1;
        }
    }
    int m[9];
};
// inferred: element of the block trees (key, list)
struct TownPair_unk {
    TownPair_unk(const int& key, const TownBlockList_unk& list) : mKey(key), mList(list) {}
    int mKey;
    TownBlockList_unk mList;
};

inline TownPair_unk TownMakePair_unk(const int& key, const TownBlockList_unk& list) {
    return TownPair_unk(key, list);
}

extern "C" TownNode_unk* fn_805B80B0(TownTree_unk* tree, const int* key, TownNode_unk** found, bool* left,
                                     bool* first);
extern "C" TownNode_unk* fn_805B9838(TownTree_unk* tree, TownNode_unk* parent, bool left, bool first,
                                     const TownValue_unk* value);

// inferred: map::operator[] of the block trees (names invented)
inline TownPair_unk& TownTreeAt_unk(TownTree_unk* tree, const int& key, TownNode_unk*& found) {
    bool left;
    bool first;
    TownNode_unk* parent = fn_805B80B0(tree, &key, &found, &left, &first);
    TownPair_unk* pair;
    if (found == 0 || (pair = (TownPair_unk*)&found->mValue, pair->mKey < key)) {
        TownNode_unk* node =
            fn_805B9838(tree, parent, left, first, (const TownValue_unk*)&TownPair_unk(key, TownBlockList_unk()));
        pair = (TownPair_unk*)&node->mValue;
    }
    return *pair;
}

extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" double strtod(const char* s); // inferred: called without the end pointer argument
extern "C" int sscanf(const char* s, const char* format, ...);

// inferred: reads a list of blocks ("xx_zz" fields up to an empty one) into a block tree (name invented)
static void ReadBlockList_unk(TownTask* town, atn::CsvTab& csv, TownTree_unk* tree) {
    const char* field = csv.fn_8005B9CC();
    int x = -1;
    int z = -1;
    TownNode_unk* found;
    TownBlockList_unk list;
    int i = 0;
    do {
        sscanf(field, "%02d_%02d", &x, &z);
        list.m[i++] = x + z * town->m138;
        field = csv.fn_8005B9CC();
    } while (strcmp("", field) != 0);
    TownTreeAt_unk(tree, list.m[0], found).mList = list;
}

// inferred: reads the town description (minimap csv already loaded at 0x58)
void TownTask::fn_805B7B24() {
    atn::CsvTab csv((const char*)GetBinary_unk()->fn_80470580(), GetBinary_unk()->fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "MAPS") == 0) {
            m158 = csv.fn_8005B9CC();
        } else if (strcmp(key, "PATTERN") == 0) {
            m14C = csv.fn_8005B9CC();
        } else if (strcmp(key, "SIZE") == 0) {
            m138 = atoi(csv.fn_8005B9CC());
            m13C = atoi(csv.fn_8005B9CC());
            m134 = strtod(csv.fn_8005B9CC());
        } else if (strcmp(key, "ORIGIN") == 0) {
            m140.x = strtod(csv.fn_8005B9CC());
            m140.y = strtod(csv.fn_8005B9CC());
            m140.z = strtod(csv.fn_8005B9CC());
        } else if (strcmp(key, "BLOCKVIS") == 0) {
            ReadBlockList_unk(this, csv, &m230);
        } else if (strcmp(key, "BLOCKSHOW") == 0) {
            ReadBlockList_unk(this, csv, &m240);
        }
    } while (csv.fn_8005BBF8() != 0);
}

// inferred: lower bounds of the block coordinates (globals of another file)
extern "C" int lbl_8087E6F8;
extern "C" int lbl_8087E6FC;

// inferred: clamp evaluating the lower bound twice (macro; name invented)
template <class T>
inline T TownClamp_unk(const T& x, const T& lo, const T& hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}
#define TOWN_CLAMP(x, lo, hi) TownClamp_unk(x, lo, hi)

// inferred: finds where `key` goes in the tree: returns the parent node (or the end node) and sets the last
// node not greater than the key, the side and whether it is the leftmost position (names invented)
extern "C" TownNode_unk* fn_805B80B0(TownTree_unk* tree, const int* key, TownNode_unk** found, bool* left,
                                     bool* first) {
    *found = 0;
    TownNode_unk* parent = (TownNode_unk*)&tree->mRoot;
    TownNode_unk* node = tree->mRoot;
    *left = true;
    *first = true;
    while (node != 0) {
        parent = node;
        if (*key < node->mValue.mKey) {
            node = node->mLeft;
            *left = true;
        } else {
            *found = node;
            node = node->mRight;
            *left = false;
            *first = false;
        }
    }
    return parent;
}

// NONMATCHING: 99.6% - registers of the 1/size division
// inferred: block coordinates of a position, clamped to the town size
extern "C" void fn_805B8110(TownTask* town, const atn::Vector3* pos, int* x, int* z) {
    float inv = 1.0f / town->m134;
    atn::Vector3 d = *pos - town->m140;
    atn::Vector3 cell = d * inv;
    *x = (int)cell.x;
    *z = (int)cell.z;
    *x = TOWN_CLAMP(*x, lbl_8087E6F8, town->m138);
    *z = TOWN_CLAMP(*z, lbl_8087E6FC, town->m13C);
}

// NONMATCHING: 88% - scheduling of the x/z fmadds
// inferred: center of a block
extern "C" atn::Vector3 fn_805B81F0(TownTask* town, int x, int z) {
    atn::Vector3 v;
    v.x = x * town->m134 + 0.5f * town->m134;
    v.y = 0.0f;
    v.z = z * town->m134 + 0.5f * town->m134;
    return v + town->m140;
}

// inferred: field (lbl_8087F430); only the members used here
struct TownField_unk {
    unsigned char m0000[0x38];
    unsigned int m38; // inferred: bit 1: town field
    unsigned char m003C[0x553C - 0x3C];
    int m553C;
};
extern "C" TownField_unk* lbl_8087F430;
extern "C" int fn_80370174(TownField_unk* field, int id); // inferred: event flag
extern "C" float fn_805F9920(const atn::Vector3* v);      // inferred: squared length

// inferred: tree::find of the block trees (names invented)
inline TownNode_unk* TownTreeFind_unk(TownTree_unk* tree, int key) {
    TownNode_unk* result = (TownNode_unk*)&tree->mRoot;
    TownNode_unk* node = tree->mRoot;
    while (node != 0) {
        if (!(node->mValue.mKey < key)) {
            result = node;
            node = node->mLeft;
        } else {
            node = node->mRight;
        }
    }
    if (result == (TownNode_unk*)&tree->mRoot || key < result->mValue.mKey) {
        result = (TownNode_unk*)&tree->mRoot;
    }
    return result;
}

// inferred: fills the list (macro: the elements are stored in order)
#define TOWN_SET(l, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
    (l).m[0] = (a0);                                     \
    (l).m[1] = (a1);                                     \
    (l).m[2] = (a2);                                     \
    (l).m[3] = (a3);                                     \
    (l).m[4] = (a4);                                     \
    (l).m[5] = (a5);                                     \
    (l).m[6] = (a6);                                     \
    (l).m[7] = (a7);                                     \
    (l).m[8] = (a8)

// inferred: id of the block (x, z)
#define TOWN_ID(x, z) ((x) + (z) * town->m138)

// NONMATCHING: 98.5% - the -1 of the list gets r31 here and r27 in the original (the parameters are one
// register higher); tried other forms of the list constructor and of its declaration.
// inferred: blocks to show around the block (x, z): special cases of the town map near some positions, the
// BLOCKSHOW list of the block, or the blocks of the mWidth x mHeight area around it (names invented)
extern "C" TownBlockList_unk fn_805B827C(TownTask* town, int x, int z, int special) {
    TownBlockList_unk list;
    if (special != 0 && lbl_8087F430 != 0 && (lbl_8087F430->m38 & 2) == 2 && lbl_8087F430->m553C == 0) {
        atn::Vector3 v = town->m168 - atn::Vector3(104.0f, 0.0f, 235.0f);
        v.y = 0.0f;
        if (fn_805F9920(&v) < 900.0f && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x713)) {
            TOWN_SET(list, TOWN_ID(3, 2), TOWN_ID(2, 2), TOWN_ID(4, 2), TOWN_ID(2, 3), TOWN_ID(3, 3), -1, -1, -1, -1);
            return list;
        }
        v = town->m168 - atn::Vector3(186.0f, 0.0f, -893.0f);
        v.y = 0.0f;
        if (fn_805F9920(&v) < 2500.0f) {
            TOWN_SET(list, TOWN_ID(3, 0), TOWN_ID(2, 1), TOWN_ID(3, 1), TOWN_ID(4, 1), TOWN_ID(3, 2), -1, -1, -1, -1);
            return list;
        }
        v = town->m168 - atn::Vector3(-751.0f, 0.0f, -213.0f);
        v.y = 0.0f;
        if (fn_805F9920(&v) < 3600.0f) {
            TOWN_SET(list, TOWN_ID(1, 2), TOWN_ID(0, 1), TOWN_ID(1, 1), TOWN_ID(0, 2), TOWN_ID(1, 2), TOWN_ID(2, 2), -1, -1, -1);
            return list;
        }
        v = town->m168 - atn::Vector3(964.0f, 0.0f, 189.0f);
        v.y = 0.0f;
        if (fn_805F9920(&v) < 900.0f) {
            TOWN_SET(list, TOWN_ID(4, 2), TOWN_ID(4, 1), TOWN_ID(4, 3), TOWN_ID(5, 1), TOWN_ID(5, 2), TOWN_ID(5, 3), -1, -1, -1);
            return list;
        }
        v = town->m168 - atn::Vector3(698.0f, 0.0f, -409.0f);
        v.y = 0.0f;
        if (fn_805F9920(&v) < 900.0f) {
            TOWN_SET(list, TOWN_ID(4, 1), TOWN_ID(3, 1), TOWN_ID(3, 2), TOWN_ID(4, 2), TOWN_ID(5, 1), TOWN_ID(5, 2), -1, -1, -1);
            return list;
        }
    }
    if (town->m168.y < 45.0f) {
        if (x == 4 && z == 2) {
            TOWN_SET(list, TOWN_ID(4, 2), TOWN_ID(3, 1), TOWN_ID(3, 2), TOWN_ID(3, 3), TOWN_ID(4, 1), TOWN_ID(4, 3), TOWN_ID(5, 1), TOWN_ID(5, 2), TOWN_ID(2, 2));
            return list;
        }
    } else {
        if (x == 4 && z == 2) {
            atn::Vector3 c = fn_805B81F0(town, x, z);
            if (town->m168.x < c.x && town->m168.z > c.z) {
                TOWN_SET(list, TOWN_ID(4, 2), TOWN_ID(2, 2), TOWN_ID(3, 2), TOWN_ID(3, 3), TOWN_ID(4, 1), TOWN_ID(4, 3), TOWN_ID(2, 3), TOWN_ID(5, 2), TOWN_ID(5, 3));
                return list;
            }
        }
        if (x == 2 && z == 2) {
            atn::Vector3 c = fn_805B81F0(town, x, z);
            if (town->m168.x > c.x && town->m168.z < c.z) {
                TOWN_SET(list, TOWN_ID(2, 2), TOWN_ID(3, 0), TOWN_ID(1, 1), TOWN_ID(1, 2), TOWN_ID(2, 1), TOWN_ID(2, 3), TOWN_ID(3, 1), TOWN_ID(3, 2), TOWN_ID(3, 3));
                return list;
            }
        }
        if (x == 3 && z == 1) {
            atn::Vector3 d(480.0f, 0.0f, -521.0f);
            d.x -= town->m168.x;
            d.y -= town->m168.y;
            d.z -= town->m168.z;
            d.y = 0.0f;
            if (fn_805F9920(&d) < 22500.0f) {
                TOWN_SET(list, TOWN_ID(3, 1), TOWN_ID(5, 2), TOWN_ID(2, 1), TOWN_ID(2, 2), TOWN_ID(3, 0), TOWN_ID(3, 2), TOWN_ID(4, 1), TOWN_ID(4, 2), TOWN_ID(5, 1));
                return list;
            }
        }
        if (x == 4 && z == 3) {
            atn::Vector3 d(535.0f, 0.0f, 245.0f);
            d.x -= town->m168.x;
            d.y -= town->m168.y;
            d.z -= town->m168.z;
            d.y = 0.0f;
            if (fn_805F9920(&d) < 4900.0f) {
                TOWN_SET(list, TOWN_ID(4, 3), TOWN_ID(2, 2), TOWN_ID(3, 2), TOWN_ID(3, 3), TOWN_ID(4, 1), TOWN_ID(4, 2), TOWN_ID(2, 3), TOWN_ID(5, 2), TOWN_ID(5, 3));
                return list;
            }
        }
    }
    int key = TOWN_ID(x, z);
    TownNode_unk* node = TownTreeFind_unk(&town->m230, key);
    if (node != (TownNode_unk*)&town->m230.mRoot) {
        return ((TownPair_unk*)&node->mValue)->mList;
    }
    list.m[0] = key;
    int* p = &list.m[1];
    for (int i = x - town->mWidth / 2; i <= x + town->mWidth / 2; i++) {
        if (i >= 0 && i < town->m138) {
            for (int j = z - town->mHeight / 2; j <= z + town->mHeight / 2; j++) {
                if (j >= 0 && j < town->m13C) {
                    if (i != x || j != z) {
                        *p++ = TOWN_ID(i, j);
                    }
                }
            }
        }
    }
    return list;
}

// NONMATCHING: fn_805B8DB4 (0x8F4, block loading) is not written yet

// NONMATCHING: 96.5% - float registers of the cell computation
// inferred: nonzero when the position is in the town area (the block under it is flagged)
extern "C" int fn_805B96A8(TownTask* town, const atn::Vector3* pos) {
    float inv = 1.0f / town->m134;
    atn::Vector3 cell = (*pos - town->m140) * inv;
    int x = (int)cell.x;
    int z = (int)cell.z;
    x = TOWN_CLAMP(x, lbl_8087E6F8, town->m138);
    z = TOWN_CLAMP(z, lbl_8087E6FC, town->m13C);
    if (x == 2 && z == 0) {
        return 1;
    }
    for (int i = 0; i < 9; i++) {
        TownBlock_unk* block = &town->mBlocks[i];
        if (block->mParts != 0 && block->mX == x && block->mZ == z) {
            return (block->mParts->m38 >> 1) & 1;
        }
    }
    return 0;
}

// NONMATCHING: 97% - the node gets r26 in the original (created after the parameters)
// inferred: inserts a new node under `parent` (tree::insert); names invented
extern "C" TownNode_unk* fn_805B9838(TownTree_unk* tree, TownNode_unk* parent, bool left, bool first,
                                     const TownValue_unk* value) {
    if (tree->mSize == 0xFFFFFFFF) {
        __msl_error("tree::insert length error");
    }
    TownNodeHolder_unk holder(&tree->mRoot, TownAllocNode_unk());
    new (&holder.mNode->mValue) TownValue_unk(*value);
    TownNode_unk* node = holder.Release_unk();
    node->mRight = 0;
    node->mLeft = 0;
    new (&node->mParent) TownNode_unk*(parent);
    if (left) {
        parent->mLeft = node;
    } else {
        parent->mRight = node;
    }
    tree->mSize++;
    fn_8003E120(node, tree->mRoot);
    if (first) {
        tree->mBegin = node;
    }
    return node;
}

inline void TownFreeNode_unk(TownNode_unk* node) {
    ::operator delete(node);
}

// NONMATCHING: 9% - MWCC inlines the recursion one level deeper here (31 calls instead of 15)
// inferred: frees a subtree (inlined into itself)
extern "C" void fn_805B99A8(TownTree_unk* tree, TownNode_unk* node) {
    if (node->mLeft != 0) {
        fn_805B99A8(tree, node->mLeft);
    }
    if (node->mRight != 0) {
        fn_805B99A8(tree, node->mRight);
    }
    delete node;
}
