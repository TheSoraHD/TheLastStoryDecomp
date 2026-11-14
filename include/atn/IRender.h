#ifndef ATN_IRENDER_H
#define ATN_IRENDER_H


// inferred: view frustum (defined in atn/Camera_unk.h)
struct Frustum_unk;

namespace atn {

// inferred: render system passed to the draw virtuals (defined in atn/RenderManager.h)
class RenderManager;
class Matrix3x4;
class Vector3;
struct AABB_unk;

// inferred: renderer interface (RTTI __RTTI__Q23atn7IRender; base of atn::ModelBase and Event::DialogHead).
// Other files still keep local copies (atn/VideoTHP.cpp, atn/Rain.cpp, ...). The first
// virtual is named GetRenderType_unk instead of vf0C: Event::DialogHead overrides it apart from
// atn::Task::vf0C, which has the same signature. The inline virtuals are emitted (weak) first in
// atn/AABBTree.cpp (0x800461FC-0x8004621C).
class IRender {
public:
    virtual ~IRender() {}
    virtual int GetRenderType_unk() { return 2; }                          // 0x80046218
    virtual void vf10(RenderManager* view) = 0;                            // inferred: draw
    // inferred: shadow test (called by atn/LayerShadow.cpp with the view matrix, the camera position, the
    // frustum and the output box; nonzero when the object casts a shadow)
    virtual int vf14(const Matrix3x4*, const Vector3*, const Frustum_unk*, AABB_unk*) { return 1; } // 0x80046200
    virtual void vf18(RenderManager* view) {}                              // 0x80046214: inferred: second draw pass
    virtual void vf1C() {}                                                 // 0x800461FC
};

} // namespace atn

#endif
