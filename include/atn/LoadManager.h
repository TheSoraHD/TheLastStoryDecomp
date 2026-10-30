#ifndef ATN_LOADMANAGER_H
#define ATN_LOADMANAGER_H

#include "atn/LoadCore.h"
#include "atn/Memory.h"
#include "std/list.h"
#include "std/string.h"
#include "atn/Task.h"
#include "game/ResHandle.h"

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

// inferred: load from an archive whose index is not read yet (0x4C bytes, atn/LoadManager_unk.cpp
// 0x8046C3FC). Name invented.
struct PendingLoad_unk {
    // inferred: path copied only from another buffer, without constructor (none is called for the
    // local copy of 0x8046C3FC)
    struct Path_unk {
        void operator=(const char* src) {
            if (src != m) {
                memcpy(m, src, strlen(src) + 1);
            }
        }
        char m[0x40];
    };
    PendingLoad_unk(PfsArchive_unk* archive, LoadCore* core, const char* path, int index)
        : mArchive(archive), mCore(core) {
        mPath = path;
        mIndex = index;
    }
    PfsArchive_unk* mArchive;
    LoadCore* mCore;
    Path_unk mPath;
    int mIndex;
};

// inferred: fixed-size path of the loader (copied in its constructor, no clearing)
struct Path256_unk {
    Path256_unk(const char* src) {
        if (src != m) {
            memcpy(m, src, strlen(src) + 1);
        }
    }
    void operator=(const char* src) {
        if (src != m) {
            memcpy(m, src, strlen(src) + 1);
        }
    }
    operator char*() { return m; }
    char m[0x100];
};

// inferred: loaded resources sorted by path hash (count, then the array). Name invented.
struct LoadCache_unk {
    LoadCache_unk() : mCount(0) {} // inferred: atn::Loader constructor (0x8046D84C)
    unsigned int size() const { return mCount; }
    LoadCore*& operator[](int i) { return mItems[i]; }
    LoadCore** begin() { return mItems; }
    LoadCore** end() { return mItems + mCount; }
    // inferred: removes [first, end) (atn::Loader::fn_8046E410; moves the elements by one only)
    void erase(LoadCore** first) {
        int i = first - begin();
        int n = (&mItems[mCount] - begin()) - i;
        for (; i < mCount - 1; i++) {
            if (i + n > mCount) {
                break;
            }
            mItems[i] = mItems[i + 1];
        }
        mCount -= n;
    }
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

// inferred: d2anime task of the loading screen (data/d2anime/ui_loading.d2a); only what the
// loader uses
struct LoaderD2A_unk : public Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x100 - 0x58]; // 0x58: inferred: layout of the panes
    float m100;
    float m104; // inferred: animation speed
    int m108;
};

// atn::Loader (RTTI name and vtable 0x8078FB70): manager of the resources being loaded (global
// at 0x8087F518, created by fn_8046D7D8). Its methods are spread over atn/LoadManager_unk.cpp,
// atn/Loader.cpp and atn/FileSystem_unk.cpp.
class Loader : public Task {
public:
    Loader(Task* parent);
    virtual ~Loader();
    virtual int vf0C();
    virtual void vf10(); // inferred: advances the pending loads
    virtual void vf14(); // inferred: releases unused archives and runs the loading screen

    void fn_8046CBB0(); // inferred: called at the end of the constructor
    void fn_8046CBBC(); // inferred: called at the start of the destructor
    int fn_8046C3FC(LoadCore* core);  // inferred: queues the resource, 0 on failure
    void fn_8046DBF8(char* out, const char* path); // inferred: copy with the language suffix
    void fn_8046E410();           // inferred: deletes the cached resources without references
    void fn_8046EC7C();           // inferred: creates the loading screen
    int fn_8046ECC8();            // inferred: the loading screen is not running
    PfsArchive_unk* fn_8046F460(const char* path); // inferred: archive by name
    int fn_8046F4D4(const char* path);             // inferred: some mounted archive has `path`
    int fn_8046F544(void* out, const char* dir, const char* ext, int flags);
    void fn_8046F5CC(LoadCore* core); // inferred: adds a resource to the sorted cache
    LoadCore* fn_8046F834(void* list, unsigned int hash); // inferred: lookup in another container
    void fn_8046F8B8(unsigned int* hashes, unsigned int count); // inferred: references cached resources
    void fn_8046F96C(unsigned int* hashes, unsigned int count); // inferred: releases them

    // inferred: getters (weak copies 0x8046D184..0x8046D1C4, out of line in fn_8046CC90)
    int fn_8046D184() { return m400C; }
    int fn_8046D18C() { return m4008; }
    char* fn_8046D194() { return m3F08; }
    std::string& fn_8046D1C4() { return m4010; }

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

    LoadCache_unk m48;                // inferred: another container, see fn_8046F834
    std::__list_owned_unk<LoadCore*> mList; // 0x3ECC
    int m3ED8; // inferred: nonzero when the files on disc take priority over the archives
    int m3EDC; // inferred: set when a resource loses its last reference (0x804730E4)
    LoaderD2A_unk* m3EE0; // inferred: loading screen
    int m3EE4;
    int m3EE8;
    int m3EEC;
    int m3EF0;
    int m3EF4;
    int m3EF8;
    std::string m3EFC;
    Path256_unk m3F08; // inferred: directory searched first (fn_8046D1EC)
    int m4008;
    int m400C; // inferred: zero when the paths are used as they are
    std::string m4010; // inferred: directory of the files that override the disc
    ResHandle m401C;
    std::__list_owned_unk<PfsArchive_unk*> mArchives; // 0x4020: inferred: every opened archive
    std::__list_owned_unk<PfsArchive_unk*> mMounted;  // 0x402C: inferred: archives searched by path
    std::__list_owned_unk<PendingLoad_unk> mPending;  // 0x4038: inferred: loads waiting for an archive index
    int m4044;
};

// Defined out of the class: its literals are in the file string pool (0x80755C90).
// NONMATCHING: 99.7% (LoadColliTree, 0x80472DCC) - same structure in all twelve copies. The saved
// registers match since std::list::push_back takes its position as a const_iterator and `core` is a
// T* (with LoadCore* it takes the lowest register). Left: in the lookup loop the original keeps the
// list end in r0 and reuses r6 for the predicate (here end r7, predicate r6/r0), and it writes the
// found resource straight into r30 while the return value of Find_unk goes through r3 here.
// The bne+b of the lookup only comes with an inline return; written directly in Get_unk (goto or an
// out parameter) the resource goes to r30 but the branch becomes a single beq.
// Tried for the loop (2026-10-30): end() in the condition (right registers, rotated loop), a named
// end before/after begin, end bound to a reference, find_if with last by reference or as a
// non-trivial iterator, by-value predicate, for/while/break forms (&& with end() gives a bool).
template <class T>
T* Loader::Get_unk(const char* path, int cached) {
    char fullPath[0x100];
    fn_8046D1EC(fullPath, path);
    fn_8006A950(fullPath);
    fn_8006B404(fullPath);
    unsigned int hash = fn_800DC6B4(fullPath);
    T* core = 0;
    if (cached) {
        core = (T*)Find_unk(hash);
    }
    if (core == 0) {
        core = new ((MemoryTag)2, "", "", 0) T(fullPath);
        if (fn_8046C3FC(core) != 0) {
            mList.push_back(core);
        } else {
            core->fn_80473104(4);
            fn_8046F5CC(core);
        }
    } else {
        core->fn_804730D4();
    }
    return core;
}

} // namespace atn

extern "C" atn::Loader* lbl_8087F518;
extern "C" atn::Loader* fn_8046D7D8(atn::Task* parent); // inferred: creates the loader
// inferred: loads a whole file into a new buffer (align is not used)
extern "C" void* fn_8046DC5C(atn::Loader* loader, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(atn::Loader* loader, void* data); // inferred: frees a buffer of fn_8046DC5C
extern "C" void fn_8046ECDC(atn::Loader* loader, int show); // inferred: shows the loading screen
extern "C" void fn_8046EED8(atn::Loader* loader);           // inferred: hides the loading screen
extern "C" void fn_8046F014(atn::Loader* loader);           // inferred: mounts the main archives
extern "C" void fn_8046F2C4(atn::Loader* loader);           // inferred: opens the preload archives

#endif
