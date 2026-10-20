#ifndef ATN_LOADMANAGER_H
#define ATN_LOADMANAGER_H

#include "atn/LoadCore.h"
#include "atn/Memory.h"
#include "std/list.h"

extern "C" void fn_8046D1EC(char* out, const char* path); // inferred: builds the full path
extern "C" void fn_8006A950(char* path);                  // inferred: path normalisation
extern "C" void fn_8006B404(char* path);
extern "C" unsigned int fn_800DC6B4(const char* s);       // inferred: string hash

namespace atn {

template <class It, class Pred>
inline It __find_if_unk(It first, It last, const Pred& pred) {
    while (first != last && !pred(*first)) {
        ++first;
    }
    return first;
}

// inferred: predicate of the lookup by path hash
struct __HashIs_unk {
    __HashIs_unk(unsigned int hash) : mHash(hash) {}
    bool operator()(LoadCore* core) const { return mHash == core->m48; }
    unsigned int mHash;
};

class PfsArchive_unk;

// inferred: loaded resources sorted by path hash (count, then the array). Name invented.
struct LoadCache_unk {
    unsigned int size() const { return mCount; }
    LoadCore*& operator[](int i) { return mItems[i]; }
    LoadCore** begin() { return mItems; }
    void push_back(LoadCore* core) {
        new (&mItems[mCount]) LoadCore*(core);
        mCount++;
    }
    void insert(LoadCore** pos, LoadCore* core) {
        unsigned int index = pos - begin();
        for (unsigned int i = mCount; i > index; i--) {
            mItems[i] = mItems[i - 1];
        }
        mItems[index] = core;
        mCount++;
    }

    unsigned int mCount;
    LoadCore* mItems[(0x3ECC - 0x4C) / 4];
};

// inferred: manager of the resources being loaded (global at 0x8087F518). Name invented.
class LoadManager_unk {
public:
    // inferred: it has a vtable (0x80473FCC calls the slot 0x10); the other slots are unknown
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10(); // inferred: advances the pending loads
    int fn_8046C3FC(LoadCore* core);  // inferred: queues the resource, 0 on failure
    PfsArchive_unk* fn_8046F460(const char* path); // inferred: archive by name
    int fn_8046F4D4(const char* path);             // inferred: some mounted archive has `path`
    int fn_8046F544(void* out, const char* dir, const char* ext, int flags);
    void fn_8046F5CC(LoadCore* core); // inferred: adds a resource to the sorted cache
    LoadCore* fn_8046F834(void* list, unsigned int hash); // inferred: lookup in another container
    void fn_8046F8B8(unsigned int* hashes, unsigned int count); // inferred: references cached resources
    void fn_8046F96C(unsigned int* hashes, unsigned int count); // inferred: releases them

    // inferred: lookup of a resource by path hash. The early return gives the bne+b of 0x80472E5C.
    LoadCore* Find_unk(unsigned int hash) {
        std::list<LoadCore*>::iterator it =
            __find_if_unk(mList.begin(), mList.end(), __HashIs_unk(hash));
        if (it != mList.end()) {
            LoadCore* core = *it;
            if (core->m05 == 0) {
                return core;
            }
        }
        return fn_8046F834(&m48, hash);
    }

    // inferred: returns the resource for `path`, creating it if needed. A template instantiated by
    // each atn::LH_* file (weak copies: LoadColliTree 0x80472DCC, LoadMotion 0x804766B0).
    template <class T>
    T* Get_unk(const char* path, int cached);

    unsigned char m04[0x48 - 0x04];
    LoadCache_unk m48;                // inferred: another container, see fn_8046F834
    std::list<LoadCore*> mList;       // 0x3ECC
    unsigned char m3ED8[0x3EDC - 0x3ED8];
    int m3EDC; // inferred: set when a resource loses its last reference (0x804730E4)
    unsigned char m3EE0[0x4020 - 0x3EE0];
    std::list<PfsArchive_unk*> mArchives; // 0x4020: inferred: every opened archive
    std::list<PfsArchive_unk*> mMounted;  // 0x402C: inferred: archives searched by path
};

// Defined out of the class: its literals are in the file string pool (0x80755C90).
// NONMATCHING: 96.6% (LoadColliTree, 0x80472DCC) - same structure in all twelve copies; only the
// saved registers differ. In every original the list end used by push_back gets the lowest one
// (r28, shared with `cached`) and the resource gets r30 (r29 in LoadMotion, whose constructor is
// not inlined); here the resource takes r28. Also, the original writes the found resource straight
// into that register, while the return value of Find_unk goes through r3 here.
// Writing the lookup directly in Get_unk (goto/break) makes everything identical except that
// register and the bne+b of the lookup, which only an inline return gives.
// Tried, without change: loop and condition forms, find_if with the end by value/reference or as
// an expression, declaration order, T* or a second variable for the new object, the order of the
// two branches, other insert/push_back nestings in std::list, an out parameter, Wii/1.0-1.3 and
// -inline deferred instead of -ipa file (all compile to the same code).
template <class T>
T* LoadManager_unk::Get_unk(const char* path, int cached) {
    char fullPath[0x100];
    fn_8046D1EC(fullPath, path);
    fn_8006A950(fullPath);
    fn_8006B404(fullPath);
    unsigned int hash = fn_800DC6B4(fullPath);
    LoadCore* core = 0;
    if (cached) {
        core = Find_unk(hash);
    }
    if (core == 0) {
        core = new (2, "", "", 0) T(fullPath);
        if (fn_8046C3FC(core) != 0) {
            mList.push_back(core);
        } else {
            core->fn_80473104(4);
            fn_8046F5CC(core);
        }
    } else {
        core->fn_804730D4();
    }
    return (T*)core;
}

} // namespace atn

extern "C" atn::LoadManager_unk* lbl_8087F518;

#endif
