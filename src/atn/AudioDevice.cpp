// atn/AudioDevice.cpp: the sound engine of the game (atn::AudioDevice, singleton lbl_8087EE90).
// It owns the NW4R sound archive (DVD), its player, three sound heaps, the 3D sound manager, the
// effects of the AUX bus, the list of loaded banks (groups of the archive) and the sound id table
// read from "rsid.csv". Only the members used here are declared; the NW4R classes are not
// reconstructed (their functions keep the fn_ names).
#include "atn/Array_unk.h"
#include "atn/MemoryManager.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/msl_common.h"
#include "std/new.h"
#include <boost/function.hpp>

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" void OSSleepTicks(long long ticks);

void* operator new(unsigned long size, int heap, const char* file, const char* name, int line);
void* operator new[](unsigned long size, int heap, const char* file, const char* name, int line);

extern "C" void* fn_800839EC(atn::MemoryManager* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line, int flags);

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name
extern "C" int fn_800DC12C(const char* s);          // inferred: atoi

// inferred: per-frame callbacks (also run by atn::LoadHandle and main)
typedef atn::Vector_unk<boost::function<void()> > FrameCallbacks_unk;
extern "C" FrameCallbacks_unk* lbl_8087EE94;

// inferred: balances the red-black tree after an insertion (MSL)
extern "C" void fn_8003E120(void* node, void* root);

// inferred: std::map of MSL C++ (nodes {left, right, parent, value}); only what this file uses,
// names invented. Extra is the size of the word between the root and the leftmost node (the maps
// of 0x10 bytes have one).
template <class K, class V, int Extra>
class AudioMap_unk {
public:
    struct Value {
        Value() {}
        Value(const K& k, const V& v) : first(k), second(v) {}
        K first;
        V second;
    };
    struct Node {
        Node* mLeft;
        Node* mRight;
        Node* mParent;
        Value mValue;
    };

    AudioMap_unk() : mSize(0), mRoot(0), mLeftmost(end()) {}
    ~AudioMap_unk() {
        if (mRoot != 0) {
            Destroy(mRoot);
        }
    }
    Node* end() { return (Node*)&mRoot; }
    Node* find(const K& key) {
        Node* node = mRoot;
        Node* result = end();
        while (node != 0) {
            if (!(node->mValue.first < key)) {
                result = node;
                node = node->mLeft;
            } else {
                node = node->mRight;
            }
        }
        if (result == end() || key < result->mValue.first) {
            result = end();
        }
        return result;
    }
    void clear() {
        if (mRoot != 0) {
            Destroy(mRoot);
            mSize = 0;
            mRoot = 0;
            mLeftmost = end();
        }
    }
    // NONMATCHING: 9.31% (0x8004A664/0x8004A8FC/0x8004ABDC) - el original inlinea dos niveles de la
    // recursion y aqui salen tres (mismo problema que game/TownTask y atn/ptr_string_base_char_0)
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
    void insert(const Value& value) {
        Node* pred;
        bool leftmost;
        bool left;
        Node* parent = FindInsertPos(value.first, pred, left, leftmost);
        if (pred == 0 || pred->mValue.first < value.first) {
            InsertNode(parent, left, leftmost, value);
        }
    }
    V& operator[](const K& key) {
        Node* pred;
        bool left;
        bool leftmost;
        Node* parent = FindInsertPos(key, pred, left, leftmost);
        Value* v;
        if (pred == 0 || (v = &pred->mValue, v->first < key)) {
            v = &InsertNode(parent, left, leftmost, Value(key, V()))->mValue;
        }
        return v->second;
    }

    unsigned long mSize;
    Node* mRoot;
    unsigned char mExtra[Extra];
    Node* mLeftmost;
};

template <class K, class V, int Extra>
typename AudioMap_unk<K, V, Extra>::Node* AudioMap_unk<K, V, Extra>::FindInsertPos(const K& key, Node*& pred,
                                                                                   bool& left, bool& leftmost) {
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

template <class K, class V, int Extra>
typename AudioMap_unk<K, V, Extra>::Node* AudioMap_unk<K, V, Extra>::InsertNode(Node* parent, bool left,
                                                                                bool leftmost, const Value& value) {
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

template <class K, class V, int Extra>
void AudioMap_unk<K, V, Extra>::Destroy(Node* node) {
    if (node->mLeft != 0) {
        Destroy(node->mLeft);
    }
    if (node->mRight != 0) {
        Destroy(node->mRight);
    }
    delete node;
}

// inferred: the maps without the extra word
template <class K, class V>
class AudioMap_unk<K, V, 0> {
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

    AudioMap_unk() : mSize(0), mRoot(0), mLeftmost(end()) {}
    ~AudioMap_unk() {
        if (mRoot != 0) {
            Destroy(mRoot);
        }
    }
    Node* end() { return (Node*)&mRoot; }
    Node* find(const K& key) {
        Node* result = end();
        Node* node = mRoot;
        while (node != 0) {
            if (!(node->mValue.first < key)) {
                result = node;
                node = node->mLeft;
            } else {
                node = node->mRight;
            }
        }
        if (result == end() || key < result->mValue.first) {
            result = end();
        }
        return result;
    }
    void clear() {
        if (mRoot != 0) {
            Destroy(mRoot);
            mSize = 0;
            mRoot = 0;
            mLeftmost = end();
        }
    }
    void Destroy(Node* node);

    unsigned long mSize;
    Node* mRoot;
    Node* mLeftmost;
};

template <class K, class V>
void AudioMap_unk<K, V, 0>::Destroy(Node* node) {
    if (node->mLeft != 0) {
        Destroy(node->mLeft);
    }
    if (node->mRight != 0) {
        Destroy(node->mRight);
    }
    delete node;
}

// inferred: sound id of a name in rsid.csv (id and bank)
struct AudioSoundId_unk {
    int mId;
    int mBank;
};

// inferred: effect preset (0x28 bytes after the key; parameters of the reverb with their defaults)
struct AudioFxPreset_unk {
    AudioFxPreset_unk()
        : m00(0.02f), m08(0.6f), m0C(0.4f), m10(1.0f), m14(5), m18(0.02f), m20(0.0f), m24(1.0f) {}
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    int m14;
    float m18;
    int m1C;
    float m20;
    float m24;
};

// inferred: DVDFileInfo of the SDK (0x3C bytes, copied as a whole)
struct DVDFileInfo_unk {
    unsigned int m00[11];
    void* mUserData; // 0x2C: inferred: the bank being read (fn_80047DBC)
    unsigned int m30[3];
};

extern "C" void OSLockMutex(OSMutex_unk* mutex);
extern "C" void OSUnlockMutex(OSMutex_unk* mutex);

extern "C" int fn_8070DC00(void* frameHeap);           // inferred: FrameHeap::GetCurrentLevel
extern "C" unsigned long fn_8070DC10(void* frameHeap); // inferred: FrameHeap::GetFreeSize

// inferred: nw4r::ut::AutoMutexLock
struct AutoMutexLock_unk {
    AutoMutexLock_unk(OSMutex_unk& mutex) : mMutex(mutex) { OSLockMutex(&mMutex); }
    ~AutoMutexLock_unk() { OSUnlockMutex(&mMutex); }
    OSMutex_unk& mMutex;
};

namespace nw4r {
namespace ut {
namespace detail {
class LinkListImpl {
public:
    ~LinkListImpl();
    unsigned char m00[0xC];
};
} // namespace detail
} // namespace ut

namespace snd {

// The names of the classes are in the DOL (vtables 0x80796BF0 and 0x80796D38); the layouts are only sizes.
class DvdSoundArchive {
public:
    DvdSoundArchive();
    ~DvdSoundArchive();
    unsigned char m00[0x11C];
    unsigned long mLabelSize;  // 0x11C: inferred: size of the label string data
    unsigned long m120;
    unsigned long mHeaderSize; // 0x124: inferred: size of the header
    unsigned char m128[0x14C - 0x128];
    DVDFileInfo_unk mFileInfo; // 0x14C
    unsigned char m188[4];
};

class SoundArchivePlayer {
public:
    SoundArchivePlayer();
    ~SoundArchivePlayer();
    unsigned char m00[0x0C];
    unsigned char mStartable[0xEC - 0x0C]; // 0x0C: inferred: SoundStartable base
};

// inferred: nw4r::snd::SoundHeap (vtable, mutex, frame heap)
class SoundHeap {
public:
    SoundHeap();
    ~SoundHeap();
    virtual void vf08();
    virtual void* Alloc(unsigned long size); // 0x0C
    // inferred: inline members of the NW4R header
    int GetCurrentLevel() {
        AutoMutexLock_unk lock(mMutex);
        return fn_8070DC00(mFrameHeap);
    }
    unsigned long GetFreeSize() {
        AutoMutexLock_unk lock(mMutex);
        return fn_8070DC10(mFrameHeap);
    }
    bool IsValid() { return *(void**)mFrameHeap != 0; }

    OSMutex_unk mMutex;          // 0x04
    unsigned char mFrameHeap[0x10]; // 0x1C
};

} // namespace snd
} // namespace nw4r

extern "C" void fn_807177D0(nw4r::snd::SoundHeap* heap); // inferred: Clear
extern "C" void fn_80717750(nw4r::snd::SoundHeap* heap); // inferred: Destroy
extern "C" int fn_80717740(nw4r::snd::SoundHeap* heap, void* buffer, unsigned long size); // inferred: Create
extern "C" int fn_80717840(nw4r::snd::SoundHeap* heap); // inferred: SaveState
extern "C" void fn_807178A0(nw4r::snd::SoundHeap* heap, int level); // inferred: LoadState

extern "C" unsigned long fn_8059E314(void* archive, unsigned long id);       // inferred: label of an id
extern "C" unsigned long fn_8059E31C(void* archive, const char* label);    // inferred: group id of a label
extern "C" unsigned long fn_8059E32C(void* archive, const char* label);    // inferred: sound id of a label
extern "C" int fn_8059E344(void* archive, unsigned long id);               // inferred: sound type
extern "C" int fn_80718470(void* startable, void* handle, unsigned long id, void* info);
extern "C" int fn_807184D0(void* startable, void* handle, unsigned long id, void* info);

namespace atn {

class Sound3dEngine {
public:
    Sound3dEngine();
    virtual ~Sound3dEngine();
    float fn_800CB1F4(class AudioDevice* engine, const struct SoundCore* core);
    unsigned char m04[0x1C - 0x04];
};

// inferred: data path of the device (fn_80046EE0; names invented)
struct AudioPath_unk {
    AudioPath_unk() : m00(-1) {}
    int m00;
    std::string mPath;
};

// inferred: the NW4R effects of the AUX bus (their classes are not reconstructed: the constructors
// are called by address and the inline destructors set the vtable of the library and call the
// Shutdown/ReleaseWorkBuffer virtuals)
// inferred: virtual functions of nw4r::snd::FxBase used here (never constructed here)
class AudioFxBase_unk {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void Shutdown();
    virtual void vf14();
    virtual void vf18();
    virtual int AssignWorkBuffer(void* buf, unsigned long size);
    virtual void ReleaseWorkBuffer();
};

#define AUDIO_FX_UNK(Name, Size, Ctor, Vtable)                                                     \
    extern "C" void Ctor(void* fx);                                                                \
    extern "C" void* Vtable[];                                                                     \
    struct Name {                                                                                  \
        Name() { Ctor(this); }                                                                     \
        ~Name() {                                                                                  \
            *(void***)this = Vtable;                                                               \
            ((AudioFxBase_unk*)this)->Shutdown();                                                  \
            ((AudioFxBase_unk*)this)->ReleaseWorkBuffer();                                         \
        }                                                                                          \
        void AssignWorkBuffer(void* buf, unsigned long size) {                                     \
            ((AudioFxBase_unk*)this)->AssignWorkBuffer(buf, size);                                 \
        }                                                                                          \
        void ReleaseWorkBuffer() { ((AudioFxBase_unk*)this)->ReleaseWorkBuffer(); }                \
        unsigned char m00[Size];                                                                   \
    };
AUDIO_FX_UNK(AudioFxA_unk, 0x230, fn_8070E760, lbl_807C5F28)
AUDIO_FX_UNK(AudioFxB_unk, 0x184, fn_8070DC20, lbl_807C5ED8)
AUDIO_FX_UNK(AudioFxC_unk, 0xE8, fn_8070E190, lbl_807C5F00)

// inferred: nw4r::snd::Sound3DManager (constructor fn_80715D90; implicit destructor: only its list
// at 0x0C)
extern "C" void fn_80715D90(void* manager);
struct LinkList_unk : public nw4r::ut::detail::LinkListImpl {
    ~LinkList_unk() {}
};
struct Sound3DManager_unk {
    Sound3DManager_unk() { fn_80715D90(this); }
    unsigned char m00[0x0C];
    LinkList_unk mList;
    unsigned char m18[0x1C - 0x18];
    int mMaxPriorityReduction; // 0x1C
    unsigned char m20[0x2C - 0x20];
};

// inferred: one bank (group of the sound archive) loaded or being loaded (0x54 bytes, names invented)
struct AudioBank_unk {
    // inferred: weak copy 0x800465E8 (constructor of the arrays of banks)
    AudioBank_unk() : mId(0), mRefCount(1), mHeapState(0), mState(0), mBusy(0), mBuffer0(0), mBuffer1(0) {}

    char mName[0x20];
    unsigned int mId;  // 0x20: sound archive item id + 1
    int mRefCount;     // 0x24
    int mHeapState;    // 0x28: state of the sound heap when the bank was loaded
    int mState;        // 0x2C: 0 load header, 1 load data, 2 attach, 3 done, 4 error
    int mBusy;         // 0x30: a DVD read is running
    unsigned int mInfo[6]; // 0x34: group info of the archive (offsets and sizes)
    void* mBuffer0;    // 0x4C
    void* mBuffer1;    // 0x50
};

class AudioDevice {
public:
    AudioDevice();
    virtual ~AudioDevice();

    // inferred: sound id of a label (from rsid.csv when it is loaded), 0 when unknown
    unsigned int GetSoundId_unk(const char* name) {
        if (m8E0 != 0) {
            AudioMap_unk<unsigned int, AudioSoundId_unk, 4>::Node* it = m8D0.find(fn_800DC6B4(name));
            if (it != m8D0.end()) {
                return it->mValue.second.mId + 1;
            }
            return 0;
        }
        if (m04 != 0) {
            return fn_8059E32C(&mArchive, name) + 1;
        }
        return 0;
    }
    // inferred: index of the sound in the archive (name invented)
    unsigned int GetSoundIndex_unk(const char* name) { return GetSoundId_unk(name) - 1; }
    // inferred: names invented (fn_80049B74, fn_80049C3C, fn_80049CDC)
    unsigned int GetGroupId_unk(const char* name) {
        if (m8E0 != 0) {
            AudioMap_unk<unsigned int, AudioSoundId_unk, 4>::Node* it = m8D0.find(fn_800DC6B4(name));
            if (it != m8D0.end()) {
                return it->mValue.second.mId + 1;
            }
            return 0;
        }
        if (m04 != 0) {
            return fn_8059E31C(&mArchive, name) + 1;
        }
        return 0;
    }
    int GetBank_unk(const char* name) {
        if (m8E0 != 0) {
            AudioMap_unk<unsigned int, AudioSoundId_unk, 4>::Node* it = m8D0.find(fn_800DC6B4(name));
            if (it != m8D0.end()) {
                return it->mValue.second.mBank;
            }
        }
        return 0;
    }
    unsigned long GetLabel_unk(unsigned int id) {
        if (m8E0 != 0) {
            id--;
            AudioMap_unk<unsigned int, const char*, 0>::Node* it = m8E8.find(id);
            if (it != m8E8.end()) {
                return (unsigned long)it->mValue.second;
            }
            return 0;
        }
        if (m04 != 0) {
            return fn_8059E314(&mArchive, id - 1);
        }
        return 0;
    }
    // inferred: frees a buffer of the memory manager (name invented)
    static void Free_unk(void*& p) {
        if (p != 0) {
            atn::MemoryManager::Instance()->Free(p);
            p = 0;
        }
    }
    // inferred: the queued banks are loading (names invented)
    bool IsQueueLoading_unk() {
        for (AudioBank_unk* it = mBankQueue.mData; it != mBankQueue.mData + mBankQueue.mSize; it++) {
            if (it->mState != 3 && it->mState != 4) {
                return true;
            }
        }
        return false;
    }
    bool IsExtraLoading_unk() {
        if (m404 != 0 && mExtraBank.mState != 3 && mExtraBank.mState != 4) {
            return true;
        }
        return false;
    }
    void EraseBank_unk(AudioBank_unk* it) {
        if (it == mBanks.mData + mBanks.mSize) {
            return;
        }
        for (int i = it - mBanks.mData; i < mBanks.mSize - 1; i++) {
            mBanks.mData[i] = mBanks.mData[i + 1];
        }
        mBanks.mSize = mBanks.mSize - 1;
    }
    // inferred: unloads every bank (body of fn_80049654, inlined in fn_80047628)
    void UnloadBanks_unk() {
        if (m04 != 0) {
            mBanks.clear();
            fn_807178A0(&m300, 0);
        }
    }

    volatile int m04; // inferred: initialized (reloaded after each test)
    nw4r::snd::DvdSoundArchive mArchive; // 0x08
    void* m194; // inferred: header buffer of the archive
    void* m198; // inferred: label buffer of the archive
    DVDFileInfo_unk mBankFile;           // 0x19C: inferred: copy of the archive file (bank loads)
    nw4r::snd::SoundArchivePlayer mPlayer; // 0x1D8
    void* m2C4;
    void* m2C8;
    nw4r::snd::SoundHeap m2CC;           // inferred: heap of the bank queue
    void* m2F8;
    unsigned long m2FC;
    nw4r::snd::SoundHeap m300;           // inferred: heap of the banks
    void* m32C;
    nw4r::snd::SoundHeap m330;           // inferred: heap of the extra bank
    void* m35C;
    unsigned long m360;
    Sound3DManager_unk m364;
    void* m390;
    Vector_unk<AudioBank_unk> mBanks;      // 0x394
    Vector_unk<AudioBank_unk> mBankQueue;  // 0x3A0
    AudioBank_unk mExtraBank;              // 0x3AC
    int m400; // inferred: banks of the queue are being loaded
    int m404; // inferred: the extra bank is being loaded
    int m408; // inferred: the reverb is enabled
    unsigned int m40C;
    AudioFxA_unk mReverb;                  // 0x410
    AudioFxB_unk m640;
    AudioFxC_unk m7C4;
    AudioMap_unk<int, AudioFxPreset_unk, 4> m8AC; // inferred: effect presets
    int m8BC;
    int m8C0;
    void* m8C4;
    unsigned long m8C8;
    int m8CC;
    AudioMap_unk<unsigned int, AudioSoundId_unk, 4> m8D0; // inferred: sound ids (rsid.csv)
    int m8E0;
    char* m8E4;
    AudioMap_unk<unsigned int, const char*, 0> m8E8; // inferred: labels of the sound ids
    AudioMap_unk<unsigned int, const char*, 0> m8F4;
    Sound3dEngine m900;
    AudioPath_unk m91C;
};

} // namespace atn

using atn::AudioBank_unk;
using atn::AudioDevice;
using atn::Sound3DManager_unk;

extern "C" int lbl_8087EE80;
extern "C" int lbl_8087EE84;
extern "C" unsigned int* lbl_8087EE88;
extern "C" char* lbl_8087EE8C;
extern "C" AudioDevice* lbl_8087EE90;

int lbl_8087EE80;
int lbl_8087EE84;
unsigned int* lbl_8087EE88;
char* lbl_8087EE8C;
AudioDevice* lbl_8087EE90;

// inferred: callbacks of the remote speaker (empty)
extern "C" void fn_80046220(int result, unsigned long arg) {
}

extern "C" void fn_80046224(int result, unsigned long arg) {
}

// NONMATCHING: 91.96% - registros del bucle de callbacks (el original guarda un GPR mas; igual que LoadHandle)
// inferred: wait callback of the sound archive: runs the per-frame callbacks and sleeps 1 ms
extern "C" void fn_80046228() {
    FrameCallbacks_unk* callbacks = lbl_8087EE94;
    if (callbacks != 0) {
        for (unsigned int i = 0; i < callbacks->mSize; i++) {
            callbacks->mData[i]();
        }
    }
    OSSleepTicks((*(unsigned long*)0x800000F8 / 4) / 1000);
}

// inferred: error callback of the sound archive
extern "C" void fn_80046398(int error) {
    lbl_8087EE80 = 1;
    lbl_8087EE84 = error;
}

// inferred: creates the device
extern "C" void fn_800463A8() {
    if (lbl_8087EE90 == 0) {
        lbl_8087EE90 = new (10, "", "", 0) AudioDevice;
    }
}

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void fn_80047628(AudioDevice* audio);
extern "C" void fn_80047B54(AudioDevice* audio, unsigned long size);
extern "C" void fn_8004A1D4(AudioDevice* audio, int preset);
extern "C" void fn_8004A3FC(AudioDevice* audio, const char* path);

// inferred: allocation of the device buffers (aligned to 32, heap 10)
static inline void* AudioAlloc_unk(unsigned long size) {
    return fn_800839EC(atn::MemoryManager::Instance(), size, 0x20, 10, "", "", 0, 0);
}

AudioDevice::AudioDevice()
    : m04(0), m194(0), m198(0), m2C4(0), m2C8(0), m2F8(0), m2FC(0), m32C(0), m35C(0), m360(0x1E000), m390(0),
      m404(0), m408(0), m40C(0), m8BC(0), m8C0(0), m8C4(0), m8C8(0), m8CC(1), m8E0(0), m8E4(0) {
    lbl_8087EE88 = new (10, "", "", 0) unsigned int[0x200];
    memset(lbl_8087EE88, 0, 0x800);
    lbl_8087EE8C = new (10, "", "", 0) char[0x100];
    memset(lbl_8087EE8C, 0, 0x100);
    sprintf(lbl_8087EE8C, "stream");
}

// NONMATCHING: 29.45% - el MWCC de aqui inlinea un nivel mas la recursion de Destroy de los mapas y saca de
// linea los destructores de los miembros (el original lo inlinea todo)
AudioDevice::~AudioDevice() {
    fn_80047628(this);
    delete[] lbl_8087EE88;
    lbl_8087EE88 = 0;
    delete[] lbl_8087EE8C;
    lbl_8087EE8C = 0;
}

extern "C" int fn_8006BA6C();                          // inferred: current language
extern "C" void fn_8006A9A4(std::string& path);        // inferred: path normalisation
extern "C" std::string fn_8006B53C(const std::string& path);
extern "C" std::string fn_8006B594(const std::string& path);
extern "C" int fn_8006AA20(const char* path);          // inferred: the file exists
extern "C" void fn_8006AD24(std::string& path, const std::string& name); // inferred: replaces the file name
extern "C" void AIInit(void* stack);
extern "C" void AXInit();
extern "C" void fn_807184F0(int priority, int dvdPriority); // inferred: SoundSystem::InitSoundSystem
extern "C" int fn_8059D8B0(nw4r::snd::DvdSoundArchive* archive, const char* path); // inferred: Open
extern "C" int fn_8059DD48(nw4r::snd::DvdSoundArchive* archive, void* buffer, unsigned long size);
extern "C" int fn_8059DE64(nw4r::snd::DvdSoundArchive* archive, void* buffer, unsigned long size);
extern "C" unsigned long fn_8059EF90(nw4r::snd::SoundArchivePlayer* player, nw4r::snd::DvdSoundArchive* archive);
extern "C" unsigned long fn_8059F0FC(nw4r::snd::SoundArchivePlayer* player, nw4r::snd::DvdSoundArchive* archive);
extern "C" int fn_8059EBD4(nw4r::snd::SoundArchivePlayer* player, nw4r::snd::DvdSoundArchive* archive, void* buf,
                           unsigned long size, void* strmBuf, unsigned long strmSize);
extern "C" unsigned long fn_80715DF0(Sound3DManager_unk* manager, nw4r::snd::DvdSoundArchive* archive);
extern "C" int fn_80715E50(Sound3DManager_unk* manager, nw4r::snd::DvdSoundArchive* archive, void* buf,
                           unsigned long size);
extern "C" void fn_80715E90(Sound3DManager_unk* manager, atn::Sound3dEngine* engine);
extern "C" unsigned char SCGetSoundMode();
extern "C" void* GetInstance__Q44nw4r3snd6detail9AxManagerFv();
extern "C" int GetOutputMode__Q44nw4r3snd6detail9AxManagerFv(void* mgr);
extern "C" void SetOutputMode__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd10OutputMode(void* mgr, int mode);
extern "C" void SetMasterVolume__Q44nw4r3snd6detail9AxManagerFfi(void* mgr, float volume, int frames);

// inferred: initializes the device with the sound archive `path`
extern "C" int fn_80046EE0(AudioDevice* audio, const char* path) {
    if (audio->m04 != 0) {
        fn_80047628(audio);
    }
    audio->m91C.m00 = fn_8006BA6C();
    audio->m91C.mPath = path;
    AIInit(0);
    AXInit();
    fn_807184F0(4, 3);
    std::string file(path);
    fn_8006A9A4(file);
    file = fn_8006B53C(file);
    if (fn_8006BA6C() == 0) {
        file = fn_8006B594(file);
    }
    if (fn_8006AA20(file.c_str()) == 0) {
        file = fn_8006B594(file);
        if (fn_8006AA20(file.c_str()) == 0) {
            return 0;
        }
    }
    std::string ids(file);
    fn_8006AD24(ids, std::string("rsid.csv"));
    if (fn_8006AA20(ids.c_str()) != 0) {
        fn_8004A3FC(audio, ids.c_str());
    }
    fn_8059D8B0(&audio->mArchive, file.c_str());
    unsigned long size = audio->mArchive.mHeaderSize;
    audio->m194 = AudioAlloc_unk(size);
    fn_8059DD48(&audio->mArchive, audio->m194, size);
    audio->mBankFile = audio->mArchive.mFileInfo;
    if (audio->m8E0 == 0) {
        unsigned long labelSize = audio->mArchive.mLabelSize;
        audio->m198 = AudioAlloc_unk(labelSize);
        fn_8059DE64(&audio->mArchive, audio->m198, labelSize);
    }
    unsigned long playerSize = fn_8059EF90(&audio->mPlayer, &audio->mArchive);
    unsigned long strmSize = fn_8059F0FC(&audio->mPlayer, &audio->mArchive) * 2;
    audio->m2C4 = AudioAlloc_unk(playerSize);
    audio->m2C8 = AudioAlloc_unk(strmSize);
    memset(audio->m2C4, 0, playerSize);
    memset(audio->m2C8, 0, strmSize);
    fn_8059EBD4(&audio->mPlayer, &audio->mArchive, audio->m2C4, playerSize, audio->m2C8, strmSize);
    unsigned long size3d = fn_80715DF0(&audio->m364, &audio->mArchive);
    audio->m390 = AudioAlloc_unk(size3d);
    fn_80715E50(&audio->m364, &audio->mArchive, audio->m390, size3d);
    audio->m364.mMaxPriorityReduction = 0x20;
    fn_80715E90(&audio->m364, &audio->m900);
    fn_80047B54(audio, 0x2D5000);
    unsigned char mode = SCGetSoundMode();
    if (mode == 0) {
        SetOutputMode__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd10OutputMode(GetInstance__Q44nw4r3snd6detail9AxManagerFv(), 3);
    } else if (mode == 2) {
        SetOutputMode__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd10OutputMode(GetInstance__Q44nw4r3snd6detail9AxManagerFv(), 2);
    } else {
        SetOutputMode__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd10OutputMode(GetInstance__Q44nw4r3snd6detail9AxManagerFv(), 0);
    }
    audio->m04 = 1;
    return 1;
}

extern "C" void fn_8059EDE4(nw4r::snd::SoundArchivePlayer* player); // inferred: Shutdown
extern "C" void fn_8059DA40(nw4r::snd::DvdSoundArchive* archive);   // inferred: Close
extern "C" void ShutdownSoundSystem__Q34nw4r3snd11SoundSystemFv();

// NONMATCHING: 24.03% - misma recursion de Destroy de los mapas (un nivel mas de inline)
// inferred: shuts the device down
extern "C" void fn_80047628(AudioDevice* audio) {
    audio->UnloadBanks_unk();
    fn_80047B54(audio, 0);
    fn_8059EDE4(&audio->mPlayer);
    fn_8059DA40(&audio->mArchive);
    AudioDevice::Free_unk(audio->m194);
    AudioDevice::Free_unk(audio->m198);
    AudioDevice::Free_unk(audio->m2C4);
    AudioDevice::Free_unk(audio->m2C8);
    AudioDevice::Free_unk(audio->m390);
    audio->m8D0.clear();
    audio->m8E0 = 0;
    audio->m8E8.clear();
    audio->m8F4.clear();
    if (audio->m8E4 != 0) {
        delete[] audio->m8E4;
        audio->m8E4 = 0;
    }
    ShutdownSoundSystem__Q34nw4r3snd11SoundSystemFv();
    audio->m04 = 0;
}

// inferred: sets the size of the bank heap
extern "C" void fn_80047B54(AudioDevice* audio, unsigned long size) {
    ((void)0); // inferred: release asserts (the original calls this function out of line)
    ((void)0);
    ((void)0);
    ((void)0);
    if (audio->m04 == 0) {
        return;
    }
    if (audio->m300.IsValid()) {
        if (size == audio->m300.GetFreeSize()) {
            return;
        }
        fn_807177D0(&audio->m300);
        fn_80717750(&audio->m300);
        atn::MemoryManager::Instance()->Free(audio->m32C);
        audio->m32C = 0;
        audio->mBanks.clear();
    }
    if (size != 0) {
        audio->m32C = AudioAlloc_unk(size);
        memset(audio->m32C, 0, size);
        fn_80717740(&audio->m300, audio->m32C, size);
    }
}

extern "C" void* GetInstance__Q44nw4r3snd6detail20RemoteSpeakerManagerFv();
extern "C" void* GetRemoteSpeaker__Q44nw4r3snd6detail20RemoteSpeakerManagerFi(void* mgr, int i);
extern "C" void fn_80710A80(void* speaker, void (*callback)(int, unsigned long)); // inferred: RemoteSpeaker::Setup
extern "C" void fn_80710B40(void* speaker, void (*callback)(int, unsigned long)); // inferred: RemoteSpeaker::Shutdown

// inferred: starts the reverb and the remote speaker
extern "C" void fn_80047C74(AudioDevice* audio) {
    if (audio->m04 == 0) {
        return;
    }
    if (audio->m408 != 0) {
        audio->m8C4 = AudioAlloc_unk(audio->m8C8);
        memset(audio->m8C4, 0, audio->m8C8);
        audio->mReverb.AssignWorkBuffer(audio->m8C4, audio->m8C8);
    }
    fn_80710A80(GetRemoteSpeaker__Q44nw4r3snd6detail20RemoteSpeakerManagerFi(
                    GetInstance__Q44nw4r3snd6detail20RemoteSpeakerManagerFv(), 0),
                fn_80046220);
}

// inferred: stops them
extern "C" void fn_80047D20(AudioDevice* audio) {
    if (audio->m04 == 0) {
        return;
    }
    if (audio->m408 != 0) {
        fn_8004A1D4(audio, 0);
        audio->mReverb.ReleaseWorkBuffer();
    }
    AudioDevice::Free_unk(audio->m8C4);
    fn_80710B40(GetRemoteSpeaker__Q44nw4r3snd6detail20RemoteSpeakerManagerFi(
                    GetInstance__Q44nw4r3snd6detail20RemoteSpeakerManagerFv(), 0),
                fn_80046224);
}

// inferred: DVD read callback of the banks
extern "C" void fn_80047DBC(long result, DVDFileInfo_unk* fileInfo) {
    ((AudioBank_unk*)fileInfo->mUserData)->mBusy = 0;
}

extern "C" int DVDReadAsyncPrio(DVDFileInfo_unk* file, void* addr, long length, long offset,
                                void (*callback)(long, DVDFileInfo_unk*), int prio);
extern "C" void fn_8059F874(nw4r::snd::SoundArchivePlayer* player); // inferred: Update
extern "C" int fn_805A1C00(nw4r::snd::SoundArchivePlayer* player, unsigned long id, void* buf0, void* buf1);

// inferred: debug message list (atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

// inferred: updates the device and the loads of the banks
extern "C" void fn_80047DCC(AudioDevice* audio) {
    if (audio->m04 == 0) {
        return;
    }
    fn_8059F874(&audio->mPlayer);
    if (audio->m400 != 0) {
        for (AudioBank_unk* bank = audio->mBankQueue.mData;
             bank != audio->mBankQueue.mData + audio->mBankQueue.mSize; bank++) {
            if (bank->mBusy != 0) {
                break;
            }
            if (bank->mState == 0) {
                bank->mState = 1;
                bank->mBusy = 1;
                audio->mBankFile.mUserData = bank;
                if (bank->mBuffer0 == 0 ||
                    DVDReadAsyncPrio(&audio->mBankFile, bank->mBuffer0, bank->mInfo[3], bank->mInfo[2],
                                     fn_80047DBC, 2) == 0) {
                    bank->mState = 4;
                    bank->mBusy = 0;
                }
                break;
            } else if (bank->mState == 1) {
                bank->mState = 2;
                bank->mBusy = 1;
                audio->mBankFile.mUserData = bank;
                if (bank->mBuffer1 == 0 ||
                    DVDReadAsyncPrio(&audio->mBankFile, bank->mBuffer1, bank->mInfo[5], bank->mInfo[4],
                                     fn_80047DBC, 2) == 0) {
                    bank->mState = 4;
                    bank->mBusy = 0;
                }
                break;
            } else if (bank->mState == 2) {
                bank->mState = 3;
                if (fn_805A1C00(&audio->mPlayer, bank->mId - 1, bank->mBuffer0, bank->mBuffer1) == 0) {
                    bank->mState = 4;
                }
            } else if (bank->mState == 4) {
                char buf[0x100];
                sprintf(buf, "SoundError: LoadError Bank[%s]\0", bank);
                if (lbl_8087EEB8 != 0) {
                    lbl_8087EEB8->fn_800697D8(buf);
                }
                bank->mState = 3;
            }
        }
        audio->m400 = 0;
        for (AudioBank_unk* bank = audio->mBankQueue.mData;
             bank != audio->mBankQueue.mData + audio->mBankQueue.mSize; bank++) {
            if (bank->mState != 3 && bank->mState != 4) {
                audio->m400 = 1;
                return;
            }
        }
    } else if (audio->m404 != 0) {
        AudioBank_unk* bank = &audio->mExtraBank;
        if (bank->mBusy != 0) {
            return;
        }
        if (bank->mState == 0) {
            bank->mState = 1;
            bank->mBusy = 1;
            audio->mBankFile.mUserData = bank;
            if (bank->mBuffer0 == 0 ||
                DVDReadAsyncPrio(&audio->mBankFile, bank->mBuffer0, bank->mInfo[3], bank->mInfo[2], fn_80047DBC,
                                 2) == 0) {
                bank->mState = 4;
                bank->mBusy = 0;
            }
        } else if (bank->mState == 1) {
            bank->mState = 2;
            bank->mBusy = 1;
            audio->mBankFile.mUserData = bank;
            if (bank->mBuffer1 == 0 ||
                DVDReadAsyncPrio(&audio->mBankFile, bank->mBuffer1, bank->mInfo[5], bank->mInfo[4], fn_80047DBC,
                                 2) == 0) {
                bank->mState = 4;
                bank->mBusy = 0;
            }
        } else if (bank->mState == 2) {
            bank->mState = 3;
            if (fn_805A1C00(&audio->mPlayer, bank->mId - 1, bank->mBuffer0, bank->mBuffer1) == 0) {
                bank->mState = 4;
            }
            audio->m404 = 0;
        } else if (bank->mState == 4) {
            char buf[0x100];
            sprintf(buf, "SoundError: LoadError Extra Bank[%s]\0", bank);
            if (lbl_8087EEB8 != 0) {
                lbl_8087EEB8->fn_800697D8(buf);
            }
            bank->mState = 3;
            audio->m404 = 0;
        }
    } else {
        for (AudioBank_unk* bank = audio->mBanks.mData; bank != audio->mBanks.mData + audio->mBanks.mSize;
             bank++) {
            if (bank->mBusy != 0) {
                return;
            }
            if (bank->mState == 0) {
                bank->mState = 1;
                bank->mBusy = 1;
                audio->mBankFile.mUserData = bank;
                if (bank->mBuffer0 == 0 ||
                    DVDReadAsyncPrio(&audio->mBankFile, bank->mBuffer0, bank->mInfo[3], bank->mInfo[2],
                                     fn_80047DBC, 2) == 0) {
                    bank->mState = 4;
                    bank->mBusy = 0;
                }
                return;
            } else if (bank->mState == 1) {
                bank->mState = 2;
                bank->mBusy = 1;
                audio->mBankFile.mUserData = bank;
                if (bank->mBuffer1 == 0 ||
                    DVDReadAsyncPrio(&audio->mBankFile, bank->mBuffer1, bank->mInfo[5], bank->mInfo[4],
                                     fn_80047DBC, 2) == 0) {
                    bank->mState = 4;
                    bank->mBusy = 0;
                }
                return;
            } else if (bank->mState == 2) {
                bank->mState = 3;
                if (fn_805A1C00(&audio->mPlayer, bank->mId - 1, bank->mBuffer0, bank->mBuffer1) == 0) {
                    bank->mState = 4;
                }
            } else if (bank->mState == 4) {
                char buf[0x100];
                sprintf(buf, "SoundError: LoadError Bank[%s]\0", bank);
                if (lbl_8087EEB8 != 0) {
                    lbl_8087EEB8->fn_800697D8(buf);
                }
                bank->mState = 3;
            }
        }
    }
}

extern "C" void fn_8059E38C(nw4r::snd::DvdSoundArchive* archive, unsigned long id, unsigned int* info); // inferred: ReadGroupInfo

// NONMATCHING: 99.98% - orden de operandos de un add (m2FC + info[5])
// inferred: loads the banks `names` into the queue heap
extern "C" int fn_8004829C(AudioDevice* audio, const char** names, unsigned int n) {
    if (audio->m2F8 == 0) {
        audio->m2FC = 0;
        for (unsigned int i = 0; i < n; i++) {
            const char* name = names[i];
            if (name != 0 && strlen(name) != 0) {
                unsigned int info[6];
                fn_8059E38C(&audio->mArchive, audio->GetSoundId_unk(name) - 1, info);
                audio->m2FC += info[3];
                audio->m2FC = info[5] + audio->m2FC + 0x100;
            }
        }
        audio->m2F8 = AudioAlloc_unk(audio->m2FC);
        memset(audio->m2F8, 0, audio->m2FC);
    }
    if (!audio->m2CC.IsValid()) {
        fn_80717740(&audio->m2CC, audio->m2F8, audio->m2FC);
    }
    for (unsigned int i = 0; i < n; i++) {
        const char* name = names[i];
        if (name != 0 && strlen(name) != 0) {
            unsigned int id = audio->GetSoundId_unk(name);
            AudioBank_unk bank;
            sprintf(bank.mName, "%s\0", names[i]);
            bank.mId = id;
            fn_8059E38C(&audio->mArchive, id - 1, bank.mInfo);
            bank.mBuffer0 = audio->m2CC.Alloc(bank.mInfo[3]);
            bank.mBuffer1 = audio->m2CC.Alloc(bank.mInfo[5]);
            fn_80717840(&audio->m2CC);
            bank.mHeapState = audio->m2CC.GetCurrentLevel();
            audio->mBankQueue.push_back(bank);
            audio->m400 = 1;
        }
    }
    return 1;
}

// inferred: unloads the bank queue
extern "C" void fn_8004895C(AudioDevice* audio) {
    if (audio->m04 == 0) {
        return;
    }
    if (audio->m2CC.IsValid()) {
        fn_807177D0(&audio->m2CC);
        fn_80717750(&audio->m2CC);
        atn::MemoryManager::Instance()->Free(audio->m2F8);
        audio->m2F8 = 0;
    }
    audio->mBankQueue.clear();
    audio->m400 = 0;
}

// inferred: loads a bank (reference counted)
extern "C" int fn_800489F4(AudioDevice* audio, const char* name) {
    if (audio->m04 == 0) {
        return 1;
    }
    if (name == 0 || strlen(name) == 0) {
        return 0;
    }
    unsigned int id = audio->GetSoundId_unk(name);
    for (AudioBank_unk* it = audio->mBanks.mData; it != audio->mBanks.mData + audio->mBanks.mSize; it++) {
        if (id == it->mId) {
            it->mRefCount++;
            return 1;
        }
    }
    AudioBank_unk bank;
    sprintf(bank.mName, "%s\0", name);
    bank.mId = id;
    fn_8059E38C(&audio->mArchive, id - 1, bank.mInfo);
    bank.mBuffer0 = audio->m300.Alloc(bank.mInfo[3]);
    bank.mBuffer1 = audio->m300.Alloc(bank.mInfo[5]);
    fn_80717840(&audio->m300);
    bank.mHeapState = audio->m300.GetCurrentLevel();
    audio->mBanks.push_back(bank);
    return 1;
}

// NONMATCHING: 92.06% - registros (name/this) y el bool de IsExtraLoading_unk materializado en el original
// inferred: a bank (or any bank when name is 0) is still loading
extern "C" int fn_80048F8C(AudioDevice* audio, const char* name) {
    if (audio->m04 == 0) {
        return 0;
    }
    if (name == 0) {
        for (AudioBank_unk* it = audio->mBanks.mData; it != audio->mBanks.mData + audio->mBanks.mSize; it++) {
            if (it->mState != 3 && it->mState != 4) {
                return 1;
            }
        }
        if (audio->m400 != 0 && audio->IsQueueLoading_unk()) {
            return 1;
        }
        if (audio->m404 != 0 && audio->IsExtraLoading_unk()) {
            return 1;
        }
        return 0;
    }
    if (lbl_8087EE90->GetSoundIndex_unk(name) == 0xFFFFFFFF) {
        return 0;
    }
    unsigned int id = audio->GetSoundId_unk(name);
    for (AudioBank_unk* it = audio->mBanks.mData; it != audio->mBanks.mData + audio->mBanks.mSize; it++) {
        if (id == it->mId && it->mState != 3 && it->mState != 4) {
            return 1;
        }
    }
    return 0;
}

// inferred: the bank is loaded
extern "C" int fn_80049268(AudioDevice* audio, const char* name) {
    if (audio->m04 == 0) {
        return 1;
    }
    unsigned int id = audio->GetSoundId_unk(name);
    for (AudioBank_unk* it = audio->mBanks.mData; it != audio->mBanks.mData + audio->mBanks.mSize; it++) {
        if (id == it->mId) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.40% - forma de salto de la comprobacion del heap (bne + b en el original)
// inferred: releases a bank; the heap is rolled back to the highest state still in use
extern "C" void fn_8004937C(AudioDevice* audio, const char* name) {
    if (audio->m04 == 0) {
        return;
    }
    if (!audio->m300.IsValid()) {
        return;
    }
    if (name == 0 || strlen(name) == 0) {
        return;
    }
    unsigned int id = audio->GetSoundId_unk(name);
    AudioBank_unk* it = audio->mBanks.mData;
    while (it != audio->mBanks.mData + audio->mBanks.mSize) {
        if (id == it->mId) {
            if (--it->mRefCount <= 0) {
                audio->EraseBank_unk(it);
            } else {
                it++;
            }
        } else {
            it++;
        }
    }
    int level = 0;
    for (it = audio->mBanks.mData; it != audio->mBanks.mData + audio->mBanks.mSize; it++) {
        if (level < it->mHeapState) {
            level = it->mHeapState;
        }
    }
    if (level < audio->m300.GetCurrentLevel()) {
        fn_807178A0(&audio->m300, level);
    }
}

// inferred: unloads every bank
extern "C" void fn_80049654(AudioDevice* audio) {
    audio->UnloadBanks_unk();
}

// inferred: loads the extra bank
extern "C" int fn_800496C0(AudioDevice* audio, const char* name) {
    if (name == 0 || strlen(name) == 0) {
        return 0;
    }
    unsigned int id = audio->GetSoundId_unk(name);
    if (audio->mExtraBank.mId == id) {
        return 1;
    }
    audio->m35C = AudioAlloc_unk(audio->m360);
    memset(audio->m35C, 0, audio->m360);
    fn_80717740(&audio->m330, audio->m35C, audio->m360);
    AudioBank_unk* bank = &audio->mExtraBank;
    sprintf(bank->mName, "%s\0", name);
    bank->mId = id;
    fn_8059E38C(&audio->mArchive, id - 1, bank->mInfo);
    bank->mBuffer0 = audio->m330.Alloc(bank->mInfo[3]);
    bank->mBuffer1 = audio->m330.Alloc(bank->mInfo[5]);
    bank->mHeapState = audio->m300.GetCurrentLevel(); // inferred: the original reads the bank heap here
    audio->m404 = 1;
    return 1;
}

// inferred: the extra bank is loading
extern "C" int fn_800498A4(AudioDevice* audio) {
    return audio->IsExtraLoading_unk();
}

// inferred: unloads the extra bank
extern "C" void fn_800498D4(AudioDevice* audio) {
    if (audio->m04 == 0) {
        return;
    }
    if (audio->m330.IsValid()) {
        fn_807177D0(&audio->m330);
        fn_80717750(&audio->m330);
        atn::MemoryManager::Instance()->Free(audio->m35C);
        audio->m35C = 0;
    }
    audio->mExtraBank = AudioBank_unk();
    audio->m404 = 0;
}

// inferred: output mode of the AX manager
extern "C" int fn_800499D0(AudioDevice* audio) {
    return GetOutputMode__Q44nw4r3snd6detail9AxManagerFv(GetInstance__Q44nw4r3snd6detail9AxManagerFv());
}

extern "C" void fn_800499F4(AudioDevice* audio, int mode) {
    SetOutputMode__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd10OutputMode(GetInstance__Q44nw4r3snd6detail9AxManagerFv(),
                                                                       mode);
}

// inferred: AX manager fields (master volume as a MoveValue: start 0x1C, target 0x20, frame 0x24/0x28)
struct AxManager_unk {
    unsigned char m00[0x1C];
    float mStart;
    float mTarget;
    int mFrames;
    int mFrame;
};

// inferred: current master volume
extern "C" float fn_80049A28(AudioDevice* audio) {
    AxManager_unk* ax = (AxManager_unk*)GetInstance__Q44nw4r3snd6detail9AxManagerFv();
    if (ax->mFrame >= ax->mFrames) {
        return ax->mTarget;
    }
    return ax->mStart + (float)ax->mFrame * (ax->mTarget - ax->mStart) / (float)ax->mFrames;
}

extern "C" void fn_80049AAC(AudioDevice* audio, float volume) {
    SetMasterVolume__Q44nw4r3snd6detail9AxManagerFfi(GetInstance__Q44nw4r3snd6detail9AxManagerFv(), volume, 0);
}

// inferred: starts a sound (the handle and the start info are passed through)
extern "C" int fn_80049AE4(AudioDevice* audio, void* handle, unsigned long id, void* info) {
    if (audio->m04 == 0) {
        return 0;
    }
    return fn_80718470(audio->mPlayer.mStartable, handle, id - 1, info);
}

extern "C" int fn_80049B08(AudioDevice* audio, void* handle, unsigned long id, void* info) {
    if (audio->m04 == 0) {
        return 0;
    }
    return fn_807184D0(audio->mPlayer.mStartable, handle, id - 1, info);
}

// inferred: the sound is a stream
extern "C" int fn_80049B2C(AudioDevice* audio, unsigned long id) {
    if (audio->m04 != 0) {
        return fn_8059E344(&audio->mArchive, id - 1) == 2;
    }
    return 0;
}

// inferred: group id of a label
extern "C" unsigned int fn_80049B74(AudioDevice* audio, const char* name) {
    return audio->GetGroupId_unk(name);
}

// inferred: bank of a label (rsid.csv)
extern "C" int fn_80049C3C(AudioDevice* audio, const char* name) {
    return audio->GetBank_unk(name);
}

// NONMATCHING: 95.08% - registros de la clave (id - 1) en la busqueda del mapa
// inferred: label of a sound id
extern "C" unsigned long fn_80049CDC(AudioDevice* audio, unsigned int id) {
    return audio->GetLabel_unk(id);
}

extern "C" unsigned int fn_80049D74(AudioDevice* audio, const char* name) {
    return audio->GetSoundId_unk(name);
}

extern "C" int fn_80049E3C(AudioDevice* audio) {
    return 1;
}

extern "C" float fn_80049E44(AudioDevice* audio, const atn::SoundCore* core) {
    return audio->m900.fn_800CB1F4(audio, core);
}

// inferred: one row of /boot/reverb_preset_param.csv (db/ReverbPresetParam_unk.cpp)
struct ReverbPreset_unk {
    int mId;
    int mType;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
};

extern "C" void fn_8070EA60(void* fx, const AudioFxPreset_unk* param); // inferred: FxReverbHi::SetParam
extern "C" unsigned long fn_8070E830(void* fx);                        // inferred: GetRequiredMemSize

#define AUDIO_MAX_UNK(a, b) ((a) > (b) ? (a) : (b))
#define AUDIO_MIN_UNK(a, b) ((a) < (b) ? (a) : (b))

// NONMATCHING: 99.75% - registros del clamp del numero de reflexiones
// inferred: adds a reverb preset
extern "C" void fn_80049E54(AudioDevice* audio, const ReverbPreset_unk* preset) {
    AudioFxPreset_unk param;
    param.m1C = preset->mType;
    param.m04 = preset->m08;
    int n = AUDIO_MIN_UNK(AUDIO_MAX_UNK((int)preset->m0C, 5), 40);
    param.m0C = preset->m10;
    param.m18 = 0.05f;
    param.m14 = n / 5 - 1;
    param.m00 = AUDIO_MIN_UNK(preset->m14, 0.05f);
    param.m08 = preset->m18;
    param.m20 = preset->m1C;
    param.m24 = preset->m20;
    param.m10 = 0.5f;
    AudioMap_unk<int, AudioFxPreset_unk, 4>::Value value;
    value.first = preset->mId;
    value.second = param;
    audio->m8AC.insert(value);
    fn_8070EA60(&audio->mReverb, &param);
    unsigned long size = fn_8070E830(&audio->mReverb);
    if (audio->m8C8 < size) {
        audio->m8C8 = size;
    }
}

// NONMATCHING: 16.21% - misma recursion de Destroy de los mapas
// inferred: removes the reverb presets
extern "C" void fn_8004A03C(AudioDevice* audio) {
    fn_8004A1D4(audio, 0);
    audio->m8AC.clear();
    audio->m8C8 = 0;
    audio->m8BC = 0;
    audio->m8C0 = 0;
}

extern "C" void ClearEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusi(void* mgr, int bus, int frames);
extern "C" int AppendEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusPQ34nw4r3snd6FxBase(void* mgr, int bus,
                                                                                               void* fx);

// NONMATCHING: 98.79% - registros nodo/resultado en la busqueda del mapa
// inferred: selects the reverb preset (0: none)
extern "C" void fn_8004A1D4(AudioDevice* audio, int preset) {
    if (audio->m408 == 0) {
        return;
    }
    if (preset == audio->m8BC) {
        return;
    }
    audio->m8C0 = audio->m8BC;
    if (preset <= 0) {
        ClearEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusi(GetInstance__Q44nw4r3snd6detail9AxManagerFv(), 0, 0);
        audio->m8BC = 0;
        return;
    }
    AudioMap_unk<int, AudioFxPreset_unk, 4>::Node* it = audio->m8AC.find(preset);
    if (it != audio->m8AC.end()) {
        fn_8070EA60(&audio->mReverb, &it->mValue.second);
        if (audio->m8BC <= 0) {
            AppendEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusPQ34nw4r3snd6FxBase(
                GetInstance__Q44nw4r3snd6detail9AxManagerFv(), 0, &audio->mReverb);
        }
    }
    audio->m8BC = preset;
}

// inferred: suspends the reverb (mask of the users, fade in milliseconds)
extern "C" void fn_8004A2CC(AudioDevice* audio, int suspend, unsigned int mask, int fade) {
    if (suspend != 0) {
        if (audio->m40C == 0) {
            ClearEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusi(GetInstance__Q44nw4r3snd6detail9AxManagerFv(),
                                                                           0, fade * 1000 / 30);
        }
        audio->m40C |= mask;
    } else {
        if ((audio->m40C &= ~mask) == 0 && audio->m8BC > 0) {
            AppendEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusPQ34nw4r3snd6FxBase(
                GetInstance__Q44nw4r3snd6detail9AxManagerFv(), 0, &audio->mReverb);
        }
    }
}

extern "C" void fn_806095E0(int on);

extern "C" void fn_8004A388(AudioDevice* audio, int on) {
    if (on != 0 && audio->m8CC == 0) {
        fn_806095E0(1);
        audio->m8CC = 1;
    } else if (on == 0 && audio->m8CC != 0) {
        fn_806095E0(0);
        audio->m8CC = 0;
    }
}

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: reads the sound id table (rsid.csv: label, id, bit, flag, bank)
extern "C" void fn_8004A3FC(AudioDevice* audio, const char* path) {
    if (audio->m8E0 != 0) {
        return;
    }
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
    atn::Csv csv;
    ((atn::CsvBase<char, ',', '\r', '"'>&)csv).vf08((const char*)data, size);
    do {
        char name[0x100];
        sprintf(name, "%s\0", csv.fn_8005B9CC());
        int id = fn_800DC12C(csv.fn_8005B9CC());
        int bit = fn_800DC12C(csv.fn_8005B9CC());
        if (fn_800DC12C(csv.fn_8005B9CC()) > 0) {
            lbl_8087EE88[bit / 32] |= 1 << (bit % 32);
        }
        int bank = fn_800DC12C(csv.fn_8005B9CC());
        AudioSoundId_unk& entry = audio->m8D0[fn_800DC6B4(name)];
        entry.mId = id;
        entry.mBank = bank;
    } while (csv.fn_8005BBF8() != 0);
    fn_8046DD20(lbl_8087F518, data);
    audio->m8E0 = 1;
}
