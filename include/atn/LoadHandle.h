#ifndef ATN_LOADHANDLE_H
#define ATN_LOADHANDLE_H

namespace atn {

// inferred from __vt__Q23atn10LoadHandle: base of the atn::LH_* resource loaders.
class LoadHandle {
public:
    LoadHandle();          // 0x80473E74
    virtual ~LoadHandle(); // 0x80473E8C
    virtual void vf0C(const char* path) = 0; // inferred: start loading `path`

    int fn_80473F50(); // inferred: nonzero while busy/not loaded

    void* m04;
    void* mCallback; // 0x08: set by derived loaders
};

} // namespace atn

#endif
