#include "atn/LayerPost3D.h"
#include "atn/RenderManager.h"

// inferred: unknown member function of the object pointed to by atn::RenderManager::instance_

namespace atn {

LayerPost3D::LayerPost3D() : Layer3D("3DPost") {}

void LayerPost3D::vf1C() {
    atn::RenderManager::instance_->fn_800C0508(0);
    Layer3D::vf1C();
}

LayerPost3D::~LayerPost3D() {}

} // namespace atn
