#ifndef ATN_GXDECL_H
#define ATN_GXDECL_H

// Minimal declarations of the GX functions used by the game code (the SDK headers are not
// on the game include path). Enums are passed as int.
struct GXColor {
    unsigned char r, g, b, a;
};
struct GXTexObj {
    unsigned int dummy[8];
};
struct GXTexRegion {
    unsigned int dummy[4];
};
struct GXLightObj {
    unsigned int dummy[16];
};

extern "C" {
void GXInitTexCacheRegion(GXTexRegion* region, unsigned char is32bMipmap, unsigned int tmemEven, int sizeEven,
                          unsigned int tmemOdd, int sizeOdd);
void GXInitTexObjLOD(GXTexObj* obj, int minFilt, int magFilt, float minLod, float maxLod, float lodBias,
                     unsigned char biasClamp, unsigned char doEdgeLod, int maxAniso);
unsigned short GXGetTexObjWidth(const GXTexObj* obj);
unsigned short GXGetTexObjHeight(const GXTexObj* obj);
void GXSetCopyClear(GXColor color, unsigned int z);
void GXSetZMode(unsigned char compareEnable, int func, unsigned char updateEnable);
void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short width, unsigned short height);
void GXSetTexCopyDst(unsigned short width, unsigned short height, int fmt, unsigned char mipmap);
void GXCopyTex(void* dest, unsigned char clear);
void GXPixModeSync();
void GXInitTexObj(GXTexObj* obj, void* image, unsigned short width, unsigned short height, int format, int wrapS,
                  int wrapT, unsigned char mipmap);
void GXSetNumTexGens(unsigned char n);
void GXSetNumChans(unsigned char n);
void GXSetNumTevStages(unsigned char n);
void GXSetNumIndStages(unsigned char n);
void GXSetTevOp(int stage, int mode);
void GXSetTevOrder(int stage, int coord, int map, int color);
void GXSetTexCoordGen2(int dst, int func, int src, unsigned int mtx, unsigned char normalize, unsigned int postMtx);
void GXSetTevDirect(int stage);
void GXSetChanCtrl(int chan, unsigned char enable, int ambSrc, int matSrc, unsigned int lightMask, int diffFn,
                   int attnFn);
void GXSetBlendMode(int type, int src, int dst, int op);
void GXInitLightAttn(GXLightObj* lt, float a0, float a1, float a2, float k0, float k1, float k2);
void GXInitLightSpot(GXLightObj* lt, float cutoff, int spotFunc);
void GXInitLightDistAttn(GXLightObj* lt, float refDistance, float refBrightness, int distFunc);
void GXInitLightPos(GXLightObj* lt, float x, float y, float z);
void GXInitLightDir(GXLightObj* lt, float nx, float ny, float nz);
void GXInitLightColor(GXLightObj* lt, GXColor color);
void GXInvalidateTexAll();
void GXSetTevKColor(int id, GXColor color);
void GXSetTevKColorSel(int stage, int sel);
void GXSetTevColorIn(int stage, int a, int b, int c, int d);
void GXSetTevColorOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetTevSwapMode(int stage, int rasSel, int texSel);
void GXSetTevSwapModeTable(int table, int red, int green, int blue, int alpha);
void GXLoadTexMtxImm(const float mtx[][4], unsigned int id, int type);
}

#endif
