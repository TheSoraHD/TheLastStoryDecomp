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
    void operator=(const LoadHandle& o); // inferred void: 0x804741C0 does not return this
    virtual void vf0C(const char* path) = 0; // inferred: start loading `path`

    int fn_80473F50(); // inferred: nonzero while busy/not loaded
    void fn_80473F88(); // inferred: releases mCore
    int fn_80473EFC();  // inferred: load state (3 = loaded)
    const char* fn_80473F18(); // inferred: path of the resource
    unsigned int fn_80473F34(); // inferred: path hash of the resource
    void fn_80473FCC(const char* path); // inferred: loads `path` and waits for it

    void* mCore; // 0x04: inferred: atn::LoadCore* of the pending/loaded resource
};

} // namespace atn

#endif
