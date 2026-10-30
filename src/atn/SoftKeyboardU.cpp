// MP::SoftKeyboard (RTTI/vtable 0x80792D38): name entry of the multiplayer mode, an on-screen
// keyboard of 4 x 11 keys (atn::SoftKeyboardU at 0xF8) drawn with the online_name_*.d2a animes.
// The boost::bind type names in .rodata give the class and the callback signatures (the callbacks
// take `const Key&` in the original; here Key*, as in include/atn/SoftKeyboardBase.h).
#include "atn/SoftKeyboardU.h"
#include "MP/D2AMpMenu_unk.h"
#include "MP/D2ACommonDialog.h"
#include <boost/bind.hpp>
#include "atn/DrawHelper.h"

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" int rand();
extern "C" int fn_8006BA30(); // inferred: language
extern "C" int fn_800A58D0(void* pad, int port);
extern "C" int fn_804F7F18();

class MenuCommonTask;
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c);
class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
};

// inferred: names of the players (see MP/D2AMpMenuFriend.cpp); only the name being entered
struct SoftKeyboardPlayers_unk {
    unsigned char m000[0x5F0];
    wchar_t m5F0[1]; // inferred: name of the local player
};
extern "C" SoftKeyboardPlayers_unk* lbl_8087F628;

// inferred: camp message table (see game/MenuCommonTask.cpp); only the texts used here
struct SoftKeyboardMsg_unk {
    struct Entry {
        int mId;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) { return mEntries[id].mText != 0 ? mEntries[id].mText : MP::lbl_808813D0; }
    Entry mEntries[1];
};
extern "C" SoftKeyboardMsg_unk* lbl_8087F1E4;

// inferred: debug drawing (lbl_8087EEB0, see game/CharaTask.cpp)

namespace atn {
// inferred: pointer to a string owned elsewhere (see MP/Text.cpp)
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    ptr_string_base(const T* p) : mPtr((T*)p) {}
    virtual ~ptr_string_base() {}
    const T* c_str() const { return mPtr != 0 ? mPtr : MP::lbl_808813D0; }

    T* mPtr;
};
} // namespace atn


// inferred: characters of the keys for each keyboard layout (QWERTY, AZERTY, QWERTZ); 0x2190
// deletes, 0x2193 confirms, '+' picks a random name
static const wchar_t sKeyChars_unk[3][4][11] = {
    {{L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', 0x2190},
     {L'Q', L'W', L'E', L'R', L'T', L'Y', L'U', L'I', L'O', L'P', L' '},
     {L'A', L'S', L'D', L'F', L'G', L'H', L'J', L'K', L'L', L' ', L'+'},
     {L'Z', L'X', L'C', L'V', L'B', L'N', L'M', L'!', L'?', L' ', 0x2193}},
    {{L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', 0x2190},
     {L'A', L'Z', L'E', L'R', L'T', L'Y', L'U', L'I', L'O', L'P', L' '},
     {L'Q', L'S', L'D', L'F', L'G', L'H', L'J', L'K', L'L', L' ', L'+'},
     {L'W', L'X', L'C', L'V', L'B', L'N', L'M', L'!', L'?', L' ', 0x2193}},
    {{L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', 0x2190},
     {L'Q', L'W', L'E', L'R', L'T', L'Z', L'U', L'I', L'O', L'P', L' '},
     {L'A', L'S', L'D', L'F', L'G', L'H', L'J', L'K', L'L', L' ', L'+'},
     {L'Y', L'X', L'C', L'V', L'B', L'N', L'M', L'!', L'?', L' ', 0x2193}},
};

// NOTE: in the original the constructor of MP::SoftKeyboard goes over the inline budget and calls
// out of line this function, the Vector2_unk and key constructors, SoftKeyboardU's constructor,
// fn_80503AB8/fn_80503B04, the seven boost::bind and the seven boost::function constructors (all
// weak copies of this file, 0x805028D8-0x80503B04). Here it stays one statement below the budget
// (one more inline call makes everything go out of line), so those 14 functions are not emitted.
// Also, the original builds the key corners in place (Vector2 constructor on the key) without an
// operator=: not reproduced (tried temporaries, named locals, a corner type with implicit copy).
// inferred: character of a key for the current language
static wchar_t fn_805028D8(int row, int col) {
    wchar_t c;
    int lang = fn_8006BA30();
    if (lang == 3) {
        c = sKeyChars_unk[1][row][col];
    } else if (lang == 2) {
        c = sKeyChars_unk[2][row][col];
    } else {
        c = sKeyChars_unk[0][row][col];
    }
    return c;
}

namespace MP {

class SoftKeyboard : public atn::Task {
public:
    typedef atn::SoftKeyboardBase<wchar_t, 0>::Key Key;

    static SoftKeyboard* fn_80502974(atn::Task* parent); // inferred: creates the instance
    SoftKeyboard(atn::Task* parent);
    virtual ~SoftKeyboard();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    void fn_80504C00(); // inferred: shows the name being entered
    void fn_805053F8(); // inferred: opens the keyboard
    void fn_80505418(); // inferred: closes the keyboard
    void fn_80505510(int row, int col, Key* key);          // inferred: key decided
    void fn_805058E4(int row, int col, Key* key);          // inferred: empty callback
    void fn_805058E8(int row, int col, Key* key, int newRow, int newCol, Key* newKey); // inferred: cursor moved
    void fn_80505970(); // inferred: confirms the name
    void fn_80505AB4(); // inferred: deletes a character
    void fn_80505BB8(); // inferred: cancels
    void fn_80505C1C(int row, int col, Key* key, int cursor); // inferred: draws a key

    MpD2A_unk* m48;       // inferred: online_name_logo.d2a
    MpD2A_unk* m4C;       // inferred: online_name_base.d2a
    MpD2A_unk* m50[0x26]; // inferred: online_name_key1.d2a, one per key
    MpD2A_unk* mE8[4];    // inferred: online_name_key2/3.d2a (special keys)
    atn::SoftKeyboardU mKeyboard; // 0xF8
    int m25AC;  // inferred: state
    int m25B0;  // inferred: cursor position
    int m25B4;  // inferred: length of the name
    int m25B8;
    int m25BC;
};

} // namespace MP

extern "C" MP::SoftKeyboard* lbl_8087F860;
MP::SoftKeyboard* lbl_8087F860; // inferred: instance

// inferred: labels of the keys for each keyboard layout (QWERTY, AZERTY, QWERTZ)
static const wchar_t* sKeyLabels_unk[0x26] = {
    L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"Q", L"W", L"E",
    L"R", L"T", L"Y", L"U", L"I", L"O", L"P", L"A", L"S", L"D", L"F", L"G", L"H",
    L"J", L"K", L"L", L"Z", L"X", L"C", L"V", L"B", L"N", L"M", L"!", L"?"};
static const wchar_t* sKeyLabelsFr_unk[0x26] = {
    L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"A", L"Z", L"E",
    L"R", L"T", L"Y", L"U", L"I", L"O", L"P", L"Q", L"S", L"D", L"F", L"G", L"H",
    L"J", L"K", L"L", L"W", L"X", L"C", L"V", L"B", L"N", L"M", L"!", L"?"};
static const wchar_t* sKeyLabelsDe_unk[0x26] = {
    L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"Q", L"W", L"E",
    L"R", L"T", L"Z", L"U", L"I", L"O", L"P", L"A", L"S", L"D", L"F", L"G", L"H",
    L"J", L"K", L"L", L"Y", L"X", L"C", L"V", L"B", L"N", L"M", L"!", L"?"};

// inferred: texts of the special keys (camp message ids)
static const int sSpecialText_unk[4] = {0x1A5, 0x1A6, 0x1A7, 0x1A8};

namespace MP {

SoftKeyboard* SoftKeyboard::fn_80502974(atn::Task* parent) {
    if (lbl_8087F860 == 0) {
        lbl_8087F860 = new ((MemoryTag)1, "", "", 0) SoftKeyboard(parent);
    }
    return lbl_8087F860;
}

// NONMATCHING: everything inlined here (see the note of fn_805028D8)
SoftKeyboard::SoftKeyboard(atn::Task* parent) : atn::Task(parent), m25AC(0), m25B0(0), m25B4(0), m25B8(1) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 11; col++) {
            Key key;
            key.m04 = fn_805028D8(row, col);
            key.mMin = atn::Vector2_unk(40.0f * col + 100.0f + 40.0f * row / 3.0f, 40.0f * row + 200.0f);
            key.mMax = atn::Vector2_unk(40.0f * (col + 1) + 100.0f + 40.0f * row / 3.0f, 40.0f * (row + 1) + 200.0f);
            mKeyboard.fn_80503AB8(row, col, key);
        }
    }
    mKeyboard.fn_80503B04(0x10);
    mKeyboard.fn_80503B0C(boost::bind(&SoftKeyboard::fn_805058E8, this, _1, _2, _3, _4, _5, _6));
    mKeyboard.fn_80503D38(boost::bind(&SoftKeyboard::fn_80505510, this, _1, _2, _3));
    mKeyboard.fn_80503F64(boost::bind(&SoftKeyboard::fn_805058E4, this, _1, _2, _3));
    mKeyboard.fn_80504190(boost::bind(&SoftKeyboard::fn_80505970, this));
    mKeyboard.fn_805043BC(boost::bind(&SoftKeyboard::fn_80505AB4, this));
    mKeyboard.fn_805045E8(boost::bind(&SoftKeyboard::fn_80505BB8, this));
    mKeyboard.fn_80504814(boost::bind(&SoftKeyboard::fn_80505C1C, this, _1, _2, _3, _4));
    m48 = fn_801F3FF8(this, "/data/d2anime/online_name_logo.d2a", 0);
    m48->fn_800D246C(1);
    m4C = fn_801F3FF8(this, "/data/d2anime/online_name_base.d2a", 0);
    m4C->fn_800D246C(1);
    for (int i = 0; i < 0x26; i++) {
        m50[i] = fn_801F3FF8(this, "/data/d2anime/online_name_key1.d2a", 0);
        m50[i]->fn_800D246C(1);
    }
    for (int i = 0; i < 4; i++) {
        if (i == 0 || i == 3) {
            mE8[i] = fn_801F3FF8(this, "/data/d2anime/online_name_key2.d2a", 0);
        } else {
            mE8[i] = fn_801F3FF8(this, "/data/d2anime/online_name_key3.d2a", 0);
        }
        mE8[i]->fn_800D246C(1);
    }
}

SoftKeyboard::~SoftKeyboard() {
    lbl_8087F860 = 0;
}

// inferred: starts an anime from its first frame (see MP/D2AMpMenu_unk.h)
inline void Start_unk(MpD2A_unk* task, float speed) {
    if (task != 0) {
        task->fn_800D246C(0);
        task->mSpeed = speed;
        task->mRepeat = 0;
    }
}

int SoftKeyboard::vf0C() {
    if (fn_800D3FA4() != 0 && fn_804F7F18() == 0) {
        Start_unk(m48, 0.0f);
        Start_unk(m4C, 0.0f);
        for (int i = 0; i < 0x26; i++) {
            m50[i]->fn_800D246C(0);
            m50[i]->mSpeed = 0.0f;
            m50[i]->mRepeat = 0;
        }
        for (int i = 0; i < 4; i++) {
            mE8[i]->fn_800D246C(0);
            mE8[i]->mSpeed = 0.0f;
            mE8[i]->mRepeat = 0;
        }
        fn_80504C00();
        return 1;
    }
    return 0;
}

// NONMATCHING: 92% - the original reads the text with a base and a separate offset register
// (lhzx) and keeps another zero register; here the loop uses a pointer.
void SoftKeyboard::fn_80504C00() {
    char name[0x40] = {0};
    atn::Text_unk<wchar_t, 0x400> text = mKeyboard.mText;
    for (int i = 0; i < 8; i++) {
        sprintf(name, "string00%d", i + 1);
        if (i < wcslen(text.m)) {
            wchar_t c[8] = {0};
            c[0] = text.m[i];
            c[1] = 0;
            SetPaneText_unk(m4C->mLayout, name, c);
            m25B4++;
        } else {
            SetPaneText_unk(m4C->mLayout, name, L"_");
        }
    }
}

void SoftKeyboard::vf10() {
    switch (m25AC) {
    case 0:
        m48->mFlags |= 4;
        m4C->mFlags |= 4;
        for (int i = 0; i < 0x26; i++) {
            m50[i]->mFlags |= 4;
        }
        for (int i = 0; i < 4; i++) {
            mE8[i]->mFlags |= 4;
        }
        break;
    case 1:
        m25AC = 2;
        m25B8 = 1;
        mKeyboard.fn_800C73B0();
        Start_unk(m48, 1.0f);
        Start_unk(m4C, 1.0f);
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        for (int i = 0; i < 0x26; i++) {
            Start_unk(m50[i], 1.0f);
            m50[i]->mFlags &= ~4;
        }
        for (int i = 0; i < 4; i++) {
            Start_unk(mE8[i], 1.0f);
            mE8[i]->mFlags &= ~4;
        }
        break;
    case 2:
        mKeyboard.vf0C();
        break;
    case 3:
        float frame = m4C->mFrame;
        if (0.0f == frame) {
            m25AC = 0;
        }
        break;
    case 4:
        if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() != 0) {
            m25AC = 2;
        }
        break;
    }
}

// NONMATCHING: 99.3% - registers of the label and layout in the first loop and of the button texts
// (layout/text swapped); tried a helper and the task form of the text setter.
void SoftKeyboard::vf14() {
    if (m25AC == 0) {
        return;
    }
    char name[0x40];
    for (int i = 0; i < 0x26; i++) {
        sprintf(name, "null_%04d", i + 1);
        MpRect_unk rect = GetPaneRect_unk(m4C, name);
        fn_801F4728(m50[i], "pos0003(1)", &rect);
        const wchar_t* label;
        int lang = fn_8006BA30();
        if (lang == 3) {
            label = sKeyLabelsFr_unk[i];
        } else if (lang == 2) {
            label = sKeyLabelsDe_unk[i];
        } else {
            label = sKeyLabels_unk[i];
        }
        SetPaneText_unk(m50[i]->mLayout, "num", label);
    }
    for (int i = 0; i < 4; i++) {
        sprintf(name, "null_020%d", i + 1);
        MpRect_unk rect = GetPaneRect_unk(m4C, name);
        fn_801F4728(mE8[i], "pos0068(1)", &rect);
        const wchar_t* text = lbl_8087F1E4->Get_unk(sSpecialText_unk[i]);
        SetPaneText_unk(mE8[i]->mLayout, "string0066(1)", text);
    }
    if (fn_800A58D0(lbl_8087EF70, 0) != 0) {
        SetPaneText_unk(mE8[0]->mLayout, "string0001(1)", L"$B+");
        SetPaneText_unk(mE8[3]->mLayout, "string0001(1)", L"$B-");
    } else {
        SetPaneText_unk(mE8[0]->mLayout, "string0001(1)", L"$B-");
        SetPaneText_unk(mE8[3]->mLayout, "string0001(1)", L"$B+");
    }
    sprintf(name, "cursor_%04d", m25B0 + 1);
    MpRect_unk rect = GetPaneRect_unk(m4C, name);
    fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
    if (wcslen(lbl_8087F628->m5F0) == 0) {
        SetPaneValue_unk(m4C->mLayout, "pos0017", 0, 1000.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0007", 0, 1000.0f);
    } else {
        SetPaneValue_unk(m4C->mLayout, "pos0017", 0, 475.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0007", 0, 496.0f);
    }
}

void SoftKeyboard::fn_805053F8() {
    if (m25AC == 0 || m25AC == 3) {
        m25AC = 1;
    }
}

void SoftKeyboard::fn_80505418() {
    if (!(m25AC != 2 && m25AC != 1)) {
        m25AC = 3;
        mKeyboard.fn_800C755C();
        m48->mSpeed = -1.0f;
        m4C->mSpeed = -1.0f;
        for (int i = 0; i < 0x26; i++) {
            m50[i]->mSpeed = -1.0f;
        }
        for (int i = 0; i < 4; i++) {
            mE8[i]->mSpeed = -1.0f;
        }
    }
}

// NONMATCHING: 95.9% - the original loads the key character before the zero stores of the buffers,
// keeps the random text in a saved register (store to load forwarding of the local
// ptr_string_base) and allocates the loop registers in another order.
void SoftKeyboard::fn_80505510(int row, int col, Key* key) {
    char name[0x40] = {0};
    wchar_t c[8] = {0};
    if (key->m04 == 0x2193) {
        fn_80505970();
    } else if (key->m04 == 0x2190) {
        fn_80505AB4();
    } else if (key->m04 == L'+') {
        atn::ptr_string_base<wchar_t, 0> random(lbl_8087F86C->Get_unk(rand() % 50 + 0x59));
        if (wcslen(random.c_str()) != 0) {
            const wchar_t* src = random.c_str();
            if (src != mKeyboard.mText.m) {
                wmemcpy(mKeyboard.mText.m, src, wcslen(src) + 1);
            }
            m25B4 = wcslen(random.c_str());
            unsigned int i = 0;
            for (const wchar_t* p = random.mPtr; i < wcslen(random.c_str()); p++, i++) {
                sprintf(name, "string00%d", i + 1);
                c[0] = *p;
                c[1] = 0;
                SetPaneText_unk(m4C->mLayout, name, c);
            }
            for (; i < 8; i++) {
                sprintf(name, "string00%d", i + 1);
                c[0] = L'_';
                c[1] = 0;
                SetPaneText_unk(m4C->mLayout, name, c);
            }
        }
        fn_80117228(1);
    } else {
        if (m25B4 < 8) {
            mKeyboard.fn_800C6E60(key->m04);
            sprintf(name, "string00%d", m25B4 + 1);
            c[0] = key->m04;
            c[1] = 0;
            SetPaneText_unk(m4C->mLayout, name, c);
            if (++m25B4 >= 8) {
                mKeyboard.fn_800C7568(3, 10);
            }
        }
        fn_80117228(1);
    }
}

void SoftKeyboard::fn_805058E4(int row, int col, Key* key) {}

void SoftKeyboard::fn_805058E8(int row, int col, Key* key, int newRow, int newCol, Key* newKey) {
    if ((unsigned int)newRow <= 1) {
        if (newCol == 10) {
            m25B0 = newRow + 200;
        } else {
            m25B0 = newCol + newRow * 10;
        }
    } else if (newCol >= 9) {
        m25B0 = newRow + 200;
    } else {
        m25B0 = newCol + newRow * 9 + 2;
    }
    fn_80117228(3);
}

void SoftKeyboard::fn_80505970() {
    atn::Text_unk<wchar_t, 0x400> text = mKeyboard.mText;
    int blank;
    if (wcslen(text.m) == 0) {
        blank = 1;
    } else {
        unsigned int i;
        unsigned int spaces;
        const wchar_t* p;
        p = text.m;
        spaces = 0;
        for (i = 0; i < wcslen(text.m); p++, i++) {
            if (*p == L' ') {
                spaces++;
            }
        }
        blank = spaces >= wcslen(text.m);
    }
    if (!blank) {
        fn_80117228(1);
        fn_80505418();
    } else {
        D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 0);
        const wchar_t* msg = lbl_8087F1E4->mEntries[0x1CC].mText;
        D2ACommonDialog::sInstance->fn_804AD000(msg != 0 ? msg : lbl_808813D0, 0);
        D2ACommonDialog::sInstance->fn_804ACD10();
        m25AC = 4;
    }
}

void SoftKeyboard::fn_80505AB4() {
    mKeyboard.fn_800C6ED8();
    char name[0x40] = {0};
    wchar_t c[8] = {0};
    if (m25B4 > 0) {
        fn_80117228(8);
        sprintf(name, "string00%d", m25B4);
        SetTaskText_unk(m4C, name, L"_");
        m25B4--;
    }
}

void SoftKeyboard::fn_80505BB8() {
    if (wcslen(lbl_8087F628->m5F0) != 0) {
        m25B8 = 0;
        fn_80117228(2);
        fn_80505418();
    }
}

// NONMATCHING: 96.3% - only the store of the terminating zero of `c` is scheduled one slot later
// in the original (tried aggregate init, other orders).
void SoftKeyboard::fn_80505C1C(int row, int col, Key* key, int cursor) {
    if (cursor == 0 && mKeyboard.m2424 != 0 && key->m04 == mKeyboard.m2424->m04) {
        cursor = 1;
    }
    float scale = 1.0f;
    if (cursor != 0) {
        scale = 1.2f;
    }
    float z = -100.0f;
    if (cursor != 0) {
        z = -101.0f;
    }
    float x = key->mMin.x - 40.0f * (scale - 1.0f) / 2.0f;
    float y = key->mMin.y - 40.0f * (scale - 1.0f) / 2.0f;
    float w = scale * (key->mMax.x - key->mMin.x);
    float h = scale * (key->mMax.y - key->mMin.y);
    wchar_t c[2];
    unsigned int color = 0xFFFFFFFF;
    if (cursor != 0) {
        color = 0xFFCCCCFF;
    }
    c[0] = key->m04;
    c[1] = 0;
    lbl_8087EEB0->fn_80060D58(x, y, z, w, h, color);
    lbl_8087EEB0->fn_800610A4(0xFF000000, x, y, z, w, h);
    float s = 40.0f * scale;
    lbl_8087EEB0->fn_80061824(x + s / 4.0f, y + s / 4.0f, z, s / 2.0f, s / 2.0f, 0.0f, 0.0f, 0.0f, c, 0xFF000000, 1, 1, 0, 0, 0xFF000000);
}

} // namespace MP
