// atn/WordWrap_unk.cpp (file name invented): one source file of the game, 0x800DF22C-0x800E2A24.
// It was split in three units (atn/WordWrap_unk, std/char_traits_char, std/allocator_char) until
// 2026-10-29; contiguous .text/.rodata/.data, one .ctors entry (the iostream __sinit) at the end.
// Contents in DOL order: word wrap of the game texts (kinsoku), the weak std::wstring/ios_base
// copies, basic_ios<char>::~basic_ios (0x800E0AC0), the weak MSL stream destructors, the tagged text
// reader (TextParser_unk, 0x800E0CF0-0x800E19AC) and the rest of the weak MSL iostream code.
//
// atn/WordWrap_unk.cpp: word wrap of the game texts (file name invented). Line breaks follow the
// Japanese kinsoku rules (table of characters that cannot start or end a line).
#include "std/string.h"
#include "std/vector.h"
#include "std/string_members.h"
#include "std/ostream.h"
#include "std/sstream.h"
#include "std/ios_init.h"

extern "C" unsigned long wcslen(const wchar_t* s);

// inferred: font of the game texts and the width of one character
extern "C" void* lbl_8087EEC8;
extern "C" float fn_8006F17C(void* font, wchar_t c, int a, int b, float size);

// inferred: kinsoku table (sorted by character)
struct KinsokuEntry_unk {
    wchar_t mChar;
    unsigned char mNoStart; // inferred: cannot start a line
    unsigned char mNoEnd;   // inferred: cannot end a line
};

static const KinsokuEntry_unk sKinsoku[138] = {
    {0x0021, 1, 0}, {0x0024, 0, 1}, {0x0025, 1, 0}, {0x0027, 1, 1}, {0x0028, 0, 1}, {0x0029, 1, 0},
    {0x002E, 1, 1}, {0x002F, 1, 1}, {0x003A, 1, 0}, {0x003B, 1, 0}, {0x003F, 1, 0}, {0x005B, 0, 1},
    {0x005C, 0, 1}, {0x005D, 1, 0}, {0x007B, 0, 1}, {0x007D, 1, 0}, {0x00A2, 1, 0}, {0x00A3, 0, 1},
    {0x00A5, 0, 1}, {0x00A7, 0, 1}, {0x00A8, 1, 0}, {0x00A9, 1, 0}, {0x00AE, 1, 0}, {0x00B0, 1, 0},
    {0x00B7, 1, 0}, {0x02C7, 1, 0}, {0x02C9, 1, 0}, {0x2013, 1, 0}, {0x2014, 1, 0}, {0x2015, 1, 0},
    {0x2016, 1, 0}, {0x2018, 0, 1}, {0x2019, 1, 0}, {0x201C, 0, 1}, {0x201D, 1, 0}, {0x2025, 1, 0},
    {0x2026, 1, 0}, {0x2027, 1, 0}, {0x2032, 1, 0}, {0x2033, 1, 0}, {0x2035, 0, 1}, {0x2103, 1, 0},
    {0x2122, 1, 0}, {0x2236, 1, 0}, {0x2574, 1, 0}, {0x266F, 0, 1}, {0x3001, 1, 0}, {0x3002, 1, 0},
    {0x3005, 1, 0}, {0x3003, 1, 0}, {0x3008, 0, 1}, {0x3009, 1, 0}, {0x300A, 0, 1}, {0x300B, 1, 0},
    {0x300C, 0, 1}, {0x300D, 1, 0}, {0x300E, 0, 1}, {0x300F, 1, 0}, {0x3010, 0, 1}, {0x3011, 1, 0},
    {0x3012, 0, 1}, {0x3014, 0, 1}, {0x3015, 1, 0}, {0x3016, 0, 1}, {0x3017, 1, 0}, {0x301D, 0, 1},
    {0x301E, 1, 0}, {0x301F, 1, 0}, {0x3041, 1, 0}, {0x3043, 1, 0}, {0x3045, 1, 0}, {0x3047, 1, 0},
    {0x3049, 1, 0}, {0x3063, 1, 0}, {0x3083, 1, 0}, {0x3085, 1, 0}, {0x3087, 1, 0}, {0x308E, 1, 0},
    {0x3099, 1, 0}, {0x309A, 1, 0}, {0x309B, 1, 0}, {0x309C, 1, 0}, {0x309D, 1, 0}, {0x309E, 1, 0},
    {0x30A1, 1, 0}, {0x30A3, 1, 0}, {0x30A5, 1, 0}, {0x30A7, 1, 0}, {0x30A9, 1, 0}, {0x30C3, 1, 0},
    {0x30E3, 1, 0}, {0x30E5, 1, 0}, {0x30E7, 1, 0}, {0x30EE, 1, 0}, {0x30F5, 1, 0}, {0x30F6, 1, 0},
    {0x30FB, 1, 0}, {0x30FC, 1, 0}, {0x30FD, 1, 0}, {0x30FE, 1, 0}, {0xFE50, 1, 0}, {0xFE51, 1, 0},
    {0xFE52, 1, 0}, {0xFE54, 1, 0}, {0xFE55, 1, 0}, {0xFE56, 1, 0}, {0xFE57, 1, 0}, {0xFE59, 0, 1},
    {0xFE5A, 1, 0}, {0xFE5B, 0, 1}, {0xFE5C, 1, 0}, {0xFE5D, 0, 1}, {0xFE5E, 1, 0}, {0xFF01, 1, 0},
    {0xFF02, 1, 0}, {0xFF04, 0, 1}, {0xFF05, 1, 0}, {0xFF07, 1, 0}, {0xFF08, 0, 1}, {0xFF09, 1, 0},
    {0xFF0C, 1, 0}, {0xFF0E, 1, 0}, {0xFF1A, 1, 0}, {0xFF1B, 1, 0}, {0xFF1F, 1, 0}, {0xFF20, 0, 1},
    {0xFF3B, 0, 1}, {0xFF3D, 1, 0}, {0xFF40, 1, 0}, {0xFF5B, 0, 1}, {0xFF5C, 1, 0}, {0xFF5D, 1, 0},
    {0xFF5E, 1, 0}, {0xFFE0, 1, 1}, {0xFFE1, 0, 1}, {0xFFE5, 0, 1}, {0xFFE6, 0, 1}, {0x0000, 0, 0},
};

static inline const KinsokuEntry_unk* FindKinsoku_unk(wchar_t c) {
    int lo = 0;
    int hi = 0x89;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (c == sKinsoku[mid].mChar) {
            return &sKinsoku[mid];
        }
        if (c < sKinsoku[mid].mChar) {
            hi = mid - 1;
        }
        if (c >= sKinsoku[mid].mChar) {
            lo = mid + 1;
        }
    }
    return 0;
}

static inline bool IsNoStart_unk(wchar_t c) {
    const KinsokuEntry_unk* e = FindKinsoku_unk(c);
    return e != 0 ? e->mNoStart : 0;
}

static inline bool IsNoEnd_unk(wchar_t c) {
    const KinsokuEntry_unk* e = FindKinsoku_unk(c);
    return e != 0 ? e->mNoEnd : 0;
}

static inline bool IsSpace_unk(wchar_t c) {
    return c == 9 || c == 13 || c == 0x20 || c == 0x3000;
}

// inferred: characters of the scripts without spaces (Hangul Jamo, CJK, compatibility, full width)
static inline bool IsWide_unk(wchar_t c) {
    return (c >= 0x1100 && c <= 0x11FF) || (c >= 0x3000 && c <= 0xD7AF) || (c >= 0xF900 && c <= 0xFAFF) ||
           (c >= 0xFF00 && c <= 0xFFDC);
}

static inline bool NoStartOf_unk(const KinsokuEntry_unk* e) {
    return e != 0 ? e->mNoStart : 0;
}

static inline bool NoEndOf_unk(const KinsokuEntry_unk* e) {
    return e != 0 ? e->mNoEnd : 0;
}

static inline bool CanBreak_unk(wchar_t prev, wchar_t c) {
    return (IsSpace_unk(c) || IsWide_unk(c) || IsWide_unk(prev) || prev == 0x2D) &&
           !NoStartOf_unk(FindKinsoku_unk(c)) && !NoEndOf_unk(FindKinsoku_unk(prev));
}

extern "C" {

// inferred: true if a line can be broken before `p`
bool fn_800DF22C(const wchar_t* p, const wchar_t* begin) {
    if (p == begin) {
        return false;
    }
    wchar_t c = p[0];
    if (IsSpace_unk(c) && IsNoStart_unk(p[1])) {
        return false;
    }
    if (p - begin > 1 && IsSpace_unk(p[-2]) && p[-1] == 0x22 && !IsSpace_unk(c)) {
        return false;
    }
    if (!IsSpace_unk(p[-1]) && c == 0x22 && IsSpace_unk(p[1])) {
        return false;
    }
    CanBreak_unk(p[-1], c);
    return CanBreak_unk(p[-1], c);
}

// inferred: skips the spaces at the start of a line (and one line feed); null at the end of the text
const wchar_t* fn_800DFADC(const wchar_t* p) {
    while (IsSpace_unk(*p)) {
        p++;
    }
    if (p != 0 && *p == 0xA) {
        p++;
    }
    if (*p == 0) {
        return 0;
    }
    return p;
}

// inferred: measures one character; returns < 0 to use the font
typedef float (*MeasureChar_unk)(wchar_t c, const wchar_t** p, float size);

// NONMATCHING: 75.8% - word wrap of `text` to `maxWidth` (inserts line feeds). Same blocks; the
// original loads the iterator tag of each appended character once before the loops and keeps the
// stack addresses out of registers (wstring += wchar_t of include/std/string.h may differ from MSL).
std::basic_string<wchar_t> fn_800DFB40(const wchar_t* text, float maxWidth, float size, float spacing, int a,
                                       int b, MeasureChar_unk measure, int mode) {
    std::basic_string<wchar_t> result;
    float width = 0.0f;
    const wchar_t* p = text;
    const wchar_t* line = text;
    do {
        wchar_t c = *p++;
        if (c == 0) {
            break;
        }
        if (c == 0xD) {
            continue;
        }
        if (c == 0xA) {
            int end = p - text;
            for (int i = line - text; i < end; i++) {
                result += text[i];
            }
            line = p;
            width = 0.0f;
            continue;
        }
        if (c == 0x5C && *p == 0x6E) {
            int end = p - text;
            for (int i = line - text; i < end; i++) {
                if (i == end - 1) {
                    result += (wchar_t)0xA;
                } else {
                    result += text[i];
                }
            }
            p++;
            line = p;
            width = 0.0f;
            continue;
        }
        float w;
        if (measure != 0) {
            w = measure(c, &p, size);
            if (w < 0.0f) {
                w = fn_8006F17C(lbl_8087EEC8, c, a, b, size);
            }
        } else {
            w = fn_8006F17C(lbl_8087EEC8, c, a, b, size);
        }
        width += w;
        width += spacing;
        if (width > maxWidth) {
            if (mode == 0) {
                const wchar_t* last = p - 1;
                while (--p > line) {
                    if (fn_800DF22C(p, line) || (*p == 0x20 && p[-1] == 0x2E)) {
                        break;
                    }
                }
                if (p <= line) {
                    if (last > line) {
                        p = last - 1;
                    } else {
                        p = last;
                    }
                }
            } else {
                p--;
                if (!fn_800DF22C(p, line)) {
                    p++;
                }
            }
            int end = p - text;
            for (int i = line - text; i < end; i++) {
                result += text[i];
            }
            result += (wchar_t)0xA;
            p = fn_800DFADC(p);
            line = p;
            width = 0.0f;
        }
    } while (p != 0);
    if (line != 0) {
        result.append(line);
    }
    return result;
}

// inferred: accessors of a small container used by game/Em203GreaterDaemon (class unknown, names
// invented; the word at +4 is a count)
struct Count4_unk {
    void* m00;
    int m04;
};

int fn_800E0AA8(const Count4_unk* c) {
    return c->m04;
}

bool fn_800E0AB0(const Count4_unk* c) {
    return c->m04 == 0;
}

} // extern "C"

// Not decompiled yet: fn_800E0000 (splits the wrapped text into a std::vector<std::wstring>), the weak
// copy of the std::wstring substring constructor (0x800E0908) and the weak MSL iostream helper
// 0x800E0A64 (basic_ios::clear).

// --- former std/allocator_char.cpp ---
// std/allocator_char.cpp: reader of the tagged text files (TextParser_unk, 0x800E0CF0-0x800E19AC) and
// the weak MSL iostream code around it (std::basic_stringbuf<char>, std::basic_ostream<char> and
// its sentry, the iostream __sinit). The unit name comes from the old automatic split; the real
// file is a game file (probably the same as atn/WordWrap_unk and std/char_traits_char, see
// docs/batch-auto-10kb-qA2.md): basic_ios<char>::~basic_ios (0x800E0AC0) and the RTTI of
// basic_ostream/basic_ios are in those splits. The weak copies are explicit instantiations here,
// in the order of the DOL. Parser and MSL stream internals are partially reconstructed.

namespace std {

// inferred: deleting destructors generated by the compiler (virtual base basic_ios at 0x08)
template <class C, class T>
basic_ostream<C, T>::~basic_ostream() {}

template <class C, class T, class A>
basic_stringbuf<C, T, A>::~basic_stringbuf() {}

template <class C, class T>
basic_streambuf<C, T>::~basic_streambuf() {}

// inferred: flushes the tied stream; the sentry of the tied stream is marked (no flush from its
// destructor) and a failure sets failbit
// NONMATCHING: 35.00% (ctor), 38.21% (dtor), 36.12% (flush) - the inlining depth differs: the
// original keeps the flush call in the inlined ~sentry of the tied stream and inlines
// setstate(failbit) here (setstate is called out of line elsewhere)
template <class C, class T>
basic_ostream<C, T>::sentry::sentry(basic_ostream<C, T>& os) : mOk(false), mNested_unk(false), mOs(&os) {
    if (os.good()) {
        if (os.tie() != 0) {
            sentry s(*os.tie());
            s.mNested_unk = true;
            if (os.tie()->rdbuf() != 0 && os.tie()->rdbuf()->pubsync() == -1) {
                os.tie()->setstate(ios_base::badbit);
            }
        }
        if (mOs->good()) {
            mOk = true;
            return;
        }
    }
    mOs->setstate(ios_base::failbit);
}

// inferred: unitbuf flush
template <class C, class T>
basic_ostream<C, T>::sentry::~sentry() {
    if (!mOs->fail() && (mOs->flags() & ios_base::unitbuf) != 0 && !mNested_unk) {
        mOs->flush();
    }
}

template <class C, class T>
basic_ostream<C, T>& basic_ostream<C, T>::flush() {
    sentry s(*this);
    s.mNested_unk = true;
    if (this->rdbuf() != 0 && this->rdbuf()->pubsync() == -1) {
        this->setstate(ios_base::badbit);
    }
    return *this;
}

template <class C, class T>
int basic_streambuf<C, T>::sync() {
    return 0;
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::overflow(int_type) {
    return -1;
}

// NONMATCHING: 99.35% - register allocation
template <class C, class T, class A>
typename basic_stringbuf<C, T, A>::int_type basic_stringbuf<C, T, A>::underflow() {
    if (this->mGptr == 0) {
        return -1;
    }
    if (mHigh < this->mPptr) {
        mHigh = this->mPptr;
    }
    C* end = mHigh;
    if (this->mEgptr < end) {
        this->mEgptr = end;
    }
    if (this->mGptr < this->mEgptr) {
        return (unsigned char)*this->mGptr;
    }
    return -1;
}

// inferred: putback one character, accepting a different character only in output mode
// NONMATCHING: 77.66% - compare of the putback character
template <class C, class T, class A>
typename basic_stringbuf<C, T, A>::int_type basic_stringbuf<C, T, A>::pbackfail(int_type c) {
    if (this->mEback >= this->mGptr) {
        return -1;
    }
    if (c == -1) {
        this->mGptr--;
        return 0;
    }
    if ((mMode & ios_base::out) == 0 && (signed char)this->mGptr[-1] != (signed char)c) {
        return -1;
    }
    this->mGptr--;
    *this->mGptr = (C)c;
    return c;
}

// NONMATCHING: 73.95% - growth of the string (inlined replace)
template <class C, class T, class A>
typename basic_stringbuf<C, T, A>::int_type basic_stringbuf<C, T, A>::overflow(int_type c) {
    if ((mMode & ios_base::out) == 0) {
        return -1;
    }
    if (c == -1) {
        return 0;
    }
    if (this->mPptr < this->mEpptr) {
        *this->mPptr = (C)c;
        this->mPptr++;
        if (mHigh < this->mPptr) {
            mHigh = this->mPptr;
        }
        if ((mMode & ios_base::in) != 0 && this->mEgptr < mHigh) {
            this->mEgptr = mHigh;
        }
        return c;
    }

    unsigned int putOffset = (unsigned int)(this->mPptr - this->mPbase);
    unsigned int highOffset = (unsigned int)(mHigh - this->mPbase);
    unsigned int getOffset = (unsigned int)(this->mGptr - this->mEback);
    unsigned int newPutOffset = putOffset + 1;
    C ch = (C)c;
    mStr.do_replace(mStr.size(), 0, &ch, &ch + 1, random_access_iterator_tag());
    unsigned int capacity = (unsigned int)mStr.capacity();
    unsigned int size = (unsigned int)mStr.size();
    if (capacity - 1 > size) {
        mStr.replace(size, 0, capacity - 1 - size, (C)0);
    } else {
        mStr.replace(capacity - 1, size - (capacity - 1), 0, (C)0);
    }
    C* data = (C*)mStr.data();
    size = (unsigned int)mStr.size();
    if ((mMode & ios_base::in) != 0) {
        this->mEback = data;
        this->mGptr = data + getOffset;
        this->mEgptr = data + highOffset;
    }
    this->mPbase = data;
    this->mPptr = data + newPutOffset;
    this->mEpptr = data + size;
    mHigh = data + size;
    return c;
}

// inferred: seekoff/seekpos return an 8-byte streamoff plus a 4-byte conversion state
// NONMATCHING: 70.84% - branch layout of the mode checks and the 64-bit compare
template <class C, class T, class A>
typename basic_stringbuf<C, T, A>::pos_type basic_stringbuf<C, T, A>::seekoff(off_type off, ios_base::seekdir way,
                                                                           ios_base::openmode which) {
    unsigned int mode = which & (ios_base::in | ios_base::out);
    if (mode == 0 || (mode == (ios_base::in | ios_base::out) && way == ios_base::cur)) {
        return pos_type(-1);
    }
    bool input = (mode & ios_base::out) != 0;
    bool output = (mode & ios_base::in) != 0;
    if ((output && (mMode & ios_base::in) == 0) || (input && (mMode & ios_base::out) == 0)) {
        return pos_type(-1);
    }
    if ((input && this->mGptr == 0) || (output && this->mPptr == 0)) {
        return pos_type(-1);
    }
    if (this->mPptr != 0 && mHigh < this->mPptr) {
        mHigh = this->mPptr;
    }
    if (this->mEgptr != 0 && this->mEgptr < mHigh) {
        this->mEgptr = mHigh;
    }
    int extent;
    int base;
    if (input) {
        extent = (int)(mHigh - this->mPbase);
        if (way == ios_base::beg) {
            base = 0;
        } else if (way == ios_base::cur) {
            base = (int)(this->mPptr - this->mPbase);
        } else if (way == ios_base::end) {
            base = extent;
        } else {
            return pos_type(-1);
        }
    } else {
        extent = (int)(this->mEgptr - this->mEback);
        if (way == ios_base::beg) {
            base = 0;
        } else if (way == ios_base::cur) {
            base = (int)(this->mGptr - this->mEback);
        } else if (way == ios_base::end) {
            base = extent;
        } else {
            return pos_type(-1);
        }
    }
    off_type position = off + base;
    if (position < 0 || position > extent) {
        return pos_type(-1);
    }
    if ((mMode & ios_base::app) != 0 && input && position != extent) {
        return pos_type(-1);
    }
    if (input) {
        this->mGptr = this->mEback + (int)position;
    }
    if (output) {
        this->mPptr = this->mPbase + (int)position;
    }
    return pos_type(position);
}

// NONMATCHING: 89.62% - same as seekoff
template <class C, class T, class A>
typename basic_stringbuf<C, T, A>::pos_type basic_stringbuf<C, T, A>::seekpos(pos_type sp, ios_base::openmode which) {
    unsigned int mode = which & (ios_base::in | ios_base::out);
    if (mode == 0) {
        return pos_type(-1);
    }
    bool input = (mode & ios_base::out) != 0;
    bool output = (mode & ios_base::in) != 0;
    if ((output && (mMode & ios_base::in) == 0) || (input && (mMode & ios_base::out) == 0)) {
        return pos_type(-1);
    }
    if ((input && this->mGptr == 0) || (output && this->mPptr == 0)) {
        return pos_type(-1);
    }
    if (this->mPptr != 0 && mHigh < this->mPptr) {
        mHigh = this->mPptr;
    }
    if (this->mEgptr != 0 && this->mEgptr < mHigh) {
        this->mEgptr = mHigh;
    }
    int extent = input ? (int)(mHigh - this->mPbase) : (int)(this->mEgptr - this->mEback);
    off_type position = sp.mOff;
    if (position < 0 || position > extent) {
        return pos_type(-1);
    }
    if ((mMode & ios_base::app) != 0 && input && position != extent) {
        return pos_type(-1);
    }
    if (input) {
        this->mGptr = this->mEback + (int)position;
    }
    if (output) {
        this->mPptr = this->mPbase + (int)position;
    }
    return sp;
}

template <class C, class T>
void basic_streambuf<C, T>::imbue(const locale&) {}

template <class C, class T>
basic_streambuf<C, T>* basic_streambuf<C, T>::setbuf(char_type*, streamsize) {
    return this;
}

template <class C, class T>
typename basic_streambuf<C, T>::pos_type basic_streambuf<C, T>::seekoff(off_type, ios_base::seekdir, ios_base::openmode) {
    return pos_type(-1);
}

template <class C, class T>
typename basic_streambuf<C, T>::pos_type basic_streambuf<C, T>::seekpos(pos_type, ios_base::openmode) {
    return pos_type(-1);
}

template <class C, class T>
streamsize basic_streambuf<C, T>::showmanyc() {
    return 0;
}

// inferred: read a sequence, using uflow when the get area is exhausted
// NONMATCHING: 96.51% - min() through a pointer, registers of the loop
template <class C, class T>
streamsize basic_streambuf<C, T>::xsgetn(char_type* s, streamsize n) {
    streamsize available;
    streamsize remaining = n;
    streamsize* selected = &remaining;
    available = (streamsize)(mEgptr - mGptr);
    if (available < remaining) {
        selected = &available;
    }
    streamsize count = *selected;
    if (count > 0) {
        memcpy(s, mGptr, count);
        mGptr += count;
        s += count;
        remaining -= count;
    }
    streamsize read = count;
    while (remaining > 0) {
        int_type c;
        if (mGptr < mEgptr) {
            char_type* current = mGptr;
            mGptr = current + 1;
            c = (unsigned char)*current;
        } else {
            c = uflow();
        }
        if (c == -1) {
            break;
        }
        *s = (char_type)c;
        read++;
        s++;
        remaining--;
    }
    return read;
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::underflow() {
    return -1;
}

// inferred: consume one character after asking underflow to make the get area valid
template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::uflow() {
    if (underflow() == -1) {
        return -1;
    }
    char_type* current = mGptr;
    mGptr = current + 1;
    return (unsigned char)*current;
}

template <class C, class T>
typename basic_streambuf<C, T>::int_type basic_streambuf<C, T>::pbackfail(int_type) {
    return -1;
}

// inferred: write a sequence, using overflow when the put area is exhausted
// NONMATCHING: 99.47% - registers of the loop
template <class C, class T>
streamsize basic_streambuf<C, T>::xsputn(const char_type* s, streamsize n) {
    streamsize available;
    streamsize remaining = n;
    available = (streamsize)(mEpptr - mPptr);
    streamsize& selected = available < remaining ? available : remaining;
    streamsize count = selected;
    if (count > 0) {
        memcpy(mPptr, s, count);
        mPptr += count;
        s += count;
        remaining -= count;
    }
    streamsize written = count;
    while (remaining > 0) {
        int_type c;
        char_type ch = *s++;
        char_type* end = mEpptr;
        char_type* current = mPptr;
        if (current < end) {
            *current = ch;
            mPptr = current + 1;
            c = (unsigned char)*current;
        } else {
            c = overflow((unsigned char)ch);
        }
        if (c == -1) {
            break;
        }
        written++;
        remaining--;
    }
    return written;
}

} // namespace std

// inferred: reader of the tagged text files ("<Name args...>", "</Name>", "<!-- -->"); token type 1
// opens a block, 2 closes it, 3 is the text after a block, 4 a comment (names invented)
struct TextParser_unk {
    TextParser_unk(void* data, void* size); // inferred: size passed as a pointer-sized word

    const char* mData;
    unsigned int mSize;
    unsigned int mPos;
    std::string mName;  // 0x0C
    std::string mLast;  // 0x18
    char mBuf[0x100];   // 0x24
    int mType;          // 0x124
    int mClosed;        // 0x128: inferred: the last block was "<Name/>"
};

static inline bool IsSpace_unk(char c) {
    return c == 9 || c == 10 || c == 13 || c == 0x20;
}

// inferred: appends one character (two for a Shift-JIS lead byte) to the buffer
static inline void AppendChar_unk(char* buf, char c) {
    unsigned long n = strlen(buf);
    buf[n] = c;
    buf[n + 1] = 0;
}

extern "C" void fn_800E1374(TextParser_unk* parser);

template std::basic_ostream<char>::~basic_ostream();      // 0x800E0B18
template std::basic_stringbuf<char>::~basic_stringbuf();  // 0x800E0B90
template std::basic_streambuf<char>::~basic_streambuf();  // 0x800E0C20 (then ~locale 0x800E0C8C)

TextParser_unk::TextParser_unk(void* data, void* size)
    : mData((const char*)data), mSize((unsigned int)size), mPos(0) {
    mBuf[0] = 0;
    mType = 0;
    mClosed = 0;
    fn_800E1374(this);
}

extern "C" {

// inferred: true while there is text left
bool fn_800E0D54(TextParser_unk* parser) {
    return parser->mPos < parser->mSize;
}

// inferred: next token
void fn_800E0D70(TextParser_unk* parser) {
    parser->mClosed = 0;
    switch (parser->mType) {
    case 1:
        while (parser->mPos < parser->mSize) {
            if (parser->mData[parser->mPos] == 0x3E) {
                parser->mPos++;
                break;
            }
            if (parser->mData[parser->mPos] == 0x2F && parser->mData[parser->mPos + 1] == 0x3E) {
                parser->mClosed = 1;
                parser->mPos += 2;
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        break;
    case 4:
        while (parser->mPos < parser->mSize) {
            if (parser->mData[parser->mPos] == 0x3E) {
                parser->mPos++;
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        break;
    case 2:
    case 3:
        while (parser->mPos < parser->mSize) {
            if (parser->mData[parser->mPos] == 0x3C) {
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        break;
    }
    if (parser->mClosed != 0) {
        parser->mType = 2;
        return;
    }
    if (parser->mData[parser->mPos] == 0x3C) {
        if (parser->mData[parser->mPos + 1] == 0x2F) {
            parser->mType = 2;
            parser->mPos += 2;
            return;
        }
        if (parser->mData[parser->mPos + 1] == 0x2D && parser->mData[parser->mPos + 2] == 0x2D) {
            parser->mType = 4;
            parser->mPos += 3;
            return;
        }
        parser->mType = 1;
        parser->mPos++;
        return;
    }
    if (parser->mType == 1) {
        parser->mType = 3;
        return;
    }
    parser->mType = 0;
}

// NONMATCHING: 99.69% - buffer pointer and size use different saved registers in the inlined string assignment.
// inferred: name of the current block
std::string* fn_800E0F84(TextParser_unk* parser) {
    if (parser->mClosed != 0) {
        return &parser->mLast;
    }
    parser->mBuf[0] = 0;
    for (unsigned int i = parser->mPos; i < parser->mSize; i++) {
        if (IsSpace_unk(parser->mData[i]) || parser->mData[i] == 0x3E) {
            break;
        }
        AppendChar_unk(parser->mBuf, parser->mData[i]);
        if ((unsigned char)parser->mData[i] >= 0x80) {
            AppendChar_unk(parser->mBuf, parser->mData[++i]);
        }
    }
    char* buf = parser->mBuf;
    parser->mName = buf;
    parser->mLast = parser->mName;
    return &parser->mName;
}

// NONMATCHING: 93.10% - locals use different saved registers from the original throughout the scan.
// inferred: argument `index` of the current block (quoted strings)
std::string* fn_800E114C(TextParser_unk* parser, int index) {
    parser->mBuf[0] = 0;
    if (parser->mType != 1) {
        parser->mName.clear();
        return &parser->mName;
    }
    unsigned int i = parser->mPos;
    while (i < parser->mSize) {
        if (IsSpace_unk(parser->mData[i]) || parser->mData[i] == 0x3E) {
            break;
        }
        if ((unsigned char)parser->mData[i] >= 0x80) {
            i++;
        }
        i++;
    }
    int count = 0;
    bool quoted = false;
    for (; i < parser->mSize; i++) {
        if (parser->mData[i] == 0x3E) {
            break;
        }
        if (!quoted && IsSpace_unk(parser->mData[i])) {
            continue;
        }
        if (parser->mData[i] == 0x22) {
            quoted = !quoted;
            if (!quoted) {
                count++;
            }
        } else if (quoted && index == count) {
            AppendChar_unk(parser->mBuf, parser->mData[i]);
            if ((unsigned char)parser->mData[i] >= 0x80) {
                AppendChar_unk(parser->mBuf, parser->mData[++i]);
            }
        }
        if (index < count) {
            break;
        }
    }
    parser->mName = parser->mBuf;
    return &parser->mName;
}

// inferred: skips the spaces and the declarations ("<?...?>", "<!...!>")
void fn_800E1374(TextParser_unk* parser) {
    for (;;) {
        while (parser->mPos < parser->mSize) {
            if (!IsSpace_unk(parser->mData[parser->mPos])) {
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        if (parser->mPos >= parser->mSize) {
            return;
        }
        if (parser->mData[parser->mPos] != 0x3C) {
            return;
        }
        if (parser->mData[parser->mPos + 1] != 0x3F && parser->mData[parser->mPos + 1] != 0x21) {
            return;
        }
        parser->mPos += 3;
        while (parser->mPos < parser->mSize) {
            if ((parser->mData[parser->mPos - 1] == 0x3F || parser->mData[parser->mPos - 1] == 0x21) &&
                parser->mData[parser->mPos] == 0x3E) {
                parser->mPos++;
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
    }
}

} // extern "C"

template std::basic_ostream<char>::sentry::sentry(std::basic_ostream<char>&); // 0x800E14A4
template std::basic_ostream<char>::sentry::~sentry();                         // 0x800E1630
template std::basic_ostream<char>& std::basic_ostream<char>::flush();         // 0x800E1A48
template int std::basic_streambuf<char>::sync();
template int std::basic_streambuf<char>::overflow(int);

std::locale std::ios_base::getloc() const {
    return *mLoc;
}

template int std::basic_stringbuf<char>::underflow();
template int std::basic_stringbuf<char>::pbackfail(int);
template int std::basic_stringbuf<char>::overflow(int);
template std::streampos std::basic_stringbuf<char>::seekoff(std::streamoff, std::ios_base::seekdir, std::ios_base::openmode);
template std::streampos std::basic_stringbuf<char>::seekpos(std::streampos, std::ios_base::openmode);
template void std::basic_streambuf<char>::imbue(const std::locale&);
template std::basic_streambuf<char>* std::basic_streambuf<char>::setbuf(char*, std::streamsize);
template std::streampos std::basic_streambuf<char>::seekoff(std::streamoff, std::ios_base::seekdir, std::ios_base::openmode);
template std::streampos std::basic_streambuf<char>::seekpos(std::streampos, std::ios_base::openmode);
template std::streamsize std::basic_streambuf<char>::showmanyc();
template std::streamsize std::basic_streambuf<char>::xsgetn(char*, std::streamsize);
template int std::basic_streambuf<char>::underflow();
template int std::basic_streambuf<char>::uflow();
template int std::basic_streambuf<char>::pbackfail(int);
template std::streamsize std::basic_streambuf<char>::xsputn(const char*, std::streamsize);

template <>
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str) {
    if (!str.is_long()) {
        mRaw = str.mRaw;
    } else {
        __zero();
        reserve(str.mLong.mSize);
        do_replace(0, 0, str.mLong.mData, str.mLong.mData + str.mLong.mSize,
                      std::random_access_iterator_tag());
    }
}
