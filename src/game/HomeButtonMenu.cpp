#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "atn/SoundManager.h"
#include "game/HBMRso.h"
#include "atn/DrawHelper.h"

namespace atn {

// inferred: renderer interface (see atn/VideoTHP.cpp, atn/TextureComposite.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C();                                   // 0x80046218
    virtual void vf10(void* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* arg3); // 0x80046200
    virtual void vf18();                                  // 0x80046214
    virtual void vf1C();                                  // 0x800461FC
};

} // namespace atn

// inferred: HOME button menu (0xD4 bytes), singleton (sInstance 0x8087F4E8)
class HomeButtonMenu : public atn::IRender {
public:
    HomeButtonMenu(void* arg);
    virtual ~HomeButtonMenu();
    virtual void vf10(void* camera);

    static void fn_804405F8(void* arg); // inferred: creates the singleton
    void fn_8044073C();                // inferred: per-frame update
    void fn_80440994();                // inferred: draws the "HOME button disabled" icon

    static HomeButtonMenu* sInstance;

    HBMDataInfo m04;
    HBMControllerData m44;
    int m84; // inferred: state (0 closed, 1 open, 2 leaving the game)
    int m88; // inferred: menu allowed
    int m8C; // inferred: drawing the "disabled" icon
    int m90; // inferred: show the icon when the menu is not allowed
    void* m94;
    unsigned char* m98; // inferred: HomeButtonSe.arc data (HBMRsoCreate)
    unsigned char* m9C; // inferred: HBM sound buffer (HBMRsoCreate)
    atn::Texture_unk mA0;
    int mD0; // inferred: frame counter of the icon (46 frames)
};

// inferred: save manager (lbl_8087EF68)
struct HbmSaveLoad_unk {
    unsigned char m000[0x174];
    int m174;
};
extern "C" HbmSaveLoad_unk* lbl_8087EF68;
extern "C" int fn_800A4228(HbmSaveLoad_unk* mgr, int* out); // inferred: busy
extern "C" void* lbl_8087EF70;                               // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void* lbl_8087EE90;
extern "C" void fn_8004A2CC(void* obj, int a, int b, int c);
struct Unk_8087F018 {
    unsigned char m0000[0x40E0];
    int m40E0;
};
extern "C" Unk_8087F018* lbl_8087F018;
extern "C" void fn_800DF0F0(Unk_8087F018* obj, int arg);
extern "C" void fn_80658240(int chan); // inferred: per controller channel (stops the rumble?)
extern "C" void* lbl_8087F9A0;
extern "C" void fn_8057155C(void* obj, int arg); // inferred: reset
extern "C" void fn_805714DC(void* obj, int arg); // inferred: return to the Wii menu
extern "C" void* lbl_8087EFB4;                   // inferred: camera
extern "C" void fn_800BDB58(void* camera, void* render, float z, int arg); // inferred: queues a render
// inferred: render manager (see game/StrapTask.cpp)
struct HbmRender_unk {
    unsigned char m00[0x0E];
    unsigned short mFbWidth;  // 0x0E
    unsigned short mFbHeight; // 0x10
    unsigned char m12[0x3C - 0x12];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
    int mWide;   // 0x44: inferred: 16:9
};
extern "C" HbmRender_unk* lbl_8087EEE0;
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture

HomeButtonMenu* HomeButtonMenu::sInstance;

void HomeButtonMenu::fn_804405F8(void* arg) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) HomeButtonMenu(arg);
    }
}

HomeButtonMenu::HomeButtonMenu(void* arg) : m84(0), m88(1), m8C(0), m90(1), m94(arg), mD0(0) {
    HBMRsoCreate(m04, m98, m9C);
    fn_800D594C(&mA0, "/hbm/ban_home_button.texture");
}

HomeButtonMenu::~HomeButtonMenu() {
    HBMRsoDelete(m04, m98, m9C);
}

void HomeButtonMenu::fn_8044073C() {
    if (m84 == 0) {
        if (fn_800A555C(lbl_8087EF70, 0, 0x11) || fn_800A555C(lbl_8087EF70, 1, 0x11) ||
            fn_800A555C(lbl_8087EF70, 2, 0x11) || fn_800A555C(lbl_8087EF70, 3, 0x11)) {
            if (m88 != 0) {
                HbmSaveLoad_unk* mgr = lbl_8087EF68;
                int out;
                int busy = 1;
                if (fn_800A4228(mgr, &out) == 0 && mgr->m174 <= 0) {
                    busy = 0;
                }
                if (busy == 0) {
                    m84 = 1;
                    lbl_8087EFE8->fn_800CF7DC(1, 1, 9);
                    fn_8004A2CC(lbl_8087EE90, 1, 1, 8);
                    HBMRsoInit(m44);
                    fn_800DF0F0(lbl_8087F018, 1);
                    lbl_8087F018->m40E0 = 0;
                    for (int i = 0; i < 4; i++) {
                        fn_80658240(i);
                    }
                    m8C = 0;
                    goto draw;
                }
            }
            if (m8C == 0 && m90 != 0) {
                m8C = 1;
                mD0 = 0;
            }
        }
    draw:
        if (m8C != 0) {
            fn_80440994();
        }
    } else if (m84 == 1) {
        int ret = HBMRsoCalc(m44);
        if (ret >= 0) {
            switch (ret) {
            case 1:
                fn_8057155C(lbl_8087F9A0, 1);
                m84 = 2;
                break;
            case 2:
                fn_805714DC(lbl_8087F9A0, 1);
                m84 = 2;
                break;
            default:
                lbl_8087EFE8->fn_800CF7DC(0, 1, 9);
                fn_8004A2CC(lbl_8087EE90, 0, 1, 0);
                fn_800DF0F0(lbl_8087F018, 0);
                lbl_8087F018->m40E0 = 1;
                m84 = 0;
                break;
            }
        } else {
            fn_800BDB58(lbl_8087EFB4, this, -100000.0f, 0x13);
        }
    }
}

void HomeButtonMenu::vf10(void* camera) {
    HBMRsoDraw();
}

inline float Ratio_unk(int a, int b) {
    return (float)a / (float)b;
}

void HomeButtonMenu::fn_80440994() {
    float alpha = 1.0f;
    if (mD0 < 8) {
        alpha = mD0 / 8.0f;
    } else if (mD0 > 38) {
        alpha = (46 - mD0) / 8.0f;
    }
    unsigned int color = ((unsigned int)(255.0f * alpha) << 24) | 0xFFFFFF;
    HbmRender_unk* render = lbl_8087EEE0;
    float k = render->mWide != 0 ? 1.3333334f : 1.0f;
    float sx = Ratio_unk(render->mWidth, render->mFbWidth);
    float sy = Ratio_unk(render->mHeight, render->mFbHeight);
    float w = 64.0f;
    float h = w;
    lbl_8087EEB0->mA0 = 0x13;
    w /= sy * (k * sx);
    h *= sy;
    lbl_8087EEB0->fn_8005DFC8(color, &mA0, 1, 60.0f, 60.0f, -10000.0f, w, h);
    lbl_8087EEB0->mA0 = 0x12;
    if (++mD0 > 46) {
        m8C = 0;
    }
}
