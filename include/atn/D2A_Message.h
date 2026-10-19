#ifndef ATN_D2A_MESSAGE_H
#define ATN_D2A_MESSAGE_H

#include "atn/D2A_Common.h"
#include "atn/D2A_Data.h"
#include "std/string.h"

namespace atn {

// inferred: animated table of strings (functions 0x801F0670, 0x801F0758; name invented)
struct D2A_StringTable_unk {
    void* mEntries;
    int mCount;
};

// inferred: text parameter of a message (0x18 bytes); registered in the layout of the owner, which
// finds it by mId (hash of "string%04d")
// inferred: base of the text parameter: id and string tables, followed by an empty inline buffer of
// keys (the constructor loop runs from the end of the tables to the end of the object)
struct D2A_TextKeys_unk {
    D2A_TextKeys_unk() : mId(0) {
        mTables[0].mEntries = 0;
        mTables[0].mCount = 0;
        for (D2A_TrackKey_unk* p = (D2A_TrackKey_unk*)(this + 1); p < (D2A_TrackKey_unk*)(this + 1); p++) {
            p->mKey = 0;
            p->mValue = 0;
        }
    }

    unsigned int mId;               // 0x00
    D2A_StringTable_unk mTables[1]; // 0x04
};

struct D2A_TextValues_unk : public D2A_TextKeys_unk {
    std::basic_string<wchar_t> mValue[1]; // 0x0C: inferred: current text
};

struct D2A_TextParam_unk : public D2A_TextValues_unk {
    void fn_801F25E0(int key, const std::basic_string<wchar_t>& value, int index, int arg); // inferred: sets a key
};

// inferred from __vt__Q23atn11D2A_Message (0x80782B68)
class D2A_Message : public D2A_Common {
public:
    D2A_Message(D2A_Data* owner, unsigned short id); // 0x801F2398
    virtual ~D2A_Message();
    virtual void vf0C();
    virtual void vf14();
    virtual void vf18(); // inferred: draw

    unsigned short mFont;  // 0x12C
    unsigned short mAlign; // 0x12E: inferred: 0 left, 1 center, 2 right
    float mLineSpace;      // 0x130
    D2A_TextParam_unk mText; // 0x134
};

} // namespace atn

#endif
