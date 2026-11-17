#ifndef ATN_SCENETRACK_UNK_H
#define ATN_SCENETRACK_UNK_H

// inferred: animation track of the scene and binary files (12 bytes; atn/LH_Binary.cpp, atn/LH_Scene.cpp,
// atn/Light_unk.cpp, atn/Motion_unk.cpp); evaluates the value at `frame`. `key` caches the index of the
// last key found (the evaluation updates it). Fields from SceneTrack_unk::fn_8047202C (0x8047202C).
struct SceneTrack_unk {
    float fn_8047202C(const short& key, float frame);
    unsigned short m00;
    unsigned short mType;  // 0x02: 0 constant float, 3 linear keys, 4 one value per frame, else hermite keys
    unsigned short mCount; // 0x04: number of keys
    unsigned short mScale; // 0x06: fraction bits of the s16 values
    void* mKeys;           // 0x08
};

#endif
