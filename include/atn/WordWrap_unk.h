#ifndef ATN_WORDWRAP_UNK_H
#define ATN_WORDWRAP_UNK_H

// inferred: word wrap of the texts (atn/WordWrap_unk.cpp). One declaration for the callers: they
// declared fn_800E0000 with the arguments in other orders (same registers on the Wii, not in every
// PC ABI). atn/D2A_Message keeps its own for now: with this one vf18 loses 0.01 of fuzzy.
#include "std/string.h"
#include "std/vector.h"

// inferred: measures one character; returns < 0 to use the font
typedef float (*MeasureChar_unk)(wchar_t c, const wchar_t** p, float size);

// inferred: wraps `text` to `maxWidth` and appends each line to `lines` (0x800E0000)
extern "C" void fn_800E0000(std::vector<std::basic_string<wchar_t> >* lines, const wchar_t* text, float maxWidth,
                            float size, float spacing, int a, int b, MeasureChar_unk measure);

#endif
