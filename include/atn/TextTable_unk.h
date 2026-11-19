#ifndef ATN_TEXTTABLE_UNK_H
#define ATN_TEXTTABLE_UNK_H

namespace atn {
// inferred: pointer to a string owned elsewhere (vptr + pointer; constructor 0x8020C4B0)
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    virtual ~ptr_string_base() {}
    T* mPtr;
};
} // namespace atn

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text

// inferred: table of the game texts read from two UTF-16 CSV files (0x1018 bytes, singleton lbl_8087F1E4,
// atn/ptr_string_base_wchar_t_0.cpp); names invented
class TextTable_unk {
public:
    TextTable_unk();
    ~TextTable_unk();
    static void fn_8020C028(); // inferred: creates the singleton

    // inferred: text of entry `id`, or the empty text
    const wchar_t* GetPair_unk(int id) { return mStrings[id].mPtr != 0 ? mStrings[id].mPtr : lbl_808813D0; }
    // inferred: the game also reads the table as 32-bit words: word 2 * i + 1 is the text of entry i (a
    // 32-bit layout; on a 64-bit build this must become mStrings[id >> 1].mPtr)
    const wchar_t* Get_unk(int id) {
        return reinterpret_cast<const wchar_t* const*>(mStrings)[id] != 0
                   ? reinterpret_cast<const wchar_t* const*>(mStrings)[id]
                   : lbl_808813D0;
    }

    atn::ptr_string_base<wchar_t, 0> mStrings[0x203];
};
extern "C" TextTable_unk* lbl_8087F1E4;

#endif
