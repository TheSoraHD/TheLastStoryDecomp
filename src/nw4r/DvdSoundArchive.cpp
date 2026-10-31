// nw4r/DvdSoundArchive.cpp: nw4r::snd::DvdSoundArchive (snd_DvdSoundArchive.cpp of NW4R, asserts
// enabled). The game changed LoadFileHeader and LoadHeader to read asynchronously with the
// callback of atn::AudioDevice and to wait for it (see atn/AudioDevice.cpp).
#include "nw4r/snd/DvdSoundArchive.h"
#include "std/new.h"

extern "C" unsigned long strlen(const char* s);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" int DVDFastOpen(long entrynum, DVDFileInfo* pInfo);
extern "C" int DVDClose(DVDFileInfo* pInfo);
extern "C" long DVDReadPrio(DVDFileInfo* pInfo, void* pDst, long size, long offset,
                            long prio);
extern "C" int DVDReadAsyncPrio(DVDFileInfo* pInfo, void* pDst, long size,
                                long offset, void (*pCallback)(long, DVDFileInfo*),
                                long prio);
extern "C" long fn_805F9EF0(const char* path); // inferred: DVDConvertPathToEntrynum

// inferred: atn::AudioDevice: read callback (sets the flag and the result), the wait step and
// the flag and result it writes (see atn/AudioDevice.cpp)
extern "C" void fn_80046398(long result, DVDFileInfo* pInfo);
extern "C" void fn_80046228();
extern "C" int lbl_8087EE80;
extern "C" int lbl_8087EE84;

namespace nw4r {
namespace snd {

class DvdSoundArchive::DvdFileStream : public ut::DvdLockedFileStream {
public:
    DvdFileStream(const DVDFileInfo* pFileInfo, unsigned long offset, unsigned long size);
    DvdFileStream(long entrynum, unsigned long offset, unsigned long size);

    virtual long Read(void* pDst, unsigned long size);   // at 0x14
    virtual void Seek(long offset, unsigned long origin); // at 0x44

    virtual unsigned long Tell() const { return ut::DvdFileStream::Tell() - mOffset; } // at 0x58
    virtual unsigned long GetSize() const { return mSize; }                          // at 0x40

private:
    long mOffset; // at 0x70
    long mSize;   // at 0x74
};

DvdSoundArchive::DvdSoundArchive() : mOpen(false) {}

DvdSoundArchive::~DvdSoundArchive() {
    Close();
}

bool DvdSoundArchive::Open(long entrynum) {
    if (!DVDFastOpen(entrynum, &mFileInfo)) {
        db::Warning("snd_DvdSoundArchive.cpp", 78, "Cannot open file %d\n", entrynum);
        return false;
    }

    mOpen = true;
    if (!LoadFileHeader()) {
        db::Warning("snd_DvdSoundArchive.cpp", 86, "Cannot load header\n");
        return false;
    }
    return true;
}

bool DvdSoundArchive::Open(const char* pPath) {
    long entrynum = fn_805F9EF0(pPath);
    if (entrynum < 0) {
        db::Warning("snd_DvdSoundArchive.cpp", 107, "Cannot find file %s\n", pPath);
        return false;
    }

    if (!Open(entrynum)) {
        return false;
    }

    char extRoot[FILE_PATH_MAX];
    for (int i = strlen(pPath) - 1; i >= 0; i--) {
        if (pPath[i] == '/' || pPath[i] == '\\') {
            if (!(i < FILE_PATH_MAX)) {
                db::Panic("snd_DvdSoundArchive.cpp", 123, "NW4R:Failed assertion i < FILE_PATH_MAX");
            }
            if (i >= FILE_PATH_MAX) {
                return false;
            }
            strncpy(extRoot, pPath, i);
            extRoot[i] = '\0';

            SetExternalFileRoot(extRoot);
            break;
        }
    }

    return true;
}

void DvdSoundArchive::Close() {
    if (mOpen) {
        DVDClose(&mFileInfo);
        mOpen = false;
    }
    Shutdown();
}

ut::FileStream* DvdSoundArchive::OpenStream(void* pBuffer, int size, unsigned long offset,
                                            unsigned long length) const {
    if (!mOpen) {
        return 0;
    }

    if (size < sizeof(DvdFileStream)) {
        return 0;
    }

    return new (pBuffer) DvdFileStream(&mFileInfo, offset, length);
}

ut::FileStream* DvdSoundArchive::OpenExtStream(void* pBuffer, int size, const char* pExtPath,
                                               unsigned long offset, unsigned long length) const {
    if (!mOpen) {
        return 0;
    }

    if (size < sizeof(DvdFileStream)) {
        return 0;
    }

    long entrynum = fn_805F9EF0(pExtPath);
    if (entrynum < 0) {
        db::Warning("snd_DvdSoundArchive.cpp", 174, "OpenExtStream: Cannot open file \"%s\"", pExtPath);
        return 0;
    }

    return new (pBuffer) DvdFileStream(entrynum, offset, length);
}

int DvdSoundArchive::detail_GetRequiredStreamBufferSize() const {
    return sizeof(DvdFileStream);
}

bool DvdSoundArchive::LoadFileHeader() {
    if (!mOpen) {
        db::Panic("snd_DvdSoundArchive.cpp", 198, "NW4R:Failed assertion mOpen");
    }

    unsigned char headerArea[0x68];
    void* pFile = (void*)(((unsigned long)headerArea + 31) & ~31);

    // inferred: 0x40 = sizeof(SoundArchiveFile::Header) (0x28) rounded up to 32
    DVDReadAsyncPrio(&mFileInfo, pFile, 0x40, 0, fn_80046398, 2);
    lbl_8087EE80 = 0;
    long bytesRead;
    while (true) {
        if (lbl_8087EE80 != 0) {
            bytesRead = lbl_8087EE84;
            break;
        }
        fn_80046228();
    }

    if (bytesRead != 0x40u) {
        db::Warning("snd_DvdSoundArchive.cpp", 232, "DvdSoundArchive::LoadFileHeader cannot read file.\n");
        return false;
    }

    mFileReader.Init(pFile);
    Setup(&mFileReader);

    return true;
}

bool DvdSoundArchive::LoadHeader(void* pBuffer, unsigned long size) {
    if (!mOpen) {
        db::Panic("snd_DvdSoundArchive.cpp", 254, "NW4R:Failed assertion mOpen");
    }

    unsigned long infoSize = mFileReader.GetInfoChunkSize();
    long infoOffset = mFileReader.GetInfoChunkOffset();

    if (size < infoSize) {
        if (!(size >= infoSize)) {
            db::Warning("snd_DvdSoundArchive.cpp", 264, "DvdSoundArchive::LoadHeader buffer size is too small.\n");
        }
        return false;
    }

    DVDReadAsyncPrio(&mFileInfo, pBuffer, infoSize, infoOffset, fn_80046398, 2);
    lbl_8087EE80 = 0;
    long bytesRead;
    while (true) {
        if (lbl_8087EE80 != 0) {
            bytesRead = lbl_8087EE84;
            break;
        }
        fn_80046228();
    }

    if (bytesRead != infoSize) {
        db::Warning("snd_DvdSoundArchive.cpp", 293, "DvdSoundArchive::LoadHeader cannot read file.\n");
        return false;
    }

    mFileReader.SetInfoChunk(pBuffer, infoSize);
    return true;
}

bool DvdSoundArchive::LoadLabelStringData(void* pBuffer, unsigned long size) {
    if (!mOpen) {
        db::Panic("snd_DvdSoundArchive.cpp", 314, "NW4R:Failed assertion mOpen");
    }

    unsigned long labelSize = mFileReader.GetLabelStringChunkSize();
    long labelOffset = mFileReader.GetLabelStringChunkOffset();

    if (size < labelSize) {
        if (!(size >= labelSize)) {
            db::Warning("snd_DvdSoundArchive.cpp", 324, "DvdSoundArchive::LoadLabelStringData buffer size is too small.");
        }
        return false;
    }

    long bytesRead = DVDReadPrio(&mFileInfo, pBuffer, labelSize, labelOffset, 2);

    if (bytesRead != labelSize) {
        db::Warning("snd_DvdSoundArchive.cpp", 339, "DvdSoundArchive::LoadLabelStringData cannot read file.\n");
        return false;
    }

    mFileReader.SetStringChunk(pBuffer, labelSize);
    return true;
}

DvdSoundArchive::DvdFileStream::DvdFileStream(long entrynum, unsigned long offset,
                                              unsigned long size)
    : DvdLockedFileStream(entrynum), mOffset(offset), mSize(size) {
    if (!(mSize <= ut::DvdLockedFileStream::GetSize())) {
        db::Panic("snd_DvdSoundArchive.cpp", 364,
                  "NW4R:Failed assertion mSize <= ut::DvdLockedFileStream::GetSize()");
    }

    ut::DvdFileStream::Seek(mOffset, SEEK_ORIGIN_BEG);
}

DvdSoundArchive::DvdFileStream::DvdFileStream(const DVDFileInfo* pFileInfo,
                                              unsigned long offset, unsigned long size)
    : DvdLockedFileStream(pFileInfo, false), mOffset(offset), mSize(size) {
    if (!(mSize <= ut::DvdLockedFileStream::GetSize())) {
        db::Panic("snd_DvdSoundArchive.cpp", 374,
                  "NW4R:Failed assertion mSize <= ut::DvdLockedFileStream::GetSize()");
    }

    ut::DvdFileStream::Seek(mOffset, SEEK_ORIGIN_BEG);
}

long DvdSoundArchive::DvdFileStream::Read(void* pDst, unsigned long size) {
    if (((unsigned long)pDst & 31) != 0) {
        db::Panic("snd_DvdSoundArchive.cpp", 381,
                  "NW4R:Alignment Error(0x%x)\nbuf must be aligned to 32 bytes boundary.", pDst);
    }
    if ((size & 31) != 0) {
        db::Panic("snd_DvdSoundArchive.cpp", 382,
                  "NW4R:Alignment Error(0x%x)\nlength must be aligned to 32 bytes boundary.", size);
    }

    unsigned long endOffset = mOffset + mSize;
    unsigned long startOffset = ut::DvdFileStream::Tell();

    if (startOffset + size > endOffset) {
        size = (endOffset - ut::DvdFileStream::Tell() + 31) & ~31;
    }

    return DvdLockedFileStream::Read(pDst, size);
}

void DvdSoundArchive::DvdFileStream::Seek(long offset, unsigned long origin) {
    switch (origin) {
    case SEEK_ORIGIN_BEG:
        offset += mOffset;
        break;
    case SEEK_ORIGIN_CUR:
        offset += ut::DvdFileStream::Tell();
        break;
    case SEEK_ORIGIN_END:
        offset = mOffset + mSize - offset;
        break;
    default:
        db::Panic("snd_DvdSoundArchive.cpp", 407, "Unsupported Seek origin");
        return;
    }

    if (offset < mOffset) {
        offset = mOffset;
    } else if (offset > mOffset + mSize) {
        offset = mOffset + mSize;
    }

    ut::DvdFileStream::Seek(offset, SEEK_ORIGIN_BEG);
}

} // namespace snd
} // namespace nw4r
