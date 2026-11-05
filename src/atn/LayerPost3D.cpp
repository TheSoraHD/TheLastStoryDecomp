#include "atn/LayerPost3D.h"
#include "atn/RenderView_unk.h"

// inferred: unknown member function of the object pointed to by lbl_8087EFB4

namespace atn {

LayerPost3D::LayerPost3D() : Layer3D("3DPost") {}

void LayerPost3D::vf1C() {
    lbl_8087EFB4->fn_800C0508(0);
    Layer3D::vf1C();
}

LayerPost3D::~LayerPost3D() {}

} // namespace atn
