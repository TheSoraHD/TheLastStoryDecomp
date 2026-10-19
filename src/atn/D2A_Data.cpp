#include "atn/D2A_Data.h"
#include "atn/D2A_Frame.h"
#include "atn/D2A_Line.h"
#include "atn/D2A_Reference.h"
#include "atn/D2A_Window.h"
#include "std/new.h"
#include "std/string_members.h"

// inferred: functions of other d2anime classes (automatic names)
extern "C" float fn_801F0544(void* table, int key); // inferred: value at key
extern "C" void fn_801FE89C(atn::D2A_Layout_unk* layout, atn::D2A_Track_unk* track);

namespace atn {

// inferred: element classes not reconstructed yet (only their size and constructor)
class D2A_Texture : public D2A_Common {
public:
    D2A_Texture(D2A_Data* owner, unsigned short id); // 0x801F94F4
    unsigned char m12C[0x214 - 0x12C];
};

class D2A_Message : public D2A_Common {
public:
    D2A_Message(D2A_Data* owner, unsigned short id); // 0x801F2398
    unsigned char m12C[0x14C - 0x12C];
};

D2A_Data::D2A_Data() : mEndFrame(1.0f), m60(0.0f), m64(-1.0f), m9C(0), mA0(0), mA4(0) {}

D2A_Name_unk::~D2A_Name_unk() {}

// NONMATCHING: 99.67% - track loop: the loop counter and the element offset are incremented in the other order (or use swapped
// registers); tried while/for, ++i, locals, int counters and accessors
D2A_Data::~D2A_Data() {
    vf14();
    for (unsigned int i = 0; i < mTracks.mCount; i++) {
        fn_801FE89C(&m10, &mTracks.mData[i]);
    }
    mTracks.Clear();
}

void D2A_Data::fn_801EF15C(unsigned int textures, unsigned int messages, unsigned int frames, unsigned int lines,
                           unsigned int references, unsigned int windows) {
    unsigned int count = textures + messages + frames + lines + references + windows;
    mElements.Reserve(count, 8);
    unsigned int size = textures * sizeof(D2A_Texture) + messages * sizeof(D2A_Message) +
                        frames * sizeof(D2A_Frame) + lines * sizeof(D2A_Line) +
                        references * sizeof(D2A_Reference) + windows * sizeof(D2A_Window);
    mPool.Init(size);
}

D2A_Common* D2A_Data::fn_801EF36C(int type, unsigned short id) {
    D2A_Common* element = 0;
    switch (type) {
    case 0:
        element = new (mPool.mData + mPool.mUsed) D2A_Texture(this, id);
        mPool.mUsed += sizeof(D2A_Texture);
        break;
    case 1:
        element = new (mPool.mData + mPool.mUsed) D2A_Message(this, id);
        mPool.mUsed += sizeof(D2A_Message);
        break;
    case 2:
        element = new (mPool.mData + mPool.mUsed) D2A_Frame(this, id);
        mPool.mUsed += sizeof(D2A_Frame);
        break;
    case 3:
        element = new (mPool.mData + mPool.mUsed) D2A_Window(this, id);
        mPool.mUsed += sizeof(D2A_Window);
        break;
    case 4:
        element = new (mPool.mData + mPool.mUsed) D2A_Line(this, id);
        mPool.mUsed += sizeof(D2A_Line);
        break;
    case 5:
        element = new (mPool.mData + mPool.mUsed) D2A_Reference(this, id);
        mPool.mUsed += sizeof(D2A_Reference);
        break;
    }
    if (element != 0) {
        element->vf0C();
        mElements.PushBack(element);
    }
    return element;
}

// NONMATCHING: 99.51% - the loop counter and the element offset are incremented in the other order (or use swapped
// registers); tried while/for, ++i, locals, int counters and accessors
void D2A_Data::vf14() {
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        mElements.mData[i]->~D2A_Common();
    }
    mElements.mSize = 0;
    mElements.mCapacity = 0;
    if (mElements.mData != 0) {
        delete[] mElements.mData;
        mElements.mData = 0;
    }
    if (mPool.mData != 0) {
        delete[] mPool.mData;
    }
    mPool.mData = 0;
    mPool.mSize = 0;
    mPool.mUsed = 0;
}

// NONMATCHING: 93.93% - the loop counter and the element offset are incremented in the other order (or use swapped
// registers); tried while/for, ++i, locals, int counters and accessors
void D2A_Data::vf0C() {
    for (unsigned int i = 0; i < mTracks.mCount; i++) {
        D2A_Track_unk* track = &mTracks.mData[i];
        track->mValue = fn_801F0544(&track->mEntries, 0);
    }
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        mElements.mData[i]->vf14();
    }
}

// NONMATCHING: 99.21% - the loop counter and the element offset are incremented in the other order (or use swapped
// registers); tried while/for, ++i, locals, int counters and accessors
void D2A_Data::vf10() {
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        mElements.mData[i]->vf18();
    }
}

// NONMATCHING: 99.33% - the loop counter and the element offset are incremented in the other order (or use swapped
// registers); tried while/for, ++i, locals, int counters and accessors
int D2A_Data::fn_801EF9B4() {
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        if (mElements.mData[i]->vf1C()) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 97.19% - offset and element registers swapped (r5/r6)
D2A_Common* D2A_Data::fn_801EFA38(unsigned int id) {
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        D2A_Common* element = mElements.mData[i];
        if (id == element->mId) {
            return element;
        }
    }
    return 0;
}

// NONMATCHING: 97.19% - offset and element registers swapped (r5/r6)
D2A_Common* D2A_Data::fn_801EFA78(unsigned int id) {
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        D2A_Common* element = mElements.mData[i];
        if (id == element->m04) {
            return element;
        }
    }
    return 0;
}

// NONMATCHING: 87.22% - the original computes the begin and the end of the empty buffer in two
// registers (not folded); a template with N = 0 or a member array with constructor drops the loop
D2A_Track_unk::D2A_Track_unk() : m00(0), mEntries(0), mCount(0) {
    for (D2A_TrackKey_unk* p = mBuf; p < mBuf + 0; p++) {
        p->mKey = 0;
        p->mValue = 0;
    }
}

// NONMATCHING: 95.79% - f0/f1 and r4/r5 swapped
void D2A_Data::fn_801EFB00() {
    float end = 1.0f;
    for (unsigned int i = 0; i < mElements.mSize; i++) {
        D2A_Common* element = mElements.mData[i];
        if (element->mType != 5 && end < element->m18) {
            end = element->m18;
        }
    }
    mEndFrame = end;
}

} // namespace atn
