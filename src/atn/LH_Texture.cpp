#include "atn/LH_Texture.h"
#include "atn/LoadManager.h"
#include "atn/Loader_unk.h"

extern "C" void DCFlushRange(void* addr, unsigned long size);

// inferred: header of a loaded texture file
struct TexHeader_unk {
    unsigned char m00[0x20];
    int mFormat;    // 0x20
    int mWidth;     // 0x24
    int mHeight;    // 0x28
    int m2C;
    unsigned char m30[4];
    void* mImage;   // 0x34
};

namespace atn {

// inferred from __vt__Q23atn11LoadTexture (0x8078FF88)
class LoadTexture : public LoadCore {
public:
    LoadTexture(const char* path) : LoadCore(path) {}
    virtual ~LoadTexture();
    virtual void vf0C();
};

LoadTexture::~LoadTexture() {}

void LoadTexture::vf0C() {
    if (mState == 2) {
        fn_80473104(3);
        Loader_unk loader;
        fn_804714A4(&loader);
        fn_804714B0(&loader, m54, m50, LoaderCallback_unk(), 0);
        DCFlushRange(m54, (unsigned long)m50);
    }
}

void LH_Texture::fn_80478158(GXTexObj* obj, int wrapS, int wrapT) {
    if (mCore != 0) {
        TexHeader_unk* header = (TexHeader_unk*)((LoadCore*)mCore)->m54;
        GXInitTexObj(obj, header->mImage, header->mWidth, header->mHeight, header->mFormat, wrapS,
                     wrapT, 0);
    }
}

int LH_Texture::fn_80478198() {
    if (mCore != 0) {
        return ((TexHeader_unk*)((LoadCore*)mCore)->m54)->m2C;
    }
    return 0;
}

void LH_Texture::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadTexture>(path, 1);
}

} // namespace atn
