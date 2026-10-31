#ifndef NW4R_SND_DVDSOUNDARCHIVE_GAME_H
#define NW4R_SND_DVDSOUNDARCHIVE_GAME_H

// nw4r::snd::DvdSoundArchive as linked in the game area (layout as
// libs/nw4r/include/nw4r/snd/snd_DvdSoundArchive.h).
#include "nw4r/snd/SoundArchive.h"
#include "nw4r/ut/DvdLockedFileStream.h"

namespace nw4r {
namespace snd {

class DvdSoundArchive : public SoundArchive {
public:
    class DvdFileStream;

    DvdSoundArchive();
    virtual ~DvdSoundArchive(); // at 0x8

    virtual const void* detail_GetFileAddress(unsigned long id) const { return 0; }         // at 0xC
    virtual const void* detail_GetWaveDataFileAddress(unsigned long id) const { return 0; } // at 0x10
    virtual int detail_GetRequiredStreamBufferSize() const;                                 // at 0x14
    virtual ut::FileStream* OpenStream(void* pBuffer, int size, unsigned long offset,
                                       unsigned long length) const;                         // at 0x18
    virtual ut::FileStream* OpenExtStream(void* pBuffer, int size, const char* pExtPath,
                                          unsigned long offset, unsigned long length) const; // at 0x1C

    bool Open(long entrynum);
    bool Open(const char* pPath);
    void Close();

    bool LoadHeader(void* pBuffer, unsigned long size);
    bool LoadLabelStringData(void* pBuffer, unsigned long size);

private:
    bool LoadFileHeader();

    detail::SoundArchiveFileReader mFileReader; // at 0x108
    DVDFileInfo mFileInfo;              // at 0x14C
    bool mOpen;                                 // at 0x188
};

} // namespace snd
} // namespace nw4r

#endif
