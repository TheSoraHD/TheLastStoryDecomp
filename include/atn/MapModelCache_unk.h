#ifndef ATN_MAPMODELCACHE_UNK_H
#define ATN_MAPMODELCACHE_UNK_H

// inferred: map models shared by name (atn/Motion_unk.cpp: fn_8009F788 creates them, fn_8009FFCC
// tells whether one is still loading). atn::ModelInstance and game/MapPartsLocatorSet keep pointers to them.
#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "std/string.h"

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" std::basic_string<char> fn_8006B174(const std::basic_string<char>& path);

// inferred: map model loaded from a csv list (0x22C bytes; created by fn_8009F788). Names invented
// (not the MapArea of game/MapArea.h).
struct MapModelCache_unk {
    MapModelCache_unk(const char* path) : m08(0), m0C(0), mModel(9), m228(0) {
        std::basic_string<char> name = fn_8006B174(std::basic_string<char>(path));
        mFile.vf0C(name.c_str());
        m224 = fn_800DC6B4(name.c_str());
        mModel.GetModel_unk()->m98 = 0.00001f * (unsigned short)m224;
    }

    atn::LH_Binary mFile;   // 0x00: inferred: the csv list
    int m08;                // inferred: the csv was read
    int m0C;                // inferred: the model is loaded
    atn::ModelBase mModel; // 0x10
    unsigned int m224;      // inferred: hash of the file name
    int m228;               // inferred: reference count
};

#endif
