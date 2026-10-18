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

// inferred: manager of the resources being loaded (global at 0x8087F518). Name invented.
class LoadManager_unk {
public:
    int fn_8046C3FC(LoadCore* core);  // inferred: queues the resource, 0 on failure
    void fn_8046F5CC(LoadCore* core); // inferred: releases a resource that could not be queued
    LoadCore* fn_8046F834(void* list, unsigned int hash); // inferred: lookup in another container

    // inferred: lookup of a resource by path hash. The early return gives the bne+b of 0x80472E5C.
    LoadCore* Find_unk(unsigned int hash) {
        std::list<LoadCore*>::iterator it = __find_if_unk(mList.begin(), mList.end(), __HashIs_unk(hash));
        if (it != mList.end()) {
            LoadCore* core = *it;
            if (core->m05 == 0) {
                return core;
            }
        }
        return fn_8046F834(m48, hash);
    }

    // inferred: returns the resource for `path`, creating it if needed. A template instantiated by
    // each atn::LH_* file (weak copies: LoadColliTree 0x80472DCC, LoadMotion 0x804766B0).
    template <class T>
    T* Get_unk(const char* path, int cached);

    unsigned char m00[0x48];
    unsigned char m48[0x3ECC - 0x48]; // inferred: another container, see fn_8046F834
    std::list<LoadCore*> mList;       // 0x3ECC
};

// Defined out of the class: its literals are in the file string pool (0x80755C90).
// NONMATCHING: 96.6% (LoadColliTree, 0x80472DCC) - same structure; the found resource, the new
// node and the list end are in a different permutation of r28-r30. Tried: loop and condition forms, a local
// find_if, declaration order, T* instead of LoadCore*, ternary/if-else for the lookup.
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
