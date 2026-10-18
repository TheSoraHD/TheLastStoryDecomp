#ifndef MP_SAVELOAD_H
#define MP_SAVELOAD_H

#include "atn/Task.h"
#include "game/CharaStatus.h"

class GameLoadBackground;
class GameSaveBackground;

namespace MP {

// inferred: owning pointers with inline destructors (null-checked as members at 0x80502400)
template <class T>
struct Ptr_unk {
    Ptr_unk() : mPtr(0) {}
    ~Ptr_unk() { delete mPtr; }
    void reset(T* p) {
        T* old = mPtr;
        mPtr = p;
        delete old;
    }
    T* mPtr;
};
template <class T>
struct ArrayPtr_unk {
    ArrayPtr_unk() : mPtr(0) {}
    ~ArrayPtr_unk() { delete[] mPtr; }
    void reset(T* p) {
        T* old = mPtr;
        mPtr = p;
        delete[] old;
    }
    T* mPtr;
};

// inferred: element type destroyed by ~SaveLoad (only its size and destructor are known)
struct SaveElemB_unk {
    ~SaveElemB_unk(); // 0x80119ECC
    unsigned char m000[0x2D4];
};
struct SaveDataSub_unk {
    unsigned char m0000[0x11C4];
    SaveElemB_unk mElems[0x1C]; // 0x11C4
};
struct SaveDataSubA_unk {
    CharaStatusBase mElems[7];
};
struct SaveData_unk {
    unsigned char m0000[0x539C];
    SaveDataSubA_unk mSubA;     // 0x539C
    unsigned char m635C[0x9E60 - 0x635C];
    SaveDataSub_unk mSub;       // 0x9E60
};
// inferred: ring buffer {count, elements, read index}; its end pointer is reused for the read
// index in the constructor (0x80502300)
template <class T, int N>
struct Queue_unk {
    Queue_unk() : mCount(0), mHead(0) {}
    T& Front_unk() { return mData[mHead]; }
    void Pop_unk() {
        mHead = (mHead + 1) % N;
        mCount--;
    }
    int mCount;
    T mData[N];
    unsigned int mHead;
};
// inferred: zero-filled work buffer of 0x13560 bytes
struct SaveWork_unk {
    SaveWork_unk();
    unsigned char m00000[0x13560];
};

// inferred: queue of save/load requests served by GameLoadBackground/GameSaveBackground
class SaveLoad : public atn::Task {
public:
    struct Request_unk {
        Request_unk() : mType(3), m04(3), mData(0), mSize(0) {}
        int mType;   // 0: load, 1: save, 2: inferred: other save request, 3: none
        int m04;
        void* mData;
        int mSize;
    };

    SaveLoad(atn::Task* parent); // 0x80502150
    virtual ~SaveLoad();
    virtual void vf10();

    static SaveLoad* fn_805020FC(atn::Task* parent); // inferred: create

    SaveData_unk* m48;
    int m4C;
    int m50;
    Ptr_unk<SaveWork_unk> mWork;          // 0x54
    GameLoadBackground* mLoad;            // 0x58
    GameSaveBackground* mSave;            // 0x5C
    ArrayPtr_unk<unsigned char> mBuffer;  // 0x60
    Queue_unk<Request_unk, 48> mQueue;    // 0x64
    int mState;                           // 0x36C
    int mError;                           // 0x370
    int m374;
};

} // namespace MP

#endif
