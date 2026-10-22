#ifndef ATN_FILELIST_H
#define ATN_FILELIST_H

#include "std/string.h"
#include "std/vector.h"

namespace atn {

class FileList {
public:
    // inferred: list entry (0x10 bytes)
    struct Entry {
        int m00;
        std::string mName; // 0x04
    };

    FileList(); // 0x8006BBCC
    virtual ~FileList();
    // inferred: name of an entry (weak copy 0x8046D19C, atn/Loader.cpp)
    const char* GetName_unk(int i) { return mEntries[i].mName.c_str(); }
    // inferred: fills the list with the files of `dir` with the extension `ext`; atn::Loader builds
    // the list on the stack (the vtable 0x80777BC0 only has the destructor: no pure virtual)
    void fn_8006C3F0(const char* dir, const char* ext, int flags);
    int fn_8006C300(const char* name, int arg); // inferred: index of `name`, negative if missing

    int m04;
    std::vector<Entry> mEntries; // 0x08
};

} // namespace atn

#endif
