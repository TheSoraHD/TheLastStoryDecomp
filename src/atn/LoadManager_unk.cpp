// Asynchronous disc reader of the load manager: requests read in 256 KB chunks by two read slots,
// decompressed while they arrive, and the queue of the loads (atn::Loader::fn_8046C3FC).
// Names invented (the pool only has ".pk").
#include "atn/LoadManager.h"
#include "atn/LH_Binary.h"
#include "atn/FixedArray.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" int fn_805F9EF0(const char* path); // inferred: DVDConvertPathToEntrynum

// inferred: SDK DVD file (only what is used here)
struct DVDCommandBlock_unk {
    unsigned char m00[0x2C];
    void* mUserData; // 0x2C
};
struct DVDFileInfo_unk {
    DVDCommandBlock_unk mBlock;
    unsigned int mStartAddr; // 0x30: in words
    unsigned int mLength;    // 0x34
    void* mCallback;
};
extern "C" int DVDFastOpen(int entry, DVDFileInfo_unk* file);
extern "C" int DVDClose(DVDFileInfo_unk* file);
extern "C" int DVDReadAsyncPrio(DVDFileInfo_unk* file, void* addr, int length, int offset,
                                void (*callback)(int, DVDFileInfo_unk*), int prio);
extern "C" void DCFlushRangeNoSync(void* addr, unsigned long size);
extern "C" void DCZeroRange(void* addr, unsigned long size);

// inferred: engine memory manager (see atn/DwcAllocator_unk.cpp)
struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, unsigned long align, int heap,
                             const char* file, const char* func, int line, int arg);

// inferred: decompression (whole buffer, and by parts with a state of 0x18 bytes)
struct Decoder_unk {
    unsigned char m00[0x18];
};
extern "C" void fn_806254D0(const void* src, void* dst);
extern "C" void fn_806250D0(Decoder_unk* decoder, void* dst);
extern "C" void fn_80625110(Decoder_unk* decoder, const void* src);

namespace atn {

// inferred: entry of an archive index (see atn/FileSystem_unk.cpp)
struct PfsEntry_unk {
    unsigned int mHash;
    unsigned int mOffset;
    unsigned int mSize;
    unsigned int mPackedSize;
};

// inferred: pfs archive (only what is used here, see atn/FileSystem_unk.cpp)
class PfsArchive_unk {
public:
    int fn_8046FBB0(const char* path) const;
    PfsEntry_unk* fn_8046FD1C(int index) const;
    void* fn_8046FD2C(int index);
    int fn_80470264();

    int mCount;             // 0x00
    PfsEntry_unk* mEntries; // 0x04
    LH_Binary mIndex;       // 0x08
    LH_Binary mData;        // 0x10
    int m18;
    LH_Binary m1C;          // 0x1C
    int mEntry;             // 0x24: inferred: disc entry of the data file
    unsigned char m28[0x70 - 0x28];
    int m70; // 0x70: inferred: data kept in memory (no copy)
};

// inferred: read of a resource (0x50 bytes)
struct ReadRequest_unk {
    ReadRequest_unk(LoadCore* core)
        : mCore(core), mSize(0), mBuffer(0), mPos(0), mOffset(0), mDone(0), mPackedSize(0),
          m3C(0), m40(0), mUrgent(0) {}
    LoadCore* mCore;           // 0x00
    unsigned int mSize;        // 0x04
    void* mBuffer;             // 0x08
    int m0C;
    unsigned long long mPos;   // 0x10: position of the file on disc
    unsigned int mOffset;      // 0x18: offset in the file
    unsigned int mDone;        // 0x1C: bytes read
    int mPackedSize;           // 0x20: nonzero when compressed
    Decoder_unk mDecoder;      // 0x24
    int m3C;
    int m40;
    int mUrgent;               // 0x44: .pk files are read first
    int mEntry;                // 0x48
};

struct ReadSlot_unk;

// inferred: reader (0x1C bytes, global 0x8087F510)
struct Reader_unk {
    static Reader_unk* Get_unk(); // inferred: creates the reader the first time
    ReadSlot_unk* GetFree_unk();

    std::list<ReadRequest_unk*> mList; // 0x00
    int mActive;                       // 0x0C
    ReadSlot_unk* mSlots;              // 0x10
    int mChunkKB;                      // 0x14
    int m18;
};

// inferred: read slot (0x400B8 bytes): one open file with its requests and a 256 KB buffer
struct ReadSlot_unk {
    ReadSlot_unk();
    void Start_unk(Reader_unk* reader, ReadRequest_unk* req);

    int mState;           // 0x00: 0 free, 1 reading, 2 read
    void* mDst;           // 0x04
    unsigned int mLength; // 0x08
    unsigned int mPos;    // 0x0C
    int mResult;          // 0x10
    DVDFileInfo_unk mFile; // 0x14
    FixedArray_unk<ReadRequest_unk*, 0x10> mRequests; // 0x50
    unsigned char mBuf[0x40020];
    void* mAligned; // 0x400B4
};

} // namespace atn

using namespace atn;

// inferred: the reader, created on first use
extern "C" {
Reader_unk* lbl_8087F510;
}

// inferred: normal member function autoinlined in its two callers; its out-of-line copy is not in
// the DOL (removed by the linker) but it must come first: its "" literals open the file pool
// ("" before ".pk", 0x80755608).
Reader_unk* Reader_unk::Get_unk() {
    if (lbl_8087F510 == 0) {
        Reader_unk* reader = (Reader_unk*)operator new(sizeof(Reader_unk), 1, "", "", 0);
        if (reader != 0) {
            reader->mList.mSize = 0;
            std::__list_node_base_unk* tail = &reader->mList.mTail;
            tail->mNext = tail;
            tail->mPrev = tail;
            reader->mActive = 0;
            reader->mChunkKB = 0x100;
            reader->m18 = 0x40;
            reader->mSlots = new (2, "", "", 0) ReadSlot_unk[2];
        }
        lbl_8087F510 = reader;
    }
    return lbl_8087F510;
}

ReadSlot_unk::ReadSlot_unk() {
    mState = 0;
    mDst = 0;
    mLength = 0;
    mPos = 0;
    mResult = 0;
    mRequests.mSize = 0;
    mAligned = (void*)(((unsigned int)mBuf + 0x1F) & ~0x1F);
}

extern "C" void fn_8046BBE0(int result, DVDFileInfo_unk* file) {
    ((ReadSlot_unk*)file->mBlock.mUserData)->mState = 2;
}

extern "C" void fn_8046BF88(Reader_unk* reader);

inline void ReadSlot_unk::Start_unk(Reader_unk* reader, ReadRequest_unk* req) {
    unsigned int length;
    void* dst;
    if (req->mPackedSize != 0) {
        dst = mAligned;
    } else {
        dst = (char*)req->mBuffer + req->mDone;
    }
    if (req->mPackedSize > 0) {
        length = reader->mChunkKB << 10;
        if (req->mPackedSize - req->mDone < length) {
            length = req->mPackedSize - req->mDone;
        }
    } else {
        length = req->mSize - req->mDone;
    }
    unsigned int pos = req->mOffset + req->mDone;
    mState = 1;
    mLength = length;
    mDst = dst;
    mPos = pos;
    mResult = 0;
    DVDFastOpen(mRequests[0]->mEntry, &mFile);
    mFile.mBlock.mUserData = this;
    if (DVDReadAsyncPrio(&mFile, mDst, (mLength + 0x1F) & ~0x1F, mPos, fn_8046BBE0, 2) == 0) {
        req->mCore->fn_80473104(4);
        delete req;
        mState = 0;
        mDst = 0;
        DVDClose(&mFile);
        reader->mActive--;
    }
}

template <class T, int N>
inline void Remove_unk(FixedArray_unk<T, N>& a, T* pos) {
    for (int i = pos - &a[0]; i < a.mSize - 1; i++) {
        a[i] = a[i + 1];
    }
    a.mSize--;
}

// NONMATCHING: 99.95% - only the register of slot->mLength in the first comparison (r5 in the
// original, r0 here)
extern "C" void fn_8046BBF0(Reader_unk* reader, ReadSlot_unk* slot) {
    ReadRequest_unk** it = &slot->mRequests[0];
    while (it != &slot->mRequests[slot->mRequests.mSize]) {
        ReadRequest_unk* req = *it;
        void* src = slot->mDst;
        int done;
        if (req->mPackedSize != 0) {
            if (slot->mResult != -3) {
                if (slot->mLength >= req->mPackedSize && req->mDone == 0) {
                    fn_806254D0(src, req->mBuffer);
                } else {
                    fn_80625110(&req->mDecoder, src);
                }
            }
            unsigned int n = slot->mLength;
            if (req->mPackedSize < n) {
                n = req->mPackedSize;
            }
            req->mDone += n;
            done = req->mDone >= req->mPackedSize;
            if (done && req->mPackedSize != 0) {
                DCFlushRangeNoSync(req->mBuffer, req->mSize);
            }
        } else {
            unsigned int n = slot->mLength;
            if (req->mSize < n) {
                n = req->mSize;
            }
            req->mDone += n;
            done = req->mDone == req->mSize;
        }
        if (done) {
            Remove_unk(slot->mRequests, it);
            int result = slot->mResult;
            ((char*)req->mBuffer)[req->mSize] = 0;
            if (result == -3) {
                req->mCore->fn_80473104(3);
            } else if (result < 0) {
                req->mCore->fn_80473104(4);
            } else {
                req->mCore->fn_80473104(2);
            }
            delete req;
        } else {
            it++;
        }
    }
    if (slot->mRequests.size() == 0) {
        slot->mState = 0;
        slot->mDst = 0;
        DVDClose(&slot->mFile);
        reader->mActive--;
        fn_8046BF88(reader);
    } else {
        slot->Start_unk(reader, slot->mRequests[0]);
    }
}

// inferred: next request: the first urgent one, else the one nearest the start of the disc
// NONMATCHING: 93.78% - registers only (the original keeps the loop iterator in r4, the register of
// `reader`). The original returns the iterator through a hidden pointer (r3) that it writes each
// time; returned by value here the iterator goes back in r3, so the out pointer is explicit.
extern "C" void fn_8046BEE4(std::list<ReadRequest_unk*>::iterator* best, Reader_unk* reader) {
    std::list<ReadRequest_unk*>::iterator end = reader->mList.end();
    *best = reader->mList.begin();
    unsigned long long bestPos = (**best)->mPos + (**best)->mOffset;
    std::list<ReadRequest_unk*>::iterator it = reader->mList.begin();
    for (++it; it != end; ++it) {
        ReadRequest_unk* req = *it;
        unsigned long long pos = req->mPos + req->mOffset;
        if (req->mUrgent != 0) {
            *best = it;
            return;
        }
        if (pos < bestPos && req->mCore != 0 && req->mCore->mRefCount > 0) {
            *best = it;
            bestPos = pos;
        }
    }
}

inline ReadSlot_unk* Reader_unk::GetFree_unk() {
    for (int i = 0; i < 2; i++) {
        if (mSlots[i].mState == 0) {
            mActive++;
            return &mSlots[i];
        }
    }
    return 0;
}

// inferred: list erase
template <class T>
inline void Erase_unk(std::list<T>& list, typename std::list<T>::iterator pos) {
    std::__list_node_base_unk* prev = pos.mNode->mPrev;
    std::__list_node_base_unk* next = pos.mNode->mNext;
    prev->mNext = next;
    next->mPrev = pos.mNode->mPrev;
    delete pos.mNode;
    list.mSize--;
}

// NONMATCHING: 98.05% - registers only: the original loads the iterator twice from the stack for the
// erase, and swaps r6/r7 (bytes done/length) in the inlined Start_unk
extern "C" void fn_8046BF88(Reader_unk* reader) {
    while (reader->mList.size() != 0) {
        std::list<ReadRequest_unk*>::iterator it;
        fn_8046BEE4(&it, reader);
        ReadRequest_unk* req = *it;
        Erase_unk(reader->mList, it);
        LoadCore* core = req->mCore;
        if (core->mRefCount == 0 && strstr(core->mPath, ".pk") == 0) {
            core->m05 = 1;
            ((char*)req->mBuffer)[req->mSize] = 0;
            req->mCore->fn_80473104(3);
            delete req;
            continue;
        }
        ReadSlot_unk* slot = reader->GetFree_unk();
        slot->mRequests.push_back(req);
        slot->Start_unk(reader, req);
        return;
    }
}

// NONMATCHING: 98.73% - registers only (pool base/reader and loop index/offset swapped)
extern "C" void fn_8046C1C8() {
    Reader_unk* reader = Reader_unk::Get_unk();
    for (int i = 0; i < 2; i++) {
        if (reader->mSlots[i].mState == 2) {
            fn_8046BBF0(reader, &reader->mSlots[i]);
        }
    }
}

extern "C" void fn_8046C2D4(Loader* mgr, PfsArchive_unk* archive, LoadCore* core, const char* path,
                            int index) {
    archive->fn_8046FD1C(index);
    unsigned int size = archive->fn_8046FD1C(index)->mSize;
    unsigned int packed = archive->fn_8046FD1C(index)->mPackedSize;
    unsigned int alloc = (size + 0x20) & ~0x1F;
    void* buf = 0;
    if (archive->m70 == 0) {
        buf = fn_800839EC(fn_800827E0(), alloc, 0x20, 2, "", "", 0, 0);
    }
    void* src = archive->fn_8046FD2C(index);
    if (packed != 0) {
        fn_806254D0(src, buf);
        DCFlushRangeNoSync(buf, alloc);
    } else if (archive->m70 == 0) {
        memcpy(buf, src, size);
    } else {
        buf = src;
    }
    if (archive->m70 == 0) {
        ((char*)buf)[size] = 0;
    }
    core->m54 = buf;
    core->m50 = (void*)size;
    core->fn_80473104(2);
    if (archive->m70 != 0) {
        core->m58 = (int)archive;
    }
}

// inferred: looks for `path` in the mounted archives
inline void FindMounted_unk(Loader* mgr, const char* path, ReadRequest_unk* req, int& index,
                            unsigned int& offset, unsigned int& size, unsigned int& packed,
                            unsigned long long& pos) {
    for (std::list<PfsArchive_unk*>::iterator it = mgr->mMounted.begin(); it != mgr->mMounted.end(); ++it) {
        index = (*it)->fn_8046FBB0(path);
        if (index != -1) {
            req->mEntry = (*it)->mEntry;
            offset = (*it)->fn_8046FD1C(index)->mOffset;
            size = (*it)->fn_8046FD1C(index)->mSize;
            packed = (*it)->fn_8046FD1C(index)->mPackedSize;
            DVDFileInfo_unk file;
            DVDFastOpen(req->mEntry, &file);
            pos = (unsigned long long)file.mStartAddr << 2;
            DVDClose(&file);
            break;
        }
    }
}

// inferred: position and size of a file on disc
inline void GetFileInfo_unk(int entry, unsigned long long& pos, unsigned int& size) {
    DVDFileInfo_unk file;
    DVDFastOpen(entry, &file);
    pos = (unsigned long long)file.mStartAddr << 2;
    size = file.mLength;
    DVDClose(&file);
}

// inferred: the archive data is loaded
inline int IsLoaded_unk(PfsArchive_unk* archive) {
    int loaded = 0;
    if (archive->mData.fn_80473EFC() == 3 && archive->mData.fn_80470580() != 0) {
        loaded = 1;
    }
    return loaded;
}

// NONMATCHING: 99.14% - registers only: list iterator/end of the archive loops swapped, the
// std::list insert of std/list.h (r4/r5, as in LoadColliTree), r6/r7 in the inlined Start_unk.
// The second "Memory allocation failure" (std::list<ReadRequest_unk*>) is the weak string of an
// earlier file in the original (0x80779810, shared by the LH_* lists): the list holds the same
// type as those (probably std::list<LoadCore*> or a void* list), not reconstructed.
int Loader::fn_8046C3FC(LoadCore* core) {
    int index = -1;
    const char* path = core->mPath;
    ReadRequest_unk* req;
    unsigned int size = 0;
    unsigned int packed = 0;
    unsigned int offset = 0;
    unsigned long long pos = 0;
    int search = 1;
    if (m3ED8 != 0 && fn_805F9EF0(path) >= 0) {
        search = 0;
    }
    if (search) {
        for (std::list<PfsArchive_unk*>::iterator it = mArchives.begin(); it != mArchives.end(); ++it) {
            int i = (*it)->fn_8046FBB0(path);
            if (i != -1) {
                if (!IsLoaded_unk(*it)) {
                    if ((*it)->fn_80470264() == 0) {
                        continue;
                    }
                    mPending.insert(mPending.begin(), PendingLoad_unk(*it, core, path, i));
                    return 1;
                }
                fn_8046C2D4(this, *it, core, path, i);
                return 1;
            }
        }
    }
    req = new (2, "", "", 0) ReadRequest_unk(core);
    if (m3ED8 == 0) {
        FindMounted_unk(this, path, req, index, offset, size, packed, pos);
    }
    if (index == -1) {
        req->mEntry = fn_805F9EF0(path);
        if (req->mEntry == -1) {
            if (m3ED8 != 0) {
                FindMounted_unk(this, path, req, index, offset, size, packed, pos);
            }
            if (index == -1) {
                delete req;
                return 0;
            }
        }
        if (index == -1) {
            GetFileInfo_unk(req->mEntry, pos, size);
        }
    }
    req->mOffset = offset;
    req->mPackedSize = packed;
    unsigned int alloc = (size + 0x20) & ~0x1F;
    req->mUrgent = strstr(path, ".pk") != 0;
    req->mBuffer = fn_800839EC(fn_800827E0(), alloc, 0x20, 2, "", "", 0, 1);
    if (req->mBuffer == 0) {
        delete req;
        return 0;
    }
    DCZeroRange(req->mBuffer, alloc);
    req->mSize = size;
    req->mPos = pos;
    core->m54 = req->mBuffer;
    core->m50 = (void*)size;
    if (req->mPackedSize != 0) {
        fn_806250D0(&req->mDecoder, req->mBuffer);
    }
    Reader_unk* reader = Reader_unk::Get_unk();
    if (reader->mActive < 2) {
        ReadSlot_unk* slot = reader->GetFree_unk();
        slot->mRequests.push_back(req);
        slot->Start_unk(reader, req);
    } else {
        reader->mList.push_back(req);
    }
    return 1;
}
