// nw4r/SoundArchive.cpp: nw4r::snd::SoundArchive (snd_SoundArchive.cpp of NW4R, asserts enabled).
// The game changed detail_OpenFileStream: the external files marked in the bit table of
// atn::AudioDevice are opened from its stream folder ("stream") instead of their own path, and
// external files are opened with their file size.
#include "nw4r/snd/SoundArchive.h"

extern "C" unsigned long strlen(const char* s);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" char* strncat(char* dst, const char* src, unsigned long n);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char* fn_806825B4(const char* s, int c); // inferred: strrchr

// inferred: atn::AudioDevice: one bit per file id for the files read from the stream folder,
// and that folder (see atn/AudioDevice.cpp)
extern "C" unsigned int* lbl_8087EE88;
extern "C" char* lbl_8087EE8C;

namespace nw4r {
namespace snd {

SoundArchive::SoundArchive() : mFileReader(0) {
    mExtFileRoot[0] = '/';
    mExtFileRoot[1] = '\0';
}

SoundArchive::~SoundArchive() {}

bool SoundArchive::IsAvailable() const {
    return mFileReader != 0;
}

void SoundArchive::Setup(detail::SoundArchiveFileReader* fileReader) {
    if (fileReader == 0) {
        db::Panic("snd_SoundArchive.cpp", 75, "NW4R:Pointer must not be NULL (fileReader)");
    }
    mFileReader = fileReader;
}

void SoundArchive::Shutdown() {
    mFileReader = 0;
    mExtFileRoot[0] = '/';
    mExtFileRoot[1] = '\0';
}

unsigned long SoundArchive::GetPlayerCount() const {
    return mFileReader->GetPlayerCount();
}

unsigned long SoundArchive::GetGroupCount() const {
    return mFileReader->GetGroupCount();
}

const char* SoundArchive::GetSoundLabelString(unsigned long id) const {
    return mFileReader->GetSoundLabelString(id);
}

unsigned long SoundArchive::ConvertLabelStringToSoundId(const char* pLabel) const {
    return mFileReader->ConvertLabelStringToSoundId(pLabel);
}

unsigned long SoundArchive::ConvertLabelStringToGroupId(const char* pLabel) const {
    return mFileReader->ConvertLabelStringToGroupId(pLabel);
}

unsigned long SoundArchive::GetSoundUserParam(unsigned long id) const {
    return mFileReader->GetSoundUserParam(id);
}

SoundType SoundArchive::GetSoundType(unsigned long id) const {
    return mFileReader->GetSoundType(id);
}

bool SoundArchive::ReadSoundInfo(unsigned long id, SoundInfo* pInfo) const {
    return mFileReader->ReadSoundInfo(id, pInfo);
}

bool SoundArchive::detail_ReadSeqSoundInfo(unsigned long id, SeqSoundInfo* pInfo) const {
    return mFileReader->ReadSeqSoundInfo(id, pInfo);
}

bool SoundArchive::detail_ReadStrmSoundInfo(unsigned long id, StrmSoundInfo* pInfo) const {
    return mFileReader->ReadStrmSoundInfo(id, pInfo);
}

bool SoundArchive::detail_ReadWaveSoundInfo(unsigned long id, WaveSoundInfo* pInfo) const {
    return mFileReader->ReadWaveSoundInfo(id, pInfo);
}

bool SoundArchive::ReadPlayerInfo(unsigned long id, PlayerInfo* pInfo) const {
    return mFileReader->ReadPlayerInfo(id, pInfo);
}

bool SoundArchive::ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pInfo) const {
    return mFileReader->ReadSoundArchivePlayerInfo(pInfo);
}

bool SoundArchive::detail_ReadSound3DParam(unsigned long id, Sound3DParam* pParam) const {
    return mFileReader->ReadSound3DParam(id, pParam);
}

bool SoundArchive::detail_ReadBankInfo(unsigned long id, BankInfo* pInfo) const {
    return mFileReader->ReadBankInfo(id, pInfo);
}

bool SoundArchive::detail_ReadGroupInfo(unsigned long id, GroupInfo* pInfo) const {
    return mFileReader->ReadGroupInfo(id, pInfo);
}

bool SoundArchive::detail_ReadGroupItemInfo(unsigned long groupId, unsigned long itemId,
                                            GroupItemInfo* pInfo) const {
    return mFileReader->ReadGroupItemInfo(groupId, itemId, pInfo);
}

unsigned long SoundArchive::detail_GetFileCount() const {
    return mFileReader->GetFileCount();
}

bool SoundArchive::detail_ReadFileInfo(unsigned long id, FileInfo* pInfo) const {
    return mFileReader->ReadFileInfo(id, pInfo);
}

bool SoundArchive::detail_ReadFilePos(unsigned long fileId, unsigned long posId,
                                      FilePos* pPos) const {
    return mFileReader->ReadFilePos(fileId, posId, pPos);
}

ut::FileStream* SoundArchive::detail_OpenFileStream(unsigned long id, void* pBuffer,
                                                    int bufferSize) const {
    FileInfo fileInfo;
    if (!detail_ReadFileInfo(id, &fileInfo)) {
        return 0;
    }

    const char* extPath = fileInfo.extFilePath;
    if (extPath != 0) {
        if (lbl_8087EE88[id >> 5] & (1 << (id & 31))) {
            // inferred: both formats end with an explicit NUL (the pool has two NULs after each)
            char path[FILE_PATH_MAX];
            const char* name = fn_806825B4(extPath, '/');
            if (name != 0) {
                sprintf(path, "%s%s\0", lbl_8087EE8C, name);
            } else {
                sprintf(path, "%s/%s\0", lbl_8087EE8C, extPath);
            }
            return OpenExtStreamImpl(pBuffer, bufferSize, path, 0, fileInfo.fileSize);
        }
        return OpenExtStreamImpl(pBuffer, bufferSize, extPath, 0, fileInfo.fileSize);
    }

    FilePos filePos;
    if (!detail_ReadFilePos(id, 0, &filePos)) {
        return 0;
    }

    GroupInfo groupInfo;
    if (!detail_ReadGroupInfo(filePos.groupId, &groupInfo)) {
        return 0;
    }

    GroupItemInfo groupItemInfo;
    if (!detail_ReadGroupItemInfo(filePos.groupId, filePos.index, &groupItemInfo)) {
        return 0;
    }

    unsigned long offset = groupInfo.offset + groupItemInfo.offset;
    unsigned long size = groupItemInfo.size;

    if (groupInfo.extFilePath != 0) {
        return OpenExtStreamImpl(pBuffer, bufferSize, groupInfo.extFilePath, offset, size);
    }

    return OpenStream(pBuffer, bufferSize, offset, size);
}

ut::FileStream* SoundArchive::OpenExtStreamImpl(void* pBuffer, int bufferSize,
                                                const char* pExtPath, unsigned long offset,
                                                unsigned long size) const {
    char pathBuffer[FILE_PATH_MAX];
    const char* pFullPath;

    if (pExtPath[0] == '/') {
        pFullPath = pExtPath;
    } else {
        unsigned long fileLen = strlen(pExtPath);
        unsigned long dirLen = strlen(mExtFileRoot);

        if (fileLen + dirLen >= FILE_PATH_MAX) {
            db::Warning("snd_SoundArchive.cpp", 366, "Too long file path \"%s/%s\"", mExtFileRoot,
                        pExtPath);
            return 0;
        }

        strncpy(pathBuffer, mExtFileRoot, dirLen + 1);
        strncat(pathBuffer, pExtPath, fileLen + 1);

        pFullPath = pathBuffer;
    }

    return OpenExtStream(pBuffer, bufferSize, pFullPath, offset, size);
}

void SoundArchive::SetExternalFileRoot(const char* pExtFileRoot) {
    unsigned long len = strlen(pExtFileRoot);
    unsigned long nullPos = len;

    if (pExtFileRoot[len - 1] != '/') {
        mExtFileRoot[len] = '/';
        nullPos++;
    }

    if (!(nullPos < FILE_PATH_MAX)) {
        db::Panic("snd_SoundArchive.cpp", 388, "NW4R:Failed assertion nullPos < FILE_PATH_MAX");
    }
    mExtFileRoot[nullPos] = '\0';

    strncpy(mExtFileRoot, pExtFileRoot, len);
}

} // namespace snd
} // namespace nw4r
