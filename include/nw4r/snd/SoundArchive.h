#ifndef NW4R_SND_SOUNDARCHIVE_GAME_H
#define NW4R_SND_SOUNDARCHIVE_GAME_H

// nw4r::snd::SoundArchive as linked in the game area (0x8059E21C): the NW4R version of the
// library (layout as libs/nw4r/include/nw4r/snd/snd_SoundArchive.h) with the asserts enabled.
// The game include path does not reach libs/nw4r, so only what SoundArchive.cpp needs is declared.

namespace nw4r {
namespace db {
void Panic(const char* file, int line, const char* fmt, ...);
void Warning(const char* file, int line, const char* msg, ...);
} // namespace db

namespace ut {
class FileStream;
} // namespace ut

namespace snd {

class SoundArchive;

// inferred: SoundType of NW4R (only passed through here)
enum SoundType {
    SOUND_TYPE_INVALID,
    SOUND_TYPE_SEQ,
    SOUND_TYPE_STRM,
    SOUND_TYPE_WAVE
};

namespace SoundArchiveFile {
struct StringTree;
} // namespace SoundArchiveFile

namespace detail {

class SoundArchiveFileReader;

} // namespace detail

class SoundArchive {
public:
    struct SoundInfo;
    struct SeqSoundInfo;
    struct StrmSoundInfo;
    struct WaveSoundInfo;
    struct Sound3DParam;
    struct BankInfo;
    struct PlayerInfo;
    struct SoundArchivePlayerInfo;

    struct GroupInfo {
        unsigned long itemCount;      // at 0x0
        const char* extFilePath;      // at 0x4
        unsigned long offset;         // at 0x8
        unsigned long size;           // at 0xC
        unsigned long waveDataOffset; // at 0x10
        unsigned long waveDataSize;   // at 0x14
    };
    struct GroupItemInfo {
        unsigned long fileId;         // at 0x0
        unsigned long offset;         // at 0x4
        unsigned long size;           // at 0x8
        unsigned long waveDataOffset; // at 0xC
        unsigned long waveDataSize;   // at 0x10
    };
    struct FileInfo {
        unsigned long fileSize;         // at 0x0
        unsigned long waveDataFileSize; // at 0x4
        const char* extFilePath;        // at 0x8
        unsigned long filePosCount;     // at 0xC
    };
    struct FilePos {
        unsigned long groupId; // at 0x0
        unsigned long index;   // at 0x4
    };

    SoundArchive();
    virtual ~SoundArchive();
    virtual const void* detail_GetFileAddress(unsigned long id) const = 0;
    virtual const void* detail_GetWaveDataFileAddress(unsigned long id) const = 0;
    virtual int detail_GetRequiredStreamBufferSize() const = 0;
    virtual ut::FileStream* OpenStream(void* pBuffer, int bufferSize, unsigned long offset,
                                       unsigned long length) const = 0;
    virtual ut::FileStream* OpenExtStream(void* pBuffer, int bufferSize, const char* pExtPath,
                                          unsigned long offset, unsigned long length) const = 0;

    bool IsAvailable() const;
    void Setup(detail::SoundArchiveFileReader* fileReader);
    void Shutdown();

    unsigned long GetPlayerCount() const;
    unsigned long GetGroupCount() const;
    const char* GetSoundLabelString(unsigned long id) const;
    unsigned long ConvertLabelStringToSoundId(const char* pLabel) const;
    unsigned long ConvertLabelStringToPlayerId(const char* pLabel) const;
    unsigned long ConvertLabelStringToGroupId(const char* pLabel) const;
    unsigned long GetSoundUserParam(unsigned long id) const;
    SoundType GetSoundType(unsigned long id) const;

    bool ReadSoundInfo(unsigned long id, SoundInfo* pInfo) const;
    bool detail_ReadSeqSoundInfo(unsigned long id, SeqSoundInfo* pInfo) const;
    bool detail_ReadStrmSoundInfo(unsigned long id, StrmSoundInfo* pInfo) const;
    bool detail_ReadWaveSoundInfo(unsigned long id, WaveSoundInfo* pInfo) const;
    bool ReadPlayerInfo(unsigned long id, PlayerInfo* pInfo) const;
    bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pInfo) const;
    bool detail_ReadSound3DParam(unsigned long id, Sound3DParam* pParam) const;
    bool detail_ReadBankInfo(unsigned long id, BankInfo* pInfo) const;
    bool detail_ReadGroupInfo(unsigned long id, GroupInfo* pInfo) const;
    bool detail_ReadGroupItemInfo(unsigned long groupId, unsigned long itemId,
                                  GroupItemInfo* pInfo) const;
    unsigned long detail_GetFileCount() const;
    bool detail_ReadFileInfo(unsigned long id, FileInfo* pInfo) const;
    bool detail_ReadFilePos(unsigned long fileId, unsigned long posId, FilePos* pPos) const;

    ut::FileStream* detail_OpenFileStream(unsigned long id, void* pBuffer, int bufferSize) const;
    ut::FileStream* detail_OpenGroupStream(unsigned long id, void* pBuffer, int bufferSize) const;
    ut::FileStream* detail_OpenGroupWaveDataStream(unsigned long id, void* pBuffer,
                                                   int bufferSize) const;

    void SetExternalFileRoot(const char* pExtFileRoot);

protected:
    static const int FILE_PATH_MAX = 256;

private:
    ut::FileStream* OpenExtStreamImpl(void* pBuffer, int bufferSize, const char* pExtPath,
                                      unsigned long offset, unsigned long size) const;

    detail::SoundArchiveFileReader* mFileReader; // at 0x4
    char mExtFileRoot[FILE_PATH_MAX];            // at 0x8
};

namespace detail {

// inferred: layout as libs/nw4r (string trees at 0x34-0x40); only the members used here
class SoundArchiveFileReader {
public:
    SoundType GetSoundType(unsigned long id) const;
    bool ReadSoundInfo(unsigned long id, SoundArchive::SoundInfo* pInfo) const;
    bool ReadSound3DParam(unsigned long id, SoundArchive::Sound3DParam* pParam) const;
    bool ReadSeqSoundInfo(unsigned long id, SoundArchive::SeqSoundInfo* pInfo) const;
    bool ReadStrmSoundInfo(unsigned long id, SoundArchive::StrmSoundInfo* pInfo) const;
    bool ReadWaveSoundInfo(unsigned long id, SoundArchive::WaveSoundInfo* pInfo) const;
    bool ReadBankInfo(unsigned long id, SoundArchive::BankInfo* pInfo) const;
    bool ReadPlayerInfo(unsigned long id, SoundArchive::PlayerInfo* pInfo) const;
    bool ReadGroupInfo(unsigned long id, SoundArchive::GroupInfo* pInfo) const;
    bool ReadGroupItemInfo(unsigned long groupId, unsigned long itemId,
                           SoundArchive::GroupItemInfo* pInfo) const;
    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* pInfo) const;
    unsigned long GetPlayerCount() const;
    unsigned long GetGroupCount() const;
    unsigned long GetFileCount() const;
    const char* GetSoundLabelString(unsigned long id) const;
    unsigned long GetSoundUserParam(unsigned long id) const;
    bool ReadFileInfo(unsigned long id, SoundArchive::FileInfo* pInfo) const;
    bool ReadFilePos(unsigned long fileId, unsigned long id, SoundArchive::FilePos* pPos) const;

    unsigned long ConvertLabelStringToSoundId(const char* pLabel) const {
        return ConvertLabelStringToId(mStringTreeSound, pLabel);
    }
    unsigned long ConvertLabelStringToPlayerId(const char* pLabel) const {
        return ConvertLabelStringToId(mStringTreePlayer, pLabel);
    }
    unsigned long ConvertLabelStringToGroupId(const char* pLabel) const {
        return ConvertLabelStringToId(mStringTreeGroup, pLabel);
    }

private:
    unsigned long ConvertLabelStringToId(const SoundArchiveFile::StringTree* pTree,
                                         const char* pLabel) const;

    unsigned char m00[0x34];
    const SoundArchiveFile::StringTree* mStringTreeSound;  // at 0x34
    const SoundArchiveFile::StringTree* mStringTreePlayer; // at 0x38
    const SoundArchiveFile::StringTree* mStringTreeGroup;  // at 0x3C
    const SoundArchiveFile::StringTree* mStringTreeBank;   // at 0x40
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
