// atn/FileUtil_unk.cpp: compresion de texturas a CMPR (DXT1 de GX), comprobacion de orden de bytes
// y lista de mensajes de depuracion en pantalla ("LoadError: ..."). Nombre de fichero inventado.
#include "atn/Memory.h"
#include "std/string_members.h"
#include "atn/DrawHelper.h"

extern "C" unsigned long strlen(const char* s);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);

// ---------------------------------------------------------------------------------------------
// CMPR (S3TC/DXT1) encoder. Pixels are RGBA8, 4x4 per block; a block is two RGB565 colors and
// sixteen 2-bit indices.

// NONMATCHING: 92.94% - register allocation and scheduling of the palette and index packing
// inferred: encodes a block with four colors (c0 > c1); rgb0/rgb1 are the 8-bit endpoints
extern "C" void fn_80068DB0(unsigned short c0, unsigned short c1, const unsigned char* rgb0,
                            const unsigned char* rgb1, const unsigned char* pixels, unsigned char* out) {
    unsigned char idx[16];
    unsigned char pal[12];
    pal[0] = rgb1[0];
    pal[1] = rgb1[1];
    pal[2] = rgb1[2];
    pal[3] = rgb0[0];
    pal[4] = rgb0[1];
    pal[5] = rgb0[2];
    pal[6] = (rgb0[0] * 3 + rgb1[0] * 5) / 8;
    pal[7] = (rgb0[1] * 3 + rgb1[1] * 5) / 8;
    pal[8] = (rgb0[2] * 3 + rgb1[2] * 5) / 8;
    pal[9] = (rgb0[0] * 5 + rgb1[0] * 3) / 8;
    pal[10] = (rgb0[1] * 5 + rgb1[1] * 3) / 8;
    pal[11] = (rgb0[2] * 5 + rgb1[2] * 3) / 8;
    unsigned char* dst = idx;
    for (int i = 0; i < 16; i++) {
        int r = pixels[0];
        int g = pixels[1];
        int b = pixels[2];
        int best = 0;
        int dr = r - pal[0];
        int dg = g - pal[1];
        int db = b - pal[2];
        int bestDist = db * db + dr * dr + dg * dg;
        for (int j = 1; j < 4; j++) {
            dr = r - pal[j * 3 + 0];
            dg = g - pal[j * 3 + 1];
            db = b - pal[j * 3 + 2];
            int dist = db * db + dr * dr + dg * dg;
            if (dist < bestDist) {
                best = j;
                bestDist = dist;
            }
        }
        *dst = best;
        pixels += 4;
        dst++;
    }
    out[0] = c1 >> 8;
    out[1] = c1;
    out[2] = c0 >> 8;
    out[3] = c0;
    out[4] = idx[0] | (idx[1] << 2) | (idx[2] << 4) | (idx[3] << 6);
    out[5] = idx[4] | (idx[5] << 2) | (idx[6] << 4) | (idx[7] << 6);
    out[6] = idx[8] | (idx[9] << 2) | (idx[10] << 4) | (idx[11] << 6);
    out[7] = idx[12] | (idx[13] << 2) | (idx[14] << 4) | (idx[15] << 6);
}

// NONMATCHING: 78.80% - as fn_80068DB0
// inferred: encodes a block with three colors and transparency (c0 <= c1)
extern "C" void fn_80069060(unsigned short c0, unsigned short c1, const unsigned char* rgb0,
                            const unsigned char* rgb1, const unsigned char* pixels, unsigned char* out) {
    unsigned char idx[16];
    unsigned char pal[12];
    pal[9] = 0;
    pal[10] = 0;
    pal[11] = 0;
    pal[0] = rgb0[0];
    pal[1] = rgb0[1];
    pal[2] = rgb0[2];
    pal[3] = rgb1[0];
    pal[4] = rgb1[1];
    pal[5] = rgb1[2];
    pal[6] = (rgb0[0] >> 1) + (rgb1[0] >> 1);
    pal[7] = (rgb0[1] >> 1) + (rgb1[1] >> 1);
    pal[8] = (rgb0[2] >> 1) + (rgb1[2] >> 1);
    unsigned char* dst = idx;
    for (int i = 0; i < 16; i++) {
        if (pixels[3] < 0x7F) {
            *dst = 3;
        } else {
            int r = pixels[0];
            int g = pixels[1];
            int b = pixels[2];
            int best = 0;
            int dr = r - pal[0];
            int dg = g - pal[1];
            int db = b - pal[2];
            int bestDist = db * db + dr * dr + dg * dg;
            for (int j = 1; j < 3; j++) {
                dr = r - pal[j * 3 + 0];
                dg = g - pal[j * 3 + 1];
                db = b - pal[j * 3 + 2];
                int dist = db * db + dr * dr + dg * dg;
                if (dist < bestDist) {
                    best = j;
                    bestDist = dist;
                }
            }
            *dst = best;
        }
        pixels += 4;
        dst++;
    }
    out[0] = c0 >> 8;
    out[1] = c0;
    out[2] = c1 >> 8;
    out[3] = c1;
    out[4] = idx[0] | (idx[1] << 2) | (idx[2] << 4) | (idx[3] << 6);
    out[5] = idx[4] | (idx[5] << 2) | (idx[6] << 4) | (idx[7] << 6);
    out[6] = idx[8] | (idx[9] << 2) | (idx[10] << 4) | (idx[11] << 6);
    out[7] = idx[12] | (idx[13] << 2) | (idx[14] << 4) | (idx[15] << 6);
}

// NONMATCHING: 77.87% - min/max loop kept in memory in the original (byte arrays reloaded each step)
// inferred: encodes one 4x4 block (the endpoints are the min and max of each channel)
extern "C" void fn_80069288(const unsigned char* pixels, unsigned char* out) {
    unsigned char lo[3];
    unsigned char hi[3];
    lo[0] = pixels[0];
    lo[1] = pixels[1];
    lo[2] = pixels[2];
    hi[0] = pixels[0];
    hi[1] = pixels[1];
    hi[2] = pixels[2];
    int alpha = 0;
    const unsigned char* p = pixels + 4;
    for (int i = 1; i < 16; i++) {
        lo[0] = p[0] < lo[0] ? p[0] : lo[0];
        lo[1] = p[1] < lo[1] ? p[1] : lo[1];
        lo[2] = p[2] < lo[2] ? p[2] : lo[2];
        hi[0] = p[0] > hi[0] ? p[0] : hi[0];
        hi[1] = p[1] > hi[1] ? p[1] : hi[1];
        hi[2] = p[2] > hi[2] ? p[2] : hi[2];
        alpha = alpha != 0 || p[3] < 0x7F;
        p += 4;
    }
    unsigned short c0 = ((lo[0] << 8) & 0xF800) | ((lo[1] << 3) & 0x7E0) | (lo[2] >> 3);
    unsigned short c1 = ((hi[0] << 8) & 0xF800) | ((hi[1] << 3) & 0x7E0) | (hi[2] >> 3);
    if (c0 > c1) {
        unsigned short t = c0;
        c0 = c1;
        c1 = t;
        unsigned char r = lo[0];
        unsigned char g = lo[1];
        unsigned char b = lo[2];
        lo[0] = hi[0];
        lo[1] = hi[1];
        lo[2] = hi[2];
        hi[0] = r;
        hi[1] = g;
        hi[2] = b;
    } else if (c0 == c1) {
        alpha = 1;
    }
    if (alpha != 0) {
        fn_80069060(c0, c1, lo, hi, pixels, out);
    } else {
        fn_80068DB0(c0, c1, lo, hi, pixels, out);
    }
}

// NONMATCHING: 73.65% - the original copies two rows per iteration of an unrolled loop
// inferred: encodes a RGBA8 image (width x height) to CMPR: 2x2 blocks per 8x8 tile
extern "C" void fn_8006945C(const unsigned char* src, int width, int height, unsigned char* dst) {
    int tilesX = width / 4;
    int tilesY = height / 4;
    int pitch = width * 4;
    for (int ty = 0; ty < tilesY; ty++) {
        int offset = pitch * (ty * 4);
        for (int tx = 0; tx < tilesX; tx++) {
            unsigned char block[64];
            int row = offset;
            unsigned char* b = block;
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    const unsigned char* s = src + row + x * 4;
                    b[0] = 0;
                    b[1] = 0;
                    b[2] = 0;
                    b[3] = s[3];
                    b[0] = s[0];
                    b[1] = s[1];
                    b[2] = s[2];
                    b[3] = s[3];
                    b += 4;
                }
                row += pitch;
            }
            fn_80069288(block, dst);
            dst += 8;
            offset += 16;
        }
    }
}

// ---------------------------------------------------------------------------------------------
// inferred: byte order test value (0x0100: the first byte is 1 on big endian)
static const unsigned short sEndian = 0x100;

// NONMATCHING: 99% - only the size of the .sdata2 object (2 here, 8 in the symbol table)
// inferred: read through a volatile pointer so that the compiler does not fold it (the original
// loads it with lhz)
extern "C" bool fn_8006969C() {
    return *(volatile const unsigned short*)&sEndian != 1;
}

extern "C" bool fn_800696B4() {
    return *(volatile const unsigned short*)&sEndian == 1;
}

// ---------------------------------------------------------------------------------------------
namespace gmk {
// inferred: fixed string of 0x80 chars (see include/gmk/RandomMerchant.h; the weak destructor
// 0x80069758 of this file is the one the linker kept)
struct MerchantString_unk {
    MerchantString_unk() { mStr[0] = 0; }
    ~MerchantString_unk() {}
    MerchantString_unk& operator=(const char* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    // inferred: assignment that truncates to 126 characters
    void Set_unk(const char* str) {
        if (strlen(str) >= 0x7F) {
            strncpy(mStr, str, 0x7E);
            mStr[0x7E] = 0;
        } else if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
    }
    char mStr[0x80];
};
} // namespace gmk

// inferred: one line of the message list (0x88 bytes)
struct DebugMessage_unk {
    DebugMessage_unk();
    ~DebugMessage_unk();
    gmk::MerchantString_unk mText;
    int mTimer;          // 0x80: frames left
    unsigned int mColor; // 0x84
};

// inferred: on-screen debug messages (singleton lbl_8087EEB8)
struct DebugMessages_unk {
    DebugMessages_unk() : mCount(0) {}
    void Add_unk(const char* text, unsigned int color) {
        if (mCount < 64) {
            DebugMessage_unk msg;
            msg.mTimer = 300;
            msg.mColor = color;
            msg.mText.Set_unk(text);
            new (&mMessages[mCount]) DebugMessage_unk(msg);
            mCount++;
        }
    }
    void fn_800697D8(const char* text); // inferred: red message
    void fn_800698DC(const char* path); // inferred: "LoadError: " + path (after "dvdroot")
    void fn_80069D48(const char* text); // inferred: yellow message
    void fn_80069E50(int draw);         // inferred: draws and ages the messages

    unsigned int mCount;
    DebugMessage_unk mMessages[64];
};

extern "C" {
DebugMessages_unk* lbl_8087EEB8;
}

extern "C" void fn_800696C8() {
    if (lbl_8087EEB8 == 0) {
        lbl_8087EEB8 = new ((MemoryTag)1, "", "", 0) DebugMessages_unk;
    }
}

DebugMessage_unk::DebugMessage_unk() {}

DebugMessage_unk::~DebugMessage_unk() {}

void DebugMessages_unk::fn_800697D8(const char* text) {
    Add_unk(text, 0xFFFF0000);
}

// NONMATCHING: 99.71% - registers of the second inlined string constructor
void DebugMessages_unk::fn_800698DC(const char* path) {
    std::string msg("LoadError: ");
    std::string str(path);
    std::string::size_type pos = str.find("dvdroot");
    if (pos != std::string::npos) {
        str = std::string(str, pos + 7);
    }
    msg += str;
    Add_unk(msg.c_str(), 0xFFFF0000);
}

void DebugMessages_unk::fn_80069D48(const char* text) {
    Add_unk(text, 0xFF00FFFF);
}

// NONMATCHING: ~90% - register allocation of the loops
void DebugMessages_unk::fn_80069E50(int draw) {
    if (draw != 0) {
        for (unsigned int i = 0; i < mCount; i++) {
            lbl_8087EEB0->fn_800616C0(10.0f, 12.0f * i + 20.0f, -1000.0f, 12.0f, 12.0f, mMessages[i].mText.mStr, mMessages[i].mColor, 0.0f, 1, 1, 0);
        }
    }
    DebugMessage_unk* msg = mMessages;
    while (msg != &mMessages[mCount]) {
        if (--msg->mTimer <= 0) {
            for (unsigned int i = msg - mMessages; i < mCount - 1; i++) {
                mMessages[i].mText = mMessages[i + 1].mText.mStr;
                mMessages[i].mTimer = mMessages[i + 1].mTimer;
                mMessages[i].mColor = mMessages[i + 1].mColor;
            }
            mCount = mCount - 1;
        } else {
            msg++;
        }
    }
}
