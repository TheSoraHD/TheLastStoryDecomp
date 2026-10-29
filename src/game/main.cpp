// inferred file name (splits-auto): entry point of the game. main() runs the start-up (fn_8047A4C8)
// and then the frame loop, which times each part of the frame into HeapInfo_unk and calls the
// per-frame callbacks registered in lbl_8087EE94 (bound members of ResetShutdown/ErrorManager).
#include "atn/Array_unk.h"
#include "atn/MemoryManager.h"
#include "atn/Memory.h"
#include "atn/SoundManager.h"
#include "atn/Task.h"
#include "atn/SequenceBoot_unk.h"
#include "game/HeapInfo_unk.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "sdk/OSFastCast.h" // OSInitFastCast (SDK header asm, approved 2026-10-21)

extern "C" void OSInit();
extern "C" void OSReport(const char* fmt, ...);
extern "C" void OSInitMutex(void* mutex);
extern "C" void DVDInit();
extern "C" void VIInit();
extern "C" void NWC24SuspendScheduler();
extern "C" unsigned char SCGetLanguage();
extern "C" unsigned long strlen(const char* s);

// inferred: power/reset handling (the GameSystem_unk of game/GameSystem_unk.cpp; class name from the
// type name of the bound functor at 0x807561F4)
class ResetShutdown {
public:
    void fn_80570CF8(); // inferred: per-frame update
};
extern "C" void fn_80570CF8(ResetShutdown* self); // the same function, called directly by main
extern "C" void fn_80570C50();                    // inferred: creates the ResetShutdown singleton
extern "C" ResetShutdown* lbl_8087F9A0;

// inferred: error manager (class name from the type name of the bound functor at 0x8075617C)
class ErrorManager {
public:
    void fn_80363DBC(); // inferred: per-frame update
    void fn_803646C4(); // inferred: per-frame update
    unsigned char m00[0x18];
    int m18;
    int m1C;
};
extern "C" ErrorManager* lbl_8087F420;
extern "C" void fn_80363CE4(); // inferred: creates the ErrorManager singleton

// inferred: HOME button menu (game/HomeButtonMenu.cpp), only the members used here
class HomeButtonMenu {
public:
    static void fn_804405F8(void* arg); // inferred: creates the singleton
    void fn_8044073C();                // inferred: per-frame update

    unsigned char m00[0x84];
    int m84; // inferred: state (1 open)

    static HomeButtonMenu* sInstance;
};

// inferred: settings (only the members used here)
struct MainConfig_unk {
    unsigned char m000[0x08];
    int m08;
    unsigned char m00C[0x10 - 0x0C];
    int m10;
    int m14;
    unsigned char m018[0x30 - 0x18];
    int m30; // inferred: frame rate (30 or 60)
    unsigned char m034[0x4C - 0x34];
    int m4C;
    unsigned char m050[0x5B8 - 0x50];
    int m5B8;
};
extern "C" MainConfig_unk* lbl_8087F0A8;

class SetConfig {
public:
    static void fn_8057E8C4(); // inferred: creates the instance
    void fn_8057EAF0();        // inferred: registers the debug menu
    static SetConfig* sInstance;
};

struct DebugMessages_unk {
    void fn_80069E50(int arg);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

// inferred: dialog game object (float at 0x1EB4: frame rate shown)
struct MainDialog_unk {
    unsigned char m0000[0x1EB4];
    float m1EB4;
};
extern "C" MainDialog_unk* lbl_8087F540;

struct MainLoadMgr_unk {
    unsigned char m0000[0x3ED8];
    int m3ED8;
};
extern "C" MainLoadMgr_unk* lbl_8087F518;
extern "C" void fn_8046F014(MainLoadMgr_unk* mgr);
extern "C" void fn_8046F2C4(MainLoadMgr_unk* mgr);

struct MainSound_unk {
    unsigned char m0000[0x40D4];
    int m40D4;
    int m40D8;
    int m40DC;
};
extern "C" MainSound_unk* lbl_8087F018;

// inferred: reference to a resource of the load manager (one word here; see game/CampSaveLoad.h)
struct ResRef_unk {
    ResRef_unk() : mRes(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_8047043C(const char* name, int a, int b); // inferred: loads `name`
    void fn_80470528();                               // inferred: drops the reference
    void* mRes;
};

namespace atn {
class PostFxManager {
public:
    static void fn_800A8FF0(Task* parent);
};
class DrawHelper {
public:
    static void fn_8005DD60();
};
} // namespace atn

extern "C" void* lbl_8087EEE0;
extern "C" void* lbl_8087EF70;
extern "C" void* lbl_8087EE98;
extern "C" void* lbl_8087EE90;
extern "C" void* lbl_8087EEB0;
extern "C" void fn_8006BA54(unsigned char lang);
extern "C" void fn_80071220();
extern "C" void fn_800713D4(void* obj, int w, int h);
extern "C" void fn_80089620();
extern "C" void fn_800B95C4();
extern "C" void fn_80185AD4();
extern "C" void fn_80187C44(MainConfig_unk* config, const char* path);
extern "C" void fn_80186520(MainConfig_unk* config);
extern "C" void fn_800696C8();
extern "C" void fn_8046D7D8(atn::Task* root);
extern "C" void fn_800BC798();
extern "C" void fn_8005DED8(void* obj, int size, const std::string& name);
extern "C" void fn_80239794(int a, int b, int c, int d);
extern "C" void fn_8004C9B4();
extern "C" void fn_800A5508();
extern "C" void fn_800463A8();
extern "C" void fn_801F0D80(atn::Task* root);
extern "C" void fn_800A2C48(atn::Task* root);
extern "C" void fn_80207F80();
extern "C" void fn_800A5554(void* pad);
extern "C" void fn_800D2494(atn::Task* root);
extern "C" void fn_8004D124(void* obj);
extern "C" void fn_800D2D7C(atn::Task* root);
extern "C" void fn_800D2AD8(atn::Task* root);
extern "C" void fn_8004D258(void* obj, int a, int b, int c, int d, int e);
extern "C" void fn_80047DCC(void* obj);
extern "C" void fn_80071D60(void* obj, int arg);

// inferred: callback of the render system with its argument (constructor 0x800BD94C)
struct RenderCallback_unk {
    RenderCallback_unk(boost::function<void(void*)> func, int arg);
    boost::function<void(void*)> mFunc;
    int mArg;
};

// inferred: render system (only the members used here)
class RenderSystem_unk {
public:
    // inferred: inline setters of the callbacks run around the frame (weak copies here)
    void fn_80479EA0(boost::function<void(void*)> func, int arg) { m8B0 = RenderCallback_unk(func, arg); }
    void fn_8047A158(boost::function<void(void*)> func, int arg) { m8C8 = RenderCallback_unk(func, arg); }

    unsigned char m000[0x8B0];
    RenderCallback_unk m8B0;
    RenderCallback_unk m8C8;
};
extern "C" RenderSystem_unk* lbl_8087EFB4;
extern "C" void fn_800BD9C0(RenderSystem_unk* render, int size, const std::string& name);
extern "C" void fn_800BDBC8(RenderSystem_unk* render);
extern "C" void fn_800BFAC4(RenderSystem_unk* render);

typedef atn::Vector_unk<boost::function<void()> > FrameCallbacks_unk;
extern "C" FrameCallbacks_unk* lbl_8087EE94;

// inferred: creates the list of per-frame callbacks (name invented)
inline void CreateFrameCallbacks_unk() {
    if (lbl_8087EE94 == 0) {
        lbl_8087EE94 = new (1, "", "", 0) FrameCallbacks_unk;
    }
}

extern "C" HeapInfo_unk* lbl_8087F534;
extern "C" atn::Task* lbl_8087F530; // inferred: root task

void fn_8047A4C8();

// inferred: start of the render time
void fn_80479998(void*) {
    lbl_8087F534->mTimer2.Start_unk();
}

// inferred: end of the render time
void fn_804799CC(void*) {
    lbl_8087F534->mTimer2.Stop_unk();
}

// NONMATCHING: 98.6% - saved register allocation (the original keeps the callback offset in r31)
int main() {
    fn_8047A4C8();
    int frame = 0;
    for (;;) {
        lbl_8087EFB4->fn_80479EA0(fn_80479998, 0);
        lbl_8087EFB4->fn_8047A158(fn_804799CC, 0);
        lbl_8087F534->fn_805A6BD4();
        lbl_8087F534->mTimer0.Start_unk();
        if (lbl_8087F540 != 0) {
            lbl_8087F540->m1EB4 = lbl_8087F534->fn_805A6D10();
        }
        fn_800A5554(lbl_8087EF70);
        fn_800BDBC8(lbl_8087EFB4);
        lbl_8087F534->mTimer1.Start_unk();
        if (HomeButtonMenu::sInstance->m84 != 1) {
            fn_800D2494(lbl_8087F530);
            fn_8004D124(lbl_8087EE98);
        }
        lbl_8087F534->mTimer1.Stop_unk();
        fn_800D2D7C(lbl_8087F530);
        fn_800D2AD8(lbl_8087F530);
        HomeButtonMenu::sInstance->fn_8044073C();
        if (lbl_8087F0A8->m08 != 0) {
            fn_8004D258(lbl_8087EE98, 1, 0, lbl_8087F0A8->m10, -1, lbl_8087F0A8->m14);
        }
        lbl_8087EEB8->fn_80069E50(lbl_8087F0A8->m4C);
        fn_800BFAC4(lbl_8087EFB4);
        lbl_8087F534->mTimer3.Start_unk();
        fn_80047DCC(lbl_8087EE90);
        lbl_8087EFE8->fn_800CE6E0();
        lbl_8087EFE8->fn_800CECF0();
        lbl_8087F534->mTimer3.Stop_unk();
        lbl_8087F534->mTimer0.Stop_unk();
        lbl_8087F534->fn_805A6BD8();
        fn_80071D60(lbl_8087EEE0, lbl_8087F0A8->m30 == 30);
        fn_80570CF8(lbl_8087F9A0);
        FrameCallbacks_unk* callbacks = lbl_8087EE94;
        for (unsigned int i = 0; i < callbacks->mSize; i++) {
            (*callbacks)[i]();
        }
        if (frame == 2) {
            frame = 0;
        }
    }
}

// inferred: start-up
// NONMATCHING: 39.7% - the original inlines the boost::bind/boost::function constructors, the
// strings and LightDesc_unk while calling Vector_unk::push_back out of line (here the three push_back
// calls make every candidate go out of line; push_back probably takes the function by value in the
// original). Over the budget OSInitFastCast (approved SDK header asm) is also called out of line here,
// while the original inlines its four mtspr GQR2-5 after OSInit.
void fn_8047A4C8() {
    OSInit();
    OSInitFastCast();
    OSInitMutex((char*)atn::MemoryManager::Instance() + 0x1B4);
    DVDInit();
    VIInit();
    NWC24SuspendScheduler();
    fn_80570C50();
    unsigned char lang = SCGetLanguage();
    if (lang >= 6) {
        lang = 1;
    }
    if (lang == 0) {
        lang = 1;
    }
    fn_8006BA54(lang);
    fn_80071220();
    fn_800713D4(lbl_8087EEE0, 640, 448);
    OSReport("0\n");
    lbl_8087F530 = new (1, "", "", 0) atn::Task(0);
    fn_80089620();
    fn_800B95C4();
    fn_80363CE4();
    lbl_8087F420->m18 = 1;
    lbl_8087F420->m1C = 1;
    CreateFrameCallbacks_unk();
    lbl_8087EE94->push_back(boost::bind(&ResetShutdown::fn_80570CF8, lbl_8087F9A0));
    lbl_8087EE94->push_back(boost::bind(&ErrorManager::fn_80363DBC, lbl_8087F420));
    lbl_8087EE94->push_back(boost::bind(&ErrorManager::fn_803646C4, lbl_8087F420));
    OSReport("0.1\n");
    fn_80185AD4();
    SetConfig::fn_8057E8C4();
    fn_80187C44(lbl_8087F0A8, "/config.ini");
    fn_80186520(lbl_8087F0A8);
    OSReport("0.2\n");
    SetConfig::sInstance->fn_8057EAF0();
    SequenceBoot_unk::Create_unk(lbl_8087F530);
    lbl_8087F534 = new (1, "", "", 0) HeapInfo_unk;
    fn_800696C8();
    fn_8046D7D8(lbl_8087F530);
    OSReport("1\n");
    lbl_8087F518->m3ED8 = lbl_8087F0A8->m5B8;
    OSReport("2\n");
    fn_8046F014(lbl_8087F518);
    OSReport("3\n");
    fn_8046F2C4(lbl_8087F518);
    lbl_8087F420->m1C = 0;
    OSReport("4\n");
    ResRef_unk boot;
    boot.fn_8047043C("/preload/boot.pk", 0, 0);
    OSReport("5\n");
    fn_800BC798();
    OSReport("6\n");
    atn::PostFxManager::fn_800A8FF0(lbl_8087F530);
    OSReport("7\n");
    fn_800BD9C0(lbl_8087EFB4, 0x40000, std::string(""));
    atn::DrawHelper::fn_8005DD60();
    fn_8005DED8(lbl_8087EEB0, 0x40000, std::string("font"));
    fn_80239794(0x480, 0x200, 0x100, -1);
    fn_8004C9B4();
    fn_800A5508();
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40D4 = 0;
        lbl_8087F018->m40D8 = 0;
        lbl_8087F018->m40DC = 0;
    }
    fn_800463A8();
    fn_800CCBA4();
    lbl_8087EFE8->fn_800D0684(2, 0.6f);
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(1));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(2));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(3));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(4));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(5));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(6));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(7));
    lbl_8087EFE8->fn_800CFDF0(atn::SoundCategory_unk(8));
    lbl_8087EFE8->fn_800CFFB8(atn::SoundCategory_unk(1));
    lbl_8087EFE8->fn_800CFFB8(atn::SoundCategory_unk(2));
    fn_801F0D80(lbl_8087F530);
    fn_800A2C48(lbl_8087F530);
    HomeButtonMenu::fn_804405F8(lbl_8087F530);
    fn_80207F80();
    boot.fn_80470528();
    lbl_8087F420->m18 = 0;
    lbl_8087F420->m1C = 0;
}
