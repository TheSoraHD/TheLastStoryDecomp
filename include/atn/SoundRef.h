#ifndef ATN_SOUNDREF_H
#define ATN_SOUNDREF_H

namespace atn {

// inferred: object holding a sound (its 0x04 field is passed to fn_80709AD0, which looks like
// a nw4r::snd BasicSound stop call) with an intrusive reference count at 0xA4.
struct SoundObject {
    void* m00;
    void* mSound; // 0x04
    char pad[0xA4 - 0x08];
    int mRefCount; // 0xA4
};

// inferred name: intrusive reference-counted pointer to a SoundObject. Out-of-line functions
// at 0x800CB360-0x800CB4A0.
class SoundRef {
public:
    SoundRef();                          // 0x800CB360
    SoundRef(const SoundRef& other);     // 0x800CB36C
    ~SoundRef();                         // 0x800CB3A0
    SoundRef& operator=(SoundObject* p); // 0x800CB404
    SoundRef& operator=(const SoundRef& other); // 0x800CB440
    void Reset();                        // 0x800CB480

    SoundObject* mPtr;
};

} // namespace atn

#endif
