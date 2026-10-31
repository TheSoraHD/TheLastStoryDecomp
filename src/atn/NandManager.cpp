// atn/NandManager.cpp: NAND file access of the game (real file name atn_nand.cpp, string in the
// pool). The manager runs one asynchronous NAND command per step: the SDK callbacks below store
// the result (fn_800A500C) and the update (vf10) starts the next command.
#include "atn/NandManager.h"
#include "atn/MemoryManager.h"
#include "atn/D2A_Message.h"
#include "atn/D2A_Task.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" {
unsigned long strlen(const char* s);
char* strcpy(char* dst, const char* src);
char* strcat(char* dst, const char* src);
int sprintf(char* buf, const char* fmt, ...);
void* memcpy(void* dst, const void* src, unsigned long n);
void* memset(void* dst, int c, unsigned long n);
char* strrchr(const char* s, int c); // inferred: strrchr

void* NANDGetUserData(const atn::NandBlock_unk* block);
void NANDSetUserData(atn::NandBlock_unk* block, void* data);
void NANDSetAutoErrorMessaging(int on);
int NANDGetHomeDir(char* path);
int NANDGetLength(atn::NandFileInfo_unk* info, unsigned long* length);
int NANDOpen(const char* path, atn::NandFileInfo_unk* info, unsigned char mode);
int NANDSeek(atn::NandFileInfo_unk* info, int offset, int whence);
int NANDWrite(atn::NandFileInfo_unk* info, const void* buf, unsigned long size);
int NANDClose(atn::NandFileInfo_unk* info);
typedef void (*NandCallback_unk)(int result, atn::NandBlock_unk* block);
int NANDOpenAsync(const char* path, atn::NandFileInfo_unk* info, unsigned char mode, NandCallback_unk cb,
                  atn::NandBlock_unk* block);
int NANDCloseAsync(atn::NandFileInfo_unk* info, NandCallback_unk cb, atn::NandBlock_unk* block);
int NANDSeekAsync(atn::NandFileInfo_unk* info, int offset, int whence, NandCallback_unk cb,
                  atn::NandBlock_unk* block);
int NANDReadAsync(atn::NandFileInfo_unk* info, void* buf, unsigned long size, NandCallback_unk cb,
                  atn::NandBlock_unk* block);
int NANDWriteAsync(atn::NandFileInfo_unk* info, const void* buf, unsigned long size, NandCallback_unk cb,
                   atn::NandBlock_unk* block);
// inferred: NANDCheckAsync
int fn_80622260(unsigned long blocks, unsigned long inodes, unsigned int* answer, NandCallback_unk cb,
                atn::NandBlock_unk* block);
// inferred: NANDSafeOpenAsync
int fn_80620220(const char* path, atn::NandFileInfo_unk* info, unsigned char mode, void* buf, unsigned long size,
                NandCallback_unk cb, atn::NandBlock_unk* block);
int fn_80620BD0(atn::NandFileInfo_unk* info, NandCallback_unk cb, atn::NandBlock_unk* block); // inferred: NANDSafeCloseAsync
int fn_80620830(atn::NandFileInfo_unk* info, NandCallback_unk cb, atn::NandBlock_unk* block); // inferred: close (read)
int fn_8061E370(const char* path, unsigned char perm, unsigned char attr, NandCallback_unk cb,
                atn::NandBlock_unk* block); // inferred: NANDCreateAsync
int fn_8061EC60(const char* path, unsigned char perm, unsigned char attr, NandCallback_unk cb,
                atn::NandBlock_unk* block); // inferred: NANDCreateDirAsync
int fn_8061EF10(const char* from, const char* to, NandCallback_unk cb, atn::NandBlock_unk* block); // inferred: NANDMoveAsync
int fn_8061E5C0(const char* path, NandCallback_unk cb, atn::NandBlock_unk* block); // inferred: NANDDeleteAsync
int fn_80622050(const char* path, unsigned char* type); // inferred: NANDGetType
int fn_80621C50(char* path);                            // inferred: NANDGetCurrentDir
int fn_80621B80(const char* path);                      // inferred: NANDChangeDir
int fn_8061EBE0(const char* path, unsigned char perm, unsigned char attr); // inferred: NANDCreateDir
int fn_8061E270(const char* path, unsigned char perm, unsigned char attr); // inferred: NANDCreate
}

extern "C" void* fn_800838B8(atn::MemoryManager* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line);

// inferred: file name of a path in the temporary folder, and its final path
extern "C" std::basic_string<char> fn_8006B174(const std::basic_string<char>& path);
std::basic_string<char> fn_8006B0C8(const std::basic_string<char>& path);

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void* fn_801FEC74(void* layout, const char* name); // inferred: finds a pane
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width
extern "C" void* lbl_8087EEC8; // inferred: font

// inferred: render manager (lbl_8087EEE0; only the widescreen flag is used here)
struct NandRender_unk {
    unsigned char m00[0x44];
    int m44;
};
extern "C" NandRender_unk* lbl_8087EEE0;

extern "C" std::basic_string<wchar_t> fn_801F0758(atn::D2A_StringTable_unk* table, int key);

// inferred: text of a pane (weak copy; called out of line by many menus)
extern "C" inline std::basic_string<wchar_t> fn_800A4450(atn::D2A_TextParam_unk* text, int key, int table) {
    return fn_801F0758(&text->mTables[table], key);
}

namespace atn {

extern "C" {
NandManager* lbl_8087EF68;
}

static void CheckDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 3);
}

static void CreateDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 5);
}

static void SafeOpenDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 7);
}

static void OpenDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 9);
}

static void ReadDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 13);
}

static void WriteDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 15);
}

static void ReadCloseDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 17);
}

static void CloseDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 19);
}

static void SeekDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 11);
}

static void SafeCloseDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 21);
}

static void DeleteDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 23);
}

static void MoveDone_unk(int result, NandBlock_unk* block) {
    ((NandManager*)NANDGetUserData(block))->fn_800A500C(result, 25);
}

extern "C" NandManager* fn_800A2C48(Task* parent) {
    if (lbl_8087EF68 == 0) {
        lbl_8087EF68 = new ((MemoryTag)1, "", "", 0) NandManager(parent);
    }
    return lbl_8087EF68;
}

NandManager::NandManager(Task* parent) : Task(parent) {
    mPath[0] = 0;
    mState = 0;
    m160 = 0;
    mLastError = 0;
    mErrorCode = 0;
    mWait = 0;
    mIdle = 0;
    NANDSetAutoErrorMessaging(1);
    mIcon = fn_801F3FF8(this, "/data/d2anime/ui_saving.d2a", 0);
    mIcon->fn_800D246C(1);
}

// inferred: local object of gmk Em001Necromancer (0x80244BC4) with an empty destructor
struct NandEmpty_unk {
    ~NandEmpty_unk();
};

NandEmpty_unk::~NandEmpty_unk() {}

int NandManager::vf0C() {
    if (fn_800D3FA4()) {
        mIcon->fn_800D246C(0);
        mIcon->mFlags |= 4;
        mIcon->mRepeat = 1;
        mIcon->mBlend = 0x13;
        return 1;
    }
    return 0;
}

inline void NandManager::SetPath_unk(const char* path) {
    if (path != mPath) {
        memcpy(mPath, path, strlen(path) + 1);
    }
}

inline int NandManager::Exists_unk(const char* path) {
    mError = 0;
    char buf[0x100];
    strcpy(buf, path);
    unsigned char type;
    int r = fn_80622050(buf, &type);
    if (r == 0) {
        return type == 1;
    }
    if (r == -12) {
        return 0;
    }
    mError = fn_800A4494(r);
    return 0;
}

// inferred: path of the temporary copy of the file being saved (written out in each step in the
// original: the format string is in @stringBase0)
#define TmpPath_unk(buf)                                                                                \
    {                                                                                                   \
        std::basic_string<char> name = fn_8006B174(std::basic_string<char>(mPath));                    \
        sprintf(buf, "/tmp/%s", name.c_str());                                                         \
    }

// inferred: first step of a save to a new file: creates or opens its temporary copy
void NandManager::OpenTmp_unk() {
    mError = 0;
    char tmp[0x100];
    TmpPath_unk(tmp);
    if (Exists_unk(tmp)) {
        mState = 8;
        NANDSetUserData(&mBlock, this);
        mError = fn_800A4494(NANDOpenAsync(tmp, &mFile, 2, OpenDone_unk, &mBlock));
    } else {
        mState = 4;
        NANDSetUserData(&mBlock, this);
        mError = fn_800A4494(fn_8061E370(tmp, 0x34, 0, CreateDone_unk, &mBlock));
    }
}

// NONMATCHING: 90.26% - the string built from mPath keeps the pointer in a register of its own in the original (one more saved register), stack offsets of the temporaries
void NandManager::vf10() {
    switch (mState) {
    case 1:
        if (mWait > 0) {
            mWait--;
        } else if ((unsigned int)(mOp - 2) <= 1) {
            if (mOp == 2) {
                mState = 6;
                NANDSetUserData(&mBlock, this);
                mError = fn_800A4494(fn_80620220(mPath, &mFile, 2, mBuffer, 0x8000, SafeOpenDone_unk, &mBlock));
            } else {
                OpenTmp_unk();
            }
            if (mError != 0) {
                SetError_unk(mError);
                mState = 0;
            }
        }
        break;
    case 3:
        if (mError == 0) {
            switch (mOp) {
            case 0: {
                mError = 0;
                char tmp[0x100];
                TmpPath_unk(tmp);
                mState = 4;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(fn_8061E370(tmp, 0x34, 0, CreateDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            case 1: {
                mState = 4;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(fn_8061EC60(mPath, 0x34, 0, CreateDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            case 10:
                mState = 0;
                break;
            }
        } else {
            SetError_unk(mError);
            mState = 0;
        }
        break;
    case 5:
        if (mError == 0) {
            switch (mOp) {
            case 0:
                if (mSize > 0) {
                    mError = 0;
                    char tmp[0x100];
                    TmpPath_unk(tmp);
                    mState = 8;
                    NANDSetUserData(&mBlock, this);
                    int err = fn_800A4494(NANDOpenAsync(tmp, &mFile, 2, OpenDone_unk, &mBlock));
                    mError = err;
                    if (err != 0) {
                        SetError_unk(err);
                        mState = 0;
                    }
                } else {
                    mState = 0;
                }
                break;
            case 1:
                mState = 0;
                break;
            case 2:
            case 3: {
                mError = 0;
                char tmp[0x100];
                TmpPath_unk(tmp);
                mState = 8;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(NANDOpenAsync(tmp, &mFile, 2, OpenDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            }
        } else {
            SetError_unk(mError);
            mState = 0;
        }
        break;
    case 7:
    case 9:
        if (mError == 0) {
            switch (mOp) {
            case 0:
                mData = fn_800838B8(atn::MemoryManager::Instance(), (mSize + 0x1F) & ~0x1F, 0x20, 1, "", "", 0);
                memset(mData, 0, (mSize + 0x1F) & ~0x1F);
                mState = 0xE;
                NANDSetUserData(&mBlock, this);
                mError = fn_800A4494(NANDWriteAsync(&mFile, mData, mSize, WriteDone_unk, &mBlock));
                if (mError != 0) {
                    atn::MemoryManager::Instance()->Free(mData);
                    SetError_unk(mError);
                    mState = 0x12;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(NANDCloseAsync(&mFile, CloseDone_unk, &mBlock));
                }
                break;
            case 2: {
                int offset = mOffset;
                if (offset > 0) {
                    mState = 0xA;
                    NANDSetUserData(&mBlock, this);
                    mError = fn_800A4494(NANDSeekAsync(&mFile, offset, 0, SeekDone_unk, &mBlock));
                } else {
                    mState = 0xE;
                    NANDSetUserData(&mBlock, this);
                    mError = fn_800A4494(NANDWriteAsync(&mFile, mData, mSize, WriteDone_unk, &mBlock));
                }
                if (mError != 0) {
                    SetError_unk(mError);
                    mState = 0x14;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
                }
                break;
            }
            case 3: {
                mState = 0xE;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(NANDWriteAsync(&mFile, mData, mSize, WriteDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0x12;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(NANDCloseAsync(&mFile, CloseDone_unk, &mBlock));
                }
                break;
            }
            case 4: {
                int offset = mOffset;
                if (offset > 0) {
                    mState = 0xA;
                    NANDSetUserData(&mBlock, this);
                    mError = fn_800A4494(NANDSeekAsync(&mFile, offset, 0, SeekDone_unk, &mBlock));
                } else {
                    mState = 0xC;
                    NANDSetUserData(&mBlock, this);
                    mError = fn_800A4494(NANDReadAsync(&mFile, mData, mSize, ReadDone_unk, &mBlock));
                }
                if (mError != 0) {
                    SetError_unk(mError);
                    mState = 0x14;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
                }
                break;
            }
            case 5:
                if (mError != 0) {
                    SetError_unk(mError);
                    mState = 0x14;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
                } else {
                    mState = 0;
                }
                break;
            }
        } else {
            SetError_unk(mError);
            if (mState == 7) {
                mState = 0x14;
                NANDSetUserData(&mBlock, this);
                fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
            } else {
                mState = 0x12;
                NANDSetUserData(&mBlock, this);
                fn_800A4494(NANDCloseAsync(&mFile, CloseDone_unk, &mBlock));
            }
            mState = 0;
        }
        break;
    case 11:
        if (mError == 0) {
            switch (mOp) {
            case 2: {
                mState = 0xE;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(NANDWriteAsync(&mFile, mData, mSize, WriteDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0x14;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
                }
                break;
            }
            case 4:
            case 6: {
                mState = 0xC;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(NANDReadAsync(&mFile, mData, mSize, ReadDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0x14;
                    NANDSetUserData(&mBlock, this);
                    fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
                }
                break;
            }
            }
        } else {
            SetError_unk(mError);
            mState = 0x14;
            NANDSetUserData(&mBlock, this);
            fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
        }
        break;
    case 13:
        if (mError == 0) {
            switch (mOp) {
            case 4: {
                mState = 0x10;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(fn_80620830(&mFile, ReadCloseDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            case 6:
                mState = 0;
                break;
            }
        } else {
            SetError_unk(mError);
            mState = 0x14;
            NANDSetUserData(&mBlock, this);
            fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
        }
        break;
    case 15:
        if (mError == 0) {
            switch (mOp) {
            case 0: {
                atn::MemoryManager::Instance()->Free(mData);
                mState = 0x12;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(NANDCloseAsync(&mFile, CloseDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            case 2: {
                mState = 0x10;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(fn_80620830(&mFile, ReadCloseDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            case 3: {
                mState = 0x12;
                NANDSetUserData(&mBlock, this);
                int err = fn_800A4494(NANDCloseAsync(&mFile, CloseDone_unk, &mBlock));
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
                break;
            }
            }
        } else {
            SetError_unk(mError);
            if (mOp == 0) {
                atn::MemoryManager::Instance()->Free(mData);
            }
            mState = 0x14;
            NANDSetUserData(&mBlock, this);
            fn_800A4494(fn_80620BD0(&mFile, SafeCloseDone_unk, &mBlock));
        }
        break;
    case 17:
    case 19:
        if (mError == 0) {
            if (mOp == 0 || mOp == 3) {
                mError = 0;
                char tmp[0x100];
                TmpPath_unk(tmp);
                mState = 0x18;
                int err;
                {
                    std::basic_string<char> dst = fn_8006B0C8(std::basic_string<char>(mPath));
                    NANDSetUserData(&mBlock, this);
                    err = fn_800A4494(fn_8061EF10(tmp, dst.c_str(), MoveDone_unk, &mBlock));
                }
                mError = err;
                if (err != 0) {
                    SetError_unk(err);
                    mState = 0;
                }
            } else {
                mState = 0;
            }
        } else {
            if (mLastError == 0) {
                mLastError = mError;
            }
            mErrorCode = (mState << 16) | (mOp << 8) | mError;
            mState = 0;
        }
        break;
    case 21:
        if (mError == 0) {
            mState = 0;
        } else {
            SetError_unk(mError);
            mState = 0;
        }
        break;
    case 23:
        if (mError == 0) {
            mState = 0;
        } else {
            SetError_unk(mError);
            mState = 0;
        }
        break;
    case 25:
        if (mError == 0) {
            mState = 0;
        } else {
            SetError_unk(mError);
            mState = 0;
        }
        break;
    }
    if (mState != 0) {
        mIdle = 3;
    } else if (--mIdle <= 0) {
        mIcon->mFlags |= 4;
    }
}

extern "C" int fn_800A4228(NandManager* nand, int* err) {
    int state = nand->mState;
    *err = nand->mLastError;
    return state != 0;
}

static inline void SetPaneValue_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
static inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
static inline D2A_DrawRect GetRect_unk(D2A_Task* task, const char* name) {
    return fn_801F4E8C(task, fn_800DC6B4(name));
}

// NONMATCHING: 96.34% - one more saved register (pane/icon registers) and the load of the rect width
void NandManager::fn_800A4244() {
    if (mIcon->IsFlag_unk(4)) {
        mIcon->mFrame = 0.0f;
        mIcon->mFlags &= ~4;
    }
    if (lbl_8087EEE0 == 0) {
        return;
    }
    if (lbl_8087EEE0->m44 != 0) {
        SetPaneValue_unk(&mIcon->mData.m10, "ofsX", 0.0f);
        SetPaneValue_unk(&mIcon->mData.m10, "ofsY", 0.0f);
    } else {
        SetPaneValue_unk(&mIcon->mData.m10, "ofsX", 5.0f);
        SetPaneValue_unk(&mIcon->mData.m10, "ofsY", -10.0f);
    }
    D2A_TextParam_unk* pane = (D2A_TextParam_unk*)fn_801FEC74(&mIcon->mData.m10, "string0011");
    if (pane != 0) {
        D2A_DrawRect rect = GetRect_unk(mIcon, "message_0011");
        float w = fn_8006EF48(lbl_8087EEC8, fn_800A4450(pane, 0, 0).data(), rect.w, 0.0f, 1, 4);
        SetPaneValue_unk(&mIcon->mData.m10, "dotpos", 0, 35.0f + w);
    }
}

extern "C" int fn_800A4460(NandManager* nand) {
    int busy = 0;
    if (nand->mState != 0 || !nand->mIcon->IsFlag_unk(4)) {
        busy = 1;
    }
    return busy;
}

int NandManager::fn_800A4494(int result) {
    switch (result) {
    case 0:
        return 0;
    case -1:
        return 1;
    case -2:
        return 2;
    case -3:
        return 3;
    case -4:
        return 4;
    case -5:
        return 5;
    case -6:
        return 6;
    case -8:
        return 7;
    case -9:
        return 8;
    case -10:
        return 9;
    case -11:
        return 10;
    case -12:
        return 11;
    case -13:
        return 12;
    case -14:
        return 13;
    case -15:
        return 14;
    case -16:
        return 15;
    case -64:
        return 16;
    case -128:
        return 17;
    }
    return 16;
}

// inferred: full path of `path` in the home folder of the game
static inline int HomePath_unk(NandManager* nand, char* buf, const char* path, int* err) {
    *err = 0;
    char home[0x100];
    int r = NANDGetHomeDir(home);
    if (r != 0) {
        *err = nand->fn_800A4494(r);
        return 0;
    }
    if (path[0] != '/') {
        sprintf(buf, "%s/%s", home, path);
    } else {
        sprintf(buf, "%s%s", home, path);
    }
    return 1;
}

extern "C" int fn_800A45D0(NandManager* nand, const char* path, int* err, int file, int home) {
    *err = 0;
    char buf[0x100];
    if (home) {
        if (!HomePath_unk(nand, buf, path, err)) {
            return 0;
        }
    } else {
        strcpy(buf, path);
    }
    unsigned char type;
    int r = fn_80622050(buf, &type);
    if (r == 0) {
        if (file) {
            return type == 1;
        }
        return type == 2;
    }
    if (r == -12) {
        return 0;
    }
    *err = nand->fn_800A4494(r);
    return 0;
}

extern "C" int fn_800A472C(NandManager* nand, unsigned long* length, int* err) {
    *err = 0;
    int r = NANDGetLength(&nand->mFile, length);
    if (r != 0) {
        *err = nand->fn_800A4494(r);
        return 0;
    }
    return 1;
}

extern "C" int fn_800A4794(NandManager* nand, const char* path, int size, int* err) {
    nand->mLastError = 0;
    nand->mOp = 0;
    nand->SetPath_unk(path);
    nand->mData = 0;
    nand->mSize = size;
    nand->mOffset = 0;
    nand->mError = 0;
    nand->mState = 2;
    NANDSetUserData(&nand->mBlock, nand);
    nand->mCheck = 0;
    int e = nand->fn_800A4494(fn_80622260(size / 0x4000 + 1, 1, &nand->mCheck, CheckDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    nand->fn_800A4244();
    return 1;
}

extern "C" int fn_800A48B8(NandManager* nand, const char* path, void* data, int size, int offset) {
    nand->mLastError = 0;
    if (nand->Exists_unk(path)) {
        nand->mOp = 2;
    } else {
        nand->mOp = 3;
    }
    nand->SetPath_unk(path);
    nand->mData = data;
    nand->mSize = size;
    nand->mOffset = offset;
    nand->mError = 0;
    nand->mWait = 5;
    nand->mState = 1;
    nand->fn_800A4244();
    return 1;
}

extern "C" int fn_800A49C4(NandManager* nand, const char* path, void* data, int size, int offset, int* err) {
    nand->mLastError = 0;
    nand->mOp = 4;
    nand->SetPath_unk(path);
    nand->mData = data;
    nand->mSize = size;
    nand->mOffset = offset;
    nand->mError = 0;
    nand->mState = 6;
    NANDSetUserData(&nand->mBlock, nand);
    int e = nand->fn_800A4494(
        fn_80620220(nand->mPath, &nand->mFile, 1, nand->mBuffer, 0x8000, SafeOpenDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    return 1;
}

extern "C" int fn_800A4AD4(NandManager* nand, int size, int inodes, int* err) {
    nand->mLastError = 0;
    nand->mOp = 10;
    nand->SetPath_unk("");
    nand->mData = 0;
    nand->mSize = size;
    nand->mOffset = 0;
    nand->mError = 0;
    nand->mState = 2;
    NANDSetUserData(&nand->mBlock, nand);
    nand->mCheck = 0;
    int e = nand->fn_800A4494(fn_80622260(size / 0x4000 + 1, inodes, &nand->mCheck, CheckDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    return 1;
}

extern "C" int fn_800A4BE4(NandManager* nand, const char* path, int* err) {
    nand->mLastError = 0;
    nand->mOp = 5;
    nand->SetPath_unk(path);
    nand->mData = 0;
    nand->mSize = 0;
    nand->mOffset = 0;
    nand->mError = 0;
    nand->mState = 6;
    NANDSetUserData(&nand->mBlock, nand);
    int e = nand->fn_800A4494(fn_80620220(path, &nand->mFile, 1, nand->mBuffer, 0x8000, SafeOpenDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    return 1;
}

extern "C" int fn_800A4D00(NandManager* nand, void* data, int size, int offset, int* err) {
    nand->mLastError = 0;
    nand->mOp = 6;
    nand->SetPath_unk("");
    nand->mData = data;
    nand->mSize = size;
    nand->mOffset = offset;
    nand->mError = 0;
    nand->mState = 0xA;
    NANDSetUserData(&nand->mBlock, nand);
    int e = nand->fn_800A4494(NANDSeekAsync(&nand->mFile, offset, 0, SeekDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    return 1;
}

extern "C" int fn_800A4E08(NandManager* nand, int* err) {
    nand->mLastError = 0;
    nand->mOp = 7;
    nand->SetPath_unk("");
    nand->mData = 0;
    nand->mSize = 0;
    nand->mOffset = 0;
    nand->mError = 0;
    nand->mState = 0x10;
    NANDSetUserData(&nand->mBlock, nand);
    int e = nand->fn_800A4494(fn_80620830(&nand->mFile, ReadCloseDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    return 1;
}

extern "C" int fn_800A4F0C(NandManager* nand, const char* path, int* err) {
    nand->mLastError = 0;
    nand->mOp = 9;
    nand->SetPath_unk(path);
    nand->mData = 0;
    nand->mSize = 0;
    nand->mOffset = 0;
    nand->mError = 0;
    nand->mState = 0x16;
    NANDSetUserData(&nand->mBlock, nand);
    int e = nand->fn_800A4494(fn_8061E5C0(nand->mPath, DeleteDone_unk, &nand->mBlock));
    *err = e;
    if (e != 0) {
        nand->SetError_unk(e);
        nand->mState = 0;
        return 0;
    }
    return 1;
}

void NandManager::fn_800A500C(int result, int step) {
    mState = step;
    switch (step) {
    case 3:
        if (result == 0) {
            unsigned int check = mCheck;
            if ((check & 1) == 1) {
                mError = 0x12;
            } else if ((check & 2) == 2) {
                mError = 0x13;
            } else if ((check & 4) == 4) {
                mError = 0x14;
            } else if ((check & 8) == 8) {
                mError = 0x15;
            } else {
                mError = 0;
            }
        } else {
            mError = fn_800A4494(result);
        }
        break;
    case 5:
        if (result != 0) {
            if (result != -6 || mOp != 2) {
                mError = fn_800A4494(result);
            }
        }
        break;
    case 7:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    case 9:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    case 17:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    case 19:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    case 11:
        if (result < 0) {
            mError = fn_800A4494(result);
        } else if (result != mOffset) {
            mError = 5;
        }
        break;
    case 13:
        if (result < 0) {
            mError = fn_800A4494(result);
        } else if (result != mSize) {
            mError = 5;
        }
        break;
    case 15:
        if (result < 0) {
            mError = fn_800A4494(result);
        } else if (result != mSize) {
            mError = 5;
        }
        break;
    case 21:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    case 23:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    case 25:
        if (result != 0) {
            mError = fn_800A4494(result);
        }
        break;
    }
}

// inferred: writes `data` at `offset` of a file of the home folder at once (used by the friend
// list, 0x8050EBC8)
extern "C" int fn_800A5218(const char* path, int offset, const void* data, int size) {
    NandFileInfo_unk info;
    char cur[0x40];
    char home[0x40];
    char full[0x40];
    char dir[0x40];
    char name[0x40];
    if (fn_80621C50(cur) != 0) {
        return 0;
    }
    if (NANDGetHomeDir(home) != 0) {
        return 0;
    }
    if (fn_80621B80(home) != 0) {
        return 0;
    }
    strcpy(name, path);
    char* p = strrchr(name, '/');
    if (p != 0) {
        *p = 0;
        strcpy(dir, name);
        strcpy(name, p + 1);
    } else {
        dir[0] = 0;
    }
    switch (fn_80621B80(dir)) {
    case -12:
        if (fn_8061EBE0(dir, 0x30, 0) != 0) {
            fn_80621B80(cur);
            return 0;
        }
        break;
    case -1:
    case -8:
    case -64:
    case -128:
        fn_80621B80(cur);
        return 0;
    }
    fn_80621B80(cur);
    strcpy(full, home);
    if (dir[0] != 0) {
        strcat(full, "/");
        strcat(full, dir);
    }
    strcat(full, "/");
    strcat(full, name);
    int r = NANDOpen(full, &info, 2);
    if (r != 0) {
        if (r == -12) {
            fn_8061E270(full, 0x30, 0);
            if (NANDOpen(full, &info, 2) != 0) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    NANDSeek(&info, offset, 0);
    int alignedSize = (size + 0x1F) / 0x20 * 0x20;
    void* buf = atn::MemoryManager::Instance()->AllocFromMem2(alignedSize, 0x20, (MemoryTag)0, "", "", 0, 0);
    memset(buf, 0, alignedSize);
    memcpy(buf, data, size);
    NANDWrite(&info, buf, alignedSize);
    atn::MemoryManager::Instance()->Free(buf);
    NANDClose(&info);
    return 1;
}

// inferred: synchronous read of a file. No caller is left in the game (stripped by the linker):
// only its two strings remain in the pool, after the "/" of fn_800A5218. Body and name invented.
extern "C" void OSPanic(const char* file, int line, const char* msg, ...);
extern "C" int NANDRead(NandFileInfo_unk* info, void* buf, unsigned long size);

int Stripped1_unk(const char* path, void* buf, int size) {
    NandFileInfo_unk info;
    if (NANDOpen(path, &info, 1) != 0) {
        OSPanic("atn_nand.cpp", 0, "[NAND] Couldn't open a file.");
        return 0;
    }
    NANDRead(&info, buf, size);
    NANDClose(&info);
    return 1;
}

} // namespace atn
