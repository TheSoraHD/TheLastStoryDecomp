#include "atn/LayerPost3D.h"

// inferred: unknown member function of the object pointed to by lbl_8087EFB4
extern "C" void fn_800C0508(void* obj, int arg);
extern "C" void* lbl_8087EFB4;

namespace atn {

LayerPost3D::~LayerPost3D() {}

void LayerPost3D::vf1C() {
    fn_800C0508(lbl_8087EFB4, 0);
    Layer3D::vf1C();
}

LayerPost3D::LayerPost3D() : Layer3D("3DPost") {}

} // namespace atn
