// inferred: cache of composed textures (used by atn::CompositeIntensityTintMulti and atn::Model): an MSL
// std::list of reference counted entries kept in a function-level static. File and class names invented;
// the types are duplicated in src/atn/CompositeIntensityTintMulti.cpp.
#include "atn/Texture_unk.h"
#include "atn/Memory.h"
#include "std/list.h"

extern "C" unsigned int fn_800DC6B4(const char* str); // inferred: hash of a string (atn/Util_unk)

namespace atn {

// inferred: RGBA color
struct CacheColor_unk {
    float r, g, b, a;
};

// inferred: description of one layer (0x40 bytes; see CompositeLayerDesc_unk in CompositeIntensityTintMulti.cpp)
struct CompositeLayerDesc_unk {
    const char* mPath;        // 0x00
    CacheColor_unk mColor[3]; // 0x04
    int mUse[3];              // 0x34
};

// inferred: the same layer with the hash of its path
struct CompositeKeyLayer_unk {
    unsigned int mHash;       // 0x00
    CacheColor_unk mColor[3]; // 0x04
    int mUse[3];              // 0x34
};

// inferred: key of a cached texture (0x188 bytes)
struct CompositeKey_unk {
    CompositeKey_unk(const char* base, const CompositeLayerDesc_unk* layers, int count);
    bool fn_800D82BC(const CompositeKey_unk& other) const;

    unsigned int mHash;               // 0x00
    int mCount;                       // 0x04
    CompositeKeyLayer_unk mLayers[6]; // 0x08
};

// inferred: cached composed texture (refcounted; 0x1BC bytes)
struct CompositeCacheEntry_unk {
    CompositeCacheEntry_unk(const char* base, const CompositeLayerDesc_unk* layers, int count)
        : mRefCount(0), mKey(base, layers, count) {}
    void fn_800D8448();
    void fn_800D8458();

    int mRefCount;          // 0x000
    CompositeKey_unk mKey;  // 0x004
    Texture_unk mTexture;   // 0x18C
};

// inferred: the MSL list destroys its nodes in its destructor (std/list.h has no destructor so that the
// other users are not changed); two levels as in the MSL __list_imp
template <class T>
struct CacheListImp_unk : public std::list<T> {
    CacheListImp_unk() {
        CacheListImp_unk* self = this;
        self->mSize = 0;
        std::__list_node_base_unk* tail = &self->mTail;
        tail->mNext = tail;
        tail->mPrev = tail;
    }
    ~CacheListImp_unk() { this->clear(); }
};
template <class T>
struct CacheListMid_unk : public CacheListImp_unk<T> {
    ~CacheListMid_unk() {}
};
template <class T>
struct CacheList_unk : public CacheListMid_unk<T> {
    ~CacheList_unk() {}
};

class CompositeCache_unk {
public:
    ~CompositeCache_unk();
    CompositeCacheEntry_unk* fn_800D8624(const char* base, const CompositeLayerDesc_unk* layers, int count);
    CompositeCacheEntry_unk* fn_800D8698(const char* base, const CompositeLayerDesc_unk* layers, int count);
    // inferred: deletes the entries without references
    void Purge_unk() {
        std::list<CompositeCacheEntry_unk*>::iterator it = mList.begin();
        while (it != mList.end()) {
            CompositeCacheEntry_unk* entry = *it;
            if (entry->mRefCount == 0) {
                delete entry;
                it = mList.erase(it);
            } else {
                ++it;
            }
        }
    }
    static CompositeCache_unk& Get_unk() {
        static CompositeCache_unk sCache;
        return sCache;
    }

    CacheList_unk<CompositeCacheEntry_unk*> mList;
};

// inferred: reference to a cache entry
struct CompositeCacheHandle_unk {
    CompositeCacheEntry_unk* mEntry;
};

CompositeKey_unk::CompositeKey_unk(const char* base, const CompositeLayerDesc_unk* layers, int count) {
    mHash = fn_800DC6B4(base);
    mCount = count;
    for (int i = 0; i < count; i++) {
        mLayers[i].mHash = fn_800DC6B4(layers->mPath);
        mLayers[i].mColor[0].r = layers->mColor[0].r;
        mLayers[i].mColor[0].g = layers->mColor[0].g;
        mLayers[i].mColor[0].b = layers->mColor[0].b;
        mLayers[i].mColor[0].a = layers->mColor[0].a;
        mLayers[i].mUse[0] = layers->mUse[0];
        mLayers[i].mColor[1].r = layers->mColor[1].r;
        mLayers[i].mColor[1].g = layers->mColor[1].g;
        mLayers[i].mColor[1].b = layers->mColor[1].b;
        mLayers[i].mColor[1].a = layers->mColor[1].a;
        mLayers[i].mUse[1] = layers->mUse[1];
        mLayers[i].mColor[2].r = layers->mColor[2].r;
        mLayers[i].mColor[2].g = layers->mColor[2].g;
        mLayers[i].mColor[2].b = layers->mColor[2].b;
        mLayers[i].mColor[2].a = layers->mColor[2].a;
        mLayers[i].mUse[2] = layers->mUse[2];
        layers++;
    }
}

bool CompositeKey_unk::fn_800D82BC(const CompositeKey_unk& other) const {
    if (mHash == other.mHash && mCount == other.mCount) {
        bool eq = true;
        for (int i = 0; i < mCount; i++) {
            eq = eq && mLayers[i].mHash == other.mLayers[i].mHash;
            for (int j = 0; j < 3; j++) {
                eq = eq && mLayers[i].mUse[j] == other.mLayers[i].mUse[j];
                if (other.mLayers[i].mUse[j] != 0) {
                    bool t = eq && (float)__fabs(mLayers[i].mColor[j].r - other.mLayers[i].mColor[j].r) < 1.1920929e-7f;
                    t = t && (float)__fabs(mLayers[i].mColor[j].g - other.mLayers[i].mColor[j].g) < 1.1920929e-7f;
                    t = t && (float)__fabs(mLayers[i].mColor[j].b - other.mLayers[i].mColor[j].b) < 1.1920929e-7f;
                    eq = t && (float)__fabs(mLayers[i].mColor[j].a - other.mLayers[i].mColor[j].a) < 1.1920929e-7f;
                }
            }
        }
        return eq;
    }
    return false;
}

void CompositeCacheEntry_unk::fn_800D8448() {
    mRefCount++;
}

// NONMATCHING: 99.04% - the list begin/end iterators are swapped in r30/r31 (~10 variants of the loop)
void CompositeCacheEntry_unk::fn_800D8458() {
    if (--mRefCount == 0) {
        CompositeCache_unk::Get_unk().Purge_unk();
    }
}

CompositeCache_unk::~CompositeCache_unk() {}

CompositeCacheEntry_unk* CompositeCache_unk::fn_800D8624(const char* base, const CompositeLayerDesc_unk* layers,
                                                         int count) {
    CompositeKey_unk key(base, layers, count);
    for (std::list<CompositeCacheEntry_unk*>::iterator it = mList.begin(); it != mList.end(); ++it) {
        if ((*it)->mKey.fn_800D82BC(key)) {
            return *it;
        }
    }
    return 0;
}

// NONMATCHING: 97.99% - registers of the inlined push_back (the same known problem as the Loader::Get_unk
// copies, see include/atn/LoadManager.h)
CompositeCacheEntry_unk* CompositeCache_unk::fn_800D8698(const char* base, const CompositeLayerDesc_unk* layers,
                                                         int count) {
    CompositeCacheEntry_unk* entry = new ((MemoryTag)6, "", "", 0) CompositeCacheEntry_unk(base, layers, count);
    mList.push_back(entry);
    return entry;
}

} // namespace atn

// 0x800D87A4-0x800D8894 (handle helpers, color ctor, texture accessors) is not part of this file: they call
// fn_800D8448/fn_800D8458 out of line, so they come from another file (probably atn/CompositeIntensityTintMulti).
