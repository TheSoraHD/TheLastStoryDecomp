#ifndef ATN_DRAWCOMMAND_UNK_H
#define ATN_DRAWCOMMAND_UNK_H

// Drawing commands of atn::DrawHelper used outside atn/DrawHelper.cpp (MiniMapTask::vf10 builds a
// ComDrawTextureMask on the stack and draws it). Moved from atn/DrawHelper.cpp; names invented.
#include "atn/DrawHelper.h"
#include "atn/IRender.h"
#include "math/Vec.h"
#include "atn/Vector4_unk.h"

namespace atn {

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
    // inferred: same with a Vector4_unk (weak copy 0x804ABF4C of game/MiniMapTask.cpp, called out of line)
    void Set_unk(const Vector3& pos, unsigned int color, const Vector4_unk& uv);
    Vector3 mPos;
    unsigned int mColor;
    DrawUV2_unk mUV;
};

__declspec(noinline) inline void DrawMaskVertex_unk::Set_unk(const Vector3& pos, unsigned int color,
                                                           const Vector4_unk& uv) {
    mPos = pos;
    mColor = color;
    __Vec4Copy(&mUV.u0, &uv.x);
}

class DrawHelper::DrawCommand : public IRender {
public:
    virtual ~DrawCommand() {}
};

class DrawHelper::ComDrawTextureMask : public DrawHelper::DrawCommand {
public:
    virtual ~ComDrawTextureMask() {}
    virtual void vf10(RenderManager* view);

    int m04;
    DrawMaskVertex_unk* mVertex; // 0x08
    Texture_unk* mTex0;          // 0x0C
    Texture_unk* mTex1;          // 0x10
    unsigned char m14;
    int m18;
    int m1C;
};

} // namespace atn

#endif
