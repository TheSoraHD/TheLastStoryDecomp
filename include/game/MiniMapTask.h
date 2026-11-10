#ifndef GAME_MINIMAPTASK_H
#define GAME_MINIMAPTASK_H

#include "atn/Color.h"

#include "atn/Camera_unk.h"
#include "atn/IRender.h"
#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "atn/Task.h"
#include "atn/Texture_unk.h"

// inferred: map minimap renderer, with a secondary atn::IRender base at +0x48
class CharaTask;
class MiniMapTask : public atn::Task, public atn::IRender {
public:
    MiniMapTask(atn::Task* parent, const char* path);
    virtual ~MiniMapTask();

    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    virtual int GetRenderType_unk();
    virtual void vf10(atn::RenderManager* view);
    virtual void vf18(atn::RenderManager* view);
    virtual void vf1C();
    // inferred: marks / unmarks a character on the minimap (GameEventScript::fn_803A6744)
    void fn_804AAD68(CharaTask* chara, const atn::Color* color);
    void fn_804AAFCC(CharaTask* chara);
    void fn_804ABF84(atn::RenderManager* view); // inferred: draws the minimap with its orthographic camera

    int m4C;                           // inferred: initialized to 1
    atn::LH_Binary m50;               // inferred: minimap resource
    atn::Texture_unk m58[9];          // inferred: nine minimap textures
    atn::Model m208;                  // inferred: minimap model, flags 0x00A00001/0x20
    atn::Task* m5D8;                  // inferred: task for the minimap controller animation
    float m5DC;                       // inferred: rectangle of the minimap on the screen (x, y, w, h)
    float m5E0;
    float m5E4;
    float m5E8;
    unsigned char m5EC[0x10];         // inferred: state up to the camera
    float m5FC;                       // inferred: scale of the view (screen height / 52)
    unsigned char m600[0x40];
    atn::Camera m640;                 // inferred: minimap camera
    int m834;                         // inferred: initialized to -1
    int m838;                         // inferred: initialized to -1
    unsigned char m83C[0x1C];         // inferred: camera state
    int m858;                         // inferred: 1
    unsigned char m85C[4];            // inferred: alignment
    unsigned char m860[0xCD8];        // inferred: display records up to m1538
    int m1538;                        // inferred: initialized to 0
    unsigned char m153C[0x80];        // inferred: minimap display state
    atn::Task* m15BC;                 // inferred: debug menu group
    float m15C0;                      // inferred: initialized to 3000
    atn::Texture_unk m15C4;           // inferred: final texture
    unsigned char m15F4[0x18];       // inferred: trailing render state
    int m160C;                        // inferred: initialized to 0
};

#endif
