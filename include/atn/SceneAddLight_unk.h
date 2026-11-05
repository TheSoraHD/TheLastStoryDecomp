#ifndef ATN_SCENEADDLIGHT_UNK_H
#define ATN_SCENEADDLIGHT_UNK_H

#include "atn/Scene.h"
#include "atn/Light_unk.h"

// inferred: inline of atn::Scene (atn/Scene.h; weak copy 0x80185188, first emitted by game/GameConfig_unk.cpp).
// Separate header because it needs the complete atn::Light; used by GameConfig_unk and MapPartsArea::ParseScene
// (which inlines it)
// NONMATCHING: 95.3% - the growth path of std::vector::push_back (as in Event/Path.cpp)
inline void atn::Scene::AddLight_unk(Light light) {
    mLights.push_back(light);
}

#endif
