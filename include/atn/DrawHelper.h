#ifndef ATN_DRAWHELPER_H
#define ATN_DRAWHELPER_H

#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"
#include "std/new.h"

struct GXTexObj;

namespace std {
template <class T> struct char_traits;
template <class T> class allocator;
template <class T, class Traits, class Alloc> class basic_string;
} // namespace std

namespace atn {

class ColliObject;
class ColliTree;

// inferred: array released with delete[] {data, size, ?} (names invented; the destructor is
// inlined into DrawHelper::~DrawHelper)
template <class T>
class DrawArray_unk {
public:
    DrawArray_unk() : mData(0), mSize(0), m08(0) {}
    ~DrawArray_unk() {
        if (mData != 0) {
            delete[] mData;
            mData = 0;
            mSize = 0;
        }
    }

    // inferred: a draw command placed in the buffer (inlined in every command of atn/ComDrawTexture);
    // names invented
    template <class U>
    U* New_unk() {
        U* p;
        if (m08 + (int)sizeof(U) > mSize) {
            p = 0;
        } else {
            p = new (mData + m08) U;
            m08 += sizeof(U);
        }
        return p;
    }
    // inferred: array of vertices placed in the buffer: 0x10 bytes of array header, then rounded up
    // with 4 - size % 4 (adds 4 even when aligned)
    template <class U>
    U* NewArray_unk(int n) {
        int size = n * sizeof(U);
        size += 0x10;
        size += 4 - size % 4;
        U* p;
        if (m08 + size > mSize) {
            p = 0;
        } else {
            p = new (mData + m08) U[n];
            m08 += size;
        }
        return p;
    }

    T* mData;
    int mSize; // inferred: capacity
    int m08;   // inferred: bytes used
};

// inferred: debug drawing helper (0x504 bytes, singleton lbl_8087EEB0)
class DrawHelper {
public:
    DrawHelper();
    virtual ~DrawHelper();

    static void fn_8005DD60(); // inferred: creates the singleton

    // Commands of atn/ComDrawTexture (vtables and RTTI there).
    class DrawCommand;
    class ComDrawLine;
    class ComDrawTexture;
    class ComDrawTextureAdd;
    class ComDrawTextureMask;
    class ComDrawTextureMono;
    class ComDrawTextureMaskMono;
    class ComDrawFont;
    class ComDrawFontSideFade;
    class ComDrawColliObject;
    class ComDrawColliTree;
    class ComDrawD3TextureMask;
    class ComDrawWindow;

    // Used only inside atn/ComDrawTexture.
    void fn_8005E000(unsigned int color, GXTexObj* tex, int mode, int arg, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1);
    void fn_8005EDF0(unsigned int color, void* tex, int arg, int flags, float x, float y, float z, float w, float h, float rot, float u0, float v0, float u1, float v1);
    void fn_8005EE38(unsigned int color, void* texture0, void* texture1, int mode, float x, float y, float z, float w, float h, float rot, float u0, float v0, float u1, float v1, float s0, float s1, float s2, float s3);
    void fn_8005EEE0(unsigned int color, void* tex, int arg, int flags, float x, float y, float z, float w, float h, float cx, float cy, float rot, float u0, float v0, float u1, float v1);
    void fn_80062F7C(unsigned int segments, unsigned int color, float x, float y, float z, float a, float radius, float b, float rot);
    void fn_8005E40C(unsigned int color, void* tex, int arg, int flags, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1);

    // Drawing commands (atn/ComDrawTexture). Names and parameter names unknown (fn_<addr>);
    // inferred: parameter types and order fixed from the asm (r4.. / f1.. usage) and the
    // calls in the game; int and float arguments use separate registers, so only the order
    // inside each class matters for the ABI. Vectors by const reference: several callers pass
    // temporaries (game/Em310Gargoyle, game/TeamAIManager).
    void fn_8005DED8(int bufSize, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& fontName);
    void fn_8005DF90(unsigned int color, GXTexObj* obj, int arg, float x, float y, float z, float w, float h);
    void fn_8005DFC8(unsigned int color, Texture_unk* tex, int arg, float x, float y, float z, float w, float h);
    void fn_8005E3E4(unsigned int color, void* tex, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1, int arg, int flags);
    void fn_8005E7F4(unsigned int color, void* texture0, void* texture1, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1);
    void fn_8005E838(unsigned int color, void* texture0, void* texture1, int mode, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1, float s0, float s1, float s2, float s3);
    void fn_8005ED6C(unsigned int color, Texture_unk* tex, float z);
    void fn_8005EEA0(unsigned int color, void* texture, int blend, unsigned int flags, float x, float y, float z, float w, float h, float scaleX, float scaleY, float rotation, float u0, float v0, float u1, float v1);
    void fn_8005F36C(unsigned int color, void* texture0, void* texture1, int blend, float x, float y, float z, float w, float h, float scaleX, float scaleY, float rotation, float u0, float v0, float u1, float v1, float s0, float s1, float s2, float s3);
    void fn_8005F90C(unsigned int color, void* texture, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1);
    void fn_8005FD80(unsigned int color, void* texture0, void* texture1, int blend, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1, float s0, float s1, float s2, float s3);
    void fn_800602E8(unsigned int color, void* texture, int blend, unsigned int flags, float x, float y, float z, float w, float h, float scaleX, float scaleY, float rotation, float u0, float v0, float u1, float v1);
    void fn_800607C0(unsigned int color, void* texture0, void* texture1, int blend, float x, float y, float z, float w, float h, float scaleX, float scaleY, float rotation, float u0, float v0, float u1, float v1, float s0, float s1, float s2, float s3);
    void fn_80060D58(float x, float y, float z, float w, float h, unsigned int color);
    void fn_800610A4(unsigned int color, float x, float y, float z, float w, float h);
    void fn_8006144C(float x0, float y0, float x1, float y1, unsigned int color, float z);
    void fn_800616C0(float x, float y, float z, float w, float h, const char* text, unsigned int color, float f6, int a, int b, int c);
    void fn_80061824(float x, float y, float z, float w, float h, float f6, float f7, float f8, const wchar_t* text, unsigned int color, int a, int b, int c, int d, unsigned int color2);
    void fn_80061AE4(const wchar_t* text, unsigned int color, int a, int b, int c, int d, unsigned int color2, float x, float y, float z, float w, float h, float f6, float f7, float f8, float clipX, float clipW, float clipScale);
    void fn_80061E2C(float x, float y, float z, float w, float h, unsigned int color, void* tex0, void* tex1, int arg, float offsetX, float offsetY, float scaleX, float scaleY);
    void fn_80062908(ColliObject* colli, int a, int b, int c, int d, float arg);
    void fn_8006297C(ColliTree* colli, int a, int b, int c, int d, float arg);
    void fn_800629F0(const Vector3& center, const Vector3& size, float arg, unsigned int color, const Matrix3x4* mtx);
    void fn_80062C8C(unsigned int segments, unsigned int color, float x, float y, float z, float a, float radius, float b);
    void fn_80063200(unsigned int segments, unsigned int color, float x, float y, float z, float a, float radius);
    void fn_80063484(unsigned int segments, unsigned int color, float x, float y, float z, float rot, float a, float radius, float angle);
    void fn_80063764(const Vector3& a, const Vector3& b, float width, unsigned int color);
    void fn_800638B0(float a, float b, const Vector3& from, const Vector3& to, unsigned int color);
    void fn_80063D3C(const Vector3& pos, float radius, float arg, unsigned int color);
    void fn_800641CC(const Vector3& pos, const Vector3& dir, unsigned int color, float length, float angle, float arg);
    void fn_8006471C(const Vector3& a, const Vector3& b, unsigned int color, float radius, float arg);

    DrawArray_unk<unsigned char> m04; // inferred: element type unknown
    Texture_unk m10; // inferred: white texture (atn/D2A_Texture, atn/ModelDraw_unk)
    Texture_unk m40;
    Texture_unk m70;
    int mA0;         // inferred: blend mode (0x12 normal, 0x13 while fading, 0xD in atn/D3Anime)
    Matrix3x4 mA4;   // inferred: model matrix
    Matrix3x4 mD4;   // inferred: view matrix
    wchar_t m104[0x200]; // inferred: text converted by fn_800616C0 (longer texts go to the heap)
};

} // namespace atn

extern "C" atn::DrawHelper* lbl_8087EEB0; // inferred: DrawHelper singleton

#endif
