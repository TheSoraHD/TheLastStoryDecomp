#ifndef ATN_SCENECOLORMATRIX_UNK_H
#define ATN_SCENECOLORMATRIX_UNK_H

#include "atn/RenderSetting_unk.h"
#include "atn/Scene.h"

// Separate from atn/Scene.h because it needs the render settings (lbl_8087EFA8), which several units
// still declare with their own partial types.
extern "C" RenderSetting_unk* lbl_8087EFA8; // render settings (created by fn_800B95C4)

// inferred: weak copy 0x8047C7DC (Event/Manager.cpp; also inlined in MapTask::fn_804A0F80): the colour
// matrix of the scene when it has one, else the one of the render settings
inline ColorMatrixParam_unk* atn::Scene::fn_8047C7DC() {
    return mColorMatrix.mSet != 0 ? &mColorMatrix.mValue : &lbl_8087EFA8->m324;
}

#endif
