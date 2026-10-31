#ifndef NW4R_UT_DVDLOCKEDFILESTREAM_GAME_H
#define NW4R_UT_DVDLOCKEDFILESTREAM_GAME_H

// nw4r::ut::DvdFileStream and DvdLockedFileStream as used by the NW4R code linked in the game
// area (layout and virtual order as libs/nw4r/include/nw4r/ut/ut_DvdFileStream.h and
// ut_DvdLockedFileStream.h; this build has the asserts enabled).
#include "nw4r/ut/FileStream.h"

namespace nw4r {
namespace ut {

class DvdFileStream : public FileStream {
public:
    explicit DvdFileStream(long entrynum);
    DvdFileStream(const DVDFileInfo* pInfo, bool close);
    virtual ~DvdFileStream(); // at 0xC

    virtual void Close();                                    // at 0x10
    virtual long Read(void* pDst, unsigned long size);       // at 0x14
    virtual bool ReadAsync(void* pDst, unsigned long size, StreamCallback pCallback,
                           void* pCallbackArg);              // at 0x18
    virtual long Peek(void* pDst, unsigned long size);       // at 0x5C
    virtual bool PeekAsync(void* pDst, unsigned long size, StreamCallback pCallback,
                           void* pCallbackArg);              // at 0x60
    virtual void Seek(long offset, unsigned long origin);    // at 0x44
    virtual void Cancel();                                   // at 0x48
    virtual bool CancelAsync(StreamCallback pCallback, void* pCallbackArg); // at 0x4C
    virtual bool IsBusy() const { return mIsBusy; }          // at 0x24
    virtual unsigned long Tell() const { return mFilePosition.Tell(); }        // at 0x58
    virtual unsigned long GetSize() const { return mFilePosition.GetFileSize(); } // at 0x40
    virtual bool CanAsync() const { return true; }           // at 0x28
    virtual bool CanSeek() const { return true; }            // at 0x50
    virtual bool CanRead() const { return true; }            // at 0x2C
    virtual bool CanWrite() const { return false; }          // at 0x30
    virtual bool CanCancel() const { return true; }          // at 0x54
    virtual unsigned long GetOffsetAlign() const { return 4; }  // at 0x34
    virtual unsigned long GetSizeAlign() const { return 32; }   // at 0x38
    virtual unsigned long GetBufferAlign() const { return 32; } // at 0x3C

private:
    FilePosition mFilePosition;     // at 0x14
    StreamCallback mCancelCallback; // at 0x1C
    void* mCancelArg;               // at 0x20
    volatile bool mIsCanceling;     // at 0x24
    unsigned char mFileInfo[0x40];  // at 0x28 (DVDFileInfo and the stream)
    long mPriority;                 // at 0x68
    volatile bool mIsBusy;          // at 0x6C
    bool mCloseOnDestroyFlg;        // at 0x6D
    bool mCloseEnableFlg;           // at 0x6E
};

class DvdLockedFileStream : public DvdFileStream {
public:
    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const { return &typeInfo; } // at 0x8
    static detail::RuntimeTypeInfo typeInfo;

    explicit DvdLockedFileStream(long entrynum);
    DvdLockedFileStream(const DVDFileInfo* pInfo, bool close);
    virtual ~DvdLockedFileStream(); // at 0xC

    virtual void Close();                              // at 0x10
    virtual long Read(void* pDst, unsigned long size); // at 0x14
    virtual bool ReadAsync(void* pDst, unsigned long size, StreamCallback pCallback,
                           void* pCallbackArg) {
        if (!CanAsync()) {
            db::Panic("DvdLockedFileStream.h", 53, "NW4R:Failed assertion this->CanAsync()");
        }
        return false;
    } // at 0x18
    virtual long Peek(void* pDst, unsigned long size); // at 0x5C
    virtual bool PeekAsync(void* pDst, unsigned long size, StreamCallback pCallback,
                           void* pCallbackArg) {
        if (!CanAsync()) {
            db::Panic("DvdLockedFileStream.h", 58, "NW4R:Failed assertion this->CanAsync()");
        }
        return false;
    } // at 0x60
    virtual void Cancel();                  // at 0x48
    virtual bool CanAsync() const { return false; } // at 0x28

private:
    bool mCancelFlag; // at 0x6F
};

} // namespace ut
} // namespace nw4r

#endif
