#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "atn/Task.h"
#include "Event/Path.h"
#include "atn/Bgm.h"
#include "std/vector.h"
#include "std/string.h"
#include "std/list.h"
#include "atn/Sphere.h"
#include "atn/ColorMatrixParam_unk.h"
#include "atn/Light_unk.h"
#include "atn/D2A_Task.h"
#include "atn/FadeCommon.h"
#include "game/ResHandle.h"
#include "atn/CharaLightData_unk.h"
#include "gmk/SeTable.h"
#include "Event/DebugInfo_unk.h"

class GameScene;
class CharaTask;
class NewStaffRoll;
struct GameMap_unk;
struct ColorMatrixParam_unk;
struct NewCampScene2_unk;

namespace Event {

class Action;
class Sequence;
struct MovieState_unk;

// inferred: entry of the queue of sequences to start (fn_8047E964)
struct QueuedSequence_unk {
    QueuedSequence_unk() : mId(0), m04(0) {}
    int mId;
    int m04;
};

// inferred: 8-byte entry of the table at 0x1F80 of the manager (127 of them, cleared by the constructor)
struct ManagerEntry_unk {
    ManagerEntry_unk() : m00(0), m04(0) {}
    int m00;
    int m04;
};

// inferred: light settings of a scene (0x190 bytes): copied into the scene (fn_8047C88C) or blended
// with it (fn_8047CB74). Name invented.
struct SceneSetting_unk {
    void fn_8047C88C(NewCampScene2_unk* scene);          // inferred: copies the settings into the scene
    void fn_8047CB74(NewCampScene2_unk* scene, float t); // inferred: blends the scene towards the settings

    atn::FogSettings mFog;              // 0x000
    atn::Color mAmbient;                // 0x01C
    atn::Color mLS;                     // 0x02C
    atn::Color m3C;                     // 0x03C
    GodRayParam_unk mGodRay;            // 0x04C
    ShadowParam_unk mShadow;            // 0x094
    ColorMatrixParam_unk mColorMatrix;  // 0x0BC
    HazeParam_unk mHaze;                // 0x10C
    int m148;                           // 0x148
    atn::Light mLight;                  // 0x14C
};

// inferred: light settings with their id (0x194 bytes; fn_804827D8 blends the scene towards one)
struct SceneSettingEntry_unk {
    int mId;
    SceneSetting_unk mSetting; // 0x04
};

// inferred: settings of the event at 0x1AB0 of the manager (0x120 bytes). Names invented.
struct EventConfig_unk {
    EventConfig_unk();
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[0x8 - 0x2];
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    float m38;
    float m3C;
    atn::Color m40;
    atn::Color m50;
    atn::Color m60;
    int m70;
    int m74;
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    int m8C;
    int m90;
    int m94;
    float m98;
    float m9C;
    float mA0;
    float mA4;
    int mA8;
    int mAC;
    float mB0;
    float mB4;
    int mB8;
    int mBC;
    int mC0;
    float mC4;
    float mC8;
    int mCC;
    int mD0;
    float mD4;
    float mD8;
    int mDC;
    int mE0;
    atn::Color mE4;
    atn::Vector3 mF4;
    int m100;
    int m104;
    float m108;
    float m10C;
    std::vector<SceneSettingEntry_unk> m110; // inferred: light settings of the map
    int m11C;
};

// inferred: curve of the manager (0x10 bytes; fn_8048B048, fn_8048B3B8). Names invented.
struct Curve_unk {
    int mId;
    void* m04;
    int m08;
    int m0C;
};



// inferred: event manager (Event/Manager.cpp, vtable __vt__Q25Event7Manager; instance lbl_8087F540).
// Partial: only what Event/Sequence.cpp uses; member and method names are invented.
class Manager : public atn::Task {
public:
    Manager(atn::Task* parent); // 0x8047CF7C
    virtual ~Manager();          // 0x8047DF70
    virtual int vf0C();          // 0x80487184
    virtual void vf10();         // 0x80487490
    virtual void vf14();         // 0x80487FA0
    static Manager* fn_8047E4BC(atn::Task* parent); // inferred: creates the single instance
    // inferred: creates the character `id` of kind `kind` for the event (Event::Actor)
    CharaTask* fn_8047FF70(int kind, int id, float arg, const atn::Vector3& pos);
    void fn_8047E528();
    void fn_8047F580();
    void fn_8047F8B0();
    void fn_8047F994(int a, int b);
    void fn_8048169C(int arg);
    void fn_804817D4();
    void fn_80482ED8();
    void fn_80483714(int a, int b);
    void fn_80483E94(int arg);
    void fn_80484FBC();
    void fn_80489A50(Action* action, int arg);
    void fn_8048E904();
    void fn_8048F184(int arg);
    // inferred: called by Event/ActionEffectPlay.cpp
    void fn_80486BEC(int on, float value);
    void fn_804846FC(const atn::Vector3& dir, int frames);
    void fn_80484A78(int frames);
    void fn_80486DAC(int on);
    void fn_80487064(int on);
    void fn_8048EE78(int a, float r, float g, float b, float c, float d, int e, float f);
    void fn_80488050(Sequence* manager, Action* arg);
    void fn_80488A74(const char* name, int fade, float volume);
    void fn_80488D80(int fade);
    GameMap_unk* fn_80480438(); // inferred: map of the event (its locators)
    void fn_8048E9E0();
    void fn_80485028(int index, const atn::Vector3& pos, float radius, int color);
    void fn_80485964(const atn::Vector3& pos, const atn::Vector3& dir, float a, float b, int color);
    void fn_80484D64(atn::Vector2_unk scale, atn::Vector2_unk pos);
    void fn_80484BF4();
    void fn_80484D40(atn::Vector2_unk scale, atn::Vector2_unk pos, float rate);
    void fn_80484E88();
    ColorMatrixParam_unk* fn_80483F70();
    void fn_80484088(ColorMatrixParam_unk* param, float rate);
    void fn_804881B8(Sequence* manager, const char* name, int fade, int stopBgm, int a, int b, int c, float volume, int d, int flags);
    void fn_8048E9A4(MovieState_unk* movie, int frames);
    void fn_8048EC7C();
    void fn_80489A2C();
    int fn_8048B048(int id);                  // inferred: time curve by id
    int fn_8048B3B8(int id);                  // inferred: move curve by id
    float fn_8048BCA4(int curve, float t);    // inferred: value of a time curve
    float fn_8048BD04(int curve, float t);    // inferred: value of a move curve
    void fn_8048C884(int curve);              // inferred: releases a move curve
    void fn_8048D3A4(int index);              // inferred: releases the time curve `index`
    void fn_8048DE54(int index);              // inferred: releases the move curve `index`
    // inferred: weak copies 0x80112F54 and 0x801130F4 (Event/Path.cpp)
    Sequence* fn_80112F54() { return m70; }
    int fn_801130F4() { return mC4; }
    void fn_8048FE88();
    void fn_8048FEF0();
    int fn_8048FF34(); // inferred: nonzero while loading
    // inferred: the music of the event is being prepared (inlined in Event::Sequence; name invented)
    bool IsBgmBusy_unk() { return m1F34 != 0 && m1F20.fn_8004B0E4() != 0; }

    // inferred: sequence with the id `id` (current one first, then the list m64; inlined); name invented
    Sequence* FindSequence_unk(int id);
    void AddSequence_unk(Sequence* seq); // inferred: not in the DOL (see Event/Manager.cpp)
    int fn_8047E5DC(int id); // inferred: loads the sequence `id`
    int fn_8047E964(int id, int queue); // inferred: starts (or queues) the sequence `id`
    int fn_8047F268(int id); // inferred: the sequence `id` is missing, not started or past state 5
    int fn_8047F2EC(int id); // inferred: the sequence `id` is playing (state 10)
    int fn_8047F364(int id); // inferred: the sequence `id` is missing, not started or ending
    void fn_8047EFD8(int id, int arg);
    void fn_8047F91C(); // inferred: skips the current sequence to its end
    void fn_804805B4();
    int fn_8047F420(int id); // inferred: the sequence `id` is missing or of kind 1
    int fn_8047F498(int id); // inferred: the sequence `id` is missing or of kind 3
    int fn_8047F510(int id); // inferred: the sequence `id` is missing or has m98_8
    int fn_8047F6B0(int arg);
    void fn_8047F6E4();
    void fn_8047F850(int a, int b, int c);
    void fn_8047F88C(int arg);
    void fn_8047F8A4(float value);
    void fn_8047F984(float time);
    void fn_80488D6C(); // inferred: sets the frame time (also in the game settings)
    void fn_8047F8C0(int arg);
    int fn_80481654();   // inferred: the task m1A7C is not frozen (no event is running)
    int fn_80481668();   // inferred: nonzero while the song stops the characters
    void fn_80481800(int on); // inferred: shows the check sphere
    // inferred: next state of m1AA0 (inlined in fn_8047F6B0 and fn_8047F6E4); name invented
    void Step1AA0_unk() {
        if (m1AA0 == 0) {
            m1AA0 = 1;
        } else if (m1AA0 == 2) {
            m1AA0 = 3;
        }
    }

    atn::D2A_Task* m48; // inferred: "ui_aspect_event.d2a"
    atn::Sphere m4C; // inferred: target of the check of type 1 (ActionCheck)
    float m5C; // inferred: debug menu "AcceptAnimeScale"
    float m60; // inferred: debug menu "AcceptAnimeSpeed"
    std::vector<Sequence*> m64; // inferred: sequences of the manager
    Sequence* m70; // inferred: current sequence
    int m74[16];
    float mB4; // inferred: time of a frame
    std::__list_owned_unk<int> mB8; // inferred: element type unknown (cleared when the cut changes)
    int mC4;   // inferred: the cameras are updated at once
    EventCamera_unk mCameras[4]; // 0xC8
    int m1A38; // inferred: current camera
    int m1A3C;
    int m1A40; // inferred: debug menu "DebugCamera"
    int m1A44; // inferred: debug menu "DebugInfoDisplay"
    int m1A48;
    int m1A4C;
    int m1A50;
    int m1A54;
    float m1A58; // inferred: debug menu "FastForwardSpeed"
    ResHandle m1A5C; // inferred: debug menu folder "Event"
    std::vector<GameMap_unk*> m1A60; // inferred: map of the event (first entry) when not empty
    GameScene* m1A6C; // inferred: scene of the event when there is no field
    std::vector<CharaTask*> m1A70; // inferred: characters created for the event (fn_8047FF70)
    atn::D2A_Task* m1A7C; // inferred: "ui_c_black_mask.d2a" (frozen by fn_8048169C)
    atn::D2A_Task* m1A80; // inferred: "ui_event_start_flash.d2a" (frozen by fn_804817D4)
    float m1A84;
    int m1A88;
    int m1A8C;
    int m1A90;
    int m1A94;
    int m1A98;
    atn::FadeCapture* m1A9C;
    int m1AA0; // inferred: state (0, 1, 2, 3)
    int m1AA4;
    atn::FadeColor* m1AA8;
    int m1AAC;
    EventConfig_unk m1AB0; // inferred: settings of the event (0x120 bytes)
    SceneSetting_unk m1BD0; // inferred: light settings of the event
    unsigned char m1D60[0x1D70 - 0x1D60];
    ColorMatrixParam_unk m1D70; // inferred: color matrix of the event
    int m1DC0;
    int m1DC4;
    int m1DC8;
    float m1DCC;
    float m1DD0;
    int m1DD4;
    int m1DD8;
    float m1DDC;
    float m1DE0;
    int m1DE4;
    int m1DE8;
    int m1DEC;
    int m1DF0;
    int m1DF4;
    int m1DF8;
    DebugInfo_unk m1DFC;
    int m1E74;
    int m1E78;
    std::string m1E7C;
    float m1E88;
    float m1E8C;
    int m1E90; // inferred: debug menu "DrawViewEventSphere"
    atn::Sphere m1E94; // inferred: sphere looked at by ActionCheck (type 1)
    int m1EA4; // inferred: the check sphere is shown (fn_80481800)
    int m1EA8;
    int m1EAC;
    int m1EB0;
    float m1EB4;
    CharaLightData_unk m1EB8; // inferred: "/data/texture/gm044_000a.texture"
    int m1F08;
    int m1F0C; // inferred: cloud map shown by ActionCloudMap
    std::vector<int> m1F10; // inferred: cleared by fn_80484FBC with the lights
    int m1F1C;
    atn::Bgm m1F20; // inferred: music of the event
    int m1F34;      // inferred: the music is used
    atn::FixedString_unk<0x20> m1F38; // inferred: a name (tested with strlen)
    int m1F58;
    int m1F5C;
    int m1F60; // inferred: debug menu "MAStartOffset"
    int m1F64; // inferred: debug menu "MAStartDelay"
    float m1F68;
    float m1F6C; // inferred: movie fade time
    int m1F70;
    int m1F74;
    int m1F78;
    int m1F7C;
    ManagerEntry_unk m1F80[127];
    int m2378;
    int m237C;
    int m2380;
    int m2384; // inferred: debug menu "SimulateWave"
    int m2388; // inferred: debug menu "FixEventDelay"
    int m238C; // inferred: debug menu "Draw2DMode"; nonzero while messages are blocked
    int m2390; // inferred: nonzero: the dialog texts are flushed at once
    std::vector<Curve_unk> m2394; // inferred: move curves (fn_8048B3B8)
    std::vector<Curve_unk> m23A0; // inferred: time curves (fn_8048B048)
    atn::D2A_Task* m23AC; // inferred: "ev_gradation.d2a" (frozen by fn_8048F184 (0))
    atn::D2A_Task* m23B0; // inferred: "ev_gradation.d2a" (frozen by fn_8048F184 (1))
    int m23B4; // inferred: set when the check of type 1 succeeds
    int m23B8; // inferred: number of queued sequences
    QueuedSequence_unk m23BC[8]; // inferred: ring of sequences to start
    int m23FC; // inferred: first entry of the ring
    NewStaffRoll* m2400; // inferred: staff roll of the ending
    unsigned char mMoviePlaying : 1; // 0x2404
    unsigned char m2404_1 : 1;
    unsigned char m2404_2 : 1;
    unsigned char m2404_3 : 1;
    unsigned char m2404_4 : 1;
    unsigned char m2404_5 : 1;
    unsigned char m2404_6 : 2;
    unsigned char m2405[0x240C - 0x2405];
    int m240C;   // inferred: debug menu "AutoLightFix/Type"
    float m2410; // inferred: debug menu "AutoLightFix/Ratio"
    float m2414; // inferred: debug menu "AutoLightFix/KeyIntensity"
    float m2418; // inferred: debug menu "AutoLightFix/AmbientIntensity"
    atn::D2A_Task* m241C; // inferred: "ui_seek.d2a"
    int m2420;
    atn::Vector2_unk m2424;
    atn::Vector2_unk m242C;
    float m2434;
    atn::D2A_Task* m2438; // inferred: "ui_seek_arow.d2a"
    atn::Vector3 m243C;
    int m2448; // inferred: nonzero to pass the checks without input
    int m244C;
};

} // namespace Event

extern "C" Event::Manager* lbl_8087F540; // inferred: the event manager (.sbss of Event/Manager.cpp)

#endif
