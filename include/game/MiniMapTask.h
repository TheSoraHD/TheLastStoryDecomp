#ifndef GAME_MINIMAPTASK_H
#define GAME_MINIMAPTASK_H

#include "atn/Color.h"

#include "atn/Camera_unk.h"
#include "atn/IRender.h"
#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "game/CharaStatus.h"

class CharaTask;

// inferred: point marker of the minimap (0x2C bytes): free when m00 is -1; names invented
struct MiniMapMarker_unk {
    MiniMapMarker_unk() : m00(-1), m04(-1), m24(1) {}
    MiniMapMarker_unk(atn::Vector3 pos, atn::Color color, float scale)
        : m00(1), m04(-1), mColor(color), mPos(pos), m24(1), mScale(scale) {}
    int m00;
    int m04;
    atn::Color mColor;  // 0x08
    atn::Vector3 mPos;  // 0x18
    int m24;            // inferred: cleared for the markers of characters
    float mScale;       // 0x28
};
// inferred: character marked on the minimap and its marker (index of mMarkers); names invented
struct MiniMapCharaMark_unk {
    MiniMapCharaMark_unk() : mChara(0), mMarker(-1) {}
    MiniMapCharaMark_unk(CharaTask* chara, int marker) : mChara(chara), mMarker(marker) {}
    CharaTask* mChara;
    int mMarker;
};

// inferred: map minimap renderer, with a secondary atn::IRender base at +0x48
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
    // inferred: adds / removes a point marker (game/ChaseManager); returns the marker or -1
    int fn_804AABA8(atn::Vector3 pos, atn::Color color, float scale);
    void fn_804AAD54(int marker);
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
    MiniMapMarker_unk mMarkers[64];   // 0x834: inferred: point markers (ChaseManager, fn_804AAD68)
    FixedArray_unk<MiniMapCharaMark_unk, 64> mCharaMarks; // 0x1334: inferred: characters with a marker
    int m1538;                        // inferred: initialized to 0
    unsigned char m153C[0x80];        // inferred: minimap display state
    atn::Task* m15BC;                 // inferred: debug menu group
    float m15C0;                      // inferred: initialized to 3000
    atn::Texture_unk m15C4;           // inferred: final texture
    unsigned char m15F4[0x18];       // inferred: trailing render state
    int m160C;                        // inferred: initialized to 0
};

#endif
