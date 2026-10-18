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
        memset(mField, 0, 0x400);
        memset(m610, 0, sizeof(m610));
        vf08(text, length);
    }
    // inferred: sets the text to read (weak copies: tab 0x8004476C, comma 0x800420F4)
    virtual void vf08(const T* text, int length) {
        m04 = text;
        m08 = length;
        m0C = 0;
    }

    // inferred: reads the next field into mField and returns it (0 at the end of the data).
    // Tab instance at 0x8005B9CC, pipe instance at 0x8005BFCC.
    // Comma instance at 0x8005B3CC.
    const T* fn_8005B9CC();
    int fn_8005BBF8(); // inferred: moves to the next line, 0 at the end (comma 0x8005B5F8)
    void fn_8005BCE8(const T* separators); // inferred (tab 0x8005BCE8, comma 0x8005B6E8)

    const T* m04; // inferred: text being read
    int m08;
    int m0C;      // inferred: read position
    T mField[0x600 / sizeof(T)]; // 0x10: current field
    unsigned char m610[0x20];
    int m630;
};

// inferred: the classes below only fix the template arguments (vtables 0x807772B0, 0x807774B8,
// 0x80782FB0, 0x8077A070, 0x8078B5E8)
extern "C" char lbl_8087D708[3]; // inferred: " \t"
// inferred: the constructor also sets the field separators (weak copy at 0x8004203C)
class Csv : public CsvBase<char, ',', '\r', '"'> {
public:
    Csv(const char* text, int length) : CsvBase<char, ',', '\r', '"'>(text, length) {
        fn_8005BCE8(lbl_8087D708);
    }
};
class CsvTab : public CsvBase<char, '\t', '\r', '"'> {};
class CsvPipe : public CsvBase<char, '|', '\r', '"'> {};
class CsvU : public CsvBase<wchar_t, ',', '\r', '"'> {};
class CsvTabU : public CsvBase<wchar_t, '\t', '\r', '"'> {};

} // namespace atn

#endif
