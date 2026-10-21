#ifndef ATN_PFSARCHIVE_UNK_H
#define ATN_PFSARCHIVE_UNK_H

// inferred: pfs archives of the load manager (see src/atn/FileSystem_unk.cpp and src/atn/ResRef_unk.cpp)
#include "atn/LoadManager.h"
#include "atn/LH_Binary.h"
#include <string.h>

namespace atn {

// inferred: entry of an archive index (16 bytes): path hash and offset in the data file
struct PfsEntry_unk {
    unsigned int mHash;
    unsigned int mOffset;
    unsigned int m08;
    unsigned int m0C;
};

// inferred: fixed-size path (cleared by its constructor, copied only from another buffer)
struct PathName_unk {
    PathName_unk() { m[0] = 0; }
    void operator=(const char* src) {
        if (src != m) {
            memcpy(m, src, strlen(src) + 1);
        }
    }
    operator char*() { return m; }
    char m[0x40];
};

// inferred: pfs archive (constructor 0x8046FA18, built by atn::Loader). Name invented.
class PfsArchive_unk {
public:
    PfsArchive_unk(const char* name, const char* index, int flags);
    int fn_8046FBB0(const char* path) const;
    PfsEntry_unk* fn_8046FD1C(int index) const;
    void* fn_8046FD2C(int index);
    int fn_8046FD7C(void* out, const char* dir, const char* ext, int flags);
    void fn_80470198();
    void fn_804701F8();
    int fn_80470264();
    int fn_804702BC();

    int mCount;              // 0x00
    PfsEntry_unk* mEntries;  // 0x04
    LH_Binary mIndex;        // 0x08
    LH_Binary mData;         // 0x10
    int m18;
    LH_Binary m1C;           // 0x1C
    int m24;
    unsigned int mHash;      // 0x28
    int mRefCount;           // 0x2C
    PathName_unk mName;      // 0x30
    int m70;
    int m74;
    int mIndexLoaded;        // 0x78
};

// inferred: data of a loaded handle
struct PfsHandle_unk : public LoadHandle {
    void* fn_80470564();
};

} // namespace atn

// inferred: reference to an archive (see game/CampSaveLoad.h)
struct ResRef_unk {
    void fn_80470364(const char* name, int a, int b, int c);
    void fn_8047043C(const char* name, int a, int b);
    void fn_80470528();
    int fn_8047054C();

    atn::PfsArchive_unk* mRes;
    void* m04;
};

extern "C" atn::PfsArchive_unk* fn_8046EF08(atn::LoadManager_unk* mgr, const char* name, const char* index,
                                            int flags); // inferred: opens an archive

#endif
