#ifndef ATN_CSV_H
#define ATN_CSV_H

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {

// CSV reader. The names are in the DOL: atn::CsvBase<char, 9, 13, 34> (separator, end of line,
// quote) and its typedef-like subclasses atn::Csv (','), atn::CsvTab ('\t'), atn::CsvPipe ('|'),
// atn::CsvU and atn::CsvTabU (wchar_t). The members are compiled once per instantiation in the
// engine (six functions each, e.g. 0x8005B5F8-0x8005BBF8 for the tab variant) and the game calls
// them out of line, so they are only declared here.
template <class T, int Sep, int Eol, int Quote>
class CsvBase {
public:
    // inferred: inlined at 0x803EDB18 (the virtual call goes through the CsvBase vtable)
    CsvBase(const T* text, int length) : m04(0), m08(0), m0C(0), m630(0) {
        memset(mField, 0, sizeof(mField));
        memset(m610, 0, sizeof(m610));
        vf08(text, length);
    }
    // inferred: without text (inlined at 0x8021F47C, the text is set later with vf08)
    CsvBase() : m04(0), m08(0), m0C(0), m630(0) {
        memset(mField, 0, sizeof(mField));
        memset(m610, 0, sizeof(m610));
    }
    // inferred: an empty destructor (the class has none in the DOL, but with it a wchar_t reader
    // built with the BOM-skipping constructor keeps its address in a saved register while the
    // inline constructor runs, as in 0x8011B8D4/0x803B9324; the char readers do not change)
    ~CsvBase() {}
    // inferred: sets the text to read (weak copies: tab 0x8004476C, comma 0x800420F4)
    virtual void vf08(const T* text, int length) {
        m04 = text;
        m08 = length;
        m0C = 0;
    }

    // inferred: reads the next field into mField and returns it (0 at the end of the data).
    // Tab instance at 0x8005B9CC, pipe instance at 0x8005BFCC.
    // Comma instance at 0x8005B3CC.
    // inferred: rereads the text from the start (inlined at 0x80531548); name invented
    void Reset_unk() {
        memset(mField, 0, sizeof(mField));
        memset(m610, 0, sizeof(m610));
        vf08(m04, m08);
    }
    const T* fn_8005B9CC();
    // inferred: current field (weak copy 0x800422CC in game/AreaEffConfig.cpp; name invented)
    const T* GetField_unk() { return mField; }
    int fn_8005BBF8(); // inferred: moves to the next line, 0 at the end (comma 0x8005B5F8)
    void fn_8005BCE8(const T* separators); // inferred (tab 0x8005BCE8, comma 0x8005B6E8)

    const T* m04; // inferred: text being read
    int m08;
    int m0C;      // inferred: read position
    // inferred: the wchar_t instance (0x8011B8D4) clears 0x800 and 0x40 bytes and keeps m630 at
    // 0xC50, so the arrays are sized in characters
    T mField[0x400]; // 0x10: current field
    T m410[0x200];
    T m610[0x20];
    int m630;
};

// inferred: the classes below only fix the template arguments (vtables 0x807772B0, 0x807774B8,
// 0x80782FB0, 0x8077A070, 0x8078B5E8)
extern "C" char lbl_8087D708[3]; // inferred: " \t"
extern "C" char lbl_8087D720[3]; // inferred: " \t"
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
// inferred: the constructor also sets the field separators (weak copy at 0x8004203C)
class Csv : public CsvBase<char, ',', '\r', '"'> {
public:
    Csv(const char* text, int length) : CsvBase<char, ',', '\r', '"'>(text, length) {
        fn_8005BCE8(lbl_8087D708);
    }
    // inferred: inlined at 0x8021F47C (another copy of " \t": lbl_8087D720)
    Csv() { fn_8005BCE8(lbl_8087D720); }
};
class CsvTab : public CsvBase<char, '\t', '\r', '"'> {
public:
    CsvTab(const char* text, int length) : CsvBase<char, '\t', '\r', '"'>(text, length) {}
    // inferred: weak copy 0x803EFCA8 (game/GimmickStateCommand.cpp): also sets the field separators
    CsvTab(const void* text, int length) : CsvBase<char, '\t', '\r', '"'>((const char*)text, length) {
        fn_8005BCE8(lbl_8087D710);
    }
};
class CsvPipe : public CsvBase<char, '|', '\r', '"'> {};
extern "C" wchar_t lbl_8077A0A8[3]; // inferred: L" \t"
extern "C" wchar_t lbl_807830B8[3]; // inferred: L" \t"
// inferred: the text starts with a byte order mark, which is skipped (constructor inlined at
// 0x8011B8D4, vf08 weak copy at 0x8011BB90)
class CsvU : public CsvBase<wchar_t, ',', '\r', '"'> {
public:
    CsvU(const wchar_t* text, int length)
        : CsvBase<wchar_t, ',', '\r', '"'>(length != 0 ? text + 1 : text, length != 0 ? length - 1 : length) {
        fn_8005BCE8(lbl_8077A0A8);
    }
    // inferred: without text (inlined at 0x8021AAB8, db/SkillGrowth_unk; another copy of L" \t":
    // lbl_807830B8, also used by MP/Text, MapLink_unk, SpText_unk)
    CsvU() { fn_8005BCE8(lbl_807830B8); }
    virtual void vf08(const wchar_t* text, int length) {
        m04 = length != 0 ? text + 1 : text;
        if (length != 0) {
            length--;
        }
        m08 = length;
        m0C = 0;
    }
};
extern "C" wchar_t lbl_8087DD98[2]; // inferred: L"\t"
// inferred: as CsvU (constructor inlined at 0x803B9324, vf08 weak copy at 0x803B97D8)
class CsvTabU : public CsvBase<wchar_t, '\t', '\r', '"'> {
public:
    CsvTabU(const wchar_t* text, int length)
        : CsvBase<wchar_t, '\t', '\r', '"'>(length != 0 ? text + 1 : text, length != 0 ? length - 1 : length) {
        fn_8005BCE8(lbl_8087DD98);
    }
    virtual void vf08(const wchar_t* text, int length) {
        if (length != 0) {
            text++;
        }
        m04 = text;
        if (length != 0) {
            length--;
        }
        m08 = length;
        m0C = 0;
    }
};

} // namespace atn

#endif
