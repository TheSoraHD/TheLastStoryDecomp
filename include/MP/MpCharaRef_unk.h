#ifndef MP_MPCHARAREF_UNK_H
#define MP_MPCHARAREF_UNK_H

class CharaTask;

// inferred: holder of the character of a player (methods in MP/CharaListeners_unk.cpp; used by the
// player slots of atn/PacketPack); name invented
struct MpCharaRef_unk {
    void fn_805010C0();
    void fn_805011CC();
    void fn_80501288(CharaTask* chara);

    CharaTask* mChara;
};

#endif
