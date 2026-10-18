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
}

#endif
