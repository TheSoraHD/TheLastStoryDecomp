#ifndef ATN_FONT_UNK_H
#define ATN_FONT_UNK_H

#include "atn/LH_Binary.h"
#include "atn/Texture_unk.h"

namespace atn {

// inferred: one glyph of a font table ("gly" file, 0xE bytes): character code and three
// signed widths in 1/32767 units (left bearing, width, right bearing?)
struct FontGlyph_unk {
    unsigned short mCode; // 0x00
    unsigned short m02;
    unsigned short m04;
    unsigned short m06;
    short m08;
    short m0A;
    short m0C;
};

// inferred: header of a "gly" file (glyphs follow at 0xA)
struct FontGlyphHeader_unk {
    unsigned short mCount; // 0x00
    unsigned short m02;
    short m04;
    short m06;
    short m08;
};

// inferred: glyph table of one font texture (0x1C bytes; constructor 0x8006CA14, destructor
// 0x8006CA20)
class FontInfo_unk {
public:
    FontInfo_unk() { mGlyphs = 0; }
    ~FontInfo_unk() {
        if (mGlyphs != 0) {
            delete[] mGlyphs;
        }
    }

    unsigned short mFirst;  // 0x00: code of the first glyph
    unsigned short mLast;   // 0x02: code of the last glyph
    float m04;
    float m08;
    float m0C;
    unsigned short mCount;  // 0x10
    float m14;
    FontGlyph_unk* mGlyphs; // 0x18
};

// inferred: result of the glyph search fn_8006D680
struct FontGlyphRef_unk {
    int mSize;  // 0x00: index of the size (third index of the tables)
    int mGlyph; // 0x04
    int mStyle; // 0x08
    int mFont;  // 0x0C
};

// inferred: 0x20 bytes, 0x100 of them at 0x1260 (used by the text drawing fn_8006D7B0)
struct FontCache_unk {
    unsigned char m00[0x20];
};

// inferred: entry of "cp932_unicode.tbl"
struct FontCodePair_unk {
    unsigned short mSjis;
    unsigned short mUnicode;
};

// inferred: font manager (0x34AA4 bytes), single instance lbl_8087EEC8 created by fn_8006C7FC.
// The real class name is unknown.
class Font_unk {
public:
    Font_unk(); // 0x8006C8F8

    Texture_unk mTextures[7][2][4];  // 0x0000: [font][style][size]
    FontInfo_unk mInfos[7][2][4];    // 0x0A80
    LH_Binary mBinaries[7][2][4];    // 0x10A0
    FontCache_unk m1260[0x100];      // 0x1260
    unsigned short m3260[0x80][0x100];   // 0x3260: SJIS -> unicode ([lead byte - 0x80][trail byte])
    unsigned short m13260[0x100][0x100]; // 0x13260: unicode -> SJIS ([high byte][low byte])
    LH_Binary mTable;                // 0x33260: "cp932_unicode.tbl"
    float m33268[3][4];              // 0x33268
    int m33298;                      // 0x33298: inferred: request to reload the fonts of font 1
    int m3329C;                      // 0x3329C: inferred: reload in progress
    int m332A0;
    unsigned char m332A4[0x1800];
};

} // namespace atn

extern "C" atn::Font_unk* lbl_8087EEC8;

#endif
