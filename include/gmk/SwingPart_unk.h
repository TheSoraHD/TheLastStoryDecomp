#ifndef GMK_SWINGPART_UNK_H
#define GMK_SWINGPART_UNK_H

#include "atn/LH_Binary.h"
#include "atn/EfpHandle_unk.h"
#include "gmk/Lemon.h"
#include "gmk/SwingObject.h"

// inferred: data of the building model of a swinging map part (views; names invented)
struct SwingPartData_unk {
    unsigned char m00[0x44];
    int mNumNodes; // 0x44
};
struct SwingPartInst_unk {
    unsigned char m00[0x4C];
    Mtx* mNodes; // 0x4C: inferred: local node matrices
    unsigned char m50[0x17C - 0x50];
    SwingPartData_unk* m17C;
};
// inferred: locator of the part (only the position is read)
struct SwingPartPos_unk {
    int m00;
    atn::Vector3 mPos; // 0x04
};

// inferred: swinging map part of a gimmick locator (MapPartsGimmickLoc): a building model instance and the
// swing state of gmk::SwingObject; 0x138 bytes (new at 0x80498A70). Class name unknown.
struct SwingPart_unk {
    SwingPart_unk(int id, void* arg, SwingPartPos_unk* pos, unsigned int flags); // 0x803FFD10
    int fn_803FFF40(); // inferred: loading (1 when ready)
    void fn_804003E0(); // inferred: update
    void fn_804006E0(); // inferred: draw

    atn::ModelInstance mInst; // 0x00 (a member: the constructor stores no vtable)
    atn::LH_Binary mLoader;   // 0x88: the .gmk file
    int m90;                  // 0x90: inferred: loading step
    SwingPartPos_unk* m94;
    Mtx* m98;                 // 0x98: inferred: world node matrices
    Mtx* m9C;                 // inferred: matrices of the three swing nodes
    Mtx* mA0;
    Mtx* mA4;
    gmk::SwingCore_unk mCore; // 0xA8
    atn::EfpFile mEfp;        // 0x128
    unsigned int m134;        // 0x134: inferred: flags (1, 2: no effect)
};

#endif
