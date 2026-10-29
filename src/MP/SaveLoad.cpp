#include "MP/SaveLoad.h"
#include "atn/Memory.h"
#include "game/GameLoadBackground.h"
#include "game/GameSaveBackground.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void* lbl_8087F518;
extern "C" void fn_8046ECDC(void* obj, int arg);
extern "C" MP::SaveData_unk* lbl_8087F460; // inferred: pending save data
extern "C" int lbl_8087DD60;               // inferred: its slot
extern "C" int lbl_8087DD64;
extern "C" unsigned char lbl_807C8508[0xB8]; // inferred: save header (same size as the work header)
// inferred: calendar time (OSCalendarTime layout, 0x28 bytes); fn_8004B20C is declared with
// SaveTime_unk in game/GameSaveLoadBackgroundCommon.h (included through GameLoadBackground.h)
typedef SaveTime_unk CalendarTime_unk;
extern "C" void fn_803BE550(MP::SaveData_unk* data);
extern "C" void fn_803BEA08(MP::SaveWork_unk* work, void* header);
extern "C" void fn_803BEA7C(MP::SaveWork_unk* work);

namespace MP {

inline SaveWork_unk::SaveWork_unk() {
    memset(this, 0, sizeof(*this));
}

SaveLoad* SaveLoad::fn_805020FC(atn::Task* parent) {
    return new ((MemoryTag)1, "", "", 0) SaveLoad(parent);
}

SaveLoad::SaveLoad(atn::Task* parent)
    : atn::Task(parent), m48(0), m4C(-1), m50(-1), mLoad(0), mSave(0), mState(0), mError(0) {
    mWork.reset(new ((MemoryTag)1, "", "", 0) SaveWork_unk);
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
            if (req->mType[0] == 0) {
                char name[0x100];
                int size;
                GameSaveLoadBackgroundCommon::fn_803B8454(name, &size);
                mBuffer.reset(new ((MemoryTag)1, "", "", 0) unsigned char[(req->mSize + 31) & ~31]);
                mLoad->fn_803B3530(name, (int)mBuffer.mPtr, (req->mSize + 31) & ~31, size);
                mState = 1;
            } else if (req->mType[0] == 1) {
                char name[0x100];
                int size;
                GameSaveLoadBackgroundCommon::fn_803B8454(name, &size);
                mBuffer.reset(new ((MemoryTag)1, "", "", 0) unsigned char[(req->mSize + 31) & ~31]);
                memcpy(mBuffer.mPtr, req->mData, req->mSize);
                mSave->fn_803B8F50(name, mBuffer.mPtr, req->mSize, size, 1);
                mState = 2;
            } else if (req->mType[0] == 2) {
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

void SaveLoad::fn_80502708() {
    m48 = lbl_8087F460;
    m4C = lbl_8087DD60;
    m50 = lbl_8087DD64;
    lbl_8087F460 = 0;
}

void SaveLoad::fn_8050272C() {
    if (m48 == 0 || mWork.mPtr == 0 || m4C < 0) {
        return;
    }
    fn_803BE550(m48);
    memcpy(mWork.mPtr->mData, m48, sizeof(mWork.mPtr->mData));
    SaveWork_unk* work = mWork.mPtr;
    CalendarTime_unk time;
    fn_8004B20C(&time);
    // GameSaveSlot_unk is the 0xB8-byte header at the start of SaveWork_unk (game/GameSaveLoadBackgroundCommon.h)
    ((GameSaveSlot_unk*)work)->fn_803BE8C0((GameSaveData_unk*)work->mData, &time);
    fn_803BEA08(mWork.mPtr, lbl_807C8508);
    mWork.mPtr->m04 = m50;
    fn_803BEA7C(mWork.mPtr);
    mQueue.Push_unk(Request_unk(2, mWork.mPtr, m4C));
}

int SaveLoad::fn_80502848() {
    if (mState == 0 && mQueue.mCount == 0) {
        return 0;
    }
    return 1;
}

int SaveLoad::fn_80502870(int type, void* data, int size) {
    mQueue.Push_unk(Request_unk(type, data, size));
    return 1;
}

} // namespace MP
