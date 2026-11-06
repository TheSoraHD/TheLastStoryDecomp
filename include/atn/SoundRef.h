#ifndef ATN_SOUNDREF_H
#define ATN_SOUNDREF_H

namespace nw4r {
namespace snd {
namespace detail {
class BasicSound;
} // namespace detail
} // namespace snd
} // namespace nw4r

namespace atn {

struct SoundCategory_unk;

// inferred: object holding a sound (its 0x04 field is stopped with
// nw4r::snd::detail::BasicSound::Stop) with an intrusive reference count at 0xA4.
struct SoundObject {
    void* m00;
    nw4r::snd::detail::BasicSound* mSound; // 0x04
    char pad[0x90 - 0x08];
    int m90;       // inferred: zero when the sound must still be prepared (atn::Bgm::fn_8004AE84)
    int m94;
    int m98;       // inferred: sound id, read by fn_800CB6F8
    unsigned int m9C; // inferred: sound state flags, tested by fn_800CB4A4
    int mA0;       // inferred: nonzero while playing, tested by fn_800CB4C4
    int mRefCount; // 0xA4
    SoundCategory_unk* mA8; // 0xA8: player parameters (SoundManager::fn_800CFDA0)
};

// inferred name: intrusive reference-counted pointer to a SoundObject. Out-of-line functions
// at 0x800CB360-0x800CB4A0.
class SoundRef {
public:
    SoundRef();                          // 0x800CB360
    SoundRef(const SoundRef& other);     // 0x800CB36C
    ~SoundRef();                         // 0x800CB3A0
    // inferred: both return void (r3 is reused as a scratch register before blr)
    void operator=(SoundObject* p); // 0x800CB404
    void operator=(const SoundRef& other); // 0x800CB440
    void Reset();                        // 0x800CB480

    SoundObject* mPtr;
};

} // namespace atn

#endif
