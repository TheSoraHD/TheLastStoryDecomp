// inferred file name (splits-auto): creates the map parts listed in the map csv by type name.
#include "game/MapPartsColliTree.h"
#include "game/MapPartsEfp.h"
#include "game/MapPartsFlare.h"
#include "game/MapPartsModel.h"
#include "game/MapPartsOcean.h"
#include "game/MapPartsArea.h"
#include "game/MapPartsGimmickLoc_unk.h"
#include "game/MapPartsLocatorSet.h"

extern "C" int strcmp(const char* a, const char* b);


// inferred: type of a map part from its csv name (-1 if unknown)
extern "C" int fn_80496A28(const char* name) {
    if (strcmp(name, "MODEL") == 0) {
        return 0;
    }
    if (strcmp(name, "COLLI_TREE") == 0) {
        return 2;
    }
    if (strcmp(name, "EFP") == 0) {
        return 3;
    }
    if (strcmp(name, "LOCATORS") == 0) {
        return 4;
    }
    if (strcmp(name, "GIMMICK_LOC") == 0) {
        return 5;
    }
    if (strcmp(name, "AREAFILE") == 0) {
        return 6;
    }
    if (strcmp(name, "LENSFLARE") == 0) {
        return 7;
    }
    if (strcmp(name, "OCEAN") != 0) {
        return -1;
    }
    return 1;
}

// inferred: creates the part of type `name`; the type is passed as the creator argument
extern "C" MapPartsBase* fn_80496B3C(atn::Task* parent, const char* name, atn::CsvTab* csv) {
    switch (fn_80496A28(name)) {
    case 0:
        return MapPartsModel::fn_8049B9B8(parent, 0, csv);
    case 2:
        return MapPartsColliTree::fn_80496420(parent, 2, csv);
    case 3:
        return MapPartsEfp::fn_804965EC(parent, 3, csv);
    case 4:
        return MapPartsLocatorSet::SetTask(parent, 4, *csv);
    case 5:
        return MapPartsGimmickLoc::SetTask(parent, 5, *csv);
    case 6:
        return MapPartsArea::SetTask(parent, 6, *csv);
    case 7:
        return MapPartsFlare::fn_80496C3C(parent, 7, csv);
    case 1:
        return MapPartsOcean::fn_8049C8DC(parent, 0, csv);
    }
    return 0;
}
