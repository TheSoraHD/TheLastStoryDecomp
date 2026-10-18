#ifndef ATN_CSV_H
#define ATN_CSV_H

namespace atn {

// CSV reader. The names are in the DOL: atn::CsvBase<char, 9, 13, 34> (separator, end of line,
// quote) and its typedef-like subclasses atn::Csv (','), atn::CsvTab ('\t'), atn::CsvPipe ('|'),
// atn::CsvU and atn::CsvTabU (wchar_t). The members are compiled once per instantiation in the
// engine (six functions each, e.g. 0x8005B5F8-0x8005BBF8 for the tab variant) and the game calls
// them out of line, so they are only declared here.
template <class T, int Sep, int Eol, int Quote>
class CsvBase {
public:
    virtual ~CsvBase();

    // inferred: reads the next field into mField and returns it (0 at the end of the data).
    // Tab instance at 0x8005B9CC, pipe instance at 0x8005BFCC.
    const T* fn_8005B9CC();

    const T* m04; // inferred: text being read
    int m08;
    int m0C;      // inferred: read position
    T mField[0x600 / sizeof(T)]; // 0x10: current field
};

// inferred: the classes below only fix the template arguments (vtables 0x807772B0, 0x807774B8,
// 0x80782FB0, 0x8077A070, 0x8078B5E8)
class Csv : public CsvBase<char, ',', '\r', '"'> {};
class CsvTab : public CsvBase<char, '\t', '\r', '"'> {};
class CsvPipe : public CsvBase<char, '|', '\r', '"'> {};
class CsvU : public CsvBase<wchar_t, ',', '\r', '"'> {};
class CsvTabU : public CsvBase<wchar_t, '\t', '\r', '"'> {};

} // namespace atn

#endif
