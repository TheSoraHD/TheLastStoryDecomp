#include "atn/DrawHelper.h"
#include "atn/Font_unk.h"
#include "atn/Camera_unk.h"
#include "atn/ColliBase.h"
#include "atn/GXDecl.h"
#include "atn/IRender.h"
#include "atn/LoadHandle.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "std/string.h"
#include <boost/scoped_array.hpp>

// atn/DrawHelper: debug and 2D drawing helper (singleton lbl_8087EEB0). Every command is placed in the command buffer of the helper (DrawHelper::m04), with its vertices behind it, and
// queued in the render system (lbl_8087EFB4) with the current blend mode (DrawHelper::mA0).

struct MemoryMode_unk {
    MemoryMode_unk(int mode, unsigned long align);
    ~MemoryMode_unk();
};

// inferred: the same function is named as a member of GimmickUvOwner_unk elsewhere (texture class)
struct GimmickUvOwner_unk {
    int fn_800D59B8(); // inferred: nonzero while loading
};

// inferred: render manager (lbl_8087EEE0): screen size
struct RenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};

extern "C" int lbl_8087D72C;
extern "C" int lbl_8087D728;
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" void* lbl_8087EFB4;
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" void fn_800BDB58(void* renderSystem, void* render, float order, int pass); // inferred: queues a render
extern "C" void fn_8006C7FC();
extern "C" int fn_8006CABC(void* font, const std::string& dir);
extern "C" void fn_800D594C(atn::Texture_unk* texture, const char* path); // inferred: loads a texture and waits
extern "C" void fn_8006C858(atn::DrawHelper* self); // inferred: releases the queued commands
extern "C" unsigned long strlen(const char* s);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" void fn_80076760(void* device, int arg1, int arg2); // inferred: sets the render state
extern "C" void GXLoadPosMtxImm(const Mtx m, unsigned int id);
extern "C" void GXSetCurrentMtx(unsigned int id);
extern "C" void GXClearVtxDesc();
extern "C" void GXSetVtxDesc(int attr, int type);
extern "C" void GXSetVtxAttrFmt(int fmt, int attr, int cnt, int type, unsigned char frac);
extern "C" void GXSetChanAmbColor(int chan, GXColor color);
extern "C" void GXSetChanMatColor(int chan, GXColor color);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int fn_800760E8(int n, int arg); // inferred: vertex format of the 3D texture commands
extern "C" void fn_8006F2F0(void* font, wchar_t* dst, const char* src, unsigned long len); // inferred: SJIS to wide

// inferred: write gather pipe of the GX FIFO (as in atn/atn_colli.cpp)
union DrawWGPipe_unk {
    float f;
    unsigned int u;
};
#define GXWGFifo (*(volatile DrawWGPipe_unk*)0xCC008000)

static inline void GXPosition2f32(float x, float y) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
}
static inline void GXPosition3f32(float x, float y, float z) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
    GXWGFifo.f = z;
}
static inline void GXColor1u32(unsigned int c) {
    GXWGFifo.u = c;
}
static inline void GXTexCoord2f32(float s, float t) {
    GXWGFifo.f = s;
    GXWGFifo.f = t;
}

extern "C" void GXSetAlphaCompare(int comp0, unsigned char ref0, int op, int comp1, unsigned char ref1);
extern "C" void GXBegin(int type, int fmt, unsigned short count);
extern "C" void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
extern "C" void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
extern "C" void fn_800761A8(void* device); // inferred: flushes the render state
extern "C" void fn_800763FC(void* device, GXTexObj* tex, int map); // inferred: loads a texture
extern "C" void fn_80072914(void* device, int add); // inferred: additive color state of the render device
// inferred: advance of a character (atn/Font_unk.cpp)
extern "C" float fn_8006F17C(atn::Font_unk* font, wchar_t c, int bold, float size, int style);
// inferred: draws a text with the font manager (atn/Font_unk.cpp, not matched there yet: the second
// argument is the view in this call)
extern "C" void fn_8006D7B0(atn::Font_unk* font, atn::RenderView_unk* view, float x, float y, float w, float h,
                            const wchar_t* text, unsigned int color, unsigned int flags, int style, float spacing,
                            unsigned int color2, float ox, float oy);

atn::DrawHelper* lbl_8087EEB0;

namespace atn {

inline DrawHelper::DrawHelper() : mA0(0x12) {
    mA4.SetIdentity();
}

void DrawHelper::fn_8005DD60() {
    if (lbl_8087EEB0 == 0) {
        lbl_8087EEB0 = new ((MemoryTag)6, "", "", 0) DrawHelper;
    }
}

DrawHelper::~DrawHelper() {
    fn_8006C858(this);
}


// inferred: vertex of the line commands (0x10)
struct DrawVertex_unk {
    DrawVertex_unk() {}
    // inferred: name invented
    void Set_unk(const Vector3& pos, unsigned int color) {
        mPos = pos;
        mColor = color;
    }
    Vector3 mPos;
    unsigned int mColor;
};

// inferred: vertex of the texture commands (0x18)
struct DrawTexVertex_unk {
    DrawTexVertex_unk() {}
    Vector3 mPos;
    unsigned int mColor;
    Vector2_unk mUV;
};

// inferred: texture coordinates of the two textures of a masked command (copied with paired singles)
struct DrawUV2_unk {
    DrawUV2_unk() {}
    DrawUV2_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    DrawUV2_unk& operator=(const DrawUV2_unk& o) {
        __Vec4Copy(&u0, &o.u0);
        return *this;
    }
    float u0;
    float v0;
    float u1;
    float v1;
};

// inferred: vertex of the masked texture commands (0x20)
struct DrawMaskVertex_unk {
    DrawMaskVertex_unk() {}
    // inferred: name invented
    void Set_unk(const Vector3& pos, unsigned int color, const DrawUV2_unk& uv) {
        mPos = pos;
        mColor = color;
        mUV = uv;
    }
    Vector3 mPos;
    unsigned int mColor;
    DrawUV2_unk mUV;
};

class DrawHelper::DrawCommand : public IRender {
public:
    virtual ~DrawCommand() {}
};

class DrawHelper::ComDrawLine : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawLine() {}
    virtual void vf10(RenderView_unk* view);

    int m04;                 // inferred: 0 for the blend modes 6, 8 and 0xD
    DrawVertex_unk* mVertex; // 0x08
    int mPrim;               // 0x0C: GX primitive
    int m10;
    int mCount; // 0x14
    unsigned char m18;
    int m1C;
};

class DrawHelper::ComDrawTexture : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawTexture() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    DrawTexVertex_unk* mVertex; // 0x08
    GXTexObj* mTex;             // 0x0C
    unsigned char m10;
    unsigned char m11;
    int m14;
};

class DrawHelper::ComDrawTextureAdd : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawTextureAdd() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    DrawTexVertex_unk* mVertex; // 0x08
    void* mTex;                 // 0x0C
    unsigned char m10;
    unsigned char m11;
    int m14;
    int m18;
};

class DrawHelper::ComDrawTextureMask : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawTextureMask() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    DrawMaskVertex_unk* mVertex; // 0x08
    Texture_unk* mTex0;          // 0x0C
    Texture_unk* mTex1;          // 0x10
    unsigned char m14;
    int m18;
    int m1C;
};

class DrawHelper::ComDrawFont : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawFont() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    int m08;
    unsigned int m0C; // inferred: flags (1, 4)
    wchar_t* mText;   // 0x10
    float m14;
    float m18;
    float m1C;
    float m20;
    unsigned int mColor; // 0x24
    float m28;
    unsigned char m2C;
    unsigned char m2D;
    Matrix3x4 mMtx; // 0x30
    int m60;
    Vector2_unk m64;
    unsigned int m6C;
    float m70;
};

class DrawHelper::ComDrawFontSideFade : public DrawHelper::ComDrawFont {
public:
    virtual ~ComDrawFontSideFade() {}
    virtual void vf10(RenderView_unk* view);

    float m74;
    float m78;
    int m7C;
};

class DrawHelper::ComDrawColliObject : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawColliObject() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    int m08;
    int m0C;
    int m10;
    ColliObject* mColli; // 0x14
    int m18;
};

class DrawHelper::ComDrawColliTree : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawColliTree() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    int m08;
    int m0C;
    int m10;
    ColliTree* mColli; // 0x14
    int m18;
};

class DrawHelper::ComDrawWindow : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawWindow() {}
    virtual void vf10(RenderView_unk* view);

    int m04;                         // inferred: 0 for the blend modes 6, 8 and 0xD
    DrawMaskVertex_unk* mCorner[4];  // 0x08: the 4 corners (4 vertices each)
    DrawMaskVertex_unk* mEdge[4];    // 0x18: the 4 edges between them
    Texture_unk* mTex0;              // 0x28
    Texture_unk* mTex1;              // 0x2C
    unsigned char m30;
    int m34;
    int m38;
};

class DrawHelper::ComDrawD3TextureMask : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawD3TextureMask() {}
    virtual void vf10(RenderView_unk* view);

    int m04;
    DrawTexVertex_unk* mVertex; // 0x08
    int m0C;
    int m10;
    int mCount; // 0x14
    int m18;
    int m1C;
    int m20;
    int m24;
};

class DrawHelper::ComDrawTextureMono : public DrawHelper::ComDrawTexture {
public:
    virtual ~ComDrawTextureMono() {}
    virtual void vf10(RenderView_unk* view);
};

class DrawHelper::ComDrawTextureMaskMono : public DrawHelper::ComDrawTextureMask {
public:
    virtual ~ComDrawTextureMaskMono() {}
    virtual void vf10(RenderView_unk* view);
};

// inferred: m * v by value (the copy of the result is kept on the stack)
inline Vector3 DrawTransform_unk(const Matrix3x4& m, const Vector3& v) {
    Vector3 r = v;
    PSMTXMultVec(m.m, &r, &r);
    return r;
}

// inferred: texture ready to be used (same test as gmk/AutomaticDoor and game/GimmickCommon)
inline bool DrawTexReady_unk(Texture_unk* tex) {
    bool ready = false;
    if (tex->mImage != 0 ||
        (((GimmickUvOwner_unk*)tex)->fn_800D59B8() == 0 && ((LoadHandle*)&tex->m20)->fn_80473EFC() == 3)) {
        ready = true;
    }
    return ready;
}

inline bool DrawTexValid_unk(Texture_unk* tex) {
    bool valid = false;
    if (tex != 0 && DrawTexReady_unk(tex)) {
        valid = true;
    }
    return valid;
}

void DrawHelper::fn_8005DED8(int bufSize, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& fontName) {
    {
        MemoryMode_unk mode(0, 0x20);
        m04.mSize = bufSize;
        m04.mData = new ((MemoryTag)6, (const char*)&lbl_8087D72C, (const char*)&lbl_8087D728, 0) unsigned char[bufSize];
    }
    fn_8006C7FC();
    fn_8006CABC(lbl_8087EEC8, fontName);
    fn_800D594C(&m10, "/common/sys_white.texture");
    fn_800D594C(&m40, "/common/sys_gray.texture");
    fn_800D594C(&m70, "/common/sys_fade.texture");
}

void DrawHelper::fn_8005DF90(unsigned int color, GXTexObj* obj, int arg, float x, float y, float z, float w, float h) {
    fn_8005E000(color, obj, 1, arg, x, y, z, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
}

void DrawHelper::fn_8005DFC8(unsigned int color, Texture_unk* tex, int arg, float x, float y, float z, float w, float h) {
    fn_8005E000(color, &tex->mObj, 1, arg, x, y, z, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
}

void DrawHelper::fn_8005E000(unsigned int color, GXTexObj* tex, int mode, int arg, float x, float y, float z,
                             float w, float h, float u0, float v0, float u1, float v1) {
    ComDrawTexture* cmd = m04.New_unk<ComDrawTexture>();
    if (cmd == 0) {
        return;
    }
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[0].mUV = Vector2_unk(u0, v0);
    v[1].mPos = p1;
    v[1].mColor = color;
    v[1].mUV = Vector2_unk(u1, v0);
    v[2].mPos = p2;
    v[2].mColor = color;
    v[2].mUV = Vector2_unk(u1, v1);
    v[3].mPos = p3;
    v[3].mColor = color;
    v[3].mUV = Vector2_unk(u0, v1);
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        mode = 0;
    }
    cmd->m04 = mode;
    cmd->mTex = tex;
    cmd->mVertex = v;
    cmd->m10 = arg;
    if (mode == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m11 = 0;
    } else {
        cmd->m11 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_8005E3E4(unsigned int color, void* tex, float x, float y, float z, float w, float h, float u0,
                             float v0, float u1, float v1, int arg, int flags) {
    fn_8005E40C(color, tex, arg, flags, x, y, z, w, h, u0, v0, u1, v1);
}

void DrawHelper::fn_8005E40C(unsigned int color, void* tex, int arg, int flags, float x, float y, float z,
                             float w, float h, float u0, float v0, float u1, float v1) {
    ComDrawTextureAdd* cmd = m04.New_unk<ComDrawTextureAdd>();
    if (cmd == 0) {
        return;
    }
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[0].mUV = Vector2_unk(u0, v0);
    v[1].mPos = p1;
    v[1].mColor = color;
    v[1].mUV = Vector2_unk(u1, v0);
    v[2].mPos = p2;
    v[2].mColor = color;
    v[2].mUV = Vector2_unk(u1, v1);
    v[3].mPos = p3;
    v[3].mColor = color;
    v[3].mUV = Vector2_unk(u0, v1);
    int mode = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        mode = 0;
    }
    cmd->m04 = mode;
    cmd->mTex = tex;
    cmd->mVertex = v;
    cmd->m10 = flags;
    if (mode == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m11 = 0;
    } else {
        cmd->m11 = 1;
    }
    cmd->m14 = arg;
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_8005E7F4(unsigned int color, void* texture0, void* texture1, float x, float y, float z, float w,
                             float h, float u0, float v0, float u1, float v1) {
    fn_8005E838(color, texture0, texture1, 0, x, y, z, w, h, u0, v0, u1, v1, 0.0f, 0.0f, 1.0f, 1.0f);
}

// NONMATCHING: 98.9% - register allocation of the texture checks (r23/r24 swapped from the vertex array on)
void DrawHelper::fn_8005E838(unsigned int color, void* texture0, void* texture1, int mode, float x, float y, float z,
                             float w, float h, float u0, float v0, float u1, float v1, float s0, float s1, float s2,
                             float s3) {
    ComDrawTextureMask* cmd = m04.New_unk<ComDrawTextureMask>();
    if (cmd == 0) {
        return;
    }
    DrawMaskVertex_unk* v = m04.NewArray_unk<DrawMaskVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[0].mUV = DrawUV2_unk(u0, v0, s0, s1);
    v[1].mPos = p1;
    v[1].mColor = color;
    v[1].mUV = DrawUV2_unk(u1, v0, s2, s1);
    v[2].mPos = p2;
    v[2].mColor = color;
    v[2].mUV = DrawUV2_unk(u1, v1, s2, s3);
    v[3].mPos = p3;
    v[3].mColor = color;
    v[3].mUV = DrawUV2_unk(u0, v1, s0, s3);
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex0 = DrawTexValid_unk((Texture_unk*)texture0) ? (Texture_unk*)texture0 : 0;
    cmd->mTex1 = DrawTexValid_unk((Texture_unk*)texture1) ? (Texture_unk*)texture1 : 0;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m14 = 0;
    } else {
        cmd->m14 = 1;
    }
    cmd->m18 = mode;
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_8005ED6C(unsigned int color, Texture_unk* tex, float z) {
    float h = lbl_8087EEE0->mHeight;
    float w = lbl_8087EEE0->mWidth;
    fn_8005E000(color, &tex->mObj, 1, 1, 0.0f, 0.0f, z, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
}

void DrawHelper::fn_8005EDF0(unsigned int color, void* tex, int arg, int flags, float x, float y, float z, float w, float h,
                             float rot, float u0, float v0, float u1, float v1) {
    fn_8005EEE0(color, tex, arg, flags, x, y, z, w, h, 0.0f, 0.0f, rot, u0, v0, u1, v1);
}

void DrawHelper::fn_8005EE38(unsigned int color, void* texture0, void* texture1, int mode, float x, float y, float z, float w,
                             float h, float rot, float u0, float v0, float u1, float v1, float s0, float s1, float s2,
                             float s3) {
    fn_8005F36C(color, texture0, texture1, mode, x, y, z, w, h, 0.0f, 0.0f, rot, u0, v0, u1, v1, s0, s1, s2, s3);
}

void DrawHelper::fn_8005EEA0(unsigned int color, void* texture, int blend, unsigned int flags, float x, float y, float z,
                             float w, float h, float scaleX, float scaleY, float rotation, float u0, float v0, float u1,
                             float v1) {
    fn_8005EEE0(color, texture, blend, flags, x, y, z, w, h, scaleX, scaleY, rotation, u0, v0, u1, v1);
}

// NONMATCHING: 99.8% - r3/r4 swapped for the vertex and position addresses in the loop
void DrawHelper::fn_8005EEE0(unsigned int color, void* tex, int arg, int flags, float x, float y, float z, float w, float h, float cx, float cy, float rot, float u0, float v0, float u1, float v1) {
    ComDrawTextureAdd* cmd = m04.New_unk<ComDrawTextureAdd>();
    if (cmd == 0) {
        return;
    }
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Mtx rotMtx;
    PSMTXRotRad(rotMtx, 'z', rot);
    Vector3 center(x + cx, y + cy, 0.0f);
    w *= 0.5f;
    h *= 0.5f;
    Vector3 pos[4];
    for (int i = 0; i < 4; i++) {
        pos[i].x = (i == 0 || i == 3) ? -w : w;
        pos[i].y = (i / 2) ? -h : h;
        pos[i].z = 0.0f;
        pos[i] += Vector3(x, y, 0.0f);
        pos[i] = pos[i] - center;
        Vector3* p = &pos[i];
        PSMTXMultVec(rotMtx, p, p);
        pos[i] += center;
        pos[i] = DrawTransform_unk(mA4, pos[i]);
        Vector2_unk uv((i == 0 || i == 3) ? u0 : u1, (i / 2) ? v0 : v1);
        v[i].mPos = pos[i];
        v[i].mColor = color;
        v[i].mUV = uv;
    }
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex = tex;
    cmd->mVertex = v;
    cmd->m10 = flags;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m11 = 0;
    } else {
        cmd->m11 = 1;
    }
    cmd->m14 = arg;
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

// NONMATCHING: 99.1% - register allocation of the texture checks
void DrawHelper::fn_8005F36C(unsigned int color, void* texture0, void* texture1, int mode, float x, float y, float z, float w, float h, float cx, float cy, float rot, float u0, float v0, float u1, float v1, float s0, float s1,
                             float s2, float s3) {
    ComDrawTextureMask* cmd = m04.New_unk<ComDrawTextureMask>();
    if (cmd == 0) {
        return;
    }
    DrawMaskVertex_unk* v = m04.NewArray_unk<DrawMaskVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Mtx rotMtx;
    PSMTXRotRad(rotMtx, 'z', rot);
    Vector3 center(x + cx, y + cy, 0.0f);
    w *= 0.5f;
    h *= 0.5f;
    Vector3 pos[4];
    for (int i = 0; i < 4; i++) {
        pos[i].x = (i == 0 || i == 3) ? -w : w;
        pos[i].y = (i / 2) ? -h : h;
        pos[i].z = 0.0f;
        pos[i] += Vector3(x, y, 0.0f);
        pos[i] = pos[i] - center;
        Vector3* p = &pos[i];
        PSMTXMultVec(rotMtx, p, p);
        *p += center;
        PSMTXMultVec(mA4.m, p, p);
        DrawUV2_unk uv((i == 0 || i == 3) ? u0 : u1, (i / 2) ? v0 : v1, (i == 0 || i == 3) ? s0 : s2,
                       (i / 2) ? s1 : s3);
        v[i].mPos = *p;
        v[i].mColor = color;
        v[i].mUV = uv;
    }
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex0 = DrawTexValid_unk((Texture_unk*)texture0) ? (Texture_unk*)texture0 : 0;
    cmd->mTex1 = DrawTexValid_unk((Texture_unk*)texture1) ? (Texture_unk*)texture1 : 0;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m14 = 0;
    } else {
        cmd->m14 = 1;
    }
    cmd->m18 = mode;
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

// NONMATCHING: 99.1% - register allocation of the texture check
void DrawHelper::fn_8005F90C(unsigned int color, void* texture, float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1) {
    ComDrawTextureMono* cmd = m04.New_unk<ComDrawTextureMono>();
    if (cmd == 0) {
        return;
    }
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[0].mUV = Vector2_unk(u0, v0);
    v[1].mPos = p1;
    v[1].mColor = color;
    v[1].mUV = Vector2_unk(u1, v0);
    v[2].mPos = p2;
    v[2].mColor = color;
    v[2].mUV = Vector2_unk(u1, v1);
    v[3].mPos = p3;
    v[3].mColor = color;
    v[3].mUV = Vector2_unk(u0, v1);
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex = DrawTexValid_unk((Texture_unk*)texture) ? &((Texture_unk*)texture)->mObj : 0;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m11 = 0;
    } else {
        cmd->m11 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

// NONMATCHING: 98.9% - register allocation of the texture checks
void DrawHelper::fn_8005FD80(unsigned int color, void* texture0, void* texture1, int blend_, float x, float y, float z, float w, float h, float u0, float v0,
                             float u1, float v1, float s0, float s1, float s2, float s3) {
    ComDrawTextureMaskMono* cmd = m04.New_unk<ComDrawTextureMaskMono>();
    if (cmd == 0) {
        return;
    }
    DrawMaskVertex_unk* v = m04.NewArray_unk<DrawMaskVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[0].mUV = DrawUV2_unk(u0, v0, s0, s1);
    v[1].mPos = p1;
    v[1].mColor = color;
    v[1].mUV = DrawUV2_unk(u1, v0, s2, s1);
    v[2].mPos = p2;
    v[2].mColor = color;
    v[2].mUV = DrawUV2_unk(u1, v1, s2, s3);
    v[3].mPos = p3;
    v[3].mColor = color;
    v[3].mUV = DrawUV2_unk(u0, v1, s0, s3);
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex0 = DrawTexValid_unk((Texture_unk*)texture0) ? (Texture_unk*)texture0 : 0;
    cmd->mTex1 = DrawTexValid_unk((Texture_unk*)texture1) ? (Texture_unk*)texture1 : 0;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m14 = 0;
    } else {
        cmd->m14 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

// NONMATCHING: 99.7% - r3/r4 swapped for the vertex and position addresses in the loop
void DrawHelper::fn_800602E8(unsigned int color, void* texture, int blend_, unsigned int flags, float x, float y, float z, float w, float h, float cx, float cy, float rot, float u0, float v0, float u1, float v1) {
    ComDrawTextureMono* cmd = m04.New_unk<ComDrawTextureMono>();
    if (cmd == 0) {
        return;
    }
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Mtx rotMtx;
    PSMTXRotRad(rotMtx, 'z', rot);
    Vector3 center(x + cx, y + cy, 0.0f);
    w *= 0.5f;
    h *= 0.5f;
    Vector3 pos[4];
    for (int i = 0; i < 4; i++) {
        pos[i].x = (i == 0 || i == 3) ? -w : w;
        pos[i].y = (i / 2) ? -h : h;
        pos[i].z = 0.0f;
        pos[i] += Vector3(x, y, 0.0f);
        pos[i] = pos[i] - center;
        Vector3* p = &pos[i];
        PSMTXMultVec(rotMtx, p, p);
        pos[i] += center;
        pos[i] = DrawTransform_unk(mA4, pos[i]);
        Vector2_unk uv((i == 0 || i == 3) ? u0 : u1, (i / 2) ? v0 : v1);
        v[i].mPos = pos[i];
        v[i].mColor = color;
        v[i].mUV = uv;
    }
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex = DrawTexValid_unk((Texture_unk*)texture) ? &((Texture_unk*)texture)->mObj : 0;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m11 = 0;
    } else {
        cmd->m11 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

// NONMATCHING: 98.8% - register allocation of the texture checks
void DrawHelper::fn_800607C0(unsigned int color, void* texture0, void* texture1, int blend_, float x, float y, float z, float w, float h, float cx, float cy, float rot, float u0, float v0, float u1, float v1, float s0, float s1,
                             float s2, float s3) {
    ComDrawTextureMaskMono* cmd = m04.New_unk<ComDrawTextureMaskMono>();
    if (cmd == 0) {
        return;
    }
    DrawMaskVertex_unk* v = m04.NewArray_unk<DrawMaskVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Mtx rotMtx;
    PSMTXRotRad(rotMtx, 'z', rot);
    Vector3 center(x + cx, y + cy, 0.0f);
    w *= 0.5f;
    h *= 0.5f;
    Vector3 pos[4];
    for (int i = 0; i < 4; i++) {
        pos[i].x = (i == 0 || i == 3) ? -w : w;
        pos[i].y = (i / 2) ? -h : h;
        pos[i].z = 0.0f;
        pos[i] += Vector3(x, y, 0.0f);
        pos[i] = pos[i] - center;
        Vector3* p = &pos[i];
        PSMTXMultVec(rotMtx, p, p);
        *p += center;
        PSMTXMultVec(mA4.m, p, p);
        DrawUV2_unk uv((i == 0 || i == 3) ? u0 : u1, (i / 2) ? v0 : v1, (i == 0 || i == 3) ? s0 : s2,
                       (i / 2) ? s1 : s3);
        v[i].mPos = *p;
        v[i].mColor = color;
        v[i].mUV = uv;
    }
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex0 = DrawTexValid_unk((Texture_unk*)texture0) ? (Texture_unk*)texture0 : 0;
    cmd->mTex1 = DrawTexValid_unk((Texture_unk*)texture1) ? (Texture_unk*)texture1 : 0;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m14 = 0;
    } else {
        cmd->m14 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_80060D58(float x, float y, float z, float w, float h, unsigned int color) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[1].mPos = p1;
    v[1].mColor = color;
    v[2].mPos = p2;
    v[2].mColor = color;
    v[3].mPos = p3;
    v[3].mColor = color;
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mPrim = 0x80;
    cmd->m10 = 2;
    cmd->mCount = 4;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m18 = 0;
    } else {
        cmd->m18 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_800610A4(unsigned int color, float x, float y, float z, float w, float h) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(5);
    if (v == 0) {
        return;
    }
    Vector3 p4, p3, p2, p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x + w, y, 0.0f));
    p2 = DrawTransform_unk(mA4, Vector3(x + w, y + h, 0.0f));
    p3 = DrawTransform_unk(mA4, Vector3(x, y + h, 0.0f));
    p4 = DrawTransform_unk(mA4, Vector3(x, y, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[1].mPos = p1;
    v[1].mColor = color;
    v[2].mPos = p2;
    v[2].mColor = color;
    v[3].mPos = p3;
    v[3].mColor = color;
    v[4].mPos = p4;
    v[4].mColor = color;
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mPrim = 0xB0;
    cmd->m10 = 4;
    cmd->mCount = 5;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m18 = 0;
    } else {
        cmd->m18 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_8006144C(float x0, float y0, float x1, float y1, unsigned int color, float z) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(2);
    if (v == 0) {
        return;
    }
    Vector3 p1, p0;
    p0 = DrawTransform_unk(mA4, Vector3(x0, y0, 0.0f));
    p1 = DrawTransform_unk(mA4, Vector3(x1, y1, 0.0f));
    v[0].mPos = p0;
    v[0].mColor = color;
    v[1].mPos = p1;
    v[1].mColor = color;
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mPrim = 0xB0;
    cmd->m10 = 1;
    cmd->mCount = 2;
    cmd->mVertex = v;
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {
        cmd->m18 = 0;
    } else {
        cmd->m18 = 1;
    }
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_800616C0(float x, float y, float z, float w, float h, const char* text, unsigned int color, float f6,
                             int a, int b, int c) {
    wchar_t* buf = m104;
    unsigned long len = strlen(text) + 1;
    boost::scoped_array<wchar_t> heap;
    if (len >= 0x200) {
        heap.reset(new ((MemoryTag)6, "", "", 0) wchar_t[len]);
        buf = heap.get();
    }
    fn_8006F2F0(lbl_8087EEC8, buf, text, len);
    fn_80061824(x, y, z, w, h, f6, 0.0f, 0.0f, buf, color, a, b, c, 0, 0xFF000000);
}

#define DRAW_FONT_BODY(TYPE)                                                                                   \
    TYPE* cmd = m04.New_unk<TYPE>();                                                                           \
    if (cmd == 0) {                                                                                            \
        return;                                                                                                \
    }                                                                                                          \
    wchar_t* str = m04.NewArray_unk<wchar_t>(wcslen(text) + 1);                                                \
    if (str == 0) {                                                                                            \
        return;                                                                                                \
    }                                                                                                          \
    wcscpy(str, text);                                                                                         \
    int blend = 1;                                                                                             \
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {                                                                  \
        blend = 0;                                                                                             \
    }                                                                                                          \
    cmd->m04 = blend;                                                                                          \
    cmd->mText = str;                                                                                          \
    cmd->m08 = b;                                                                                              \
    cmd->m0C = 0;                                                                                              \
    if (a != 0) {                                                                                              \
        cmd->m0C |= 1;                                                                                         \
    }                                                                                                          \
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {                                                                  \
        cmd->m0C |= 4;                                                                                         \
    }                                                                                                          \
    cmd->m14 = x;                                                                                              \
    cmd->m18 = y;                                                                                              \
    cmd->m1C = w;                                                                                              \
    cmd->m20 = h;                                                                                              \
    cmd->mColor = color;                                                                                       \
    cmd->m28 = f6;                                                                                             \
    if (blend == 1 || mA0 == 8 || mA0 == 0xD) {                                                                \
        cmd->m2C = 0;                                                                                          \
    } else {                                                                                                   \
        cmd->m2C = 1;                                                                                          \
    }                                                                                                          \
    cmd->m2D = c;                                                                                              \
    cmd->mMtx = mA4;                                                                                           \
    cmd->m60 = d;                                                                                              \
    cmd->m64 = Vector2_unk(f7, f8);                                                                            \
    cmd->m6C = color2;

void DrawHelper::fn_80061824(float x, float y, float z, float w, float h, float f6, float f7, float f8,
                             const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                             unsigned int color2) {
    DRAW_FONT_BODY(ComDrawFont)
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_80061AE4(const wchar_t* text, unsigned int color, int a, int b, int c, int d, unsigned int color2,
                             float x, float y, float z, float w, float h, float f6, float f7, float f8, float clipX,
                             float clipW, float clipScale) {
    DRAW_FONT_BODY(ComDrawFontSideFade)
    cmd->m70 = clipX;
    cmd->m74 = clipW;
    cmd->m78 = clipScale;
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

// inferred: window frame of 9 parts drawn with a corner texture (tex0) masked by tex1: the 4 corners
// keep the size of the texture (scaled, at most half the window) and the 4 edges are stretched between them
// NONMATCHING: 94.8% - same instructions; float/int register allocation differs from the start (z, the
// scales and the sizes) and the ready flag of the texture checks is li instead of mr
void DrawHelper::fn_80061E2C(float x, float y, float z, float w, float h, unsigned int color, void* tex0, void* tex1,
                             int arg, float offsetX, float offsetY, float scaleX, float scaleY) {
    ComDrawWindow* cmd = m04.New_unk<ComDrawWindow>();
    if (cmd == 0) {
        return;
    }
    float left = x + offsetX;
    float top = y + offsetY;
    float width = w - 2.0f * offsetX;
    float height = h - 2.0f * offsetY;
    float texW = (int)GXGetTexObjWidth(&((Texture_unk*)tex0)->mObj);
    float texH = (int)GXGetTexObjHeight(&((Texture_unk*)tex0)->mObj);
    float cw = Abs_unk(0.5f * width);
    float limW = texW * scaleX;
    cw = (cw < limW) ? cw : limW;
    cw = cw * (width / Abs_unk(width));
    float ch = Abs_unk(0.5f * height);
    float limH = texH * scaleY;
    ch = (ch < limH) ? ch : limH;
    ch = ch * (height / Abs_unk(height));
    float innerW = width - 2.0f * cw;
    float innerH = height - 2.0f * ch;
    DrawUV2_unk uvs[4];
    uvs[0] = DrawUV2_unk(0.0f, 0.0f, 1.0f, 1.0f);
    uvs[1] = DrawUV2_unk(1.0f, 0.0f, 0.0f, 1.0f);
    uvs[2] = DrawUV2_unk(1.0f, 1.0f, 0.0f, 0.0f);
    uvs[3] = DrawUV2_unk(0.0f, 1.0f, 1.0f, 0.0f);
    Vector2_unk corners[4] = {Vector2_unk(left, top), Vector2_unk(left + (width - cw), top),
                              Vector2_unk(left + (width - cw), top + (height - ch)),
                              Vector2_unk(left, top + (height - ch))};
    Vector3 p;
    for (int i = 0; i < 4; i++) {
        DrawMaskVertex_unk* v = m04.NewArray_unk<DrawMaskVertex_unk>(4);
        if (v == 0) {
            return;
        }
        p = DrawTransform_unk(mA4, Vector3(corners[i].x, corners[i].y, 0.0f));
        v[0].mPos = p;
        v[0].mColor = color;
        v[0].mUV = DrawUV2_unk(0.0f, 0.0f, uvs[i].u0, uvs[i].v0);
        p = DrawTransform_unk(mA4, Vector3(cw + corners[i].x, corners[i].y, 0.0f));
        v[1].mPos = p;
        v[1].mColor = color;
        v[1].mUV = DrawUV2_unk(1.0f, 0.0f, uvs[i].u1, uvs[i].v0);
        p = DrawTransform_unk(mA4, Vector3(cw + corners[i].x, ch + corners[i].y, 0.0f));
        v[2].mPos = p;
        v[2].mColor = color;
        v[2].mUV = DrawUV2_unk(1.0f, 1.0f, uvs[i].u1, uvs[i].v1);
        p = DrawTransform_unk(mA4, Vector3(corners[i].x, ch + corners[i].y, 0.0f));
        v[3].mPos = p;
        v[3].mColor = color;
        v[3].mUV = DrawUV2_unk(0.0f, 1.0f, uvs[i].u0, uvs[i].v1);
        cmd->mCorner[i] = v;
    }
    Vector3 centers[4];
    Vector3 sizes[4];
    sizes[0].x = innerW + 2.0f * (0.001f * cw);
    sizes[0].y = ch;
    sizes[1].x = innerH + 2.0f * (0.001f * ch);
    sizes[1].y = cw;
    sizes[2].x = innerW + 2.0f * (0.001f * cw);
    sizes[2].y = ch;
    sizes[3].x = innerH + 2.0f * (0.001f * ch);
    sizes[3].y = cw;
    centers[0].x = 0.5f * innerW + (left + cw);
    centers[0].y = 0.5f * ch + top;
    centers[1].x = 0.5f * cw + (left + (width - cw));
    centers[1].y = 0.5f * innerH + (top + ch);
    centers[2].x = 0.5f * innerW + (left + cw);
    centers[2].y = 0.5f * ch + (top + (height - ch));
    centers[3].x = 0.5f * cw + left;
    centers[3].y = 0.5f * innerH + (top + ch);
    for (int j = 0; j < 4; j++) {
        DrawMaskVertex_unk* v = m04.NewArray_unk<DrawMaskVertex_unk>(4);
        if (v == 0) {
            return;
        }
        sizes[j].x *= 0.5f;
        sizes[j].y *= 0.5f;
        Mtx rot;
        PSMTXRotRad(rot, 'z', 1.5707964f * j);
        Vector3 c(centers[j].x, centers[j].y, 0.0f);
        Vector3 pts[4];
        for (int k = 0; k < 4; k++) {
            pts[k].Set_unk((k == 0 || k == 3) ? -sizes[j].x : sizes[j].x, (k / 2) ? -sizes[j].y : sizes[j].y,
                           0.0f);
            pts[k] += Vector3(centers[j].x, centers[j].y, 0.0f);
            pts[k] = pts[k] - c;
            Vector3& p2 = pts[k];
            PSMTXMultVec(rot, &p2, &p2);
            p2 += c;
            pts[k] = DrawTransform_unk(mA4, pts[k]);
            v[k].Set_unk(pts[k], color, DrawUV2_unk((k == 0 || k == 3) ? 0.0f : 1.0f, (k / 2) ? 0.0f : 1.0f,
                                                (k == 0 || k == 3) ? 0.0f : 1.0f, (k / 2) ? 0.0f : 1.0f));
        }
        cmd->mEdge[j] = v;
    }
    int blend = 1;
    if (mA0 == 6 || mA0 == 8 || mA0 == 0xD) {
        blend = 0;
    }
    cmd->m04 = blend;
    cmd->mTex0 = DrawTexValid_unk((Texture_unk*)tex0) ? (Texture_unk*)tex0 : 0;
    cmd->mTex1 = DrawTexValid_unk((Texture_unk*)tex1) ? (Texture_unk*)tex1 : 0;
    cmd->m30 = 1;
    if (mA0 == 8 || mA0 == 0xD) {
        cmd->m30 = 0;
    }
    cmd->m34 = arg;
    fn_800BDB58(lbl_8087EFB4, cmd, z, mA0);
}

void DrawHelper::fn_80062908(ColliObject* colli, int a, int b, int c, int d, float z) {
    ComDrawColliObject* cmd = m04.New_unk<ComDrawColliObject>();
    if (cmd == 0) {
        return;
    }
    cmd->m04 = 0;
    cmd->mColli = colli;
    cmd->m08 = a;
    cmd->m0C = b;
    cmd->m10 = c;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

void DrawHelper::fn_8006297C(ColliTree* colli, int a, int b, int c, int d, float z) {
    ComDrawColliTree* cmd = m04.New_unk<ComDrawColliTree>();
    if (cmd == 0) {
        return;
    }
    cmd->m04 = 0;
    cmd->mColli = colli;
    cmd->m08 = a;
    cmd->m0C = b;
    cmd->m10 = c;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

void DrawHelper::fn_800629F0(const Vector3& center, const Vector3& size, float z, unsigned int color,
                             const Matrix3x4* mtx) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(20);
    if (v == 0) {
        return;
    }
    v[0].mPos.Set_unk(-size.x, -size.y, size.z);
    v[1].mPos.Set_unk(-size.x, size.y, size.z);
    v[2].mPos.Set_unk(size.x, size.y, size.z);
    v[3].mPos.Set_unk(-size.x, -size.y, size.z);
    v[4].mPos.Set_unk(size.x, -size.y, size.z);
    v[5].mPos.Set_unk(size.x, size.y, size.z);
    v[6].mPos.Set_unk(size.x, size.y, -size.z);
    v[7].mPos.Set_unk(size.x, -size.y, size.z);
    v[8].mPos.Set_unk(size.x, -size.y, -size.z);
    v[9].mPos.Set_unk(size.x, size.y, -size.z);
    v[10].mPos.Set_unk(-size.x, size.y, -size.z);
    v[11].mPos.Set_unk(size.x, -size.y, -size.z);
    v[12].mPos.Set_unk(-size.x, -size.y, -size.z);
    v[13].mPos.Set_unk(-size.x, size.y, -size.z);
    v[14].mPos.Set_unk(-size.x, size.y, size.z);
    v[15].mPos.Set_unk(-size.x, -size.y, -size.z);
    v[16].mPos.Set_unk(-size.x, -size.y, size.z);
    v[17].mPos.Set_unk(size.x, -size.y, -size.z);
    v[18].mPos.Set_unk(size.x, size.y, -size.z);
    v[19].mPos.Set_unk(-size.x, size.y, size.z);
    DrawVertex_unk* p = v;
    for (unsigned int i = 0; i < 20; i++) {
        if (mtx != 0) {
            PSMTXMultVec(mtx->m, &p->mPos, &p->mPos);
        }
        p->mPos += center;
        p->mColor = color;
        p++;
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xB0;
    cmd->m10 = 0x13;
    cmd->mCount = 0x14;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

void DrawHelper::fn_80062C8C(unsigned int segments, unsigned int color, float x, float y, float z, float a,
                             float radius, float b) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>((segments + 1) * 2);
    if (v == 0) {
        return;
    }
    float hh = 0.5f * b;
    for (unsigned int i = 0; i < segments + 1; i++) {
        Vector3 p(radius, hh, 0.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', i * (6.2831855f / segments));
        PSMTXMultVec(m, &p, &p);
        p.x += x;
        p.y += y;
        p.z += z;
        v[i * 2].mPos = p;
        v[i * 2].mColor = color;
        p.y -= b;
        DrawVertex_unk& v1 = v[i * 2 + 1];
        v1.mPos = p;
        v1.mColor = color;
    }
    cmd->m04 = 0;
    cmd->mPrim = 0x98;
    cmd->m10 = segments * 2;
    cmd->mCount = (segments + 1) * 2;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

void DrawHelper::fn_80062F7C(unsigned int segments, unsigned int color, float x, float y, float z, float a,
                             float radius, float b, float rot) {
    fn_80062C8C(segments, color, x, y, z, a, radius, b);
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Vector3 c(x, 0.5f * b + y, z);
    float hr = 0.5f * radius;
    v[0].mPos.Set_unk(0.0f, 0.0f, radius);
    v[0].mColor = color;
    v[1].mPos.Set_unk(-hr, 0.0f, 0.7f * -radius);
    v[1].mColor = color;
    v[2].mPos.Set_unk(0.0f, 0.0f, 0.0f);
    v[2].mColor = color;
    v[3].mPos.Set_unk(hr, 0.0f, 0.7f * -radius);
    v[3].mColor = color;
    Mtx m;
    PSMTXRotRad(m, 'y', rot);
    DrawVertex_unk* p = v;
    for (unsigned int i = 0; i < 4; i++) {
        PSMTXMultVec(m, &p->mPos, &p->mPos);
        p->mPos += c;
        p++;
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xA0;
    cmd->m10 = 2;
    cmd->mCount = 4;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

void DrawHelper::fn_80063200(unsigned int segments, unsigned int color, float x, float y, float z, float a,
                             float radius) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(segments + 1);
    if (v == 0) {
        return;
    }
    for (unsigned int i = 0; i < segments + 1; i++) {
        Vector3 p(radius, 0.0f, 0.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', i * (6.2831855f / segments));
        PSMTXMultVec(m, &p, &p);
        p.x += x;
        p.y += y;
        p.z += z;
        v[i].mPos = p;
        v[i].mColor = color;
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xB0;
    cmd->m10 = segments;
    cmd->mCount = segments + 1;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

// NONMATCHING: 99.9% - operand order of the fnmsubs of the start angle (angle * 0.5f)
void DrawHelper::fn_80063484(unsigned int segments, unsigned int color, float x, float y, float z, float rot, float a,
                             float radius, float angle) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(segments + 2);
    if (v == 0) {
        return;
    }
    Vector3 c(x, y, z);
    v[0].mPos = c;
    v[0].mColor = color;
    Vector3 d(0.0f, 0.0f, radius);
    Mtx m;
    PSMTXRotRad(m, 'y', rot - angle * 0.5f);
    PSMTXMultVec(m, &d, &d);
    for (unsigned int i = 1; i < segments + 1; i++) {
        v[i].mPos = d + c;
        v[i].mColor = color;
        Mtx step;
        PSMTXRotRad(step, 'y', angle / (segments - 1));
        PSMTXMultVec(step, &d, &d);
    }
    v[segments + 1].mPos = c;
    v[segments + 1].mColor = color;
    cmd->m04 = 0;
    cmd->mPrim = 0xB0;
    cmd->m10 = segments + 1;
    cmd->mCount = segments + 2;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

void DrawHelper::fn_80063764(const Vector3& a, const Vector3& b, float z, unsigned int color) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(2);
    if (v == 0) {
        return;
    }
    v[0].mPos = a;
    v[0].mColor = color;
    v[1].mPos = b;
    v[1].mColor = color;
    cmd->m04 = 0;
    cmd->mPrim = 0xB0;
    cmd->m10 = 1;
    cmd->mCount = 2;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

// inferred: linear interpolation in the form of atn/Rain (local inline, name invented)
inline Vector3 DrawLerp_unk(const Vector3& a, const Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: arrow from 'from' to 'to' (a line, then a cone of 4 sides at 'to'); 'head' is the length
// of the head (0.3 of the arrow when not positive)
void DrawHelper::fn_800638B0(float a, float head, const Vector3& from, const Vector3& to, unsigned int color) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(2);
    if (v == 0) {
        return;
    }
    v[0].mPos = from;
    v[0].mColor = color;
    v[1].mPos = to;
    v[1].mColor = color;
    cmd->m04 = 0;
    cmd->mPrim = 0xB0;
    cmd->m10 = 1;
    cmd->mCount = 2;
    cmd->mVertex = v;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);

    cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    v = m04.NewArray_unk<DrawVertex_unk>(6);
    if (v == 0) {
        return;
    }
    Vector3 dir = to - from;
    float len = dir.Length_unk();
    dir.Normalize_unk();
    Vector3 up(0.0f, 1.0f, 0.0f);
    if (Abs_unk(dir.y) > 0.9f) {
        up.Set_unk(1.0f, 0.0f, 0.0f);
    }
    Vector3 side;
    PSVECCrossProduct(&dir, &up, &side);
    if (head <= 0.0f) {
        head = 0.3f * len;
    }
    float radius = 0.3f * head;
    Vector3 base = DrawLerp_unk(to, from, head / len);
    v[0].mPos = to;
    v[0].mColor = color;
    for (int i = 0; i < 5; i++) {
        v[i + 1].mPos = side * radius;
        Mtx m;
        PSMTXRotAxisRad(m, &dir, 1.5707964f * i);
        PSMTXMultVec(m, &v[i + 1].mPos, &v[i + 1].mPos);
        v[i + 1].mColor = color;
        v[i + 1].mPos += base;
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xA0;
    cmd->m10 = 4;
    cmd->mCount = 6;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

// inferred: wire sphere (4 meridians around y, the arc down to the bottom, 4 meridians around x)
void DrawHelper::fn_80063D3C(const Vector3& pos, float radius, float a, unsigned int color) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    unsigned int n = 8;
    unsigned int count = n * 8 + 3;
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(count);
    if (v == 0) {
        return;
    }
    Mtx outerY;
    Mtx outerX;
    v[0].mPos = Vector3(pos.x, pos.y + radius, pos.z);
    v[0].mColor = color;
    for (unsigned int j = 0; j < 4; j++) {
        PSMTXRotRad(outerY, 'y', 0.7853982f * j);
        for (unsigned int i = 0; i < n; i++) {
            Vector3 p(0.0f, radius, 0.0f);
            Mtx mz;
            PSMTXRotRad(mz, 'z', (i + 1) * (6.2831855f / n));
            PSMTXMultVec(mz, &p, &p);
            PSMTXMultVec(outerY, &p, &p);
            p += pos;
            DrawVertex_unk& d = v[i + j * n + 1];
            d.mPos = p;
            d.mColor = color;
        }
    }
    for (unsigned int i = 0; i < n / 4; i++) {
        Vector3 p(0.0f, radius, 0.0f);
        Mtx mz;
        PSMTXRotRad(mz, 'z', (i + 1) * (6.2831855f / n));
        PSMTXMultVec(mz, &p, &p);
        p += pos;
        DrawVertex_unk& d = v[i + 4 * n + 1];
        d.mPos = p;
        d.mColor = color;
    }
    for (unsigned int j = 0; j < 4; j++) {
        PSMTXRotRad(outerX, 'x', 0.7853982f * j);
        for (unsigned int i = 0; i < n; i++) {
            Vector3 p(-radius, 0.0f, 0.0f);
            Mtx my;
            PSMTXRotRad(my, 'y', (i + 1) * (6.2831855f / n));
            PSMTXMultVec(my, &p, &p);
            PSMTXMultVec(outerX, &p, &p);
            p += pos;
            DrawVertex_unk& d = v[i + n / 4 + (j + 4) * n + 1];
            d.mPos = p;
            d.mColor = color;
        }
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xB0;
    cmd->m10 = count - 1;
    cmd->mCount = count;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

// inferred: cross product returned by value (local inline, name invented)
inline Vector3 DrawCross_unk(const Vector3& a, const Vector3& b) {
    Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: point transformed by a matrix, by value (local inline, name invented)
inline Vector3 DrawMultVec_unk(const Mtx m, const Vector3& v) {
    Vector3 r = v;
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: point of the unit circle in the xy plane (clockwise) at height z (local inline, name invented)
inline Vector3 DrawCircle_unk(float t, float z) {
    return Vector3((float)cos(t), -(float)sin(t), z);
}

// inferred: wire cone from 'pos' along 'dir' ('angle' in degrees, half aperture)
// NONMATCHING: 99.9% - r3/r4 swapped in the copy of the fallback up vector (Vector3 copy from a
// temporary, docs/mwcc-blockers-q3.md)
void DrawHelper::fn_800641CC(const Vector3& pos, const Vector3& dir, unsigned int color, float length, float angle,
                             float a) {
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    int n = 12;
    int count = n * 4;
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(count);
    if (v == 0) {
        return;
    }
    Vector3 up(0.0f, 1.0f, 0.0f);
    if (Abs_unk(PSVECDotProduct(&dir, &up)) > 0.99f) {
        up = Vector3(0.0f, 0.0f, -1.0f);
    }
    Vector3 right = DrawCross_unk(dir, up).GetNormalize();
    up = DrawCross_unk(right, dir).GetNormalize();
    Camera_unk camera(0, 0);
    camera.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    camera.SetTarget_unk(Vector3(-dir.x, -dir.y, -dir.z));
    camera.mUp = up;
    camera.fn_8004B378();
    Matrix3x4 view = camera.mView;
    Mtx trans;
    PSMTXTrans(trans, pos.x, pos.y, pos.z);
    Mtx m;
    PSMTXConcat(trans, view.m, m);
    float r = length * (float)sin(DegToRad_unk(angle));
    float t = 0.0f;
    float step = 6.2831855f / n;
    for (int i = 0; i < n; i++) {
        Vector3 s(r, r, 1.0f);
        Vector3 p0 = DrawCircle_unk(t, length);
        Vector3 p1 = DrawCircle_unk(t + step, length);
        p0.x *= s.x;
        p0.y *= s.y;
        p0.z *= s.z;
        p1.x *= s.x;
        p1.y *= s.y;
        p1.z *= s.z;
        Vector3 q0 = DrawMultVec_unk(m, p0);
        Vector3 q1 = DrawMultVec_unk(m, p1);
        DrawVertex_unk& v0 = v[i * 4];
        v0.mPos = q0;
        v0.mColor = color;
        DrawVertex_unk& v1 = v[i * 4 + 1];
        v1.mPos = q1;
        v1.mColor = color;
        DrawVertex_unk& v2 = v[i * 4 + 2];
        v2.mPos = pos;
        v2.mColor = color;
        DrawVertex_unk& v3 = v[i * 4 + 3];
        v3.mPos = q0;
        v3.mColor = color;
        t += step;
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xA8;
    cmd->m10 = count / 2;
    cmd->mCount = count;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, a, 0xD);
}

// inferred: exact comparison of two points (local inline, name invented)
inline bool DrawSamePos_unk(const Vector3& a, const Vector3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// inferred: wire capsule between 'a' and 'b' (two spheres and 8 lines)
// NONMATCHING: 99.9% - r19/r20 swapped for the addresses of 'side' and 'p' in the loop (copy of a
// Vector3, docs/mwcc-blockers-q3.md)
void DrawHelper::fn_8006471C(const Vector3& a, const Vector3& b, unsigned int color, float radius, float z) {
    fn_80063D3C(a, radius, z, color);
    if (DrawSamePos_unk(a, b)) {
        return;
    }
    fn_80063D3C(b, radius, z, color);
    ComDrawLine* cmd = m04.New_unk<ComDrawLine>();
    if (cmd == 0) {
        return;
    }
    int count = 16;
    DrawVertex_unk* v = m04.NewArray_unk<DrawVertex_unk>(count);
    if (v == 0) {
        return;
    }
    Vector3 dir = b - a;
    if (dir.LengthSq_unk() < 0.00001f) {
        return;
    }
    dir.Normalize_unk();
    Vector3 side;
    if (Abs_unk(dir.y) > 0.99f) {
        side.Set_unk(0.0f, 0.0f, radius);
    } else {
        side = DrawCross_unk(dir, Vector3(0.0f, 1.0f, 0.0f));
        side.Normalize_unk();
        side *= radius;
    }
    for (unsigned int i = 0; i < 8; i++) {
        Vector3 p = side;
        Mtx m;
        PSMTXRotAxisRad(m, &dir, 0.7853982f * i);
        PSMTXMultVec(m, &p, &p);
        v[i * 2].Set_unk(p + a, color);
        v[i * 2 + 1].Set_unk(p + b, color);
    }
    cmd->m04 = 0;
    cmd->mPrim = 0xA8;
    cmd->m10 = 8;
    cmd->mCount = count;
    cmd->mVertex = v;
    cmd->m18 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

// inferred: textured quad of 1x1 centered in the origin, scaled, rotated (z, y, x) and moved to 'pos'
// (a 3D sprite of game/GameUI); uv0 for the left/top corners, uv1 for the others
// NONMATCHING: 92.6% - stack slots of the matrices (the original has the locals of the three
// __MtxRotate before the identity and the products) and the stfsux of the corner
void DrawHelper::fn_80064AC4(const Vector3& pos, const Vector3& rot, const Vector3& scale, const Vector2_unk& uv0,
                             const Vector2_unk& uv1, unsigned int color, int a, int b, int c, float z) {
    ComDrawD3TextureMask* cmd = m04.New_unk<ComDrawD3TextureMask>();
    if (cmd == 0) {
        return;
    }
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(4);
    if (v == 0) {
        return;
    }
    Mtx mtx;
    Vector3 pts[4];
    PSMTXTrans(mtx, pos.x, pos.y, pos.z);
    Matrix3x4 r;
    r.SetIdentity();
    __MtxRotate(r.m, 'z', rot.z);
    __MtxRotate(r.m, 'y', rot.y);
    __MtxRotate(r.m, 'x', rot.x);
    Mtx t;
    PSMTXConcat(mtx, r.m, t);
    __MtxCopy(mtx, t);
    Mtx sc;
    PSMTXScale(sc, scale.x, scale.y, scale.z);
    Mtx t2;
    PSMTXConcat(mtx, sc, t2);
    __MtxCopy(mtx, t2);
    for (int i = 0; i < 4; i++) {
        int col = i % 2;
        Vector3& p = pts[i];
        p.x = col ? -0.5f : 0.5f;
        int row = i / 2;
        p.y = row ? 0.5f : -0.5f;
        p.z = 0.0f;
        PSMTXMultVec(mtx, &p, &p);
        Vector2_unk uv(col ? uv0.x : uv1.x, row ? uv0.y : uv1.y);
        v[i].mPos = p;
        v[i].mColor = color;
        v[i].mUV = uv;
    }
    cmd->m04 = 0;
    cmd->m0C = 0x98;
    cmd->m10 = 2;
    cmd->mCount = 4;
    cmd->m18 = a;
    cmd->m1C = b;
    cmd->mVertex = v;
    cmd->m20 = c;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

void DrawHelper::fn_80064F00(const DrawTexVertex_unk* src, int n, int prim, int a, int b, float z) {
    ComDrawD3TextureMask* cmd = m04.New_unk<ComDrawD3TextureMask>();
    if (cmd == 0) {
        return;
    }
    int size = n * sizeof(DrawTexVertex_unk);
    DrawTexVertex_unk* v = m04.NewArray_unk<DrawTexVertex_unk>(n);
    if (v == 0) {
        return;
    }
    memcpy(v, src, size);
    cmd->m04 = 0;
    cmd->m0C = prim;
    cmd->m10 = fn_800760E8(n, prim);
    cmd->mCount = n;
    cmd->m18 = a;
    cmd->m1C = b;
    cmd->mVertex = v;
    cmd->m20 = 1;
    fn_800BDB58(lbl_8087EFB4, cmd, z, 0xD);
}

// inferred: color 0xAARRGGBB to the GX order (RGBA)
inline unsigned int DrawRGBA_unk(unsigned int c) {
    return (c << 8) | (c >> 24);
}

// inferred: depth test on (render states 8 and 9) and depth write (state 0xA); name invented
inline void DrawSetZMode_unk(RenderState_unk* device, int write) {
    fn_80076760(device, 8, 1);
    fn_80076760(device, 9, 2);
    fn_80076760(device, 0xA, write);
}

void DrawHelper::ComDrawLine::vf10(RenderView_unk* view) {
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetBlendMode(1, 4, 5, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        fn_800761A8(lbl_8087EEE0);
        GXBegin(mPrim, 0, mCount);
        for (int i = 0; i < mCount; i++) {
            GXPosition2f32(mVertex[i].mPos.x, mVertex[i].mPos.y);
            GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        }
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m18);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        fn_800761A8(lbl_8087EEE0);
        GXBegin(mPrim, 0, mCount);
        for (int i = 0; i < mCount; i++) {
            GXPosition3f32(mVertex[i].mPos.x, mVertex[i].mPos.y, mVertex[i].mPos.z);
            GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        }
    }
}

void DrawHelper::ComDrawTexture::vf10(RenderView_unk* view) {
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m11);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    fn_800763FC(lbl_8087EEE0, mTex, 0);
    GXSetBlendMode(m10 != 0, 4, 5, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    }
    fn_800761A8(lbl_8087EEE0);
    GXBegin(0x80, 0, 4);
    for (int i = 0; i < 4; i++) {
        if (m04 == 1) {
            GXPosition2f32(mVertex[i].mPos.x, mVertex[i].mPos.y);
        } else {
            GXPosition3f32(mVertex[i].mPos.x, mVertex[i].mPos.y, mVertex[i].mPos.z);
        }
        GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        GXTexCoord2f32(mVertex[i].mUV.x, mVertex[i].mUV.y);
    }
}

void DrawHelper::ComDrawTextureAdd::vf10(RenderView_unk* view) {
    fn_800761A8(lbl_8087EEE0);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m11);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    fn_800763FC(lbl_8087EEE0, (GXTexObj*)mTex, 0);
    switch (m14) {
    case 0:
        GXSetBlendMode(m10 != 0, 4, 5, 0);
        break;
    case 1:
        GXSetBlendMode(m10 != 0, 4, 1, 0);
        break;
    case 2:
        GXSetBlendMode(m10 != 0, 3, 1, 0);
        break;
    case 3:
        GXSetBlendMode(m10 != 0, 0, 5, 0);
        break;
    case 4:
        GXSetBlendMode(m10 != 0, 0, 3, 0);
        break;
    }
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    }
    GXBegin(0x80, 0, 4);
    for (int i = 0; i < 4; i++) {
        if (m04 == 1) {
            GXPosition2f32(mVertex[i].mPos.x, mVertex[i].mPos.y);
        } else {
            GXPosition3f32(mVertex[i].mPos.x, mVertex[i].mPos.y, mVertex[i].mPos.z);
        }
        GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        GXTexCoord2f32(mVertex[i].mUV.x, mVertex[i].mUV.y);
    }
    GXSetBlendMode(m10 != 0, 4, 5, 0);
}

void DrawHelper::ComDrawTextureMask::vf10(RenderView_unk* view) {
    fn_800761A8(lbl_8087EEE0);
    GXSetNumChans(1);
    GXSetNumTexGens(2);
    GXSetNumTevStages(2);
    GXSetTevSwapModeTable(1, 0, 0, 0, 0);
    GXSetTevSwapMode(1, 0, 1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorIn(0, 0xF, 8, 0xA, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0, 0xC, 0xF);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 5, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m14);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    fn_800763FC(lbl_8087EEE0, mTex0 != 0 ? &mTex0->mObj : &lbl_8087EEB0->m10.mObj, 0);
    fn_800763FC(lbl_8087EEE0, mTex1 != 0 ? &mTex1->mObj : &lbl_8087EEB0->m10.mObj, 1);
    switch (m18) {
    case 0:
        GXSetBlendMode(1, 4, 5, 0);
        break;
    case 1:
        GXSetBlendMode(1, 4, 1, 0);
        break;
    case 2:
        GXSetBlendMode(1, 3, 1, 0);
        break;
    case 3:
        GXSetBlendMode(1, 0, 5, 0);
        break;
    case 4:
        GXSetBlendMode(1, 0, 3, 0);
        break;
    }
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxDesc(0xE, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xE, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xE, 1, 4, 0);
    }
    fn_800761A8(lbl_8087EEE0);
    GXBegin(0x80, 0, 4);
    for (int i = 0; i < 4; i++) {
        if (m04 == 1) {
            GXPosition2f32(mVertex[i].mPos.x, mVertex[i].mPos.y);
        } else {
            GXPosition3f32(mVertex[i].mPos.x, mVertex[i].mPos.y, mVertex[i].mPos.z);
        }
        GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        GXTexCoord2f32(mVertex[i].mUV.u0, mVertex[i].mUV.v0);
        GXTexCoord2f32(mVertex[i].mUV.u1, mVertex[i].mUV.v1);
    }
    GXSetTevSwapMode(1, 0, 0);
    GXSetBlendMode(1, 4, 5, 0);
}

// inferred: float color with a plain (member-wise) copy, as in atn/PostFxColorMatrix (name invented)
struct DrawColorF_unk {
    DrawColorF_unk() {}
    DrawColorF_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    float x, y, z, w;
};
static inline GXColor DrawToGXColor_unk(const DrawColorF_unk& v) {
    GXColor c = {(unsigned char)(255.0f * v.x), (unsigned char)(255.0f * v.y), (unsigned char)(255.0f * v.z),
                 (unsigned char)(255.0f * v.w)};
    return c;
}

// inferred: grey scale version of ComDrawTexture (color matrix with the luminance weights in the TEV)
// NONMATCHING: 98.1% - same instructions; order of the stores of the matrix (by columns in the original) and
// stack slot of lum
void DrawHelper::ComDrawTextureMono::vf10(RenderView_unk* view) {
    float s = 1.0f;
    float lum[4] = {0.298912f, 0.586611f, 0.114478f, 0.0f};
    DrawColorF_unk mat[4] = {
        DrawColorF_unk(lum[0] * s, lum[0] * s, lum[0] * s, 0.0f),
        DrawColorF_unk(lum[1] * s, lum[1] * s, lum[1] * s, 0.0f),
        DrawColorF_unk(lum[2] * s, lum[2] * s, lum[2] * s, 0.0f),
        DrawColorF_unk(0.0f, 0.0f, 0.0f, 1.0f),
    };
    DrawColorF_unk k[4];
    for (int i = 0; i < 3; i++) {
        DrawColorF_unk c = mat[i];
        GXColor b = DrawToGXColor_unk(c);
        k[i] = DrawColorF_unk(b.r / 255.0f, b.g / 255.0f, b.b / 255.0f, b.a / 255.0f);
    }
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(4);
    fn_800763FC(lbl_8087EEE0, mTex, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetTevKColor(0, DrawToGXColor_unk(k[0]));
    GXSetTevKColor(1, DrawToGXColor_unk(k[1]));
    GXSetTevKColor(2, DrawToGXColor_unk(k[2]));
    GXSetTevSwapModeTable(0, 0, 0, 0, 0);
    GXSetTevSwapModeTable(1, 1, 1, 1, 1);
    GXSetTevSwapModeTable(2, 2, 2, 2, 2);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevColorIn(0, 0xF, 0xE, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 7, 7, 7);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetTevSwapMode(1, 1, 1);
    GXSetTevKColorSel(1, 0xD);
    GXSetTevColorIn(1, 0xF, 0xE, 8, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 7);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevDirect(1);
    GXSetTevSwapMode(2, 2, 2);
    GXSetTevKColorSel(2, 0xE);
    GXSetTevColorIn(2, 0xF, 0xE, 8, 0);
    GXSetTevAlphaIn(2, 7, 7, 7, 7);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevOrder(2, 0, 0, 0xFF);
    GXSetTevDirect(2);
    GXSetTevSwapMode(3, 3, 3);
    GXSetTevColorIn(3, 0xF, 0, 0xA, 0xF);
    GXSetTevAlphaIn(3, 7, 4, 5, 7);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevOrder(3, 0, 0, 4);
    GXSetTevDirect(3);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m11);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    }
    fn_800761A8(lbl_8087EEE0);
    GXBegin(0x80, 0, 4);
    for (int i = 0; i < 4; i++) {
        if (m04 == 1) {
            GXPosition2f32(mVertex[i].mPos.x, mVertex[i].mPos.y);
        } else {
            GXPosition3f32(mVertex[i].mPos.x, mVertex[i].mPos.y, mVertex[i].mPos.z);
        }
        GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        GXTexCoord2f32(mVertex[i].mUV.x, mVertex[i].mUV.y);
    }
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapModeTable(2, 0, 1, 2, 3);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevSwapMode(3, 0, 0);
}

// inferred: grey scale version of ComDrawTextureMask
// NONMATCHING: 98.1% - same instructions; as in ComDrawTextureMono::vf10, order of the stores of the
// matrix and stack slot of lum
void DrawHelper::ComDrawTextureMaskMono::vf10(RenderView_unk* view) {
    fn_800761A8(lbl_8087EEE0);
    float s = 1.0f;
    float lum[4] = {0.298912f, 0.586611f, 0.114478f, 0.0f};
    DrawColorF_unk mat[4] = {
        DrawColorF_unk(lum[0] * s, lum[0] * s, lum[0] * s, 0.0f),
        DrawColorF_unk(lum[1] * s, lum[1] * s, lum[1] * s, 0.0f),
        DrawColorF_unk(lum[2] * s, lum[2] * s, lum[2] * s, 0.0f),
        DrawColorF_unk(0.0f, 0.0f, 0.0f, 1.0f),
    };
    DrawColorF_unk k[4];
    for (int i = 0; i < 3; i++) {
        DrawColorF_unk c = mat[i];
        GXColor b = DrawToGXColor_unk(c);
        k[i] = DrawColorF_unk(b.r / 255.0f, b.g / 255.0f, b.b / 255.0f, b.a / 255.0f);
    }
    GXSetNumChans(1);
    GXSetNumTexGens(2);
    GXSetNumTevStages(4);
    GXSetTevKColor(0, DrawToGXColor_unk(k[0]));
    GXSetTevKColor(1, DrawToGXColor_unk(k[1]));
    GXSetTevKColor(2, DrawToGXColor_unk(k[2]));
    GXSetTevSwapModeTable(0, 0, 0, 0, 0);
    GXSetTevSwapModeTable(1, 1, 1, 1, 1);
    GXSetTevSwapModeTable(2, 2, 2, 2, 2);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevColorIn(0, 0xF, 0xE, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 7, 7, 1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetTevSwapMode(1, 1, 1);
    GXSetTevKColorSel(1, 0xD);
    GXSetTevColorIn(1, 0xF, 0xE, 8, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 1);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevDirect(1);
    GXSetTevSwapMode(2, 2, 2);
    GXSetTevKColorSel(2, 0xE);
    GXSetTevColorIn(2, 0xF, 0xE, 8, 0);
    GXSetTevAlphaIn(2, 7, 7, 7, 1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevOrder(2, 0, 0, 0xFF);
    GXSetTevDirect(2);
    GXSetTevSwapMode(3, 3, 0);
    GXSetTevColorIn(3, 0xF, 0, 0xA, 0xF);
    GXSetTevAlphaIn(3, 7, 4, 5, 7);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevOrder(3, 1, 1, 4);
    GXSetTevDirect(3);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 5, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m14);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    fn_800763FC(lbl_8087EEE0, mTex0 != 0 ? &mTex0->mObj : &lbl_8087EEB0->m10.mObj, 0);
    fn_800763FC(lbl_8087EEE0, mTex1 != 0 ? &mTex1->mObj : &lbl_8087EEB0->m10.mObj, 1);
    GXSetBlendMode(1, 4, 5, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxDesc(0xE, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xE, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xE, 1, 4, 0);
    }
    fn_800761A8(lbl_8087EEE0);
    GXBegin(0x80, 0, 4);
    for (int i = 0; i < 4; i++) {
        if (m04 == 1) {
            GXPosition2f32(mVertex[i].mPos.x, mVertex[i].mPos.y);
        } else {
            GXPosition3f32(mVertex[i].mPos.x, mVertex[i].mPos.y, mVertex[i].mPos.z);
        }
        GXColor1u32(DrawRGBA_unk(mVertex[i].mColor));
        GXTexCoord2f32(mVertex[i].mUV.u0, mVertex[i].mUV.v0);
        GXTexCoord2f32(mVertex[i].mUV.u1, mVertex[i].mUV.v1);
    }
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapModeTable(2, 0, 1, 2, 3);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevSwapMode(3, 0, 0);
}

void DrawHelper::ComDrawFont::vf10(RenderView_unk* view) {
    RenderState_unk* device = lbl_8087EEE0;
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevSwapModeTable(1, 0, 0, 0, 0);
    GXSetTevSwapMode(0, 0, 1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorIn(0, 0xF, 0xC, 0xA, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m2C);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    if (m2D != 0) {
        fn_80072914(device, 1);
        GXSetBlendMode(1, 4, 1, 0);
    } else {
        fn_80072914(device, 0);
        GXSetBlendMode(1, 4, 5, 0);
    }
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        *(Matrix3x4*)lbl_8087EEC8->m33268 = mMtx;
    }
    if (m60 == 1) {
        m0C |= 2;
    } else if (m60 == 2) {
        m0C |= 8;
    } else if (m60 == 3) {
        m0C |= 0x10;
    }
    fn_8006D7B0(lbl_8087EEC8, view, m14, m18, m1C, m20, mText, mColor, m0C, m08, m28, m6C, m64.x, m64.y);
    ((Matrix3x4*)lbl_8087EEC8->m33268)->SetIdentity();
    GXSetTevSwapMode(0, 0, 0);
    if (m2D != 0) {
        fn_80072914(device, 0);
        GXSetBlendMode(1, 4, 5, 0);
    }
}

// inferred: character of a text faded at the sides, kept in the buffer of the font manager
// (Font_unk::m332A0); the character and a terminator form a one-character string
struct DrawFadeChar_unk {
    wchar_t mStr[2];
    float mX;
    unsigned int mAlpha;
};
struct DrawFadeText_unk {
    int mLength;
    DrawFadeChar_unk mChars[1];
};

// inferred: text that fades out at the left and right ends of a window of width m74 (fade of m74 * m78),
// drawn one character at a time
// NONMATCHING: 92.3% - the original keeps the buffer base and an offset for the visible characters (ours
// walks a pointer), so the registers differ
void DrawHelper::ComDrawFontSideFade::vf10(RenderView_unk* view) {
    int len = wcslen(mText);
    float fade = m74 * m78;
    Font_unk* font = lbl_8087EEC8;
    int count = 0;
    DrawFadeText_unk* buf = (DrawFadeText_unk*)&font->m332A0;
    buf->mLength = len;
    float x = m14 - m70;
    float right = (m74 - fade) - m1C;
    for (int i = 0; i < len; i++) {
        wchar_t c = mText[i];
        if (0.0f <= x && x <= m74 - m1C) {
            DrawFadeChar_unk* e = &buf->mChars[count];
            if (x < fade) {
                e->mAlpha = (unsigned int)(255.0f * (x / fade)) << 24;
            } else if (right < x) {
                e->mAlpha = (unsigned int)(255.0f * (1.0f - (x - right) / fade)) << 24;
            } else {
                e->mAlpha = 0xFF000000;
            }
            count++;
            e->mX = x + m70;
            e->mStr[0] = c;
            e->mStr[1] = 0;
        }
        x += fn_8006F17C(lbl_8087EEC8, c, m0C & 1, m1C, m08);
    }
    RenderState_unk* device = lbl_8087EEE0;
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevSwapModeTable(1, 0, 0, 0, 0);
    GXSetTevSwapMode(0, 0, 1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorIn(0, 0xF, 0xC, 0xA, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    if (m04 == 1) {
        Matrix3x4 m;
        m.SetIdentity();
        GXLoadPosMtxImm(m.m, 0);
        GXSetCurrentMtx(0);
    } else {
        fn_80076760(lbl_8087EEE0, 6, 0);
        DrawSetZMode_unk(lbl_8087EEE0, m2C);
        GXLoadPosMtxImm(view->mCamera.mView.m, 0);
        GXSetCurrentMtx(0);
    }
    if (m2D != 0) {
        fn_80072914(device, 1);
        GXSetBlendMode(1, 4, 1, 0);
    } else {
        fn_80072914(device, 0);
        GXSetBlendMode(1, 4, 5, 0);
    }
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    if (m04 == 1) {
        GXSetVtxAttrFmt(0, 9, 0, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    } else {
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        *(Matrix3x4*)lbl_8087EEC8->m33268 = mMtx;
    }
    if (m60 == 1) {
        m0C |= 2;
    } else if (m60 == 2) {
        m0C |= 8;
    } else if (m60 == 3) {
        m0C |= 0x10;
    }
    for (int i = 0; i < count; i++) {
        DrawFadeChar_unk* e = &buf->mChars[i];
        fn_8006D7B0(lbl_8087EEC8, view, e->mX, m18, m1C, m20, e->mStr, (mColor & 0xFFFFFF) | e->mAlpha, m0C, m08,
                    m28, (m6C & 0xFFFFFF) | e->mAlpha, m64.x, m64.y);
    }
    ((Matrix3x4*)lbl_8087EEC8->m33268)->SetIdentity();
    GXSetTevSwapMode(0, 0, 0);
    if (m2D != 0) {
        fn_80072914(device, 0);
        GXSetBlendMode(1, 4, 5, 0);
    }
}

// Pending (in the order of the .dol): the vf10 of every command (0x80065060-0x800685E4) and the weak
// destructors.

} // namespace atn
