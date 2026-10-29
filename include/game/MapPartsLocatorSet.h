#ifndef GAME_MAPPARTSLOCATORSET_H
#define GAME_MAPPARTSLOCATORSET_H

#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/EffectModel_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Texture_unk.h"
#include "game/MapInstance_unk.h"
#include "game/MapPartsBase.h"
#include "gmk/SeTable.h"
#include <boost/scoped_ptr.hpp>

// inferred from __vt__18MapPartsLocatorSet (0x807905D8): set of building instances placed by the
// locators of a ".building" file, or one merged model ("merge_%s_occ.model"/"merge_%s_loc.model")
class MapPartsLocatorSet : public MapPartsBase {
public:
    MapPartsLocatorSet(atn::Task* parent, void* arg, atn::CsvTab* csv); // 0x8049A09C
    virtual ~MapPartsLocatorSet();                  // 0x8049A6B8
    virtual int vf0C();                             // 0x8049A7D8
    virtual void vf10();                            // 0x8049AA1C
    virtual void vf14();                            // 0x8049AAFC
    virtual bool vf20(const atn::Vector3& pos);     // 0x8049B980
    virtual MapPartsBox_unk vf24(MapPartsTree_unk* tree); // 0x8049AC18 (not decompiled)
    virtual void vf2C(unsigned int hash, int draw); // 0x8049B880

    void fn_8049B1F0();                 // inferred: creates the instances from the loaded file
    void fn_8049B72C(unsigned int flags); // inferred: sets the draw flags
    void fn_8049B780(void* lens);       // inferred: sets the lens of the instances
    void fn_8049B7CC(void* tree);       // inferred: picks the light of each instance

    atn::LH_Binary mLoader;               // 0x50: the ".building" file
    float mHideY;                         // 0x58: inferred: "HIDE_BIRDVIEW_Y"
    atn::FixedString_unk<0x40> mName;     // 0x5C: inferred: path of the file
    atn::FixedString_unk<0x40> mTexName;  // 0x9C: inferred: light map ("LMAP")
    atn::Texture_unk mTexture;            // 0xDC
    unsigned int mFlags;                  // 0x10C: inferred: model flags (REFLECT, NO_SHADOW...)
    EffectModel_unk mModel;               // 0x110: inferred: "merge_%s_occ.model"
    boost::scoped_ptr<EffectModel_unk> mMerged; // 0x324: inferred: "merge_%s_loc.model"
    MapPartsBox_unk mBox;                 // 0x328
    int mLoaded : 1;                      // 0x340 bit 31
    int mLinked : 1;                      // inferred: light map given to the instances
    int mHide : 1;                        // inferred: hidden in the bird view
    int mDraw : 1;                        // inferred: set by vf2C
    int mHasModel : 1;                    // inferred: mModel was loaded
    int mHasMerged : 1;                   // inferred: mMerged was loaded
    atn::SimpleArray<MapInstance_unk*> mInstances; // 0x344
// inferred: allocated with 0x350 bytes: 8-byte aligned (as MapPartsModel)
} __attribute__((aligned(8)));

#endif
