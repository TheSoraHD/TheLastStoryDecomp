#include "atn/Font_unk.h"
#include "atn/LoadManager.h"
#include "atn/Memory.h"
#include "game/GimmickCommon.h"
// string_members.h: this file has the weak copy of the wchar_t range replace (its error messages
// are in the string pool); NONMATCHING 86% like the char one (see std/string.h)
#include "std/string_members.h"

// inferred: font manager (file name unknown). Loads the font textures and their glyph tables
// ("gly" files) and the SJIS/unicode conversion table, measures and draws text.

extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" int fn_8006BA30();                                        // inferred: nonzero outside Japan
extern "C" int fn_8006AA20(const char* path);                        // inferred: the file exists
extern "C" void fn_8006AD24(std::string& path, const std::string& ext); // inferred: adds an extension
extern "C" void fn_800D58A4(atn::Texture_unk* tex);                  // inferred: releases a texture
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* path); // inferred: loads a texture
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture and waits

using namespace atn;

atn::Font_unk* lbl_8087EEC8;
const char* lbl_8087D730 = "cp932_unicode.tbl";

extern "C" int fn_8006D54C(Font_unk* self, int font, int style, int size, const void* data, int dataSize);
extern "C" int fn_8006F684(Font_unk* self, const void* data, unsigned int dataSize);

// inferred: texture name of a font
extern "C" void fn_8006C6A0(char* buf, int font, int style, int size, int town) {
    if (font == 1) {
        if (fn_8006BA30() == 0) {
            if (style == 0) {
                sprintf(buf, "font_%02d_%02d%s_jp.texture", font + 1, size + 1, town ? "_town" : "");
            } else {
                sprintf(buf, "font_%02d_%02d_s_jp.texture", font + 1, size + 1);
            }
        } else {
            if (style == 0) {
                sprintf(buf, "font_%02d_%02d%s_en.texture", font + 1, size + 1, town ? "_town" : "");
            } else {
                sprintf(buf, "font_%02d_%02d_s_en.texture", font + 1, size + 1);
            }
        }
    } else {
        if (style == 0) {
            sprintf(buf, "font_%02d_%02d%s.texture", font + 1, size + 1, town ? "_town" : "");
        } else {
            sprintf(buf, "font_%02d_%02d_s.texture", font + 1, size + 1);
        }
    }
}

// inferred: creates the font manager
extern "C" void fn_8006C7FC() {
    if (lbl_8087EEC8 == 0) {
        lbl_8087EEC8 = new (6, "", "", 0) Font_unk;
    }
}

// inferred: destroys the font manager
extern "C" void fn_8006C858() {
    if (lbl_8087EEC8 != 0) {
        delete lbl_8087EEC8;
        lbl_8087EEC8 = 0;
    }
}

namespace atn {

Font_unk::Font_unk() {
    m33298 = 0;
    m3329C = 0;
    m332A0 = 0;
    m33268[0][1] = m33268[0][2] = m33268[0][3] = m33268[1][0] = m33268[1][2] = m33268[1][3] = m33268[2][0] =
        m33268[2][1] = m33268[2][3] = 0.0f;
    m33268[0][0] = m33268[1][1] = m33268[2][2] = 1.0f;
    // asserts of the release build: without them the constructor is inlined into fn_8006C7FC
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    memset(m3260, 0, sizeof(m3260));
    memset(m13260, 0, sizeof(m13260));
}

} // namespace atn

// inferred: loads the font textures and glyph tables of `dir` and the conversion table
// NONMATCHING: 93% - stack slots of the string temporaries and saved register allocation
extern "C" int fn_8006CABC(Font_unk* self, const std::string& dir) {
    std::string path;
    for (int style = 0; style < 2; style++) {
        for (int font = 0; font < 7; font++) {
            for (int size = 0; size < 4; size++) {
                char name[64];
                fn_8006C6A0(name, font, style, size, 0);
                path = dir + "/" + name;
                if (lbl_8087F518 != 0 && lbl_8087F518->fn_8046D184() == 0) {
                    path = "data/texture/";
                    path += name;
                }
                if (fn_8006AA20(path.c_str())) {
                    fn_800D594C(&self->mTextures[font][style][size], path.c_str());
                    if (lbl_8087F518 != 0 && lbl_8087F518->fn_8046D184() == 0) {
                        path = dir + "/" + name;
                    }
                    fn_8006AD24(path, std::string("gly"));
                    LH_Binary bin;
                    bin.fn_80473FCC(path.c_str());
                    fn_8006D54C(self, font, style, size, bin.fn_80470580(), bin.fn_8047059C());
                }
            }
        }
    }
    std::string tbl = dir + "/";
    tbl += lbl_8087D730;
    LH_Binary bin;
    bin.fn_80473FCC(tbl.c_str());
    fn_8006F684(self, bin.fn_80470580(), bin.fn_8047059C());
    return 1;
}

// inferred: starts reloading the textures and glyph tables of font 1 (sizes 1-3)
// NONMATCHING: 93% - the original does not keep the address of `name` in a saved register
extern "C" void fn_8006D0C8(Font_unk* self, int town) {
    if (self->m33298 == 0) {
        return;
    }
    self->m3329C = 1;
    self->m33298 = 0;
    std::string path;
    for (int style = 0; style < 2; style++) {
        for (int size = 1; size < 4; size++) {
            char name[64];
            fn_800D58A4(&self->mTextures[1][style][size]);
            if (self->mInfos[1][style][size].mGlyphs != 0) {
                delete[] self->mInfos[1][style][size].mGlyphs;
            }
            self->mInfos[1][style][size].mGlyphs = 0;
            fn_8006C6A0(name, 1, style, size, 0);
            path = "data/texture/";
            path += name;
            if (!fn_8006AA20(path.c_str())) {
                break;
            }
            fn_800D5908(&self->mTextures[1][style][size], path.c_str());
            path = "font/";
            path += name;
            fn_8006AD24(path, std::string("gly"));
            self->mBinaries[1][style][size].vf0C(path.c_str());
        }
    }
}

// inferred: finishes the reload started by fn_8006D0C8; returns 1 while loading
extern "C" int fn_8006D3F8(Font_unk* self) {
    if (self->m3329C == 0) {
        return 0;
    }
    for (int style = 0; style < 2; style++) {
        for (int size = 1; size < 4; size++) {
            if (((GimmickUvOwner_unk*)&self->mTextures[1][style][size])->fn_800D59B8()) {
                return 1;
            }
            if (self->mBinaries[1][style][size].fn_80473F50()) {
                return 1;
            }
        }
    }
    for (int style = 0; style < 2; style++) {
        for (int size = 1; size < 4; size++) {
            if (self->mInfos[1][style][size].mGlyphs == 0 && self->mBinaries[1][style][size].fn_80470580() != 0) {
                fn_8006D54C(self, 1, style, size, self->mBinaries[1][style][size].fn_80470580(),
                            self->mBinaries[1][style][size].fn_8047059C());
                self->mBinaries[1][style][size].fn_80473F88();
            }
        }
    }
    self->m3329C = 0;
    return 0;
}

// NONMATCHING: 43% - instruction scheduling of the header conversions
// inferred: reads a glyph table
extern "C" int fn_8006D54C(Font_unk* self, int font, int style, int size, const void* data, int dataSize) {
    FontInfo_unk& info = self->mInfos[font][style][size];
    const FontGlyphHeader_unk* h = (const FontGlyphHeader_unk*)data;
    float f04 = (unsigned int)h->m02;
    float f08 = h->m04 / 32767.0f;
    float f0C = h->m06 / 32767.0f;
    float f14 = h->m08 / 32767.0f;
    info.mCount = h->mCount;
    info.m04 = f04;
    info.m08 = f08;
    info.m0C = f0C;
    info.m14 = f14;    info.mGlyphs = new (6, "", "", 0) FontGlyph_unk[info.mCount];
    memcpy(info.mGlyphs, h + 1, info.mCount * sizeof(FontGlyph_unk));
    info.mFirst = info.mGlyphs[0].mCode;
    info.mLast = info.mGlyphs[info.mCount - 1].mCode;
    return 1;
}

// inferred: finds the glyph of `code`
extern "C" int fn_8006D680(Font_unk* self, unsigned int code, float size, int font, FontGlyphRef_unk* ref) {
    int found = 0;
    int style = 0;
    if (size <= 15.001f) {
        style = 1;
    }
    ref->mFont = font;
    for (; style >= 0; style--) {
        for (unsigned int i = 0; i < 4; i++) {
            if (i >= 1) {
                ref->mFont = 1;
            }
            FontInfo_unk& info = self->mInfos[ref->mFont][style][i];
            FontGlyph_unk* glyphs = info.mGlyphs;
            if (glyphs == 0) {
                break;
            }
            if (code <= info.mLast && info.mCount != 0) {
                int lo = 0;
                int hi = info.mCount;
                while (lo <= hi) {
                    int mid = (lo + hi) >> 1;
                    unsigned int c = glyphs[mid].mCode;
                    if (code == c) {
                        ref->mSize = i;
                        found = 1;
                        ref->mGlyph = mid;
                        ref->mStyle = style;
                        break;
                    }
                    if (c < code) {
                        lo = mid + 1;
                    }
                    if (c >= code) {
                        hi = mid - 1;
                    }
                }
                if (found) {
                    break;
                }
            }
        }
        if (found) {
            break;
        }
    }
    return found;
}

// fn_8006D7B0 (0x1798 bytes): text drawing with GX, not decompiled yet

// inferred: width of a wide string
// NONMATCHING: 90% - float register allocation and loop shape
extern "C" float fn_8006EF48(Font_unk* self, const wchar_t* text, float size, float spacing, int proportional, int font) {
    float w = 0.0f;
    unsigned int len = wcslen(text);
    if (proportional) {
        for (unsigned int i = 0; i < len; i++) {
            unsigned int c = *text;
            FontGlyphRef_unk ref;
            if (fn_8006D680(self, c, size, font, &ref)) {
                FontGlyph_unk& g = self->mInfos[ref.mFont][ref.mStyle][ref.mSize].mGlyphs[ref.mGlyph];
                w += size * (2.0f * (g.m08 / 32767.0f));
                w += size * (2.0f * (g.m0A / 32767.0f));
                w += size * (2.0f * (g.m0C / 32767.0f));
            } else if (c <= 0xFF) {
                w += 0.5f * size;
            } else {
                w += size;
            }
            w -= 2.0f * (size * 0.0f);
            text++;
            w += spacing;
        }
    } else {
        for (unsigned int i = 0; i < len; i++) {
            if (*text <= 0xFF) {
                w += 0.5f * size;
            } else {
                w += size;
            }
            w += spacing;
            text++;
        }
    }
    return w;
}

// inferred: width of one character
// NONMATCHING: 99.7% - f3/f6 swapped (int->float constant)
extern "C" float fn_8006F17C(Font_unk* self, unsigned int c, int proportional, int font, float size) {
    float w = 0.0f;
    if (proportional) {
        FontGlyphRef_unk ref;
        if (fn_8006D680(self, c, size, font, &ref)) {
            FontGlyph_unk& g = self->mInfos[ref.mFont][ref.mStyle][ref.mSize].mGlyphs[ref.mGlyph];
            float k = 0.0f;
            w += size * (2.0f * (g.m08 / 32767.0f));
            w += size * (2.0f * (g.m0A / 32767.0f));
            w += size * (2.0f * (g.m0C / 32767.0f));
            w -= 2.0f * (size * k);
        } else if (c <= 0xFF) {
            w += 0.5f * size;
        } else {
            w += size;
        }
    } else if (c <= 0xFF) {
        w = 0.5f * size;
    } else {
        w = size;
    }
    return w;
}

struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8; // inferred: debug message output

// inferred: SJIS to wide string; returns the number of characters written with the terminator
// NONMATCHING: 91% - loop variables (pointer and index) allocation
extern "C" int fn_8006F2F0(Font_unk* self, wchar_t* dst, const char* src, unsigned int size) {
    if (dst == 0 || src == 0) {
        return 0;
    }
    unsigned int n = 0;
    unsigned int len = strlen(src);
    const unsigned char* p = (const unsigned char*)src;
    for (unsigned int i = 0; i < len; i++, p++) {
        if (*p >= 0x80) {
            unsigned short c = (unsigned char)src[i + 1] | ((unsigned char)src[i] << 8);
            i++;
            p++;
            dst[n++] = self->m3260[(c >> 8) - 0x80][c & 0xFF];
        } else {
            dst[n++] = src[i];
        }
        if (n >= size - 1) {
            if (i < len - 1) {
                char buf[256];
                sprintf(buf, "[ConvertUnicode] dest wstring over %s\n", src);
                lbl_8087EEB8->fn_800697D8(buf);
            }
            break;
        }
    }
    dst[n] = 0;
    return n + 1;
}

// inferred: wide string to SJIS; returns the number of bytes written with the terminator
// NONMATCHING: 96% - loop variables allocation
extern "C" int fn_8006F420(Font_unk* self, char* dst, const wchar_t* src, unsigned int size) {
    if (src == 0 || dst == 0) {
        return 0;
    }
    unsigned int n = 0;
    unsigned int len = wcslen(src);
    char* q = dst;
    for (unsigned int i = 0; i < len; i++) {
        unsigned int c = *src;
        if (c <= 0x7F) {
            *q++ = c;
            n++;
        } else {
            if (n >= size - 2) {
                break;
            }
            n += 2;
            q[0] = self->m13260[c >> 8][c & 0xFF] >> 8;
            q[1] = self->m13260[c >> 8][c & 0xFF];
            q += 2;
        }
        if (n >= size - 1) {
            if (i < len - 1) {
                char buf[256];
                dst[n] = 0;
                sprintf(buf, "[ConvertSjis] dest string over %s\n", dst);
                lbl_8087EEB8->fn_800697D8(buf);
            }
            break;
        }
        src++;
    }
    dst[n] = 0;
    return n + 1;
}

// inferred: SJIS to std::wstring
extern "C" int fn_8006F548(Font_unk* self, std::basic_string<wchar_t>& dst, const char* src) {
    wchar_t buf[256];
    int n = fn_8006F2F0(self, buf, src, 256);
    dst = buf;
    return n;
}

// inferred: wide string to std::string
// NONMATCHING: 85% - the original computes buf + strlen(buf) through a copy of buf (mr r7,r6)
extern "C" int fn_8006F5E8(Font_unk* self, std::string& dst, const wchar_t* src) {
    char buf[256];
    int n = fn_8006F420(self, buf, src, 256);
    dst = buf;
    return n;
}

// inferred: reads "cp932_unicode.tbl" (pairs of SJIS and unicode codes, ended by 0xFFFF)
// NONMATCHING: 67% - the original computes both table addresses before the test and keeps dead
// code after the first store (some if/else shape not found)
extern "C" int fn_8006F684(Font_unk* self, const void* data, unsigned int dataSize) {
    if (data == 0) {
        return 0;
    }
    const unsigned short* p = (const unsigned short*)data;
    for (unsigned int i = 0; i < dataSize; i += 4, p += 2) {
        unsigned short sjis = *p;
        if (sjis == 0xFFFF) {
            break;
        }
        unsigned short uni = *(const unsigned short*)((const char*)data + i + 2);
        int hi = (sjis >> 8) & 0xFF;
        int lo = sjis & 0xFF;
        int uhi = (uni >> 8) & 0xFF;
        int ulo = uni & 0xFF;
        self->m13260[uhi][ulo] = sjis;
        if (hi >= 0x80) {
            self->m3260[(unsigned short)(hi - 0x80)][lo] = uni;
        }
    }
    return 1;
}
