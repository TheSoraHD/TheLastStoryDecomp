#ifndef GAME_REVOLUTION_OS_FASTCAST_H
#define GAME_REVOLUTION_OS_FASTCAST_H

// OSInitFastCast of the RVL SDK header revolution/os/OSFastCast.h (static inline with an asm body, as in
// the SDK: sets GQR2-5 for the u8/u16/s8/s16 fast casts). The game includes the SDK header; this copy
// exists because the game units do not have the SDK include path. Approved by the user (2026-10-21,
// game/main fn_8047A4C8, example 0x8047A4E4).
#ifdef __cplusplus
extern "C" {
#endif

static inline void OSInitFastCast(void) {
    asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3

        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3

        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3

        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    }
}

#ifdef __cplusplus
}
#endif

#endif
