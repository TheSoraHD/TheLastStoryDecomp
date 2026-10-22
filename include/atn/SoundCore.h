#ifndef ATN_SOUNDCORE_H
#define ATN_SOUNDCORE_H

namespace atn {

class Vector3;

// atn::SoundCore (RTTI name): sound played through a nw4r::snd::SoundHandle. The full class is in
// src/atn/Sound3dListener.cpp (the file that defines it); this header only declares the members
// called from other files, which still use their own partial views of the object (SoundObject,
// SoundObjectSe_unk) and cast. Names of the members are not known.
class SoundCore {
public:
    void fn_800C8780(const char* label);                     // inferred: starts the sound with that label
    void fn_800C8BA4(const char* label, const Vector3* pos); // inferred: starts it as a 3D sound at pos
    void fn_800C9010(unsigned int id);                       // inferred: starts the sound with that id
    void fn_800C93D4(const char* label);                     // inferred: prepares the sound with that label
    void fn_800C97F8(const char* label, const Vector3* pos); // inferred: prepares it as a 3D sound at pos
    void fn_800CA0AC(float volume, int frames);
    void fn_800CA2FC(float a, float b, float c);             // inferred: effect sends A/B/C
    void fn_800CA798(int enable, float value);
    void fn_800CA7D4(int offset);                            // inferred: start offset (clamped to >= 0)
    float fn_800CA844() const;                               // inferred: total volume
    int fn_800CA910();                                       // inferred: length in ms (-1: unknown)
    int fn_800CAA88();                                       // inferred: play position in ms (-1: unknown)
    void fn_800CAC48(int track, float volume, int frames);   // inferred: stream track volume
};

} // namespace atn

#endif
