#ifndef GAME_MAPPARTSMODEL_H
#define GAME_MAPPARTSMODEL_H

#include "atn/Csv.h"
#include "game/MapPartsBase.h"
#include "math/Vec.h"
#include "atn/Model_unk.h"

// inferred from __vt__13MapPartsModel (0x80790630)
class MapPartsModel : public MapPartsBase {
public:
    MapPartsModel(atn::Task* parent, void* arg, atn::CsvTab* csv); // 0x8049BAAC
    virtual ~MapPartsModel();          // 0x8049C350
    virtual int vf0C();                // 0x8049C3C0
    virtual void vf10();               // 0x8049C454
    virtual void vf14();               // 0x8049C458
    virtual bool vf20(const atn::Vector3& pos); // 0x8049C8A8
    virtual MapPartsBox_unk vf24(MapPartsTree_unk* tree); // 0x8049C688
    virtual void vf28(atn::CsvTab* csv);     // 0x8049BB4C
    virtual void vf2C(unsigned int hash, int draw); // 0x8049C7AC

    static MapPartsModel* fn_8049B9B8(atn::Task* parent, void* arg, atn::CsvTab* csv);

    atn::Model mModel; // 0x50
    int mHide;        // 0x420: inferred: hidden in the bird view ("HIDE_BIRDVIEW")
    float mHideY;     // 0x424: inferred: height above which it is hidden ("HIDE_BIRDVIEW_Y")
    int mCenterCamera; // 0x428: inferred: follows the camera ("CENTER_CAMERA")
    int mHasBox;      // 0x42C: inferred: drawn only with the camera inside mBox ("BOX")
    MapPartsBox_unk mBox; // 0x430
    int mDraw;        // 0x448: inferred: set by vf2C for the model name or "<all>"
// inferred: allocated with 0x450 bytes (fn_8049B9B8) while MapPartsOcean puts its first member at
// 0x44C (the compiler reuses the tail padding): the class is 8-byte aligned, probably through a
// member not identified yet
} __attribute__((aligned(8)));

#endif
