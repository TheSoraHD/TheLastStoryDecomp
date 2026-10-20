// inferred: YUV->RGB drawing of THP frames, adapted from THPDraw.c of the THP SDK sample player
// (THPGXRestore 0x805B4DF0, THPGXYuv2RgbSetup 0x805B4F08, THPGXYuv2RgbDraw 0x805B53CC); the texture
// loads go through the engine renderer (fn_800763FC) instead of GXLoadTexObj. Name invented.

struct GXColor {
    unsigned char r, g, b, a;
};
struct GXColorS10 {
    short r, g, b, a;
};
struct GXTexObj {
    unsigned int dummy[8];
};
struct GXRenderModeObj {
    int viTVmode;
    unsigned short fbWidth;   // 0x04
    unsigned short efbHeight; // 0x06
};
union GXWGFifo_unk {
    unsigned char u8;
    short s16;
    unsigned short u16;
    unsigned int u32;
    float f32;
};
#define GXWGFifo (*(volatile GXWGFifo_unk*)0xCC008000)

extern "C" {
void GXSetZMode(unsigned char compareEnable, int func, unsigned char updateEnable);
void GXSetBlendMode(int type, int src, int dst, int op);
void GXSetNumTexGens(unsigned char n);
void GXSetNumChans(unsigned char n);
void GXSetNumTevStages(unsigned char n);
void GXSetTevOrder(int stage, int coord, int map, int color);
void GXSetTevOp(int stage, int mode);
void GXSetTevSwapMode(int stage, int ras, int tex);
void GXSetTevSwapModeTable(int table, int r, int g, int b, int a);
void GXSetPixelFmt(int pixFmt, int zFmt);
void C_MTXOrtho(float m[4][4], float t, float b, float l, float r, float n, float f);
void GXSetProjection(float m[4][4], int type);
void GXSetViewport(float left, float top, float width, float height, float nearZ, float farZ);
void GXSetScissor(unsigned int left, unsigned int top, unsigned int width, unsigned int height);
void PSMTXIdentity(float m[3][4]);
void GXLoadPosMtxImm(float m[3][4], unsigned int id);
void GXSetCurrentMtx(unsigned int id);
void GXSetColorUpdate(unsigned char enable);
void GXSetAlphaUpdate(unsigned char enable);
void GXSetDispCopyGamma(int gamma);
void GXSetTexCoordGen2(int dst, int func, int src, unsigned int mtx, unsigned char normalize, unsigned int postMtx);
void GXInvalidateTexAll();
void GXClearVtxDesc();
void GXSetVtxDesc(int attr, int type);
void GXSetVtxAttrFmt(int fmt, int attr, int cnt, int type, unsigned char frac);
void GXSetTevColorIn(int stage, int a, int b, int c, int d);
void GXSetTevColorOp(int stage, int op, int bias, int scale, unsigned char clamp, int out);
void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int out);
void GXSetTevKColorSel(int stage, int sel);
void GXSetTevKAlphaSel(int stage, int sel);
void GXSetTevColorS10(int id, GXColorS10 color);
void GXSetTevKColor(int id, GXColor color);
void GXInitTexObj(GXTexObj* obj, void* image, unsigned short width, unsigned short height, int format, int wrapS,
                  int wrapT, unsigned char mipmap);
void GXInitTexObjLOD(GXTexObj* obj, int minFilt, int magFilt, float minLod, float maxLod, float lodBias,
                     unsigned char biasClamp, unsigned char doEdgeLod, int maxAniso);
void GXBegin(int type, int fmt, unsigned short n);
}

// inferred: engine renderer (global 0x8087EEE0); fn_800763FC loads a texture object into a map
extern "C" void* lbl_8087EEE0;
extern "C" void fn_800763FC(void* renderer, GXTexObj* obj, int map);

extern "C" void fn_805B4DF0() {
    GXSetZMode(1, 7, 0);
    GXSetBlendMode(0, 1, 0, 0xF);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevSwapMode(3, 0, 0);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 0, 0, 3);
    GXSetTevSwapModeTable(2, 1, 1, 1, 3);
    GXSetTevSwapModeTable(3, 2, 2, 2, 3);
}

extern "C" void fn_805B4F08(GXRenderModeObj* rmode) {
    int scrWidth;
    int scrHeight;
    float pMtx[4][4];
    float mMtx[3][4];

    scrWidth = rmode->fbWidth;
    scrHeight = rmode->efbHeight;

    GXSetPixelFmt(0, 0);
    C_MTXOrtho(pMtx, 0.0f, (float)scrHeight, 0.0f, (float)scrWidth, 0.0f, -1.0f);
    GXSetProjection(pMtx, 1);
    GXSetViewport(0.0f, 0.0f, (float)scrWidth, (float)scrHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, scrWidth, scrHeight);
    PSMTXIdentity(mMtx);
    GXLoadPosMtxImm(mMtx, 0);
    GXSetCurrentMtx(0);
    GXSetZMode(1, 7, 0);
    GXSetBlendMode(0, 1, 0, 0);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(0);
    GXSetDispCopyGamma(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    GXInvalidateTexAll();
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(7, 9, 1, 3, 0);
    GXSetVtxAttrFmt(7, 0xD, 1, 2, 0);

    GXSetNumTevStages(4);

    GXSetTevOrder(0, 1, 1, 0xFF);
    GXSetTevColorIn(0, 0xF, 8, 0xE, 2);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaIn(0, 7, 4, 6, 1);
    GXSetTevAlphaOp(0, 1, 0, 0, 0, 0);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevKAlphaSel(0, 0x1C);
    GXSetTevSwapMode(0, 0, 0);

    GXSetTevOrder(1, 1, 2, 0xFF);
    GXSetTevColorIn(1, 0xF, 8, 0xE, 0);
    GXSetTevColorOp(1, 0, 0, 1, 0, 0);
    GXSetTevAlphaIn(1, 7, 4, 6, 0);
    GXSetTevAlphaOp(1, 1, 0, 0, 0, 0);
    GXSetTevKColorSel(1, 0xD);
    GXSetTevKAlphaSel(1, 0x1D);
    GXSetTevSwapMode(1, 0, 0);

    GXSetTevOrder(2, 0, 0, 0xFF);
    GXSetTevColorIn(2, 0xF, 8, 0xC, 0);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(2, 4, 7, 7, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevSwapMode(2, 0, 0);

    GXSetTevOrder(3, 0xFF, 0xFF, 0xFF);
    GXSetTevColorIn(3, 1, 0, 0xE, 0xF);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(3, 7, 7, 7, 7);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevSwapMode(3, 0, 0);
    GXSetTevKColorSel(3, 0xE);

    const GXColorS10 tevS10Reg0 = {-90, 0, -114, 135};
    GXSetTevColorS10(1, tevS10Reg0);
    const GXColor tevKColor0 = {0x00, 0x00, 0xE2, 0x58};
    GXSetTevKColor(0, tevKColor0);
    const GXColor tevKColor1 = {0xB3, 0x00, 0x00, 0xB6};
    GXSetTevKColor(1, tevKColor1);
    const GXColor tevKColor2 = {0xFF, 0x00, 0xFF, 0x80};
    GXSetTevKColor(2, tevKColor2);

    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
}

extern "C" void fn_805B53CC(void* yData, void* uData, void* vData, short x, short y, short textureWidth,
                            short textureHeight, short polygonWidth, short polygonHeight) {
    GXTexObj yObj;
    GXTexObj uObj;
    GXTexObj vObj;

    GXInitTexObj(&yObj, yData, textureWidth, textureHeight, 1, 0, 0, 0);
    GXInitTexObjLOD(&yObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    fn_800763FC(lbl_8087EEE0, &yObj, 0);
    GXInitTexObj(&uObj, uData, textureWidth >> 1, textureHeight >> 1, 1, 0, 0, 0);
    GXInitTexObjLOD(&uObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    fn_800763FC(lbl_8087EEE0, &uObj, 1);
    GXInitTexObj(&vObj, vData, textureWidth >> 1, textureHeight >> 1, 1, 0, 0, 0);
    GXInitTexObjLOD(&vObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    fn_800763FC(lbl_8087EEE0, &vObj, 2);

    GXBegin(0x80, 7, 4);
    GXWGFifo.s16 = x;
    GXWGFifo.s16 = y;
    GXWGFifo.s16 = 0;
    GXWGFifo.s16 = 0;
    GXWGFifo.s16 = 0;

    GXWGFifo.s16 = x + polygonWidth;
    GXWGFifo.s16 = y;
    GXWGFifo.s16 = 0;
    GXWGFifo.s16 = 1;
    GXWGFifo.s16 = 0;

    GXWGFifo.s16 = x + polygonWidth;
    GXWGFifo.s16 = y + polygonHeight;
    GXWGFifo.s16 = 0;
    GXWGFifo.s16 = 1;
    GXWGFifo.s16 = 1;

    GXWGFifo.s16 = x;
    GXWGFifo.s16 = y + polygonHeight;
    GXWGFifo.s16 = 0;
    GXWGFifo.s16 = 0;
    GXWGFifo.s16 = 1;
}
