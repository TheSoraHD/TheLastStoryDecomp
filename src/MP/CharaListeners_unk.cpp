// inferred file name (splits-auto): helpers of atn/PacketPack (MP::ManagerMP) for the character
// of a remote player; the class name is invented.
#include "game/CharaTask.h"

struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048; // inferred: bullet manager (see gmk/Coffin.cpp)
// inferred: effect/bullet of `chara` along `dir` (also called by chr::ControlDamage, CharaStatus)
extern "C" void fn_800FDE60(BulletMgr_unk* mgr, int arg, CharaTask* chara, const atn::Vector3* dir);

// inferred: multiplayer session manager (lbl_8087F610, see MP/D2AMpMenuCockpit.cpp)
struct MpSession_unk {
    unsigned char m000[0x4FC];
    int m4FC; // inferred: state
    unsigned char m500[0x50C - 0x500];
    int m50C;

    int IsState30_unk() { return m4FC == 30; } // inferred: name invented
};
extern "C" MpSession_unk* lbl_8087F610;

// inferred: listeners (see gmk/Item.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

// inferred: holder of the character of a player
struct MpCharaRef_unk {
    void fn_805010C0();
    void fn_805011CC();
    void fn_80501288(CharaTask* chara);

    CharaTask* mChara;
};

void MpCharaRef_unk::fn_805010C0() {
    if (mChara != 0) {
        fn_8014F8F4()->mEnabled = 0;
        if (lbl_8087F048 != 0) {
            atn::Vector3 dir(0.0f, 1.0f, 0.0f);
            fn_800FDE60(lbl_8087F048, 0, mChara, &dir);
        }
        fn_8014F8F4()->mEnabled = 1;
    }
}

void MpCharaRef_unk::fn_805011CC() {
    if (mChara != 0) {
        mChara->mStatus.fn_8012D8B8();
        mChara->fn_800D246C(0);
        mChara->m58C = 0;
        mChara->fn_801446F0();
        mChara->mColli.mFlags |= 1;
        if ((mChara->mFlags & 4) == 4) {
            mChara->fn_80176ACC();
            mChara->fn_800D246C(0);
            mChara->mFlag15 = 0;
            mChara->mFlag25 = 0;
            mChara->fn_8016E4C4(1);
        }
    }
}

void MpCharaRef_unk::fn_80501288(CharaTask* chara) {
    if (lbl_8087F610 != 0 && lbl_8087F610->IsState30_unk() == 1 && lbl_8087F610->m50C >= 3 &&
        lbl_8087F610->m50C <= 4) {
        return;
    }
    mChara = chara;
}
