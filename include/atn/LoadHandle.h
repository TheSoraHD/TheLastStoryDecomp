#ifndef ATN_LOADHANDLE_H
#define ATN_LOADHANDLE_H

namespace atn {

// inferred from __vt__Q23atn10LoadHandle: base of the atn::LH_* resource loaders.
class LoadHandle {
public:
    LoadHandle();          // 0x80473E74
    virtual ~LoadHandle(); // 0x80473E8C
    // inferred: releases the current core and references the other one (0x804741C0, called by
    // the copy of atn::Texture_unk in game/NewCampPartsGmk.cpp 0x80514218)
    LoadHandle& operator=(const LoadHandle& o);
    virtual void vf0C(const char* path) = 0; // inferred: start loading `path`

    int fn_80473F50(); // inferred: nonzero while busy/not loaded
    void fn_80473F88(); // inferred: releases mCore
    int fn_80473EFC();  // inferred: load state (3 = loaded)

    void* mCore; // 0x04: inferred: atn::LoadCore* of the pending/loaded resource
};

} // namespace atn

#endif
