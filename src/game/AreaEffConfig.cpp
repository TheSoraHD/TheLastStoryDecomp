#include "game/AreaEffConfig.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/vector.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s);         // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s);  // inferred: string hash

// inferred: the constructor (0x80042740) and destructor (0x80041C0C) are emitted here as weak
// copies (other files call these copies)
template <int N, class C, class Tr>
inline atn::static_string<N, C, Tr>::static_string() {
    mStr[0] = '\0';
}
template <int N, class C, class Tr>
inline atn::static_string<N, C, Tr>::~static_string() {}

AreaEffConfig* AreaEffConfig::fn_80041A80(atn::Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) AreaEffConfig(parent, path);
    }
    return 0;
}

AreaEffConfig::~AreaEffConfig() {}

int AreaEffConfig::vf0C() {
    if (mBinary.fn_80473F50() == 0) {
        fn_80041D44();
        mBinary.fn_80473F88();
        mFlags |= 4;
        return 1;
    }
    return 0;
}

// NONMATCHING: 0% - the original is over the inlining budget and calls every inline member out of
// line (the weak copies 0x8004203C-0x80042EEC and 0x80043130 come from here); ours inlines them all.
// Measured: one extra first candidate of about 81 statements tips it over and then the body matches
// apart from symbol names (same open case as LoadSeTableTask::vf0C, docs/inlining.md). The generic
// std::vector::push_back of include/std/vector.h also differs from these weak copies (see
// docs/batch-auto-10kb-qG1.md).
void AreaEffConfig::fn_80041D44() {
    int size = mBinary.fn_8047059C();
    atn::Csv csv((const char*)mBinary.fn_80470580(), size);
    std::vector<AreaEffEntry_unk> entries;
    do {
        std::string kind(csv.fn_8005B9CC());
        if (kind.empty() || kind[0] == ';') {
            continue;
        }
        const char* group = csv.fn_8005B9CC();
        int type = 0;
        if (strcmp(group, "ATTR") == 0) {
            type = 1;
        } else if (strcmp(group, "MAGIC") == 0) {
            type = 2;
        } else if (strcmp(group, "MAGIC_CIRCLE") == 0) {
            type = 3;
        }
        atn::D2A_DrawColor color;
        color.r = fn_800DC288(csv.fn_8005B9CC());
        color.g = fn_800DC288(csv.fn_8005B9CC());
        color.b = fn_800DC288(csv.fn_8005B9CC());
        color.a = fn_800DC288(csv.fn_8005B9CC());
        if (kind == "DEFAULT") {
            mDefault[type] = color;
        } else {
            std::vector<atn::static_string<32> > names;
            while (*csv.fn_8005B9CC() != '\0') {
                names.push_back(atn::static_string<32>(csv.GetField_unk()));
            }
            AreaEffEntry_unk entry;
            entry.mHash = fn_800DC6B4(kind.c_str());
            entry.mType = type;
            entry.mColor = color;
            entry.mNames.Alloc0_unk(names.size());
            for (unsigned int i = 0; i < names.size(); i++) {
                entry.mNames[i] = names[i];
            }
            entries.push_back(entry);
        }
    } while (csv.fn_8005BBF8() != 0);
    mEntries.Alloc0_unk(entries.size());
    for (unsigned int i = 0; i < entries.size(); i++) {
        mEntries[i] = entries[i];
    }
}

// NONMATCHING: register allocation - the outer count and the strength-reduced offset of the inner
// loop take r27/r28 swapped (tried index types, declaration orders and size in the condition)
const atn::D2A_DrawColor* AreaEffConfig::fn_80043014(int type, const char* name) {
    if (name != 0) {
        unsigned int count = mEntries.mSize;
        unsigned int n;
        for (int i = 0; i < count; i++) {
            AreaEffEntry_unk& entry = mEntries[i];
            if (entry.mType == type) {
                n = entry.mNames.mSize;
                for (int j = 0; j < n; j++) {
                    if (entry.mNames[j] == name) {
                        return &entry.mColor;
                    }
                }
            }
        }
    }
    return &mDefault[type];
}
