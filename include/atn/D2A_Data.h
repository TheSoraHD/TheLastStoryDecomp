#ifndef ATN_D2A_DATA_H
#define ATN_D2A_DATA_H

#include "atn/D2A_Common.h"
#include "atn/Memory.h"
#include "std/string.h"

namespace atn {

// inferred: memory pool where the elements are constructed (name invented)
class D2A_Pool_unk {
public:
    D2A_Pool_unk() : mSize(0), mData(0), mUsed(0) {}
    ~D2A_Pool_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mSize = 0;
    }

    void Init(unsigned int size) {
        if (mData != 0) {
            delete[] mData;
        }
        mSize = size;
        if (size != 0) {
            mData = new (8, "", "", 0) unsigned char[size];
        } else {
            mData = 0;
        }
        mUsed = 0;
    }

    unsigned int mSize;   // 0x00
    unsigned char* mData; // 0x04
    unsigned int mUsed;   // 0x08
};

// inferred: growable array of pointers {size, capacity, data} (name invented)
template <class T>
class D2A_PtrArray_unk {
public:
    D2A_PtrArray_unk() : mSize(0), mCapacity(0), mData(0) {}
    ~D2A_PtrArray_unk() {
        if (mData != 0) {
            delete[] mData;
        }
    }

    void Reserve(unsigned int n, int heap = 0) {
        if (mCapacity > n) {
            return;
        }
        T* data = new (heap, "", "", 0) T[n];
        if (mData != 0) {
            unsigned int count = (n > mSize) ? mSize : n;
            for (unsigned int i = 0; i < count; i++) {
                data[i] = mData[i];
            }
            delete[] mData;
        }
        mData = data;
        mCapacity = n;
    }

    void PushBack(const T& value) {
        if (mData == 0 || mCapacity == 0) {
            Reserve(8);
        } else if (mSize >= mCapacity) {
            Reserve(mSize * 2);
        }
        mData[mSize] = value;
        mSize++;
    }

    // inferred: stores the address of `obj` (computed after the growth; D2A_Message::vf0C)
    template <class U>
    void PushBackRef_unk(U& obj) {
        if (mData == 0 || mCapacity == 0) {
            Reserve(8);
        } else if (mSize >= mCapacity) {
            Reserve(mSize * 2);
        }
        mData[mSize] = &obj;
        mSize++;
    }

    unsigned int Size() const { return mSize; }
    T& operator[](unsigned int i) { return mData[i]; }

    unsigned int mSize;     // 0x00
    unsigned int mCapacity; // 0x04
    T* mData;               // 0x08
};

// inferred: array allocated by the loader {count, data} (name invented)
template <class T>
class D2A_Buffer_unk {
public:
    D2A_Buffer_unk() : mCount(0), mData(0) {}
    ~D2A_Buffer_unk() { Clear(); }

    void Clear() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mCount = 0;
    }
    // inferred: used by the loader 0x801FFA04 (name invented)
    void Alloc(unsigned int count) {
        if (mData != 0) {
            delete[] mData;
        }
        mCount = count;
        if (count != 0) {
            mData = new (8, "", "", 0) T[count];
        } else {
            mData = 0;
        }
    }

    unsigned int mCount; // 0x00
    T* mData;            // 0x04
};

// inferred: layout of the elements (functions 0x801FDE94-0x801FF8EC; name invented)
struct D2A_TextParam_unk;

class D2A_Layout_unk {
public:
    D2A_Layout_unk();  // 0x801FDE94
    ~D2A_Layout_unk(); // 0x801FDEE4

    D2A_PtrArray_unk<D2A_Anim_unk<1>*> mAnims1; // 0x00: inferred: animated values registered by the elements
    D2A_PtrArray_unk<D2A_Anim_unk<5>*> mAnims5; // 0x0C
    unsigned char m18[0x24 - 0x18];
    D2A_PtrArray_unk<D2A_Anim_unk<4>*> mAnims4; // 0x24
    D2A_PtrArray_unk<D2A_TextParam_unk*> mTexts; // 0x30: inferred: text parameters of the messages
    D2A_PtrArray_unk<D2A_IntAnim_unk*> mIntAnims; // 0x3C
};

// inferred: entries of the loader tables (sizes from the loader 0x801FFA04)
struct D2A_Entry8a_unk {
    D2A_Entry8a_unk(); // 0x80201DC4
    int m00;
    int m04;
};
struct D2A_Entry8b_unk {
    D2A_Entry8b_unk(); // 0x80201DD4
    int m00;
    float m04; // inferred: cleared with 0.0f (same layout as D2A_ParamTable::Entry)
};
struct D2A_NameEntry_unk {
    unsigned int mKey;
    std::basic_string<wchar_t> mName; // 0x04: inferred: wide (assigned from UTF-16 by the loader)
};
struct D2A_Name_unk {
    D2A_Name_unk(); // 0x80201DE8
    ~D2A_Name_unk();
    D2A_NameEntry_unk mEntry;
};
// inferred: {key, value} pair
struct D2A_TrackKey_unk {
    int mKey;
    int mValue;
};

// inferred: animated value of the layout (0x10 bytes); the table has an empty inline buffer of keys
struct D2A_Track_unk {
    D2A_Track_unk();
    unsigned int m00;
    D2A_TrackKey_unk* mEntries; // 0x04: inferred: same layout as D2A_ParamTable
    int mCount;                 // 0x08
    D2A_TrackKey_unk mBuf[0];   // 0x0C
    float mValue;               // 0x0C
};

// inferred: animation data of a d2anime file (0xA8 bytes)
class D2A_Data {
public:
    D2A_Data();          // 0x801EEE54
    virtual ~D2A_Data(); // 0x801EEF74
    virtual void vf0C(); // 0x801EF894: inferred: update
    virtual void vf10(); // 0x801EF944: inferred: draw
    virtual void vf14(); // 0x801EF7E0: inferred: clear

    void fn_801EF15C(unsigned int textures, unsigned int messages, unsigned int frames, unsigned int lines,
                     unsigned int references, unsigned int windows); // inferred: reserves the elements
    D2A_Common* fn_801EF36C(int type, unsigned short id);            // inferred: creates an element
    int fn_801EF9B4();                        // inferred: nonzero while textures are loading
    D2A_Common* fn_801EFA38(unsigned int id); // inferred: element by id
    D2A_Common* fn_801EFA78(unsigned int id); // inferred: element by name hash
    void fn_801EFB00();                       // inferred: computes the end frame

    D2A_PtrArray_unk<D2A_Common*> mElements; // 0x04
    D2A_Layout_unk m10;
    float mEndFrame; // 0x58
    float mFrame;    // 0x5C
    float m60;
    float m64;
    D2A_Buffer_unk<D2A_Entry8a_unk> m68;
    D2A_Buffer_unk<D2A_Entry8b_unk> m70;
    D2A_Buffer_unk<D2A_Name_unk> m78;
    D2A_Buffer_unk<unsigned char> m80; // inferred: byte code of the expressions (see fn_800A6E8C)
    D2A_Buffer_unk<D2A_Track_unk> mTracks; // 0x88
    D2A_Pool_unk mPool;                    // 0x90
    void (*m9C)(D2A_Common* element, D2A_DrawColor* color, void* outlineColor); // inferred: message draw callback
    void* mA0; // inferred: draw callback (see D2A_Window)
    int mA4;
};

} // namespace atn

#endif
