#ifndef ATN_EFFECTFILEENTRY_UNK_H
#define ATN_EFFECTFILEENTRY_UNK_H

// inferred: entry of the effect data loaded by atn::LoadEffect (atn/LH_Effect.cpp; 0x138 bytes)
struct EffectFileEntry_unk {
    char mName[0x80];    // 0x00
    char mTexture[0x84]; // 0x80
    unsigned int mType;  // 0x104: 1 = model, 2 = texture pair, otherwise a texture
    int m108;
    float m10C;          // 0x10C: inferred: m108 * m120 (fn_8022D754)
    unsigned char m110[0x120 - 0x110];
    float m120;
    unsigned char m124[0x138 - 0x124];
};
extern "C" void fn_8022D754(EffectFileEntry_unk* entry); // inferred: prepares an entry

#endif
