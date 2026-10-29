#include "atn/D2A_Task.h"
#include "atn/Texture_unk.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "std/vector.h"
#include "std/string_members.h"

// Manager of the d2anime icons and textures shared by the menus: loads the icon layouts (one task
// per icon kind), the window textures and the table of texts, and replaces the "$xx" tags of the
// texts by icons. Names invented (atn_d2a_manager.cpp, from the class name of the RTTI).

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" int fn_80686B24(const wchar_t* a, const wchar_t* b, int n); // inferred: wcsncmp
extern "C" unsigned int fn_800DC6B4(const char* s);                    // inferred: string hash
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture

// inferred: setters of the layout at D2A_Data + 0x10 (automatic names)
extern "C" void fn_801FED24(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FEDBC(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FECE0(void* layout, unsigned int id, float value);

struct Pad_unk;
extern "C" Pad_unk* lbl_8087EF70;
extern "C" unsigned int fn_800A58D0(Pad_unk* pad, int port); // inferred: classic controller in use

struct TextMgr_unk;
extern "C" TextMgr_unk* lbl_8087EEC8;
extern "C" float fn_8006F17C(TextMgr_unk* mgr, wchar_t c, int arg, int font, float size); // inferred: char width

// inferred: the texture loader of game/GimmickCommon.h (nonzero while loading)
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

// inferred: table of texts of atn/D2A_MessageTable_unk.cpp (defined there, 0x18 bytes)
struct D2A_MessageTable_unk {
    D2A_MessageTable_unk();
    ~D2A_MessageTable_unk();
    unsigned char m00[0x18];
};
extern "C" void fn_801F35B4(D2A_MessageTable_unk* table, const char* path, int wait);
extern "C" int fn_801F362C(D2A_MessageTable_unk* table);

// inferred: definition of an icon tag ("$Bca"...): 0x34 bytes
struct D2A_IconDef_unk {
    wchar_t mName[8]; // 0x00: with the '$'
    int mLength;      // 0x10: length of mName
    int mTask;        // 0x14: icon layout (8 = start button, 9 when the classic controller is used)
    int mUv;          // 0x18: uv of the remote
    int mUvClassic;   // 0x1C: uv of the classic controller
    float mWidth;     // 0x20
    float mRot;       // 0x24
    float mOffsetY;   // 0x28
    float mScaleY;    // 0x2C
    float mScaleH;    // 0x30
};

// inferred: texture rectangle and mask of an icon (u0, v0, u1, v1), passed by value to the setters
struct D2A_IconUV_unk {
    D2A_IconUV_unk() {}
    D2A_IconUV_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    D2A_IconUV_unk(const D2A_IconUV_unk& o) { __Vec4Copy(&u0, &o.u0); }
    float u0, v0, u1, v1;
};

// inferred: piece of a line of text: a string or an icon (mIcon >= 0), as in atn/D2A_Message.cpp
struct D2A_MsgPiece_unk {
    std::basic_string<wchar_t> mText;
    int mIcon; // 0x0C
};

// inferred: rectangle of an icon (x, y, w, h, z)
struct D2A_IconRect_unk {
    float x, y, w, h, z;
};

namespace std {
// inferred: the pieces are copied one by one when push_back reallocates and the buffer destroys
// them before releasing its storage (as the lines of atn/D2A_MessageTable_unk.cpp)
template <>
struct __vec_split_unk<D2A_MsgPiece_unk> : public __vec_storage_unk<D2A_MsgPiece_unk> {
    typedef D2A_MsgPiece_unk T;
    __vec_split_unk(void* alloc) : mAlloc(alloc), mStart(0) {}
    ~__vec_split_unk() {
        T* b = begin();
        T* p = b + this->mSize;
        while (p > b) {
            (--p)->~T();
        }
        this->mSize = 0;
    }
    T* begin() { return this->mData + mStart; }
    void push_back(const T& x) {
        new (begin() + this->mSize) T(x);
        this->mSize++;
    }
    void __push_front_unk(T* first, T* last) {
        T* p = begin();
        while (last > first) {
            --last;
            new (--p) T(*last);
            mStart--;
            this->mSize++;
        }
    }
    void* mAlloc;
    unsigned int mStart;
};

// inferred: the old pieces were copied, so they stay in the buffer and are destroyed with it
template <>
inline void vector<D2A_MsgPiece_unk>::__swap_out_unk(__vec_split_unk<D2A_MsgPiece_unk>& buf) {
    buf.__push_front_unk(this->mData, this->mData + this->mSize);
    __vec_swap_unk(this->mCap, buf.mCap);
    __vec_swap_unk(this->mData, buf.mData);
    __vec_swap_unk(this->mSize, buf.mSize);
}
} // namespace std

namespace atn {

// inferred from __vt__Q23atn11D2A_Manager (0x80782B10); allocated with 0x1B0 bytes
class D2A_Manager : public Task {
public:
    D2A_Manager(Task* parent);
    virtual ~D2A_Manager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    Texture_unk mTextures[3][2];   // 0x48: inferred: window textures
    D2A_MessageTable_unk mTexts;   // 0x168
    D2A_Task* mIcons[10];          // 0x180: icon layouts
    D2A_Task* mCurrent;            // 0x1A8
    void* m1AC;
};

} // namespace atn

extern "C" atn::D2A_Manager* lbl_8087F138; // inferred: the instance

static const char* const sIconFiles[10] = {
    "icon_skull.d2a", "icon_upitem.d2a", "tips_button.d2a", "tips_u.d2a", "tips_d.d2a",
    "tips_l.d2a",     "tips_r.d2a",      "tips_ud.d2a",     "tips_h.d2a", "tips_hl.d2a",
};

static const D2A_IconDef_unk sIconDefs[0x60] = {
    {L"$@", 2, 0, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$U", 2, 1, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bca", 4, 2, 9, 9, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bcb", 4, 2, 10, 10, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bcx", 4, 2, 11, 11, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bcy", 4, 2, 12, 12, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bcl", 4, 2, 13, 13, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bcr", 4, 2, 14, 14, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bczl", 5, 2, 15, 15, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bczr", 5, 2, 16, 16, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Ba", 3, 2, 1, 9, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bb", 3, 2, 2, 14, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bc", 3, 2, 3, 13, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bz", 3, 2, 4, 15, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$B1", 3, 2, 5, 8, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$B2", 3, 2, 6, 7, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$B+", 3, 2, 7, 12, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$B-", 3, 2, 8, 11, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bmb", 4, 2, 2, 10, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bm+", 4, 2, 7, 7, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bu", 3, 3, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bd", 3, 4, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Bl", 3, 5, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$Br", 3, 6, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$BUD", 4, 7, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$St", 3, 8, 0, 0, 1.5f, 0.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBca", 5, 2, 9, 9, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBcb", 5, 2, 10, 10, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBcx", 5, 2, 11, 11, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBcy", 5, 2, 12, 12, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBcl", 5, 2, 13, 13, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBcr", 5, 2, 14, 14, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBczl", 6, 2, 15, 15, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBczr", 6, 2, 16, 16, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBa", 4, 2, 1, 9, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBb", 4, 2, 2, 14, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBc", 4, 2, 3, 13, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBz", 4, 2, 4, 15, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RB1", 4, 2, 5, 8, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RB2", 4, 2, 6, 7, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RB+", 4, 2, 7, 12, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RB-", 4, 2, 8, 11, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBmb", 5, 2, 2, 10, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBm+", 5, 2, 7, 7, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBu", 4, 3, 0, 0, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBd", 4, 4, 0, 0, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBl", 4, 5, 0, 0, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBr", 4, 6, 0, 0, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RBUD", 5, 7, 0, 0, 1.0f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$RSt", 4, 8, 0, 0, 1.5f, -3.0f, 0.0f, 1.0f, 1.0f},
    {L"$TBca", 5, 2, 9, 9, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBcb", 5, 2, 10, 10, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBcx", 5, 2, 11, 11, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBcy", 5, 2, 12, 12, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBcl", 5, 2, 13, 13, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBcr", 5, 2, 14, 14, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBczl", 6, 2, 15, 15, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBczr", 6, 2, 16, 16, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBa", 4, 2, 1, 9, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBb", 4, 2, 2, 14, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBc", 4, 2, 3, 13, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBz", 4, 2, 4, 15, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TB1", 4, 2, 5, 8, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TB2", 4, 2, 6, 7, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TB+", 4, 2, 7, 12, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TB-", 4, 2, 8, 11, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBmb", 5, 2, 2, 10, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBm+", 5, 2, 7, 7, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBu", 4, 3, 0, 0, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBd", 4, 4, 0, 0, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBl", 4, 5, 0, 0, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBr", 4, 6, 0, 0, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TBUD", 5, 7, 0, 0, 1.0f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$TSt", 4, 8, 0, 0, 1.5f, 0.0f, 0.22992702f, 1.25f, 1.459854f},
    {L"$MBca", 5, 2, 9, 9, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBcb", 5, 2, 10, 10, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBcx", 5, 2, 11, 11, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBcy", 5, 2, 12, 12, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBcl", 5, 2, 13, 13, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBcr", 5, 2, 14, 14, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBczl", 6, 2, 15, 15, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBczr", 6, 2, 16, 16, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBa", 4, 2, 1, 1, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBb", 4, 2, 2, 2, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBc", 4, 2, 3, 3, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBz", 4, 2, 4, 4, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MB1", 4, 2, 5, 5, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MB2", 4, 2, 6, 6, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MB+", 4, 2, 7, 7, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MB-", 4, 2, 8, 8, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBu", 4, 3, 0, 0, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBd", 4, 4, 0, 0, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBl", 4, 5, 0, 0, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBr", 4, 6, 0, 0, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MBUD", 5, 7, 0, 0, 1.0f, 0.0f, 0.2f, 1.2f, 1.4f},
    {L"$MSt", 4, 8, 0, 0, 1.5f, 0.0f, 0.2f, 1.2f, 1.4f},
};

// inferred: uv and mask of an icon
struct D2A_IconUVPair_unk {
    D2A_IconUV_unk mUv;
    D2A_IconUV_unk mMask;
};

// NONMATCHING (__sinit, 98.55%): the original gives r3 to the table and r4 to the first temporary
// (ours the other way round); tried a 2D array, a pair with a constructor and an array member
static D2A_IconUVPair_unk sIconUVs[17] = {
    {D2A_IconUV_unk(0.0f, 0.0f, 1.0f, 1.0f), D2A_IconUV_unk(0.0f, 0.0f, 1.0f, 1.0f)},
    {D2A_IconUV_unk(0.0f, 0.5f, 0.25f, 0.75f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.25f, 0.5f, 0.5f, 0.75f), D2A_IconUV_unk(0.25f, 0.0f, 0.5f, 0.25f)},
    {D2A_IconUV_unk(0.5f, 0.5f, 0.75f, 0.75f), D2A_IconUV_unk(0.5f, 0.0f, 0.75f, 0.25f)},
    {D2A_IconUV_unk(0.75f, 0.5f, 1.0f, 0.75f), D2A_IconUV_unk(0.75f, 0.0f, 1.0f, 0.25f)},
    {D2A_IconUV_unk(0.0f, 0.75f, 0.25f, 1.0f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.25f, 0.75f, 0.5f, 1.0f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.5f, 0.75f, 0.75f, 1.0f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.75f, 0.75f, 1.0f, 1.0f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.25f, 0.0f, 0.5f, 0.25f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.5f, 0.0f, 0.75f, 0.25f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.75f, 0.0f, 1.0f, 0.25f), D2A_IconUV_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {D2A_IconUV_unk(0.0f, 0.25f, 0.25f, 0.5f), D2A_IconUV_unk(0.0f, 0.25f, 0.25f, 0.5f)},
    {D2A_IconUV_unk(0.25f, 0.25f, 0.5f, 0.5f), D2A_IconUV_unk(0.75f, 0.25f, 1.0f, 0.5f)},
    {D2A_IconUV_unk(0.5f, 0.25f, 0.75f, 0.5f), D2A_IconUV_unk(0.5f, 0.25f, 0.75f, 0.5f)},
    {D2A_IconUV_unk(0.75f, 0.25f, 1.0f, 0.5f), D2A_IconUV_unk(0.5f, 0.25f, 0.75f, 0.5f)},
};

atn::D2A_Manager* lbl_8087F138;

extern "C" atn::D2A_Manager* fn_801F0D80(atn::Task* parent) {
    if (parent != 0 && lbl_8087F138 == 0) {
        lbl_8087F138 = new ((MemoryTag)1, "", "", 0) atn::D2A_Manager(parent);
    }
    return lbl_8087F138;
}

namespace atn {

D2A_Manager::D2A_Manager(Task* parent) : Task(parent), mCurrent(0), m1AC(0) {
    fn_800D5908(&mTextures[0][0], "wd_01_02.texture");
    mTextures[0][0].m2D = 0;
    mTextures[0][0].m2E = 0;
    fn_800D5908(&mTextures[0][1], "wd_01_01.texture");
    mTextures[0][1].m2D = 0;
    mTextures[0][1].m2E = 0;
    fn_801F35B4(&mTexts, "/data/d2anime/d2a_text_xx.u16", 0);
    for (int i = 0; i < 10; i++) {
        mIcons[i] = fn_801F3FF8(this, sIconFiles[i], 0);
        mIcons[i]->mVisible = 0;
        mIcons[i]->fn_800D246C(1);
    }
}

D2A_Manager::~D2A_Manager() {}

int D2A_Manager::vf0C() {
    int done = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            if (((GimmickUvOwner_unk*)&mTextures[i][j])->fn_800D59B8() != 0) {
                done = 0;
            }
        }
    }
    if (fn_801F362C(&mTexts) != 0) {
        done = 0;
    }
    if (fn_800D3FA4() == 0) {
        done = 0;
    }
    if (done != 0) {
        for (int i = 0; i < 10; i++) {
            mIcons[i]->fn_800D246C(0);
            mIcons[i]->mFlags |= 4;
        }
    }
    return done;
}

void D2A_Manager::vf10() {
    for (int i = 0; i < 10; i++) {
        fn_801F4484(mIcons[i]);
    }
}

void D2A_Manager::vf14() {}

} // namespace atn

// inferred: window texture `index` (0 or 1) of the frame style `id` (0 to 2)
extern "C" atn::Texture_unk* fn_801F1098(atn::D2A_Manager* manager, int id, int index) {
    if (0 > id || id >= 3) {
        return 0;
    }
    if (0 > index || index >= 2) {
        return 0;
    }
    return &manager->mTextures[id][index];
}

inline void SetIconParamA_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetIconParamC_unk(void* layout, const char* name, int index, float value) {
    fn_801FEDBC(layout, fn_800DC6B4(name), index, value);
}
inline void SetIconFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetIconUV_unk(atn::D2A_Task* task, const char* name, D2A_IconUV_unk uv) {
    task->SetParam2_unk(name, &uv.u0);
}

// NONMATCHING: 96.41% - register allocation: the original keeps the address of the layout (task +
// 0x58) of the first call in r31 instead of the task, and computes the address of the task before
// the icon definition in the second uv call; tried layout/task inline parameters, local copies
// and by-value uv parameters
// inferred: draws the icon `icon` in `rect` with the alpha `alpha`
extern "C" void fn_801F10E0(atn::D2A_Manager* manager, int icon, const D2A_IconRect_unk* rect, float alpha) {
    int task = sIconDefs[icon].mTask;
    if (task == 8 && fn_800A58D0(lbl_8087EF70, 0) != 0) {
        task = 9;
    }
    if (0 <= icon && icon < 0x60 && 0 <= task && task < 10 && manager->mIcons[task] != 0) {
        SetIconParamA_unk(&manager->mIcons[task]->mData.m10, "BasePos", 0, rect->x);
        SetIconParamA_unk(&manager->mIcons[task]->mData.m10, "BasePos", 1, rect->y - rect->h * sIconDefs[icon].mOffsetY);
        SetIconParamA_unk(&manager->mIcons[task]->mData.m10, "BasePos", 2, rect->w * sIconDefs[icon].mScaleY);
        SetIconParamA_unk(&manager->mIcons[task]->mData.m10, "BasePos", 3, rect->h * sIconDefs[icon].mScaleH);
        SetIconParamA_unk(&manager->mIcons[task]->mData.m10, "BasePos", 4, rect->z);
        SetIconParamC_unk(&manager->mIcons[task]->mData.m10, "BaseCol", 0, alpha);
        SetIconFloat_unk(&manager->mIcons[task]->mData.m10, "BaseRot", sIconDefs[icon].mRot);
        if (fn_800A58D0(lbl_8087EF70, 0) == 0) {
            D2A_IconUV_unk uv = sIconUVs[sIconDefs[icon].mUv].mUv;
            manager->mIcons[task]->SetParam2_unk("iconUv", &uv.u0);
            D2A_IconUV_unk mask = sIconUVs[sIconDefs[icon].mUv].mMask;
            manager->mIcons[task]->SetParam2_unk("iconUvMask", &mask.u0);
        } else {
            D2A_IconUV_unk uv = sIconUVs[sIconDefs[icon].mUvClassic].mUv;
            manager->mIcons[task]->SetParam2_unk("iconUv", &uv.u0);
            D2A_IconUV_unk mask = sIconUVs[sIconDefs[icon].mUvClassic].mMask;
            manager->mIcons[task]->SetParam2_unk("iconUvMask", &mask.u0);
        }
        fn_801F465C(manager->mIcons[task], 0);
    }
}

// inferred: width of the icon tag that starts after the '$' `c` (the tag is skipped); -1 if `c`
// does not start one. Also passed as a callback to the line splitter (atn/D2A_Message.cpp).
extern "C" float fn_801F1418(wchar_t c, const wchar_t** text, float size) {
    if (c == L'$') {
        const wchar_t* p = *text;
        int length = wcslen(p);
        for (int i = 0; i < 0x60; i++) {
            if (sIconDefs[i].mLength - 1 <= length &&
                fn_80686B24(p, &sIconDefs[i].mName[1], sIconDefs[i].mLength - 1) == 0) {
                *text += sIconDefs[i].mLength - 1;
                return size * sIconDefs[i].mWidth * sIconDefs[i].mScaleY;
            }
        }
    }
    return -1.0f;
}

// inferred: width of a text with its icons
extern "C" float fn_801F14FC(const wchar_t* text, int arg, int font, float size, float spacing) {
    float width = 0.0f;
    const wchar_t* p = text;
    do {
        wchar_t c = *p++;
        if (c == 0) {
            break;
        }
        float w = fn_801F1418(c, &p, size);
        if (w < 0.0f) {
            w = fn_8006F17C(lbl_8087EEC8, c, arg, font, size);
        }
        width += w;
        width += spacing;
    } while (p != 0);
    return width;
}

// NONMATCHING: 0% - the original is over the inlining budget and calls all its inline helpers out of
// line (weak copies 0x801F1818-0x801F19D0: operator=, clear, the piece constructor/destructor, size,
// operator[], data and push_back); ours inlines them all. A candidate of 3-4 more statements is enough
// to flip it, so the original push_back or piece helpers are slightly larger than ours (not found).
// With the calls, only registers of the loop indices differ. Note also that the weak push_back loads
// the size before the capacity (see include/std/vector.h) and orders its stack temporaries
// differently.
// inferred: splits a line in pieces of text and icons
extern "C" void fn_801F1658(std::vector<D2A_MsgPiece_unk>* pieces, std::basic_string<wchar_t>* line) {
    D2A_MsgPiece_unk piece;
    unsigned int size = line->size();
    unsigned int start = 0;
    unsigned int i = 0;
    while (i < size) {
        if ((*line)[i] == L'$') {
            i++;
            int length = wcslen(&(*line)[i]);
            for (int j = 0; j < 0x60; j++) {
                int n = sIconDefs[j].mLength - 1;
                if (n <= length && fn_80686B24(line->data() + i, &sIconDefs[j].mName[1], n) == 0) {
                    if (i - 1 - start != 0) {
                        piece.mText = std::basic_string<wchar_t>(*line, start, i - 1 - start);
                        piece.mIcon = -1;
                        pieces->push_back(piece);
                    }
                    piece.mText.clear();
                    piece.mIcon = j;
                    pieces->push_back(piece);
                    i = sIconDefs[j].mLength + i - 1;
                    start = i;
                    break;
                }
            }
        } else {
            i++;
        }
    }
    if (start < size) {
        piece.mText = std::basic_string<wchar_t>(*line, start);
        piece.mIcon = -1;
        pieces->push_back(piece);
        start += piece.mText.size();
    }
}

// inferred: width of the icon `icon`
extern "C" float fn_801F1E7C(int icon, float size) {
    return size * (sIconDefs[icon].mWidth * sIconDefs[icon].mScaleY);
}
