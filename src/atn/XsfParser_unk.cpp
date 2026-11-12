// inferred: reader of XSF files (chunks of {size, type, flags}, "XSF@" magic, byte swapped when the
// magic is "@FSX") and the curves read from them (one MSL std::map<float, key> per channel, laid out
// by hand as in game/GameEventScript.h). Names invented.

#include "std/string.h"
#include "std/tree.h"
#include "std/new.h"

extern "C" int fn_8006969C(); // inferred: nonzero when the data must be byte swapped

// inferred: segment of a game curve built from two keys (defined in atn/Bezier_unk.cpp)
struct CurveSegment_unk {
    float mX[4];
    float mY[4];
    float mTime;           // 0x20
    unsigned char mBezier; // 0x24
};
extern "C" void fn_80507790(CurveSegment_unk* seg, float time, const float* a, const float* b, unsigned char bezier);
extern "C" float fn_805077F0(CurveSegment_unk* seg);

// inferred: point of a curve segment: key time and value, tangent (x, y)
struct XsfCurvePoint_unk {
    XsfCurvePoint_unk(float x, float y, float tx, float ty) : mX(x), mY(y), mTX(tx), mTY(ty) {}
    float mX;
    float mY;
    float mTX;
    float mTY;
};

// inferred: chunk header
struct XsfChunk_unk {
    unsigned int mSize;
    unsigned short mType;  // 0x04
    unsigned short mFlags; // 0x06
};

// inferred: key of a curve: value, tangent in (x, y), tangent out (x, y) and whether the segment is
// a curve (else linear)
struct XsfKey_unk {
    XsfKey_unk(float value, float inX, float inY, float outX, float outY, bool curve)
        : mValue(value), mInX(inX), mInY(inY), mOutX(outX), mOutY(outY), mCurve(curve) {}
    float mValue; // 0x00
    float mInX;   // 0x04
    float mInY;   // 0x08
    float mOutX;  // 0x0C
    float mOutY;  // 0x10
    bool mCurve;  // 0x14
};

// inferred: std::pair<const float, XsfKey_unk>
struct XsfKeyPair_unk {
    XsfKeyPair_unk(const float& time, const XsfKey_unk& key) : first(time), second(key) {}
    const float first;
    XsfKey_unk second;
};

// inferred: node of the map of keys
struct XsfKeyNode_unk {
    typedef XsfKeyPair_unk Value;
    XsfKeyNode_unk* mLeft;
    XsfKeyNode_unk* mRight;
    unsigned int mParent; // bit 0: red
    XsfKeyPair_unk mValue; // 0x0C

    XsfKeyNode_unk* Parent_unk() const { return (XsfKeyNode_unk*)(mParent & ~1); }
    // inferred: iterator increment / decrement of the MSL tree, in place
    static void Next_unk(XsfKeyNode_unk*& node) {
        if (node->mRight != 0) {
            XsfKeyNode_unk* n = node->mRight;
            while (n->mLeft != 0) {
                n = n->mLeft;
            }
            node = n;
        } else {
            while (node != node->Parent_unk()->mLeft) {
                node = node->Parent_unk();
            }
            node = node->Parent_unk();
        }
    }
    static void Prev_unk(XsfKeyNode_unk*& node) {
        if (node->mLeft != 0) {
            XsfKeyNode_unk* n = node->mLeft;
            while (n->mRight != 0) {
                n = n->mRight;
            }
            node = n;
        } else {
            while (node == node->Parent_unk()->mLeft) {
                node = node->Parent_unk();
            }
            node = node->Parent_unk();
        }
    }
};

// inferred: iterator of the map
struct XsfKeyIter_unk {
    XsfKeyIter_unk(XsfKeyNode_unk* node) : mNode(node) {}
    XsfKeyIter_unk& operator--() {
        XsfKeyNode_unk::Prev_unk(mNode);
        return *this;
    }
    XsfKeyNode_unk* mNode;
};

// inferred: result of an insert (std::pair<iterator, bool>)
struct XsfInsert_unk {
    XsfInsert_unk(XsfKeyNode_unk* const& node, const bool& done) : first(node), second(done) {}
    XsfKeyNode_unk* first;
    bool second;
};

// inferred: the MSL red-black tree of std::map<float, XsfKey_unk>: size, root (the end node is at
// &mRoot), the compare object and the first node. Names invented.
template <class Node>
struct XsfKeyTree_unk {
    XsfKeyTree_unk() : mSize(0), mRoot(0), mFirst(End_unk()) {}
    ~XsfKeyTree_unk() {
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
    Node* Begin_unk() { return mFirst; }
    // inferred: last node (std::map::rbegin)
    Node* Last_unk() {
        if (mSize == 0) {
            return End_unk();
        }
        if (mSize == 1) {
            return mFirst;
        }
        XsfKeyIter_unk it(End_unk());
        --it;
        return it.mNode;
    }
    Node* FindInsertPos_unk(const float& key, Node*& pred, bool& left, bool& leftmost);
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
    bool Insert_unk(const typename Node::Value& value) {
        Node* pred;
        bool leftmost;
        bool left;
        Node* parent = FindInsertPos_unk(value.first, pred, left, leftmost);
        if (pred == 0 || mComp(pred->mValue.first, value.first)) {
            return XsfInsert_unk(InsertNode_unk(parent, left, leftmost, value), true).second;
        }
        return XsfInsert_unk(pred, false).second;
    }

    unsigned int mSize;
    Node* mRoot;
    // inferred: std::less<float>
    struct Less_unk {
        bool operator()(const float& a, const float& b) const { return a < b; }
    } mComp; // 0x08
    Node* mFirst;             // 0x0C
};

// inferred: the layers of std::map over the tree (the destructor tests the object five times)
template <class Node>
struct XsfKeyTreeL4_unk : public XsfKeyTree_unk<Node> {
    ~XsfKeyTreeL4_unk() {}
};
template <class Node>
struct XsfKeyTreeL3_unk : public XsfKeyTreeL4_unk<Node> {
    ~XsfKeyTreeL3_unk() {}
};
template <class Node>
struct XsfKeyTreeL2_unk : public XsfKeyTreeL3_unk<Node> {
    ~XsfKeyTreeL2_unk() {}
};
template <class Node>
struct XsfKeyMap_unk : public XsfKeyTreeL2_unk<Node> {
    ~XsfKeyMap_unk() {}
};

// inferred: curve set: duration, a vector copied from the chunk header, the time range of the keys,
// the name and 13 channels of keys
struct XsfCurveSet_unk {
    typedef XsfKeyMap_unk<XsfKeyNode_unk> Channel_unk;

    XsfCurveSet_unk(const char* name);
    ~XsfCurveSet_unk();
    int fn_805BF1E8(int channel);
    float fn_805BF204(float time, int channel);
    float fn_805BF214(float time, Channel_unk* channel);
    void fn_805BF410(float time, float* out, int x, int y, int z);
    int fn_805BF4B4(int channel, float time, float v0, float v1, float v2, float v3, float v4);
    int fn_805BF590(int channel, float time, float value);
    void fn_805BF660();

    float mLength; // 0x00
    float m04;     // 0x04: inferred: vector copied from the chunk header by atn/Xsca_unk
    float m08;
    float m0C;
    float mStart;  // 0x10
    float mEnd;    // 0x14
    std::string mName;          // 0x18
    Channel_unk mChannels[13];  // 0x24
};

// inferred: byte swap written with the rlwimi intrinsic (slwi + three rlwimi in the original)
static inline unsigned int __Swap32_unk(unsigned int x) {
    unsigned int r = x << 24;
    r = __rlwimi(r, x, 8, 24, 31);
    r = __rlwimi(r, x, 24, 16, 23);
    r = __rlwimi(r, x, 8, 8, 15);
    return r;
}

static inline void __SwapInPlace32_unk(unsigned int* p) {
    __stwbrx(*p, p, 0);
}

static inline void __SwapInPlace16_unk(unsigned short* p) {
    __sthbrx(*p, p, 0);
}

static inline float __SwapF_unk(float f) {
    float r;
    __stwbrx(*(unsigned int*)&f, &r, 0);
    return r;
}

// inferred: keys of a chunk with the other byte order
// NONMATCHING: 50.4% - the original swaps each float through a stack temporary whose address is
// recomputed in the loop; here the temporaries' addresses are kept in saved registers.
extern "C" void fn_805BE8FC(void* self, unsigned int* chunk, XsfCurveSet_unk* curves, int channel) {
    unsigned int count = __Swap32_unk(chunk[0]);
    unsigned int size = __Swap32_unk(chunk[1]);
    float* p = (float*)(chunk + 2);
    for (unsigned int i = 0; i < count; i++) {
        float time = __SwapF_unk(p[0]);
        float value = __SwapF_unk(p[1]);
        if (size > 8) {
            curves->fn_805BF4B4(channel, time, value, __SwapF_unk(p[2]), __SwapF_unk(p[3]), __SwapF_unk(p[4]),
                                __SwapF_unk(p[5]));
        } else {
            curves->fn_805BF590(channel, time, value);
        }
        p = (float*)((char*)p + (size & ~3));
    }
}

// inferred: keys of a chunk
// NONMATCHING: 99.5% - count/step registers swapped (r31/r29)
extern "C" void fn_805BEA34(void* self, unsigned int* chunk, XsfCurveSet_unk* curves, int channel) {
    unsigned int size = chunk[1];
    unsigned int count = chunk[0];
    float* p = (float*)(chunk + 2);
    for (unsigned int i = 0; i < count; i++) {
        float time = p[0];
        float value = p[1];
        if (size > 8) {
            curves->fn_805BF4B4(channel, time, value, p[2], p[3], p[4], p[5]);
        } else {
            curves->fn_805BF590(channel, time, value);
        }
        p = (float*)((char*)p + (size & ~3));
    }
}

static const char sMagic[] = "XSF@";
static const char sMagicSwapped[] = "@FSX";

// inferred: header check of an XSF file
class XsfFile_unk {
public:
    XsfFile_unk(void* data);
    ~XsfFile_unk();
    void fn_805BEB54();

    unsigned int* mData; // 0x00
    void* m04;
    int mValid;          // 0x08
    int mSwapped;        // 0x0C
    XsfChunk_unk* mFirst; // 0x10
};

XsfFile_unk::XsfFile_unk(void* data) {
    mData = (unsigned int*)data;
    m04 = data;
    mValid = 0;
    mSwapped = 0;
    mFirst = 0;
    fn_805BEB54();
}

XsfFile_unk::~XsfFile_unk() {}

// NONMATCHING: 94.6% - register allocation of the magic addresses and of the swap loop
void XsfFile_unk::fn_805BEB54() {
    unsigned int magic = *(unsigned int*)sMagic;
    unsigned int swapped = *(unsigned int*)sMagicSwapped;
    unsigned int value = *mData;
    if (fn_8006969C() != 0) {
        magic = __Swap32_unk(magic);
        swapped = __Swap32_unk(swapped);
    }
    if (value == magic) {
        mValid = 1;
        mSwapped = 0;
    } else if (value == swapped) {
        mValid = 1;
        mSwapped = 1;
        for (XsfChunk_unk* p = (XsfChunk_unk*)(mData + 1); p->mSize != 0;
             p = (XsfChunk_unk*)((char*)p + p->mSize)) {
            __SwapInPlace32_unk(&p->mSize);
            __SwapInPlace16_unk(&p->mType);
            __SwapInPlace16_unk(&p->mFlags);
        }
    } else {
        mValid = 0;
    }
    mFirst = (XsfChunk_unk*)(mData + 1);
}

// inferred: position in the chunk list
class XsfIter_unk {
public:
    static bool IsValid(XsfChunk_unk* p) { return p != 0 && p->mSize != 0; }
    static unsigned short GetType(XsfChunk_unk* p) { return p != 0 ? p->mType : 0; }
    static unsigned int GetSize(XsfChunk_unk* p) { return p != 0 ? p->mSize : 0; }

    void fn_805BEC68(XsfChunk_unk* p);
    int fn_805BEC70();
    XsfChunk_unk* fn_805BECA0();
    XsfChunk_unk* fn_805BED50();
    int fn_805BEE04();

    XsfChunk_unk* mChunk;
};

void XsfIter_unk::fn_805BEC68(XsfChunk_unk* p) {
    mChunk = p;
}

int XsfIter_unk::fn_805BEC70() {
    if ((mChunk != 0 ? mChunk->mFlags : 0) & 1) {
        return 1;
    }
    return 0;
}

// inferred: next chunk of the same level
XsfChunk_unk* XsfIter_unk::fn_805BECA0() {
    XsfChunk_unk* p = mChunk;
    if (IsValid(p)) {
        unsigned short type = GetType(p);
        do {
            p = (XsfChunk_unk*)((char*)p + p->mSize);
            if (!IsValid(p)) {
                break;
            }
            if (type == GetType(p)) {
                return p;
            }
            if (type > GetType(p)) {
                break;
            }
        } while (p->mSize != 0);
    }
    return 0;
}

// inferred: first child chunk
XsfChunk_unk* XsfIter_unk::fn_805BED50() {
    XsfChunk_unk* p = mChunk;
    if (IsValid(p)) {
        unsigned int type = GetType(p) + 1;
        do {
            p = (XsfChunk_unk*)((char*)p + p->mSize);
            if (!IsValid(p)) {
                break;
            }
            if (type == GetType(p)) {
                return p;
            }
            if (type > GetType(p)) {
                break;
            }
        } while (p->mSize != 0);
    }
    return 0;
}

// inferred: number of child chunks
// NONMATCHING: 87.3% - the original keeps `first == 0` as a value and a running pointer; tried a
// running pointer instead of the offset (73%).
int XsfIter_unk::fn_805BEE04() {
    XsfChunk_unk* first = mChunk;
    unsigned int type = GetType(first) + 1;
    int count = 0;
    int offset = 0;
    for (;;) {
        XsfChunk_unk* p = (XsfChunk_unk*)((char*)first + offset + GetSize(first));
        if (!IsValid(p)) {
            return count;
        }
        if (type > GetType(p)) {
            return count;
        }
        if (type == GetType(p)) {
            count++;
        }
        offset += GetSize(p);
    }
}

XsfCurveSet_unk::XsfCurveSet_unk(const char* name)
    : mLength(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), mStart(0.0f), mEnd(0.0f), mName(name != 0 ? name : "") {}

XsfCurveSet_unk::~XsfCurveSet_unk() {}

int XsfCurveSet_unk::fn_805BF1E8(int channel) {
    return mChannels[channel].mSize != 0;
}

float XsfCurveSet_unk::fn_805BF204(float time, int channel) {
    return fn_805BF214(time, &mChannels[channel]);
}

// inferred: value of a channel at a time (linear or curve segment between the keys around it)
float XsfCurveSet_unk::fn_805BF214(float time, Channel_unk* channel) {
    if (channel->mSize == 0) {
        return 0.0f;
    }
    if (time < channel->Begin_unk()->mValue.first) {
        return channel->Begin_unk()->mValue.second.mValue;
    }
    XsfKeyNode_unk* next = channel->End_unk();
    XsfKeyNode_unk* node = channel->mRoot;
    while (node != 0) {
        if (!channel->mComp(node->mValue.first, time)) {
            next = node;
            node = node->mLeft;
        } else {
            node = node->mRight;
        }
    }
    if (next == channel->End_unk()) {
        float value = 0.0f;
        for (XsfKeyNode_unk* p = channel->Begin_unk(); p != channel->End_unk(); XsfKeyNode_unk::Next_unk(p)) {
            value = p->mValue.second.mValue;
        }
        return value;
    }
    if (time == next->mValue.first) {
        return next->mValue.second.mValue;
    }
    XsfKeyIter_unk it(next);
    --it;
    XsfKeyNode_unk* prev = it.mNode;
    float t0 = prev->mValue.first;
    float t1 = next->mValue.first;
    float v0 = prev->mValue.second.mValue;
    float v1 = next->mValue.second.mValue;
    if (!prev->mValue.second.mCurve || !next->mValue.second.mCurve) {
        float r = (time - t0) / (t1 - t0);
        return r * (v1 - v0) + v0;
    }
    if (prev->mValue.second.mOutX == 0.0f && prev->mValue.second.mOutY == 0.0f) {
        return v0;
    }
    XsfCurvePoint_unk a(t0, v0, prev->mValue.second.mOutX, prev->mValue.second.mOutY);
    XsfCurvePoint_unk b(t1, v1, next->mValue.second.mInX, next->mValue.second.mInY);
    CurveSegment_unk seg;
    fn_80507790(&seg, time, &a.mX, &b.mX, 1);
    return fn_805077F0(&seg);
}

void XsfCurveSet_unk::fn_805BF410(float time, float* out, int x, int y, int z) {
    out[0] = fn_805BF214(time, &mChannels[x]);
    out[1] = fn_805BF214(time, &mChannels[y]);
    out[2] = fn_805BF214(time, &mChannels[z]);
}

__declspec(noinline) int XsfCurveSet_unk::fn_805BF4B4(int channel, float time, float v0, float v1, float v2, float v3, float v4) {
    return mChannels[channel].Insert_unk(XsfKeyPair_unk(time, XsfKey_unk(v0, v1, v2, v3, v4, true)));
}

__declspec(noinline) int XsfCurveSet_unk::fn_805BF590(int channel, float time, float value) {
    return mChannels[channel]
        .Insert_unk(XsfKeyPair_unk(time, XsfKey_unk(value, 0.0f, 0.0f, 0.0f, 0.0f, false)));
}

template <class T>
static inline const T& Min_unk(const T& a, const T& b) {
    return b < a ? b : a;
}
template <class T>
static inline const T& Max_unk(const T& a, const T& b) {
    return a < b ? b : a;
}

// inferred: time range of the keys of all the channels
// NONMATCHING: 79.2% - the original selects the std::min/std::max reference with a preloaded pointer
// (one arm), indexes the channels with i * 16 and checks the size twice; tried if/return, operand
// orders, MIN/MAX macros, a named reference and the inline fn_805BF1E8.
void XsfCurveSet_unk::fn_805BF660() {
    float start = 3.4028235e38f;
    float end = 1.1754944e-38f;
    for (int i = 0; i < 13; i++) {
        if (mChannels[i].mSize != 0) {
            if (mChannels[i].mSize == 0) {
            } else if (mChannels[i].mSize == 1) {
                XsfKeyNode_unk* first = mChannels[i].Begin_unk();
                start = Min_unk(first->mValue.first, start);
                end = Max_unk(first->mValue.first, end);
            } else {
                XsfKeyIter_unk last(mChannels[i].End_unk());
                XsfKeyNode_unk* first = mChannels[i].Begin_unk();
                --last;
                start = Min_unk(first->mValue.first, start);
                end = Max_unk(last.mNode->mValue.first, end);
            }
        }
    }
    mStart = start;
    mEnd = end;
    mLength = end - start;
}

template <class Node>
Node* XsfKeyTree_unk<Node>::FindInsertPos_unk(const float& key, Node*& pred, bool& left, bool& leftmost) {
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
Node* XsfKeyTree_unk<Node>::InsertNode_unk(Node* parent, bool left, bool leftmost, const typename Node::Value& value) {
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
