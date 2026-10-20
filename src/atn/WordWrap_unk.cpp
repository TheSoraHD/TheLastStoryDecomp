// atn/WordWrap_unk.cpp: word wrap of the game texts (file name invented). Line breaks follow the
// Japanese kinsoku rules (table of characters that cannot start or end a line).
#include "std/string.h"
#include "std/vector.h"

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
