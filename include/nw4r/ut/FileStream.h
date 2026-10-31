#ifndef NW4R_UT_FILESTREAM_GAME_H
#define NW4R_UT_FILESTREAM_GAME_H

// nw4r::ut::IOStream and FileStream as used by the NW4R code linked in the game area (layout and
// virtual order as libs/nw4r/include/nw4r/ut/ut_IOStream.h and ut_FileStream.h). Only the parts
// used are declared.

// DVDFileInfo of the RVL SDK (0x3C bytes); only its address is used here
struct DVDFileInfo {
    unsigned char m00[0x3C];
};

namespace nw4r {
namespace db {
void Panic(const char* file, int line, const char* fmt, ...);
void Warning(const char* file, int line, const char* msg, ...);
} // namespace db

namespace ut {

namespace detail {
struct RuntimeTypeInfo {
    const RuntimeTypeInfo* mParentTypeInfo; // at 0x0
};
} // namespace detail

class IOStream {
public:
    typedef void (*StreamCallback)(long result, IOStream* pStream, void* pCallbackArg);

    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const; // at 0x8
    virtual ~IOStream() {}                                              // at 0xC
    virtual void Close() = 0;                                           // at 0x10
    virtual long Read(void* pDst, unsigned long size) = 0;              // at 0x14
    virtual bool ReadAsync(void* pDst, unsigned long size, StreamCallback pCallback,
                           void* pCallbackArg);                         // at 0x18
    virtual bool Write(const void* pSrc, unsigned long size);           // at 0x1C
    virtual bool WriteAsync(const void* pSrc, unsigned long size, StreamCallback pCallback,
                            void* pCallbackArg);                        // at 0x20
    virtual bool IsBusy() const;                                        // at 0x24
    virtual bool CanAsync() const = 0;                                  // at 0x28
    virtual bool CanRead() const = 0;                                   // at 0x2C
    virtual bool CanWrite() const = 0;                                  // at 0x30
    virtual unsigned long GetOffsetAlign() const { return 1; }          // at 0x34
    virtual unsigned long GetSizeAlign() const { return 1; }            // at 0x38
    virtual unsigned long GetBufferAlign() const { return 1; }          // at 0x3C

protected:
    bool mAvailable;          // at 0x4
    long mAsyncResult;        // at 0x8
    StreamCallback mCallback; // at 0xC
    void* mArg;               // at 0x10
};

class FileStream : public IOStream {
public:
    enum SeekOrigin { SEEK_ORIGIN_BEG, SEEK_ORIGIN_CUR, SEEK_ORIGIN_END };

    virtual ~FileStream() {}                                         // at 0xC
    virtual unsigned long GetSize() const = 0;                       // at 0x40
    virtual void Seek(long offset, unsigned long origin);            // at 0x44
    virtual void Cancel();                                           // at 0x48
    virtual bool CancelAsync(StreamCallback pCallback, void* pCallbackArg); // at 0x4C
    virtual bool CanSeek() const = 0;                                // at 0x50
    virtual bool CanCancel() const = 0;                              // at 0x54
    virtual unsigned long Tell() const = 0;                          // at 0x58

protected:
    class FilePosition {
    public:
        unsigned long GetFileSize() const { return mFileSize; }
        unsigned long Tell() const { return mPosition; }

    private:
        unsigned long mFileSize; // at 0x0
        unsigned long mPosition; // at 0x4
    };
};

} // namespace ut
} // namespace nw4r

#endif
