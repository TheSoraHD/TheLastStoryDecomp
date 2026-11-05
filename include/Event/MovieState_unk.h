#ifndef EVENT_MOVIESTATE_UNK_H
#define EVENT_MOVIESTATE_UNK_H

namespace Event {

// inferred: movie player (only the virtual and the members used here)
struct MovieState_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C(int frames, float volume); // inferred: starts the playback (fade in)
    int m04;
    int m08; // inferred: 2 when finished, 4 paused
    int m0C; // inferred: loop
};

} // namespace Event

#endif
