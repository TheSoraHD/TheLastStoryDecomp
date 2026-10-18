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
    virtual void vf0C() = 0;

    int m04;
    std::vector<Entry> mEntries; // 0x08
};

} // namespace atn

#endif
