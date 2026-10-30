#include "atn/DrawHelper.h"
#include "atn/ColliBase.h"
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
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" void fn_800BDB58(void* renderSystem, void* render, float order, int pass); // inferred: queues a render
extern "C" void fn_8006C7FC();
extern "C" int fn_8006CABC(void* font, const std::string& dir);
extern "C" void* lbl_8087EEC8;
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

// Pending (in the order of the .dol): fn_800638B0, fn_80063D3C, fn_800641CC, fn_8006471C, fn_80064AC4 before
// fn_80064F00; fn_80061E2C (window) before fn_80062908; then the vf10 of every command
// (0x80065060-0x800685E4) and the weak destructors.

} // namespace atn
