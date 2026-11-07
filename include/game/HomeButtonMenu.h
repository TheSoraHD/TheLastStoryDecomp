#ifndef GAME_HOMEBUTTONMENU_H
#define GAME_HOMEBUTTONMENU_H

#include "atn/Texture_unk.h"
#include "game/HBMRso.h"
#include "atn/IRender.h"

// inferred: HOME button menu (0xD4 bytes), singleton (sInstance 0x8087F4E8)
class HomeButtonMenu : public atn::IRender {
public:
    HomeButtonMenu(void* arg);
    virtual ~HomeButtonMenu();
    virtual void vf10(atn::RenderManager* view);

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

#endif
