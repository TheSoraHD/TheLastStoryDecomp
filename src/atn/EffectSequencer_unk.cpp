#include "atn/EfpData_unk.h"
#include "atn/Memory.h"
#include "std/string_members.h"
#include "std/ios_init.h"

// Effect sequences (.efp text files: "EffectSequencer", "EffectLine", "Effect" blocks with
// Frame/Pos/Rot/Scl/Color/Parent and "Throw" sub-blocks). Name of the file invented (its pool
// starts with "EffectSequencer").

extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" int fn_800DC12C(const char* s);          // inferred: string to int (atn/Util_unk.cpp)
extern "C" float fn_800DC288(const char* s);        // inferred: string to float (atn/Util_unk.cpp)
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: CRC-32 of a string (atn/Util_unk.cpp)
extern "C" int fn_80237784(atn::EffFile* loader, int frames); // inferred: (atn/EfpHandle_unk.cpp)

// inferred: reader of the tagged text files (functions 0x800E0CF0-0x800E1A48 in
// std/allocator_char); token type 1 opens a block "<Name args...>", 2 closes it. Names invented.
struct TextParser_unk {
    TextParser_unk(void* data, void* size); // 0x800E0CF0
    int GetType_unk() { return mType; }

    void* mData;
    void* mSize;
    int m08;
    std::string m0C;
    std::string m18;
    char m24[0x100];
    int mType; // 0x124
    int m128;
};
extern "C" int fn_800E0D54(TextParser_unk* parser);                 // inferred: nonzero while there is text
extern "C" void fn_800E0D70(TextParser_unk* parser);                // inferred: next token
extern "C" std::string* fn_800E0F84(TextParser_unk* parser);        // inferred: name of the token
extern "C" std::string* fn_800E114C(TextParser_unk* parser, int i); // inferred: argument `i`

// inferred: converts degrees to radians in place (inline, name invented)
inline void fn_DegToRad_unk(float* v) {
    float x = v[0];
    float y = v[1];
    float z = v[2];
    x = 0.017453292f * x;
    y = 0.017453292f * y;
    z = 0.017453292f * z;
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

EfpData_unk::EfpData_unk() : mCount(0), mElems(0), mState(0), mLength(0), mEnd(0) {}

EfpElem_unk::~EfpElem_unk() {}

void EfpData_unk::fn_802437B4(const char* path) {
    mState = 1;
}

void EfpData_unk::fn_802437C0() {
    if (mElems != 0) {
        delete[] mElems;
    }
    mElems = 0;
    mCount = 0;
    mState = 0;
}

int EfpData_unk::fn_80243814(void* data, void* size) {
    switch (mState) {
    case 1:
        fn_802438D4(data, size);
        mState = 2;
    case 2:
        for (unsigned int i = 0; i < mCount; i++) {
            if (mElems[i].mLoader.IsLoading()) {
                return 1;
            }
        }
        fn_802440F0();
        mState = 3;
        break;
    }
    return 0;
}

EfpElem_unk* EfpData_unk::fn_802438C4(int index) {
    return &mElems[index];
}

void EfpData_unk::fn_802438D4(void* data, void* size) {
    TextParser_unk parser(data, size);
    int index = 0;
    int state = 0;
    int version = 1;
    mEnd = 0;
    while (fn_800E0D54(&parser)) {
        int type = parser.GetType_unk();
        if (type == 1) {
            std::string name = *fn_800E0F84(&parser);
            switch (state) {
            case 0:
                if (name == "EffectSequencer") {
                    version = fn_800DC12C(fn_800E114C(&parser, 0)->c_str());
                } else if (name == "EffectLine") {
                    fn_80243FAC(fn_800DC12C(fn_800E114C(&parser, 0)->c_str()), 7, "EffectLine");
                } else if (name == "Effect") {
                    char path[0x100];
                    sprintf(path, "%s", fn_800E114C(&parser, 0)->c_str());
                    fn_802438C4(index)->mLoader.LoadRequest(path);
                    fn_802438C4(index)->m00 = fn_800DC12C(fn_800E114C(&parser, 1)->c_str());
                    state = 1;
                }
                break;
            case 1:
                if (name == "Frame") {
                    fn_802438C4(index)->mStart = fn_800DC12C(fn_800E114C(&parser, 0)->c_str());
                    fn_802438C4(index)->mEnd = fn_800DC12C(fn_800E114C(&parser, 1)->c_str());
                    mEnd = mEnd > fn_802438C4(index)->mEnd ? mEnd : fn_802438C4(index)->mEnd;
                } else if (name == "Pos") {
                    fn_802438C4(index)->mPos0 = fn_800DC288(fn_800E114C(&parser, 0)->c_str());
                    fn_802438C4(index)->mPos1 = fn_800DC288(fn_800E114C(&parser, 1)->c_str());
                    fn_802438C4(index)->mPos2 = fn_800DC288(fn_800E114C(&parser, 2)->c_str());
                } else if (name == "Rot") {
                    fn_802438C4(index)->mRot0 = fn_800DC288(fn_800E114C(&parser, 0)->c_str());
                    fn_802438C4(index)->mRot1 = fn_800DC288(fn_800E114C(&parser, 1)->c_str());
                    fn_802438C4(index)->mRot2 = fn_800DC288(fn_800E114C(&parser, 2)->c_str());
                    fn_DegToRad_unk(&fn_802438C4(index)->mRot0);
                } else if (name == "Scl") {
                    fn_802438C4(index)->mScale = fn_800DC288(fn_800E114C(&parser, 0)->c_str());
                } else if (name == "Color") {
                    fn_802438C4(index)->mColor0 = fn_800DC288(fn_800E114C(&parser, 0)->c_str());
                    fn_802438C4(index)->mColor1 = fn_800DC288(fn_800E114C(&parser, 1)->c_str());
                    fn_802438C4(index)->mColor2 = fn_800DC288(fn_800E114C(&parser, 2)->c_str());
                    fn_802438C4(index)->mColor3 = fn_800DC288(fn_800E114C(&parser, 3)->c_str());
                } else if (name == "Parent") {
                    fn_802438C4(index)->mParent = fn_800DC12C(fn_800E114C(&parser, 0)->c_str());
                    std::string node = *fn_800E114C(&parser, 1);
                    fn_802438C4(index)->mNode = fn_800DC6B4(node.c_str());
                    fn_802438C4(index)->m40 = fn_800DC12C(fn_800E114C(&parser, 3)->c_str());
                    fn_802438C4(index)->m44 = fn_800DC12C(fn_800E114C(&parser, 4)->c_str());
                    if (version < 2 && fn_802438C4(index)->m44 == 0) {
                        fn_802438C4(index)->m44 = -1;
                    }
                } else if (name == "Throw") {
                    state = 2;
                } else if (name == "Erase") {
                }
                break;
            }
        } else if (type == 2) {
            std::string name = *fn_800E0F84(&parser);
            switch (state) {
            case 1:
                if (name == "Effect") {
                    state = 0;
                    index++;
                }
                break;
            case 2:
                if (name == "Throw") {
                    state = 1;
                }
                break;
            }
        }
        fn_800E0D70(&parser);
    }
}

inline void EfpData_unk::fn_80243FAC(int count, int heap, const char* name) {
    if (mElems != 0) {
        delete[] mElems;
    }
    mCount = count;
    if (count != 0) {
        mElems = new ((MemoryTag)heap, "", "", 0) EfpElem_unk[count];
    } else {
        mElems = 0;
    }
}

// NONMATCHING: 95.55% - register allocation: the original keeps the element in r31 and its offset in
// r30 (swapped here); tried declaration orders, references and inline helpers
void EfpData_unk::fn_802440F0() {
    EfpElem_unk* elem;
    mLength = 1;
    for (unsigned int i = 0; i < mCount; i++) {
        elem = &mElems[i];
        if (elem->m00 != 0) {
            int frames = elem->mEnd - elem->mStart;
            int length = fn_80237784(&elem->mLoader, frames < 0 ? 0 : frames);
            if (length == 0) {
                mLength = 0;
                break;
            }
            length += elem->mStart;
            mLength = mLength > length ? mLength : length;
        }
    }
    if (mLength < mEnd) {
        mEnd = mLength;
    }
}
