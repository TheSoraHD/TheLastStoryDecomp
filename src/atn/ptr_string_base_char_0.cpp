#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Array_unk.h"
#include "std/string.h"
#include "std/string_members.h"
#include "std/new.h"

// The real file reads /boot/voice_table.csv: per character id, 43 voice names and 8 parameters.

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

static const char sFileNameStr[] = "/boot/voice_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

namespace atn {

// inferred: pointer to a string owned elsewhere (see atn/ptr_string_base_wchar_t_0.cpp)
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    virtual ~ptr_string_base() {}
    const T* c_str() const { return mPtr != 0 ? mPtr : &sEmpty_unk; }

    T* mPtr;
    static const T sEmpty_unk; // inferred: the empty string is a 4-byte .sdata2 object (0x80882FEC)
};

template <class T, int N>
const T ptr_string_base<T, N>::sEmpty_unk = 0;

} // namespace atn

// inferred: one line of the voice table (0x17C bytes; names invented)
struct VoiceEntry_unk {
    const char* fn_8021FD7C(int kind) const;
    int fn_8021FFB0(int i) const;

    int mId;                                      // 0x000
    atn::ptr_string_base<char, 0> mNames[0x2B];   // 0x004
    int mParams[8];                               // 0x15C
};

extern "C" VoiceEntry_unk* lbl_8087F350; // inferred: entries
extern "C" int lbl_8087F354;             // inferred: number of entries
extern "C" char* lbl_8087F358;           // inferred: buffer with all the names

// inferred: balances the red-black tree after an insertion (MSL)
extern "C" void fn_8003E120(void* node, void* root);

// inferred: std::map of MSL C++ (layout {size, root, leftmost}; nodes {left, right, parent, key, value});
// only what this file uses, names invented
template <class K, class V>
class Map_unk {
public:
    struct Value {
        K first;
        V second;
    };
    struct Node {
        Node* mLeft;
        Node* mRight;
        Node* mParent;
        Value mValue;
    };
    struct InsertResult {
        Node* first;
        bool second;
    };

    Map_unk() : mSize(0), mRoot(0), mLeftmost(end()) {}
    ~Map_unk() {
        if (mRoot != 0) {
            Destroy(mRoot);
        }
    }
    Node* end() { return (Node*)&mRoot; }
    Node* find(const K& key) {
        Node* node = mRoot;
        Node* result = end();
        while (node != 0) {
            if (node->mValue.first < key) {
                node = node->mRight;
            } else {
                result = node;
                node = node->mLeft;
            }
        }
        if (result == end() || key < result->mValue.first) {
            result = end();
        }
        return result;
    }
    InsertResult insert(const Value& value) {
        InsertResult r;
        Node* pred;
        bool left;
        bool leftmost;
        Node* parent = FindInsertPos(value.first, pred, left, leftmost);
        if (pred == 0 || pred->mValue.first < value.first) {
            r.first = InsertNode(parent, left, leftmost, value);
            r.second = true;
        } else {
            r.first = pred;
            r.second = false;
        }
        return r;
    }
    void Destroy(Node* node);
    static Node* __allocate() {
        Node* p = (Node*)::operator new(sizeof(Node));
        if (p == 0) {
            __msl_error("Memory allocation failure");
        }
        return p;
    }
    // inferred: frees the node if the construction does not finish (names invented)
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
    Node* FindInsertPos(const K& key, Node*& pred, bool& left, bool& leftmost);
    Node* InsertNode(Node* parent, bool left, bool leftmost, const Value& value);

    unsigned long mSize;
    Node* mRoot;
    Node* mLeftmost;
};

// inferred: parses "xx1_2_3" into 123 (the first two characters are a prefix)
static int ParseId_unk(const char* name) {
    if (name == 0 || strlen(name) <= 2) {
        return -1;
    }
    std::string s(name + 2);
    for (;;) {
        std::string::size_type pos = s.find("_");
        if (pos == std::string::npos) {
            break;
        }
        s.erase(pos, 1);
    }
    return atoi(s.c_str());
}

// NONMATCHING: 72% - the map is a local reconstruction of the MSL std::map (inlined find/insert/destroy)
extern "C" void fn_8021F47C() {
    atn::Csv csv;
    if (lbl_8087F350 == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        Map_unk<unsigned int, const char*> names;
        atn::Vector_unk<unsigned int> seen;
        csv.vf08((const char*)data, size);
        int total = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (field[0] == ';' || field[0] == '#') {
                continue;
            }
            if (strcmp(field, "") != 0) {
                continue;
            }
            if (strcmp(csv.fn_8005B9CC(), "") == 0) {
                continue;
            }
            for (int i = 0; i < 0x2B; i++) {
                const char* name = csv.fn_8005B9CC();
                bool found = false;
                for (unsigned int j = 0; j < seen.mSize; j++) {
                    if (seen[j] == fn_800DC6B4(name)) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    total += strlen(name) + 1;
                }
            }
            lbl_8087F354++;
        }
        lbl_8087F350 = new (1, "", "", 0) VoiceEntry_unk[lbl_8087F354];
        lbl_8087F358 = new (1, "", "", 0) char[total];
        csv.vf08((const char*)data, size);
        int pos = 0;
        int index = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (field[0] == ';' || field[0] == '#') {
                continue;
            }
            if (strcmp(field, "") != 0) {
                continue;
            }
            const char* idName = csv.fn_8005B9CC();
            if (strcmp(idName, "") == 0) {
                continue;
            }
            VoiceEntry_unk* entry = &lbl_8087F350[index];
            entry->mId = ParseId_unk(idName);
            csv.fn_8005B9CC(); // inferred: a column that is not used
            for (int i = 0; i < 0x2B; i++) {
                const char* name = csv.fn_8005B9CC();
                unsigned int hash = fn_800DC6B4(name);
                Map_unk<unsigned int, const char*>::Node* it = names.find(hash);
                if (it == names.end()) {
                    sprintf(lbl_8087F358 + pos, "%s", name);
                    entry->mNames[i].mPtr = lbl_8087F358 + pos;
                    Map_unk<unsigned int, const char*>::Value value;
                    value.first = hash;
                    value.second = lbl_8087F358 + pos;
                    names.insert(value);
                    pos += strlen(name) + 1;
                } else {
                    entry->mNames[i].mPtr = (char*)it->mValue.second;
                }
            }
            for (int i = 0; i < 8; i++) {
                entry->mParams[i] = atoi(csv.fn_8005B9CC());
            }
            index++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// NONMATCHING: 99.7% - the string object and its length swap r30/r31 (with `inline` ParseId_unk the registers
// match but its "_" goes to a @STRING@ object instead of the string pool)
extern "C" VoiceEntry_unk* fn_8021FC00(const char* name) {
    int id = ParseId_unk(name);
    for (int i = 0; i < lbl_8087F354; i++) {
        if (id == lbl_8087F350[i].mId) {
            return &lbl_8087F350[i];
        }
    }
    return 0;
}

const char* VoiceEntry_unk::fn_8021FD7C(int kind) const {
    // note: written `0x2B <= kind`; with `kind >= 0x2B` MWCC merges both tests into one cmplwi
    if (kind < 0 || 0x2B <= kind) {
        return 0;
    }
    switch (kind) {
    case 0:
    case 1:
        kind = rand() % 2;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        kind = rand() % 4 + 2;
        break;
    case 6:
    case 7:
        kind = rand() % 2 + 6;
        break;
    case 8:
    case 9:
        kind = rand() % 2 + 8;
        break;
    case 10:
    case 11:
        kind = rand() % 2 + 10;
        break;
    case 12:
    case 13:
        kind = rand() % 2 + 12;
        break;
    case 19:
    case 20:
        kind = rand() % 2 + 19;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
        kind = rand() % 4 + 21;
        break;
    case 25:
    case 26:
        kind = rand() % 2 + 25;
        break;
    case 28:
    case 29:
        kind = rand() % 2 + 28;
        break;
    case 30:
    case 31:
        kind = rand() % 2 + 30;
        break;
    case 32:
    case 33:
    case 34:
    case 35:
        kind = rand() % 4 + 32;
        break;
    case 38:
    case 39:
        kind = rand() % 2 + 38;
        break;
    case 40:
    case 41:
        kind = rand() % 2 + 40;
        break;
    }
    if (strlen(mNames[kind].c_str()) == 0) {
        return 0;
    }
    return mNames[kind].c_str();
}

int VoiceEntry_unk::fn_8021FFB0(int i) const {
    if (i < 0 || 8 <= i) {
        return 0;
    }
    return mParams[i];
}

template <class K, class V>
void Map_unk<K, V>::Destroy(Node* node) {
    if (node->mLeft != 0) {
        Destroy(node->mLeft);
    }
    if (node->mRight != 0) {
        Destroy(node->mRight);
    }
    delete node;
}

// NONMATCHING: 99.99% - same code, but the original pool has one more "" before "tree::insert length error"
template <class K, class V>
typename Map_unk<K, V>::Node* Map_unk<K, V>::InsertNode(Node* parent, bool left, bool leftmost, const Value& value) {
    if (mSize == 0xFFFFFFFF) {
        __msl_error("tree::insert length error");
    }
    NodeHolder_unk holder(&mRoot, __allocate());
    ::new (&holder.mNode->mValue) Value(value);
    Node* node = holder.release();
    node->mRight = 0;
    node->mLeft = 0;
    ::new (&node->mParent) Node*(parent);
    if (left) {
        parent->mLeft = node;
    } else {
        parent->mRight = node;
    }
    mSize++;
    fn_8003E120(node, mRoot);
    if (leftmost) {
        mLeftmost = node;
    }
    return node;
}

template <class K, class V>
typename Map_unk<K, V>::Node* Map_unk<K, V>::FindInsertPos(const K& key, Node*& pred, bool& left, bool& leftmost) {
    pred = 0;
    Node* parent = end();
    Node* node = mRoot;
    left = true;
    leftmost = true;
    while (node != 0) {
        parent = node;
        if (key < node->mValue.first) {
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
