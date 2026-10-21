#ifndef ATN_NANDMANAGER_H
#define ATN_NANDMANAGER_H

#include "atn/Task.h"

namespace atn {

class D2A_Task;

// NAND command block and file information of the SDK (sizes from the layout below)
struct NandBlock_unk {
    unsigned char m00[0xBC];
};
struct NandFileInfo_unk {
    unsigned char m00[0x8C];
};

// inferred from __vt__Q23atn11NandManager (0x80778A9C) and the RTTI name; real file atn_nand.cpp
// (string in its pool). Saves and loads files of the NAND asynchronously, one step per frame,
// showing the "saving" icon (ui_saving.d2a). Members are named after their use (invented).
class NandManager : public Task {
public:
    NandManager(Task* parent);
    virtual int vf0C();
    virtual void vf10();

    void fn_800A4244();          // inferred: shows the saving icon
    int fn_800A4494(int result); // inferred: converts a NAND result to an error code
    void fn_800A500C(int result, int step); // inferred: end of an asynchronous command

    // inferred: copies the path of the current operation
    void SetPath_unk(const char* path);
    // inferred: records the error of the current step
    void SetError_unk(int err) {
        mLastError = err;
        mErrorCode = (mState << 16) | (mOp << 8) | mError;
    }
    // inferred: nonzero when `path` is an existing file
    int Exists_unk(const char* path);
    void OpenTmp_unk();

    int mOp;             // 0x48: inferred: operation (0 create, 1 create dir, 2/3 save, 4/6 load...)
    char mPath[0x100];   // 0x4C
    void* mData;         // 0x14C
    int mSize;           // 0x150
    int mOffset;         // 0x154
    int mError;          // 0x158
    int mState;          // 0x15C: inferred: current step (odd: a command just finished)
    int m160;
    unsigned int mCheck; // 0x164: inferred: answer of the space check
    int mLastError;      // 0x168
    int mErrorCode;      // 0x16C
    int mWait;           // 0x170: inferred: frames before the save starts
    int mIdle;           // 0x174: inferred: frames before the icon is hidden
    NandBlock_unk mBlock;       // 0x178
    NandFileInfo_unk mFile;     // 0x234
    unsigned char mBuffer[0x8000]; // 0x2C0
    D2A_Task* mIcon;            // 0x82C0: inferred: saving icon
    unsigned char m82C4[0x82E0 - 0x82C4];
};

} // namespace atn

extern "C" atn::NandManager* lbl_8087EF68; // inferred: the instance

#endif
