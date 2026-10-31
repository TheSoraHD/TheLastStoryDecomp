#ifndef ATN_D2A_TASKINSTANCE_H
#define ATN_D2A_TASKINSTANCE_H

#include "atn/Task.h"
#include "atn/D2A_Common.h"
#include "std/string.h"

namespace atn {

class D2A_Task;
class D2A_Data;

// inferred: array with a fixed capacity and a count (names invented)
template <class T, int N>
struct D2A_FixedArray_unk {
    D2A_FixedArray_unk() : mCount(0) {}

    unsigned int size() const { return mCount; }
    T& operator[](unsigned int i) { return mData[i]; }
    void push_back(const T& value) {
        new (&mData[mCount]) T(value);
        mCount++;
    }
    void erase(T* it) {
        for (int i = it - mData; i < mCount - 1; i++) {
            mData[i] = mData[i + 1];
        }
        mCount = mCount - 1;
    }

    unsigned int mCount; // 0x00
    T mData[N];          // 0x04
};

// inferred: the texts of the instance (an extra destructor level: the original tests the address
// of the member twice before destroying the array)
template <class T, int N>
struct D2A_FixedArray2_unk : public D2A_FixedArray_unk<T, N> {
    ~D2A_FixedArray2_unk() {}
};

// inferred: color passed by value to SetColor255_unk (name invented)
struct D2A_RGB_unk {
    float r, g, b;
};

// inferred: base of the d2anime task instances: they share the D2A_Task of a file (RTTI
// "atn::ID2A_TaskInstance"; no virtual functions of its own)
class ID2A_TaskInstance : public Task {
public:
    // inferred: cache of the loaded files {hash of the path, task, references} (names invented)
    struct CacheEntry_unk {
        unsigned int mHash;
        D2A_Task* mTask;
        int mRef;
    };
    typedef D2A_FixedArray_unk<CacheEntry_unk, 256> Cache_unk;

    ID2A_TaskInstance(Task* parent) : Task(parent) {}

    static Cache_unk* sCache_unk; // 0x8087F148
};

// inferred: instance of a d2anime file with up to N values and M texts that are applied to the
// shared D2A_Task before it is drawn. The member functions are defined in
// atn/D2A_TaskInstanceBase_64_8.cpp (names invented).
template <int N, int M>
class D2A_TaskInstanceBase : public ID2A_TaskInstance {
public:
    // inferred: value of a pane parameter: type 0..3 float (by index), 5 int
    struct Param_unk {
        signed char mType;  // 0x00
        signed char mIndex; // 0x01
        unsigned int mHash; // 0x04
        union {
            float f;
            int i;
        } mValue; // 0x08
    };
    struct Text_unk {
        unsigned int mHash;                // 0x00
        std::basic_string<wchar_t> mText; // 0x04
    };

    static Cache_unk* GetCache_unk();
    static D2A_Task* Acquire_unk(Task* parent, const char* path); // inferred: shared task of `path`
    static void Release_unk(D2A_TaskInstanceBase* inst);
    static D2A_TaskInstanceBase* Create(Task* parent, const char* path);
    D2A_TaskInstanceBase(Task* parent, const char* path);
    virtual ~D2A_TaskInstanceBase();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    void Update();
    void Draw();
    void Apply();
    D2A_Data* GetData();
    float GetEndFrame();
    void* GetTrack_unk(const char* name);
    void SetValue0_unk(const char* name, float value);
    void SetValue1_unk(const char* name, int index, float value);
    void SetValue2_unk(const char* name, int index, float value);
    void SetValues1_unk(const char* name, D2A_DrawRect r);
    void SetValues1Rect_unk(const char* name, D2A_DrawRect r); // inferred: x, y and z only
    void SetValues2_unk(const char* name, D2A_DrawColor c);
    void SetValue3_unk(const char* name, int index, float value);
    void SetColor_unk(const char* name, unsigned int color);
    void SetColor_unk(const char* name, float r, float g, float b);
    void SetColor255_unk(const char* name, D2A_RGB_unk rgb); // inferred: [0,1] channels
    void SetText_unk(const char* name, const wchar_t* text);
    void SetNumber_unk(const char* name, int value, int digits);
    void SetInt_unk(const char* name, int value);
    void SetFloat_unk(const char* name, float value); // inferred: removed by the linker
    D2A_DrawRect GetRect_unk(const char* name);
    void* GetLayout_unk(); // inferred: layout of the shared task (inline)
    // inferred: weak copies in game/GameUI.cpp (0x803D2148, 0x803D218C, 0x803D2A60, 0x803D737C,
    // 0x803D7384); names invented
    void SetLoop_unk(int loop) { mLoop = loop != 0; }
    void SetSpeed_unk(float speed) { mSpeed = speed; }
    void Rewind_unk() { mFrame = 0.0f; }
    float GetFrame_unk() const { return mFrame; }
    void SetFrame_unk(float frame) { mFrame = frame; }
    // inferred: weak copies 0x803D215C and 0x803D733C; defined for <32, 8> in atn/D2A_TaskInstanceC.h
    inline void SetEnd_unk();
    inline bool IsEnd_unk();

    D2A_Task* mTask;                         // 0x48
    unsigned char mPlaying;                  // 0x4C
    unsigned char mLoop;                     // 0x4D
    unsigned char mVisible;                  // 0x4E
    float mFrame;                            // 0x50
    float mSpeed;                            // 0x54
    D2A_FixedArray_unk<Param_unk, N> mParams; // 0x58
    D2A_FixedArray2_unk<Text_unk, M> mTexts;
};

} // namespace atn

// The automatic names of some members of D2A_TaskInstanceBase<32, 8> are declared in
// atn/D2A_TaskInstanceC.h.

#endif
