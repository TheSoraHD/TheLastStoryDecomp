#ifndef GAME_MAPTASK_H
#define GAME_MAPTASK_H

#include "atn/AABB_unk.h"
#include "atn/AABBTree.h"
#include "atn/FixedArray.h"
#include "atn/LH_Binary.h"
#include "atn/LH_Xsca.h"
#include "atn/Task.h"
#include "game/ResHandle.h"
#include "atn/ResRef_unk.h"
#include "std/string.h"

#define MAPTASK_ASSERT_UNK(x) ((void)0)

class MapPartsArea;
struct MapId_unk; // db/MapIdTable_unk.h
class MapPartsBase;
class MapInstance_unk;
namespace gs {
struct CameraBox;
}
namespace atn {
class CsvTab;
}

// Map of the field (vtable 0x80790740, 0x11C bytes): the map parts of a map file (areas, models,
// locators...), created by SetTask (fn_8049D68C) or fn_8049D704 and registered in MapManager.
// Real name and accessors from the RSO (GetMapType/GetMapId/GetMapSubId__7MapTaskCFv,
// SetTask__7MapTaskFPQ23atn4TaskPCc, GetMapNameFromId__7MapTaskFPciii, DWARF
// atn::static_vector<P12MapPartsArea,8>). Returned by GameTask::GetCurrentMap (fn_80373148).
// Member names invented unless noted.
class MapTask : public atn::Task {
public:
    MapTask(atn::Task* parent, const char* path);             // 0x8049D7E4
    MapTask(atn::Task* parent, int type, int id, int subId); // 0x8049D974
    virtual ~MapTask();         // 0x8049DDD8
    virtual int vf0C();         // 0x8049DEE4
    virtual void vf10();        // 0x8049EBAC
    virtual void vf14();        // 0x8049ED6C

    // inferred: name of the map file of an id (0x8049D52C; RSO name)
    static int GetMapNameFromId(char* name, int type, int id, int subId);
    // inferred: name of a map (0x8049D5DC, same code with other folders)
    static int fn_8049D5DC(char* name, int type, int id, int subId);
    // inferred: maps drawn without depth shadows (0x8049D2E4)
    static bool fn_8049D2E4(int type, int id, int subId);
    // inferred: creates the map of a file (0x8049D68C; SetTask__7MapTaskFPQ23atn4TaskPCc in the RSO)
    static MapTask* SetTask(atn::Task* parent, const char* path);
    // inferred: creates the map of an id (0x8049D704)
    static MapTask* fn_8049D704(atn::Task* parent, int type, int id, int subId);
    void fn_8049E480(); // inferred: applies the current area list (0x8049E480)
    void fn_8049ED90(); // inferred: resets the render settings and creates the parts of the map file (0x8049ED90)
    void CreateParts(atn::CsvTab& csv, int replace); // 0x8049F4F4 (RSO name)
    // inferred: inlined in CreateParts (names invented)
    void AddPart_unk(MapPartsBase* part);
    void RemovePart_unk(MapPartsBase* part);
    void fn_804A04AC(int camera, int sub, int mode, gs::CameraBox* box); // inferred: sets the camera of the map
    void fn_804A0580(int arg, const atn::Vector3* pos);           // inferred
    MapInstance_unk* fn_804A0614(const std::string& name);         // inferred: instance of the locator sets
    MapPartsBase* fn_804A06B4(int type, const char* name); // inferred: part of a type with this file name
    MapPartsBase* fn_804A0BCC(int type, unsigned int hash); // inferred: part of a type whose file name has this hash
    void fn_804A0E18(const char* name, int draw);   // inferred: MapPartsBase::vf2C on all the parts
    void fn_804A0E90(unsigned int hash, int draw);  // inferred: MapPartsBase::vf2C on all the parts
    void fn_804A0EFC(MapPartsArea* area);                // inferred: switches to an area list
    void fn_804A0F58(int index, float t, float speed);    // inferred: switches to an area list with a blend
    void fn_804A0F80();                                  // inferred: transition between the area lists
    void fn_804A1C4C(); // inferred: refreshes the map after a load
    void fn_804A1D2C(); // inferred: stores the state of the map

    // RSO names; weak copies 0x803EFD9C, 0x803EFDA4 and 0x803EFDAC (game/GimmickStateCommand.cpp, also
    // called by gmk/Cannonball and MapPartsGimmickLoc). GetMapType carries the empty asserts (debug build
    // only) that put GimmickCommonB0_unk::fn_803EF780 over the inlining budget (docs/inlining.md R3), so
    // that it calls the three accessors as the original does.
    int GetMapType() const {
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0); MAPTASK_ASSERT_UNK(this != 0);
        return mType;
    }
    int GetMapId() const { return mId; }
    int GetMapSubId() const { return mSubId; }
    // inferred: weak copies 0x80339F7C and 0x80339F84 (game/EnemyQuarkSecond.cpp); names invented
    int GetCamera_unk() { return mCamera; }
    void SetDefaultCamera_unk(int camera) { mDefaultCamera = camera; }

    // inferred: the loader at 0x58 (names invented)
    atn::LoadHandle* GetLoader_unk() { return &mFile; }
    bool IsAreaIndex_unk(int i) const { return i >= 0 && i < (int)mAreas.mSize; }
    // inferred: current area list (the one replacing the current, else the current one)
    MapPartsArea* GetArea_unk() {
        if (mNextArea != 0 && mAreaIndex != m10C) {
            return mNextArea;
        }
        if (IsAreaIndex_unk(mAreaIndex)) {
            return mAreas.mData[mAreaIndex];
        }
        return 0;
    }

    int mType;                                // 0x48: GetMapType (0: field, 1: town, 6: ...)
    int mId;                                  // 0x4C: GetMapId
    int mSubId;                               // 0x50: GetMapSubId
    MapId_unk* m54;                           // 0x54: inferred: entry of the map id table (fn_8021771C)
    atn::LH_Binary mFile;                     // 0x58: inferred: csv of the parts
    atn::LH_Binary m60;                       // 0x60
    atn::LH_Xsca m68;                         // 0x68
    MapPartsBase* mParts;                     // 0x70: inferred: first part (list through MapPartsBase::mNext)
    unsigned int m74;                         // 0x74: inferred: 0xDEADBEEF once destroyed
    float m78;
    float m7C;
    float m80;
    float m84;
    atn::AABB_unk mBox;                       // 0x88: inferred: bounds of the parts (drawn by vf14)
    atn::FixedArray_unk<MapPartsArea*, 8> mAreas; // 0xA0: atn::static_vector<MapPartsArea*, 8> (DWARF)
    MapPartsArea* mNextArea;                  // 0xC4: inferred: area list replacing the current one
    ResHandle mC8;                            // 0xC8: inferred: resource released by the destructor
    int mCC;
    atn::AABBTree mTree;                      // 0xD0
    int mCamera;                              // 0xE0: inferred: current camera (fn_804A04AC)
    int mE4;
    int mDefaultCamera;                       // 0xE8: inferred: default camera
    int mEC;
    float mF0;
    int mF4;                                  // 0xF4: inferred: set by the gimmicks that load a map (OnOff...)
    int mF8;                                  // 0xF8: inferred: nonzero when hidden (TownTask sets 1)
    ResRef_unk mFC;                           // 0xFC: inferred: archive reference (destructor)
    int m100;
    int m104;                                 // 0x104: inferred: light mode (0-3, Event::Manager fn_80482ED8)
    int mAreaIndex;                           // 0x108: inferred: current area list index
    int m10C;
    float m110;
    float m114;
    int m118;
// inferred: allocated with 0x120 bytes (fn_8049D704) while TownTask puts its first member at 0x11C (the
// compiler reuses the tail padding): 8-byte aligned like MapPartsModel (probably through atn::Task)
} __attribute__((aligned(8)));

#endif
