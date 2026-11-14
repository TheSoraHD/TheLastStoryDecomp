#ifndef ATN_LAYERSHADOW_H
#define ATN_LAYERSHADOW_H

#include "atn/GXDecl.h"
#include "atn/Camera_unk.h"
#include "atn/AABB_unk.h"
#include "atn/Layer3D.h"

namespace atn {

// inferred: shadow map layer (depth from the light camera)
class LayerShadow : public Layer3D {
public:
    LayerShadow();
    virtual ~LayerShadow();
    virtual void vf10(void* list); // inferred: list of the render objects (ShadowNode_unk)
    virtual void vf1C();
    virtual void vf20();

    AABB_unk fn_800B6C70(float step, const AABB_unk& box);
    Camera_unk fn_800B7090(const AABB_unk& box, const Vector3& dir);
    // inferred: copies the frame buffer at half size into mImage; name invented
    void CopyHalf_unk(unsigned short w, unsigned short h) {
        GXSetTexCopySrc(0, 0, w, h);
        GXSetTexCopyDst(w / 2, h / 2, 4, 1);
        GXCopyTex(mImage, 1);
    }

    AABB_unk mBox;            // 0x5C: inferred: shadow receiver box
    Camera_unk mCamera;       // 0x74: inferred: view camera of the frame
    Camera_unk mLightCamera;  // 0x268
    void* mImage;             // 0x45C
    GXTexObj mTex;            // 0x460
    GXTexObj mDepthTex;       // 0x480
    Matrix3x4 mDepthMtx;      // 0x4A0: inferred: view space to light depth
    Matrix3x4 mTexMtx;        // 0x4D0: inferred: view space to shadow texture
};


} // namespace atn

#endif
