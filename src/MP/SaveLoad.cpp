#include "MP/SaveLoad.h"
#include "atn/Memory.h"
#include "game/GameLoadBackground.h"
#include "game/GameSaveBackground.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void* lbl_8087F518;
extern "C" void fn_8046ECDC(void* obj, int arg);

namespace MP {

inline SaveWork_unk::SaveWork_unk() {
    memset(this, 0, sizeof(*this));
}

SaveLoad* SaveLoad::fn_805020FC(atn::Task* parent) {
    return new (1, "", "", 0) SaveLoad(parent);
}

SaveLoad::SaveLoad(atn::Task* parent)
    : atn::Task(parent), m48(0), m4C(-1), m50(-1), mLoad(0), mSave(0), mState(0), mError(0) {
    mWork.reset(new (1, "", "", 0) SaveWork_unk);
    mLoad = GameLoadBackground::fn_803B33F8(this);
    mSave = GameSaveBackground::fn_803B8C6C(this);
}

SaveLoad::~SaveLoad() {
    if (m48 != 0) {
        delete m48;
        m48 = 0;
        m4C = -1;
    }
}

void SaveLoad::vf10() {
    switch (mState) {
    case 0:
        if (mQueue.mCount != 0) {
            Request_unk* req = &mQueue.Front_unk();
            if (req->mType == 0) {
                char name[0x100];
                int size;
                GameSaveLoadBackgroundCommon::fn_803B8454(name, &size);
                mBuffer.reset(new (1, "", "", 0) unsigned char[(req->mSize + 31) & ~31]);
                mLoad->fn_803B3530(name, (int)mBuffer.mPtr, (req->mSize + 31) & ~31, size);
                mState = 1;
            } else if (req->mType == 1) {
                char name[0x100];
                int size;
                GameSaveLoadBackgroundCommon::fn_803B8454(name, &size);
                mBuffer.reset(new (1, "", "", 0) unsigned char[(req->mSize + 31) & ~31]);
                memcpy(mBuffer.mPtr, req->mData, req->mSize);
                mSave->fn_803B8F50(name, mBuffer.mPtr, req->mSize, size, 1);
                mState = 2;
            } else if (req->mType == 2) {
                mSave->fn_803B8E60(req->mData, req->mSize, 1);
                mSave->m160 = 0;
                mState = 2;
            }
        }
        break;
    case 1:
        if (mLoad->m154 == 0) {
            if (mLoad->fn_803B8144() == 0) {
                memcpy(mQueue.Front_unk().mData, mBuffer.mPtr, mQueue.Front_unk().mSize);
                mState = 0;
            } else {
                mError = mLoad->fn_803B8144();
                mState = 3;
            }
            mQueue.Pop_unk();
        }
        fn_8046ECDC(lbl_8087F518, 0);
        break;
    case 2:
        if (mSave->m154 == 0) {
            if (mSave->fn_803B8144() == 0) {
                mState = 0;
            } else {
                // inferred: the original reads the error through the load task here as well
                mError = mLoad->fn_803B8144();
                mState = 3;
            }
            mQueue.Pop_unk();
        }
        break;
    case 3:
        mState = 0;
        break;
    }
}

} // namespace MP
