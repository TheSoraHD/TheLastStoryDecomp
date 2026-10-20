// inferred: pfs archives of the load manager (index + data file, sorted entries by path hash) and
// the cache of loaded resources. Names invented.
#include "atn/LoadManager.h"
#include "atn/LH_Binary.h"
#include "atn/PfsArchive_unk.h"
#include <string.h>
#include <ctype.h>
#include "std/string.h"

extern "C" int sprintf(char* s, const char* format, ...);
struct DebugMessages_unk { void fn_800698DC(const char* path); }; // inferred: see atn/FileUtil_unk.cpp
extern "C" DebugMessages_unk* lbl_8087EEB8;                                 // inferred: listener of failed loads
extern "C" int fn_805F9EF0(const char* name);                  // inferred: opens the archive file
extern "C" std::string fn_8006AC08(const std::string& path);     // inferred: extension of a path

// inferred: output of the directory listing (atn::FileList, 16-byte entries: kind first)
struct FileListEntry_unk {
    int mKind; // 1 = directory, 2 = file
    unsigned char m04[0xC];
};
struct FileListOut_unk {
    FileListEntry_unk& At_unk(int i) { return mEntries[i]; }
    int m00;
    int mCount;
    FileListEntry_unk* mEntries;
};
extern "C" void fn_8006BD78(FileListOut_unk* list, const char* format, ...); // inferred: adds an entry

// inferred: directory of the archive file table (0x18 bytes)
struct PfsDir_unk {
    unsigned int m00;
    unsigned int m04;
    int mFirstDir;  // 0x08
    int mDirCount;  // 0x0C
    int mFirstFile; // 0x10
    int mFileCount; // 0x14
};
// inferred: header of the file table
struct PfsTable_unk {
    unsigned int m00;
    unsigned int m04;
    int mDirCount;  // 0x08
    int mFileCount; // 0x0C
    PfsDir_unk mDirs[1]; // 0x10, then the name offsets of the directories and of the files
};


namespace atn {

// inferred: copy of the path only when it is not the same buffer
inline void __CopyName_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}

// NONMATCHING: 98.3% - temporaries of the loop take other volatile registers
PfsArchive_unk* LoadManager_unk::fn_8046F460(const char* path) {
    if (*path == '/') {
        path++;
    }
    unsigned int hash = fn_800DC6B4(path);
    std::list<PfsArchive_unk*>::iterator it = mArchives.begin();
    std::list<PfsArchive_unk*>::iterator end = mArchives.end();
    for (; it != end; ++it) {
        if (hash == (*it)->mHash) {
            return *it;
        }
    }
    return 0;
}

// NONMATCHING: 91.8% - list end/iterator swap r30/r31 (tried local end, iterator first, find_if)
int LoadManager_unk::fn_8046F4D4(const char* path) {
    for (std::list<PfsArchive_unk*>::iterator it = mMounted.begin(); it != mMounted.end(); ++it) {
        if ((*it)->fn_8046FBB0(path) != -1) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98.8% - list end/iterator swap r30/r31
int LoadManager_unk::fn_8046F544(void* out, const char* dir, const char* ext, int flags) {
    int count = 0;
    std::list<PfsArchive_unk*>::iterator it = mMounted.begin();
    std::list<PfsArchive_unk*>::iterator end = mMounted.end();
    for (; it != end; ++it) {
        count += (*it)->fn_8046FD7C(out, dir, ext, flags);
    }
    return count;
}

static char sSizeFormat[] = "size=%d";

void LoadCore::vf10(char* out) {
    sprintf(out, sSizeFormat, m50);
}

// NONMATCHING: 95.0% - the original reloads m48[mid] for the second test of the search and computes
// the index of the first insert without the begin pointer. Tried a reference to the cache.
void LoadManager_unk::fn_8046F5CC(LoadCore* core) {
    if (m48.size() == 0) {
        m48.push_back(core);
        return;
    }
    unsigned int hash = core->m48;
    int lo = 0;
    int hi = m48.size() - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (hash > m48[mid]->m48) {
            lo = mid + 1;
        }
        if (hash <= m48[mid]->m48) {
            hi = mid - 1;
        }
    }
    if (hash < m48[lo]->m48) {
        m48.insert(&m48[lo], core);
    } else {
        m48.insert(&m48[lo] + 1, core);
    }
}

LoadCore* LoadManager_unk::fn_8046F834(void* list, unsigned int hash) {
    LoadCache_unk& cache = *(LoadCache_unk*)list;
    int lo = 0;
    int hi = cache.size() - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (hash > cache[mid]->m48) {
            lo = mid + 1;
        } else if (hash < cache[mid]->m48) {
            hi = mid - 1;
        } else {
            if (cache[mid]->m58 != 0 && cache[mid]->mRefCount == 0) {
                return 0;
            }
            return cache[mid];
        }
    }
    return 0;
}

// NONMATCHING: 93.2% - register allocation (the hash pointer and the cache pointer)
void LoadManager_unk::fn_8046F8B8(unsigned int* hashes, unsigned int count) {
    unsigned int j = 0;
    for (unsigned int i = 0; i < count; i++) {
        while (j < m48.mCount && hashes[i] > m48.mItems[j]->m48) {
            j++;
        }
        if (j < m48.mCount && hashes[i] == m48.mItems[j]->m48 && m48.mItems[j]->m58 == 0) {
            m48.mItems[j]->fn_804730D4();
        } else {
            hashes[i] = 0;
        }
    }
}

// NONMATCHING: 97.6% - register allocation
void LoadManager_unk::fn_8046F96C(unsigned int* hashes, unsigned int count) {
    unsigned int j = 0;
    for (unsigned int i = 0; i < count; i++) {
        if (hashes[i] == 0) {
            continue;
        }
        while (j < m48.mCount && hashes[i] > m48.mItems[j]->m48) {
            j++;
        }
        if (j >= m48.mCount) {
            return;
        }
        if (hashes[i] == m48.mItems[j]->m48 && m48.mItems[j]->m58 == 0) {
            m48.mItems[j]->fn_804730E4();
        }
    }
}

PfsArchive_unk::PfsArchive_unk(const char* name, const char* index, int flags)
    : m18(0), mRefCount(0), m70(0), m74(0), mIndexLoaded(0) {
    mName = *name == '/' ? name + 1 : name;
    mHash = fn_800DC6B4(mName);
    if (flags & 2) {
        mIndex.vf0C(index);
        mCount = 0;
        mEntries = 0;
    } else {
        mIndex.fn_80473FCC(index);
        mIndexLoaded = 1;
        if (mIndex.fn_80470580() == 0 || mIndex.fn_8047059C() == 0) {
            mCount = 0;
            mEntries = 0;
            return;
        }
        mCount = *(int*)mIndex.fn_80470580();
        mEntries = (PfsEntry_unk*)((int*)mIndex.fn_80470580() + 1);
        if (mCount <= 0) {
            return;
        }
    }
    if (!(flags & 1)) {
        m24 = fn_805F9EF0(name);
    }
}

int PfsArchive_unk::fn_8046FBB0(const char* path) const {
    if (mIndexLoaded == 0 || mCount == 0) {
        return -1;
    }
    char lower[0x100];
    char* p = lower;
    if (*path == '/' || *path == '\\') {
        path++;
    }
    while (*path != 0) {
        if (*path == '\\') {
            *p++ = '/';
            path++;
        } else {
            *p++ = tolower(*path++);
        }
    }
    *p = 0;
    unsigned int hash = fn_800DC6B4(lower);
    int hi = mCount;
    int lo = 0;
    int mid = hi / 2;
    do {
        mid += lo;
        unsigned int h = mEntries[mid].mHash;
        if (h == hash) {
            return mid;
        }
        if (h < hash) {
            lo = mid;
        }
        if (h >= hash) {
            hi = mid;
        }
        mid = (hi - lo) / 2;
    } while (mid > 0);
    if (mid == 0 && hash == fn_8046FD1C(mid)->mHash) {
        return 0;
    }
    return -1;
}

PfsEntry_unk* PfsArchive_unk::fn_8046FD1C(int index) const {
    return &mEntries[index];
}

void* PfsArchive_unk::fn_8046FD2C(int index) {
    return (char*)((PfsHandle_unk*)&mData)->fn_80470564() + mEntries[index].mOffset;
}

// NONMATCHING: 92.3% - register allocation (the original keeps the string pool base and copies of
// the table pointers in saved registers) and the pool starts with an unused "pfs" here missing.
int PfsArchive_unk::fn_8046FD7C(void* out, const char* dir, const char* ext, int flags) {
    FileListOut_unk* list = (FileListOut_unk*)out;
    int count = 0;
    if (m18 != 0 && m1C.fn_80470580() != 0) {
        if (*dir == '/') {
            dir++;
        }
        PfsTable_unk* table = (PfsTable_unk*)m1C.fn_80470580();
        PfsDir_unk* dirs = table->mDirs;
        unsigned int* dirNames = (unsigned int*)(dirs + table->mDirCount);
        unsigned int* fileNames = dirNames + table->mDirCount;
        const char* strings = (const char*)(fileNames + table->mFileCount);
        PfsDir_unk* cur = dirs;
        const char* p = dir;
        while (cur != 0 && *p != 0) {
            const char* slash = strstr(p, "/");
            int len;
            if (slash != 0) {
                len = slash - p;
            } else {
                len = strlen(p);
            }
            int i = cur->mFirstDir;
            int end = i + cur->mDirCount;
            cur = 0;
            for (; i < end; i++) {
                if (strncmp(strings + dirNames[i], p, len) == 0) {
                    cur = &dirs[i];
                    break;
                }
            }
            p += len;
            if (*p == '/') {
                p++;
            }
        }
        if (cur != 0 && *p == 0) {
            for (int i = cur->mFirstDir; i < cur->mFirstDir + cur->mDirCount; i++) {
                const char* name = strings + dirNames[i];
                fn_8006BD78(list, name);
                count++;
                list->At_unk(list->mCount - 1).mKind = 1;
                if (flags & 1) {
                    char sub[0x100];
                    sprintf(sub, "%s/%s", dir, name);
                    count += fn_8046FD7C(out, sub, ext, flags);
                }
            }
            for (int i = cur->mFirstFile; i < cur->mFirstFile + cur->mFileCount; i++) {
                const char* name = strings + fileNames[i];
                bool match = ext == 0 || strcmp(ext, "*") == 0 || fn_8006AC08(std::string(name)) == ext;
                if (match) {
                    fn_8006BD78(list, name);
                    count++;
                    list->At_unk(list->mCount - 1).mKind = 2;
                }
            }
        }
    }
    return count;
}

void PfsArchive_unk::fn_80470198() {
    mRefCount++;
    if (mData.fn_80473EFC() == 0 || mData.fn_80473EFC() == 4) {
        mData.fn_80473FCC(mName);
    }
}

void PfsArchive_unk::fn_804701F8() {
    mRefCount++;
    if (mData.fn_80473EFC() == 0 || mData.fn_80473EFC() == 4) {
        mData.vf0C(mName);
    }
}

int PfsArchive_unk::fn_80470264() {
    int busy = 0;
    if (mData.fn_80473F50() != 0 || mIndexLoaded == 0) {
        busy = 1;
    }
    return busy;
}

int PfsArchive_unk::fn_804702BC() {
    if (mIndexLoaded == 0) {
        if (mIndex.fn_80473F50() == 0) {
        mIndexLoaded = 1;
        if (mIndex.fn_80470580() == 0 || mIndex.fn_8047059C() == 0) {
            mCount = 0;
            mEntries = 0;
        } else {
            mCount = *(int*)mIndex.fn_80470580();
            mEntries = (PfsEntry_unk*)((int*)mIndex.fn_80470580() + 1);
        }
        }
        return 1;
    }
    return 0;
}

} // namespace atn
