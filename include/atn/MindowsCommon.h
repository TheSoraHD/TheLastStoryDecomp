#ifndef ATN_MINDOWSCOMMON_H
#define ATN_MINDOWSCOMMON_H

#include <string.h>

// atn::Mindows*: tree of debug menu items ("Mindows Root", "Mindows Temp", "DebugMenu"), edited
// with the USB keyboard. Class names from the RTTI; member names inferred.

namespace atn {

class MindowsFolder;

// inferred: callback called with the argument and the address of the value pointer
typedef void (*MindowsFunc_unk)(void* arg, void* value);

// inferred: drawing state passed to vf18
struct MindowsDrawCtx_unk {
    float mX;     // 0x00
    float mY;     // 0x04
    float mW;     // 0x08
    int mTop;     // 0x0C: first visible line
    int mBottom;  // 0x10: end of the visible lines
    int mCursor;  // 0x14: selected line
    int mEditing; // 0x18
};

class MindowsCommon {
public:
    MindowsCommon(MindowsFolder* parent, const char* name, int realtime) {
        if (name != mName) {
            memcpy(mName, name, strlen(name) + 1);
        }
        mNext = 0;
        mParent = parent;
        m4C = realtime;
        mArg = 0;
        mFunc = 0;
    }
    virtual ~MindowsCommon(); // 0x80084FD8
    virtual void vf0C() {}    // inferred: begin editing
    virtual void vf10() {}    // inferred: end editing (apply the typed text)
    virtual void vf14() = 0;  // inferred: input
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) = 0; // inferred: draw
    virtual int vf1C() const { return 1; } // inferred: number of lines
    // inferred: item of line `index` (counter incremented per line)
    virtual MindowsCommon* vf20(int index, int* counter) {
        MindowsCommon* r;
        if (index == *counter) {
            r = this;
        } else {
            r = 0;
        }
        (*counter)++;
        return r;
    }
    virtual int vf24(MindowsCommon* child) { return 0; } // inferred: removes a child
    virtual int vf28() { return 0; }                      // inferred: is a folder
    virtual void vf2C(MindowsFunc_unk func, void* arg) {
        mFunc = func;
        mArg = arg;
    }

    char mName[0x40];       // 0x04
    MindowsCommon* mNext;   // 0x44
    MindowsFolder* mParent; // 0x48
    int m4C;                // 0x4C: inferred: calls the callback while typing
    void* mArg;             // 0x50
    MindowsFunc_unk mFunc;  // 0x54
};

class MindowsInt : public MindowsCommon {
public:
    MindowsInt(MindowsFolder* parent, const char* name, int* value, int min, int max, int step)
        : MindowsCommon(parent, name, 0) {
        mValue = value;
        mMin = min;
        mMax = max;
        mStep = step;
        mEdit[0] = 0;
    }
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);

    int* mValue;    // 0x58
    int mMin;       // 0x5C
    int mMax;       // 0x60
    int mStep;      // 0x64
    char mEdit[0x40]; // 0x68
};

class MindowsUInt : public MindowsCommon {
public:
    MindowsUInt(MindowsFolder* parent, const char* name, unsigned int* value, unsigned int min, unsigned int max,
                unsigned int step)
        : MindowsCommon(parent, name, 0) {
        mValue = value;
        mMin = min;
        mMax = max;
        mStep = step;
        mEdit[0] = 0;
    }
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);

    unsigned int* mValue;
    unsigned int mMin;
    unsigned int mMax;
    unsigned int mStep;
    char mEdit[0x40];
};

class MindowsFloat : public MindowsCommon {
public:
    MindowsFloat(MindowsFolder* parent, const char* name, float* value, float min, float max, float step)
        : MindowsCommon(parent, name, 0) {
        mValue = value;
        mMin = min;
        mMax = max;
        mStep = step;
        mEdit[0] = 0;
    }
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);

    float* mValue;
    float mMin;
    float mMax;
    float mStep;
    char mEdit[0x40];
};

class MindowsBool : public MindowsCommon {
public:
    MindowsBool(MindowsFolder* parent, const char* name, int* value) : MindowsCommon(parent, name, 1) {
        mValue = value;
    }
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);

    int* mValue; // 0x58
};

class MindowsFlag : public MindowsCommon {
public:
    MindowsFlag(MindowsFolder* parent, const char* name, unsigned int* value, unsigned int mask)
        : MindowsCommon(parent, name, 1) {
        mValue = value;
        mMask = mask;
    }
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);

    unsigned int* mValue; // 0x58
    unsigned int mMask;   // 0x5C
};

// inferred: angle shown and typed in degrees, stored in radians
class MindowsRadian : public MindowsCommon {
public:
    MindowsRadian(MindowsFolder* parent, const char* name, float* value, float min, float max, float step)
        : MindowsCommon(parent, name, 0) {
        mValue = value;
        mMin = min;
        mMax = max;
        mStep = step;
        mEdit[0] = 0;
    }
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);

    float* mValue;
    float mMin;
    float mMax;
    float mStep;
    char mEdit[0x40];
};

class MindowsFolder : public MindowsCommon {
public:
    MindowsFolder(MindowsFolder* parent, const char* name) : MindowsCommon(parent, name, 1) {
        mFirst = 0;
        mLast = 0;
        mOpen = 0;
    }
    virtual ~MindowsFolder();
    virtual void vf14();
    virtual void vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line);
    virtual int vf1C() const;
    virtual MindowsCommon* vf20(int index, int* counter);
    virtual int vf24(MindowsCommon* child);
    virtual int vf28() { return 1; }

    void Append_unk(MindowsCommon* item) {
        if (mFirst == 0) {
            mFirst = item;
            mLast = item;
        } else {
            mLast->mNext = item;
            mLast = item;
        }
    }


    MindowsCommon* mFirst; // 0x58
    MindowsCommon* mLast;  // 0x5C
    int mOpen;             // 0x60
};

// inferred: the debug menu (0xF4 bytes, single instance lbl_8087EF10); real name unknown
class Mindows_unk {
public:
    Mindows_unk(void* owner);

    void* mOwner;         // 0x00
    int m04;
    int m08;
    int m0C;
    MindowsFolder mRoot;  // 0x10
    MindowsFolder mTemp;  // 0x74: items whose folder could not be allocated
    float mD8;
    float mDC;
    float mE0;
    float mE4;
    int mE8;
    int mEC;
    int mF0;
};

} // namespace atn

extern "C" atn::Mindows_unk* lbl_8087EF10;

// Item creation (the game calls them with their automatic names)
extern "C" void fn_800874C8(atn::MindowsFolder* self, const char* name, int* value, int min, int max, int step,
                            atn::MindowsFunc_unk func, void* arg); // AddInt
extern "C" void fn_800875F8(atn::MindowsFolder* self, const char* name, unsigned int* value, unsigned int min,
                            unsigned int max, unsigned int step, atn::MindowsFunc_unk func, void* arg); // AddUInt
extern "C" void fn_8008771C(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // AddFloat
extern "C" void fn_80087858(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // AddRadian
extern "C" void fn_80087994(atn::MindowsFolder* self, const char* name, int* value, atn::MindowsFunc_unk func,
                            void* arg); // AddBool
extern "C" void fn_80087AA0(atn::MindowsFolder* self, const char* name, unsigned int* value, unsigned int mask,
                            atn::MindowsFunc_unk func, void* arg); // AddFlag
extern "C" void fn_80087BB4(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // x, y
extern "C" void fn_80087E9C(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // x, y, z
extern "C" void fn_8008826C(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // x, y, z, w
extern "C" void fn_80088724(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // x, y, z in degrees
extern "C" void fn_80088AF4(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // R, G, B, A
extern "C" void fn_80088FAC(atn::MindowsFolder* self, const char* name, float* value, float min, float max,
                            float step, atn::MindowsFunc_unk func, void* arg); // R, G, B
extern "C" atn::MindowsFolder* fn_8008937C(atn::MindowsFolder* self, const char* name); // AddFolder

#endif
