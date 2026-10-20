#include "atn/D2A_Data.h"
#include "atn/Memory.h"
#include "std/vector.h"
#include "std/string_members.h"
#include "std/ios_init.h"

// Loader of the d2anime files (.d2b): header, tables of keys and names, expression byte code,
// tracks and the elements (textures, messages, frames, lines, references and windows). Names
// invented (file between D2A_Window and D3Anime; called by D2A_Task::vf0C).

struct D2A_MessageTable_unk;
// inferred: text of an id from the table of texts (atn/D2A_MessageTable_unk.cpp)
extern "C" int fn_801F3E20(D2A_MessageTable_unk* table, std::basic_string<wchar_t>* out, unsigned int id);
// inferred: functions of the layout and the elements (automatic names)
extern "C" void fn_801FE090(atn::D2A_Layout_unk* layout, void* header);
extern "C" void fn_801ED910(atn::D2A_Common* child, atn::D2A_Common* parent);
extern "C" void fn_801FD5A0(atn::D2A_Common* element, const char* path, int arg);
extern "C" void fn_801FDA20(atn::D2A_Common* element, const char* path, int arg);
// inferred: resolves the variables of the expression byte code (atn/ParseTree.cpp)
extern "C" void fn_800A6E8C(atn::D2A_Buffer_unk<unsigned char>* code, std::vector<float*>* vars);

namespace atn {

// inferred: tables of keys of an element: count and up to N tables
template <int N>
struct D2A_TrackGroup_unk {
    int mCount;
    D2A_ParamTable mTables[N];
};

// inferred: the fields of D2A_Common read by the loader (D2A_Common.h only names a few of them)
struct D2A_ElementData_unk {
    void* mVtable;
    int m04;
    unsigned short mType; // 0x08
    unsigned short mId;   // 0x0A
    unsigned char m0C[0x0E - 0x0C];
    unsigned short m0E;
    unsigned int m10; // 0x10: flags
    float m14;
    float m18;
    D2A_TrackGroup_unk<5> m1C;
    unsigned char m48[0x5C - 0x48];
    D2A_TrackGroup_unk<4> m5C;
    unsigned char m80[0x90 - 0x80];
    D2A_TrackGroup_unk<1> m90;
    unsigned char m9C[0xA0 - 0x9C];
    D2A_TrackGroup_unk<1> mA0; // keys of m68
    unsigned char mAC[0xB0 - 0xAC];
    int mB0;
    int mB4;
    D2A_TrackGroup_unk<4> mB8;
    unsigned char mDC[0xEC - 0xDC];
    D2A_TrackGroup_unk<1> mEC;
    unsigned char mF8[0x104 - 0xF8];
    const unsigned char* mCode[10]; // 0x104: expressions
};

// inferred: texture element (0x214 bytes)
struct D2A_TextureData_unk : public D2A_ElementData_unk {
    D2A_TrackGroup_unk<4> m12C;
    unsigned char m150[0x160 - 0x150];
    D2A_TrackGroup_unk<4> m160;
    unsigned char m184[0x1F4 - 0x184];
    const unsigned char* mCode2[8]; // 0x1F4
};

// inferred: message element
struct D2A_MessageData_unk : public D2A_ElementData_unk {
    unsigned short mFont;  // 0x12C
    unsigned short mAlign; // 0x12E
    float m130;
    unsigned int mTextId;    // 0x134
    D2A_Name_unk* mTexts;    // 0x138
    int mTextCount;          // 0x13C
};

// inferred: reference and window elements
struct D2A_ParamsData_unk : public D2A_ElementData_unk {
    int m12C;
    int m130;
    int m134;
    int m138;
    int m13C;
};
struct D2A_GroupData_unk : public D2A_ElementData_unk {
    D2A_Buffer_unk<unsigned int> mChildren; // 0x12C: ids, then elements
};

// inferred: state of the loader
struct D2A_LoadContext_unk {
    D2A_Data* mData;
    D2A_MessageTable_unk* mTable;
    int mVersion;
    int mArg;
};

// inferred: header of the layout (six words passed to fn_801FE090)
struct D2A_LayoutHeader_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
};

// inferred: points a key table to the keys of the file
template <class T>
inline void SetTable_unk(D2A_ParamTable* table, T* entries, int count) {
    table->mEntries = (D2A_ParamTable::Entry*)entries;
    table->mCount = count;
}

// inferred: reader of the file
struct D2A_Reader_unk {
    template <class T>
    void Read(T* dst) {
        memcpy(dst, mData + mPos, sizeof(T));
        mPos += sizeof(T);
    }
    template <class T>
    void ReadArray(T* dst, int count) {
        memcpy(dst, mData + mPos, count * sizeof(T));
        mPos += count * sizeof(T);
    }
    void ReadBytes(void* dst, int size) {
        if (size > 0) {
            memcpy(dst, mData + mPos, size);
            mPos += size;
        }
    }
    void ReadString(std::string& s) {
        char buf[256];
        int len;
        Read(&len);
        ReadBytes(buf, len);
        buf[len] = 0;
        s = buf;
    }
    template <int N>
    void ReadTracks(D2A_TrackGroup_unk<N>* group) {
        unsigned int i;
        D2A_Data* data = mCtx->mData;
        int count = 0;
        Read(&count);
        group->mCount = count;
        for (i = 0; i < N; i++) {
            int value = 0;
            int index = 0;
            Read(&value);
            Read(&index);
            if (index >= 0 && data->m70.mCount != 0) {
                SetTable_unk(&group->mTables[i], &data->m70.mData[index], value);
            }
        }
    }
    void ReadTracksA(D2A_TrackGroup_unk<1>* group) {
        D2A_Data* data = mCtx->mData;
        int count = 0;
        Read(&count);
        group->mCount = count;
        int value = 0;
        int index = 0;
        Read(&value);
        Read(&index);
        if (index >= 0 && data->m68.mCount != 0) {
            SetTable_unk(&group->mTables[0], &data->m68.mData[index], value);
        }
    }
    void ReadTrack(D2A_TrackGroup_unk<1>* group) {
        D2A_Data* data = mCtx->mData;
        int count = 0;
        Read(&count);
        group->mCount = count;
        int value = 0;
        int index = 0;
        Read(&value);
        Read(&index);
        if (index >= 0 && data->m70.mCount != 0) {
            SetTable_unk(&group->mTables[0], &data->m70.mData[index], value);
        }
    }

    const char* mData;          // 0x00
    int mSize;                  // 0x04
    int mPos;                   // 0x08
    D2A_LoadContext_unk* mCtx;  // 0x0C
};

} // namespace atn

using namespace atn;

extern "C" void fn_80200ED0(D2A_Reader_unk* reader, const char* name, D2A_Common* element);
extern "C" void fn_80201794(D2A_Reader_unk* reader, const char* name, D2A_Common* element);

// NONMATCHING: 91.99% - the prologue stores of the reader/context are scheduled differently, the
// inlined vector<float*>::resize folds the length check with the known size and capacity (the
// original reloads them and keeps a dead fast path), and loop registers differ (pointer and count
// swapped in the key loops, the name loop has a guard in the original)
// inferred: loads a d2anime file into `d2a`; the texts of the messages are taken from `table`
extern "C" int fn_801FFA04(const char* file, int size, D2A_Data* d2a, D2A_MessageTable_unk* table, int arg) {
    D2A_Reader_unk reader;
    D2A_LoadContext_unk ctx;
    ctx.mData = d2a;
    ctx.mTable = table;
    ctx.mVersion = 0;
    ctx.mArg = arg;
    reader.mData = file;
    reader.mSize = size;
    reader.mPos = 0;
    reader.mCtx = &ctx;
    reader.Read(&ctx.mVersion);

    unsigned int textures = 0;
    unsigned int messages = 0;
    unsigned int frames = 0;
    unsigned int lines = 0;
    unsigned int references = 0;
    unsigned int windows = 0;
    reader.Read(&textures);
    reader.Read(&messages);
    reader.Read(&frames);
    reader.Read(&lines);
    reader.Read(&references);
    if (ctx.mVersion >= 0x514) {
        reader.Read(&windows);
    }
    d2a->fn_801EF15C(textures, messages, frames, lines, references, windows);
    if (ctx.mVersion >= 0x578) {
        float f = 0.0f;
        reader.Read(&f);
        d2a->m60 = f;
        reader.Read(&f);
        d2a->m64 = f;
    }

    unsigned int codeSize = 0;
    unsigned int keyCountA = 0;
    unsigned int keyCount = 0;
    unsigned int nameCount = 0;
    unsigned int trackCount = 0;
    unsigned int varCount = 0;
    unsigned int skip;
    reader.Read(&codeSize);
    d2a->m80.Alloc(codeSize);
    reader.Read(&keyCountA);
    d2a->m68.Alloc(keyCountA);
    reader.Read(&keyCount);
    d2a->m70.Alloc(keyCount);
    reader.Read(&nameCount);
    d2a->m78.Alloc(nameCount);
    reader.Read(&trackCount);
    d2a->mTracks.Alloc(trackCount);

    D2A_LayoutHeader_unk header;
    reader.Read(&header.m00);
    reader.Read(&header.m04);
    reader.Read(&header.m08);
    reader.Read(&header.m0C);
    reader.Read(&header.m10);
    reader.Read(&header.m14);
    fn_801FE090(&d2a->m10, &header);

    reader.Read(&varCount);
    reader.ReadBytes(d2a->m80.mData, codeSize);

    int countA = d2a->m68.mCount;
    D2A_Entry8a_unk* keyA = d2a->m68.mData;
    for (int i = 0; i < countA; keyA++, i++) {
        reader.Read(&keyA->m00);
        reader.Read(&keyA->m04);
    }
    int countB = d2a->m70.mCount;
    D2A_Entry8b_unk* key = d2a->m70.mData;
    for (int i = 0; i < countB; key++, i++) {
        reader.Read(&key->m00);
        reader.Read(&key->m04);
    }
    int countN = d2a->m78.mCount;
    D2A_Name_unk* name = d2a->m78.mData;
    for (int i = 0; i < countN; name++, i++) {
        wchar_t buf[256];
        reader.Read(&name->mEntry.mKey);
        int len;
        reader.Read(&len);
        reader.ReadArray(buf, len);
        buf[len] = 0;
        name->mEntry.mName = buf;
    }

    skip = 0;
    reader.Read(&skip);
    reader.mPos = reader.mPos + skip + varCount * 8;

    int countT = d2a->mTracks.mCount;
    D2A_Track_unk* track = d2a->mTracks.mData;
    for (int i = 0; i < countT; track++, i++) {
        reader.ReadTrack((D2A_TrackGroup_unk<1>*)track);
    }

    std::vector<float*> vars;
    vars.resize(d2a->mTracks.mCount);
    for (unsigned int i = 0; i < d2a->mTracks.mCount; i++) {
        vars[i] = &d2a->mTracks.mData[i].mValue;
    }
    fn_800A6E8C(&d2a->m80, &vars);

    int count = textures + messages + frames + lines + references + windows;
    for (int i = 0; i < count; i++) {
        unsigned short type = 0;
        unsigned short id = 0;
        reader.Read(&type);
        reader.Read(&id);
        if (type == 0) {
            fn_80201794(&reader, "Texture", d2a->fn_801EF36C(type, id));
        } else if (type == 1) {
            D2A_MessageData_unk* e = (D2A_MessageData_unk*)d2a->fn_801EF36C(type, id);
            fn_80200ED0(&reader, "Common", (D2A_Common*)e);
            reader.Read(&e->mFont);
            reader.Read(&e->mAlign);
            D2A_Data* data = reader.mCtx->mData;
            unsigned int textId = 0;
            reader.Read(&textId);
            e->mTextId = textId;
            int value = 0;
            int index = 0;
            reader.Read(&value);
            reader.Read(&index);
            if (index >= 0 && data->m78.mCount != 0) {
                e->mTexts = &data->m78.mData[index];
                e->mTextCount = value;
            }
        } else if (type == 2) {
            fn_80200ED0(&reader, "Common", d2a->fn_801EF36C(type, id));
        } else if (type == 4) {
            fn_80200ED0(&reader, "Common", d2a->fn_801EF36C(type, id));
        } else if (type == 3) {
            D2A_ParamsData_unk* e = (D2A_ParamsData_unk*)d2a->fn_801EF36C(type, id);
            fn_80200ED0(&reader, "Common", (D2A_Common*)e);
            reader.Read(&e->m12C);
            reader.Read(&e->m130);
            reader.Read(&e->m134);
            reader.Read(&e->m138);
            reader.Read(&e->m13C);
        } else if (type == 5) {
            D2A_GroupData_unk* e = (D2A_GroupData_unk*)d2a->fn_801EF36C(type, id);
            fn_80200ED0(&reader, "Common", (D2A_Common*)e);
            unsigned int children = 0;
            reader.Read(&children);
            e->mChildren.Alloc(children);
            for (unsigned int j = 0; j < e->mChildren.mCount; j++) {
                unsigned short child = 0;
                reader.Read(&child);
                e->mChildren.mData[j] = child;
            }
        }
    }

    D2A_PtrArray_unk<D2A_Track_unk*>* tracks = (D2A_PtrArray_unk<D2A_Track_unk*>*)&d2a->m10;
    for (unsigned int i = 0; i < d2a->mTracks.mCount; i++) {
        tracks->PushBack(&d2a->mTracks.mData[i]);
    }

    for (unsigned int i = 0; i < d2a->mElements.mSize; i++) {
        D2A_GroupData_unk* e = (D2A_GroupData_unk*)d2a->mElements.mData[i];
        if (e->mType == 5) {
            for (unsigned int j = 0; j < e->mChildren.mCount; j++) {
                D2A_Common* child = d2a->fn_801EFA38(e->mChildren.mData[j]);
                e->mChildren.mData[j] = (unsigned int)child;
                fn_801ED910(child, (D2A_Common*)e);
            }
        }
    }

    if (table != 0) {
        for (unsigned int i = 0; i < d2a->mElements.mSize; i++) {
            D2A_MessageData_unk* e = (D2A_MessageData_unk*)d2a->mElements.mData[i];
            if (e->mType == 1) {
                for (int j = 0; j < e->mTextCount; j++) {
                    std::basic_string<wchar_t>* text = &e->mTexts[j].mEntry.mName;
                    if (text->size() != 0 && text->data()[0] == 0xFFFF) {
                        unsigned int textId;
                        if (ctx.mVersion >= 0x581) {
                            textId = (text->data()[1] << 20) + ((text->data()[2] & 0xFFF) << 8) +
                                     (text->data()[3] & 0xFF);
                        } else {
                            textId = text->data()[1];
                        }
                        fn_801F3E20(table, text, textId);
                    }
                }
            }
        }
    }
    return 1;
}

// NONMATCHING: 98.73% - the original materialises the 0 of each inlined ReadTracks/ReadTrack again
// (ours keeps one 0 in a saved register), which shifts the registers of the key loops
// inferred: reads the fields common to all the elements
extern "C" void fn_80200ED0(D2A_Reader_unk* reader, const char* name, D2A_Common* element) {
    D2A_Data* data = reader->mCtx->mData;
    D2A_ElementData_unk* e = (D2A_ElementData_unk*)element;
    int id = 0;
    unsigned short unused = 0;
    reader->Read(&id);
    reader->Read(&unused);
    e->m04 = id;
    reader->Read(&e->m10);
    reader->Read(&e->m14);
    reader->Read(&e->m18);
    reader->ReadTracks(&e->m1C);
    reader->ReadTracks(&e->m5C);
    reader->ReadTrack(&e->m90);
    reader->ReadTracksA(&e->mA0);
    reader->Read(&e->mB0);
    reader->Read(&e->mB4);
    reader->Read(&e->m0E);
    reader->ReadTracks(&e->mB8);
    reader->ReadTrack(&e->mEC);
    int index = 0;
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[0] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[1] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[2] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[3] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[4] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[5] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[6] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[7] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[8] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode[9] = data->m80.mData + index;
    }
}

// NONMATCHING: 95.98% - same 0/register differences as fn_80200ED0, the assign of the strings copies
// the pointer before adding the length (mr; add) and the context is loaded before c_str()
// inferred: reads a texture element
extern "C" void fn_80201794(D2A_Reader_unk* reader, const char* name, D2A_Common* element) {
    D2A_TextureData_unk* e = (D2A_TextureData_unk*)element;
    D2A_Data* data = reader->mCtx->mData;
    fn_80200ED0(reader, "Common", element);
    std::string path;
    std::string path2;
    reader->ReadString(path);
    reader->ReadString(path2);
    reader->ReadTracks(&e->m12C);
    reader->ReadTracks(&e->m160);
    int index = 0;
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[0] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[1] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[2] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[3] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[4] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[5] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[6] = data->m80.mData + index;
    }
    reader->Read(&index);
    if (index >= 0 && data->m80.mCount != 0) {
        e->mCode2[7] = data->m80.mData + index;
    }
    fn_801FD5A0(element, path.c_str(), reader->mCtx->mArg);
    if ((e->m10 & 8) == 8) {
        fn_801FDA20(element, path2.c_str(), reader->mCtx->mArg);
    }
}

D2A_Entry8a_unk::D2A_Entry8a_unk() : m00(0), m04(0) {}

D2A_Entry8b_unk::D2A_Entry8b_unk() : m00(0), m04(0.0f) {}

// note: MWCC leaves the dead long-string branch of clear() after the blr (0x80201E10, which dtk
// took for a function of its own)
D2A_Name_unk::D2A_Name_unk() {
    mEntry.mKey = 0;
    mEntry.mName.clear();
}
