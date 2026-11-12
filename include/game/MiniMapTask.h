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
namespace atn {
class D2A_Task;
}

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

// inferred: rectangle of the minimap ("OFF_AREA" of the csv: two corners, copied with two paired singles);
// names invented
struct MiniMapArea_unk {
    atn::Vector2_unk mMin;
    atn::Vector2_unk mMax;
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
    void fn_804A9EB4(); // inferred: places the frame and draws the markers
    void fn_804AA490(); // inferred: reads the csv of the minimap (markers, radar, arrow, point...)
    int fn_804AABA8(atn::Vector3 pos, atn::Color color, float scale);
    void fn_804AAD54(int marker);
    void fn_804AAD68(CharaTask* chara, const atn::Color* color);
    void fn_804AAFCC(CharaTask* chara);
    void fn_804ABF84(atn::RenderManager* view); // inferred: draws the minimap with its orthographic camera

    int m4C;                           // inferred: initialized to 1
    atn::LH_Binary m50;               // inferred: minimap resource
    atn::Texture_unk m58[9];          // inferred: nine minimap textures
    atn::Model m208;                  // inferred: minimap model, flags 0x00A00001/0x20
    atn::D2A_Task* m5D8;              // inferred: d2anime of the minimap frame ("basePos", "rot01"...)
    float m5DC;                       // inferred: rectangle of the minimap on the screen (x, y, w, h)
    float m5E0;
    float m5E4;
    float m5E8;
    float m5EC;                       // inferred: "RADAR" values 10-13 (fn_804AA490)
    float m5F0;
    float m5F4;
    float m5F8;
    float m5FC;                       // inferred: scale of the view (screen height / 52)
    atn::Color m600;                  // inferred: "RADAR" colour
    atn::Color m610;                  // inferred: "ARROW" colour
    atn::Color m620;                  // inferred: "POINT" colour
    float m630;                       // inferred: "ARROW" values 1-2
    float m634;
    float m638;                       // inferred: "POINT" values 1-2
    float m63C;
    atn::Camera m640;                 // inferred: minimap camera
    MiniMapMarker_unk mMarkers[64];   // 0x834: inferred: point markers (ChaseManager, fn_804AAD68)
    FixedArray_unk<MiniMapCharaMark_unk, 64> mCharaMarks; // 0x1334: inferred: characters with a marker
    FixedArray_unk<MiniMapArea_unk, 8> m1538; // inferred: "OFF_AREA" rectangles
    atn::Task* m15BC;                 // inferred: debug menu group
    float m15C0;                      // inferred: initialized to 3000
    atn::Texture_unk m15C4;           // inferred: final texture
    float m15F4;                      // inferred: "LOWER_LAYER" values (m1608 is set to 1)
    float m15F8;
    float m15FC;
    float m1600;
    float m1604;
    float m1608;
    int m160C;                        // inferred: initialized to 0
};

#endif
