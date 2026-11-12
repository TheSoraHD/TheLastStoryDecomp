#ifndef ATN_EFFECTFILEENTRY_UNK_H
#define ATN_EFFECTFILEENTRY_UNK_H

// inferred: texture rectangle: u, v, width and height (names invented)
struct EffectUV_unk {
    EffectUV_unk() {}
    EffectUV_unk(float u_, float v_, float w_, float h_) : u(u_), v(v_), w(w_), h(h_) {}
    float u;
    float v;
    float w;
    float h;
};

// inferred: entry of the effect data loaded by atn::LoadEffect (atn/LH_Effect.cpp; 0x138 bytes)
struct EffectFileEntry_unk {
    char mName[0x80];    // 0x00
    char mTexture[0x84]; // 0x80
    unsigned int mType;  // 0x104: 1 = model, 2 = texture pair, otherwise a texture
    int m108;
    float m10C;          // 0x10C: inferred: m108 * mUV.w (fn_8022D754)
    unsigned char m110[0x118 - 0x110];
    EffectUV_unk mUV;    // 0x118: inferred: first frame (fn_8022D790)
    int m128;
    int m12C;            // 0x12C: inferred: frame curve (1/3: cosine, 2: triangle, 4: random, 6: reverse)
    float m130;          // 0x130: inferred: loops before it stays on the last frame (0: forever)
    float m134;          // 0x134: inferred: size (fn_80230F3C)
};
extern "C" void fn_8022D754(EffectFileEntry_unk* entry); // inferred: prepares an entry

#endif
