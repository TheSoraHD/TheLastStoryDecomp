#ifndef MP_MPSESSION_UNK_H
#define MP_MPSESSION_UNK_H

// inferred: multiplayer session manager (lbl_8087F610; see MP/CharaListeners_unk.cpp and
// MP/D2AMpMenuCockpit.cpp); only the members used here are known
struct MpSession_unk {
    unsigned char m000[0x4FC];
    int m4FC; // inferred: state
    unsigned char m500[0x50C - 0x500];
    int m50C;

    int IsState30_unk() { return m4FC == 30; } // inferred: name invented
    // inferred: state 30 before the step 5 (RecycleBuffer_gmk/ItemSlotAnime fn_8044F96C); name invented
    bool IsStarting_unk() { return m4FC == 30 && m50C < 5; }
};
extern "C" MpSession_unk* lbl_8087F610;

#endif
