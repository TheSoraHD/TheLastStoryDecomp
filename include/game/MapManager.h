#ifndef GAME_MAPMANAGER_H
#define GAME_MAPMANAGER_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "atn/Sphere.h"
#include "std/vector.h"
#include <boost/function.hpp>
#include "atn/RenderSetting_unk.h"
#include "atn/Color4f_unk.h"
#include "game/MapArea.h"

class MapTask;
class MapPartsArea;

// inferred: color returned by the material of a model (AreaEffConfig::fn_80043014)
struct MapColor_unk {
    MapColor_unk() {}
    MapColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    MapColor_unk(const MapColor_unk& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    float r, g, b, a;
};
// inferred: camera callback: the map model hit by a sphere
typedef boost::function<atn::Scene*(atn::Sphere, void*)> MapHitFunc_unk;
typedef boost::function<void(MapArea*, void*)> MapModelFunc_unk;

// inferred: areas registered without a map (AddArea/DeleteArea(MapPartsArea*) in the RSO): a
// std::vector<MapPartsArea*> (DWARF) whose destructor (inlined in 0x80491324) tests the address four
// times, one more than our std::vector<T*>: kept as a derived class (name invented)
struct MapAreaList_unk : public std::vector<MapPartsArea*> {};

// inferred from __vt__10MapManager (0x807901E0): manager of the map parts (singleton)
class MapManager : public atn::Task {
public:
    MapManager(atn::Task* parent);
    static MapManager* SetTask(atn::Task* parent);
    virtual ~MapManager();
    virtual int vf0C();
    void fn_80491418(MapTask* map);  // inferred: adds a map (called by the MapTask constructors)
    void fn_80491440(MapTask* map);  // inferred: removes a map
    void fn_80491528(MapModelFunc_unk func, void* arg);
    MapArea* FindArea(atn::Sphere sphere);
    MapColor_unk fn_80491D50(atn::Sphere sphere);
    MapColor_unk fn_80491DF8(atn::Sphere sphere);
    // inferred: name invented (the callback copies the sphere twice, 0x804914BC)
    MapArea* Find_unk(atn::Sphere sphere) { return FindArea(sphere); }

    static MapManager* instance_; // 0x8087F558
    // inferred: weak getter (called with bl by Event::Manager, e.g. 0x80487510); name invented
    static MapManager* GetInstance_unk() { return instance_; }

    atn::FixedArray_unk<MapTask*, 64> mMaps; // 0x48: atn::static_vector<MapTask*, 64> (DWARF)
    int mType;                               // 0x14C: inferred: only maps of this type (0: all)
    MapAreaList_unk mAreas;                  // 0x150
    int m15C;
};

#endif
