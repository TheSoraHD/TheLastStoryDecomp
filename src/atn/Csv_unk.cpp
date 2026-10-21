// atn/Csv_unk.cpp: out-of-line members of the CSV reader atn::CsvBase (file name invented; the
// engine file is probably atn_csv.cpp, between atn_colli and atn_curve). The members are
// instantiated here for the comma, tab and pipe readers (char and wchar_t); the game calls them
// out of line. The pipe reader only has the two members it uses.
#include "atn/Csv.h"

namespace atn {

// inferred: Shift-JIS lead byte (0x80-0x9F): the next byte belongs to the same character; wide
// characters have none (names invented)
inline bool __IsLeadByte_unk(char c) {
    return (unsigned char)(c + 0x80) <= 0x1F;
}
inline bool __IsLeadByte_unk(wchar_t c) {
    return false;
}

// inferred: c is one of the characters of the null-terminated list (name invented)
template <class T>
inline bool __IsOneOf_unk(T c, const T* list) {
    for (int i = 0; list[i] != 0; i++) {
        if (c == list[i]) {
            return true;
        }
    }
    return false;
}

// NONMATCHING: 99.68% (char) / 98.93% (wchar_t) - numeracion de registros en el recorte inicial
template <class T, int Sep, int Eol, int Quote>
const T* CsvBase<T, Sep, Eol, Quote>::fn_8005B9CC() {
    memset(mField, 0, sizeof(mField) + sizeof(m410));
    int quoted = 0;
    int open = 1;
    int len = 0;
    unsigned int pos = m0C;
    for (; pos < (unsigned int)m08; pos++) {
        if (quoted) {
            if (m04[pos] == Quote) {
                if (m04[pos + 1] == Quote) {
                    mField[len++] = m04[pos];
                    pos++;
                } else {
                    quoted = 0;
                }
            } else {
                mField[len++] = m04[pos];
                if (__IsLeadByte_unk(m04[pos])) {
                    pos++;
                    mField[len++] = m04[pos];
                }
            }
        } else {
            T c = m04[pos];
            if (c == Quote) {
                quoted = 1;
            } else if (c == Sep) {
                m0C = pos + 1;
                open = 0;
                break;
            } else if (c == Eol) {
                m0C = pos;
                open = 0;
                break;
            } else {
                mField[len++] = m04[pos];
                if (__IsLeadByte_unk(m04[pos])) {
                    pos++;
                    mField[len++] = m04[pos];
                }
            }
        }
    }
    if (open && (unsigned int)(len + m0C) >= (unsigned int)m08) {
        m0C = m08;
    }
    if (m630 != 0) {
        int i;
        int n = 0;
        for (i = 0; mField[i] != 0; i++) {
            if (!__IsOneOf_unk(mField[i], m610)) {
                break;
            }
            n++;
        }
        if (n > 0) {
            T c;
            for (i = 0; (c = mField[n + i]) != 0; i++) {
                mField[i] = c;
            }
            mField[i] = 0;
        }
    }
    return mField;
}

template <class T, int Sep, int Eol, int Quote>
int CsvBase<T, Sep, Eol, Quote>::fn_8005BBF8() {
    int quoted = 0;
    for (; (unsigned int)m0C < (unsigned int)m08; m0C++) {
        if (quoted) {
            if (m04[m0C] == Quote) {
                if (m04[m0C + 1] == Quote) {
                    m0C++;
                } else {
                    quoted = 0;
                }
            } else if (__IsLeadByte_unk(m04[m0C])) {
                m0C++;
            }
        } else {
            T c = m04[m0C];
            if (c == Quote) {
                quoted = 1;
            } else if (c == Eol) {
                m0C += 2;
                return (unsigned int)m0C < (unsigned int)m08;
            } else if (__IsLeadByte_unk(m04[m0C])) {
                m0C++;
            }
        }
    }
    return 0;
}

template <class T, int Sep, int Eol, int Quote>
void CsvBase<T, Sep, Eol, Quote>::fn_8005BCE8(const T* separators) {
    for (int i = 0;; i++) {
        m610[i] = separators[i];
        if (separators[i] == 0) {
            return;
        }
    }
}

template const char* CsvBase<char, ',', '\r', '"'>::fn_8005B9CC();
template int CsvBase<char, ',', '\r', '"'>::fn_8005BBF8();
template void CsvBase<char, ',', '\r', '"'>::fn_8005BCE8(const char*);
template const wchar_t* CsvBase<wchar_t, ',', '\r', '"'>::fn_8005B9CC();
template int CsvBase<wchar_t, ',', '\r', '"'>::fn_8005BBF8();
template void CsvBase<wchar_t, ',', '\r', '"'>::fn_8005BCE8(const wchar_t*);
template const char* CsvBase<char, '\t', '\r', '"'>::fn_8005B9CC();
template int CsvBase<char, '\t', '\r', '"'>::fn_8005BBF8();
template void CsvBase<char, '\t', '\r', '"'>::fn_8005BCE8(const char*);
template const wchar_t* CsvBase<wchar_t, '\t', '\r', '"'>::fn_8005B9CC();
template int CsvBase<wchar_t, '\t', '\r', '"'>::fn_8005BBF8();
template void CsvBase<wchar_t, '\t', '\r', '"'>::fn_8005BCE8(const wchar_t*);
template const char* CsvBase<char, '|', '\r', '"'>::fn_8005B9CC();
template void CsvBase<char, '|', '\r', '"'>::fn_8005BCE8(const char*);

} // namespace atn

// inferred: bit set of words from 0x08 (atn/PacketPack.cpp fn_804D0140 tests bit `bit` of word
// `word`); the owner of this function is unknown, it stays in this unit (no data to place it)
struct BitWords_unk {
    unsigned char m00[0x08];
    unsigned int mWords[1]; // 0x08
};

extern "C" int fn_8005C220(const BitWords_unk* bits, int word, int bit) {
    unsigned int mask = 1 << bit;
    return (bits->mWords[word] & mask) == mask;
}
